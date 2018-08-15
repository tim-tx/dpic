#include <stdlib.h>

/* Isdigit(c) { return isdigit(c); }
Isnamech(c) { return (isalnum(c) || (c==(int)'_')); }
Isupper(c) { return isupper(c); } */

unsigned long
/*
ordp(p) char **p; { return (p==NULL) ? (unsigned long) 0 : (unsigned long) *p; }
*/
ordp(p) char *p; { return (p==NULL) ? (long) 0 : (long) abs((long)p); }

/*
#include <stdio.h>
#include <string.h>
sprint( s,f,e )
char *s,*f; float *e;
{
char *sprintf();
   return( (int) sprintf( s,f,*e ) );
   } */
