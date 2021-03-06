/* gzio.c -- IO on .gz files
 * Copyright (C) 1995-2002 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
 *
 * Compile this file with -DNO_DEFLATE to avoid the compression code.
 */

/* memgzio.c - IO on .gz files in memory
 * Adapted from original gzio.c from zlib library by Forgotten
 */
#ifndef HAVE_ZUTIL_H
#include "../win32/include/zlib/zutil.h"
#else
#include <zlib.h>
#endif

#ifndef local
#  define local static
#endif

#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif

#define OS_CODE  0x03  /* assume Unix */
#define zmemcpy memcpy

gzFile ZEXPORT memgzopen(char *memory, int, const char *);
int ZEXPORT memgzread(gzFile, voidp, unsigned);
int ZEXPORT memgzwrite(gzFile, const voidp, unsigned);
int ZEXPORT memgzclose(gzFile);
long ZEXPORT memtell(gzFile);
