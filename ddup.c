#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ddup.h"

char * auth64;
char * auth;

int main(int argc, char *argv[])
{
  int status;
  char *username;
  char *pass;

  opt(argc, argv);

  read_config(&auth);

  status = update(auth);
   
  exit(status);
}

void usage(void)
{
  printf("DD-UP version %s by Thomas Gandy and Tom Groves (ddupwww@ddup.org)\n", AGENT_VER);
  printf("Usage:  ddup --host host.domain.ext (multiple hosts separated by ,)\n");
  printf("        (host.domain.ext,host2.domain.ext) [--ip address] [--wildcard]\n");
  printf("        [--proxyserv server] [--proxyport port] [--help] [--debug] [--mx mxhost]\n");
  printf("        [--backmx] [--static]\n");
  printf("        --host host.domain.ext : REQUIRED to update.\n"); 
  printf("                                 Where host.domain.ext is your domain.\n"); 
  printf("                                 eg. gandy.dyndns.org, gandy.ath.cx\n");
  printf("        --ip address : Forces manual ip specification\n");
  printf("                       Where address is your IP address.\n");
  printf("        --wildcard : Enables wildcards.\n");
  printf("	--mx mxhost : Sets your MX to mxhost.\n");
  printf("	--backmx : Needs --mx and sets mxhost to be a backup MX.\n");
  printf("        --proxyserv server : Uses server as proxy.\n");
  printf("        --proxyport port : connects to proxy with port.\n");
  printf("        --help : Prints this message.\n");
  printf("	--debug : Prints debug output of what is sent to server.\n");
  printf("	--static : Use the Static DNS service. (Added by Patrick D.)\n");
  printf("\n");
  printf("	--makeconf : Creates the config file for you. (Base64 Encodes your password)\n");
  exit(0);
}

void makeconf(void)
{
  FILE * fp; /* file pointer */
  char * mc_user = (char*)malloc(sizeof(char)*100); /* NIC username */
  char * mc_pass = (char*)malloc(sizeof(char)*100);  /* NIC passowrd */
  char mc_allowed[256]; /* allowed users */
  char * running; /* temporary pointer */
  char * pt; /* temporary pointer number 2 */
   
    if (fp = fopen(file_name, "w")) {
      printf("Please enter your user name:\n");
      fgets(mc_user, 99, stdin);

      printf("Please enter your password:\n");
      fgets(mc_pass, 99, stdin);

      auth = safe_malloc(strlen(mc_user) + strlen(mc_pass) + 2);
      chomp(mc_user);
      chomp(mc_pass);
      sprintf(auth, "%s:%s", mc_user, mc_pass);

      printf("%s\n", auth);
      auth64 = base64(auth);
      fprintf(fp, "auth=%s\n", auth64);

#ifndef WIN32
      printf("OK..now you need to enter the users who can use ddup:\n");
      printf("Seperate the users with a , ie. root,test,this,that\n");

      fgets(mc_allowed, 99, stdin);
      running = mc_allowed;
  
      pt = strtok(running, ",");
      while(pt)                  /* quit on NULL */
      {
        fprintf(fp, "allowed_user=%s\n",pt);
        pt = strtok(NULL, ",");     /* next token */
      }  
#endif
      fclose(fp);
    } else {
      printf("You don't have permissions to write %s\n", file_name);
      exit(1);
    }
}
