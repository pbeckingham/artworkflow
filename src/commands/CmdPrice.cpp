////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2026, Paul Beckingham.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// https://opensource.org/license/mit
//
////////////////////////////////////////////////////////////////////////////////

#include <commands.h>
#include <iostream>
#include <inttypes.h>
#include <format.h>
#include <artworkflow.h>

static std::map <std::string, std::string> series_names
{
  {"STL", "Still Life"},
  {"LAN", "Landscape"},
  {"BOA", "Boat"},
  {"POR", "Portrait"},
  {"DFL", "Dead Flower"},
  {"FLO", "Floral"},
  {"MCP", "Master Copy"},
  {"ABS", "Abstract"},
  {"WRK", "Workshop"},
  {"WIL", "Wildlife"},
};

static std::map <std::string, std::string> varnish_names
{
  {"D",     "Dammar"},
  {"G",     "Gamvar"},
  {"C",     "Conservar"},
  {"W",     "Winsor & Newton"},
  {"Other", "Other"},
};

static std::map <std::string, std::string> substrate_names
{
  {"ACM", "Aluminum Composite Material"},
  {"CB",  "Canvas Board"},
  {"WP",  "Wood Panel"},
  {"C",   "Canvas"},
};

////////////////////////////////////////////////////////////////////////////////
// Piedmont Plastics 8 sheets 3mm AluPoly = $480.25
// 8 sheets (4'x8'): 8 * 32 sq ft = 256 sqft
// $480.25 / 256 = $1.88 per sq ft
// $480.25 / (256 * 144) = $0.013 per sq in
// Assume 10% wasted by cuts
//
// Gessobord: 9"x12", pack of 4 = $66.99 on Amazon
// $66.99 / (4 * 9 * 12) = $0.155 per square inch
//
// Baltic Birch: 1@"x12", pack of 3 = $17.99 on Amazon
// $17.99 / (3 * 12 * 12) = $0.042 per square inch
//
// Canvas Board: 8"x10", pack of 12 = $12.99 on Amazon
// $12.99 / (12 * 8 * 10) = $0.014 per square inch
//
// Centurion Stretched Canvas: 16"x20", pack of 5 = $155.09
// $155.09 / (6 * 16 * 20) = $0.05 per square inch
//
// 2025-02-13
double substrate_cost (
  const Config& config,
  const std::string& substrate,
  const int height,
  const int width)
{
  // Assume 10% waste.
  if (substrate == "ACM")
    return config.getReal ("cost_estimate_substrate_acm", 0.013) * height * width * 1.1;

  if (substrate == "WP")
    return config.getReal ("cost_estimate_substrate_wp", 0.155) * height * width;

  if (substrate == "CB")
    return config.getReal ("cost_estimate_substrate_cb", 0.014) * height * width;

  if (substrate == "C")
    return config.getReal ("cost_estimate_substrate_c", 0.05) * height * width;

  return config.getReal ("cost_estimate_substrate_default", 50.0);
}

////////////////////////////////////////////////////////////////////////////////
// 1 can, 32oz Gamblin White Oil Ground = $48.00 on Amazon
// 1 can lasts approximately 18 months
// $48.00 * 12 / 18 = $32.00 per year
// 35 paintings per year average
// $32.00 / 35 = $0.92 per painting
//
// 2026-07-17
double ground_cost (const Config& config, const int height, const int width)
{
  return config.getReal ("cost_estimate_ground", 0.92);
}

////////////////////////////////////////////////////////////////////////////////
// 35 paintings per year average
// Estimated 5 brushes consumed per year
// Average brush price: $12.00
//
// Total Brushes = 5 x $12.00 = $60.00
// Therefore $60.00 / 35 = $1.72 per painting.
//
// 2026-07-17
double brush_cost (const Config& config)
{
  return config.getReal ("cost_estimate_brush", 1.72);
}

////////////////////////////////////////////////////////////////////////////////
// Using Geneva paint as the example, with 8 years of data: = $0.32/d
// Assume 350 days painting a year: $0.32/d * 350d = $112/yr
// Average 33 paintings completed per year: $112 / 33 = $3.39
//
// 2026-07-17
double paint_cost (const Config& config, const int height, const int width)
{
  return config.getReal ("cost_estimate_paint", 3.39);
}


