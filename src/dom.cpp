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

#include <Database.h>
#include <Duration.h>
#include <Pig.h>
#include <format.h>
#include <iostream>
#include <artworkflow.h>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
bool domGet (
  Database& database,
  const Config& config,
  const std::string& reference,
  std::string& value)
{
  // Find the lowest ID that is not currently used.
  if (reference == "dom.newid")
  {
    // Load all objects, extract the integer form or ID.
    std::vector <int> ids;
    for (auto& p : database.allPaintings ())
      ids.push_back (strtol (p.id ().c_str (), nullptr, 10));

    // Find the first gap.
    int id = 1;
    while (std::find (ids.begin (), ids.end (), id) != ids.end ())
      ++id;

    // Use whichever number id contains. This is safe and free of wrapping
    // below INT_MAX, which is a lot.
    debug (format ("{1} paintings loaded", ids.size ()));
    debug (format ("{1} is free", id));

    value = "#" + rightJustifyZero (id, 3);
    return true;
  }
  else
  {
    Pig pig (reference);
    if (pig.skipLiteral ("dom."))
    {
      // dom.[all|inventory|sold|gifted|abandoned|destroyed|wip].[ids|count]
      if (pig.skipLiteral ("all."))
      {
        if (pig.skipLiteral ("ids"))
        {
        }
        else if (pig.skipLiteral ("count"))
        {
        }
      }
      else if (pig.skipLiteral ("inventory."))
      {
        if (pig.skipLiteral ("ids"))
        {
        }
        else if (pig.skipLiteral ("count"))
        {
        }
      }
      else if (pig.skipLiteral ("sold."))
      {
        if (pig.skipLiteral ("ids"))
        {
        }
        else if (pig.skipLiteral ("count"))
        {
        }
      }
      else if (pig.skipLiteral ("gifted."))
      {
        if (pig.skipLiteral ("ids"))
        {
        }
        else if (pig.skipLiteral ("count"))
        {
        }
      }
      else if (pig.skipLiteral ("abandoned."))
      {
        if (pig.skipLiteral ("ids"))
        {
        }
        else if (pig.skipLiteral ("count"))
        {
        }
      }
      else if (pig.skipLiteral ("destroyed."))
      {
        if (pig.skipLiteral ("ids"))
        {
        }
        else if (pig.skipLiteral ("count"))
        {
        }
      }
      else if (pig.skipLiteral ("wip."))
      {
        if (pig.skipLiteral ("ids"))
        {
        }
        else if (pig.skipLiteral ("count"))
        {
        }
      }

      // TODO dom.<id>.<meta>
      // TODO dom.<id>.[start|end|varnish|action].[year|month|day|age]

      // dom.comfig.<name>
      else if (pig.skipLiteral ("config."))
      {
        std::string name;
        if (pig.getRemainder (name))
        {
          // TODO Need type coercion. If it's a boolean, render it as a string, etc.
          value = config.get (name);
          return true;
        }
      }
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
