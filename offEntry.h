/***************************************************************************
 *   Copyright (C) 2019 by John D. Robertson                               *
 *   john@rrci.com                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef OFFENTRY_H
#define OFFENTRY_H

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <zlib.h>

#include "map.h"
#include "pdns.h"

/* One of these for each offense found in a log file */
typedef struct _OFFENTRY {

   unsigned count,
            severity;

   time_t latest;

   char addr[46],
        cntry[3];

   /* This data populated by PDNS_lookup() */
   struct {
      enum PDNS_flags flags;
      char *name;
   } dns;
   
} OFFENTRY;

#if 0
struct OFFENTRY_cacheWrite {
   FILE *fh;
   DB *dbh;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define OFFENTRY_addr_create(p, addr) \
  ((p)=(OFFENTRY_addr_constructor((p)=malloc(sizeof(OFFENTRY)), addr) ? (p) : ( p ? realloc(OFFENTRY_destructor(p),0) : 0 )))
OFFENTRY*
OFFENTRY_addr_constructor(OFFENTRY *self, const char *addr);
/********************************************************
 * Prepare for use from an address found in a log entry.
 */

#define OFFENTRY_cache_create(p, cacheFileEntry) \
  ((p)=(OFFENTRY_cache_constructor((p)=malloc(sizeof(OFFENTRY)), cacheFileEntry) ? (p) : ( p ? realloc(OFFENTRY_destructor(p),0) : 0 )))
OFFENTRY*
OFFENTRY_cache_constructor(OFFENTRY *self, const char *cacheFileEntry);
/********************************************************
 * Prepare for use with entry from cache file.
 */

#define OFFENTRY_destroy(s) \
  {if(OFFENTRY_destructor(s)) {free(s); (s)=0;}}
void*
OFFENTRY_destructor(OFFENTRY *self);
/********************************************************
 * Free resources.
 */

void
OFFENTRY_register(OFFENTRY *self, unsigned severity, time_t when);
/********************************************************
 * Register the current failure try.
 */

int
OFFENTRY_cacheWrite(OFFENTRY *self, FILE *fh);
/********************************************************
 * Write to the cache file in a form we can read later.
 */

int
OFFENTRY_print(OFFENTRY *self, FILE *fh);
/********************************************************
 * Print a human readable representation of *self.
 */

int
OFFENTRY_list(OFFENTRY *self, FILE *fh, int flags, unsigned nAllowed);
/********************************************************
 * Print in listing form
 */

int
OFFENTRY_map_addr(OFFENTRY *self, MAP *h_rtnMap);
/********************************************************
 * Create a map of OFFENTRY objects with composite
 * counts by address.
 */

int
OFFENTRY_offenseCount(OFFENTRY *self, unsigned *h_sum);
/********************************************************
 * Get a count of all offenses for this entry.
 */

#ifdef __cplusplus
}
#endif

#endif