////////////////////////////////////////////////////////////////////////////////
// Natural Pigments Conservar 118ml bottle is $12.10
// One bottle lasts 6 months
// $12.10 * 2 = $24.20 per year
// Average 33 paintings completed per year: $24.20 / 33 = $0.73
//
// Estimate 2026-07-17
double varnish_cost (const Config& config, const std::string& varnish)
{
  // TODO: Vary by area and type of varnish.
  return config.getReal ("cost_estimate_varnish", 0.73);
}

////////////////////////////////////////////////////////////////////////////////
// FrankenFrame ECO profile:
//   8"x8"   = 32" linear = $21.65 + $5 shipping = $26.65 / 32" = $0.83 per linear inch
//   12"x12" = 48" linear = $33.60 + $5 shipping = $38.75 / 48" = $0.81 per linear inch
//   12"x24" = 72" linear = $50.54 + $5 shipping = $55.54 / 72" = $0.77 per linear inch
//   24"x24' = 96" linear = $75.00 + $5 shipping = $80.00 / 96" = $0.83 per linear inch
//
// AmericanFrame
//   12"x12" = 48" linear =  $60 + $15 shipping =  $75 / 48" = $1.56 per linear inch
//   12"x24" = 72" linear =  $96 + $15 shipping = $111 / 72" = $1.54 per linear inch
//   18"x18" = 72" linear = $112 + $15 shipping = $127 / 72" = $1.76 per linear inch
//   24"x24" = 96" linear = $140 + $15 shipping = $155 / 96" = $1.62 per linear inch
//
// Assume FrankenFrame, highest cost.
//
// 2026-07-17
double frame_cost (const Config& config, const int height, const int width)
{
  return config.getReal ("cost_estimate_frame", 0.83) * (height + height + width + width);
}

////////////////////////////////////////////////////////////////////////////////
// Given the 2024 12"x12" price of $1100, and the 24"x48" price of $5000,
// assume a straight line proportinal to painting area.
//
// Linear price = mx + c
double area_price (
  const int height,
  const int width)
{
  auto slope = (5000 - 1250) / (1152 - 144);
  auto area = height * width;

  return ((area - 144) * slope) + 1250;
}

////////////////////////////////////////////////////////////////////////////////
double ani_price (
  const int height,
  const int width,
  const double cost,
  const int complexity)
{
  return ((height * width * 2) + cost) * complexity;
}

////////////////////////////////////////////////////////////////////////////////
// #379 'Central Line Surfer'
//   Still Life Series
//   12"x18", Aluminum Composite Material
//   Created 2026-02-23 - 2026-04-06 (42d)
//   Varnished 2026-04-17, dried for 11d, Conservar
//   Complexity 2
//   Materials cost $   59.65
//   Area Price     $ 1400.00
//   Ani Price      $ 1000.00
//
int CmdPrice (CLI& cli, Config& config, Database& database)
{
  int count = 0;

  for (auto& painting : database.allPaintings ())
  {
    if (filterByCLI (cli, painting))
    {
      ++count;

      int height = painting.height ();
      int width = painting.width ();

      std::cout << '#' << painting.id () << ' ' << painting.title () << '\n'
                << "  " << series_names [painting.series ()] << '\n'
                << "  " << height << "\"x" << width << "\", " << substrate_names [painting.substrate ()] << '\n';

      auto s = substrate_cost (config, painting.substrate (), height, width);
      auto g = ground_cost (config, height, width);
      auto v = varnish_cost (config, painting.varnished ());
      auto b = brush_cost (config);
      auto p = paint_cost (config, height, width);
      auto f = frame_cost (config, height, width);
      auto total_cost = s + g + v + b + p + f;
      std::cout << "  Materials cost $" << format (total_cost, 8, 4) << '\n';

      std::cout << "  Area price     $" << format (area_price (height, width), 5, 4) << '\n';

      auto complexity = strtoimax (painting.complexity ().substr (1).c_str (), nullptr, 10);
      std::cout << "  Ani price      $" << format (ani_price (height, width, total_cost, complexity), 5, 4) << '\n';
      std::cout << '\n';
    }
  }

  return count > 0 ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
