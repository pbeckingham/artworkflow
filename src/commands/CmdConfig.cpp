////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2018, 2020, 2022 - 2024, Gothenburg Bit Factory.
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

#include <JSON.h>
#include <Rules.h>
#include <commands.h>
#include <format.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// artworkflow config name value       Set name=value
// artworkflow config name ''          Set name=''
// artworkflow config name             Remove name
// artworkflow config                  Show all config
int CmdConfig (
  CLI& cli,
  Rules& rules)
{
  int rc = 0;

  // Get the command line args that are not binary, ext or command.
  auto words = cli.getWords ();

  // Support:
  //   artworkflow config name value    # set name to value
  //   artworkflow config name ""       # set name to blank
  //   artworkflow config name          # remove name
  if (words.empty ())
    return CmdShow (rules);

  const bool verbose = rules.getBoolean ("verbose");

  bool confirmation = rules.getBoolean ("confirmation");
  std::string name = words[0];
  std::string value;

  if (name.empty ()) // is this possible?
    return CmdShow (rules);

  bool change = false;

  // artworkflow config name value
  // artworkflow config name ""
  if (words.size () > 1)
  {
    // Join the remaining words into config variable's value
    for (unsigned int i = 1; i < words.size (); ++i)
    {
      if (i > 1)
        value += " ";

      value += words[i];
    }

    value = Lexer::dequote (value);

    change = Rules::setConfigVariable (rules, name, value, confirmation);

    if (! change)
      rc = 1;
  }
  // artworkflow config name
  else
  {
    bool found = false;
    rc = Rules::unsetConfigVariable (rules, name, confirmation);
    if (rc == 0)
    {
      change = true;
      found = true;
    }
    else if (rc == 1)
      found = true;

    if (! found)
      throw format ("No entry named '{1}' found.", name);
  }

  if (verbose)
    if (change)
      std::cout << "Config file '" << rules.file () << "' modified.\n";
    else
      std::cout << "No changes made.\n";

  return rc;
}

////////////////////////////////////////////////////////////////////////////////
