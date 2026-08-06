////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2017, 2019 - 2021, 2023, Gothenburg Bit Factory.
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

#include <Palette.h>

////////////////////////////////////////////////////////////////////////////////
// Use a default palette, which is overwritten in ::initialize.
Palette::Palette ()
{
  _colors = {
    // Flat UI Colors
    Color ("0x000000 on 0xF8C82D"),
    Color ("0x000000 on 0xFBCF61"),
    Color ("0x000000 on 0xFF6F6F"),
    Color ("0x000000 on 0xE3A712"),
    Color ("0x000000 on 0xE5BA5A"),
    Color ("0x000000 on 0xD1404A"),
    Color ("0x000000 on 0x0DCCC0"),
    Color ("0x000000 on 0xA8D164"),
    Color ("0x000000 on 0x3498DB"),
    Color ("0x000000 on 0x0EAD9A"),
    Color ("0x000000 on 0x27AE60"),
    Color ("0xFFFFFF on 0x2980B9"),
    Color ("0x000000 on 0xD49E99"),
    Color ("0xFFFFFF on 0xB23F73"),
    Color ("0xFFFFFF on 0x48647C"),
    Color ("0xFFFFFF on 0x74525F"),
    Color ("0xFFFFFF on 0x832D51"),
    Color ("0xFFFFFF on 0x2C3E50"),
    Color ("0x000000 on 0xE84B3A"),
    Color ("0x000000 on 0xFE7C60"),
    Color ("0x000000 on 0xECF0F1"),
    Color ("0xFFFFFF on 0xC0392B"),
    Color ("0xFFFFFF on 0x404148"),
    Color ("0x000000 on 0xBDC3C7"),

  /*
    // Autumn Luxury
    Color ("0xffffff on 0x007571"),
    Color ("0x000000 on 0x10C2BB"),
    Color ("0xffffff on 0xC24D0F"),
    Color ("0x000000 on 0xFF772E"),
    Color ("0xffffff on 0x752C05"),

    // Rosy Glow
    Color ("0xffffff on 0xdd614a"),
    Color ("0xffffff on 0xf48668"),
    Color ("0x000000 on 0xf4a698"),
    Color ("0x000000 on 0xc5c392"),
    Color ("0xffffff on 0x73a580"),

    // Citrus Burst
    Color ("0xffffff on 0x8ea604"),
    Color ("0x000000 on 0xf5bb00"),
    Color ("0x000000 on 0xec9f05"),
    Color ("0xffffff on 0xd76a03"),
    Color ("0xffffff on 0xbf3100"),

    // Summer Splash
    Color ("0xffffff on 0x086788"),
    Color ("0xffffff on 0x07a0c3"),
    Color ("0x000000 on 0xf0c808"),
    Color ("0x000000 on 0xfff1d0"),
    Color ("0x000000 on 0xdd1c1a"),

    // Tranquil Earth
    Color ("0xffffff on 0x73877b"),
    Color ("0xffffff on 0x839788"),
    Color ("0x000000 on 0xbdbbb6"),
    Color ("0x000000 on 0xe5d1d0"),
    Color ("0x000000 on 0xf534d7"),

    // 16 color
    Color ("white on red"),
    Color ("white on blue"),
    Color ("black on green"),
    Color ("black on magenta"),
    Color ("black on cyan"),
    Color ("black on yellow"),
    Color ("black on white"),
    Color ("white on bright red"),
    Color ("white on bright blue"),
    Color ("black on bright green"),
    Color ("black on bright magenta"),
    Color ("black on bright cyan"),
    Color ("black on bright yellow"),
*/
  };

  _current = 0;
}

////////////////////////////////////////////////////////////////////////////////
void Palette::add (Color c)
{
  _colors.push_back (c);
}

////////////////////////////////////////////////////////////////////////////////
// Return the next color in the list.  Cycle to the beginning if necessary.
Color Palette::next ()
{
  if (enabled)
    return _colors[_current++ % _colors.size ()];

  return Color ();
}

////////////////////////////////////////////////////////////////////////////////
int Palette::size () const
{
  return static_cast <int> (_colors.size ());
}

////////////////////////////////////////////////////////////////////////////////
void Palette::clear ()
{
  _colors.clear ();
  _current = 0;
}

////////////////////////////////////////////////////////////////////////////////
