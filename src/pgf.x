(* pgf.x *)
(* pgf output routines *)
procedure pgfprelude;
begin
   writeln('\begin{tikzpicture}[scale=2.54]');
   writeln('% VERSIONDATE option -g for TikZ and PGF 1.01');
   writeln('\ifx\dpiclw\undefined\newdimen\dpiclw\fi');
   writeln('\global\def\dpicdraw{\draw[line width=\dpiclw]}');
   writeln('\global\def\dpicstop{;}');
   writeln('\dpiclw=0.8bp')
   end;

procedure pgfpostlude;
begin
   writeln('\end{tikzpicture}')
   (*D; if debuglevel > 0 then writeln(log,'pgfpostlude done') D*)
   end;
                                (* np is always <> nil: *)
procedure pgfwrtext(np: primitivep; tp: strptr; x,y: real );
var A,B,L,R: boolean;
begin
   if tp <> nil then begin
      write('\draw '); wcoord(output,x,y); write(' node');
      if (np@.ptype=XLstring) and (np@.name<>nil) then begin
         write('('); wstring( output,np@.name ); write(')')
         end;
      if tp@.next <> nil then begin
         write('{'); texstacktext( np,tp ) end
      else begin
         checkjust(tp,A,B,L,R);
         if A or B or L or R then begin
            write('['); 
            if      A and L then write('above right')
            else if A and R then write('above left')
            else if B and L then write('below right')
            else if B and R then write('below left')
            else if A then write('above')
            else if B then write('below')
            else if L then write('right')
            else if R then write('left');
            write('=');
                               (* Assume pgf built-in text offset = 4 bp *)
            wfloat(output,(venv(np,XLtextoffset)*72/scale-4.0)/fsc);
            write('bp]');
            end;
         write('{'); wstring( output,tp )
         end;
      writeln('};')
      end
   end;

procedure pgfendpath;
begin
  writeln('\dpicstop')
  end;

procedure pgfsetthick(lthick: real);
begin
   if not ismdistmax(lthick) then begin end
   else if not ismdistmax(gslinethick) then lthick := gslinethick
   else lthick := 0.8;
   (*D if debuglevel > 0 then begin
      writeln(log);
      write(log,'lthick='); wfloat(log,lthick);
      write(log,' gslinethick='); wfloat(log,gslinethick); writeln(log)
      end;D*)
   if (lthick >= 0.0) and (lthick <> gslinethick) then begin
      write('\dpiclw='); wfloat(output,lthick); writeln('bp');
      gslinethick := lthick
	  end
   end;

procedure pgfbox( ctr: postype; halfwid,halfht,rad: real);
var r,h,v: real;
begin
   if rad = 0.0 then begin
     wcoord(output,ctr.xpos-halfwid,ctr.ypos-halfht);
     write(' rectangle ');
     wcoord(output,ctr.xpos+halfwid,ctr.ypos+halfht);
     pgfendpath
     end
   else begin
     r := Min(Min(rad,halfht),halfwid);
     h := halfwid-r; v := halfht-r;
     mpocorner(ctr,h,v,r,1,0);
     mpocorner(ctr,-h,v,r,0,1);
     mpocorner(ctr,-h,-v,r,-1,0);
     mpocorner(ctr,h,-v,r,0,-1);
     write('cycle');
     pgfendpath
     end;
   end;

procedure pgfahead( atyp: integer;
 var point: postype; shaft: postype; ht,wid,lth: real );
var 
   P,L,R,Px,Lx,Rx,Q: postype;
   x,y: real;
