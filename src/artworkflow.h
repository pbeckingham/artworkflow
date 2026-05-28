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

#ifndef INCLUDED_ARTWORKFLOW
#define INCLUDED_ARTWORKFLOW

#include <Color.h>
#include <CLI.h>
#include <Database.h>
#include <Rules.h>
#include <Palette.h>

// debug.cpp
void enableDebugMode (bool);
void setDebugIndicator (const std::string&);
void setDebugColor (const Color&);
void debug (const std::string&);
void warn (const std::string&);

// helper.cpp
Color summaryIntervalColor (const Rules&, const std::set <std::string>&);
Color summaryIntervalColor (std::map <std::string, Color>&, const std::set <std::string>&);
Color chartIntervalColor (const std::set <std::string>&, const std::map <std::string, Color>&);
Color tagColor (const Rules&, const std::string&);
Palette createPalette (const Rules&);
int quantizeToNMinutes (int, int);

// init.cpp
bool lightweightVersionCheck (int, const char**);
void initializeEntities (CLI&);
void initializeDataAndRules (const CLI&, Database&, Rules&);
int dispatchCommand (CLI&, Database&, Rules&);

// util.cpp
std::string escape (const std::string&, int);
std::string quoteIfNeeded (const std::string&);
std::string join(const std::string& glue, const std::set <std::string>& array);
template <typename Container> std::string joinQuotedIfNeeded (const std::string&, const Container&);

#endif
