.PS
# Spiral.m4
threeD_init
copy "HOMELIB_/dpictools.pic"

  azimuth = 15                     # Set view angles in degrees
  elevation = 35
  setview(azimuth,elevation)

  crad = 1                         # Sphere radius
  thetamin = -89                   # Rotation about the y axis (deg)
  thetamax = 89
  turns = 6

  South: 0,0                       # South pole
  define(`N3D',`0,0,2*crad')       # North pole
  North: project(N3D)
  define(`C3D',`0,0,crad')         # Centre of the sphere
  C: project(C3D)
                                   # Shaded sphere at C
  g = 0.65
  Hlight: C+(Rect_(crad*g,135))
  rm = crad*(1+g)
  n = int(rm/(linethick bp__))
  lt = linethick * ifpgf(2,1.5)
  for i=1 to n do {
    r = i/n*rm
    u = 1-(i/n)^2
    if r < (1-g)*crad then {
      circle thick lt outlined rgbstring(u,u,u) rad r at Hlight }\
    else {
      arc cw thick lt outlined rgbstring(u,u,u) \
        from Cintersect(Hlight,r,C,crad) \
        to Cintersect(Hlight,r,C,crad,R) with .c at Hlight }
    }
  circle rad crad ifpgf(+linethick bp__/2) at C

                                   # The parametric spiral curve S(theta)
                                   # theta is degrees rotation about y
                                   # Rotation about z is 2*turns*theta
define(`Radius',
 `rot3Dz((-2*turns*`$1'*dtor_),rot3Dy((-`$1'*dtor_),crad,0,0))')
define(`S',`sum3D(C3D,Radius(`$1'))')
                                   # The visibility function is the dot product
                                   # of the view vector and the radius vector
define Radiusvis { $2 = dot3D(Radius($1),View3D) }

  dphi = 5                         # Horiz degrees between test pts on the curve
  dt = (dphi)/(2*turns)
  n = 0
  P[n]: project(S(thetamin))
  Radiusvis(thetamin,xs)           # Visibility of the initial pt
  for theta=thetamin+dt to thetamax by dt do {
    Radiusvis(theta,xf)            # Visibility of the next pt
    if xs*xf < 0 then {
      findroot( Radiusvis, theta-dt,theta,1e-8,x ) # Find the transition pt
      n+=1; P[n]: project(S(x))
      if xs < 0 then { fitcurve(P,n,dashed) } else { fitcurve(P,n) }
      P[0]: P[n]
      n = 0
      xs = -xs
      }
    n+=1; P[n]: project(S(theta))
    }
  n+=1; P[n]: project(S(thetamax))
  if xs < 0 then { fitcurve(P,n,dashed) } else { fitcurve(P,n) }

                                   # Axes
  X: project(crad,0,0)
  Y: project(0,crad,0)

define linevis { # ratio           # Visibility for lines fom South to Tmp
  $2 = distance(($1 between South and Tmp),C)-crad }

define invisline { # name          # Draw dashed invisible part of line in
  Tmp: $1                          # the plane
  dw = 3pt__
  findroot( linevis, 0, 1, 1e-8, x )
  line dashed dw from South to x between South and Tmp chop 0 chop dw/2 }

thinlines_
  invisline(X)
  arrow to X chop dw/2 chop 0; "$x$" below
  invisline(Y)
  arrow to Y chop dw/2 chop 0; "$y$" ljust
  line dashed from South to North chop 0 chop dashwid
  arrow up crad*0.5 chop 0.05 chop 0 ; "$z$" above

.PE
