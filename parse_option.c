#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "getopt.h"

#include "ddup.h"

#ifndef lint
static char rcsid[]="$Id: parse_option.c,v 1.4 2000/02/18 02:08:39 tegandy Exp $";
#endif

/* Flag set by `--proxy'. */
int proxy_flag = 0;
/* Flag set by `--wildcard'. */
int wildcard_flag = 0;
/* Flag set by `--debug'. */
int debug_flag = 0;
/* Flag set by `--help'. */
int help_flag = 0;
/* Flag set by `--makeconf'. Added by magick- */
int makeconf_flag = 0;
/* Flag set by `--version'. */
int ver_flag = 0;
/* Flag set by `--backmx'. */
int backmx_flag = 0;
/* Flag set by '--static' (Added by Patrick D.) */
int static_flag = 0;
/* Vars for other options */
char *ip_address;
char *host_id;
char *mx_addr;
char *proxy_server;
int proxy_port = 0;
int mx_flag = 0;

void opt(int argc, char **argv)
{
  int c;
  static struct option long_options[] = {
    /* These options set a flag. */
    {"wildcard", 0, &wildcard_flag, 1},
    {"help", 0, &help_flag, 1},
    {"makeconf", 0, &makeconf_flag, 1},
    {"debug", 0, &debug_flag, 1},
    {"backmx", 0, &backmx_flag, 1},
    {"version", 0, &ver_flag, 1},
    /* Added by Patrick D. */
    {"static", 0, &static_flag, 1},
    /* These options don't set a flag.
       We distinguish them by their indices. */
    {"ip", 1, 0, 0},
    {"host", 1, 0, 0},
    {"mx", 1, 0, 0},
    {"proxyserv", 1, 0, 0},
    {"proxyport", 1, 0, 0},
    {0, 0, 0, 0} };
  /* getopt_long stores the option index here. */
  int option_index = 0;

  while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {

    switch (c) {
    case 0:
      /* If this option set a flag, do nothing else now. */
      if (long_options[option_index].flag != 0)
	break;
      if (!strcmp(long_options[option_index].name, "ip")) {
	ip_address = optarg;
	break;
      }
      if (!strcmp(long_options[option_index].name, "mx")) {
	mx_addr = optarg;
	break;
      }
      if (!strcmp(long_options[option_index].name, "host")) {
	host_id = optarg;
	break;
      }
      if (!strcmp(long_options[option_index].name, "proxyserv")) {
	proxy_server = optarg;
	break;
      }
      if (!strcmp(long_options[option_index].name, "proxyport")) {
	proxy_port = atoi(optarg);
	break;
      }
      break;
    case '?':
      /* getopt_long already printed an error message. */
      break;
    default:
      abort ();
    }
  }

  if (help_flag) {
    usage();
  }

  if (ver_flag) {
    printf("DD-UP version %s by Thomas Gandy (tegandy@enid.com)\n", AGENT_VER);
    exit(0);
  }

  if (makeconf_flag) {
    makeconf();
    exit(0);
  }

  if (proxy_server != NULL) {
    proxy_flag = 1;
  }

  if (host_id == NULL) {
    printf("--host is required to update.\n");
    usage();
  } else {
    chkname(host_id);
  }
	
  if (mx_addr != NULL)
    mx_flag = 1;
  
  /* Print any remaining command line arguments (not options). */
  if (optind < argc) {
    printf ("Unknown non-option ARGV-elements: ");
    while (optind < argc)
      printf ("%s ", argv[optind++]);
    printf("We will try to continue\n");
  } 
}
