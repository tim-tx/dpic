(* pdf.x *)
(* Output routines for PDF *)

procedure pdfstream(s: string; ls: integer; var strm: strptr);
var i,ll,s0: integer;
ns: strptr;
begin
   s0 := 1;
   while ls > 0 do begin
      with strm@ do if len-1+ls <= CHBUFSIZ then ll := ls
      else ll := CHBUFSIZ-len+1;
      with strm@ do begin
         for i:= 0 to ll-1 do segmnt@[len+i] := s[s0+i];
         s0 := s0+ll;
         len := len + ll
         end;
      if strm@.len > CHBUFSIZ then begin
         newstr(ns); new(ns@.segmnt); strm@.next := ns; strm := ns end;
      ls := ls-ll
      end
   end;

procedure pdfwln(s: string; ln: integer; var strm: strptr);
begin
   pdfstream(s,ln,strm);
   pdfstream(nlch,1,strm)
   end;

procedure addbytes(n: integer);
begin
   if pdfobjcount=0 then pdfoffs[1] := n
   else pdfoffs[pdfobjcount+1] := pdfoffs[pdfobjcount]+n;
   pdfobjcount := pdfobjcount+1
   end;

procedure pdfprelude(n,s,e,w,lth: real);
var wx,ex,nx,sx: real;
begin
   pdfobjcount := 0;
   writeln('%PDF-1.4');
   writeln('% Creator: VERSIONDATE option -d for PDF');
     addbytes(9+53);

(*          123456789 123456789 123456789 123456789 123456789 123456789 12345*)
(*                   1.        2.        3.        4.        5.        6.    *)
   writeln(pdfobjcount:0, (* pdfobjcount must be 1 here *)
            ' 0 obj << /Type /Catalog /Pages 2 0 R >> endobj');
            addbytes(49);

   writeln(pdfobjcount:0,
            ' 0 obj << /Type /Pages /Kids [ 3 0 R ] /Count 1 >> endobj');
            addbytes(59);

   writeln(pdfobjcount:0,
            ' 0 obj << /Type /Page /Parent 2 0 R');
   wx := w/fsc-lth/2; ex := e/fsc+lth/2; nx := n/fsc+lth/2; sx := s/fsc-lth/2;
   write(' /MediaBox [ '); writeln(
      Floor(round(wx*1000000)/1000000.0):5, ' ',
      Floor(round(sx*1000000)/1000000.0):5, ' ',
       Ceil(round(ex*1000000)/1000000.0):5, ' ',
       Ceil(round(nx*1000000)/1000000.0):5, ' ]');
   (*D; if debuglevel > 0 then begin
      write(log,'lth= '); wfloat(log,lth);
      write(log,' wx='); wfloat(log,wx);
      write(log,' Floor wx='); wfloat(log,Floor(round(wx*1000000)/1000000.0));
      writeln(log)
      end; D*)
   writeln(' /Contents 6 0 R');
   writeln(' /Resources <<');
   writeln('   /ProcSet ',(pdfobjcount+1):0,' 0 R');
   writeln('   /Font << /F1 ',(pdfobjcount+2):0,' 0 R >>');
   writeln('   >>');
   writeln(' >> endobj');
     addbytes(37+39+17+15+18+25+6+11);

   writeln(pdfobjcount:0,
            ' 0 obj [/PDF /Text] endobj');
            addbytes(28);

   writeln(pdfobjcount:0,
            ' 0 obj << /Type /Font /Subtype /Type1');
   writeln(' /Name /F1 /BaseFont /Courier              ');
   writeln(' /Encoding /WinAnsiEncoding');
   writeln(' >> endobj');
     addbytes(39+44+28+11);

(* writeln(pdfobjcount:0,
            ' 0 obj << /Type /Font /Subtype /Type1');
   writeln(' /Name /F2 /BaseFont /Helvetica-BoldOblique');
   writeln(' /Encoding /WinAnsiEncoding');
   writeln(' >> endobj');
     addbytes(39+44+28+11); *)

(*                                  initialize system parameters             *)
   gsocolor := false; gsfcolor := false; gsgcolor := false;
   gslinecap := 0; gslinejoin := 0;
   gsdashw := 0; gsdashs := 0;
   newstr(stream); new(stream@.segmnt); cx := stream;
   pdfwln(' 0.8 w',6,cx); gslinethick := 0.8
   end;

