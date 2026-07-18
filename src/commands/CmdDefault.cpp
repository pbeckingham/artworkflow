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

////////////////////////////////////////////////////////////////////////////////
int CmdDefault (CLI& cli, Config& config, Database& database)
{
  const bool verbose = config.getBoolean ("verbose");

  if (config.getBoolean ("temp.shiny"))
    std::cout << '\n'
              << "Welcome to ArtWorkFlow.\n"
              << '\n'
              << "There is built-in help:\n"
              << "    artworkflow help\n"
              << "    artworkflow help <command>\n"
              << "    (and more)\n"
              << '\n'
              << "There is a fully-detailed man page:\n"
              << "    man artworkflow\n"
              << '\n';

  if (verbose)
    std::cout << "Running default command...\n";

  return CmdKanban (cli, config, database);
}

////////////////////////////////////////////////////////////////////////////////
