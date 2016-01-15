(* dp0.x *)
                                (* Global definitions                 *)

                                (* Some PC versions of p2c crash on Pascal
                                   const declarations at low levels.
                                   All consts should be in dp0.x *)

const
   (*MF distmax = 3.40282347e+38; FM*) (*assumes at least IEEE single *)
   (*MF MaxReal = distmax; FM*)
   (*GH distmax = MaxReal; HG*)
   mdistmax = -distmax;
   pi = 3.1415926535897931;
   ln10 = 2.3025850929940459;

   maxint = (*GHM 2147483647 MHG*) (*F MaxInt F*);
   randmax = maxint;
   HASHLIM = 9;                 (* Hash array upper val for var names *)

                                (* Parser constants                   *)
   next = 1; kind = 2; symb = 3; err = 4; rs = 3; prod = 4; lb = 3;

(* include parscst.i *)
(*GHMF#include 'parscst.i'FMHG*)

(* include parscdc.i *)
#include 'parscdc.i'

(* include lxcst.h *)
#include 'lxcst.h'

                                (* Machine constants                  *)
    ordMINCH = 0;               (* first element of type character    *)

                                (* last element of type character,
                                   newline, tab, CR, ETX              *)
(*GHMF
    ordMAXCH = 255; ordNL = 10; ordTAB = 9; ordCR = 13; ordETX = 3;
    ordBSL = 92;                                                  FMHG*)

    bslch = chr(ordBSL); tabch = chr(ordTAB);  nlch = chr(ordNL);
    crch = chr(ordCR);  etxch = chr(ordETX); 

    (*P2CIP*)
    maxbval = (ordMAXCH+1)*(ordMAXCH+1)-1;
    (*P2CP maxbval = 128*128-1; *) (* must be > CHBUFSIZ-2 *)

                                (* Lexical parameters                 *)
    CHBUFSIZ = 4095;            (* size of chbuf arrays, input record *)
    (*GHM FILENAMELEN = 1024; MHG*)       (* max length of file names *)
    (*F FILENAMELEN = 255; F*)

                                (* Lalr machine parameters            *)
    STACKMAX = 255;             (* size of attstack and parsestack    *)
    REDUMAX = 128;              (* size of reduction buffer           *)
    MAXERRCOUNT = 3;            (* max no of errors before giving up  *)

                                (* Text parameters                    *)
    DFONT = 11;                 (* default svg textht, pt; should be adj*)
    TEXTRATIO = 1.6;            (* baseline to text height ratio      *)

                                (* Draw types                         *)
   MFpic = 1; MPost = 2; PDF = 3; PGF = 4; Pict2e = 5; PS = 6;
   PSfrag = 7; PSTricks = 8; SVG = 9; TeX = 10; tTeX = 11; xfig = 12;

                                (* processor constants                *)
   SVGPX = 90;                  (* SVG pixels per inch                *)
   SPLT = 0.551784;             (* optimum spline tension for arcs    *)
   xfigres = 1200;
   xdispres = 80;
   pointd = 72;
   
type
                                (* Lexical types                      *)
    chbufinx = 0..CHBUFSIZ;
    symbol = 0..symbmax;
    lxinx = 0..lxmax;
    production = 0..prodmax;
    chbufarray = packed array [chbufinx] of char;
    chbufp = @chbufarray;
    mstring = packed array [1..FILENAMELEN] of char;

                                (* Environment variable index         *)
    environx = (*MF XLenvvar+1 FM*)(*GH1HG*)..XLlastenv;

                                (* Lalr machine types                 *)
    stackinx = 0..STACKMAX;
    redubufrange = -REDUMAX..REDUMAX;
    reduelem = (* packed *) record
       (*D oldtop, D*) newtop: stackinx;
       prod: production
       end;

                                (* Production types                   *)
                                (* For storing names                  *)
   strptr = @nametype;
   nametype = record
      val: real;
      segmnt: chbufp;
      seginx: chbufinx;
      len: integer;
      next: strptr
      end;

                                (* Lexical input for loops and macros *)
   fbufferp = @fbuffer;
   fbuffer = record
      carray: chbufp;
      savedlen,readx,attrib: integer;
      higherb,prevb,nextb: fbufferp
      end;

                                (* Macro argument list pointer        *)
   argp = @arg;
   arg = record
      argbody: fbufferp;
      highera,nexta: argp
      end;

                                (* Pic position                       *)
   postype = (* packed *) record
      xpos,ypos: real
      end;

   envarray = array[environx] of real;

   primitivep = @primitive;

#define PRIMbase name,textp,outlinep,shadedp: strptr; parent,son,\
 next: primitivep; aat: postype; lparam,lthick: real; direction,spec: integer;
#define PRIMbox	( boxheight,boxwidth,boxfill,boxradius: real )
#define PRIMBLOCK ( blockheight,blockwidth: real; here: postype; \
            vars: array[0..HASHLIM] of strptr; \
            nvars: array[0..HASHLIM] of integer; \
            env: @envarray )