procedure pdfwfloat( y: real);
var ix,j,k,ln: integer;
  ts: array[1..10] of char;
  nz: boolean;
begin
   pdfstream(' ',1,cx);
   if y < 0 then begin pdfstream('-',1,cx); y := -y end;
   if y = trunc(y) then begin j := 7; ix := round(y); nz := true end
   else begin j := 0; ix := round(y*1000000); nz := false end;
   ln := 0;
   repeat
      j := j+1;
      k := ix mod 10;
      if (j=7) and nz then begin ln := ln+1; ts[ln] := '.' end
      else if j=7 then nz := true
      else if not nz then nz := k<>0;
      if nz then begin ln := ln+1; ts[ln] := chr(ord('0')+k) end;
      ix := ix div 10
   until (ix = 0) and (j>6);
   for k:=1 to ln do pdfstream(ts[ln+1-k],1,cx)
   end;

procedure resetgs(node: primitivep);
var x: real;
   begin
   if gsocolor then begin pdfwln(' 0 0 0 RG',9,cx); gsocolor := false end;
   if gsfcolor then begin pdfwln(' 0 0 0 rg',9,cx); gsfcolor := false end;
   if gsgcolor then begin pdfwln(' 0 g',4,cx); gsgcolor := false end;
   x := venv(node,XLlinethick);
   if (gslinethick<>x) then
     begin pdfwfloat(x); pdfwln(' w',2,cx); gslinethick := x end;
   if (gslinecap<>0) then begin pdfwln(' 0 J',4,cx); gslinecap := 0 end;
   if (gslinejoin<>0) then begin pdfwln(' 0 j',4,cx); gslinejoin := 0 end;
   if (gsdashw<>0) or (gsdashs<>0) then
     begin pdfwln(' [] 0 d',7,cx); gsdashw := 0; gsdashs := 0 end
   end;

procedure pdfwlz( n: integer );
var
  s: array[1..10] of char;
  i,j: integer;
begin
   j := 10;
   while n > 0 do begin
      s[j] := chr(ord('0')+(n mod 10));
      j := j-1;
      n := n div 10
      end;
   for i:= 1 to j do write('0');
   for i:= j+1 to 10 do write(s[i]);
   writeln(' 00000 n ');
   end;

procedure pdfpostlude;
var s,t: strptr;
   i,streamlen: integer;
begin
   streamlen := 0;
   s := stream;
   while s<>nil do begin
      streamlen := streamlen+s@.len;
      s := s@.next
      end;
   writeln(pdfobjcount:0,
            ' 0 obj << /Length ',streamlen:6,' >>');
   writeln('stream');
   s := stream;
   while s<>nil do with s@ do begin
      for i:=0 to len-1 do write(segmnt@[i]);
      s := next
      end;
   writeln('endstream');
   writeln('endobj');
     addbytes((29+7+10+7)+streamlen);

   writeln('xref');
   writeln('0 ',pdfobjcount:0);
   writeln('0000000000 65535 f ');
   for i:=1 to pdfobjcount-1 do pdfwlz( pdfoffs[i] );
   writeln('trailer');
   writeln('   << /Size ',pdfobjcount:0);
   writeln('      /Root 1 0 R');
   writeln('   >>');
   writeln('startxref');
   writeln(pdfoffs[pdfobjcount]:0);
   writeln('%%EOF');
                                (* delete output buffers                     *)
   while stream<>nil do begin
      (*D if debuglevel > 0 then write(log,'dispose stream:'); D*)
      s := stream; t := nil;
      while s@.next<>nil do begin t := s; s := s@.next end;
      (*D if debuglevel > 0 then begin
         write(log,'[');
         if s <> nil then write(log,ordp(s):1) else write(log,'nil');
         writeln(log,']')
         end; D*)
      dispose(s@.segmnt); dispose(s);
      if t=nil then stream := nil else t@.next := nil
      end
   (*D; if debuglevel > 0 then writeln(log,'pdfpostlude done');D*)
   end;

procedure pdfwcoord( x,y: real);
begin
   pdfwfloat(x/fsc);
   pdfwfloat(y/fsc)
   end;

procedure pdfwpos(pos: postype);
begin
   pdfwcoord( pos.xpos,pos.ypos )
   end;

