(* mfp.x *)
(* Output routines for mfpic *)
procedure mfpprelude( n,s,e,w: real );
begin
   write('\begin{mfpic}[72]');
   wbrace(w/fsc); wbrace(e/fsc); wbrace(s/fsc); wbrace(n/fsc);
   writeln;
   writeln('% VERSIONDATE option -m for mfpic');
   writeln('\dashlen=4bp\dashspace=4bp\dotspace=3bp\pen{0.8bp}');
   writeln('\def\mfpdefaultcolor{black}\drawcolor{\mfpdefaultcolor}');
   gslinethick := 0.8
   end;

procedure mfppostlude;
begin
   writeln('\end{mfpic}');
   (*D if debuglevel > 0 then writeln(log,'mfpbpostlude done');D*)
   end;

procedure mfpwrtext(np: primitivep; tp: strptr; x,y: real );
var A,B,L,R: boolean;
begin
   if tp = nil then begin end
   else if tp@.next <> nil then begin
      write('\tlabel[cc]');
      wcoord(output,x,y);
      write('{'); texstacktext( np, tp ); writeln('}')
      end
   else begin
      write('\tlabel[');
      checkjust(tp,A,B,L,R);
      if A then write('b') else if B then write('t') else write('c');
      if L then write('l') else if R then write('r') else write('c');
      write(']');
      wcoord(output,x,y);
      write('{');
      if A then begin write('\raisebox{');
        wfloat(output,venv(np,XLtextoffset)/scale/12+0.5*venv(np,XLtextht));
        write('ex}{') end
      else if B then begin write('\raisebox{');
        wfloat(output,-venv(np,XLtextoffset)/scale/12-0.5*venv(np,XLtextht));
        write('ex}{') end;
      wstring( output,tp );
      if A or B then write('}');
      writeln('}')
      end
   end;

procedure comma;
begin
   write(',')
   end;

procedure commacr;
begin
   writeln(',')
   end;

procedure mfpsetshade( fill: real; shade: strptr );
begin
   if shade<>nil then begin
      write('\gfill['); wstring(output,shade); write(']')
      end
   else if fill <= 0.0 then write('\gfill')
   else if fill >= 0.99 then begin end
   else begin
      write('\shadespace=');
      wfloat(output,0.44/sqrt(1-fill)); write('bp\shade') end
   end;

procedure mfpellipse( aat: postype; elwidth,elheight: real );
begin
   write('\ellipse{');
   wpos( aat ); comma; wfloat(output, 0.5*elwidth/fsc ); comma;
   wfloat(output, 0.5*elheight/fsc ); writeln('}')
   end;

procedure mfpcircle( aat: postype; radius: real );
begin
   write('\circle{'); wpos( aat ); comma; 
   wfloat(output, radius/fsc ); writeln('}')
   end;

procedure mfpsetdash(dash: real);
begin
   if dash >= 0.0 then begin 
      write('\dashlen=');   wfloat(output,dash); write('in');
      write('\dashspace='); wfloat(output,dash); write('in')
	  end
   end;

procedure mfsetthick(lthk: real);
begin
   if (lthk >= 0.0) and (lthk<>gslinethick) then begin
	  write('\pen{'); wfloat(output,lthk); write('bp}');
      gslinethick := lthk
	  end
   end;

procedure mfpdashdot( lsp: integer; param,lth: real );
begin
   if lsp = XLdashed then begin
      mfpsetdash(param/fsc); write('\dashed')
      end
   else if lsp = XLdotted then begin
      write('\dotted['); wfloat(output,lth); write('pt,');
      if param >= 0.0 then begin
         wfloat(output,param/fsc); write('in]') end
      else begin
         wfloat(output,lth*3); write('pt]') end
      end
   end;

procedure mfplineopts( lth,param: real; lsp: integer; sou: strptr );
begin
   if sou<> nil then begin
      write('\drawcolor{'); wstring(output,sou); writeln('}')
      end;
   mfsetthick(lth);
   mfpdashdot(lsp,param,lth);
   end;

procedure mfpahead( atyp: integer;
 var point: postype; shaft: postype; ht,wid,lth: real; sou: strptr );
var 
   P,L,R,Px,Lx,Rx,Q: postype;
   lwu,x,y,cs,ss,d: real;
