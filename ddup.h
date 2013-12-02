
#ifndef _DDUP_H_
#define _DDUP_H_

#include "options.h"

#define AGENT_NAME "ddup"
#define AGENT_VER "3.0.1"
#define MEMBERS_HOST "members.dyndns.org"

/* ddup_functions.c */
extern char * base64(char *src);
extern void chkname(char *tochk);

/* ddup.c */
extern void usage(void);

/* parse_config.c */
extern void read_config(char **auth_ptr);

/* parse_option.c */
extern void opt(int argc, char **argv);
extern int update(char *auth64);
extern void *safe_malloc(int n);
extern void *safe_realloc(void *orig, int n);
extern void chomp(char *);

/* update.c */
extern int update(char *auth64);
void send_mail (char * response, char * host_id, char * sent_req);
int make_connect(char * host, int port);
void check_response (char * response, char * host_id, char * mailresp, char * mailreq);
#endif /* _DDUP_H_ */
