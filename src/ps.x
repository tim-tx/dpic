(* ps.x *)
(* Output routines for Postscript, PSfrag *)
procedure pswfloat(var iou: text; x: real);
begin
   write(iou,' '); wfloat(iou,x);
   (*DGHMF ;flush(iou) FMHGD*)
   end;

procedure psprelude(n,s,e,w,lth: real);
var wx,ex,nx,sx: real;
begin
   (* writeln('%!PS'); *)
   (* writeln('%!PS-Adobe-3.0 EPSF-3.0'); *)
   writeln('%!PS-Adobe-3.0');
   wx := w/fsc-lth/2; ex := e/fsc+lth/2; nx := n/fsc+lth/2; sx := s/fsc-lth/2;
   writeln('%%BoundingBox: ',
      Floor(round(wx*1000000)/1000000.0):0, ' ',
      Floor(round(sx*1000000)/1000000.0):0, ' ',
       Ceil(round(ex*1000000)/1000000.0):0, ' ',
       Ceil(round(nx*1000000)/1000000.0):0);
   (*D; if debuglevel > 0 then begin write(log,'lth= '); wfloat(log,lth);
      write(log,' wx='); wfloat(log,wx);
      write(log,' Floor wx='); wfloat(log,Floor(round(wx*1000000)/1000000.0));
      writeln(log)
      end; D*)
   write('%%HiResBoundingBox:');
      pswfloat(output,wx); pswfloat(output,sx);
      pswfloat(output,ex); pswfloat(output,nx); writeln;
   write('%%Creator: VERSIONDATE option ');
   case drawmode of
      PSfrag: write('-f (psfrag strings)');
      PS: write('-r')
      end;
   writeln(' for Postscript');
   writeln('%%Pages: 1');
   writeln('%%EndComments');
   (* writeln('%%EndProlog'); *)
   writeln('%%Page: 1 1');
   (* writeln('userdict begin'); *)
   writeln('/DpicDict 100 dict def DpicDict begin');
   (* writeln('/setlinewid {0 exch 72.27 div 72 mul');
   writeln(' dtransform truncate idtransform setlinewidth pop} def'); *)
   (* write('/setlineparms { [] 0 setdash 0 setlinecap 1 setlinejoin'); *)
   write('/setlineparms { [] 0 setdash 0 setlinecap 0 setlinejoin');
   writeln(' 10 setmiterlimit} def');
   writeln('/ostroke { stroke setlineparms } def');
   writeln('/endstroke { ostroke } def /npath { newpath } def');
   writeln(' setlineparms');
   if (printstate mod 2) = 1 then begin
     if drawmode = PS then begin
        writeln('/strsiz');
        write  (' {newpath 0 0 moveto true charpath flattenpath');
        writeln(' pathbbox 4 1 roll pop pop pop}def');
        writeln('/setcapht {gsave (I) strsiz /capht exch def grestore} def');
        writeln(
      '/postext {1 add baselineskip mul 1 sub 2 div capht mul add moveto} def');
        writeln('/strwidth { dup stringwidth pop } def');
        writeln('/ljust { labelsep } def');
        writeln('/rjust { strwidth labelsep add neg } def');
        writeln('/cjust { strwidth 2 div neg } def');
        writeln('/above { capht 2 div labelsep add } def');
        writeln('/below { capht 2 div neg labelsep sub } def');
        writeln('/vjust {0 capht baselineskip mul neg rmoveto');
        writeln(' currentpoint /y exch def /x exch def} def')
        end
     else begin
        write  ('/ljust { labelsep } def /rjust { labelsep neg } def ');
        writeln('/cjust { 0 } def');
        write  ('/above { labelsep } def /below { labelsep neg } def ')
        end;
     writeln('/vcenter { 0 } def');
     writeln('/basefont {/Times-Roman findfont} def');
     writeln(' basefont 11 scalefont setfont');
     writeln('/labelsep 2 def');
     writeln('/baselineskip 1.4 def')
     end;
   end;

procedure pspostlude;
begin
   writeln('showpage end');
   writeln('%%EOF')
   (*D; if debuglevel > 0 then writeln(log,'pspostlude done');D*)
   end;

procedure psnewpath;
begin
  writeln('npath');
  end;

procedure pswcoord(var iou: text; x,y: real);
begin
   pswfloat(iou,x/fsc);
   pswfloat(iou,y/fsc)
   (*DGHMF ;flush(iou) FMHGD*)
   end;

procedure pswpos(pos: postype);
begin
   pswcoord(output, pos.xpos,pos.ypos )
   end;

