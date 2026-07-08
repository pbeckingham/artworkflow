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

#include <Painting.h>
#include <test.h>

////////////////////////////////////////////////////////////////////////////////
int main (int, char**)
{
  UnitTest t (17);

  try
  {
    Painting p1 ("");
    t.fail ("Painting: Missing data not detected");
  }
  catch (const std::string& e) { t.is (e, "Missing id to parse.", "Painting: Missing id detected"); }

  try
  {
    Painting p2("#123  Title                                       ");
    t.is (p2.title (), "Title", "Painting: title parsed");
  }
  catch (const std::string& e) { t.fail ("Painting: Parsing failure"); }

/*
                                                                                                    1         1
          1         2         3         4         5         6         7         8         9         0         1
012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
ID    TITLE                       SER START       END         VARNISH     ACTION      SIZE  SUB T V AA WWW CX NOTES
----- --------------------------- --- ----------- ----------- ----------- ----------- ----- --- - - -- --- -- -----
#S001 Juan de Pareja (Velasquez)  POR s2025-04-04 e2025-04-05 v2025-05-24             7x5   ACM Y C        C2 G:Study
#S002 Mike Derby                  POR s2025-05-24 e2025-06-25 v2025-07-19 g2025-07-23 10x10 WP  Y W        C2 12h G:Study
*/

  std::string line = "#S001 Juan de Pareja (Velasquez)  POR s2025-04-04 e2025-04-05 v2025-05-24             7x5   ACM Y C        C2 24h G:Study\n";
  Painting p3 (line);
  t.is (p3.id (),         "S001",                       "Painting: id");
  t.is (p3.title (),      "Juan de Pareja (Velasquez)", "Painting: title");
  t.is (p3.series (),     "POR",                        "Painting: series");
  t.is (p3.start (),      "s2025-04-04",                "Painting: start");
  t.is (p3.end (),        "e2025-04-05",                "Painting: end");
  t.is (p3.varnish (),    "v2025-05-24",                "Painting: varnish");
  t.is (p3.action (),     "",                           "Painting: action");
  t.is (p3.size (),       "7x5",                        "Painting: size");
  t.is (p3.substrate (),  "ACM",                        "Painting: substrate");
  t.is (p3.tagged (),     "Y",                          "Painting: tagged");
  t.is (p3.varnished (),  "C",                          "Painting: varnished");
  t.is (p3.archived (),   "",                           "Painting: archived");
  t.is (p3.www (),        "",                           "Painting: www");
  t.is (p3.complexity (), "C2",                         "Painting: complexity");
  t.is (p3.notes (),      "24h G:Study",                "Painting: notes");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
