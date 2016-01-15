(* sysdep.x Required UNIX functions *)
(*F{$linklib c} F*)

function  access( var f:mstring; n:integer ): integer; (*F cdecl; F*)
   external (*HF name 'access' FH*)(*G; asmname '_p_Access' G*);
function  isatty( n:integer ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'isatty' FHG*);
function time( var t: integer ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'time' FHG*);
procedure sprintf( var s,f:char;
            e: (*M double M*)(*GHF real FHG*) ); (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'sprintf' FHG*);
function  snprintf( var s:char; n:integer; var f:char;
            e: (*M double M*)(*GHF real FHG*) ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'snprintf' FHG*);
function system( var s: char ): integer; (*F cdecl; F*)
(* procedure system( var s: char ); *)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'system' FHG*);

(* These need tweaking for different operating systems:      *)
function  random(*M () M*): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'random' FHG*);

(*P2CIP*) procedure srandom(s: integer); (*F cdecl; F*)
(*P2CP function srandom(s: integer): integer; *)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'srandom' FHG*);

(*-----------------------------------------------------------------*)
