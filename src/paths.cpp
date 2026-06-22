////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2022 - 2023, Thomas Lauf, Paul Beckingham, Federico Hernandez.
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
///
#include <FS.h>
#include <format.h>
#include <paths.h>
#include <shared.h>

namespace paths
{
  static const char *legacy_config_dir = "~/.artworkflow";
  static const bool uses_legacy_config = Directory (legacy_config_dir).exists ();
  const char *artworkflowdb = getenv ("ARTWORKFLOWDB");

  #if defined(__unix__) || defined(__APPLE__) || defined(__linux__) || defined(_SYSTYPE_BSD)
  std::string getPath (const char *xdg_path)
  {
    if (artworkflowdb != nullptr)
    {
      return artworkflowdb;
    }
    else if (uses_legacy_config)
    {
      return legacy_config_dir;
    }
    else
    {
      return std::string (xdg_path) + "/artworkflow";
    }
  }

  const char *getenv_default (const char *env, const char *default_value)
  {
    const char *value = getenv (env);
    if (value == nullptr)
    {
      return default_value;
    }

    return value;
  }

  static std::string conf_dir = getPath (getenv_default ("XDG_CONFIG_HOME", "~/.config"));
  static std::string data_dir = getPath (getenv_default ("XDG_DATA_HOME", "~/.local/share"));
  std::string configDir () { return conf_dir; }
  std::string dbDir () { return data_dir; }

#else
  std::string getPath ()
  {
    if (artworkflowdb != nullptr)
    {
      return artworkflowdb;
    }
    else
    {
      return legacy_config_dir;
    }
  }

  static std::string path = getPath ();

  std::string configDir ()
  {
      return path;
  }

  std::string dbDir ()
  {
      return path;
  }

#endif

  std::string configFile () { return configDir () + "/config.lua"; }
  std::string dbDataDir () { return dbDir () + "/data"; }

  void initializeDirs (Rules& rules)
  {
    Directory configLocation = Directory (configDir ());
    bool configDirExists = configLocation.exists ();

    if (configDirExists &&
        (! configLocation.readable () ||
         ! configLocation.writable () ||
         ! configLocation.executable ()))
    {
      throw format ("Config is not readable at '{1}'", configLocation._data);
    }

    Directory dbLocation = Directory (dbDir ());
    bool dataLocationExists = dbLocation.exists ();
    if (dataLocationExists &&
            (! dbLocation.readable () ||
             ! dbLocation.writable () ||
             ! dbLocation.executable ()))
    {
      throw format ("Database is not readable at '{1}'", dbLocation._data);
    }

    std::string question = "";
    if (! configDirExists)
    {
      question += "Create new config in " + configLocation._data + "?";
    }
    if (! dataLocationExists && configLocation._data != dbLocation._data)
    {
      if (question != "") {
          question += "\n";
      }
      question += "Create new database in " + dbLocation._data + "?";
    }

    if (! configDirExists || ! dataLocationExists)
    {
      if (!rules.getBoolean ("confirmation", true) || confirm (question))
      {
        if (! configDirExists)
        {
          configLocation.create (0700);
        }
        if (! dataLocationExists)
        {
          dbLocation.create (0700);
        }
      }
      else
      {
        throw std::string ("Initial setup aborted by user");
      }
    }

    // Create data subdirectory if necessary.
    Directory dbDataLocation (dbDataDir ());

    if (! dbDataLocation.exists ())
    {
      dbDataLocation.create (0700);
    }

    Path configFileLocation (configFile ());

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

    // Load the configuration data.
    rules.load (configFileLocation);

    // This value is not written out to disk, as there would be no point.
    // Having located the config file, the 'db' location is already known.
    // This is just for subsequent internal use.
    rules.set ("temp.db", dbLocation);
    rules.set ("temp.config", configFileLocation);

    // Perhaps some subsequent code would like to know this is a new db and possibly a first run.
    if (! dataLocationExists)
      rules.set ("temp.shiny", 1);
  }
}
