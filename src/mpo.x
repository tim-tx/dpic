(* mpo.x *)
(* Output routines for MetaPost *)
procedure mpoprelude;
begin
   printstate := printstate+1;
   writeln('% VERSIONDATE option -s for MetaPost');
   writeln('beginfig(',printstate:1,')');
   writeln('def lcbutt=linecap:=butt enddef;');
   writeln('def lcsq=linecap:=squared enddef;');
   writeln('def lcrnd=linecap:=rounded enddef;');
   writeln('def ljmit=linejoin:=mitered enddef;');
   writeln('def lcolr= enddef; def drw=draw enddef; def X=;enddef;');
   writeln('lcbutt; ljmit;')
   end;

procedure mpopostlude;
begin
   writeln('endfig;')
   (*D; if debuglevel > 0 then writeln(log,'mpobpostlude done');D*)
   end;

procedure mpowrtext( np: primitivep; tp: strptr; x,y: real );
var A,B,L,R: boolean;
begin
   if tp = nil then begin end
   else if tp@.next <> nil then begin
      write('label');
      write('(btex '); texstacktext( np, tp );
      write(' etex,'); wcoord(output,x,y); writeln(') X')
      end
   else begin
      write('label');
      checkjust(tp,A,B,L,R);
      if      A and L then write('.urt')
      else if A and R then write('.ulft')
      else if B and L then write('.lrt')
      else if B and R then write('.llft')
      else if A then write('.top')
      else if B then write('.bot')
      else if L then write('.rt')
      else if R then write('.lft');
      write('(btex '); wstring( output,tp );
      write(' etex,'); wcoord(output,x,y); writeln(') X')
      end
   end;

procedure mposetthick(lthick: real);
begin
   if not ismdistmax(lthick) then begin end
   else if not ismdistmax(gslinethick) then lthick := gslinethick
   else lthick := 0.8;
   if (lthick >= 0.0) and (lthick <> gslinethick) then begin
      write('pickup pencircle scaled ');
      wfloat(output,lthick);
      writeln('bp;');
      gslinethick := lthick
	  end
   (*D; if debuglevel > 0 then begin writeln(log,' mposetthick:');
      write(log,' lthick='); wfloat(log,lthick);
      write(log,' gslinethick='); wfloat(log,gslinethick);
      writeln(log)
      end D*)
   end;

procedure mpolinecap( lsp: integer );
begin
   case lsp of
      XLdashed: writeln('lcsq;');
      XLdotted: writeln('lcrnd;');
      otherwise
      end
   end;

procedure addcolor(sp: strptr; fillv: real);
begin
   write(' withcolor ');
   if sp <> nil then wstring(output, sp )
   else begin
      wfloat(output,fillv); write('white')
      end
   end;

procedure mpodashdot( lsp: integer; param: real; op: strptr );
begin
   if lsp = XLdashed then begin
      write(' dashed evenly');
      if param > 0 then begin
         write(' scaled '); wfloat(output,param/fsc/3) end;
      if op<>nil then addcolor(op,-1);
      writeln(' X lcbutt;')
      end
   else if lsp = XLdotted then begin
      write(' dashed withdots scaled ');
      if param <= 0 then write('0.8') else wfloat(output,param/fsc/5);
      if op<>nil then addcolor(op,-1);
      writeln(' X lcbutt;')
      end
   else if op<>nil then begin
      addcolor(op,-1.0);
      writeln(' X')
      end
   else
      writeln(' X')
   end;

procedure mpocorner( ctr:postype; x,y,r,ct,st: real );
begin
  wcoord(output,ctr.xpos+x+ct*r,ctr.ypos+y+st*r); controls;
  wcoord(output,ctr.xpos+x+ct*r-st*r*SPLT,ctr.ypos+y+st*r+ct*r*SPLT); wrand;
  wcoord(output,ctr.xpos+x+ct*r*SPLT-st*r,ctr.ypos+y+st*r*SPLT+ct*r); ddot;
  wcoord(output,ctr.xpos+x-st*r,ctr.ypos+y+ct*r); ddash;
  end;

