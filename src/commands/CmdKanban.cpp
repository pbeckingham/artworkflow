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
int CmdKanban (CLI& cli, Config& config, Database& database)
{
  auto verbose = config.getBoolean ("verbose");

  std::vector <Painting> concepts;
  std::vector <Painting> wip;
  std::vector <Painting> drying;
  std::vector <Painting> inventory;
  std::vector <Painting> abandoned;
  std::vector <Painting> destroyed;
  std::vector <Painting> sold;
  std::vector <Painting> gifted;
  for (auto& painting : database.allPaintings ())
  {
    if (filterByCLI (cli, painting, true))
    {
           if (painting.is_concept ())   concepts.push_back (painting);
      else if (painting.is_wip ())       wip.push_back (painting);
      else if (painting.is_drying ())    drying.push_back (painting);
      else if (painting.is_inventory ()) inventory.push_back (painting);
      else if (painting.is_abandoned ()) abandoned.push_back (painting);
      else if (painting.is_destroyed ()) destroyed.push_back (painting);
      else if (painting.is_sold ())      sold.push_back (painting);
      else if (painting.is_gifted ())    gifted.push_back (painting);
    }
  }

  debug (std::format ("{} concepts", concepts.size ()));
  for (auto& p : concepts)
    debug (std::format ("  {} {}", p.id (), p.title ()));

  debug (std::format ("{} wip", wip.size ()));
  for (auto& p : wip)
    debug (std::format ("  {} {}", p.id (), p.title ()));

  debug (std::format ("{} drying", drying.size ()));
  for (auto& p : drying)
    debug (std::format ("  {} {}", p.id (), p.title ()));

  debug (std::format ("{} inventory", inventory.size ()));
//  for (auto& p : inventory)
//    debug (std::format ("  {} {}", p.id (), p.title ()));

  debug (std::format ("{} abandoned", abandoned.size ()));
//  for (auto& p : abandoned)
//    debug (std::format ("  {} {}", p.id (), p.title ()));

  debug (std::format ("{} destroyed", destroyed.size ()));
//  for (auto& p : destroyed)
//    debug (std::format ("  {} {}", p.id (), p.title ()));

  debug (std::format ("{} sold", sold.size ()));
//  for (auto& p : sold)
//    debug (std::format ("  {} {}", p.id (), p.title ()));

  debug (std::format ("{} gifted", gifted.size ()));
//  for (auto& p : gifted)
//    debug (std::format ("  {} {}", p.id (), p.title ()));

  // TODO: load configuration that determines Kanban columns.
  // reportKanban = {
  //   ["columns"] = {"Concept", "WIP", "Drying"}
  // }

  // There may be settings overrides.
  std::optional <sol::table> reportKanban = (*config.lua ())["reportKanban"];
  if (reportKanban)
  {
    debug ("CmdKanban found Lua/reportKanban settings");
    for (const auto& entry : reportKanban.value ())
    {
      sol::object key = entry.first;
      sol::object value = entry.second;
      debug (key.as<std::string> ());
    }
  }

  debug (std::format ("Terminal is {}x{}", get_terminal_width (), get_terminal_height ()));

  Color frame ("0x808080 on 0x200030");
  int left_column_width = 14;
  std::vector <std::string> left_column;

  Composite left;
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("Concepts   {}", concepts.size ()), 1, frame);
  left_column.push_back (left.str ());

  left.clear ();
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("WIP        {}", wip.size ()), 1, frame);
  left_column.push_back (left.str ());

  left.clear ();
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("Drying     {}", drying.size ()), 1, frame);
  left_column.push_back (left.str ());

  left.clear ();
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("Abandoned  {}", abandoned.size ()), 1, frame);
  left_column.push_back (left.str ());

  left.clear ();
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("Destroyed  {}", destroyed.size ()), 1, frame);
  left_column.push_back (left.str ());

  left.clear ();
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("Inventory  {}", inventory.size ()), 1, frame);
  left_column.push_back (left.str ());

  left.clear ();
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("Gifted     {}", gifted.size ()), 1, frame);
  left_column.push_back (left.str ());

  left.clear ();
  left.add (std::string (left_column_width, ' '), 0, frame);
  left.add (std::format ("Sold       {}", sold.size ()), 1, frame);
  left_column.push_back (left.str ());

  // TODO: Compose output line by line for each of the columns.
  for (auto& line : left_column)
    std::cout << line << '\n';

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
