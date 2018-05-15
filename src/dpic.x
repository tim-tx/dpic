(* dpic translator program.  *)

(* BSD Licence:

    Copyright (c) 2018, J. D. Aplevich
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
    OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*)

(* Comments within D ... D pairs are (Pascal) debug statements that are
   activated by the Makefile if debug is enabled. Comments within other
   uppercase pairs are activated for specific operating systems or compilers.
   Pascal between P2CIP ... P2CP comment pairs is ignored during p2c conversion.
   P2  IP and P2  P comments are converted to P2CIP ... P2CP for some compilers.
   C code in P2CC ... comments is included by p2c.
   *)

(*GHMF
program dpic(input,output,errout,copyin,redirect
   (*P2 IP*) ,dpictabl (*P2 P*)
   (*D,log D*)); FMHG*)

(*P2CIP*)
(*MGH label 999; HGM*)(*P2CP*)   (* exit label for parser errors      *)

(* include dp0.h *)
#include 'dp0.h'

procedure copyleft( mac: fbufferp; var buf: fbufferp ); forward;
procedure doprod(prno: integer); forward;
procedure markerror(emi: integer); forward;
procedure readfor( p0: fbufferp; attx: integer; var p2: fbufferp ); forward;
procedure skiptobrace; forward;
procedure deletename(var head: strptr); forward;
procedure newstr( var sp: strptr ); forward;
procedure deletetree( var p: primitivep ); forward;
(*D procedure printobject(primp: primitivep); forward;
    procedure snaptype(var iou: text; p: integer); forward;
    procedure wrbuf(p: fbufferp; job,r: integer); forward;
    procedure logaddr(b: fbufferp); forward;
    procedure wlogfl( nm: string; v: real; cr: integer); forward;
  D*)
