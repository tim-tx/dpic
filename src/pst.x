(* pst.x *)
(* Output routines for PSTricks *)
procedure pstprelude( n,s,e,w: real );
begin
   writeln('\psset{unit=1in,cornersize=absolute,dimen=middle}%');
   write('\begin{pspicture}');
   wcoord(output,w,s);
   wcoord(output,e,n);
   writeln('%');
   writeln('% VERSIONDATE option -p for PSTricks 0.93a or later')
   end;

procedure pstpostlude;
begin
   writeln('\end{pspicture}%');
   (*D if debuglevel > 0 then writeln(log,'pstpostlude done'); D*)
   end;

procedure pstwrtext(np: primitivep; tp: strptr; x,y: real );
var A,B,L,R: boolean;
begin
   if tp = nil then begin end
   else if tp@.next <> nil then begin
      write('\rput');
      wcoord(output,x,y);
      write( '{'); texstacktext( np,tp ); writeln('}' )
      end
   else begin
      checkjust(tp,A,B,L,R);
      if A or B or L or R then begin
         write('\uput{'); wfloat(output,venv(np,XLtextoffset)/scale*72);
         write('bp}[');
         if B then write('d') else if A then write('u');
         if L then write('r') else if R then write('l');
         write(']')
         end
      else write('\rput');
      wcoord(output,x,y);
      write( '{'); wstring( output,tp ); writeln('}' )
      end
   end;

procedure pstlineoptions( node: primitivep; lsp: integer; sep: char );
begin
   with node@ do begin
      if lthick >= 0.0 then begin
         write(sep,'linewidth='); wfloat(output,lthick); write('pt');
         sep := ',' end;
      if outlinep<>nil then begin
         write(sep,'linecolor=');
         wstring(output, outlinep );
         sep := ',' end;
      if ptype = XBLOCK then lsp := XLinvis;
      if lsp in [(* XLsolid,*) XLdashed,XLdotted,XLinvis] then begin
         writeln(sep,'%');
         write('linestyle='); case lsp of
            (* XLsolid: write('solid'); *)   (* solid is the default *)
            XLdashed: begin write('dashed');
               if lparam >= 0 then begin
                  write(',dash=');
                  wfloat(output,lparam/fsc); write(' ');
                  wfloat(output,lparam/fsc)
                  end 
               end;
            XLdotted: begin write('dotted');
               if lparam >= 0 then begin
                  write(',dotsep='); wfloat(output,lparam/fsc)
                  end
               end;
            XLinvis: write('none') end;
         sep := ','
         end;
      if (ptype in [XLarrow,XLline,XLspline]) then begin
         if (ahlex(atype) <> XEMPTY) and
            (ahnum(atype) <> 0) and (ahnum(atype) <> 3) then begin
            writeln(sep,'%');
            write('arrowsize='); wfloat(output,width/scale);
            write('in 0,arrowlength='); 
            if width=0.0 then wfloat(output,0.0)
            else wfloat(output,height/width);
            write(',arrowinset=0'); sep := ','
            end
         end;
      if sep = ',' then write(']');
      end
   end;

procedure pstfilloptions(node: primitivep; lsp: integer; a: real);
var sep: char;
   fill: real;
begin
   fill := -1.0;
   with node@ do begin
      case ptype of
         XBLOCK: ;
         XLbox: fill := boxfill;
         XLcircle: fill := cfill;
         XLellipse: fill := efill;
         XLarc,XLline,XLarrow,XLmove,XLspline: fill := lfill;
         otherwise
         end;
      fill := round(fill*1000000)/1000000;
      if (fill > 0.0) and (fill < 1.0) and (fill <> lastfillval) and
         (fill <> 0.25) and (fill <> 0.5) and (fill <> 0.75) then begin
         lastfillval := fill;
         write('\newgray{fillval}'); wbrace(fill); writeln
         end;
      case ptype of
         XLbox,XBLOCK: write('\psframe');
         XLellipse: write('\psellipse');
         XLcircle: write('\pscircle');
         XLspline: write('\psbezier');
         XLline,XLarrow: write('\psline');
         XLarc: if a >= 0.0 then write('\psarc') else write('\psarcn');
         otherwise
         end;
      if sshade<>nil then begin
         write('[fillstyle=solid,fillcolor=');
         wstring(output, sshade );
         sep := ',' end
      else if (fill >= 0.0) and (fill <= 1.0) then begin
         write('[fillstyle=solid,fillcolor=');
         if fill = 0.0 then write('black')
         else if fill = 0.25 then write('darkgray')
         else if fill = 0.5  then write('gray')
         else if fill = 0.75 then write('lightgray')
         else if fill = 1.0  then write('white')
         else write('fillval');
         sep := ',' end
      else sep := '[';
      if ptype <> XLbox then begin end
      else if boxradius <> 0.0 then begin
         write(sep,'linearc='); wfloat(output,boxradius/fsc);
         sep := ','
         end;
      pstlineoptions(node,lsp,sep)
      end
   end;

