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
#include <Composite.h>
#include <format>
#include <util.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
int CmdAll (CLI& cli, Config& config, Database& database)
{
  auto verbose = config.getBoolean ("verbose");

  std::vector <Painting> all;
  for (auto& painting : database.allPaintings ())
    if (filterByCLI (cli, painting, true))
      all.push_back (painting);

  int terminal_width = get_terminal_width ();
  int terminal_height = get_terminal_height ();
  debug (std::format ("Terminal is {}x{}", terminal_width, terminal_height));

  // How many mini_cards can be displayed?
  int column_width = 20;
  if (verbose)
    column_width = 24;

  int columns = terminal_width / (column_width + 1);
  debug (std::format ("Can display {} columns", columns));

  std::vector <std::vector <std::string>> grid;
  for (int i = 0; i < columns; i++)
    grid.push_back (std::vector <std::string> {});

  for (unsigned int i = 0; i < all.size (); ++i)
  {
    auto column = i % columns;
    auto row = i / columns;

    if (verbose)
      for (auto& line : all[i].card (column_width))
        grid[column].push_back (line);
    else
      for (auto& line : all[i].mini_card (column_width))
        grid[column].push_back (line);

    grid[column].push_back (std::string (column_width, ' '));
  }

  for (int row = 0; row < grid[0].size (); ++row)
  {
    for (int col = 0; col < columns; ++col)
    {
      if (col)
        std::cout << ' ';

      if (grid[col].size ())
        std::cout << grid[col][row];
    }

    std::cout << '\n';
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
