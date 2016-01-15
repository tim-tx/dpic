# This converts the raw lexical table into C include files
BEGIN { 
   ORDMAXCH = 255
   print "" >  "entryhp.h"
   print "" >  "entrytv.h"
   print "" >  "lxnp.h"
   print "" >  "lxhp.h"
   print "" >  "lxtv.h"
   print "" >  "lxch.h"
   for (i=0; i<=ORDMAXCH; i++){
      entryhp[sprintf("%c",i)] = 0*0
      entrytv[sprintf("%c",i)] = 0*0
      }
   cc = "x" 
   lxmax = 0 }

NR == 1 { 
   cc = substr($1,1,1); hp = $2; tv = $3
   next }

ch1 != cc {
   ch1 = cc
   entryhp[cc] = hp*1
   entrytv[cc] = tv*1
   cc = substr($1,1,1); hp = $2; tv = $3
   next
   }

$0 !~ /^ *$/ {
   lxch[lxmax] = substr($1,1,1)
   lxnp[lxmax] = substr($1,2)*1
   lxhp[lxmax] = $2*1
   lxtv[lxmax] = $3*1
   lxmax++
   }

END {
   lxmax--
   for (i=0; i<ORDMAXCH; i++){
      printf("%d,\n", entryhp[sprintf("%c",i)]) >> "entryhp.h"
      printf("%d,\n", entrytv[sprintf("%c",i)]) >> "entrytv.h"
      }
   printf("%d\n", entryhp[sprintf("%c",ORDMAXCH)]) >> "entryhp.h"
   printf("%d\n", entrytv[sprintf("%c",ORDMAXCH)]) >> "entrytv.h"

   for (i=0; i<lxmax; i++) {
      printf("'%s',\n",lxch[i]) >> "lxch.h"
      printf("%d,\n",lxnp[i]) >> "lxnp.h"
      printf("%d,\n",lxhp[i]) >> "lxhp.h"
      printf("%d,\n",lxtv[i]) >> "lxtv.h"
      }
   printf("'%s'\n",lxch[lxmax]) >> "lxch.h"
   printf("%d\n",lxnp[lxmax]) >> "lxnp.h"
   printf("%d\n",lxhp[lxmax]) >> "lxhp.h"
   printf("%d\n",lxtv[lxmax]) >> "lxtv.h"
   }
