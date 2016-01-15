# Awk file to read PG.out and generate a compact parserr.i
state == 2 {
   if (substr($1,1,1)=="'") printf(" '%s", $1 )
   else printf(" %s", $1 )
   count++
   if (count == nerrs) {print "');" ; state = 1}
   next
   }
state == 1 {
   errno = $1; nerrs = $2; count = 0
   if (errno==0) {exit}
   printf(" %d: writeln(errout,'", errno)
   state = 2
   }
$(NF) == "lxmax" {state = 1}
   {next}
