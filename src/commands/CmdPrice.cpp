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
#include <artworkflow.h>

static std::map <std::string, std::string> series_map
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
// Returns 0 if tracking is active, 1 if not.
int CmdPrice (CLI& cli, Config& config, Database& database)
{
  debug ("CmdPrice start");
  debug ("CmdPrice end");
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
