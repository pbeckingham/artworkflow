////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2026, Paul Beckingham.
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

#include <unistd.h>
#include <sys/ioctl.h>
#include <string>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
static int terminal_width {0};
static int terminal_height {0};

////////////////////////////////////////////////////////////////////////////////
// Escape all 'c' --> '\c'.
std::string escape (const std::string& input, int c)
{
  std::string output;

  auto last = input.begin ();
  for (auto i = input.begin (); i != input.end (); ++i)
  {
    if (*i == c)
    {
      output.append (last, i);
      output += std::string ("\\") + *i;
      last = i + 1;
    }

    // Default NOP.
  }

  output.append (last, input.end ());
  return output;
}

////////////////////////////////////////////////////////////////////////////////
std::string quoteIfNeeded (const std::string& input)
{
  auto quote = input.find ('"');
  auto space = input.find (' ');
  auto op    = input.find_first_of ("-+/()<^!=~_%");

  if (quote == std::string::npos &&
      space == std::string::npos &&
      op    == std::string::npos)
    return input;

  std::string output;
  if (quote != std::string::npos)
    output = escape (input, '"');
  else
    output = input;

  return std::string ("\"") + output + "\"";
}

////////////////////////////////////////////////////////////////////////////////
std::string join (const std::string& glue, const std::set <std::string>& array)
{
  if (array.empty ())
    return "";

  auto iterator = array.begin ();
  std::string value = *iterator++;
  while (iterator != array.end ())
    value += glue + *iterator++;

  return value;
}

////////////////////////////////////////////////////////////////////////////////
template <typename Container>
std::string joinQuotedIfNeeded (const std::string& glue, const Container& container)
{
  if (container.empty ())
    return "";

  auto iterator = container.begin ();
  std::string value = quoteIfNeeded (*iterator++);
  while (iterator != container.end ())
    value += glue + quoteIfNeeded (*iterator++);

  return value;
}

template std::string joinQuotedIfNeeded(const std::string&, const std::set <std::string>&);

template std::string joinQuotedIfNeeded(const std::string&, const std::vector <std::string>&);

////////////////////////////////////////////////////////////////////////////////
int get_terminal_width ()
{
  if (terminal_width == 0 &&
      terminal_height == 0)
  {
    terminal_width = 80;
    terminal_height = 24;

#ifdef TIOCGSIZE
    struct ttysize ts{};
    ioctl (STDIN_FILENO, TIOCGSIZE, &ts);
    terminal_width  = ts.ts_cols;
    terminal_height = ts.ts_lines;
#elif defined(TIOCGWINSZ)
    struct winsize ts {};
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    terminal_width  = ts.ws_col;
    terminal_height = ts.ws_row;
#endif
  }

  return terminal_width;
}

////////////////////////////////////////////////////////////////////////////////
int get_terminal_height()
{
  (void) get_terminal_width ();
  return terminal_height;
}

////////////////////////////////////////////////////////////////////////////////
Palette createPalette (const Config& config)
{
  Palette p;
  // TODO: Provide a config value iterator.
/*
  auto colors = config.all ("theme.palette.color");

  if (! colors.empty ())
  {
    p.clear ();
    for (auto& c : colors)
      p.add (Color (config.get (c)));
  }

  p.enabled = config.getBoolean ("color");
*/
  return p;
}

////////////////////////////////////////////////////////////////////////////////
