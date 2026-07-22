////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2018, 2020 - 2025, Gothenburg Bit Factory.
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

#include <commands.h>
#include <format.h>
#include <iostream>
#include <shared.h>
#include <artworkflow.h>

////////////////////////////////////////////////////////////////////////////////
// Identify DOM references in CLI, provide space-separated results.
int CmdInfo (
  CLI& cli,
  Database& database)
{
  for (auto& painting : database.allPaintings ())
  {
    if (filterByCLI (cli, painting))
    {
      for (auto& line : painting.card (60))
        std::cout << line << '\n';

       std::cout << '\n';
    }
  }

  // TODO: Upgrade to new filtering mechanism.
  for (auto& id : cli.getIds ())
  {
    if (Exhibition::is_exhibition (id))
    {
      for (auto& exhibition : database.allExhibitions ())
      {
        if (exhibition.matches (id))
        {
          std::cout << exhibition.id () << " " << exhibition.title () << '\n';
          if (exhibition.submission () != "")
            std::cout << "  Submission   " << exhibition.submission () << '\n';
          if (exhibition.notification () != "")
            std::cout << "  Notification " << exhibition.notification () << '\n';
          if (exhibition.delivery () != "")
            std::cout << "  Delivery     " << exhibition.delivery () << '\n';
          if (exhibition.opening () != "")
            std::cout << "  Opening      " << exhibition.opening () << '\n';
          if (exhibition.reception () != "")
            std::cout << "  Reception    " << exhibition.reception () << '\n';
          if (exhibition.closing () != "")
            std::cout << "  Closing      " << exhibition.closing () << '\n';
          if (exhibition.pickup () != "")
            std::cout << "  Pickup       " << exhibition.pickup () << '\n';

          std::cout << '\n';
        }
      }
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
