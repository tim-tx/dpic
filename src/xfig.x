(* xfig.x *)
(* Output routines for xfig *)

procedure xfigprelude;
begin
(* writeln('#FIG 3.1');
   writeln('Landscape');
   writeln('Center');
   writeln('Inches');
   writeln(xfigres:1,' 2');
   writeln('# VERSIONDATE option -x for Fig 3.1')   
   *)

   writeln('#FIG 3.2');
   writeln('Landscape');
   writeln('Center');
   writeln('Inches');
   writeln('Letter');
   writeln('100.00');
   writeln('Single');
   writeln('-2');
   writeln('# VERSIONDATE option -x for Fig 3.2');
   writeln(xfigres:1,' 2')
     
   end;

procedure wfigpt( x: real );
begin
   write(' ',round(x/fsc*xfigres):1)
   end;

procedure wfigcoord( x,y: real );
begin
   wfigpt( x ); wfigpt( xfheight-y )
   end;

procedure arrowline( atype: integer; wid,ht,lth: real );
begin
   if ahnum(atype) = 0 then write(tabch,'0 0 ') else write(tabch,'1 1 ');
   wfloat(output,lth*xdispres/pointd); write(' ');
   wfloat(output, wid*xfigres ); write(' ');
   wfloat(output, ht*xfigres );
   writeln
   end;

function linstyle( i: integer): integer;
begin
   if i = XLsolid then i := 0
   else if i = XLdashed then i := 1
   else if i = XLdotted then i := 2
   else i := -1;
   linstyle := i
   end;

procedure hdrline( object_code,sub_type,line_style: integer; lth,gfill: real);
(* first 10 values object_code .. style_val *)
begin
   write(object_code:1,' ', sub_type:1,' ', linstyle(line_style):1,' ');

   if line_style = XLinvis then write('0 ')
   else write(round(lth*xdispres/pointd):1,' ');
   write('0 -1 0 -1 ');  (* pencolor, fillcolor, depth, penstyle *)
   if gfill = -1.0 then write('-1 ')    (* area fill *)
   else write(round((1.0-gfill)*20):1,' ');

   (* style_val *)
   if line_style = XLdashed then wfloat(output, 5.0*xdispres/pointd )
   else if line_style = XLdotted then wfloat(output, 3.0*xdispres/pointd )
   else write('0'); write(' ')
   end;

function fwdarrow( i: integer): integer;
begin
   if (ahlex(i)=XRIGHTHEAD) or (ahlex(i)=XDOUBLEHEAD) then
      fwdarrow := 1 else fwdarrow := 0
   end;

function bckarrow( i: integer): integer;
begin
   if (ahlex(i)=XLEFTHEAD) or (ahlex(i)=XDOUBLEHEAD) then
      bckarrow := 1 else bckarrow := 0
   end;

procedure polyline( object_code, sub_type, line_style: integer;
                    lth, gfill: real;
                    lrad: real;
                    atype: integer;
                    lwid, lht: real;
                    npoints: integer );
begin
   hdrline( object_code,sub_type,line_style, lth,gfill);
   if object_code = 3 then write('0 ')  (* cap_style = butt for splines *)
                        (* join_style = miter, cap_style = butt, radius *)
   else write('0 0 ',round(lrad*xdispres):1,' ');
   writeln(fwdarrow(atype):1,' ',bckarrow(atype):1,' ',npoints:1);
   if fwdarrow(atype) = 1 then arrowline(atype,lwid,lht,lth);
   if bckarrow(atype) = 1 then arrowline(atype,lwid,lht,lth)
   end;

procedure xfigwrtext(np: primitivep; tp: strptr; bxht,bxwid,x,y: real);
var ydisp: real;
   istr,nstr,figjust,i: integer;
   A,B,L,R: boolean;
   p: strptr;
begin
   if bxht = 0.0 then bxht := venv(np,XLtextht);
   if tp <> nil then begin
      nstr := 0; p := tp;
      repeat nstr := nstr+1; p := p@.next until p = nil;
      checkjust(tp,A,B,L,R);
      istr := 0
      end;
   while tp <> nil do begin
      istr := istr + 1;
      write('4 ');
      if L then figjust := 0 else if R then figjust := 2 else figjust := 1;
      write(figjust:1,' ');
      write('-1 0 -1 '); (* color, depth, penstyle *)
      write('0 10 0.0 2');     (* font, font_size, angle, font_flags *)
      wfigpt( bxht );
      if bxwid = 0 then wfigpt(tp@.len*bxht*2/3) else wfigpt(bxwid);
      (*D if debuglevel > 0 then writeln(log,
        ' bxht=',bxht:9:2,' bxwid=',bxwid:9:2,' length=',tp@.len:1); D*)
      if A then ydisp := bxht/5+venv(np,XLtextoffset)
      else if B then ydisp := -bxht-venv(np,XLtextoffset)
      else ydisp := -bxht/3;
      wfigcoord( x, y+ydisp+((nstr+1)/2-istr)*bxht ); write(' ');
      with tp@ do for i:=0 to len-1 do begin
         if segmnt@[seginx+i] = bslch then write(bslch);
         write(segmnt@[seginx+i]) end;
      writeln('\001');
      tp := tp@.next
      end
   end;

procedure farc( object_code,sub_type,line_style: integer; lth,gfill: real;
             atype: integer; radius,strtang,arcang,x,y,lwid,lht: real);
