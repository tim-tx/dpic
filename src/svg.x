(* svg.x *)
(* Output routines for SVG *)

procedure svgprelude( n,s,e,w,lth: real );
var hsize,vsize: real;
begin
   (*D if debuglevel > 0 then begin
      writeln(log,' svgprelude:');
      write(log,' n='); wfloat(log,n); write(log,' s='); wfloat(log,s);
      write(log,' e='); wfloat(log,e); write(log,' w='); wfloat(log,w);
      write(log,' fsc='); wfloat(log,fsc); write(log,' lth='); wfloat(log,lth);
      writeln(log) end; D*)
   gslinethick := 0.8;

   writeln('<?xml version="1.0" encoding="UTF-8" standalone="no"?>');
   writeln('<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"');
   writeln('"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">');
   writeln('<!-- Creator: VERSIONDATE option -v for SVG 1.1 -->');
   hsize := (e-w+2*lth)/fsc; vsize := (n-s+2*lth)/fsc;
   writeln('<!-- width="',Ceil(hsize+0.5):1,
             '" height="',Ceil(vsize+0.5):1,'" -->');
   writeln('<svg');
   write  (' xmlns="http://www.w3.org/2000/svg"');
   writeln(' xmlns:xlink="http://www.w3.org/1999/xlink"');
   writeln(' xml:space="preserve"');
   write  (' style="shape-rendering:geometricPrecision;');
   writeln(' text-rendering:geometricPrecision;');
   write  (' image-rendering:optimizeQuality;');
   writeln(' fill-rule:evenodd; clip-rule:evenodd"');
   write  (' width="'); wfloat(output,hsize); write('px"');
   write  (' height="');wfloat(output,vsize); writeln('px"');
   write  (' viewBox="0 0 '); wfloat(output,hsize); write(' ');
      wfloat(output,vsize); writeln('"');
   write  (' font-size="'); wfloat(output,DFONT);
   writeln('pt" text-anchor="middle"');
   write  (' stroke="black"'); write(' stroke-miterlimit="10"');
   write  (' stroke-width="'); wfloat(output,0.8/72*SVGPX);
   writeln('" fill="none">');
   writeln('<g>')
   end;

procedure svgpostlude;
begin
   writeln('</g></svg>')
   (*D; if debuglevel > 0 then writeln(log,'svgpostlude done') D*)
   end;

procedure svgsetstroke( lth: real );
begin
   if lth <> gslinethick then begin
      write(' stroke-width="'); wfloat(output,lth/72*SVGPX); writeln('"')
      end
   end;

procedure svgsoutline( so: strptr );
begin
   write(' stroke="'); wstring(output,so); writeln('"')
   end;

procedure fillgray(fll: real);
begin
   write(' fill="');
   if fll = 0.0 then write('black')
   else if fll = 0.25 then write('darkgray')
   else if fll = 0.5  then write('gray')
   else if fll = 0.75 then write('lightgray')
   else if fll = 1.0  then write('white')
   else begin
      write('rgb(');
      write(output,round(fll*255):1); write(',');
      write(output,round(fll*255):1); write(',');
      write(output,round(fll*255):1); write(')')
      end;
   writeln('"')
   end;

procedure svglineoptions( node: primitivep; lnspec: integer );
var param: real;
begin
   with node@ do begin
      if lthick >= 0.0 then svgsetstroke( lthick );
      if soutline<>nil then svgsoutline( soutline );
      if ptype = XBLOCK then lnspec := XLinvis;
      case lnspec of
         XLdashed: begin
            param := qenv(node,XLdashwid,lparam)/fsc;
            write(' stroke-dasharray="');
            wfloat(output,param*7/6); write(',');
            wfloat(output,param*5/6); writeln('"')
            end; 
         XLdotted: begin
            (* if ismdistmax(lparam) then param := 5/72*scale
               else param := lparam; *)
            if not ismdistmax(lparam) then param := lparam
            else param := qenv(node,XLlinethick,lthick)/72*5*scale;
            write(' stroke-linecap="round"');
            write(' stroke-dasharray="0.5,');
            wfloat(output,param/fsc); writeln('"')
            end;
         XLinvis: write(' stroke="none"');
         otherwise
         end
      end
   end;

