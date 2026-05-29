////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2025, Gothenburg Bit Factory.
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

#include <cmake.h>
#include <commands.h>
#include <format.h>
#include <iostream>
#include <paths.h>
#include <unistd.h>
#include <CLI.h>
#include <shared.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
bool lightweightVersionCheck (int argc, const char** argv)
{
  if (argc == 2 && std::string (argv[1]) == "--version")
  {
    std::cout << VERSION << '\n';
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
void initializeEntities (CLI& cli)
{
  // Command entities.
  cli.entity ("command", "config");
  cli.entity ("command", "diagnostics");
  cli.entity ("command", "help");
  cli.entity ("command", "--help");
  cli.entity ("command", "-h");
  cli.entity ("command", "show");
  cli.entity ("command", "version");
  cli.entity ("command", "--version"); // Note: this only appears to overlap with
				       // the lightweight version checking.
  cli.entity ("command", "-v");

  // Hint entities.
  cli.entity ("hint", ":debug");
  cli.entity ("hint", ":quiet");
  cli.entity ("hint", ":color");
  cli.entity ("hint", ":nocolor");
  cli.entity ("hint", ":yes");
}

////////////////////////////////////////////////////////////////////////////////
void initializeDataAndRules (
  const CLI& cli,
  Database& database,
  Rules& rules)
{
  // Rose tint my world, make me safe from my trouble and pain.
  rules.set ("color", isatty (STDOUT_FILENO) ? "on" : "off");

  // Make common hints available via rules:
  //   :debug   --> debug=on
  //   :quiet   --> verbose=off
  //   :color   --> color=on
  //   :nocolor --> color=off
  //   :yes     --> confirmation=off
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("HINT"))
    {
      if (arg.attribute ("canonical") == ":debug")   rules.set ("debug",        "on");
      if (arg.attribute ("canonical") == ":quiet")   rules.set ("verbose",      "off");
      if (arg.attribute ("canonical") == ":color")   rules.set ("color",        "on");
      if (arg.attribute ("canonical") == ":nocolor") rules.set ("color",        "off");
      if (arg.attribute ("canonical") == ":yes")     rules.set ("confirmation", "off");
    }
  }

  enableDebugMode (rules.getBoolean ("debug"));
  paths::initializeDirs (rules);

  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("HINT"))
    {
      if (arg.attribute ("canonical") == ":debug")   rules.set ("debug",        "on");
      if (arg.attribute ("canonical") == ":quiet")   rules.set ("verbose",      "off");
      if (arg.attribute ("canonical") == ":color")   rules.set ("color",        "on");
      if (arg.attribute ("canonical") == ":nocolor") rules.set ("color",        "off");
      if (arg.attribute ("canonical") == ":yes")     rules.set ("confirmation", "off");
    }
  }

  if (rules.has ("debug.indicator"))
    setDebugIndicator (rules.get ("debug.indicator"));

  if (rules.has ("theme.colors.debug"))
    setDebugColor (Color (rules.get ("theme.colors.debug")));

  // Apply command line overrides.
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("CONFIG"))
    {
      rules.set (arg.attribute ("name"), arg.attribute ("value"));
      debug (format ("Configuration override {1} = {2}", arg.attribute ("name"), arg.attribute ("value")));
    }
  }

  std::string dbDataDir = paths::dbDataDir ();
  // Initialize the database (no data read), but files are enumerated.
  database.initialize (dbDataDir);
}

////////////////////////////////////////////////////////////////////////////////
int dispatchCommand (
  CLI& cli,
  Database& database,
  Rules& rules)
{
  int status {0};

  // Debug output.
  if (rules.getBoolean ("debug"))
    std::cout << cli.dump () << '\n';

  // Dispatch to the right command function.
  std::string command = cli.getCommand ();

  if (! command.empty ())
  {
    // These signatures are expected to be all different, therefore no command to fn mapping.
         if (command == "config")      status = CmdConfig        (cli, rules          );
    else if (command == "diagnostics") status = CmdDiagnostics   (     rules, database);
    else if (command == "help"    ||
             command == "--help"  ||
             command == "-h")          status = CmdHelp          (cli                 );
    else if (command == "show")        status = CmdShow          (     rules          );
    else if (command == "version" ||
             command == "-v")          status = CmdVersion       (                    );
//    else                               status = CmdReport        (cli, rules, database);
  }
  else
  {
    auto words = cli.getWords ();

    if (! words.empty ())
    {
      throw format ("'{1}' is not an artworkflow command. See 'artworkflow help'.", words[0]);
    }
    else
    {
      status = CmdDefault (rules);
    }
  }

  return status;
}

////////////////////////////////////////////////////////////////////////////////