procedure pstahead(var point: postype; shaft: postype; ht,wid,lth: real;
   typ: integer; sou: strptr );
var
   P,L,R,Px,Lx,Rx,Q: postype;
   x,y: real;
begin
   dahead( point,shaft,ht,wid,lth/72*scale,P,L,R,Px,Lx,Rx,Q,x,y );
   (*D if debuglevel > 0 then begin
       write(log,' dahead out: point:');
       wcoord(log,point.xpos,point.ypos);
       write(log,';shaft:'); wcoord(log,shaft.xpos,shaft.ypos); writeln(log);
       write(log,' ht='); wfloat(log,ht);
       write(log,' wid='); wfloat(log,wid);
       write(log,' lth='); wfloat(log,lth);
       write(log,' typ=',typ:0,' ');
       write(log,' fsc='); wfloat(log,fsc); writeln(log);
       write(log,' P:'); wcoord(log,P.xpos,P.ypos);
       write(log,';L:'); wcoord(log,L.xpos,L.ypos);
       write(log,';R:'); wcoord(log,R.xpos,R.ypos);
       write(log,';Q:'); wcoord(log,Q.xpos,Q.ypos); writeln(log);
       write(log,' Px:'); wcoord(log,Px.xpos,Px.ypos);
       write(log,';Lx:'); wcoord(log,Lx.xpos,Lx.ypos);
       write(log,';Rx:'); wcoord(log,Rx.xpos,Rx.ypos); writeln(log);
       write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
       writeln(log) end; D*)
   if typ = 0 then begin
      write('\pscustom[linewidth='); wfloat(output,lth);
      write('pt,linecolor=');
      if sou<>nil then wstring(output,sou) else write('black');
      writeln(']{%');
      write('\psline');
      wprop(P,L,x-y,y,x); wpos(P); wprop(P,R,x-y,y,x); writeln('}') end
   else begin
      write('\pscustom[linewidth=0pt,fillstyle=solid,fillcolor=');
      if sou<>nil then wstring(output,sou) else write('black');
      write(',linecolor=');
      if sou<>nil then wstring(output,sou) else write('black');
      writeln(']{%');
      write('\psline');
      wpos(Px); wpos(Rx); wpos(point); wpos(Lx);
      writeln('\closepath}%')
      end;
   point := P
   end;

procedure pstwarc( C: postype; r,startangle,endangle,ccw: real );
begin
   wpos(C);
   if (ccw > 0) and (endangle < startangle) then
      endangle := endangle + 2*pi 
   else if (ccw < 0) and (endangle > startangle) then
      endangle := endangle - 2*pi;
   wbrace(r/fsc); wbrace((180.0/pi)*startangle); wbrace((180.0/pi)*endangle);
   writeln
   (*D; if debuglevel > 0 then begin
      write(log,' pstwarc:');
      logpos(' C',C);
      write(log,' r='); wfloat(log,r);
      write(log,' startangle='); wfloat(log,startangle*180/pi);
      write(log,' endangle='); wfloat(log,endangle*180/pi);
      write(log,' ccw='); wfloat(log,ccw);
      writeln(log)
      end D*)
   end;

procedure pstarc( ccw: real );
begin
   if ccw >= 0.0 then write('\psarc') else write('\psarcn')
   end;