procedure svgfilloptions(node: primitivep; fill: real; sh: strptr;
                         lnspec: integer; poly: boolean);
begin
   fill := round(fill*1000000)/1000000;
   case node@.ptype of
      XLbox,XBLOCK: write('<rect');
      XLellipse: write('<ellipse');
      XLcircle: write('<circle');
      XLspline: write('<path');
      XLline,XLarrow: if poly then write('<polyline') else write('<line');
      XLarc: write('<path');
      otherwise
      end;
   if sh<>nil then begin
      write(' fill="');
      wstring(output, sh);
      writeln('"')
      end
   else if (fill >= 0.0) and (fill <= 1.0) then fillgray(fill)
   else if node@.ptype=XLstring then fillgray(0.0);
   svglineoptions(node,lnspec)
   end;

procedure svgendpath;
begin
  writeln(' />')
  end;

procedure svgparam( p:string; x:real);
begin
   write(' ',p,'="'); wfloat(output,x/fsc); quote
   end;

procedure svgcoord(s1,s2: string; x,y: real);
begin
   svgparam(s1,x);
   svgparam(s2,xfheight-y)
   (*DGHMF ;flush(output) FMHGD*)
   end;

procedure svgwpos(p: postype);
begin
   wfloat(output,p.xpos/fsc);
   write(',');
   wfloat(output,(xfheight-p.ypos)/fsc)
   (*D;if debuglevel > 0 then begin
      write(log,'svgwpos(');
      wfloat(log,p.xpos/fsc);
      write(log,',');
      wfloat(log,(xfheight-p.ypos)/fsc);
      writeln(log,')')
      end D*)
   end;

procedure svgwprop( p1,p2: postype; a,b,c: real );
begin
   pprop( p1,p2, a,b,c );
   svgwpos( p2 )
   end;

procedure svgwstring( p: strptr );
var i: integer;
   c: char;
   waswhite,iswhite: boolean;
begin
   if p<>nil then with p@ do if segmnt<>nil then begin
     waswhite := false;
     for i:= 0 to len-1 do begin
       c := segmnt@[seginx+i];
       iswhite := (c in [' ',tabch,nlch,etxch]);
       if (not iswhite) or (not waswhite) then write(c);
       waswhite := iswhite
       end
     end
   end;

procedure svgwtext(node: primitivep; tp: strptr; x,y: real );
var nstr: integer;
   tx: strptr;
   L,R,A,B: boolean;
   textht,textoff,dx,dy: real;
