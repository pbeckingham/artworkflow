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
bool filterByCLI (
  const CLI& cli,
  const Painting& painting,
  bool inclusive /* = false */)
{
  bool trivial = true;
  for (auto& arg : cli._args)
  {
    // Filter hints are a quick way to filter based on status.
    if (arg.hasTag ("FILTER") && arg.hasTag ("HINT"))
    {
      trivial = false;

      auto hint = arg.attribute ("canonical");
      if (hint == ":concept"   && ! painting.is_concept ())   return false;
      if (hint == ":wip"       && ! painting.is_wip ())       return false;
      if (hint == ":drying"    && ! painting.is_drying ())    return false;
      if (hint == ":varnished" && ! painting.is_varnished ()) return false;
      if (hint == ":inventory" && ! painting.is_inventory ()) return false;
      if (hint == ":gifted"    && ! painting.is_gifted ())    return false;
      if (hint == ":sold"      && ! painting.is_sold ())      return false;
      if (hint == ":abandoned" && ! painting.is_abandoned ()) return false;
      if (hint == ":destroyed" && ! painting.is_destroyed ()) return false;
      if (hint == ":nfs"       && ! painting.is_nfs ())       return false;
    }

    if (arg.hasTag ("FILTER") && ! arg.hasTag ("EID") && ! arg.hasTag ("HINT"))
    {
      // Use canonical form if available, otherwise raw input.
      auto value = arg.attribute ("canonical");
      if (value == "")
        value = arg.attribute ("raw");

      if (arg._lextype == Lexer::Type::word)
      {
        if (arg.hasTag ("ID"))
        {
          trivial = false;
          if (! painting.matches (arg.attribute ("raw")))
            return false;
        }
        else
        {
          trivial = false;
          if (painting.title ().find (value) == std::string::npos)
            return false;
        }
      }
      else if (arg._lextype == Lexer::Type::pattern)
      {
        trivial = false;
        RX rx (value.substr (1, value.length () - 2));
        if (! rx.match (painting.title ()))
          return false;
      }

      // TODO: Implement other filtering metadata: size...
    }
  }

//  if (inclusive || ! trivial)
//    debug (format ("#{1} '{2}' matches", painting.id (), painting.title ()));

  return inclusive || ! trivial;
}

////////////////////////////////////////////////////////////////////////////////
bool filterByCLI (
  const CLI& cli,
  const Exhibition& exhibition,
  bool inclusive /* = false */)
{
  bool trivial = true;
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("FILTER") && ! arg.hasTag ("ID") && ! arg.hasTag ("HINT"))
    {
      // Use canonical form if available, otherwise raw input.
      auto value = arg.attribute ("canonical");
      if (value == "")
        value = arg.attribute ("raw");

      if (arg._lextype == Lexer::Type::number)
      {
        if (arg.hasTag ("EID"))
        {
          trivial = false;
          if (! exhibition.matches (arg.attribute ("raw")))
            return false;
        }
      }
      else if (arg._lextype == Lexer::Type::word)
      {
        trivial = false;
        if (exhibition.title ().find (value) == std::string::npos)
          return false;
      }
      else if (arg._lextype == Lexer::Type::pattern)
      {
        trivial = false;
        RX rx (value.substr (1, value.length () - 2));
        if (! rx.match (exhibition.title ()))
          return false;
      }
    }
  }

//  if (inclusive || ! trivial)
//    debug (format ("{1} '{2}' matches", exhibition.id (), exhibition.title ()));

  return inclusive || ! trivial;
}
////////////////////////////////////////////////////////////////////////////////
