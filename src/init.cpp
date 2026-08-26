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
#include <format>
#include <iostream>
#include <paths.h>
#include <unistd.h>
#include <CLI.h>
#include <shared.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
void initializeEntities (CLI& cli)
{
  // Command entities.
  cli.entity ("command", "all");
  cli.entity ("command", "check");
  cli.entity ("command", "diagnostics");
  cli.entity ("command", "disposition");
  cli.entity ("command", "get");
  cli.entity ("command", "help");
  cli.entity ("command", "--help");
  cli.entity ("command", "-h");
  cli.entity ("command", "information");
  cli.entity ("command", "kanban");
  cli.entity ("command", "new");
  cli.entity ("command", "price");
  cli.entity ("command", "sales");
  cli.entity ("command", "series");
  cli.entity ("command", "version");
  cli.entity ("command", "--version"); // Note: this only appears to overlap with
                                       // the lightweight version checking.
  cli.entity ("command", "-v");

  // Hint entities.
  cli.entity ("hint", ":debug");
  cli.entity ("hint", ":quiet");
  cli.entity ("hint", ":verbose");
  cli.entity ("hint", ":color");
  cli.entity ("hint", ":nocolor");
  cli.entity ("hint", ":yes");

  // Filtering hints.
  cli.entity ("hint", ":concept");
  cli.entity ("hint", ":wip");
  cli.entity ("hint", ":drying");
  cli.entity ("hint", ":varnished");
  cli.entity ("hint", ":inventory");
  cli.entity ("hint", ":gifted");
  cli.entity ("hint", ":sold");
  cli.entity ("hint", ":abandoned");
  cli.entity ("hint", ":destroyed");
  cli.entity ("hint", ":nfs");
}

////////////////////////////////////////////////////////////////////////////////
void initializeDataAndConfig (
  const CLI& cli,
  Database& database,
  Config& config)
{
  // Rose tint my world, keep me safe from my trouble and pain.
  config.set ("color", isatty (STDOUT_FILENO) ? "on" : "off");

  // Make common hints available via config:
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("HINT"))
    {
      if (arg.attribute ("canonical") == ":debug")   config.set ("debug",        true);
      if (arg.attribute ("canonical") == ":quiet")   config.set ("verbose",      false);
      if (arg.attribute ("canonical") == ":verbose") config.set ("verbose",      true);
      if (arg.attribute ("canonical") == ":color")   config.set ("color",        true);
      if (arg.attribute ("canonical") == ":nocolor") config.set ("color",        false);
      if (arg.attribute ("canonical") == ":yes")     config.set ("confirmation", false);
    }
  }

  enableDebugMode (config.getBoolean ("debug"));
//  paths::initializeDirs (config);
// WARN: Kept only because it provides one solution to config/data initial condition.
/*
void initializeConfigAndDatabase (Config& config)
{
  Directory configLocation = Directory (conf_dir);
  bool configDirExists = configLocation.exists ();

  if (configDirExists &&
      (! configLocation.readable () ||
       ! configLocation.writable () ||
       ! configLocation.executable ()))
    throw std::format ("Config is not readable at '{}'", configLocation._data);

  Directory dbLocation = Directory (getDataLocation ());
  bool dataLocationExists = dbLocation.exists ();
  if (dataLocationExists &&
          (! dbLocation.readable () ||
           ! dbLocation.writable () ||
           ! dbLocation.executable ()))
    throw std::format ("Database is not readable at '{}'", dbLocation._data);

  std::string question = "";
  if (! configDirExists)
    question += "Create new config in " + configLocation._data + "?";

  if (! dataLocationExists && configLocation._data != dbLocation._data)
  {
    if (question != "")
        question += "\n";
    question += "Create new database in " + dbLocation._data + "?";
  }

  if (! configDirExists || ! dataLocationExists)
  {
    if (!config.getBoolean ("confirmation", true) || confirm (question))
    {
      if (! configDirExists)
        configLocation.create (0700);
      if (! dataLocationExists)
        dbLocation.create (0700);
    }
    else
      throw std::string ("Initial setup aborted by user");
  }

  // Create data subdirectory if necessary.
  Directory dbDataLocation (getDataLocation ());

  if (! dbDataLocation.exists ())
    dbDataLocation.create (0700);

  Path configFileLocation (getConfigLocation ());

  if (! configFileLocation.exists ())
  {
    File configFile (configFileLocation);
    configFile.create (0600);
    std::vector<std::string> defaultConfig = {
      "confirmation = \"on\"\n",
      "debug = \"off\"\n",
      "verbose = \"on\"\n",
    };
    configFile.append(defaultConfig);
  }

*/

  // Load the configuration data.
  File configFile (getConfigFile ());
  config.load (configFile.realpath ());

