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

#include <format.h>
#include <shared.h>
#include <iostream>
#include <Config.h>
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

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

////////////////////////////////////////////////////////////////////////////////
void extensionOnEntry (Config& config)
{
  debug ("extensionOnEntry");

  // Determine if 'extensionOnEntry' is a lua function, before calling it.
  sol::protected_function function_object = (*config.lua ())["extensionOnEntry"];
  if (function_object)
    function_object ();
}

////////////////////////////////////////////////////////////////////////////////
void extensionOnExit (Config& config)
{
  debug ("extensionOnExit");

  // Determine if 'extensionOnExit' is a lua function, before calling it.
  sol::protected_function function_object = (*config.lua ())["extensionOnExit"];
  if (function_object)
    function_object ();
}

////////////////////////////////////////////////////////////////////////////////
void extensionCmdVersion (Config& config)
{
  debug ("extensionCmdVersion");

  // Determine if 'extensionCmdVersion' is a lua function, before calling it.
  sol::protected_function function_object = (*config.lua ())["extensionCmdVersion"];
  if (function_object)
    function_object ();
}

////////////////////////////////////////////////////////////////////////////////
void extensionCmdHelp (Config& config)
{
  debug ("extensionCmdHelp");

  // Determine if 'extensionCmdHelp' is a lua function, before calling it.
  sol::protected_function function_object = (*config.lua ())["extensionCmdHelp"];
  if (function_object)
    function_object ();
}

////////////////////////////////////////////////////////////////////////////////