procedure pdfwstring( p: strptr );
var i: integer;
   c: char;
   waswhite,iswhite: boolean;
begin
   if p<>nil then with p@ do if segmnt<>nil then begin
     waswhite := false;
     for i:= 0 to len-1 do begin
       c := segmnt@[seginx+i];
       iswhite := (c in [' ',tabch,nlch,etxch]);
       if (not iswhite) or (not waswhite) then begin
         if c in ['(',')',bslch] then pdfstream(bslch,1,cx);
         pdfstream(c,1,cx) end;
       waswhite := iswhite
       end
     end
   end;

procedure pdfwtext(node: primitivep; tp: strptr; x,y: real );
var nstr: integer;
   tx: strptr;
   L,R,A,B: boolean;
   textht,textwid,textoff,dx,dy,ratio: real;
begin
  ratio := 13/11;
  resetgs(node);
  if (tp <> nil) then with node@ do begin
     tx := tp; nstr := 0;
     while tx<>nil do begin nstr := nstr+1; tx := tx@.next end;
     textht := venv(node,XLtextht); textwid := venv(node,XLtextwid);
     if (ptype=XLstring) and (nstr > 0) then begin
       textht := boxheight/nstr;
       textwid := boxwidth end;
     textoff := venv(node,XLtextoffset);
     (*D if debuglevel>0 then begin
       write(log,' pdfwtext: x=',x:8:3,'[',x/fsc:8:3,']');
       writeln(log,        ' y=',y:8:3,'[',y/fsc:8:3,']');
       writeln(log,' textoff=',textoff:8:3,'[',textoff/fsc:8:3,']');
       writeln(log,' textht=',textht:8:3,'[',textht/fsc:8:3,']',
                   ' textwid=',textwid:8:3,'[',textwid/fsc:8:3,']') end; D*)
     y := y + (nstr/2-4/5)*textht;
     nstr := 1;
     with node@ do repeat
        if (textwid <= 0) or (nstr > 1) then textwid := tp@.len*textht*0.6;
        checkjust(tp, A,B,L,R );
        pdfwln('BT',2,cx);
        pdfstream('/F1',3,cx); pdfwfloat(textht/scale*72); pdfwln(' Tf',3,cx);
        if L then dx := textoff
        else if R then dx := -textoff-tp@.len*textht*0.6
        else if ptype=XLstring then dx := -textwid/2
        else dx := -tp@.len*textht*0.6/2;
        dy := textht/10;
        if A then dy := dy+textoff+textht*0.3
        else if B then dy := dy-textoff-textht*(ratio-1)*0.7;
        (*D if debuglevel>0 then begin
          writeln(log,' A=',A,' B=',B,' L=',L,' R=',R);
          writeln(log,' dx=',dx:8:3,'[',dx/fsc:8:3,']',
                      ' dy=',dy:8:3,'[',dy/fsc:8:3,']');
          writeln(log,' x=',x:8:3,'[',x/fsc:8:3,']',
                      ' y=',y:8:3,'[',y/fsc:8:3,']');
          writeln(log,' x+dx=',(x+dx):8:3,'[',(x+dx)/fsc:8:3,']',
                      ' y+dy=',(y+dy):8:3,'[',(y+dy)/fsc:8:3,']') end; D*)
        pdfwcoord(Max(0,x+dx),Max(0,y+dy));
        pdfwln(' Td',3,cx);
        pdfstream('(',1,cx); pdfwstring( tp ); pdfwln(') Tj',4,cx);
        pdfwln('ET',2,cx);
        y:=y-textht;
        nstr := nstr+1;
        tp := tp@.next
     until tp = nil
     end
  end;

procedure pdfwprop( p1,p2: postype; a,b,c: real );
begin
   pprop( p1,p2, a,b,c );
   pdfwpos( p2 )
   end;

procedure pdfsetthick(lt: real);
begin
   if (lt >= 0.0) and (lt <> gslinethick) then begin
      pdfwfloat(lt); pdfwln(' w',2,cx);
      gslinethick := lt
	  end
   end;

procedure pdflinearfill( f: real; ss: strptr );
begin
   if (f>=0.0) and (f<=1.0) then begin
      pdfwfloat(f); pdfwln(' g',2,cx); gsgcolor := true end
   else if ss<>nil then begin
      pdfstream(' ',1,cx); pdfwstring(ss); pdfwln(' rg',3,cx);
      gsfcolor := true end
   end;