/*
  // This value is not written out to disk, as there would be no point.
  // Having located the config file, the 'db' location is already known.
  // This is just for subsequent internal use.
  config.set ("temp.db", dbLocation);
  config.set ("temp.config", configFileLocation);

  // Perhaps some subsequent code would like to know this is a new db and possibly a first run.
  if (! dataLocationExists)
    config.set ("temp.shiny", 1);
}
*/

  // TODO: Determine why the following is needed for debug output to appear.
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("HINT"))
    {
      if (arg.attribute ("canonical") == ":debug")   config.set ("debug",        true);
      if (arg.attribute ("canonical") == ":quiet")   config.set ("verbose",      false);
      if (arg.attribute ("canonical") == ":verbose") config.set ("verbose",      true);
      if (arg.attribute ("canonical") == ":color")   config.set ("color",        true);
      if (arg.attribute ("canonical") == ":nocolor") config.set ("color",        false);
      if (arg.attribute ("canonical") == ":yes")     config.set ("confirmation", false);
    }
  }

  // TODO: Is this working?
  if (config.has ("debug_indicator"))
    setDebugIndicator (config.get ("debug.indicator"));

  // TODO: This is not working.
  if (config.has ("theme.colors.debug"))
    setDebugColor (Color (config.get ("theme.colors.debug")));

  // Apply command line overrides.
  // TODO: This is not working.
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("CONFIG"))
    {
      config.set (arg.attribute ("name"), arg.attribute ("value"));
      debug (std::format ("Configuration override {} = {}", arg.attribute ("name"), arg.attribute ("value")));
    }
  }

  // Initialize the database (no data read), but files are enumerated.
  database.initialize (getDataDirectory ());
}

////////////////////////////////////////////////////////////////////////////////
int dispatchCommand (
  CLI& cli,
  Database& database,
  Config& config)
{
  int status {0};

  // Debug output.
  if (config.getBoolean ("debug"))
    std::cout << cli.dump () << '\n';

  // Dispatch to the right command function.
  std::string command = cli.getCommand ();
  if (! command.empty ())
  {
    // These signatures are expected to be all different, therefore no command to fn mapping.
         if (command == "all")         status = CmdAll           (cli, config, database);
    else if (command == "check")       status = CmdCheck         (cli, config, database);
    else if (command == "diagnostics") status = CmdDiagnostics   (     config          );
    else if (command == "disposition") status = CmdDisposition   (cli, config, database);
    else if (command == "get")         status = CmdGet           (cli, config, database);
    else if (command == "help"    ||
             command == "--help"  ||
             command == "-h")          status = CmdHelp          (cli, config          );
    else if (command == "information") status = CmdInfo          (cli,         database);
    else if (command == "kanban")      status = CmdKanban        (cli, config, database);
    else if (command == "new")         status = CmdNew           (cli, config, database);
    else if (command == "price")       status = CmdPrice         (cli, config, database);
    else if (command == "sales")       status = CmdSales         (cli, config, database);
    else if (command == "series")      status = CmdSeries        (cli, config, database);
    else if (command == "version" ||
             command == "-v")          status = CmdVersion       (     config          );
  }
  else
  {
    auto words = cli.getWords ();

    if (! words.empty ())
    {
      throw std::format ("'{}' is not an artworkflow command. See 'artworkflow help'.", words[0]);
    }
    else
    {
      status = CmdDefault (cli, config, database);
    }
  }

  return status;
}

////////////////////////////////////////////////////////////////////////////////
std::string getConfigLocation ()
{
  auto p = getenv ("XDG_CONFIG_HOME");
  if (p)
    return std::string (p);

  return "~/.config";
}

////////////////////////////////////////////////////////////////////////////////
std::string getDataLocation ()
{
  auto p = getenv ("XDG_DATA_HOME");
  if (p)
    return std::string (p);

  return "~/.local/share";
}

////////////////////////////////////////////////////////////////////////////////
std::string getConfigFile ()
{
  return getConfigLocation () + "/artworkflow/config.lua";
}

////////////////////////////////////////////////////////////////////////////////
std::string getDataDirectory ()
{
  return getDataLocation () + "/artworkflow/data";
}

////////////////////////////////////////////////////////////////////////////////
