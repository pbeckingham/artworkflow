////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2023, Thomas Lauf, Paul Beckingham, Federico Hernandez.
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

#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <Datetime.h>
#include <Duration.h>
#include <Table.h>
#include <format.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
// Select a color to represent the interval in a summary report.
Color summaryIntervalColor (
  const Rules& rules,
  const std::set <std::string>& tags)
{
  Color c;

  for (auto& tag : tags)
  {
    c.blend (tagColor (rules, tag));
  }

  return c;
}

////////////////////////////////////////////////////////////////////////////////
Color summaryIntervalColor (
  std::map <std::string, Color>& tagColors,
  const std::set <std::string>& tags)
{
  Color c;

  for (const auto& tag : tags)
  {
    c.blend (tagColors[tag]);
  }

  return c;
}

////////////////////////////////////////////////////////////////////////////////
// Select a color to represent the interval on a chart.
Color chartIntervalColor (
  const std::set <std::string>& tags,
  const std::map <std::string, Color>& tag_colors)
{
  if (tags.empty ())
  {
    return tag_colors.at ("");
  }

  Color c;

  for (auto& tag : tags)
  {
    c.blend (tag_colors.at (tag));
  }

  return c;
}

////////////////////////////////////////////////////////////////////////////////
// Consult rules to find any defined color for the given tag, and colorize it.
Color tagColor (const Rules& rules, const std::string& tag)
{
  Color c;
  std::string name = std::string ("tags.") + tag + ".color";
  if (rules.has (name))
  {
    c = Color (rules.get (name));
  }

  return c;
}

////////////////////////////////////////////////////////////////////////////////
Palette createPalette (const Rules& rules)
{
  Palette p;
  auto colors = rules.all ("theme.palette.color");

  if (! colors.empty ())
  {
    p.clear ();
    for (auto& c : colors)
    {
      p.add (Color (rules.get (c)));
    }
  }

  p.enabled = rules.getBoolean ("color");
  return p;
}

////////////////////////////////////////////////////////////////////////////////
int quantizeToNMinutes (const int minutes, const int N)
{
  if (minutes % N == 0)
  {
    return minutes;
  }

  auto deviation = minutes % N;
  if (deviation < N / 2)
  {
    return minutes - deviation;
  }

  return minutes + N - deviation;
}

////////////////////////////////////////////////////////////////////////////////
bool findHint (const CLI& cli, const std::string& hint)
{
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("HINT") &&
        arg.getToken () == hint)
    {
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
std::string minimalDelta (const Datetime& left, const Datetime& right)
{
  std::string result = right.toISOLocalExtended ();

  if (left.sameYear (right))
  {
    result.replace (0, 5, "     ");
    if (left.sameMonth (right))
    {
      result.replace (5, 3, "   ");
      if (left.sameDay (right))
      {
        result.replace (8, 3, "   ");
        if (left.sameHour (right))
        {
          result.replace (11, 3, "   ");
          if (left.minute () == right.minute ())
          {
            result.replace (14, 3, "   ");
          }
        }
      }
    }
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////
int getTerminalWidth ()
{
  int terminalWidth;
#ifdef TIOCGSIZE
  struct ttysize ts{};
  ioctl (STDIN_FILENO, TIOCGSIZE, &ts);
  terminalWidth = ts.ts_cols;
#elif defined(TIOCGWINSZ)
  struct winsize ts {};
  ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
  terminalWidth = ts.ws_col;
#endif

  if (terminalWidth == 0)
  {
    char *columns = getenv ("COLUMNS");
    if (columns != NULL)
    {
      terminalWidth = atoi (columns);
    }
  }

  return terminalWidth > 0 ? terminalWidth : 80;
}

////////////////////////////////////////////////////////////////////////////////
