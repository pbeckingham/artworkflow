////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2026, Paul Beckingham
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

#include <iostream>
#include <sstream>
#include <string>
#include <shared.h>
#include <format.h>
#include <RX.h>
#include <Lexer.h>
#include <Exhibition.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
Exhibition::Exhibition (const std::string& line)
{
  parse (line);
  debug (line);
  debug (dump ());
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::id () const
{
  return _id;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::submission () const
{
  return _submission;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::notification () const
{
  return _notification;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::delivery () const
{
  return _delivery;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::opening () const
{
  return _opening;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::reception () const
{
  return _reception;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::closing () const
{
  return _closing;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::pickup () const
{
  return _pickup;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::title () const
{
  return _title;
}

////////////////////////////////////////////////////////////////////////////////
bool Exhibition::matches (const std::string& id) const
{
  return id == _id;
}

////////////////////////////////////////////////////////////////////////////////
const std::string Exhibition::compose () const
{
  return "composed";
}

////////////////////////////////////////////////////////////////////////////////
// Note that incomplete data should validate.
//
// TODO: Ensure all dates are in logical sequence
bool Exhibition::validate (std::vector <std::string>& errors) const
{
  if (_id == "")
  {
    errors.push_back ("Missing ID");
    return false;
  }

  if (_title == "")
  {
    errors.push_back (format ("{1} Missing title", _id));
    return false;
  }

  if (_submission != "" && _notification != "" && _submission > _notification)
  {
    errors.push_back (format ("{1} Submission should be before notification", _id));
    return false;
  }

  if (_submission != "" && _delivery != "" && _submission > _delivery)
  {
    errors.push_back (format ("{1} Submission should be before delivery", _id));
    return false;
  }

  if (_submission != "" && _opening != "" && _submission > _opening)
  {
    errors.push_back (format ("{1} Submission should be before opening", _id));
    return false;
  }

  if (_submission != "" && _reception != "" && _submission > _reception)
  {
    errors.push_back (format ("{1} Submission should be before reception", _id));
    return false;
  }

  if (_submission != "" && _closing != "" && _submission > _closing)
  {
    errors.push_back (format ("{1} Submission should be before closing", _id));
    return false;
  }

  if (_submission != "" && _pickup != "" && _submission > _pickup)
  {
    errors.push_back (format ("{1} Submission should be before pickup", _id));
    return false;
  }

  if (_notification != "" && _delivery != "" && _notification > _delivery)
  {
    errors.push_back (format ("{1} Notification should be before delivery", _id));
    return false;
  }

  if (_notification != "" && _opening != "" && _notification > _opening)
  {
    errors.push_back (format ("{1} Notification should be before opening", _id));
    return false;
  }

  if (_notification != "" && _reception != "" && _notification > _reception)
  {
    errors.push_back (format ("{1} Notification should be before reception", _id));
    return false;
  }

  if (_notification != "" && _closing != "" && _notification > _closing)
  {
    errors.push_back (format ("{1} Notification should be before closing", _id));
    return false;
  }

  if (_notification != "" && _pickup != "" && _notification > _pickup)
  {
    errors.push_back (format ("{1} Notification should be before pickup", _id));
    return false;
  }

  if (_delivery != "" && _opening != "" && _delivery > _opening)
  {
    errors.push_back (format ("{1} Delivery should be before opening", _id));
    return false;
  }

  if (_delivery != "" && _reception != "" && _delivery > _reception)
  {
    errors.push_back (format ("{1} Delivery should be before reception", _id));
    return false;
  }

  if (_delivery != "" && _closing != "" && _delivery > _closing)
  {
    errors.push_back (format ("{1} Delivery should be before closing", _id));
    return false;
  }

  if (_delivery != "" && _pickup != "" && _delivery > _pickup)
  {
    errors.push_back (format ("{1} Delivery should be before pickup", _id));
    return false;
  }

  if (_opening != "" && _reception != "" && _opening > _reception)
  {
    errors.push_back (format ("{1} Opening should be before reception", _id));
    return false;
  }

  if (_opening != "" && _closing != "" && _opening > _closing)
  {
    errors.push_back (format ("{1} Opening should be before closing", _id));
    return false;
  }

  if (_opening != "" && _pickup != "" && _opening > _pickup)
  {
    errors.push_back (format ("{1} Opening should be before pickup", _id));
    return false;
  }

  if (_reception != "" && _closing != "" && _reception > _closing)
  {
    errors.push_back (format ("{1} Reception should be before closing", _id));
    return false;
  }

  if (_reception != "" && _pickup != "" && _reception > _pickup)
  {
    errors.push_back (format ("{1} Reception should be before pickup", _id));
    return false;
  }

  if (_closing != "" && _pickup != "" && _closing > _pickup)
  {
    errors.push_back (format ("{1} Closing should be before pickup", _id));
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
std::string Exhibition::dump (const std::string& title) const
{
  std::stringstream out;

  out << "\033[1m" << title << "\033[0m\n"
      << "_id            " << _id           << '\n'
      << "_submission    " << _submission   << '\n'
      << "_notification  " << _notification << '\n'
      << "_delivery      " << _delivery     << '\n'
      << "_opening       " << _opening      << '\n'
      << "_reception     " << _reception    << '\n'
      << "_closing       " << _closing      << '\n'
      << "_pickup        " << _pickup       << '\n'
      << "_title         " << _title        << '\n';

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
// EID: ^\d{4}\.\d{2}
bool Exhibition::is_exhibition (const std::string& line)
{
  RX eid ("^\\d{4}\\.\\d{2}");
  return eid.match (line);
}

////////////////////////////////////////////////////////////////////////////////
// ID:  ^\s{4}#S?\d{3}[a-d]?
bool Exhibition::is_submission (const std::string& line)
{
  RX id ("^\\s{4}#S?\\d{3}[a-d]?");
  return id.match (line);
}

////////////////////////////////////////////////////////////////////////////////
/*
                                                                                                    1         1
          1         2         3         4         5         6         7         8         9         0         1
012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

#ID     Deadline   Notify     Delivery   Open       Reception  Close      Pickup     Title
2026.04                       2026-08-06 2026-08-13 2026-08-13 2026-09-12 2026-09-16 Co|So, Resonance of the Ordinary
    #295 The Long Wave           12x24  V  F
    #330 The Last Toys           8x10   V  F
    #358 Generations             18x18  V  F
    #362 Choo Choo               6x18   V  F
*/

void Exhibition::parse (const std::string& line)
{
  if (line.length () > 0)
    _id = Lexer::trimRight (line.substr (0, 7));
  else
    throw format("Missing id to parse.");

  if (line.length () > 17)
    _submission = Lexer::trimRight (line.substr (8, 10));

  if (line.length () > 28)
    _notification = Lexer::trimRight (line.substr (19, 10));

  if (line.length () > 39)
    _delivery = Lexer::trimRight (line.substr (30, 10));

  if (line.length () > 50)
    _opening = Lexer::trimRight (line.substr (41, 10));

  if (line.length () > 61)
    _reception = Lexer::trimRight (line.substr (52, 10));

  if (line.length () > 72)
    _closing = Lexer::trimRight (line.substr (63, 10));

  if (line.length () > 83)
    _pickup = Lexer::trimRight (line.substr (74, 10));

  if (line.length () > 85)
    _title      = Lexer::trimRight (line.substr (85), " \n");
  else
    throw format("Missing title to parse.");

  // TODO: Parse submissions
}

////////////////////////////////////////////////////////////////////////////////
