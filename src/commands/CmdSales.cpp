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
#include <Table.h>
#include <Composite.h>
#include <util.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
std::string round_percentage (int numerator, int denominator)
{
  if (denominator)
    return std::format ("{: >3.1f}%", 100.0 * numerator / denominator);

  return "";
}

////////////////////////////////////////////////////////////////////////////////
int CmdSales (CLI& cli, Config& config, Database& database)
{
  auto verbose = config.getBoolean ("verbose");

  std::map <std::string, int> started;
  std::map <std::string, int> finished;
  std::map <std::string, int> abandoned;
  std::map <std::string, int> varnished;
  std::map <std::string, int> destroyed;
  std::map <std::string, int> nfs;
  std::map <std::string, int> inventory;
  std::map <std::string, int> gifted;
  std::map <std::string, int> sold;
  for (auto& painting : database.allPaintings ())
  {
    if (filterByCLI (cli, painting, true))
    {
      auto start = painting.start ();
      if (start != "")
      {
        auto year = painting.start ().substr (1, 4);
        if (started.find (year) == started.end ())
        {
          started[year] = 0;
          finished[year] = 0;
          abandoned[year] = 0;
          varnished[year] = 0;
          destroyed[year] = 0;
          nfs[year] = 0;
          inventory[year] = 0;
          gifted[year] = 0;
          sold[year] = 0;
        }

        ++started[year];
        if (painting.end () != "")    ++finished[year];
        if (painting.is_varnished ()) ++varnished[year];
        if (painting.is_abandoned ()) ++abandoned[year];
        if (painting.is_destroyed ()) ++destroyed[year];
        if (painting.is_nfs ())       ++nfs[year];
        if (painting.is_inventory ()) ++inventory[year];
        if (painting.is_gifted ())    ++gifted[year];
        if (painting.is_sold ())      ++sold[year];
      }
    }
  }

  Table table;
  table.underlineHeaders ();
  table.add ("Start Year");
  table.add ("Started", false);
  table.add ("Finished", false);
  table.add ("Comp Rate", false);
  table.add ("Varnish", false);
  table.add ("Abandoned", false);
  table.add ("Destroyed", false);
  table.add ("NFS", false);
  table.add ("Inventory", false);
  table.add ("Gifted", false);
  table.add ("Sold", false);
  table.add ("Sell Rate", false);

  Color color_abandoned ("0x0060a0");
  Color color_destroyed ("0xff0000");
  Color color_nfs ("0x80a080");
  Color color_inventory ("0xf0f0f0");
  Color color_gifted ("0x00b000");
  Color color_sold ("0x00ff00");

  for (auto& key : std::views::keys (started))
  {
    auto row = table.addRow ();
    table.set (row, 0, key);
    table.set (row, 1, started[key]);
    table.set (row, 2, finished[key]);
    table.set (row, 3, round_percentage (finished[key], started[key]));
    table.set (row, 4, varnished[key]);
    table.set (row, 5, abandoned[key], color_abandoned);
    table.set (row, 6, destroyed[key], color_destroyed);
    table.set (row, 7, nfs[key], color_nfs);
    table.set (row, 8, inventory[key], color_inventory);
    table.set (row, 9, gifted[key], color_gifted);
    table.set (row, 10, sold[key], color_sold);
    table.set (row, 11, round_percentage (sold[key], finished[key]));
  }

  auto row = table.addRow ();
  table.set (row, 0, " ");

  row = table.addRow ();
  table.set (row, 0, "Total");
  auto total_started = std::accumulate (started.begin (),
                                        started.end (),
                                        0,
                                        [](int current_total, const auto& pair) {return current_total + pair.second;});
  table.set (row, 1, total_started);

  auto total_finished = std::accumulate (finished.begin (),
                                         finished.end (),
                                         0,
                                         [](int current_total, const auto& pair) {return current_total + pair.second;});
  table.set (row, 2, total_finished);

  if (total_started)
    table.set (row, 3, round_percentage (total_finished, total_started));

  table.set (row, 4, std::accumulate (varnished.begin (), varnished.end (), 0,
             [](int current_total, const auto& pair) {return current_total + pair.second;}));
  table.set (row, 5, std::accumulate (abandoned.begin (), abandoned.end (), 0,
             [](int current_total, const auto& pair) {return current_total + pair.second;}));
  table.set (row, 6, std::accumulate (destroyed.begin (), destroyed.end (), 0,
             [](int current_total, const auto& pair) {return current_total + pair.second;}));
  table.set (row, 7, std::accumulate (nfs.begin (), nfs.end (), 0,
             [](int current_total, const auto& pair) {return current_total + pair.second;}));
  table.set (row, 8, std::accumulate (inventory.begin (), inventory.end (), 0,
             [](int current_total, const auto& pair) {return current_total + pair.second;}));
  table.set (row, 9, std::accumulate (gifted.begin (), gifted.end (), 0,
             [](int current_total, const auto& pair) {return current_total + pair.second;}));
  auto total_sold = std::accumulate (sold.begin (),
                                     sold.end (),
                                     0,
                                     [](int current_total, const auto& pair) {return current_total + pair.second;});
  table.set (row, 10, total_sold);
  table.set (row, 11, round_percentage (total_sold, total_finished));

  std::cout << table.render ();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
