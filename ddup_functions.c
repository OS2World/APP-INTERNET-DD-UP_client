/*
 makeconf.c - Tom Groves
 This is here to get the functions into one file
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ddup.h"

void chomp(char *);

void *safe_malloc(int n)
{
  char *mem = malloc(n);
  if (!mem) {
    fprintf(stderr, "no memory available\n");
     exit(1);
  }
  return mem;
}

void *safe_realloc(void *orig, int n)
{
  char *mem = realloc(orig, n);
  if (!mem) {
    fprintf(stderr, "no memory available\n");
    exit(1);
  }
  return mem;
}

void chomp(char *string)
{
  if (string && *string) {
    int pos = strlen(string) - 1;
    if (string[pos] == '\n')
      string[pos] = '\0';
    pos--;
    if (pos < 0)
      return;
    if (string[pos] == '\r')
      string[pos] = '\0';
  }
}

char *base64(char *src)
{
  static unsigned char alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  unsigned int bits;
  int i = 0;
  int j = 0;
  int k;
  int len;
  char *dst;

  len = strlen(src);
  dst = safe_malloc(((((len - 1) / 3) + 1) * 4) + 1 + len / 54);

  while (i < len) {
    if (i && i % 54 == 0)
      dst[j++] = '\n';

    bits = src[i++];
    for (k = 0; k < 2; k++) {
      bits <<= 8;
      if (i < len)
        bits += src[i++];
    }

    dst[j++] = alphabet[bits >> 18];
    dst[j++] = alphabet[(bits >> 12) & 0x3f];
    dst[j++] = alphabet[(bits >> 6) & 0x3f];
    dst[j++] = alphabet[bits & 0x3f];
  }

  switch (len % 3) {
  case 1:
    dst[j - 2] = '=';
  case 2:
    dst[j - 1] = '=';
    break;
  }
  dst[j] = '\0';

  return (dst);
}

void chkname(char *tochk)
{
  int count = 0;
  int x = 0;

  while(x<strlen(tochk)) {
    if(tochk[x] == '.')
      count++;
    x++;
  } 
  if(count < 2) {
    printf("Your hostname %s isn't long enough please try again.\n", tochk);
    exit(1);
  }
}
