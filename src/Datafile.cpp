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
#include <Datafile.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <format>
#include <sstream>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
void Datafile::initialize (const std::string& name)
{
  _file = Path (name);
  debug (std::format ("Datafile::initialize {}", name));
}

////////////////////////////////////////////////////////////////////////////////
std::string Datafile::name () const
{
  return _file.name ();
}

////////////////////////////////////////////////////////////////////////////////
const std::vector <std::string>& Datafile::allLines ()
{
  if (! _lines_loaded)
    load_lines ();

  return _lines;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector <Painting> Datafile::allPaintings ()
{
  std::vector <Painting> all;
  for (auto& line : allLines ())
    if (Painting::is_painting (line))
      all.push_back (Painting (line));

  debug (std::format ("{}: {} paintings", _file.name (), all.size ()));
  return all;
}

////////////////////////////////////////////////////////////////////////////////
// Accepted paintings: Valid ID
//                     Valid Title
void Datafile::addPainting (const Painting& painting)
{
  assert (painting.id () != "");

  if (! _lines_loaded)
    load_lines ();

  auto composed = painting.compose ();
  debug (std::format ("Painting {}: {}", painting.id(), composed));

/*
  const std::string serialization = interval.serialize ();

  // Ensure that the IntervalFactory can properly parse the serialization before
  // adding it to the database.
  try
  {
    Interval test = IntervalFactory::fromSerialization (serialization);
    test.id = interval.id;
    if (interval != test)
      throw (std::format ("Encode / decode check failed:\n  {}\nis not equal to:\n  {}",
                     interval.dump (), test.dump ()));

    _lines.push_back (serialization);
    debug (std::format ("{}: Added {}", _file.name (), _lines.back ()));
    _dirty = true;
  }
  catch (const std::string& error)
  {
    debug (std::format ("Datafile::addInterval() failed.\n{}", error));
    throw std::string ("Internal error. Failed encode / decode check.");
  }
*/
}

////////////////////////////////////////////////////////////////////////////////
void Datafile::deletePainting (const Painting& painting)
{
  assert (painting.id () != "");

  if (! _lines_loaded)
    load_lines ();

/*
  auto serialized = interval.serialize ();
  auto i = std::find (_lines.begin (), _lines.end (), serialized);
  if (i == _lines.end ())
    throw std::format ("Datafile::deleteInterval failed to find '{}'", serialized);

  _lines.erase (i);
  _dirty = true;
  debug (std::format ("{}: Deleted {}", _file.name (), serialized));
*/
}

////////////////////////////////////////////////////////////////////////////////
const std::vector <Exhibition> Datafile::allExhibitions ()
{
  std::vector <Exhibition> all;
  auto lines = allLines ();
  for (unsigned int i = 0; i < lines.size (); ++i)
  {
    if (Exhibition::is_exhibition (lines[i]))
      all.push_back (Exhibition (lines[i]));
    else if (Submission::is_submission (lines[i]))
      all[all.size () - 1].add (Submission (lines[i]));
  }

  debug (std::format ("{}: {} exhibitions", _file.name (), all.size ()));
  return all;
}

////////////////////////////////////////////////////////////////////////////////
// Accepted exhibitions: Valid ID
//                     Valid Title
void Datafile::addExhibition (const Exhibition& exhibition)
{
  assert (exhibition.id () != "");

  if (! _lines_loaded)
    load_lines ();

  auto composed = exhibition.compose ();
  debug (std::format ("Exhibition {}: {}", exhibition.id(), composed));

/*
  const std::string serialization = interval.serialize ();

  // Ensure that the IntervalFactory can properly parse the serialization before
  // adding it to the database.
  try
  {
    Interval test = IntervalFactory::fromSerialization (serialization);
    test.id = interval.id;
    if (interval != test)
      throw (std::format ("Encode / decode check failed:\n  {}\nis not equal to:\n  {}",
                     interval.dump (), test.dump ()));

    _lines.push_back (serialization);
    debug (std::format ("{}: Added {}", _file.name (), _lines.back ()));
    _dirty = true;
  }
  catch (const std::string& error)
  {
    debug (std::format ("Datafile::addInterval() failed.\n{}", error));
    throw std::string ("Internal error. Failed encode / decode check.");
  }
*/
}

////////////////////////////////////////////////////////////////////////////////
void Datafile::deleteExhibition (const Exhibition& exhibition)
{
  assert (exhibition.id () != "");

  if (! _lines_loaded)
    load_lines ();

/*
  auto serialized = interval.serialize ();
  auto i = std::find (_lines.begin (), _lines.end (), serialized);
  if (i == _lines.end ())
    throw std::format ("Datafile::deleteInterval failed to find '{}'", serialized);

  _lines.erase (i);
  _dirty = true;
  debug (std::format ("{}: Deleted {}", _file.name (), serialized));
*/
}

////////////////////////////////////////////////////////////////////////////////
void Datafile::commit ()
{
  // The _dirty flag indicates that the file needs to be written.
  if (_dirty)
  {
    AtomicFile file (_file);
    if (! _lines.empty ())
    {
      if (file.open ())
      {
        // Sort the intervals by ascending start time.
        std::sort (_lines.begin (), _lines.end ());

        // Write out all the lines.
        file.truncate ();
        for (auto& line : _lines)
          file.write_raw (line + '\n');

        _dirty = false;
      }
      else
        throw std::format ("Could not write to data file {}", _file._data);
    }
    else
      file.remove ();
  }
}

////////////////////////////////////////////////////////////////////////////////
std::string Datafile::dump () const
{
  std::stringstream out;
  out << "Datafile\n"
      << "  Name:        " << _file.name () << (_file.exists () ? "" : " (does not exist)") << '\n'
      << "  dirty:       " << (_dirty ? "true" : "false") << '\n'
      << "  lines:       " << _lines.size () << '\n'
      << "    loaded     " << (_lines_loaded ? "true" : "false") << '\n';

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
void Datafile::load_lines ()
{
  AtomicFile file (_file);
  if (file.open ())
  {
    // Load the data.
    std::vector <std::string> read_lines;
    file.read (read_lines);
    file.close ();

    // Append the lines that were read.
    for (auto& line : read_lines)
      _lines.push_back (line);

    _lines_loaded = true;
    debug (std::format ("{}: {} lines", file.name (), read_lines.size ()));
  }
}

////////////////////////////////////////////////////////////////////////////////