procedure mpobox( initial: string; ctr: postype; halfwid,halfht,rad: real );
var r,h,v: real;
begin
   write(initial);
   if rad = 0.0 then begin
     wcoord(output,ctr.xpos-halfwid,ctr.ypos+halfht); ddash;
     wcoord(output,ctr.xpos+halfwid,ctr.ypos+halfht); ddash;
     wcoord(output,ctr.xpos+halfwid,ctr.ypos-halfht); ddash;
     wcoord(output,ctr.xpos-halfwid,ctr.ypos-halfht); ddash
     end
   else begin
     r := Min(Min(rad,halfht),halfwid);
     h := halfwid-r; v := halfht-r;
     mpocorner(ctr,h,v,r,1,0);
     mpocorner(ctr,-h,v,r,0,1);
     mpocorner(ctr,-h,-v,r,-1,0);
     mpocorner(ctr,h,-v,r,0,-1)
     end;
   write('cycle')
   end;

procedure mpoellipse(initial: string; ctr: postype; halfwid,halfht: real );
begin
   write(initial);
   wcoord(output,ctr.xpos+halfwid     ,ctr.ypos            ); controls;
   wcoord(output,ctr.xpos+halfwid     ,ctr.ypos+halfht*SPLT); wrand;
   wcoord(output,ctr.xpos+halfwid*SPLT,ctr.ypos+halfht     ); ddot;
   wcoord(output,ctr.xpos             ,ctr.ypos+halfht     ); controls;
   wcoord(output,ctr.xpos-halfwid*SPLT,ctr.ypos+halfht     ); wrand;
   wcoord(output,ctr.xpos-halfwid     ,ctr.ypos+halfht*SPLT); ddot;
   wcoord(output,ctr.xpos-halfwid     ,ctr.ypos            ); controls;
   wcoord(output,ctr.xpos-halfwid     ,ctr.ypos-halfht*SPLT); wrand;
   wcoord(output,ctr.xpos-halfwid*SPLT,ctr.ypos-halfht     ); ddot;
   wcoord(output,ctr.xpos             ,ctr.ypos-halfht     ); controls;
   wcoord(output,ctr.xpos+halfwid*SPLT,ctr.ypos-halfht     ); wrand;
   wcoord(output,ctr.xpos+halfwid     ,ctr.ypos-halfht*SPLT); ddot;
   write('cycle')
   end;

procedure mpoahead(atyp: integer;
   var point: postype; shaft: postype; ht,wid,lth: real; sou: strptr );
var
   P,L,R,Px,Lx,Rx,Q: postype;
   x,y: real;
begin
   dahead( point,shaft,ht,wid,lth/72*scale,P,L,R,Px,Lx,Rx,Q,x,y );
   if atyp=0 then begin
      write('drw ');
      wprop(P,L,x-y,y,x); ddash;
      wpos(P); ddash;
      wprop(P,R,x-y,y,x);
      if sou<>nil then addcolor(sou,-1);
      writeln(' X')
      end
   else begin
      write('fill ');
      wpos(Rx); ddash;
      wpos(point); ddash;
      wpos(Lx);
      if atyp=3 then begin ddash; wpos(Px) end;
      writeln; write(' --cycle');
      if sou<>nil then addcolor(sou,-1);
      writeln(' X')
      end;
   point := P
   end;

procedure mpoarcahead(C,point: postype; atyp: integer; sou: strptr;
       ht,wid,lth,radius,arcangle: real;
   var P: postype );
var 
   Co,Ci,Px,Cox,Cix,Ao,Ai: postype;
   ccw,lwi: real;
   startarrow: boolean;
begin
   (*D; if debuglevel > 0 then writeln(log,' mpoarcahead:'); D*)
   arcahead( C,point,atyp, ht,wid,lth,radius,arcangle,
      P,Co,Ci,Px,Cox,Cix,Ao,Ai, ccw,lwi, startarrow );
   radius := abs(radius);
   write('fill '); wpos(Ai);
   popgwarc( Ci,radius,posangle(Ai,Ci),posangle(point,Ci),-ccw );
   popgwarc( Co,radius,posangle(point,Co),posangle(Ao,Co), ccw);
   if (atyp=0) and (lwi < (wid-lwi)/2) then begin
      ddash; wprop(Ao,Co,radius-lwi,lwi,radius);
      popgwarc( Co,radius-lwi,posangle(Ao,Co),posangle(P,Co),-ccw);
      popgwarc( Ci,radius+lwi,posangle(P,Ci),posangle(Ai,Ci), ccw)
      end
   else if (atyp=3) and (lwi < (wid-lwi)/2) then begin
      popgwarc( Cox,radius,posangle(Ao,Cox),posangle(Px,Cox),-ccw);
      popgwarc( Cix,radius,posangle(Px,Cix),posangle(Ai,Cix), ccw)
      end;
   write(' --cycle'); addcolor(sou,0); writeln(' X')
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