procedure pswstring( p: strptr );
var i: integer;
   c: char;
   waswhite,iswhite: boolean;
begin
   waswhite := false;
   if p<>nil then with p@ do if segmnt<>nil then for i:= 0 to len-1 do begin
      c := segmnt@[seginx+i];
      iswhite := (c in [' ',tabch,nlch,etxch]);
      if (not iswhite) or (not waswhite) then begin
         if c in ['(',')',bslch] then write(bslch);
         write(c) end;
      waswhite := iswhite
      end
   end;

procedure pswtext( np: primitivep; tp: strptr; x,y: real );
var i: integer;
   tx: strptr;
   L,R,A,B: boolean;
   toff,theight: real;
begin
  if (tp <> nil) and (drawmode = PS) then begin
     tx := tp; i := 0;
     repeat i := i+1; tx := tx@.next until tx = nil;
     write(' setcapht');
     pswcoord(output,x,y); writeln(' ',i:1,' postext');
     repeat
        write(' vjust (');
        checkjust(tp, A,B,L,R );
        pswstring( tp );
        writeln(')');
        if L then write(' ljust') else if R then write(' rjust')
        else write(' cjust');
        if A then write(' above') else if B then write(' below')
        else write(' vcenter');
        writeln(' rmoveto show x y moveto');
        tp := tp@.next
     until tp = nil
     end
  else if tp <> nil then begin
     toff := venv(np,XLtextoffset)/scale*72;
     theight := venv(np,XLtextht)/scale*72;
     write('(\\tex[');
     checkjust(tp, A,B,L,R );
     if L then write('l') else if R then write('r') else write('c');
     if A then write('b') else if B then write('t') else write('c');
     write('][lB]');
     if theight<>0 then begin
        write('['); wfloat(output,theight/11); write(']')
        end;
     write('{');
     if tp@.next <> nil then write('\\shortstack{');
     tx := tp;
     repeat
        if L then begin
           write('\\rlap{\\hbox to '); wfloat(output,toff); write('bp{}')
           end
        else if R then write('\\llap{');
        pswstring( tx );
        if R then begin
           write('\\hbox to '); wfloat(output,toff); write('bp{}')
           end;
        if L or R then write('}');
        tx := tx@.next;
        if tx <> nil then begin write('\\\\ '); checkjust(tx, A,B,L,R ) end
     until tx = nil;
     if tp@.next <> nil then writeln('}})') else writeln('})');
     pswcoord(output,x,y); write(' moveto');
     checkjust(tp, A,B,L,R );
     if L then write(' ljust') else if R then write(' rjust')
     else write(' cjust');
     if A then write(' above') else if B then write(' below')
     else write(' vcenter');
     writeln(' rmoveto show')
     end
  end;

procedure pswprop( p1,p2: postype; a,b,c: real );
begin
   pprop( p1,p2, a,b,c );
   pswpos( p2 )
   end;

procedure pssetthick(lthk: real);
begin
   if (lthk >= 0.0) and (lthk <> gslinethick) then begin
      pswfloat(output,lthk); writeln(' setlinewidth');
      gslinethick := lthk
	  end
   end;

procedure pslinearfill( f: real; ss: strptr );
begin
   if (f>=0.0) and (f<=1.0) then begin
      write(' currentrgbcolor');
      pswfloat(output,f); writeln(' setgray fill setrgbcolor')
      end
   else if ss<>nil then begin
      write(' currentrgbcolor');
      write(' '); wstring(output,ss); writeln(' setrgbcolor fill setrgbcolor')
      end
   end;

procedure pssetcolor(op: strptr);
begin
   if op<>nil then begin
      write(' currentrgbcolor '); wstring(output,op); writeln(' setrgbcolor')
      end
   end;

procedure psdashdot( lspec: integer; param: real );
begin
   if lspec = XLdashed then begin 
      if ismdistmax(param) then param := 3*fsc;
      write(' [');
      pswfloat(output,param/fsc); pswfloat(output,param/fsc);
      writeln(' ] 0 setdash')
      end
   else if lspec = XLdotted then begin
      if ismdistmax(param) then param := 5*fsc;
      write(' [ 0'); pswfloat(output,param/fsc);
      writeln(' ] 0 setdash 1 setlinecap')
      end
   end;

procedure psendline(op: strptr);
begin
   write(' endstroke');
   if op<>nil then writeln(' setrgbcolor') else writeln
   end;

