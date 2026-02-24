#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif
// Compile with -DLEAK to leak memory
#ifndef LEAK
#define LEAK 0
#endif
#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)
int
main (int argc, char **argv)
{
char *obj[OBJECTS];
int i, j, errors = 0;
// fill memory with objects
for (i = 0; i < OBJECTS; i++) {
obj[i] = malloc (OBJSIZE);
if (obj[i] == NULL) {
printf ("Unable to allocate object %d\n", i);
exit (EXIT_FAILURE);
}
}
// fill each object with distinct bytes
for (i = 0; i < OBJECTS; i++) {
memset (obj[i], i, OBJSIZE);
}
// check that all objects contain the correct bytes
for (i = 0; i < OBJECTS; i++) {
for (j = 0; j < OBJSIZE; j++) {
if (obj[i][j] != i) {
errors++;
printf ("Object %d byte %d incorrect: %d\n", i, j, obj[i]
[j]);
}
}
}
// free all objects
if (!LEAK) {
for (i = 0; i < OBJECTS; i++) {
free (obj[i]);
}
}
printf("%d incorrect bytes\n", errors);
return EXIT_SUCCESS;
}
