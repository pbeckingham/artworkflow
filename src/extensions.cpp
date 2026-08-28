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

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <format.h>
#include <shared.h>
#include <iostream>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
// List of C functions accessible to Lua:
//   apiGetVersion         Returns product version string
//   apiDomGet             Returns DOM value
//
// List of functions to override in Lua:
//   extensionOnEntry      Earliest opportunity to run
//   extensionOnExit       Earliest opportunity to run
//   extensionCmdVersion   Returns additional version text
//   extensionCmdHelp      Returns additional help text
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void extensionAPI ()
{
  (*get_lua_vm ())["apiGetVersion"] = []()
    {
      return VERSION;
    };

  (*get_lua_vm ())["apiGet"] = []()
    {
      // TODO: Perform a domGetcall.
    };
}

////////////////////////////////////////////////////////////////////////////////
void extensionOnEntry ()
{
  debug ("extensionOnEntry");

  // Determine if 'extensionOnEntry' is a lua function, before calling it.
  sol::protected_function function_object = (*get_lua_vm ())["extensionOnEntry"];
  if (function_object)
  {
    auto result = function_object ();
    if (result.valid ())
    {
      debug ("extensionOnEntry success");
    }
    else
    {
      sol::error e = result;
      debug (format ("extensionOnEntry: {1}", e.what ()));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void extensionOnExit ()
{
  debug ("extensionOnExit");

  // Determine if 'extensionOnExit' is a lua function, before calling it.
  sol::protected_function function_object = (*get_lua_vm ())["extensionOnExit"];
  if (function_object)
  {
    auto result = function_object ();
    if (result.valid ())
    {
      debug ("extensionOnExit success");
    }
    else
    {
      sol::error e = result;
      debug (format ("extensionOnExit: {1}", e.what ()));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void extensionCmdVersion ()
{
  debug ("extensionCmdVersion");

  // Determine if 'extensionCmdVersion' is a lua function, before calling it.
  sol::protected_function function_object = (*get_lua_vm ())["extensionCmdVersion"];
  if (function_object)
  {
    auto result = function_object ();
    if (result.valid ())
    {
      debug ("extensionCmdVersion success");
    }
    else
    {
      sol::error e = result;
      debug (format ("extensionCmdVersion: {1}", e.what ()));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void extensionCmdHelp ()
{
  debug ("extensionCmdHelp");

  // Determine if 'extensionCmdHelp' is a lua function, before calling it.
  sol::protected_function function_object = (*get_lua_vm ())["extensionCmdHelp"];
  if (function_object)
  {
    auto result = function_object ();
    if (result.valid ())
    {
      debug ("extensionCmdHelp success");
    }
    else
    {
      sol::error e = result;
      debug (format ("extensionCmdHelp: {1}", e.what ()));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
std::string extensionCard ()
{
  debug ("extensionCard");
  std::string output {};

  // Determine if 'extensionCmdHelp' is a lua function, before calling it.
  sol::protected_function function_object = (*get_lua_vm ())["extensionCard"];
  if (function_object)
  {
    auto result = function_object ();
    if (result.valid ())
    {
      output = result;
      debug ("extensionCard success");
    }
    else
    {
      sol::error e = result;
      debug (format ("extensionCard: {1}", e.what ()));
    }
  }

  return output;
}

////////////////////////////////////////////////////////////////////////////////
std::string extensionMiniCard ()
{
  debug ("extensionMiniCard");
  std::string output {};

  // Determine if 'extensionCmdHelp' is a lua function, before calling it.
  sol::protected_function function_object = (*get_lua_vm ())["extensionMiniCard"];
  if (function_object)
  {
    auto result = function_object ();
    if (result.valid ())
    {
      output = result;
      debug ("extensionMiniCard success");
    }
    else
    {
      sol::error e = result;
      debug (format ("extensionMiniCard: {1}", e.what ()));
    }
  }

  return output;
}

////////////////////////////////////////////////////////////////////////////////
