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
#include <Lexer.h>
#include <RX.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
Submission::Submission (const std::string& line)
{
  parse (line);
}

////////////////////////////////////////////////////////////////////////////////
std::string Submission::id () const
{
  return _id;
}

////////////////////////////////////////////////////////////////////////////////
std::string Submission::dump (const std::string& title) const
{
  std::stringstream out;

  out << "\033[1m" << title << "\033[0m\n"
      << "_id            " << _id           << '\n'
      << "_status        " << _status       << '\n';

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
// ID:  ^\s{4}#S?\d{3}[a-d]?
bool Submission::is_submission (const std::string& line)
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
2021.04                                                                              ACA Real!
    #192  Medicinal Compound         Accepted, Sold, Award (Juror's Prize)
*/

void Submission::parse (const std::string& line)
{
  if (line.length () > 4)
    _id = Lexer::trimRight (line.substr (4, 6));
  else
    throw format("Missing id to parse.");

  // NOTE: Ignoring submission title.

  if (line.length () > 37)
    _status = Lexer::trimRight (line.substr (37));
}

////////////////////////////////////////////////////////////////////////////////
