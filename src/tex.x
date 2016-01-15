(* tex.x *)
(* Output routines for TeX, tTeX (eepicemu), Pict2e *)

procedure texprelude( n,s,e,w: real );
begin
   writeln( '\setlength{\unitlength}{1in}' );
   if not isdistmax(s) then begin
      write( '\begin{picture}' );
      wcoord(output, e-w,n-s );
      wcoord(output, w,s );
      writeln;
      writeln( '\thicklines' )
      end;
   write('% VERSIONDATE option ');
   case drawmode of
      TeX: write('(none, LaTeX picture assumed)');
      Pict2e: write('-e (Pict2e)');
      tTeX: write('-t (eepicemu)')
      end;
   writeln(' for LaTeX')
   end;

procedure texpostlude;
begin
   writeln('\end{picture}' );
   (*D if debuglevel > 0 then writeln(log,'texpostlude done');D*)
   end;

procedure arrowhead( pointx,pointy,tailx,taily: real );
var x,y,r,ct: real;
begin
   r := linlen(pointx-tailx,pointy-taily);
   if r <> 0.0 then begin
      ct := Min(0.05,0.5*r)/r;
      x := pointx+ct*(tailx-pointx); y := pointy+ct*(taily-pointy);
      writeln; write('\put');
      wcoord(output, x,y ); write('{\vector');
      wrslope( pointx-x,pointy-y, true );
      write('}')
      end
   end;

procedure texwrtext( np: primitivep; tp: strptr; x,y: real );
var A,B,L,R: boolean;
begin
   if tp <> nil then begin
      write('\put');
      checkjust(tp,A,B,L,R);
      if A then wcoord(output,x,y+0.1)
      else if B then wcoord(output,x,y-0.05)
      else wcoord(output,x,y);
      checkjust(tp,A,B,L,R);
      write('{');
      write('\makebox(0,0)');
      if L then write('[l]') else if R then write('[r]');
      write('{');
      texstacktext( np,tp );
      writeln('}}')
      end
   end;

procedure p2ahead( var point: postype; shaft: postype; ht: real );
var
   butx: postype;
begin
   (*D; if debuglevel > 0 then begin
       write(log,'p2ahead point='); wpair(log,point.xpos,point.ypos);
       write(log,' shaft='); wpair(log,shaft.xpos,shaft.ypos);
       butx := affang(shaft,point);
       write(log,' affang='); wpair(log,butx.xpos,butx.ypos);
       wlogfl('ht',ht,1) end; D*)
                            (* affang contains direction cosines *)
   butx := affine(ht,0.0,point,affang(shaft,point));
   (*D; if debuglevel > 0 then begin
       write(log,' butx='); wpair(log,butx.xpos,butx.ypos);
       writeln(log) end ;D*)
   write('\put'); wpos(butx); write('{\vector');
   wrslope( point.xpos-butx.xpos,point.ypos-butx.ypos, true );
   writeln('}');
   point := butx
   end;

procedure p2setthick(lt: real);
begin
   if (lt >= 0.0) and (lt <> gslinethick) then begin
      write('\linethickness{');
      wfloat(output,lt); 
      writeln('pt}');
      gslinethick := lt
      end
   end;

procedure texdraw( node: primitivep );
var i,npts,lsp: integer;
    r,x,y,x1,y1,ct,st,lgth,lth: real;
    tn,p: primitivep;
    tmpat,X0,X3: postype;
