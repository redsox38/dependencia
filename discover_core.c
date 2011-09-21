/*
 *
 * Copyright (C) 2010 Todd Merritt <redsox38@gmail.com>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define SYSLOG_NAMES

#include "config.h"
#include <stdio.h>
#include <signal.h>

/* array of command line options */
static struct option long_options[] = {
  {"configfile", 1, 0, 'c'},
  {"interface", 1, 0, 'i'},
  {"readfile", 1, 0, 'r'},
  {0,0,0,0}
};

char *dev = NULL, *readfile = NULL, *configfile = NULL;

/* 
   local termination handler for this file 
   called by registered term handler after
   the registered termination handlers for the
   other components have been called
*/
void core_term_handler()
{
  if (dev)
    free(dev);
  
  if (readfile)
    free(readfile);

  if (configfile)
    free(configfile);

  unlink(PIDFILE);
  closelog();
  exit(0);
}

/* 
   registered signal handler
   calls termination handler function for
   all of the other files/libraries
*/
void term_handler(int signum)
{
  syslog(LOG_DEBUG, "Term handler fired");
  core_term_handler();
}

/* display help message */
void usage() 
{
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "\t-c, --configfile <configuration file>\n");
  fprintf(stderr, "\t-i, --interface <interface for packet capture>\n");
  fprintf(stderr, "\t-r, --readfile <packet capture file>\n");
}

int main(int argc, char *argv[]) {
  char               *facil_str;
  int                facil = -1, c, option_index;
  CODE               *cs;
  extern char        *optarg;
  extern int         optind, optopt, opterr;

  /* process command line */

  while ((c = getopt_long(argc, argv, "i:r:c:", long_options, 
                          &option_index)) != -1) {
    switch (c) {
    case 'c':
      configfile = strdup(optarg);
      break;
    case 'i':
      dev = strdup(optarg);
      break;
    case 'r':
      readfile = strdup(optarg);
      break;
    case '?':
      usage();
      exit(-1);
      break;
    }
  }

  /* install signal handler(s) */
  if (signal(SIGTERM, term_handler) == SIG_IGN)
    signal(SIGTERM, SIG_IGN);

  /* open syslog */
  facil_str = "local7";
  for(cs = facilitynames; cs->c_name; cs++) {
    if (!(strcmp(facil_str, cs->c_name))) {
      facil = cs->c_val;
      break;
    }
  }

  if (facil < 0) {
    fprintf(stderr, "Invalid syslog facility");
    exit(-1);
  }

  openlog("dependencia", LOG_PID, facil);

  /* invoke term handler */
  kill(0, SIGTERM);
}
