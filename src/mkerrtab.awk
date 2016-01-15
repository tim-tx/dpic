# Awk file to read PG.out and generate parserrtab
state == 2 {
   errline = (errline)$1" "
   errcount++
   if (errcount == nerrs) {
     printf("%c%s",length(errline)-1,substr(errline,1,length(errline)-1))
     errline = ""
     state = 1 }
   next
   }
state == 1 {
   if ($1==0) {exit}
   nerrs = $2; errcount = 0 ; errline = ""
   state = 2
   }
#$(NF) == "lxmax" {state = 1}
BEGIN {state = 1}
   {next}