begin
   getlinespec(node,lsp,tn);
   with node@ do begin
      lth := qenv(node,XLlinethick,lthick);
      case ptype of
         XLbox,XBLOCK: if drawn(node,lsp,-1.0) or (textp<>nil) then begin
            initnesw; nesw( node );
            if drawmode = Pict2e then p2setthick(lth);
            write('\put'); wcoord(output, west,south );
            if ptype = XBLOCK then write('{\makebox' )
            else if lsp = XLsolid then write('{\framebox' )
            else if lsp = XLinvis then write('{\makebox' )
            else if lsp = XLdashed then begin
               write('{\dashbox' );
               if not ismdistmax(lparam) then wbrace(lparam/fsc)
               else wbrace(venv(node,XLdashwid)/fsc)
               end
            else if lsp = XLdotted then begin
               write('{\dashbox' ); write('{0.01}') end;
            if ptype=XLbox then
               wcoord(output, boxwidth,boxheight )
            else
               wcoord(output, blockwidth,blockheight );
            write( '{');
            texstacktext( node,textp );
            writeln('}}' )
            end;
         XLline,XLarrow: begin
            if drawn(node,lsp,-1.0) then begin
               if firstsegment(node) then begin           (* first segment *)
                  if drawmode = Pict2e then p2setthick(lth);
                  if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then
                     p2ahead(aat,endpos,qenv(node,XLarrowht,tn@.height))
                  end;
               if (son=nil) and (ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD]) then
                     p2ahead(endpos,aat,qenv(node,XLarrowht,tn@.height));
               lgth:=linlen(endpos.xpos-aat.xpos,endpos.ypos-aat.ypos);
               if (drawmode=Pict2e) or ((lsp = XLsolid) and
                  ((lgth > 0.18) or (drawmode=tTeX))) then begin
                  if lgth > 0 then begin
                     write('\put'); wpos( aat );
                     write('{\line' );
                     wrslope(endpos.xpos-aat.xpos,endpos.ypos-aat.ypos, false);
                     writeln( '}' )
                     end
                  end
               else if lsp = XLsolid then begin
                  npts := round( lgth / 0.005 ); if npts = 0 then npts := 1;
                  write('\multiput'); wpos( aat );
                  wcoord(output, (endpos.xpos-aat.xpos)/npts,
                     (endpos.ypos-aat.ypos)/npts );
                  write('{',npts+1:1,'}');
                  writeln('{\makebox(0.005555,0.00825){\tiny .}}')
                  end
               else if lsp = XLdashed then begin
                  write('\put'); wpos( aat );
                  write('{\dashline[50]' );
                  if not ismdistmax(lparam) then wbrace(lparam/fsc)
                  else wbrace(venv(node,XLdashwid)/fsc);
                  write( '(0,0)' );
                  wcoord(output, endpos.xpos-aat.xpos,endpos.ypos-aat.ypos );
                  writeln( '}' )
                  end
               else if lsp = XLdotted then begin
                  write('\dottedline{'); wfloat(output,0.05/fsc); write('}');
                  wpos( aat );
                  wpos( endpos );
                  writeln
                  end
               end;
            texwrtext( node,textp,0.5*(endpos.xpos+aat.xpos),
                          0.5*(aat.ypos+endpos.ypos) )
            end;
         XLspline: if (drawmode = Pict2e) and drawn(node,lsp,-1.0) then begin
               if firstsegment(node) then begin           (* first spline *)
                  spltot := primdepth( node ); splcount := spltot;
                  tmpat := aat;
                  p2setthick(lth);
                  if ahlex(tn@.atype) in [XLEFTHEAD,XDOUBLEHEAD] then
                     p2ahead(aat,endpos,qenv(node,XLarrowht,tn@.height));
                  (* p2lineopts(lsp,lparam,outlinep); *)
                  if (spltot > 1) and ismdistmax(aradius)  then begin
                     write('\put'); wcoord(output, aat.xpos,aat.ypos );
                     write('{\line' );
                     wrslope( (endpos.xpos-aat.xpos)/2,
                              (endpos.ypos-aat.ypos)/2, false ); writeln('}');
                     write('\cbezier');
                     wprop(endpos,aat, 1.0,1.0,2.0 );
                     wprop(aat,endpos,1.0,5.0,6.0)
                     end
                  else if spltot > 1 then begin
                     write('\cbezier'); wpos(aat); aat := tmpat;
                     wprop(aat,endpos,1-aradius,aradius,1.0)
                     end
                  end
               else if (splcount > 1) and ismdistmax(aradius) then begin
                  wprop(aat,endpos, 5.0,1.0,6.0 );
                  wprop(aat,endpos, 1.0,1.0,2.0 ); writeln('%');
                  write('\cbezier'); wprop(aat,endpos, 1.0,1.0,2.0 );
                  wprop(aat,endpos, 1.0,5.0,6.0 )
                  end
               else if splcount > 1 then begin
                  wprop(aat,endpos, 0.5+aradius/2,0.5-aradius/2,1.0 );
                  wprop(aat,endpos, 1.0,1.0,2.0 ); writeln('%');
                  write('\cbezier'); wprop(aat,endpos, 1.0,1.0,2.0 );
                  wprop(aat,endpos, 0.5-aradius/2,0.5+aradius/2,1.0 )
                  end;
               if splcount = 1 then begin
                  tmpat := endpos;
                  if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                     x := linlen(endpos.xpos-aat.xpos,endpos.ypos-aat.ypos);
                     y := qenv(node,XLarrowht,tn@.height);
                     pprop(aat,endpos,y,x-y,x)
                     end;
                  if (spltot > 1) and ismdistmax(aradius) then begin
                     wprop(aat,endpos, 5.0,1.0,6.0 );
                     wprop(aat,endpos, 1.0,1.0,2.0 ); writeln('%');
                     write('\put'); wprop(aat,endpos, 1.0,1.0,2.0 );
                     write('{\line' );
                     wrslope((endpos.xpos-aat.xpos)/2,
                             (endpos.ypos-aat.ypos)/2, false ); writeln('}')
                     end
                  else if spltot > 1 then begin
                     wprop(aat,endpos, aradius,1-aradius,1.0 );
                     wpos(endpos); writeln('%')
                     end;
                  if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then
                     p2ahead(tmpat,aat,qenv(node,XLarrowht,tn@.height))
                  end;
               (*D if debuglevel > 0 then writeln(log,'node ',splcount:1) ;D*)
               splcount := splcount - 1
               end
            else if lsp in [XLsolid,XLdashed,XLdotted] then begin
               if firstsegment(node) and        (* first spline *)
                  ((ahlex(atype) = XDOUBLEHEAD) or (ahlex(atype) = XLEFTHEAD))
                  then arrowhead( aat.xpos,aat.ypos,endpos.xpos,endpos.ypos );
               write('\spline');
               wpos( aat );
               wpos( endpos );
               if son = nil then writeln
               else if son@.son = nil then begin
                  wpos( son@.endpos );
                  if (ahlex(atype) = XRIGHTHEAD) or (ahlex(atype) = XDOUBLEHEAD)
                     then arrowhead( son@.endpos.xpos,son@.endpos.ypos,
                                son@.aat.xpos,son@.aat.ypos );
                  ptype := XLline;
                  writeln;
                  (* son := nil *)
                  deletetree( son )
                  end
               else begin
                  pprop( son@.endpos,son@.aat, 1.0,1.0,2.0 );
                  wpos(son@.aat);
                  ptype := XLline;
                  writeln
                  end
               end;
         XLmove: texwrtext( node,textp,0.5*(endpos.xpos+aat.xpos),
                             0.5*(aat.ypos+endpos.ypos) );
         XLellipse: begin
            if drawmode = Pict2e then p2setthick(lth);
            write('\put'); wpos( aat );
            if drawmode = TeX then begin
               write('{\ellipse');
               wbrace(elwidth/fsc); wbrace(elheight/fsc);
               writeln('}')
               end
            else begin
               write('{\oval'); wcoord(output,elwidth,elheight);
               writeln('}')
               end;
            texwrtext( node,textp,aat.xpos,aat.ypos )
            end;
         XLcircle: begin
            if lsp <> XLinvis then begin
               if drawmode = Pict2e then p2setthick(lth);
               write('\put'); wpos( aat );
               write('{\circle'); wbrace((2.0*radius)/fsc);
               writeln('}')
               end;
            texwrtext( node,textp,aat.xpos,aat.ypos )
            end;
         XLarc: if (drawmode = Pict2e) and drawn(node,lsp,-1.0) then begin
              p2setthick(lthick); (* p2linecap(lsp); *)
              X0 := arcstart(node);
              if ahlex(atype) in [XLEFTHEAD,XDOUBLEHEAD] then begin
                 if aradius = 0.0 then x := 0.0
                 else x := arctan(qenv(node,XLarrowht,height)/aradius);
                 if |arcangle| >= 0.0 then begin
                    |startangle| := |startangle|+x; |arcangle| := |arcangle|-x
                    end
                 else begin
                    |startangle| := |startangle|-x; |arcangle| := |arcangle|+x
                    end;
                 tmpat := X0;
                 X0 := arcstart(node);
                 p2ahead(tmpat,X0,qenv(node,XLarrowht,height))
                 end;
              X3 := arcend(node);
              if ahlex(atype) in [XRIGHTHEAD,XDOUBLEHEAD] then begin
                 if aradius = 0.0 then x := 0.0
                 else x := arctan(qenv(node,XLarrowht,height)/aradius);
                 if |arcangle| >= 0.0 then |arcangle| := |arcangle|-x
                 else |arcangle| := |arcangle|+x;
                 tmpat := X3;
                 X3 := arcend(node);
                 p2ahead(tmpat,X3,qenv(node,XLarrowht,height))
                 end;
              npts := trunc(1.0+abs(|arcangle|)/pi);
              |arcangle| := |arcangle|/npts;
              while npts > 0 do begin
                 write('\cbezier'); wpos(X0);
                 x := cos(|arcangle|/2); y := sin(|arcangle|/2); x1 := (4-x)/3;
                 if y<>0.0 then y1 := (1.0-x*x1)/y else y1 := 0.0;
                 tmpat.xpos := cos(|startangle|+|arcangle|/2.0);
                 tmpat.ypos := sin(|startangle|+|arcangle|/2.0);
                 wpos(affine(aradius*x1,-aradius*y1,aat,tmpat));
                 wpos(affine(aradius*x1, aradius*y1,aat,tmpat));
                 if npts > 1 then begin
                    |startangle| := |startangle|+|arcangle|;
                    X0 := arcstart(node);
                    wpos(X0); writeln('%')
                    end;
                 npts := npts-1
                 end;
              wpos(X3); writeln('%') (*; p2dashdot(lsp,lparam,outlinep) *)
              end
           else if iscorner(principal(|startangle|,0.5*pi)) and
            (abs(abs(|arcangle|)-pi/2.0) < 0.001) then begin
               if drawmode = Pict2e then p2setthick(lth);
               write('\put'); wpos( aat ); write('{\oval');
               wcoord(output,2*aradius/fsc,2*aradius/fsc);
               if not (direction in [XLleft,XLright,XLup,XLdown]) then begin
                  p := findenv(node); if p<>nil then direction := p@.direction
                  end;
               if( ((direction=XLleft) and (|arcangle|<0.0)) or
                ((direction=XLdown) and (|arcangle|>0.0)))then writeln('[bl]}')
               else if( ((direction=XLleft) and (|arcangle|>0.0)) or
                 ((direction=XLup) and (|arcangle|<0.0))) then writeln('[tl]}')
               else if( ((direction=XLright) and (|arcangle|<0.0)) or
                 ((direction=XLup) and (|arcangle|>0.0))) then writeln('[tr]}')
               else writeln('[br]}');
               texwrtext( node,textp,aat.xpos,aat.ypos )
               end
            else if (drawmode in [TeX,Pict2e]) and (aradius > 0.0) then begin
               if aradius >= 0.05/sin(pi/18.0) then begin
                  npts := round(abs(|arcangle|)/(pi/18.0))
                  end
               else if aradius = 0.0 then npts := 4
               else
                  npts := round(abs(|arcangle|)/arctan(0.05/aradius));
               if npts < 4 then npts := 4;
               r := |arcangle| / npts;
               ct := cos(r); st := sin(r);
               x := aradius*cos(|startangle|);
               y := aradius*sin(|startangle|);
