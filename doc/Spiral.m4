% Spiral.m4
.PS
include(HOMELIB_`'lib3D.m4)
gen_init
def_bisect                         # Bring in the equation solver

  azimuth = 15                     # Set view angles in degrees
  elevation = 35
  setview(azimuth,elevation)

  alpha = 1                        # Sphere radius
  thetamin = -89                   # Rotation about the y axis (deg)
  thetamax = 89
  turns = 6

  South: 0,0                       # South pole
  define(`N3D',`0,0,2*alpha')      # North pole
  North: project(N3D)
  define(`C3D',`0,0,alpha')        # Centre of the sphere
  C: project(C3D)

                                   # Shaded sphere: this is black magic but
                                   # PSTricks seems to give more control
  ifpstricks(
   `Highlight: project(sum3D(C3D,rot3Dz(-15*dtor_,rot3Dy(-60*dtor_,alpha,0,0))))
    command "\pscustom[fillstyle=gradient,gradmidpoint=0.0,%"
    command sprintf("gradbegin=gray,gradend=white,gradlines=%g,%%",alpha*200)
    command "GradientCircle=true,GradientScale=1.0,%"
    command sprintf("GradientPos={(%g,%g)}]{",Highlight.x,Highlight.y)
     circle rad alpha at C
     command "}%"',

  `ifpgf(                          # A little too dark with pgf, maybe:
#  `command sprintf(\
#     "\dpicdraw[ball color=white](%g,%g) circle (%gin)\dpicstop",\
#      C.x,C.y,alpha/2.54)'
   `circle rad alpha at C fill_(1) '
,

   `circle rad alpha at C fill_(1) ')')

                                   # The parametric spiral curve S(theta)
                                   # theta is degrees rotation about y
                                   # Rotation about z is 2*turns*theta
define(`Radius',
 `rot3Dz((-2*turns*`$1'*dtor_),rot3Dy((-`$1'*dtor_),alpha,0,0))')
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
      bisect( Radiusvis, theta-dt,theta,1e-8,x ) # Find the transition pt
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
  X: project(alpha,0,0)
  Y: project(0,alpha,0)

define linevis { # ratio           # Visibility for lines fom South to Tmp
  $2 = distance(($1 between South and Tmp),C)-alpha }

define invisline { # name          # Draw dashed invisible part of line in
  Tmp: $1                          # the plane
  dw = 3pt__
  bisect( linevis, 0, 1, 1e-8, x )
  line dashed dw from South to x between South and Tmp chop 0 chop dw/2 }

thinlines_
  invisline(X)
  arrow to X chop dw/2 chop 0; "$x$" below
  invisline(Y)
  arrow to Y chop dw/2 chop 0; "$y$" ljust
  line dashed from South to North chop 0 chop dashwid
  arrow up alpha*0.5 chop 0.05 chop 0 ; "$z$" above

.PE
