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

#ifndef INCLUDED_PAINTING
#define INCLUDED_PAINTING

#include <map>
#include <string>
#include <vector>

// Represents a physical painting.
class Painting
{
public:
  Painting () = default;
  Painting (const std::string&);

  std::string id () const;
  std::string title () const;
  std::string series () const;
  std::string start () const;
  std::string end () const;
  std::string varnish () const;
  std::string action () const;
  std::string size () const;
  std::string substrate () const;
  std::string tagged () const;
  std::string varnished () const;
  std::string archived () const;
  std::string www () const;
  std::string complexity () const;
  std::string notes () const;

  bool is_concept () const;
  bool is_wip () const;
  bool is_drying () const;
  bool is_inventory () const;
  bool is_sold () const;
  bool is_gifted () const;
  bool is_abandoned () const;
  bool is_destroyed () const;
  bool is_varnished () const;

  bool matches (const std::string&);

  const std::string compose () const;

  bool validate (std::vector <std::string>&) const;
  std::string dump (const std::string& title = "Painting") const;
  std::string card () const;

  static bool is_painting (const std::string&);

private:
  void parse (const std::string&);

private:
  std::string _original_line;

  std::string _id;
  std::string _title;
  std::string _series;
  std::string _start;
  std::string _end;
  std::string _varnish;
  std::string _action;
  std::string _size;
  std::string _substrate;
  std::string _tagged;
  std::string _varnished;
  std::string _archived;
  std::string _www;
  std::string _complexity;
  std::string _notes;

  // TODO: groups
  // TODO: time
};

#endif
