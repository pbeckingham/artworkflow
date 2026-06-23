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

#include <cinttypes>

#include <Config.h>
#include <format.h>
#include <shared.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
Config::Config ()
{
  // Load the default values.
  _settings =
  {
    {"confirmation",             "on"},
    {"debug",                    "off"},
    {"verbose",                  "on"},
  };
}

////////////////////////////////////////////////////////////////////////////////
void Config::initialize (sol::state& lua)
{
  // TODO Retain the lua VM for settings queries.
}

////////////////////////////////////////////////////////////////////////////////
bool Config::has (const std::string& key) const
{
  return _settings.find (key) != _settings.end ();
}

////////////////////////////////////////////////////////////////////////////////
// Return the configuration value given the specified key.
std::string Config::get (const std::string& key, const std::string& defaultValue) const
{
  auto found = _settings.find (key);

  if (found != _settings.end ())
    return found->second;

  return defaultValue;
}

////////////////////////////////////////////////////////////////////////////////
int Config::getInteger (const std::string& key, int defaultValue) const
{
  auto found = _settings.find (key);

  if (found != _settings.end ())
  {
    int value = strtoimax (found->second.c_str (), nullptr, 10);

    // Invalid values are handled.
    // ERANGE errors are simply capped by strtoimax, which is desired behavior.
    // Note that not all platforms behave alike, and the EINVAL is not necessarily returned.
    if (value == 0 && (errno == EINVAL || found->second != "0"))
      throw format ("Invalid integer value for '{1}': '{2}'", key, found->second);

    return value;
  }

  return defaultValue;
}

////////////////////////////////////////////////////////////////////////////////
double Config::getReal (const std::string& key) const
{
  auto found = _settings.find (key);

  if (found != _settings.end ())
    return strtod (found->second.c_str (), nullptr);

  return 0.0;
}

////////////////////////////////////////////////////////////////////////////////
bool Config::getBoolean (const std::string& key, bool defaultValue) const
{
  auto found = _settings.find (key);

  if (found != _settings.end ())
  {
    auto value = lowerCase (found->second);

    // TODO Use only idiomatic Lua boolean values.
    if (value == "true"   ||
        value == "1"      ||
        value == "y"      ||
        value == "yes"    ||
        value == "on")
      return true;

    return false;
  }

  return defaultValue;
}

////////////////////////////////////////////////////////////////////////////////
void Config::set (const std::string& key, const int value)
{
  _settings[key] = format (value);
}

////////////////////////////////////////////////////////////////////////////////
void Config::set (const std::string& key, const double value)
{
  _settings[key] = format (value, 1, 8);
}

////////////////////////////////////////////////////////////////////////////////
void Config::set (const std::string& key, const std::string& value)
{
  _settings[key] = value;
}

////////////////////////////////////////////////////////////////////////////////