procedure pswarc( C,S,E: postype; r,ccw: real );
var y: real;
begin
   pswpos(C); pswfloat(output,r/fsc);
   y := datan(S.ypos-C.ypos,S.xpos-C.xpos);
   pswfloat(output,(180.0/pi)*y);
   y := datan(E.ypos-C.ypos,E.xpos-C.xpos);
   pswfloat(output,(180.0/pi)*y);
   if ccw >= 0.0 then writeln(' arc') else writeln(' arcn')
   end;

procedure psarcahead(C: postype; atyp: integer;
   var point: postype;
       ht,wid,lth,radius,angle: real );
var 
   P,Q,Co,Ci,Px,Cox,Cix,Ao,Ai: postype;
   ccw,lwi: real;
   startarrow: boolean;
begin
   arcahead( C,point,atyp, ht,wid,lth,radius,angle,
      P,Co,Ci,Px,Cox,Cix,Ao,Ai, ccw,lwi, startarrow );
   radius := abs(radius);
                                 (* Trace arrowhead outline *)
   psnewpath;
   pswarc(Ci,Ai,point,radius,-ccw);
   pswarc(Co,point,Ao,radius,ccw);
   if (atyp=0) and (lwi < (wid-lwi)/2) then begin
      Q := Co; pprop(Ao,Q,radius-lwi,lwi,radius);
      pswpos(Q); writeln(' lineto');
      pswarc(Co,Q,P,radius-lwi,-ccw);
      Q := Ci; pprop(Ai,Q,radius+lwi,-lwi,radius);
      pswarc(Ci,P,Q,radius+lwi,ccw)
      end;
   if (atyp=3) and (lwi < (wid-lwi)/2) then begin
      pswarc(Cox,Ao,Px,radius,-ccw);
      pswarc(Cix,Px,Ai,radius,ccw)
      end
   else begin pswpos(Ai); writeln(' lineto') end;
   writeln(' fill');
   point := P
   end;

procedure psahead(atyp: integer;
   var point: postype;
       shaft: postype;
       ht,wid,lth: real );
var 
   P,L,R,Px,Lx,Rx,Q: postype;
   x,y: real;
begin
   dahead( point,shaft,ht,wid,lth/72*scale,P,L,R,Px,Lx,Rx,Q,x,y );
   psnewpath;
   if atyp=0 then begin
      pswprop(P,R,x-y,y,x); write(' moveto');
      pswpos(P); writeln(' lineto');
      pswprop(P,L,x-y,y,x); write(' lineto');
      psendline(nil)
      end
   else begin
      pswpos(Rx); writeln(' moveto');
      pswpos(point); write(' lineto');
      pswpos(Lx); writeln(' lineto');
      if atyp=3 then begin pswpos(Px); writeln(' lineto') end;
      writeln(' closepath fill')
      end;
   point := P
   end;

procedure psbox( aat: postype; halfwid,halfht,rad: real);
var i: integer;
   corner: array[0..3] of postype;
begin
   psnewpath;
   corner[0].xpos := aat.xpos+halfwid; corner[0].ypos := aat.ypos+halfht;
   corner[1].xpos := aat.xpos-halfwid; corner[1].ypos := aat.ypos+halfht;
   corner[2].xpos := aat.xpos-halfwid; corner[2].ypos := aat.ypos-halfht;
   corner[3].xpos := aat.xpos+halfwid; corner[3].ypos := aat.ypos-halfht;
   pswcoord(output,aat.xpos+halfwid,aat.ypos); writeln(' moveto 0 setlinejoin');
   rad := Min(Min(abs(rad),abs(halfht)),abs(halfwid));
   if rad = 0.0 then for i:= 0 to 3 do begin
      pswpos(corner[i]); write(' lineto');
      if (i=1) or (i=3) then writeln
      end
   else for i:= 0 to 3 do begin
      pswpos(corner[i]); pswpos(corner[(i+1) mod 4]);
      pswfloat(output,rad/fsc); writeln(' arcto 4 {pop} repeat')
      end;
   write(' closepath')
   end;

procedure pscircle(rd: real);
begin
   psnewpath;
   write(' 0 0'); pswfloat(output,abs(rd)/fsc); write(' 0 360 arc closepath')
   end;

procedure psellipse(x,y: real);
begin
   x := abs(x)/2; y := abs(y)/2;
   psnewpath;
   pswcoord(output,x,0.0); writeln(' moveto');
   pswcoord(output,x,y*SPLT); 
   pswcoord(output,x*SPLT,y); 
   pswcoord(output,0.0,y); writeln(' curveto'); 
   pswcoord(output,-x*SPLT,y); 
   pswcoord(output,-x,y*SPLT); 
   pswcoord(output,-x,0.0); writeln(' curveto'); 
   pswcoord(output,-x,-y*SPLT); 
   pswcoord(output,-x*SPLT,-y); 
   pswcoord(output,0.0,-y); writeln(' curveto'); 
   pswcoord(output,x*SPLT,-y); 
   pswcoord(output,x,-y*SPLT); 
   pswcoord(output,x,0.0); writeln(' curveto closepath'); 
   end;