(*D            if debuglevel = 2 then writeln(log,'x,y',x:7:4,y:7:4);  D*)
               for i := 1 to npts do begin
                  x1 :=  ct * x - st * y;
                  y1 :=  st * x + ct * y;
(*D               if debuglevel = 2 then writeln(log,x1:7:4,y1:7:4);   D*)
                  write('\put'); wcoord(output, aat.xpos+x,aat.ypos+y );
                  write('{\line' ); wcoord(output, x1-x,y1-y );
                  if abs(x1-x) <> 0.0 then wbrace(abs(x1-x)/fsc)
                  else wbrace(abs(y1-y)/fsc);
                  writeln( '}' );
                  if (i = 1) and
                     ((ahlex(atype) = XDOUBLEHEAD) or (ahlex(atype)=XLEFTHEAD))
                     then
                     arrowhead( aat.xpos+x,aat.ypos+y,aat.xpos+x1,aat.ypos+y1 )
                  else if (i = npts) and
                     ((ahlex(atype) = XDOUBLEHEAD) or (ahlex(atype)=XRIGHTHEAD))
                     then
                     arrowhead(aat.xpos+x1,aat.ypos+y1,aat.xpos+x,aat.ypos+y );
                  x := x1; y := y1
                  end;
               texwrtext( node,textp,aat.xpos,aat.ypos )
               end;
         XLstring: texwrtext( node,textp,aat.xpos,aat.ypos );
         XLaTeX: if textp <> nil then begin
            wstring(output, textp );
            writeln
            end;
         otherwise
         end
      (*DGHMF ; flush(output) FMHGD*)
      end
   end; (* texdraw *)
