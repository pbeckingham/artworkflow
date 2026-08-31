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
#include <format>
#include <RX.h>
#include <Pig.h>
#include <Lexer.h>
#include <Color.h>
#include <Composite.h>
#include <utf8.h>
#include <Painting.h>
#include <artworkflow.h>

static std::map <std::string, std::string> series_names
{
  {"STL", "Still Life"},
  {"LAN", "Landscape"},
  {"BOA", "Boat"},
  {"POR", "Portrait"},
  {"DFL", "Dead Flower"},
  {"FLO", "Floral"},
  {"MCP", "Master Copy"},
  {"ABS", "Abstract"},
  {"WRK", "Workshop"},
  {"WIL", "Wildlife"},
};

static std::map <std::string, std::string> varnish_names
{
  {"D",     "Dammar"},
  {"G",     "Gamvar"},
  {"C",     "Conservar"},
  {"W",     "Winsor & Newton"},
  {"Other", "Other"},
};

static std::map <std::string, std::string> substrate_names
{
  {"ACM", "Aluminum Composite Material"},
  {"CB",  "Canvas Board"},
  {"WP",  "Wood Panel"},
  {"C",   "Canvas"},
};

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
std::string Painting::series_name () const
{
  return series_names[_series];
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
std::string Painting::varnish_name () const
{
  return varnish_names[_varnish];
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
std::string Painting::substrate () const
{
  return _substrate;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::substrate_name () const
{
  return substrate_names[_substrate];
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
// Notes may contain: 34h G:Still_Life G:13Forest
std::vector <std::string> Painting::groups () const
{
  RX group_pattern ("G:\\S+");
  std::vector <std::string> all;
  group_pattern.match (all, _notes);

  std::vector <std::string> results;
  for (auto& group : all)
    results.push_back (group.substr (2));

  return results;
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::effort () const
{
  RX effort_pattern ("\\d+h");
  std::vector <std::string> all;
  if (effort_pattern.match (all, _notes))
    return all[0];

  return "";
}

////////////////////////////////////////////////////////////////////////////////
std::string Painting::disposition () const
{
  if (is_concept ())   return "Concept";
  if (is_drying ())    return "Drying";
  if (is_destroyed ()) return "Destroyed";
  if (is_nfs ())       return "NFS";
  if (is_inventory ()) return "Inventory";
  if (is_gifted ())    return "Gifted";
  if (is_sold ())      return "Sold";
  if (is_abandoned ()) return "Abandoned";
  if (is_wip ())       return "WIP";

  return "Unknown";
}

////////////////////////////////////////////////////////////////////////////////
int Painting::height () const
{
  return _height;
}

////////////////////////////////////////////////////////////////////////////////
int Painting::width () const
{
  return _width;
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_concept () const
{
  return _id != "" && _title != "" && _start == "";
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_wip () const
{
  return _start != "" && _end == "" && _action == "" && _notes.find ("Abandoned") == std::string::npos;
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_drying () const
{
  return _start != "" && _end != "" && _varnish == "" && _notes.find ("Abandoned") == std::string::npos && _action == "";
}

////////////////////////////////////////////////////////////////////////////////
// return self.start_date and \
//   self.end_date and \
//   self.varnish_date and \
//   'Abandoned' not in self.notes and \
//   'NFS' not in self.notes and \
//   '13Forest' not in self.groups and \
//   'Bowersock' not in self.groups and \
//   not self.action
bool Painting::is_inventory () const
{
  return _start != "" &&
         _end != "" &&
         _varnish != "" &&
         _notes.find ("Abandoned") == std::string::npos &&
         _notes.find ("NFS") == std::string::npos &&
         _action == "";
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_sold () const
{
  return _action != "" && _action[0] == '$';
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_gifted () const
{
  return _action != "" && _action[0] == 'g';
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_abandoned () const
{
  return _start != "" && _notes.find ("Abandoned") != std::string::npos;
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_destroyed () const
{
  return _action != "" && _action[0] == 'd';
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_varnished () const
{
  return _varnish != "";
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::is_nfs () const
{
  return _start != "" && _notes.find ("NFS") != std::string::npos;
}

////////////////////////////////////////////////////////////////////////////////
bool Painting::matches (const std::string& id) const
{
  return Lexer::trimLeft (_id, "#") == Lexer::trimLeft (id, "#");
}

////////////////////////////////////////////////////////////////////////////////
const std::string Painting::compose () const
{
  return "composed";
}

////////////////////////////////////////////////////////////////////////////////
// Note that incomplete data should validate.
//
// TODO: Ensure all dates are in logical sequence
// TODO: Ensure completion hours are tracked
bool Painting::validate (
  std::vector <std::string>& errors,
  std::vector <std::string>& warnings) const
{
  if (_id == "")
    errors.push_back ("Missing ID");

  if (_title == "")
    errors.push_back ("Missing title");

  if (! is_destroyed () &&
      ((_varnished != "" && _varnish == "") ||
       (_varnished == "" && _varnish != "")))
    errors.push_back ("Inconsistent varnish information");

  if (_action != "" && _action [0] == '$' && _varnish == "")
    warnings.push_back ("Sold unvarnished");

  if (_action != "" && _action[0] == 'g' && _varnish == "")
    warnings.push_back ("Gifted unvarnished");

  if (_series == "")
    errors.push_back ("Missing a series");

  if ((_end != "" || _varnish != "") && _start == "")
    errors.push_back ("Missing a start date");

  if (_varnish != "" && _end == "")
    errors.push_back ("Missing an end date");

  if (_start != "" && ! is_destroyed () && _substrate == "")
    errors.push_back ("Missing a substrate");

  if (_start != "" && ! is_destroyed () && _size == "")
    errors.push_back ("Missing a size");

  if (_end != "" && _action == "" && _tagged == "")
    warnings.push_back ("Not tagged");

// Note: Too vocal.
//  if (_varnished != "" && _www == "")
//    errors.push_back ("is not posted on website");

  if (_end != "" && _action == "" && _varnished != "" && _archived == "")
    warnings.push_back ("Not archived");

  if (is_sold () && _tagged == "")
    warnings.push_back ("Sold untagged");

  return errors.size () + warnings.size () > 0 ? true : false;
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
      << "_substrate  " << _substrate   << '\n'
      << "_tagged     " << _tagged      << '\n'
      << "_varnished  " << _varnished   << '\n'
      << "_archived   " << _archived    << '\n'
      << "_www        " << _www         << '\n'
      << "_complexity " << _complexity  << '\n'
      << "_notes      " << _notes       << '\n';

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
const std::string Painting::get_background_color (void) const
{
  std::string background = "on 0xffff00";
       if (is_concept ())   background = "on 0x401050";
  else if (is_destroyed ()) background = "on 0xa00000";
  else if (is_abandoned ()) background = "on 0x000030";
  else if (is_nfs ())       background = "on 0x304030";
  else if (is_gifted ())    background = "on 0x003000";
  else if (is_sold ())      background = "on 0x006000";
  else if (is_wip ())       background = "on 0x202080";
  else if (is_inventory ()) background = "on 0x404040";
  else if (is_drying ())    background = "on 0x2020c0";

  return background;
}

////////////////////////////////////////////////////////////////////////////////
std::vector <std::string> Painting::card (int width /* = 40 */) const
{
  std::string background = get_background_color ();
  Color card ("gray12 on " + background);

  if (groups ().size ())
  {
    // TODO: Override bg with a rota color.
  }

  std::vector <std::string> lines = mini_card (width);

  if (_start != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    std::string line ("Start      " + _start.substr (1));
    if (_end == "")
    {
      Datetime ds (_start.substr (1));
      Datetime now;
      Duration age = now - ds;
      line += std::format (" ({} days)", age.days ());
    }
    cline.add (line.substr (0, width - 1), 1, card);
    lines.push_back (cline.str ());
  }

  if (_end != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    std::string line ("End        " + _end.substr (1));
    if (_varnish == "")
    {
      Datetime de (_end.substr (1));
      Datetime now;
      Duration age = now - de;
      line += std::format (" (Drying {} days)", age.days ());
    }
    else
    {
      Datetime ds (_start.substr (1));
      Datetime de (_end.substr (1));
      Duration age = de - ds;
      line += std::format (" ({} days elapsed)", age.days ());
    }
    cline.add (line.substr (0, width - 1), 1, card);
    lines.push_back (cline.str ());
  }

  if (_varnish != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);

    std::string line (" Varnish    " + _varnish.substr (1));
    if (_varnished != "")
      line += ' ' + varnish_names[_varnished];

    Datetime de (_end.substr (1));
    Datetime dv (_varnish.substr (1));
    Duration age = dv - de;
    line += std::format (" (Dried for {} days)", age.days ());

    cline.add (line.substr (0, width), 0, card);
    lines.push_back (cline.str ());
  }

  if (_series != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    std::string line ("Series     " + series_name ());
    cline.add (line.substr (0, width - 1), 1, card);
    lines.push_back (cline.str ());
  }

  if (_size != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add ("Size       " + _size, 1, card);
    lines.push_back (cline.str ());
  }

  if (_complexity != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add ("Complexity " + _complexity, 1, card);
    lines.push_back (cline.str ());
  }

  if (_substrate != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add (std::string ("Substrate  " + substrate_name ()).substr (0, width - 1), 1, card);
    lines.push_back (cline.str ());
  }

  if (effort () != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add ("Effort     " + effort (), 1, card);
    lines.push_back (cline.str ());
  }

  if (_tagged != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add ("Tagged     " + _tagged, 1, card);
    lines.push_back (cline.str ());
  }

  if (_archived != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add ("Archived   " + _archived, 1, card);
    lines.push_back (cline.str ());
  }

  if (_www != "")
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add ("WWW        " + _tagged, 1, card);
    lines.push_back (cline.str ());
  }

  if (groups ().size ())
  {
    Composite cline;
    cline.add (std::string (width, ' '), 0, card);
    cline.add ("Groups", 1, card);
    cline.add (join (" ", groups ()).substr (0, width - 12), 12, card);
    lines.push_back (cline.str ());
  }

  auto extra = extensionCard ();
  if (extra != "")
  {
    Composite cline;
    auto extra_lines = split (extra, '\n');
    for (auto& line : extra_lines)
    {
      cline.add (std::string (width, ' '), 0, card);
      cline.add (line.substr (0, width - 1), 1, card);
      lines.push_back (cline.str ());
    }
  }

  return lines;
}

////////////////////////////////////////////////////////////////////////////////
// A mini card is used for the Kanban report, being only three lines long, and
// about 20 characterswide, using color-coded backgrounds.
std::vector <std::string> Painting::mini_card (int width /* = 24 */) const
{
  // Backgrounds: Concept, WIP, Inventory, Sold/Gifted, Destroyed, Abandoned
//  std::string background = "on 0x303030";
  std::string background = get_background_color ();
  Color color_id ("0x808080 " + background);
  Color color_title ("0xaaaaaa " + background);

  std::vector <std::string> lines;

  Composite cid;
  cid.add (std::string (width, ' '), 0, color_id);
  cid.add ("#" + _id, 0, color_id);

  auto label = disposition ();
  cid.add (label, width - label.length (), color_id);

  lines.push_back (cid.str ());

  Composite ctitle;
  ctitle.add (std::string (width, ' '), 0, color_title);
  ctitle.add (_title.substr (0, width), 0, color_title);
  lines.push_back (ctitle.str ());

  auto extra = extensionMiniCard ();
  if (extra != "")
  {
    Composite cline;
    auto extra_lines = split (extra, '\n');
    for (auto& line : extra_lines)
    {
      cline.add (std::string (width, ' '), 0, color_id);
      cline.add (line.substr (0, width - 1), 1, color_id);
      lines.push_back (cline.str ());
    }
  }

  return lines;
}

////////////////////////////////////////////////////////////////////////////////
// A painting ID is: ^#S?\d\d\d[a-d]?
bool Painting::is_painting (const std::string& sample)
{
  RX id ("^#?S?\\d\\d\\d[a-d]?");
  return id.match (sample);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                                                     1         1
//           1         2         3         4         5         6         7         8         9         0         1
// 012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
// ----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
// ID    TITLE                       SER START       END         VARNISH     ACTION      SIZE  SUB T V AA WWW CX NOTES
// ----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
// #378  [Pumpkins]                  STL s2025-08-09                         d2025-11-11 6x8   ACM            C2 Abandoned
// #379  Central Line Surfer         STL s2026-02-23 e2026-04-06 v2026-04-17             12x18 ACM Y C        C2 43h G:Still_Life
// #380  [Crafts]                    STL s2026-05-07 e2026-05-23                         12x18 ACM            C4 34h G:Still_Life
// #381  [Herring 2]                 WIL s2026-05-23 e2026-05-27                         7x17  S              C1 2h
// #382  [Herring 3]                 WIL s2026-05-23 e2026-05-27                         4x10  S              C1 2h
// #383  Coleoptera IV               WIL                                                                      C4
// #374  Mmmmm                       STL s2026-03-23 e2026-04-20 v2026-05-27             18x18 ACM Y C  Y     C3 71h G:Still_Life
//
// #S001 Juan de Pareja (Velasquez)  POR s2025-04-04 e2025-04-05 v2025-05-24             7x5   ACM Y C        C2 G:Study
// #S002 Mike Derby                  POR s2025-05-24 e2025-06-25 v2025-07-19 g2025-07-23 10x10 WP  Y W        C2 12h G:Study
void Painting::parse (const std::string& line)
{
  if (line.length () > 1)
    _id         = Lexer::trimRight (utf8_substr (line, 1, 4));
  else
    throw "Missing id to parse.";

  if (line.length () >  7)
    _title      = Lexer::trimRight (utf8_substr (line, 6, 28));
  else
    throw "Missing title to parse.";

  if (line.length () > 35)
    _series     = Lexer::trimRight (utf8_substr (line, 34, 3));

  if (line.length () > 39)
    _start      = Lexer::trimRight (utf8_substr (line, 38, 11));

  if (line.length () > 51)
    _end        = Lexer::trimRight (utf8_substr (line, 50, 11));

  if (line.length () > 63)
    _varnish    = Lexer::trimRight (utf8_substr (line, 62, 11));

  if (line.length () > 75)
    _action     = Lexer::trimRight (utf8_substr (line, 74, 11));

  if (line.length () > 87)
    _size       = Lexer::trimRight (utf8_substr (line, 86, 5));

  if (line.length () > 93)
    _substrate  = Lexer::trimRight (utf8_substr (line, 92, 3));

  if (line.length () > 97)
    _tagged     = Lexer::trimRight (utf8_substr (line, 96, 1));

  if (line.length () > 99)
    _varnished  = Lexer::trimRight (utf8_substr (line, 98, 1));

  if (line.length () > 101)
    _archived   = Lexer::trimRight (utf8_substr (line, 101, 1));

  if (line.length () > 106)
    _www        = Lexer::trimRight (utf8_substr (line, 105, 1));

  if (line.length () > 108)
    _complexity = Lexer::trimRight (utf8_substr (line, 107, 2));

  if (line.length () > 111)
    _notes      = Lexer::trimRight (utf8_substr (line, 110), " \n");

  _height = _width = 0;
  if (_size != "")
  {
    Pig pig (_size);
    if (! (pig.getDigits (_height) &&
        pig.skipLiteral ("x")      &&
        pig.getDigits (_width)     &&
        pig.eos ()))
    {
      warn (std::format ("Unparseable Painting size '{}', input '{}'", _size, line));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
