(*dp1.x---------------------------------------------------------------*)
(*
function Ceil(x: real): integer; external;
function Floor(x: real): integer; external;
function Max(x,y: real): real; external;
function Min(x,y: real): real; external;
function datan(y,x: real): real; external;
function isdistmax(x: real): boolean; external;
function ismdistmax(x: real): boolean; external;
function principal(x,r: real): real; external;
procedure fatal( t: integer); external;
*)
function findenv( p: primitivep ): primitivep; external;
(* function findmacro( p:argp;
   chb: chbufp; inx,length: chbufinx; var last:argp ): argp; external; 
function isthen( pr: primitivep ): boolean; external;
function firstsegment( pr: primitivep ): boolean; external;
function linlen( x,y: real ): real; external;
function lspec( n: integer): integer; external;
procedure checkjust( tp: strptr; var A,B,L,R: boolean ); external;
procedure getlinespec( nd: primitivep;
   var lsp: integer; var lastnd: primitivep ); external;
procedure consoleflush; external;
procedure copyleft( mac: fbufferp; var buf: fbufferp ); external;
procedure deletetree(var p: primitivep); external;
procedure newbuf( var buf: fbufferp ); external;
procedure disposebufs( var buf: fbufferp (*D; loc: integer D*)); external;
procedure drawtree(n,s,e,w: real; eb: primitivep); external;
procedure initnesw; external;
*)
procedure doprod(prno: integer); external;
procedure markerror(emi: integer); external;
procedure nesw(ptmp: primitivep); external;
procedure newarg( var ar: argp ); external;
procedure produce( newp: stackinx; p: integer); external;
procedure readfor( p0: fbufferp; attx: integer; var p2: fbufferp ); external;
procedure skiptobrace; external;
procedure wcoord(var iou: text; x,y: real ); external;
procedure wfloat(var iou: text; y: real ); external;
procedure wpair(var iou: text; x,y: real ); external;
procedure wpos( pos: postype ); external;
procedure wstring( var iou: text; p: strptr ); external;
function ahlex( atyp: integer): integer; external;
function ahnum( atyp: integer): integer; external;
function pahlex(atyp,alex: integer): integer; external;
function pahnum(atyp,anum: integer): integer; external;
function arcstart(n: primitivep): postype; external;
function arcend(n: primitivep): postype; external;
(*DGHM function ordp(p:pointer): integer; external; MHGD*)
(*F function ordp(p:pointer): PtrUInt; external; F*)
(*D procedure snaptype(var iou: text; p: integer); external; D*)
(*D procedure wrbuf(p: fbufferp; job,r: integer); external; D*)
(*D procedure logaddr(b: fbufferp); external; D*)
(*D procedure wlogfl( nm: string; v: real; cr: integer); external; D*)
(*P2CC #ifndef SAFE_MODE *)
procedure pointinput( txt: strptr ); external;
procedure pointoutput(nw: boolean; txt: strptr; var ier: integer ); external;
(*P2CC #endif *)

(*--------------------------------------------------------------------*)