procedure pdfsetcolor(op: strptr; f:boolean);
begin
   if op<>nil then begin
      pdfstream(' ',1,cx); pdfwstring(op);
      if f then begin pdfwln(' rg',3,cx); gsfcolor := true end
           else begin pdfwln(' RG',3,cx); gsocolor := true end
      end
   end;

procedure pdflineopts( lspec: integer; param,thck: real; op:strptr );
begin
   pdfsetthick(thck);
   if lspec = XLdashed then begin 
      if ismdistmax(param) then param := 6*fsc;
      pdfstream(' [',2,cx);
      pdfwfloat(param*7/6/fsc); pdfwfloat(param*5/6/fsc);
      pdfwln(' ] 0 d 0 j',10,cx);
      gsdashw := param; gsdashs := param
      end
   else if lspec = XLdotted then begin
      if ismdistmax(param) then param := 4*fsc;
      pdfstream(' [',2,cx);
      pdfwfloat(0.0);
      pdfwfloat(param/fsc);
      pdfwln('] 0 d 1 J',9,cx);
      gsdashw := 0; gsdashs := param
      end;
   pdfsetcolor(op,false)
   end;

procedure pdfwarc( Ctr,St,En: postype; radius,ccw: real );
var narcs,i: integer;
   c,s,cc,ss,startangle,endangle,arcangle: real;
   Q: postype;
begin
   startangle := datan(St.ypos-Ctr.ypos,St.xpos-Ctr.xpos);
   endangle   := datan(En.ypos-Ctr.ypos,En.xpos-Ctr.xpos);
   if (ccw > 0) and (endangle < startangle) then
      endangle := endangle + 2*pi
   else if (ccw < 0) and (endangle > startangle) then
      endangle := endangle - 2*pi;
   narcs := trunc(1.0+abs(endangle-startangle)/(pi/2));
   arcangle := (endangle - startangle)/narcs;
   c := cos(arcangle/2); s := sin(arcangle/2);
   cc := (4-c)/3; if s<>0.0 then ss := (1.0-c*cc)/s else ss := 0.0;
   for i := 1 to narcs do begin
      Q.xpos := cos(startangle+(i-0.5)*arcangle);
      Q.ypos := sin(startangle+(i-0.5)*arcangle);
      pdfwpos(affine(radius*cc,-radius*ss,Ctr,Q));
      pdfwpos(affine(radius*cc, radius*ss,Ctr,Q));
      Q.xpos := Ctr.xpos + radius*cos(startangle+i*arcangle);
      Q.ypos := Ctr.ypos + radius*sin(startangle+i*arcangle);
      pdfwpos(Q);
      pdfwln(' c',2,cx)
      end
   end;

procedure pdfarcahead(C: postype; atyp: integer;
   var point: postype;
       ht,wid,lth,radius,angle: real; sou: strptr );
var 
   P,Q,Co,Ci,Px,Cox,Cix,Ao,Ai: postype;
   ccw,lwi: real;
   startarrow: boolean;
begin
   arcahead( C,point,atyp, ht,wid,lth,radius,angle,
      P,Co,Ci,Px,Cox,Cix,Ao,Ai, ccw,lwi, startarrow );
   radius := abs(radius);
                                 (* Trace arrowhead outline *)
   pdfwpos(Ai); pdfwln(' m',2,cx);
   pdfwarc(Ci,Ai,point,radius,-ccw);
   pdfwarc(Co,point,Ao,radius,ccw);
   if (atyp=0) and (lwi < (wid-lwi)/2) then begin
      Q := Co; pprop(Ao,Q,radius-lwi,lwi,radius);
      pdfwpos(Q); pdfwln(' l',2,cx);
      pdfwarc(Co,Q,P,radius-lwi,-ccw);
      Q := Ci; pprop(Ai,Q,radius+lwi,-lwi,radius);
      pdfwarc(Ci,P,Q,radius+lwi,ccw)
      end;
   if (atyp=3) and (lwi < (wid-lwi)/2) then begin
      pdfwarc(Cox,Ao,Px,radius,-ccw);
      pdfwarc(Cix,Px,Ai,radius,ccw)
      end
   else begin pdfwpos(Ai); pdfwln(' l',2,cx) end;
   pdfwln(' f',2,cx);
   point := P
   end;

