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
#include <map>
#include <ranges>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <format>
#include <util.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
static const std::string bar (int value, Color color)
{
  std::string segment {""};
  if (value)
    segment += Color::colorize (std::string (value, ' '), color);

  return segment;
}

////////////////////////////////////////////////////////////////////////////////
int CmdDisposition (CLI& cli, Config& config, Database& database)
{
  auto verbose = config.getBoolean ("verbose");

  std::map <std::string, int> concepts;
  std::map <std::string, int> wip;
  std::map <std::string, int> drying;
  std::map <std::string, int> abandoned;
  std::map <std::string, int> destroyed;
  std::map <std::string, int> nfs;
  std::map <std::string, int> inventory;
  std::map <std::string, int> gifted;
  std::map <std::string, int> sold;

  for (auto& painting : database.allPaintings ())
  {
    if (filterByCLI (cli, painting, true))
    {
      auto year = std::format ("{}", Datetime ().year ());

      auto start = painting.start ();
      if (start != "")
        year = painting.start ().substr (1, 4);

      if (concepts.find (year) == concepts.end ())
      {
        concepts[year] = 0;
        wip[year] = 0;
        drying[year] = 0;
        abandoned[year] = 0;
        destroyed[year] = 0;
        nfs[year] = 0;
        inventory[year] = 0;
        gifted[year] = 0;
        sold[year] = 0;
      }

      if (painting.is_concept ())   ++concepts[year];
      if (painting.is_wip ())       ++wip[year];
      if (painting.is_drying ())    ++drying[year];
      if (painting.is_abandoned ()) ++abandoned[year];
      if (painting.is_destroyed ()) ++destroyed[year];
      if (painting.is_nfs ())       ++nfs[year];
      if (painting.is_inventory ()) ++inventory[year];
      if (painting.is_gifted ())    ++gifted[year];
      if (painting.is_sold ())      ++sold[year];
    }
  }

  std::cout << "Year Disposition\n";
  std::cout << "---- ----------------------------------------------------------------------------------------------------\n";

  Color color_concept   ("0xa0a0a0 on 0x401050");
  Color color_wip       ("0xa0a0a0 on 0x202080");
  Color color_drying    ("0xa0a0a0 on 0x2020c0");
  Color color_abandoned ("0xa0a0a0 on 0x000030");
  Color color_destroyed ("0xa0a0a0 on 0xa00000");
  Color color_nfs       ("0x000000 on 0x80a080");
  Color color_inventory ("0x000000 on 0xa0d0a0");
  Color color_gifted    ("0x000000 on 0x009000");
  Color color_sold      ("0x000000 on 0x00b000");

  for (auto& key : std::views::keys (concepts))
  {
    auto line = bar (concepts[key], color_concept);
    line += bar (wip[key], color_wip);
    line += bar (drying[key], color_drying);
    line += bar (abandoned[key], color_abandoned);
    line += bar (destroyed[key], color_destroyed);
    line += bar (nfs[key], color_nfs);
    line += bar (inventory[key], color_inventory);
    line += bar (gifted[key], color_gifted);
    line += bar (sold[key], color_sold);

    std::cout << key << ' ' << line << '\n';
  }

  std::cout << '\n'
            << ' ' << color_concept.colorize (" Concept ")
            << ' ' << color_wip.colorize (" WIP ")
            << ' ' << color_drying.colorize (" Drying ")
            << ' ' << color_abandoned.colorize (" Abandoned ")
            << ' ' << color_destroyed.colorize (" Destroyed ")
            << ' ' << color_nfs.colorize (" NFS ")
            << ' ' << color_inventory.colorize (" Inventory ")
            << ' ' << color_gifted.colorize (" Gifted ")
            << ' ' << color_sold.colorize (" Sold ")
            << '\n';
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