begin
   dahead( point,shaft,ht,wid,lth/72*scale,P,L,R,Px,Lx,Rx,Q,x,y );
   (*D if debuglevel > 0 then begin
       write(log,' dahead values: point:');
       wcoord(log,point.xpos,point.ypos);
       write(log,' ;shaft:'); wcoord(log,shaft.xpos,shaft.ypos); writeln(log);
       write(log,' ht='); wfloat(log,ht);
       write(log,' ;wid='); wfloat(log,wid);
       write(log,' ;lth='); wfloat(log,lth);
       write(log,' ;fsc='); wfloat(log,fsc); writeln(log);
       write(log,' P:'); wcoord(log,P.xpos,P.ypos);
       write(log,' ;L:'); wcoord(log,L.xpos,L.ypos);
       write(log,' ;R:'); wcoord(log,R.xpos,R.ypos);
       D*)(* write(log,' ;Q:'); wcoord(log,Q.xpos,Q.ypos); *)(*D
       writeln(log);
       write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
       writeln(log) end; D*)
   if atyp = 0 then begin
      wprop(P,L,x-y,y,x); ddash; wpos(P); ddash; wprop(P,R,x-y,y,x);
      writeln
      end
   else if atyp = 3 then begin
      wpos(Px); ddash; wpos(Rx); ddash; wpos(point); ddash; wpos(Lx);
      write(' --cycle')
      (* wprop(P,L,x-y,y,x); ddash; wpos(P); ddash; wprop(P,R,x-y,y,x) *)
      end
   else begin
      (* wpos(affine(x,0.0,P,Q));
      ddash; wpos(R); ddash; wpos(P); ddash; wpos(L); *)
      wpos(Rx); ddash; wpos(point); ddash; wpos(Lx);
      write(' --cycle')
      end;
   pgfendpath;
   point := P
   end;

procedure pgfstartdraw(initial: integer; node: primitivep; lsp: integer);
var sep: char;
   fill: real;
begin
   if lsp = XLinvis then write('\fill')
   else if initial = 0 then write('\dpicdraw')
   else write('\filldraw'); (* initial = 1 or 3 for filled arrowheads *)
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
      if (shadedp<>nil) or (sshade<>nil) then begin
         write('[fill=');
         if shadedp<>nil then wstring(output, shadedp)
         else wstring(output, sshade);
         sep := ',' end
      else if (fill >= 0.0) and (fill <= 1.0) then begin
         write('[fill=');
         if fill = 0.0 then write('black')
         else if fill = 0.25 then write('darkgray')
         else if fill = 0.5  then write('gray')
         else if fill = 0.75 then write('lightgray')
         else if fill = 1.0  then write('white')
         else begin
            write('white!'); wfloat(output,fill*100); write('!black') end;
         sep := ',' end
      else sep := '[';
      if (initial=1) or (initial=3) then begin
         write(sep,'line width=0bp'); sep := ',' end
      else if (lthick >= 0.0) and (lthick<>gslinethick) then begin
         write(sep,'line width='); wfloat(output,lthick); write('bp');
         sep := ',' end;
      if ((outlinep<>nil) or (soutline<>nil)) and (lsp<>XLinvis) then begin
         write(sep,'draw=');
         if outlinep<>nil then wstring(output, outlinep )
         else wstring(output,soutline);
         if (initial=1) or (initial=3) then begin
            write(',fill=');
            if outlinep<>nil then wstring(output, outlinep )
            else wstring(output,soutline)
            end;
         sep := ','
         end;
      if (lsp = XLdashed) and (initial<>1) and (initial<>3) then begin
         if lparam <= 0 then write(sep,'dashed')
         else begin
            write(sep,'dash pattern=on ');
            wfloat(output,lparam/fsc); write('in off ');
            wfloat(output,lparam/fsc); write('in')
            end;
         sep := ','
         end
      else if lsp = XLdotted then begin
         if lparam <= 0 then write(sep,'dotted')
         else begin
            write(sep,'cap=round,dash pattern=on ');
            write('0pt off ');
            wfloat(output,lparam*72/fsc); write('bp')
            end;
         sep := ','
         end;
      if sep = ',' then write(']') else write(' ');
      end
   end;

procedure pgfarcahead(C,point: postype; atyp: integer; sou: strptr;
       ht,wid,lth,radius,arcangle: real;
   var P: postype );
var 
   Q,Co,Ci,Px,Cox,Cix,Ao,Ai: postype;
   ccw,lwi: real;
   startarrow: boolean;
