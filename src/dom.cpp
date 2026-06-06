////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016, 2018 - 2023, 2025, Gothenburg Bit Factory.
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
  const Rules& rules,
  const std::string& reference,
  std::string& value)
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

    // dom.<id>.<meta>
    // dom.<id>.[start|end|varnish|action].[year|month|day|age]

    // dom.rc.<name>
    else if (pig.skipLiteral ("rc."))
    {
      std::string name;
      if (pig.getRemainder (name))
      {
        value = rules.get (name);
        return true;
      }
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