procedure pdfahead(atyp: integer;
   var point: postype;
       shaft: postype;
       ht,wid,lth: real; sou: strptr );
var 
   P,L,R,Px,Lx,Rx,Q: postype;
   x,y: real;
begin
   dahead( point,shaft,ht,wid,lth/72*scale,P,L,R,Px,Lx,Rx,Q,x,y );
   if atyp=0 then begin
      pdfsetthick(lth);
      pdfsetcolor(sou,false);
      pdfwprop(P,R,x-y,y,x); pdfstream(' m',2,cx);
      pdfwpos(P); pdfwln(' l',2,cx);
      pdfwprop(P,L,x-y,y,x); pdfstream(' l',2,cx);
      pdfwln(' S',2,cx)
      end
   else begin
      pdfsetcolor(sou,true);
      pdfwpos(Rx); pdfwln(' m',2,cx);
      pdfwpos(point); pdfstream(' l',2,cx);
      pdfwpos(Lx); pdfstream(' l',2,cx);
      if atyp=3 then begin pdfwpos(Px); pdfstream(' l',2,cx) end;
      pdfwln(' h f',4,cx)
      end;
   point := P
   end;

(* function pdfpos(x,y:real): postype;
var
  Q: postype;
begin
   Q.xpos := x; Q.ypos := y;
   pdfpos := Q
   end; *)

procedure pdfbox( aat: postype; halfwid,halfht,rad: real);
var r,n,s,e,w: real;
begin
   r := 1-SPLT;
   rad := Min(Min(abs(rad),abs(halfht)),abs(halfwid));
   n := aat.ypos+halfht;  s := aat.ypos-halfht;
   e := aat.xpos+halfwid; w := aat.xpos-halfwid;
   if rad = 0.0 then begin
      pdfwcoord(w,s);
      pdfwcoord(2*halfwid,2*halfht);
      pdfwln(' re',3,cx)
      end
   else begin
     pdfwcoord(e,n-rad); pdfwln(' m',2,cx);
     pdfwcoord(e,n-rad*r);
     pdfwcoord(e-rad*r,n);
     pdfwcoord(e-rad,n); pdfwln(' c',2,cx);
     pdfwcoord(w+rad,n); pdfwln(' l',2,cx);
     pdfwcoord(w+rad*r,n);
     pdfwcoord(w,n-rad*r);
     pdfwcoord(w,n-rad); pdfwln(' c',2,cx);
     pdfwcoord(w,s+rad); pdfwln(' l',2,cx);
     pdfwcoord(w,s+rad*r);
     pdfwcoord(w+rad*r,s);
     pdfwcoord(w+rad,s); pdfwln(' c',2,cx);
     pdfwcoord(e-rad,s); pdfwln(' l',2,cx);
     pdfwcoord(e-rad*r,s);
     pdfwcoord(e,s+rad*r);
     pdfwcoord(e,s+rad); pdfstream(' c h',4,cx)
     end
   end;

procedure pdfellipse(Ctr: postype; x,y: real);
begin
   x := abs(x)/2; y := abs(y)/2;
   pdfwcoord(Ctr.xpos+x,Ctr.ypos); pdfwln(' m',2,cx);
   pdfwcoord(Ctr.xpos+x,Ctr.ypos+y*SPLT); 
   pdfwcoord(Ctr.xpos+x*SPLT,Ctr.ypos+y); 
   pdfwcoord(Ctr.xpos,Ctr.ypos+y); pdfwln(' c',2,cx); 
   pdfwcoord(Ctr.xpos-x*SPLT,Ctr.ypos+y); 
   pdfwcoord(Ctr.xpos-x,Ctr.ypos+y*SPLT); 
   pdfwcoord(Ctr.xpos-x,Ctr.ypos); pdfwln(' c',2,cx); 
   pdfwcoord(Ctr.xpos-x,Ctr.ypos-y*SPLT); 
   pdfwcoord(Ctr.xpos-x*SPLT,Ctr.ypos-y); 
   pdfwcoord(Ctr.xpos,Ctr.ypos-y); pdfwln(' c',2,cx); 
   pdfwcoord(Ctr.xpos+x*SPLT,Ctr.ypos-y); 
   pdfwcoord(Ctr.xpos+x,Ctr.ypos-y*SPLT); 
   pdfwcoord(Ctr.xpos+x,Ctr.ypos); pdfstream(' c h',4,cx); 
   end;