begin
   lwu := lth/72*scale;
   dahead( point,shaft,ht,wid,lwu,P,L,R,Px,Lx,Rx,Q,x,y );
   mfsetthick(0.0);
   write('\draw\gfill');
   if sou<>nil then begin
      write('['); wstring(output,sou); write(']') end;
   write('\polygon{');
   d := sqrt(ht*ht+wid*wid/4);
   if d=0 then begin cs := 1; ss := 0 end
   else begin cs := ht/d; ss := wid/2/d end;
   (*D if debuglevel > 0 then begin
      writeln(log,' mfpahead:');
      end; D*) 
   wpos(Rx); commacr; wpos(point); comma; wpos(Lx);
   if (atyp = 0) and (lwu < (wid-lwu)/2) then begin
      comma; writeln;
      wpos(affine(ss*(lwu),cs*(lwu),Lx,Q)); comma;
      wprop(point,P,-1,2,1); comma;
      wpos(affine(-ss*(-lwu),cs*(-lwu),Rx,Q))
      end
   else if (atyp = 3) and (lwu < (wid-lwu)/2) then begin
      comma; writeln;
      wpos(Px)
      end;
   writeln('}');
   point := P
   end;

procedure mfpwarc( Ctr: postype; radius,startangle,endangle,ccw: real );
begin
   write('\arc[p]{'); wpos(Ctr);
   comma;
   if (ccw > 0) and (endangle < startangle) then
      endangle := endangle + 2*pi 
   else if (ccw < 0) and (endangle > startangle) then
      endangle := endangle - 2*pi;
   wfloat(output,(180.0/pi)*startangle); comma;
   wfloat(output,(180.0/pi)*endangle); comma;
   wfloat(output,abs(radius)/fsc);
   writeln('}')
   end;

procedure mfparcahead(C,point: postype; atyp: integer; sou: strptr;
       ht,wid,lth,radius,arcangle: real;
   var P: postype );
var 
   Co,Ci,Px,Cox,Cix,Ao,Ai: postype;
   ccw,lwi: real;
   startarrow: boolean;
begin
   (*D; if debuglevel > 0 then writeln(log,' mfparcahead:'); D*)
   arcahead( C,point,atyp, ht,wid,lth,radius,arcangle,
      P,Co,Ci,Px,Cox,Cix,Ao,Ai, ccw,lwi, startarrow );
   radius := abs(radius);
   mfsetthick(0.0);
   write('\draw\gfill');
   if sou<>nil then begin
      write('['); wstring(output,sou); write(']') end;
   writeln('\lclosed\begin{connect}');
   mfpwarc( Ci,radius,posangle(Ai,Ci),posangle(point,Ci),-ccw );
   mfpwarc( Co,radius,posangle(point,Co),posangle(Ao,Co),ccw );
   if (atyp=0) and (lwi < (wid-lwi)/2) then begin
      mfpwarc( Co,radius-lwi,posangle(Ao,Co),posangle(P,Co),-ccw );
      mfpwarc( Ci,radius+lwi,posangle(P,Ci),posangle(Ai,Ci),ccw )
      end
   else if (atyp=3) and (lwi < (wid-lwi)/2) then begin
      mfpwarc( Cox,radius,posangle(Ao,Cox),posangle(Px,Cox),-ccw );
      mfpwarc( Cix,radius,posangle(Px,Cix),posangle(Ai,Cix),ccw )
      end;
   writeln('\end{connect}')
   (*D; if debuglevel > 0 then begin
      write(log,' <point '); wfloat(log,posangle(point,C)*180/pi);
      write(log,' <P '); wfloat(log,posangle(P,C)*180/pi);
      write(log,' <Ao '); wfloat(log,posangle(Ao,C)*180/pi);
      write(log,' <Ai '); wfloat(log,posangle(Ai,C)*180/pi);
      write(log,' <arcangle '); wfloat(log,arcangle*180/pi);
      writeln(log)
      end D*)
   end;

