////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2017 - 2021, 2023, Gothenburg Bit Factory.
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

#include <Exhibition.h>
#include <test.h>

////////////////////////////////////////////////////////////////////////////////
int main (int, char**)
{
  UnitTest t (11);

  try
  {
    Exhibition e1 ("");
    t.fail ("Exhibition: Missing data not detected");
  }
  catch (const std::string& e) { t.is (e, "Missing id to parse.", "Exhibition: Missing id detected"); }

  try
  {
    Exhibition e2("2026.04                                                                                     \n");
    t.is (e2.title (), "", "Exhibition: title parsed");
  }
  catch (const std::string& e) { t.fail ("Exhibition: Parsing failure"); }

/*
                                                                                                    1         1
          1         2         3         4         5         6         7         8         9         0         1
012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

#ID     Deadline   Notify     Delivery   Open       Reception  Close      Pickup     Title
2026.04                       2026-08-06 2026-08-13 2026-08-13 2026-09-12 2026-09-16 Co|So, Resonance of the Ordinary
    #295 The Long Wave           12x24  V  F
    #330 The Last Toys           8x10   V  F
    #358 Generations             18x18  V  F
    #362 Choo Choo               6x18   V  F
*/

  std::string line = "2026.04                       2026-08-06 2026-08-13 2026-08-13 2026-09-12 2026-09-16 Co|So, Resonance of the Ordinary\n";
  Exhibition e3 (line);
  t.is (e3.id (),           "2026.04",                          "Exhibition: id");
  t.is (e3.submission (),   "",                                 "Exhibition: submission");
  t.is (e3.notification (), "",                                 "Exhibition: notification");
  t.is (e3.delivery (),     "2026-08-06",                       "Exhibition: delivery");
  t.is (e3.opening (),      "2026-08-13",                       "Exhibition: opening");
  t.is (e3.reception (),    "2026-08-13",                       "Exhibition: reception");
  t.is (e3.closing (),      "2026-09-12",                       "Exhibition: closing");
  t.is (e3.pickup (),       "2026-09-16",                       "Exhibition: pickup");
  t.is (e3.title (),        "Co|So, Resonance of the Ordinary", "Exhibition: title");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
