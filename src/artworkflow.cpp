////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2026, Paul Beckingham
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

#include <iomanip>
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <Config.h>
#include <AtomicFile.h>
#include <Color.h>
#include <Timer.h>
#include <shared.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
int main (int argc, const char** argv)
{
  Timer run_time;

  // Lightweight version checking that doesn't require initialization or I/O.
  int status = 0;
  if (lightweightVersionCheck (argc, argv))
    return status;

  try
  {
    // Create the Lua VM and load its libraries.
    sol::state lua;
    lua.open_libraries (sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);

    // Config provides settings access to all code.
    Config config (lua);

    // Add entities so that command line tokens such as 'help' are recognized as
    // commands.
    CLI cli;
    initializeEntities (cli);

    // Capture the args.
    std::string commandLine;
    for (int i = 0; i < argc; i++)
    {
      cli.add (argv[i]);

      if (i)
        commandLine += " ";

      commandLine += quoteIfNeeded (argv[i]);
    }

    // Scan command line.
    cli.analyze ();

    // Prepare the database, but do not read data.
    Database database;
    initializeDataAndConfig (cli, database, config);
    debug ("initializeDataAndConfig"); // After initializeDataAndConfig because of default values

    // Dispatch to commands.
    debug ("dispatchCommand");
    status = dispatchCommand (cli, database, config);

    // Save any outstanding changes.
    debug ("database.commit");
    database.commit ();

    AtomicFile::finalize_all ();
    debug ("AtomicFile::finalize_all");
  }

  catch (const std::string& error)
  {
    std::cerr << error << '\n';
    status = -1;
  }

  catch (std::bad_alloc& error)
  {
    auto message = std::string ("Memory allocation failed: ") + error.what ();
    std::cerr << "Error: " << message << '\n';
    status = -3;
  }

  catch (...)
  {
    auto message = "Unknown problem, please report.";
    std::cerr << "Error: " << message << '\n';
    status = -2;
  }

  run_time.stop ();
  std::stringstream s;
  s << "Timer artworkflow "
    << std::setprecision (6)
    << std::fixed
    << run_time.total_us () / 1000000.0
    << " sec\n";
  debug (s.str ());

  return status;
}

////////////////////////////////////////////////////////////////////////////////
