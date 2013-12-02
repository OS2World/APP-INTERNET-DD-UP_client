/* Update Functions are here -- Thomas Gandy */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "ddup.h"

/* Flag set by `--proxy'. */
extern int proxy_flag;
/* Flag set by `--wildcard'. */
extern int wildcard_flag;
/* Flag set by `--debug'. */
extern int debug_flag;
/* Flag set by `--help'. */
extern int help_flag;
/* Flag set by `--makeconf'. Added by magick- */
extern int makeconf_flag;
/* Flag set by `--version'. */
extern int ver_flag;
/* Flag set by `--backmx'. */
extern int backmx_flag;
/* Flag set by '--static' (Added by Patrick D.) */
extern int static_flag;
/* Vars for other options */
extern char *ip_address;
extern char *host_id;
extern char *mx_addr;
extern char *proxy_server;
extern int proxy_port;
extern int mx_flag;


int make_connect(char * host, int port)
{
  struct hostent *ptrh;
  struct protoent *ptrp;
  struct sockaddr_in sad;
  int sd;

  if (proxy_flag) {
    host = proxy_server;
    if (proxy_port != 0)
      port = proxy_port;
  }
  memset((void *)&sad, 0, sizeof (sad));
  sad.sin_family = AF_INET;

  sad.sin_port = htons((u_short)port);

  ptrh = gethostbyname(host);

  if (((char *)ptrh) == NULL) {
    fprintf(stderr, "invalid host: %s\n", host);
    exit(1);
  }

  memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

  if (((int)(ptrp = getprotobyname("tcp"))) == 0) {
    fprintf(stderr,"cannot map \"tcp\" to protocol number\n");
    exit(1);
  }

  sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

  if (sd < 0) {
    fprintf(stderr, "Socket creation failed\n");
    exit(1);
  }

  if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr, "Connect failed\n");
    exit(1);
  }

  return (sd);
}

#define sock_write(sd, s)       (write((sd), (s), strlen(s)))

int update(char *auth64)
{
  char buf_in[1024];
  char buf_out[1024];
  int  response_size = 0;
  char *response;
  int response_bytes;
  int sd;
  int result;
  char * retval;
  char * tmpreturn;
  char * thisline;
  char * thishost;
  char hosts[256][20];
  char lines[256][20];

int i = 0;
int j = 0;
int numoflines = 0;
int numofhosts = 0;

  printf("Attempting to update %s\n", host_id);

  sd = make_connect(MEMBERS_HOST, 80);

  response_bytes = sizeof (buf_in);
  response = safe_malloc(response_bytes);


  strcpy(response, "");

  strcpy(buf_out, "GET ");

  if (proxy_flag)
  {
    strcat(buf_out, "http://");
    strcat(buf_out, MEMBERS_HOST);
  }

  strcat(buf_out, "/nic/update?system=");

  if (static_flag)
    strcat(buf_out, "stat");
  else
    strcat(buf_out, "dyn");

  strcat(buf_out, "dns&hostname=");
  strcat(buf_out, host_id);

  if (ip_address)
  {
    strcat(buf_out, "&myip=");
    strcat(buf_out, ip_address);
  }

  if (wildcard_flag)
  {
    strcat(buf_out, "&wildcard=ON");
  }

  if (mx_flag)
  {
    strcat(buf_out, "&mx=");
    strcat(buf_out, mx_addr);
    if (backmx_flag)
    {
      strcat(buf_out, "&backmx=YES");
    }
  }

  strcat(buf_out, " HTTP/1.0\r\n");

  strcat(buf_out, "Host: ");
  strcat(buf_out, MEMBERS_HOST);
  strcat(buf_out, "\r\nAuthorization: Basic ");
  strcat(buf_out, auth64);
  strcat(buf_out, "\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: ");
  strcat(buf_out, AGENT_NAME);
  strcat(buf_out, "/");
  strcat(buf_out, AGENT_VER);
  strcat(buf_out, "\r\n\r\n");

  sock_write(sd, buf_out);

  /* wait at most 60 seconds for the server to respond */
  alarm(60);

  do
  {
    memset(buf_in, 0, sizeof (buf_in));
    result = read(sd, buf_in, sizeof (buf_in));

    if (result > 0) {
      /* append the part of the page read */
      if (response_size + result > response_bytes) {
        response_bytes += sizeof (buf_in);
        response = safe_realloc(response, response_bytes);
      }
      memcpy(&response[response_size], buf_in, result);
      response_size += result;
      response[response_size] = '\0';

    }
  } while (result > 0);


   if (debug_flag)
   {
      printf("Sent:\n%s\n", buf_out);
      printf("\n\nResponse:\n%s\n", response);
   }

tmpreturn = response;

retval=strstr(tmpreturn,"\r\n\r\n");
if (retval != NULL)
{
 *retval='\0';
 retval+=2;
} else {
 printf("ERROR: Invalid response from web server\nAbnormal Termination of DD-UP\n");
 return 0;
}

i = 0;
thishost = strtok(host_id, ",");
while (thishost != NULL)
{
 strcpy(hosts[i], thishost);

 thishost = strtok(NULL, ",");
 i++;
}


i = 0;
thisline = strtok(retval, "\n");
while (thisline != NULL)
{
 if (strlen(thisline) > 2)
 {
        strcpy(lines[i], thisline);
        i++;
 }
 thisline = strtok(NULL, "\n");
}

j = i;
i = 0;


if (strstr(response, "Authorization Required"))
{
        printf("ERROR: Server returned Authorization Required.\nThis probably means that you supplied an invalid username or password.\n");
        return (0);
}

while (i < j)
{
 check_response(lines[i], hosts[i],response,buf_out);
 i++;
}

   close(sd);

   free(response);

   return (result <= 0);
}

void check_response (char * response, char * host_id, char * mailresp, char * mailreq)
{
        if (strstr(response, "good"))
                printf("Update of %s succeeded\n", host_id);
        else if (strstr(response, "nochg"))
                printf("Host %s not changed\n", host_id);
        else if (strstr(response, "notfqdn"))
                printf("Host %s is not a Fully-Qualified Domain Name\n", host_id);
        else if (strstr(response, "nohost"))
                printf("Host %s does not exist\n", host_id);
        else if (strstr(response, "!yours"))
                printf("Host %s does not belong to you\n", host_id);
        else if (strstr(response, "abuse"))
                printf("Host %s blocked due to abuse\n", host_id);
        else if (strstr(response, "numhost"))
                printf("Too many or too few hosts found\n", host_id);
        else if (strstr(response, "dnserr"))
                printf("DNS error encountered\n", host_id);
        else if (strstr(response, "Authorization Required"))
                printf("Invalid user credentials supplied\n", host_id);
        else
        {
                printf("Unknown error: Got response %s on host %s\n", response, host_id);
                send_mail (response, host_id, mailreq);
        }
}

void send_mail (char * response, char * host_id, char * sent_req)
{
        printf("\n\
There appears to be a problem.  Please feel free to contact DD-UP Support Team by sending an e-mail to : support@ddup.org.\n\
\n\
This will enter your query into a database and you will be contacted soon.\n\
\n");
}
