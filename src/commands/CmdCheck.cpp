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
#include <iomanip>
#include <algorithm>
#include <format>
#include <Composite.h>
#include <util.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
int CmdCheck (CLI& cli, Config& config, Database& database)
{
  auto verbose = config.getBoolean ("verbose");
  std::size_t error_count = 0;
  for (auto& painting : database.allPaintings ())
  {
    if (filterByCLI (cli, painting, true))
    {
      std::vector <std::string> errors, warnings;
      painting.validate (errors, warnings);
      if (errors.size () || warnings.size ())
      {
        Color default_color;
        Color error_color ("0xFF6F6F");
        Color warning_color ("0xF8C82D");
        if (verbose)
        {
          Composite line;
          line.add (std::string (24, ' '), 0, default_color);
          line.add (std::format ("#{} {}", painting.id (), painting.title ()), 0, default_color);
          std::cout << line.str ().substr (0, 24) << "  ";
          bool first = true;
          for (auto& error : errors)
          {
            if (! first)
              std::cout << std::string (26, ' ');
            std::cout << Color::colorize (error, "0xFF6F6F") << '\n';
            first = false;
          }
          if (verbose)
          {
            for (auto& warning : warnings)
            {
              if (! first)
                std::cout << std::string (25, ' ');
              std::cout << Color::colorize (warning, "0xF8C82D") << '\n';
              first = false;
            }
          }
        }

        error_count += errors.size ();
      }
    }
  }

  return error_count ? 1 : 0;
}

////////////////////////////////////////////////////////////////////////////////