begin
  if (tp <> nil) then with node@ do begin
     tx := tp; nstr := 0; textht := venv(node,XLtextht);
     while tx<>nil do begin nstr := nstr+1; tx := tx@.next end;
                           (* boxheight = nstrings * textht *)
     if (ptype=XLstring) and (nstr > 0) then textht := boxheight/nstr;
     textoff := venv(node,XLtextoffset);
       (*D if debuglevel>0 then begin
         writeln(log,' svgwtext: x=',x:8:3,'[',x/fsc:8:3,'] y=',y:8:3,
          '[',(xfheight-y)/fsc:8:3,']');
         writeln(log,' xfheight=',xfheight:8:3);
         writeln(log,' textoff=',textoff:8:3,'[',textoff/fsc:8:3,
          '] textht=',textht:8:3,'[',textht/fsc:8:3,']')
         end; D*)
     y := y + (nstr/2-4/5)*textht;
     with node@ do repeat
        checkjust(tp, A,B,L,R );
        write('<text');
        if ptype<>XLstring then write(' stroke-width="0.2pt" fill="black"')
        else begin
           write  (' font-size="'); wfloat(output,textht/scale*72);
           write('pt"');
           svgfilloptions(node,boxfill,shadedp,lspec(spec),false);
           if lthick < 0 then svgsetstroke(0.2)
           end;
        if L then begin
          write(' text-anchor="start"'); dx := textoff end
        else if R then begin
          write(' text-anchor="end"'); dx := -textoff end
        else dx := 0;
        dy := -textht/20;
        if A then dy := dy+textoff+textht/2
        else if B then dy := dy-textoff-textht*(TEXTRATIO-1)/2;
          (*D if debuglevel>0 then begin
            writeln(log,' A=',A,' B=',B,' L=',L,' R=',R,
             ' dy=',dy:8:3,'[',dy/fsc:8:3,']');
            writeln(log,' x=',x:8:3,'[',x/fsc:8:3,
             '] y=',y:8:3,'[',(xfheight-y)/fsc:8:3);
            writeln(log,' x+dx=',(x+dx):8:3,'[',(x+dx)/fsc:8:3,']',
             ' y+dy=',(y+dy):8:3,'[',(xfheight-(y+dy))/fsc:8:3,']') end; D*)
        svgcoord('x','y',Max(0,x+dx),Max(0,y+dy)); writeln;
        write('>');
        svgwstring( tp );
        writeln('</text>');
        y := y-textht;
        tp := tp@.next
     until tp = nil
     end
  end;

procedure svgwarc( E: postype; r,angle,ccw: real );
begin
   write(' A ');
   wfloat(output,abs(r)/fsc); space;
   wfloat(output,abs(r)/fsc); write(' 0 ');
   if abs(angle) > pi then write('1') else write('0'); 
   if ccw > 0 then write(' 0 ') else write(' 1 ');
   svgwpos(E);
   if angle=0 then writeln
   end;