procedure mfpsplinesegment(tv: primitivep; splc,splt: integer);
begin
   if tv<>nil then with tv@ do if splt = 1 then begin
      write('\polyline{'); wpos(aat); comma; wpos(endpos); writeln('}')
      end
   else if ismdistmax(aradius) then begin
      if splc = splt then begin (* 1st seg *)
         write('\cbeziers{'); wpos( aat ); comma;
         wprop(aat,endpos, 5.0,1.0,6.0 ); comma;
         wprop(aat,endpos, 2.0,1.0,3.0 ); comma;
         wprop(aat,endpos, 1.0,1.0,2.0 ); commacr;
         wprop(aat,endpos, 1.0,5.0,6.0 ); comma
         end
      else if splc > 1 then begin (* interior segment *)
         wprop(aat,endpos, 5.0,1.0,6.0 ); comma;
         wprop(aat,endpos, 1.0,1.0,2.0 ); comma;
         wprop(aat,endpos, 1.0,5.0,6.0 ); commacr
         end
      else begin    (* last segment *)
         wprop(aat,endpos, 5.0,1.0,6.0 ); comma;
         wprop(aat,endpos, 1.0,1.0,2.0 ); comma;
         wprop(aat,endpos, 1.0,2.0,3.0 ); comma;
         wprop(aat,endpos, 1.0,5.0,6.0 ); commacr;
         wpos(endpos); writeln('}')
         end
      end
   else if (splc = splt) and (splc > 1) then begin
      write('\cbeziers{'); wpos( aat ); comma;
      wprop(aat,endpos,1-aradius,aradius,1.0); commacr
      end
   else if splc > 1 then begin
      wprop(aat,endpos, 0.5+aradius/2,0.5-aradius/2,1.0 ); comma;
      wprop(aat,endpos, 1.0,1.0,2.0 ); comma;
      wprop(aat,endpos, 0.5-aradius/2,0.5+aradius/2,1.0 ); commacr
      end
   else begin
      wprop(aat,endpos, aradius,1-aradius,1.0 ); comma;
      wpos(endpos); writeln('}')
      end
   end;

procedure mfpbox( x,y,n,s,e,w,rad: real);
var h,v,rr: real;
begin
   rad := Min(Min(abs(rad),abs((n-s)/2)),abs((e-w)/2));
   if rad = 0.0 then begin
      write('\rect{');
      wcoord(output,w,s); comma; wcoord(output,e,n)
      end
   else begin
     rr := (1-SPLT)*rad; h := (e-w)/2-rad; v := (n-s)/2-rad;
     write('\lclosed\cbeziers{');
     wcoord(output,e,y+v);   comma; wcoord(output,e,n-rr);  comma;
     wcoord(output,e-rr,n);  comma; wcoord(output,x+h,n);   commacr;
     wcoord(output,x+h/3,n); comma; wcoord(output,x-h/3,n); comma;
     wcoord(output,x-h,n);   comma; wcoord(output,w+rr,n);  comma;
     wcoord(output,w,n-rr);  comma; wcoord(output,w,y+v);   commacr;
     wcoord(output,w,y+v/3); comma; wcoord(output,w,y-v/3); comma;
     wcoord(output,w,y-v);   comma; wcoord(output,w,s+rr);  comma;
     wcoord(output,w+rr,s);  comma; wcoord(output,x-h,s);   commacr;
     wcoord(output,x-h/3,s); comma; wcoord(output,x+h/3,s); comma;
     wcoord(output,x+h,s);   comma; wcoord(output,e-rr,s);  comma;
     wcoord(output,e,s+rr);  comma; wcoord(output,e,y-v);   commacr;
     wcoord(output,e,y-v/3); comma; wcoord(output,e,y+v/3)
     end;
   writeln('}')
   end;
                                (* node is always <> nil: *)
procedure mfpdraw( node: primitivep );
var lsp: integer;
    X0,X1: postype;
    tn,tx: primitivep;
    lth: real;
