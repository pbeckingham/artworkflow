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
#include <cassert>

#include <Config.h>
#include <format.h>
#include <shared.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
Config::Config (sol::state& lua)
: _lua (&lua)
{
  assert (_lua);
}

////////////////////////////////////////////////////////////////////////////////
void Config::load (const std::string& file)
{
  assert (_lua);
  _original_file = file;

  try
  {
    _lua->safe_script_file (_original_file);
    debug ("Configuration file loaded");
  }
  catch (sol::error& e)
  {
    debug ("Configuration file error:");
    std::cout << std::string (e.what ()) << '\n';
  }
}

////////////////////////////////////////////////////////////////////////////////
bool Config::has (const std::string& key) const
{
  assert (_lua);
  return false;
}

////////////////////////////////////////////////////////////////////////////////
// Return the configuration value given the specified key.
std::string Config::get (const std::string& key, const std::string& defaultValue) const
{
  assert (_lua);
  return (*_lua)[key].get_or<std::string> (defaultValue);
}

////////////////////////////////////////////////////////////////////////////////
int Config::getInteger (const std::string& key, int defaultValue) const
{
  assert (_lua);
  return (*_lua)[key].get_or (defaultValue);
}

////////////////////////////////////////////////////////////////////////////////
double Config::getReal (const std::string& key, double defaultValue) const
{
  assert (_lua);
  return (*_lua)[key].get_or (defaultValue);
}

////////////////////////////////////////////////////////////////////////////////
bool Config::getBoolean (const std::string& key, bool defaultValue) const
{
  assert (_lua);
  return (*_lua)[key].get_or (defaultValue);
}

////////////////////////////////////////////////////////////////////////////////
void Config::set (const std::string& key, const std::string& value)
{
  assert (_lua);
  (*_lua)[key] = value;
}

////////////////////////////////////////////////////////////////////////////////
void Config::set (const std::string& key, const int value)
{
  assert (_lua);
  (*_lua)[key] = value;
}

////////////////////////////////////////////////////////////////////////////////
void Config::set (const std::string& key, const double value)
{
  assert (_lua);
  (*_lua)[key] = value;
}

////////////////////////////////////////////////////////////////////////////////
void Config::set (const std::string& key, const bool value)
{
  assert (_lua);
  (*_lua)[key] = value;
}

////////////////////////////////////////////////////////////////////////////////
sol::state* Config::lua ()
{
  return _lua;
}

////////////////////////////////////////////////////////////////////////////////