procedure pssplinesegment(tv: primitivep; splc,splt: integer);
begin
   if tv<>nil then with tv@ do if splt = 1 then begin
      pswpos( aat ); write(' moveto'); pswpos( endpos ); writeln(' lineto')
      end
   else if ismdistmax(aradius) then begin
      if (splc = splt) and (splc > 1) then begin (* 1st seg *)
         pswpos( aat ); writeln(' moveto');
         pswprop( aat,endpos, 1.0,1.0,2.0 ); writeln(' lineto');
         pswprop( aat,endpos, 1.0,5.0,6.0 )
         end
      else if splc > 1 then begin (* interior segment *)
         pswprop( aat,endpos, 5.0,1.0,6.0 );
         pswprop( aat,endpos, 1.0,1.0,2.0 ); writeln(' curveto');
         pswprop( aat,endpos, 1.0,5.0,6.0 )
         end
      else begin    (* last segment *)
         pswprop( aat,endpos, 5.0,1.0,6.0 );
         pswprop( aat,endpos, 1.0,1.0,2.0 ); writeln(' curveto');
         pswpos( endpos ); writeln(' lineto')
         end
      end
   else if (splc = splt) and (splc > 1) then begin
      pswpos( aat ); writeln(' moveto');
      pswprop( aat,endpos, 1-aradius,aradius,1.0 )
      end
   else if splc > 1 then begin
      pswprop(aat,endpos, 1+aradius,1-aradius,2.0 );
      pswprop(aat,endpos, 1.0,1.0,2.0 ); writeln(' curveto');
      pswprop(aat,endpos, 1-aradius,1+aradius,2.0 )
      end
   else begin
      pswprop(aat,endpos, aradius,1-aradius,1.0 );
      pswpos( endpos ); writeln(' curveto')
      end
   end;
                                (* node is always <> nil *)
procedure psdraw( node: primitivep );
var lsp: integer;
    X1,X2: postype;
    tn,tx: primitivep;
    h,w,lth,fill: real;