procedure splinesegment(tv: primitivep; splc,splt: integer);
begin
   if tv<>nil then with tv@ do if splt = 1 then begin
      wpos( aat ); ddash; wpos( endpos )
      end
   else if ismdistmax(aradius) then begin
      if splc = splt then begin (* 1st seg *)
         wpos( aat ); ddash;
         wprop( aat,endpos, 1.0,1.0,2.0 ); controls;
         wprop( aat,endpos, 1.0,5.0,6.0 )
         end
      else if splc > 1 then begin (* interior segment *)
         wrand;
         wprop( aat,endpos, 5.0,1.0,6.0 ); ddot;
         wprop( aat,endpos, 1.0,1.0,2.0 ); controls;
         wprop( aat,endpos, 1.0,5.0,6.0 )
         end
      else begin    (* last segment *)
         wrand;
         wprop( aat,endpos, 5.0,1.0,6.0 ); ddot;
         wprop( aat,endpos, 1.0,1.0,2.0 ); ddash;
         wpos( endpos )
         end
      end
   else if (splc = splt) and (splc > 1) then begin
      wpos( aat ); controls;
      wprop( aat,endpos, 1-aradius,aradius,1.0 )
      end
   else if splc > 1 then begin
      wrand;
      wprop(aat,endpos, 1+aradius,1-aradius,2.0 ); ddot;
      wprop(aat,endpos, 1.0,1.0,2.0 ); controls;
      wprop(aat,endpos, 1-aradius,1+aradius,2.0 )
      end
   else begin
      wrand;
      wprop(aat,endpos, aradius,1-aradius,1.0 ); ddot;
      wpos( endpos )
      end
   end;
                                (* node is always <> nil: *)
procedure mpodraw( node: primitivep );
var lsp: integer;
    X0,X1: postype;
    tn,tx: primitivep;
    lth: real;