begin
   hdrline( object_code,sub_type,line_style, lth,gfill);
   write('0 ');
   if arcang < 0.0 then write('0 ') else write('1 ');
   write(fwdarrow(atype):1,' ',bckarrow(atype):1,' ');
   wfigcoord( x,y );
   wfigcoord(x+radius*cos(strtang),y+radius*sin(strtang) );
   wfigcoord(x+radius*cos(strtang+arcang/2),y+radius*sin(strtang+arcang/2));
   wfigcoord(x+radius*cos(strtang+arcang),y+radius*sin(strtang+arcang));
   writeln;
   if fwdarrow(atype) = 1 then arrowline(atype,lwid,lht,lth);
   if bckarrow(atype) = 1 then arrowline(atype,lwid,lht,lth)
   end;

procedure fellipse(object_code,sub_type,line_style: integer; lth,gfill: real;
             center_x,center_y,radius_x,radius_y: real);
begin
   hdrline( object_code,sub_type,line_style,lth,gfill);
   write('1 0.0');
   wfigcoord( center_x, center_y );
   wfigpt( radius_x ); wfigpt( radius_y ); 
   wfigcoord( center_x+radius_x, center_y);
   wfigcoord(center_x+radius_x, center_y);
   writeln
   end;

function rdrawn( np: primitivep ): boolean;
var rv,v: boolean;
begin
   rv := false;
   while (rv = false) and (np <> nil) do with np@ do begin
      if ptype = XLbox then v := drawn(np,lspec(spec),boxfill)
      else if ptype = XLcircle then v := drawn(np,lspec(spec),cfill)
      else if ptype = XLellipse then v := drawn(np,lspec(spec),efill)
      else if ptype in [XLarc,XLline,XLarrow,XLspline] then
         (* v := drawn(np,lspec(spec),-1.0) *)
         v := drawn(np,lspec(spec),lfill)
      else v := false; 
      if v or (textp<>nil) then rv := true
      else if rdrawn(son) then rv := true
      else np := next
      end;
   rdrawn := rv
   end;

procedure xfigdraw( node: primitivep );
var i,lsp: integer;
   fill: real;
   tn: primitivep;
begin (* xfigdraw *)
   getlinespec(node,lsp,tn);
   with node@ do begin
      if lthick < 0.0 then lthick := venv(node,XLlinethick);
      (*D if debuglevel > 0 then with node@ do begin
         write(log,'xfigdraw(',ordp(node):1,'): lthick=',lthick:9:2,' ');
         snaptype(log,ptype); writeln(log)
         end; D*)
      end;
   with node@ do case ptype of
      XLarc: begin
         if drawn(node,lsp,lfill) then farc(5,1,lsp,lthick,
            lfill,atype,aradius,|startangle|,
            |arcangle|,aat.xpos,aat.ypos,width,height);
         xfigwrtext( node,textp, 0.0, 0.0, aat.xpos, aat.ypos )
         end;
      XLline,XLarrow,XLspline: begin
         if drawn(node,lsp,lfill) then begin
            if firstsegment(node) then begin
               spltot := primdepth( node );
               if ptype = XLspline then polyline(3,0,lsp,lthick,
                  lfill,0.0,atype,width,height,spltot+1)
               else polyline(2,1,lsp,lthick,
                  lfill,0.0,atype,width,height,spltot+1);
               write(tabch);
               wfigcoord(aat.xpos,aat.ypos)
               end;
            wfigcoord(endpos.xpos,endpos.ypos);
            if (son = nil) and (ptype = XLspline) then begin
               writeln; write(tabch,' 0.0');
               for i:= 2 to spltot do write(' 1.0'); writeln(' 0.0') 
               end
            else if son = nil then writeln
            end;
         xfigwrtext( node,textp,0.0,0.0,0.5*(endpos.xpos+aat.xpos),
                               0.5*(aat.ypos+endpos.ypos) )
         end;
      XLbox,XBLOCK: begin
         initnesw; nesw( node );
         if (ptype = XBLOCK) and
            ((textp<>nil) or drawn(node,lsp,-1.0) or rdrawn(son)) then begin
            direction := -1;
            write('6 '); wfigcoord(west,north); wfigcoord(east,south);
            writeln end;
         if ptype=XBLOCK then fill := -1.0 else fill := boxfill;
         if drawn(node,lsp,fill) then begin
            polyline(2,2,lsp,lthick,fill,0.0,-1,0.0,0.0,6);
            write(tabch);
            wfigcoord(west, (south+north)/2);
            wfigcoord(west, south); wfigcoord(east, south);
            wfigcoord(east, north); wfigcoord(west, north);
            wfigcoord(west, (south+north)/2); writeln
            end;
         xfigwrtext( node,textp,0.0,0.0,aat.xpos, aat.ypos )
         end;
      XLmove: xfigwrtext( node,textp,0.0,0.0,0.5*(endpos.xpos+aat.xpos),
                          0.5*(aat.ypos+endpos.ypos) );
      XLellipse: begin
         if drawn(node,lsp,efill) then fellipse(1,1,lsp,lthick,efill,
               aat.xpos,aat.ypos,0.5*elwidth,0.5*elheight);
         xfigwrtext( node,textp,0.0,0.0,aat.xpos,aat.ypos )
         end;
      XLcircle: begin
         if drawn(node,lsp,cfill) then fellipse(1,3,lsp,lthick,cfill,
            aat.xpos,aat.ypos,radius,radius);
         xfigwrtext( node,textp,0.0,0.0,aat.xpos,aat.ypos )
         end;
      XLstring: xfigwrtext( node,textp,boxheight,boxwidth,aat.xpos,aat.ypos);
      XLaTeX: if textp<>nil then begin
         wstring(output, textp );
         writeln
         end;
      otherwise
      end
   end; (* xfigdraw *)
