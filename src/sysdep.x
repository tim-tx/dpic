(* sysdep.x Required UNIX functions *)
(*F{$linklib c} F*)

(* access *)
function  access( var f:mstring; n:integer ): integer; (*F cdecl; F*)
   external (*HF name 'access' FH*)(*G; asmname '_p_Access' G*);
(* isatty *)
function  isatty( n:integer ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'isatty' FHG*);
(* time *)
function time( var t: integer ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'time' FHG*);
(* sprintf *)
function sprintf( var s,f:char;
            e: (*M double M*)(*GHF real FHG*) ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'sprintf' FHG*);
(* snprintf *)
function  snprintf( var s:char; n:integer; var f:char;
            e: (*M double M*)(*GHF real FHG*) ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'snprintf' FHG*);
(* system *)
function system( var s: char ): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'system' FHG*);

(* The following legacy tests may need tweaking for different operating
   systems and probably could be handled in the configure script.      *)
(* random *)
function  random(*M () M*): integer; (*F cdecl; F*)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'random' FHG*);

(* srandom *)
(*P2CIP*) procedure srandom(s: integer); (*F cdecl; F*)
(*P2CP function srandom(s: integer): integer; *)
   external (*HF name FH*)(*G; asmname G*)(*GHF 'srandom' FHG*);

(*-----------------------------------------------------------------*)