procedure svgarcahead(C: postype; atyp: integer;
   var point: postype;
       sou: strptr;
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
   write('<path stroke-width="0"');
   if sou <> nil then begin
      svgsoutline( sou );
      write(' fill="'); wstring(output,sou); writeln('"')
      end
   else writeln(' fill="black"');
   if (atyp=0) and (lwi < (wid-lwi)/2) then begin
      write(' d="M '); svgwpos(P); writeln;
      Q := Ci; pprop(Ai,Q,radius+lwi,-lwi,radius);
      svgwarc(Q,radius+lwi,0,ccw);
      write(' L '); svgwpos(Ai); writeln;
      svgwarc(point,radius,0,-ccw);
      svgwarc(Ao,radius,0,ccw);
      write(' L '); svgwprop(Ao,Co,radius-lwi,lwi,radius); writeln;
      svgwarc(P,radius-lwi,1,-ccw)
      end
   else if (atyp=3) and (lwi < (wid-lwi)/2) then begin
      write(' d="M '); svgwpos(Px); writeln;
      svgwarc(Ai,radius,0,ccw);
      svgwarc(point,radius,0,-ccw);
      svgwarc(Ao,radius,0,ccw);
      svgwarc(Px,radius,1,-ccw)
      end
   else begin
      Q.xpos := (Ao.xpos+Ai.xpos)/2; Q.ypos := (Ao.ypos+Ai.ypos)/2;
      write(' d="M '); svgwpos(Q); writeln;
      write(' L '); svgwpos(Ai); writeln;
      svgwarc(point,radius,0,-ccw);
      svgwarc(Ao,radius,0,ccw);
      write(' L '); svgwpos(Q)
      end;
   quote; svgendpath;
   point := P
   end;

procedure svgahead(atyp: integer;
   var point: postype;
       shaft: postype;
       sou: strptr;
       ht,wid,lth,fill: real );
var 
   P,L,R,Px,Lx,Rx,Q: postype;
   x,y: real;
begin
   dahead( point,shaft,ht,wid,lth/72*scale,P,L,R,Px,Lx,Rx,Q,x,y );
   (*D if debuglevel > 0 then begin
       write(log,' dahead values: ');
       logpos('point',point); logpos('shaft',shaft); writeln(log);
       write(log,' ht='); wfloat(log,ht);
       write(log,' wid='); wfloat(log,wid);
       write(log,' lth='); wfloat(log,lth);
       write(log,' scale='); wfloat(log,scale);
       write(log,' fsc='); wfloat(log,fsc);
       write(log,' xfheight='); wfloat(log,xfheight);
       logpos('P',P); logpos('L',L); logpos('R',R); logpos('C',Q);
       logpos('Px',Px); logpos('Lx',Lx); logpos('Rx',Rx);
       writeln(log);
       write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
       writeln(log) end; D*)
   if atyp=0 then begin
      write('<polyline');
      if lth > 0.0 then svgsetstroke( lth );
      if sou <> nil then svgsoutline( sou );
      write(' points="');
      svgwprop(P,L,x-y,y,x); space;
      svgwpos(P); writeln;
      svgwprop(P,R,x-y,y,x)
      end
   else if atyp=3 then begin
      write('<polygon stroke-width="0"');
      if sou <> nil then begin
         svgsoutline( sou );
         write(' fill="'); wstring(output,sou); writeln('"')
         end
      else if (fill >= 0.0) and (fill <= 1.0) then fillgray(fill)
      else writeln(' fill="black"');
      write(' points="');
      svgwpos(Px); space; svgwpos(Lx); writeln;
      svgwpos(point); space; svgwpos(Rx)
      end
   else begin
      write('<polygon stroke-width="0"');
      if sou <> nil then begin
         svgsoutline( sou );
         write(' fill="'); wstring(output,sou); writeln('"')
         end
      else if (fill >= 0.0) and (fill <= 1.0) then fillgray(fill)
      else writeln(' fill="black"');
      write(' points="');
      svgwpos(Lx); space; svgwpos(point); writeln; svgwpos(Rx)
      end;
   quote;
   svgendpath;
   point := P
   end;

procedure svgsplinesegment(tv: primitivep; splc,splt: integer);
begin
   if tv<>nil then with tv@ do if splt = 1 then begin
      write('<line x1="'); wfloat(output,aat.xpos/fsc);
      write('" y1="'); wfloat(output,aat.ypos/fsc);
      write('" x2="'); wfloat(output,endpos.xpos/fsc);
      write('" y2="'); wfloat(output,endpos.ypos/fsc);
      svgendpath
      end
   else if ismdistmax(aradius) then begin
      if splc = splt then begin (* 1st seg *)
         write(' d="M '); svgwpos(aat); writeln; write(' C ');
         svgwprop(aat,endpos, 5.0,1.0,6.0 ); space;
         svgwprop(aat,endpos, 2.0,1.0,3.0 ); space;
         svgwprop(aat,endpos, 1.0,1.0,2.0 ); space;
         svgwprop(aat,endpos, 1.0,5.0,6.0 ); writeln
         end
      else if splc > 1 then begin (* interior segment *)
         svgwprop(aat,endpos, 5.0,1.0,6.0 ); space;
         svgwprop(aat,endpos, 1.0,1.0,2.0 ); space;
         svgwprop(aat,endpos, 1.0,5.0,6.0 ); writeln
         end
      else begin    (* last segment *)
         svgwprop(aat,endpos, 5.0,1.0,6.0 ); space;
         svgwprop(aat,endpos, 1.0,1.0,2.0 ); space;
         svgwprop(aat,endpos, 1.0,2.0,3.0 ); space;
         svgwprop(aat,endpos, 1.0,5.0,6.0 ); writeln;
         svgwpos(endpos); quote; svgendpath
         end
      end
   else if (splc = splt) and (splc > 1) then begin
      write(' d="M '); svgwpos(aat); writeln; write(' C ');
      svgwprop(aat,endpos,1-aradius,aradius,1.0); space
      end
   else if splc > 1 then begin
      svgwprop(aat,endpos, 0.5+aradius/2,0.5-aradius/2,1.0 ); space;
      svgwprop(aat,endpos, 1.0,1.0,2.0 ); space;
      svgwprop(aat,endpos, 0.5-aradius/2,0.5+aradius/2,1.0 ); space
      end
   else begin
      svgwprop(aat,endpos, aradius,1-aradius,1.0 ); space;
      svgwpos(endpos); quote; svgendpath
      end
   end;
                                (* node is always <> nil *)
procedure svgdraw( node: primitivep );
var lsp: integer;
    X1,X2: postype;
    tn,tx: primitivep;
    h,w,lth: real;
begin
   getlinespec(node,lsp,tn);
   with node@ do begin
      sshade := shadedp;
      soutline := outlinep;
      lth := qenv(node,XLlinethick,lthick);
      (*D if debuglevel > 0 then begin write(log,'svgdraw: ');
         snaptype(log,ptype); D*) (* printobject(node); *) (*D
         write(log,' lth='); wfloat(log,lth);
         write(log,' aat=(');wfloat(log,aat.xpos); write(log,',');
         wfloat(log,aat.ypos); write(log,')');
         writeln(log)
         end;
      if linesignal > 0 then begin write(errout,'svgdraw: ');
         snaptype(errout,ptype); writeln(errout) end D*)
      end;
   with node@ do case ptype of
      XLbox: begin
         initnesw; nesw( node );
         if drawn(node,lsp,boxfill) then begin
            svgfilloptions(node,boxfill,shadedp,lsp,false);
            svgcoord('x','y',
               aat.xpos-abs(boxwidth)/2,aat.ypos+abs(boxheight)/2);
            if boxradius > 0.0 then begin
               svgparam('rx',boxradius);
               svgparam('ry',boxradius)
               end;
            svgparam('width',abs(boxwidth));
            svgparam('height',abs(boxheight));
            svgendpath
            end;
         svgwtext(node, textp, aat.xpos, aat.ypos )
         end;
      XBLOCK: svgwtext(node, textp, aat.xpos, aat.ypos );
      XLellipse: begin
         if drawn(node,lsp,efill) then begin
            svgfilloptions(node,efill,shadedp,lsp,false);
            svgcoord('cx','cy',aat.xpos,aat.ypos);
            svgparam('rx',0.5*abs(elwidth));
            svgparam('ry',0.5*abs(elheight));
            svgendpath
            end;
         svgwtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLcircle: begin
         if drawn(node,lsp,cfill) then begin
            svgfilloptions(node,cfill,shadedp,lsp,false);
            svgcoord('cx','cy',aat.xpos,aat.ypos);
            svgparam('r',abs(radius));
            svgendpath
            end;
         svgwtext(node,textp,aat.xpos,aat.ypos )
         end;
      XLline,XLarrow,XLmove: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            if bfill then begin
               write('<polyline stroke-width="0"');
               if soutline<>nil then svgsoutline( soutline );
               if (vfill>=0) and (vfill<=1) then fillgray(vfill)
               else begin
                  write(' fill="'); wstring(output, sshade); writeln('"')
                  end;
               write(' points="'); svgwpos(aat); tx := node;
               while tx<>nil do begin
                  writeln; svgwpos(tx@.endpos);
                  tx := tx@.son end;
               writeln('" />');
               vfill := -1; sshade := nil
               end;
            if lsp<>XLinvis then begin
               lth := qenv(tn,XLlinethick,tn@.lthick);
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then
                  svgahead(ahnum(tn@.atype),aat,endpos,soutline,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,vfill);
               if (ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD]) then
                  svgahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,soutline,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,vfill);
               if node@.son=nil then begin
                  svgfilloptions( tn,vfill,sshade,lsp,false );
                  svgcoord('x1','y1',aat.xpos,aat.ypos);
                  svgcoord('x2','y2',endpos.xpos,endpos.ypos);
                  svgendpath
                  end
               else begin
                  svgfilloptions( tn,vfill,sshade,lsp,true );
                  write(' points="');
                  svgwpos(aat); space
                  end
               end
            end
         else if lsp<>XLinvis then begin
            svgwpos(aat); writeln;
            if son=nil then begin
               svgwpos(endpos); quote;
               svgendpath
               end
            end;
         if son=nil then while snode<>nil do begin
            with snode@ do if textp<>nil then svgwtext(node, textp,
               0.5*(endpos.xpos+aat.xpos),0.5*(aat.ypos+endpos.ypos) );
            snode := snode@.son
            end
         end;
      XLspline: begin
         if firstsegment(node) then begin
            snode := node;
            getlinshade(node,tn,sshade,soutline,vfill,bfill);
            lthick := tn@.lthick; lparam := tn@.lparam;
            if bfill then begin
               write('<path stroke-width="0"');
               if soutline<>nil then svgsoutline( soutline );
               if (vfill>=0) and (vfill<=1) then fillgray(vfill)
               else begin
                  write(' fill="'); wstring(output, sshade); writeln('"')
                  end;
               spltot := primdepth( node ); splcount := spltot; tx := node;
               while tx<>nil do begin
                  svgsplinesegment(tx,splcount,spltot);
                  splcount := splcount - 1; tx := tx@.son
                  end;
               vfill := -1; sshade := nil
               end;
            if lsp<>XLinvis then begin
               lth := qenv(tn,XLlinethick,tn@.lthick);
               if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then
                  svgahead(ahnum(tn@.atype),aat,endpos,soutline,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,lfill);
               if ahlex(tn@.atype) in [XRIGHTHEAD,XDOUBLEHEAD] then
                  svgahead(ahnum(tn@.atype),tn@.endpos,tn@.aat,soutline,
                     qenv(tn,XLarrowht,tn@.height),
                     qenv(tn,XLarrowwid,tn@.width),lth,lfill);
               spltot := primdepth( node ); splcount := spltot;
               svgfilloptions(node,vfill,sshade,lsp,false)
               end
            end;
         if lsp<>XLinvis then svgsplinesegment(node,splcount,spltot);
         splcount := splcount - 1
         end;
      XLarc: begin
         X1 := arcstart(node); X2 := arcend(node);
         getlinshade(node,tn,sshade,soutline,vfill,bfill);
         if bfill then begin
            write('<path stroke-width="0"');
            if (vfill>=0) and (vfill<=1) then fillgray(vfill)
            else begin
               write(' fill="'); wstring(output, sshade); writeln('"') end;
            write(' d="M '); svgwpos(X1); writeln;
            svgwarc( X2,aradius,|arcangle|,|arcangle| );
            quote; svgendpath;
            vfill := -1; sshade := nil
            end;
         if lsp<>XLinvis then begin
            lth := qenv(tn,XLlinethick,tn@.lthick);
            if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
               startarc(node,X1,lth,h,w);
               svgarcahead(aat,ahnum(atype),X1,
                  soutline,h,w,lth,abs(aradius),|arcangle|)
               end;
            if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
               endarc(node,X2,lth,h,w);
               svgarcahead(aat,ahnum(atype),X2,
                  soutline,h,w,lth,-abs(aradius),|arcangle|)
               end;
            write('<path');
            svglineoptions( node,lsp );
            write(' d="M '); svgwpos(X1); writeln;
            svgwarc( X2,aradius,|arcangle|,|arcangle| );
            quote; svgendpath
            end;
         svgwtext(node, textp,aat.xpos,aat.ypos )
         end;
      XLstring: svgwtext(node, textp,aat.xpos,aat.ypos );
      XLaTeX: if textp <> nil then begin
            wstring(output, textp );
            writeln
            end;
      otherwise
      end
   (*DGHMF ;flush(output) FMHGD*)
   end; (* svgdraw *)