begin
   getlinespec(node,lsp,tn);
   with node@ do begin
     lth := qenv(node,XLlinethick,lthick)
     end;
   (*D if debuglevel > 0 then begin write(log,'mfpdraw: ');
         snaptype(log,node@.ptype);
         write(log,' lth='); wfloat(log,lth); writeln(log) end; D*)
   with node@ do case ptype of
      XLbox,XBLOCK: begin
         initnesw; nesw( node );
         if ptype = XLbox then begin
            if ((boxfill>=0.0) and (boxfill<=1.0)) or (shadedp<>nil) then begin
               mfpsetshade(boxfill,shadedp);
               mfpbox(aat.xpos,aat.ypos,north,south,east,west,boxradius)
               end;
            if lsp<>XLinvis then begin
               mfplineopts(lth,lparam,lsp,outlinep);
               mfpbox(aat.xpos,aat.ypos,north,south,east,west,boxradius);
               if outlinep<>nil then writeln('\drawcolor{\mfpdefaultcolor}')
               end
            end;
         mfpwrtext(node,textp, aat.xpos, aat.ypos )
         end;
      XLellipse: begin
         if ((efill >= 0.0) and (efill <= 1.0)) or (shadedp<>nil) then begin
            mfpsetshade(efill,shadedp); mfpellipse(aat,elwidth,elheight) end;
         if lsp<>XLinvis then begin
            mfplineopts(lth,lparam,lsp,outlinep);
            mfpellipse(aat,elwidth,elheight);
            if outlinep<>nil then writeln('\drawcolor{\mfpdefaultcolor}')
            end;
         mfpwrtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLcircle: begin
         if ((cfill >= 0.0) and (cfill <= 1.0)) or (shadedp<>nil) then begin
            mfpsetshade(cfill,shadedp); mfpcircle(aat,radius) end;
         if lsp<>XLinvis then begin
            mfplineopts(lth,lparam,lsp,outlinep);
            mfpcircle(aat,radius);
            if outlinep<>nil then writeln('\drawcolor{\mfpdefaultcolor}')
            end;
         mfpwrtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLspline: begin
         if firstsegment(node) then begin
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               mfsetthick(0.0); write('\draw');
               mfpsetshade(vfill,sshade); writeln('%');
               write('\lclosed');
               spltot := primdepth( node ); splcount := spltot; tx := node;
               while tx<>nil do begin
                  mfpsplinesegment(tx,splcount,spltot);
                  splcount := splcount - 1; tx := tx@.son end;
               vfill := -1; sshade := nil
               end;
            if lsp<>XLinvis then begin
               lth := qenv(tn,XLlinethick,tn@.lthick);
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then
                  mfpahead(ahnum(tn@.atype),aat,endpos,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,soutline);
               if (ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD]) then
                  mfpahead(ahnum(atype),tn@.endpos,tn@.aat,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,soutline);
               spltot := primdepth( node ); splcount := spltot;
               mfplineopts(lth,lparam,lsp,soutline)
               end
            end;
         if lsp<>XLinvis then mfpsplinesegment(node,splcount,spltot);
         splcount := splcount - 1;
         if (son=nil) and (outlinep<>nil) then
            writeln('\drawcolor{\mfpdefaultcolor}')
         end;
      XLline,XLarrow,XLmove: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               mfsetthick(0.0); write('\draw');
               mfpsetshade(vfill,sshade); writeln('%');
               write('\lclosed\polyline{'); wpos(aat);
               tx := node;
               while tx<>nil do begin
                  commacr; wpos(tx@.endpos);
                  tx := tx@.son end;
               writeln('}');
               vfill := -1; sshade := nil
               end;
            if lsp<>XLinvis then begin
               lth := qenv(tn,XLlinethick,tn@.lthick);
               if (ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD]) then
                  mfpahead(ahnum(tn@.atype),aat,endpos,
                              qenv(node,XLarrowht,tn@.height),
                              qenv(node,XLarrowwid,tn@.width),lth,soutline);
               if (ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD]) then
                  mfpahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                              qenv(node,XLarrowht,tn@.height),
                              qenv(node,XLarrowwid,tn@.width),lth,soutline);
               mfplineopts(lth,lparam,lsp,soutline);
               write('\polyline{'); wpos(aat)
               end
            end;
         if lsp<>XLinvis then begin
            commacr; wpos(endpos);
            if son = nil then begin
               writeln('}');
               if soutline<>nil then writeln('\drawcolor{\mfpdefaultcolor}')
               end
            end;
         if son=nil then while snode<>nil do begin
            with snode@ do if textp<>nil then mfpwrtext(snode,textp,
               0.5*(aat.xpos+endpos.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLarc: begin
         X0 := arcstart(node); X1 := arcend(node);
         getlinshade(node,tn,sshade,soutline,vfill,bfill);
         if bfill then begin
            mfsetthick(0.0); write('\draw');
            mfpsetshade(vfill,sshade); writeln('%');
            write('\lclosed');
            mfpwarc(aat,aradius,posangle(X0,aat),posangle(X1,aat),|arcangle|);
            vfill := -1; sshade := nil
            end;
         if lsp<>XLinvis then begin
            if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
               mfparcahead( aat,X0,ahnum(atype),soutline,
                  qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                  lth, abs(aradius),|arcangle|,X0 )
               end;
            if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
               mfparcahead( aat,X1,ahnum(atype),soutline,
                  qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                  lth,-abs(aradius),|arcangle|,X1 )
               end;
            (*D if debuglevel > 0 then begin write(log,' XLarc: lth=');
                wfloat(log,lth); writeln(log) end;D*)
            mfplineopts(lth,lparam,lsp,soutline);
            mfpwarc( aat,
               aradius,posangle(X0,aat),posangle(X1,aat),|arcangle|);
            if soutline<>nil then writeln('\drawcolor{\mfpdefaultcolor}')
            end;
         mfpwrtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLstring: mfpwrtext(node,textp,aat.xpos,aat.ypos );
      XLaTeX: if textp <> nil then begin
            wstring(output, textp );
            writeln
            end
         else if lth >= 0.0 then mfsetthick(lth)
         else if lparam >= 0.0 then begin mfpsetdash(lparam); writeln end;
      otherwise
      end
   end; (* mfpdraw *)