procedure pstarcahead(C,point: postype; atyp: integer; sou: strptr;
       ht,wid,lth,radius,arcangle: real;
   var P: postype );
var 
   Co,Ci,Px,Cox,Cix,Ao,Ai: postype;
   ccw,lwi: real;
   startarrow: boolean;
begin
   (*D; if debuglevel > 0 then writeln(log,' pstarcahead:'); D*)
   arcahead( C,point,atyp, ht,wid,lth,radius,arcangle,
      P,Co,Ci,Px,Cox,Cix,Ao,Ai, ccw,lwi, startarrow );
   radius := abs(radius);
   write('\pscustom[linewidth=0pt,fillstyle=solid,fillcolor=');
   if sou<>nil then wstring(output,sou) else write('black');
   write(',linecolor=');
   if sou<>nil then wstring(output,sou) else write('black');
   writeln(']{%');
   pstarc(-ccw);
   pstwarc( Ci,radius,posangle(Ai,Ci),posangle(point,Ci),-ccw );
   pstarc( ccw);
   pstwarc( Co,radius,posangle(point,Co),posangle(Ao,Co), ccw);
   if (atyp=0) and (lwi < (wid-lwi)/2) then begin
      pstarc(-ccw);
      pstwarc( Co,radius-lwi,posangle(Ao,Co),posangle(P,Co),-ccw);
      pstarc( ccw);
      pstwarc( Ci,radius+lwi,posangle(P,Ci),posangle(Ai,Ci), ccw)
      end
   else if (atyp=3) and (lwi < (wid-lwi)/2) then begin
      pstarc(-ccw);
      pstwarc( Cox,radius,posangle(Ao,Cox),posangle(Px,Cox),-ccw);
      pstarc( ccw);
      pstwarc( Cix,radius,posangle(Px,Cix),posangle(Ai,Cix), ccw)
      end;
   writeln('\closepath}%')
   (*D; if debuglevel > 0 then begin
      write(log,' <Ai,Ci '); wfloat(log,posangle(Ai,Ci)*180/pi);
      write(log,' <point,Ci '); wfloat(log,posangle(point,Ci)*180/pi);
      write(log,' <point,Co '); wfloat(log,posangle(point,Co)*180/pi);
      write(log,' <Ao,Co '); wfloat(log,posangle(Ao,Co)*180/pi);
      writeln(log);
      write(log,' <point '); wfloat(log,posangle(point,C)*180/pi);
      write(log,' <P '); wfloat(log,posangle(P,C)*180/pi);
      write(log,' <Ao '); wfloat(log,posangle(Ao,C)*180/pi);
      write(log,' <Ai '); wfloat(log,posangle(Ai,C)*180/pi);
      write(log,' <arcangle '); wfloat(log,arcangle*180/pi);
      writeln(log)
      end D*)
   end;

procedure pstsplinesegment(tv: primitivep; splc,splt: integer);
begin
   if tv<>nil then with tv@ do if ismdistmax(aradius) then begin
      if (splc = splt) and (splc > 1) then begin (* 1st seg *)
         wpos( aat );
         wprop( aat,endpos, 5.0,1.0,6.0 );  (* 1/6 from 1st to 2nd *)
         wprop( aat,endpos, 4.0,2.0,6.0 );  (* 1/3 from 1st to 2nd *)
         wprop( aat,endpos, 1.0,1.0,2.0 );  (* 1/2 from 1st to 2nd *)
         wprop( aat,endpos, 1.0,5.0,6.0 )   (* 5/6 from 1st to 2nd *)
         end
      else if splc > 1 then begin (* interior segment *)
         wprop( aat,endpos, 5.0,1.0,6.0 );   (* 1/6 from 1st to 2nd *)
         wprop( aat,endpos, 1.0,1.0,2.0 );   (* 1/2 from 1st to 2nd *)
         wprop( aat,endpos, 1.0,5.0,6.0 )    (* 5/6 from 1st to 2nd *)
         end
      else begin    (* last segment *)
         wprop( aat,endpos, 5.0,1.0,6.0 );  (* 1/6 along *)
         wprop( aat,endpos, 1.0,1.0,2.0 );  (* 1/2 along *)
         wprop( aat,endpos, 2.0,4.0,6.0 );  (* 2/3 along *)
         wprop( aat,endpos, 1.0,5.0,6.0 );  (* 5/6 along *)
         wpos( endpos )
         end
      end
   else if (splc = splt) and (splc > 1) then begin
      wpos(aat);
      wprop( aat,endpos, 1-aradius,aradius,1.0 )
      end
   else if splc > 1 then begin
      wprop( aat,endpos, 1+aradius,1-aradius,2.0 );
      wprop( aat,endpos, 1.0,1.0,2.0 );
      wprop( aat,endpos, 1-aradius,1+aradius,2.0 )
      end
   else if splt = 1 then begin
      wprop( aat,endpos, aradius,1-aradius,1.0 );
      wprop( aat,endpos, 1-aradius,aradius,1.0 );
      wpos(endpos)
      end
   else begin
      wprop( aat,endpos, aradius,1-aradius,1.0 );
      wpos(endpos)
      end;
   writeln
   end;

