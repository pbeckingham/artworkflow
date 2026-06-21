////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2018, 2020 - 2025, Gothenburg Bit Factory.
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
#include <format.h>
#include <iostream>
#include <shared.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
// Identify DOM references in CLI, provide space-separated results.
int CmdInfo (
  CLI& cli,
  Rules& rules,
  Database& database)
{
  for (auto& id : cli.getIds ())
    for (auto& painting : database.allPaintings ())
      if (painting.matches (id))
      {
        std::cout << painting.id () << " " << painting.title () << '\n';
        if (painting.start () != "")
          std::cout << "  Started     " << painting.start ().substr (1, std::string::npos) << '\n';
        if (painting.end () != "")
          std::cout << "  Ended       " << painting.end ().substr (1, std::string::npos) << '\n';
        if (painting.varnish () != "")
          std::cout << "  Varnished   " << painting.varnish ().substr (1, std::string::npos) << '\n';
        if (painting.series () != "")
          std::cout << "  Series      " << painting.series () << '\n';
        if (painting.substrate () != "")
          std::cout << "  Substrate   " << painting.substrate () << '\n';
      }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