begin
   getlinespec(node,lsp,tn);
   with node@ do begin
      lth := qenv(node,XLlinethick,lthick);
      (*D if debuglevel > 0 then begin write(log,'psdraw: ');
         snaptype(log,ptype); D*) (* printobject(node); *) (*D
         write(log,' lth='); wfloat(log,lth);
         writeln(log)
         end;
      if linesignal > 0 then begin write(errout,'psdraw: ');
         snaptype(errout,ptype); writeln(errout) end D*)
      end;
   with node@ do case ptype of
      XLbox: begin
         if ((boxfill >= 0.0) and (boxfill <= 1.0)) or (shadedp<>nil) then begin
            psbox(aat,boxwidth/2,boxheight/2,boxradius);
            pssetthick(lth);
            if lsp<>XLinvis then writeln(' gsave');
            if shadedp=nil then begin
               write(' currentrgbcolor');
               pswfloat(output,boxfill); write(' setgray') end
            else pssetcolor(shadedp);
            write(' fill');
            if lsp<>XLinvis then writeln(' grestore') else writeln;
            psdashdot(lsp,lparam); pssetcolor(outlinep);
            psendline(outlinep);
            write(' setrgbcolor');
            writeln(' setlineparms')
            end
         else if lsp<>XLinvis then begin
            psbox(aat,boxwidth/2,boxheight/2,boxradius);
            pssetthick(lth);
            psdashdot(lsp,lparam); pssetcolor(outlinep);
            psendline(outlinep); writeln(' setlineparms')
            end;
         pswtext( node,textp, aat.xpos, aat.ypos )
         end;
      XBLOCK: pswtext( node,textp, aat.xpos, aat.ypos );
      XLellipse,XLcircle: begin
         if ptype = XLellipse then fill := efill else fill := cfill;
         if ((fill >= 0.0) and (fill <= 1.0)) or (shadedp<>nil) then begin
            pssetthick(lth);
            write(' gsave '); pswpos(aat); writeln(' translate');
            if ptype = XLellipse then psellipse(elwidth,elheight)
            else pscircle(radius);
            write(' gsave ');
            if shadedp=nil then begin
                pswfloat(output,fill); write(' setgray') end
            else begin
                wstring(output,shadedp); write(' setrgbcolor') end;
            writeln(' fill grestore');
            if lsp<>XLinvis then begin
               psdashdot(lsp,lparam);
               pssetcolor(outlinep); psendline(outlinep);
               writeln(' setlineparms')
               end;
            writeln(' grestore')
            end
         else if lsp<>XLinvis then begin
            pssetthick(lth);
            write(' gsave '); pswpos(aat); writeln(' translate');
            if ptype = XLellipse then psellipse(elwidth,elheight)
            else pscircle(radius);
            psdashdot(lsp,lparam);
            pssetcolor(outlinep); psendline(outlinep);
            writeln(' setlineparms');
            writeln(' grestore')
            end;
         pswtext( node,textp,aat.xpos,aat.ypos )
         end;
      XLline,XLarrow,XLspline: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               psnewpath;
               if ptype<>XLspline then begin
                  pswpos(aat); writeln(' moveto') end;
               spltot := primdepth( node ); splcount := spltot; tx := node;
               while tx<>nil do begin
                  if ptype=XLspline then begin
                     pssplinesegment(tx,splcount,spltot);
                     splcount := splcount - 1 end
                  else begin
                     pswpos( tx@.endpos ); writeln(' lineto') end;
                  tx := tx@.son
                  end;
               pssetthick(0); pslinearfill(vfill,sshade);
               vfill := -1; sshade := nil
               end;
            if lsp<>XLinvis then begin
               lth := qenv(tn,XLlinethick,tn@.lthick);
               spltot := primdepth( node ); splcount := spltot;
               pssetthick(lth);
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                  pssetcolor(soutline);
                  psahead(ahnum(tn@.atype),aat,endpos,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth);
                  if soutline<>nil then writeln(' setrgbcolor')
                  end;
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                  pssetcolor(soutline);
                  psahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth);
                  if soutline<>nil then writeln(' setrgbcolor')
                  end;
               if ptype<>XLspline then begin
                  psnewpath;
                  pswpos(aat); writeln(' moveto')
                  end
               end
            end;
         if lsp<>XLinvis then begin
            if ptype=XLspline then pssplinesegment(node,splcount,spltot)
            else begin pswpos( endpos ); writeln(' lineto') end;
            if son=nil then begin
               psdashdot(lsp,lparam);
               pssetcolor(soutline);
               psendline(soutline);
               end;
            end;
         splcount := splcount - 1;
         if son=nil then while snode<>nil do with snode@ do begin
            if textp<>nil then pswtext( node,textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLmove: begin
         if firstsegment(node) then snode := node;
         if son=nil then while snode<>nil do with snode@ do begin
            if textp<>nil then pswtext( node,textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLarc: begin
         if drawn(node,lsp,lfill) then begin
            pssetthick(lth);
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            X1 := arcstart(node); X2 := arcend(node);
            if bfill then begin
               writeln(' currentrgbcolor');
               psnewpath;
               pswarc(aat,X1,X2,aradius,|arcangle|);
               pssetthick(0);
               pslinearfill(vfill,sshade); vfill := -1; sshade := nil;
               writeln(' setrgbcolor')
               end;
            if lsp<>XLinvis then begin
               pssetthick(lth);
               if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                  pssetcolor(soutline);
                  startarc(node,X1,lth,h,w);
                  psarcahead(aat,ahnum(atype),X1,
                     h,w,lth,abs(aradius),|arcangle|);
                  if soutline<>nil then writeln(' setrgbcolor')
                  end;
               if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                  pssetcolor(soutline);
                  endarc(node,X2,lth,h,w);
                  psarcahead(aat,ahnum(atype),X2,
                     h,w,lth,-abs(aradius),|arcangle|);
                  if soutline<>nil then writeln(' setrgbcolor')
                  end;
               psnewpath;
               pswarc(aat,X1,X2,aradius,|arcangle|);
               psdashdot(lsp,lparam);
               pssetcolor(soutline);
               psendline(soutline)
               end;
            writeln(' setlineparms')
            end;
         pswtext( node,textp,aat.xpos,aat.ypos )
         end;
      XLstring: pswtext( node,textp,aat.xpos,aat.ypos );
      XLaTeX: if textp <> nil then begin
            wstring(output, textp );
            writeln
            end
         else begin
            if lthick >= 0.0 then pssetthick(lthick);
            end;
      otherwise
      end
   (*DGHMF ;flush(output) FMHGD*)
   end; (* psdraw *)