begin
   getlinespec(node,lsp,tn);
   with node@ do lth := qenv(node,XLlinethick,lthick);
   with node@ do case ptype of
      XLbox,XBLOCK: begin
         if ptype = XLbox then begin
            if (shadedp <> nil) or ((boxfill >= 0.0) and (boxfill <= 1.0)) then
               begin
               mpobox('fill ',aat,boxwidth/2,boxheight/2,boxradius);
               addcolor(shadedp,boxfill); writeln(' X')
               end;
            if lsp<>XLinvis then begin
               mposetthick(lthick);
               mpolinecap(lsp);
               mpobox('drw ',aat,boxwidth/2,boxheight/2,boxradius);
               mpodashdot(lsp,lparam,outlinep)
               end
            end;
         mpowrtext( node, textp, aat.xpos, aat.ypos )
         end;
      XLellipse: begin
         if (shadedp<>nil) or ((efill >= 0.0) and (efill <= 1.0)) then begin
            mpoellipse('fill ',aat,elwidth/2,elheight/2);
            addcolor(shadedp,efill); writeln(' X')
            end;
         if lsp<>XLinvis then begin
            mposetthick(lthick);
            mpolinecap(lsp);
            mpoellipse('drw ',aat,elwidth/2,elheight/2);
            mpodashdot(lsp,lparam,outlinep)
            end;
         mpowrtext( node, textp,aat.xpos,aat.ypos )
         end;
      XLcircle: begin
         if (shadedp<>nil) or ((cfill >= 0.0) and (cfill <= 1.0)) then begin
            write('fill fullcircle scaled ');
            wfloat(output,radius*2/fsc); write(' shifted '); wpos(aat);
            addcolor(shadedp,cfill); writeln(' X')
            end;
         if lsp<>XLinvis then begin
            mposetthick(lthick);
            mpolinecap(lsp);
            write('drw fullcircle scaled ');
            wfloat(output,radius*2/fsc); write(' shifted '); wpos(aat);
            mpodashdot(lsp,lparam,outlinep)
            end;
         mpowrtext( node, textp,aat.xpos,aat.ypos )
         end;
      XLarc: begin
         X0 := arcstart(node); X1 := arcend(node);
         getlinshade(node,tn,sshade,soutline,vfill,bfill);
         if bfill then begin
            mposetthick(0.0);
            write('fill '); wpos(X0);
            popgwarc( aat,abs(aradius),
               posangle(X0,aat),posangle(X1,aat),|arcangle| );
            write(' --cycle'); addcolor(sshade,vfill); writeln(' X');
            vfill := -1; sshade := nil
            end;
         if lsp<>XLinvis then begin
            mposetthick(lth);
            if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
               mpoarcahead( aat,X0,ahnum(atype),soutline,
                  qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                  lth, abs(aradius),|arcangle|,X0 )
               end;
            if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
               mpoarcahead( aat,X1,ahnum(atype),soutline,
                  qenv(node,XLarrowht,height),qenv(node,XLarrowwid,width),
                  lth,-abs(aradius),|arcangle|,X1 )
               end;
            mpolinecap(lsp);
            write('drw '); wpos(X0);
            popgwarc( aat,abs(aradius),posangle(X0,aat),posangle(X1,aat),
               |arcangle| );
            mpodashdot(lsp,lparam,outlinep)
            end;
         mpowrtext( node, textp,aat.xpos,aat.ypos )
         end;
      XLline,XLarrow: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               mposetthick(0.0); write('fill ');
               wpos(aat); tx := node;
               while tx<>nil do begin
                  ddash; wpos(tx@.endpos);
                  tx := tx@.son end;
               write(' --cycle'); addcolor(sshade,vfill); writeln(' X');
               vfill := -1; sshade := nil
               end;
            lth := qenv(node,XLlinethick,tn@.lthick);
            if lsp<>XLinvis then begin
               mposetthick(lth);
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD]
                  then mpoahead(ahnum(tn@.atype),aat,endpos,
                     qenv(node,XLarrowht,tn@.height),
                     qenv(node,XLarrowwid,tn@.width),lth,soutline);
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD]
                  then mpoahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                     qenv(node,XLarrowht,tn@.height),
                     qenv(node,XLarrowwid,tn@.width),lth,soutline);
               mpolinecap(lsp);
               write('drw '); wpos(aat)
               end
            end;
         if lsp<>XLinvis then begin
            ddash; wpos( endpos );
            if son=nil then mpodashdot(lsp,lparam,outlinep)
            end;
         if son=nil then while snode<>nil do begin
            with snode@ do if textp<>nil then mpowrtext( node, textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(endpos.ypos+aat.ypos) );
            snode := snode@.son
            end
         end;
      XLmove: begin
         if firstsegment(node) then snode := node;
         if son=nil then while snode<>nil do begin
            with snode@ do if textp<>nil then mpowrtext( node, textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(endpos.ypos+aat.ypos) );
            snode := snode@.son
            end
         end;
      XLspline: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               spltot := primdepth( node ); splcount := spltot;
               mposetthick(0.0); write('fill ');
               tx := node;
               while tx<>nil do begin
                  splinesegment(tx,splcount,spltot);
                  splcount := splcount - 1; tx := tx@.son
                  end;
               write(' --cycle'); addcolor(sshade,vfill); writeln(' X');
               vfill := -1; sshade := nil
               end;
            lth := qenv(tn,XLlinethick,tn@.lthick);
            if lsp<>XLinvis then begin
               spltot := primdepth( node ); splcount := spltot;
               mposetthick(lth);
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD]
                  then mpoahead(ahnum(tn@.atype),aat,endpos,
                     qenv(node,XLarrowht,tn@.height),
                     qenv(node,XLarrowwid,tn@.width),lth,soutline);
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD]
                  then mpoahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,
                     qenv(node,XLarrowht,tn@.height),
                     qenv(node,XLarrowwid,tn@.width),lth,soutline);
               deletename(shadedp); sshade := nil; lfill := -1; vfill:= -1;
               mpolinecap(lsp);
               write('drw ')
               end
            end;
         if lsp<>XLinvis then begin
            splinesegment(node,splcount,spltot);
            if splcount=1 then begin
               mpodashdot(lsp,lparam,outlinep);
               while snode<>nil do begin
                  with snode@ do if textp<>nil then mpowrtext( node, textp,
                     0.5*(endpos.xpos+aat.xpos),0.5*(endpos.ypos+aat.ypos) );
                  snode := snode@.son
                  end
               end
            end;
         splcount := splcount - 1
         (*D; if debuglevel > 0 then
                 writeln(log,'pgfdraw: node ',splcount:1);D*)
         end;
      XLstring: mpowrtext( node, textp,aat.xpos,aat.ypos );
      XLaTeX: if textp <> nil then begin
            wstring(output, textp );
            writeln
            end
         else if lthick >= 0.0 then mposetthick(lthick);
      otherwise
      end
   end; (* mpodraw *)
