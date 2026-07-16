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
#include <format.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
// Returns 0 if tracking is active, 1 if not.
int CmdKanban (CLI& cli, Config& config, Database& database)
{
  const bool verbose = config.getBoolean ("verbose");

  std::vector <Painting> concepts;
  std::vector <Painting> wip;
  std::vector <Painting> drying;
  for (auto& painting : database.allPaintings ())
  {
    if (painting.is_concept ())
      concepts.push_back (painting);

    else if (painting.is_wip ())
      wip.push_back (painting);

    else if (painting.is_drying ())
      drying.push_back (painting);
  }

  debug (format ("{1} concepts", concepts.size ()));
  for (auto& c : concepts)
    debug (format ("  {1} {2}", c.id (), c.title ()));
  debug (format ("{1} wip", wip.size ()));
  for (auto& w : wip)
    debug (format ("  {1} {2}", w.id (), w.title ()));
  debug (format ("{1} drying", drying.size ()));
  for (auto& d : drying)
    debug (format ("  {1} {2}", d.id (), d.title ()));

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
