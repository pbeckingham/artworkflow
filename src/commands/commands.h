////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2018, 2022 - 2025, Gothenburg Bit Factory.
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

#ifndef INCLUDED_COMMANDS
#define INCLUDED_COMMANDS

#include <CLI.h>
#include <Database.h>
#include <Config.h>

//  CmdXxx           (CLI&, Config&, Database&);
int CmdAll           (CLI&, Config&, Database&);
int CmdCheck         (CLI&, Config&, Database&);
int CmdDiagnostics   (      Config&           );
int CmdDisposition   (CLI&, Config&, Database&);
int CmdGet           (CLI&, Config&, Database&);
int CmdHelp          (CLI&                    );
int CmdInfo          (CLI&,          Database&);
int CmdKanban        (CLI&, Config&, Database&);
int CmdMetrics       (CLI&, Config&, Database&);
int CmdNew           (CLI&, Config&, Database&);
int CmdPrice         (CLI&, Config&, Database&);
int CmdSales         (CLI&, Config&, Database&);
int CmdSeries        (CLI&, Config&, Database&);
int CmdVersion       (                        );
int CmdDefault       (CLI&, Config&, Database&);

#endif