#define PRIMcircle	( cfill,radius: real )
#define PRIMellipse	( elheight,elwidth,efill: real )
#define PRIMline ( endpos: postype; height,width,lfill,aradius: real; \
 atype: integer )

   primitive = (*MF packed FM*) record
      PRIMbase
      case ptype:integer of
         XLbox,XLstring: PRIMbox ;
         XBLOCK: PRIMBLOCK ;
         XLcircle: PRIMcircle ;
         XLellipse: PRIMellipse ;
         (* XLarc:( |startangle|,|arcangle|: real );
                     endpos.xpos, endpos.ypos (uses XLline) *)
         XLline,XLarrow,XLmove,XLspline: PRIMline ;
         XLabel:()
      end;

                                (* To force optimum dynamic storage of
                                   primitives: *)
(*P2CP
   XLboxprimitive = packed record
      PRIMbase
      case ptype:integer of XLbox,XLstring: PRIMbox end;
   XLcircleprimitive = packed record
      PRIMbase
      case ptype:integer of XLcircle: PRIMcircle end;
   XLellipseprimitive = packed record
      PRIMbase
      case ptype:integer of XLellipse: PRIMellipse end;
   XLlineprimitive = packed record
      PRIMbase
      case ptype:integer of XLline,XLarrow,XLmove,XLarc,XLspline: PRIMline end;
   XLabelprimitive = packed record
      PRIMbase
      ptype: integer end; *)
(*SpecialSizeOf primitive.XLbox=sizeof(XLboxprimitive) *)
(*SpecialSizeOf primitive.XLstring=sizeof(XLboxprimitive) *)
(*SpecialSizeOf primitive.XBLOCK=sizeof(primitive) *)
(*SpecialSizeOf primitive.XLcircle=sizeof(XLcircleprimitive) *)
(*SpecialSizeOf primitive.XLellipse=sizeof(XLellipseprimitive) *)
(*SpecialSizeOf primitive.XLline=sizeof(XLlineprimitive) *)
(*SpecialSizeOf primitive.XLarrow=sizeof(XLlineprimitive) *)
(*SpecialSizeOf primitive.XLmove=sizeof(XLlineprimitive) *)
(*SpecialSizeOf primitive.XLarc=sizeof(XLlineprimitive) *)
(*SpecialSizeOf primitive.XLspline=sizeof(XLlineprimitive) *)
(*SpecialSizeOf primitive.XLabel=sizeof(XLabelprimitive) *)

                                (* Attribute stack types              *)
    attribute = record
       chbufx: chbufinx;
       length: integer;
       prim,internal: primitivep;
       varname: strptr;
       xval,yval,startchop,endchop: real;
       lexval,state: integer
       end;
    attstacktype = array [stackinx] of attribute;

                                (* Parser types                       *)
    stackelm = packed record
       link: stackinx;
       table: integer
       end;
    tparsestack = array [stackinx] of stackelm;

(*GHMF var FMHG*)
                                (* Machine-dependent characters       *)
    (* tabch, nlch, crch, etxch: char; *)