(* DGHM function ordp(p:pointer): integer; forward; MHGD *)
(* F function ordp(p:pointer): PtrUInt; forward; F *)
(*P2CC #ifndef SAFE_MODE *)
procedure pointinput( txt: strptr ); forward;
procedure pointoutput(nw: boolean; txt: strptr; var ier: integer ); forward;
(*P2CC #endif *)

(*--------------------------------------------------------------------*)

(* include sysdep.h *)
#include 'sysdep.h'

(*P2CIP*)
   (*DF function ordp(p:pointer): PtrUInt;
   begin
      if p=nil then ordp := 0 else
      ordp := PtrUint(p)
      end; FD*)
   (*DH function ordp(p:pointer): integer;
   begin
      if p=nil then ordp := 0 else
      ordp := Integer(p)
      end; HD*)
   (*DG function ordp GD*)(* p:pointer *)(*DG: integer;
   begin
      if p=nil then ordp := 0 else
      ordp := Integer(p)
      end; GD*)
(*P2CP*)
(*DM function ordp(p:pointer): integer; external; MD*)

(*DFGHM function odp(p:pointer): integer; MHGFD*)
(*D begin
      odp := ordp(p) mod 10000
      end; D*)

                                (* Numerical utilities: *)
function principal( x,r: real): real;
begin
   while (x > r) do x := x - 2*r;
   while (x < -r) do x := x + 2*r;
   principal := x
   end;

function Max( x,y: real): real;
begin
   if y > x then Max := y else Max := x
   end;

function Min( x,y: real): real;
begin
   if y < x then Min := y else Min := x
   end;

function Floor( x: real): integer; 
begin
  if (x >= 0) or (trunc(x) = x) then Floor := trunc(x)
  else Floor := trunc(x)-1
  end;

function Ceil( x: real): integer;
begin
  if (x < 0) or (trunc(x) = x) then Ceil := trunc(x)
  else Ceil := trunc(x)+1
  end;

function isdistmax( x: real): boolean;
begin
  isdistmax := (abs(x/distmax-1.0) < 1e-6) 
  end;

function ismdistmax( x: real): boolean;
begin
  ismdistmax := (abs(x/mdistmax-1.0) < 1e-6) 
  end;

function datan( y,x: real ): real;
var r: real;
begin
(*P2CIP*)
   if x > 0.0 then r := arctan(y/x)
   else if x < 0.0 then begin
      if y >= 0.0 then r := pi - arctan(-y/x)
      else r := arctan(y/x)-pi
      end
   else begin
      if y > 0.0 then r := pi/2
      else if y < 0.0 then r := -pi/2
      else begin (* markerror(907); *) r := 0.0 end
      end; (*P2CP*)
(*P2CC r = atan2(y , x); *)
   datan := r
   end;

function linlen( x,y: real ): real;
var xm,ym: real;
begin
   (* linlen := sqrt( x*x + y*y ) *)
   if abs(x) > abs(y) then begin xm := abs(x); ym := y/xm end
   else if y = 0.0 then begin xm := 0.0; ym := 0.0 end
   else begin xm := abs(y); ym := x/xm end;
   linlen := xm * sqrt(1.0 + ym*ym)
   end;
                                (* Get and initialize a buffer from the
                                   old-buffer stack or make a new one *)
procedure newbuf( var buf: fbufferp );
begin
   (*D if debuglevel > 0 then write(log,' newbuf'); D*)
   if freeinbuf = nil then begin
      (* p2c automatically adds memory allocation failure checks *)
      new(buf); new(buf@.carray)
      (*D; if debuglevel > 0 then logaddr( buf ) D*)
      end
   else begin
      (*D if debuglevel > 0 then write(log,' f'); D*)
      buf := freeinbuf;
      freeinbuf := freeinbuf@.nextb
      end;
   with buf@ do begin
      savedlen := 0; carray@[0] := ' '; readx := 1; attrib := 0;
      higherb := nil; prevb := nil; nextb := nil
      end
   (*D; if debuglevel > 0 then writeln(log) D*)
   end;
                                (* Put buffers onto top of old-buffer stack *)
procedure disposebufs( var buf: fbufferp (*D; loc: integer D*));
var bu: fbufferp;
begin
   (*D if debuglevel > 0 then begin writeln(log);
       write(log,'disposebufs(',loc:1,')[',odp(buf):1,']') end; D*)
   if buf <> nil then begin
      bu := buf;
      while bu@.nextb <> nil do bu := bu@.nextb;
      bu@.nextb := freeinbuf;
      freeinbuf := buf;
      buf := nil
      end
   end;
                                (* Get and initialize an arg from the
                                   old-arg stack or make a new one *)
procedure newarg( var ar: argp );
begin
   if freearg = nil then new(ar)
   else begin ar := freearg; freearg := freearg@.nexta end;
   with ar@ do begin
      argbody := nil; highera := nil; nexta := nil
      end
   (*D; if debuglevel > 0 then
      writeln(log,' newarg[',odp(ar):1,']') D*)
   end;
                                (* Put arg structs onto top of old-arg stack *)
procedure disposeargs( var ar: argp );
var a: argp;
begin
   (*D if debuglevel > 0 then writeln(log,'disposeargs[',odp(ar):1,']'); D*)
   if ar <> nil then begin
      a := ar; disposebufs(a@.argbody (*D,0D*));
      while a@.nexta <> nil do begin
         a := a@.nexta; disposebufs(a@.argbody (*D,1D*)) end;
      a@.nexta := freearg;
      freearg := ar;
      ar := nil
      end
   end;
                                (* Causes immediate physical write to console,
                                   not needed for most systems: *)
procedure consoleflush;
begin
   flush(errout)
   (*DGHMF; if debuglevel > 0 then flush(log) FMHGD*)
   end;

procedure deletebufs( var buf: fbufferp; mv: boolean );
var bu: fbufferp;
begin
   (*D if debuglevel > 0 then write(log,' deletebufs dispose:'); D*)
   if mv then begin
      bu := buf;
      while bu <> nil do begin
         (*D if debuglevel > 0 then write(log,' up'); D*)
         buf := bu;
         bu := bu@.prevb
         end
      end;
   while buf <> nil do begin
      bu := buf@.nextb;
      (*D if debuglevel > 0 then begin write(log,' '); logaddr(buf) end; D*)
      dispose(buf@.carray); dispose(buf);
      buf := bu
      end
      (*D; if debuglevel > 0 then writeln(log) D*)
   end;
                                (* We are finished *)
procedure epilog;
begin
   deletebufs(inbuf,true);
   (*D if debuglevel > 0 then
      writeln(log,'dispose(chbuf)[',odp(chbuf):1,']'); D*)
   dispose(chbuf);
   (*D if debuglevel > 0 then begin
         writeln(log,'stackhigh=',stackhigh:1);
         writeln(log,'Dpic log ends');
         writeln(log) end; D*)
                                (* Seems needed for some Cygwin machines: *)
   (* GH consoleflush;
   flush(stdout) HG *)
   end;

                                (* Unrecoverable errors *)
procedure fatal( t: integer );
begin
   if t <> 0 then write(errout,' *** dpic: ');
   case t of
      0: ;
      1: writeln(errout,'input file not readable');
      (*P2 IP*)
      2: writeln(errout,'dpic data file dpic.tab not readable');
      (*P2 P*)
      3: writeln(errout,'maximum error count exceeded');
      4: writeln(errout,'character buffer overflow: "CHBUFSIZ" exceeded');
      5: writeln(errout,'end of file encountered on input');
      6: begin
         writeln(errout,'too many pending actions, const "STACKMAX" exceeded,');
         writeln(errout,' possibly caused by infinite recursion.')
         end;
      7: writeln(errout,'input too complex, const "REDUMAX" exceeded');
      8: writeln(errout,'error recovery abandoned');
      9: writeln(errout,'subscript out of range');
      otherwise writeln(errout,'unknown fatal error')
      end;
   (*P2CIP*)
   (*MGH goto 999 HGM*)
   (*DF if debuglevel > 0 then flush(log); FD*)
   (*F halt(1) F*)
   (*P2CP*)

   (*P2CC epilog();
     if (input != NULL) fclose(input);
     if (output != NULL) fclose(output);
     if (errout != NULL) fclose(errout); *)
   (*DP2CC if (log_ != NULL) fclose(log_); *)
   (*P2CC exit(EXIT_FAILURE); *)

   end;

(*--------------------------------------------------------------------*)

procedure controls;
begin
   writeln; write(' ..controls ')
   end;

procedure wrand;
begin
   write(' and ')
   end;

procedure ddot;
begin
   writeln; write(' ..')
   end;

procedure ddash;
begin
   writeln; write(' --')
   end;

procedure space;
begin
   write(' ')
   end;

procedure quote;
begin
   write('"')
   end;

procedure getlinshade( nod:primitivep;
                    var tn:primitivep;
                    var ss,so:strptr; var fillval:real; var hshade:boolean );
begin 
   tn := nod; ss := nil; so := nil; fillval := -1.0;
   while tn<>nil do with tn@ do begin
      if outlinep<>nil then so := outlinep;
      tn := tn@.son
      end;
   tn := nod;
   if hshade then hshade := false else while nod<>nil do with nod@ do begin
      if shadedp<>nil then ss := shadedp;
      if (lfill >= 0.0) and (lfill <= 1.0) then fillval := lfill;
      tn := nod; nod := nod@.son
      end;
   if (ss <> nil) or (fillval >= 0.0) then hshade := true
   end;

(* Arrowhead location (lex value) and number *)
function ahlex( atyp: integer): integer;
begin
  ahlex := (atyp div 8)
  end;

function ahnum( atyp: integer): integer;
begin
  ahnum := (atyp mod 8)
  end;

function pahlex(atyp,alex: integer): integer;
begin
  pahlex := (atyp mod 8) + (alex * 8)
  end;

function pahnum(atyp,anum: integer): integer;
begin
  pahnum := (atyp div 8) * 8 + (anum mod 8)  (* 0 < anum < 7 *)  
  end;

procedure wfloat( var iou: text; y: real );
(*P2CIP*)
var k: integer;
  i: (*M integer; M*) (*GHF LongInt; FHG*)
begin
   if isdistmax(abs(y)) then write(iou,y:11)
   else begin
      i := round(abs(abs(y) - int(abs(y)))*1000000);
      if i=0 then write(iou,round(y):1)
      else begin
         k := 0;
         while (i <> 0) and ((i mod 10) = 0) do begin
            k := k+1; i := (i div 10) end;
         write(iou,y:1:6-k)
         end
      end
(*P2CP begin*)
(*P2CC
   char buf[25];
   int i;
   if (fabs(y)==distmax)
      sprintf(buf,"%24.6e", y);
   else if (y >= 0.0)
      sprintf(buf,"%24.6f", floor( 1000000*y+0.5)/1000000.0 );
   else
      sprintf(buf,"%24.6f",-floor(-1000000*y+0.5)/1000000.0 );
   for (i=23; buf[i]=='0'; ) i-- ;
   if (buf[i]=='.') buf[i] = (char)0;
   else buf[i+1] = (char)0;
   for (;(i>=0)&&(buf[i]!=' ');) i-- ;
   fprintf( *iou,"%s",&buf[i+1]); *)
   end;

procedure wpair( var iou: text; x,y: real );
begin
   write(iou,'(');
   wfloat(iou,x); write(iou,','); wfloat(iou,y);
   write(iou,')')
   end;

procedure wcoord( var iou: text; x,y: real );
begin
   write(iou,'(');
   wfloat(iou,x/fsc); write(iou,',');
   wfloat(iou,y/fsc); write(iou,')')
   (*DGHMF ;flush(iou) FMHGD*)
   end;

procedure wpos( pos: postype );
begin
   wcoord(output, pos.xpos,pos.ypos )
   end;

procedure wstring( var iou: text; p: strptr );
var i: integer;
begin
   (*D if debuglevel <= 0 then begin end
       else if p = nil then begin end
       else if p@.segmnt = nil then begin end
       else with p@ do begin
          writeln(log,' seginx : len : seginx+len-1=',
             seginx:1,' : ',len:1,' : ',seginx+len-1:1);
          if seginx+len-1 > CHBUFSIZ then begin
             writeln(log,' Offending string:');
             write(log,'|');
             for i:=seginx to CHBUFSIZ do write(log,segmnt@[i]);
             writeln(log,'|')
             end;
          end;
          D*)
   if p<>nil then with p@ do if segmnt<>nil then
      for i := seginx to seginx+len-1 do write(iou,segmnt@[i])
   end;

procedure wbrace( x: real );
begin
   write('{'); wfloat(output,x); write('}')
   end;

procedure setjust( tp: strptr; v: integer );
   var i: integer;
begin
   if tp <> nil then with tp@ do begin
      i := round(val);
      case v of
         XLrjust: val := (i div 4)*4 + 1;
         XLljust: val := (i div 4)*4 + 2;
         XLbelow: val := (i div 16)*16 + 4 + (i mod 4);
         XLabove: val := (i div 16)*16 + 8 + (i mod 4);
         XLcenter:val := (i div 16)*16;
         otherwise
         end
      end
   end;

procedure checkjust( tp: strptr; var A,B,L,R: boolean );
var i: integer;
begin
   if tp=nil then begin
      A := false; B := false; L := false; R := false end
   else begin
      i := round(tp@.val);
      R := odd(i);
      L := odd(i div 2);
      B := odd(i div 4);
      A := odd(i div 8)
      end
   end;
(*                                  Test if ht of string has been set
function checkht( tp: strptr ): boolean;
var i: integer;
begin
   if tp=nil then checkht := false
   else begin
      i := round(tp@.val);
      checkht := odd(i div 16)
      end
   end; *)

function lspec( n: integer): integer;
begin
   (* if ((n div 16) mod 2) <> 0 then lspec := XLsolid
   else *) lspec := (n mod 8) + XLlinetype
   end;

procedure getlinespec( nd:primitivep; var lsp:integer; var lastnd:primitivep);
var tn: primitivep;
begin
   tn := nd; if nd@.ptype in [XLspline,XLline,XLarrow,XLarc] then
     while tn@.son <> nil do tn := tn@.son;
   lastnd := tn;
   lsp := lspec(tn@.spec)
   end;


function findenv( p: primitivep ): primitivep;
var q: primitivep;
begin
   q := nil;
   while p<>q do begin
      if p@.ptype<>XBLOCK then
         p := p@.parent
      else if p@.env=nil then
         p := p@.parent
      else q := p
      end;
   (*D if debuglevel > 0 then begin
      if p = nil then writeln(log,'findenv: p=nil')
      else if p@.env = nil then writeln(log,'findenv: p@.env=nil');
      flush(log) end; D*)
   findenv := p
   end;

function venv(p: primitivep; ind: integer): real;
var v: real;
begin
   v := 0.0;
   if (ind > XLenvvar) and (ind <= XLlastenv) then begin
      p := findenv(p);
      if p <> nil then v := p@.env@[ind]
      end;
   venv := v
   end;

function qenv(p: primitivep; ind: integer; localval: real): real;
var noval: real;
begin
  case ind of
     XLfillval: noval := -1.0;
     XLlinethick: noval := mdistmax;
     XLdashwid: noval := mdistmax;
     otherwise noval := 0.0
     end;
  if localval <> noval then qenv := localval
  else qenv := venv(p,ind)
  end;

(*        orig + mat(cs) * [x,y] *)
function affine(x,y: real; orig,cs: postype): postype;
var tpos: postype;
begin
  tpos.xpos := orig.xpos + cs.xpos*x-cs.ypos*y;
  tpos.ypos := orig.ypos + cs.ypos*x+cs.xpos*y;
  affine := tpos
  end;

function affang(point,shaft: postype): postype;
var
  lgth: real;
  tpos: postype;
begin
   lgth := linlen(shaft.xpos-point.xpos,shaft.ypos-point.ypos);
   if lgth = 0.0 then begin tpos.xpos := 1.0; tpos.ypos := 0.0 end
   else begin
      tpos.xpos := (point.xpos-shaft.xpos)/lgth;
      tpos.ypos := (point.ypos-shaft.ypos)/lgth
      end;
   affang := tpos
   end;

function posangle(V,C: postype): real;
begin
  posangle := datan(V.ypos-C.ypos,V.xpos-C.xpos)
  end;

procedure initnesw;
begin
   south := distmax;
   north := -south;
   west := south;
   east := -west
   end;

procedure neswstring( pmp: primitivep; ht,wd: real );
var A,B,L,R: boolean;
   x,y,offst: real;
begin
   if pmp <> nil then with pmp@ do begin
      checkjust(textp,A,B,L,R);
      offst := venv(pmp,XLtextoffset);
      y := aat.ypos;
      if A then y := y+ht*0.5+offst else if B then y := y-ht*0.5-offst;
      x := aat.xpos;
      if R then x := x-wd*0.5-offst else if L then x := x+wd*0.5+offst;
      north := Max(north,y+ht*0.5);
      south := Min(south,y-ht*0.5);
      west := Min(west,x-wd*0.5);
      east := Max(east,x+wd*0.5)
      (*D;  if debuglevel>0 then begin
         write(log,' neswstring:');
         wlogfl('aat.xpos',aat.xpos,0); wlogfl('x',x,0); wlogfl('y',y,1);
         wlogfl('east',east,0); wlogfl('west',west,0); wlogfl('wd',wd,1);
         wlogfl('north',north,0); wlogfl('south',south,0); wlogfl('ht',ht,1)
         end D*)
      end
   end;

procedure neswline( pmp: primitivep );
var aht,awd: real;
   cs,cc,cd: postype;
begin
   if pmp <> nil then with pmp@ do begin
      west := Min(west,Min(aat.xpos,endpos.xpos));
      east := Max(east,Max(aat.xpos,endpos.xpos));
      south := Min(south,Min(aat.ypos,endpos.ypos));
      north := Max(north,Max(aat.ypos,endpos.ypos));
      if ahlex(atype) in [XDOUBLEHEAD,XLEFTHEAD] then begin
         cs := affang(endpos,aat);
         awd := qenv(pmp,XLarrowht,width);
         aht := qenv(pmp,XLarrowwid,height);
         cc := affine(aht,awd/2,aat,cs);
         cd := affine(aht,-awd/2,aat,cs);
         west := Min(west,Min(cc.xpos,cd.xpos));
         east := Max(east,Max(cc.xpos,cd.xpos));
         south := Min(south,Min(cc.ypos,cd.ypos));
         north := Max(north,Max(cc.ypos,cd.ypos))
         end;
      if ahlex(atype) in [XDOUBLEHEAD,XRIGHTHEAD] then begin
         cs := affang(aat,endpos);
         awd := qenv(pmp,XLarrowht,width);
         aht := qenv(pmp,XLarrowwid,height);
         cc := affine(aht,awd/2,endpos,cs);
         cd := affine(aht,-awd/2,endpos,cs);
         west := Min(west,Min(cc.xpos,cd.xpos));
         east := Max(east,Max(cc.xpos,cd.xpos));
         south := Min(south,Min(cc.ypos,cd.ypos));
         north := Max(north,Max(cc.ypos,cd.ypos))
         end
      end
   end;

function inarc(strt,fin,ang,arcang: real): boolean;
var inarctmp: boolean;
begin
   (*D if debuglevel > 0 then begin
      write(log,'Inarc strt,fin='); wfloat(log,strt*180.0/pi);
         write(log,' ' ); wfloat(log,fin*180.0/pi);
      write(log,' ang,arcang='); wfloat(log,ang*180.0/pi);
         write(log,' ' ); wfloat(log,arcang*180.0/pi) end; D*)
   if arcang >= 0.0 then begin
      while fin < strt do fin := fin + 2.0*pi;
      while ang < strt do ang := ang + 2.0*pi;
      if (ang <= fin) then inarctmp := true
      else inarctmp := false
      end
   else begin
      while fin > strt do fin := fin - 2.0*pi;
      while ang > strt do ang := ang - 2.0*pi;
      if (ang >= fin) then inarctmp := true
      else inarctmp := false
      end;
   inarc := inarctmp
   (*D; if debuglevel > 0 then writeln(log,' Inarc =',inarctmp) D*)
   end;

procedure nesw( ptmp: primitivep );
var hight,wdth,sang,eang: real;
begin
   if ptmp <> nil then begin
      with ptmp@ do case ptype of
         XLstring:  begin hight := boxheight;    wdth := boxwidth end;
         XLbox:     begin hight := boxheight;    wdth := boxwidth end;
         XBLOCK:    begin hight := blockheight;  wdth := blockwidth end;
         XLcircle:  begin hight := 2.0 * radius; wdth := hight end;
         XLellipse: begin hight := elheight;     wdth := elwidth end;
         otherwise begin hight := 0; wdth := 0 end
         end;
      with ptmp@ do case ptype of
         XLbox,XBLOCK,XLcircle,XLellipse: begin
            north := Max(north,aat.ypos+hight*0.5);
            south := Min(south,aat.ypos-hight*0.5);
            west := Min(west,aat.xpos-wdth*0.5);
            east := Max(east,aat.xpos+wdth*0.5)
            end;
         XLstring: if drawmode = SVG then begin
               north := Max(north,aat.ypos+hight*0.5);
               south := Min(south,aat.ypos-hight*0.5);
               west := Min(west,aat.xpos-wdth*0.5);
               east := Max(east,aat.xpos+wdth*0.5)
               end
            else neswstring( ptmp,hight,wdth );
         XLline,XLarrow,XLmove,XLspline: neswline( ptmp );
         XLaTeX,XLabel: ;
         XLarc: begin
            sang := principal(|startangle|,pi);
            eang := |startangle|+|arcangle|;
            (*D if debuglevel > 0 then begin write(log,'(sang,eang)(deg)=');
               wpair(log,sang*180/pi,eang*180/pi); writeln(log) end; D*)
            if inarc(sang,eang,0.5*pi,|arcangle|) then
               north := Max(north,aat.ypos+aradius)
            else north := Max(north,aat.ypos+aradius*Max(sin(sang),sin(eang)));
            if inarc(sang,eang,-0.5*pi,|arcangle|) then
               south := Min(south,aat.ypos-aradius)
            else south := Min(south,aat.ypos+aradius*Min(sin(sang),sin(eang)));
            if inarc(sang,eang,pi,|arcangle|) then
               west := Min(west,aat.xpos-aradius)
            else west := Min(west,aat.xpos+aradius*Min(cos(sang),cos(eang)));
            if inarc(sang,eang,0,|arcangle|) then
               east := Max(east,aat.xpos+aradius)
            else east := Max(east,aat.xpos+aradius*Max(cos(sang),cos(eang)))
            end
         end
      (*D;if debuglevel > 0 then with ptmp@ do begin
         write(log, 'nesw(',odp(ptmp):1,') ptype=',ptype:1);
         wlogfl('W',west,0); wlogfl('S',south,0);
         wlogfl('E',east,0); wlogfl('N',north,1)
         end D*)
      end
   end;

procedure texstacktext( np: primitivep; tp: strptr );
var tx: strptr;
    A,B,L,R: boolean;
    toff: real;
begin
   if tp <> nil then begin
      tx := tp@.next;
      if tx <> nil then write('\shortstack{');
      toff := venv(np,XLtextoffset)/scale*72;
      repeat
         checkjust(tp,A,B,L,R);
         if L then begin
            write('\rlap{\hbox to '); wfloat(output,toff); write('bp{}')
            end
         else if R then write('\llap{');
         wstring(output, tp);
         if R then begin
            write('\hbox to '); wfloat(output,toff); write('bp{}')
            end;
         if L or R then write('}');
         tp := tp@.next;
         if tp <> nil then writeln('\\%')
      until tp = nil;
      if tx <> nil then write('}')
      end
   end;

function primdepth( ptmp: primitivep ): integer;
var dep: integer;
begin
   dep := 0;
   while ptmp <> nil do begin dep := dep+1; ptmp := ptmp@.son end;
   primdepth := dep
   end;

procedure pprop( p1: postype; var p2: postype; a,b,c: real );
begin
   if c <> 0.0 then begin
      p2.xpos := (a*p1.xpos+b*p2.xpos)/c;
      p2.ypos := (a*p1.ypos+b*p2.ypos)/c
      end
   end;

procedure wprop( p1,p2: postype; a,b,c: real );
begin
   pprop( p1,p2, a,b,c );  (* Note: p2 is not var *)
   wpos( p2 )
   end;

function iscorner(theta: real): boolean;
begin
   (*D if debuglevel = 2 then
     writeln(log,'iscorner(',theta*180/pi:7:4,')=',
       (abs(theta) < 0.001) or (abs(0.5*pi-abs(theta)) < 0.001)); D*)
   iscorner := (abs(theta) < 0.001) or (abs(0.5*pi-abs(theta)) < 0.001)
   end;

function hcf(x,y: integer): integer;
var i: integer;
begin
   if x < 0 then x := -x;
   if y < 0 then y := -y;
   if y > x then begin i := y; y := x; x := i end;
   while y > 0 do begin
      i := y; y := x-(x div y)*y; x := i end;
   if x = 0 then hcf := 1 else hcf := x
   end;

(*
function iabs(i: integer): integer;
begin
  if i < 0 then iabs := -i else iabs := i
  end;
*)

procedure wrslope( xp,yp: real; arrow: boolean );
var i,ix,iy: integer;
   r: real;
begin
   (*D if debuglevel > 0 then begin
      write(log,'wrslope xp,yp: '); wpair(log,xp,yp) end; D*)
   if (xp=0.0) and (yp=0.0) then begin xp := 1; yp := 0 end;
   r := linlen(xp,yp);
   if drawmode = Pict2e then i := 1000 (*4096*)
   else if drawmode = tTeX then i := 453
   else if arrow then i := 4
   else i := 6;
   iy := round((i+0.49999)*yp/r); ix := round((i+0.49999)*xp/r);
   i := hcf(ix,iy);
   iy := iy div i; ix := ix div i;
   (*D if debuglevel > 0 then begin
          write(log,' ix,iy:(',ix:1,',',iy:1,')'); write(log,' ',chr(123));
          if ix = 0 then wfloat(log,abs(yp)/fsc) else wfloat(log,abs(xp)/fsc);
          writeln(log,chr(125))
          end; D*)
   write( '(', ix:1, ',', iy:1, ')' );
   if ix = 0 then wbrace( abs(yp)/fsc ) else wbrace( abs(xp)/fsc )
   end;

function isthen( pr: primitivep ): boolean;
begin
   if pr = nil then isthen := false
   else isthen := ((pr@.spec div 8) mod 2) <> 0
   end;

function firstsegment( pr: primitivep ): boolean;
begin
   if pr = nil then firstsegment := false
   else firstsegment := ((pr@.spec div 8) mod 2) = 0
   end;

function drawn(node: primitivep; linesp: integer; fill: real): boolean;
begin
   if node = nil then drawn := false
   else if node@.shadedp<>nil then drawn := true
   else if (linesp in [XLsolid,XLdashed,XLdotted])
      or ((fill>=0.0) and (fill<=1.0)) then drawn := true
   else drawn := false
   end;
                                (* distance to P control point *)
function ahoffset(ht,wid,lti: real): real;
begin
   if wid = 0.0 then ahoffset := 0.0
   else ahoffset := lti*sqrt(ht*ht+wid*wid/4)/wid
   end;

(*D procedure logpos( lbl: string; P: postype );
begin
   writeln(log);
   write(log,' ',lbl,':(');
   wfloat(log,P.xpos); write(log,','); wfloat(log,P.ypos); write(log,')')
   end; D*)
                                   (* Arrowhead control points *)
procedure dahead(point,shaft: postype;
    ht,wid, (* arrowhead ht and wid, user units *)
    ltu     (* line thickness in diagram units *)
    : real;
  var P,L,R,Px,Lx,Rx,C: postype;   (* adj point, left, right pts, dir cosines *)
  var x,y: real );
var
   h,v,po,t: real;
begin
   (*D if debuglevel > 0 then begin
      writeln(log,' dahead input:');
      write(log,' ht='); wfloat(log,ht);
      write(log,';wid='); wfloat(log,wid);
      write(log,';ltu='); wfloat(log,ltu);
      write(log,';fsc='); wfloat(log,fsc);
      logpos('point',point);
      logpos('shaft',shaft);
      writeln(log) end; D*)
   C := affang(shaft,point);          (* shaft direction cosines *)
   po := ahoffset(ht,wid,ltu);
   if po > ht then po := ht;
   P := affine(po,0.0,point,C);       (* point adjusted by line thickness *)
   h := ht-ltu/2;
   x := h - po;
   if ht = 0.0 then v := 0.0 else v := (wid/2)*x/ht;
   R := affine(h, v,point,C);
   L := affine(h,-v,point,C);
   if x = 0.0 then t := 1 else t := ht/x;
   Rx.xpos := point.xpos + (R.xpos-P.xpos)*t;  (* right corner *)
   Rx.ypos := point.ypos + (R.ypos-P.ypos)*t;
   Lx.xpos := point.xpos + (L.xpos-P.xpos)*t;  (* left corner  *)
   Lx.ypos := point.ypos + (L.ypos-P.ypos)*t;
   Px.xpos := (point.xpos+Lx.xpos+Rx.xpos)/3;  (* type 3 center pt *)
   Px.ypos := (point.ypos+Lx.ypos+Rx.ypos)/3;
   if ht = 0.0 then y := 0 else y := ht-po+ltu*wid/ht/4
   (*D; if debuglevel > 0 then begin
       write(log,' dahead out: po='); wfloat(log,po);
       logpos('P',P); logpos('L',L); logpos('R',R); logpos('C',C);
       logpos('Px',Px); logpos('Lx',Lx); logpos('Rx',Rx); writeln(log);
       write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
       writeln(log) end D*)
   end;

procedure popgwarc( Ctr: postype; radius,startangle,endangle,ccw: real );
var narcs,i: integer;
   c,s,cc,ss,arcangle: real;
   Q: postype;
begin
   if (ccw > 0) and (endangle < startangle) then
      endangle := endangle + 2*pi 
   else if (ccw < 0) and (endangle > startangle) then
      endangle := endangle - 2*pi;
   narcs := trunc(1.0+abs(endangle-startangle)/pi);
   arcangle := (endangle - startangle)/narcs;
   c := cos(arcangle/2); s := sin(arcangle/2);
   cc := (4-c)/3; if s<>0.0 then ss := (1.0-c*cc)/s else ss := 0.0;
   for i := 1 to narcs do begin
      controls;
      Q.xpos := cos(startangle+(i-0.5)*arcangle);
      Q.ypos := sin(startangle+(i-0.5)*arcangle);
      wpos(affine(radius*cc,-radius*ss,Ctr,Q)); wrand;
      wpos(affine(radius*cc, radius*ss,Ctr,Q)); ddot;
      Q.xpos := Ctr.xpos + radius*cos(startangle+i*arcangle);
      Q.ypos := Ctr.ypos + radius*sin(startangle+i*arcangle);
      wpos(Q)
      end
   end;
                                 (* Parameters and positions for traced arrows*)
procedure arcahead( C,point: postype; atyp: integer;
       ht,wid,lth,radius,angle: real;
   var P,Co,Ci,Px,Cox,Cix,Ao,Ai: postype;
   var ccw,lwi: real;
   var startarrow: boolean );
var lw,aa,bb,cc,s,v,d,b,t: real;
   Q: postype;
begin
   (*D if debuglevel > 0 then begin
      write(log,' arcahead input:');
      logpos('C',C); logpos('point',point); writeln(log);
      write(log,' atyp='); write(log,atyp:1);
      write(log,';ht='); wfloat(log,ht);
      write(log,';wid='); wfloat(log,wid);
      write(log,';lth='); wfloat(log,lth);
      write(log,';radius='); wfloat(log,radius);
      write(log,';angle='); wfloat(log,angle);
      writeln(log) end; D*)
   if radius*angle > 0 then ccw := 1 else ccw := -1;
   startarrow := (radius >= 0);
   ht := abs(ht); wid := abs(wid);
   lwi := lth/72*scale;          (* line thickness in diagram units *)
   lw := Min(abs(lwi),Min(wid/2,ht/2));
   wid := Max(wid,lw);
   radius := abs(radius);
   d := sqrt(ht*ht+wid*wid/4);
                                 (* Centres of the wing arcs *)
   if d = 0 then begin Q.xpos := 1; Q.ypos := 0 end
   else begin Q.xpos := ht/d; Q.ypos := ccw*wid/2/d end;
   Ci := affine(C.xpos-point.xpos,C.ypos-point.ypos,point,Q);
   Q.ypos := -Q.ypos;
   Co := affine(C.xpos-point.xpos,C.ypos-point.ypos,point,Q);
                                 (* Outer corner *)
   if radius = 0 then t := 0 else t := Min(pi/2,d/radius);
   Q.xpos := cos(t); Q.ypos := ccw*sin(t);
   Ao := affine(point.xpos-Co.xpos,point.ypos-Co.ypos,Co,Q);
                                 (* Make angle(C to Ai) = angle(C to Ao) *)
   aa := sqr(Ao.xpos-C.xpos)+sqr(Ao.ypos-C.ypos);
   bb:= 2*((Ao.xpos-C.xpos)*(C.xpos-Ci.xpos)+(Ao.ypos-C.ypos)*(C.ypos-Ci.ypos));
   cc := sqr(C.xpos-Ci.xpos)+sqr(C.ypos-Ci.ypos)-radius*radius;
   s := bb*bb-4*aa*cc;
   if s < 0 then v := aa else v := (-bb+sqrt(s))/2;
   Ai := Ao;
   pprop(C,Ai,aa-v,v,aa);
                                 (* Point adjusted for line thickness *)
   if d=0 then P := point else if radius=0 then P := C else if ht=d then P := Ao
   else begin
      b := 2*radius*sqrt((1-ht/d)/2);                (* distance C to Co *)
                                 (* Angle from Co-C to P, center C *)
      Q.xpos := (b*b - lw*lw + 2*lw*radius)/(2*b*radius);
      if abs(Q.xpos) > 1 then begin
         P.xpos := (Ao.xpos+Ai.xpos)/2; P.ypos := (Ao.ypos+Ai.ypos)/2
         end
      else begin
         Q.ypos := -ccw*sqrt(1-Q.xpos*Q.xpos);
         P := affine(radius*(Co.xpos-C.xpos)/b,radius*(Co.ypos-C.ypos)/b,C,Q)
         end
      end;
                                 (* Type 3 center and corners *)
   if radius = 0 then t := 0 else t := Min(pi/2,ht/radius*2/3);
   Q.xpos := cos(t); Q.ypos := ccw*sin(t);
   Px := affine(point.xpos-C.xpos,point.ypos-C.ypos,C,Q);

   v := radius*radius;
   d := sqr(Ao.xpos-Px.xpos)+sqr(Ao.ypos-Px.ypos);
   if d=0 then s := sqrt(v) else if v/d<0.25 then s := 0
   else s := sqrt(v/d-0.25); 
   Cox.xpos := (Px.xpos+Ao.xpos)/2 - ccw*(Ao.ypos-Px.ypos)*s;
   Cox.ypos := (Px.ypos+Ao.ypos)/2 + ccw*(Ao.xpos-Px.xpos)*s;

   d := sqr(Ai.xpos-Px.xpos)+sqr(Ai.ypos-Px.ypos);
   if d=0 then s := sqrt(v) else if v/d<0.25 then s := 0
   else s := sqrt(v/d-0.25); 
   Cix.xpos := (Px.xpos+Ai.xpos)/2 - ccw*(Ai.ypos-Px.ypos)*s;
   Cix.ypos := (Px.ypos+Ai.ypos)/2 + ccw*(Ai.xpos-Px.xpos)*s

   (*D; if debuglevel > 0 then begin
      writeln(log,' arcahead out:');
      write(log,' lw='); wfloat(log,lw);
      logpos('C',C); logpos( 'point',point);
      logpos('Ci',Ci); logpos( 'Ai',Ai);
      logpos('Co',Co); logpos( 'Ao',Ao);
      logpos('P',P);
      C.xpos := (Ao.xpos+Ai.xpos)/2; C.ypos := (Ao.ypos+Ai.ypos)/2;
      logpos('AoAi',C);
      logpos('Px',Px); logpos( 'Cox',Cox); logpos( 'Cix',Cix);
      writeln(log)
      end D*)
   end;

procedure startarc(n: primitivep; X0: postype; lth: real;
  var h,w: real);
var x,y: real;
begin
   with n@ do begin
      h := qenv(n,XLarrowht,height);
      w := qenv(n,XLarrowwid,width);
      y := ahoffset(h,w,lth/72*scale);
      if (aradius*aradius-y*y) <= 0.0 then x := 0.0
      else x := 2*arctan(y/sqrt(aradius*aradius-y*y));
      if |arcangle| >= 0.0 then begin
         |startangle| := |startangle|+x; |arcangle| := |arcangle|-x
         end
      else begin
         |startangle| := |startangle|-x; |arcangle| := |arcangle|+x
         end
      end
   end; 

procedure endarc(n: primitivep; X0: postype; lth: real; var h,w: real);
var x,y: real;
begin
   with n@ do begin
      h := qenv(n,XLarrowht,height);
      w := qenv(n,XLarrowwid,width);
      y := ahoffset(h,w,lth/72*scale);
      if (aradius*aradius-y*y) <= 0.0 then x := 0.0
      else x := 2*arctan(y/sqrt(aradius*aradius-y*y));
      if |arcangle| >= 0.0 then |arcangle| := |arcangle|-x
      else |arcangle| := |arcangle|+x 
      end
   end;

function arcstart(n: primitivep): postype;
var X: postype;
begin
   with n@ do begin
      X.xpos := aat.xpos + aradius*cos(|startangle|);
      X.ypos := aat.ypos + aradius*sin(|startangle|)
      end;
   arcstart := X
   end;

function arcend( n: primitivep): postype;
var X: postype;
begin
   with n@ do begin
      X.xpos := aat.xpos + aradius*cos(|startangle|+|arcangle|);
      X.ypos := aat.ypos + aradius*sin(|startangle|+|arcangle|)
      end;
   arcend := X
   end;

(* include xfig.h *)
#include 'xfig.h'

(* include svg.h *)
#include 'svg.h'

(* include pst.h *)
#include 'pst.h'

(* include mfp.h *)
#include 'mfp.h'

(* include mpo.h *)
#include 'mpo.h'

(* include pgf.h *)
#include 'pgf.h'

(* include ps.h *)
#include 'ps.h'

(* include pdf.h *)
#include 'pdf.h'

(* # include 'mps.h' *)

(* include tex.h *)
#include 'tex.h'


procedure treedraw( node: primitivep );
begin
   while node <> nil do begin
      case drawmode of
         TeX,tTeX,Pict2e: texdraw(node);
         PDF: pdfdraw(node);
         PGF: pgfdraw(node);
         PSTricks: pstdraw(node);
         MFpic: mfpdraw(node);
         PS,PSfrag: psdraw(node);
         MPost: mpodraw(node);
         SVG: svgdraw(node);
         xfig: xfigdraw(node)
         end;
      if node@.son <> nil then treedraw(node@.son);
      if drawmode = PDF then resetgs(node)
      else if (drawmode=xfig) and (node@.ptype = XBLOCK) 
         and (node@.direction = -1) then writeln('-6');
      bfill := false; sshade := nil; soutline := nil;
      node := node@.next
      end
   end;
                                (* without recursion: *)
(*
procedure treedraw( node: primitivep );
var nd: primitivep;
begin
   if node <> nil then nd := node@.parent else nd := nil;
   while node <> nd do if node@.ptype < XLlastenv then begin
      case drawmode of
         TeX,tTeX,Pict2e: texdraw(node);
         PDF: pdfdraw(node);
         PGF: pgfdraw(node);
         PSTricks: pstdraw(node);
         MFpic: mfpdraw(node);
         PS,PSfrag: psdraw(node);
         MPost: begin mpodraw(node); if bfill then mpodraw(node) end;
         SVG: svgdraw(node);
         xfig: xfigdraw(node)
         end;
      if node@.son <> nil then begin
         node@.ptype := node@.ptype + XLlastenv;
         node := node@.son
         end
      else if node@.next <> nil then begin
         node@.ptype := node@.ptype + 2*XLlastenv;
         node := node@.next
         end
      else node := node@.parent
      end
   else if node@.ptype < 2*XLlastenv then begin
      if drawmode <> xfig then begin end
      else if (node@.ptype = (XBLOCK + XLlastenv))
         and (node@.direction = -1) then writeln('-6');
      bfill := false; sshade := nil; soutline := nil;
      if node@.next <> nil then begin
         node@.ptype := node@.ptype + XLlastenv;
         node := node@.next
         end
      else begin
         node@.ptype := node@.ptype - XLlastenv;
         node := node@.parent
         end
      end
   else begin
      node@.ptype := node@.ptype - 2*XLlastenv;
      node := node@.parent
      end
   end; *)

procedure drawtree( n,s,e,w: real; eb: primitivep );
var fsctmp: real;
begin
   bfill := false; sshade := nil; soutline := nil;
   case drawmode of
      SVG: begin
         fsctmp := fsc; fsc := fsc/dpPPI;       (* output pixels *)
         svgprelude(n,s,e,w,venv(eb,XLlinethick)/72*scale);
         treedraw( eb );
         svgpostlude;
         fsc := fsctmp
         end;
      PGF: begin
         pgfprelude;
         treedraw( eb );
         pgfpostlude
         end;
      PSTricks: begin
         pstprelude(n,s,e,w);
         treedraw( eb );
         pstpostlude
         end;
      MFpic: begin
         mfpprelude(n,s,e,w);
         treedraw( eb );
         mfppostlude
         end;
      PS,PSfrag: begin
         fsctmp := fsc; fsc := fsc/72;            (* output points *)
         psprelude(n,s,e,w,venv(eb,XLlinethick));
         treedraw( eb );
         pspostlude;
         fsc := fsctmp
         end;
      PDF: begin
         fsctmp := fsc; fsc := fsc/72;            (* output points *)
         pdfprelude(n,s,e,w,venv(eb,XLlinethick));
         treedraw( eb );
         pdfpostlude;
         fsc := fsctmp
         end;
      MPost: begin
         fsctmp := fsc; fsc := fsc/72;            (* output points *)
         mpoprelude;
         treedraw( eb );
         mpopostlude;
         fsc := fsctmp
         end;
      xfig: begin
         xfigprelude;
         treedraw( eb );
         (* xfigpostlude *)
         end;
      TeX,tTeX,Pict2e: if not isdistmax(s) then begin
         texprelude(n,s,e,w);
         treedraw( eb );
         texpostlude
         end
      end
   end;
(* G end. G *)

(* include dpic1.p *)
(*MGHF#include 'dpic1.p'FHGM*)

(*--------------------------------------------------------------------*)

                                (* The log file is only for debugging *)
(*DGHMF
procedure openlogfile;
begin FMHGD*)
   (*DGHM rewrite(log,'dpic.log'); MHGD*)
   (*DF assign(log,'./dpic.log'); rewrite(log); FD*)(*DGHMF
   writeln(log,'Dpic log file'); flush(log);
   if oflag <= 0 then oflag := 1
   end; FMHGD*)

                                (* Check if a file is accessible *)
(*MGH
function checkfile( var ifn: mstring; isverbose: boolean ): integer;
var cf,i,j: integer;
begin
   j := FILENAMELEN; i := 0;
   while j > i do if ifn[j] = ' ' then j := j-1 else i := j;
   if j < FILENAMELEN then j := j+1 else fatal(1);
   ifn[j] := chr(0);
   HGM*)(*MH cf := access(ifn,4); HM*)
   (*G  cf := 4-Access(ifn,4); G*)(*MGH
   if isverbose and (cf <> 0) then begin
      write(errout,' *** dpic: Searching for file "');
      for i:=1 to j-1 do write(errout,ifn[i]);
      writeln(errout,'" returns ', cf:1)
      end;
   checkfile := cf
   end; HGM*)

                                (* If pascal, open the parse table file *)
(*P2 IP*) 
(*GHF procedure openparse; FHG*)
(*DGHF var i,j: integer; FHGD*)
(*GHF begin
   if length(ParamStr(0)) > FILENAMELEN-5 then fatal(2)
   else infname := ParamStr(0) + '.tab' + chr(0); FHG*)
   (*DGHF if oflag > 0 then begin
      write(log,'Tables: '); j := FILENAMELEN+1; i := 1;
      while i < j do if infname[i]=chr(0) then j := i else begin
         write(log,infname[i]); i := i+1 end;
      writeln(log); flush(log) end; FHGD*)
   (*GH if access(infname,4) < 0 then fatal(2) else reset(dpictabl,infname) HG*)
   (*F assign(dpictabl,infname); F*) {$I-} (*F reset(dpictabl); F*) {$I+} (*F
   if IOResult<>0 then fatal(2) F*)
   (*GHF end; FHG*)
(*M procedure openparse;
var i,j: integer;
begin
   argv(0,infname);
   j := FILENAMELEN; i := 0;
   while j > i do if ((infname[j]='c') or (infname[j]='C')) then
      i := j else j := j-1; M*)(*DM
   if oflag > 0 then begin write(log,'Tables: ');
   for i:= 1 to j do write(log,infname[i]); flush(log) end; MD*)(*M
   if j < FILENAMELEN-3 then begin
      infname[j+1] := '.';
      infname[j+2] := 't'; infname[j+3] := 'a'; infname[j+4] := 'b';
      if checkfile(infname,false) <> 0 then begin
         infname[j+2] := 'T'; infname[j+3] := 'A'; infname[j+4] := 'B';
         for i := j+5 to FILENAMELEN do infname[i] := ' ';
         if checkfile(infname,true) <> 0 then fatal(2)
         end
      end;
   M*)(*DM if oflag > 0 then begin
      for i:=j+1 to j+4 do write(log,infname[i]); writeln(log)end; MD*)(*M
   reset(dpictabl,infname)
   end;
M*)
(*P2 P*)
                                (* Open the error stream *)
procedure openerror;
begin
   (*P2CIP*)
   (*MGH rewrite(errout,'/dev/stderr') HGM*)
   (*F assign(errout,'/dev/stderr'); rewrite(errout); F*)
   (*P2CP*)
   (*P2CC errout = stderr; *)
   end;
                                (* Open required input and outputs *)
(*GHM
procedure openfiles; MHG*)
(*DGHM var i,j: integer; MHGD*)
(*GHM begin
   savebuf := nil; MHG*)
   (*P2CC output = stdout; input = stdin; *)
   (*M if argct < argc then begin argv(argct,infname); M*)
   (*GH if argct <= ParamCount then begin infname := ParamStr(argct); HG*)
      (*MGH if checkfile(infname,true) <> 0 then fatal(1);
      reset(input,infname); HGM*)
      (*DGHM if oflag > 0 then begin
         openlogfile; debuglevel := oflag;
         write(log,'Input file: ');
         i := 1; j := FILENAMELEN;
         while j >= i do if (infname[j]=' ') or (infname[j]=chr(0))
            then j := j-1 else i := j+1;
         for i := 1 to j do write(log,infname[i]); writeln(log)
         end; MHGD*)
      (*GHM end; MHG*)
   (*P2 IP*)(*GHM openparse MHG*)(*P2 P*)
   (*GHM end; MHG*)
(*F
procedure openfiles;
begin F*)
(*DF if oflag > 0 then begin openlogfile; debuglevel := oflag end; FD*)
  (*F savebuf := nil;
      if argct <= ParamCount then begin
      infname := ParamStr(argct);
      assign(input,ParamStr(argct)); F*) {$I-} (*F reset(input); F*) {$I+} (*F
      if IOResult<>0 then fatal(1) F*)
      (*DF; if oflag>0 then writeln(log,'Input file: ',ParamStr(argct)) FD*)
      (*F end;
   openparse
   end;
F*)

function isprint(ch: char): boolean;
begin
   isprint := (ord(ch) >= 32) and (ord(ch) <= 126)
   end;

procedure wchar( var iou: text; c: char );
begin
   if isprint(c) then write(iou,c)
   else if c=nlch then writeln(iou)
   else if c=crch then write(iou,'\r')
   else if c=tabch then write(iou,'\t')
   else if ord(c)<32 then write(iou,'^',chr(ord(c)+64))
   else write(iou,chr(92),
      ((ord(c) div 64) mod 8):1,((ord(c) div 8) mod 8):1,(ord(c) mod 8):1)
   end;
(*------------------------------ For debug: -------------------------*)
(*D procedure logchar( c: char );
begin
   write(log,'ch(',ord(c):1,')="');
   wchar(log,c);
   write(log,'"')
   end;

procedure winbuf;
begin
   write(log,' inbuf=[',odp(inbuf):1,'] ')
   end;

procedure wlogfl D*)(*DF(nm: string; v: real; cr: integer)FD*)(*D;
begin
   write(log,' ',nm,'=');
   if (MaxReal-abs(v)) < (MaxReal*1e-6) then begin
     if v < 0 then write(log,'-MaxReal') else write(log,'MaxReal') end 
   else wfloat(log,v);
   if cr<>0 then writeln(log)
   end;

procedure logaddr D*)(*DF(b: fbufferp)FD*)(*D;
begin
   write(log,'[');
   if b <> nil then write(log,odp(b):1) else write(log,'nil');
   writeln(log,']')
   end;

procedure wrbuf D*)(*DF(p: fbufferp; job,r: integer)FD*)(*D;
var i,j,k,m: integer;
begin
  if p=nil then write(log,' nil buffer ')
  else with p@ do begin
     if job > 2 then begin
        write(log,' buf[');
        if p@.prevb<>nil then write(log,odp(p@.prevb):1);
        write(log,'<'); write(log,odp(p):1,'>');
        if p@.nextb<>nil then write(log,odp(p@.nextb):1);
        write(log,']')
        end;
     if job > 1 then
       write(log,' readx=',readx:1,' savedlen=',savedlen:1,' attrib=',attrib:1);
     if r = 0 then j := 1 else if r < 0 then j := -r else j := readx;
     if job > 0 then write(log,'(',j:1,',',savedlen:1,')');
     writeln(log);write(log,'|');
     if carray = nil then write(log,'nil')
     else begin
        i := j;
        while i <= savedlen do begin
           if ord(carray@[i])<>0 then wchar(log,carray@[i])
           else begin
              m := i; k := savedlen+1;
              while i < k do if ord(carray@[i])=0 then i:=i+1 else k:=i;
              write(log,'(',(i-m):1,')x'); wchar(log,chr(0));
              i := i-1
              end;
           i := i+1
           end
        end;
     writeln(log,'|')
     end
  end; D*)
(*--------------------------------------------------------------------*)

                                (* Write error message with symbol found
                                   and symbol expected if possible *)
procedure markerror(*F(emi: integer)F*);
var inx, j,k: integer;
  thisbuf,lastbuf: fbufferp;
begin
    if emi < 900 then errcount := errcount + 1; (* Do not count warnings *)
    (*D if debuglevel > 0 then begin
        write(log,'*** Markerror[');
        if inbuf=nil then write(log,'nil') else write(log,odp(inbuf):1);
        writeln(log,'] emi=', emi:1, ', lexsymb=', lexsymb:1,':') end; D*)
                                    (* Write out the current and prev line *)
    if emi < 903 then begin
      thisbuf := inbuf; lastbuf := nil; inx := 1;
      while thisbuf<>nil do with thisbuf@ do begin
        lastbuf := thisbuf; j := 0;
        if readx > savedlen then inx := savedlen else inx := readx-1;
        if (readx-1) < savedlen then j := readx-1 else j := savedlen;
        while inx > j do if carray@[inx] in [nlch,crch,' ',tabch,etxch] then
          inx := inx-1 else j := inx;
        j := 0;
        while inx > j do if isprint(carray@[inx]) or (carray@[inx]=tabch)
           then inx := inx-1 else j := inx;
        if inx = 0 then thisbuf := prevb else thisbuf := nil
        end;
      while lastbuf<>nil do begin
        (*D if debuglevel > 0 then wrbuf(lastbuf,3,0); D*)
        if lastbuf = inbuf then k := lastbuf@.readx-1
        else k := lastbuf@.savedlen; 
        (*D if debuglevel > 0 then writeln(log,'inx=',inx:1,' k=',k:1); D*)
        if inx < 1 then inx := 1;
        with lastbuf@ do for j := inx to k do wchar(errout,carray@[j]);
        if lastbuf<>inbuf then lastbuf := lastbuf@.nextb else lastbuf := nil;
        if lastbuf<> nil then inx := lastbuf@.readx
        end
      end;
    writeln(errout);
    write(errout,'*** dpic: line ',lineno:1,' ' );
    if emi < 900 then write(errout,'ERROR: ') else write(errout,'WARNING: ');
    if emi < 800 then begin
      case lexsymb of
        XLname: write(errout,'Name');
        XLabel: write(errout,'Label');
        XLaTeX: write(errout,
          'Backslash not followed by line end (LaTeX macro?)');
        XLfloat: write(errout,'Float');
        XERROR: write(errout,'Error');
(* include controlerr.i *)
(*GHMF#include 'controlerr.i'FMHG*)
        otherwise write(errout,'Punctuation characters')
        end;
      writeln(errout,' found.');
      writeln(errout,' The following were expected:')
      end;

    case emi of
                                (* Expected lexical symbols        *)
(* include parserr.i *)
(*GHMF#include 'parserr.i'FMHG*)
                                (* lexical error messages *)
      800: writeln(errout,'Character not recognized: ignored');
      802: writeln(errout,'Invalid exponent character after e in a number');
      804,807: begin
         write(errout,'End of file while reading ');
         if emi=807 then write(errout,'string in ');
         case currprod of 
            elsehead1:         write(errout,'else');
            ifhead1:           write(errout,'if');
            forhead1:          write(errout,'for');
            defhead1,defhead2: write(errout,'define');
            otherwise
            end;
         writeln(errout,' {...} contents')
         end;
      805: writeln(errout,'Bad macro argument number');
      806: writeln(errout,'End of file while evaluating macro argument');
                                (* context error messages *)
      851: writeln(errout,'Variable not found');
      852: writeln(errout,'Zero divisor not allowed');
      853: writeln(errout,'Only one copy file may be open at any time');
      854: writeln(errout,'Place name not found');
      855: writeln(errout,'Internal name not found');
      856: writeln(errout,'Invalid non-positive value for object count');
      857: writeln(errout,'Enumerated or previous object not found');
      858: writeln(errout,'This usage is inapplicable in this context');
      859: writeln(errout,'File not readable');
      860: writeln(errout,'Infinite looping not allowed');
      861: writeln(errout,'Missing or blank string');
      862: writeln(errout,'For ... by *() limits must have the same sign');
      863: writeln(errout,'Non-integer power of negative value');
      864: writeln(errout,'Incorrect number of sprintf arguments');
      865: writeln(errout,'Bad sprintf format; only e, f, g allowed');
      866: writeln(errout,'String exceeds max length of 4095 characters');
      867: writeln(errout,'Invalid log or sqrt argument');
      868: writeln(errout,'Function argument out of range');
      869: writeln(errout,'Improper use of logical operator');
      870: writeln(errout,'Zero value of scale not allowed');
      871: writeln(errout,'Zero second argument of pmod not allowed');
      872: writeln(errout,'Buffer overflow while defining macro argument');
      873: writeln(errout,'Format width too large');
      874: writeln(errout,'System routine snprintf error: bad formatted value');
                                (* warning messages *)
      901: writeln(errout,'Safe mode: sh, copy, and print to file disallowed');
      903: writeln(errout,'Picture size adjusted to maxpswid value');
      904: writeln(errout,'Picture size adjusted to maxpsht value');
      otherwise writeln(errout,'Unknown error')
    end; (* case *)

    (* writeln(errout); *) consoleflush;
    if errcount > MAXERRCOUNT then fatal(3)
end; (* markerror *)

(*--------------------------------------------------------------------*)
                                (* Initialize random number routine *)
procedure initrandom;
 (* Comment equired by p2c *)
(*F var seed: SmallInt; F*)
(*M var seed: integer; M*)
(*GH var t: TimeStamp; HG*)
begin
   (*MF srandom(time(seed)) FM*)
   (*GH GetTimeStamp(t);
       srandom(t.Microsecond) HG*)
   end;

                                (* Interface routine to push terminal
                                   attributes onto the attribute stack
                                   for use by semantic actions *)
procedure stackattribute( stackp: stackinx );
begin
   with attstack@[stackp] do begin
      (* D if debuglevel = 2 then writeln(log,
         'stackp=',stackp:1,' lexsymb=',lexsymb:1 ); D *)
      chbufx := oldbufi;
      length := chbufi-oldbufi;
      prim := nil;
      internal := nil;
      varname := nil;
      xval := floatvalue;
      yval := 0.0;
      startchop := 0.0;
      endchop := 0.0;
      state := 0;
      lexval := lexsymb
      end
   end; (* stackattribute *)
                                (* End of macro found *)
procedure exitmacro;
var a (*,lastarg *): argp;
begin
  (*D if debuglevel > 0 then writeln(log,' exitmacro'); D*)
  a := args;
  if args <> nil then args := args@.highera;  (* first get rid of args *)
  disposeargs(a)
  (* while a <> nil do begin
    disposebufs(a@.argbody ( D,1D ) );
    lastarg := a; a := a@.nexta; dispose(lastarg)
    end *)
  end;
                                (* Read a line from the input *)
procedure readline(var infname: text);
begin
(*P2CIP*)
   if inputeof then begin end
   else if eof(infname) then inputeof := true
   else with inbuf@ do begin
      while (not eoln(infname)) and (savedlen < CHBUFSIZ-1) do begin
         savedlen := savedlen + 1;
         read(infname, carray@[savedlen]) end;
      if (savedlen = CHBUFSIZ-1) and not eoln(infname) then begin
         savedlen := savedlen + 1;
         carray@[savedlen] := bslch
         end
      else begin
         while (savedlen>0) and (carray@[savedlen]=crch) do
            savedlen := savedlen-1;
         if (carray@[savedlen]<>bslch) or (lexstate<>2) then begin
            savedlen := savedlen + 1;
            carray@[savedlen] := nlch;
            end;
         readln(infname)
         end
      end
(*P2CP*)
(*P2CC
   int ll,c;
   if (feof( *infname )) |* inputeof = true; return; *|
   c = ' ';
   for (ll = CHBUFSIZ-1; inbuf->savedlen < ll; ) |*
      c = getc( *infname );
      if ((char)c == '\n') ll = inbuf->savedlen;
      else if ((char)c == '\r') |*
         if ((char)(c=getc( *infname )) != '\n') |*
            if (c != EOF) ungetc(c, *infname );
            else if (inbuf->savedlen == 0) |* inputeof = true; return; *|
            c = '\n'; *|
         ll = inbuf->savedlen; *|
      else if (c != EOF) |*
         inbuf->savedlen++; inbuf->carray[inbuf->savedlen] = (char)c; *|
      else if (inbuf->savedlen == 0) |* inputeof = true; return; *|
      else ll = inbuf->savedlen;
      *|
   if ((ll == CHBUFSIZ-1) && ((char)c != '\n') && (c != EOF)) |*
      inbuf->savedlen++; inbuf->carray[inbuf->savedlen] = bslch; *|
   else if (inbuf->carray[inbuf->savedlen] != bslch || lexstate != 2) |*
      inbuf->savedlen++;
      inbuf->carray[inbuf->savedlen] = nlch; *|
   *)

   (*D; if debuglevel > 0 then begin writeln(log); write(log,'readline ');
      if inputeof then writeln(log,'inputeof') else wrbuf(inbuf,3,0) end D*)
   end;
                                (* Get the next line and set lexstate *)
procedure nextline(lastchar: char);
var i: integer;
begin
   (*D if debuglevel > 0 then begin writeln(log);
      write(log,'nextline('); wchar(log,lastchar);
      write(log,') ')
      end; D*)
   with inbuf@ do begin readx := 1; savedlen := 0 end;
   repeat
      if savebuf<>nil then begin
         readline(copyin);
         if inputeof then begin
            inputeof := false;
            while inbuf@.prevb<>nil do inbuf := inbuf@.prevb;
            disposebufs(inbuf (*D,11D*) );
            inbuf := savebuf; savebuf := nil
            end
         end
      else begin
         if not inputeof then lineno := lineno+1;
         readline(input) end;
                                (* Check for .PS, .PE, and zero length *)
      with inbuf@ do if savedlen < 1 then begin end
      else if carray@[1] <> '.' then begin end
      else if lexstate<>2 then begin
         if (savedlen < 4) or (carray@[2] <> 'P') then begin end
         else if savebuf<>nil then savedlen := 0  (* skip .P* lines *)
         else if carray@[3] in ['S','F'] then begin lexstate:= 1; readx := 4 end
         else if carray@[3] = 'E' then begin lexstate := 3; readx := 4 end
         end
      else if lastchar=bslch then begin end
      else if (savedlen < 4) or (carray@[2]<>'P') or (savebuf<>nil) then
         savedlen := 0  (* skip . lines *)
      else if carray@[3] in ['S','F'] then begin lexstate:= 1; readx := 4 end
      else if carray@[3] = 'E' then begin lexstate := 3; readx := 4 end
      else savedlen := 0;

      (*D if linesignal > 0 then begin
            i := lineno div (10*linesignal);
            if lineno = i*10*linesignal then writeln(errout,'lineno ',lineno:1)
            end;
         if debuglevel > 1 then begin
            writeln(log); writeln(log,'lineno ',lineno:1,':') end; D*)
                                (* Dump the line if not between .PS and .PE *)
      if (lexstate = 0) and not inputeof then with inbuf@ do begin
         for i := 1 to savedlen-1 do write(carray@[i]);
         if carray@[savedlen]<>bslch then writeln;
         savedlen := 0
         end
   until (inbuf@.savedlen > 0) or inputeof
   (*D; if debuglevel > 0 then begin
      writeln(log); write(log,'nextline |');
      with inbuf@ do for i:=1 to savedlen do wchar(log,carray@[i]);
      writeln(log,'| lexstate=',lexstate:1)
      end D*)
   end;
                                (* Read the next char into ch, accounting for
                                   strings and end of buffer *)
procedure inchar;
var tp: fbufferp;
   endofbuf: boolean;
begin
   if inbuf=nil then begin newbuf(inbuf); inbuf@.attrib := -1;
      topbuf := inbuf end;
   (*D if debuglevel = 2 then with inbuf@ do begin
      writeln(log);
      write(log,' inchar['); if prevb<>nil then
        write(log,odp(prevb):1);
      write(log,'<',odp(inbuf):1,'>');
      if nextb<>nil then write(log,odp(nextb):1);
      write(log,']: instr=',instr,' rx=',readx:1) end; D*)

   endofbuf := inbuf@.readx >= inbuf@.savedlen;
   while endofbuf do begin
      with inbuf@ do if (readx < savedlen) or inputeof then endofbuf := false
      else if readx > savedlen then begin end
      else if (carray@[readx] <> bslch) or (nextb <> nil) then endofbuf := false
      else if instr then begin
         if savedlen < CHBUFSIZ then begin savedlen := savedlen+1;
            carray@[savedlen] := nlch end
         else if attrib > 0 then begin
            newbuf(tp); with tp@ do begin (* for *)
               attrib := inbuf@.attrib; prevb := inbuf;
               savedlen := 1; carray@[1] := nlch end;
            nextb := tp end
         else begin
            readx := readx-1; carray@[readx] := bslch;
            carray@[savedlen] := nlch end;
         endofbuf := false
         end;
      if endofbuf then begin
         if inbuf@.nextb <> nil then begin
            inbuf := inbuf@.nextb
            end
         else if inbuf@.attrib > 0 then begin   (* for buf *)
            (*D if debuglevel = 2 then begin
              write(log,' For detected, '); logchar(ch); write(log,' ')
              end; D*)
            inbuf@.readx := 1;
            while inbuf@.prevb <> nil do begin
               inbuf := inbuf@.prevb; inbuf@.readx := 1
               end;
            forbufend := true
            end
         else begin                             (* This is not a loop *)
            while inbuf@.prevb <> nil do inbuf := inbuf@.prevb;
            if inbuf@.nextb<>nil then disposebufs(inbuf@.nextb (*D,10D*) );
            if inbuf@.higherb<>nil then begin
               tp := inbuf@.higherb; disposebufs(inbuf (*D,3D*)); inbuf := tp
               (* tp := inbuf; inbuf := inbuf@.higherb;
               disposebufs(tp ( *D,3D* )) *)
               end
            else if not inputeof then with inbuf@ do begin
               if (savedlen < 1) or (inbuf <> topbuf) then nextline(' ')
               else nextline( carray@[savedlen] )
               end
            end
         end
      end;

   if forbufend or inputeof then ch := nlch
   else with inbuf@ do begin ch := carray@[readx]; readx := readx + 1 end

   (*D; if debuglevel = 2 then with inbuf@ do begin
      write(log,' savedlen=',savedlen:1,' ');
      if inputeof then write(log,'inputeof ');
      logchar(ch); write(log,' readx=',readx:1)
      end D*)
   end; (* inchar *)

                                (* skip to end of the current input line *)
procedure skiptoend;
var skip: boolean;
begin
   (*D if debuglevel>1 then begin writeln(log);
       writeln(log,'skiptoend:'); winbuf end; D*)
   skip := true;
   while skip do if inbuf = nil then skip := false else with inbuf@ do begin
      (*D if (debuglevel>1) and (carray@[readx]=nlch) then
          write(log,'carray@[',readx:1,']=',ord(carray@[readx]):1); D*)
      if readx > savedlen then begin end
      else if carray@[readx] = nlch then skip := false
      else if carray@[readx] = etxch then exitmacro;
      readx := readx+1;
      if readx <= savedlen then begin end
      else if inbuf@.nextb<>nil then begin
         inbuf := inbuf@.nextb
         end
      else skip := false
      end
   (*D; if (debuglevel>1) then writeln(log,' readx=',inbuf@.readx:1) D*)
   end;
                                (* Move back in chbuf *)
procedure backup;
begin
   with inbuf@ do readx := readx-1;
   ch := ' '
   end;

(*--------------------------------------------------------------------*)

(*P2CIP*)
procedure initchars;
begin
   digits := ['0'..'9'];
   namech := ['0'..'9', 'A'..'I', 'J'..'R', 'S'..'Z', '_',
      'a'..'i', 'j'..'r', 's'..'z'];
   uppercase := ['A'..'I', 'J'..'R', 'S'..'Z']
   end; (*P2CP*)

(*P2 IP*)
procedure inittables;
var
    cc, ch1: char;
    i: integer;
begin (* inittables *)
                                (* Initialize and read in lexical tree *)
    for i := ordMINCH to ordMAXCH do begin
       entryhp[i] := 0;
       entrytv[i] := 0
       end;
    read(dpictabl, cc, cc, i);
    repeat
       ch1 := cc;
       readln(dpictabl, entryhp[ord(cc)], entrytv[ord(cc)]);
       read(dpictabl, cc, cc, i)
       until cc = ch1;
    readln(dpictabl);

    for i := 0 to lxmax do
        readln(dpictabl, cc, lxch[i], lxnp[i], lxhp[i], lxtv[i]);

                                (* lalr parser table *)
    i := 0;
    while i <= lrmax do begin
       read(dpictabl, lr[i], lr[i+next], lr[i+kind]);
       case lr[i+kind] of
          1, 2, 4, 6: read(dpictabl, lr[i+symb], lr[i+err]);
          0, 3: read(dpictabl, lr[i+rs], lr[i+prod]);
          5: begin read(dpictabl, lr[i+lb]); i := i-1 end
          end;
       if eoln(dpictabl) then readln(dpictabl);
       i := i+5
       end
   end; (* inittables *)(*P2 P*)

(*--------------------------------------------------------------------*)

                                (* Copy ch char into chbuf and get new ch *)
procedure pushch;
begin
   chbuf@[chbufi] := ch;
   if chbufi < CHBUFSIZ then chbufi := chbufi + 1
   else fatal(4);  (* Leave 1 location free at the end of chbuf@ *)
   inchar
   end; (*pushch*)
                                (* Read complete line into chbuf *)
procedure readlatex;
begin
   while ch <> nlch do pushch;
   newsymb := XLaTeX
   end;

                                (* Pascal C-equivalents *)
(*P2CIP*)
function isdigit(ch: char): boolean;
begin
   isdigit := ch in digits
   end;
function isalnum(ch: char): boolean;
begin
   isalnum := ch in namech
   end;
function isupper(ch: char): boolean;
begin
   isupper := ch in uppercase
   end;
(*P2CP*)

function argcount(a: argp): integer;
var i: integer;
begin
   if a=nil then markerror(805);
   i := 0;
   while a<>nil do begin
      if a@.argbody <> nil then i := i+1;
      a := a@.nexta
      end;
   argcount := i
   end;
                                (* Find the k-th argument *)
function findarg( arlst: argp; k: integer): argp;
var ar: argp;
   i,j: integer;
begin
   (*D if debuglevel > 0 then begin writeln(log);
       write(log, 'findarg(');
       if arlst = nil then write(log,'nil') else write(log,odp(arlst):1);
       write(log,',',k:0,'):') end; D*)
   if k > 0 then ar := arlst else ar := nil;
   i := 1; j := k;
   while i < j do
      if ar <> nil then begin ar := ar@.nexta; i := i+1 end
      else j := i;
   (*D if debuglevel > 0 then begin
      if (i<>k) or (ar=nil) then writeln(log,'not found')
      else wrbuf(ar@.argbody,2,1) end; D*)
   findarg := ar
   end;

(*P2CC #ifndef SAFE_MODE *)
(*GHMF
procedure pointinput FMHG*)(*F(txt: strptr)F*)(*GHMF;
var i: integer;
  c: char;
begin
   if txt = nil then begin end
   else if txt@.segmnt <> nil then with txt@ do begin
      c := FMHG*)(*GHM ' ' MHG*)(*F chr(0) F*)(*GHMF ;
      if len >= FILENAMELEN then len := FILENAMELEN-1;
      for i := 1 to len do infname[i] := segmnt@[seginx-1+i];
      for i := len+1 to FILENAMELEN do infname[i] := c; FMHG*)
      (*DGHMF if debuglevel > 0 then begin
         write(log,'Pointinput(segmnt ',seginx:1,', len ',len:1,') ');
         for i := 1 to len do write(log,infname[i]); writeln(log) end; FMHGD*)
      (*GHM if savebuf<>nil then markerror(853)
      else if checkfile(infname,true) <> 0 then markerror(859)
      else begin
         reset(copyin,infname);
         backup; ch := nlch; savebuf := inbuf; inbuf := nil
         end MHG*)
      (*F if savebuf<>nil then markerror(853)
      else begin
         assign(copyin,infname); F*) {$I-} (*F reset(copyin); F*) {$I+} (*F
         if IOResult<>0 then fatal(2);
         backup; ch := nlch; savebuf := inbuf; inbuf := nil
         end F*)
      (*GHMF end
   end; FMHG*)
                                (* Redirect output *)
(*GHMF
procedure pointoutput FMHG*)(*F(nw:boolean;txt:strptr;var ier:integer)F*)
(*GHMF ;
var i: integer;
begin
   ier := 1;
   if txt@.segmnt <> nil then with txt@ do begin
      if len >= FILENAMELEN then len := FILENAMELEN-1;
      for i := 1 to len do begin
         outfnam[i] := segmnt@[seginx-1+i];
         if ier=1 then begin
            if isprint(outfnam[i]) and (outfnam[i]<>' ') then ier := 0
            end
         end;
      outfnam[len+1] := chr(0); FMHG*)
      (*DGHMF if debuglevel > 0 then begin
         write(log,'Pointoutput(',nw,' segmnt ',seginx:1,', len ',len:1,') "');
         for i := 1 to len do write(log,outfnam[i]); writeln(log,'"');
         flush(log) end; FMHGD*)
      (*GHM if ier<>0 then markerror(861)
      else if nw then rewrite(redirect,outfnam)
      else append(redirect,outfnam) MHG*)
      (*F if ier<>0 then markerror(861)
      else begin
        assign(redirect,outfnam);
        if nw then rewrite(redirect) else append(redirect)
        end F*)
      (*GHMF end
   end; FMHG*)
(*P2CC #endif *)
                                (* Read string terminal into chbuf *)
procedure readstring;
var n (*D,jD*) : integer;
   ar: argp;
   abuf: fbufferp;
   c: char;
begin
   (*D if debuglevel > 0 then begin
      writeln(log); write(log,'readstring '); j := chbufi end; D*)
   instr := true;
   repeat
      c := ch; pushch;
      if (c=bslch) and (ch='"') then begin chbufi := chbufi-1; pushch end
      else if c = '$' then begin
         n := 0;
         if args<> nil then while isdigit(ch) do begin
            n :=10*n+ord(ch)-ord('0');
            inchar
            end;
         if n > 0 then begin
            ar := findarg(args,n);
            chbufi := chbufi-1;
            if ar<>nil then begin
               abuf := ar@.argbody;
               while abuf <> nil do with abuf@ do begin
                  for n:=readx to savedlen do begin
                     chbuf@[chbufi] := carray@[n];
                     if chbufi < CHBUFSIZ then chbufi := chbufi+1 else fatal(4)
                     end;
                  abuf := abuf@.nextb
                  end
               end
            end
         end
   until (c='"') or inputeof;
   if not inputeof then chbufi := chbufi-1;
   instr := false
   (*D; if debuglevel > 0 then begin
      write(log,'done, chbufi=',chbufi:1,' |');
      while j < chbufi do begin write(log,chbuf@[j]); j := j+1 end;
      write(log,'| ');
      if inputeof then write(log,'inputeof ');
      wchar(log,ch); writeln(log) end D*)
   end;

procedure readexponent;
var neg: boolean;
   k: integer;
begin
   pushch;
   if ch = '-' then begin neg := true; pushch end
   else if ch = '+' then begin neg := false; pushch end
   else neg := false;
   if not isdigit(ch) then markerror(802)
   else begin
      k := 0;
      repeat
         k := k*10 + (ord(ch)-ord('0'));
         pushch
      until not isdigit(ch);
      if neg then floatvalue := floatvalue*exp(-k*ln10)
      else floatvalue := floatvalue*exp(k*ln10)
      end
   end; (* readexponent *)

procedure readfraction;
var x: real;
begin
   x := 10.0;
   while isdigit(ch) do begin
      floatvalue := floatvalue + (ord(ch)-ord('0'))/x;
      x := x*10;
      pushch
      end;
   end; (* readfraction *)

procedure readconst( initch: char);
begin
   (*D if debuglevel=2 then begin writeln(log);
      write(log,' readconst(',initch,') readx=',inbuf@.readx:1,' ');
        logchar(ch); writeln(log) end; D*)
   if( initch = '.' ) then readfraction
   else begin
      while isdigit(ch) do begin
         floatvalue := 10*floatvalue + (ord(ch)-ord('0'));
         pushch
         end;
      if ch = '.' then begin
         pushch;
         readfraction
         end
      end;
   if (ch = 'e') or (ch = 'E') then readexponent;
   if ch = 'i' then pushch;     (* A rather odd way to allow inch units *)
   newsymb := XLfloat
   end;

(*D
procedure prlex(acc: boolean);
var i: integer;
begin
   if debuglevel > 0 then begin
      writeln(log);
      write(log, 'LEXICAL(oldsymb=',oldsymb:1,' newsymb=',newsymb:1,')');
      if newsymb <> XLaTeX then begin
         write(log,' chbuf(',oldbufi:1,':',chbufi-1:1,')="');
         for i := oldbufi to chbufi-1 do write(log, chbuf@[i]);
         write(log,'"') end
      else write(log,'=<LaTeX>');
      if newsymb = XLfloat then wlogfl('value',floatvalue,0);
      write(log,' '); logchar(ch);
      if not acc then write( log,' not accepted');
      writeln( log );
      consoleflush
      end
   end; D*)
                                (* Go up one buffer level *)
function nbuf(buf: fbufferp): fbufferp;
begin
   if buf@.prevb=nil then newbuf(buf@.prevb);
   with buf@.prevb@ do begin
      attrib := buf@.attrib; savedlen := CHBUFSIZ;
      readx := CHBUFSIZ+1; nextb := buf end;
   nbuf := buf@.prevb
   end;
                                (* Push macro or arg or string from mac into
                                   the head of the input stream *)
procedure copyleft(*F(mac: fbufferp; var buf: fbufferp)F*);
var ml: fbufferp;
   i,k: integer;
   newflag, copied: boolean;
begin
   (*D if debuglevel > 0 then writeln(log, 'copyleft:'); D*)
   (*D if debuglevel > 1 then begin
      write(log,' input string'); wrbuf(mac,3,1);
      write(log,' output'); wrbuf(buf,3,1) end; D*)
   ml := mac; while ml <> nil do begin mac := ml; ml := ml@.nextb end;
   copied := false;
   while mac <> nil do begin
      if mac@.carray<>nil then begin
         if mac@.savedlen >= mac@.readx then begin
            copied := true;
            if buf = nil then newflag := true
            else if buf@.attrib >= 0 then newflag := true  (* for *)
            else newflag := false;
            (*D if debuglevel > 0 then
               writeln(log,'newflag=',newflag:1,' attrib=',buf@.attrib:1); D*)
            if newflag then begin
               newbuf(ml); with ml@ do begin
                  attrib := -1; savedlen := CHBUFSIZ; readx := CHBUFSIZ+1;
                  higherb := buf end;
               buf := ml
               end
            end;
         k := mac@.savedlen;
         if (buf@.readx - 1) < (k-mac@.readx+1) then begin (*not enough space*)
            with buf@ do while readx > 1 do begin
               readx := readx-1; carray@[readx] := mac@.carray@[k]; k := k-1
               end;
            buf := nbuf(buf)
            end;
         with buf@ do for i:=k downto mac@.readx do begin
            readx := readx-1; carray@[readx] := mac@.carray@[i] end
         end;
      mac := mac@.prevb
      end;
   if copied then begin
     if buf@.readx <= 1 then buf := nbuf(buf);
     with buf@ do carray@[readx-1] := nlch
     end;
   (*D; if debuglevel > 0 then begin
      ml := buf; writeln(log,' copyleft result'); while ml <> nil do begin
         wrbuf(ml,3,1); ml := ml@.nextb end end D*)
   end;

                                (* $n has been seen in a macro argument;
                                   copy the body into the tail of the input
                                   buffer *)
procedure copyright( mac: fbufferp; var buf: fbufferp );
var ml: fbufferp;
   i,k: integer;
begin
   (*D if debuglevel > 0 then begin
      writeln(log, 'copyright:');
      write(log,' input'); wrbuf(mac,3,1);
      write(log,' output'); wrbuf(buf,3,0) end; D*)
   while mac <> nil do begin
      if buf = nil then begin
         newbuf(buf); with buf@ do begin
            attrib := -1; savedlen := 0; readx := 1 end;
         end;
      k := mac@.readx;
      if (CHBUFSIZ-buf@.savedlen) < (mac@.savedlen-k+1) then begin
         with buf@ do while savedlen < CHBUFSIZ do begin
            savedlen := savedlen+1; carray@[savedlen] := mac@.carray@[k];
            k := k+1 end;
         newbuf(ml); with ml@ do begin attrib := buf@.attrib; prevb := buf end;
         buf@.nextb := ml; buf := ml
         end;
      with buf@ do for i:=k to mac@.savedlen do begin
         savedlen := savedlen+1; carray@[savedlen] := mac@.carray@[i] end;
      mac := mac@.nextb
      end
   (*D; if debuglevel > 0 then with buf@ do begin
      write(log,' result'); wrbuf(buf,3,0) end D*)
   end;
                                (* Check the current char for line
                                   continuation *)
procedure skipcontinue(instrg: boolean);
var c: char;
begin
   (*D if debuglevel=2 then begin writeln(log);
      write(log,' skipcontinue readx=',inbuf@.readx:1,' ') end; D*)
   c := ch;
   while c = bslch do with inbuf@ do begin
      if readx <= savedlen then c := carray@[readx]
      else if nextb = nil then begin inchar; c := ch end
      else c := nextb@.carray@[nextb@.readx];
      if c = nlch then begin inchar; inchar; c := ch end
      (* else if c = '#' then begin *)
      else if (c = '#') and (not instrg) then begin
         skiptoend; inchar; c := ch end
      else c := ' '
      end
   end;
                                (* Skip white space characters *)
procedure skipwhite;
begin
   (* D if debuglevel = 2 then writeln(log, 'skipwhite: ' ); D *)
   while ch in [' ', tabch, nlch,etxch] do begin
      if ch=etxch then exitmacro;
      inchar;
      if ch = bslch then skipcontinue(false)
      end
   end;
                                (* Stash the current argument into the arg
                                   struct*)
procedure defineargbody( var parenlevel: integer; var p2: fbufferp );
var j,n: integer;
   ar: argp;
   p,p1: fbufferp;
   inarg,instring: boolean;
   prevch: char;
begin
   (*D if debuglevel > 0 then begin writeln(log);
       write(log, 'defineargbody: parenlevel=',parenlevel:1) end; D*)
   skipwhite;
   p1 := nil;
   if parenlevel >= 0 then inarg := true else inarg := false;
   while inarg do begin
      newbuf(p);
      p@.attrib := -1;
      if p1 <> nil then begin p1@.nextb := p; p@.prevb := p1 end;
      p1 := p;
      j := CHBUFSIZ; instring := false; prevch := ' ';
      repeat with p1@ do begin
         if prevch = bslch then begin end
         else if instring and (ch='"') then instring := false
         else if ch = '"' then instring := true;
         if instring then begin end
         else if ch = '(' then parenlevel := parenlevel+1
         else if ch = ')' then parenlevel := parenlevel-1;
         (*D if debuglevel=2 then write(log,' instring=',instring,' '); D*)
         if (not instring) and
            ((parenlevel < 0) or ((parenlevel=0) and (ch = ','))) then begin
            j := savedlen; inarg := false end
         else if ch <> '$' then begin
            prevch := ch;
            if savedlen < CHBUFSIZ then savedlen := savedlen + 1
            else markerror(872);
            carray@[savedlen] := ch;
            (*D if debuglevel = 2 then begin
               write(log,'defineargbody2: savedlen=',savedlen:1,' ');
               logchar(ch); writeln(log,' parenlevel=',parenlevel:1)
               end; D*)
            inchar
            end
         else begin                      (* $ found in a macro arg *)
            (*D if debuglevel=2 then write(log,' defineargbody3'); D*)
            prevch := ch; inchar;
            if isdigit(ch) then begin
               n := 0; 
               repeat n :=10*n+ord(ch)-ord('0'); inchar until not isdigit(ch);
               ar := findarg(args,n);
               if ar<>nil then copyright(ar@.argbody,p1)
               end
            else begin
               if savedlen < CHBUFSIZ then savedlen := savedlen + 1
               else markerror(872);
               carray@[savedlen] := prevch;
               (*D if debuglevel = 2 then begin
                   write(log,'defineargbody2: savedlen=',savedlen:1,' ');
                   logchar(ch); writeln(log) end; D*)
               end
            end;
         if inputeof then begin
            markerror(806); j := savedlen; inarg := false; parenlevel := -1
            end
         end
      until p1@.savedlen = j
      end;
   if p1 <> nil then while p1@.prevb <> nil do p1 := p1@.prevb;
   ch := ' ';
   p2 := p1
   (*D; if debuglevel > 0 then with p1@ do begin
      write(log,' defineargbody'); wrbuf(p1,3,0) end D*)
   end;

                                (* Check for macro name *)
function ismacro(chb: chbufp; obi,chbi: chbufinx): boolean;
var mac,lastp: argp;
   ar,lastarg,firstarg: argp;
   level: integer;
   ism: boolean;
begin
   (*D if debuglevel > 0 then begin
       writeln(log); write(log, 'ismacro[');
       for level:=obi to chbi-1 do write(log,chb@[level]);
       write(log,']:') end;
       D*)
   ism := false;
   if oldsymb = XLdefine then skipwhite
   else if oldsymb <> XLundefine then begin
      mac := findmacro(macros, chb, obi,chbi-obi,lastp);
      if mac <> nil then begin
         ism := true; firstarg := nil; lastarg := nil;
         if ch = '(' then level := 0
         else begin backup; level := -1 end;
         repeat
            newarg(ar); ar@.highera := args;
            if lastarg = nil then firstarg := ar else lastarg@.nexta := ar;
            if level >= 0 then begin
               inchar; defineargbody( level,ar@.argbody ) end;
            lastarg := ar
         until level < 0;
         args := firstarg;
         copyleft(mac@.argbody,inbuf)
         end
      end;
   (*D if debuglevel > 0 then begin
       writeln(log); writeln(log,'ismacro returning:',ism) end; D*)
   ismacro := ism
   end;

                                (* Push an argument to the left of the
                                   input stream *)
procedure copyarg(chb: chbufp; chbs,chbi: chbufinx);
var n,i: integer;
   ar: argp;
begin
   (*D if debuglevel > 0 then begin write(log,'copyarg(');
      for i:= chbs to chbi-1 do write(log,chb@[i]); write(log,') ') end; D*)
   n := 0; for i := chbs+1 to chbi-1 do n := 10*n + ord(chb@[i])-ord('0');
   ar := findarg( args,n );
   backup;
   if ar=nil then begin end
   else if ar@.argbody<>nil then copyleft(ar@.argbody,inbuf)
   end;
                                (* Check for $+ or copy arg *)
function insertarg: boolean;
var icx: integer;
begin
   pushch;
   if ch = '+' then begin
      backup; ch := '$'; chbufi := chbufi-1;
      insertarg := false
      end
   else begin
      icx := chbufi-1;
      while isdigit(ch) do pushch;
      copyarg(chbuf,icx,chbufi);
      chbufi := icx;
      inchar;
      insertarg := true
      end
   end;
                                (* Find the next terminal.
                                   Set lexsymb, lexval, and float value.
                                   Identify and handle all terminals
                                   of the form <...> in the grammar.
                                   Reads one character after the terminal end *)
procedure lexical;
var
   lxix: integer;
   firstch: char;
   terminalaccepted,looping: boolean;

   (* argstruct: argp;
   j,k: integer;
   varptr,lastp: strptr; *)

begin (*lexical*)
   floatvalue := 0.0;
   lexsymb := -1;
   repeat
      terminalaccepted := true;
      oldbufi := chbufi;    (* first char of current terminal in chbuf *)
      while (ch = ' ') or (ch = tabch) do inchar;

                                (* lexstate is
                                     0 before .PS or after .PE
                                     1 when .PS found
                                     2 between .PS and .PE
                                     3 when .PE found
                                     4 to process .PE *)
      if lexstate = 1 then begin
         newsymb := XSTART;
         lexstate := 2
         end
      else if lexstate = 3 then begin
         (*D if linesignal > 0 then begin
            writeln(errout,'.PE'); consoleflush end; D*)
         newsymb := XEND;
         lexstate := 4
         end
      else if lexstate = 4 then begin
         newsymb := XNL;
         skiptoend;
         ch := ' ';
         lexstate := 0
         end
      else if inputeof then begin
         if oldsymb = 0 then fatal(5)
         else if oldsymb = 1 then newsymb := 0
         else newsymb := 1
         end

      else if forbufend then begin
         newsymb := XLendfor;
         forbufend := false;
         ch := ' '
         end
      else if ch=nlch then begin
         newsymb := XNL;
         ch := ' ';
         if (oldsymb in [XNL,XCOLON,XLthen,XLBRACE,XLelse]) then
            terminalaccepted := false
         end
      else if isdigit(ch) then readconst(ch)
      else if ch = etxch then begin
         exitmacro;
         inchar;
         terminalaccepted := false
         end

                                (* Use the lexical tables to identify
                                   terminals, checking for macro names *)
      else begin
         firstch := ch;
         pushch;
                                (* Continuation, comment, constant, latex *)
         with inbuf@ do if (firstch = bslch) then begin
            if (ch = nlch) or (ch = '#') then begin
               if ch = '#' then skiptoend;
               ch := ' '; newsymb := -2; (* flag for test below *)
               terminalaccepted := false
               end
            else readlatex
            end
         else if (firstch = '.') and isdigit(ch) then readconst( firstch )
         else if (firstch = '.') and (readx = 3) and (inbuf@.prevb = nil) and
            (newsymb <> -2) then readlatex
                                (* Search in the lexical tree *)
         else begin
            newsymb := entrytv[abs(ord(firstch))];
            lxix := entryhp[abs(ord(firstch))];
            while lxix <> 0 do
               if ch='$' then begin if not insertarg then lxix := 0 end
               else if lxch[lxix] = ch then begin
                  newsymb := lxtv[lxix];
                  lxix := lxhp[lxix];
                  pushch
                  end
               else lxix := lxnp[lxix];

                                (* Insert argument or macro *)
            if isupper(firstch) and (isalnum(ch) or (ch='$')) then begin
                                (* Label *)
               looping := true; while looping do
                  if ch='$' then looping := insertarg
                  else if isalnum(ch) then pushch else looping := false;
               if ismacro(chbuf,oldbufi,chbufi) then terminalaccepted := false
               else newsymb := XLabel
               end
            else if isalnum(firstch) and (isalnum(ch) or (ch='$')) then begin
                                (* variable name *)
               looping := true; while looping do if ch='$' then
                     looping := insertarg
                  else if isalnum(ch) then pushch
                  else looping := false;
               if ismacro(chbuf,oldbufi,chbufi) then terminalaccepted := false
               else newsymb := XLname
               end
            else if newsymb=XLstring then begin
               chbufi := chbufi-1;
               readstring
               end
            else if newsymb=XCOMMENT then begin
               skiptoend;
               ch := nlch;
               terminalaccepted := false
               end
                                (* Skip after designated terminals *)
            else if (newsymb = XNL) and
               (oldsymb in [XNL,XCOLON,XLthen,XLBRACE,XLelse]) then
               terminalaccepted := false
            else if (newsymb = XLT) and inlogic then begin
               lexsymb := XLcompare; newsymb := XLcompare end
            else if (newsymb > XEQ) and (newsymb <= XLremeq) then begin
               lexsymb := newsymb; newsymb := XEQ end
                                (* Multiple-valued terminals *)
            else if newsymb > XLcorner then begin
               lexsymb := newsymb;
               if      newsymb > XLenvvar    then newsymb:=XLenvvar
               else if newsymb > XLprimitiv  then newsymb:=XLprimitiv
               else if newsymb > XLdirecton  then newsymb:=XLdirecton
               else if newsymb > XLarrowhd   then newsymb:=XLarrowhd
               else if newsymb > XLtextpos   then newsymb:=XLtextpos
               else if newsymb > XLcolrspec  then newsymb:=XLcolrspec
               else if newsymb > XLlinetype  then newsymb:=XLlinetype
               else if newsymb > XLfunc2     then newsymb:=XLfunc2
               else if newsymb > XLfunc1     then newsymb:=XLfunc1
               else if newsymb > XLparam     then newsymb:=XLparam
               else if newsymb > XLcompare   then newsymb:=XLcompare
               else newsymb := XLcorner
               end
            else if newsymb = XLarg then begin
               if ch='+' then begin                                   (* $+ *)
                  floatvalue := argcount(args);
                  newsymb := XLfloat;
                  inchar
                  end
               else if isdigit(ch) then begin                 (* $<integer> *)
                  repeat pushch until not isdigit(ch);
                  copyarg(chbuf,oldbufi,chbufi);
                  terminalaccepted := false
                  end
               else (* if not isalnum(ch) then *) markerror(805)   (* $name *)
               (* else begin
                  repeat pushch until not isalnum(ch);
                  backup;
                  varptr := glfindname(envblock,chbuf,oldbufi+1,
                     chbufi-oldbufi-1,lastp,k);
                  if varptr=nil then markerror(805) else begin 
                     j := round(varptr@.val); argstruct := findarg( args,j );
                     if argstruct=nil then markerror(805)
                     else if argstruct@.argbody<>nil then 
                        copyleft(argstruct@.argbody,inbuf)
                     end;
                  terminalaccepted := false
                  end *)
               end
            else if newsymb = XLdo then skipwhite
            (*D else if newsymb = XAND then begin
               chbufi := oldbufi;
               if debuglevel > 0 then consoleflush;
               if ord(ch) >=ord('A') then
                  linesignal := 1+ord(ch)-ord('A')
               else if ord(ch) > ord('0') then begin
                  if oflag <= 0 then openlogfile;
                  debuglevel := ord(ch)-ord('0')
                  end
               else if ord(ch) = ord('0') then debuglevel := 0;
               writeln(log,'Debuglevel = ',debuglevel:1,
                  ' linesignal = ',linesignal:1); consoleflush;
               inchar;
               terminalaccepted := false
               end D*)
            else if (newsymb = 0) and isupper(firstch) then begin
                                (* Label, second possibility *)
               if ismacro(chbuf,oldbufi,chbufi) then terminalaccepted := false
               else newsymb := XLabel
               end
            else if (newsymb = 0) and isalnum(firstch) then begin
                                (* name, second possibility *)
               if ismacro(chbuf,oldbufi,chbufi) then terminalaccepted := false
               else newsymb := XLname
               end
            else if newsymb = 0 then begin
                                (* char not recognized *)
               (*D if debuglevel > 0 then writeln(log,
                    'Marking 800:ord(firstch)=',ord(firstch),
                    ' ord(ch)=',ord(ch)); D*)
               write(errout,'Char chr(',ord(firstch):1,')');
               write(errout,'"'); wchar(errout,firstch); write(errout,'"');
               writeln(errout,' unknown');
               markerror(800);
               terminalaccepted := false
               end
            end (*lookahead terminals*)
         end;

      (*D prlex( terminalaccepted ); D*)
      if not (newsymb in [XLname,XLabel,XLstring,XLaTeX]) then
         chbufi := oldbufi
      until terminalaccepted;
   if lexsymb = -1 then lexsymb := newsymb;
   oldsymb := newsymb
   end; (* lexical *)

(*--------------------------------------------------------------------*)

                                (* Skip white to next left brace, accounting
                                   for strings *)
procedure skiptobrace;
var bracelevel: integer;
   instring: boolean;
   prevch: char;
begin
   (*D if debuglevel = 2 then begin writeln(log);
      writeln(log, 'skiptobrace: ' ) end; D*)
   bracelevel := 1; prevch := ' '; instring := false;
   while bracelevel > 0 do begin
      if ch = bslch then skipcontinue(instring);
      if ch=etxch then exitmacro;
      if instring then begin
         if (ch='"') and (prevch<>bslch) then instring := false end
      else if ch = '{' then bracelevel := bracelevel+1
      else if ch = '}' then bracelevel := bracelevel-1
      else if ch = '"' then instring := true;
      if bracelevel <= 0 then begin end
      else if inputeof then begin
         bracelevel := 0;
         if instring then markerror(807) else markerror(804)
         end
      else begin prevch := ch; inchar end
      end
   end;
                                (* Stuff the body of a for loop or a macro
                                   body into p2 *)
procedure readfor(*F(p0: fbufferp; attx: integer; var p2: fbufferp)F*);
                                (* attx: attstack index or -(name length)
                                   p0 <> nil: append the output to this buffer.
                                   Should we check for macro arguments? *)
var j,bracelevel: integer;
   p,p1: fbufferp;
   instring: boolean;
   prevch: char;
begin
   (*D if debuglevel > 0 then begin
       write(log,'readfor: p0');
       if p0=nil then write(log,'=') else write(log,'<>');
       write(log,'nil attx(');
       if attx<0 then write(log,'-length)=') else write(log,'attstack idx)=');
       writeln(log,attx:5)
       end; D*)
   prevch := ' '; instring := false; bracelevel := 1;
   p1 := nil;
   while bracelevel > 0 do begin
      if p0 = nil then newbuf(p) else begin p := p0; p0 := nil end;
      with p@ do begin
         attrib := attx;                (* attstack index or -(name length) *)
         if p1 <> nil then begin p1@.nextb := p; prevb := p1 end;
         higherb := inbuf
         end;
      p1 := p;
      j := CHBUFSIZ;
      repeat with p1@ do begin
         if ch = bslch then skipcontinue(instring);
         (* D if debuglevel = 2 then begin write(log, 'readfor1: ');
             logchar(ch); write(log,' instring=',instring,' ') end; D *)
         if instring then begin  (* do not check braces in strings *)
            if (ch='"') and (prevch<>bslch) then instring := false
            end
         else if ch = '#' then begin skiptoend; ch := nlch end
         else if ch = '{' then bracelevel := bracelevel+1
         else if ch = '}' then bracelevel := bracelevel-1
         else if ch = '"' then instring := true;
         if bracelevel <= 0 then begin j := savedlen+1; ch := nlch end;
         savedlen := savedlen + 1; carray@[savedlen] := ch;
         prevch := ch;
         (* D if debuglevel = 2 then begin
             write(log,' savedlen=',savedlen:1,' carray(',savedlen:1,')=');
             logchar(ch); write(log,' instring=',instring); writeln(log)end;D *)
         if bracelevel > 0 then inchar;
         if inputeof then begin
            if instring then markerror(807) else markerror(804);
            j := savedlen; bracelevel := 0
            end
         end
      until p1@.savedlen = j
      end;
   while p1@.prevb <> nil do p1 := p1@.prevb;
   backup;
   (*D if debuglevel > 0 then begin
      write(log,'readfor: for/macro buffer'); wrbuf(p1,3,0) end; D*)
   p2 := p1
   end;

(*--------------------------------------------------------------------*)

procedure bumptop( chk: stackinx; var sttop: stackinx);
begin
  (* D if chk<>sttop then
     writeln(errout,'chk=',chk:4,' sttop=',sttop:4); D *)
  if chk < STACKMAX then sttop := sttop+1 else fatal(6)
  end;

(*D
procedure prattstack(j: integer);
var i,k: integer;
begin
   write(log,'attstack: ');
   for i := 1 to j do write(log,i:4);                   writeln(log);
   write(log,'lexval  : ');
   for i := 1 to j do write(log,attstack@[i].lexval:4); writeln(log);
   write(log,'state   : ');
   for i := 1 to j do write(log,attstack@[i].state:4) ; writeln(log);
   write(log,'chbufx  : ');
   for i := 1 to j do write(log,attstack@[i].chbufx:4); writeln(log);
   write(log,'length  : ');
   for i := 1 to j do write(log,attstack@[i].length:4); writeln(log);
   D*)(* D
   write(log,'xval    : ');
   for i := 1 to j do write(log,attstack@[i].xval:4:1); writeln(log);
   write(log,'yval    : ');
   for i := 1 to j do write(log,attstack@[i].yval:4:1); writeln(log);
   D *)(*D 
   write(log,'ptype   : ');
   for i := 1 to j do if attstack@[i].prim = nil then write(log,'    ')
      else write(log,attstack@[i].prim@.ptype:4);       writeln(log);
   k := j+1; i := 1;
   while i < k do if attstack@[i].internal=nil then i := i+1 else k := i;
   if k <= j then begin
      write(log,'intrtype: ');
      for i := 1 to j do if attstack@[i].internal = nil then write(log,'    ')
         else write(log,attstack@[i].internal@.ptype:4);    writeln(log) end
   end; D*)

procedure doprod(*F(prno: integer)F*);
begin
   redubuf[reduinx].prod := prno;
   reduinx := reduinx - 1
   end;

                                (* Call the semantic action routine
                                   (produce) for the queued productions *)
procedure advance;
var
    i, j: integer;
begin (* perform reductions *)
    (*D if (debuglevel > 1) and (redutop > 0) then begin
       write(log,'advance prod nos:');
       for i:=1 to redutop do write(log,redubuf[i].prod:4);
       writeln(log);
       write(log,'        newtop  :');
       for i:=1 to redutop do write(log,redubuf[i].newtop:4);
       writeln(log)
       end; D*)

    reduinx := 1;
    while reduinx <= redutop do with redubuf[reduinx] do begin

       (*D if debuglevel > 1 then begin
          j := newtop; if oldtop > newtop then j := oldtop;
          writeln(log,'Attstack for next production:');
          prattstack(j)
          end; D*)

       produce(newtop, prod);   (* may change reduinx *)
       (* D if debuglevel > 1 then writeln(log,
          'attstack@[', newtop:1, '].chbufx<-', attstack@[newtop-1].chbufx:1);
           D *)
       (* attstack@[newtop].chbufx := attstack@[newtop-1].chbufx; *)
       reduinx := reduinx + 1
       end;
                                (* update stack *)
    if validtop <> top then for i := 1 to stacktop - validtop do
       parsestack@[validtop + i] := parsestack@[top + i];

                                (* This maintains the input string buffer
                                   and could be removed if the buffer is
                                   not required *)
    j := attstack@[stacktop-1].chbufx;

    (*D if (debuglevel > 0) and (redutop > 0) and (j < oldbufi) then begin
       write(log,' advance: stacktop=',stacktop:1);
       write(log,' attstack@[stacktop-1](chbufx,length) = ');
       with attstack@[stacktop-1] do wpair(log,chbufx,length);
       writeln(log); write(log,' oldbufi=',oldbufi:1,' ');
       snapname(chbuf,0,chbufi); writeln(log)
       end;D*)

    if (redutop > 0) and (j < oldbufi)
      and (not (redubuf[redutop].prod in [suffix1,suffix2,closeblock1]))
      then begin

        (*D if debuglevel > 0 then begin write(log,
          'chbuf(', oldbufi:1, ':', chbufi-1:1, ') =');
          snapname(chbuf,oldbufi,chbufi-oldbufi);
          writeln(log,'->(',j:1, ':', j+chbufi-oldbufi-1:1, ')') end; D*)

        for i:=0 to chbufi-oldbufi-1 do chbuf@[j+i]:=chbuf@[oldbufi+i];
        chbufi := j + chbufi - oldbufi;
        oldbufi := j
        end;
                                (* shift *)
    bumptop( stacktop,stacktop );
    parsestack@[stacktop].table := startinx;
    stackattribute( stacktop );
                                (* freeze stack, ready for new lookahead *)
    top := stacktop;
    pseudotop := stacktop;
    validtop := stacktop;
    start := lri;
    redutop := 0
end; (*advance*)

procedure backtrack(btop: stackinx; bstart: integer);
begin
    stacktop := btop;
    validtop := btop;
    pseudotop := btop;
    startinx := bstart;
    lri := bstart;
    redutop := 0
end; (* backtrack *)

procedure pseudoshift;
begin
    bumptop( pseudotop,stacktop );
    pseudotop := top + (stacktop - validtop);
    parsestack@[pseudotop].table := startinx;
    stackattribute( pseudotop )
end; (* pseudoshift *)

(*--------------------------------------------------------------------*)

                                (* Add production number to the production
                                   buffer if it is positive; negative value
                                   is for nul production *)
procedure queue(rs,p: integer);
begin
   if p >= 0 then begin
      if redutop < REDUMAX then redutop := redutop + 1 else fatal(7);
      with redubuf[redutop] do begin
         (*D oldtop := stacktop; D*)
         stacktop := stacktop - rs;
         newtop := stacktop;
         prod := p
         end
      end
   else stacktop := stacktop - rs;
   if stacktop <= validtop then begin
      pseudotop := stacktop;
      validtop := stacktop
      end
   else pseudotop := pseudotop - rs
   end; (* queue *)

                                (* Check the lexical terminal against the
                                   parse table.  Queue productions. *)
function lookahead(lsymbol: symbol): boolean;
var
    decided,la: boolean;
    si: integer;
begin
  decided := false; la := false;
  (*D if trace then write(log,' lri:');D*)
  repeat
     (*D if trace then write(log, lri: 4);D*)
     startinx := lri;
     case lr[lri+kind] of
         0: begin
              decided := true;
              la := true;
              parsestop := true
              end;
         1: begin
              while (lr[lri+symb]<>lsymbol) and (lr[lri]<>0) do lri := lr[lri];
              decided := true;
              la := (lr[lri+symb] = lsymbol)
              end;
         2, 4, 6: begin
              while (lr[lri+symb]<>lsymbol) and (lr[lri]<>0) do lri := lr[lri];
              if lr[lri+kind] = 2 then begin
                  decided := true;
                  la := true
                  end
              else if lr[lri+kind] = 6 then begin
                  pseudoshift;
                  stacktop := stacktop - 1;
                  pseudotop := pseudotop - 1;
                  queue(-1, lr[lri+err])
                  end
              end;
         3: queue(lr[lri+rs], lr[lri+prod]);
         5: begin
              si := parsestack@[pseudotop].table;
              (*D if trace then
                  writeln(log, ' SI(', si: 1, ')');D*)
              while (lr[lri+lb] <> si) and (lr[lri] <> 0) do lri := lr[lri]
              end
         end; (* case *)
     lri := lr[lri+next]
     (*D;if trace then write(log,'|')D*)
  until decided;
  lookahead := la
  (*D; if trace then writeln(log,' lookahead done')D*)
end; (* lookahead *)

(*--------------------------------------------------------------------*)

                                (* Look in the grammar for error recovery *)
procedure syntaxerror;
var
    success: boolean;
    s, s1: stackinx;
begin
    (*D if debuglevel > 0 then write(log, ' <---SYNTAXERROR');D*)

    markerror(lr[startinx+err]);
    backtrack(top, start);
    pseudoshift;
    s := 0;
    for s1 := 0 to top do begin
       backtrack(s1, parsestack@[s1 + 1].table);
       if lookahead(XERROR) then begin
          parsestack@[s1].link := s;
          s := s1
          end
       end;
    success := false;
    repeat
       s1 := s;
       repeat
          backtrack(s1, parsestack@[s1 + 1].table);
          if lookahead(XERROR) then begin
             pseudoshift;
             success := lookahead(newsymb)
             end;
          s1 := parsestack@[s1].link
       until success or (s1 = 0);
       if not success then
          (*D begin if debuglevel > 0 then writeln(log, ' <---SKIPPED'); D*)
          lexical (*D end D*)
    until success or parsestop;
    if parsestop then fatal(8)
end; (* syntaxerror *)

                                (* Initialize the semantic actions, the
                                   parse state, and lexical state; then
                                   parse to end of input, managing the
                                   parse stack. *)
procedure parse;
begin (* parse *)
   initrandom;

   new(chbuf);
   (*D if debuglevel > 0 then writeln(log,'new(chbuf)[',odp(chbuf):1,']'); D*)
   (*P2 IP*) inittables; (*P2 P*)
   (*P2CIP*) initchars; (*P2CP*)
   entrytv[ordNL] := XNL;
   entrytv[ordCR] := XNL;       (* treat ^M as line end *)

   errcount := 0;
                                (* change for debugging *)
   (* linesignal := 0; *)
   (*DGHMF trace := false;
   if trace and (oflag = 0) then openlogfile; FMHGD*)

   new(parsestack);
   produce(1, -2);
   printstate := 0;

                                (* lexical initializations, see also
                                   production -1 *)
   ch := ' ';
   lineno := 0;
   chbufi := 0;
   oldbufi := 0;
   newsymb := XNL;
   oldsymb := XNL;
   lexstate := 0;
   macros := nil;
   args := nil;
   forbufend := false;
   instr := false;

   stackattribute( 0 );

                                (* parser initializations *)
   parsestop := false;
   top := 0;
   start := 5;
   parsestack@[0].table := 0;
   parsestack@[0].link := 0;
   backtrack(top, start);
   (*D stackhigh := top; D*)
   produce(1, -1);
                                (* Main parse loop *)
   while not parsestop do begin
      lexical;
      (*D if trace then
         writeln(log, ' SYM:', newsymb: 3, ' STATE:', lri: 3);D*)
      if not lookahead(newsymb) then syntaxerror;
      advance
      end
   end; (* parse *)

(*GHMF
function optionchar(var fn: mstring ): char;
var j,k: integer;
begin
   j := 1; k := FILENAMELEN+1;
   while j < k do if fn[j] = FMHG*)(*GHM ' ' MHG*)(*F chr(0) F*)(*GHMF
   then j := j+1 else k := j;
   if j >= FILENAMELEN then optionchar := chr(0)
   else if fn[j] = '-' then begin
      optionchar := fn[j+1];
      FMHG*)(*
      if fn[j+2] <> ' ' then for k:=j+2 to FILENAMELEN do fn[k-j-1] := fn[k]
      *)(*GHMF
      end
   else optionchar := chr(0)
   end; FMHG*)

                                (* Set safe mode and one of 12 output formats.
                                   The version date is set during
                                   preprocessing *)
(*GHMF
procedure getoptions;
var cht: char;
   istop: integer;
begin
   FMHG*)(*DGHMF oflag := 0; linesignal := 0; FMHGD*)(*GHMF
   argct := 1;
   istop := FMHG*)(*M argc; M*)(*GHF ParamCount+1; FHG*)(*GHMF
   while argct < istop do begin
      FMHG*)(*M argv(argct,infname); M*)
            (*GHF infname := ParamStr(argct); FHG*)(*GHMF
      cht := optionchar(infname);
      if ord(cht) = 0 then istop := argct
      else begin
              if cht = 'd' then drawmode := PDF
         else if cht = 'e' then drawmode := Pict2e
         else if cht = 'f' then drawmode := PSfrag
         else if cht = 'g' then drawmode := PGF
         else if cht = 'm' then drawmode := MFpic
         else if cht = 'p' then drawmode := PSTricks
         else if cht = 'r' then drawmode := PS
         else if cht = 's' then drawmode := MPost
         else if cht = 't' then drawmode := tTeX
         else if cht = 'v' then drawmode := SVG
         else if cht = 'x' then drawmode := xfig
         else if cht = 'z' then safemode := true
         FMHG*)(*DGHMF
         else if cht = 'y' then linesignal := 2
         else if (cht >= '0') and (cht <= '9') then oflag := ord(cht)-ord('0')
         FMHGD*)(*GHMF
         else if (cht = 'h') or (cht = '-') then begin
            writeln(errout,' *** VERSIONDATE');
            FMHG*)(*DGHMF
            writeln(errout,' Debug is enabled');
            FMHGD*)(*GHMF
            writeln(errout,' Options:');
            writeln(errout,'     (none) LaTeX picture output');
            writeln(errout,'     -d PDF output');
            writeln(errout,'     -e Pict2e output');
            writeln(errout,'     -f Postscript output, psfrag strings');
            writeln(errout,'     -g PGF-TikZ output');
            writeln(errout,'     -h write this message and quit');
            writeln(errout,'     -m mfpic output');
            writeln(errout,'     -p PSTricks output');
            writeln(errout,'     -r Postscript output');
            writeln(errout,'     -s MetaPost output');
            writeln(errout,'     -t eepicemu output');
            writeln(errout,'     -v SVG output');
            writeln(errout,'     -x xfig output');
            writeln(errout,
        '     -z safe mode (disable sh, copy, and print to file)');
            fatal(0)
            end
         else begin
            write(errout,' *** dpic terminating: Unknown option "');
            if isprint(cht) then writeln(errout,cht,'"')
            else writeln(errout,'char(',ord(cht):1,')"');
            fatal(0)
            end;
         argct := argct+1
         end
      end
   end; FMHG*)(* getoptions *)

begin (* dpic *)
   openerror;
   drawmode := TeX;
      (*P2CC #ifdef SAFE_MODE *)
      (*P2CP safemode := true; *)
      (*P2CC #else *)
      safemode := false;
      (*P2CC #endif *)
   (*GHMF getoptions; FMHG*)
   openfiles;
   inputeof := false;
   new(attstack);
   tmpbuf := nil;
   tmpfmt := nil;
   parse;
   (*P2CIP*)
   (*MGH 999: HGM*)
   (*P2CP*)
   epilog;

   end (* dpic *) .
