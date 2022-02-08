#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *p = malloc(1);
  p = calloc(2, 1);
  p = realloc(p, 3);
  p = reallocarray(p, 2, 2);

  p = malloc(100);
  memset(p, 42, 100);
  p = realloc(p, 50);
  if (p[10] != 42) printf("Realloc failed\n");
  p = realloc(p, 0);
  if (p != NULL) printf("Realloc failed to free\n");
  p = realloc(p, 2048);
  memset(p, 7, 2048);

  p = calloc(100000000000, 1000000000000);
  if (errno != ENOMEM) printf("calloc didnt set errno\n");
  if (p != NULL) printf("calloc didnt set p to null on overflow\n");
}