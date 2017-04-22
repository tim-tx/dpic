(* G module dpic1; G *)

(* include dp0.h *)
(* G#include 'dp0.h'G *)

(* include sysdep.h *)
(* G#include 'sysdep.h'G *)

(* Recursive routines: snaptree FindExitPoint neswrec shift treedraw scaleobj *)

function bval( buf:chbufp ): integer;
begin
(*P2CIP*)
   bval := ord(buf@[1])*(ordMAXCH+1) + ord(buf@[2])
(*P2CP*)(*P2CC return (((int) buf[0]) << 7) + (int) buf[1] ; *)
   end;

procedure putbval( buf:chbufp; n:integer );
(*P2CIP*) var i: integer; (*P2CP*)
begin
(* D
  if debuglevel > 0 then writeln(log,'putbval[',ordp(buf):1,'](',n:1,')'); D *)
(*P2CIP*)
   i := n div (ordMAXCH+1);
   buf@[1] := chr(i);
   buf@[2] := chr( n - i*(ordMAXCH+1) )
(*P2CP*)(*P2CC buf[0] = (Char)(n>>7); buf[1] = (Char)(n % 128); *)
   end;

procedure deletename(*F(var head: strptr)F*);
var pn,r: strptr;
   j: integer;
begin
   while head <> nil do begin
      pn := head; r := pn;
      while pn@.next <> nil do begin r := pn; pn := pn@.next end;
      r@.next := nil;
      if pn = head then head := nil;
      with pn@ do if segmnt = nil then begin end
      else if bval(segmnt) > 1 then begin
         j := bval(segmnt); putbval(segmnt,j-1);
         if (segmnt=freeseg) and ((seginx+len)=freex) then begin
            freex := seginx; j := 3;
            while freex > j do
               if segmnt@[freex-1]=nlch then freex := freex-1 else j := freex
            end
         else for j:=seginx to seginx+len-1 do segmnt@[j] := nlch
         end
      else if (segmnt = freeseg) and (freeseg <> nil) then begin
         (*D if debuglevel > 0 then
            writeln(log,'deletename freeseg[',ordp(freeseg):1,']'); D*)
         dispose(freeseg); freeseg := nil
         end
      else begin
         (*D if debuglevel > 0 then
            writeln(log,'deletename segmnt[',ordp(segmnt):1,']'); D*)
         dispose(segmnt); segmnt := nil
         end;
      (*D if debuglevel > 0 then
         writeln(log,'deletename strptr[',ordp(pn):1,']'); D*)
      dispose(pn)
      end
   end;

procedure setspec(var specv: integer; svalue: integer);
begin
   specv := (specv div 8)*8 + (svalue - XLlinetype);
   (* if svalue = XLsolid then
      specv := (specv div 32)*32 + 16 + (specv mod 16) *)
   end;

procedure resetspec(var specv: integer; svalue: integer);
begin
   specv := 0; setspec(specv, svalue)
   end;

procedure setthen(var specv: integer);
begin
   specv := (specv div 16)*16 + 8 + (specv mod 8)
   end;

(*D
procedure prtstval(st: integer);
begin
   write(log,'state=',st:1);
   if (st mod 4)<>0 then
   case (st mod 4) of
      1: write(log,',XL','to');
      2: write(log,',XL','from');
      3: write(log,',XL','at');
      otherwise
      end;
   if odd(st div 4) then write(log,',XL','chop');
   if odd(st div 8) then write(log,',XL','directon')
   end;

procedure snapname( chbu:chbufp; inx,ll:chbufinx);
var j: integer;
begin
   write(log,' (',ordp(chbu):1,' inx=',inx:1,' len=',ll:1,')|');
   if chbu = nil then write(log,'**nil string pointer**')
   else for j := inx to inx+ll-1 do write(log,chbu@[j]);
   write(log,'|'); flush(log)
   end;

procedure snaptype D*)(*DF(var iou: text; p: integer)FD*)(*D ;
begin
   case p of
      XLbox: write(iou,'<box>');
      XBLOCK: write(iou,'<[]>');
      XLellipse: write(iou,'<ellipse>');
      XLcircle: write(iou,'<circle>');
      XLline: write(iou,'<line>');
      XLarrow: write(iou,'<arrow>');
      XLmove: write(iou,'<move>');
      XLspline: write(iou,'<spline>');
      XLarc: write(iou,'<arc>');
      XLstring: write(iou,'<<string>>');
      XLaTeX: write(iou,'<<LaTeX>>');
      XLabel: write(iou,'<<Label>>');
      otherwise write(iou,'Unknown type ',p:3)
      end;
   flush(iou)
   end;

procedure snaptree(pr: primitivep; indent: integer);
var i: integer;
begin
   while (pr <> nil) and (indent<=240) do with pr@ do begin
      snaptype(log,ptype);
      case ptype of
         XBLOCK: i := 4;
         XLbox,XLarc: i := 5;
         XLline,XLmove: i := 6;
         XLarrow: i := 7;
         XLcircle,XLspline: i := 8;
         XLellipse,XLaTeX: i := 9;
         XLstring: i := 10;
         XLabel: i := 12;
         otherwise i := 3
         end;
      snaptree(next,indent+i);
      if son <> nil then begin
         writeln(log);
         write(log,' ':indent);
         end;
      pr := son
      end
   end;

procedure printptype(ptype: integer);
begin
   case ptype of
   XLline: write(log,'XLline'); XLarrow: write(log,'XLarrow');
   XLmove: write(log,'XLmove'); XLspline: write(log,'XLspline');
   XLbox: write(log,'XLbox'); XLstring: write(log,'XLstring');
   XBLOCK: write(log,'XBLOCK'); XLcircle: write(log,'XLcircle');
   XLellipse: write(log,'XLellipse'); XLarc: write(log,'XLarc');
   otherwise write(log, ptype:4)
   end
   end;

procedure printcorner(lexv: integer);
begin
   case lexv of
   XDn: writeln(log,'.n'); XDs: writeln(log,'.s');
   XDe: writeln(log,'.e'); XDw: writeln(log,'.w');
   XDnw: writeln(log,'.nw'); XDne: writeln(log,'.ne');
   XDse: writeln(log,'.se'); XDsw: writeln(log,'.sw');
   XDc: writeln(log,'.c');
   XDstart: writeln(log,'.start'); XDend: writeln(log,'.end')
   otherwise writeln(log,'unknown corner lexv=',lexv:1)
   end
   end;

procedure printtext(namptr: strptr);
begin
   while namptr <> nil do with namptr@ do begin
      write(log,' ');
      D*)(*DM wpair(log,ord(namptr),ord(next)); MD*)
      (*DGHF wpair(log,ordp(namptr),ordp(next)); FHGD*)(*D
      write(log,' val='); wfloat(log,val); flush(log);
      snapname(segmnt,seginx,len);
      namptr := next; writeln(log) end
   end;

procedure printobject D*)(*DF(primp: primitivep)FD*)(*D ;
var
   xx,yy: real;
   i: integer;
begin
   if debuglevel = 0 then begin end
   else if primp = nil then writeln(log,'Object is nil')
   else while primp <> nil do with primp@ do begin
      write  (log,'Object(',ordp(primp):1,
                  ') type='); snaptype(log,ptype);
      writeln(log,'(',ptype:1,')' );
      write  (log,' Parent(',ordp(parent):1);
      if parent<>nil then write(log,
                  ') Parent@.son(',ordp(parent@.son):1);
      writeln(log,') Son(',   ordp(son):1,
                  ') Next(',  ordp(next):1, ')');
      if name<>nil then begin write(log,' name: '); printtext(name) end;
      if outlinep<>nil then begin
         write(log,' outline:'); printtext(outlinep) end;
      if shadedp<>nil then begin
         write(log,' shaded:'); printtext(shadedp) end;
      if textp<>nil then printtext(textp);
      write(log,' aat'); wpair(log,aat.xpos,aat.ypos);
      wlogfl('lparam',lparam,0);
      wlogfl('lthick',lthick,0);
      case direction of
         XLup: write(log,' <up>');
         XLdown: write(log,' <down>');
         XLleft: write(log,' <left>');
         XLright: write(log,' <right>');
         otherwise write(log,' dir =',direction:1)
         end;
      writeln(log,' spec=',spec:1);
      flush(log);
      case ptype of
         XLbox,XLstring: begin
            wlogfl('boxfill',boxfill,0);
            wlogfl('boxheight',boxheight,0); wlogfl('boxwidth',boxwidth,0);
            wlogfl('rad',boxradius,0) end;
         XBLOCK: begin
            wlogfl('blockheight',blockheight,0);
            wlogfl('blockwidth',blockwidth,0);
            write(log,' here='); wpair(log,here.xpos,here.ypos); writeln(log);
            write(log,' vars=');
            for i:=0 to HASHLIM do begin
               if vars[i] = nil then write(log,' ',i:1,' nil;') D*)
               (*DM else write(log,' ',i:1,' ',ord(vars[i]):1,';'); MD*)
               (*DGHF  else write(log,' ',i:1,' ',ordp(vars[i]):1,';');FHGD*)(*D
               end;
            writeln(log);
            write(log,' env=');
               if env = nil then write(log,'nil') D*)
               (*DM else write(log,ord(env):1) MD*)
               (*DGHF  else write(log,ordp(env):1)FHGD*)(*D
            end;
         XLcircle: begin
            wlogfl('cfill',cfill,0); wlogfl('radius',radius,0) end;
         XLellipse: begin
            wlogfl('efill',efill,0);
            wlogfl('elheight',elheight,0); wlogfl('elwidth',elwidth,0) end;
         XLline,XLarrow,XLmove,XLspline: begin
            write(log,' endpos='); wpair(log,endpos.xpos,endpos.ypos);
            wlogfl('height',height,0); wlogfl('width',width,0);
            wlogfl('lfill',lfill,0); wlogfl('aradius',aradius,0); writeln(log);
            write(log,' ahlex(atype)=',ahlex(atype):1);
            write(log,' ahnum(atype)=',ahnum(atype):1)
            end;
         XLarc: begin
            write(log,' lspec=',lspec(spec):1);
            wlogfl('lfill',lfill,0); wlogfl('aradius',aradius,0);
            write(log,' (|','startangle|,|','arcangle|)(deg)=');
            wpair(log,|startangle| *180.0/pi,|arcangle| *180.0/pi); writeln(log);
            write(log,' (from)=');
            wpair(log,aat.xpos+aradius*cos(|startangle|),
                      aat.ypos+aradius*sin(|startangle|));
            write(log,' (to)=');
            xx := aat.xpos+aradius*cos(|startangle|+|arcangle|);
            yy := aat.ypos+aradius*sin(|startangle|+|arcangle|);
            wpair(log,xx,yy);
            write(log,' ahlex(atype)=',ahlex(atype):1);
            write(log,' ahnum(atype)=',ahnum(atype):1)
            end;
         XLaTeX,XLabel: ;
         otherwise writeln(log,' Bad case in printobject; this cannot happen')
         end;
      writeln(log); flush(log);
      if ptype = XBLOCK then primp := nil else primp := son
      end;
   writeln(log); flush(log)
   end;

procedure prvars(np: integer);
var lv: strptr;
   i,x: integer;
begin
   with attstack@[np] do if envblock = nil then
      write(log,'vars=nil: nil envblock')
   else begin
      i := 0; x := HASHLIM+1;
      while i < x do if envblock@.vars[i]<>nil then x := i else i := i+1;
      writeln(log,'vars=');
      if x > HASHLIM then writeln(log,' None set') else
      for i:=0 to HASHLIM do begin write(log,i:1);
         lv := envblock@.vars[i];
         if lv = nil then writeln(log,' nil');
         while lv <> nil do begin
            write(log,' (',ordp(lv):1,',',ordp(lv@.next):1,')=');
            with lv@ do begin
               snapname(segmnt,seginx,len);
               write(log,'='); wfloat(log,val); writeln(log)
               end;
            lv := lv@.next
            end
         end
      end
   end;
D*)

procedure deletetree(*F(var p: primitivep)F*);
var r: primitivep;
    i: integer;
begin
   if p <> nil then p@.parent := nil;
   while p <> nil do begin
      while (p@.next<>nil) or (p@.son<>nil) do begin
         r := p;
         if p@.next <> nil then begin p := p@.next; r@.next := nil end
         else begin p := p@.son; r@.son := nil end;
         p@.parent := r
         end;
      r := p@.parent;
      deletename( p@.shadedp );
      deletename( p@.outlinep );
      deletename( p@.textp );
      deletename( p@.name );
      (*D if debuglevel > 0 then
         writeln(log,'deletetree prim[',ordp(p):1,']'); D*)
(*MGH
      with p@ do case ptype of
         XLbox,XLstring: dispose(p,XLbox);
         XBLOCK: begin for i:=HASHLIM downto 0 do deletename(vars[i]);
            if env<>nil then dispose(env); dispose(p,XBLOCK) end;
         XLcircle:  dispose(p,XLcircle);
         XLellipse: dispose(p,XLellipse);
         XLline,XLarc,XLarrow,XLmove,XLspline: dispose(p,XLline);
         XLaTeX,XLabel: dispose(p,XLabel);
         otherwise
         end;
HGM*)
(*F   with p@ do if ptype=XBLOCK then begin
         for i:=HASHLIM downto 0 do deletename(vars[i]);
         if env<>nil then dispose(env)
         end;
      dispose(p);
F*)
      p := r
      end
   end;

procedure setangles(var strtang,arcang:real; ctr:postype; xs,ys,xf,yf:real );
var ra: real;  (* set arc angles given centre, start, end *)
begin
   strtang := datan((ys-ctr.ypos),(xs-ctr.xpos));
   ra := principal(datan((yf-ctr.ypos),(xf-ctr.xpos)) - strtang, pi);
   if (ra < 0.0) and (arcang > 0.0) then ra := ra + 2.0*pi
   else if (ra > 0.0) and (arcang < 0.0) then ra := ra-2.0*pi;
   arcang := ra
   end;

procedure eqop(var x: real; op: integer; y: real);
var i,j: integer;
begin
   case op of
      XEQ,XLcoloneq: x := y;
      XLpluseq: x := x + y;
      XLminuseq: x := x - y;
      XLmulteq: x := x * y;
      XLdiveq: if y = 0 then markerror(852) else x := x / y;
      XLremeq: begin
         i := round(x); j := round(y);
         if j = 0 then begin markerror(852); x := 0 end
         else x := i - (i div j)*j
         end;
      otherwise
      end
   end;

procedure setstval(var st: integer; value: integer);
begin
   st := value*256 + (st mod 256)
   end;

function getstval(st: integer): integer;
begin
   getstval := st div 256
   end;

procedure setstflag(var st: integer; value: integer);
begin
   case value of
      XEMPTY:     st := (st div 64)*64;
      XLto:       st := (st div  2)*2  +  1;
      XLfrom:     st := (st div  4)*4  +  2 + (st mod  2);
      XLat:       st := (st div  8)*8  +  4 + (st mod  4);
      XLradius:   st := (st div 16)*16 +  8 + (st mod  8);
      XLcw:       st := (st div 32)*32 + 16 + (st mod 16);
      XLccw:      st := (st div 64)*64 + 32 + (st mod 32);
      XLchop:     st := (st div 128)*128 + 64 + (st mod 64);
      XLdirecton: st := (st div 256)*256 + 128 + (st mod 128);
      otherwise
      end

   end;

function teststflag(st,value: integer): boolean;
var b: boolean;
begin
   case value of
      XLto:     b := odd(st);
      XLfrom:   b := odd(st div 2);
      XLat:     b := odd(st div 4);
      XLradius: b := odd(st div 8);
      XLcw:     b := odd(st div 16);
      XLccw:    b := odd(st div 32);
      XLchop:   b := odd(st div 64);
      XLdirecton: b := odd(st div 128)
      end;
   teststflag := b
   end;

function eqstring( seg1:chbufp; inx1,len1: chbufinx;
                   seg2:chbufp; inx2,len2: chbufinx): integer;
var i,j,k: integer;
begin
   (*D if debuglevel = 2 then begin
      writeln(log,' eqstring:'); write(log,' 1st arg =');
      if seg1 = nil then write(log,' nil') else snapname(seg1,inx1,len1);
      writeln(log); write(log,' 2nd arg =');
      if seg2 = nil then write(log,' nil') else snapname(seg2,inx2,len2);
      writeln(log) end; D*)
   if (seg1 = nil) or (seg2 = nil) then k := maxint
   else begin
      i := 0; k := 0; if len1 < len2 then j := len1 else j := len2;
      while i < j do begin
           (*D if debuglevel = 2 then
                write(log,'(',seg1@[inx1+i],seg2@[inx2+i],')'); D*)
         k := ord(seg1@[inx1+i]) - ord(seg2@[inx2+i]);
         if k = 0 then i := i+1 else j := i
         end;
      if k <> 0 then begin end
      else if len1 > len2 then k := ord(seg1@[inx1+i])
      else if len1 < len2 then k := -ord(seg2@[inx2+i])
      (*D; if debuglevel = 2 then 
         writeln(log,' i=',i:1,' j=',j:1,' eqstring=',k:1) D*)
      end;
   eqstring := k
   end;

function cmpstring( p1,p2: primitivep ): integer;
begin
   if (p1 = nil) or (p2 = nil) then cmpstring := maxint
   else if p1@.textp = nil then cmpstring := maxint
   else if p2@.textp = nil then cmpstring := -maxint
   else
      cmpstring := eqstring(p1@.textp@.segmnt,p1@.textp@.seginx,p1@.textp@.len,
                            p2@.textp@.segmnt,p2@.textp@.seginx,p2@.textp@.len)
   end;

function findplace(p: primitivep; chb:chbufp; inx,length:chbufinx): primitivep;
var pj: primitivep;
begin
   pj := nil;
   (*D if debuglevel = 2 then begin
      write(log,'findplace:'); snapname(chb,inx,length); writeln(log)
      end; D*)
   while p <> pj do begin
      (*D if debuglevel = 2 then printobject(p); D*)
      if p@.name = nil then p := p@.next
      else with p@.name@ do
         if eqstring(segmnt,seginx,len, chb,inx,length) = 0 then pj := p
      else p := p@.next
      end;
   findplace := p
   end;

function findmacro( p:argp;
                     chb: chbufp; inx,length: chbufinx;
                     var last:argp ): argp;
var pj: argp;
begin
   last := nil; pj := nil;
   (*D if debuglevel = 2 then begin
      write(log,'findmacro:'); snapname(chb,inx,length); writeln(log)
      end; D*)
   while p <> pj do with p@ do
      if argbody = nil then begin last := p; p := p@.nexta end
      else if eqstring(argbody@.carray,1,-argbody@.attrib, chb,inx,length) = 0
         then pj := p
      else begin last := p; p := p@.nexta end;
   findmacro := p
   end;

function varhash( chb: chbufp; chbufx,length: chbufinx ): integer;
var idx: integer;
begin
   if chb = nil then idx := 0
   else begin
      idx := ord(chb@[chbufx]);
      if length > 2 then idx := idx+ord(chb@[chbufx+length-2])
      end;
   varhash := idx - (idx div (HASHLIM+1))*(HASHLIM+1)
   end;

function findname( eb:primitivep;
                   chb: chbufp; chbufx,length: chbufinx;
                   var last:strptr; var k: integer ): strptr;
var leftptr,rightptr: strptr;
    left,right,midpt,i,idx: integer;
begin
   idx := varhash(chb,chbufx,length);
   (*D if debuglevel > 0 then begin
       write(log,' findname|');
       for i:= chbufx to chbufx-1+length do write(log,chb@[i]);
       write(log,'|:');
       if eb=nil then write(log,' eb=nil')
       else write(log,' nvars[',idx:1,']=',eb@.nvars[idx]:1);
       if debuglevel > 1 then writeln(log)
       end; D*)
   k := 1; last := nil; rightptr := nil; left := 0; right := 0;
   if eb = nil then leftptr := nil
   else begin leftptr := eb@.vars[idx]; last := leftptr end;
                         (* Check the first (highest) name *)
   if leftptr <> nil then begin
      with leftptr@ do k := eqstring(chb,chbufx,length, segmnt,seginx,len);
      if k < 0 then begin
         left := 2; leftptr := leftptr@.next; right := eb@.nvars[idx]+1 end
      else rightptr := leftptr 
      end;
   while leftptr <> rightptr do begin
      midpt := (left+right) div 2; 
                         (*D if debuglevel > 0 then
                                            write(log,' midpt=',midpt:1); D*)
      last := leftptr;
      for i:=left+1 to midpt do last := last@.next;
      with last@ do k := eqstring(chb,chbufx,length, segmnt,seginx,len);
                         (*D if debuglevel > 0 then write(log,' k=',k:1); D*)
      if k < 0 then begin
         left := midpt+1;
         leftptr := last@.next
         end
      else if k = 0 then begin leftptr := last; rightptr := leftptr end
      else begin
         right := midpt;
         rightptr := last
         end
      end;
   if k = 0 then findname := leftptr else findname := nil
                         (*D; if debuglevel > 0 then writeln(log) D*)
   end;

procedure marknotfound(eno:integer; chb:chbufp; inx,len:chbufinx);
var i: integer;
begin
  (*D if debuglevel > 0 then begin write(log,'Search failure ',eno:1);
      if chb <> nil then begin write(log,' for "');
         for i := inx to inx+len-1 do write(log,chb@[i]); write(log,'"') end;
      writeln(log) end; D*)
   markerror(eno);
   writeln(errout);
   write(errout,'Search failure');
   if chb <> nil then begin
      write(errout,' for "');
      for i := inx to inx+len-1 do write(errout,chb@[i]);
      write(errout,'"')
      end;
   writeln(errout)
   end;

function glfindname( eb: primitivep; chb: chbufp;
            chbufx,length: chbufinx; var last:strptr; var k:integer ): strptr;
var np: strptr;
   pp: primitivep;
begin
   pp := nil; np := nil; k := 1;
   while eb <> pp do begin
      (* np := findname(eb@.vars,chb,chbufx,length,last); *)
      np := findname(eb,chb,chbufx,length,last,k);
      if np <> nil then pp := eb else eb := eb@.parent
      end;
   if eb = nil then marknotfound(851,chb,chbufx,length);
   glfindname := np
   end;

procedure newstr(*F(var sp: strptr)F*);
begin
   new(sp);
   with sp@ do begin
      val := 0.0; segmnt := nil; seginx := 0; len:=0; next:=nil
      end
   (*D; if debuglevel > 0 then
      writeln(log,'newstr[',ordp(sp):1,']') D*)
   end;

                                    (* put a string into freeseg *)
procedure storestring( outstr:strptr;
                       srcbuf:chbufp; psrc,lsrc:chbufinx );
var i,j: integer;
   newseg: boolean;
begin
   if (freeseg = nil) or (lsrc > CHBUFSIZ-freex+1) then newseg := true
   else newseg := false;
   if newseg then begin
      new(freeseg);
      (*D if debuglevel > 0 then
         writeln(log,'storestring new[',ordp(freeseg):1,']'); D*)
      putbval(freeseg,0);
      freex := 3
      end;
   for i := 0 to lsrc-1 do freeseg@[freex+i] := srcbuf@[psrc+i];
   with outstr@ do begin segmnt := freeseg; seginx := freex; len:=lsrc end;
   j := bval(freeseg); putbval(freeseg,j+1);
   freex := freex + lsrc
   (*D; if debuglevel > 0 then begin writeln(log,
      'storestring to strptr ',ordp(outstr):1,': segmnt=',ordp(freeseg):1,
      ' seginx=',outstr@.seginx:0, ' links=', bval(freeseg):0,
      ' freex=',freex:0 );
      snapname(freeseg,outstr@.seginx,outstr@.len);
      write(log,' from:'); snapname(srcbuf,psrc,lsrc);
      writeln( log ) end D*)
   end;

                                    (* duplicate a strptr and copy the body *)
procedure copystr( var sp:strptr; ip:strptr);
begin
   if ip=nil then sp := nil else begin
      newstr(sp);
      storestring(sp,ip@.segmnt,ip@.seginx,ip@.len)
      end
   end;

procedure appendstring(sp:strptr; buf:chbufp; px,ll:chbufinx);
var i,j(*D,k D*): integer;
    tmpseg: chbufp;
begin (*D k := 0; D*)
   if (sp=nil) or (buf=nil) then begin end
   else if (sp@.segmnt=freeseg) and (sp@.seginx+sp@.len=freex) and
      (freex+ll-1 <= CHBUFSIZ) then begin
      (*D if debuglevel > 0 then begin
          write(log,' appending |');
          for i:=0 to ll-1 do write(log,buf@[px+i]); writeln(log,'|') end; D*)
      for i:=0 to ll-1 do freeseg@[freex+i] := buf@[px+i]; (*D k := 1; D*)
      sp@.len:=sp@.len+ll;
      freex := freex + ll
      end
   else if sp@.len+ll+2 > CHBUFSIZ then markerror(866)
   else with sp@ do begin
      new(tmpseg);
      (*D if debuglevel > 0 then
         writeln(log,'apendstring new[',ordp(tmpseg):1,']'); D*)
      for i:=0 to len-1 do tmpseg@[3+i] := segmnt@[seginx+i];
      j := bval(segmnt);
      if j > 1 then begin
         putbval(segmnt,j-1); (*D k := 2; D*)
         if (segmnt=freeseg) and ((seginx+len)=freex) then begin
            freex := seginx; j := 3;
            while freex > j do
               if segmnt@[freex-1]=nlch then freex := freex-1 else j := freex
            end
         else for i:=seginx to seginx+len-1 do segmnt@[i] := nlch
         end
      else begin
         if segmnt = freeseg then freeseg := nil; (*D k := 3; D*)
         dispose(segmnt)
         end;
      for i:=0 to ll-1 do tmpseg@[3+len+i] := buf@[px+i];
      freeseg := tmpseg;
      freex := 3+len+ll;
      putbval(freeseg,1);
      segmnt := freeseg;
      seginx := 3;
      len := len+ll
      end
   (*D; if debuglevel > 0 then if sp<>nil then with sp@ do begin
      writeln(log,
       'appendstring to strptr ',ordp(sp):1,': segmnt=',ordp(freeseg):1,
       ' seginx:seginx+len-1=',seginx:1,':',seginx+len-1:1);
      writeln(log,
       ' len=',len:1, ' branch k=',k:1);
      snapname(segmnt,seginx,len); writeln( log )
      end D*)
   end;

function putstring(ix:integer; sp:strptr; buf:chbufp; px,ll:chbufinx): integer;
begin
   if ix <= 0 then storestring( sp, buf, px, ll )
   else           appendstring( sp, buf, px, ll );
   putstring := ix + 1
   end;

function pheight(pr: primitivep ): real;
var ph: real;
begin
   if pr = nil then ph := 0.0
   else with pr@ do case ptype of
      XLbox,XLstring: ph := boxheight;
      XBLOCK: ph := blockheight;
      XLellipse: ph := elheight;
      XLcircle: ph := 2.0 * radius;
  (*  XLline,XLarrow,XLmove: ph := abs(endpos.ypos - aat.ypos); *)
      XLline,XLarrow,XLmove,XLspline: ph := height;
      otherwise ph := 0.0
      end;
   pheight := ph
   end;

function pwidth(pr: primitivep ): real;
var pw: real;
begin
   if pr = nil then pw := 0.0
   else with pr@ do case ptype of
      XLbox,XLstring: pw := boxwidth;
      XBLOCK: pw := blockwidth;
      XLellipse: pw := elwidth;
      XLcircle: pw := 2.0 * radius;
  (*  XLline,XLarrow,XLmove: pw := abs(endpos.xpos - aat.xpos); *)
      XLline,XLarrow,XLmove,XLspline: pw := width;
      otherwise pw := 0.0
      end;
   pwidth := pw
   end;

procedure neswrec(ptm: primitivep);
begin
   while ptm <> nil do begin
      nesw( ptm );
      if ptm@.ptype <> XBLOCK then neswrec( ptm@.son );
      ptm := ptm@.next
      end
   end;

procedure getnesw(ptm: primitivep);
begin
   initnesw;
   neswrec( ptm );
   if south > north then begin south := 0.0; north := 0.0 end;
   if west > east then begin west := 0.0; east := 0.0 end
   end;

procedure FindExitPoint( pr: primitivep; var pe: postype );
begin
   if pr = nil then begin
      pe.xpos := 0.0;
      pe.ypos := 0.0
      end
   else with pr@ do if (ptype <> XBLOCK) and (son <> nil) then
      FindExitPoint(son,pe)
   else begin
      pe := aat;
      case ptype of
         XLbox,XLstring: case direction of
            XLup: pe.ypos := aat.ypos + boxheight * 0.5;
            XLdown: pe.ypos := aat.ypos - boxheight * 0.5;
            XLleft: pe.xpos := aat.xpos - boxwidth * 0.5;
            XLright: pe.xpos := aat.xpos + boxwidth * 0.5
            end;
         (* XLstring: begin
            getnesw(pr);
            case direction of
               XLup: begin pe.xpos := (east+west)/2; pe.ypos := north end;
               XLdown: begin pe.xpos := (east+west)/2; pe.ypos := south end;
               XLleft: begin pe.ypos := (north+south)/2; pe.xpos := west end;
               XLright: begin pe.ypos := (north+south)/2; pe.xpos := east; end
               end
            end; *)
         XBLOCK: case direction of
            XLup: pe.ypos := aat.ypos + blockheight * 0.5;
            XLdown: pe.ypos := aat.ypos - blockheight * 0.5;
            XLleft: pe.xpos := aat.xpos - blockwidth * 0.5;
            XLright: pe.xpos := aat.xpos + blockwidth * 0.5
            end;
         XLcircle: case direction of
            XLup: pe.ypos := aat.ypos + radius;
            XLdown: pe.ypos := aat.ypos - radius;
            XLleft: pe.xpos := aat.xpos - radius;
            XLright: pe.xpos := aat.xpos + radius
            end;
         XLellipse: case direction of
            XLup: pe.ypos := aat.ypos + elheight * 0.5;
            XLdown: pe.ypos := aat.ypos - elheight * 0.5;
            XLleft: pe.xpos := aat.xpos - elwidth * 0.5;
            XLright: pe.xpos := aat.xpos + elwidth * 0.5
            end;
         XLarc: begin
            pe := arcend(pr)
            end;
         XLline,XLarrow,XLmove,XLspline: pe := endpos;
         XLabel,XLaTeX: ;
         otherwise
         end
      end
   end;

procedure newprim(var pr: primitivep; primtype: integer; envblk: primitivep);
var i: integer;
begin
(*MGH
   case primtype of
      XLbox,XLstring:    new(pr,XLbox);
      XBLOCK:     new(pr,XBLOCK);
      XLcircle:  new(pr,XLcircle);
      XLellipse: new(pr,XLellipse);
      XLline,XLarc,XLarrow,XLmove,XLspline: new(pr,XLline);
      XLabel,XLaTeX: new(pr,XLabel);
      otherwise
      end;
HGM*)
(*F   new(pr); F*)
      (*D if debuglevel > 0 then
         writeln(log,'newprim[',ordp(pr):1,']'); D*)
   with pr@ do begin
      name := nil; textp := nil; outlinep := nil; shadedp := nil;
      son := nil; next := nil;
      if envblk = nil then begin
         parent := nil; aat.xpos := 0.0; aat.ypos := 0.0;
         direction := XLright
         end
      else begin
         if envblk@.son = nil then envblk@.son := pr;
         parent := envblk; aat := envblk@.here;
         direction := envblk@.direction
         end;
      lparam := mdistmax; lthick := mdistmax;
      if primtype in
         [XLbox,XLcircle,XLellipse,XLline,XLarrow,XLarc,XLspline,XLstring]
         then resetspec(spec,XLsolid)
      else resetspec(spec,XLinvis);
      ptype := primtype;
      case primtype of
         XLbox,XLstring: begin
            boxfill := -1.0;
            boxheight := 0.0; boxwidth := 0.0; boxradius := 0.0 end;
         XBLOCK: begin
            blockheight := 0.0; blockwidth := 0.0;
            here := aat; for i:=0 to HASHLIM do begin
               vars[i] := nil; nvars[i] := 0 end;
            env := nil end;
         XLcircle: begin
            cfill := -1.0; radius := 0.0 end;
         XLellipse:begin
            efill := -1.0; elheight := 0.0; elwidth := 0.0 end;
         XLline,XLarrow,XLmove,XLarc,XLspline: begin
            endpos.xpos := 0.0; endpos.ypos := 0.0;
            height := 0.0; width := 0.0; 
            lfill := -1.0; aradius := mdistmax;
            atype := pahlex(0,XEMPTY)
            end;
         XLabel,XLaTeX: ;
         otherwise
         end
      end
   end; (* newprim *)

procedure arcenddir(pr: primitivep);
begin
   with pr@ do begin
      if |arcangle| > 0.0 then case direction of
         0: ;
         XLup:    envblock@.direction := XLleft;
         XLdown:  envblock@.direction := XLright;
         XLleft:  envblock@.direction := XLdown;
         XLright: envblock@.direction := XLup;
         otherwise
         end
      else case direction of
         0: ;
         XLup:    envblock@.direction := XLright;
         XLdown:  envblock@.direction := XLleft;
         XLleft:  envblock@.direction := XLup;
         XLright: envblock@.direction := XLdown;
         otherwise
         end
      end
   end;

procedure shift(pr: primitivep; x,y: real);
begin
   while pr <> nil do with pr@ do begin
      aat.xpos := aat.xpos + x; aat.ypos := aat.ypos + y;
      if ptype in [XLline,XLarrow,XLmove,XLspline] then begin
         endpos.xpos := endpos.xpos + x; endpos.ypos := endpos.ypos + y
         end;
      if son <> nil then shift(son,x,y);
      pr := next
      end
   end;

procedure scaleobj(pr: primitivep; s: real);
begin
   while pr <> nil do with pr@ do begin
      aat.xpos := aat.xpos * s; aat.ypos := aat.ypos * s;
      if ptype = XLbox then begin
		 boxheight := boxheight * s; boxwidth := boxwidth * s;
		 boxradius := boxradius * s
		 end
      else if ptype = XBLOCK then begin
		 blockheight := blockheight * s; blockwidth := blockwidth * s
		 end
	  else if ptype = XLcircle then radius := radius * s
	  else if ptype = XLellipse then begin
		 elheight := elheight * s; elwidth := elwidth * s
		 end
	  else if ptype = XLarc then aradius := aradius * s
      else if ptype in [XLline,XLarrow,XLmove,XLspline] then begin
         endpos.xpos := endpos.xpos * s; endpos.ypos := endpos.ypos * s
         end;
      if son <> nil then scaleobj(son,s);
      pr := next
      end
   end;

(*        corner(prim,<corner>,xval,yval);
                           Put the corner coordinates into xval,yval   *)
procedure corner(pr: primitivep; lexv: integer; var x,y: real);
var pe: primitivep;
  sb: boolean;
begin
   if pr <> nil then with pr@ do begin
      (*D if debuglevel>0 then begin write(log,
          'Corner: ptype(',ordp(pr):1,'): ptype='); printptype(ptype);
         write(log,' corner='); printcorner(lexv) end; D*)
      x := aat.xpos;
      y := aat.ypos;
      pe := pr;
      if (lexv = XEMPTY) and (ptype in [XLline,XLarrow,XLmove,XLspline])
         then begin
         while pe@.son <> nil do pe := pe@.son;
         x := 0.5 * (aat.xpos + pe@.endpos.xpos);
         y := 0.5 * (aat.ypos + pe@.endpos.ypos)
         end
      else if (lexv = XEMPTY) and (not (ptype = XLaTeX))
         then begin end
      else case ptype of
         XLbox,XLstring,XBLOCK,XLcircle,XLellipse,XLarc: begin
            x := aat.xpos; y := aat.ypos;
            initnesw; nesw(pr);
            (*D if debuglevel>0 then begin
                write(log,' aat'); wpair(log,aat.xpos,aat.ypos);
                write(log,' n,s'); wpair(log,north,south);
                write(log,' w,e'); wpair(log,west,east)
                end; D*)
            if (ptype in [XLbox,XLellipse,XLcircle,XLarc])
            and (lexv in [XDne,XDse,XDsw,XDnw]) then begin
               case ptype of
                  XLbox: begin
                     y :=Min(boxradius,
                             Min(abs(boxheight),abs(boxwidth))/2)*(1-1/sqrt(2));
                     x :=boxwidth/2-y; y := boxheight/2-y
                     end;
                  XLellipse: begin
                     x := elwidth *(0.5/sqrt(2.0));
                     y := elheight*(0.5/sqrt(2.0))
                     end;
                  XLarc: begin x := aradius/sqrt(2.0); y := x end;
                  XLcircle: begin x := radius/sqrt(2.0); y := x end
                  end;
               case lexv of
                  XDne: ; XDse: y :=-y; XDnw: x := -x;
                  XDsw: begin x := -x; y := -y end
                  end;
               x := aat.xpos + x; y := aat.ypos + y
               end
            else if (ptype = XLarc) then case lexv of
               XDn: y := aat.ypos + aradius;
               XDs: y := aat.ypos - aradius;
               XDe: x := aat.xpos + aradius;
               XDw: x := aat.xpos - aradius;
               XDc: ;
               XDstart: begin
                  x := aat.xpos + aradius * cos(|startangle|);
                  y := aat.ypos + aradius * sin(|startangle|)
                  end;
               XDend: begin
                  x := aat.xpos + aradius * cos(|startangle|+|arcangle|);
                  y := aat.ypos + aradius * sin(|startangle|+|arcangle|)
                  end
               end
            else case lexv of
               XDn: y := north; XDs: y := south;
               XDe: x := east; XDw: x := west;
               XDne: begin y := north; x := east end;
               XDse: begin y := south; x := east end;
               XDsw: begin y := south; x := west end;
               XDnw: begin y := north; x := west end;
               XDc: begin y := aat.ypos; x := aat.xpos end;
               XDstart,XDend: markerror(858)
               end
            end;
         XLline,XLarrow,XLmove,XLspline: if lexv = XDstart then begin end
            else if lexv = XDend then begin
               while pe@.son <> nil do pe := pe@.son;
               x := pe@.endpos.xpos; y := pe@.endpos.ypos
               end
            else if lexv = XDc then begin
               while pe@.son <> nil do pe := pe@.son;
               x := 0.5 * (x + pe@.endpos.xpos);
               y := 0.5 * (y + pe@.endpos.ypos)
               end
            else repeat
               with pe@ do begin
                  case lexv of
                     XDn: sb := (endpos.ypos > y);
                     XDs: sb := (endpos.ypos < y);
                     XDe: sb := (endpos.xpos > x);
                     XDw: sb := (endpos.xpos < x);
                     XDne: sb := ((endpos.ypos > y) and (endpos.xpos >=x))
                              or ((endpos.ypos >=y) and (endpos.xpos > x));
                     XDse: sb := ((endpos.ypos < y) and (endpos.xpos >=x))
                              or ((endpos.ypos <=y) and (endpos.xpos > x));
                     XDsw: sb := ((endpos.ypos < y) and (endpos.xpos <=x))
                              or ((endpos.ypos <=y) and (endpos.xpos < x));
                     XDnw: sb := ((endpos.ypos > y) and (endpos.xpos <=x))
                              or ((endpos.ypos >=y) and (endpos.xpos < x))
                     end;
                  if sb then begin x := endpos.xpos; y := endpos.ypos end
                  end;
               pe := pe@.son
            until pe = nil;
         XLabel: ;
         XLaTeX: markerror(858);
         otherwise
         end
         (*D; if debuglevel > 0 then begin write(log,' corner=');
             wpair(log,x,y); writeln(log) end D*)
      end
   end;

function nthprimobj(primp: primitivep; nth,objtype: integer): primitivep;
var prp,pp: primitivep;
   i: integer;
begin
   (*D if debuglevel = 2 then writeln(log,'nthprimobj nth=',nth:1,' type=',
      objtype:1); D*)
   prp := nil;
   if nth = 0 then while primp <> nil do begin
      (*D if debuglevel = 2 then begin
         printobject(primp);
         if primp@.ptype = objtype then begin
            prp := primp;
            writeln(log,'found[nth=',nth:1,'](',ordp(prp):1,')') end;
         primp := primp@.next
         end
      else D*) begin
         if primp@.ptype = objtype then prp := primp;
         primp := primp@.next
         end
      end
   else begin
      if nth < 0 then begin
         i := 0; pp := primp;
         while pp <> nil do begin
            if pp@.ptype = objtype then i := i+1;
            pp := pp@.next
            end;
         nth := i+1+nth
         end;
      i := 0;
      (*D if debuglevel = 2 then write(log,' nth=',nth:1); D*)
      if nth > 0 then while primp <> prp do begin
         (*D if debuglevel = 2 then printobject(primp); D*)
         if primp@.ptype = objtype then i := i+1;
         if i = nth then prp := primp
         else primp := primp@.next
         (*D;if (debuglevel = 2) and (i = nth) then
            writeln(log,'found[nth=',nth:1,'](',ordp(prp):1,')') D*)
         end
      end;
   nthprimobj := prp
   end;

procedure resetenv(envval: integer; envbl: primitivep);
var i,last: environx;
begin
   if envbl <> nil then with envbl@ do begin
      if envval = 0 then begin envval := XLenvvar+1; last := XLlastenv end
      else last := envval;
      if env = nil then begin
         new(env);
         (*D if debuglevel > 0 then
            writeln(log,'resetenv new[',ordp(env):1,']'); D*)
         end;
      for i := envval to last do case i of
         XLarcrad:     env@[i] := 0.25;     (* scaled environment vars (in) *)
         XLarrowht:    env@[i] := 0.1;
         XLarrowwid:   env@[i] := 0.05;
         XLboxht:      env@[i] := 0.5;
         XLboxrad:     env@[i] := 0.0;
         XLboxwid:     env@[i] := 0.75;
         XLcirclerad:  env@[i] := 0.25;
         XLdashwid:    env@[i] := 0.05;
         XLellipseht:  env@[i] := 0.5;
         XLellipsewid: env@[i] := 0.75;
         XLlineht:     env@[i] := 0.5;
         XLlinewid:    env@[i] := 0.5;
         XLmoveht:     env@[i] := 0.5;
         XLmovewid:    env@[i] := 0.5;
         XLtextht: if drawmode in [SVG,PDF] then env@[i] := DFONT/72
                   else env@[i] := 0.0;
         XLtextoffset: env@[i] := 2.5/72 (*.27*);
         XLtextwid:    env@[i] := 0.0;
                                            (* The following are unscaled *)
         XLarrowhead:  env@[i] := 1.0;
         XLfillval:    env@[i] := 0.5;

         XLlinethick:  env@[i] := 0.8;      (* points *)
         XLmaxpsht:    env@[i] := 11.0;     (* in *)
         XLmaxpswid:   env@[i] := 8.5;

         XLscale:      env@[i] := 1.0;
         otherwise
         end
      end
   end;

procedure inheritenv(envbl: primitivep);
var i: environx;
  pr: primitivep;
begin
   pr := findenv(envbl);
   if pr <> nil then with envbl@ do begin
      new(env);
      (*D if debuglevel > 0 then
         writeln(log,'inheritenv new[',ordp(env):1,']'); D*)
      for i := XLenvvar+1 to XLlastenv do env@[i] := pr@.env@[i]
      end
   else resetenv(0,envbl)
   end;

procedure resetscale(x: real; opr: integer; envbl: primitivep);
var r,s: real;
   i: integer;
begin
   resetenv(0,envbl);
   r := envbl@.env@[XLscale];
   eqop(envbl@.env@[XLscale],opr,x);
   s := envbl@.env@[XLscale];
   if s = 0.0 then begin 
     envbl@.env@[XLscale] := r; s := 1;
     markerror(870)
     end
   else if r = 0.0 then markerror(852)
   else s := s/r;
   for i:=XLenvvar+1 to XLlastsc do eqop(envbl@.env@[i],XLmulteq,s)
   end;
                                (* performed for each input diagram: *)
procedure inittwo;
begin
   freeinbuf := nil;
   freeseg := nil;
   freex := 0;
   freearg := nil;
   lastfillval := mdistmax;
   gslinethick := mdistmax;
   newprim(envblock,XBLOCK,nil);
   resetenv(0,envblock)
   (*D; if debuglevel > 0 then printobject(envblock) D*)
   end;

procedure deletefreeargs(var a: argp);
var na: argp;
begin
   while a <> nil do begin
      na := a@.nexta;
      (*D if debuglevel > 0 then writeln(log,'del arg[',ordp(a):1,']'); D*)
      disposebufs(a@.argbody (*D,8D*)); dispose(a);
      a := na
      end
   end;

procedure deletefreeinbufs(var p: fbufferp);
var q: fbufferp;
begin
   while p <> nil do begin
      q := p@.nextb;
      (*D if debuglevel > 0 then logaddr(p); D*)
      dispose(p@.carray); dispose(p);
      p := q
      end
   end;

function intpow(x: real; k: integer): real;
                                (* 0^(-k) does not occur *)
begin
   if k = 0 then x := 1.0                                 (* 0^0 returns 1.0 *)
   else if (x = 0.0) or (k = 1) then begin end
   else if k < 0 then x := intpow(1.0/x,-k)
   else if k = 2 then x := x*x               (* common enough to test for it *)
   else if odd(k) then x := x*intpow(x,k-1)
   else begin
      x := intpow(x,k div 2);
      x := x*x
      end;
   intpow := x
   end;

procedure getscale(xv,yv: real; lp: primitivep; var sfact,xsc: real);
(* .PS xv yv
   sfact = nominal scale factor set by scale = ...
   xsc = effective scale factor to achieve correct max picture size
   ie (size in inches)/(desired size in inches) *) 
var gs: real;
    erno: integer;
    qp: primitivep;
begin
   erno := 0;
   gs := 1.0;
   sfact := gs;
   if lp = nil then begin end
   else if lp@.ptype = XBLOCK then begin
      qp := findenv(lp);
      if qp@.env@[XLscale] > 0.0 then sfact := qp@.env@[XLscale];
             (*D if debuglevel > 0 then begin
               write(log,'getscale: sfact='); wfloat(log,sfact);
               write(log,' maxpswid='); wfloat(log,qp@.env@[XLmaxpswid]);
               write(log,' maxpsht='); wfloat(log,qp@.env@[XLmaxpsht]);
               writeln(log) end; D*)
      if (east>west) and ((east-west)/sfact > qp@.env@[XLmaxpswid]) and
         (qp@.env@[XLmaxpswid] > 0.0 ) then begin
         erno := 903;
         gs := (east-west)/qp@.env@[XLmaxpswid]
         end;
      if (north>south) and ((north-south)/sfact > qp@.env@[XLmaxpsht]) and
         (qp@.env@[XLmaxpsht] > 0.0 ) then begin
         erno := 904;
         gs := Max(gs,(north-south)/qp@.env@[XLmaxpsht])
         end
      end;
   if (xv > 0.0) and (east > west) then begin
      erno := 0;
      gs := (east-west)/sfact/xv
      end;
   if (yv > 0.0) and (north > south) and ((xv = 0.0) or
      ((north-south)/gs > yv*sfact)) then begin
      erno := 0;
      gs := (north-south)/sfact/yv
      end;
   if erno <> 0 then markerror(erno);
      (*D if debuglevel > 0 then begin write(log,' getscale=');
         wfloat(log,gs*sfact); writeln(log) end; D*)
   xsc := gs*sfact
   end;

procedure addelem(prold,prnew: primitivep);
var pp,pq: primitivep;
begin
   (*D if debuglevel > 0 then write(log,'addelem: '); D*)
   if (prold <> nil) and (prnew <> nil) then begin
      pp := prold;
      while pp@.next <> nil do begin
         pq := prnew;           (* Refuse to make a loop: *)
         while pq@.next <> nil do
            if pq@.next = pp then pq@.next := nil
            else pq := pq@.next;
         pp := pp@.next
         end;
      (*D if debuglevel > 0 then begin
         write(log,'prev element:'); snaptype(log,pp@.ptype); writeln(log);
         writeln(log,' addelem snaptype done') end; D*)
      pp@.next := prnew
      (*D; if debuglevel > 0 then begin printobject(prnew);
         writeln(log,' addelem printobject done'); flush(log);
         snaptree(prold,0); writeln(log,' addelem done') end D*)
      end
   end;

procedure copyprim(prin: primitivep; var prout: primitivep);
(* Needed because assignment of variant records is unreliable *)
var i: integer;
begin
   if prin <> nil then with prin@ do begin
      newprim(prout,ptype,nil);
      prout@.name := name; prout@.textp := textp;
      prout@.outlinep := outlinep; prout@.shadedp := shadedp;
      prout@.parent := parent; prout@.son := son; prout@.next := next;
      prout@.aat := aat; prout@.lparam := lparam; prout@.lthick := lthick;
      prout@.direction := direction; prout@.spec := spec;
      prout@.ptype := ptype;
      case ptype of
         XLbox,XLstring: begin
            prout@.boxfill := boxfill;
            prout@.boxheight := boxheight;
            prout@.boxwidth := boxwidth;
            prout@.boxradius := boxradius
            end;
         XBLOCK: begin
            prout@.blockheight := blockheight;
            prout@.blockwidth := blockwidth;
            prout@.here := here;
            for i:=0 to HASHLIM do begin
               prout@.vars[i] := vars[i];
               prout@.nvars[i] := nvars[i]
               end;
            if env <> nil then begin
               new(prout@.env);
               (*D if debuglevel > 0 then
                  writeln(log,'copyprim  new(env)[',ordp(prout@.env):1,']'); D*)
               for i := XLenvvar+1 to XLlastenv do prout@.env@[i] := env@[i]
               end
            end;
         XLcircle: begin
            prout@.cfill := cfill;
            prout@.radius := radius
            end;
         XLellipse: begin
            prout@.efill := efill;
            prout@.elheight := elheight;
            prout@.elwidth := elwidth
            end;
         XLline,XLarrow,XLmove,XLarc,XLspline: begin
            prout@.endpos := endpos;
            prout@.height := height;
            prout@.width := width;
            prout@.lfill := lfill;
            prout@.aradius := aradius;
            prout@.atype := atype
            end;
         XLabel,XLaTeX: ;
         otherwise
         end
      end
   end;

procedure deletestringbox( var pr: primitivep );
var prx: primitivep;
begin
   if pr = nil then begin end
   else if pr@.parent = nil then begin end
   else if pr@.parent@.son = nil then begin end
   else if pr@.parent@.son = pr then pr@.parent@.son := nil
   else begin
      prx := pr@.parent@.son;
      while (prx@.next<>nil) and (prx@.next<>pr) do prx := prx@.next;
      prx@.next := nil
      end;
   deletetree(pr)
   end;

(*        addsuffix(chbuf,chbufx,length,attstack@[newp+1].xval); *)
procedure addsuffix(buf:chbufp; var inx:chbufinx; var len:integer; suff:real);
var i,j,k: integer;
begin
   (*D if debuglevel <> 0 then begin writeln(log,
     'addsuffix(buf:chbufp; var inx:chbufinx; var len:integer; suff:real)');
      write(log,D*)(*DGHF ordp FHGD*)(*DM ord MD*)(*D(buf):1,
       ' inx=',inx:1,' len=',len:1,' suff=');
      wfloat(log,suff); write(log,' chbufi=',chbufi:1);
      snapname(buf,inx,len); writeln(log) end; D*)

   if chbufi-1+len > CHBUFSIZ then fatal(4);
   if inx+len <> chbufi then begin
      for i:=0 to len-1 do buf@[chbufi+i] := buf@[inx+i];
      inx := chbufi
      end;
   i := round(suff); if i < 0 then begin len := len+1; i := -i end;
   repeat
      len := len+1;
      i := i div 10
   until i = 0;
   len := len+2;
   if inx-1+len > CHBUFSIZ then fatal(4);
   buf@[inx-1+len] := ']';
   j := len-2;
   i := round(suff); if i < 0 then i := -i;
   repeat
      k := i div 10;
      buf@[inx+j] := chr(i-k*10 + ord('0'));
      i := k;
      j := j-1
   until i = 0;
   if round(suff) < 0 then begin buf@[inx+j] := '-'; j := j-1 end;
   buf@[inx+j] := '[';
   chbufi := inx+len (* ?? *)
   (*D ; if debuglevel <> 0 then begin
      snapname(buf,inx,len); writeln(log) end D*)
   end; (* addsuffix *)

procedure appendthen(var pr: primitivep);
var prp: primitivep;
begin
   copyprim(pr,prp);
   with prp@ do begin
      parent := pr; next := nil; name := nil; textp := nil; shadedp := nil;
      if outlinep<>nil then begin  (* We have to duplicate the stored string *)
         copystr(outlinep,pr@.outlinep)
         end
      end;
   FindExitPoint(pr,prp@.aat);
   pr@.son := prp;
   setthen(prp@.spec);
   pr := prp
   end;

procedure lineardir( pr:primitivep; dy,dx: real; var state:integer );
begin
   with pr@ do begin
      if not (teststflag(state,XLto) or teststflag(state,XLdirecton)) then
         endpos := aat;
      case direction of
         XLup: endpos.ypos := endpos.ypos + dy;
         XLdown: endpos.ypos := endpos.ypos - dy;
         XLleft: endpos.xpos := endpos.xpos - dx;
         XLright: endpos.xpos := endpos.xpos + dx;
         otherwise
         end
      end;
      setstflag(state,XLdirecton)
   end;

function hasoutline(lx:integer; warn:boolean): boolean;
var hs: boolean;
begin
  hs := lx in [XLbox,XLcircle,XLellipse,XLarc,XLline,XLarrow,XLspline];
  if drawmode=SVG then hs := hs or (lx=XLstring);
  if (not hs) and warn then markerror(858);
  hasoutline := hs
  end;

function hasshade(lx:integer; warn:boolean): boolean;
var hs: boolean;
begin
  if (lx in [XLbox,XLcircle,XLellipse]) then hs := true
  else if drawmode in [Pict2e,TeX,tTeX,xfig] then hs := false
  else hs := (lx in [XLarc,XLline,XLarrow,XLspline]);
  if drawmode=SVG then hs := hs or (lx=XLstring);
  if (not hs) and warn then markerror(858);
  hasshade := hs
  end;

procedure makevar(s: string; ln,nameval: integer );
var vn,lastvar,namptr: strptr;
   j,k: integer;
begin
   for j:=1 to ln do chbuf@[chbufi-1+j] := s[j];
   vn := findname(envblock,chbuf,chbufi,ln,lastvar,k);
   newstr(vn);
   j := varhash(chbuf,chbufi,ln);
   storestring(vn,chbuf,chbufi,ln);
   (*D if debuglevel > 1 then begin
       writeln(log,'makevar: envblock=',ordp(envblock):1,' eqstr val=',k:1);
       write(log,' lastvar=',ordp(lastvar):1);
       if lastvar<>nil then with lastvar@ do snapname(segmnt,seginx,len);
       writeln(log)
       end; D*)
   with envblock@ do begin
      (*D if debuglevel > 1 then writeln(log,' vn=',ordp(vn):1); D*)
      if lastvar = nil then vars[j] := vn
      else if k < 0 then begin
         if vars[j]@.next = nil then vars[j]@.next := vn
         else begin
            vn@.next := lastvar@.next;
            lastvar@.next := vn
            end
         end
      else if lastvar = vars[j] then begin
         vn@.next := vars[j];
         vars[j] := vn
         end
      else begin
         namptr := vars[j];
         while namptr@.next<>lastvar do
            namptr := namptr@.next;
         namptr@.next := vn;
         vn@.next := lastvar
         end;
      nvars[j] := nvars[j]+1
      end;
   vn@.val := nameval
   end;

(*                   This is the syntactic action routine. *)
procedure produce( newp: stackinx; p: integer );
var
   lastc: char;
   lastvar,namptr: strptr;
   lastm: fbufferp;
   macp,lastp: argp;
   primp,prp,eb: primitivep;
   i,j,k,kk,lj,ll,nexprs: integer;
   (*D kx: integer; D*)
   r,s,t,x1,y1,dx,dy,ts: real;
   bswitch: boolean;

begin (*produce*)
(*D if (debuglevel > 0) then begin
      writeln(log);
      writeln(log, 'Production(newp=',newp:1,
                   ',lexval=',attstack@[newp].lexval:1,
                   ',p=',p:1,')' );
      flush(log)
      end; D*)

   with attstack@[newp] do case p of

   -2: inittwo;

   -1: begin
      if envblock<>nil then envblock@.direction := XLright ;
      inlogic := false;
      inbuf := nil
      end;

(*      METAGOAL = input "<EOF>"   *)
   METAGOAL1: ;

(*      input = "<EMPTY>"   *)
(*            | input picture NL   *)
   input1: ;
   input2: begin
      (*D if debuglevel > 0 then writeln(log,'deletetree:');
          if linesignal > 0 then writeln(errout,'deletetree:');flush(errout);D*)
      deletetree(envblock);

      (*D if debuglevel > 0 then write(log,'deletefreeargs: ');
          if linesignal > 0 then writeln(errout,'deletefreeargs:');
          flush(errout);D*)
      deletefreeargs(freearg);

      (*D if debuglevel > 0 then write(log,'deletefreeinbufs: ');
          if linesignal > 0 then writeln(errout,'deletefreeinbufs:');
          flush(errout);D*)
      deletefreeinbufs(freeinbuf);

      (*D if debuglevel > 0 then writeln(log,'inittwo:');
          if linesignal > 0 then writeln(errout,'inittwo:'); flush(errout); D*)
      inittwo;

      if envblock<>nil then envblock@.direction := XLright
      end;

(*      picture = start NL elementlist "<END>"   *)
(*              | start NL elementlist NL "<END>"   *)
   picture1,picture2: begin
      if prim <> nil then attstack@[newp+2].prim := prim;
      with attstack@[newp+2] do begin
         getnesw( prim );
            (*D if debuglevel > 0 then begin
              writeln(log,' Global dimensions:'); write(log,'(n,s)(e,w)=');
              wpair(log,north,south); wpair(log,east,west);
              writeln(log,' envblock<>nil:',envblock<>nil); flush(log) end; D*)
         if envblock <> nil then with envblock@ do begin
            aat.xpos := (east+west) * 0.5;
            aat.ypos := (north+south) * 0.5;
            blockheight := north-south; blockwidth := east-west
            end;
         if drawmode = xfig then begin
            shift(envblock, -west,-south );
            north := north-south; east := east-west;
            west := 0; south := 0
            end
         else if (drawmode in [SVG,PDF,PS]) and (envblock<>nil) then begin
            if envblock@.env<>nil then begin
               r := envblock@.env@[XLlinethick]/2/72*envblock@.env@[XLscale];
                  (*D if debuglevel > 0 then begin
                    write(log,' west='); wfloat(log,west);
                    write(log,' south='); wfloat(log,south);
                    write(log,' r='); wfloat(log,r);
                    write(log,' shift=('); wfloat(log,-west+r);
                    write(log,','); wfloat(log,-south+r);
                    writeln(log,')'); flush(log) end; D*)
               shift(envblock, -west+2*r,-south+2*r);
               north := north-south+3*r; east := east-west+3*r;
               west := r; south := r
               end
            end;
         xfheight := north;
         with attstack@[newp] do getscale( xval,yval,envblock,scale,fsc );
            (*D if debuglevel > 0 then begin flush(log);
              writeln(log,'After shift:'); printobject( envblock );
              printobject( envblock@.son ); writeln(log);
              write(log,'Starting drawtree ================= ');
              with attstack@[newp] do if (xval > 0.0) and (east > west) then
                 begin write( log,'fsc='); wfloat(log, fsc ) end;
              writeln(log);
              snaptree(envblock,0);
              writeln(log); flush(log) end; D*)
         drawtree(north,south,east,west,envblock)
         end
      end;

(*      NL = ["^M"]"<NL>"   *)
(*         | "<ERROR>"   *)
   NL1,NL2: ;

(*      start = "<START>"   *)
(*            | "<START>" term   *)
(*            | "<START>" term term   *)
   start1,start2,start3: begin
      makevar('dpicopt',     7, drawmode);
      if safemode then i := 1 else i := 0;
      makevar('optsafe',     7, i);
      makevar('optMFpic',    8, MFpic);
      makevar('optMpost',    8, MPost);
      makevar('optPDF',      6, PDF);
      makevar('optPGF',      6, PGF);
      makevar('optPict2e',   9, Pict2e);
      makevar('optPS',       5, PS);
      makevar('optPSfrag',   9, PSfrag);
      makevar('optPSTricks',11, PSTricks);
      makevar('optSVG',      6, SVG);
      makevar('optTeX',      6, TeX);
      makevar('opttTeX',     7, tTeX);
      makevar('optxfig',     7, xfig);
      if p<>start1 then begin
         xval := attstack@[newp+1].xval;
         if p=start3 then yval := attstack@[newp+2].xval
         end
      (*D; if debuglevel > 0 then begin
          write(log,'.PS');
          if p<>start1 then begin
             write(log,' '); wfloat(log,xval);
             if p=start3 then begin write(log,' '); wfloat(log,yval) end
             end;
          writeln(log);
          writeln(log,' dpicopt=',drawmode:1,' optsafe=',i:1);
          flush(log)
          end; D*)
      end;

(*      elementlist = "<EMPTY>"   *)
   elementlist1: state := 0;
(*                  | element   *)
   elementlist2: begin
      state := 0;
      if (prim <> nil) and (lexval<>XLcontinue) then begin
         i := newp-1; j := 1;
         (*D if debuglevel>0 then write(log,
            ' Elementlist2: Searching for last drawn element, newp=',newp:1);D*)
         while i > j do if
            (attstack@[i].lexval in [XNL,XLBRACE,XFOR,XLendfor,XLabel])
            and (attstack@[i].prim = nil) then i := i-1 else j := i;
         (*D if debuglevel>0 then begin
            if i = 1 then writeln(log,' Not found') else begin
              write(log,' Found:');
              writeln(log,', attstack@[',i:1,'].lexval=',attstack@[i].lexval:1);
              if attstack@[i].prim<>nil then begin
                writeln(log,'attstack@[',i:1,'].prim=');
                snaptype(log,attstack@[i].prim@.ptype)
                end end end; D*)
         if attstack@[i].prim <> nil then begin
            addelem(attstack@[i].prim,prim);
            (*D if debuglevel > 0  then begin
               writeln(log,' From stack[',newp:1,'] to [',i:1,']');
               printobject(attstack@[i].prim) end; D*)
            state := i-newp
            end
         else if attstack@[i].lexval = XBRACKETL then begin
            attstack@[i+1].prim := prim;
            (*D if debuglevel > 0 then begin
               writeln(log,' From stack[',newp:1,'] to [',i+1:1,']');
               printobject(attstack@[i+1].prim) end; D*)
            state := i+1-newp
            end
         else begin
            attstack@[i].prim := prim;
            (*D if debuglevel > 0 then begin
               writeln(log,' state[',newp:1,']=',i-1:1);
               printobject(attstack@[i].prim) end; D*)
            state := i-newp
            end
         end
      end;
(*                  | elementlist NL element *)
   elementlist3: if (attstack@[newp+2].prim=nil) then begin end
      else if (attstack@[newp+2].lexval <> XLcontinue) then begin
      if prim = nil then begin
         attstack@[newp] := attstack@[newp+2];
         doprod(elementlist2)
         end
      else addelem(attstack@[newp+state].prim,attstack@[newp+2].prim)
      end;

(*      term = factor   *)
(*           | term "*" factor   *)
(*           | term "/" factor   *)
(*           | term "%" factor   *)
   term1: ;
   term2: xval := xval * attstack@[newp+2].xval;
   term3: if attstack@[newp+2].xval = 0.0 then begin
         markerror(852);
         xval := 0.0
         end
      else xval := xval / attstack@[newp+2].xval;
   term4: begin
      i := round(xval); j := round(attstack@[newp+2].xval);
      if j = 0 then begin markerror(852); xval := 0.0 end
      else xval := i - (i div j)*j
      end;

(*      element = namedobj   *)
(*              | "<Label>" suffix ":" position   *)
(*              | assignlist   *)
(*              | "<directon>"   *)
(*              | "<LaTeX>"   *)
(*              | command   *)
(*              | lbrace "lbrace" elementlist optnl "rbrace"   *)
(*              | ifpart   *)
(*              | elsehead "lbrace" elementlist optnl "rbrace"   *)
(*              | for "rbrace"   *)
(*              | "command" stringexpr   *)
(*              | "exec" stringexpr   *)
   element1: if prim <> nil then begin
      prp := prim;
      if prim@.ptype in [XLline,XLarrow,XLmove,XLspline] then begin
         if startchop <> 0.0 then with prim@ do begin
            dx := endpos.xpos-aat.xpos; dy := endpos.ypos-aat.ypos;
            s := linlen( dx,dy );
            (*D if debuglevel = 2 then begin
               write(log,' element1 startchop='); wfloat(log,startchop);
               write(log,' (dx,dy)='); wpair(log,dx,dy);
               write(log,' s='); wfloat(log,s); writeln(log) end; D*)
            if s <> 0.0 then begin
               t := startchop/s;
               aat.xpos := aat.xpos + t*dx;
               aat.ypos := aat.ypos + t*dy
               end;
            end;
         while prp@.son<>nil do prp := prp@.son;
         if endchop <> 0.0 then with prp@ do begin
            dx := endpos.xpos-aat.xpos; dy := endpos.ypos-aat.ypos;
            s := linlen( dx,dy );
            (*D if debuglevel = 2 then begin
               write(log,' element1 endchop='); wfloat(log,endchop);
               write(log,' (dx,dy)='); wpair(log,dx,dy);
               write(log,' s='); wfloat(log,s); writeln(log) end; D*)
            if s <> 0.0 then begin
               t := endchop/s;
               endpos.xpos := endpos.xpos - t*dx;
               endpos.ypos := endpos.ypos - t*dy
               end
            end
         end;
      FindExitPoint(prp,envblock@.here)
      (*D ; if debuglevel > 0 then begin
         write(log,' element1 Here=');
         with envblock@ do wpair(log,here.xpos,here.ypos); writeln(log);
         printobject(prim)
         end D*)
      end;
(*              | "<Label>" suffix ":" position   *)
   element2: begin
      if attstack@[newp+1].lexval <> XEMPTY then
         addsuffix(chbuf,chbufx,length,attstack@[newp+1].xval);
      prp := findplace(envblock@.son,chbuf,chbufx,length);
      if prp = nil then begin
         newprim(prim,XLabel,envblock);
         newstr(prim@.name);
         storestring(prim@.name,chbuf,chbufx,length);
         prp := prim
         end
      else if prp@.ptype <> XLabel then begin
         newprim(prim,XLabel,envblock);
         prim@.name := prp@.name;
         prp@.name := nil;
         prp := prim
         end;
      with prp@ do begin
         aat.xpos := attstack@[newp+3].xval;
         aat.ypos := attstack@[newp+3].yval
         end
      (*D; if (debuglevel > 0) and (prim<>nil) then printobject(prim)D*)
      end;
(*              | assignlist   *)
   element3: lexval := XLBRACE;
(*              | "<directon>"   *)
   element4: begin
      envblock@.direction := lexval;
      lexval := XLBRACE
      end;
(*              | "<LaTeX>"   *)
   element5: begin
      newprim( prim,XLaTeX,envblock );
      newstr(prim@.textp);
      storestring(prim@.textp,chbuf,chbufx,length)
      end;
(*              | command   *)
   element6: lexval := XLBRACE;
(*              | lbrace "lbrace" elementlist optnl "rbrace"   *)
   element7: begin
      envblock@.here.xpos := xval;
      envblock@.here.ypos := yval;
      if state in [XLup,XLdown,XLleft,XLright] then
         envblock@.direction := state;
      if attstack@[newp+2].state = 0 then attstack@[newp] := attstack@[newp+2]
      end;
(*              | ifpart   *)
   element8: ;
(*              | elsehead "lbrace" elementlist optnl "rbrace"   *)
   element9: ;
(*              | for "rbrace"   *)
   element10: ;
(*              | "command" stringexpr   *)
   element11: if attstack@[newp+1].prim <> nil then begin
      newprim( prim,XLaTeX,envblock );
      prim@.textp := attstack@[newp+1].prim@.textp;
      attstack@[newp+1].prim@.textp := nil;
      if envblock@.son = attstack@[newp+1].prim then envblock@.son := prim;
      deletestringbox(attstack@[newp+1].prim)
      end;
(*              | "exec" stringexpr   *)
   element12: begin
      if attstack@[newp+1].prim = nil then begin end
      else if attstack@[newp+1].prim@.textp = nil then begin end
      else if attstack@[newp+1].prim@.textp@.segmnt = nil then begin end
      else with attstack@[newp+1].prim@.textp@ do begin
         (*D if debuglevel > 0 then write(log,'"exec" stringexpr:'); D*)
         newbuf(lastm);
         lastm@.carray@[1] := nlch; lastm@.savedlen := 1; copyleft(lastm,inbuf);
         for i:= 1 to len do lastm@.carray@[i] := segmnt@[seginx-1+i];
         lastm@.savedlen := len; copyleft(lastm,inbuf);
         lastm@.carray@[1] := nlch; lastm@.savedlen := 1; copyleft(lastm,inbuf);
         deletestringbox(attstack@[newp+1].prim);
         disposebufs(lastm (*D,2D*))
         end;
      lexval := XLBRACE
      end;
(*              | loop "rbrace" *)
(*              | break *)
   (*repeat element13,element14: ; *)

(*      lbrace = "<EMPTY>"   *)
   lbrace1: begin
      xval := envblock@.here.xpos;
      yval := envblock@.here.ypos;
      state := envblock@.direction
      end;

(*      namedobj = object   *)
   namedobj1: if prim <> nil then begin (* then, arc, deferred shift *)
      prp := prim;
      while isthen(prim) do prim := prim@.parent;
      if prp <> prim then begin
         prim@.name := prp@.name; prp@.name := nil end;
      if prim@.ptype = XLarc then arcenddir(prp);
      if teststflag(state,XLat) then with prim@ do begin
         i := getstval(state);
         if i = XLfloat then begin (* pair at *)
            getnesw( prim ); dx := west + startchop; dy := south + endchop
            end
         else if internal <> nil then corner(internal,i,dx,dy)
         else corner(prim,i,dx,dy);
         internal := nil;
         if ptype <> XLarc then shift(prim,xval-dx,yval-dy)
         else begin
            x1 := aat.xpos + aradius * cos(|startangle|); (* from *)
            y1 := aat.ypos + aradius * sin(|startangle|);
            if teststflag(state,XLto) (* to X from Here|Y implied *) then begin
               if not (i in [XDc,XEMPTY]) then markerror(858);
               r  := aat.xpos + aradius * cos(|startangle|+|arcangle|); (* to *)
               s  := aat.ypos + aradius * sin(|startangle|+|arcangle|);
               aat.xpos := xval; aat.ypos := yval;
               aradius := linlen( r-aat.xpos, s-aat.ypos );
               setangles(|startangle|,|arcangle|,aat,x1,y1,r,s)
               end
            else if teststflag(state,XLfrom) then begin
               if not (i in [XDc,XEMPTY]) then markerror(858);
               aat.xpos := xval; aat.ypos := yval;
               t := datan((y1-aat.ypos),(x1-aat.xpos));
               r := aat.xpos + aradius*cos(t+|arcangle|);
               s := aat.ypos + aradius*sin(t+|arcangle|);
               aradius := linlen( x1-aat.xpos, y1-aat.ypos );
               setangles(|startangle|,|arcangle|,aat,x1,y1,r,s)
               end
            else shift(prim,xval-dx,yval-dy)
            end
         (*D; if debuglevel > 0 then printobject(prim); D*)
         end
      end;
(*               | "<Label>" suffix ":" object   *)
   namedobj2: if attstack@[newp+3].prim <> nil then begin
      if attstack@[newp+1].lexval <> XEMPTY then
         addsuffix(chbuf,chbufx,length,attstack@[newp+1].xval);
      primp := findplace(envblock@.son,chbuf,chbufx,length);
      if primp <> nil then begin
         attstack@[newp+3].prim@.name := primp@.name;
         primp@.name := nil
         end
      else with attstack@[newp+3].prim@ do begin
         (*D if debuglevel = 2 then
            writeln(log,'Label not found, creating it'); D*)
         newstr(name);
         storestring(name,chbuf,chbufx,length)
         end;
      attstack@[newp] := attstack@[newp+3];
      doprod(namedobj1)
      end;

(*      suffix = "<EMPTY>"   *)
(*             | "[" expression "]"   *)
   suffix1: lexval := XEMPTY;
   suffix2: xval := attstack@[newp+1].xval;

(*      position = pair   *)
   position1: ;
(*               | expression "between" position "and" position   *)
(*               | expression "<" position "," position "<compare>" shift   *)
   position2,position4: begin
      r := xval;
      xval := attstack@[newp+2].xval +
         r * (attstack@[newp+4].xval - attstack@[newp+2].xval);
      yval := attstack@[newp+2].yval +
         r * (attstack@[newp+4].yval - attstack@[newp+2].yval);
      if p<>position4 then begin end
      else if attstack@[newp+5].lexval <> XGT then markerror(869)
      else if attstack@[newp+6].lexval <> XEMPTY then begin
         xval := xval + attstack@[newp+6].xval;
         yval := yval + attstack@[newp+6].yval
         end
      end;
(*             | expression "of" "the" "way" "between" position "and" position*)
   position3: begin
      r := xval;
      xval := attstack@[newp+5].xval +
         r * (attstack@[newp+7].xval - attstack@[newp+5].xval);
      yval := attstack@[newp+5].yval +
         r * (attstack@[newp+7].yval - attstack@[newp+5].yval)
      end;

(*      assignlist = assignment   *)
(*                 | assignlist "," assignment   *)
assignlist1: ;
assignlist2: xval := attstack@[newp+2].xval ;

(*      command = "print" expression redirect   *)
(*              | "print" position redirect   *)
(*              | "print" stringexpr redirect   *)
   command1,command2,command3: with attstack@[newp+1] do begin
      if attstack@[newp+2].lexval=XEMPTY then begin
         case p of
            command1: wfloat(errout, xval);
            command2: wpair(errout, xval,yval);
            command3: if prim<>nil then wstring(errout, prim@.textp )
            end;
         writeln(errout); consoleflush
         end
      else if attstack@[newp+2].state=0 then begin
         (*P2CC #ifndef SAFE_MODE *)
         case p of
            command1: wfloat(redirect, xval);
            command2: wpair(redirect, xval,yval);
            command3: if prim<>nil then wstring(redirect, prim@.textp)
            end;
         writeln(redirect); close(redirect)
         (*P2CC #endif *)
         end;
      if p=command3 then deletestringbox( prim )
      end;
(*              | "reset"   *)
   command4: resetenv(0,envblock);
(*              | "reset" resetlist   *)
   command5: ;
(*              | systemcmd   *)
   command6: ;

(*              | "copy" stringexpr   *)
   command7: with attstack@[newp+1] do if prim <> nil then begin
      (*P2CC #ifdef SAFE_MODE *) (*P2CP markerror(901); *)
      (*P2CC #else *) pointinput(prim@.textp);
      (*P2CC #endif *)
      deletestringbox( prim )
      end;
(*              | defhead optnl "lbrace" "rbrace"   *)
   command8: ;
(*              | "undefine" "<name>"   *)
(*              | "undefine" "<Label>"   *)
   command9,command10: begin
      attstack@[newp] := attstack@[newp+1];
      macp := findmacro(macros,chbuf,chbufx,length,lastp);
      if macp <> nil then begin
         if lastp <> nil then lastp@.nexta := macp@.nexta;
         if macp = macros then macros := macp@.nexta;
         disposebufs(macp@.argbody (*D,5D*) );
         dispose(macp); macp := nil
         end
      end;

(*      optnl = "<EMPTY>"   *)
(*            | NL   *)
   optnl1,optnl2: ;

(*      ifpart = ifhead "lbrace" elementlist optnl "rbrace"   *)
   ifpart1: ;

(*      elsehead = ifpart "else"   *)
   elsehead1: if xval = 0.0 then begin
         attstack@[newp+1].lexval := XLBRACE;
         lexval := XLBRACE
         end
      else begin
         currprod := elsehead1;
         skiptobrace
         end;

(*      for = forhead "lbrace" elementlist optnl   *)
(*          | for forincr "<endfor>" elementlist optnl   *)
   for1,for2: ;

(*      stringexpr = string   *)
   stringexpr1: ;
(*                 | stringexpr "+" string   *)
   stringexpr2: if attstack@[newp+2].prim <> nil then with prim@ do begin
   prp := attstack@[newp+2].prim;
   boxwidth := boxwidth + prp@.boxwidth;
   boxheight := Max(boxheight,prp@.boxheight);
   if prp@.textp = nil then begin end
   else if textp = nil then begin textp := prp@.textp; prp@.textp := nil end
   else if (textp@.segmnt = prp@.textp@.segmnt) and
           (textp@.seginx+textp@.len = prp@.textp@.seginx) then begin
      (*D if debuglevel > 0 then writeln(log,' stringexpr2 branch 1,',
         ' seginx,length=',textp@.seginx:1,',',textp@.len:1,
         ' seginx,length=',prp@.textp@.seginx:1,',',prp@.textp@.len:1); D*)
      textp@.len := textp@.len + prp@.textp@.len;
      putbval(textp@.segmnt,bval(textp@.segmnt)-1);
      prp@.textp@.segmnt := nil
      end
   else appendstring(
      textp,prp@.textp@.segmnt,prp@.textp@.seginx,prp@.textp@.len);
   deletetree(attstack@[newp+2].prim)
   end;
(*      string = "<string>"   *)
   string1: begin
      newprim(prim,XLstring,envblock);
      eb := findenv(envblock);
      with prim@ do begin
         boxheight := eb@.env@[XLtextht];
         boxwidth := eb@.env@[XLtextwid];
         if (drawmode = xfig) and (boxwidth = 0.0) then begin
            (* To keep xfig from crashing, assume text height is 0.1
               and a character is 0.1*0.75 wide *)
            eb := findenv(envblock);
            if boxheight = 0.0 then boxheight := 0.1*eb@.env@[XLscale];
            boxwidth := boxheight*length*0.75
            end;
         if (drawmode=PDF) and (boxwidth=0.0) then
            boxwidth := boxheight*length*0.6;
         boxradius := 0.0;
         (*D if debuglevel > 0 then write(log,'string1 '); D*)
         newstr(textp);
         storestring(textp,chbuf,chbufx,length)
         end
      end;
(*             | sprintf ")"   *)
   string2: ;

(*      assignment = "<name>" suffix "=" expression   *)
(*                 | "<name>" suffix "=" assignment   *)
   assignment1,assignment2: begin
      if attstack@[newp+1].lexval <> XEMPTY then
         addsuffix(chbuf,chbufx,length,attstack@[newp+1].xval);
      varname := findname(envblock,chbuf,chbufx,length,lastvar,k);
      if (varname = nil) and (attstack@[newp+2].lexval <> XEQ) then
         varname := glfindname(envblock@.parent,chbuf,chbufx,length,namptr,kk);
      if varname = nil then begin
         newstr(varname);
         j := varhash(chbuf,chbufx,length);
         storestring(varname,chbuf,chbufx,length);
            (*D if debuglevel > 1 then begin
                writeln(log,'assignment1: envblock=',ordp(envblock):1,
                        ' eqstr val=',k:1);
                write(log,' lastvar=',ordp(lastvar):1);
                        if lastvar<>nil then with lastvar@ do
                          snapname(segmnt,seginx,len); writeln(log) end; D*)
         with envblock@ do begin
            (*D if debuglevel > 1 then begin writeln(log,
                         ' varname=',ordp(varname):1); prvars(newp) end; D*)
            if lastvar = nil then vars[j] := varname
            else if k < 0 then begin
               if vars[j]@.next = nil then vars[j]@.next := varname
               else begin
                  varname@.next := lastvar@.next;
                  lastvar@.next := varname
                  end
               end
            else if lastvar = vars[j] then begin
               varname@.next := vars[j];
               vars[j] := varname
               end
            else begin
               namptr := vars[j];
               (* while (namptr@.next<>nil) and (namptr@.next<>lastvar) do *)
               while namptr@.next<>lastvar do
                  namptr := namptr@.next;
               namptr@.next := varname;
               varname@.next := lastvar
               end;
            nvars[j] := nvars[j]+1
            end;
         varname@.val := 0
         end;
      if attstack@[newp+2].lexval = XEQ then
         varname@.val := attstack@[newp+3].xval
      else
         eqop(varname@.val,attstack@[newp+2].lexval,attstack@[newp+3].xval);
      xval := varname@.val
      end;
(*                 | "<envvar>" "=" expression   *)
(*                 | "<envvar>" "=" assignment   *)
   assignment3,assignment4: if envblock <> nil then begin
      if lexval = XLscale then
         resetscale(attstack@[newp+2].xval,attstack@[newp+1].lexval,envblock)
      else if (lexval = XLarrowhead) and (drawmode = TeX) and
         (attstack@[newp+2].xval = 0.0) then markerror(858)
      else begin
         if envblock@.env = nil then inheritenv(envblock);
         eqop(envblock@.env@[lexval],
            attstack@[newp+1].lexval,attstack@[newp+2].xval)
         end;
      xval := envblock@.env@[lexval];
      (*D if debuglevel > 0 then begin
         write(log,
         ' Assignment3or4 envblock[',ordp(envblock),']: lexval=',lexval:1,
         ' value='); wfloat(log,envblock@.env@[lexval]); writeln(log) end; D*)
      startchop := lexval;
      if lexval in [XLlinethick,XLdashwid] then begin
         newprim( prim,XLaTeX,envblock );
         if lexval = XLlinethick then prim@.lthick := xval
         else prim@.lparam := xval
         end
      else
         lexval := XLBRACE
      end;

(*      expression = term   *)
(*                 | "+" term   *)
(*                 | "-" term   *)
(*                 | expression "+" term   *)
(*                 | expression "-" term   *)
   expression1: ;
   expression2: xval :=  attstack@[newp+1].xval;
   expression3: xval := -attstack@[newp+1].xval;
   expression4: xval := xval+attstack@[newp+2].xval;
   expression5: xval := xval-attstack@[newp+2].xval;

(*      ifhead = setlogic "if" logexpr "then"   *)
   ifhead1: begin
      inlogic := false;
      xval := attstack@[newp+2].xval;
      (*D if debuglevel > 0 then begin
          if xval <> 0.0 then writeln(log,' logexpr=true')
          else writeln(log,'logexpr=false')
          end; D*)
      if xval <> 0.0 then for i := 1 to 3 do attstack@[newp+i].lexval := XLBRACE
      else begin
         currprod := ifhead1;
         skiptobrace
         end;
      lexval := XLBRACE
      end;

(*      setlogic = "<EMPTY>"   *)
   setlogic1:  inlogic := true;

(*      logexpr = logprod   *)
(*              | logexpr "||" logprod   *)
   logexpr1: ;
   logexpr2: if (xval <> 0.0) or (attstack@[newp+2].xval <> 0.0)
      then xval := 1.0 else xval := 0.0;

(*      forhead = "for" assignlist "to" expression do   *)
   forhead1: begin
      xval := attstack@[newp+1].xval;   (* initial value  *)
      yval := attstack@[newp+4].xval;   (* increment *)
      endchop := attstack@[newp+3].xval;(* final value *)
      length := 0;
      if xval = endchop then begin
         length := -1; yval := 1 end
      else if attstack@[newp+4].lexval = XSTAR then begin
         t := 0.0;
         if xval = 0.0 then length := 860
         else t := endchop*xval;
         if (t < 0.0) (* or (yval < 0.0) *) then length := 862
         else if (t = 0.0) and (abs(yval*xval) <> 0.0) then length := 860
         else if (yval = 0.0) and (t <> 0.0) then length := 860
         else if (abs(yval)=1) and (abs(xval)<>abs(endchop)) then length := 860
         else length := 1
         end
      else if yval = 0.0 then length := 860
      else if (endchop-xval)*yval > 0 then length := -1;
      if length > 800 then begin
         markerror(length); length := 0 end;
                                    (* store loop variable *)
      startchop := attstack@[newp+1].startchop;
      if startchop = 0.0 then begin
         varname := attstack@[newp+1].varname;
         varname@.val := xval
         end
      else if round(startchop) <> XLscale then begin
         if envblock@.env = nil then inheritenv(envblock);
         envblock@.env@[round(startchop)] := xval
         end
      else resetscale(xval,XEQ,envblock);

      currprod := forhead1;
      if length = 0 then skiptobrace
      else begin
         for i := 0 to 4 do attstack@[newp+i].lexval := XLBRACE;
         readfor(nil,newp,inbuf)
         end
      (*D; if debuglevel > 0 then begin
         write(log,'for: initial='); wfloat(log,xval);
         write(log,' final='); wfloat(log,endchop);
         write(log,' incr='); wfloat(log,yval);
         writeln(log) end D*)
      end;

(*      forincr = "<EMPTY>"   *)
   forincr1: begin
      lexval := XLBRACE;
      with attstack@[newp-1] do begin
         if varname <> nil then xval := varname@.val
         else xval := envblock@.env@[round(startchop)];
         bswitch := false;
         if length < 0 then begin
            if yval = 0 then bswitch := true
            else begin
               xval := xval + yval;
               if ((yval > 0) and (xval > endchop)) or
                  ((yval < 0) and (xval < endchop)) then bswitch := true
               end
            end
         else if xval = 0 then bswitch := true
         else begin
            xval := xval * yval;
            if ((abs(yval) >= 1.0) and (abs(xval) > abs(endchop))) or
               ((abs(yval) < 1.0) and (abs(xval) < abs(endchop))) then
               bswitch := true
            end;
         if varname <> nil then varname@.val := xval
         else if round(startchop) <> XLscale then
            envblock@.env@[round(startchop)] := xval
         else resetscale(xval,XEQ,envblock);
         (*D if debuglevel > 0 then begin write(log,'forincr1: xval=');
             wfloat(log,xval); writeln(log,' bswitch=',bswitch) end; D*)
         if bswitch then begin
            while inbuf@.attrib < 0 do begin
               lastm := inbuf; inbuf := inbuf@.higherb;
               disposebufs(lastm (*D,4D*) )
               (*D; if debuglevel > 0 then wrbuf(inbuf,3,0) D*)
               end;
            lastm := inbuf; inbuf := inbuf@.higherb;
            disposebufs(lastm (*D,41D*) )
            (*D; if debuglevel > 0 then wrbuf(inbuf,3,0) D*)
            end
         end
      end;

(*      loop = loophead "{" elementlist optnl *)
(*           | loopincr elementlist optnl *)
   (*repeat loop1,loop2: ; *)

(*      do = "do"   *)
(*         | by expression "do"   *)
   do1: xval := 1.0;
   do2: xval := attstack@[newp+1].xval;

(*      loophead = "repeat" *)
   (*repeat loophead1: ; *) (* Do not enter into nullprod file *)

(*      loopincr = loop "~" *)
   (*repeat loopincr1: attstack@[newp+1].lexval := XLBRACE ; *)

(*      by = "by"   *)
(*         | "by" "*"   *)
   by1: ;
   by2: lexval := attstack@[newp+1].lexval;

(*      redirect = "<EMPTY>" *)
   redirect1: lexval := XEMPTY;
(*               | "<compare>" stringexpr  *)
   redirect2: with attstack@[newp+1] do begin
      attstack@[newp].state := 1; bswitch := false;
      if prim=nil then begin end
      else if prim@.textp=nil then markerror(861)
      else if (p=redirect2) and(attstack@[newp].lexval<>XGT) then markerror(869)
      else if safemode then markerror(901)
      else bswitch := true;
      (*P2CC #ifdef SAFE_MODE *)
      (*P2CP if bswitch then markerror(901); *)
      (*P2CC #else *)
      if bswitch then pointoutput(true,prim@.textp,attstack@[newp].state);
      (*P2CC #endif *)
      deletestringbox( prim )
      end;
(*               | "<compare>" "<compare>" stringexpr   *)
   redirect3: with attstack@[newp+2] do begin
      attstack@[newp].state := 1; bswitch := false;
      if prim=nil then begin end
      else if prim@.textp=nil then markerror(861)
      else if (attstack@[newp].lexval<>XGT) or (attstack@[newp+1].lexval<>XGT)
         then markerror(869)
      else if safemode then markerror(901)
      else bswitch := true;
      (*P2CC #ifdef SAFE_MODE *)
      (*P2CP if bswitch then markerror(901); *)
      (*P2CC #else *)
      if bswitch then pointoutput(false,prim@.textp,attstack@[newp].state);
      (*P2CC #endif *)
      deletestringbox( prim )
      end;

(*      resetlist = "<envvar>"   *)
(*                | resetlist "," "<envvar>"   *)
   resetlist1: resetenv(lexval,envblock);
   resetlist2: resetenv(attstack@[newp+2].lexval,envblock);

(*      systemcmd = "sh" stringexpr   *)
   systemcmd1: begin
      xval := -1;
      with attstack@[newp+1] do if prim <> nil then begin
         if prim@.textp = nil then begin end
         else with prim@.textp@ do if segmnt = nil then begin end
         else if seginx+len >= CHBUFSIZ then markerror(866)
         else if safemode then markerror(901)
         else begin
            (*P2CC #ifndef SAFE_MODE *)
            segmnt@[seginx+len] := chr(0);
            attstack@[newp].xval := system(segmnt@[seginx])
            (*P2CC #endif *)
            end;
         deletestringbox( prim )
         end
      end;

(*      defhead = "define" "<name>"   *)
(*              | "define" "<Label>"   *)
   defhead1,defhead2: begin
      attstack@[newp] := attstack@[newp+1];
      macp := findmacro(macros,chbuf,chbufx,length,lastp);
      if macp = nil then begin
         newarg(macp);
         if lastp = nil then macros := macp
         else lastp@.nexta := macp
         end;
      disposebufs(macp@.argbody (*D,6D*) ); newbuf(macp@.argbody);
      with macp@.argbody@ do begin (* copy the macro name *)
         for i := 1 to length do carray@[i] := chbuf@[chbufx-1+i];
         savedlen := length; readx := length+1 end;
      currprod := p;
      readfor(macp@.argbody,-length,macp@.argbody); (* append the body *)
      lastm := macp@.argbody; while lastm@.nextb<>nil do lastm := lastm@.nextb;
      with lastm@ do carray@[savedlen] := etxch
      (*D; if debuglevel > 1 then begin
         writeln(log);
         if p=defhead1 then write(log,'defhead1') else write(log,'defhead2');
         lastm := macp@.argbody; while lastm<> nil do begin
            wrbuf(lastm,3,0); lastm := lastm@.nextb end
         end D*)
      end;

(*      sprintf = "sprintf" "(" stringexpr   *)
(*              | "sprintf" "(" stringexpr "," exprlist   *)
   sprintf1,sprintf2: begin
      newprim(prim,XLstring,envblock); with prim@ do begin
         eb := findenv(envblock);
         (*D if debuglevel <= 0 then begin end
             else if eb=nil then writeln(log,' ! sprintf2: eb=nil')
             else if eb@.env=nil then writeln(log,' ! sprintf2: env=nil'); D*)
         boxheight := eb@.env@[XLtextht];
         boxwidth := eb@.env@[XLtextwid];
         boxradius := 0.0;
         newstr(textp)
         end;
      if p=sprintf1 then nexprs := 0    (* no of expression arguments *)
      else nexprs := attstack@[newp+4].state;
      if tmpbuf = nil then begin
         new(tmpbuf);
         (*D if debuglevel > 0 then begin write(log,'sprintf1,2 tmpbuf[');
            writeln(log,ordp(tmpbuf):1,']') end; D*)
         end;
      i := 0;                       (* expression count *)
      j := 0;                       (* end of current substring *)
      lj := j;                      (* start of current substring *)
      kk := 0;                      (* substring count *)
      if attstack@[newp+2].prim = nil then begin
         (*D if debuglevel > 0 then writeln(log,' sprintf2: prim[2] = nil'); D*)
         end
      else if attstack@[newp+2].prim@.textp = nil then begin
         (*D if debuglevel > 0 then writeln(log,' sprintf2: textp=nil'); D*)
         end
      else with attstack@[newp+2].prim@.textp@ do while j < len do begin (*-- *)
      (*D if debuglevel > 0 then begin writeln(log,
            ' sprintf looping, nexprs,i,j,lj,len=',nexprs:4,i:4,j:4,lj:4,len:4,
            ' c="', segmnt@[seginx+j],'"'); flush(log) end; D*) (* end; below *)
      if segmnt@[seginx+j] <> '%' then begin
         j := j+1;
         if j = len then
           kk:=putstring(kk,attstack@[newp].prim@.textp,segmnt,seginx+lj,j-lj)
         end
      else if segmnt@[seginx+j+1] = '%' then begin  (* %% prints % *)
         kk:=putstring(kk,attstack@[newp].prim@.textp,segmnt,seginx+lj,j-lj+1);
         j := j+2; lj := j
         end
      else if i >= nexprs then begin (* not enough exprs *)
         markerror(864); j := len end
      else begin
         if j > lj then begin
           kk:=putstring(kk,attstack@[newp].prim@.textp,segmnt,seginx+lj,j-lj);
           lj := j end;
         k := len; j := j+1;
         if segmnt@[seginx+j] = '-' then j := j+1;
         (*P2CC  #ifdef NO_SNPRINTF *) (*P2CP
            attstack@[newp+1].state := j; *)
         (*P2CC #endif *)
         while j < k do if segmnt@[seginx+j] in ['e','f','g'] then k := j
            else if segmnt@[seginx+j] in ['0'..'9','.'] then j := j+1
            else j := k;

         if k = len then markerror(865)
         else begin
            j := j+1;
               (*D if debuglevel > 0 then begin write(log,'format="');
                 for kx := lj to j-1 do write(log,segmnt@[seginx+kx]);
                 write(log, '" nexprs=',nexprs:2,' Numerical print value=');
                 wfloat(log,attstack@[newp+4+2*i].xval); writeln(log);
                 flush(log) end; D*)
                                 (* Write the expression to tmpbuf *)
            (*P2CC  #ifdef NO_SNPRINTF *) (*P2CP
            k := attstack@[newp+1].state; ll := j-1; s := 0.0;
            while k < ll do if segmnt@[seginx+k]='.' then ll := k else begin
                s := 10.0*s + ord(segmnt@[seginx+k])-ord('0'); k := k+1 end;
            r := 0.0;
            for ll := k+1 to j-1 do r :=10.0*r+ord(segmnt@[seginx+ll])-ord('0');
            ll := 0;
            if (s > (CHBUFSIZ-2)) or (r > (CHBUFSIZ-2)) then begin
                markerror(865); j := len end
            else begin
               lastc := segmnt@[seginx+j]; segmnt@[seginx+j] := chr(0);
               ll := sprintf( tmpbuf@[1],
                 segmnt@[seginx+lj], attstack@[newp+4+2*i].xval );
               segmnt@[seginx+j] := lastc
               end; *)
            (*P2CC #else *)
               lastc := segmnt@[seginx+j]; segmnt@[seginx+j] := chr(0);
               ll := snprintf( tmpbuf@[1], (CHBUFSIZ-1),
                  segmnt@[seginx+lj], attstack@[newp+4+2*i].xval );
               segmnt@[seginx+j] := lastc;
            (*P2CC #endif *)
            if ll > (CHBUFSIZ-2) then begin markerror(865); ll:= CHBUFSIZ-2 end;
               (*D if debuglevel > 0 then begin
                  write(log,' tmpbuf(1:',ll:1,')=');
                  for kx := 1 to ll do write(log,tmpbuf@[kx]);
                  writeln(log); flush(log) end; D*)
                                 (* Copy tmpbuf to the string *)
            if ll > 0 then
               kk := putstring(kk,attstack@[newp].prim@.textp,tmpbuf,1,ll);
            i := i+1;
            lj := j
            end
         end;
         (*  --*) end;
      if nexprs > i then markerror(864);  (* Too many exprs *)
      if drawmode = xfig then with prim@ do if boxwidth = 0.0 then begin
         if boxheight = 0.0 then boxheight := 0.1*eb@.env@[XLscale];
         if textp<>nil then boxwidth := boxheight*textp@.len*0.75
         end
      else if (drawmode=PDF) and (textp<>nil) then
         boxwidth := boxheight*textp@.len*0.6;
      if envblock@.son = attstack@[newp+2].prim then envblock@.son := prim;
      deletestringbox(attstack@[newp+2].prim)
      end;

(*      exprlist = expression   *)
   exprlist1: state := 1 ;
(*               | expression "," exprlist   *)
   exprlist2: state := attstack@[newp+2].state+1 ;

(*      object = block  *)
(*             | object "height" expression  *)
(*             | object "width" expression  *)
(*             | object "radius" expression  *)
(*             | object "diameter" expression  *)
(*             | object "thickness" expression  *)
(*             | object "scaled" expression  *)
(*             | object "<directon>" optexp  *)
(*             | object "<linetype>" optexp  *)
(*             | object "chop" optexp  *)
(*             | object "filled" optexp  *)
(*             | object "<arrowhd>" optexp  *)
(*             | object "then"  *)
(*             | object "cw"  *)
(*             | object "ccw"  *)
(*             | object "same"  *)
(*             | object stringexpr  *)
(*             | object "by" position  *)
(*             | object "from" position  *)
(*             | object "to" position  *)
(*             | object "at" position  *)
(*             | object "<textpos>"  *)
(*             | object "<colrspec>" stringexpr  *)
(*             | objectwith "at" position  *)
(*             | objectwith "<corner>" "at" position  *)
(*             | objectwith pair "at" position  *)
(*             | "continue"  *)

   object1: ;
(*                          | object "height" expression    *)
   object2: if prim <> nil then with prim@ do case ptype of
      XLbox,XBLOCK: begin
         if ptype = XBLOCK then begin
            r := 0.5*(attstack@[newp+2].xval - blockheight);
            blockheight := attstack@[newp+2].xval end
         else begin
            r := 0.5*(attstack@[newp+2].xval - boxheight);
            boxheight := attstack@[newp+2].xval end;
         if not teststflag(state,XLat) then case direction of
            XLleft,XLright: ;
            XLup: shift(prim,0,r);
            XLdown: shift(prim,0,-r)
            end
         end;
      XLstring: begin
         boxheight := attstack@[newp+2].xval;
         if (drawmode=PDF) and (textp<>nil) then
            boxwidth := boxheight*textp@.len*0.6
         end;
      XLcircle: begin
         if not teststflag(state,XLat) then case direction of
            XLleft,XLright: ;
            XLup  : aat.ypos := aat.ypos - radius + 0.5*attstack@[newp+2].xval;
            XLdown: aat.ypos := aat.ypos + radius - 0.5*attstack@[newp+2].xval
            end;
         radius := attstack@[newp+2].xval * 0.5
         end;
      XLellipse: begin
         if not teststflag(state,XLat) then case direction of
            XLleft,XLright: ;
            XLup  : aat.ypos := aat.ypos+0.5*(-elheight+attstack@[newp+2].xval);
            XLdown: aat.ypos := aat.ypos+0.5*( elheight-attstack@[newp+2].xval)
            end;
         elheight := attstack@[newp+2].xval
         end;
      XLline,XLarrow,XLmove,XLarc,XLspline: begin
         prp := prim;
         while prp<>nil do begin
            prp@.height := attstack@[newp+2].xval;
            if isthen(prp) then prp := prp@.parent else prp := nil
            end
         end;
      XLabel,XLaTeX: markerror(858)
      end;
(*                          | object "width" expression    *)
   object3: if prim <> nil then with prim@ do case ptype of
      XLbox,XBLOCK: begin
         if ptype = XBLOCK then begin
            r := 0.5*(attstack@[newp+2].xval - blockwidth);
            blockwidth := attstack@[newp+2].xval end
         else begin
            r := 0.5*(attstack@[newp+2].xval - boxwidth);
            boxwidth := attstack@[newp+2].xval end;
         if not teststflag(state,XLat) then case direction of
            XLup,XLdown: ;
            XLright: shift(prim,r,0);
            XLleft: shift(prim,-r,0)
            end
         end;
      XLstring: boxwidth := attstack@[newp+2].xval;
      XLcircle: begin
         if not teststflag(state,XLat) then case direction of
            XLup,XLdown: ;
            XLright: aat.xpos := aat.xpos - radius + 0.5*attstack@[newp+2].xval;
            XLleft : aat.xpos := aat.xpos + radius - 0.5*attstack@[newp+2].xval
            end;
         radius := attstack@[newp+2].xval * 0.5
         end;
      XLellipse: begin
         if not teststflag(state,XLat) then case direction of
            XLup,XLdown: ;
            XLright: aat.xpos := aat.xpos+0.5*(-elwidth+attstack@[newp+2].xval);
            XLleft : aat.xpos := aat.xpos+0.5*( elwidth-attstack@[newp+2].xval)
            end;
         elwidth := attstack@[newp+2].xval
         end;
      XLline,XLarrow,XLmove,XLarc,XLspline: begin
         prp := prim;
         while prp<>nil do begin
            prp@.width := attstack@[newp+2].xval;
            if isthen(prp) then prp := prp@.parent else prp := nil
            end
         end;
      XLabel,XLaTeX: markerror(858)
      end;
(*                          | object "radius" expression    *)
   object4: if prim <> nil then with prim@ do case ptype of
      XLbox: boxradius := attstack@[newp+2].xval;
      XLarc: begin
            t := attstack@[newp+2].xval;
            if direction = 0 then begin
               attstack@[newp+2].xval := aat.xpos +
                  aradius * cos(|startangle| + |arcangle|);
               attstack@[newp+2].yval := aat.ypos +
                  aradius * sin(|startangle| + |arcangle|);
               (*D if debuglevel = 2 then begin write(log, 'to ');
                 with attstack@[newp+2] do wpair(log,xval,yval) end; D*)
               doprod(object20)
               end;
            r := cos(|startangle|); s := sin(|startangle|);
            aat.xpos := aat.xpos + aradius * r;
            aat.ypos := aat.ypos + aradius * s;
            aradius := t;
            aat.xpos := aat.xpos - aradius * r;
            aat.ypos := aat.ypos - aradius * s;
            setstflag(state,XLradius)
            end;
      XLcircle: begin
         if not teststflag(state,XLat) then case direction of
            XLleft: aat.xpos := aat.xpos-(attstack@[newp+2].xval - radius);
            XLright:aat.xpos := aat.xpos+(attstack@[newp+2].xval - radius);
            XLup:   aat.ypos := aat.ypos+(attstack@[newp+2].xval - radius);
            XLdown: aat.ypos := aat.ypos-(attstack@[newp+2].xval - radius)
            end;
         radius := attstack@[newp+2].xval;
         setstflag(state,XLradius)
         end;
      otherwise markerror(858)
      end;
(*                          | object "diameter" expression    *)
   object5: if prim = nil then begin end
      else with prim@ do if ptype = XLcircle then begin
         r := 0.5*attstack@[newp+2].xval;
         if not teststflag(state,XLat) then case direction of
            XLleft: aat.xpos := aat.xpos-(r - radius);
            XLright:aat.xpos := aat.xpos+(r - radius);
            XLup:   aat.ypos := aat.ypos+(r - radius);
            XLdown: aat.ypos := aat.ypos-(r - radius);
            otherwise
            end;
         radius := r
         end
      else markerror(858);
(*                          | object "thickness" expression    *)
   object6: if prim <> nil then with prim@ do
      if attstack@[newp+2].xval < 0.0 then begin
         eb := findenv(envblock);
         lthick := eb@.env@[XLlinethick]
         end
      else lthick := attstack@[newp+2].xval;
(*                          | object "scaled" expression    *)
   object7: if (prim <> nil) and (attstack@[newp+2].lexval <> XEMPTY) then
      with prim@ do begin
         r := attstack@[newp+2].xval-1;
         corner(prim,XDc,x1,y1);
         case ptype of
         XLbox,XBLOCK,XLstring: begin
            if teststflag(state,XLat) then begin dx := 0; dy := 0 end
            else if ptype = XBLOCK then begin
               dx := blockwidth*r/2; dy := blockheight*r/2
               end
            else begin
               dx := boxwidth*r/2; dy := boxheight*r/2
               end;
   		    scaleobj(prim,attstack@[newp+2].xval);
            case direction of
               XLright: shift(prim,x1-aat.xpos+dx,y1-aat.ypos);
               XLleft: shift(prim,x1-aat.xpos-dx,y1-aat.ypos);
               XLup: shift(prim,x1-aat.xpos,y1-aat.ypos+dy);
               XLdown: shift(prim,x1-aat.xpos,y1-aat.ypos-dy)
               end
            end;
         XLcircle: begin
            radius := attstack@[newp+2].xval * radius;
            if not teststflag(state,XLat) then case direction of
               XLup: begin aat.xpos := x1; aat.ypos := y1 + radius end;
               XLdown: begin aat.xpos := x1; aat.ypos := y1 - radius end;
               XLright: begin aat.xpos := x1 + radius; aat.ypos := y1 end;
               XLleft: begin aat.xpos := x1 - radius; aat.ypos := y1 end
               end
            end;
         XLellipse: begin
            elwidth := elwidth*attstack@[newp+2].xval;
            elheight := elheight*attstack@[newp+2].xval;
            if not teststflag(state,XLat) then case direction of
               XLup: begin aat.xpos := x1; aat.ypos := y1 + elheight/2 end;
               XLdown: begin aat.xpos := x1; aat.ypos := y1 - elheight/2 end;
               XLright: begin aat.xpos := x1 + elwidth/2; aat.ypos := y1 end;
               XLleft: begin aat.xpos := x1 - elwidth/2; aat.ypos := y1 end
               end;
            end;
         XLline,XLarrow,XLmove,XLarc,XLspline: begin
   		    scaleobj(prim,attstack@[newp+2].xval);
            corner(prim,XDc,r,s);
            shift(prim,x1-r,y1-s)
            end;
         XLabel,XLaTeX: markerror(858);
         otherwise
         end
      end;
(*                          | object "<directon>" optexp *)
   object8: if prim <> nil then with prim@ do if ptype in
      [XLarc,XLline,XLmove,XLarrow,XLspline] then begin
      i := attstack@[newp+1].lexval;
      envblock@.direction := i;
      eb := findenv(envblock);
      case ptype of
         XLarc: begin
            r := cos(|startangle|); s := sin(|startangle|);
            aat.xpos := aat.xpos + aradius * r;
            aat.ypos := aat.ypos + aradius * s;
            if ((direction = XLup   ) and (i = XLleft )) or
               ((direction = XLdown ) and (i = XLright)) or
               ((direction = XLright) and (i = XLup   )) or
               ((direction = XLleft ) and (i = XLdown )) then
                  |arcangle| := pi * 0.5
            else if ((direction = XLup   ) and (i = XLright)) or
               ((direction = XLdown ) and (i = XLleft )) or
               ((direction = XLright) and (i = XLdown )) or
               ((direction = XLleft ) and (i = XLup   )) then
                  |arcangle| := - pi * 0.5;
            if attstack@[newp+2].lexval <> XEMPTY then
               aradius := attstack@[newp+2].lexval;
            aat.xpos := aat.xpos - aradius * r;
            aat.ypos := aat.ypos - aradius * s;
            direction := i
            end;
         XLline,XLmove,XLarrow,XLspline: begin
            direction := i;
            if attstack@[newp+2].lexval <> XEMPTY then begin
               r := attstack@[newp+2].xval; s := r end
            else case ptype of
               XLline,XLarrow,XLspline: begin
                  r := eb@.env@[XLlineht];
                  s := eb@.env@[XLlinewid] end;
               XLmove: begin
                  r := eb@.env@[XLmoveht];
                  s := eb@.env@[XLmovewid] end
               end;
            lineardir(prim,r,s,state)
            end
         end
      end
   else markerror(858);
(*                          | object "<linetype>" optexp    *)
   object9: if prim <> nil then begin
      setspec(prim@.spec,attstack@[newp+1].lexval);
      if attstack@[newp+2].lexval = XEMPTY then begin end
      else if (attstack@[newp+1].lexval in [XLinvis,XLsolid]) and
         (prim@.ptype in [XLline,XLarrow,XLspline,XLmove]) then
         lineardir(prim,attstack@[newp+2].xval,attstack@[newp+2].xval,state)
      else if attstack@[newp+1].lexval in [XLinvis,XLsolid]
         then markerror(858)
      else prim@.lparam := attstack@[newp+2].xval
      end;
(*                          | object "chop" optexp    *)
   object10: if prim = nil then begin end
      else if not (prim@.ptype in [XLline,XLarrow,XLmove,XLspline]) then
         markerror(858)
      else with prim@ do begin
         if attstack@[newp+2].lexval <> XEMPTY then
            r := attstack@[newp+2].xval
         else begin eb := findenv(envblock); r := eb@.env@[XLcirclerad] end;
         if teststflag(state,XLchop) then endchop := r
         else begin startchop := r; endchop := r; setstflag(state,XLchop) end
         end;
(*                          | object "fill" optexp    *)
   object11: if prim <> nil then with prim@ do begin
      if attstack@[newp+2].lexval <> XEMPTY then s := attstack@[newp+2].xval
      else begin eb := findenv(envblock); s := eb@.env@[XLfillval] end;
      prp := prim;
      while prp<>nil do begin
         case ptype of
            XLbox: prp@.boxfill := s;
            XLcircle: prp@.cfill := s;
            XLellipse: prp@.efill := s;
            otherwise if drawmode in [TeX,tTeX,Pict2e] then markerror(858)
            else case ptype of
               XLline,XLarrow,XLmove,XLspline,XLarc: prp@.lfill := s;
               otherwise markerror(858)
               end
            end;
         if isthen(prp) then prp := prp@.parent else prp := nil 
         end
      end;
(*                          | object "<arrowhd>" optexp    *)
   object12: if prim <> nil then with prim@ do
      if not (ptype in [XLline,XLarrow,XLarc,XLspline]) then markerror(858)
      else begin
         atype := pahlex(atype,attstack@[newp+1].lexval);
         if attstack@[newp+2].lexval <> XEMPTY then
            lj := round(attstack@[newp+2].xval)
         else begin
            eb := findenv(envblock);
            lj := round(eb@.env@[XLarrowhead])
            end;
         atype := pahnum(atype,lj)
         end;
(*                          | object "then"    *)
   object13: if prim <> nil then begin
      (*D if debuglevel > 0 then begin write(log,'lexical "then" found, ');
          prtstval(state); writeln(log) end; D*)
      appendthen(prim);
      if prim@.ptype <> XLarc then setstflag(state,XEMPTY);
      end;
(*                          | object "cw"    *)
   object14: if prim <> nil then with prim@ do if ptype <> XLarc then
         markerror(858)
      else begin
         (*D if debuglevel = 2 then begin
             write(log,'direction=',direction:1,' |arcangle','|=');
             wfloat(log,|arcangle| *180/pi); writeln(log);
             write(log,'principal(|arcangle','|,pi)=');
             wfloat(log,(principal(|arcangle|,pi))*180/pi); writeln(log);
             write(log,'2.0 * pi - principal(|arcangle','|,pi)=');
             wfloat(log,(2.0 * pi - principal(|arcangle|,pi))*180/pi);
             writeln(log) end; D*)
      (* if teststflag(state,XLat) then begin
         if |arcangle|>0.0 then |arcangle| := |arcangle|-2*pi
         end
      else *) if (|arcangle|>0.0) and (direction=0) then
         |arcangle| := -abs(principal(2.0*pi-|arcangle|,2.0*pi))
      else if direction <> 0 then begin
         aat := arcstart(prim);
         case direction of
            XLup: aat.xpos := aat.xpos + aradius;
            XLdown: aat.xpos := aat.xpos - aradius;
            XLleft: aat.ypos := aat.ypos + aradius;
            XLright: aat.ypos := aat.ypos - aradius
            end;
         if |arcangle| > 0.0 then |startangle| := principal(|startangle|+pi,pi);
         |arcangle| := -abs(|arcangle|)
         end;
      setstflag(state,XLcw)
      end;
(*                          | object "ccw"    *)
   object15: if prim <> nil then with prim@ do if ptype <> XLarc then
         markerror(858)
      else begin
      (* if teststflag(state,XLat) then begin
         if |arcangle|<0.0 then |arcangle| := 2*pi+|arcangle|
         end
      else *) if (|arcangle|<0.0) and (direction=0) then
         |arcangle| := abs(principal(|arcangle|-2.0*pi,2.0*pi))
      else if direction <> 0 then begin
         aat := arcstart(prim);
         case direction of
            XLup: aat.xpos := aat.xpos - aradius;
            XLdown: aat.xpos := aat.xpos + aradius;
            XLleft: aat.ypos := aat.ypos - aradius;
            XLright: aat.ypos := aat.ypos + aradius
            end;
         if |arcangle| < 0.0 then |startangle| := principal(|startangle|+pi,pi);
         |arcangle| := abs(|arcangle|)
         end;
      setstflag(state,XLccw)
      end;
(*                          | object "same"    *)
   object16: if prim <> nil then begin
      prp := nthprimobj(envblock@.son,0,prim@.ptype);
      if prp = nil then markerror(857)
      else with prim@ do begin
         lparam := prp@.lparam;
         lthick := prp@.lthick;
         direction := prp@.direction;
         spec := prp@.spec;
         if hasoutline(lexval,false) then
            copystr(outlinep,prp@.outlinep);
         if hasshade(lexval,false) then copystr(shadedp,prp@.shadedp)
         end;
      if prp <> nil then with prim@ do case ptype of
         XLbox,XLstring: begin
            if ptype = XLbox then case direction of
               XLup:aat.ypos := aat.ypos+0.5*(prp@.boxheight - boxheight);
               XLdown: aat.ypos := aat.ypos-0.5*(prp@.boxheight - boxheight);
               XLleft: aat.xpos := aat.xpos-0.5*(prp@.boxwidth - boxwidth);
               XLright: aat.xpos := aat.xpos+0.5*(prp@.boxwidth - boxwidth)
               end;
            boxfill := prp@.boxfill;
            boxheight := prp@.boxheight;
            boxwidth := prp@.boxwidth;
            boxradius := prp@.boxradius
            end;
         XBLOCK: markerror(858);
         XLcircle: begin
            case direction of
               XLup:aat.ypos := aat.ypos+(prp@.radius - radius);
               XLdown: aat.ypos := aat.ypos-(prp@.radius - radius);
               XLleft: aat.xpos := aat.xpos-(prp@.radius - radius);
               XLright: aat.xpos := aat.xpos+(prp@.radius - radius)
               end;
            cfill := prp@.cfill;
            radius := prp@.radius
            end;
         XLellipse: begin
            case direction of
               XLup:aat.ypos := aat.ypos+0.5*(prp@.elheight - elheight);
               XLdown: aat.ypos := aat.ypos-0.5*(prp@.elheight - elheight);
               XLleft: aat.xpos := aat.xpos-0.5*(prp@.elwidth - elwidth);
               XLright: aat.xpos := aat.xpos+0.5*(prp@.elwidth - elwidth)
               end;
            efill := prp@.efill;
            elheight :=prp@.elheight; elwidth :=prp@.elwidth
            end;
         XLarc: begin
            x1 := aat.xpos + aradius*cos(|startangle|);
            y1 := aat.ypos + aradius*sin(|startangle|);
            aradius := prp@.aradius;
            |startangle| := prp@.|startangle|;
            aat.xpos := x1 - aradius*cos(|startangle|);
            aat.ypos := y1 - aradius*sin(|startangle|);
            |arcangle| := prp@.|arcangle|
            end;
         XLline,XLarrow,XLmove,XLspline: begin
            endpos.xpos := aat.xpos + prp@.endpos.xpos - prp@.aat.xpos;
            endpos.ypos := aat.ypos + prp@.endpos.ypos - prp@.aat.ypos;
            height := prp@.height; width := prp@.width;
            atype := prp@.atype
            end;
         XLabel,XLaTeX: markerror(858)
         end
      end;
(*                            | object stringexpr *)
   object17: if attstack@[newp+1].prim <> nil then begin
      if prim<>nil then with prim@ do begin
         if textp = nil then
            textp := attstack@[newp+1].prim@.textp
         else begin
            namptr := textp; i := 1;
            while namptr@.next <> nil do begin
               namptr := namptr@.next;
               i := i + 1
               end;
            namptr@.next := attstack@[newp+1].prim@.textp;
            if ptype = XLstring then boxheight := boxheight*(i+1)/i
            (* boxheight/((i-1)*TEXTRATIO+1)*(i*TEXTRATIO+1) *)
            end;
         if drawmode in [PS,PDF,PSfrag] then
            (* output contains text *)
            printstate := (printstate div 2)*2 + 1
         end;
      attstack@[newp+1].prim@.textp := nil;
      deletestringbox( attstack@[newp+1].prim )
      end;

(*                          | object "by" position    *)
   object18: if prim = nil then begin end
      else if prim@.ptype in [XLarc,XLline,XLarrow,XLspline,XLmove] then
      with prim@ do begin
      x1 := attstack@[newp+2].xval + aat.xpos;
      y1 := attstack@[newp+2].yval + aat.ypos;
      if ptype = XLarc then begin
         x1 := x1 + aradius*cos(|startangle|);
         y1 := y1 + aradius*sin(|startangle|)
         end;
      attstack@[newp+2].xval := x1;
      attstack@[newp+2].yval := y1;
      doprod(object20)
      end
   else markerror(858);
(*                            | object "from" position       *)
   object19: if prim = nil then begin end
      else with prim@ do if ptype in [XLarc,XLline,XLarrow,XLspline,XLmove]
      then begin
         if ptype = XLarc then begin
            r := attstack@[newp+2].xval; s := attstack@[newp+2].yval;
            if teststflag(state,XLto) then begin
               attstack@[newp+2].xval := aat.xpos +
                  aradius * cos(|startangle|+|arcangle|);
               attstack@[newp+2].yval := aat.ypos +
                  aradius * sin(|startangle|+|arcangle|);
               doprod(object20)
               end;
            aat.xpos := r - aradius * cos(|startangle|);
            aat.ypos := s - aradius * sin(|startangle|)
            end
         else if not teststflag(state,XLto) then begin
            prp := prim; while isthen(prp) do prp := prp@.parent;
            shift(prp,attstack@[newp+2].xval-prp@.aat.xpos,
                      attstack@[newp+2].yval-prp@.aat.ypos)
            end
         else begin
            aat.xpos := attstack@[newp+2].xval;
            aat.ypos := attstack@[newp+2].yval
            end;
         setstflag(state,XLfrom)
         end
      else markerror(858);
(*                            | object "to" position         *)
   object20: if prim = nil then begin end
      else if prim@.ptype in [XLarc,XLline,XLarrow,XLspline,XLmove] then begin
      if (prim@.ptype <> XLarc) and teststflag(state,XLto)
         (* (teststflag(state,XLto) or teststflag(state,XLdirecton)) *)
         then begin
         (*D if debuglevel > 0 then begin writeln(log,'"then" inserted, ');
             prtstval(state); writeln(log) end; D*)
         appendthen(prim);
         setstflag(state,XEMPTY)
         end;
      with prim@ do begin
         if ptype <> XLarc then begin
            endpos.xpos := attstack@[newp+2].xval;
            endpos.ypos := attstack@[newp+2].yval
            end
         else begin
            x1 := aat.xpos + aradius*cos(|startangle|); (* from position *)
            y1 := aat.ypos + aradius*sin(|startangle|);
            dx := attstack@[newp+2].xval - x1;
            dy := attstack@[newp+2].yval - y1;
            ts := dx*dx + dy*dy;                        (* chord^2 *)
            if direction <> 0 then i := direction else i := length;
            (*D if debuglevel = 2 then begin
               write(log,' (x1,y1)='); wpair(log,x1,y1);
               write(log,' (dx,dy)='); wpair(log,dx,dy);
               write(log,' ts='); wfloat(log,ts);
               write(log,' i=',i:1 ) end; D*)
            if ts = 0.0 then |arcangle| := 0.0
            else begin                   (* ratio centre-to-chord/half-chord *)
               t := sqrt(Max(0.0,4.0*aradius*aradius - ts)/ts);
               (*D if debuglevel = 2 then begin
                  write(log,' t='); wfloat(log,t);
                  write(log,' |arcangle','|='); wfloat(log,|arcangle| *180/pi);
                  writeln(log) end; D*)
               r := sqrt(ts);
               if t <= 0.0 then aradius := 0.5*r; (* t is always nonnegative  *)
               case i of (* Determine which of the two default arcs to draw: *)
                  XLup:    if |arcangle| *(-dx-t*dy) < 0.0 then t := -t;
                  XLdown:  if |arcangle| *(-dx-t*dy) > 0.0 then t := -t;
                  XLright: if |arcangle| *(dy-t*dx)  < 0.0 then t := -t;
                  XLleft:  if |arcangle| *(dy-t*dx)  > 0.0 then t := -t
                  end;
               aat.xpos := x1 + 0.5*(dx+t*dy);
               aat.ypos := y1 + 0.5*(dy-t*dx);
               (*D if debuglevel = 2 then begin
                  write(log,' t='); wfloat(log,t);
                  write(log,' aradius='); wfloat(log,aradius);
                  write(log,' aat='); wpair(log,aat.xpos,aat.ypos);
                  writeln(log) end; D*)
               setangles(|startangle|,|arcangle|,aat,x1,y1,
                  attstack@[newp+2].xval,attstack@[newp+2].yval)
               end;
            if direction <> 0 then length := direction;
            direction := 0
            end;
         setstflag(state,XLto)
         (*D ; if debuglevel > 0 then printobject(prim) D*)
         end
      end
      else markerror(858);
(*                          | object "at" position    *)
   object21: if prim <> nil then with prim@ do begin
      xval := attstack@[newp+2].xval;
      yval := attstack@[newp+2].yval;
      setstval(state,XDc);
      setstflag(state,XLat)
      end;
(*                          | object "<textpos>"    *)
(* This might be altered (in the case of strings, not other objects with text),
     so that aat is changed as a function of textpos.  Then
     the output routines have to take that into account.   The alternative
     is to alter nesw for strings, as now. *)
   object22: if prim <> nil then begin
      namptr := prim@.textp;
      if namptr <> nil then begin
         while namptr@.next <> nil do namptr := namptr@.next;
         i := round(namptr@.val); if i > 8 then i := 0;
         case attstack@[newp+1].lexval of
            XLcenter: namptr@.val := 15;
            XLrjust: namptr@.val := (i/4)*4+1;
            XLljust: namptr@.val := (i/4)*4+2;
            XLbelow: namptr@.val := (i mod 4)+4;
            XLabove: namptr@.val := (i mod 4)+8
            end
         end
      else markerror(861)
      end;
(*                          | object ("colour"|"outline"|"shade") stringexpr *)
   object23: begin
      if drawmode in [Pict2e,TeX,tTeX,xfig] then markerror(858)
      else if (attstack@[newp+2].prim <> nil) and (prim<>nil) then
         with prim@ do case attstack@[newp+1].lexval of
         XLshaded: if hasshade(lexval,true) then begin
            deletename( shadedp );
            shadedp := attstack@[newp+2].prim@.textp;
            attstack@[newp+2].prim@.textp := nil
            end;
         XLoutlined: if hasoutline(lexval,true) then begin
            deletename( outlinep );
            outlinep := attstack@[newp+2].prim@.textp;
            attstack@[newp+2].prim@.textp := nil
            end;
         XLcolour: if hasoutline(lexval,true) then begin
            deletename( outlinep );
            outlinep := attstack@[newp+2].prim@.textp;
            attstack@[newp+2].prim@.textp := nil;
            if (not (ptype in [XLarc,XLline,XLarrow,XLspline])) and
               hasshade(lexval,false) then begin
               deletename( shadedp ); copystr(shadedp,outlinep)
               end
            end;
         otherwise
         end;
      deletestringbox( attstack@[newp+2].prim )
      end;

(*                          | objectwith "at" position  *)
   object24: if prim <> nil then begin
      xval := attstack@[newp+2].xval;
      yval := attstack@[newp+2].yval;
      setstval(state,XEMPTY);
      setstflag(state,XLat)
      end;
(*                          | objectwith "<corner>" "at" position    *)
(*                          | objectwith pair "at" position    *)
   object25,object26: if prim <> nil then begin
      xval := attstack@[newp+3].xval;
      yval := attstack@[newp+3].yval;
      if p = object25 then setstval(state,attstack@[newp+1].lexval)
      else if not (prim@.ptype in
         [XLbox,XLstring,XBLOCK,XLcircle,XLellipse,XLarc]) then markerror(858)
      else begin
         startchop := attstack@[newp+1].xval;
         endchop := attstack@[newp+1].yval;
         setstval(state,XLfloat)
         end;
      setstflag(state,XLat)
      end;
(*                         | "continue" *)
   object27: begin
      primp := nil; prp := envblock@.son;
      while prp <> nil do begin
         if not (prp@.ptype in [XLabel,XBLOCK,XLstring,XLaTeX]) then
            primp := prp;
         prp := prp@.next
         end;
      if primp = nil then markerror(857)
      else begin
         while primp@.son <> nil do primp := primp@.son;
         prim := primp;
         doprod(object13)
         end
      end;

(*     openblock = "<EMPTY>"   *)
   openblock1: begin
      (*D if debuglevel > 0 then writeln(log,
         'Calling newprim(prim,',XBLOCK:1,'):'); D*)
      newprim(prim,XBLOCK,envblock);
      (* prim@.here := envblock@.here; *)(* Bah! gpic compatibility: *)
      (* with prim@.here do begin xpos := 0.0; ypos := 0.0 end; *)
      prim@.here.xpos := 0.0; prim@.here.ypos := 0.0;
      envblock := prim;
      lexval := 0
      (* ; resetenv(0,envblock) *)
      (*D; if (debuglevel > 0) and (prim <> nil) then printobject(prim)D*)
      end;

(*     block = "<primitiv>" optexp   *)
(*           | stringexpr   *)
(*           | openblock "[" closeblock "]"   *)
(*           | openblock "[]"   *)
                                    (* set parameters for primitive object *)
   block1: if (lexval > XLprimitiv) and (lexval < XLenvvar) then begin
      newprim(prim,lexval,envblock);
      eb := findenv(envblock);
      if ((drawmode in [MPost,Pict2e,PDF,PS,SVG,PSfrag])
         and (lexval <> XLmove))
         or ((* (drawmode in [PGF,PSTricks]) and *) (lexval=XLarc))
         then begin
         prim@.lthick := eb@.env@[XLlinethick]
         (*D; if debuglevel > 0 then begin write(log,
             ' envblock[',ordp(eb),'] setting linethick=');
             wfloat(log,prim@.lthick); writeln(log) end D*)
         end;
      if (attstack@[newp+1].lexval <> XEMPTY) and
         not (lexval in [XLline,XLarrow,XLspline,XLmove]) then markerror(858);
      with prim@ do case lexval of
         XLbox: begin
            boxheight := eb@.env@[XLboxht];
            boxwidth := eb@.env@[XLboxwid];
            boxradius := eb@.env@[XLboxrad];
            case direction of
               XLup: aat.ypos := aat.ypos + boxheight * 0.5;
               XLdown: aat.ypos := aat.ypos - boxheight * 0.5;
               XLleft: aat.xpos := aat.xpos - boxwidth * 0.5;
               XLright: aat.xpos := aat.xpos + boxwidth * 0.5
               end
            end;
         XLcircle: begin
            radius := eb@.env@[XLcirclerad];
            case direction of
               XLup: aat.ypos := aat.ypos + radius;
               XLdown: aat.ypos := aat.ypos - radius;
               XLleft: aat.xpos := aat.xpos - radius;
               XLright: aat.xpos := aat.xpos + radius
               end
            end;
         XLellipse: begin
            elheight := eb@.env@[XLellipseht];
            elwidth := eb@.env@[XLellipsewid];
            case direction of
               XLup: aat.ypos := aat.ypos + elheight * 0.5;
               XLdown: aat.ypos := aat.ypos - elheight * 0.5;
               XLleft: aat.xpos := aat.xpos - elwidth * 0.5;
               XLright: aat.xpos := aat.xpos + elwidth * 0.5
               end
            end;
         XLarc: begin
            aradius := eb@.env@[XLarcrad];
            case direction of
               XLup: begin
                  |startangle| := 0.0;
                  aat.xpos := aat.xpos - aradius
                  end;
               XLdown: begin
                  |startangle| := pi;
                  aat.xpos := aat.xpos + aradius
                  end;
               XLleft: begin
                  |startangle| := 0.5* pi;
                  aat.ypos := aat.ypos - aradius
                  end;
               XLright: begin
                  |startangle| := -0.5 * pi;
                  aat.ypos := aat.ypos + aradius
                  end
               end;
            height := eb@.env@[XLarrowht];
            width := eb@.env@[XLarrowwid]; (* atype := XEMPTY; *)
            atype := pahnum(pahlex(0,XEMPTY),round(eb@.env@[XLarrowhead]));
            |arcangle| := pi*0.5
            end;
         XLline,XLarrow,XLspline,XLmove: begin
            endpos := aat;
            if (ptype = XLspline) and (attstack@[newp+1].lexval <> XEMPTY) then
               (* This implements adjusted splines *) begin
               aradius := attstack@[newp+1].xval;
               attstack@[newp+1].lexval := XEMPTY end; (* *)
            if attstack@[newp+1].lexval <> XEMPTY then
               r := attstack@[newp+1].xval
            else if ptype = XLmove then case direction of
               XLup,XLdown: r := eb@.env@[XLmoveht];
               XLleft,XLright: r := eb@.env@[XLmovewid]
               end
            else case direction of
               XLup,XLdown: r := eb@.env@[XLlineht];
               XLleft,XLright: r := eb@.env@[XLlinewid]
               end;
            case direction of
               XLup: endpos.ypos := aat.ypos + r;
               XLdown: endpos.ypos := aat.ypos - r;
               XLleft: endpos.xpos := aat.xpos - r;
               XLright: endpos.xpos := aat.xpos + r
               end;
            height := eb@.env@[XLarrowht];
            width := eb@.env@[XLarrowwid];
            if ptype = XLarrow then atype := pahlex(0,XRIGHTHEAD)
            else atype := pahlex(0,XEMPTY);
            atype := pahnum(atype,round(eb@.env@[XLarrowhead]))
            end
         end
      end;
(*                          | stringexpr *)
   block2: if drawmode in [PS,PDF,PSfrag] then
      (* flag text in output *)
      printstate := (printstate div 2)*2 + 1;
(*                          | openblock "[" closeblock "]" *)
   block3: if prim <> nil then begin
      prim@.son := attstack@[newp+2].prim;
      envblock := prim@.parent;
      getnesw( prim@.son );
      with prim@ do begin
         blockwidth := east-west;
         blockheight := north-south;
         aat.xpos := (east+west)*0.5;
         aat.ypos := (north+south)*0.5;
         dx := envblock@.here.xpos - aat.xpos;
         dy := envblock@.here.ypos - aat.ypos;
         case envblock@.direction of
            XLright: dx := dx + blockwidth * 0.5;
            XLleft:  dx := dx - blockwidth * 0.5;
            XLup:   dy := dy + blockheight * 0.5;
            XLdown: dy := dy - blockheight * 0.5
            end;
         direction := envblock@.direction   (* gpic compatibility *)
         end;
      (*D if (debuglevel > 0) and (prim@.son <> nil) then with prim@ do begin
             printobject(son);
             if son@.next <> nil then printobject(son@.next)
             end; D*)
      shift(prim,dx,dy)
      (*D;if debuglevel > 0 then with prim@ do begin
         write(log, 'Object3: (north,south),(west,east)');
         wpair(log,aat.ypos+blockheight*0.5,aat.ypos-blockheight*0.5);
         wpair(log,aat.xpos-blockwidth*0.5,aat.xpos+blockwidth*0.5);
         write(log, ' here=');
         wpair(log,here.xpos,here.ypos); writeln(log);
         if prim@.son<>nil then begin
            printobject(prim@.son);
            if prim@.son@.next <> nil then printobject(prim@.son@.next)
            end
         end;
      if debuglevel > 0 then begin snaptree(prim,0); writeln(log) end D*)
      end;
(*                          | openblock "[]" *)
   block4: if prim <> nil then envblock := prim@.parent;

(*     optexp = "<EMPTY>"   *)
(*            | expression   *)
   optexp1: lexval := XEMPTY;
   optexp2: ;

(*     closeblock = elementlist optnl   *)
(*    Add latex to reset env variables changed within a block *)
closeblock1: if (prim <> nil) and (envblock@.env <> nil) then begin
      eb := findenv(envblock@.parent);
      if eb <> nil then begin (* check and reset variables: *)
         if envblock@.env@[XLlinethick] <> eb@.env@[XLlinethick] then begin
            newprim( attstack@[newp+2].prim,XLaTeX,envblock );
            attstack@[newp+2].prim@.lthick := eb@.env@[XLlinethick];
            addelem(attstack@[newp+state].prim,attstack@[newp+2].prim)
            end;
         if envblock@.env@[XLdashwid] <> eb@.env@[XLdashwid] then begin
            newprim( attstack@[newp+2].prim,XLaTeX,envblock );
            attstack@[newp+2].prim@.lparam := eb@.env@[XLdashwid];
            addelem(attstack@[newp+state].prim,attstack@[newp+2].prim)
            end
         end
      end;

(*     pair = expression "," expression   *)
(*          | location shift   *)
   pair1: yval := attstack@[newp+2].xval;
   pair2: if attstack@[newp+1].lexval <> XEMPTY then begin
      xval := xval + attstack@[newp+1].xval;
      yval := yval + attstack@[newp+1].yval
      end;

(*     objectwith = object "with"  *)
(*                | objectwith "." "<Label>" suffix  *)
   objectwith1: ;
   objectwith2: if prim <> nil then begin
      if attstack@[newp+3].lexval <> XEMPTY then with attstack@[newp+2] do
         addsuffix(chbuf,chbufx,length,attstack@[newp+3].xval);
      if internal = nil then prp := prim else prp := internal;
      internal := findplace(prp@.son,chbuf,
         attstack@[newp+2].chbufx,attstack@[newp+2].length);
      if internal = nil then begin marknotfound(855,
            chbuf,attstack@[newp+2].chbufx,attstack@[newp+2].length);
         deletetree( prim )
         end
      end;
(*                | objectwith "." nth primobj  *)
   objectwith3: if prim <> nil then begin
      if internal = nil then prp := prim else prp := internal;
      internal := nthprimobj(prp@.son,
         attstack@[newp+2].length,attstack@[newp+3].lexval);
      if internal = nil then begin
         markerror(857);
         deletetree( prim )
         end
      (* else lexval := attstack@[newp+1].lexval *)
      end;
(*     shift = "<EMPTY>"   *)
(*           | shift "+" location   *)
(*           | shift "-" location   *)
   shift1: begin
      xval := 0.0; yval := 0.0;
      lexval := XEMPTY
      end;
   shift2: begin
      xval := xval + attstack@[newp+2].xval;
      yval := yval + attstack@[newp+2].yval;
      lexval := XLfloat
      end;
   shift3: begin
      xval := xval - attstack@[newp+2].xval;
      yval := yval - attstack@[newp+2].yval;
      lexval := XLfloat
      end;

(*     location = "(" position ")"   *)
(*              | "(" position "," position ")"   *)
(*              | place   *)
(*              | location "*" factor   *)
(*              | location "/" factor   *)
   location1: attstack@[newp] := attstack@[newp+1] ;
   location2: begin
      xval := attstack@[newp+1].xval;
      yval := attstack@[newp+3].yval
      end;
   location3: ;
   location4: begin
      xval := xval * attstack@[newp+2].xval;
      yval := yval * attstack@[newp+2].xval
      end;
   location5: if attstack@[newp+2].xval = 0.0 then markerror(852) else begin
      xval := xval / attstack@[newp+2].xval;
      yval := yval / attstack@[newp+2].xval
      end;

(*     place = placename   *)
(*           | placename "<corner>"   *)
(*           | "<corner>" placename   *)
(*           | "<corner>" "of" placename   *)
(*           | "Here"   *)
   place1: corner(prim,XEMPTY,xval,yval);
   place2: corner(prim,attstack@[newp+1].lexval,xval,yval);
   place3: corner(attstack@[newp+1].prim,lexval,xval,yval);
   place4: corner(attstack@[newp+2].prim,lexval,xval,yval);
   place5: begin
      xval := envblock@.here.xpos;
      yval := envblock@.here.ypos
      end;

(*     factor = primary   *)
(*            | "!" primary   *)
(*            | primary "^" factor   *)
   factor1: ;
   factor2: if attstack@[newp+1].xval = 0.0 then xval := 1.0 else xval := 0.0;
   factor3: if (xval = 0.0) and (attstack@[newp+2].xval < 0.0) then
         markerror(852)
      else begin
        j := round(attstack@[newp+2].xval); r := j;
        if r=attstack@[newp+2].xval then xval := intpow(xval,j)
        else if xval < 0.0 then begin markerror(863); xval := 0.0 end
        else if xval = 0.0 then begin end
        else xval := exp( attstack@[newp+2].xval * ln(xval) )
        end;

(*     placename = "<Label>" suffix   *)
   placename1: begin
      if attstack@[newp+1].lexval <> XEMPTY then
         addsuffix(chbuf,chbufx,length,attstack@[newp+1].xval);
      prp := nil; primp := envblock;
      while primp <> prp do begin
         prp := findplace(primp@.son,chbuf,chbufx,length);
         if prp <> nil then primp := prp
         else primp := primp@.parent
         end;
      if prp = nil then marknotfound(854,chbuf,chbufx,length)
      (*D else if debuglevel > 0 then with prp@ do begin
          write(log,'placename: type ', ptype:1,', ');
          wpair(log,aat.xpos,aat.ypos); writeln(log) end D*);
      prim := prp
      end;
(*            | nth primobj                   *)
   placename2: begin 
      prim := nthprimobj(envblock@.son,length,attstack@[newp+1].lexval);
      if prim = nil then markerror(857)
      end;
(*            | placename "." "<Label>" suffix  *)
   placename3: if prim <> nil then begin
      if attstack@[newp+3].lexval <> XEMPTY then with attstack@[newp+2] do
         addsuffix(chbuf,chbufx,length,attstack@[newp+3].xval);
      (*D if debuglevel > 0 then begin
         write(log,'Searching:'); printobject(prim);
         with attstack@[newp+2] do snapname(chbuf,chbufx,length);
         writeln(log) end; D*)
      primp := findplace(prim@.son,chbuf,
         attstack@[newp+2].chbufx,attstack@[newp+2].length);
      if primp = nil then marknotfound(855,
         chbuf,attstack@[newp+2].chbufx,attstack@[newp+2].length)
      (*D else if debuglevel > 0 then begin
         write(log,'placename:'); printobject(primp) end D*);
      prim := primp
      end;
(*            | placename "." nth primobj      *)
   placename4: if prim <> nil then begin 
      prim := nthprimobj(prim@.son,
         attstack@[newp+2].length,attstack@[newp+3].lexval);
      if prim = nil then markerror(857)
      end;

(*     nth = ncount "th"   *)
(*         | ncount "th" "last"   *)
(*         | "last"   *)
   nth1: if xval <= 0.0 then markerror(856)
      else length := round(xval);
   nth2: if xval <= 0.0 then markerror(856)
      else length := -round(xval);
   nth3: length := 0;

(*     primobj = "<primitiv>"   *)
(*             | "[]"   *)
(*             | "<string>"   *)
(*             | "[" "]"   *)
   primobj1,primobj2,primobj3: ;
   primobj4: lexval := XBLOCK;

(*     ncount = "<float>"   *)
   ncount1: ;
(*            | "`" expression "'"   *)
(*            | "lbrace" expression "rbrace"   *)
   ncount2, ncount3: xval := attstack@[newp+1].xval ;

(*     logprod = logval   *)
   logprod1: ;
(*             | logprod "&&" logval   *)
   logprod2: if (xval = 0.0) or (attstack@[newp+2].xval = 0.0) then
      xval := 0.0 else xval := 1.0;

(*   logval = lcompare *)
   logval1: if lexval = XLstring then begin
      markerror(869);
      lexval := XLfloat;
      deletestringbox(prim)
      end;
(*          | stringexpr "<" stringexpr *)
   logval2: begin
      i := cmpstring( prim,attstack@[newp+2].prim );
      if i < 0 then xval := 1.0 else xval := 0.0;
      lexval := XLfloat;
      deletestringbox( attstack@[newp+2].prim );
      deletestringbox( prim )
      end;
(*          | expression "<" expression *)
   logval3: if xval < attstack@[newp+2].xval then xval := 1.0 else xval := 0.0;

(*   lcompare = expression *)
(*            | stringexpr *)
   lcompare1,lcompare2: ;
(*            | lcompare "<compare>" expression *)
(*             "<" "==" "!=" ">=" "<=" ">"     *)
   lcompare3: begin
      if lexval=XLstring then begin
         markerror(869); bswitch := false; deletestringbox( prim ) end
      else case attstack@[newp+1].lexval-XLcompare of
          0: bswitch := (xval <  attstack@[newp+2].xval);
          1: bswitch := (xval =  attstack@[newp+2].xval);
          2: bswitch := (xval <> attstack@[newp+2].xval);
          3: bswitch := (xval >= attstack@[newp+2].xval);
          4: bswitch := (xval <= attstack@[newp+2].xval);
          5: bswitch := (xval >  attstack@[newp+2].xval);
          otherwise bswitch := false
          end;
      if bswitch then xval := 1.0 else xval := 0.0
      end;
(*            | lcompare "<compare>" stringexpr *)
   lcompare4: begin
      if lexval<>XLstring then begin markerror(869); bswitch := false end
      else begin
         i := cmpstring( prim,attstack@[newp+2].prim );
         (*D if debuglevel > 0 then writeln(log,' cmpstring=',i:1); D*)
         case attstack@[newp+1].lexval-XLcompare of
            0: bswitch := (i < 0);
            1: bswitch := (i = 0);
            2: bswitch := (i <> 0);
            3: bswitch := (i >= 0);
            4: bswitch := (i <= 0);
            5: bswitch := (i > 0);
            otherwise bswitch := false
            end;
         deletestringbox( prim )
         end;
      if bswitch then xval := 1.0 else xval := 0.0;
      lexval := XLfloat;
      deletestringbox( attstack@[newp+2].prim )
      end;

(*     primary = "<envvar>"   *)
(*             | "<name>" suffix   *)
(*             | "<float>"   *)
(*             | "(" logexpr ")"   *)
(*             | location ".x"   *)
(*             | location ".y"   *)
(*             | placename "<param>"   *)
(*             | "rand" "(" ")"   *)
(*             | "rand" "(" expression ")"   *)
(*             | "<func1>" "(" expression ")"   *)
(*             | "<func2>" "(" expression "," expression ")"   *)
(*             | "(" assignlist ")"   *)
(*             | "(" systemcmd ")"   *)
   primary1: if envblock <> nil then begin
      eb := findenv(envblock); xval := eb@.env@[lexval]
      end;
(*                           | "<name>" suffix    *)
   primary2: begin
      if attstack@[newp+1].lexval <> XEMPTY then
         addsuffix(chbuf,chbufx,length,attstack@[newp+1].xval);
      namptr := glfindname(envblock,chbuf,chbufx,length,lastvar,k);
      if namptr = nil then xval := 0.0 else xval := namptr@.val
      end;

(*                           | "<float>"    *)
   primary3: ;
(*                           | "(" logexpr ")"    *)
   primary4: xval := attstack@[newp+1].xval;
   primary5: ;                       (*   | place ".x"  *)
   primary6: xval := yval;           (*   | place ".y"  *)
(*                                        | placename "<param>"  *)
   primary7: if prim <> nil then case attstack@[newp+1].lexval of
      XLheight: xval := pheight(prim);
      XLwidth: xval := pwidth(prim);
      XLradius,XLdiameter: begin
         with prim@ do if ptype = XLcircle then xval := radius
         else if ptype = XLarc then xval := aradius
         else markerror(858);
         if attstack@[newp+1].lexval = XLdiameter then xval := xval*2.0
         end;
      XLthickness: with prim@ do if ptype
         in [XLbox,XLellipse,XLcircle,XLline,XLarrow,XLspline,XLarc] then
         xval := lthick
         else markerror(858);
      XLlength: with prim@ do if ptype in [XLline,XLarrow,XLmove,XLspline]
         then begin
           primp := prim; while primp@.son<>nil do primp := primp@.son;
           r := abs(primp@.endpos.xpos-aat.xpos);
           s := abs(primp@.endpos.ypos-aat.ypos);
           if r=0.0 then xval := s else if s=0.0 then xval := r
           else xval := sqrt(r*r+s*s)
           end
         else markerror(858);
      otherwise
      end;
(*                  | "rand" "(" ")"  random number between 0 and 1 *)
   primary8: begin
      xval := (random(*M () M*))/randmax
      end;
(*                  | "rand" "(" expression ")"  *)
   primary9: begin
      srandom(round(attstack@[newp+2].xval));
      xval := (random(*M () M*))/randmax
      end;
(*                  | "<func1>" "(" expression ")"  *)
   primary10: case lexval of
      XLabs: xval := abs( attstack@[newp+2].xval );
      XLacos: begin
         t := attstack@[newp+2].xval;
         if abs(t) > 1.0 then markerror(868)
         else xval := datan(sqrt(1-sqr(t)),t)
         end;
      XLasin: begin
         t := attstack@[newp+2].xval;
         if abs(t) > 1.0 then markerror(868)
         else xval := datan(t,sqrt(1-sqr(t)))
         end;
      XLcos: xval := cos( attstack@[newp+2].xval );
      XLexp: xval := exp( ln(10.0) * attstack@[newp+2].xval );
      XLexpe: xval := exp( attstack@[newp+2].xval );
      XLint: xval := trunc(attstack@[newp+2].xval);
      XLlog:  if attstack@[newp+2].xval <= 0.0 then markerror(867)
         else xval := ln( attstack@[newp+2].xval )/ln(10.0);
      XLloge: if attstack@[newp+2].xval <= 0.0 then markerror(867)
         else xval := ln( attstack@[newp+2].xval );
      XLsign: if attstack@[newp+2].xval >= 0.0 then xval := 1.0
         else xval := -1.0;
      XLsin: xval := sin( attstack@[newp+2].xval );
      XLsqrt: if attstack@[newp+2].xval < 0.0 then markerror(867)
         else xval := sqrt( attstack@[newp+2].xval );
      XLtan: begin
         t := cos( attstack@[newp+2].xval );
         if t = 0.0 then begin markerror(868); xval := distmax end
         else xval := sin( attstack@[newp+2].xval )/ t
         end;
      XLfloor: xval := Floor(attstack@[newp+2].xval);
      otherwise
      end;
(*                  | "<func2>" "(" expression "," expression ")"  *)
   primary11: case lexval of
      XLatan2: xval := datan( attstack@[newp+2].xval,attstack@[newp+4].xval );
      XLmax: xval := Max( attstack@[newp+2].xval,attstack@[newp+4].xval );
      XLmin: xval := Min( attstack@[newp+2].xval,attstack@[newp+4].xval );
      XLpmod: begin
         t := attstack@[newp+2].xval; s := attstack@[newp+4].xval;
         if s = 0 then markerror(871)
         else begin
            xval := t - s*Floor(t/s);
            if xval < 0 then xval := xval + abs(s)
            end
         end;
      otherwise
      end;
(*                 | "(" assignlist ")"    *)
   primary12: xval := attstack@[newp+1].xval;
(*                 | "(" systemcmd ")"    *)
   primary13: xval := attstack@[newp+1].xval

      end; (* case *)

   (*D with attstack@[newp] do if (debuglevel = 2) and (
      ((p >= block1) and (p <= block3)) or
      ((p > object1) and (p <= object26))
      or (p in [sprintf2,string2,element5,element11,namedobj2]))
        then printobject(prim);
   if debuglevel > 0 then with attstack@[newp] do
      if p in [ assignment1..assignment4,
                expression1..expression5,
                logexpr2,
                ncount2,
                term2..term4,
                factor2,factor3,
                logprod2,
                logval2,logval3,
                lcompare3,lcompare4,
                primary1..primary11
                ] then begin
            write(log,'(After prod=',p:1,')');
            write(log,' value='); wfloat(log,xval);
            write(log,' chbufx='); wfloat(log,chbufx);
            write(log,' chbufi='); wfloat(log,chbufi);
            writeln(log);
            prvars(newp)
            end
      else if p in [position1..position3,
         location1,location2,shift2,shift3,place1..place3] then begin
         write(log,'location:');
         with attstack@[newp] do wpair(log,xval,yval);
         writeln(log); flush(log)
         end;
   if (linesignal > 0) and (p=input2) then begin
      writeln(errout,'bottom:'); flush(errout); end
   D*)
   end; (* produce *)
(* G end.G *)
