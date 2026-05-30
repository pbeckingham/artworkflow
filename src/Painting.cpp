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

#include <sstream>
#include <string>
#include <shared.h>
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
const std::string Painting::compose () const
{
  return "composed";
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::validate () const
{
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
  _id         = line.substr (0, 5);
  _title      = line.substr (6, 33);
  _series     = line.substr (34, 37);
  _start      = line.substr (38, 49);
  _end        = line.substr (50, 61);
  _varnish    = line.substr (62, 73);
  _action     = line.substr (74, 85);
  _size       = line.substr (86, 91);
  _sub        = line.substr (92, 95);
  _tagged     = line.substr (96, 97);
  _varnished  = line.substr (98, 99);
  _archived   = line.substr (100, 102);
  _www        = line.substr (103, 106);
  _complexity = line.substr (107, 109);
  _notes      = line.substr (110, std::string::npos);
}

////////////////////////////////////////////////////////////////////////////////