procedure pdfsplinesegment(tv: primitivep; splc,splt: integer);
   begin
      if tv<>nil then with tv@ do if splt=1 then begin
         pdfwpos(aat); pdfwln(' m',2,cx);
         pdfwpos( endpos ); pdfstream(' l',2,cx)
         end
      else if ismdistmax(aradius) then begin
         if splc = splt then begin (* 1st seg *)
            pdfwpos(aat); pdfwln(' m',2,cx);
            pdfwprop(aat,endpos,1.0,1.0,2.0 ); pdfwln(' l',2,cx);
            pdfwprop(aat,endpos,1.0,5.0,6.0)
            end
         else if splc > 1 then begin (* interior segment *)
            pdfwprop(aat,endpos, 5.0,1.0,6.0 );
            pdfwprop(aat,endpos, 1.0,1.0,2.0 ); pdfwln(' c',2,cx);
            pdfwprop(aat,endpos, 1.0,5.0,6.0 )
            end
         else begin    (* last segment *)
            pdfwprop(aat,endpos, 5.0,1.0,6.0 );
            pdfwprop(aat,endpos, 1.0,1.0,2.0 ); pdfwln(' c',2,cx);
            pdfwpos( endpos ); pdfstream(' l',2,cx)
            end
         end
      else if splc=splt then begin
         pdfwpos(aat); pdfwln(' m',2,cx);
         pdfwprop(aat,endpos,1-aradius,aradius,1.0)
         end
      else if splc > 1 then begin
         pdfwprop(aat,endpos, 1+aradius,1-aradius,2.0 );
         pdfwprop(aat,endpos, 1.0,1.0,2.0 ); pdfwln(' c',2,cx);
         pdfwprop(aat,endpos, 1-aradius,1+aradius,2.0 )
         end
      else begin
         pdfwprop(aat,endpos, aradius,1-aradius,1.0 );
         pdfwpos( endpos ); pdfstream(' c',2,cx)
         end
   end;
                                (* node is always <> nil *)
procedure pdfdraw( node: primitivep );
var lsp: integer;
    X1,X2: postype;
    tn,tx: primitivep;
    h,w,x,y,lth,fill: real;
    fll: boolean;