procedure pstdraw( node: primitivep );
var lsp,ahn: integer;
   X0,X1: postype;
   v: boolean;
   s,c,lth: real;
   tn,tx: primitivep;
   sx: strptr;
begin
   getlinespec(node,lsp,tn);
   with node@ do begin
      sshade := shadedp;
      lth := qenv(node,XLlinethick,tn@.lthick);
      (*D if debuglevel > 0 then begin
         write(log,'pstdraw[',ordp(node):1,']: ');
         printobject(node); writeln(log) end;
      if linesignal > 0 then begin write(errout,'pstdraw: ');
         snaptype(errout,ptype); writeln(errout) end D*)
      end;
   with node@ do case ptype of
      XLbox,XBLOCK: begin
         initnesw; nesw( node );
         if ptype=XBLOCK then v := drawn(node,lsp,-1.0)
         else v := drawn(node,lsp,boxfill);
         if v then begin
            pstfilloptions(node,lsp,0.0);
            wcoord(output,west,south);
            wcoord(output,east,north);
            writeln
            end;
         pstwrtext(node,textp, aat.xpos, aat.ypos )
         end;
      XLellipse: begin
         if drawn(node,lsp,efill) then begin
            pstfilloptions(node,lsp,0.0);
            wpos( aat );
            wcoord(output, 0.5*elwidth,0.5*elheight );
            writeln
            end;
         pstwrtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLcircle: begin
         if drawn(node,lsp,cfill) then begin
            pstfilloptions(node,lsp,0.0);
            wpos( aat );
            write('{'); wfloat(output,radius/fsc); writeln('}')
            end;
         pstwrtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLarc: begin
         if drawn(node,lsp,lfill) then begin
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill and (vfill >= 0.0) then lfill := vfill;
            X0 := arcstart(node); X1 := arcend(node);
            if bfill then begin
               s := lthick; lthick := 0.0;
               pstfilloptions(node,lsp,|arcangle|);
               pstwarc( aat,abs(aradius),posangle(X0,aat),posangle(X1,aat),
                  |arcangle| );
               lthick := s
               end;
            if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
               pstarcahead( aat,X0,ahnum(atype),soutline,
                  qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                  lth, abs(aradius),|arcangle|,X0 )
               end;
            if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
               pstarcahead( aat,X1,ahnum(atype),soutline,
                  qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                  lth,-abs(aradius),|arcangle|,X1 )
               end;
            sx := sshade; sshade := nil; c := lfill; lfill := -1;
            pstfilloptions(node,lsp,|arcangle|);
            sshade := sx; lfill := c;
            pstwarc( aat,abs(aradius),posangle(X0,aat),posangle(X1,aat),
               |arcangle| )
            end;
         pstwrtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLline,XLarrow: begin
         if firstsegment(node) then snode := node;
         if drawn(node,lsp,lfill) then begin
            if firstsegment(node) then begin
               getlinshade(node,tn,sshade,soutline,vfill,bfill);
               if bfill and (vfill >= 0.0) then lfill := vfill;
               if bfill then begin
                  s := tn@.lthick; tn@.lthick := 0.0;
                  pstfilloptions(tn,lsp,0.0);
                  tn@.lthick := s;
                  wpos( aat );
                  tx := node;
                  while tx<>nil do begin wpos(tx@.endpos); writeln;
                     tx := tx@.son end
                  end;
               ahn := ahnum(tn@.atype);
               if (ahn=0) or (ahn=3) then begin
                  if (ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD]) then
                     pstahead(aat,endpos,qenv(node,XLarrowht,tn@.height),
                                         qenv(node,XLarrowwid,tn@.width),
                                         qenv(node,XLlinethick,tn@.lthick),
                                         ahnum(tn@.atype),soutline);
                  if (ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD]) then
                    pstahead(tn@.endpos,tn@.aat,qenv(node,XLarrowht,tn@.height),
                                         qenv(node,XLarrowwid,tn@.width),
                                         qenv(node,XLlinethick,tn@.lthick),
                                         ahnum(tn@.atype),soutline)
                  end;
               sx := sshade; sshade := nil;
               pstfilloptions(tn,lsp,0.0);
               sshade := sx;
               if ((ahn<>0) and (ahn<>3)) then case ahlex(tn@.atype) of
                  XLEFTHEAD: write('{<-}');
                  XDOUBLEHEAD: write('{<','->}');
                  XRIGHTHEAD: write('{->}');
                  otherwise
                  end;
               bfill := false;
               wpos( aat )
               end;
            wpos( endpos ); writeln
            end;
         if son=nil then while snode<>nil do with snode@ do begin
            if textp<>nil then pstwrtext(snode,textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLmove: begin
         if firstsegment(node) then snode := node;
         if son=nil then while snode<>nil do with snode@ do begin
            if textp<>nil then pstwrtext(snode,textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLspline: begin
         if firstsegment(node) then begin         (* first spline *)
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill and (vfill >= 0.0) then lfill := vfill;
            if bfill then begin
               s := tn@.lthick; tn@.lthick := 0.0;
               pstfilloptions(tn,lsp,0.0);
               tn@.lthick := s;
               spltot := primdepth( node ); splcount := spltot;
               writeln('%');
               tx := node;
               while tx<>nil do begin
                  pstsplinesegment(tx, splcount,spltot);
                  splcount := splcount - 1;
                  tx := tx@.son end;
               bfill := false
               end;
            spltot := primdepth( node ); splcount := spltot;
            ahn := ahnum(tn@.atype);
            if ((ahn=0) or (ahn=3)) then begin
               if (ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD]) then
                  pstahead(tn@.endpos,tn@.aat,qenv(node,XLarrowht,tn@.height),
                                      qenv(node,XLarrowwid,tn@.width),
                                      qenv(node,XLlinethick,tn@.lthick),
                                      ahnum(tn@.atype),soutline);
               if (ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD]) then
                  pstahead(aat,endpos,qenv(node,XLarrowht,tn@.height),
                                      qenv(node,XLarrowwid,tn@.width),
                                      qenv(node,XLlinethick,tn@.lthick),
                                      ahnum(tn@.atype),soutline)
               end;
            if lsp<>XLinvis then begin
               sx := sshade; sshade := nil;
               pstfilloptions(tn,lsp,0.0); sshade := sx;
               if ((ahn<>0) and (ahn<>3)) then case ahlex(tn@.atype) of
                  XLEFTHEAD: write('{<-}');
                  XDOUBLEHEAD: write('{<','->}');
                  XRIGHTHEAD: write('{->}');
                  otherwise
                  end;
               writeln('%');
               end
            end;
         if lsp<>XLinvis then pstsplinesegment(node, splcount,spltot);
         splcount := splcount - 1
         end;
      XLstring: pstwrtext(node,textp,aat.xpos,aat.ypos );
      XLaTeX: if textp <> nil then begin
            wstring(output, textp );
            writeln
            end
         else if lthick >= 0.0 then begin
            write('\psset{linewidth='); wfloat(output,lthick);
            writeln('pt}%');
            end;
      otherwise
      end
   (*DGHMF ;flush(output) FMHGD*)
   end; (* pstdraw *)
