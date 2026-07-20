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
#include <CLI.h>
#include <RX.h>
#include <Painting.h>
#include <format.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
bool filterByCLI (const CLI& cli, const Painting& painting)
{
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("FILTER"))
    {
      // Use canonical form if available, otherwise raw input.
      auto value = arg.attribute ("canonical");
      if (value == "")
        value = arg.attribute ("raw");

      if (arg._lextype == Lexer::Type::word)
      {
        if (arg.hasTag ("ID"))
          if (! painting.matches (arg.attribute ("raw")))
            return false;
      }
      else if (arg._lextype == Lexer::Type::pattern)
      {
        RX rx (value.substr (1, value.length () - 2));
        if (! rx.match (painting.title ()))
          return false;
      }
      else
      {
        throw format ("Unrecognized filter type '{1}'", value);
      }

      // TODO: Implement other filtering metadata: size...
    }
  }

  debug (format ("#{1} '{2}' matches", painting.id (), painting.title ()));
  return true;
}

////////////////////////////////////////////////////////////////////////////////
bool filterByCLI (const CLI& cli, const Exhibition& exhibition)
{

  return true;
}
////////////////////////////////////////////////////////////////////////////////
