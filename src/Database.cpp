////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016, 2018 - 2023, Thomas Lauf, Paul Beckingham, Federico Hernandez.
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

#include <AtomicFile.h>
#include <Database.h>
#include <JSON.h>
#include <cassert>
#include <format.h>
#include <iomanip>
#include <iostream>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
void Database::initialize (const std::string& location)
{
  _location = location;
}

////////////////////////////////////////////////////////////////////////////////
void Database::commit ()
{
  _paintings.commit ();
  _exhibitions.commit ();
}

////////////////////////////////////////////////////////////////////////////////
std::vector <std::string> Database::files () const
{
  std::vector <std::string> all;
  all.push_back (_paintings.name());
  all.push_back (_exhibitions.name());

  return all;
}

////////////////////////////////////////////////////////////////////////////////
/*
void Database::addInterval (const Interval& interval, bool verbose)
{
  assert ((interval.end == 0) || (interval.start <= interval.end));

  // Get the index into _files for the appropriate Datafile, which may be created on demand.
  auto df = getDatafile (interval.start.year (), interval.start.month ());
  _files[df].addInterval (interval);
}
*/

////////////////////////////////////////////////////////////////////////////////
/*
void Database::deleteInterval (const Interval& interval)
{
  // Get the index into _files for the appropriate Datafile, which may be
  // created on demand.
  auto df = getDatafile (interval.start.year (), interval.start.month ());

  _files[df].deleteInterval (interval);
}
*/

////////////////////////////////////////////////////////////////////////////////
// The algorithm to modify an interval is first to find and remove it from the
// Datafile, then add it back to the right Datafile. This is because
// modification may involve changing the start date, which could mean the
// Interval belongs in a different file.
/*
void Database::modifyInterval (const Interval& from, const Interval& to, bool verbose)
{
  if (! from.empty ())
  {
    deleteInterval (from);
  }

  if (! to.empty ())
  {
    addInterval (to, verbose);
  }
}
*/

////////////////////////////////////////////////////////////////////////////////
std::string Database::dump () const
{
  std::stringstream out;
  out << "Database\n"
      << "  Paintings:   " << _paintings.name () << '\n'
      << "  Exhibitions: " << _exhibitions.name () << '\n';

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
