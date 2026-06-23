////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2026 Paul Beckingham.
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

#ifndef INCLUDED_CONFIG
#define INCLUDED_CONFIG

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <map>
#include <string>

class Config
{
public:
  Config (sol::state&);
  void load (const std::string&);

  bool        has        (const std::string&) const;
  std::string get (const std::string&, const std::string& = "") const;
  int         getInteger (const std::string&, int = 0) const;
  double      getReal    (const std::string&) const;
  bool        getBoolean (const std::string&, bool = false) const;

  void set (const std::string&, int);
  void set (const std::string&, double);
  void set (const std::string&, const std::string&);

private:
  std::string _original_file {};
  sol::state* _lua {NULL};
};

#endif
