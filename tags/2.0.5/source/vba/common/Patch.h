// -*- C++ -*-
// VisualBoyAdvance - Nintendo Gameboy/GameboyAdvance (TM) emulator.
// Copyright (C) 1999-2003 Forgotten
// Copyright (C) 2004 Forgotten and the VBA development team

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or(at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef VBA_PATCH_H
#define VBA_PATCH_H

#include "memfile.h"
#include "System.h"

bool applyPatch(const char *patchname, u8 **rom, int *size);
bool patchApplyIPS(MFILE * f, u8 **r, int *s);
bool patchApplyUPS(MFILE * f, u8 **rom, int *size);
bool patchApplyPPF(MFILE *f, u8 **rom, int *size);

#endif