begin
   (*D; if debuglevel > 0 then writeln(log,' pgfarcahead:'); D*)
   arcahead( C,point,atyp, ht,wid,lth,radius,arcangle,
      P,Co,Ci,Px,Cox,Cix,Ao,Ai, ccw,lwi, startarrow );
   radius := abs(radius);
   write('\filldraw[line width=0bp');
   if sou<>nil then begin
      write(',draw='); wstring(output,soutline);
      write(',fill='); wstring(output,soutline)
      end;
   write(']');
                                 (* Trace arrowhead outline *)
   if (atyp=0) and (lwi < (wid-lwi)/2) then begin
      wpos(P);
      popgwarc( Ci,radius+lwi,posangle(P,Ci),posangle(Ai,Ci),ccw );
      ddash; wpos(Ai);
      popgwarc( Ci,radius,posangle(Ai,Ci),posangle(point,Ci),-ccw );
      popgwarc( Co,radius,posangle(point,Co),posangle(Ao,Co),ccw );
      ddash; wprop(Ao,Co,radius-lwi,lwi,radius);
      popgwarc( Co,radius-lwi,posangle(Ao,Co),posangle(P,Co),-ccw )
      end
   else if (atyp=3) and (lwi < (wid-lwi)/2) then begin
      wpos(Px);
      popgwarc( Cix,radius,posangle(Px,Cix),posangle(Ai,Cix),ccw );
      popgwarc( Ci,radius,posangle(Ai,Ci),posangle(point,Ci),-ccw );
      popgwarc( Co,radius,posangle(point,Co),posangle(Ao,Co),ccw );
      popgwarc( Cox,radius,posangle(Ao,Cox),posangle(Px,Cox),-ccw )
      end
   else begin
      Q.xpos := (Ao.xpos+Ai.xpos)/2; Q.ypos := (Ao.ypos+Ai.ypos)/2;
      wpos(Q); ddash; wpos(Ai);
      popgwarc( Ci,radius,posangle(Ai,Ci),posangle(point,Ci),-ccw );
      popgwarc( Co,radius,posangle(point,Co),posangle(Ao,Co),ccw );
      ddash;
      wpos(Q)
      end;
   pgfendpath
   end;
                                (* node is always <> nil: *)
procedure pgfdraw( node: primitivep );
var lsp: integer;
    X0,X1: postype;
    tn,tx: primitivep;
    s,c,lth: real;
    v: boolean;
    sf,sg: strptr;