begin
   getlinespec(node,lsp,tn); (* lsp = dotted, dashed ... *)
   with node@ do begin
      lth := qenv(node,XLlinethick,lthick);
      (*D if debuglevel > 0 then begin write(log,'pdfdraw: ');
         snaptype(log,ptype); D*) (* printobject(node); *) (*D
         write(log,' lth='); wfloat(log,lth);
         writeln(log)
         end;
      if linesignal > 0 then begin write(errout,'pdfdraw: ');
         snaptype(errout,ptype); writeln(errout) end D*)
      end;
   with node@ do case ptype of
      XBLOCK: pdfwtext(node,textp, aat.xpos, aat.ypos );
      XLbox: begin
         if drawn(node,lsp,boxfill) then begin
            fll := ((boxfill >= 0.0) and (boxfill <= 1.0)) or (shadedp<>nil);
            pdflinearfill(boxfill,shadedp);
            pdflineopts(lsp,lparam,lth,outlinep);
            pdfbox(aat,boxwidth/2,boxheight/2,boxradius);
            if fll and (lsp<>XLinvis) then pdfwln(' B',2,cx)
            else if fll then pdfwln(' f',2,cx)
            else pdfwln(' S',2,cx)
            end;
         pdfwtext(node,textp,aat.xpos,aat.ypos)
         end;
      XLellipse,XLcircle: begin
         if ptype = XLellipse then begin
            x := elwidth; y := elheight; fill := efill end
         else begin
            x := 2*radius; y := x; fill := cfill end;
         if drawn(node,lsp,fill) then begin
            fll := ((fill >= 0.0) and (fill <= 1.0)) or (shadedp<>nil);
            pdflinearfill(fill,shadedp);
            pdflineopts(lsp,lparam,lth,outlinep);
            pdfellipse(aat,x,y);
            if fll and (lsp<>XLinvis) then pdfwln(' B',2,cx)
            else if fll then pdfwln(' f',2,cx)
            else pdfwln(' S',2,cx)
            end;
         pdfwtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLarc: begin
        getlinshade(node,tn,sshade,soutline,vfill,bfill);
        X1 := arcstart(node); X2 := arcend(node);
        if bfill then begin
           pdflinearfill(vfill,sshade);
           pdfwpos( X1 ); pdfwln(' m',2,cx);
           pdfwarc(aat,X1,X2,aradius,|arcangle|);
           pdfwln(' f',2,cx); resetgs(node)
           end;
        if lsp<>XLinvis then begin
           if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
              pdfsetcolor(soutline,true);
              startarc(node,X1,lth,h,w);
              pdfarcahead(aat,ahnum(atype),X1,
                 h,w,lth,abs(aradius),|arcangle|,soutline);
              resetgs(node)
              end;
           if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
              pdfsetcolor(soutline,true);
              endarc(node,X2,lth,h,w);
              pdfarcahead(aat,ahnum(atype),X2,
                 h,w,lth,-abs(aradius),|arcangle|,soutline);
              resetgs(node)
              end;
           pdflineopts(lsp,lparam,lth,soutline);
           pdfwpos( X1 ); pdfwln(' m',2,cx);
           pdfwarc(aat,X1,X2,aradius,|arcangle|);
           pdfwln(' S',2,cx)
           end;
        pdfwtext(node,textp,aat.xpos,aat.ypos )
        end;
      XLline,XLarrow,XLmove: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               pdflinearfill(vfill,sshade);
               tx := node; pdfwpos(aat); pdfwln(' m',2,cx);
               while tx<>nil do begin pdfwpos(tx@.endpos);
                  pdfwln(' l',2,cx); tx := tx@.son end;
               pdfwln(' f',2,cx);
               resetgs(node)
               end;
            lth := qenv(tn,XLlinethick,tn@.lthick);
            if lsp<>XLinvis then begin
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                  pdfsetcolor(soutline,true);
                  pdfahead(ahnum(tn@.atype),aat,endpos,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,soutline);
                  resetgs(node)
                  end;
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                  pdfsetcolor(soutline,true);
                  pdfahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,soutline);
                  resetgs(node)
                  end;
               pdflineopts(lsp,lparam,lth,soutline);
               pdfwpos(aat); pdfwln(' m',2,cx)
               end
            end;
         if lsp<>XLinvis then begin
            pdfwpos( endpos ); pdfstream(' l',2,cx);
            if son<>nil then pdfstream(nlch,1,cx) else pdfwln(' S',2,cx)
            end;
         if son=nil then while snode<>nil do with snode@ do begin
            if textp<>nil then pdfwtext(snode,textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLspline: begin
         if firstsegment(node) then begin
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               spltot := primdepth( node ); splcount := spltot;
               pdflinearfill(vfill,sshade);
               tx := node; 
               while tx<>nil do with tx@ do begin
                  pdfsplinesegment(tx,splcount,spltot);
                  splcount := splcount - 1; tx := tx@.son
                  end;
               pdfwln(' f',2,cx);
               resetgs(node)
               end;
            if lsp<>XLinvis then begin
               spltot := primdepth( node ); splcount := spltot;
               lth := qenv(tn,XLlinethick,tn@.lthick);
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                  pdfsetcolor(soutline,true);
                  pdfahead(ahnum(tn@.atype),aat,endpos,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,soutline);
                  resetgs(node)
                  end;
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                  pdfsetcolor(soutline,true);
                  pdfahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,soutline);
                  resetgs(node)
                  end;
               pdflineopts(lsp,tn@.lparam,lth,soutline)
               end
            end;
         if lsp<>XLinvis then begin
            pdfsplinesegment(node,splcount,spltot);
            if splcount=1 then begin
               pdfwln(' S',2,cx);
               resetgs(node)
               end
            end;
           (*D; if debuglevel > 0 then writeln(log,'node ',splcount:1) ;D*)
         splcount := splcount - 1
         end;
      XLstring: pdfwtext(node,textp,aat.xpos,aat.ypos );
      XLaTeX: if textp <> nil then begin
         pdfwstring( textp ); pdfstream(nlch,1,cx)
         end;
      otherwise
      end
   (*DGHMF ;flush(output) FMHGD*)
   end; (* pdfdraw *)
