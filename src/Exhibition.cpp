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
#include <Exhibition.h>

////////////////////////////////////////////////////////////////////////////////
Exhibition::Exhibition (const std::string& line)
{
  parse (line);
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
const std::string Exhibition::compose () const
{
  return "composed";
}

////////////////////////////////////////////////////////////////////////////////
// Note that incomplete data should validate.
//
// TODO Ensure all dates are in logical sequence
/*
      # Logical inconsistencies in the data file.
      if bool(varnish_date) != bool(varnish) and action != 'd':
        errors.append(f'#{serial} {title} has inconsistent varnish information')

      if start_date and action != 'd' and not size:
        errors.append(f'#{serial} {title} is missing size information')

      if start_date and action != 'd' and not substrate:
        errors.append(f'#{serial} {title} is missing substrate information')

      if end_date and not action:
        if not tagged:
          errors.append(f'#{serial} {title} is not tagged')
        if not archive:
          errors.append(f'#{serial} {title} is not listed on ArtworkArchive')
        #if not www and 'NFS' not in notes:
        #  errors.append(f'#{serial} {title} is not listed on paulbeckingham.com')

      if action == '$' and action_date and not varnish_date:
        errors.append(f'#{serial} {title} is sold unvarnished')

      if end_date and not action and title.startswith('[') and title.endswith(']'):
        errors.append(f'#{serial} {title} needs a title')
*/
bool Exhibition::validate () const
{
  if (_id    == "" || _title == "")
    return false;

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
/*
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
    _id         = rtrim (line.substr (1, 7));
  else
    throw format("Missing id to parse.");

  if (line.length () > 17)
    _submission = rtrim (line.substr (8, 10));

  if (line.length () > 28)
    _notification = rtrim (line.substr (19, 10));

  if (line.length () > 39)
    _delivery = rtrim (line.substr (30, 10));

  if (line.length () > 50)
    _opening = rtrim (line.substr (41, 10));

  if (line.length () > 61)
    _reception = rtrim (line.substr (52, 10));

  if (line.length () > 72)
    _closing = rtrim (line.substr (63, 10));

  if (line.length () > 83)
    _pickup = rtrim (line.substr (74, 10));

  if (line.length () > 85)
    _title      = rtrim (line.substr (85, std::string::npos));
  else
    throw format("Missing title to parse.");

  // TODO Parse submissions
}

////////////////////////////////////////////////////////////////////////////////