begin
   getlinespec(node,lsp,tn);
   lth := qenv(node,XLlinethick,node@.lthick);
   with node@ do case ptype of
      XLbox,XBLOCK: begin
         if ptype=XBLOCK then v := drawn(node,lsp,-1.0)
         else v := drawn(node,lsp,boxfill);
         if v then begin
            pgfstartdraw(0,node,lsp);
            pgfbox(aat,boxwidth/2,boxheight/2,boxradius)
            end;
         pgfwrtext( node,textp, aat.xpos, aat.ypos )
         end;
      XLellipse: begin
         if drawn(node,lsp,efill) then begin
            pgfstartdraw(0,node,lsp);
            wpos( aat );
            write(' ellipse (');
            wfloat(output,elwidth/2/fsc/2.54); write('in and ');
            wfloat(output,elheight/2/fsc/2.54); write('in)');
            pgfendpath
            end;
         pgfwrtext( node,textp,aat.xpos,aat.ypos )
         end;
      XLcircle: begin
         if drawn(node,lsp,cfill) then begin
            pgfstartdraw(0,node,lsp);
            wpos( aat );
            write(' circle (');
            wfloat(output,radius/fsc/2.54); write('in)');
            pgfendpath
            end;
         pgfwrtext( node,textp,aat.xpos,aat.ypos )
         end;
      XLarc: begin
         if drawn(node,lsp,-1.0) then begin
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill and (vfill >= 0.0) then lfill := vfill;
            X0 := arcstart(node); X1 := arcend(node);
            if bfill then begin
               s := lthick; lthick := 0;
               pgfstartdraw(0,node,XLinvis); lthick := s;
               wpos(X0);
               popgwarc( aat,abs(aradius),posangle(X0,aat),posangle(X1,aat),
                  |arcangle| );
               writeln('--cycle'); pgfendpath
               end;
            if lsp<>XLinvis then begin
               if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                  sf := shadedp; shadedp := nil; sg := sshade; sshade:=soutline;
                  pgfarcahead( aat,X0,ahnum(atype),soutline,
                     qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                     lth, abs(aradius),|arcangle|,X0 );
                  shadedp := sf; sshade := sg
                  end;
               if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                  sf := shadedp; shadedp := nil; sg := sshade; sshade:=soutline;
                  pgfarcahead( aat,X1,ahnum(atype),soutline,
                     qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                     lth,-abs(aradius),|arcangle|,X1 );
                  shadedp := sf; sshade := sg
                  end;
               sf := shadedp; shadedp := nil; sg := sshade; sshade := nil;
               c := lfill; lfill := -1.0;
               pgfstartdraw(0,node,lsp);
               wpos(X0);
               popgwarc( aat,abs(aradius),posangle(X0,aat),posangle(X1,aat),
                  |arcangle| );
               shadedp := sf; sshade := sg; lfill := c;
               pgfendpath
               end
            end;
         pgfwrtext( node,textp,aat.xpos,aat.ypos )
         end;
      XLline,XLarrow,XLmove: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            lth := qenv(node,XLlinethick,tn@.lthick);
            if bfill then begin
               s := lthick; lthick := 0;
               pgfstartdraw(0,node,XLinvis); lthick := s;
               wpos(aat);
               tx := node;
               while tx<>nil do begin
                  ddash; wpos(tx@.endpos);
                  tx := tx@.son
                  end;
               writeln('--cycle'); pgfendpath
               end;
            if lsp<>XLinvis then begin
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                  sf := shadedp; shadedp := nil;
                  sg := sshade; sshade := soutline;
                  pgfstartdraw(ahnum(atype),node,lsp);
                  pgfahead(ahnum(tn@.atype),aat,endpos,
                     qenv(node,XLarrowht,tn@.height),
                     qenv(node,XLarrowwid,tn@.width),lth);
                  shadedp := sf; sshade := sg
                  end;
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                  sf := shadedp; shadedp := nil;
                  sg := sshade; sshade := soutline;
                  pgfstartdraw(ahnum(atype),node,lsp);
                  pgfahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                     qenv(node,XLarrowht,tn@.height),
                     qenv(node,XLarrowwid,tn@.width),lth);
                  shadedp := sf; sshade := sg
                  end;
               sf := shadedp; shadedp := nil; sg := sshade; sshade := nil;
               s := lfill; lfill := -1.0; c := vfill; vfill := -1.0;
               pgfstartdraw(0,node,lsp);
               shadedp := sf; sshade := sg; lfill := s; vfill := c;
               wpos(aat)
               end
            end;
         if lsp<>XLinvis then begin
            ddash; wpos( endpos );
            if son=nil then pgfendpath
            end;
         if son=nil then while snode<>nil do with snode@ do begin
            if textp<>nil then pgfwrtext( snode,textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLspline: begin
         if firstsegment(node) then begin
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            lparam := tn@.lparam; lthick := tn@.lthick;
            if bfill then begin
               c := lthick; lthick := 0;
               sg := soutline; soutline := sshade;
               pgfstartdraw(0,node,XLinvis); lthick := c; soutline := sg;
               spltot := primdepth( node ); splcount := spltot;
               tx := node;
               while tx<>nil do begin
                  splinesegment(tx,splcount,spltot);
                  splcount := splcount - 1;
                  tx := tx@.son
                  end;
               write(' --cycle'); pgfendpath
               end;
            if lsp<>XLinvis then begin
               spltot := primdepth( node ); splcount := spltot;
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                  sf := shadedp; shadedp := nil;
                  sg := sshade; sshade := soutline;
                  pgfstartdraw(ahnum(tn@.atype),node,lsp);
                  pgfahead(ahnum(tn@.atype),aat,endpos,
                              qenv(node,XLarrowht,tn@.height),
                              qenv(node,XLarrowwid,tn@.width),
                              qenv(node,XLlinethick,tn@.lthick));
                  shadedp := sf; sshade := sg
                  end;
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                  sf := tn@.shadedp; tn@.shadedp := nil;
                  sg := sshade; sshade := soutline;
                  pgfstartdraw(ahnum(tn@.atype),tn,lsp);
                  pgfahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                              qenv(node,XLarrowht,tn@.height),
                              qenv(node,XLarrowwid,tn@.width),
                              qenv(node,XLlinethick,tn@.lthick));
                  tn@.shadedp := sf; sshade := sg
                  end;
               deletename(shadedp); sshade := nil; lfill := -1; vfill:= -1;
               pgfstartdraw(0,node,lsp)
               end
            end;
         if lsp<>XLinvis then begin
            splinesegment(node,splcount,spltot);
            if splcount=1 then pgfendpath
            end;
         splcount := splcount - 1
         (*D; if debuglevel > 0 then
                 writeln(log,'pgfdraw: node ',splcount:1);D*)
         end;
      XLstring: pgfwrtext( node,textp,aat.xpos,aat.ypos );
      XLaTeX: if textp <> nil then begin
            wstring(output, textp );
            writeln
            end
         else if lthick >= 0.0 then pgfsetthick(lthick) ;
      otherwise
      end
   end; (* pgfdraw *)
