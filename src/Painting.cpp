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
#include <Painting.h>

////////////////////////////////////////////////////////////////////////////////
Painting::Painting (const std::string& line)
{
  parse (line);
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::id () const
{
  return _id;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::title () const
{
  return _title;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::series () const
{
  return _series;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::start () const
{
  return _start;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::end () const
{
  return _end;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::varnish () const
{
  return _varnish;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::action () const
{
  return _action;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::size () const
{
  return _size;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::sub () const
{
  return _sub;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::tagged () const
{
  return _tagged;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::varnished () const
{
  return _varnished;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::archived () const
{
  return _archived;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::www () const
{
  return _www;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::complexity () const
{
  return _complexity;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::notes () const
{
  return _notes;
}

////////////////////////////////////////////////////////////////////////////////
const std::string Painting::compose () const
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
bool Painting::validate () const
{
  if (_id    == "" || _title == "")
    return false;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::dump (const std::string& title) const
{
  std::stringstream out;

  out << "\033[1m" << title << "\033[0m\n";
  out << "_id         " << _id          << '\n'
      << "_title      " << _title       << '\n'
      << "_series     " << _series      << '\n'
      << "_start      " << _start       << '\n'
      << "_end        " << _end         << '\n'
      << "_varnish    " << _varnish     << '\n'
      << "_action     " << _action      << '\n'
      << "_size       " << _size        << '\n'
      << "_sub        " << _sub         << '\n'
      << "_tagged     " << _tagged      << '\n'
      << "_varnished  " << _varnished   << '\n'
      << "_archived   " << _archived    << '\n'
      << "_www        " << _www         << '\n'
      << "_complexity " << _complexity  << '\n'
      << "_notes      " << _notes       << '\n';

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
/*
                                                                                                    1         1
          1         2         3         4         5         6         7         8         9         0         1
012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
ID    TITLE                       SER START       END         VARNISH     ACTION      SIZE  SUB T V AA WWW CX NOTES
----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
#378  [Pumpkins]                  STL s2025-08-09                         d2025-11-11 6x8   ACM            C2 Abandoned
#379  Central Line Surfer         STL s2026-02-23 e2026-04-06 v2026-04-17             12x18 ACM Y C        C2 43h G:Still_Life
#380  [Crafts]                    STL s2026-05-07 e2026-05-23                         12x18 ACM            C4 34h G:Still_Life
#381  [Herring 2]                 WIL s2026-05-23 e2026-05-27                         7x17  S              C1 2h
#382  [Herring 3]                 WIL s2026-05-23 e2026-05-27                         4x10  S              C1 2h
#383  Coleoptera IV               WIL                                                                      C4

#S001 Juan de Pareja (Velasquez)  POR s2025-04-04 e2025-04-05 v2025-05-24             7x5   ACM Y C        C2 G:Study
#S002 Mike Derby                  POR s2025-05-24 e2025-06-25 v2025-07-19 g2025-07-23 10x10 WP  Y W        C2 12h G:Study
*/

void Painting::parse (const std::string& line)
{
  if (line.length () > 0)
    _id         = rtrim (line.substr (1, 4));
  else
    throw format("Missing id to parse.");

  if (line.length () > 32)
    _title      = rtrim (line.substr (6, 28));
  else
    throw format("Missing title to parse.");

/*
          1         2         3         4         5         6         7         8         9         0         1
012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
ID    TITLE                       SER START       END         VARNISH     ACTION      SIZE  SUB T V AA WWW CX NOTES
----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
#378  [Pumpkins]                  STL s2025-08-09                         d2025-11-11 6x8   ACM            C2 Abandoned
#379  Central Line Surfer         STL s2026-02-23 e2026-04-06 v2026-04-17             12x18 ACM Y C        C2 43h G:Still_Life
*/
  if (line.length () > 36)
    _series     = rtrim (line.substr (34, 3));

  if (line.length () > 48)
    _start      = rtrim (line.substr (38, 11));

  if (line.length () > 60)
    _end        = rtrim (line.substr (50, 11));

  if (line.length () > 72)
    _varnish    = rtrim (line.substr (62, 11));

  if (line.length () > 84)
    _action     = rtrim (line.substr (74, 11));

  if (line.length () > 90)
    _size       = rtrim (line.substr (86, 5));

  if (line.length () > 94)
    _sub        = rtrim (line.substr (92, 3));

  if (line.length () > 96)
    _tagged     = rtrim (line.substr (96, 1));

  if (line.length () > 98)
    _varnished  = rtrim (line.substr (98, 1));

  if (line.length () > 100)
    _archived   = rtrim (line.substr (100, 1));

  if (line.length () > 105)
    _www        = rtrim (line.substr (105, 1));

  if (line.length () > 108)
    _complexity = rtrim (line.substr (107, 2));

  if (line.length () > 110)
    _notes      = rtrim (line.substr (110));
}

////////////////////////////////////////////////////////////////////////////////
