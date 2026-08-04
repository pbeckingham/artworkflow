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

  std::size_t columns = terminal_width / (column_width + 1);
  debug (std::format ("Can display {} columns", columns));

  std::vector <std::vector <std::string>> grid;
  for (std::size_t i = 0; i < columns; i++)
    grid.push_back (std::vector <std::string> {});

  for (unsigned int i = 0; i < all.size (); ++i)
  {
    // Find shortest column.
    std::size_t shortest_col = 0;
    std::size_t shortest_col_length = 1000; // Approximately infinity.
    for (std::size_t col = 0; col < grid.size (); ++col)
      if (grid[col].size () < shortest_col_length)
      {
        shortest_col_length = grid[col].size ();
        shortest_col = col;
      }

    if (verbose)
      for (auto& line : all[i].card (column_width))
        grid[shortest_col].push_back (line);
    else
      for (auto& line : all[i].mini_card (column_width))
        grid[shortest_col].push_back (line);

    grid[shortest_col].push_back (std::string (column_width, ' '));
  }

  std::size_t max_rows = 0;
  for (std::size_t col = 0; col < grid.size (); ++col)
    max_rows = std::max (max_rows, grid[col].size ());

  for (std::size_t row = 0; row < max_rows; ++row)
  {
    for (std::size_t col = 0; col < columns; ++col)
    {
      if (col)
        std::cout << ' ';

      if (row < grid[col].size ())
        std::cout << grid[col][row];
      else
        std::cout << std::string (column_width, ' ');
    }

    std::cout << '\n';
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
