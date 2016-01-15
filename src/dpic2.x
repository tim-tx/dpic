(*BXsegment dpic2;XB*)
(* G module dpic2; G *)

(* include dp0.h *)
(*BXU#include 'dp0.h'UXB*)
(* G#include 'dp0.h'G *)

(* include dp1.h *)
(*BXU#include 'dp1.h'UXB*)
(* G#include 'dp1.h'G *)

(* include sysdep.h *)
(*DBXU#include 'sysdep.h'UXBD*)
(* DG#include 'sysdep.h'GD *)

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

procedure wfloat(* var iou: text; y: real *);
(*P2CIP*)
var x: real;
  i,k: integer;
begin
   if isdistmax(abs(y)) then write(iou,y:11)
   else begin
      x := abs(y) - trunc(abs(y));
      if x = 0 then write(iou,round(y):1)
      else begin
         i := round(x*1000000); k := 0;
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

procedure wpair(* var iou: text; x,y: real *);
begin
   write(iou,'(');
   wfloat(iou,x); write(iou,','); wfloat(iou,y);
   write(iou,')')
   end;

procedure wcoord(* var iou: text; x,y: real *);
begin
   write(iou,'(');
   wfloat(iou,x/fsc); write(iou,',');
   wfloat(iou,y/fsc); write(iou,')')
   (*DUGHMF ;flush(iou) FMHGUD*)
   end;

procedure wpos(* pos: postype *);
begin
   wcoord(output, pos.xpos,pos.ypos )
   end;

procedure wstring(* var iou: text; p: strptr *);
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

function testjust(n,flag: integer): boolean;
begin
   case flag of
      XEMPTY: testjust := (n=0);
      XLcenter: testjust := (n=15);
      XLrjust: testjust := (n mod 4) = 1;
      XLljust: testjust := (n mod 4) = 2;
      XLbelow: testjust := (n div 4) = 1;
      XLabove: testjust := (n div 4) = 2
      end
   end;

procedure checkjust(* tp: strptr; var A,B,L,R: boolean *);
var i: integer;
begin
   if tp=nil then begin
      A := false; B := false; L := false; R := false end
   else begin
      i := round(tp@.val);
      A := testjust(i,XLabove);
      B := testjust(i,XLbelow);
      L := testjust(i,XLljust);
      R := testjust(i,XLrjust);
      end
   end;

function lspec(* n: integer): integer *);
begin
   (* if ((n div 16) mod 2) <> 0 then lspec := XLsolid
   else *) lspec := (n mod 8) + XLlinetype
   end;

procedure getlinespec(* nd:primitivep; var lsp:integer; var lastnd:primitivep*);
var tn: primitivep;
begin
   tn := nd; if nd@.ptype in [XLspline,XLline,XLarrow,XLarc] then
     while tn@.son <> nil do tn := tn@.son;
   lastnd := tn;
   lsp := lspec(tn@.spec)
   end;


function findenv(* p: primitivep ): primitivep *);
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
      if drawmode=SVG then offst := boxheight/3;
      x := aat.xpos;
      if R then x := x-wd*0.5-offst else if L then x := x+wd*0.5+offst;
      north := Max(north,y+ht*0.5);
      south := Min(south,y-ht*0.5);
      west := Min(west,x-wd*0.5);
      east := Max(east,x+wd*0.5)
      (*D;  if debuglevel>0 then begin
         write(log,' neswstring:');
         wlogfl('aat.xpos',aat.xpos,0); wlogfl('x',x,0);
         wlogfl('east',east,0); wlogfl('west',west,0); wlogfl('wd',wd,1)
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

procedure nesw(* ptmp: primitivep *);
var hight,wdth,sang,eang: real;
begin
   if ptmp <> nil then begin
      with ptmp@ do case ptype of
         XLbox,XLstring:begin hight := boxheight;    wdth := boxwidth end;
         XBLOCK:        begin hight := blockheight;  wdth := blockwidth end;
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
         XLstring: neswstring( ptmp,hight,wdth );
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
         write(log, 'nesw(',ordp(ptmp):1,') ptype=',ptype:1);
         wlogfl('W',west,0); wlogfl('S',south,0);
         wlogfl('E',east,0); wlogfl('N',north,1)
         end D*)
      end
   end;

function linlen(* x,y: real ): real *);
var xm,ym: real;
begin
   (* linlen := sqrt( x*x + y*y ) *)
   if abs(x) > abs(y) then begin xm := abs(x); ym := y/xm end
   else if y = 0.0 then begin xm := 0.0; ym := 0.0 end
   else begin xm := abs(y); ym := x/xm end;
   linlen := xm * sqrt(1.0 + ym*ym)
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
var i,small,large: integer;
begin
   if x < 0 then x := -x;
   if y < 0 then y := -y;
   if y < x then begin small := y; large := x end
   else begin small := x; large := y end;
   while small > 0 do begin
      i := small; small := large mod small; large := i end;
   if large = 0 then hcf := 1 else hcf := large
   end;

function iabs(i: integer): integer;
begin
  if i < 0 then iabs := -i else iabs := i
  end;

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

function isthen(* pr: primitivep ): boolean *);
begin
   if pr = nil then isthen := false
   else isthen := ((pr@.spec div 8) mod 2) <> 0
   end;

function firstsegment(* pr: primitivep ): boolean *);
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
   C := affang(shaft,point);       (* shaft direction cosines *)
   po := ahoffset(ht,wid,ltu);
   if po > ht then po := ht;
   P := affine(po,0.0,point,C);    (* point adjusted by line thickness *)
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
   Px.xpos := (point.xpos+Lx.xpos+Rx.xpos)/3;    (* type 3 center pt *)
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

function arcend(n: primitivep): postype;
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
   (*D; if debuglevel > 0 then writeln(log,' treedraw done') D*)
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
   *)(* D; if debuglevel > 0 then writeln(log,' treedraw done') D *)(*
   end; *)

procedure drawtree(* n,s,e,w: real; eb: primitivep *);
var fsctmp: real;
begin
   bfill := false; sshade := nil; soutline := nil;
   case drawmode of
      SVG: begin
         fsctmp := fsc; fsc := fsc/SVGPX;         (* output pixels *)
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
(*B.B*)
(* G end. G *)
