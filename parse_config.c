#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>

#include "ddup.h"

#ifndef lint
static char rcsid[] = "$Id: parse_config.c,v 1.3 2000/02/18 02:08:39 tegandy Exp $";
#endif

static void allowed(int);
static int allowed_uids[100];

void read_config(char **auth_ptr)
{
  FILE *fptr;
  char line[1024];
  char *string;
  int x = 0;
  struct passwd *my_passwd;

  fptr = fopen(file_name, "r");
  if(fptr == NULL) {
    perror("Failed to open config file");
    exit(1);
  }
  while (fgets(line, sizeof (line), fptr) != NULL) {
    if (line[0] == '#') { 
      continue;
    }
    if (strncmp(line, "auth", 4) == 0 ) {
      string = strchr(line,'=');
      string += 1;
      chomp(string);
      *auth_ptr = safe_malloc(strlen(string) + 1);
      strcpy(*auth_ptr, string);
    }

    if (strncmp(line, "allowed_user", 12) == 0) {
      string = strchr(line,'=');
      string += 1;
      chomp(string);
      my_passwd = getpwnam(string);
      if (my_passwd != NULL)
	allowed_uids[x++] = my_passwd->pw_uid;
    }
  }
  fclose(fptr);
  allowed (x);
}
	
static void allowed(int read_uids)
{
  int my_uid = geteuid();
  int j = 0;
  int status = 0;
	
  for (j = 0; j < read_uids; j++) {
    if (allowed_uids[j] == my_uid) {
      status = 1;
      break;
    }
  }
#ifndef WIN32
  if (!status) {
    printf("Permission denied.\n");
    printf("If this is wrong contact your sys-admin\n");
    exit (0);
  }
#endif
}