(* File names                                                         *)
    (*P2 IP*) dpictabl: text; (*G asmname 'dpic_table'; G*)
    (*P2CP input, output: text; *)
    (*GHMF errout: text; FMHG*)(*G asmname 'std_err'; G*)
    (*GHMF copyin: text; FMHG*)(*G asmname 'copy_in'; G*)
    (*GHMF redirect: text; FMHG*)(*G asmname 'redi_rect'; G*)
    (*D log: text; D*)(*DG asmname 'log_file'; GD*)

    (*GHMF infname: mstring; FMHG*)  (* name of current input file    *)
    (*GHMF outfnam: mstring; FMHG*)  (* name of current output file   *)

    inputeof: boolean;          (* end of input flag                  *)
    forbufend: boolean;         (* end of for buffer                  *)
    (*GHMF argct: integer; FMHG*)   (* argument counter for options   *)
    drawmode: integer;          (* output conversion                  *)
    safemode: boolean;          (* disable sh and copy                *)
    (*D oflag: integer; D*)     (* debug level and open logfile flag  *)

                                (* Lexical analyzer character buffer  *)
    (* chbuf: strptr; *)
    chbuf: chbufp;
    chbufi,oldbufi: chbufinx;   (* character buffer index             *)

                                (* Lexical variables                  *)
    (*P2CIP*)
    namech,uppercase,digits: set of char;         (* used in lexicals *)
    (*P2CP*)
    ch: char;                   (* current character                  *)
    newsymb: -2..symbmax;       (* current lexical symbol             *)
    lexstate: integer;          (* 0..4: <.PS; .PS; in pic; .PE; >.PE *)
    lexsymb: integer;
    inlogic: boolean;           (* set < to <compare> in context      *)
    instr: boolean;             (* set while reading a string         *)
    inbuf,savebuf,freeinbuf: fbufferp;

                                (* Error handling                     *)
    errcount: integer;          (* becomes nonzero when errors found  *)
    lineno: integer;            (* current input line number          *)
    currprod: integer;          (* current production for error msgs  *)

                                (* Production variables               *)
    attstack: @attstacktype;
    (*D stackhigh: integer;D*)
    reduinx,redutop: redubufrange;
    redubuf: array [redubufrange] of reduelem; (* reduction buffer    *)

    floatvalue: real;           (* numerical value of floats read     *)
    envblock: primitivep;       (* block containing the current scope *)

    north,south,east,west: real;(* compass corners of a primitive     *)
    xfheight: real;             (* for calculating xfig coordinates   *)
    freeseg: chbufp;            (* segment open to store strings      *)
    freex: 0..CHBUFSIZ+1;       (* next free location                 *)
    tmpbuf: chbufp;             (* buffer for snprintf or sprintf     *)
    scale,fsc: real;            (* scale factor and final scale factor*)
    splcount,spltot: integer;   (* spline depth counter               *)
    pdfobjcount: integer;       (* pdf objects                        *)
    snode: primitivep;          (* temporary node storage             *)
    bfill: boolean;             (* fill flag for linear objects       *)
    vfill: real;                (* fill value *)
    sshade,soutline: strptr;    (* temp values for linear objects     *)
    lastfillval: real;          (* last-used fill density             *)
    printstate: integer;        (* for passing output state info      *)
                                (* graphics state parameters          *)
    gsocolor,gsfcolor,gsgcolor: (* stroke, fill, gray fill flags      *)
       boolean;
    gslinethick: real;          (* last-used line thickness           *)
    gslinecap,gslinejoin: integer; (* 0 = butt                        *)
    gsdashw,gsdashs: real;      (* line dash and space lengths        *)
    stream,cx: strptr;          (* pdf stream storage and current seg *)
    pdfoffs: array [1..8] of integer; (* pdf output byte counts       *)

                                (* Global tables for easy C conversion.
                                   Alternative: use run-time space    *)
    lr: array[0..lrmax] of integer;
    entryhp: array[0..ordMAXCH] of lxinx;  (* assumes ordMINCH = 0    *)
    lxhp: array[lxinx] of lxinx;
    lxnp: array[lxinx] of lxinx;
    lxtv: array[lxinx] of symbol;
    entrytv: array[0..ordMAXCH] of symbol;
    lxch: array[lxinx] of char;
                                (* integer debugging constants        *)
    (*D debuglevel, linesignal: integer; D*)
    (*D trace: boolean; D*)     (* used for debugging the parser      *)

    (* Parser variables *)
    oldsymb: integer;           (* last lexical symbol                *)
    macros,args,freearg: argp;  (* macro and macro argument list      *)
    stacktop, pseudotop, validtop, top: stackinx;
    parsestack: @tparsestack;                 (* parse stack          *)
    parsestop: boolean;         (* parser flags                       *)
    startinx, lri, start: integer;
(*--------------------------------------------------------------------*)
