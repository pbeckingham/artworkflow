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

#include <algorithm>
#include <commands.h>
#include <iostream>
#include <shared.h>

////////////////////////////////////////////////////////////////////////////////
int CmdHelpUsage ()
{
  std::cout << '\n'
            << "Usage: artworkflow [--version]\n"
            << "       artworkflow diagnostics\n"
            << "       artworkflow get <DOM> [<DOM> ...]\n"
            << "       artworkflow help [<command>]\n"
            << "       artworkflow info <id>\n"
            << '\n'
            << "Hints:\n"
            << "       :debug       Debug mode, showing all processing\n"
            << "       :quiet       Minimum feedback\n"
            << "       :color       Use color always\n"
            << "       :nocolor     Do not use color\n"
            << "       :yes         Override confirmation requests\n"
            << '\n'
            << "DOM References:\n"
            << "       dom.<id>.<meta>\n"
            << "       dom.<id>.[start|end|varnish|action].[year|month|day|age]\n"
            << "       dom.[all|inventory|sold|gifted|abandoned|destroyed|wip].[ids|count]\n"
            << '\n';

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int CmdHelp (CLI& cli)
{
  auto words = cli.getWords ();

  if (! words.empty ())
  {
    std::string man_command = "man artworkflow-" + words[0];
    int ret = system (man_command.c_str());
    return (WIFEXITED (ret)) ? WEXITSTATUS (ret) : -1;
  }

  return CmdHelpUsage ();
}

////////////////////////////////////////////////////////////////////////////////
