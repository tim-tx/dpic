/* Output from p2c, the Pascal-to-C translator */
/* From input file "<stdin>" */


/* dpic translator program.

   The p2c-dependent, debugging-dependent, and machine-dependent
   comment symbols are stripped out by the makefile or by p2c

*/

/* BSD Licence:

    Copyright (c) 2014, J. D. Aplevich
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
    OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include "p2c.h"
#include <time.h>


/*D,log D*/

/*X program dpic(input,output); X*/

/* exit label for parser errors */

/* include dp0.h */
/* dp0.x */
/* Global definitions */

/* Some PC versions of p2c crash on Pascal const declarations at low levels.
   All consts should be in dp0.x */

#define Version         "dpic version 2014.07.29"

/* UMBX distmax = 1E25; XBMU */

#define distmax         3.40282347e+38   /* assumes at least IEEE single */
/*DM MaxReal = distmax; MD*/
/*GH distmax = MaxReal; HG*/
#define mdistmax        (-distmax)
#define pi              3.1415926535897931
#define ln10            2.3025850929940459

#define SVGPX           90   /* SVG pixels per inch */

/*UGH maxint = 2147483647; HGU*/

#define randmax         SHORT_MAX
#if defined RAND_MAX
#undef randmax
#define randmax RAND_MAX
#endif

#define HASHLIM         7   /* Hash array len for variable names */

/* Parser constants */
#define next            1
#define kind            2
#define symb            3
#define err             4
#define rs              3
#define prod            4
#define lb              3

/* include parscst.i */
#define symbmax         242
#define prodmax         180
#define lrmax           4760
#define lxmax           459
/*B%include parscstB*/

/* include parscdc.i */
#define METAGOAL1       0
#define input1          1
#define input2          2
#define picture1        3
#define picture2        4
#define NL1             5
#define NL2             6
#define start1          7
#define start2          8
#define start3          9
#define elementlist1    10
#define elementlist2    11
#define elementlist3    12
#define term1           13
#define term2           14
#define term3           15
#define term4           16
#define element1        17
#define element2        18
#define element3        19
#define element4        20
#define element5        21
#define element6        22
#define element7        23
#define element8        24
#define element9        25
#define element10       26
#define element11       27
#define element12       28
#define lbrace1         29
#define namedobj1       30
#define namedobj2       31
#define suffix1         32
#define suffix2         33
#define position1       34
#define position2       35
#define position3       36
#define position4       37
#define assignlist1     38
#define assignlist2     39
#define command1        40
#define command2        41
#define command3        42
#define command4        43
#define command5        44
#define command6        45
#define command7        46
#define command8        47
#define command9        48
#define command10       49
#define optnl1          50
#define optnl2          51
#define ifpart1         52
#define elsehead1       53
#define for1            54
#define for2            55
#define stringexpr1     56
#define stringexpr2     57
#define string1         58
#define string2         59
#define assignment1     60
#define assignment2     61
#define assignment3     62
#define assignment4     63
#define expression1     64
#define expression2     65
#define expression3     66
#define expression4     67
#define expression5     68
#define ifhead1         69
#define setlogic1       70
#define logexpr1        71
#define logexpr2        72
#define forhead1        73
#define forincr1        74
#define do1             75
#define do2             76
#define by1             77
#define by2             78
#define redirect1       79
#define redirect2       80
#define redirect3       81
#define resetlist1      82
#define resetlist2      83
#define defhead1        84
#define defhead2        85
#define sprintf1        86
#define sprintf2        87
#define exprlist1       88
#define exprlist2       89
#define object1         90
#define object2         91
#define object3         92
#define object4         93
#define object5         94
#define object6         95
#define object7         96
#define object8         97
#define object9         98
#define object10        99
#define object11        100
#define object12        101
#define object13        102
#define object14        103
#define object15        104
#define object16        105
#define object17        106
#define object18        107
#define object19        108
#define object20        109
#define object21        110
#define object22        111
#define object23        112
#define object24        113
#define object25        114
#define object26        115
#define object27        116
#define openblock1      117
#define block1          118
#define block2          119
#define block3          120
#define block4          121
#define optexp1         122
#define optexp2         123
#define closeblock1     124
#define objectwith1     125
#define objectwith2     126
#define objectwith3     127
#define pair1           128
#define pair2           129
#define nth1            130
#define nth2            131
#define nth3            132
#define primobj1        133
#define primobj2        134
#define primobj3        135
#define primobj4        136
#define shift1          137
#define shift2          138
#define shift3          139
#define location1       140
#define location2       141
#define location3       142
#define location4       143
#define location5       144
#define place1          145
#define place2          146
#define place3          147
#define place4          148
#define place5          149
#define factor1         150
#define factor2         151
#define factor3         152
#define placename1      153
#define placename2      154
#define placename3      155
#define placename4      156
#define ncount1         157
#define ncount2         158
#define ncount3         159
#define logprod1        160
#define logprod2        161
#define logval1         162
#define logval2         163
#define logval3         164
#define lcompare1       165
#define lcompare2       166
#define lcompare3       167
#define lcompare4       168
#define primary1        169
#define primary2        170
#define primary3        171
#define primary4        172
#define primary5        173
#define primary6        174
#define primary7        175
#define primary8        176
#define primary9        177
#define primary10       178
#define primary11       179
#define primary12       180

/* include lxcst.h */
/*D XAND = 36; D*/
#define XBLOCK          26
#define XLBRACKET       21
#define XRBRACKET       22
#define XCOMMENT        42
#define XLcolrspec      138
#define XLcolour        139
#define XCOLON          20
#define XD              25
#define XDOUBLEHEAD     151
#define XDc             98
#define XDe             94
#define XDend           97
#define XDn             92
#define XDne            88
#define XDnw            90
#define XDs             93
#define XDse            89
#define XDstart         96
#define XDsw            91
#define XDw             95
#define XEMPTY          2
#define XEND            45
#define XEQEQ           100
#define XEQ             29
#define XGE             102
#define XLE             103
#define XERROR          3
#define XFOR            83
#define XGT             104
#define XLBRACE         23
#define XLEFTHEAD       149
#define XLT             4
#define XLabel          39
#define XLabove         146
#define XLabs           113
#define XLacos          114
#define XLarc           161
#define XLarcrad        167
#define XLarg           43
#define XLarrow         163
#define XLarrowhd       148
#define XLarrowhead     184
#define XLarrowht       168
#define XLarrowwid      169
#define XLasin          115
#define XLat            54
#define XLatan2         128
#define XLbelow         147
#define XLbox           158
#define XLboxht         170
#define XLboxrad        171
#define XLboxwid        172
#define XLccw           6
#define XLcenter        143
#define XLchop          59
#define XLcircle        159
#define XLcirclerad     173
#define XLcoloneq       30
#define XLcontinue      58
#define XLcorner        87
#define XLcos           116
#define XLcw            5
#define XLdashed        135
#define XLdashwid       174
#define XLdefine        78
#define XLdiameter      109
#define XLdirecton      152
#define XLdiveq         34
#define XLdo            84
#define XLdotted        134
#define XLdown          154
#define XLellipse       160
#define XLellipseht     175
#define XLellipsewid    176
#define XLelse          82
#define XLendfor        85
#define XLenvvar        166
#define XLexp           117
#define XLexpe          118
#define XLfillval       185
#define XLfloat         37
#define XLfloor         126
#define XLfrom          52
#define XLfunc1         112
#define XLfunc2         127
#define XLheight        106
#define XLint           119
#define XLinvis         136
#define XLlastenv       189
#define XLlastsc        183
#define XLleft          156
#define XLlength        111
#define XLline          162
#define XLlineht        177
#define XLlinethick     186
#define XLlinetype      132
#define XLlinewid       178
#define XLljust         144
#define XLlog           120
#define XLloge          121
#define XLcompare       99
#define XLmax           129
#define XLmaxpsht       187
#define XLmaxpswid      188
#define XLmin           130
#define XLminuseq       32
#define XLmove          164
#define XLmoveht        179
#define XLmovewid       180
#define XLmulteq        33
#define XLname          38
#define XLoutlined      140
#define XLparam         105
#define XLpath          137
#define XLpluseq        31
#define XLpmod          131
#define XLprimitiv      157
#define XLradius        108
#define XLremeq         35
#define XLright         155
#define XLrjust         145
#define XLscale         189
#define XLshaded        141
#define XLsign          122
#define XLsin           123
#define XLsolid         133
#define XLspline        165
#define XLsqrt          124
#define XLstring        41
#define XLtan           125
#define XLtextht        181
#define XLtextoffset    182
#define XLtextpos       142
#define XLtextwid       183
#define XLthen          57
#define XLthickness     110
#define XLto            53
#define XLaTeX          40
#define XLundefine      79
#define XLup            153
#define XLwidth         107
#define XNEQ            101
#define XNOT            16
#define XNL             14
#define XRBRACE         24
#define XRIGHTHEAD      150
#define XSEMICOLON      14
#define XSTAR           9
#define XSTART          44
#define XANDAND         17
#define XOROR           18
#define Xlparen         7
#define Xrparen         8
#define Xplus           10
#define Xcomma          19
#define Xminus          11
#define XSLASH          12

/* Machine constants */
#define ordMINCH        0   /* first element of type character */

/* last element of type character,
   newline, tab, CR, ETX */

#define ordMAXCH        255
#define ordNL           10
#define ordTAB          9
#define ordCR           13
#define ordETX          3
#define ordBSL          92
/*B
    ordMAXCH = 255; ordNL = 37; ordTAB = 5; ordCR = 13; ordETX = 3;
    ordBSL = 224; B*/

#define bslch           ((Char)ordBSL)
#define tabch           ((Char)ordTAB)
#define nlch            ((Char)ordNL)
#define crch            ((Char)ordCR)
#define etxch           ((Char)ordETX)

#define maxbval         16383
/* must be > CHBUFSIZ-2 */

/* Lexical parameters */
#define CHBUFSIZ        4095   /* size of chbuf arrays, input record */
#define FILENAMELEN     1024   /* max length of file names */
#define DFONT           11   /* default svg textht, pt; should be adj*/

#define TEXTRATIO       1.6   /* baseline to text height ratio */

/* Lalr machine parameters */

#define STACKMAX        255   /* size of attstack and parsestack */
#define REDUMAX         128   /* size of reduction buffer */
#define ERRMAX          3   /* max no of error messages per line */
#define MAXERRCOUNT     3   /* max no of errors before giving up */


/* Lexical types */
typedef short chbufinx;

typedef uchar symbol;

typedef short lxinx;

typedef uchar production;

typedef Char chbufarray[CHBUFSIZ + 1];
typedef Char mstring[FILENAMELEN];

/* Environment variable index */
/*GH1HG*/
typedef uchar environx;


/* Lalr machine types */
typedef uchar stackinx;

typedef short redubufrange;

typedef struct reduelem {
  /* packed */
  /*D oldtop, D*/
  stackinx newtop;
  production prod_;
} reduelem;

/* Message types */
typedef char errmsginx;

#define TeX             0
#define tTeX            1
#define PSTricks        2
#define MFpic           3
#define MPost           4
#define PGF             5
#define PS              6
#define PSfrag          7
#define pict2e          8
#define xfig            9
#define SVG             10

/* Production types */
/*,PSmps*/

/* For storing names */

typedef struct nametype {
  Char *segmnt;
  chbufinx seginx, len;
  double val;
  struct nametype *next_;
} nametype;

/* Lexical input for loops and macros */

typedef struct fbuffer {
  Char *carray;
  int savedlen, readx, attrib;
  struct fbuffer *higherb, *prevb, *nextb;
} fbuffer;

/* Macro argument list pointer */

typedef struct arg {
  fbuffer *argbody;
  struct arg *higherb, *nextb;
} arg;

/* Pic position */

typedef struct postype {
  /* packed */
  double xpos, ypos;
} postype;

typedef double envarray[XLlastenv - XLenvvar];


/* maybe textp should be a primitivep to allow text to have the full
   properties of boxes */
/* To save some space:
   shadedp: box, circle, ellipse
   outlinep: box, circle, ellipse, line, arc, spline
   textp: not for block
   */

typedef struct primitive {
  nametype *name, *textp, *outlinep, *shadedp;
  struct primitive *parent, *son, *next_;
  postype aat;
  double lparam, lthick;
  int direction, spec, ptype;
  union {
    struct {
      double boxheight, boxwidth, boxfill, boxradius;
    } Ubox;
    struct {
      double blockheight, blockwidth;
      postype here;
      nametype *vars[HASHLIM + 1];
      int nvars[HASHLIM + 1];
      double *env;
    } Ublock;
    struct {
      double cfill, radius;
    } Ucircle;
    struct {
      double elheight, elwidth, efill;
    } Uellipse;
    struct {
      /* XLarc:( endpos.xpos,endpos.ypos: real );
                  endpos.xpos, endpos.ypos */
      postype endpos;
      double height, width, lfill, aradius;
      int atype;
    } Uline;
  } UU;
} primitive;

/* To force optimum dynamic storage of primitives: */

typedef struct XLboxprimitive {
  nametype *name, *textp, *outlinep, *shadedp;
  primitive *parent, *son, *next_;
  postype aat;
  double lparam, lthick;
  int direction, spec, ptype;
  union {
    struct {
      double boxheight, boxwidth, boxfill, boxradius;
    } Ubox;
  } UU;
} XLboxprimitive;

typedef struct XLcircleprimitive {
  nametype *name, *textp, *outlinep, *shadedp;
  primitive *parent, *son, *next_;
  postype aat;
  double lparam, lthick;
  int direction, spec, ptype;
  union {
    struct {
      double cfill, radius;
    } Ucircle;
  } UU;
} XLcircleprimitive;

typedef struct XLellipseprimitive {
  nametype *name, *textp, *outlinep, *shadedp;
  primitive *parent, *son, *next_;
  postype aat;
  double lparam, lthick;
  int direction, spec, ptype;
  union {
    struct {
      double elheight, elwidth, efill;
    } Uellipse;
  } UU;
} XLellipseprimitive;

typedef struct XLlineprimitive {
  nametype *name, *textp, *outlinep, *shadedp;
  primitive *parent, *son, *next_;
  postype aat;
  double lparam, lthick;
  int direction, spec, ptype;
  union {
    struct {
      postype endpos;
      double height, width, lfill, aradius;
      int atype;
    } Uline;
  } UU;
} XLlineprimitive;

typedef struct XLabelprimitive {
  nametype *name, *textp, *outlinep, *shadedp;
  primitive *parent, *son, *next_;
  postype aat;
  double lparam, lthick;
  int direction, spec, ptype;
} XLabelprimitive;

/* Attribute stack types */

typedef struct attribute {
  chbufinx chbufx;
  int length;
  primitive *prim, *root;
  nametype *varname;
  double xval, yval, startchop, endchop;
  int lexval, state;
} attribute;

typedef attribute attstacktype[STACKMAX + 1];

/* Parser types */

typedef struct stackelm {
  stackinx link;
  int table;
} stackelm;

typedef stackelm tparsestack[STACKMAX + 1];


typedef struct _REC_errmsg {
  int no, col, symb_;
} _REC_errmsg;


/*BX def XB*/
/* Machine-dependent characters */
/* tabch, nlch, crch, etxch: char; */

/* File names */
FILE *input, *output, *errout;   /*G asmname 'std_err'; G*/
FILE *copyin;   /*G asmname 'copy_in'; G*/
FILE *redirect;   /*G asmname 'redi_rect'; G*/
/*D log: text; D*/
/*DG asmname 'log_file'; GD*/

mstring infname;   /* name of current input file */
mstring outfnam;   /* name of current output file */
/*X infname: string; X*/

boolean inputeof;   /* end of input flag */
boolean forbufend;   /* end of for buffer */
int argct;   /* argument counter for options */
char drawmode;   /* output conversion */
boolean safemode;   /* disable sh and copy */
/*D oflag: integer; D*/

/* Lexical analyzer character buffer */
/* chbuf: strptr; */
Char *chbuf;
chbufinx chbufi, oldbufi;   /* character buffer index */

/* Lexical variables */
Char ch;   /* current character */
short newsymb;   /* current lexical symbol */
int lexstate, lexsymb;
_REC_errmsg errmsg[ERRMAX + 1];
boolean inlogic;   /* set < to <compare> in context */
boolean instr;   /* set while reading a string */
fbuffer *inbuf, *savebuf, *freeinbuf;

/* Error handling */
int errcount;   /* becomes nonzero when errors found */
errmsginx errmp;   /* index of error messages in errmsg */
int lineno;   /* current input line number */

/* Production variables */
attribute *attstack;
/*D stackhigh: integer;D*/
redubufrange reduinx, redutop;
reduelem redubuf[REDUMAX + REDUMAX + 1];   /* reduction buffer */

double floatvalue;   /* numerical value of floats read */
primitive *envblock;

double north, south, east, west;   /* compass corners of a primitive */
double xfheight;   /* for calculating xfig coordinates */
Char *freeseg;   /* segment open to store strings */
short freex;   /* next free location */
Char *tmpbuf;   /* buffer for snprintf or sprintf */
double scale, fsc;   /* scale factor and final scale factor*/
int splcount, spltot;   /* spline depth counter */
primitive *snode;   /* temporary node storage */
boolean sfill;   /* fill flag for linear objects */
double vfill;   /* fill value */
nametype *sshade, *soutline;   /* temp values for linear objects */
double lastfillval;   /* last-used fill density */
double lastthick;   /* last-used line thickness */
int printstate;   /* for passing output state info */
postype splineend;   /* tmp storage for svg spline */

/* Global tables for easy C conversion.
   Alternative: use run-time space */
short lr[lrmax + 1]={
0,   5,1,   0,  0,   0,  10,6,   0, -1,  15,3745,1,   1,  1,
0,  20,1,  44,  7,  25, 580,2,  16,151,  30, 575,2, 166,169,
35, 565,2,  38,170,  40, 555,2,  37,171,  45, 415,2,   7,172,
50, 325,2,  80,176,  55, 315,2, 112,178,  60, 305,2, 127,179,
65, 285,2,  39,153,  70, 210,2,  87,147,  75, 205,2,  66,149,
80, 180,2,  68,132,  85, 175,2,  27,158,  90,  95,2,  23,159,
0,4486,4,   0,  7, 100, 670,1,  10,  2, 105, 615,1,  11, 66,
110, 580,1,  16,151, 115, 575,1, 166,169, 120, 565,1,  38,170,
125, 555,1,  37,171, 130, 415,1,   7,172, 135, 325,1,  80,176,
140, 315,1, 112,178, 145, 305,1, 127,179, 150, 285,1,  39,153,
155, 210,1,  87,147, 160, 205,1,  66,149, 165, 180,1,  68,132,
170, 175,1,  27,158,   0,  95,1,  23,159, 100, 670,1,  10,  2,
0,4286,3,   0,132,   0, 190,1,  67,  3, 195, 200,2,  68,131,
0,4491,4,   0,130,   0,4286,3,   2,131,   0, 300,3,   0,149,
215,1690,1,  61,  4, 220, 285,1,  39,153, 225, 180,1,  68,132,
165, 230,1,  37,157,   0, 185,3,   0,-157, 240, 275,1, 157,  5,
245, 270,1,  26,134, 250, 265,1,  41,135,   0, 255,1,  21,136,
0, 260,1,  22,  6,   0,4298,3,   1,136,   0,4298,3,   0,-135,
0,4298,3,   0,-134,   0,4298,3,   0,-133,   0,4438,3,   1,154,
290, 295,2,  21, 33,   0,3838,6,   0, 32, 100, 670,1,  10,  2,
0,4318,3,   0,-142,   0, 310,1,   7,  7, 100, 670,1,  10,  2,
0, 320,1,   7,  7, 100, 670,1,  10,  2,   0, 330,1,   7,  7,
95,1640,1,   8,  8, 340, 385,2, 105,175, 345, 360,2,  25,155,
350, 355,2,  87,146,   0,4496,4,   0,145,   0, 300,3,   1,146,
220, 375,1,  39,  9, 240, 275,1, 157,  5,   0,4438,3,   3,156,
290, 295,2,  21, 33,   0,4438,3,   3,155,   0,4478,3,   1,175,
395, 920,1,  70, 10, 400, 915,1,  71,174, 405, 925,1,   9,143,
0, 410,1,  12,144, 110, 580,1,  16, 11, 420,1290,1,  38, 12,
425,1255,1, 166, 62, 430, 670,1,  10, 65, 435, 615,1,  11, 66,
440, 415,1,   7,140, 445, 210,1,  87,147, 450, 205,1,  66,149,
455, 580,1,  16,151, 460, 515,1,  41, 58, 465, 285,1,  39,153,
470, 555,1,  37,171, 475, 325,1,  80,176, 480, 315,1, 112,178,
485, 305,1, 127,179, 160, 490,1,  86, 86,   0, 495,1,   7,  7,
500, 515,1,  41, 13,   0, 490,1,  86, 86,   0, 510,1,   8, 14,
0,4050,3,   1,-59,   0,4050,3,   0, 58,   0,3978,3,   0,-56,
530, 550,2,  19, 87, 535, 540,2,  10, 57,   0,4501,4,   0, 86,
500, 515,1,  41, 13,   0,3978,3,   2, 57, 100, 670,1,  10,  2,
560, 230,4,  67,157,   0,4506,4,   0,171, 290, 295,2,  21, 33,
0,4478,3,   1,170,   0,4478,3,   0,169, 115, 575,1, 166, 15,
0,4410,3,   1,151, 595, 600,2,  15,152,   0,4511,4,   0,150,
110, 580,1,  16, 11,   0,4410,3,   2,152,   0,3798,3,   0,-13,
110, 580,1,  16, 11, 625, 660,2,   9, 14, 630, 650,2,  12, 15,
635, 640,2,  13, 16,   0,4516,4,   0, 66, 110, 580,1,  16, 11,
0,3798,3,   2, 16, 110, 580,1,  16, 11,   0,3798,3,   2, 15,
110, 580,1,  16, 11,   0,3798,3,   2, 14, 110, 580,1,  16, 11,
680, 660,2,   9, 14, 685, 650,2,  12, 15, 690, 640,2,  13, 16,
0,4521,4,   0, 65, 700, 660,2,   9, 14, 705, 650,2,  12, 15,
710, 640,2,  13, 16,   0,4526,4,   0, 64, 720, 785,2,  19, 89,
725, 760,2,  10, 67, 730, 735,2,  11, 68,   0,4531,4,   0, 88,
110, 580,1,  16, 11, 745, 660,2,   9, 14, 750, 650,2,  12, 15,
755, 640,2,  13, 16,   0,4536,4,   0, 68, 110, 580,1,  16, 11,
770, 660,2,   9, 14, 775, 650,2,  12, 15, 780, 640,2,  13, 16,
0,4541,4,   0, 67, 100, 670,1,  10,  2,   0,4238,3,   2, 89,
0, 505,3,   4, 87, 805, 815,2,   4,163, 810, 540,2,  10, 57,
0,4546,4,   0,166, 500, 515,1,  41, 13, 825, 540,2,  10, 57,
0,4551,4,   0,163, 835, 840,2,  99,167,   0,4556,4,   0,162,
845, 670,1,  10, 12, 850, 615,1,  11, 66, 855, 515,1,  41, 58,
860, 580,1,  16,151, 110, 490,1,  86, 86, 870, 540,2,  10, 57,
0,4561,4,   0,168, 880, 760,2,  10, 67, 885, 735,2,  11, 68,
0,4566,4,   0,167, 895, 925,2,   9,143, 900, 410,2,  12,144,
905, 920,2,  70,173, 910, 915,2,  71,174,   0,4310,6,   0,137,
0,4478,3,   1,174,   0,4478,3,   1,-173, 110, 580,1,  16, 11,
0,4318,3,   2,143, 940,1135,2,  10,138, 945, 950,2,  11,139,
0,4571,4,   0,129, 955, 975,1,   7, 16, 960, 210,1,  87,147,
215, 205,1,  66,149, 970, 355,2,  87,146, 350, 360,2,  25,155,
980, 670,1,  10,  2, 985, 615,1,  11, 66, 990, 415,1,   7,140,
995, 210,1,  87,147,1000, 205,1,  66,149,1005, 580,1,  16,151,
1010, 285,1,  39,153,1015, 575,1, 166,169,1020, 565,1,  38,170,
1025, 555,1,  37,171,1030, 325,1,  80,176,1035, 315,1, 112,178,
160, 305,1, 127,179,1045,1190,1,  64, 17,1050,1155,1,  61, 36,
1055,1090,1,   4, 37,1060,1070,1,  19,128,1065, 760,1,  10, 67,
0, 735,1,  11, 68, 100, 670,1,  10,  2,1080, 760,2,  10, 67,
1085, 735,2,  11, 68,   0,4576,4,   0,128, 980, 670,1,  10,  2,
0,3866,3,   0,-34,   0,1105,1,  19, 18, 980, 670,1,  10,  2,
0,1115,1,  99, 19,   0,4310,6,   0,137,1125,1135,2,  10,138,
1130, 950,2,  11,139,   0,4581,4,   0, 37, 955, 975,1,   7, 16,
1145, 925,2,   9,143,1150, 410,2,  12,144,   0,4586,4,   0,138,
0,1160,1,  62, 20,   0,1165,1,  63, 21,   0,1170,1,  64, 22,
980, 670,1,  10,  2,   0,1180,1,  65, 23, 980, 670,1,  10,  2,
0,3866,3,   7, 36, 980, 670,1,  10,  2,   0,1200,1,  65, 23,
980, 670,1,  10,  2,   0,3866,3,   4, 35,1215,1235,1,   8, 24,
0,1220,1,  19,141, 980, 670,1,  10,  2,   0,1230,1,   8, 14,
0,4318,3,   4,141,   0,4318,3,   2,140,1245, 925,2,   9,143,
1250, 410,2,  12,144,   0,4591,4,   0,139,1260,1265,2,  29, 62,
0, 575,4,   0,169,1270, 670,1,  10,  2,1275, 615,1,  11, 66,
1280,1290,1,  38, 60,1285,1255,1, 166, 62, 120, 580,1,  16,151,
290, 295,2,  21, 33,1300,1305,2,  29, 60,   0, 570,4,   0,170,
1270, 670,1,  10,  2,   0,4058,3,   3, 61,1320, 760,2,  10, 67,
1325, 735,2,  11, 68,   0,4596,4,   0, 60,   0,4058,3,   2, 63,
1340, 760,2,  10, 67,1345, 735,2,  11, 68,   0,4601,4,   0, 62,
0,4462,3,   0,-160,1360,1190,2,  64, 35,1365,1155,2,  61, 36,
1370,1390,2,   4, 37,1375,1070,2,  19,128,1380, 760,2,  10, 67,
1385, 735,2,  11, 68,   0,4606,4,   0,165,1395, 670,1,  10,  2,
1400, 615,1,  11, 66,1405, 415,1,   7,140,1410, 580,1,  16,151,
1415, 210,1,  87,147,1420, 205,1,  66,149,1425, 575,1, 166,169,
1430, 565,1,  38,170,1435, 555,1,  37,171,1440, 325,1,  80,176,
1445, 315,1, 112,178,1450, 305,1, 127,179, 160, 285,1,  39,153,
1460,1190,2,  64, 35,1465,1155,2,  61, 36,1470,1090,2,   4, 37,
1475, 760,2,  10, 67,1480, 735,2,  11, 68,1485,1070,2,  19,128,
0,4611,4,   0,164,   0,3938,3,   0,-38,1500,1505,2,  17,161,
0,4616,4,   0, 71,1510, 670,1,  10, 12,1515, 615,1,  11, 66,
1520, 515,1,  41, 58, 105, 490,1,  86, 86,1375,1530,2,   4,164,
100, 670,1,  10,  2,1540, 760,2,  10, 67,1485, 735,2,  11, 68,
0,4462,3,   2,161,1555,1590,1,   8, 24,   0,1560,1,  19, 39,
1565,1575,1,  38, 25,   0,1570,1, 166, 62,   0,1265,1,  29, 26,
290, 295,2,  21, 33,   0,1305,1,  29, 26,   0,3938,3,   2, 39,
0,4478,3,   2,180,1600,1620,1,   8, 27,   0,1605,1,  18, 72,
1510, 670,1,  10, 12,1615,1505,2,  17,161,   0,4621,4,   0, 72,
0,4478,3,   2,172,   0,4318,3,   2,144,1060,1635,1,   8, 28,
0,4478,3,   3,177,   0,4478,3,   2,176,1060,1650,1,   8, 28,
0,4478,3,   3,178,1060,1660,1,  19, 29, 100, 670,1,  10,  2,
1060,1670,1,   8, 28,   0,4478,3,   5,179,1060,1680,1,  22, 30,
0,3838,3,   2, 33,   0,4438,3,   1,153, 220, 285,1,  39,  9,
1700, 360,2,  25,155,   0,4626,4,   0,148,1710, 360,2,  25,155,
0,4631,4,   0,147,1060,1720,1,  28, 31,   0, 185,3,   2,158,
1060,1730,1,  24, 32,   0, 185,3,   2,159,1740, 660,2,   9, 14,
1745, 650,2,  12, 15,1750, 640,2,  13, 16,1755, 580,2,  16,151,
1760, 575,2, 166,169,1765, 565,2,  38,170,1770, 555,2,  37,171,
1775, 415,2,   7,172,1780, 325,2,  80,176,1785, 315,2, 112,178,
1790, 305,2, 127,179,1795, 285,2,  39,153,1800, 210,2,  87,147,
1805, 205,2,  66,149,1810, 180,2,  68,132,1815, 175,2,  27,158,
1820,  95,2,  23,159,   0,4636,4,   0,  8,1830, 660,2,   9, 14,
1835, 650,2,  12, 15,1840, 640,2,  13, 16,   0,4641,4,   0,  9,
1850,1860,1,  14, 33,   0,1855,1,   3,  6,   0,3750,3,   0, -6,
0,3750,3,   0, -5,1870,3150,2,  39, 18,1875,3135,2, 152, 20,
1880,3130,2,  40, 21,1885,3055,2,  77, 27,1890,3040,2,  75, 28,
1895,2480,2,  72, 40,1900,2445,2,  74, 43,1905,2430,2,  76, 45,
1910,2415,2,  73, 46,1915,2360,2,  79, 48,1920,3115,6,  23, 29,
1925,2230,2,  58,116,1930,1575,2,  38, 60,1935,1570,2, 166, 62,
1940,2210,2,  78, 84,1945,2115,2,  83, 73,1950,2010,2, 157,118,
1955,2190,6,  81, 70,1960,1980,6,  21,117,1965,1980,6,  26,117,
1970, 515,2,  41, 58,1975, 490,2,  86, 86,   0,3770,6,   0, 10,
1985,1995,1,  21, 34,   0,1990,1,  26,121,   0,2335,3,   1,121,
1870,3150,2,  39, 18,2005, 540,2,  10, 57,   0,4646,4,   0,119,
2015, 670,2,  10, 65,2020, 615,2,  11, 66,2025, 580,2,  16,151,
2030, 575,2, 166,169,2035, 565,2,  38,170,2040, 555,2,  37,171,
2045, 415,2,   7,172,2050, 325,2,  80,176,2055, 315,2, 112,178,
2060, 305,2, 127,179,2065, 285,2,  39,153,2070, 210,2,  87,147,
2075, 205,2,  66,149,2080, 180,2,  68,132,2085, 175,2,  27,158,
2090,  95,2,  23,159,   0,4254,6,   0,122,2100, 760,2,  10, 67,
2105, 735,2,  11, 68,   0,4651,4,   0,123,   0,2335,3,   1,118,
1565,1575,1,  38, 25,1555,2125,1,  53, 35, 100, 670,1,  10,  2,
2135, 760,1,  10, 36,2140, 735,1,  11, 68,2145,2180,1,  84, 75,
0,2150,1,  56, 77,2155,2160,2,   9, 78,   0,4656,4,   0, 77,
0,2165,3,   1, 78, 100, 670,1,  10,  2,1060,2175,1,  84, 37,
0,2185,3,   2, 76,   0,2185,3,   0, 75,   0,2340,3,   4, 73,
0,2195,1,  81, 38,1510, 670,1,  10, 12,1600,2205,1,  57, 39,
0,2350,3,   3, 69,2215,2225,1,  38, 40,   0,2220,1,  39, 85,
0,2380,3,   1, 85,   0,2380,3,   1, 84,   0,4246,3,   0,116,
2240,2325,1,  54, 41,2245,2310,1,  87,114,2250,2270,1,  25,126,
2255, 670,1,  10, 65,2260, 615,1,  11, 66, 995, 415,1,   7,140,
1060,1070,1,  19, 29, 220,2285,1,  39,  9, 240, 275,1, 157,  5,
0,2235,3,   3,127, 290, 295,2,  21, 33,   0,2235,3,   3,126,
0,2300,1,  54, 42, 980, 670,1,  10,  2,   0,4246,3,   3,115,
210,2315,1,  54, 43, 980, 670,1,  10,  2,   0,4246,3,   3,114,
980, 670,1,  10,  2,   0,4246,3,   2,113,   0,4246,3,   0,-90,
0,2345,1,  23, 44,1870,3150,2,  39, 18,   0,2355,1,  23, 44,
1870,3150,2,  39, 18,2365,2375,1,  38, 40,   0,2370,1,  39, 49,
0,3125,3,   1, 49,   0,3125,3,   1, 48,2385,1860,2,  14,  5,
2390,1855,2,   3,  6,   0,3950,6,   0,-50,   0,3950,3,   0,-51,
0,2405,1,  23, 44,   0,2410,1,  24, 45,   0,3125,3,   3,-47,
500, 515,1,  41, 13,2425, 540,2,  10, 57,   0,4661,4,   0, 46,
500, 515,1,  41, 13,2440, 540,2,  10, 57,   0,4666,4,   0, 45,
2450,2455,2, 166, 82,   0,4671,4,   0, 43,   0,2460,3,   0, 82,
2465,2470,2,  19, 83,   0,4676,4,   0, 44,   0,2475,1, 166, 46,
0,2460,3,   2, 83,2485, 670,1,  10, 12,2490, 615,1,  11, 66,
2495, 515,1,  41, 58,2500, 580,1,  16,151,2505, 415,1,   7,140,
2510, 490,1,  86, 86,2515, 575,1, 166,169,2520, 565,1,  38,170,
2525, 555,1,  37,171,2530, 325,1,  80,176,2535, 315,1, 112,178,
2540, 305,1, 127,179,2545, 210,1,  87,147,2550, 205,1,  66,149,
160, 285,1,  39,153,2560, 540,2,  10, 57,2565,2570,2,  99, 80,
0,4226,6,   0, 79, 495,2575,1,  99, 47, 500, 515,1,  41, 13,
2585, 540,2,  10, 57,   0,4681,4,   0, 81,2595, 540,2,  10, 57,
0,4686,4,   0, 80,   0,3125,3,   2, 42,2565,2570,2,  99, 80,
0,3125,3,   2, 41,2620, 760,2,  10, 67,2625, 735,2,  11, 68,
2630,1190,2,  64, 35,2635,1155,2,  61, 36,2640,1090,2,   4, 37,
2560,1070,2,  19,128,   0,3125,3,   2, 40,2655,3020,2,  46, 91,
2660,3000,2,  47, 92,2665,2980,2,  48, 93,2670,2960,2,  49, 94,
2675,2940,2,  50, 95,2680,2920,2,  51, 96,2685,2910,2, 152, 97,
2690,2900,2, 132, 98,2695,2890,2,  59, 99,2700,2880,2,  69,100,
2705,2870,2, 148,101,2710,2865,2,  57,102,2715,2860,2,   5,103,
2720,2855,2,   6,104,2725,2850,2,  60,105,2730,2830,2,  56,107,
2735,2820,2,  52,108,2740,2810,2,  53,109,2745,2800,2,  54,110,
2750,2795,2, 142,111,2755,2780,2, 138,112,2760,2775,2,  55,125,
2765, 515,2,  41, 58,2770, 490,2,  86, 86,   0,4691,4,   0, 30,
0,2235,3,   1,-125, 500, 515,1,  41, 13,2790, 540,2,  10, 57,
0,4696,4,   0,112,   0,4246,3,   1,111, 980, 670,1,  10,  2,
0,4246,3,   2,110, 980, 670,1,  10,  2,   0,4246,3,   2,109,
980, 670,1,  10,  2,   0,4246,3,   2,108, 980, 670,1,  10,  2,
0,4246,3,   2,107,2845, 540,2,  10, 57,   0,4701,4,   0,106,
0,4246,3,   1,105,   0,4246,3,   1,104,   0,4246,3,   1,103,
0,4246,3,   1,102,2015, 670,2,  10, 65,   0,4246,3,   2,101,
2015, 670,2,  10, 65,   0,4246,3,   2,100,2015, 670,2,  10, 65,
0,4246,3,   2, 99,2015, 670,2,  10, 65,   0,4246,3,   2, 98,
2015, 670,2,  10, 65,   0,4246,3,   2, 97, 100, 670,1,  10,  2,
2930, 760,2,  10, 67,2935, 735,2,  11, 68,   0,4706,4,   0, 96,
100, 670,1,  10,  2,2950, 760,2,  10, 67,2955, 735,2,  11, 68,
0,4711,4,   0, 95, 100, 670,1,  10,  2,2970, 760,2,  10, 67,
2975, 735,2,  11, 68,   0,4716,4,   0, 94, 100, 670,1,  10,  2,
2990, 760,2,  10, 67,2995, 735,2,  11, 68,   0,4721,4,   0, 93,
100, 670,1,  10,  2,3010, 760,2,  10, 67,3015, 735,2,  11, 68,
0,4726,4,   0, 92, 100, 670,1,  10,  2,3030, 760,2,  10, 67,
3035, 735,2,  11, 68,   0,4731,4,   0, 91, 500, 515,1,  41, 13,
3050, 540,2,  10, 57,   0,4736,4,   0, 28, 500, 515,1,  41, 13,
3065, 540,2,  10, 57,   0,4741,4,   0, 27,3075,3565,2,  24, 26,
0,3080,6,   0, 74,   0,3085,1,  85, 48,1870,3150,2,  39, 18,
0,3095,1,  23, 44,1870,3150,2,  39, 18,3105,3110,2,  82, 53,
0,4746,4,   0, 24,   0,3090,3,   1, 53,   0,3120,1,  23, 44,
1870,3150,2,  39, 18,   0,3826,3,   0, 22,   0,3826,3,   0, 21,
0,3826,3,   0, 20,3145,1560,2,  19, 39,   0,4751,4,   0, 19,
290, 295,2,  21, 33,   0,3160,1,  20, 49,3165,2230,2,  58,116,
3170, 670,2,  10, 65,3175, 615,2,  11, 66,3180,2010,2, 157,118,
3185, 415,2,   7,140,3190, 210,2,  87,147,3195, 205,2,  66,149,
3200, 580,2,  16,151,3205, 515,2,  41, 58,3210, 285,2,  39,153,
3215, 575,2, 166,169,3220, 565,2,  38,170,3225, 555,2,  37,171,
3230, 325,2,  80,176,3235, 315,2, 112,178,3240, 305,2, 127,179,
3245, 490,2,  86, 86,3250, 180,2,  68,132,3255, 175,2,  27,158,
3260,  95,2,  23,159,   0,1980,6,   0,117,3270,3020,2,  46, 91,
3275,3000,2,  47, 92,3280,2980,2,  48, 93,3285,2960,2,  49, 94,
3290,2940,2,  50, 95,3295,2920,2,  51, 96,3300,2910,2, 152, 97,
3305,2900,2, 132, 98,3310,2890,2,  59, 99,3315,2880,2,  69,100,
3320,2870,2, 148,101,3325,2865,2,  57,102,3330,2860,2,   5,103,
3335,2855,2,   6,104,3340,2850,2,  60,105,3345,2830,2,  56,107,
3350,2820,2,  52,108,3355,2810,2,  53,109,3360,2800,2,  54,110,
3365,2795,2, 142,111,3370,2780,2, 138,112,3375,2775,2,  55,125,
3380, 515,2,  41, 58,3385, 490,2,  86, 86,   0,4756,4,   0, 31,
0,3826,3,   3, 18,   0,3826,3,   0, 17,   0,3770,3,   0, 11,
2385,1860,2,  14,  5,3415,3150,2,  39, 18,3420,3135,2, 152, 20,
3425,3130,2,  40, 21,3430,3055,2,  77, 27,3435,3040,2,  75, 28,
3440,2480,2,  72, 40,3445,2445,2,  74, 43,3450,2430,2,  76, 45,
3455,2415,2,  73, 46,3460,2360,2,  79, 48,3465,3115,6,  23, 29,
3470,2230,2,  58,116,3475,1575,2,  38, 60,3480,1570,2, 166, 62,
3485,2210,2,  78, 84,3490,2115,2,  83, 73,3495,2010,2, 157,118,
3500,2190,6,  81, 70,3505,1980,6,  21,117,3510,1980,6,  26,117,
3515, 515,2,  41, 58,3520, 490,2,  86, 86,   0,2395,4,   0, 51,
0,3770,3,   2, 12,   0,3535,1,  24, 45,   0,3826,3,   4, 23,
2385,1860,2,  14,  5,   0,3550,1,  24, 45,   0,3826,3,   4,-25,
2385,1860,2,  14,  5,   0,3070,3,   4, 55,   0,3826,3,   1,-26,
2385,1860,2,  14,  5,   0,3580,1,  24, 45,   0,3100,3,   4,-52,
2385,1860,2,  14,  5,   0,3070,3,   3,-54,2385,1860,2,  14,  5,
0,3605,3,   1,124,   0,3610,1,  22,  6,   0,2335,3,   3,120,
1845,3730,1,  45, 50,3625,3725,2,  45,  4,3630,3150,2,  39, 18,
3635,3135,2, 152, 20,3640,3130,2,  40, 21,3645,3055,2,  77, 27,
3650,3040,2,  75, 28,3655,2480,2,  72, 40,3660,2445,2,  74, 43,
3665,2430,2,  76, 45,3670,2415,2,  73, 46,3675,2360,2,  79, 48,
3680,3115,6,  23, 29,3685,2230,2,  58,116,3690,1575,2,  38, 60,
3695,1570,2, 166, 62,3700,2210,2,  78, 84,3705,2115,2,  83, 73,
3710,2010,2, 157,118,3715,2190,6,  81, 70,3720, 515,2,  41, 58,
3260, 490,2,  86, 86,   0,3735,3,   4,  4,   0,3735,3,   3,  3,
1850,1860,1,  14, 33,   0,  10,3,   2,  2,   0,   0,0,   2,  0,
3754,1865,5,1845    ,3758,2395,5,2380    ,3762,3620,5,3615    ,
3766,3740,5,3735    ,   0,3410,5,   0    ,3774,3595,5,1995    ,
3778,3585,5,2345    ,3782,3570,5,2355    ,3786,3555,5,3085    ,
3790,3540,5,3095    ,3794,3405,5,3120    ,   0,3615,5,   0    ,
3802,1735,5,  20    ,3806, 620,5, 615    ,3810, 675,5, 670    ,
3814, 740,5, 735    ,3818, 765,5, 760    ,3822,1825,5,1735    ,
0, 695,5,   0    ,3830,3525,5,3410    ,3834,3525,5,3620    ,
0,3400,5,   0    ,3842, 380,5, 375    ,3846, 570,5, 565    ,
3850,1295,5,1290    ,3854,1580,5,1575    ,3858,2290,5,2285    ,
3862,3155,5,3150    ,   0,1685,5,   0    ,3870,1100,5,1090    ,
3874,1110,5,1105    ,3878,1175,5,1170    ,3882,1185,5,1180    ,
3886,1195,5,1190    ,3890,1205,5,1200    ,3894,1225,5,1220    ,
3898,1100,5,1390    ,3902,2305,5,2300    ,3906,2320,5,2315    ,
3910,2330,5,2325    ,3914,2605,5,2480    ,3918,2805,5,2800    ,
3922,2815,5,2810    ,3926,2825,5,2820    ,3930,2835,5,2830    ,
3934,3390,5,3160    ,   0,1210,5,   0    ,3942,1550,5, 415    ,
3946,2120,5,2115    ,   0,3140,5,   0    ,3954,3530,5,3405    ,
3958,3545,5,3540    ,3962,3560,5,3555    ,3966,3575,5,3570    ,
3970,3590,5,3585    ,3974,3600,5,3595    ,   0,2400,5,   0    ,
3982, 800,5, 415    ,3986, 525,5, 495    ,3990, 820,5, 815    ,
3994, 865,5, 840    ,3998, 800,5,1505    ,4002, 800,5,1605    ,
4006, 800,5,2195    ,4010,2420,5,2415    ,4014,2435,5,2430    ,
4018,2555,5,2480    ,4022,2590,5,2570    ,4026,2580,5,2575    ,
4030,2840,5,2650    ,4034,2785,5,2780    ,4038,3045,5,3040    ,
4042,3060,5,3055    ,4046,2840,5,3265    ,   0,2000,5,   0    ,
4054, 545,5, 540    ,   0, 520,5,   0    ,4062,1330,5,1265    ,
4066,1310,5,1305    ,4070,1585,5,1560    ,   0,1490,5,   0    ,
4078,1725,5,  95    ,4082,1715,5, 175    ,4086,1675,5, 295    ,
4090,1655,5, 310    ,4094,1645,5, 320    ,4098,1630,5, 330    ,
4102,1355,5, 415    ,4106, 715,5, 550    ,4110, 715,5, 785    ,
4114, 875,5, 840    ,4118,1075,5,1070    ,4122,1335,5,1265    ,
4126,1315,5,1305    ,4130,1455,5,1390    ,4134,1525,5,1505    ,
4138,1535,5,1530    ,4142,1525,5,1605    ,4146,1665,5,1660    ,
4150,2095,5,2010    ,4154,2130,5,2125    ,4158,2170,5,2165    ,
4162,1525,5,2195    ,4166,2265,5,2235    ,4170,2615,5,2480    ,
4174,2095,5,2870    ,4178,2095,5,2880    ,4182,2095,5,2890    ,
4186,2095,5,2900    ,4190,2095,5,2910    ,4194,2925,5,2920    ,
4198,2945,5,2940    ,4202,2965,5,2960    ,4206,2985,5,2980    ,
4210,3005,5,3000    ,4214,3025,5,3020    ,   0,1040,5,   0    ,
4222,2200,5,2195    ,   0,1595,5,   0    ,4230,2610,5,2605    ,
4234,2645,5,2615    ,   0,2600,5,   0    ,4242, 790,5, 785    ,
0, 795,5,   0    ,4250,3265,5,3160    ,   0,2650,5,   0    ,
4258,2875,5,2870    ,4262,2885,5,2880    ,4266,2895,5,2890    ,
4270,2905,5,2900    ,4274,2915,5,2910    ,   0,2110,5,   0    ,
4282,2295,5,2235    ,   0,1095,5,   0    ,4290, 365,5, 360    ,
4294,2275,5,2270    ,   0, 235,5,   0    ,4302, 370,5, 365    ,
4306,2280,5,2275    ,   0, 280,5,   0    ,4314,1120,5,1115    ,
0, 935,5,   0    ,4322, 890,5, 415    ,4326,1240,5, 950    ,
4330, 890,5, 975    ,4334, 890,5,1090    ,4338, 890,5,1105    ,
4342,1140,5,1135    ,4346, 890,5,1170    ,4350, 890,5,1180    ,
4354, 890,5,1190    ,4358, 890,5,1200    ,4362, 890,5,1220    ,
4366, 890,5,1390    ,4370, 890,5,2235    ,4374, 890,5,2300    ,
4378, 890,5,2315    ,4382, 890,5,2325    ,4386, 890,5,2480    ,
4390, 890,5,2800    ,4394, 890,5,2810    ,4398, 890,5,2820    ,
4402, 890,5,2830    ,4406, 890,5,3160    ,   0, 390,5,   0    ,
4414,1625,5, 410    ,4418, 605,5, 600    ,4422, 645,5, 640    ,
4426, 655,5, 650    ,4430, 665,5, 660    ,4434, 930,5, 925    ,
0, 610,5,   0    ,4442,1705,5, 210    ,4446, 965,5, 950    ,
4450, 965,5,1135    ,4454,1695,5,1690    ,4458,1705,5,2310    ,
0, 335,5,   0    ,4466,1610,5,1605    ,   0,1495,5,   0    ,
4474,1545,5,1505    ,   0,1350,5,   0    ,4482, 585,5, 580    ,
0, 590,5,   0    ,   0,1845,3,   0,  7,   0,4286,3,   1,130,
0, 300,3,   0,145,   0, 505,3,   2, 86,   0,4478,3,   0,-171,
0,4410,3,   0,-150,   0,4074,3,   1, 66,   0,4074,3,   1, 65,
0,4074,3,   0,-64,   0,4238,3,   0, 88,   0,4074,3,   2, 68,
0,4074,3,   2, 67,   0, 830,3,   0,-166,   0,4470,3,   2,163,
0,4470,3,   0,162,   0, 830,3,   2,168,   0, 830,3,   2,167,
0,4278,3,   1,129,   0,4278,3,   2,128,   0,3866,3,   6, 37,
0,4310,3,   2,138,   0,4310,3,   2,139,   0,4058,3,   3, 60,
0,4058,3,   2, 62,   0, 830,3,   0,-165,   0,4470,3,   2,164,
0,4218,3,   0,-71,   0,4218,3,   2, 72,   0, 300,3,   2,148,
0, 300,3,   1,147,   0,1845,3,   1,  8,   0,1845,3,   2,  9,
0,2335,3,   0,119,   0,4254,3,   0,-123,   0,2165,3,   0,-77,
0,3125,3,   1, 46,   0,3125,3,   1, 45,   0,3125,3,   0, 43,
0,3125,3,   1,-44,   0,4226,3,   2, 81,   0,4226,3,   1, 80,
0,3395,3,   0, 30,   0,4246,3,   2,112,   0,4246,3,   1,106,
0,4246,3,   2, 96,   0,4246,3,   2, 95,   0,4246,3,   2, 94,
0,4246,3,   2, 93,   0,4246,3,   2, 92,   0,4246,3,   2, 91,
0,3826,3,   1, 28,   0,3826,3,   1, 27,   0,3826,3,   0,-24,
0,3826,3,   0, 19,   0,3395,3,   3, 31

};
lxinx entryhp[ordMAXCH + 1]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,2,
3,0,0,0,4,5,0,6,8,100,0,0,0,0,0,0,0,0,0,0,101,0,102,105,106,0,0,0,0,0,0,0,0,
0,107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,110,0,0,0,0,0,111,144,168,213,240,
260,0,277,283,0,0,293,319,338,339,347,0,357,377,415,443,0,451,0,0,0,0,459,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
lxinx lxhp[lxmax + 1]={
0,0,0,0,0,0,0,0,95,88,81,76,70,63,57,48,38,28,21,0,0,26,23,24,25,0,27,0,31,30,
0,32,33,34,35,36,37,0,0,45,42,0,43,44,0,46,47,0,53,50,51,52,0,54,55,56,0,0,60,
0,61,62,0,64,69,66,67,68,0,0,72,0,73,74,75,0,79,78,0,80,0,82,83,84,85,86,87,
0,89,90,91,94,93,0,0,96,97,98,99,0,0,0,104,0,0,0,0,108,109,0,0,140,138,137,122,
120,117,118,119,0,121,0,134,124,125,126,130,128,129,0,132,0,133,0,135,136,0,
0,139,0,142,0,143,0,160,147,0,157,149,156,154,152,153,0,155,0,0,158,159,0,166,
162,163,164,165,0,167,0,212,206,204,197,174,0,189,185,180,179,0,0,181,182,183,
184,0,186,187,188,0,190,195,192,193,194,0,196,0,198,199,200,201,202,203,0,205,
0,207,208,211,210,0,0,0,233,229,223,217,220,219,0,221,222,0,224,225,226,227,
228,0,230,231,232,0,234,235,239,237,238,0,0,248,247,243,246,245,0,0,0,251,250,
0,252,253,254,255,259,257,258,0,0,270,267,266,264,265,0,0,268,269,0,271,272,
276,274,275,0,0,279,0,280,281,282,0,0,285,0,287,288,289,290,291,292,0,317,315,
303,300,298,299,0,301,302,0,304,305,314,310,308,309,0,311,312,313,0,0,316,0,
318,0,330,329,322,323,324,328,326,327,0,0,0,331,332,333,337,335,336,0,0,0,0,
341,342,343,344,345,346,0,355,353,350,351,352,0,354,0,356,0,371,0,368,365,362,
363,364,0,366,367,0,369,370,0,374,373,0,375,376,0,413,409,405,402,399,390,388,
385,386,387,0,389,0,396,392,393,394,395,0,397,398,0,400,401,0,404,0,0,406,407,
408,0,410,411,412,0,414,0,442,429,420,419,0,428,422,423,424,425,426,427,0,0,
430,431,441,436,434,435,0,437,438,439,440,0,0,0,445,0,446,447,448,449,450,0,
458,453,456,455,0,457,0,0,0};
lxinx lxnp[lxmax + 1]={
0,0,0,0,0,0,7,0,9,10,11,12,13,14,15,16,17,18,19,20,0,22,0,0,0,0,0,0,29,0,0,0,
0,0,0,0,0,0,39,40,41,0,0,0,0,0,0,0,49,0,0,0,0,0,0,0,0,58,59,0,0,0,0,0,65,0,0,
0,0,0,71,0,0,0,0,0,77,0,0,0,0,0,0,0,0,0,0,0,0,0,0,92,0,0,0,0,0,0,0,0,0,0,103,
0,0,0,0,0,0,0,0,112,113,114,115,116,0,0,0,0,0,0,123,0,0,0,127,0,0,0,131,0,0,
0,0,0,0,0,0,0,141,0,0,0,145,146,0,148,0,150,151,0,0,0,0,0,0,0,0,0,161,0,0,0,
0,0,0,0,169,170,171,172,173,0,175,176,177,178,0,0,0,0,0,0,0,0,0,0,0,0,191,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,209,0,0,0,0,214,215,216,0,218,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,236,0,0,0,0,241,242,0,244,0,0,0,0,249,0,0,0,0,0,0,256,0,
0,0,0,261,262,263,0,0,0,0,0,0,0,0,0,273,0,0,0,0,278,0,0,0,0,0,284,0,286,0,0,
0,0,0,0,0,294,295,296,297,0,0,0,0,0,0,0,0,306,307,0,0,0,0,0,0,0,0,0,0,0,0,320,
321,0,0,0,325,0,0,0,0,0,0,0,0,334,0,0,0,0,0,340,0,0,0,0,0,0,0,348,349,0,0,0,
0,0,0,0,0,358,359,360,361,0,0,0,0,0,0,0,0,0,0,372,0,0,0,0,0,378,379,380,381,
382,383,384,0,0,0,0,0,0,391,0,0,0,0,0,0,0,0,0,0,0,403,0,0,0,0,0,0,0,0,0,0,0,
0,416,417,418,0,0,421,0,0,0,0,0,0,0,0,0,0,432,433,0,0,0,0,0,0,0,0,0,0,444,0,
0,0,0,0,0,0,452,0,454,0,0,0,0,0,0};
symbol lxtv[lxmax + 1]={
0,101,35,17,33,31,32,150,93,98,0,94,0,95,92,94,93,92,95,70,71,0,0,107,0,107,
0,95,0,0,92,0,0,110,0,0,0,110,89,0,0,91,0,0,96,0,0,93,0,0,0,0,94,108,0,0,108,
88,0,90,0,0,92,0,0,111,0,0,111,95,0,106,0,0,0,106,0,0,97,0,94,0,0,109,0,0,0,
109,0,0,0,0,0,98,98,0,93,0,0,93,34,30,149,103,151,100,102,0,0,66,26,0,0,0,0,
0,54,0,0,128,0,115,161,0,0,163,0,0,0,169,0,168,0,184,0,0,167,65,0,114,0,113,
0,146,0,0,56,0,158,0,0,0,0,172,0,171,170,0,0,93,0,0,0,0,0,64,0,147,0,0,0,0,0,
5,0,0,0,0,116,73,0,0,0,0,58,0,0,0,77,0,139,0,139,0,139,0,139,0,0,0,159,0,0,173,
0,59,0,0,0,0,143,143,6,0,0,0,84,0,0,154,0,0,134,0,49,0,0,0,49,0,0,0,78,0,0,0,
0,0,174,135,0,0,0,0,117,118,75,97,0,0,82,0,0,0,160,0,0,0,176,175,0,0,0,0,0,52,
83,0,0,126,0,69,0,0,0,185,69,0,46,0,0,0,46,81,0,119,0,0,136,0,0,0,136,0,0,0,
0,0,120,121,0,0,144,0,162,0,0,0,0,178,0,0,0,186,177,0,156,0,68,0,0,0,0,164,0,
0,0,180,179,130,129,0,0,0,0,0,188,187,67,61,0,0,0,0,0,140,140,0,0,0,0,0,72,0,
131,0,137,0,67,0,0,0,0,0,145,0,0,155,0,0,74,48,0,80,0,0,48,0,0,76,0,0,0,0,67,
0,0,96,0,124,0,0,0,0,0,86,0,0,165,0,0,133,0,123,122,0,0,141,141,0,0,189,51,0,
60,0,0,67,53,92,62,0,0,50,0,0,0,50,57,0,0,0,0,0,0,183,0,0,0,0,182,181,125,0,
153,0,0,79,0,0,79,0,0,47,0,55,0,47,63,18};
symbol entrytv[ordMAXCH + 1]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,41,42,43,
13,36,28,7,8,9,10,19,11,25,12,0,0,0,0,0,0,0,0,0,0,20,14,4,29,104,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,0,22,15,0,27,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Char lxch[lxmax + 1]={
'0','=','=','&','=','=','=','>','b','c','d','e','h','l','n','r','s','t','w',
'x','y','e','i','d','t','h','s','t','h','o','p','i','c','k','n','e','s','s',
'e','o','t','w','a','r','t','u','t','h','a','i','g','h','t','d','i','u','s',
'e','o','w','r','t','h','e','f','n','g','t','h','t','e','t','i','g','h','t',
'a','n','d','s','t','i','a','m','e','t','e','r','e','n','t','e','r','e','r',
'o','t','t','o','m','=','=','-','=','>','=','=','e','r','e',']','b','c','n',
'r','s','t','a','n','2','i','n','c','r','o','w','h','w','i','d','e','t','a',
'd','r','a','d','d','o','s','o','s','v','e','e','o','y','t','x','h','r','w',
'i','d','a','d','t','t','o','m','l','t','w','e','e','n','o','w','c','e','h',
'i','o','w','l','m','n','p','s','y','t','i','n','u','e','m','a','n','d','o',
'r','u','r','e','d','e','d','r','c','l','e','r','a','d','o','p','n','t','e',
'r','e','r','w','a','e','i','o','t','w','n','t','e','d','a','m','e','t','e',
'r','f','i','n','e','s','h','e','w','i','d','d','l','n','x','e','p','e','c',
'd','l','s','e','i','p','s','e','h','w','i','d','t','i','l','o','r','o','m',
'r','o','o','r','l','l','e','v','a','l','d','e','t','i','g','h','t','f','n',
't','v','i','s','i','b','l','e','a','e','i','j','o','g','e','u','s','t','n',
'e','h','t','w','i','d','h','i','c','k','t','f','t','s','t','a','i','o','v',
'e','h','w','i','d','t','n','x','p','s','h','w','i','d','t','d','f','u','t',
'l','i','n','e','d','a','m','r','i','n','t','o','d','t','h','a','d','e','i',
'j','u','s','t','g','h','t','s','e','t','d','n','d','i','u','s','a','c','h',
'i','o','p','q','t','a','r','t','r','t','l','r','i','n','t','f','i','n','e',
'l','i','d','g','n','n','a','d','e','d','a','l','e','d','m','e','a','e','h',
'o','p','e','i','c','k','n','e','s','s','n','x','t','h','o','w','i','d','f',
'f','s','e','t','t','n','n','p','d','e','f','i','n','e','a','i','d','t','h',
't','h','y','|'};
/* integer debugging constants */
/*D debuglevel, linesignal: integer; D*/
/*D trace: boolean; D*/
/* used for debugging the parser */

/* Parser variables */
int oldsymb;   /* last lexical symbol */
arg *macros, *args;   /* macro and macro argument list */
stackinx stacktop, pseudotop, validtop, top;
stackelm *parsestack;   /* parse stack */
boolean parsestop;   /* parser flags */
int startinx, lri, start;


/*--------------------------------------------------------------------*/

/* include dp1.z */
/*dp1.x---------------------------------------------------------------*/
int Ceil(double x);

int Floor(double x);

double Max(double x, double y);

double Min(double x, double y);

double datan(double y, double x);

primitive *findenv(primitive *p);

arg *findmacro(arg *p, Char *chb, chbufinx inx, chbufinx length,
		      arg **last);

boolean isthen(primitive *pr);

double linlen(double x, double y);

int lspec(int n);

/*DGHM function ordp(p:pointer): integer; forward; MHGD*/
double principal(double x, double r);

void checkjust(nametype *tp, boolean *A, boolean *B, boolean *L,
		      boolean *R);

void getlinespec(primitive *nd, int *lsp, primitive **lastnd);

void consoleflush(void);

void copyleft(fbuffer *mac, fbuffer **buf);

void deletetree(primitive **p);

/*D; loc: integer D*/
void disposebufs(fbuffer **buf);

void drawtree(double n, double s, double e, double w, primitive *eb);

void fatal(int t);

void initnesw(void);

void doprod(int prno);

void markerror(int n);

void nesw(primitive *ptmp);

void newarg(arg **arg_);

void newbuf(fbuffer **buf);

#ifndef SAFE_MODE
void pointinput(nametype *txt);

void pointoutput(boolean nw, nametype *txt, int *ier);

#endif
void produce(stackinx newp, int p);

void readfor(fbuffer *p0, int attx, fbuffer **p2);

void skiptobrace(void);

/*D procedure snaptype(var iou: text; p: integer); forward; D*/
void wcoord(FILE **iou, double x, double y);

void wfloat(FILE **iou, double y);

void wpair(FILE **iou, double x, double y);

void wpos(postype pos);

/*D procedure wrbuf(p: fbufferp; job,r: integer); forward; D*/
/*D procedure wlogfl( nm: string; v: real; cr: integer); forward; D*/
void wstring(FILE **iou, nametype *p);

int ahlex(int atyp);

int ahnum(int atyp);

int pahlex(int atyp, int alex);

int pahnum(int atyp, int anum);

postype arcstart(primitive *n);

postype arcend(primitive *n);

fbuffer *newinbuf(fbuffer *p);


/*--------------------------------------------------------------------*/

/* include sysdep.h */
/*GH#include 'sysdep.h'HG*/

double principal(double x, double r)
{
  /* x,r: real): real */
  while (x > r)
    x -= 2 * r;
  while (x < -r)
    x += 2 * r;
  return x;
}


double Max(double x, double y)
{
  /* x,y: real): real */
  if (y > x)
    return y;
  else
    return x;
}


double Min(double x, double y)
{
  /* x,y: real): real */
  if (y < x)
    return y;
  else
    return x;
}


int Floor(double x)
{
  /* x: real): integer */
  if (x >= 0 || (long)x == x)
    return ((long)x);
  else
    return ((long)x - 1);
}


int Ceil(double x)
{
  /* x: real): integer */
  if (x < 0 || (long)x == x)
    return ((long)x);
  else
    return ((long)x + 1);
}


double datan(double y, double x)
{
  /* y,x: real ): real */
  double r;

  /*D if debuglevel=2 then write(log,'datan(',y:8:4,',',x:8:4,')='); D*/
  r = atan2(y , x);
  /*D if debuglevel=2 then writeln(log,r:8:4); D*/
  return r;
}


void consoleflush(void)
{
  /* causes immediate physical write to console, not
                               needed for most systems. */
  /*DUGHM; if debuglevel > 0 then flush(log) MHGUD*/
  fflush(errout);
  P_ioresult = 0;
}


void epilog(void)
{
  /* produce(0, 0); */
  /*D if debuglevel > 0 then begin
        writeln(log,'stackhigh=',stackhigh:1);
        writeln(log,'Dpic log ends');
        writeln(log) end; D*/
  /* Seems needed for some Cygwin machines: */
  /* GH flush(errout);
  flush(stdout) HG */
}


void currentline(int *startlin)
{  /* Print out the current input line */
  int inx, j, lastinx;
  fbuffer *bf, *ba, *WITH;

  putc('\n', errout);
  consoleflush();
  bf = inbuf;
  ba = NULL;
  /*D if debuglevel > 0 then begin
     write(log,'currentline:'); wrbuf(bf,3,0) end; D*/
  while (bf != NULL) {
    WITH = bf;
    ba = bf;
    j = 0;
    if (bf->readx - 1 < bf->savedlen)
      inx = bf->readx - 1;
    else
      inx = bf->savedlen;
    while (inx > j) {
      if (WITH->carray[inx] == etxch || WITH->carray[inx] == tabch ||
	  WITH->carray[inx] == ' ' ||
	  WITH->carray[inx] == crch || WITH->carray[inx] == nlch)
	inx--;
      else
	j = inx;
    }
    /*D if debuglevel > 0 then write(log,'skipwhite inx=',inx:1); D*/
    if (bf == inbuf)
      lastinx = inx;
    j = 0;
    while (inx > j) {
      if (WITH->carray[inx] != nlch)
	inx--;
      else
	j = inx;
    }
    /*D if debuglevel > 0 then begin write(log,' skipnonwhite inx=',inx:1);
       if bf<>inbuf then wrbuf(bf,3,-inx) else writeln(log) end; D*/
    if (inx == 0)
      bf = WITH->prevb;
    else
      bf = NULL;
  }

  *startlin = inx;
  while (ba != NULL) {
    WITH = ba;
    if (ba == inbuf)
      j = lastinx;
    else
      j = WITH->savedlen;
    while (inx < j) {
      inx++;
      if (WITH->carray[inx] == nlch)
	putc('\n', errout);
      else if (WITH->carray[inx] != etxch)
	putc(WITH->carray[inx], errout);
    }
    if (ba == inbuf)
      ba = NULL;
    else
      ba = ba->nextb;
    inx = 0;
  }
  putc('\n', errout);
  consoleflush();
}


void fatal(int t)
{
  /* t: integer */
  int i;

  if (t != 0) {
    fprintf(errout, " *** dpic: ");
    currentline(&i);
  }
  switch (t) {

  case 0:
    /* blank case */
    break;

  case 1:
    fprintf(errout, "Input file not readable\n");
    break;

  case 3:
    fprintf(errout, "Maximum error count exceeded\n");
    break;

  case 4:
    fprintf(errout, "Character buffer overflow: \"CHBUFSIZ\" exceeded\n");
    break;

  case 5:
    fprintf(errout, "End of file encountered on input\n");
    break;

  case 6:
    fprintf(errout,
	    "Too many pending actions, const \"STACKMAX\" exceeded,\n");
    fprintf(errout, " possibly caused by infinite recursion.\n");
    break;

  case 7:
    fprintf(errout, "Input too complex, const \"REDUMAX\" exceeded\n");
    break;

  case 8:
    fprintf(errout, "Error recovery abandoned\n");
    /*D; 9: writeln(errout,'Debug special exit') D*/
    break;
  }

  epilog();
  if (input != NULL) fclose(input);
  if (output != NULL) fclose(output);
  if (errout != NULL) fclose(errout);
  /*DP2CC if (log_ != NULL) fclose(log_); */
  exit(EXIT_FAILURE);

}


/*--------------------------------------------------------------------*/

/* include sysdep.h */
/* sysdep.x Required UNIX functions */

/*H name 'access' H*/
/*G; asmname '_p_Access' G*/
extern int access(Char *f, int n);

/*H name H*/
/*G; asmname G*/
/*GH 'isatty' HG*/

/*H name H*/
/*G; asmname G*/
/*GH 'time' HG*/

/*BXUGH real HGUXB*/
/*H name H*/
/*G; asmname G*/
/*GH 'sprintf' HG*/

/*BXUGH real HGUXB*/
/*H name H*/
/*G; asmname G*/
/*GH 'snprintf' HG*/

/* function system( var s: char ): integer; */
/*H name H*/
/*G; asmname G*/
/*GH 'system' HG*/

/* These need tweaking for different operating systems: */
/*H name H*/
/*G; asmname G*/
/*GH 'random' HG*/
#if defined __MSDOS__ || defined __CYGWIN__ || defined RAND 
#undef random
#define random() rand()
#else
extern long random(void);
#if !defined RAND_MAX
#undef randmax
#define randmax LONG_MAX
#endif
#endif

/*H name H*/
/*G; asmname G*/
/*GH 'srandom' HG*/
#if defined _POSIX_SOURCE || defined sun
extern void srandom(unsigned s);
#elif defined __MSDOS__ || defined __CYGWIN32__ || defined RAND 
extern void srand(unsigned s);
#define srandom(x) srand((unsigned)(x))
#elif defined mips
#elif defined __APPLE__
#elif defined __OpenBSD__
extern void srand(unsigned int s);
#else
extern void srandom(int s);
#endif


/*DUBX function ordp(var p:primitivep ): integer; external; XBUD*/
/*-----------------------------------------------------------------*/

/* include dpic1.p */
/*BX segment dpic1; XB*/
/* onefile */
/* G module dpic1; G */

/* include dp0.h */
/*BXU#include 'dp0.h'UXB*/
/* onefile */
/* G#include 'dp0.h'G */

/* include dp1.h */
/*BXU#include 'dp1.h'UXB*/
/* onefile */
/* G#include 'dp1.h'G */

/* include sysdep.h */
/*BXU#include 'sysdep.h'UXB*/
/* onefile */
/* G#include 'sysdep.h'G */

/* Recursive routines: snaptree pexit neswrec shift treedraw scaleobj */

int bval(Char *buf)
{
  return (((int) buf[0]) << 7) + (int) buf[1] ;
}


void putbval(Char *buf, int n)
{
  /* D
    if debuglevel > 0 then writeln(log,'putbval[',ordp(buf):1,'](',n:1,')'); D */
  buf[0] = (Char)(n>>7); buf[1] = (Char)(n % 128);
}


void deletename(nametype **head)
{
  nametype *pn, *r;
  int j, FORLIM;

  while (*head != NULL) {
    pn = *head;
    r = pn;
    while (pn->next_ != NULL) {
      r = pn;
      pn = pn->next_;
    }
    r->next_ = NULL;
    if (pn == *head)
      *head = NULL;
    if (pn->segmnt != NULL) {
      if (bval(pn->segmnt) > 1) {
	j = bval(pn->segmnt);
	putbval(pn->segmnt, j - 1);
	if (pn->segmnt == freeseg && pn->seginx + pn->len == freex) {
	  freex = pn->seginx;
	  j = 3;
	  while (freex > j) {
	    if (pn->segmnt[freex - 1] == nlch)
	      freex--;
	    else
	      j = freex;
	  }
	} else {
	  FORLIM = pn->seginx + pn->len;
	  for (j = pn->seginx; j < FORLIM; j++)
	    pn->segmnt[j] = nlch;
	}
      } else if (pn->segmnt == freeseg) {
	Free(freeseg);
	freeseg = NULL;
      } else
	Free(pn->segmnt);
    }
    Free(pn);
  }
}


/*
procedure setvis(var specv: integer; nonz: integer);
begin
   specv := (specv div 32)*32 + nonz*16 + (specv mod 16)
   end; */

void setspec(int *specv, int svalue)
{
  *specv = ((*specv) >> 3) * 8 + svalue - XLlinetype;
  /* if svalue = XLsolid then
     specv := (specv div 32)*32 + 16 + (specv mod 16) */
}


void resetspec(int *specv, int svalue)
{
  *specv = 0;
  setspec(specv, svalue);
}


void setthen(int *specv)
{
  *specv = ((*specv) >> 4) * 16 + ((*specv) & 7) + 8;
}


/*D
procedure prtstval(st: integer);
begin
write(log,'state=',st:1);
if (st mod 4)<>0 then
case (st mod 4) of
   1: write(log,',XL','to');
   2: write(log,',XL','from');
   3: write(log,',XL','at');
   otherwise
   end;
if odd(st div 4) then write(log,',XL','chop');
if odd(st div 8) then write(log,',XL','directon')
end;

procedure snapname( chbu:chbufp; inx,ll:chbufinx);
var j: integer;
begin
write(log,' (',ordp(chbu):1,' inx=',inx:1,' len=',ll:1,')|');
if chbu = nil then write(log,'**nil string pointer**')
else for j := inx to inx+ll-1 do write(log,chbu^[j]);
write(log,'|'); flush(log)
end;

procedure snaptype D*/
/* var iou: text; p: integer */
/*D;
begin
   case p of
      XLbox: write(iou,'<box>');
      XBLOCK: write(iou,'<[]>');
      XLellipse: write(iou,'<ellipse>');
      XLcircle: write(iou,'<circle>');
      XLline: write(iou,'<line>');
      XLarrow: write(iou,'<arrow>');
      XLmove: write(iou,'<move>');
      XLspline: write(iou,'<spline>');
      XLarc: write(iou,'<arc>');
      XLstring: write(iou,'<<string>>');
      XLaTeX: write(iou,'<<LaTeX>>');
      XLabel: write(iou,'<<Label>>');
      otherwise write(iou,'Unknown type ',p:3)
      end;
   flush(iou)
   end;

procedure snaptree(pr: primitivep; indent: integer);
var i: integer;
begin
   while (pr <> nil) and (indent<=240) do with pr^ do begin
      snaptype(log,ptype);
      case ptype of
         XBLOCK: i := 4;
         XLbox,XLarc: i := 5;
         XLline,XLmove: i := 6;
         XLarrow: i := 7;
         XLcircle,XLspline: i := 8;
         XLellipse,XLaTeX: i := 9;
         XLstring: i := 10;
         XLabel: i := 12;
         otherwise i := 3
         end;
      snaptree(next,indent+i);
      if son <> nil then begin
         writeln(log);
         write(log,' ':indent);
         end;
      pr := son
      end
   end;

procedure printptype(ptype: integer);
begin
   case ptype of
   XLline: write(log,'XLline'); XLarrow: write(log,'XLarrow');
   XLmove: write(log,'XLmove'); XLspline: write(log,'XLspline');
   XLbox: write(log,'XLbox'); XLstring: write(log,'XLstring');
   XBLOCK: write(log,'XBLOCK'); XLcircle: write(log,'XLcircle');
   XLellipse: write(log,'XLellipse'); XLarc: write(log,'XLarc');
   otherwise write(log, ptype:4)
   end
   end;

procedure printcorner(lexv: integer);
begin
   case lexv of
   XDn: writeln(log,'.n'); XDs: writeln(log,'.s');
   XDe: writeln(log,'.e'); XDw: writeln(log,'.w');
   XDnw: writeln(log,'.nw'); XDne: writeln(log,'.ne');
   XDse: writeln(log,'.se'); XDsw: writeln(log,'.sw');
   XDc: writeln(log,'.c');
   XDstart: writeln(log,'.start'); XDend: writeln(log,'.end')
   otherwise writeln(log,'unknown corner lexv=',lexv:1)
   end
   end;

procedure printtext(namptr: strptr);
begin
   while namptr <> nil do with namptr^ do begin
      write(log,' ');
      D*/
/*DBUMX wpair(log,ord(namptr),ord(next)); XMUBD*/
/*DGH wpair(log,ordp(namptr),ordp(next)); HGD*/
/*D
write(log,' val='); wfloat(log,val); flush(log);
snapname(segmnt,seginx,len);
namptr := next; writeln(log) end
   end;

procedure printobject(primp: primitivep);
var
   xx,yy: real;
   i: integer;
begin
   if debuglevel = 0 then begin end
   else if primp = nil then writeln(log,'Object is nil')
   else while primp <> nil do with primp^ do begin
write (log,'Object(',ordp(primp):1,
            ') Parent(',ordp(parent):1);
if parent<>nil then write(log,
            ') Parent^.son(',ordp(parent^.son):1);
writeln(log,') Son(', ordp(son):1,
            ') Next(', ordp(next):1, ')');
if name<>nil then begin write(log,' name: '); printtext(name) end;
if outlinep<>nil then begin
   write(log,' outline:'); printtext(outlinep) end;
if shadedp<>nil then begin
   write(log,' shaded:'); printtext(shadedp) end;
if textp<>nil then printtext(textp);
write(log,' aat'); wpair(log,aat.xpos,aat.ypos);
wlogfl('lparam',lparam,0);
wlogfl('lthick',lthick,0);
case direction of
   XLup: write(log,' <up>');
   XLdown: write(log,' <down>');
   XLleft: write(log,' <left>');
   XLright: write(log,' <right>');
   otherwise write(log,' dir =',direction:1)
   end;
write(log,' spec=',spec:1);
write(log,' type='); snaptype(log,ptype);
writeln(log,'(',ptype:1,')' );
flush(log);
case ptype of
   XLbox,XLstring: begin
      wlogfl('boxfill',boxfill,0);
      wlogfl('boxheight',boxheight,0); wlogfl('boxwidth',boxwidth,0);
      wlogfl('rad',boxradius,0) end;
   XBLOCK: begin
      wlogfl('blockheight',blockheight,0);
      wlogfl('blockwidth',blockwidth,0);
      write(log,' here='); wpair(log,here.xpos,here.ypos); writeln(log);
      write(log,' vars= ');
      for i:=0 to HASHLIM do begin
         if vars[i] = nil then write(log,' ',i:1,' nil;') D*/
/*DBUMX else write(log,' ',i:1,' ',ord(vars[i]):1,';'); XMUBD*/
/*DGH else write(log,' ',i:1,' ',ordp(vars[i]):1,';'); HGD*/
/*D
end;
            writeln(log);
            write(log,' env=');
if env = nil then write(log,'nil') D*/
/*DBUMX else write(log,ord(env):1) XMUBD*/
/*DGH else write(log,ordp(env):1) HGD*/
/*D
            end;
         XLcircle: begin
            wlogfl('cfill',cfill,0); wlogfl('radius',radius,0) end;
         XLellipse: begin
            wlogfl('efill',efill,0);
            wlogfl('elheight',elheight,0); wlogfl('elwidth',elwidth,0) end;
         XLline,XLarrow,XLmove,XLspline: begin
            write(log,' endpos='); wpair(log,endpos.xpos,endpos.ypos);
            wlogfl('height',height,0); wlogfl('width',width,0);
            wlogfl('lfill',lfill,0); wlogfl('aradius',aradius,0);
            write(log,' ahlex(atype)=',ahlex(atype):1);
            write(log,' ahnum(atype)=',ahnum(atype):1)
            end;
         XLarc: begin
            write(log,' lspec=',lspec(spec):1);
            wlogfl('lfill',lfill,0); wlogfl('aradius',aradius,0);
            write(log,' (|','startangle|,|','arcangle|)(deg)=');
            wpair(log,endpos.xpos*180.0/pi,endpos.ypos*180.0/pi); writeln(log);
            write(log,' (from)=');
            wpair(log,aat.xpos+aradius*cos(endpos.xpos),
       aat.ypos+aradius*sin(endpos.xpos));
            write(log,' (to)=');
            xx := aat.xpos+aradius*cos(endpos.xpos+endpos.ypos);
            yy := aat.ypos+aradius*sin(endpos.xpos+endpos.ypos);
            wpair(log,xx,yy);
            write(log,' ahlex(atype)=',ahlex(atype):1);
            write(log,' ahnum(atype)=',ahnum(atype):1)
            end;
         XLaTeX,XLabel: ;
         otherwise writeln(log,' Bad case in printobject; this cannot happen')
         end;
      writeln(log); flush(log);
      if ptype = XBLOCK then primp := nil
      else primp := son
      end;
   flush(log)
   end;

D*/
/*
procedure setenvironment(n: integer);
begin
   chbuf^[chbufi] := "d";
   chbuf^[chbufi+1] := "p";
   chbuf^[chbufi+2] := "i";
   chbuf^[chbufi+3] := "c";
   chbuf^[chbufi+4] := "w";
   chbuf^[chbufi+5] := "i";
   chbuf^[chbufi+6] := "d";
   chbuf^[chbufi+7] := "=";
   chbuf^[chbufi+8] := "0";
   chbuf^[chbufi+9] := ";";
   chbuf^[chbufi+10] := "d";
   chbuf^[chbufi+11] := "p";
   chbuf^[chbufi+12] := "i";
   chbuf^[chbufi+13] := "c";
   chbuf^[chbufi+14] := "h";
   chbuf^[chbufi+15] := "t";
   chbuf^[chbufi+16] := "=";
   chbuf^[chbufi+17] := "0";
   with attstack^[n+1] do begin
      chbufx := chbufi;
      length := 18;
      end;
   redubuf[reduinx].prod := element12;
   redubuf[reduinx-1].prod := string1;
   redubuf[reduinx-1].newtop := n+1;
   reduinx := reduinx - 2
   end;
*/
/*D

procedure prvars(np: integer);
var lv: strptr;
   i,x: integer;
begin
   with attstack^[np] do if envblock = nil then
      write(log,' vars=nil: nil envblock')
   else begin
      i := 0; x := HASHLIM+1;
      while i < x do if envblock^.vars[i]<>nil then x := i else i := i+1;
      writeln(log,' vars=');
      if x > HASHLIM then writeln(log,' None set') else
      for i:=0 to HASHLIM do begin write(log,' ',i:1);
         lv := envblock^.vars[i];
         if lv = nil then write(log,' nil');
         while lv <> nil do begin
            write(log,' (',ordp(lv):1,',',ordp(lv^.next):1,')=');
            with lv^ do begin
               snapname(segmnt,seginx,len);
               write(log,'='); wfloat(log,val)
               end;
            D*/
/* writeln(log); */
/*D
            lv := lv^.next
            end;
         writeln(log)
         end
      end
   end;

D*/

void deletetree(primitive **p)
{
  /* var p: primitivep */
  primitive *r;
  int i;
  primitive *WITH;

  if (*p != NULL)
    (*p)->parent = NULL;
  while (*p != NULL) {
    while ((*p)->next_ != NULL || (*p)->son != NULL) {
      r = *p;
      if ((*p)->next_ != NULL) {
	*p = (*p)->next_;
	r->next_ = NULL;
      } else {
	*p = (*p)->son;
	r->son = NULL;
      }
      (*p)->parent = r;
    }
    r = (*p)->parent;
    deletename(&(*p)->shadedp);
    deletename(&(*p)->outlinep);
    deletename(&(*p)->textp);
    deletename(&(*p)->name);
    WITH = *p;
    switch (WITH->ptype) {

    case XLbox:
    case XLstring:
      Free(*p);
      break;

    case XBLOCK:
      for (i = HASHLIM; i >= 0; i--)
	deletename(&WITH->UU.Ublock.vars[i]);
      if (WITH->UU.Ublock.env != NULL)
	Free(WITH->UU.Ublock.env);
      Free(*p);
      break;

    case XLcircle:
      Free(*p);
      break;

    case XLellipse:
      Free(*p);
      break;

    case XLline:
    case XLarc:
    case XLarrow:
    case XLmove:
    case XLspline:
      Free(*p);
      break;

    case XLaTeX:
    case XLabel:
      Free(*p);
      break;
    }
    *p = r;
  }
}


void setangles(double *strtang, double *arcang, postype ctr, double xs,
		      double ys, double xf, double yf)
{
  double ra;   /* set arc angles given centre, start, end */

  *strtang = datan(ys - ctr.ypos, xs - ctr.xpos);
  ra = principal(datan(yf - ctr.ypos, xf - ctr.xpos) - *strtang, pi);
  if (ra < 0.0 && *arcang > 0.0)
    ra += 2.0 * pi;
  else if (ra > 0.0 && *arcang < 0.0)
    ra -= 2.0 * pi;
  *arcang = ra;
}


void eqop(double *x, int op, double y)
{
  switch (op) {

  case XEQ:
  case XLcoloneq:
    *x = y;
    break;

  case XLpluseq:
    *x += y;
    break;

  case XLminuseq:
    *x -= y;
    break;

  case XLmulteq:
    *x *= y;
    break;

  case XLdiveq:
    if (y == 0)
      markerror(852);
    else
      *x /= y;
    break;

  case XLremeq:
    if (y == 0)
      markerror(852);
    else {
      *x = (long)floor(*x + 0.5) % (long)floor(y + 0.5);
/* p2c: dpic1.p, line 410:
 * Note: Using % for possibly-negative arguments [317] */
    }
    break;
  }
}


void setstval(int *st, int value)
{
  *st = value * 256 + ((*st) & 255);
}


int getstval(int st)
{
  return (st >> 8);
}


void setstflag(int *st, int value)
{
  switch (value) {

  case XEMPTY:
    *st = ((*st) >> 6) * 64;
    break;

  case XLto:
    *st = ((*st) >> 1) * 2 + 1;
    break;

  case XLfrom:
    *st = ((*st) >> 2) * 4 + ((*st) & 1) + 2;
    break;

  case XLat:
    *st = ((*st) >> 3) * 8 + ((*st) & 3) + 4;
    break;

  case XLradius:
    *st = ((*st) >> 4) * 16 + ((*st) & 7) + 8;
    break;

  case XLcw:
    *st = ((*st) >> 5) * 32 + ((*st) & 15) + 16;
    break;

  case XLccw:
    *st = ((*st) >> 6) * 64 + ((*st) & 31) + 32;
    break;

  case XLchop:
    *st = ((*st) >> 7) * 128 + ((*st) & 63) + 64;
    break;

  case XLdirecton:
    *st = ((*st) >> 8) * 256 + ((*st) & 127) + 128;
    break;
  }

}


boolean teststflag(int st, int value)
{
  boolean b;

  switch (value) {

  case XLto:
    b = st & 1;
    break;

  case XLfrom:
    b = (st >> 1) & 1;
    break;

  case XLat:
    b = (st >> 2) & 1;
    break;

  case XLradius:
    b = (st >> 3) & 1;
    break;

  case XLcw:
    b = (st >> 4) & 1;
    break;

  case XLccw:
    b = (st >> 5) & 1;
    break;

  case XLchop:
    b = (st >> 6) & 1;
    break;

  case XLdirecton:
    b = (st >> 7) & 1;
    break;

  default:
    b = false;
    break;
  }
  return b;
}


int eqstring(Char *seg1, chbufinx inx1, chbufinx len1, Char *seg2,
		    chbufinx inx2, chbufinx len2)
{
  int i, j, k;

  /*D if debuglevel = 2 then begin
     writeln(log,' eqstring:'); write(log,' 1st arg =');
     if seg1 = nil then write(log,' nil') else snapname(seg1,inx1,len1);
     writeln(log); write(log,' 2nd arg =');
     if seg2 = nil then write(log,' nil') else snapname(seg2,inx2,len2);
     writeln(log) end; D*/
  if (seg1 == NULL || seg2 == NULL) {
    k = SHORT_MAX;
    return k;
  }
  i = 0;
  k = 0;
  if (len1 < len2)
    j = len1;
  else
    j = len2;
  while (i < j) {
    /*D if debuglevel = 2 then
         write(log,'(',seg1^[inx1+i],seg2^[inx2+i],')'); D*/
    k = seg1[inx1 + i] - seg2[inx2 + i];
    if (k == 0)
      i++;
    else
      j = i;
  }
  if (k != 0)
    return k;
  if (len1 > len2)
    k = seg1[inx1 + i];
  else if (len1 < len2)
    k = -seg2[inx2 + i];
  /*D if debuglevel = 2 then if (seg1 = nil) or (seg2 = nil) then
     writeln(log,' nil segment(s)')
     else writeln(log,' i=',i:1,' j=',j:1,' eqstring=',k:1); D*/
  return k;
}


int cmpstring(primitive *p1, primitive *p2)
{
  if (p1 == NULL || p2 == NULL)
    return SHORT_MAX;
  else if (p1->textp == NULL)
    return SHORT_MAX;
  else if (p2->textp == NULL)
    return (-SHORT_MAX);
  else
    return (eqstring(p1->textp->segmnt, p1->textp->seginx, p1->textp->len,
		     p2->textp->segmnt, p2->textp->seginx, p2->textp->len));
}


primitive *findplace(primitive *p, Char *chb, chbufinx inx,
			    chbufinx length)
{
  primitive *pj;
  nametype *WITH;

  pj = NULL;
  /*D if debuglevel = 2 then begin
     write(log,'findplace:'); snapname(chb,inx,length); writeln(log)
     end; D*/
  while (p != pj) {   /*D begin printobject(p); D*/
    if (p->name == NULL)
      p = p->next_;
    else {
      WITH = p->name;
      if (eqstring(WITH->segmnt, WITH->seginx, WITH->len, chb, inx, length) == 0)
	pj = p;
      else
	p = p->next_;
    }
  }
  /*D end; D*/
  return p;
}


arg *findmacro(arg *p, Char *chb, chbufinx inx, chbufinx length,
		      arg **last)
{
  /*H( p:argp;
                       chb: chbufp; inx,length: chbufinx;
                       var last:argp )H*/
  arg *pj, *WITH;

  *last = NULL;
  pj = NULL;
  /*D if debuglevel = 2 then begin
     write(log,'findmacro:'); snapname(chb,inx,length); writeln(log)
     end; D*/
  while (p != pj) {
    WITH = p;
    if (WITH->argbody == NULL) {
      *last = p;
      p = p->nextb;
    } else if (eqstring(WITH->argbody->carray, 1, -WITH->argbody->attrib, chb,
			inx, length) == 0)
      pj = p;
    else {
      *last = p;
      p = p->nextb;
    }
  }
  return p;
}


int varhash(Char *chb, chbufinx chbufx, chbufinx length)
{
  int idx;

  if (chb == NULL) {
    idx = 0;
    return (idx % (HASHLIM + 1));
  }
  idx = chb[chbufx];
  if (length > 2)
    idx += chb[chbufx + length - 2];
  return (idx % (HASHLIM + 1));
/* p2c: dpic1.p, line 540:
 * Note: Using % for possibly-negative arguments [317] */
}


nametype *findname(primitive *eb, Char *chb, chbufinx chbufx,
			  chbufinx length, nametype **last, int *k)
{
  nametype *leftptr, *rightptr;
  int left, right, midpt, i, idx;
  nametype *WITH;

  idx = varhash(chb, chbufx, length);
  /*D if debuglevel > 0 then begin
      write(log,' findname|');
      for i:= chbufx to chbufx-1+length do write(log,chb^[i]);
      write(log,'|:');
      if eb=nil then write(log,' eb=nil')
      else write(log,' nvars[',idx:1,']=',eb^.nvars[idx]:1);
      if debuglevel > 1 then writeln(log)
      end; D*/
  *k = 1;
  *last = NULL;
  rightptr = NULL;
  if (eb == NULL)
    leftptr = NULL;
  else {
    leftptr = eb->UU.Ublock.vars[idx];
    *last = leftptr;
  }
  /* Check the first (highest) name */
  if (leftptr != NULL) {
    *k = eqstring(chb, chbufx, length, leftptr->segmnt, leftptr->seginx,
		  leftptr->len);
    if (*k < 0) {
      left = 2;
      leftptr = leftptr->next_;
      right = eb->UU.Ublock.nvars[idx] + 1;
    } else
      rightptr = leftptr;
  }
  while (leftptr != rightptr) {
    midpt = (left + right) >> 1;
    /*D if debuglevel > 0 then
                       write(log,' midpt=',midpt:1); D*/
    *last = leftptr;
    for (i = left + 1; i <= midpt; i++)
      *last = (*last)->next_;
    WITH = *last;
    *k = eqstring(chb, chbufx, length, WITH->segmnt, WITH->seginx, WITH->len);
    /*D if debuglevel > 0 then write(log,' k=',k:1); D*/
    if (*k < 0) {
      left = midpt + 1;
      leftptr = (*last)->next_;
      continue;
    }
    if (*k == 0) {
      leftptr = *last;
      rightptr = leftptr;
    } else {
      right = midpt;
      rightptr = *last;
    }
  }
  if (*k == 0)
    return leftptr;
  else {
    return NULL;
    /*D; if debuglevel > 0 then writeln(log) D*/
  }
}


void marknotfound(int eno, Char *chb, chbufinx inx, chbufinx len)
{
  int i;

  /*D if debuglevel > 0 then begin write(log,'Search failure ',eno:1);
      if chb <> nil then begin write(log,' for "');
         for i := inx to inx+len-1 do write(log,chb^[i]); write(log,'"') end;
      writeln(log) end; D*/
  markerror(eno);
  fprintf(errout, "\nSearch failure");
  if (chb != NULL) {
    fprintf(errout, " for \"");
    for (i = inx; i < inx + len; i++)
      putc(chb[i], errout);
    putc('"', errout);
  }
  putc('\n', errout);
}


nametype *glfindname(primitive *eb, Char *chb, chbufinx chbufx,
			    chbufinx length, nametype **last, int *k)
{
  nametype *np;
  primitive *pp;

  pp = NULL;
  np = NULL;
  *k = 1;
  while (eb != pp) {
    /* np := findname(eb^.vars,chb,chbufx,length,last); */
    np = findname(eb, chb, chbufx, length, last, k);
    if (np != NULL)
      pp = eb;
    else
      eb = eb->parent;
  }
  if (eb == NULL)
    marknotfound(851, chb, chbufx, length);
  return np;
}


void newstr(nametype **sp)
{
  nametype *WITH;

  *sp = Malloc(sizeof(nametype));
  WITH = *sp;
  WITH->segmnt = NULL;
  WITH->seginx = 0;
  WITH->len = 0;
  WITH->val = 0.0;
  WITH->next_ = NULL;
}


/* copy a string into the current freeseg */
void storestring(Char **outbuf, nametype *outstr, Char *srcbuf,
			chbufinx psrc, chbufinx lsrc)
{
  int i, j;
  boolean newseg;

  if (*outbuf == NULL || lsrc > CHBUFSIZ - freex + 1)
    newseg = true;
  else if (bval(*outbuf) >= maxbval)
    newseg = true;
  else
    newseg = false;
  if (newseg) {
    *outbuf = Malloc(sizeof(chbufarray));
    putbval(*outbuf, 0);
    freex = 3;
    freeseg = *outbuf;
  }
  for (i = 0; i < lsrc; i++)
    (*outbuf)[freex + i] = srcbuf[psrc + i];
  /* j := psrc-freex;
     for i := freex to freex+lsrc-1 do outbuf^[i] := srcbuf^[j+i]; */
  outstr->segmnt = *outbuf;
  outstr->seginx = freex;
  outstr->len = lsrc;
  j = bval(*outbuf);
  putbval(*outbuf, j + 1);
  freex += lsrc;
  /*D; if debuglevel > 0 then begin writeln(log,
     'storestring to strptr ',ordp(outstr):1,': segmnt=',ordp(freeseg):1,
     ' seginx= ',outstr^.seginx:1, ' links= ', bval(outbuf):1,
     ' freex=',freex:1 );
     snapname(outbuf,outstr^.seginx,outstr^.len);
     write(log,' from:'); snapname(srcbuf,psrc,lsrc);
     writeln( log ) end D*/
}


/* outbuf is always the current freeseg: */
void copystr(Char **outbuf, nametype **sp, nametype *ip)
{
  if (ip == NULL)
    *sp = NULL;
  else {
    newstr(sp);
    storestring(outbuf, *sp, ip->segmnt, ip->seginx, ip->len);
  }
}


void appendstring(nametype *sp, Char *buf, chbufinx px, chbufinx ll)
{  /*D k := 0; D*/
  int i;
  /*D,k D*/
  int j;
  Char *tmpseg;
  int FORLIM;

  if (sp == NULL || buf == NULL)
    return;
  if (sp->segmnt == freeseg && sp->seginx + sp->len == freex &&
      freex + ll - 1 <= CHBUFSIZ) {
    /*D if debuglevel > 0 then begin
        write(log,' appending |');
        for i:=0 to ll-1 do write(log,buf^[px+i]); writeln(log,'|') end; D*/
    for (i = 0; i < ll; i++)   /*D k := 1; D*/
      freeseg[freex + i] = buf[px + i];
    sp->len += ll;
    freex += ll;
    return;
  }
  if (sp->len + ll + 2 > CHBUFSIZ) {
    markerror(866);
    return;
  }
  tmpseg = Malloc(sizeof(chbufarray));
  FORLIM = sp->len;
  for (i = 0; i < FORLIM; i++)
    tmpseg[i + 3] = sp->segmnt[sp->seginx + i];
  j = bval(sp->segmnt);
  if (j > 1) {
    putbval(sp->segmnt, j - 1);   /*D k := 2; D*/
    if (sp->segmnt == freeseg && sp->seginx + sp->len == freex) {
      freex = sp->seginx;
      j = 3;
      while (freex > j) {
	if (sp->segmnt[freex - 1] == nlch)
	  freex--;
	else
	  j = freex;
      }
    } else {
      FORLIM = sp->seginx + sp->len;
      for (i = sp->seginx; i < FORLIM; i++)
	sp->segmnt[i] = nlch;
    }
  } else {
    if (sp->segmnt == freeseg)   /*D k := 3; D*/
      freeseg = NULL;
    Free(sp->segmnt);
  }
  for (i = 0; i < ll; i++)
    tmpseg[sp->len + i + 3] = buf[px + i];
  freeseg = tmpseg;
  freex = sp->len + ll + 3;
  putbval(freeseg, 1);
  sp->segmnt = freeseg;
  sp->seginx = 3;
  sp->len += ll;
  /*D; if debuglevel > 0 then if sp<>nil then with sp^ do begin
     writeln(log,
      'appendstring to strptr ',ordp(sp):1,': segmnt=',ordp(freeseg):1,
      ' seginx:seginx+len-1= ',seginx:1,':',seginx+len-1:1);
     writeln(log,
      ' len= ',len:1, ' branch k=',k:1);
     snapname(segmnt,seginx,len); writeln( log )
     end D*/
}


int putstring(int ix, nametype *sp, Char *buf, chbufinx px,
		     chbufinx ll)
{
  if (ix <= 0)
    storestring(&freeseg, sp, buf, px, ll);
  else
    appendstring(sp, buf, px, ll);
  return (ix + 1);
}


double pheight(primitive *pr)
{
  double ph;

  if (pr == NULL) {
    ph = 0.0;
    return ph;
  }
  switch (pr->ptype) {

  case XLbox:
  case XLstring:
    ph = pr->UU.Ubox.boxheight;
    break;

  case XBLOCK:
    ph = pr->UU.Ublock.blockheight;
    break;

  case XLellipse:
    ph = pr->UU.Uellipse.elheight;
    break;

  case XLcircle:
    ph = 2.0 * pr->UU.Ucircle.radius;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    ph = fabs(pr->UU.Uline.endpos.ypos - pr->aat.ypos);
    break;

  default:
    ph = 0.0;
    break;
  }
  return ph;
}


double pwidth(primitive *pr)
{
  double ph;

  if (pr == NULL) {
    ph = 0.0;
    return ph;
  }
  switch (pr->ptype) {

  case XLbox:
  case XLstring:
    ph = pr->UU.Ubox.boxwidth;
    break;

  case XBLOCK:
    ph = pr->UU.Ublock.blockwidth;
    break;

  case XLellipse:
    ph = pr->UU.Uellipse.elwidth;
    break;

  case XLcircle:
    ph = 2.0 * pr->UU.Ucircle.radius;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    ph = fabs(pr->UU.Uline.endpos.xpos - pr->aat.xpos);
    break;

  default:
    ph = 0.0;
    break;
  }
  return ph;
}


void neswrec(primitive *ptm)
{
  while (ptm != NULL) {
    nesw(ptm);
    if (ptm->ptype != XBLOCK)
      neswrec(ptm->son);
    ptm = ptm->next_;
  }
}


void getnesw(primitive *ptm)
{
  initnesw();
  neswrec(ptm);
  if (south > north) {
    south = 0.0;
    north = 0.0;
  }
  if (west > east) {
    west = 0.0;
    east = 0.0;
  }
}


void pexit(primitive *pr, postype *pe)
{
  if (pr == NULL) {
    pe->xpos = 0.0;
    pe->ypos = 0.0;
    return;
  }
  if (pr->ptype != XBLOCK && pr->son != NULL) {
    pexit(pr->son, pe);
    return;
  }
  *pe = pr->aat;
  switch (pr->ptype) {

  case XLbox:
  case XLstring:
    switch (pr->direction) {

    case XLup:
      pe->ypos = pr->aat.ypos + pr->UU.Ubox.boxheight * 0.5;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->UU.Ubox.boxheight * 0.5;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->UU.Ubox.boxwidth * 0.5;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->UU.Ubox.boxwidth * 0.5;
      break;
    }
    break;

  /* XLstring: begin
     getnesw(pr);
     case direction of
        XLup: begin pe.xpos := (east+west)/2; pe.ypos := north end;
        XLdown: begin pe.xpos := (east+west)/2; pe.ypos := south end;
        XLleft: begin pe.ypos := (north+south)/2; pe.xpos := west end;
        XLright: begin pe.ypos := (north+south)/2; pe.xpos := east; end
        end
     end; */
  case XBLOCK:
    switch (pr->direction) {

    case XLup:
      pe->ypos = pr->aat.ypos + pr->UU.Ublock.blockheight * 0.5;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->UU.Ublock.blockheight * 0.5;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->UU.Ublock.blockwidth * 0.5;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->UU.Ublock.blockwidth * 0.5;
      break;
    }
    break;

  case XLcircle:
    switch (pr->direction) {

    case XLup:
      pe->ypos = pr->aat.ypos + pr->UU.Ucircle.radius;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->UU.Ucircle.radius;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->UU.Ucircle.radius;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->UU.Ucircle.radius;
      break;
    }
    break;

  case XLellipse:
    switch (pr->direction) {

    case XLup:
      pe->ypos = pr->aat.ypos + pr->UU.Uellipse.elheight * 0.5;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->UU.Uellipse.elheight * 0.5;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->UU.Uellipse.elwidth * 0.5;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->UU.Uellipse.elwidth * 0.5;
      break;
    }
    break;

  case XLarc:
    *pe = arcend(pr);
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    *pe = pr->UU.Uline.endpos;
    break;

  case XLabel:
  case XLaTeX:
    /* blank case */
    break;
  }
}


void newprim(primitive **pr, int primtype, primitive *envblk)
{
  int i;
  primitive *WITH;

  switch (primtype) {

  case XLbox:
  case XLstring:
    *pr = Malloc(sizeof(XLboxprimitive));
    break;

  case XBLOCK:
    *pr = Malloc(sizeof(primitive));
    break;

  case XLcircle:
    *pr = Malloc(sizeof(XLcircleprimitive));
    break;

  case XLellipse:
    *pr = Malloc(sizeof(XLellipseprimitive));
    break;

  case XLline:
  case XLarc:
  case XLarrow:
  case XLmove:
  case XLspline:
    *pr = Malloc(sizeof(XLlineprimitive));
    break;

  case XLabel:
  case XLaTeX:
    *pr = Malloc(sizeof(XLabelprimitive));
    break;
  }
  WITH = *pr;
  WITH->name = NULL;
  WITH->textp = NULL;
  WITH->outlinep = NULL;
  WITH->shadedp = NULL;
  WITH->son = NULL;
  WITH->next_ = NULL;
  if (envblk == NULL) {
    WITH->parent = NULL;
    WITH->aat.xpos = 0.0;
    WITH->aat.ypos = 0.0;
    WITH->direction = XLright;
  } else {
    if (envblk->son == NULL)
      envblk->son = *pr;
    WITH->parent = envblk;
    WITH->aat = envblk->UU.Ublock.here;
    WITH->direction = envblk->direction;
  }
  WITH->lparam = mdistmax;
  WITH->lthick = mdistmax;
  if (primtype == XLstring || primtype == XLspline || primtype == XLarc ||
      primtype == XLarrow || primtype == XLline || primtype == XLellipse ||
      primtype == XLcircle || primtype == XLbox)
    resetspec(&WITH->spec, XLsolid);
  else
    resetspec(&WITH->spec, XLinvis);
  /* spec := 0;
  if primtype in [XLbox,XLcircle,XLellipse,XLline,XLarrow,XLarc,XLspline]
     then setvis(spec,1); */
  WITH->ptype = primtype;
  switch (primtype) {

  case XLbox:
  case XLstring:
    WITH->UU.Ubox.boxfill = -1.0;
    WITH->UU.Ubox.boxheight = 0.0;
    WITH->UU.Ubox.boxwidth = 0.0;
    WITH->UU.Ubox.boxradius = 0.0;
    break;

  case XBLOCK:
    WITH->UU.Ublock.blockheight = 0.0;
    WITH->UU.Ublock.blockwidth = 0.0;
    WITH->UU.Ublock.here = WITH->aat;
    for (i = 0; i <= HASHLIM; i++) {
      WITH->UU.Ublock.vars[i] = NULL;
      WITH->UU.Ublock.nvars[i] = 0;
    }
    WITH->UU.Ublock.env = NULL;
    break;

  case XLcircle:
    WITH->UU.Ucircle.cfill = -1.0;
    WITH->UU.Ucircle.radius = 0.0;
    break;

  case XLellipse:
    WITH->UU.Uellipse.efill = -1.0;
    WITH->UU.Uellipse.elheight = 0.0;
    WITH->UU.Uellipse.elwidth = 0.0;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLarc:
  case XLspline:
    WITH->UU.Uline.endpos.xpos = 0.0;
    WITH->UU.Uline.endpos.ypos = 0.0;
    WITH->UU.Uline.height = 0.0;
    WITH->UU.Uline.width = 0.0;
    WITH->UU.Uline.lfill = -1.0;
    WITH->UU.Uline.aradius = mdistmax;
    WITH->UU.Uline.atype = pahlex(0, XEMPTY);
    break;

  case XLabel:
  case XLaTeX:
    /* blank case */
    break;
  }
}  /* newprim */


void arcenddir(primitive *pr)
{
  if (pr->UU.Uline.endpos.ypos > 0.0) {
    switch (pr->direction) {

    case 0:
      /* blank case */
      break;

    case XLup:
      envblock->direction = XLleft;
      break;

    case XLdown:
      envblock->direction = XLright;
      break;

    case XLleft:
      envblock->direction = XLdown;
      break;

    case XLright:
      envblock->direction = XLup;
      break;
    }
    return;
  }
  switch (pr->direction) {

  case 0:
    /* blank case */
    break;

  case XLup:
    envblock->direction = XLright;
    break;

  case XLdown:
    envblock->direction = XLleft;
    break;

  case XLleft:
    envblock->direction = XLup;
    break;

  case XLright:
    envblock->direction = XLdown;
    break;
  }
}


void shift(primitive *pr, double x, double y)
{
  primitive *WITH;

  while (pr != NULL) {
    WITH = pr;
    WITH->aat.xpos += x;
    WITH->aat.ypos += y;
    if (WITH->ptype == XLspline || WITH->ptype == XLmove ||
	WITH->ptype == XLarrow || WITH->ptype == XLline) {
      WITH->UU.Uline.endpos.xpos += x;
      WITH->UU.Uline.endpos.ypos += y;
    }
    if (WITH->son != NULL)
      shift(WITH->son, x, y);
    pr = WITH->next_;
  }
}


void scaleobj(primitive *pr, double s)
{
  primitive *WITH;

  while (pr != NULL) {
    WITH = pr;
    WITH->aat.xpos *= s;
    WITH->aat.ypos *= s;
    if (WITH->ptype == XLbox) {
      WITH->UU.Ubox.boxheight *= s;
      WITH->UU.Ubox.boxwidth *= s;
      WITH->UU.Ubox.boxradius *= s;
    } else if (WITH->ptype == XBLOCK) {
      WITH->UU.Ublock.blockheight *= s;
      WITH->UU.Ublock.blockwidth *= s;
    } else if (WITH->ptype == XLcircle)
      WITH->UU.Ucircle.radius *= s;
    else if (WITH->ptype == XLellipse) {
      WITH->UU.Uellipse.elheight *= s;
      WITH->UU.Uellipse.elwidth *= s;
    } else if (WITH->ptype == XLarc)
      WITH->UU.Uline.aradius *= s;
    else if (WITH->ptype == XLspline || WITH->ptype == XLmove ||
	     WITH->ptype == XLarrow || WITH->ptype == XLline) {
      WITH->UU.Uline.endpos.xpos *= s;
      WITH->UU.Uline.endpos.ypos *= s;
    }
    if (WITH->son != NULL)
      scaleobj(WITH->son, s);
    pr = WITH->next_;
  }
}


/* corner(prim,<corner>,xval,yval);
                           Put the corner coordinates into xval,yval */
void corner(primitive *pr, int lexv, double *x, double *y)
{
  primitive *pe;
  boolean sb;

  if (pr == NULL)
    return;
  /*D if debuglevel>0 then begin write(log,
      'Corner: ptype(',ordp(pr):1,'): ptype='); printptype(ptype);
     write(log,' corner='); printcorner(lexv) end; D*/
  *x = pr->aat.xpos;
  *y = pr->aat.ypos;
  pe = pr;
  if (lexv == XEMPTY &&
      (pr->ptype == XLspline || pr->ptype == XLmove || pr->ptype == XLarrow ||
       pr->ptype == XLline)) {
    while (pe->son != NULL)
      pe = pe->son;
    *x = 0.5 * (pr->aat.xpos + pe->UU.Uline.endpos.xpos);
    *y = 0.5 * (pr->aat.ypos + pe->UU.Uline.endpos.ypos);
    return;
  }
  if (lexv == XEMPTY && pr->ptype != XLaTeX)
    return;
  switch (pr->ptype) {

  case XLbox:
  case XLstring:
  case XBLOCK:
  case XLcircle:
  case XLellipse:
  case XLarc:
    *x = pr->aat.xpos;
    *y = pr->aat.ypos;
    initnesw();
    nesw(pr);
    /*D if debuglevel>0 then begin
        write(log,' aat'); wpair(log,aat.xpos,aat.ypos);
        write(log,' n,s'); wpair(log,north,south);
        write(log,' w,e'); wpair(log,west,east)
        end; D*/
    if ((pr->ptype == XLarc || pr->ptype == XLcircle ||
	 pr->ptype == XLellipse || pr->ptype == XLbox) &&
	(lexv == XDnw || lexv == XDsw || lexv == XDse || lexv == XDne)) {
      switch (pr->ptype) {

      case XLbox:
	*y = Min(pr->UU.Ubox.boxradius, Min(fabs(pr->UU.Ubox.boxheight),
		     fabs(pr->UU.Ubox.boxwidth)) / 2) * (1 - 1 / sqrt(2.0));
	*x = pr->UU.Ubox.boxwidth / 2 - *y;
	*y = pr->UU.Ubox.boxheight / 2 - *y;
	break;

      case XLellipse:
	*x = pr->UU.Uellipse.elwidth * (0.5 / sqrt(2.0));
	*y = pr->UU.Uellipse.elheight * (0.5 / sqrt(2.0));
	break;

      case XLarc:
	*x = pr->UU.Uline.aradius / sqrt(2.0);
	*y = *x;
	break;

      case XLcircle:
	*x = pr->UU.Ucircle.radius / sqrt(2.0);
	*y = *x;
	break;
      }
      switch (lexv) {

      case XDne:
	/* blank case */
	break;

      case XDse:
	*y = -*y;
	break;

      case XDnw:
	*x = -*x;
	break;

      case XDsw:
	*x = -*x;
	*y = -*y;
	break;
      }
      *x = pr->aat.xpos + *x;
      *y = pr->aat.ypos + *y;
    } else if (pr->ptype == XLarc) {
      switch (lexv) {

      case XDn:
	*y = pr->aat.ypos + pr->UU.Uline.aradius;
	break;

      case XDs:
	*y = pr->aat.ypos - pr->UU.Uline.aradius;
	break;

      case XDe:
	*x = pr->aat.xpos + pr->UU.Uline.aradius;
	break;

      case XDw:
	*x = pr->aat.xpos - pr->UU.Uline.aradius;
	break;

      case XDc:
	/* blank case */
	break;

      case XDstart:
	*x = pr->aat.xpos + pr->UU.Uline.aradius * cos(pr->UU.Uline.endpos.xpos);
	*y = pr->aat.ypos + pr->UU.Uline.aradius * sin(pr->UU.Uline.endpos.xpos);
	break;

      case XDend:
	*x = pr->aat.xpos + pr->UU.Uline.aradius *
	       cos(pr->UU.Uline.endpos.xpos + pr->UU.Uline.endpos.ypos);
	*y = pr->aat.ypos + pr->UU.Uline.aradius *
	       sin(pr->UU.Uline.endpos.xpos + pr->UU.Uline.endpos.ypos);
	break;
      }
    } else {
      switch (lexv) {

      case XDn:
	*y = north;
	break;

      case XDs:
	*y = south;
	break;

      case XDe:
	*x = east;
	break;

      case XDw:
	*x = west;
	break;

      case XDne:
	*y = north;
	*x = east;
	break;

      case XDse:
	*y = south;
	*x = east;
	break;

      case XDsw:
	*y = south;
	*x = west;
	break;

      case XDnw:
	*y = north;
	*x = west;
	break;

      case XDc:
	*y = pr->aat.ypos;
	*x = pr->aat.xpos;
	break;

      case XDstart:
      case XDend:
	markerror(858);
	break;
      }
    }
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    if (lexv != XDstart) {
      if (lexv == XDend) {
	while (pe->son != NULL)
	  pe = pe->son;
	*x = pe->UU.Uline.endpos.xpos;
	*y = pe->UU.Uline.endpos.ypos;
      } else if (lexv == XDc) {
	while (pe->son != NULL)
	  pe = pe->son;
	*x = 0.5 * (*x + pe->UU.Uline.endpos.xpos);
	*y = 0.5 * (*y + pe->UU.Uline.endpos.ypos);
      } else {
	do {
	  switch (lexv) {

	  case XDn:
	    sb = (pe->UU.Uline.endpos.ypos > *y);
	    break;

	  case XDs:
	    sb = (pe->UU.Uline.endpos.ypos < *y);
	    break;

	  case XDe:
	    sb = (pe->UU.Uline.endpos.xpos > *x);
	    break;

	  case XDw:
	    sb = (pe->UU.Uline.endpos.xpos < *x);
	    break;

	  case XDne:
	    sb = ((pe->UU.Uline.endpos.ypos > *y &&
		   pe->UU.Uline.endpos.xpos >= *x) ||
		  (pe->UU.Uline.endpos.ypos >= *y &&
		   pe->UU.Uline.endpos.xpos > *x));
	    break;

	  case XDse:
	    sb = ((pe->UU.Uline.endpos.ypos < *y &&
		   pe->UU.Uline.endpos.xpos >= *x) ||
		  (pe->UU.Uline.endpos.ypos <= *y &&
		   pe->UU.Uline.endpos.xpos > *x));
	    break;

	  case XDsw:
	    sb = ((pe->UU.Uline.endpos.ypos < *y &&
		   pe->UU.Uline.endpos.xpos <= *x) ||
		  (pe->UU.Uline.endpos.ypos <= *y &&
		   pe->UU.Uline.endpos.xpos < *x));
	    break;

	  case XDnw:
	    sb = ((pe->UU.Uline.endpos.ypos > *y &&
		   pe->UU.Uline.endpos.xpos <= *x) ||
		  (pe->UU.Uline.endpos.ypos >= *y &&
		   pe->UU.Uline.endpos.xpos < *x));
	    break;
	  }
	  if (sb) {
	    *x = pe->UU.Uline.endpos.xpos;
	    *y = pe->UU.Uline.endpos.ypos;
	  }
	  pe = pe->son;
	} while (pe != NULL);
      }
    }
    break;

  case XLabel:
    /* blank case */
    break;

  case XLaTeX:
    markerror(858);
    break;
  }
  /*D; if debuglevel > 0 then begin write(log,' corner=');
      wpair(log,x,y); writeln(log) end D*/
}


primitive *nthprimobj(primitive *primp, int nth, int objtype)
{
  primitive *prp, *pp;
  int i;

  /*D if debuglevel = 2 then writeln(log,'nthprimobj nth=',nth:1,' type=',
     objtype:1); D*/
  prp = NULL;
  if (nth == 0) {
    while (primp != NULL) {
      /*D if debuglevel = 2 then begin
         printobject(primp);
         if primp^.ptype = objtype then begin
            prp := primp;
            writeln(log,'found[nth=',nth:1,'](',ordp(prp):1,')') end;
         primp := primp^.next
         end
      else D*/
      if (primp->ptype == objtype)
	prp = primp;
      primp = primp->next_;
    }
    return prp;
  }
  if (nth < 0) {
    i = 0;
    pp = primp;
    while (pp != NULL) {
      if (pp->ptype == objtype)
	i++;
      pp = pp->next_;
    }
    nth += i + 1;
  }
  i = 0;
  /*D if debuglevel = 2 then write(log,' nth=',nth:1); D*/
  if (nth <= 0)
    return prp;
  while (primp != prp) {
    /*D if debuglevel = 2 then printobject(primp); D*/
    if (primp->ptype == objtype)
      i++;
    if (i == nth)
      prp = primp;
    else {
      primp = primp->next_;
      /*D;if (debuglevel = 2) and (i = nth) then
         writeln(log,'found[nth=',nth:1,'](',ordp(prp):1,')') D*/
    }
  }
  return prp;
}


void resetenv(int envval, primitive *envbl)
{
  environx i, last;

  if (envbl == NULL)
    return;
  if (envval == 0) {
    envval = XLenvvar + 1;
    last = XLlastenv;
  } else
    last = envval;
  if (envbl->UU.Ublock.env == NULL)
    envbl->UU.Ublock.env = Malloc(sizeof(envarray));
  for (i = envval - 1; i < last; i++) {
    switch (i + 1) {

    case XLarcrad:   /* scaled environment vars (in) */
      envbl->UU.Ublock.env[i - XLenvvar] = 0.25;
      break;

    case XLarrowht:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.1;
      break;

    case XLarrowwid:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.05;
      break;

    case XLboxht:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.5;
      break;

    case XLboxrad:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.0;
      break;

    case XLboxwid:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.75;
      break;

    case XLcirclerad:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.25;
      break;

    case XLdashwid:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.05;
      break;

    case XLellipseht:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.5;
      break;

    case XLellipsewid:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.75;
      break;

    case XLlineht:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.5;
      break;

    case XLlinewid:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.5;
      break;

    case XLmoveht:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.5;
      break;

    case XLmovewid:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.5;
      break;

    case XLtextht:
      if (drawmode == SVG)
	envbl->UU.Ublock.env[i - XLenvvar] = DFONT / 72.0;
      else
	envbl->UU.Ublock.env[i - XLenvvar] = 0.0;
      break;

    case XLtextoffset:
      envbl->UU.Ublock.env[i - XLenvvar] = 2.5 / 72;   /*.27*/
      break;

    case XLtextwid:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.0;
      break;

    /* The following are unscaled */
    case XLarrowhead:
      envbl->UU.Ublock.env[i - XLenvvar] = 1.0;
      break;

    case XLfillval:
      envbl->UU.Ublock.env[i - XLenvvar] = 0.5;
      break;

    case XLlinethick:   /* points */
      envbl->UU.Ublock.env[i - XLenvvar] = 0.8;
      break;

    case XLmaxpsht:   /* in */
      envbl->UU.Ublock.env[i - XLenvvar] = 11.0;
      break;

    case XLmaxpswid:
      envbl->UU.Ublock.env[i - XLenvvar] = 8.5;
      break;

    case XLscale:
      envbl->UU.Ublock.env[i - XLenvvar] = 1.0;
      break;
    }
  }
}


void inheritenv(primitive *envbl)
{
  environx i;
  primitive *pr;

  pr = findenv(envbl);
  if (pr == NULL) {
    resetenv(0, envbl);
    return;
  }
  envbl->UU.Ublock.env = Malloc(sizeof(envarray));
  for (i = XLenvvar; i < XLlastenv; i++)
    envbl->UU.Ublock.env[i - XLenvvar] = pr->UU.Ublock.env[i - XLenvvar];
}


void resetscale(double x, int opr, primitive *envbl)
{
  double r, s;
  int i;

  resetenv(0, envbl);
  r = envbl->UU.Ublock.env[XLscale - XLenvvar - 1];
  eqop(&envbl->UU.Ublock.env[XLscale - XLenvvar - 1], opr, x);
  s = envbl->UU.Ublock.env[XLscale - XLenvvar - 1];
  if (s == 0.0) {
    envbl->UU.Ublock.env[XLscale - XLenvvar - 1] = r;
    s = 1.0;
    markerror(870);
  } else if (r == 0.0)
    markerror(852);
  else
    s /= r;
  for (i = XLenvvar; i < XLlastsc; i++)
    eqop(&envbl->UU.Ublock.env[i - XLenvvar], XLmulteq, s);
}


void inittwo(void)
{
  /* performed for each input diagram */
  freeinbuf = NULL;
  freeseg = NULL;
  freex = 0;

  lastfillval = mdistmax;
  lastthick = mdistmax;
  newprim(&envblock, XBLOCK, NULL);
  /*D; if debuglevel > 0 then printobject(envblock) D*/
  resetenv(0, envblock);
}


void deleteinbufs(fbuffer **p)
{
  fbuffer *q;

  while (*p != NULL) {
    q = (*p)->nextb;
    Free(*p);
    *p = q;
  }
}


double intpow(double x, int k)
{
  /* 0^(-k) does not occur */
  if (k == 0) {
    x = 1.0;   /* 0^0 returns 1.0 */
    return x;
  }
  if (x == 0.0 || k == 1)
    return x;
  if (k < 0) {
    x = intpow(1.0 / x, -k);
    return x;
  }
  if (k == 2) {
    x *= x;   /* common enough to test for it */
    return x;
  }
  if (k & 1)
    x *= intpow(x, k - 1);
  else {
    x = intpow(x, k >> 1);
    x *= x;
  }
  return x;
}


void getscale(double xv, double yv, primitive *lp, double *sfact,
		     double *xsc)
{
  /* .PS xv yv
     sfact = nominal scale factor set by scale = ...
     xsc = effective scale factor to achieve correct max picture size
     ie (size in inches)/(desired size in inches) */
  double gs;
  int erno;
  primitive *qp;

  erno = 0;
  gs = 1.0;
  *sfact = gs;
  if (lp != NULL) {
    if (lp->ptype == XBLOCK) {
      qp = findenv(lp);
      if (qp->UU.Ublock.env[XLscale - XLenvvar - 1] > 0.0)
	*sfact = qp->UU.Ublock.env[XLscale - XLenvvar - 1];
      /*D if debuglevel > 0 then begin
        write(log,'getscale: sfact='); wfloat(log,sfact);
        write(log,' maxpswid='); wfloat(log,qp^.env^[XLmaxpswid]);
        write(log,' maxpsht='); wfloat(log,qp^.env^[XLmaxpsht]);
        writeln(log) end; D*/
      if (east > west &&
	  (east - west) / *sfact > qp->UU.Ublock.env[XLmaxpswid - XLenvvar - 1] &&
	  qp->UU.Ublock.env[XLmaxpswid - XLenvvar - 1] > 0.0) {
	erno = 903;
	gs = (east - west) / qp->UU.Ublock.env[XLmaxpswid - XLenvvar - 1];
      }
      if (north > south &&
	  (north - south) / *sfact > qp->UU.Ublock.env[XLmaxpsht - XLenvvar - 1] &&
	  qp->UU.Ublock.env[XLmaxpsht - XLenvvar - 1] > 0.0) {
	erno = 904;
	gs = Max(gs,
		 (north - south) / qp->UU.Ublock.env[XLmaxpsht - XLenvvar - 1]);
      }
    }
  }
  if (xv > 0.0 && east > west) {
    erno = 0;
    gs = (east - west) / *sfact / xv;
  }
  if (yv > 0.0 && north > south &&
      (xv == 0.0 || (north - south) / gs > yv * *sfact)) {
    erno = 0;
    gs = (north - south) / *sfact / yv;
  }
  if (erno != 0)
    markerror(erno);
  /*D if debuglevel > 0 then begin write(log,' getscale=');
     wfloat(log,gs*sfact); writeln(log) end; D*/
  *xsc = gs * *sfact;
}


void addelem(primitive *prold, primitive *prnew)
{
  primitive *pp, *pq;

  if (prold == NULL || prnew == NULL)
    return;
  pp = prold;
  while (pp->next_ != NULL) {
    pq = prnew;   /* Refuse to make a loop: */
    while (pq->next_ != NULL) {
      if (pq->next_ == pp)
	pq->next_ = NULL;
      else
	pq = pq->next_;
    }
    pp = pp->next_;
  }
  /*D if debuglevel > 0 then begin
     write(log,'prev element:'); snaptype(log,pp^.ptype); writeln(log);
     writeln(log,' addelem snaptype done') end; D*/
  pp->next_ = prnew;
  /*D; if debuglevel > 0 then begin printobject(prnew);
     writeln(log,' addelem printobject done'); flush(log);
     snaptree(prold,0); writeln(log,' addelem done') end D*/
}


void copyprim(primitive *prin, primitive **prout)
{
  /* Needed because assignment of variant records is unreliable */
  int i;

  if (prin == NULL)
    return;
  newprim(prout, prin->ptype, NULL);
  (*prout)->name = prin->name;
  (*prout)->textp = prin->textp;
  (*prout)->outlinep = prin->outlinep;
  (*prout)->shadedp = prin->shadedp;
  (*prout)->parent = prin->parent;
  (*prout)->son = prin->son;
  (*prout)->next_ = prin->next_;
  (*prout)->aat = prin->aat;
  (*prout)->lparam = prin->lparam;
  (*prout)->lthick = prin->lthick;
  (*prout)->direction = prin->direction;
  (*prout)->spec = prin->spec;
  (*prout)->ptype = prin->ptype;
  switch (prin->ptype) {

  case XLbox:
  case XLstring:
    (*prout)->UU.Ubox.boxfill = prin->UU.Ubox.boxfill;
    (*prout)->UU.Ubox.boxheight = prin->UU.Ubox.boxheight;
    (*prout)->UU.Ubox.boxwidth = prin->UU.Ubox.boxwidth;
    (*prout)->UU.Ubox.boxradius = prin->UU.Ubox.boxradius;
    break;

  case XBLOCK:
    (*prout)->UU.Ublock.blockheight = prin->UU.Ublock.blockheight;
    (*prout)->UU.Ublock.blockwidth = prin->UU.Ublock.blockwidth;
    (*prout)->UU.Ublock.here = prin->UU.Ublock.here;
    for (i = 0; i <= HASHLIM; i++) {
      (*prout)->UU.Ublock.vars[i] = prin->UU.Ublock.vars[i];
      (*prout)->UU.Ublock.nvars[i] = prin->UU.Ublock.nvars[i];
    }
    if (prin->UU.Ublock.env != NULL) {
      (*prout)->UU.Ublock.env = Malloc(sizeof(envarray));
      for (i = XLenvvar; i < XLlastenv; i++)
	(*prout)->UU.Ublock.env[i - XLenvvar] = prin->UU.Ublock.env[i - XLenvvar];
    }
    break;

  case XLcircle:
    (*prout)->UU.Ucircle.cfill = prin->UU.Ucircle.cfill;
    (*prout)->UU.Ucircle.radius = prin->UU.Ucircle.radius;
    break;

  case XLellipse:
    (*prout)->UU.Uellipse.efill = prin->UU.Uellipse.efill;
    (*prout)->UU.Uellipse.elheight = prin->UU.Uellipse.elheight;
    (*prout)->UU.Uellipse.elwidth = prin->UU.Uellipse.elwidth;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLarc:
  case XLspline:
    (*prout)->UU.Uline.endpos = prin->UU.Uline.endpos;
    (*prout)->UU.Uline.height = prin->UU.Uline.height;
    (*prout)->UU.Uline.width = prin->UU.Uline.width;
    (*prout)->UU.Uline.lfill = prin->UU.Uline.lfill;
    (*prout)->UU.Uline.aradius = prin->UU.Uline.aradius;
    (*prout)->UU.Uline.atype = prin->UU.Uline.atype;
    break;

  case XLabel:
  case XLaTeX:
    /* blank case */
    break;
  }
}


void deletestringbox(primitive **pr)
{
  primitive *prx;

  if (*pr != NULL) {
    if ((*pr)->parent != NULL) {
      if ((*pr)->parent->son != NULL) {
	if ((*pr)->parent->son == *pr)
	  (*pr)->parent->son = NULL;
	else {  /* is the following ever reached? */
	  prx = (*pr)->parent->son;
	  while (prx->next_ != NULL && prx->next_ != *pr)
	    prx = prx->next_;
	  prx->next_ = NULL;
	}
      }
    }
  }
  deletetree(pr);
}


/* addsuffix(chbuf,chbufx,length,attstack^[newp+1].xval); */
void addsuffix(Char *buf, chbufinx *inx, int *len, double suff)
{   /*DGH ordp HGD*/
  int i, j, FORLIM;

  /*D if debuglevel <> 0 then begin writeln(log,
    'addsuffix(buf:chbufp; var inx:chbufinx; var len:integer; suff:real)');
     write(log,D*/
  /*DBUMX ord XMUBD*/
  /*D(buf):1,
         ' inx=',inx:1,' len=',len:1,' suff=');
        wfloat(log,suff); write(log,' chbufi=',chbufi:1);
        snapname(buf,inx,len); writeln(log) end; D*/

  if (chbufi + *len - 1 > CHBUFSIZ)
    fatal(4);
  if (*inx + *len != chbufi) {
    FORLIM = *len;
    for (i = 0; i < FORLIM; i++)
      buf[chbufi + i] = buf[*inx + i];
    *inx = chbufi;
  }
  i = (long)floor(suff + 0.5);
  if (i < 0) {
    (*len)++;
    i = -i;
  }
  do {
    (*len)++;
    i /= 10;
  } while (i != 0);
  *len += 2;
  if (*inx + *len - 1 > CHBUFSIZ)
    fatal(4);
  buf[*inx + *len - 1] = ']';
  j = *len - 2;
  i = (long)floor(suff + 0.5);
  if (i < 0)
    i = -i;
  do {
    buf[*inx + j] = i % 10 + '0';
/* p2c: dpic1.p, line 1370:
 * Note: Using % for possibly-negative arguments [317] */
    j--;
    i /= 10;
  } while (i != 0);
  if ((long)floor(suff + 0.5) < 0) {
    buf[*inx + j] = '-';
    j--;
  }
  buf[*inx + j] = '[';
  chbufi = *inx + *len;   /* ?? */
  /*D ; if debuglevel <> 0 then begin
     snapname(buf,inx,len); writeln(log) end D*/
}  /* addsuffix */


void appendthen(primitive **pr)
{
  primitive *prp;

  copyprim(*pr, &prp);
  prp->parent = *pr;
  prp->next_ = NULL;
  prp->name = NULL;
  prp->textp = NULL;
  prp->shadedp = NULL;
  if (prp->outlinep != NULL)  /* We have to duplicate the stored string */
    copystr(&freeseg, &prp->outlinep, (*pr)->outlinep);
  pexit(*pr, &prp->aat);
  (*pr)->son = prp;
  setthen(&prp->spec);
  *pr = prp;
}


void lineardir(primitive *pr, double dy, double dx, int *state)
{
  if (!(teststflag(*state, XLto) | teststflag(*state, XLdirecton)))
    pr->UU.Uline.endpos = pr->aat;
  switch (pr->direction) {

  case XLup:
    pr->UU.Uline.endpos.ypos += dy;
    break;

  case XLdown:
    pr->UU.Uline.endpos.ypos -= dy;
    break;

  case XLleft:
    pr->UU.Uline.endpos.xpos -= dx;
    break;

  case XLright:
    pr->UU.Uline.endpos.xpos += dx;
    break;
  }
  setstflag(state, XLdirecton);
}


boolean hasoutline(int lx, boolean warn)
{
  boolean hs;

  hs = (lx == XLspline || lx == XLarrow || lx == XLline || lx == XLarc ||
	lx == XLellipse || lx == XLcircle || lx == XLbox);
  if (drawmode == SVG)
    hs = (hs || lx == XLstring);
  if (!hs && warn)
    markerror(858);
  return hs;
}


boolean hasshade(int lx, boolean warn)
{
  boolean hs;

  if (lx == XLellipse || lx == XLcircle || lx == XLbox)
    hs = true;
  else
    hs = (((1L << ((long)drawmode)) &
	   ((1L << ((long)PSTricks)) | (1L << ((long)PGF)) |
	    (1L << ((long)MPost)) | (1L << ((long)MFpic)) | (1L << ((long)PS)) |
	    (1L << ((long)PSfrag)) | (1L << ((long)SVG)))) != 0 &&
	  (lx == XLspline || lx == XLarrow || lx == XLline || lx == XLarc));
  if (drawmode == SVG)
    hs = (hs || lx == XLstring);

  if (!hs && warn)
    markerror(858);
  return hs;
}


/* This is the syntactic action routine. */
void produce(stackinx newp, int p)
{
  /* newp: stackinx; p: integer */
  Char lastc;
  nametype *lastvar, *namptr;
  fbuffer *lastm;
  arg *macp, *lastp;
  primitive *primp, *prp, *eb;
  int i, j, k, kk, lj, ll, nexprs;
  double r, s, t, x1, y1, dx, dy, ts;
  boolean bswitch;
  attribute *WITH, *WITH1;
  primitive *WITH2;
  attribute *WITH3;
  nametype *WITH4;
  int FORLIM;
  fbuffer *WITH5;
  postype *WITH6;

  /* D if (debuglevel > 0) then begin
        writeln( 'Production(newp=',newp:1,
                     ',lexval=',attstack^[newp].lexval:1,
                     ',prod=',p:1,')' )
        end; D */
  /*D if (debuglevel > 0) then begin
        writeln(log, 'Production(newp=',newp:1,
                     ',lexval=',attstack^[newp].lexval:1,
                     ',prod=',p:1,')' );
        flush(log)
        end; D*/
  /* D writeln('Produce Step 1, debuglevel=',debuglevel);
       writeln(log,'Produce Step 1'); D */

  WITH = &attstack[newp];

  /*D with attstack^[newp] do if (debuglevel = 2) and (
     ((p >= block1) and (p <= block3)) or
     ((p > object1) and (p <= object23))
     or (p in [sprintf2,string2,element5,element11,namedobj2]))
       then printobject(prim);
  if debuglevel > 0 then with attstack^[newp] do
     if p in [ assignment1..assignment4,
               expression1..expression5,
               logexpr2,
               ncount2,
               term2..term4,
               factor2,factor3,
               logprod2,
               logval2,logval3,
               lcompare3,lcompare4,
               primary1..primary11
               ] then begin
           write(log,'(After prod=',p:1,')');
           write(log,' value='); wfloat(log,xval);
           write(log,' chbufx='); wfloat(log,chbufx);
           write(log,' chbufi='); wfloat(log,chbufi);
           writeln(log);
           prvars(newp)
           end
     else if p in [position1..position3,
        location1,location2,shift2,shift3,place1..place3] then begin
        write(log,'location:');
        with attstack^[newp] do wpair(log,xval,yval);
        writeln(log); flush(log); end D*/
  /*D;if (linesignal > 0) and (p=input2) then begin
      writeln(errout,'bottom:'); flush(errout); end D*/
  switch (p) {

  case -2:
    inittwo();
    break;

  case -1:
    if (envblock != NULL)
      envblock->direction = XLright;
    inlogic = false;
    inbuf = NULL;
    break;

  /* METAGOAL = input "<EOF>" */

  case METAGOAL1:
    break;

  /* input = "<EMPTY>" */
  /* | input picture NL */
  case input1:
    break;

  case input2:
    /*D if debuglevel > 0 then writeln(log,'deletetree:');
        if linesignal > 0 then writeln(errout,'deletetree:');flush(errout);D*/
    deletetree(&envblock);

    /*D if debuglevel > 0 then writeln(log,'deleteinbufs:');
        if linesignal > 0 then writeln(errout,'deleteinbufs:');
        flush(errout);D*/
    deleteinbufs(&freeinbuf);

    /*D if debuglevel > 0 then writeln(log,'inittwo:');
        if linesignal > 0 then writeln(errout,'inittwo:'); flush(errout); D*/
    inittwo();

    if (envblock != NULL)
      envblock->direction = XLright;
    break;

  /* picture = start NL elementlist "<END>" */
  /* | start NL elementlist NL "<END>" */
  case picture1:
  case picture2:
    if (WITH->prim != NULL)
      attstack[newp + 2].prim = WITH->prim;
    WITH1 = &attstack[newp + 2];
    getnesw(WITH1->prim);
    /*D if debuglevel > 0 then begin
       writeln(log,' Global dimensions:');
       write(log,'(n,s)(e,w)=');
       wpair(log,north,south); wpair(log,east,west);
       writeln(log); flush(log)
       end; D*/
    if (envblock != NULL) {
      WITH2 = envblock;
      WITH2->aat.xpos = (east + west) * 0.5;
      WITH2->aat.ypos = (north + south) * 0.5;
      WITH2->UU.Ublock.blockheight = north - south;
      WITH2->UU.Ublock.blockwidth = east - west;
    }
    if (((1L << ((long)drawmode)) &
	 ((1L << ((long)xfig)) | (1L << ((long)SVG)))) != 0) {
      r = 0.0;
      if (drawmode == xfig)
	shift(envblock, -west, -south);
      else {
	r = envblock->UU.Ublock.env[XLlinethick - XLenvvar - 1] / 2 / 72 *
	    envblock->UU.Ublock.env[XLscale - XLenvvar - 1];
	/*D if debuglevel > 0 then begin
	   write(log,' west='); wfloat(log,west);
	   write(log,' south='); wfloat(log,south);
	   write(log,' r='); wfloat(log,r);
	   write(log,' shift=('); wfloat(log,-west+r);
	   write(log,','); wfloat(log,-south+r);
	   writeln(log,')')
	   end; D*/
	shift(envblock, r - west, r - south);
      }
      north += 2 * r - south;
      east += 2 * r - west;
      west = 0.0;
      south = 0.0;
      xfheight = north;
      /*D; if debuglevel > 0 then begin flush(log);
         writeln(log,'After shift:');
         printobject( envblock );
         printobject( envblock^.son );
         writeln(log); flush(log) end D*/
    }
    WITH3 = &attstack[newp];
    getscale(WITH3->xval, WITH3->yval, envblock, &scale, &fsc);
    /*D if debuglevel > 0 then begin
       write(log,'Starting drawtree ================= ');
       with attstack^[newp] do if (xval > 0.0) and (east > west) then
          begin write( log,'fsc='); wfloat(log, fsc ) end;
       writeln(log);
       snaptree(envblock,0);
       writeln(log); flush(log) end; D*/
    drawtree(north, south, east, west, envblock);
    break;

  /* NL = ["^M"]"<NL>" */
  /* | "<ERROR>" */
  case NL1:
  case NL2:
    break;

  /* start = "<START>" */
  /* | "<START>" term */
  /* | "<START>" term term */
  case start1:   /*D if debuglevel > 0 then begin writeln(log,'.PS'); D*/
    break;

  /* setenvironment(newp); */
  /*D
  flush(log) end D*/
  case start2:
    WITH->xval = attstack[newp + 1].xval;
    break;

  case start3:
    WITH->xval = attstack[newp + 1].xval;
    WITH->yval = attstack[newp + 2].xval;
    break;

  /* elementlist = "<EMPTY>" */
  case elementlist1:
    WITH->state = 0;
    break;

  /* | element */
  case elementlist2:
    WITH->state = 0;
    if (WITH->prim != NULL && WITH->lexval != XLcontinue) {
      i = newp - 1;
      j = 1;
      /*D if debuglevel>0 then writeln(log,
         ' Elementlist2: Searching for last drawn element'); D*/
      while (i > j) {
	if ((attstack[i].lexval == XLendfor || attstack[i].lexval == XFOR ||
	     attstack[i].lexval == XLBRACE ||
	     attstack[i].lexval == XSEMICOLON ||
	     attstack[i].lexval == XNL) && attstack[i].prim == NULL)
	      /*repeat ,Xrepeat */
		i--;
	else
	  j = i;
      }
      /*D if debuglevel > 0 then begin
         if i = 1 then write(log,' Not found') else write(log,' Found:');
         writeln(log,' newp= ',newp:1,
            ', attstack^[',i:1,'].lexval= ',attstack^[i].lexval:1);
         if attstack^[i].prim<>nil then begin
            writeln(log,'attstack^[',i:1,'].prim= ');
            snaptype(log,attstack^[i].prim^.ptype)
            end
         end; D*/
      if (attstack[i].prim != NULL) {
	addelem(attstack[i].prim, WITH->prim);
	/*D if debuglevel > 0 then printobject(attstack^[i].prim); D*/
	WITH->state = i - newp;
      } else if (attstack[i].lexval == XLBRACKET) {
	attstack[i + 1].prim = WITH->prim;
	/*D if debuglevel > 0 then printobject(attstack^[i+1].prim); D*/
	WITH->state = i - newp + 1;
      } else {
	attstack[i].prim = WITH->prim;
	/*D if debuglevel > 0 then printobject(attstack^[i].prim); D*/
	WITH->state = i - newp;
      }
    }
    break;

  /* | elementlist NL element */
  case elementlist3:
    if (attstack[newp + 2].prim != NULL) {
      if (attstack[newp + 2].lexval != XLcontinue) {
	if (WITH->prim == NULL) {
	  attstack[newp] = attstack[newp + 2];
	  doprod(elementlist2);
	} else
	  addelem(attstack[newp + WITH->state].prim, attstack[newp + 2].prim);
      }
    }
    break;

  /* term = factor */
  /* | term "*" factor */
  /* | term "/" factor */
  /* | term "%" factor */
  case term1:
    break;

  case term2:
    WITH->xval *= attstack[newp + 2].xval;
    break;

  case term3:
    if (attstack[newp + 2].xval == 0.0) {
      markerror(852);
      WITH->xval = 0.0;
    } else
      WITH->xval /= attstack[newp + 2].xval;
    break;

  case term4:
    if (attstack[newp + 2].xval == 0.0) {
      markerror(852);
      WITH->xval = 0.0;
    } else {
      WITH->xval = (long)floor(WITH->xval + 0.5) %
		   (long)floor(attstack[newp + 2].xval + 0.5);
/* p2c: dpic1.p, line 1627:
 * Note: Using % for possibly-negative arguments [317] */
    }
    break;

  /* element = namedobj */
  /* | "<Label>" suffix ":" position */
  /* | assignlist */
  /* | "<directon>" */
  /* | "<LaTeX>" */
  /* | command */
  /* | lbrace "lbrace" elementlist optnl "rbrace" */
  /* | ifpart */
  /* | elsehead "lbrace" elementlist optnl "rbrace" */
  /* | for "rbrace" */
  /* | "command" stringexpr */
  /* | "exec" stringexpr */
  case element1:
    if (WITH->prim != NULL) {
      prp = WITH->prim;
      if (WITH->prim->ptype == XLspline || WITH->prim->ptype == XLmove ||
	  WITH->prim->ptype == XLarrow || WITH->prim->ptype == XLline) {
	if (WITH->startchop != 0.0) {
	  WITH2 = WITH->prim;
	  dx = WITH2->UU.Uline.endpos.xpos - WITH2->aat.xpos;
	  dy = WITH2->UU.Uline.endpos.ypos - WITH2->aat.ypos;
	  s = linlen(dx, dy);
	  /*D if debuglevel = 2 then begin
	     write(log,' element1 startchop='); wfloat(log,startchop);
	     write(log,' (dx,dy)='); wpair(log,dx,dy);
	     write(log,' s='); wfloat(log,s); writeln(log) end; D*/
	  if (s != 0.0) {
	    t = WITH->startchop / s;
	    WITH2->aat.xpos += t * dx;
	    WITH2->aat.ypos += t * dy;
	  }
	}
	while (prp->son != NULL)
	  prp = prp->son;
	if (WITH->endchop != 0.0) {
	  dx = prp->UU.Uline.endpos.xpos - prp->aat.xpos;
	  dy = prp->UU.Uline.endpos.ypos - prp->aat.ypos;
	  s = linlen(dx, dy);
	  /*D if debuglevel = 2 then begin
	     write(log,' element1 endchop='); wfloat(log,endchop);
	     write(log,' (dx,dy)='); wpair(log,dx,dy);
	     write(log,' s='); wfloat(log,s); writeln(log) end; D*/
	  if (s != 0.0) {
	    t = WITH->endchop / s;
	    prp->UU.Uline.endpos.xpos -= t * dx;
	    prp->UU.Uline.endpos.ypos -= t * dy;
	  }
	}
      }
      /*D ; if debuglevel > 0 then begin
         write(log,' element1 Here=');
         with envblock^ do wpair(log,here.xpos,here.ypos); writeln(log);
         printobject(prim)
         end D*/
      pexit(prp, &envblock->UU.Ublock.here);
    }
    break;

  /* | "<Label>" suffix ":" position */
  case element2:
    if (attstack[newp + 1].lexval != XEMPTY)
      addsuffix(chbuf, &WITH->chbufx, &WITH->length, attstack[newp + 1].xval);
    prp = findplace(envblock->son, chbuf, WITH->chbufx, WITH->length);
    if (prp == NULL) {
      newprim(&WITH->prim, XLabel, envblock);
      newstr(&WITH->prim->name);
      storestring(&freeseg, WITH->prim->name, chbuf, WITH->chbufx,
		  WITH->length);
      prp = WITH->prim;
    } else if (prp->ptype != XLabel) {
      newprim(&WITH->prim, XLabel, envblock);
      WITH->prim->name = prp->name;
      prp->name = NULL;
      prp = WITH->prim;
    }
    prp->aat.xpos = attstack[newp + 3].xval;
    prp->aat.ypos = attstack[newp + 3].yval;
    /*D; if (debuglevel > 0) and (prim<>nil) then printobject(prim)D*/
    break;

  /* | assignlist */
  case element3:
    WITH->lexval = XLBRACE;
    break;

  /* | "<directon>" */
  case element4:
    envblock->direction = WITH->lexval;
    WITH->lexval = XLBRACE;
    break;

  /* | "<LaTeX>" */
  case element5:
    newprim(&WITH->prim, XLaTeX, envblock);
    newstr(&WITH->prim->textp);
    storestring(&freeseg, WITH->prim->textp, chbuf, WITH->chbufx,
		WITH->length);
    break;

  /* | command */
  case element6:
    WITH->lexval = XLBRACE;
    break;

  /* | lbrace "lbrace" elementlist optnl "rbrace" */
  case element7:
    envblock->UU.Ublock.here.xpos = WITH->xval;
    envblock->UU.Ublock.here.ypos = WITH->yval;
    if (WITH->state == XLright || WITH->state == XLleft ||
	WITH->state == XLdown || WITH->state == XLup)
      envblock->direction = WITH->state;
    if (attstack[newp + 2].state == 0)
      attstack[newp] = attstack[newp + 2];
    break;

  /* | ifpart */
  case element8:
    break;

  /* | elsehead "lbrace" elementlist optnl "rbrace" */
  case element9:
    break;

  /* | for "rbrace" */
  case element10:
    break;

  /* | "command" stringexpr */
  case element11:
    if (attstack[newp + 1].prim != NULL) {
      newprim(&WITH->prim, XLaTeX, envblock);
      WITH->prim->textp = attstack[newp + 1].prim->textp;
      attstack[newp + 1].prim->textp = NULL;
      if (envblock->son == attstack[newp + 1].prim)
	envblock->son = WITH->prim;
      deletestringbox(&attstack[newp + 1].prim);
    }
    break;

  /* | "exec" stringexpr */
  case element12:
    if (attstack[newp + 1].prim != NULL) {
      if (attstack[newp + 1].prim->textp != NULL) {
	if (attstack[newp + 1].prim->textp->segmnt != NULL) {
	  WITH4 = attstack[newp + 1].prim->textp;
	  newbuf(&lastm);
	  lastm->carray[1] = nlch;
	  lastm->savedlen = 1;
	  copyleft(lastm, &inbuf);
	  FORLIM = WITH4->len;
	  for (i = 1; i <= FORLIM; i++)
	    lastm->carray[i] = WITH4->segmnt[WITH4->seginx + i - 1];
	  lastm->savedlen = WITH4->len;
	  copyleft(lastm, &inbuf);
	  lastm->carray[1] = nlch;
	  lastm->savedlen = 1;
	  copyleft(lastm, &inbuf);
	  deletestringbox(&attstack[newp + 1].prim);   /*D,2D*/
	  disposebufs(&lastm);
	}
      }
    }
    WITH->lexval = XLBRACE;
    break;
    /* | loop "}" */
    /* | break */
    /*repeat element13,element14: ; */

  /* lbrace = "<EMPTY>" */
  case lbrace1:
    WITH->xval = envblock->UU.Ublock.here.xpos;
    WITH->yval = envblock->UU.Ublock.here.ypos;
    WITH->state = envblock->direction;
    break;

  /* namedobj = object */
  case namedobj1:
    if (WITH->prim != NULL) {  /* then, arc, deferred shift */
      prp = WITH->prim;
      while (isthen(WITH->prim))
	WITH->prim = WITH->prim->parent;
      if (prp != WITH->prim) {
	WITH->prim->name = prp->name;
	prp->name = NULL;
      }
      if (WITH->prim->ptype == XLarc)
	arcenddir(prp);
      if (teststflag(WITH->state, XLat)) {
	WITH2 = WITH->prim;
	i = getstval(WITH->state);
	if (i == XLfloat) {
	  getnesw(WITH->prim);
	  dx = west + WITH->startchop;
	  dy = south + WITH->endchop;
	} else if (WITH->root != NULL) {
	  corner(WITH->root, i, &dx, &dy);
	  WITH->root = NULL;
	} else
	  corner(WITH->prim, i, &dx, &dy);
	if (WITH2->ptype != XLarc)
	  shift(WITH->prim, WITH->xval - dx, WITH->yval - dy);
	else {
	  x1 = WITH2->aat.xpos +
	       WITH2->UU.Uline.aradius * cos(WITH2->UU.Uline.endpos.xpos);
	      /* from */
	  y1 = WITH2->aat.ypos +
	       WITH2->UU.Uline.aradius * sin(WITH2->UU.Uline.endpos.xpos);
	  if (teststflag(WITH->state, XLto)) {
		/* to X from Here|Y implied */
		  if (i != XEMPTY && i != XDc)
	      markerror(858);
	    r = WITH2->aat.xpos + WITH2->UU.Uline.aradius * cos(
		    WITH2->UU.Uline.endpos.xpos + WITH2->UU.Uline.endpos.ypos);
		/* to */
	    s = WITH2->aat.ypos + WITH2->UU.Uline.aradius * sin(
		    WITH2->UU.Uline.endpos.xpos + WITH2->UU.Uline.endpos.ypos);
	    WITH2->aat.xpos = WITH->xval;
	    WITH2->aat.ypos = WITH->yval;
	    WITH2->UU.Uline.aradius = linlen(r - WITH2->aat.xpos,
					    s - WITH2->aat.ypos);
	    setangles(&WITH2->UU.Uline.endpos.xpos,
		      &WITH2->UU.Uline.endpos.ypos, WITH2->aat, x1, y1, r, s);
	  } else if (teststflag(WITH->state, XLfrom)) {
	    if (i != XEMPTY && i != XDc)
	      markerror(858);
	    WITH2->aat.xpos = WITH->xval;
	    WITH2->aat.ypos = WITH->yval;
	    t = datan(y1 - WITH2->aat.ypos, x1 - WITH2->aat.xpos);
	    r = WITH2->aat.xpos +
		WITH2->UU.Uline.aradius * cos(t + WITH2->UU.Uline.endpos.ypos);
	    s = WITH2->aat.ypos +
		WITH2->UU.Uline.aradius * sin(t + WITH2->UU.Uline.endpos.ypos);
	    WITH2->UU.Uline.aradius = linlen(x1 - WITH2->aat.xpos,
					    y1 - WITH2->aat.ypos);
	    setangles(&WITH2->UU.Uline.endpos.xpos,
		      &WITH2->UU.Uline.endpos.ypos, WITH2->aat, x1, y1, r, s);
	  } else
	    shift(WITH->prim, WITH->xval - dx, WITH->yval - dy);
	}
      }
    }
    break;

  /* | "<Label>" suffix ":" object */
  case namedobj2:
    if (attstack[newp + 3].prim != NULL) {
      if (attstack[newp + 1].lexval != XEMPTY)
	addsuffix(chbuf, &WITH->chbufx, &WITH->length,
		  attstack[newp + 1].xval);
      primp = findplace(envblock->son, chbuf, WITH->chbufx, WITH->length);
      if (primp != NULL) {
	attstack[newp + 3].prim->name = primp->name;
	primp->name = NULL;
      } else {
	WITH2 = attstack[newp + 3].prim;
	/*D if debuglevel = 2 then
	   writeln(log,'Label not found, creating it'); D*/
	newstr(&WITH2->name);
	storestring(&freeseg, WITH2->name, chbuf, WITH->chbufx, WITH->length);
      }
      attstack[newp] = attstack[newp + 3];
      doprod(namedobj1);
    }
    break;

  /* suffix = "<EMPTY>" */
  /* | "[" expression "]" */
  case suffix1:
    WITH->lexval = XEMPTY;
    break;

  case suffix2:
    WITH->xval = attstack[newp + 1].xval;
    break;

  /* position = pair */
  case position1:
    break;

  /* | expression "between" position "and" position */
  /* | expression "<" position "," position "<compare>" shift */
  case position2:
  case position4:
    r = WITH->xval;
    WITH->xval = attstack[newp + 2].xval +
		 r * (attstack[newp + 4].xval - attstack[newp + 2].xval);
    WITH->yval = attstack[newp + 2].yval +
		 r * (attstack[newp + 4].yval - attstack[newp + 2].yval);
    if (p == position4) {
      if (attstack[newp + 5].lexval != XGT)
	markerror(869);
      else if (attstack[newp + 6].lexval != XEMPTY) {
	WITH->xval += attstack[newp + 6].xval;
	WITH->yval += attstack[newp + 6].yval;
      }
    }
    break;

  /* | expression "of" "the" "way" "between" position "and" position*/
  case position3:
    r = WITH->xval;
    WITH->xval = attstack[newp + 5].xval +
		 r * (attstack[newp + 7].xval - attstack[newp + 5].xval);
    WITH->yval = attstack[newp + 5].yval +
		 r * (attstack[newp + 7].yval - attstack[newp + 5].yval);
    break;

  /* assignlist = assignment */
  /* | assignlist "," assignment */
  case assignlist1:
    break;

  case assignlist2:
    WITH->xval = attstack[newp + 2].xval;
    break;

  /* command = "print" expression redirect */
  /* | "print" position redirect */
  /* | "print" stringexpr redirect */
  case command1:
  case command2:
  case command3:
    WITH1 = &attstack[newp + 1];
    if (attstack[newp + 2].lexval == XEMPTY) {
      switch (p) {

      case command1:
	wfloat(&errout, WITH1->xval);
	break;

      case command2:
	wpair(&errout, WITH1->xval, WITH1->yval);
	break;

      case command3:
	if (WITH1->prim != NULL)
	  wstring(&errout, WITH1->prim->textp);
	break;
      }
      putc('\n', errout);
      consoleflush();
    } else if (attstack[newp + 2].state == 0) {
#ifndef SAFE_MODE
      switch (p) {

      case command1:
	wfloat(&redirect, WITH1->xval);
	break;

      case command2:
	wpair(&redirect, WITH1->xval, WITH1->yval);
	break;

      case command3:
	if (WITH1->prim != NULL)
	  wstring(&redirect, WITH1->prim->textp);
	break;
      }
      putc('\n', redirect);
      if (redirect != NULL)
	fclose(redirect);
      redirect = NULL;
#endif
    }
    if (p == command3)
      deletestringbox(&WITH1->prim);
    break;

  /* | "reset" */
  case command4:
    resetenv(0, envblock);
    break;

  /* | "reset" resetlist */
  case command5:
    break;

  /* | "sh" stringexpr */
  case command6:
    WITH1 = &attstack[newp + 1];
    if (WITH1->prim != NULL) {
      if (WITH1->prim->textp != NULL) {
	WITH4 = WITH1->prim->textp;
	if (WITH4->segmnt != NULL) {
	  if (WITH4->seginx + WITH4->len >= CHBUFSIZ)
	    markerror(866);
	  else {
	    WITH4->segmnt[WITH4->seginx + WITH4->len] = '\0';
#ifdef SAFE_MODE
	    markerror(906);
#else
	    if (safemode)
	      markerror(906);
	    else
	      system(&WITH4->segmnt[WITH4->seginx]);
#endif
	  }
	}
      }
      deletestringbox(&WITH1->prim);
    }
    break;

  /* | "copy" stringexpr */
  case command7:
    WITH1 = &attstack[newp + 1];
    if (WITH1->prim != NULL) {
#ifdef SAFE_MODE
      markerror(906);
#else
      pointinput(WITH1->prim->textp);
#endif
      deletestringbox(&WITH1->prim);
    }
    break;

  /* | defhead optnl "lbrace" "rbrace" */
  case command8:
    break;

  /* | "undefine" "<name>" */
  /* | "undefine" "<Label>" */
  case command9:
  case command10:
    attstack[newp] = attstack[newp + 1];
    macp = findmacro(macros, chbuf, WITH->chbufx, WITH->length, &lastp);
    if (macp != NULL) {
      if (lastp != NULL)
	lastp->nextb = macp->nextb;
      if (macp == macros)   /*D,5D*/
	macros = macp->nextb;
      disposebufs(&macp->argbody);
      Free(macp);
    }
    break;

  /* optnl = "<EMPTY>" */
  /* | NL */
  case optnl1:
  case optnl2:
    break;

  /* ifpart = ifhead "lbrace" elementlist optnl "rbrace" */
  case ifpart1:
    break;

  /* elsehead = ifpart "else" */
  case elsehead1:
    if (WITH->xval == 0.0) {
      attstack[newp + 1].lexval = XLBRACE;
      WITH->lexval = XLBRACE;
    } else
      skiptobrace();
    break;

  /* for = forhead "lbrace" elementlist optnl */
  /* | for forincr "<endfor>" elementlist optnl */
  case for1:
  case for2:
    break;

  /* stringexpr = string */
  case stringexpr1:
    break;

  /* | stringexpr "+" string */
  case stringexpr2:
    if (attstack[newp + 2].prim != NULL) {
      WITH2 = WITH->prim;
      prp = attstack[newp + 2].prim;
      WITH2->UU.Ubox.boxwidth += prp->UU.Ubox.boxwidth;
      WITH2->UU.Ubox.boxheight = Max(WITH2->UU.Ubox.boxheight,
				     prp->UU.Ubox.boxheight);
      if (prp->textp != NULL) {
	if (WITH2->textp == NULL) {
	  WITH2->textp = prp->textp;
	  prp->textp = NULL;
	} else if (WITH2->textp->segmnt == prp->textp->segmnt &&
	    WITH2->textp->seginx + WITH2->textp->len == prp->textp->seginx) {
	  /*D if debuglevel > 0 then writeln(log,' stringexpr2 branch 1,',
	     ' seginx,length=',textp^.seginx:1,',',textp^.len:1,
	     ' seginx,length=',prp^.textp^.seginx:1,',',prp^.textp^.len:1); D*/
	  WITH2->textp->len += prp->textp->len;
	  putbval(WITH2->textp->segmnt, bval(WITH2->textp->segmnt) - 1);
	  prp->textp->segmnt = NULL;
	} else
	  appendstring(WITH2->textp, prp->textp->segmnt, prp->textp->seginx,
		       prp->textp->len);
      }
      deletetree(&attstack[newp + 2].prim);
    }
    break;

  /* break = "break" */
  /*repeat break1:
     if inbuf = nil then begin end
     else if inbuf^.attrib <= 0 then begin end
     else if attstack^[inbuf^.attrib].lexval <> Xrepeat then markerror(858)
     else begin
        while inbuf^.nextb <> nil do inbuf := inbuf^.nextb ;
        inbuf^.attrib := -1 ;
        inbuf^.readx := inbuf^.savedlen+1
        end;
     */

  /* string = "<string>" */
  case string1:
    newprim(&WITH->prim, XLstring, envblock);
    eb = findenv(envblock);
    WITH2 = WITH->prim;
    WITH2->UU.Ubox.boxheight = eb->UU.Ublock.env[XLtextht - XLenvvar - 1];
    WITH2->UU.Ubox.boxwidth = eb->UU.Ublock.env[XLtextwid - XLenvvar - 1];
    if (drawmode == xfig && WITH2->UU.Ubox.boxwidth == 0.0) {
      /* To keep xfig from crashing, assume text height is 0.1
         and a character is 0.1*0.75 wide */
      eb = findenv(envblock);
      if (WITH2->UU.Ubox.boxheight == 0.0)
	WITH2->UU.Ubox.boxheight = 0.1 * eb->UU.Ublock.env[XLscale - XLenvvar - 1];
      WITH2->UU.Ubox.boxwidth = WITH2->UU.Ubox.boxheight * WITH->length * 0.75;
    }
    WITH2->UU.Ubox.boxradius = 0.0;
    newstr(&WITH2->textp);
    storestring(&freeseg, WITH2->textp, chbuf, WITH->chbufx, WITH->length);
    break;

  /* | sprintf ")" */
  case string2:
    break;

  /* assignment = "<name>" suffix "=" expression */
  /* | "<name>" suffix "=" assignment */
  case assignment1:
  case assignment2:
    if (attstack[newp + 1].lexval != XEMPTY)
      addsuffix(chbuf, &WITH->chbufx, &WITH->length, attstack[newp + 1].xval);
    WITH->varname = findname(envblock, chbuf, WITH->chbufx, WITH->length,
			     &lastvar, &k);
    if (WITH->varname == NULL && attstack[newp + 2].lexval != XEQ)
      WITH->varname = glfindname(envblock->parent, chbuf, WITH->chbufx,
				 WITH->length, &namptr, &kk);
    if (WITH->varname == NULL) {
      newstr(&WITH->varname);
      j = varhash(chbuf, WITH->chbufx, WITH->length);
      storestring(&freeseg, WITH->varname, chbuf, WITH->chbufx, WITH->length);
      WITH2 = envblock;
      /*D if debuglevel > 1 then begin
          writeln(log,'assignment1: envblock=',ordp(envblock):1,
                  ' eqstr val=',k:1);
          write(log,' lastvar=',ordp(lastvar):1);
                  if lastvar<>nil then with lastvar^ do
                    snapname(segmnt,seginx,len); writeln(log) end; D*/
      /*D if debuglevel > 1 then begin writeln(log,
                   ' varname=',ordp(varname):1); prvars(newp) end; D*/
      if (lastvar == NULL)
	WITH2->UU.Ublock.vars[j] = WITH->varname;
      else if (k < 0) {
	if (WITH2->UU.Ublock.vars[j]->next_ == NULL)
	  WITH2->UU.Ublock.vars[j]->next_ = WITH->varname;
	else {
	  WITH->varname->next_ = lastvar->next_;
	  lastvar->next_ = WITH->varname;
	}
      } else if (lastvar == WITH2->UU.Ublock.vars[j]) {
	WITH->varname->next_ = WITH2->UU.Ublock.vars[j];
	WITH2->UU.Ublock.vars[j] = WITH->varname;
      } else {
	namptr = WITH2->UU.Ublock.vars[j];
	/* while (namptr^.next<>nil) and (namptr^.next<>lastvar) do */
	while (namptr->next_ != lastvar)
	  namptr = namptr->next_;
	namptr->next_ = WITH->varname;
	WITH->varname->next_ = lastvar;
      }
      WITH2->UU.Ublock.nvars[j]++;
      WITH->varname->val = 0.0;
    }
    if (attstack[newp + 2].lexval == XEQ)
      WITH->varname->val = attstack[newp + 3].xval;
    else
      eqop(&WITH->varname->val, attstack[newp + 2].lexval,
	   attstack[newp + 3].xval);
    WITH->xval = WITH->varname->val;
    break;

  /* | "<envvar>" "=" expression */
  /* | "<envvar>" "=" assignment */
  case assignment3:
  case assignment4:
    if (envblock != NULL) {
      if (WITH->lexval == XLscale)
	resetscale(attstack[newp + 2].xval, attstack[newp + 1].lexval,
		   envblock);
      else if (WITH->lexval == XLarrowhead && drawmode == TeX &&
	       attstack[newp + 2].xval == 0.0)
	markerror(858);
      else {
	if (envblock->UU.Ublock.env == NULL)
	  inheritenv(envblock);
	eqop(&envblock->UU.Ublock.env[WITH->lexval - XLenvvar - 1],
	     attstack[newp + 1].lexval, attstack[newp + 2].xval);
      }
      WITH->xval = envblock->UU.Ublock.env[WITH->lexval - XLenvvar - 1];
      /*D if debuglevel > 0 then begin
         write(log,
         ' Assignment3or4 envblock[',ordp(envblock),']: lexval=',lexval:1,
         ' value='); wfloat(log,envblock^.env^[lexval]); writeln(log) end; D*/
      WITH->startchop = WITH->lexval;
      if (WITH->lexval == XLdashwid || WITH->lexval == XLlinethick) {
	newprim(&WITH->prim, XLaTeX, envblock);
	if (WITH->lexval == XLlinethick)
	  WITH->prim->lthick = WITH->xval;
	else
	  WITH->prim->lparam = WITH->xval;
      } else
	WITH->lexval = XLBRACE;
    }
    break;

  /* expression = term */
  /* | "+" term */
  /* | "-" term */
  /* | expression "+" term */
  /* | expression "-" term */
  case expression1:
    break;

  case expression2:
    WITH->xval = attstack[newp + 1].xval;
    break;

  case expression3:
    WITH->xval = -attstack[newp + 1].xval;
    break;

  case expression4:
    WITH->xval += attstack[newp + 2].xval;
    break;

  case expression5:
    WITH->xval -= attstack[newp + 2].xval;
    break;

  /* ifhead = setlogic "if" logexpr "then" */
  case ifhead1:
    inlogic = false;
    WITH->xval = attstack[newp + 2].xval;
    /*D if debuglevel > 0 then begin
        if xval <> 0.0 then writeln(log,' true') else writeln(log,' false')
        end; D*/
    if (WITH->xval != 0.0) {
      for (i = 0; i <= 3; i++)
	attstack[newp + i].lexval = XLBRACE;
    } else
      skiptobrace();
    break;

  /* setlogic = "<EMPTY>" */
  case setlogic1:
    inlogic = true;
    break;

  /* logexpr = logprod */
  /* | logexpr "||" logprod */
  case logexpr1:
    break;

  case logexpr2:
    if (WITH->xval != 0.0 || attstack[newp + 2].xval != 0.0)
      WITH->xval = 1.0;
    else
      WITH->xval = 0.0;
    break;

  /* forhead = "for" assignlist "to" expression do */
  case forhead1:
    WITH->xval = attstack[newp + 1].xval;   /* initial value */
    WITH->yval = attstack[newp + 4].xval;   /* increment */
    WITH->endchop = attstack[newp + 3].xval;   /* final value */
    WITH->length = 0;
    if (WITH->xval == WITH->endchop) {
      WITH->length = -1;
      WITH->yval = 1.0;
    } else if (attstack[newp + 4].lexval == XSTAR) {
      t = 0.0;
      if (WITH->xval == 0.0)
	WITH->length = 860;
      else
	t = WITH->endchop * WITH->xval;
      if (t < 0.0)   /* or (yval < 0.0) */
	WITH->length = 862;
      else if (t == 0.0 && fabs(WITH->yval * WITH->xval) != 0.0)
	WITH->length = 860;
      else if (WITH->yval == 0.0 && t != 0.0)
	WITH->length = 860;
      else if (fabs(WITH->yval) == 1 &&
	       fabs(WITH->xval) != fabs(WITH->endchop))
	WITH->length = 860;
      else
	WITH->length = 1;
    } else if (WITH->yval == 0.0)
      WITH->length = 860;
    else if ((WITH->endchop - WITH->xval) * WITH->yval > 0)
      WITH->length = -1;
    if (WITH->length > 800) {
      markerror(WITH->length);
      WITH->length = 0;
    }
    /* store loop variable */
    WITH->startchop = attstack[newp + 1].startchop;
    if (WITH->startchop == 0.0) {
      WITH->varname = attstack[newp + 1].varname;
      WITH->varname->val = WITH->xval;
    } else if ((long)floor(WITH->startchop + 0.5) != XLscale) {
      if (envblock->UU.Ublock.env == NULL)
	inheritenv(envblock);
      envblock->UU.Ublock.env[(int)((long)floor(WITH->startchop + 0.5)) -
			   XLenvvar - 1] = WITH->xval;
    } else
      resetscale(WITH->xval, XEQ, envblock);

    if (WITH->length == 0)
      skiptobrace();
    else {
      for (i = 0; i <= 4; i++)
	attstack[newp + i].lexval = XLBRACE;
      readfor(NULL, newp, &inbuf);
    }
    break;

  /* forincr = "<EMPTY>" */
  case forincr1:
    WITH->lexval = XLBRACE;
    WITH1 = &attstack[newp - 1];
    if (WITH1->varname != NULL)
      WITH1->xval = WITH1->varname->val;
    else
      WITH1->xval = envblock->UU.Ublock.env[(int)((long)floor(WITH1->startchop + 0.5)) -
					 XLenvvar - 1];
    bswitch = false;
    if (WITH1->length < 0) {
      if (WITH1->yval == 0)
	bswitch = true;
      else {
	WITH1->xval += WITH1->yval;
	if (WITH1->yval > 0 && WITH1->xval > WITH1->endchop ||
	    WITH1->yval < 0 && WITH1->xval < WITH1->endchop)
	  bswitch = true;
      }
    } else if (WITH1->xval == 0)
      bswitch = true;
    else {
      WITH1->xval *= WITH1->yval;
      if (fabs(WITH1->yval) >= 1.0 && fabs(WITH1->xval) > fabs(WITH1->endchop) ||
	  fabs(WITH1->yval) < 1.0 && fabs(WITH1->xval) < fabs(WITH1->endchop))
	bswitch = true;
    }
    if (WITH1->varname != NULL)
      WITH1->varname->val = WITH1->xval;
    else if ((long)floor(WITH1->startchop + 0.5) != XLscale)
      envblock->UU.Ublock.env[(int)((long)floor(WITH1->startchop + 0.5)) -
			   XLenvvar - 1] = WITH1->xval;
    else
      resetscale(WITH1->xval, XEQ, envblock);
    /*D if debuglevel > 0 then begin write(log,'forincr1: xval=');
        wfloat(log,xval); writeln(log,' bswitch=',bswitch) end; D*/
    if (bswitch) {
      while (inbuf->attrib < 0) {
	lastm = inbuf;
	inbuf = newinbuf(inbuf->higherb);   /*D,4D*/
	/*D; if debuglevel > 0 then wrbuf(inbuf,3,0) D*/
	disposebufs(&lastm);
      }
      lastm = inbuf;
      inbuf = newinbuf(inbuf->higherb);   /*D,4D*/
      /*D; if debuglevel > 0 then wrbuf(inbuf,3,0) D*/
      disposebufs(&lastm);
    }
    break;

  /* loop = loophead "{" elementlist optnl */
  /* | loopincr elementlist optnl */
  /*repeat loop1,loop2: ; */

  /* do = "do" */
  /* | by expression "do" */
  case do1:
    WITH->xval = 1.0;
    break;

  case do2:
    WITH->xval = attstack[newp + 1].xval;
    break;

  /* loophead = "repeat" */
  /*repeat loophead1: ; */
  /* Do not enter into nullprod file */

  /* loopincr = loop "~" */
  /*repeat loopincr1: attstack^[newp+1].lexval := XLBRACE ; */

  /* by = "by" */
  /* | "by" "*" */
  case by1:
    break;

  case by2:
    WITH->lexval = attstack[newp + 1].lexval;
    break;

  /* redirect = "<EMPTY>" */
  case redirect1:
    WITH->lexval = XEMPTY;
    break;

  /* | "<compare>" stringexpr */
  case redirect2:
    WITH1 = &attstack[newp + 1];
    attstack[newp].state = 1;
    bswitch = false;
    if (WITH1->prim != NULL) {
      if (WITH1->prim->textp == NULL)
	markerror(861);
      else if (p == redirect2 && attstack[newp].lexval != XGT)
	markerror(869);
      else if (safemode)
	markerror(906);
      else
	bswitch = true;
    }
#ifdef SAFE_MODE
    if (bswitch)
      markerror(906);
#else
    if (bswitch)
      pointoutput(true, WITH1->prim->textp, &attstack[newp].state);
#endif
    deletestringbox(&WITH1->prim);
    break;

  /* | "<compare>" "<compare>" stringexpr */
  case redirect3:
    WITH1 = &attstack[newp + 2];
    attstack[newp].state = 1;
    bswitch = false;
    if (WITH1->prim != NULL) {
      if (WITH1->prim->textp == NULL)
	markerror(861);
      else if (attstack[newp].lexval != XGT ||
	       attstack[newp + 1].lexval != XGT)
	markerror(869);
      else if (safemode)
	markerror(906);
      else
	bswitch = true;
    }
#ifdef SAFE_MODE
    if (bswitch)
      markerror(906);
#else
    if (bswitch)
      pointoutput(false, WITH1->prim->textp, &attstack[newp].state);
#endif
    deletestringbox(&WITH1->prim);
    break;

  /* resetlist = "<envvar>" */
  /* | resetlist "," "<envvar>" */
  case resetlist1:
    resetenv(WITH->lexval, envblock);
    break;

  case resetlist2:
    resetenv(attstack[newp + 2].lexval, envblock);
    break;

  /* defhead = "define" "<name>" */
  /* | "define" "<Label>" */
  case defhead1:
  case defhead2:
    attstack[newp] = attstack[newp + 1];
    macp = findmacro(macros, chbuf, WITH->chbufx, WITH->length, &lastp);
    if (macp == NULL) {   /*D,6D*/
      newarg(&macp);
      if (lastp == NULL)
	macros = macp;
      else
	lastp->nextb = macp;
    }
    disposebufs(&macp->argbody);
    newbuf(&macp->argbody);   /* copy the macro name */
    WITH5 = macp->argbody;
    FORLIM = WITH->length;
    for (i = 1; i <= FORLIM; i++)
      WITH5->carray[i] = chbuf[WITH->chbufx + i - 1];
    WITH5->savedlen = WITH->length;
    WITH5->readx = WITH->length + 1;
    readfor(macp->argbody, -WITH->length, &macp->argbody);
	/* append the body */
    lastm = macp->argbody;
    while (lastm->nextb != NULL)
      lastm = lastm->nextb;
    lastm->carray[lastm->savedlen] = etxch;
    /*D; if debuglevel > 1 then begin
       writeln(log); write(log,'def','head1');
       lastm := macp^.argbody; while lastm<> nil do begin
          wrbuf(lastm,3,0); lastm := lastm^.nextb end
       end D*/
    break;

  /* sprintf = "sprintf" "(" stringexpr */
  /* | "sprintf" "(" stringexpr "," exprlist */
  case sprintf1:
  case sprintf2:
    newprim(&WITH->prim, XLstring, envblock);
    WITH2 = WITH->prim;
    eb = findenv(envblock);
    /*D if debuglevel <= 0 then begin end
        else if eb=nil then writeln(log,' ! sprintf2: eb=nil')
        else if eb^.env=nil then writeln(log,' ! sprintf2: env=nil'); D*/
    WITH2->UU.Ubox.boxheight = eb->UU.Ublock.env[XLtextht - XLenvvar - 1];
    WITH2->UU.Ubox.boxwidth = eb->UU.Ublock.env[XLtextwid - XLenvvar - 1];
    WITH2->UU.Ubox.boxradius = 0.0;
    newstr(&WITH2->textp);
    if (p == sprintf1)
      nexprs = 0;   /* no of expression args */
    else
      nexprs = attstack[newp + 4].state;
    if (tmpbuf == NULL)
      tmpbuf = Malloc(sizeof(chbufarray));
    i = 0;   /* expression count */
    j = 0;   /* end of current substring */
    lj = j;   /* start of current substring */
    kk = 0;   /* substring count */
    if (attstack[newp + 2].prim != NULL) {
      if (attstack[newp + 2].prim->textp != NULL) {
	WITH4 = attstack[newp + 2].prim->textp;
	while (j < WITH4->len) {   /* end; below */
	  if (WITH4->segmnt[WITH4->seginx + j] != '%') {
	    j++;
	    if (j == WITH4->len)
	      kk = putstring(kk, attstack[newp].prim->textp, WITH4->segmnt,
			     WITH4->seginx + lj, j - lj);
	    continue;
	  }
	  if (WITH4->segmnt[WITH4->seginx + j + 1] == '%') {
		/* %% prints % */
		  kk = putstring(kk, attstack[newp].prim->textp,
				 WITH4->segmnt, WITH4->seginx + lj,
				 j - lj + 1);
	    j += 2;
	    lj = j;
	    continue;
	  }
	  if (i >= nexprs) {  /* not enough exprs */
	    markerror(864);
	    j = WITH4->len;
	    continue;
	  }
	  if (j > lj) {
	    kk = putstring(kk, attstack[newp].prim->textp, WITH4->segmnt,
			   WITH4->seginx + lj, j - lj);
	    lj = j;
	  }
	  k = WITH4->len;
	  j++;
	  if (WITH4->segmnt[WITH4->seginx + j] == '-')
	    j++;
#ifdef NO_SNPRINTF
	  attstack[newp + 1].state = j;
#endif
	  while (j < k) {
	    if (WITH4->segmnt[WITH4->seginx + j] == 'g' ||
		WITH4->segmnt[WITH4->seginx + j] == 'f' ||
		WITH4->segmnt[WITH4->seginx + j] == 'e')
	      k = j;
	    else if (WITH4->segmnt[WITH4->seginx + j] == '.' ||
		     isdigit(WITH4->segmnt[WITH4->seginx + j]))
	      j++;
	    else
	      j = k;
	  }

	  if (k == WITH4->len) {
	    markerror(865);
	    continue;
	  }
	  j++;
	  /*D if debuglevel > 0 then begin write(log,'format="');
	    for k := lj to j-1 do write(log,segmnt^[seginx+k]);
	    write(log, '" nexprs=',nexprs:2,' Numerical print value=');
	    wfloat(log,attstack^[newp+4+2*i].xval); writeln(log);
	    flush(log) end; D*/
	  /* Write the expression to tmpbuf */
#ifdef NO_SNPRINTF
	  k = attstack[newp + 1].state;
	  ll = j - 1;
	  s = 0.0;
	  while (k < ll) {
	    if (WITH4->segmnt[WITH4->seginx + k] == '.')
	      ll = k;
	    else {
	      s = 10.0 * s + WITH4->segmnt[WITH4->seginx + k] - '0';
	      k++;
	    }
	  }
	  r = 0.0;
	  for (ll = k + 1; ll < j; ll++)
	    r = 10.0 * r + WITH4->segmnt[WITH4->seginx + ll] - '0';
	  ll = 1;
	  if (s >= CHBUFSIZ || r >= CHBUFSIZ) {
	    markerror(865);
	    j = WITH4->len;
	  } else {
	    lastc = WITH4->segmnt[WITH4->seginx + j];
	    WITH4->segmnt[WITH4->seginx + j] = '\0';
	    sprintf(&tmpbuf[1], &WITH4->segmnt[WITH4->seginx + lj],
		     attstack[newp + i * 2 + 4].xval);
	    WITH4->segmnt[WITH4->seginx + j] = lastc;
	    k = CHBUFSIZ;
	    while (ll < k) {
	      if (tmpbuf[ll] == '\0')
		k = ll;
	      else
		ll++;
	    }
	    ll--;
	  }
#else
	  lastc = WITH4->segmnt[WITH4->seginx + j];
	  WITH4->segmnt[WITH4->seginx + j] = '\0';
	  k = snprintf(&tmpbuf[1], CHBUFSIZ,
		       &WITH4->segmnt[WITH4->seginx + lj],
		       attstack[newp + i * 2 + 4].xval);
	  WITH4->segmnt[WITH4->seginx + j] = lastc;
	  if (k > CHBUFSIZ - 2) {
	    markerror(865);
	    ll = CHBUFSIZ - 2;
	  } else if (k > 0)
	    ll = k;
	  else
	    ll = 0;
#endif
	  /*D if debuglevel > 0 then begin
	     write(log,' tmpbuf(1:',ll:1,')=');
	     for lj := 1 to ll do write(log,tmpbuf^[lj]);
	     writeln(log); flush(log) end; D*/
	  /* Copy tmpbuf to the string */
	  if (ll > 0 && ll <= CHBUFSIZ - 2)
	    kk = putstring(kk, attstack[newp].prim->textp, tmpbuf, 1, ll);
	  i++;
	  lj = j;
	}
	/*D begin
	   if debuglevel > 0 then begin writeln(log,
	      ' sprintf looping, nexprs,i,j,lj,len=',nexprs:4,i:4,j:4,lj:4,len:4,
	      ' c="', segmnt^[seginx+j],'"'); flush(log) end; D*/
      }
      /*D if debuglevel > 0 then writeln(log,' sprintf2: textp=nil'); D*/
    }
    /*D if debuglevel > 0 then writeln(log,' sprintf2: prim[2] = nil'); D*/
    /*D end; D*/
    /* begin above */
    if (nexprs > i)   /* Too many exprs */
      markerror(864);
    if (drawmode == xfig) {
      WITH2 = WITH->prim;
      if (WITH2->UU.Ubox.boxwidth == 0.0) {
	if (WITH2->UU.Ubox.boxheight == 0.0)
	  WITH2->UU.Ubox.boxheight = 0.1 * eb->UU.Ublock.env[XLscale - XLenvvar - 1];
	WITH2->UU.Ubox.boxwidth = WITH2->UU.Ubox.boxheight *
				  WITH2->textp->len * 0.75;
      }
    }
    if (envblock->son == attstack[newp + 2].prim)
      envblock->son = WITH->prim;
    deletestringbox(&attstack[newp + 2].prim);
    break;

  /* exprlist = expression */
  case exprlist1:
    WITH->state = 1;
    break;

  /* | expression "," exprlist */
  case exprlist2:
    WITH->state = attstack[newp + 2].state + 1;
    break;

  /* object = block */
  /* | object "height" expression */
  /* | object "width" expression */
  /* | object "radius" expression */
  /* | object "diameter" expression */
  /* | object "thickness" expression */
  /* | object "scaled" expression */
  /* | object "<directon>" optexp */
  /* | object "<linetype>" optexp */
  /* | object "chop" optexp */
  /* | object "filled" optexp */
  /* | object "<arrowhd>" optexp */
  /* | object "then" */
  /* | object "cw" */
  /* | object "ccw" */
  /* | object "same" */
  /* | object stringexpr */
  /* | object "by" position */
  /* | object "from" position */
  /* | object "to" position */
  /* | object "at" position */
  /* | object "<textpos>" */
  /* | object "<colrspec>" stringexpr */
  /* | objectwith "at" position */
  /* | objectwith "<corner>" "at" position */
  /* | objectwith pair "at" position */
  /* | "continue" */

  case object1:
    break;

  /* | object "height" expression */
  case object2:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      switch (WITH2->ptype) {

      case XLbox:
      case XBLOCK:
	if (WITH2->ptype == XBLOCK) {
	  r = 0.5 * (attstack[newp + 2].xval - WITH2->UU.Ublock.blockheight);
	  WITH2->UU.Ublock.blockheight = attstack[newp + 2].xval;
	} else {
	  r = 0.5 * (attstack[newp + 2].xval - WITH2->UU.Ubox.boxheight);
	  WITH2->UU.Ubox.boxheight = attstack[newp + 2].xval;
	}
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLleft:
	  case XLright:
	    /* blank case */
	    break;

	  case XLup:
	    shift(WITH->prim, 0.0, r);
	    break;

	  case XLdown:
	    shift(WITH->prim, 0.0, -r);
	    break;
	  }
	}
	break;

      case XLstring:
	WITH2->UU.Ubox.boxheight = attstack[newp + 2].xval;
	break;

      case XLcircle:
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLleft:
	  case XLright:
	    /* blank case */
	    break;

	  case XLup:
	    WITH2->aat.ypos += 0.5 * attstack[newp + 2].xval - WITH2->UU.Ucircle.radius;
	    break;

	  case XLdown:
	    WITH2->aat.ypos += WITH2->UU.Ucircle.radius - 0.5 * attstack[newp + 2].xval;
	    break;
	  }
	}
	WITH2->UU.Ucircle.radius = attstack[newp + 2].xval * 0.5;
	break;

      case XLellipse:
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLleft:
	  case XLright:
	    /* blank case */
	    break;

	  case XLup:
	    WITH2->aat.ypos +=
	      0.5 * (attstack[newp + 2].xval - WITH2->UU.Uellipse.elheight);
	    break;

	  case XLdown:
	    WITH2->aat.ypos +=
	      0.5 * (WITH2->UU.Uellipse.elheight - attstack[newp + 2].xval);
	    break;
	  }
	}
	WITH2->UU.Uellipse.elheight = attstack[newp + 2].xval;
	break;

      case XLline:
      case XLarrow:
      case XLmove:
      case XLarc:
      case XLspline:
	prp = WITH->prim;
	while (prp != NULL) {
	  prp->UU.Uline.height = attstack[newp + 2].xval;
	  if (isthen(prp))
	    prp = prp->parent;
	  else
	    prp = NULL;
	}
	break;

      case XLabel:
      case XLaTeX:
	markerror(858);
	break;
      }
    }
    break;

  /* | object "width" expression */
  case object3:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      switch (WITH2->ptype) {

      case XLbox:
      case XBLOCK:
	if (WITH2->ptype == XBLOCK) {
	  r = 0.5 * (attstack[newp + 2].xval - WITH2->UU.Ublock.blockwidth);
	  WITH2->UU.Ublock.blockwidth = attstack[newp + 2].xval;
	} else {
	  r = 0.5 * (attstack[newp + 2].xval - WITH2->UU.Ubox.boxwidth);
	  WITH2->UU.Ubox.boxwidth = attstack[newp + 2].xval;
	}
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLup:
	  case XLdown:
	    /* blank case */
	    break;

	  case XLright:
	    shift(WITH->prim, r, 0.0);
	    break;

	  case XLleft:
	    shift(WITH->prim, -r, 0.0);
	    break;
	  }
	}
	break;

      case XLstring:
	WITH2->UU.Ubox.boxwidth = attstack[newp + 2].xval;
	break;

      case XLcircle:
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLup:
	  case XLdown:
	    /* blank case */
	    break;

	  case XLright:
	    WITH2->aat.xpos += 0.5 * attstack[newp + 2].xval - WITH2->UU.Ucircle.radius;
	    break;

	  case XLleft:
	    WITH2->aat.xpos += WITH2->UU.Ucircle.radius - 0.5 * attstack[newp + 2].xval;
	    break;
	  }
	}
	WITH2->UU.Ucircle.radius = attstack[newp + 2].xval * 0.5;
	break;

      case XLellipse:
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLup:
	  case XLdown:
	    /* blank case */
	    break;

	  case XLright:
	    WITH2->aat.xpos +=
	      0.5 * (attstack[newp + 2].xval - WITH2->UU.Uellipse.elwidth);
	    break;

	  case XLleft:
	    WITH2->aat.xpos +=
	      0.5 * (WITH2->UU.Uellipse.elwidth - attstack[newp + 2].xval);
	    break;
	  }
	}
	WITH2->UU.Uellipse.elwidth = attstack[newp + 2].xval;
	break;

      case XLline:
      case XLarrow:
      case XLmove:
      case XLarc:
      case XLspline:
	prp = WITH->prim;
	while (prp != NULL) {
	  prp->UU.Uline.width = attstack[newp + 2].xval;
	  if (isthen(prp))
	    prp = prp->parent;
	  else
	    prp = NULL;
	}
	break;

      case XLabel:
      case XLaTeX:
	markerror(858);
	break;
      }
    }
    break;

  /* | object "radius" expression */
  case object4:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      switch (WITH2->ptype) {

      case XLbox:
	WITH2->UU.Ubox.boxradius = attstack[newp + 2].xval;
	break;

      case XLarc:
	t = attstack[newp + 2].xval;
	if (WITH2->direction == 0) {
	  attstack[newp + 2].xval = WITH2->aat.xpos + WITH2->UU.Uline.aradius *
		cos(WITH2->UU.Uline.endpos.xpos + WITH2->UU.Uline.endpos.ypos);
	  attstack[newp + 2].yval = WITH2->aat.ypos + WITH2->UU.Uline.aradius *
		sin(WITH2->UU.Uline.endpos.xpos + WITH2->UU.Uline.endpos.ypos);
	  /*D if debuglevel = 2 then begin write(log, 'to ');
	    with attstack^[newp+2] do wpair(log,xval,yval) end; D*/
	  doprod(object20);
	}
	r = cos(WITH2->UU.Uline.endpos.xpos);
	s = sin(WITH2->UU.Uline.endpos.xpos);
	WITH2->aat.xpos += WITH2->UU.Uline.aradius * r;
	WITH2->aat.ypos += WITH2->UU.Uline.aradius * s;
	WITH2->UU.Uline.aradius = t;
	WITH2->aat.xpos -= WITH2->UU.Uline.aradius * r;
	WITH2->aat.ypos -= WITH2->UU.Uline.aradius * s;
	setstflag(&WITH->state, XLradius);
	break;

      case XLcircle:
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLleft:
	    WITH2->aat.xpos += WITH2->UU.Ucircle.radius - attstack[newp + 2].xval;
	    break;

	  case XLright:
	    WITH2->aat.xpos += attstack[newp + 2].xval - WITH2->UU.Ucircle.radius;
	    break;

	  case XLup:
	    WITH2->aat.ypos += attstack[newp + 2].xval - WITH2->UU.Ucircle.radius;
	    break;

	  case XLdown:
	    WITH2->aat.ypos += WITH2->UU.Ucircle.radius - attstack[newp + 2].xval;
	    break;
	  }
	}
	WITH2->UU.Ucircle.radius = attstack[newp + 2].xval;
	setstflag(&WITH->state, XLradius);
	break;

      default:
	markerror(858);
	break;
      }
    }
    break;

  /* | object "diameter" expression */
  case object5:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (WITH2->ptype == XLcircle) {
	r = 0.5 * attstack[newp + 2].xval;
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLleft:
	    WITH2->aat.xpos += WITH2->UU.Ucircle.radius - r;
	    break;

	  case XLright:
	    WITH2->aat.xpos += r - WITH2->UU.Ucircle.radius;
	    break;

	  case XLup:
	    WITH2->aat.ypos += r - WITH2->UU.Ucircle.radius;
	    break;

	  case XLdown:
	    WITH2->aat.ypos += WITH2->UU.Ucircle.radius - r;
	    break;
	  }
	}
	WITH2->UU.Ucircle.radius = r;
      } else
	markerror(858);
    }
    break;

  /* | object "thickness" expression */
  case object6:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (attstack[newp + 2].xval < 0.0) {
	eb = findenv(envblock);
	WITH2->lthick = eb->UU.Ublock.env[XLlinethick - XLenvvar - 1];
      } else
	WITH2->lthick = attstack[newp + 2].xval;
    }
    break;

  /* | object "scaled" expression */
  case object7:
    if (WITH->prim != NULL && attstack[newp + 2].lexval != XEMPTY) {
      WITH2 = WITH->prim;
      r = attstack[newp + 2].xval - 1;
      corner(WITH->prim, XDc, &x1, &y1);
      switch (WITH2->ptype) {

      case XLbox:
      case XBLOCK:
      case XLstring:
	if (teststflag(WITH->state, XLat)) {
	  dx = 0.0;
	  dy = 0.0;
	} else if (WITH2->ptype == XBLOCK) {
	  dx = WITH2->UU.Ublock.blockwidth * r / 2;
	  dy = WITH2->UU.Ublock.blockheight * r / 2;
	} else {
	  dx = WITH2->UU.Ubox.boxwidth * r / 2;
	  dy = WITH2->UU.Ubox.boxheight * r / 2;
	}
	scaleobj(WITH->prim, attstack[newp + 2].xval);
	switch (WITH2->direction) {

	case XLright:
	  shift(WITH->prim, x1 - WITH2->aat.xpos + dx, y1 - WITH2->aat.ypos);
	  break;

	case XLleft:
	  shift(WITH->prim, x1 - WITH2->aat.xpos - dx, y1 - WITH2->aat.ypos);
	  break;

	case XLup:
	  shift(WITH->prim, x1 - WITH2->aat.xpos, y1 - WITH2->aat.ypos + dy);
	  break;

	case XLdown:
	  shift(WITH->prim, x1 - WITH2->aat.xpos, y1 - WITH2->aat.ypos - dy);
	  break;
	}
	break;

      case XLcircle:
	WITH2->UU.Ucircle.radius = attstack[newp + 2].xval * WITH2->UU.Ucircle.radius;
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLup:
	    WITH2->aat.xpos = x1;
	    WITH2->aat.ypos = y1 + WITH2->UU.Ucircle.radius;
	    break;

	  case XLdown:
	    WITH2->aat.xpos = x1;
	    WITH2->aat.ypos = y1 - WITH2->UU.Ucircle.radius;
	    break;

	  case XLright:
	    WITH2->aat.xpos = x1 + WITH2->UU.Ucircle.radius;
	    WITH2->aat.ypos = y1;
	    break;

	  case XLleft:
	    WITH2->aat.xpos = x1 - WITH2->UU.Ucircle.radius;
	    WITH2->aat.ypos = y1;
	    break;
	  }
	}
	break;

      case XLellipse:
	WITH2->UU.Uellipse.elwidth *= attstack[newp + 2].xval;
	WITH2->UU.Uellipse.elheight *= attstack[newp + 2].xval;
	if (!teststflag(WITH->state, XLat)) {
	  switch (WITH2->direction) {

	  case XLup:
	    WITH2->aat.xpos = x1;
	    WITH2->aat.ypos = y1 + WITH2->UU.Uellipse.elheight / 2;
	    break;

	  case XLdown:
	    WITH2->aat.xpos = x1;
	    WITH2->aat.ypos = y1 - WITH2->UU.Uellipse.elheight / 2;
	    break;

	  case XLright:
	    WITH2->aat.xpos = x1 + WITH2->UU.Uellipse.elwidth / 2;
	    WITH2->aat.ypos = y1;
	    break;

	  case XLleft:
	    WITH2->aat.xpos = x1 - WITH2->UU.Uellipse.elwidth / 2;
	    WITH2->aat.ypos = y1;
	    break;
	  }
	}
	break;

      case XLline:
      case XLarrow:
      case XLmove:
      case XLarc:
      case XLspline:
	scaleobj(WITH->prim, attstack[newp + 2].xval);
	corner(WITH->prim, XDc, &r, &s);
	shift(WITH->prim, x1 - r, y1 - s);
	break;

      case XLabel:
      case XLaTeX:
	markerror(858);
	break;
      }
    }
    break;

  /* | object "<directon>" optexp */
  case object8:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (WITH2->ptype == XLspline || WITH2->ptype == XLarrow ||
	  WITH2->ptype == XLmove || WITH2->ptype == XLline ||
	  WITH2->ptype == XLarc) {
	i = attstack[newp + 1].lexval;
	envblock->direction = i;
	eb = findenv(envblock);
	switch (WITH2->ptype) {

	case XLarc:
	  r = cos(WITH2->UU.Uline.endpos.xpos);
	  s = sin(WITH2->UU.Uline.endpos.xpos);
	  WITH2->aat.xpos += WITH2->UU.Uline.aradius * r;
	  WITH2->aat.ypos += WITH2->UU.Uline.aradius * s;
	  if (WITH2->direction == XLup && i == XLleft ||
	      WITH2->direction == XLdown && i == XLright ||
	      WITH2->direction == XLright && i == XLup ||
	      WITH2->direction == XLleft && i == XLdown)
	    WITH2->UU.Uline.endpos.ypos = pi * 0.5;
	  else if (WITH2->direction == XLup && i == XLright ||
		   WITH2->direction == XLdown && i == XLleft ||
		   WITH2->direction == XLright && i == XLdown ||
		   WITH2->direction == XLleft && i == XLup)
	    WITH2->UU.Uline.endpos.ypos = -pi * 0.5;
	  if (attstack[newp + 2].lexval != XEMPTY)
	    WITH2->UU.Uline.aradius = attstack[newp + 2].lexval;
	  WITH2->aat.xpos -= WITH2->UU.Uline.aradius * r;
	  WITH2->aat.ypos -= WITH2->UU.Uline.aradius * s;
	  WITH2->direction = i;
	  break;

	case XLline:
	case XLmove:
	case XLarrow:
	case XLspline:
	  WITH2->direction = i;
	  if (attstack[newp + 2].lexval != XEMPTY) {
	    r = attstack[newp + 2].xval;
	    s = r;
	  } else {
	    switch (WITH2->ptype) {

	    case XLline:
	    case XLarrow:
	    case XLspline:
	      r = eb->UU.Ublock.env[XLlineht - XLenvvar - 1];
	      s = eb->UU.Ublock.env[XLlinewid - XLenvvar - 1];
	      break;

	    case XLmove:
	      r = eb->UU.Ublock.env[XLmoveht - XLenvvar - 1];
	      s = eb->UU.Ublock.env[XLmovewid - XLenvvar - 1];
	      break;
	    }
	  }
	  lineardir(WITH->prim, r, s, &WITH->state);
	  break;
	}
      } else
	markerror(858);
    }
    break;

  /* | object "<linetype>" optexp */
  case object9:
    if (WITH->prim != NULL) {
      if (attstack[newp + 1].lexval == XLpath && drawmode != PSTricks) {
	/* (not (drawmode in [PSTricks,PS,PSfrag,PSmps])) */
	markerror(858);
      } else {
	setspec(&WITH->prim->spec, attstack[newp + 1].lexval);
	/* if attstack^[newp+1].lexval = XLinvis then setvis(spec,0); */
	if (attstack[newp + 2].lexval != XEMPTY) {
	  if ((attstack[newp + 1].lexval == XLpath ||
	       attstack[newp + 1].lexval == XLsolid ||
	       attstack[newp + 1].lexval == XLinvis) &&
	      (WITH->prim->ptype == XLmove || WITH->prim->ptype == XLspline ||
	       WITH->prim->ptype == XLarrow || WITH->prim->ptype == XLline))
	    lineardir(WITH->prim, attstack[newp + 2].xval,
		      attstack[newp + 2].xval, &WITH->state);
	  else if (attstack[newp + 1].lexval == XLpath ||
		   attstack[newp + 1].lexval == XLsolid ||
		   attstack[newp + 1].lexval == XLinvis)
	    markerror(858);
	  else
	    WITH->prim->lparam = attstack[newp + 2].xval;
	}
      }
    }
    break;

  /* | object "chop" optexp */
  case object10:
    if (WITH->prim != NULL) {
      if (WITH->prim->ptype != XLspline && WITH->prim->ptype != XLmove &&
	  WITH->prim->ptype != XLarrow && WITH->prim->ptype != XLline)
	markerror(858);
      else {
	WITH2 = WITH->prim;
	if (attstack[newp + 2].lexval != XEMPTY)
	  r = attstack[newp + 2].xval;
	else {
	  eb = findenv(envblock);
	  r = eb->UU.Ublock.env[XLcirclerad - XLenvvar - 1];
	}
	if (teststflag(WITH->state, XLchop))
	  WITH->endchop = r;
	else {
	  WITH->startchop = r;
	  WITH->endchop = r;
	  setstflag(&WITH->state, XLchop);
	}
      }
    }
    break;

  /* | object "fill" optexp */
  case object11:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (attstack[newp + 2].lexval != XEMPTY)
	s = attstack[newp + 2].xval;
      else {
	eb = findenv(envblock);
	s = eb->UU.Ublock.env[XLfillval - XLenvvar - 1];
      }
      prp = WITH->prim;
      while (prp != NULL) {
	switch (WITH2->ptype) {

	case XLbox:
	  prp->UU.Ubox.boxfill = s;
	  break;

	case XLcircle:
	  prp->UU.Ucircle.cfill = s;
	  break;

	case XLellipse:
	  prp->UU.Uellipse.efill = s;
	  break;

	default:
	  if (((1L << ((long)drawmode)) & ((1L << ((long)TeX)) |
		 (1L << ((long)tTeX)) | (1L << ((long)pict2e)))) != 0)
	    markerror(858);
	  else {
	    switch (WITH2->ptype) {

	    case XLline:
	    case XLarrow:
	    case XLmove:
	    case XLspline:
	    case XLarc:
	      prp->UU.Uline.lfill = s;
	      break;

	    default:
	      markerror(858);
	      break;
	    }
	  }
	  break;
	}
	if (isthen(prp))
	  prp = prp->parent;
	else
	  prp = NULL;
      }
    }
    break;

  /* | object "<arrowhd>" optexp */
  case object12:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (WITH2->ptype != XLspline && WITH2->ptype != XLarc &&
	  WITH2->ptype != XLarrow && WITH2->ptype != XLline)
	markerror(858);
      else {
	WITH2->UU.Uline.atype = pahlex(WITH2->UU.Uline.atype,
				      attstack[newp + 1].lexval);
	if (attstack[newp + 2].lexval != XEMPTY)
	  lj = (long)floor(attstack[newp + 2].xval + 0.5);
	else {
	  eb = findenv(envblock);
	  lj = (long)floor(eb->UU.Ublock.env[XLarrowhead - XLenvvar - 1] + 0.5);
	}
	WITH2->UU.Uline.atype = pahnum(WITH2->UU.Uline.atype, lj);
      }
    }
    break;

  /* | object "then" */
  case object13:
    if (WITH->prim != NULL) {
      /*D if debuglevel > 0 then begin write(log,'lexical "then" found, ');
          prtstval(state); writeln(log) end; D*/
      appendthen(&WITH->prim);
      if (WITH->prim->ptype != XLarc)
	setstflag(&WITH->state, XEMPTY);
    }
    break;

  /* | object "cw" */
  case object14:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (WITH2->ptype != XLarc)
	markerror(858);
      else {
	/*D if debuglevel = 2 then begin
	    write(log,'direction=',direction:1,' |arcangle','|=');
	    wfloat(log,endpos.ypos*180/pi); writeln(log);
	    write(log,'principal(|arcangle','|,pi)=');
	    wfloat(log,(principal(endpos.ypos,pi))*180/pi); writeln(log);
	    write(log,'2.0 * pi - principal(|arcangle','|,pi)=');
	    wfloat(log,(2.0 * pi - principal(endpos.ypos,pi))*180/pi);
	    writeln(log) end; D*/
	/* if teststflag(state,XLat) then begin
	   if endpos.ypos>0.0 then endpos.ypos := endpos.ypos-2*pi
	   end
	else */
	if (WITH2->UU.Uline.endpos.ypos > 0.0 && WITH2->direction == 0)
	  WITH2->UU.Uline.endpos.ypos = -fabs(
	      principal(2.0 * pi - WITH2->UU.Uline.endpos.ypos, 2.0 * pi));
	else if (WITH2->direction != 0) {
	  WITH2->aat = arcstart(WITH->prim);
	  switch (WITH2->direction) {

	  case XLup:
	    WITH2->aat.xpos += WITH2->UU.Uline.aradius;
	    break;

	  case XLdown:
	    WITH2->aat.xpos -= WITH2->UU.Uline.aradius;
	    break;

	  case XLleft:
	    WITH2->aat.ypos += WITH2->UU.Uline.aradius;
	    break;

	  case XLright:
	    WITH2->aat.ypos -= WITH2->UU.Uline.aradius;
	    break;
	  }
	  if (WITH2->UU.Uline.endpos.ypos > 0.0)
	    WITH2->UU.Uline.endpos.xpos = principal(
		WITH2->UU.Uline.endpos.xpos + pi, pi);
	  WITH2->UU.Uline.endpos.ypos = -fabs(WITH2->UU.Uline.endpos.ypos);
	}
	setstflag(&WITH->state, XLcw);
      }
    }
    break;

  /* | object "ccw" */
  case object15:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (WITH2->ptype != XLarc)
	markerror(858);
      else {
	/* if teststflag(state,XLat) then begin
	   if endpos.ypos<0.0 then endpos.ypos := 2*pi+endpos.ypos
	   end
	else */
	if (WITH2->UU.Uline.endpos.ypos < 0.0 && WITH2->direction == 0)
	  WITH2->UU.Uline.endpos.ypos = fabs(
	      principal(WITH2->UU.Uline.endpos.ypos - 2.0 * pi, 2.0 * pi));
	else if (WITH2->direction != 0) {
	  WITH2->aat = arcstart(WITH->prim);
	  switch (WITH2->direction) {

	  case XLup:
	    WITH2->aat.xpos -= WITH2->UU.Uline.aradius;
	    break;

	  case XLdown:
	    WITH2->aat.xpos += WITH2->UU.Uline.aradius;
	    break;

	  case XLleft:
	    WITH2->aat.ypos -= WITH2->UU.Uline.aradius;
	    break;

	  case XLright:
	    WITH2->aat.ypos += WITH2->UU.Uline.aradius;
	    break;
	  }
	  if (WITH2->UU.Uline.endpos.ypos < 0.0)
	    WITH2->UU.Uline.endpos.xpos = principal(
		WITH2->UU.Uline.endpos.xpos + pi, pi);
	  WITH2->UU.Uline.endpos.ypos = fabs(WITH2->UU.Uline.endpos.ypos);
	}
	setstflag(&WITH->state, XLccw);
      }
    }
    break;

  /* | object "same" */
  case object16:
    if (WITH->prim != NULL) {
      prp = nthprimobj(envblock->son, 0, WITH->prim->ptype);
      if (prp == NULL)
	markerror(857);
      else {
	WITH2 = WITH->prim;
	WITH2->lparam = prp->lparam;
	WITH2->lthick = prp->lthick;
	WITH2->direction = prp->direction;
	WITH2->spec = prp->spec;
	if (hasoutline(WITH->lexval, false))
	  copystr(&freeseg, &WITH2->outlinep, prp->outlinep);
	if (hasshade(WITH->lexval, false))
	  copystr(&freeseg, &WITH2->shadedp, prp->shadedp);
      }
      if (prp != NULL) {
	WITH2 = WITH->prim;
	switch (WITH2->ptype) {

	case XLbox:
	case XLstring:
	  if (WITH2->ptype == XLbox) {
	    switch (WITH2->direction) {

	    case XLup:
	      WITH2->aat.ypos +=
		0.5 * (prp->UU.Ubox.boxheight - WITH2->UU.Ubox.boxheight);
	      break;

	    case XLdown:
	      WITH2->aat.ypos -=
		0.5 * (prp->UU.Ubox.boxheight - WITH2->UU.Ubox.boxheight);
	      break;

	    case XLleft:
	      WITH2->aat.xpos -=
		0.5 * (prp->UU.Ubox.boxwidth - WITH2->UU.Ubox.boxwidth);
	      break;

	    case XLright:
	      WITH2->aat.xpos +=
		0.5 * (prp->UU.Ubox.boxwidth - WITH2->UU.Ubox.boxwidth);
	      break;
	    }
	  }
	  WITH2->UU.Ubox.boxfill = prp->UU.Ubox.boxfill;
	  WITH2->UU.Ubox.boxheight = prp->UU.Ubox.boxheight;
	  WITH2->UU.Ubox.boxwidth = prp->UU.Ubox.boxwidth;
	  WITH2->UU.Ubox.boxradius = prp->UU.Ubox.boxradius;
	  break;

	case XBLOCK:
	  markerror(858);
	  break;

	case XLcircle:
	  switch (WITH2->direction) {

	  case XLup:
	    WITH2->aat.ypos += prp->UU.Ucircle.radius - WITH2->UU.Ucircle.radius;
	    break;

	  case XLdown:
	    WITH2->aat.ypos += WITH2->UU.Ucircle.radius - prp->UU.Ucircle.radius;
	    break;

	  case XLleft:
	    WITH2->aat.xpos += WITH2->UU.Ucircle.radius - prp->UU.Ucircle.radius;
	    break;

	  case XLright:
	    WITH2->aat.xpos += prp->UU.Ucircle.radius - WITH2->UU.Ucircle.radius;
	    break;
	  }
	  WITH2->UU.Ucircle.cfill = prp->UU.Ucircle.cfill;
	  WITH2->UU.Ucircle.radius = prp->UU.Ucircle.radius;
	  break;

	case XLellipse:
	  switch (WITH2->direction) {

	  case XLup:
	    WITH2->aat.ypos +=
	      0.5 * (prp->UU.Uellipse.elheight - WITH2->UU.Uellipse.elheight);
	    break;

	  case XLdown:
	    WITH2->aat.ypos -=
	      0.5 * (prp->UU.Uellipse.elheight - WITH2->UU.Uellipse.elheight);
	    break;

	  case XLleft:
	    WITH2->aat.xpos -= 0.5 * (prp->UU.Uellipse.elwidth - WITH2->UU.Uellipse.elwidth);
	    break;

	  case XLright:
	    WITH2->aat.xpos += 0.5 * (prp->UU.Uellipse.elwidth - WITH2->UU.Uellipse.elwidth);
	    break;
	  }
	  WITH2->UU.Uellipse.efill = prp->UU.Uellipse.efill;
	  WITH2->UU.Uellipse.elheight = prp->UU.Uellipse.elheight;
	  WITH2->UU.Uellipse.elwidth = prp->UU.Uellipse.elwidth;
	  break;

	case XLarc:
	  x1 = WITH2->aat.xpos +
	       WITH2->UU.Uline.aradius * cos(WITH2->UU.Uline.endpos.xpos);
	  y1 = WITH2->aat.ypos +
	       WITH2->UU.Uline.aradius * sin(WITH2->UU.Uline.endpos.xpos);
	  WITH2->UU.Uline.aradius = prp->UU.Uline.aradius;
	  WITH2->UU.Uline.endpos.xpos = prp->UU.Uline.endpos.xpos;
	  WITH2->aat.xpos =
	    x1 - WITH2->UU.Uline.aradius * cos(WITH2->UU.Uline.endpos.xpos);
	  WITH2->aat.ypos =
	    y1 - WITH2->UU.Uline.aradius * sin(WITH2->UU.Uline.endpos.xpos);
	  WITH2->UU.Uline.endpos.ypos = prp->UU.Uline.endpos.ypos;
	  break;

	case XLline:
	case XLarrow:
	case XLmove:
	case XLspline:
	  WITH2->UU.Uline.endpos.xpos =
	    WITH2->aat.xpos + prp->UU.Uline.endpos.xpos - prp->aat.xpos;
	  WITH2->UU.Uline.endpos.ypos =
	    WITH2->aat.ypos + prp->UU.Uline.endpos.ypos - prp->aat.ypos;
	  WITH2->UU.Uline.height = prp->UU.Uline.height;
	  WITH2->UU.Uline.width = prp->UU.Uline.width;
	  WITH2->UU.Uline.atype = prp->UU.Uline.atype;
	  break;

	case XLabel:
	case XLaTeX:
	  markerror(858);
	  break;
	}
      }
    }
    break;

  /* | object stringexpr */
  case object17:
    if (attstack[newp + 1].prim != NULL) {
      if (WITH->prim != NULL) {
	WITH2 = WITH->prim;
	if (WITH2->textp == NULL)
	  WITH2->textp = attstack[newp + 1].prim->textp;
	else {
	  namptr = WITH2->textp;
	  i = 1;
	  while (namptr->next_ != NULL) {
	    namptr = namptr->next_;
	    i++;
	  }
	  namptr->next_ = attstack[newp + 1].prim->textp;
	  if (WITH2->ptype == XLstring)
	    WITH2->UU.Ubox.boxheight = WITH2->UU.Ubox.boxheight * (i + 1) / i;
	}
	if (((1L << ((long)drawmode)) &
	     ((1L << ((long)PS)) | (1L << ((long)PSfrag)))) != 0)
	      /*,PSmps*/
		printstate = (printstate >> 1) * 2 + 1;
	/* output contains text */
      }
      attstack[newp + 1].prim->textp = NULL;
      deletestringbox(&attstack[newp + 1].prim);
    }
    break;

  /* | object "by" position */
  case object18:
    if (WITH->prim != NULL) {
      if (WITH->prim->ptype == XLmove || WITH->prim->ptype == XLspline ||
	  WITH->prim->ptype == XLarrow || WITH->prim->ptype == XLline ||
	  WITH->prim->ptype == XLarc) {
	WITH2 = WITH->prim;
	x1 = attstack[newp + 2].xval + WITH2->aat.xpos;
	y1 = attstack[newp + 2].yval + WITH2->aat.ypos;
	if (WITH2->ptype == XLarc) {
	  x1 += WITH2->UU.Uline.aradius * cos(WITH2->UU.Uline.endpos.xpos);
	  y1 += WITH2->UU.Uline.aradius * sin(WITH2->UU.Uline.endpos.xpos);
	}
	attstack[newp + 2].xval = x1;
	attstack[newp + 2].yval = y1;
	doprod(object20);
      } else
	markerror(858);
    }
    break;

  /* | object "from" position */
  case object19:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      if (WITH2->ptype == XLmove || WITH2->ptype == XLspline ||
	  WITH2->ptype == XLarrow || WITH2->ptype == XLline ||
	  WITH2->ptype == XLarc) {
	if (WITH2->ptype == XLarc) {
	  r = attstack[newp + 2].xval;
	  s = attstack[newp + 2].yval;
	  if (teststflag(WITH->state, XLto)) {
	    attstack[newp + 2].xval = WITH2->aat.xpos +
		WITH2->UU.Uline.aradius *
		cos(WITH2->UU.Uline.endpos.xpos + WITH2->UU.Uline.endpos.ypos);
	    attstack[newp + 2].yval = WITH2->aat.ypos +
		WITH2->UU.Uline.aradius *
		sin(WITH2->UU.Uline.endpos.xpos + WITH2->UU.Uline.endpos.ypos);
	    doprod(object20);
	  }
	  WITH2->aat.xpos =
	    r - WITH2->UU.Uline.aradius * cos(WITH2->UU.Uline.endpos.xpos);
	  WITH2->aat.ypos =
	    s - WITH2->UU.Uline.aradius * sin(WITH2->UU.Uline.endpos.xpos);
	} else if (!teststflag(WITH->state, XLto)) {
	  prp = WITH->prim;
	  while (isthen(prp))
	    prp = prp->parent;
	  shift(prp, attstack[newp + 2].xval - prp->aat.xpos,
		attstack[newp + 2].yval - prp->aat.ypos);
	} else {
	  WITH2->aat.xpos = attstack[newp + 2].xval;
	  WITH2->aat.ypos = attstack[newp + 2].yval;
	}
	setstflag(&WITH->state, XLfrom);
      } else
	markerror(858);
    }
    break;

  /* | object "to" position */
  case object20:
    if (WITH->prim != NULL) {
      if (WITH->prim->ptype == XLmove || WITH->prim->ptype == XLspline ||
	  WITH->prim->ptype == XLarrow || WITH->prim->ptype == XLline ||
	  WITH->prim->ptype == XLarc) {
	if ((WITH->prim->ptype != XLarc) & teststflag(WITH->state, XLto)) {
	  /* (teststflag(state,XLto) or teststflag(state,XLdirecton)) */
	  /*D if debuglevel > 0 then begin writeln(log,'"then" inserted, ');
	      prtstval(state); writeln(log) end; D*/
	  appendthen(&WITH->prim);
	  setstflag(&WITH->state, XEMPTY);
	}
	WITH2 = WITH->prim;
	if (WITH2->ptype != XLarc) {
	  WITH2->UU.Uline.endpos.xpos = attstack[newp + 2].xval;
	  WITH2->UU.Uline.endpos.ypos = attstack[newp + 2].yval;
	} else {
	  x1 = WITH2->aat.xpos +
	       WITH2->UU.Uline.aradius * cos(WITH2->UU.Uline.endpos.xpos);
	      /* from position */
	  y1 = WITH2->aat.ypos +
	       WITH2->UU.Uline.aradius * sin(WITH2->UU.Uline.endpos.xpos);
	  dx = attstack[newp + 2].xval - x1;
	  dy = attstack[newp + 2].yval - y1;
	  ts = dx * dx + dy * dy;   /* chord^2 */
	  if (WITH2->direction != 0)
	    i = WITH2->direction;
	  else
	    i = WITH->length;
	  /*D if debuglevel = 2 then begin
	     write(log,' (x1,y1)='); wpair(log,x1,y1);
	     write(log,' (dx,dy)='); wpair(log,dx,dy);
	     write(log,' ts='); wfloat(log,ts);
	     write(log,' i=',i:1 ) end; D*/
	  if (ts == 0.0)
	    WITH2->UU.Uline.endpos.ypos = 0.0;
	  else {  /* ratio centre-to-chord/half-chord */
	    t = sqrt(Max(0.0, 4.0 * WITH2->UU.Uline.aradius * WITH2->
				UU.Uline.aradius - ts) / ts);
	    /*D if debuglevel = 2 then begin
	       write(log,' t='); wfloat(log,t);
	       write(log,' |arcangle','|='); wfloat(log,endpos.ypos*180/pi);
	       writeln(log) end; D*/
	    r = sqrt(ts);
	    if (t <= 0.0)   /* t is always nonnegative */
	      WITH2->UU.Uline.aradius = 0.5 * r;
	    switch (i) {
		/* Determine which of the two default arcs to draw: */

	    case XLup:
	      if (WITH2->UU.Uline.endpos.ypos * (-dx - t * dy) < 0.0)
		t = -t;
	      break;

	    case XLdown:
	      if (WITH2->UU.Uline.endpos.ypos * (-dx - t * dy) > 0.0)
		t = -t;
	      break;

	    case XLright:
	      if (WITH2->UU.Uline.endpos.ypos * (dy - t * dx) < 0.0)
		t = -t;
	      break;

	    case XLleft:
	      if (WITH2->UU.Uline.endpos.ypos * (dy - t * dx) > 0.0)
		t = -t;
	      break;
	    }
	    WITH2->aat.xpos = x1 + 0.5 * (dx + t * dy);
	    WITH2->aat.ypos = y1 + 0.5 * (dy - t * dx);
	    /*D if debuglevel = 2 then begin
	       write(log,' t='); wfloat(log,t);
	       write(log,' aradius='); wfloat(log,aradius);
	       write(log,' aat='); wpair(log,aat.xpos,aat.ypos);
	       writeln(log) end; D*/
	    setangles(&WITH2->UU.Uline.endpos.xpos,
		      &WITH2->UU.Uline.endpos.ypos, WITH2->aat, x1, y1,
		      attstack[newp + 2].xval, attstack[newp + 2].yval);
	  }
	  if (WITH2->direction != 0)
	    WITH->length = WITH2->direction;
	  WITH2->direction = 0;
	}
	/*D ; if debuglevel > 0 then printobject(prim) D*/
	setstflag(&WITH->state, XLto);
      } else
	markerror(858);
    }
    break;

  /* | object "at" position */
  case object21:
    if (WITH->prim != NULL) {
      WITH2 = WITH->prim;
      WITH->root = NULL;
      WITH->xval = attstack[newp + 2].xval;
      WITH->yval = attstack[newp + 2].yval;
      setstval(&WITH->state, XDc);
      setstflag(&WITH->state, XLat);
    }
    break;

  /* | object "<textpos>" */
  /* This might be altered (in the case of strings, not other objects with text),
       so that aat is changed as a function of textpos. Then
       the output routines have to take that into account. The alternative
       is to alter nesw for strings, as now. */
  case object22:
    if (WITH->prim != NULL) {
      namptr = WITH->prim->textp;
      if (namptr != NULL) {
	while (namptr->next_ != NULL)
	  namptr = namptr->next_;
	i = (long)floor(namptr->val + 0.5);
	if (i > 8)
	  i = 0;
	switch (attstack[newp + 1].lexval) {

	case XLcenter:
	  namptr->val = 15.0;
	  break;

	case XLrjust:
	  namptr->val = i / 4.0 * 4 + 1;
	  break;

	case XLljust:
	  namptr->val = i / 4.0 * 4 + 2;
	  break;

	case XLbelow:
	  namptr->val = (i & 3) + 4.0;
	  break;

	case XLabove:
	  namptr->val = (i & 3) + 8.0;
	  break;
	}
      } else
	markerror(861);
    }
    break;

  /* | object ("colour"|"outline"|"shade") stringexpr */
  case object23:
    if (((1L << ((long)drawmode)) &
	 ((1L << ((long)MPost)) | (1L << ((long)MFpic)) |
	  (1L << ((long)PGF)) | (1L << ((long)PS)) | (1L << ((long)PSfrag)) |
	  (1L << ((long)PSTricks)) | (1L << ((long)SVG)))) == 0)
	  /*,PSmps*/
	    markerror(858);
    else if (attstack[newp + 2].prim != NULL && WITH->prim != NULL) {
      WITH2 = WITH->prim;
      switch (attstack[newp + 1].lexval) {

      case XLshaded:
	if (hasshade(WITH->lexval, true)) {
	  deletename(&WITH2->shadedp);
	  WITH2->shadedp = attstack[newp + 2].prim->textp;
	  attstack[newp + 2].prim->textp = NULL;
	}
	break;

      case XLoutlined:
	if (hasoutline(WITH->lexval, true)) {
	  deletename(&WITH2->outlinep);
	  WITH2->outlinep = attstack[newp + 2].prim->textp;
	  attstack[newp + 2].prim->textp = NULL;
	}
	break;

      case XLcolour:
	if (hasoutline(WITH->lexval, true)) {
	  deletename(&WITH2->outlinep);
	  WITH2->outlinep = attstack[newp + 2].prim->textp;
	  attstack[newp + 2].prim->textp = NULL;
	  if ((attstack[newp + 1].lexval == XLcolour &&
	       WITH2->ptype != XLspline && WITH2->ptype != XLarrow &&
	       WITH2->ptype != XLline &&
	       WITH2->ptype != XLarc) & hasshade(WITH->lexval, false)) {
	    deletename(&WITH2->shadedp);
	    copystr(&freeseg, &WITH2->shadedp, WITH2->outlinep);
	  }
	}
	break;
      }
    }
    deletestringbox(&attstack[newp + 2].prim);
    break;

  /* | objectwith "at" position */
  case object24:
    if (WITH->prim == NULL)
      WITH->root = NULL;
    else {
      if (WITH->root != NULL) {
	prp = WITH->prim;
	WITH->prim = WITH->root;
	WITH->root = prp;
      }
      WITH->xval = attstack[newp + 2].xval;
      WITH->yval = attstack[newp + 2].yval;
      setstval(&WITH->state, XEMPTY);
      setstflag(&WITH->state, XLat);
    }
    break;

  /* | objectwith "<corner>" "at" position */
  /* | objectwith pair "at" position */
  case object25:
  case object26:
    if (WITH->prim != NULL) {
      if (WITH->root != NULL) {
	prp = WITH->prim;
	WITH->prim = WITH->root;
	WITH->root = prp;
      }
      WITH->xval = attstack[newp + 3].xval;
      WITH->yval = attstack[newp + 3].yval;
      if (p == object25)
	setstval(&WITH->state, attstack[newp + 1].lexval);
      else if (WITH->prim->ptype != XLarc && WITH->prim->ptype != XLellipse &&
	       WITH->prim->ptype != XLcircle && WITH->prim->ptype != XBLOCK &&
	       WITH->prim->ptype != XLstring && WITH->prim->ptype != XLbox)
	markerror(858);
      else {
	WITH->startchop = attstack[newp + 1].xval;
	WITH->endchop = attstack[newp + 1].yval;
	setstval(&WITH->state, XLfloat);
      }
      setstflag(&WITH->state, XLat);
    }
    break;

  /* | "continue" */
  case object27:
    primp = NULL;
    prp = envblock->son;
    while (prp != NULL) {
      if (prp->ptype != XLaTeX && prp->ptype != XLstring &&
	  prp->ptype != XBLOCK && prp->ptype != XLabel)
	primp = prp;
      prp = prp->next_;
    }
    if (primp == NULL)
      markerror(857);
    else {
      while (primp->son != NULL)
	primp = primp->son;
      WITH->prim = primp;
      doprod(object13);
    }
    break;

  /* openblock = "<EMPTY>" */
  case openblock1:
    /*D if debuglevel > 0 then writeln(log,
       'Calling newprim(prim,',XBLOCK:1,'):'); D*/
    newprim(&WITH->prim, XBLOCK, envblock);
    WITH6 = &WITH->prim->UU.Ublock.here;
    /* prim^.here := envblock^.here; */
    /* Bah! gpic compatibility: */
    WITH6->xpos = 0.0;
    WITH6->ypos = 0.0;
    envblock = WITH->prim;
    WITH->lexval = 0;
    /* ; resetenv(0,envblock) */
    /*D; if (debuglevel > 0) and (prim <> nil) then printobject(prim)D*/
    break;

  /* block = "<primitiv>" optexp */
  /* | stringexpr */
  /* | openblock "[" closeblock "]" */
  /* | openblock "[]" */
  /* set parameters for primitive object */
  case block1:
    if (WITH->lexval > XLprimitiv && WITH->lexval < XLenvvar) {
      newprim(&WITH->prim, WITH->lexval, envblock);
      eb = findenv(envblock);
      if ((((1L << ((long)drawmode)) & ((1L << ((long)MPost)) |
	      (1L << ((long)pict2e)) | (1L << ((long)PS)) |
	      (1L << ((long)SVG)) | (1L << ((long)PSfrag)))) != 0 &&
	   WITH->lexval != XLmove) || WITH->lexval == XLarc)
      {   /*,PSmps*/
	WITH->prim->lthick = eb->UU.Ublock.env[XLlinethick - XLenvvar - 1];
	/*D; if debuglevel > 0 then begin write(log,
	    ' envblock[',ordp(eb),'] setting linethick=');
	    wfloat(log,prim^.lthick); writeln(log) end D*/
      }
      /* (drawmode in [PGF,PSTricks]) and */
      if (attstack[newp + 1].lexval != XEMPTY && WITH->lexval != XLmove &&
	  WITH->lexval != XLspline &&
	  WITH->lexval != XLarrow && WITH->lexval != XLline)
	markerror(858);
      WITH2 = WITH->prim;
      switch (WITH->lexval) {

      case XLbox:
	WITH2->UU.Ubox.boxheight = eb->UU.Ublock.env[XLboxht - XLenvvar - 1];
	WITH2->UU.Ubox.boxwidth = eb->UU.Ublock.env[XLboxwid - XLenvvar - 1];
	WITH2->UU.Ubox.boxradius = eb->UU.Ublock.env[XLboxrad - XLenvvar - 1];
	switch (WITH2->direction) {

	case XLup:
	  WITH2->aat.ypos += WITH2->UU.Ubox.boxheight * 0.5;
	  break;

	case XLdown:
	  WITH2->aat.ypos -= WITH2->UU.Ubox.boxheight * 0.5;
	  break;

	case XLleft:
	  WITH2->aat.xpos -= WITH2->UU.Ubox.boxwidth * 0.5;
	  break;

	case XLright:
	  WITH2->aat.xpos += WITH2->UU.Ubox.boxwidth * 0.5;
	  break;
	}
	break;

      case XLcircle:
	WITH2->UU.Ucircle.radius = eb->UU.Ublock.env[XLcirclerad - XLenvvar - 1];
	switch (WITH2->direction) {

	case XLup:
	  WITH2->aat.ypos += WITH2->UU.Ucircle.radius;
	  break;

	case XLdown:
	  WITH2->aat.ypos -= WITH2->UU.Ucircle.radius;
	  break;

	case XLleft:
	  WITH2->aat.xpos -= WITH2->UU.Ucircle.radius;
	  break;

	case XLright:
	  WITH2->aat.xpos += WITH2->UU.Ucircle.radius;
	  break;
	}
	break;

      case XLellipse:
	WITH2->UU.Uellipse.elheight = eb->UU.Ublock.env[XLellipseht - XLenvvar - 1];
	WITH2->UU.Uellipse.elwidth = eb->UU.Ublock.env[XLellipsewid - XLenvvar - 1];
	switch (WITH2->direction) {

	case XLup:
	  WITH2->aat.ypos += WITH2->UU.Uellipse.elheight * 0.5;
	  break;

	case XLdown:
	  WITH2->aat.ypos -= WITH2->UU.Uellipse.elheight * 0.5;
	  break;

	case XLleft:
	  WITH2->aat.xpos -= WITH2->UU.Uellipse.elwidth * 0.5;
	  break;

	case XLright:
	  WITH2->aat.xpos += WITH2->UU.Uellipse.elwidth * 0.5;
	  break;
	}
	break;

      case XLarc:
	WITH2->UU.Uline.aradius = eb->UU.Ublock.env[XLarcrad - XLenvvar - 1];
	switch (WITH2->direction) {

	case XLup:
	  WITH2->UU.Uline.endpos.xpos = 0.0;
	  WITH2->aat.xpos -= WITH2->UU.Uline.aradius;
	  break;

	case XLdown:
	  WITH2->UU.Uline.endpos.xpos = pi;
	  WITH2->aat.xpos += WITH2->UU.Uline.aradius;
	  break;

	case XLleft:
	  WITH2->UU.Uline.endpos.xpos = 0.5 * pi;
	  WITH2->aat.ypos -= WITH2->UU.Uline.aradius;
	  break;

	case XLright:
	  WITH2->UU.Uline.endpos.xpos = -0.5 * pi;
	  WITH2->aat.ypos += WITH2->UU.Uline.aradius;
	  break;
	}
	WITH2->UU.Uline.height = eb->UU.Ublock.env[XLarrowht - XLenvvar - 1];
	WITH2->UU.Uline.width = eb->UU.Ublock.env[XLarrowwid - XLenvvar - 1];
	    /* atype := XEMPTY; */
	WITH2->UU.Uline.atype = pahnum(pahlex(0, XEMPTY),
	    (long)floor(eb->UU.Ublock.env[XLarrowhead - XLenvvar - 1] + 0.5));
	WITH2->UU.Uline.endpos.ypos = pi * 0.5;
	break;

      case XLline:
      case XLarrow:
      case XLspline:
      case XLmove:
	WITH2->UU.Uline.endpos = WITH2->aat;
	if (WITH2->ptype == XLspline && attstack[newp + 1].lexval != XEMPTY) {
	  /* This implements adjusted splines */
	  WITH2->UU.Uline.aradius = attstack[newp + 1].xval;
	  attstack[newp + 1].lexval = XEMPTY;
	}  /* */
	if (attstack[newp + 1].lexval != XEMPTY)
	  r = attstack[newp + 1].xval;
	else if (WITH2->ptype == XLmove) {
	  switch (WITH2->direction) {

	  case XLup:
	  case XLdown:
	    r = eb->UU.Ublock.env[XLmoveht - XLenvvar - 1];
	    break;

	  case XLleft:
	  case XLright:
	    r = eb->UU.Ublock.env[XLmovewid - XLenvvar - 1];
	    break;
	  }
	} else {
	  switch (WITH2->direction) {

	  case XLup:
	  case XLdown:
	    r = eb->UU.Ublock.env[XLlineht - XLenvvar - 1];
	    break;

	  case XLleft:
	  case XLright:
	    r = eb->UU.Ublock.env[XLlinewid - XLenvvar - 1];
	    break;
	  }
	}
	switch (WITH2->direction) {

	case XLup:
	  WITH2->UU.Uline.endpos.ypos = WITH2->aat.ypos + r;
	  break;

	case XLdown:
	  WITH2->UU.Uline.endpos.ypos = WITH2->aat.ypos - r;
	  break;

	case XLleft:
	  WITH2->UU.Uline.endpos.xpos = WITH2->aat.xpos - r;
	  break;

	case XLright:
	  WITH2->UU.Uline.endpos.xpos = WITH2->aat.xpos + r;
	  break;
	}
	WITH2->UU.Uline.height = eb->UU.Ublock.env[XLarrowht - XLenvvar - 1];
	WITH2->UU.Uline.width = eb->UU.Ublock.env[XLarrowwid - XLenvvar - 1];
	if (WITH2->ptype == XLarrow)
	  WITH2->UU.Uline.atype = pahlex(0, XRIGHTHEAD);
	else
	  WITH2->UU.Uline.atype = pahlex(0, XEMPTY);
	WITH2->UU.Uline.atype = pahnum(WITH2->UU.Uline.atype,
	    (long)floor(eb->UU.Ublock.env[XLarrowhead - XLenvvar - 1] + 0.5));
	break;
      }
    }
    break;

  /* | stringexpr */
  case block2:
    if (((1L << ((long)drawmode)) &
	 ((1L << ((long)PS)) | (1L << ((long)PSfrag)))) != 0)
	  /*,PSmps*/
	    printstate = (printstate >> 1) * 2 + 1;
    /* flag text in output */
    break;

  /* | openblock "[" closeblock "]" */
  case block3:
    if (WITH->prim != NULL) {
      WITH->prim->son = attstack[newp + 2].prim;
      envblock = WITH->prim->parent;
      getnesw(WITH->prim->son);
      WITH2 = WITH->prim;
      WITH2->UU.Ublock.blockwidth = east - west;
      WITH2->UU.Ublock.blockheight = north - south;
      WITH2->aat.xpos = (east + west) * 0.5;
      WITH2->aat.ypos = (north + south) * 0.5;
      dx = envblock->UU.Ublock.here.xpos - WITH2->aat.xpos;
      dy = envblock->UU.Ublock.here.ypos - WITH2->aat.ypos;
      switch (envblock->direction) {

      case XLright:
	dx += WITH2->UU.Ublock.blockwidth * 0.5;
	break;

      case XLleft:
	dx -= WITH2->UU.Ublock.blockwidth * 0.5;
	break;

      case XLup:
	dy += WITH2->UU.Ublock.blockheight * 0.5;
	break;

      case XLdown:
	dy -= WITH2->UU.Ublock.blockheight * 0.5;
	break;
      }
      WITH2->direction = envblock->direction;   /* gpic compatibility */
      /*D if (debuglevel > 0) and (prim^.son <> nil) then with prim^ do begin
             printobject(son);
             if son^.next <> nil then printobject(son^.next)
             end; D*/
      /*D;if debuglevel > 0 then with prim^ do begin
         write(log, 'Object3: (north,south),(west,east)');
         wpair(log,aat.ypos+blockheight*0.5,aat.ypos-blockheight*0.5);
         wpair(log,aat.xpos-blockwidth*0.5,aat.xpos+blockwidth*0.5);
         write(log, ' here=');
         wpair(log,here.xpos,here.ypos); writeln(log);
         if prim^.son<>nil then begin
            printobject(prim^.son);
            if prim^.son^.next <> nil then printobject(prim^.son^.next)
            end
         end;
      if debuglevel > 0 then begin snaptree(prim,0); writeln(log) end D*/
      shift(WITH->prim, dx, dy);
    }
    break;

  /* | openblock "[]" */
  case block4:
    if (WITH->prim != NULL)
      envblock = WITH->prim->parent;
    break;

  /* optexp = "<EMPTY>" */
  /* | expression */
  case optexp1:
    WITH->lexval = XEMPTY;
    break;

  case optexp2:
    break;

  /* closeblock = elementlist optnl */
  /* Add latex to reset env variables changed within a block */
  case closeblock1:
    if (WITH->prim != NULL && envblock->UU.Ublock.env != NULL) {
      eb = findenv(envblock->parent);
      if (eb != NULL) {  /* check and reset variables: */
	if (envblock->UU.Ublock.env[XLlinethick - XLenvvar - 1] !=
	    eb->UU.Ublock.env[XLlinethick - XLenvvar - 1]) {
	  newprim(&attstack[newp + 2].prim, XLaTeX, envblock);
	  attstack[newp + 2].prim->lthick =
	    eb->UU.Ublock.env[XLlinethick - XLenvvar - 1];
	  addelem(attstack[newp + WITH->state].prim, attstack[newp + 2].prim);
	}
	if (envblock->UU.Ublock.env[XLdashwid - XLenvvar - 1] !=
	    eb->UU.Ublock.env[XLdashwid - XLenvvar - 1]) {
	  newprim(&attstack[newp + 2].prim, XLaTeX, envblock);
	  attstack[newp + 2].prim->lparam = eb->UU.Ublock.env[XLdashwid - XLenvvar - 1];
	  addelem(attstack[newp + WITH->state].prim, attstack[newp + 2].prim);
	}
      }
    }
    break;

  /* pair = expression "," expression */
  /* | location shift */
  case pair1:
    WITH->yval = attstack[newp + 2].xval;
    break;

  case pair2:
    if (attstack[newp + 1].lexval != XEMPTY) {
      WITH->xval += attstack[newp + 1].xval;
      WITH->yval += attstack[newp + 1].yval;
    }
    break;

  /* objectwith = object "with" */
  /* | objectwith "." "<Label>" suffix */
  case objectwith1:
    break;

  case objectwith2:
    if (WITH->prim != NULL) {
      if (attstack[newp + 3].lexval != XEMPTY) {
	WITH1 = &attstack[newp + 2];
	addsuffix(chbuf, &WITH1->chbufx, &WITH1->length,
		  attstack[newp + 3].xval);
      }
      primp = findplace(WITH->prim->son, chbuf, attstack[newp + 2].chbufx,
			attstack[newp + 2].length);
      if (primp == NULL) {
	marknotfound(855, chbuf, attstack[newp + 2].chbufx,
		     attstack[newp + 2].length);
	deletetree(&WITH->prim);
      } else {
	if (WITH->root == NULL)
	  WITH->root = WITH->prim;
	WITH->prim = primp;
      }
    }
    break;

  /* | objectwith "." nth primobj */
  case objectwith3:
    if (WITH->prim != NULL) {
      WITH->lexval = attstack[newp + 1].lexval;
      primp = nthprimobj(WITH->prim->son, attstack[newp + 2].length,
			 attstack[newp + 3].lexval);
      if (primp == NULL) {
	markerror(857);
	deletetree(&WITH->prim);
      } else {
	if (WITH->root == NULL)
	  WITH->root = WITH->prim;
	WITH->prim = primp;
      }
    }
    break;

  /* shift = "<EMPTY>" */
  /* | shift "+" location */
  /* | shift "-" location */
  case shift1:
    WITH->xval = 0.0;
    WITH->yval = 0.0;
    WITH->lexval = XEMPTY;
    break;

  case shift2:
    WITH->xval += attstack[newp + 2].xval;
    WITH->yval += attstack[newp + 2].yval;
    WITH->lexval = XLfloat;
    break;

  case shift3:
    WITH->xval -= attstack[newp + 2].xval;
    WITH->yval -= attstack[newp + 2].yval;
    WITH->lexval = XLfloat;
    break;

  /* location = "(" position ")" */
  /* | "(" position "," position ")" */
  /* | place */
  /* | location "*" factor */
  /* | location "/" factor */
  case location1:
    attstack[newp] = attstack[newp + 1];
    break;

  case location2:
    WITH->xval = attstack[newp + 1].xval;
    WITH->yval = attstack[newp + 3].yval;
    break;

  case location3:
    /* blank case */
    break;

  case location4:
    WITH->xval *= attstack[newp + 2].xval;
    WITH->yval *= attstack[newp + 2].xval;
    break;

  case location5:
    if (attstack[newp + 2].xval == 0.0)
      markerror(852);
    else {
      WITH->xval /= attstack[newp + 2].xval;
      WITH->yval /= attstack[newp + 2].xval;
    }
    break;

  /* place = placename */
  /* | placename "<corner>" */
  /* | "<corner>" placename */
  /* | "<corner>" "of" placename */
  /* | "Here" */
  case place1:
    corner(WITH->prim, XEMPTY, &WITH->xval, &WITH->yval);
    break;

  case place2:
    corner(WITH->prim, attstack[newp + 1].lexval, &WITH->xval, &WITH->yval);
    break;

  case place3:
    corner(attstack[newp + 1].prim, WITH->lexval, &WITH->xval, &WITH->yval);
    break;

  case place4:
    corner(attstack[newp + 2].prim, WITH->lexval, &WITH->xval, &WITH->yval);
    break;

  case place5:
    WITH->xval = envblock->UU.Ublock.here.xpos;
    WITH->yval = envblock->UU.Ublock.here.ypos;
    break;

  /* factor = primary */
  /* | "!" primary */
  /* | primary "^" factor */
  case factor1:
    break;

  case factor2:
    if (attstack[newp + 1].xval == 0.0)
      WITH->xval = 1.0;
    else
      WITH->xval = 0.0;
    break;

  case factor3:
    if (WITH->xval == 0.0 && attstack[newp + 2].xval < 0.0)
      markerror(852);
    else {
      j = (long)floor(attstack[newp + 2].xval + 0.5);
      r = j;
      if (r == attstack[newp + 2].xval)
	WITH->xval = intpow(WITH->xval, j);
      else if (WITH->xval < 0.0) {
	markerror(863);
	WITH->xval = 0.0;
      } else if (WITH->xval != 0.0)
	WITH->xval = exp(attstack[newp + 2].xval * log(WITH->xval));
    }
    break;

  /* placename = "<Label>" suffix */
  case placename1:
    if (attstack[newp + 1].lexval != XEMPTY)
      addsuffix(chbuf, &WITH->chbufx, &WITH->length, attstack[newp + 1].xval);
    prp = NULL;
    primp = envblock;
    while (primp != prp) {
      prp = findplace(primp->son, chbuf, WITH->chbufx, WITH->length);
      if (prp != NULL)
	primp = prp;
      else
	primp = primp->parent;
    }
    if (prp == NULL) {
      /*D else if debuglevel > 0 then with prp^ do begin
          write(log,'placename: type ', ptype:1,', ');
          wpair(log,aat.xpos,aat.ypos); writeln(log) end D*/
      marknotfound(854, chbuf, WITH->chbufx, WITH->length);
    }
    WITH->prim = prp;
    break;

  /* | nth primobj */
  case placename2:
    WITH->prim = nthprimobj(envblock->son, WITH->length,
			    attstack[newp + 1].lexval);
    if (WITH->prim == NULL)
      markerror(857);
    break;

  /* | placename "." "<Label>" suffix */
  case placename3:
    if (WITH->prim != NULL) {
      if (attstack[newp + 3].lexval != XEMPTY) {
	WITH1 = &attstack[newp + 2];
	addsuffix(chbuf, &WITH1->chbufx, &WITH1->length,
		  attstack[newp + 3].xval);
      }
      /*D if debuglevel > 0 then begin
         write(log,'Searching:'); printobject(prim);
         with attstack^[newp+2] do snapname(chbuf,chbufx,length);
         writeln(log) end; D*/
      primp = findplace(WITH->prim->son, chbuf, attstack[newp + 2].chbufx,
			attstack[newp + 2].length);
      if (primp == NULL) {
	/*D else if debuglevel > 0 then begin
	   write(log,'placename:'); printobject(primp) end D*/
	marknotfound(855, chbuf, attstack[newp + 2].chbufx,
		     attstack[newp + 2].length);
      }
      WITH->prim = primp;
    }
    break;

  /* | placename "." nth primobj */
  case placename4:
    if (WITH->prim != NULL) {
      WITH->prim = nthprimobj(WITH->prim->son, attstack[newp + 2].length,
			      attstack[newp + 3].lexval);
      if (WITH->prim == NULL)
	markerror(857);
    }
    break;

  /* nth = ncount "th" */
  /* | ncount "th" "last" */
  /* | "last" */
  case nth1:
    if (WITH->xval <= 0.0)
      markerror(856);
    else
      WITH->length = (long)floor(WITH->xval + 0.5);
    break;

  case nth2:
    if (WITH->xval <= 0.0)
      markerror(856);
    else
      WITH->length = -(long)floor(WITH->xval + 0.5);
    break;

  case nth3:
    WITH->length = 0;
    break;

  /* primobj = "<primitiv>" */
  /* | "[]" */
  /* | "<string>" */
  /* | "[" "]" */
  case primobj1:
  case primobj2:
  case primobj3:
    break;

  case primobj4:
    WITH->lexval = XBLOCK;
    break;

  /* ncount = "<float>" */
  case ncount1:
    break;

  /* | "`" expression "'" */
  /* | "lbrace" expression "rbrace" */
  case ncount2:
  case ncount3:
    WITH->xval = attstack[newp + 1].xval;
    break;

  /* logprod = logval */
  case logprod1:
    break;

  /* | logprod "&&" logval */
  case logprod2:
    if (WITH->xval == 0.0 || attstack[newp + 2].xval == 0.0)
      WITH->xval = 0.0;
    else
      WITH->xval = 1.0;
    break;

  /* logval = lcompare */
  case logval1:
    if (WITH->lexval == XLstring) {
      markerror(869);
      WITH->lexval = XLfloat;
      deletestringbox(&WITH->prim);
    }
    break;

  /* | stringexpr "<" stringexpr */
  case logval2:
    i = cmpstring(WITH->prim, attstack[newp + 2].prim);
    if (i < 0)
      WITH->xval = 1.0;
    else
      WITH->xval = 0.0;
    WITH->lexval = XLfloat;
    deletestringbox(&attstack[newp + 2].prim);
    deletestringbox(&WITH->prim);
    break;

  /* | expression "<" expression */
  case logval3:
    if (WITH->xval < attstack[newp + 2].xval)
      WITH->xval = 1.0;
    else
      WITH->xval = 0.0;
    break;

  /* lcompare = expression */
  /* | stringexpr */
  case lcompare1:
  case lcompare2:
    break;

  /* | lcompare "<compare>" expression */
  /* "<" "==" "!=" ">=" "<=" ">" */
  case lcompare3:
    if (WITH->lexval == XLstring) {
      markerror(869);
      bswitch = false;
      deletestringbox(&WITH->prim);
    } else {
      switch (attstack[newp + 1].lexval - XLcompare) {

      case 0:
	bswitch = (WITH->xval < attstack[newp + 2].xval);
	break;

      case 1:
	bswitch = (WITH->xval == attstack[newp + 2].xval);
	break;

      case 2:
	bswitch = (WITH->xval != attstack[newp + 2].xval);
	break;

      case 3:
	bswitch = (WITH->xval >= attstack[newp + 2].xval);
	break;

      case 4:
	bswitch = (WITH->xval <= attstack[newp + 2].xval);
	break;

      case 5:
	bswitch = (WITH->xval > attstack[newp + 2].xval);
	break;
      }
    }
    if (bswitch)
      WITH->xval = 1.0;
    else
      WITH->xval = 0.0;
    break;

  /* | lcompare "<compare>" stringexpr */
  case lcompare4:
    if (WITH->lexval != XLstring) {
      markerror(869);
      bswitch = false;
    } else {
      i = cmpstring(WITH->prim, attstack[newp + 2].prim);
      /*D if debuglevel > 0 then writeln(log,' cmpstring=',i:1); D*/
      switch (attstack[newp + 1].lexval - XLcompare) {

      case 0:
	bswitch = (i < 0);
	break;

      case 1:
	bswitch = (i == 0);
	break;

      case 2:
	bswitch = (i != 0);
	break;

      case 3:
	bswitch = (i >= 0);
	break;

      case 4:
	bswitch = (i <= 0);
	break;

      case 5:
	bswitch = (i > 0);
	break;
      }
      deletestringbox(&WITH->prim);
    }
    if (bswitch)
      WITH->xval = 1.0;
    else
      WITH->xval = 0.0;
    WITH->lexval = XLfloat;
    deletestringbox(&attstack[newp + 2].prim);
    break;

  /* primary = "<envvar>" */
  /* | "<name>" suffix */
  /* | "<float>" */
  /* | "(" logexpr ")" */
  /* | location ".x" */
  /* | location ".y" */
  /* | placename "<param>" */
  /* | "rand" "(" ")" */
  /* | "rand" "(" expression ")" */
  /* | "<func1>" "(" expression ")" */
  /* | "<func2>" "(" expression "," expression ")" */
  /* | "(" assignlist ")" */
  case primary1:
    if (envblock != NULL) {
      eb = findenv(envblock);
      WITH->xval = eb->UU.Ublock.env[WITH->lexval - XLenvvar - 1];
    }
    break;

  /* | "<name>" suffix */
  case primary2:
    if (attstack[newp + 1].lexval != XEMPTY)
      addsuffix(chbuf, &WITH->chbufx, &WITH->length, attstack[newp + 1].xval);
    namptr = glfindname(envblock, chbuf, WITH->chbufx, WITH->length, &lastvar,
			&k);
    if (namptr == NULL)
      WITH->xval = 0.0;
    else
      WITH->xval = namptr->val;
    break;

  /* | "<float>" */
  case primary3:
    break;

  /* | "(" logexpr ")" */
  case primary4:
    WITH->xval = attstack[newp + 1].xval;
    break;

  case primary5:   /* | place ".x" */
    break;

  case primary6:   /* | place ".y" */
    WITH->xval = WITH->yval;
    break;

  /* | placename "<param>" */
  case primary7:
    if (WITH->prim != NULL) {
      switch (attstack[newp + 1].lexval) {

      case XLheight:
	WITH->xval = pheight(WITH->prim);
	break;

      case XLwidth:
	WITH->xval = pwidth(WITH->prim);
	break;

      case XLradius:
      case XLdiameter:
	WITH2 = WITH->prim;
	if (WITH2->ptype == XLcircle)
	  WITH->xval = WITH2->UU.Ucircle.radius;
	else if (WITH2->ptype == XLarc)
	  WITH->xval = WITH2->UU.Uline.aradius;
	else
	  markerror(858);
	if (attstack[newp + 1].lexval == XLdiameter)
	  WITH->xval *= 2.0;
	break;

      case XLthickness:
	WITH2 = WITH->prim;
	if (WITH2->ptype == XLarc || WITH2->ptype == XLspline ||
	    WITH2->ptype == XLarrow || WITH2->ptype == XLline ||
	    WITH2->ptype == XLcircle || WITH2->ptype == XLellipse ||
	    WITH2->ptype == XLbox)
	  WITH->xval = WITH2->lthick;
	else
	  markerror(858);
	break;

      case XLlength:
	WITH2 = WITH->prim;
	if (WITH2->ptype == XLspline || WITH2->ptype == XLmove ||
	    WITH2->ptype == XLarrow || WITH2->ptype == XLline) {
	  primp = WITH->prim;
	  while (primp->son != NULL)
	    primp = primp->son;
	  r = fabs(primp->UU.Uline.endpos.xpos - WITH2->aat.xpos);
	  s = fabs(primp->UU.Uline.endpos.ypos - WITH2->aat.ypos);
	  if (r == 0.0)
	    WITH->xval = s;
	  else if (s == 0.0)
	    WITH->xval = r;
	  else
	    WITH->xval = sqrt(r * r + s * s);
	} else
	  markerror(858);
	break;
      }
    }
    break;

  /* | "rand" "(" ")" random number between 0 and 1 */
  case primary8:
    WITH->xval = (double)random() / randmax;
    break;

  /* | "rand" "(" expression ")" */
  case primary9:
    srandom((long)floor(attstack[newp + 2].xval + 0.5));
    WITH->xval = (double)random() / randmax;
    break;

  /* | "<func1>" "(" expression ")" */
  case primary10:
    switch (WITH->lexval) {

    case XLabs:
      WITH->xval = fabs(attstack[newp + 2].xval);
      break;

    case XLacos:
      t = attstack[newp + 2].xval;
      if (fabs(t) > 1.0)
	markerror(868);
      else
	WITH->xval = datan(sqrt(1 - t * t), t);
      break;

    case XLasin:
      t = attstack[newp + 2].xval;
      if (fabs(t) > 1.0)
	markerror(868);
      else
	WITH->xval = datan(t, sqrt(1 - t * t));
      break;

    case XLcos:
      WITH->xval = cos(attstack[newp + 2].xval);
      break;

    case XLexp:
      WITH->xval = exp(log(10.0) * attstack[newp + 2].xval);
      break;

    case XLexpe:
      WITH->xval = exp(attstack[newp + 2].xval);
      break;

    case XLint:
      WITH->xval = (long)attstack[newp + 2].xval;
      break;

    case XLlog:
      if (attstack[newp + 2].xval <= 0.0)
	markerror(867);
      else
	WITH->xval = log(attstack[newp + 2].xval) / log(10.0);
      break;

    case XLloge:
      if (attstack[newp + 2].xval <= 0.0)
	markerror(867);
      else
	WITH->xval = log(attstack[newp + 2].xval);
      break;

    case XLsign:
      if (attstack[newp + 2].xval >= 0.0)
	WITH->xval = 1.0;
      else
	WITH->xval = -1.0;
      break;

    case XLsin:
      WITH->xval = sin(attstack[newp + 2].xval);
      break;

    case XLsqrt:
      if (attstack[newp + 2].xval < 0.0)
	markerror(867);
      else
	WITH->xval = sqrt(attstack[newp + 2].xval);
      break;

    case XLtan:
      t = cos(attstack[newp + 2].xval);
      if (t == 0.0) {
	markerror(868);
	WITH->xval = distmax;
      } else
	WITH->xval = sin(attstack[newp + 2].xval) / t;
      break;

    case XLfloor:
      WITH->xval = Floor(attstack[newp + 2].xval);
      break;
    }
    break;

  /* | "<func2>" "(" expression "," expression ")" */
  case primary11:
    switch (WITH->lexval) {

    case XLatan2:
      WITH->xval = datan(attstack[newp + 2].xval, attstack[newp + 4].xval);
      break;

    case XLmax:
      WITH->xval = Max(attstack[newp + 2].xval, attstack[newp + 4].xval);
      break;

    case XLmin:
      WITH->xval = Min(attstack[newp + 2].xval, attstack[newp + 4].xval);
      break;

    case XLpmod:
      t = attstack[newp + 2].xval;
      s = attstack[newp + 4].xval;
      if (s == 0)
	markerror(871);
      else {
	WITH->xval = t - s * Floor(t / s);
	if (WITH->xval < 0)
	  WITH->xval += fabs(s);
      }
      break;
    }
    break;

  /* | "(" assignlist ")" */
  case primary12:
    WITH->xval = attstack[newp + 1].xval;

    break;
  }/* case */

  /*D; if debuglevel > 0 then printobject(prim); D*/
  /*D; if debuglevel > 0 then begin
     write(log,'for: initial='); wfloat(log,xval);
     write(log,' final='); wfloat(log,endchop);
     write(log,' incr='); wfloat(log,yval);
     writeln(log) end D*/
}


/*B.B*/
/* onefile */
/* G end.G */
/* onefile */
/*GH#include 'dpic1.p'HG*/

/* include dpic2.p */
/*BXsegment dpic2;XB*/
/* onefile */
/* G module dpic2; G */

/* include dp0.h */
/*BXU#include 'dp0.h'UXB*/
/* onefile */
/* G#include 'dp0.h'G */

/* include dp1.h */
/*BXU#include 'dp1.h'UXB*/
/* onefile */
/* G#include 'dp1.h'G */

/* include sysdep.h */
/*DBXU#include 'sysdep.h'UXBD*/
/* onefile */
/* DG#include 'sysdep.h'GD */

void controls(void)
{
  printf("\n ..controls ");
}


void wrand(void)
{
  printf(" and ");
}


void ddot(void)
{
  printf("\n ..");
}


void ddash(void)
{
  printf("\n --");
}


void getlinshade(primitive *n, primitive **tn, nametype **ss,
			nametype **so, double *f, boolean *sf)
{
  primitive *WITH;

  *tn = n;
  *ss = NULL;
  *so = NULL;
  *f = -1.0;
  while (*tn != NULL) {
    WITH = *tn;
    if (WITH->outlinep != NULL)
      *so = WITH->outlinep;
    *tn = (*tn)->son;
  }
  *tn = n;
  if (*sf)
    *sf = false;
  else {
    while (n != NULL) {
      WITH = n;
      if (WITH->shadedp != NULL)
	*ss = WITH->shadedp;
      if (WITH->UU.Uline.lfill >= 0.0 && WITH->UU.Uline.lfill <= 1.0)
	*f = WITH->UU.Uline.lfill;
      *tn = n;
      n = n->son;
    }
  }
  if (*ss != NULL || *f >= 0.0)
    *sf = true;
}


/* Arrowhead location (lex value) and number */
int ahlex(int atyp)
{
  return (atyp >> 3);
}


int ahnum(int atyp)
{
  return (atyp & 7);
}


int pahlex(int atyp, int alex)
{
  return ((atyp & 7) + alex * 8);
}


int pahnum(int atyp, int anum)
{
  return ((atyp >> 3) * 8 + (anum & 7));   /* 0 < anum < 7 */
}


void wfloat(FILE **iou, double y)
{
  /* var iou: text; y: real */
  char buf[25];
  int i;
  if (fabs(y)==distmax)
     sprintf(buf,"%24.6e", y);
  else if (y >= 0.0)
     sprintf(buf,"%24.6f", floor( 1000000*y+0.5)/1000000.0 );
  else
     sprintf(buf,"%24.6f",-floor(-1000000*y+0.5)/1000000.0 );
  for (i=23; buf[i]=='0'; ) i-- ;
  if (buf[i]=='.') buf[i] = (char)0;
  else buf[i+1] = (char)0;
  for (;(i>=0)&&(buf[i]!=' ');) i-- ;
  fprintf( *iou,"%s",&buf[i+1]);
}


void wpair(FILE **iou, double x, double y)
{
  /* var iou: text; x,y: real */
  putc('(', *iou);
  wfloat(iou, x);
  putc(',', *iou);
  wfloat(iou, y);
  putc(')', *iou);
}


void wcoord(FILE **iou, double x, double y)
{
  /* var iou: text; x,y: real */
  putc('(', *iou);
  wfloat(iou, x / fsc);
  putc(',', *iou);
  wfloat(iou, y / fsc);
  putc(')', *iou);
  /*DUGHM ;flush(iou) MHGUD*/
}


void wpos(postype pos)
{
  /* pos: postype */
  wcoord(&output, pos.xpos, pos.ypos);
}


void wstring(FILE **iou, nametype *p)
{
  /* var iou: text; p: strptr */
  int i, FORLIM;

  /*D if debuglevel <= 0 then begin end
      else if p = nil then begin end
      else if p^.segmnt = nil then begin end
      else with p^ do begin
         writeln(log,' seginx : len : seginx+len-1=',
            seginx:1,' : ',len:1,' : ',seginx+len-1:1);
         if seginx+len-1 > CHBUFSIZ then begin
            writeln(log,' Offending string:');
            write(log,'|');
            for i:=seginx to CHBUFSIZ do write(log,segmnt^[i]);
            writeln(log,'|')
            end;
         end;
         D*/
  if (p == NULL)
    return;
  if (p->segmnt != NULL) {
    FORLIM = p->seginx + p->len;
    for (i = p->seginx; i < FORLIM; i++)
      putc(p->segmnt[i], *iou);
  }
}


void wbrace(double x)
{
  putchar('{');
  wfloat(&output, x);
  putchar('}');
}


boolean testjust(int n, int flag)
{
  boolean Result;

  switch (flag) {

  case XEMPTY:
    Result = (n == 0);
    break;

  case XLcenter:
    Result = (n == 15);
    break;

  case XLrjust:
    Result = ((n & 3) == 1);
    break;

  case XLljust:
    Result = ((n & 3) == 2);
    break;

  case XLbelow:
    Result = (n >> 2 == 1);
    break;

  case XLabove:
    Result = (n >> 2 == 2);
    break;
  }
  return Result;
}


void checkjust(nametype *tp, boolean *A, boolean *B, boolean *L,
		      boolean *R)
{
  /* tp: strptr; var A,B,L,R: boolean */
  int i;

  if (tp == NULL) {
    *A = false;
    *B = false;
    *L = false;
    *R = false;
    return;
  }
  i = (long)floor(tp->val + 0.5);
  *A = testjust(i, XLabove);
  *B = testjust(i, XLbelow);
  *L = testjust(i, XLljust);
  *R = testjust(i, XLrjust);
}


int lspec(int n)
{
  /* n: integer): integer */
  /* if ((n div 16) mod 2) <> 0 then lspec := XLsolid
  else */
  return ((n & 7) + XLlinetype);
}


void getlinespec(primitive *nd, int *lsp, primitive **lastnd)
{
  /* nd: primitivep;
     var lsp: integer; var lastnd: primitivep */
  primitive *tn;

  tn = nd;
  if (nd->ptype == XLarc || nd->ptype == XLarrow || nd->ptype == XLline ||
      nd->ptype == XLspline) {
    while (tn->son != NULL)
      tn = tn->son;
  }
  *lastnd = tn;
  *lsp = lspec(tn->spec);
}


primitive *findenv(primitive *p)
{
  /* p: primitivep ): primitivep; external */
  primitive *q;

  q = NULL;
  while (p != q) {
    if (p->ptype != XBLOCK) {
      /* if (p^.ptype mod XLlastenv)<>XBLOCK then */
      p = p->parent;
    } else if (p->UU.Ublock.env == NULL)
      p = p->parent;
    else
      q = p;
  }
  /*D if debuglevel > 0 then begin
     if p = nil then writeln(log,'findenv: p=nil')
     else if p^.env = nil then writeln(log,'findenv: p^.env=nil');
     flush(log) end; D*/
  return p;
}


double venv(primitive *p, int ind)
{
  double v;

  v = 0.0;
  if (ind <= XLenvvar || ind > XLlastenv)
    return v;
  p = findenv(p);
  if (p != NULL)
    v = p->UU.Ublock.env[ind - XLenvvar - 1];
  return v;
}


double qenv(primitive *p, int ind, double localval)
{
  double noval;

  switch (ind) {

  case XLfillval:
    noval = -1.0;
    break;

  case XLlinethick:
    noval = mdistmax;
    break;

  case XLdashwid:
    noval = mdistmax;
    break;

  default:
    noval = 0.0;
    break;
  }
  if (localval != noval)
    return localval;
  else
    return (venv(p, ind));
}


/* orig + mat(cs) * [x,y] */
postype affine(double x, double y, postype orig, postype cs)
{
  postype tpos;

  tpos.xpos = orig.xpos + cs.xpos * x - cs.ypos * y;
  tpos.ypos = orig.ypos + cs.ypos * x + cs.xpos * y;
  return tpos;
}


postype affang(postype point, postype shaft)
{
  double lgth;
  postype tpos;

  lgth = linlen(shaft.xpos - point.xpos, shaft.ypos - point.ypos);
  if (lgth == 0.0) {
    tpos.xpos = 1.0;
    tpos.ypos = 0.0;
  } else {
    tpos.xpos = (point.xpos - shaft.xpos) / lgth;
    tpos.ypos = (point.ypos - shaft.ypos) / lgth;
  }
  return tpos;
}


double posangle(postype V, postype C)
{
  return (datan(V.ypos - C.ypos, V.xpos - C.xpos));
}


void initnesw(void)
{
  south = distmax;
  north = -south;
  west = south;
  east = -west;
}


/* Local variables for nesw: */
struct LOC_nesw {
  double hight, wdth;
} ;

Local void neswstring(primitive *pmp, struct LOC_nesw *LINK)
{
  boolean A, B, L, R;
  double x, y, offst;

  if (pmp == NULL)
    return;
  checkjust(pmp->textp, &A, &B, &L, &R);
  offst = venv(pmp, XLtextoffset);
  y = pmp->aat.ypos;
  if (A)
    y += LINK->hight * 0.5 + offst;
  else if (B)
    y += -LINK->hight * 0.5 - offst;
  if (drawmode == SVG)
    offst = pmp->UU.Ubox.boxheight / 3;
  x = pmp->aat.xpos;
  if (R)
    x += -LINK->wdth * 0.5 - offst;
  else if (L)
    x += LINK->wdth * 0.5 + offst;
  north = Max(north, y + LINK->hight * 0.5);
  south = Min(south, y - LINK->hight * 0.5);
  west = Min(west, x - LINK->wdth * 0.5);
  east = Max(east, x + LINK->wdth * 0.5);
  /*D; if debuglevel>0 then begin
     write(log,' neswstring:');
     wlogfl('aat.xpos',aat.xpos,0); wlogfl('x',x,0);
     wlogfl('east',east,0); wlogfl('west',west,0); wlogfl('wdth',wdth,1)
     end D*/
}

Local void neswline(primitive *pmp, struct LOC_nesw *LINK)
{
  double aht, awd;
  postype cs, cc, cd;
  int TEMP;

  if (pmp == NULL)
    return;
  west = Min(west, Min(pmp->aat.xpos, pmp->UU.Uline.endpos.xpos));
  east = Max(east, Max(pmp->aat.xpos, pmp->UU.Uline.endpos.xpos));
  south = Min(south, Min(pmp->aat.ypos, pmp->UU.Uline.endpos.ypos));
  north = Max(north, Max(pmp->aat.ypos, pmp->UU.Uline.endpos.ypos));
  TEMP = ahlex(pmp->UU.Uline.atype);
  if (TEMP == XLEFTHEAD || TEMP == XDOUBLEHEAD) {
    cs = affang(pmp->UU.Uline.endpos, pmp->aat);
    awd = qenv(pmp, XLarrowht, pmp->UU.Uline.width);
    aht = qenv(pmp, XLarrowwid, pmp->UU.Uline.height);
    cc = affine(aht, awd / 2, pmp->aat, cs);
    cd = affine(aht, awd / -2, pmp->aat, cs);
    west = Min(west, Min(cc.xpos, cd.xpos));
    east = Max(east, Max(cc.xpos, cd.xpos));
    south = Min(south, Min(cc.ypos, cd.ypos));
    north = Max(north, Max(cc.ypos, cd.ypos));
  }
  TEMP = ahlex(pmp->UU.Uline.atype);
  if (!(TEMP == XRIGHTHEAD || TEMP == XDOUBLEHEAD))
    return;
  cs = affang(pmp->aat, pmp->UU.Uline.endpos);
  awd = qenv(pmp, XLarrowht, pmp->UU.Uline.width);
  aht = qenv(pmp, XLarrowwid, pmp->UU.Uline.height);
  cc = affine(aht, awd / 2, pmp->UU.Uline.endpos, cs);
  cd = affine(aht, awd / -2, pmp->UU.Uline.endpos, cs);
  west = Min(west, Min(cc.xpos, cd.xpos));
  east = Max(east, Max(cc.xpos, cd.xpos));
  south = Min(south, Min(cc.ypos, cd.ypos));
  north = Max(north, Max(cc.ypos, cd.ypos));
}

Local boolean inarc(double strt, double fin, double ang, double arcang,
		    struct LOC_nesw *LINK)
{
  boolean inarctmp;

  /*D if debuglevel > 0 then begin
     write(log,'Inarc strt,fin='); wfloat(log,strt*180.0/pi);
        write(log,' ' ); wfloat(log,fin*180.0/pi);
     write(log,' ang,arcang='); wfloat(log,ang*180.0/pi);
        write(log,' ' ); wfloat(log,arcang*180.0/pi) end; D*/
  if (arcang >= 0.0) {
    while (fin < strt)
      fin += 2.0 * pi;
    while (ang < strt)
      ang += 2.0 * pi;
    if (ang <= fin)
      inarctmp = true;
    else
      inarctmp = false;
    return inarctmp;
  }
  while (fin > strt)
    fin -= 2.0 * pi;
  while (ang > strt)
    ang -= 2.0 * pi;
  if (ang >= fin)
    inarctmp = true;
  else
    inarctmp = false;
  return inarctmp;
  /*D; if debuglevel > 0 then writeln(log,' Inarc =',inarctmp) D*/
}


void nesw(primitive *ptmp)
{
  /* ptmp: primitivep */
  struct LOC_nesw V;
  double sa, ea;

  if (ptmp == NULL)
    return;
  switch (ptmp->ptype) {

  case XLbox:
  case XLstring:
    V.hight = ptmp->UU.Ubox.boxheight;
    V.wdth = ptmp->UU.Ubox.boxwidth;
    break;

  case XBLOCK:
    V.hight = ptmp->UU.Ublock.blockheight;
    V.wdth = ptmp->UU.Ublock.blockwidth;
    break;

  case XLcircle:
    V.hight = 2.0 * ptmp->UU.Ucircle.radius;
    V.wdth = 2.0 * ptmp->UU.Ucircle.radius;
    break;

  case XLellipse:
    V.hight = ptmp->UU.Uellipse.elheight;
    V.wdth = ptmp->UU.Uellipse.elwidth;
    break;
  }
  switch (ptmp->ptype) {

  case XLbox:
  case XBLOCK:
  case XLcircle:
  case XLellipse:
    north = Max(north, ptmp->aat.ypos + V.hight * 0.5);
    south = Min(south, ptmp->aat.ypos - V.hight * 0.5);
    west = Min(west, ptmp->aat.xpos - V.wdth * 0.5);
    east = Max(east, ptmp->aat.xpos + V.wdth * 0.5);
    break;

  case XLstring:
    neswstring(ptmp, &V);
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    neswline(ptmp, &V);
    break;

  case XLaTeX:
  case XLabel:
    /* blank case */
    break;

  case XLarc:
    sa = principal(ptmp->UU.Uline.endpos.xpos, pi);
    ea = ptmp->UU.Uline.endpos.xpos + ptmp->UU.Uline.endpos.ypos;
    /*D if debuglevel > 0 then begin write(log,'(sa,ea)(deg)=');
       wpair(log,sa*180/pi,ea*180/pi); writeln(log) end; D*/
    if (inarc(sa, ea, 0.5 * pi, ptmp->UU.Uline.endpos.ypos, &V))
      north = Max(north, ptmp->aat.ypos + ptmp->UU.Uline.aradius);
    else
      north = Max(north,
	  ptmp->aat.ypos + ptmp->UU.Uline.aradius * Max(sin(sa), sin(ea)));
    if (inarc(sa, ea, -0.5 * pi, ptmp->UU.Uline.endpos.ypos, &V))
      south = Min(south, ptmp->aat.ypos - ptmp->UU.Uline.aradius);
    else
      south = Min(south,
	  ptmp->aat.ypos + ptmp->UU.Uline.aradius * Min(sin(sa), sin(ea)));
    if (inarc(sa, ea, pi, ptmp->UU.Uline.endpos.ypos, &V))
      west = Min(west, ptmp->aat.xpos - ptmp->UU.Uline.aradius);
    else
      west = Min(west,
	  ptmp->aat.xpos + ptmp->UU.Uline.aradius * Min(cos(sa), cos(ea)));
    if (inarc(sa, ea, 0.0, ptmp->UU.Uline.endpos.ypos, &V))
      east = Max(east, ptmp->aat.xpos + ptmp->UU.Uline.aradius);
    else
      east = Max(east,
	  ptmp->aat.xpos + ptmp->UU.Uline.aradius * Max(cos(sa), cos(ea)));
    break;
  }
  /*D;if debuglevel > 0 then with ptmp^ do begin
     write(log, 'nesw(',ordp(ptmp):1,') ptype=',ptype:1);
     wlogfl('W',west,0); wlogfl('S',south,0);
     wlogfl('E',east,0); wlogfl('N',north,1)
     end D*/
}  /* nesw */


double linlen(double x, double y)
{
  /* x,y: real ): real */
  double xm, ym;

  /* linlen := sqrt( x*x + y*y ) */
  if (fabs(x) > fabs(y)) {
    xm = fabs(x);
    ym = y / xm;
    return (xm * sqrt(1.0 + ym * ym));
  }
  if (y == 0.0) {
    xm = 0.0;
    ym = 0.0;
  } else {
    xm = fabs(y);
    ym = x / xm;
  }
  return (xm * sqrt(1.0 + ym * ym));
}


void texstacktext(nametype *tp)
{
  nametype *tx;
  boolean A, B, L, R;

  if (tp == NULL)
    return;
  tx = tp->next_;
  if (tx != NULL)
    printf("\\shortstack{");
  do {
    checkjust(tp, &A, &B, &L, &R);
    if (L)
      printf("\\rlap{");
    else if (R)
      printf("\\llap{");
    wstring(&output, tp);
    if (L || R)
      putchar('}');
    tp = tp->next_;
    if (tp != NULL)
      printf("\\\\%%\n");
  } while (tp != NULL);
  if (tx != NULL)
    putchar('}');
}


int primdepth(primitive *ptmp)
{
  int dep;

  dep = 0;
  while (ptmp != NULL) {
    dep++;
    ptmp = ptmp->son;
  }
  return dep;
}


void pprop(postype p1, postype *p2, double a, double b, double c)
{
  if (c != 0.0) {
    p2->xpos = (a * p1.xpos + b * p2->xpos) / c;
    p2->ypos = (a * p1.ypos + b * p2->ypos) / c;
  }
}


void wprop(postype p1, postype p2, double a, double b, double c)
{
  pprop(p1, &p2, a, b, c);   /* Note: p2 is not var */
  wpos(p2);
}


boolean iscorner(double theta)
{
  /*D if debuglevel = 2 then
    writeln(log,'iscorner(',theta*180/pi:7:4,')=',
      (abs(theta) < 0.001) or (abs(0.5*pi-abs(theta)) < 0.001)); D*/
  return (fabs(theta) < 0.001 || fabs(0.5 * pi - fabs(theta)) < 0.001);
}


int hcf(int x, int y)
{
  int i, small, large;

  if (x < 0)
    x = -x;
  if (y < 0)
    y = -y;
  if (y < x) {
    small = y;
    large = x;
  } else {
    small = x;
    large = y;
  }
  while (small > 0) {
    i = small;
    small = large % small;
/* p2c: dpic2.p, line 478:
 * Note: Using % for possibly-negative arguments [317] */
    large = i;
  }
  if (large == 0)
    return 1;
  else
    return large;
}


int iabs(int i)
{
  if (i < 0)
    return (-i);
  else
    return i;
}


void wrslope(double xp, double yp, boolean arrow)
{
  int i, ix, iy;
  double r;

  /*D if debuglevel > 0 then begin
     write(log,'wrslope xp,yp: '); wpair(log,xp,yp) end; D*/
  if (xp == 0.0 && yp == 0.0) {
    xp = 1.0;
    yp = 0.0;
  }
  r = linlen(xp, yp);
  if (drawmode == pict2e)
    i = 1000;   /*4096*/
  else if (drawmode == tTeX)
    i = 453;
  else if (arrow)
    i = 4;
  else
    i = 6;
  iy = (long)floor((i + 0.49999) * yp / r + 0.5);
  ix = (long)floor((i + 0.49999) * xp / r + 0.5);
  i = hcf(ix, iy);
  iy /= i;
  ix /= i;
  /*D if debuglevel > 0 then begin
         write(log,' ix,iy:(',ix:1,',',iy:1,')'); write(log,' {');
         if ix = 0 then wfloat(log,abs(yp)/fsc) else wfloat(log,abs(xp)/fsc);
         writeln(log,'}')
         end; D*/
  printf("(%d,%d)", ix, iy);
  if (ix == 0)
    wbrace(fabs(yp) / fsc);
  else
    wbrace(fabs(xp) / fsc);
}


boolean isthen(primitive *pr)
{
  /* pr: primitivep ): boolean */
  if (pr == NULL)
    return false;
  else
    return (((pr->spec >> 3) & 1) != 0);
}


boolean drawn(primitive *node, int lsp, double fill)
{
  if (node == NULL)
    return false;
  else if (node->shadedp != NULL)
    return true;
  else if (lsp == XLpath || lsp == XLdotted || lsp == XLdashed ||
	   lsp == XLsolid || fill >= 0.0 && fill <= 1.0)
    return true;
  else
    return false;
}


double ahoffset(double ht, double wid, double lti)
{
  /* distance to P control point */
  if (wid == 0.0)
    return 0.0;
  else
    return (lti * sqrt(ht * ht + wid * wid / 4) / wid);
}


/*D procedure logpos( lbl: string; P: postype );
begin
   writeln(log);
   write(log,' ',lbl,':(');
   wfloat(log,P.xpos); write(log,','); wfloat(log,P.ypos); write(log,')')
   end; D*/
/* Arrowhead control points */
void dahead(postype point, postype shaft, double ht, double wid,
		   double ltu, postype *P, postype *L, postype *R,
		   postype *Px, postype *Lx, postype *Rx, postype *Q,
		   double *x, double *y)
{
  /* arrowhead ht and wid, user units */
  /* line thickness in diagram units */
  /* adj point, left, right pts, dir cosines */
  double h, v, po, t;

  /*D if debuglevel > 0 then begin
     writeln(log,' dahead input:');
     write(log,' ht='); wfloat(log,ht);
     write(log,';wid='); wfloat(log,wid);
     write(log,';ltu='); wfloat(log,ltu);
     write(log,';fsc='); wfloat(log,fsc);
     logpos('point',point);
     logpos('shaft',shaft);
     writeln(log) end; D*/
  *Q = affang(shaft, point);   /* shaft direction cosines */
  po = ahoffset(ht, wid, ltu);
  *P = affine(po, 0.0, point, *Q);   /* point adjusted by line thickness */
  h = ht - ltu / 2;
  *x = h - po;
  if (ht == 0.0)
    v = 0.0;
  else
    v = wid / 2 * *x / ht;
  *R = affine(h, v, point, *Q);
  *L = affine(h, -v, point, *Q);
  if (*x == 0.0)
    t = 1.0;
  else
    t = ht / *x;
  Rx->xpos = point.xpos + (R->xpos - P->xpos) * t;   /* right corner */
  Rx->ypos = point.ypos + (R->ypos - P->ypos) * t;
  Lx->xpos = point.xpos + (L->xpos - P->xpos) * t;   /* left corner */
  Lx->ypos = point.ypos + (L->ypos - P->ypos) * t;
  Px->xpos = (point.xpos + Lx->xpos + Rx->xpos) / 3;   /* type 3 center pt */
  Px->ypos = (point.ypos + Lx->ypos + Rx->ypos) / 3;
  if (ht == 0.0)
    *y = ht;
  else {
    *y = ht - po + ltu * wid / ht / 4;
    /*D; if debuglevel > 0 then begin
        write(log,' dahead out:');
        logpos('P',P); logpos('L',L); logpos('R',R); logpos('Q',Q);
        logpos('Px',Px); logpos('Lx',Lx); logpos('Rx',Rx); writeln(log);
        write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
        writeln(log) end D*/
  }
}


void popgwarc(postype Ctr, double radius, double startangle,
		     double endangle, double ccw)
{
  int narcs, i;
  double c, s, cc, ss, arcangle;
  postype Q;

  if (ccw > 0 && endangle < startangle)
    endangle += 2 * pi;
  else if (ccw < 0 && endangle > startangle)
    endangle -= 2 * pi;
  narcs = (long)(1.0 + fabs(endangle - startangle) / pi);
  arcangle = (endangle - startangle) / narcs;
  c = cos(arcangle / 2);
  s = sin(arcangle / 2);
  cc = (4 - c) / 3;
  if (s != 0.0)
    ss = (1.0 - c * cc) / s;
  else
    ss = 0.0;
  for (i = 1; i <= narcs; i++) {
    controls();
    Q.xpos = cos(startangle + (i - 0.5) * arcangle);
    Q.ypos = sin(startangle + (i - 0.5) * arcangle);
    wpos(affine(radius * cc, -radius * ss, Ctr, Q));
    wrand();
    wpos(affine(radius * cc, radius * ss, Ctr, Q));
    ddot();
    Q.xpos = Ctr.xpos + radius * cos(startangle + i * arcangle);
    Q.ypos = Ctr.ypos + radius * sin(startangle + i * arcangle);
    wpos(Q);
  }
}


/* Parameters and positions for traced arrows*/
void arcahead(postype C, postype point, int atyp, double ht,
		     double wid, double lth, double radius, double angle,
		     postype *P, postype *Co, postype *Ci, postype *Px,
		     postype *Cox, postype *Cix, postype *Ao, postype *Ai,
		     double *ccw, double *lwi, boolean *startarrow)
{
  double lw, aa, bb, cc, s, v, d, b, t;
  postype Q;
  double TEMP, TEMP1;

  /*D if debuglevel > 0 then begin
     write(log,' arcahead input:');
     logpos('C',C); logpos('point',point); writeln(log);
     write(log,' atyp='); write(log,atyp:1);
     write(log,';ht='); wfloat(log,ht);
     write(log,';wid='); wfloat(log,wid);
     write(log,';lth='); wfloat(log,lth);
     write(log,';radius='); wfloat(log,radius);
     write(log,';angle='); wfloat(log,angle);
     writeln(log) end; D*/
  if (radius * angle > 0)
    *ccw = 1.0;
  else
    *ccw = -1.0;
  *startarrow = (radius >= 0);
  ht = fabs(ht);
  wid = fabs(wid);
  *lwi = lth / 72 * scale;   /* line thickness in diagram units */
  lw = Min(fabs(*lwi), Min(wid / 2, ht / 2));
  wid = Max(wid, lw);
  radius = fabs(radius);
  d = sqrt(ht * ht + wid * wid / 4);
  /* Centres of the wing arcs */
  if (d == 0) {
    Q.xpos = 1.0;
    Q.ypos = 0.0;
  } else {
    Q.xpos = ht / d;
    Q.ypos = *ccw * wid / 2 / d;
  }
  *Ci = affine(C.xpos - point.xpos, C.ypos - point.ypos, point, Q);
  Q.ypos = -Q.ypos;
  *Co = affine(C.xpos - point.xpos, C.ypos - point.ypos, point, Q);
  /* Outer corner */
  if (radius == 0)
    t = 0.0;
  else
    t = Min(pi / 2, d / radius);
  Q.xpos = cos(t);
  Q.ypos = *ccw * sin(t);
  *Ao = affine(point.xpos - Co->xpos, point.ypos - Co->ypos, *Co, Q);
  TEMP = Ao->xpos - C.xpos;
  TEMP1 = Ao->ypos - C.ypos;
  /* Make angle(C to Ai) = angle(C to Ao) */
  aa = TEMP * TEMP + TEMP1 * TEMP1;
  bb = 2 * ((Ao->xpos - C.xpos) * (C.xpos - Ci->xpos) +
	    (Ao->ypos - C.ypos) * (C.ypos - Ci->ypos));
  TEMP = C.xpos - Ci->xpos;
  TEMP1 = C.ypos - Ci->ypos;
  cc = TEMP * TEMP + TEMP1 * TEMP1 - radius * radius;
  s = bb * bb - 4 * aa * cc;
  if (s < 0)
    v = aa;
  else
    v = (sqrt(s) - bb) / 2;
  *Ai = *Ao;
  pprop(C, Ai, aa - v, v, aa);
  /* Point adjusted for line thickness */
  if (d == 0)
    *P = point;
  else if (radius == 0)
    *P = C;
  else if (ht == d)
    *P = *Ao;
  else {
    b = 2 * radius * sqrt((1 - ht / d) / 2);   /* distance C to Co */
    /* Angle from Co-C to P, center C */
    Q.xpos = (b * b - lw * lw + 2 * lw * radius) / (2 * b * radius);
    if (fabs(Q.xpos) > 1) {
      P->xpos = (Ao->xpos + Ai->xpos) / 2;
      P->ypos = (Ao->ypos + Ai->ypos) / 2;
    } else {
      Q.ypos = -*ccw * sqrt(1 - Q.xpos * Q.xpos);
      *P = affine(radius * (Co->xpos - C.xpos) / b,
		  radius * (Co->ypos - C.ypos) / b, C, Q);
    }
  }
  /* Type 3 center and corners */
  if (radius == 0)
    t = 0.0;
  else
    t = Min(pi / 2, ht / radius * 2 / 3);
  Q.xpos = cos(t);
  Q.ypos = *ccw * sin(t);
  *Px = affine(point.xpos - C.xpos, point.ypos - C.ypos, C, Q);

  v = radius * radius;
  TEMP = Ao->xpos - Px->xpos;
  TEMP1 = Ao->ypos - Px->ypos;
  d = TEMP * TEMP + TEMP1 * TEMP1;
  if (d == 0)
    s = sqrt(v);
  else if (v / d < 0.25)
    s = 0.0;
  else
    s = sqrt(v / d - 0.25);
  Cox->xpos = (Px->xpos + Ao->xpos) / 2 - *ccw * (Ao->ypos - Px->ypos) * s;
  Cox->ypos = (Px->ypos + Ao->ypos) / 2 + *ccw * (Ao->xpos - Px->xpos) * s;

  TEMP = Ai->xpos - Px->xpos;
  TEMP1 = Ai->ypos - Px->ypos;
  d = TEMP * TEMP + TEMP1 * TEMP1;
  if (d == 0)
    s = sqrt(v);
  else if (v / d < 0.25)
    s = 0.0;
  else
    s = sqrt(v / d - 0.25);
  Cix->xpos = (Px->xpos + Ai->xpos) / 2 - *ccw * (Ai->ypos - Px->ypos) * s;
  Cix->ypos = (Px->ypos + Ai->ypos) / 2 + *ccw * (Ai->xpos - Px->xpos) * s;

  /*D; if debuglevel > 0 then begin
     writeln(log,' arcahead out:');
     write(log,' lw='); wfloat(log,lw);
     logpos('C',C); logpos( 'point',point);
     logpos('Ci',Ci); logpos( 'Ai',Ai);
     logpos('Co',Co); logpos( 'Ao',Ao);
     logpos('P',P);
     C.xpos := (Ao.xpos+Ai.xpos)/2; C.ypos := (Ao.ypos+Ai.ypos)/2;
     logpos('AoAi',C);
     logpos('Px',Px); logpos( 'Cox',Cox); logpos( 'Cix',Cix);
     writeln(log)
     end D*/
}


void startarc(primitive *n, postype X0, double lth, double *h,
		     double *w)
{
  double x, y;

  *h = qenv(n, XLarrowht, n->UU.Uline.height);
  *w = qenv(n, XLarrowwid, n->UU.Uline.width);
  y = ahoffset(*h, *w, lth / 72 * scale);
  if (n->UU.Uline.aradius * n->UU.Uline.aradius - y * y <= 0.0)
    x = 0.0;
  else
    x = 2 * atan(y / sqrt(n->UU.Uline.aradius * n->UU.Uline.aradius - y * y));
  if (n->UU.Uline.endpos.ypos >= 0.0) {
    n->UU.Uline.endpos.xpos += x;
    n->UU.Uline.endpos.ypos -= x;
  } else {
    n->UU.Uline.endpos.xpos -= x;
    n->UU.Uline.endpos.ypos += x;
  }
}


void endarc(primitive *n, postype X0, double lth, double *h, double *w)
{
  double x, y;

  *h = qenv(n, XLarrowht, n->UU.Uline.height);
  *w = qenv(n, XLarrowwid, n->UU.Uline.width);
  y = ahoffset(*h, *w, lth / 72 * scale);
  if (n->UU.Uline.aradius * n->UU.Uline.aradius - y * y <= 0.0)
    x = 0.0;
  else
    x = 2 * atan(y / sqrt(n->UU.Uline.aradius * n->UU.Uline.aradius - y * y));
  if (n->UU.Uline.endpos.ypos >= 0.0)
    n->UU.Uline.endpos.ypos -= x;
  else
    n->UU.Uline.endpos.ypos += x;
}


postype arcstart(primitive *n)
{
  postype X;

  X.xpos = n->aat.xpos + n->UU.Uline.aradius * cos(n->UU.Uline.endpos.xpos);
  X.ypos = n->aat.ypos + n->UU.Uline.aradius * sin(n->UU.Uline.endpos.xpos);
  return X;
}


postype arcend(primitive *n)
{
  postype X;

  X.xpos = n->aat.xpos + n->UU.Uline.aradius *
			 cos(n->UU.Uline.endpos.xpos + n->UU.Uline.endpos.ypos);
  X.ypos = n->aat.ypos + n->UU.Uline.aradius *
			 sin(n->UU.Uline.endpos.xpos + n->UU.Uline.endpos.ypos);
  return X;
}


/* include xfig.h */
/* xfig.x */
/* Output routines for xfig */

#define xfigres         1200
#define dispres         80
#define points          72


void xfigprelude(void)
{
  /* writeln('#FIG 3.1');
     writeln('Landscape');
     writeln('Center');
     writeln('Inches');
     writeln(xfigres:1,' 2');
     writeln('# ',Version, ' option -x for Fig 3.1')
     */

  printf("#FIG 3.2\n");
  printf("Landscape\n");
  printf("Center\n");
  printf("Inches\n");
  printf("Letter\n");
  printf("100.00\n");
  printf("Single\n");
  printf("-2\n");
  printf("# %s option -x for Fig 3.2\n", Version);
  printf("%ld 2\n", (long)xfigres);

}


void wfigpt(double x)
{
  printf(" %ld", (long)floor(x / fsc * xfigres + 0.5));
}


void wfigcoord(double x, double y)
{
  wfigpt(x);
  wfigpt(xfheight - y);
}


void arrowline(int atype, double wid, double ht, double lth)
{
  /* arrowtype,arrowstyle: integer;
     arrowthickness,arrowwidth,arrowheight: real */
  if (ahnum(atype) == 0)
    printf("%c0 0 ", tabch);
  else
    printf("%c1 1 ", tabch);
  wfloat(&output, lth * dispres / points);
  putchar(' ');
  wfloat(&output, wid * xfigres);
  putchar(' ');
  wfloat(&output, ht * xfigres);
  putchar('\n');
}


int linstyle(int i)
{
  if (i == XLsolid) {
    i = 0;
    return i;
  }
  if (i == XLdashed) {
    i = 1;
    return i;
  }
  if (i == XLdotted)
    i = 2;
  else
    i = -1;
  return i;
}


void hdrline(int object_code, int sub_type, int line_style, double lth,
		    double gfill)
{
  /* first 10 values object_code .. style_val */
  printf("%d %d %d ", object_code, sub_type, linstyle(line_style));

  if (line_style == XLinvis)
    printf("0 ");
  else
    printf("%ld ", (long)floor(lth * dispres / points + 0.5));
  printf("0 -1 0 -1 ");   /* pencolor, fillcolor, depth, penstyle */
  if (gfill == -1.0)
    printf("-1 ");   /* area fill */
  else
    printf("%ld ", (long)floor((1.0 - gfill) * 20 + 0.5));

  /* style_val */
  if (line_style == XLdashed)
    wfloat(&output, 5.0 / points * dispres);
  else if (line_style == XLdotted)
    wfloat(&output, 3.0 / points * dispres);
  else
    putchar('0');
  putchar(' ');
}


int fwdarrow(int i)
{
  if ((ahlex(i) == XRIGHTHEAD) | (ahlex(i) == XDOUBLEHEAD))
    return 1;
  else
    return 0;
}


int bckarrow(int i)
{
  if ((ahlex(i) == XLEFTHEAD) | (ahlex(i) == XDOUBLEHEAD))
    return 1;
  else
    return 0;
}


void polyline(int object_code, int sub_type, int line_style,
		     double lth, double gfill, double lrad, int atype,
		     double lwid, double lht, int npoints)
{
  hdrline(object_code, sub_type, line_style, lth, gfill);
  if (object_code == 3) {
    printf("0 ");   /* cap_style = butt for splines */
    /* join_style = miter, cap_style = butt, radius */
  } else
    printf("0 0 %ld ", (long)floor(lrad * dispres + 0.5));
  printf("%d %d %d\n", fwdarrow(atype), bckarrow(atype), npoints);
  if (fwdarrow(atype) == 1)
    arrowline(atype, lwid, lht, lth);
  if (bckarrow(atype) == 1)
    arrowline(atype, lwid, lht, lth);
}


void xfigwrtext(primitive *np, nametype *tp, double bxht, double bxwid,
		       double x, double y)
{
  double ydisp;
  int istr, nstr, figjust, i;
  boolean A, B, L, R;
  nametype *p;
  int FORLIM;

  if (bxht == 0.0)
    bxht = venv(np, XLtextht);
  if (tp != NULL) {
    nstr = 0;
    p = tp;
    do {
      nstr++;
      p = p->next_;
    } while (p != NULL);
    checkjust(tp, &A, &B, &L, &R);
    istr = 0;
  }
  while (tp != NULL) {
    istr++;
    printf("4 ");
    if (L)
      figjust = 0;
    else if (R)
      figjust = 2;
    else
      figjust = 1;
    printf("%d ", figjust);
    printf("-1 0 -1 ");   /* color, depth, penstyle */
    printf("0 10 0.0 2");   /* font, font_size, angle, font_flags */
    wfigpt(bxht);
    if (bxwid == 0)
      wfigpt(tp->len * bxht * 2 / 3);
    else
      wfigpt(bxwid);
    /*D if debuglevel > 0 then writeln(log,
      ' bxht=',bxht:9:2,' bxwid=',bxwid:9:2,' length=',tp^.len:1); D*/
    if (A)
      ydisp = bxht / 5 + venv(np, XLtextoffset);
    else if (B)
      ydisp = -bxht - venv(np, XLtextoffset);
    else
      ydisp = bxht / -3;
    wfigcoord(x, y + ydisp + ((nstr + 1.0) / 2 - istr) * bxht);
    putchar(' ');
    FORLIM = tp->len;
    for (i = 0; i < FORLIM; i++) {
      if (tp->segmnt[tp->seginx + i] == bslch)
	putchar(bslch);
      putchar(tp->segmnt[tp->seginx + i]);
    }
    printf("\\001\n");
    tp = tp->next_;
  }
}


void farc(int object_code, int sub_type, int line_style, double lth,
		 double gfill, int atype, double radius, double strtang,
		 double arcang, double x, double y, double lwid, double lht)
{
  hdrline(object_code, sub_type, line_style, lth, gfill);
  printf("0 ");
  if (arcang < 0.0)
    printf("0 ");
  else
    printf("1 ");
  printf("%d %d ", fwdarrow(atype), bckarrow(atype));
  wfigcoord(x, y);
  wfigcoord(x + radius * cos(strtang), y + radius * sin(strtang));
  wfigcoord(x + radius * cos(strtang + arcang / 2),
	    y + radius * sin(strtang + arcang / 2));
  wfigcoord(x + radius * cos(strtang + arcang),
	    y + radius * sin(strtang + arcang));
  putchar('\n');
  if (fwdarrow(atype) == 1)
    arrowline(atype, lwid, lht, lth);
  if (bckarrow(atype) == 1)
    arrowline(atype, lwid, lht, lth);
}


void fellipse(int object_code, int sub_type, int line_style,
		     double lth, double gfill, double center_x,
		     double center_y, double radius_x, double radius_y)
{
  hdrline(object_code, sub_type, line_style, lth, gfill);
  printf("1 0.0");
  wfigcoord(center_x, center_y);
  wfigpt(radius_x);
  wfigpt(radius_y);
  wfigcoord(center_x + radius_x, center_y);
  wfigcoord(center_x + radius_x, center_y);
  putchar('\n');
}


boolean rdrawn(primitive *np)
{
  boolean rv, v;
  primitive *WITH;

  rv = false;
  while (rv == false && np != NULL) {
    WITH = np;
    if (WITH->ptype == XLbox)
      v = drawn(np, lspec(WITH->spec), WITH->UU.Ubox.boxfill);
    else if (WITH->ptype == XLcircle)
      v = drawn(np, lspec(WITH->spec), WITH->UU.Ucircle.cfill);
    else if (WITH->ptype == XLellipse)
      v = drawn(np, lspec(WITH->spec), WITH->UU.Uellipse.efill);
    else if (WITH->ptype == XLspline || WITH->ptype == XLarrow ||
	     WITH->ptype == XLline || WITH->ptype == XLarc) {
      /* v := drawn(np,lspec(spec),-1.0) */
      v = drawn(np, lspec(WITH->spec), WITH->UU.Uline.lfill);
    } else
      v = false;
    if (v || WITH->textp != NULL) {
      rv = true;
      break;
    }
    if (rdrawn(WITH->son))
      rv = true;
    else
      np = WITH->next_;
  }
  return rv;
}


void xfigdraw(primitive *node)
{
  int i, lsp;
  double fill;
  primitive *tn;
  int FORLIM;

  getlinespec(node, &lsp, &tn);
  if (node->lthick < 0.0)
    node->lthick = venv(node, XLlinethick);
  /*D if debuglevel > 0 then with node^ do begin
     write(log,'xfigdraw(',ordp(node):1,'): lthick=',lthick:9:2,' ');
     snaptype(log,ptype); writeln(log)
     end; D*/
  switch (node->ptype) {

  case XLarc:
    /* if drawn(node,lsp,-1.0) then farc(5,1,lsp,lthick, */
    if (drawn(node, lsp, node->UU.Uline.lfill)) {
      /* -1.0,atype,aradius,endpos.xpos, */
      farc(5, 1, lsp, node->lthick, node->UU.Uline.lfill, node->UU.Uline.atype,
	   node->UU.Uline.aradius, node->UU.Uline.endpos.xpos,
	   node->UU.Uline.endpos.ypos, node->aat.xpos, node->aat.ypos,
	   node->UU.Uline.width, node->UU.Uline.height);
    }
    xfigwrtext(node, node->textp, 0.0, 0.0, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
  case XLspline:
    /* if drawn(node,lsp,-1.0) then begin */
    if (drawn(node, lsp, node->UU.Uline.lfill)) {
      if (!isthen(node)) {
	spltot = primdepth(node);
	if (node->ptype == XLspline) {
	  /* -1.0,0.0,atype,width,height,spltot+1) */
	  polyline(3, 0, lsp, node->lthick, node->UU.Uline.lfill, 0.0,
		   node->UU.Uline.atype, node->UU.Uline.width,
		   node->UU.Uline.height, spltot + 1);
	} else {
	  /* -1.0,0.0,atype,width,height,spltot+1); */
	  polyline(2, 1, lsp, node->lthick, node->UU.Uline.lfill, 0.0,
		   node->UU.Uline.atype, node->UU.Uline.width,
		   node->UU.Uline.height, spltot + 1);
	}
	putchar(tabch);
	wfigcoord(node->aat.xpos, node->aat.ypos);
      }
      wfigcoord(node->UU.Uline.endpos.xpos, node->UU.Uline.endpos.ypos);
      if (node->son == NULL && node->ptype == XLspline) {
	printf("\n%c 0.0", tabch);
	FORLIM = spltot;
	for (i = 2; i <= FORLIM; i++)
	  printf(" 1.0");
	printf(" 0.0\n");
      } else if (node->son == NULL)
	putchar('\n');
    }
    xfigwrtext(node, node->textp, 0.0, 0.0,
	       0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	       0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XBLOCK:
  case XLbox:
    initnesw();
    nesw(node);
    if ((node->ptype == XBLOCK) & ((node->textp != NULL) | drawn(node, lsp,
				     -1.0) | rdrawn(node->son))) {
      node->direction = -1;
      printf("6 ");
      wfigcoord(west, north);
      wfigcoord(east, south);
      putchar('\n');
    }
    if (node->ptype == XBLOCK)
      fill = -1.0;
    else
      fill = node->UU.Ubox.boxfill;
    if (drawn(node, lsp, fill)) {
      polyline(2, 2, lsp, node->lthick, fill, 0.0, -1, 0.0, 0.0, 6);
      putchar(tabch);
      wfigcoord(west, (south + north) / 2);
      wfigcoord(west, south);
      wfigcoord(east, south);
      wfigcoord(east, north);
      wfigcoord(west, north);
      wfigcoord(west, (south + north) / 2);
      putchar('\n');
    }
    xfigwrtext(node, node->textp, 0.0, 0.0, node->aat.xpos, node->aat.ypos);
    break;

  case XLmove:
    xfigwrtext(node, node->textp, 0.0, 0.0,
	       0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	       0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLellipse:
    if (drawn(node, lsp, node->UU.Uellipse.efill))
      fellipse(1, 1, lsp, node->lthick, node->UU.Uellipse.efill, node->aat.xpos,
	       node->aat.ypos, 0.5 * node->UU.Uellipse.elwidth,
	       0.5 * node->UU.Uellipse.elheight);
    xfigwrtext(node, node->textp, 0.0, 0.0, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->UU.Ucircle.cfill))
      fellipse(1, 3, lsp, node->lthick, node->UU.Ucircle.cfill, node->aat.xpos,
	       node->aat.ypos, node->UU.Ucircle.radius, node->UU.Ucircle.radius);
    xfigwrtext(node, node->textp, 0.0, 0.0, node->aat.xpos, node->aat.ypos);
    break;

  case XLstring:
    xfigwrtext(node, node->textp, node->UU.Ubox.boxheight,
	       node->UU.Ubox.boxwidth, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
      wstring(&output, node->textp);
      putchar('\n');
    }
    break;
  }
}  /* xfigdraw */


/* include svg.h */
/* svg.x */
/* Output routines for SVG */

void svgprelude(double n, double s, double e, double w, double lth)
{
  double hsize, vsize;

  /*D if debuglevel > 0 then begin
     writeln(log,' svgprelude:');
     write(log,' n='); wfloat(log,n); write(log,' s='); wfloat(log,s);
     write(log,' e='); wfloat(log,e); write(log,' w='); wfloat(log,w);
     write(log,' fsc='); wfloat(log,fsc); write(log,' lth='); wfloat(log,lth);
     writeln(log) end; D*/
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
  printf("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
  printf("\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  printf("<!-- Creator: %s option -v for SVG 1.1 -->\n", Version);
  hsize = (e - w + 2 * lth) / fsc;
  vsize = (n - s + 2 * lth) / fsc;
  printf("<!-- width=\"%d\" height=\"%d\" -->\n",
	 Ceil(hsize + 0.5), Ceil(vsize + 0.5));
  printf("<svg\n");
  printf(" xmlns=\"http://www.w3.org/2000/svg\"");
  printf(" xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n");
  printf(" xml:space=\"preserve\"\n");
  printf(" style=\"shape-rendering:geometricPrecision;");
  printf(" text-rendering:geometricPrecision;\n");
  printf(" image-rendering:optimizeQuality;");
  printf(" fill-rule:evenodd; clip-rule:evenodd\"\n");
  printf(" width=\"");
  wfloat(&output, hsize);
  printf("px\"");
  printf(" height=\"");
  wfloat(&output, vsize);
  printf("px\"\n");
  printf(" viewBox=\"0 0 ");
  wfloat(&output, hsize);
  putchar(' ');
  wfloat(&output, vsize);
  printf("\"\n");
  printf(" font-size=\"");
  wfloat(&output, DFONT);
  printf("pt\" text-anchor=\"middle\"\n");
  printf(" stroke=\"black\"");
  printf(" stroke-miterlimit=\"10\"");
  printf(" stroke-width=\"");
  wfloat(&output, 0.8 / 72 * SVGPX);
  printf("\" fill=\"none\">\n");
  printf("<g>\n");
  lastthick = 0.8;
}


void space(void)
{
  putchar(' ');
}


void quote(void)
{
  putchar('"');
}


void svgpostlude(void)
{
  printf("</g></svg>\n");
  /*D; if debuglevel > 0 then writeln(log,'svgpostlude done') D*/
}


void svgsetstroke(double lth)
{
  if (lth == lastthick)
    return;
  printf(" stroke-width=\"");
  wfloat(&output, lth / 72 * SVGPX);
  printf("\"\n");
}


void svgsoutline(nametype *so)
{
  printf(" stroke=\"");
  wstring(&output, so);
  printf("\"\n");
}


void fillgray(double fll)
{
  printf(" fill=\"");
  if (fll == 0.0)
    printf("black");
  else if (fll == 0.25)
    printf("darkgray");
  else if (fll == 0.5)
    printf("gray");
  else if (fll == 0.75)
    printf("lightgray");
  else if (fll == 1.0)
    printf("white");
  else {
    printf("rgb(");
    fprintf(output, "%ld", (long)floor(fll * 255 + 0.5));
    putchar(',');
    fprintf(output, "%ld", (long)floor(fll * 255 + 0.5));
    putchar(',');
    fprintf(output, "%ld", (long)floor(fll * 255 + 0.5));
    putchar(')');
  }
  printf("\"\n");
}


void svglineoptions(primitive *node, int lnspec)
{
  double param;

  if (node->lthick >= 0.0)
    svgsetstroke(node->lthick);
  if (soutline != NULL)
    svgsoutline(soutline);
  if (node->ptype == XBLOCK)
    lnspec = XLinvis;
  if (lnspec != XLinvis && lnspec != XLpath && lnspec != XLdotted &&
      lnspec != XLdashed)
	/* XLsolid,*/
	  return;
  switch (lnspec) {

  case XLdashed:
    param = qenv(node, XLdashwid, node->lparam) / fsc;
    printf(" stroke-dasharray=\"");
    wfloat(&output, param);
    putchar(',');
    wfloat(&output, param);
    printf("\"\n");
    break;

  case XLdotted:
    /* if lparam = mdistmax then param := 5/72*scale
       else param := lparam; */
    if (node->lparam != mdistmax)
      param = node->lparam;
    else
      param = qenv(node, XLlinethick, node->lthick) / 72 * 5 * scale;
    printf(" stroke-linecap=\"round\"");
    printf(" stroke-dasharray=\"0,");
    wfloat(&output, param / fsc);
    printf("\"\n");
    break;

  case XLinvis:
  case XLpath:
    printf(" stroke=\"none\"");
    break;
  }
}


void svgfilloptions(primitive *node, int lnspec, boolean poly)
{
  double fill;

  fill = -1.0;
  switch (node->ptype) {

  case XBLOCK:
    /* blank case */
    break;

  case XLbox:
  case XLstring:
    fill = node->UU.Ubox.boxfill;
    break;

  case XLcircle:
    fill = node->UU.Ucircle.cfill;
    break;

  case XLellipse:
    fill = node->UU.Uellipse.efill;
    break;

  case XLarc:
  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    fill = node->UU.Uline.lfill;
    break;
  }
  fill = (long)floor(fill * 1000000L + 0.5) / 1000000.0;
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    printf("<rect");
    break;

  case XLellipse:
    printf("<ellipse");
    break;

  case XLcircle:
    printf("<circle");
    break;

  case XLspline:
    printf("<path");
    break;

  case XLline:
  case XLarrow:
    if (poly)
      printf("<polyline");
    else
      printf("<line");
    break;

  case XLarc:
    printf("<path");
    break;
  }
  if (sshade != NULL) {
    printf(" fill=\"");
    wstring(&output, sshade);
    printf("\"\n");
  } else if (fill >= 0.0 && fill <= 1.0)
    fillgray(fill);
  else if (node->ptype == XLstring)
    fillgray(0.0);
  svglineoptions(node, lnspec);
}


void svgendpath(void)
{
  printf(" />\n");
}


void svgparam(Char *p, double x)
{
  printf(" %s=\"", p);
  wfloat(&output, x / fsc);
  quote();
}


void svgcoord(Char *s1, Char *s2, double x, double y)
{
  svgparam(s1, x);
  /*DUGHM ;flush(output) MHGUD*/
  svgparam(s2, xfheight - y);
}


void svgwpos(postype p)
{
  wfloat(&output, p.xpos / fsc);
  putchar(',');
  /*D;if debuglevel > 0 then begin
     write(log,'svgwpos(');
     wfloat(log,p.xpos/fsc);
     write(log,',');
     wfloat(log,(xfheight-p.ypos)/fsc);
     writeln(log,')')
     end D*/
  wfloat(&output, (xfheight - p.ypos) / fsc);
}


void svgwprop(postype p1, postype p2, double a, double b, double c)
{
  pprop(p1, &p2, a, b, c);
  svgwpos(p2);
}


void svgwstring(nametype *p)
{
  int i;
  Char c;
  boolean waswhite, iswhite;
  int FORLIM;

  if (p == NULL)
    return;
  if (p->segmnt == NULL)
    return;
  waswhite = false;
  /* kludge: precede string with a blank to add offset */
  if (p->len > 0)
    putchar(' ');
  FORLIM = p->len;
  for (i = 0; i < FORLIM; i++) {
    c = p->segmnt[p->seginx + i];
    iswhite = (c == etxch || c == nlch || c == tabch || c == ' ');
    if (!iswhite || !waswhite)
      putchar(c);
    waswhite = iswhite;
  }
}


void svgwtext(primitive *node, nametype *tp, double x, double y)
{
  int nstr;
  nametype *tx;
  boolean L, R, A, B;
  double textht, textoff, dx, dy;

  if (tp == NULL)
    return;
  tx = tp;
  nstr = 0;
  textht = venv(node, XLtextht);
  while (tx != NULL) {
    nstr++;
    tx = tx->next_;
  }
  /* boxheight = nstrings * textht */
  if (node->ptype == XLstring && nstr > 0)
    textht = node->UU.Ubox.boxheight / nstr;
  textoff = venv(node, XLtextoffset);
  /*D if debuglevel>0 then begin
    writeln(log,' svgwtext: x=',x:8:3,'[',x/fsc:8:3,'] y=',y:8:3,
     '[',(xfheight-y)/fsc:8:3,']');
    writeln(log,' xfheight=',xfheight:8:3);
    writeln(log,' textoff=',textoff:8:3,'[',textoff/fsc:8:3,
     '] textht=',textht:8:3,'[',textht/fsc:8:3,']')
    end; D*/
  y += (nstr / 2.0 - 4.0 / 5) * textht;
  do {
    checkjust(tp, &A, &B, &L, &R);
    printf("<text");
    if (node->ptype != XLstring)
      printf(" stroke-width=\"0.2pt\" fill=\"black\"");
    else {
      printf(" font-size=\"");
      wfloat(&output, textht / scale * 72);
      printf("pt\"");
      svgfilloptions(node, lspec(node->spec), false);
      if (node->lthick < 0)
	svgsetstroke(0.2);
    }
    if (L)
      printf(" text-anchor=\"start\"");
    else if (R)
      printf(" text-anchor=\"end\"");
    if (L)
      dx = textoff - textht * 2 / 5;
    else if (R)
      dx = textht / 7 - textoff;
    else
      dx = textht / -7;
    dy = textht / -20;
    if (A)
      dy += textoff + textht / 2;
    else if (B)
      dy += textht * (1 - TEXTRATIO) / 2 - textoff;
    /*D if debuglevel>0 then begin
      writeln(log,' A=',A,' B=',B,' L=',L,' R=',R,
       ' dy=',dy:8:3,'[',dy/fsc:8:3,']');
      writeln(log,' x=',x:8:3,'[',x/fsc:8:3,
       '] y=',y:8:3,'[',(xfheight-y)/fsc:8:3);
      writeln(log,' x+dx=',(x+dx):8:3,'[',(x+dx)/fsc:8:3,']',
       ' y+dy=',(y+dy):8:3,'[',(xfheight-(y+dy))/fsc:8:3,']') end; D*/
    svgcoord("x", "y", Max(0.0, x + dx), Max(0.0, y + dy));
    printf(">\n");
    svgwstring(tp);
    printf("\n</text>\n");
    /*if ptype=XLstring then y:=y-textp^.val else y := y-textht*TEXTRATIO;*/
    y -= textht;
    tp = tp->next_;
  } while (tp != NULL);
}


postype rot(postype V, postype Q)
{
  postype p;

  p.xpos = Q.xpos * V.xpos - Q.ypos * V.ypos;
  p.ypos = Q.ypos * V.xpos + Q.xpos * V.ypos;
  return p;
}


void svgwarc(postype E, double r, double angle, double ccw)
{
  printf(" A ");
  wfloat(&output, fabs(r) / fsc);
  space();
  wfloat(&output, fabs(r) / fsc);
  printf(" 0 ");
  if (fabs(angle) > pi)
    putchar('1');
  else
    putchar('0');
  if (ccw > 0)
    printf(" 0 ");
  else
    printf(" 1 ");
  svgwpos(E);
  if (angle == 0)
    putchar('\n');
}


void svgarcahead(postype C, int atyp, postype *point, nametype *sou,
			double ht, double wid, double lth, double radius,
			double angle)
{
  postype P, Q, Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  arcahead(C, *point, atyp, ht, wid, lth, radius, angle, &P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  /* Trace arrowhead outline */
  printf("<path stroke-width=\"0\"");
  if (sou != NULL) {
    svgsoutline(sou);
    printf(" fill=\"");
    wstring(&output, sou);
    printf("\"\n");
  } else
    printf(" fill=\"black\"\n");
  if (atyp == 0 && lwi < (wid - lwi) / 2) {
    printf(" d=\"M ");
    svgwpos(P);
    putchar('\n');
    Q = Ci;
    pprop(Ai, &Q, radius + lwi, -lwi, radius);
    svgwarc(Q, radius + lwi, 0.0, ccw);
    printf(" L ");
    svgwpos(Ai);
    putchar('\n');
    svgwarc(*point, radius, 0.0, -ccw);
    svgwarc(Ao, radius, 0.0, ccw);
    printf(" L ");
    svgwprop(Ao, Co, radius - lwi, lwi, radius);
    putchar('\n');
    svgwarc(P, radius - lwi, 1.0, -ccw);
  } else if (atyp == 3 && lwi < (wid - lwi) / 2) {
    printf(" d=\"M ");
    svgwpos(Px);
    putchar('\n');
    svgwarc(Ai, radius, 0.0, ccw);
    svgwarc(*point, radius, 0.0, -ccw);
    svgwarc(Ao, radius, 0.0, ccw);
    svgwarc(Px, radius, 1.0, -ccw);
  } else {
    Q.xpos = (Ao.xpos + Ai.xpos) / 2;
    Q.ypos = (Ao.ypos + Ai.ypos) / 2;
    printf(" d=\"M ");
    svgwpos(Q);
    printf("\n L ");
    svgwpos(Ai);
    putchar('\n');
    svgwarc(*point, radius, 0.0, -ccw);
    svgwarc(Ao, radius, 0.0, ccw);
    printf(" L ");
    svgwpos(Q);
  }
  quote();
  svgendpath();
  *point = P;
}


void svgahead(int atyp, postype *point, postype shaft, nametype *sou,
		     double ht, double wid, double lth, double fill)
{
  postype P, L, R, Px, Lx, Rx, Q;
  double x, y;

  dahead(*point, shaft, ht, wid, lth / 72 * scale, &P, &L, &R, &Px, &Lx, &Rx,
	 &Q, &x, &y);
  /*D if debuglevel > 0 then begin
      write(log,' dahead values: ');
      logpos('point',point); logpos('shaft',shaft); writeln(log);
      write(log,' ht='); wfloat(log,ht);
      write(log,' wid='); wfloat(log,wid);
      write(log,' lth='); wfloat(log,lth);
      write(log,' scale='); wfloat(log,scale);
      write(log,' fsc='); wfloat(log,fsc);
      write(log,' xfheight='); wfloat(log,xfheight);
      logpos('P',P); logpos('L',L); logpos('R',R); logpos('Q',Q);
      logpos('Px',Px); logpos('Lx',Lx); logpos('Rx',Rx);
      writeln(log);
      write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
      writeln(log) end; D*/
  if (atyp == 0) {
    printf("<polyline");
    if (lth > 0.0)
      svgsetstroke(lth);
    if (sou != NULL)
      svgsoutline(sou);
    printf(" points=\"");
    svgwprop(P, L, x - y, y, x);
    space();
    svgwpos(P);
    putchar('\n');
    svgwprop(P, R, x - y, y, x);
  } else if (atyp == 3) {
    printf("<polygon stroke-width=\"0\"");
    if (sou != NULL) {
      svgsoutline(sou);
      printf(" fill=\"");
      wstring(&output, sou);
      printf("\"\n");
    } else if (fill >= 0.0 && fill <= 1.0)
      fillgray(fill);
    else
      printf(" fill=\"black\"\n");
    printf(" points=\"");
    svgwpos(Px);
    space();
    svgwpos(Lx);
    putchar('\n');
    svgwpos(*point);
    space();
    svgwpos(Rx);
  } else {
    printf("<polygon");
    if (lth > 0.0)
      svgsetstroke(lth);
    if (sou != NULL) {
      svgsoutline(sou);
      printf(" fill=\"");
      wstring(&output, sou);
      printf("\"\n");
    } else if (fill >= 0.0 && fill <= 1.0)
      fillgray(fill);
    else
      printf(" fill=\"black\"\n");
    printf(" points=\"");
    Q.xpos = (L.xpos + R.xpos) / 2;
    Q.ypos = (L.ypos + R.ypos) / 2;
    svgwpos(Q);
    space();
    svgwpos(R);
    putchar('\n');
    svgwpos(P);
    space();
    svgwpos(L);
  }
  quote();
  svgendpath();
  *point = P;
}


void svgdraw(primitive *node)
{
  /* node is always <> nil */
  int lsp;
  postype X0, X1, X2;
  primitive *tn;
  boolean v;
  double h, w, lth;
  int TEMP;

  getlinespec(node, &lsp, &tn);
  sshade = node->shadedp;
  soutline = node->outlinep;
  lth = qenv(node, XLlinethick, node->lthick);   /* printobject(node); */
  /*D if debuglevel > 0 then begin write(log,'svgdraw: ');
     snaptype(log,ptype); D*/
  /*D
           write(log,' lth='); wfloat(log,lth);
           write(log,' aat=(');wfloat(log,aat.xpos); write(log,',');
           wfloat(log,aat.ypos); write(log,')');
           writeln(log)
           end;
        if linesignal > 0 then begin write(errout,'svgdraw: ');
           snaptype(errout,ptype); writeln(errout) end D*/
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    initnesw();
    nesw(node);
    if (node->ptype == XBLOCK)
      v = drawn(node, lsp, -1.0);
    else
      v = drawn(node, lsp, node->UU.Ubox.boxfill);
    if (v) {
      svgfilloptions(node, lsp, false);
      svgcoord("x", "y", node->aat.xpos - fabs(node->UU.Ubox.boxwidth) / 2,
	       node->aat.ypos + fabs(node->UU.Ubox.boxheight) / 2);
      if (node->UU.Ubox.boxradius > 0.0) {
	svgparam("rx", node->UU.Ubox.boxradius);
	svgparam("ry", node->UU.Ubox.boxradius);
      }
      svgparam("width", fabs(node->UU.Ubox.boxwidth));
      svgparam("height", fabs(node->UU.Ubox.boxheight));
      svgendpath();
    }
    svgwtext(node, node->textp, 0.5 * (east + west), 0.5 * (north + south));
    break;

  case XLellipse:
    if (drawn(node, lsp, node->UU.Uellipse.efill)) {
      svgfilloptions(node, lsp, false);
      svgcoord("cx", "cy", node->aat.xpos, node->aat.ypos);
      svgparam("rx", 0.5 * fabs(node->UU.Uellipse.elwidth));
      svgparam("ry", 0.5 * fabs(node->UU.Uellipse.elheight));
      svgendpath();
    }
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->UU.Ucircle.cfill)) {
      svgfilloptions(node, lsp, false);
      svgcoord("cx", "cy", node->aat.xpos, node->aat.ypos);
      svgparam("r", fabs(node->UU.Ucircle.radius));
      svgendpath();
    }
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    if (drawn(node, lsp, -1.0)) {
      getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
      lth = qenv(tn, XLlinethick, tn->lthick);
      X1 = arcstart(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	startarc(node, X1, lth, &h, &w);
	svgarcahead(node->aat, ahnum(node->UU.Uline.atype), &X1, soutline, h,
		    w, lth, fabs(node->UU.Uline.aradius),
		    node->UU.Uline.endpos.ypos);
      }
      X2 = arcend(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	endarc(node, X2, lth, &h, &w);
	svgarcahead(node->aat, ahnum(node->UU.Uline.atype), &X2, soutline, h,
		    w, lth, -fabs(node->UU.Uline.aradius),
		    node->UU.Uline.endpos.ypos);
      }
      printf("<path");
      svglineoptions(node, lsp);
      printf(" d=\"M ");
      svgwpos(X1);
      putchar('\n');
      svgwarc(X2, node->UU.Uline.aradius, node->UU.Uline.endpos.ypos,
	      node->UU.Uline.endpos.ypos);
      quote();
      svgendpath();
    }
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLspline:
    if (drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {  /* first spline */
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	lth = qenv(tn, XLlinethick, tn->lthick);
	X0 = node->aat;
	splineend = tn->UU.Uline.endpos;
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  svgahead(ahnum(node->UU.Uline.atype), &node->aat,
		   node->UU.Uline.endpos, soutline,
		   qenv(tn, XLarrowht, tn->UU.Uline.height),
		   qenv(tn, XLarrowwid, tn->UU.Uline.width), lth,
		   node->UU.Uline.lfill);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	  svgahead(ahnum(tn->UU.Uline.atype), &tn->UU.Uline.endpos, tn->aat,
		   soutline, qenv(tn, XLarrowht, tn->UU.Uline.height),
		   qenv(tn, XLarrowwid, tn->UU.Uline.width), lth,
		   node->UU.Uline.lfill);
	spltot = primdepth(node);
	splcount = spltot;
	svgfilloptions(node, lsp, false);
	printf(" d=\"M ");
	svgwpos(node->aat);
	printf("\n C ");
	if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	  svgwprop(X0, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	  space();
	  svgwprop(X0, node->UU.Uline.endpos, 2.0, 1.0, 3.0);
	  space();
	  svgwprop(X0, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	  space();
	  svgwprop(X0, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
	  putchar('\n');
	} else if (spltot > 1) {
	  svgwprop(X0, node->UU.Uline.endpos, 1 - node->UU.Uline.aradius,
		   node->UU.Uline.aradius, 1.0);
	  space();
	}
      } else if (splcount > 1 && node->UU.Uline.aradius == mdistmax) {
	svgwprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	space();
	svgwprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	space();
	svgwprop(node->aat, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
	putchar('\n');
      } else if (splcount > 1) {
	svgwprop(node->aat, node->UU.Uline.endpos,
		 0.5 + node->UU.Uline.aradius / 2,
		 0.5 - node->UU.Uline.aradius / 2, 1.0);
	space();
	svgwprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	space();
	svgwprop(node->aat, node->UU.Uline.endpos,
		 0.5 - node->UU.Uline.aradius / 2,
		 0.5 + node->UU.Uline.aradius / 2, 1.0);
	space();
      }
      if (splcount == 1) {
	if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	  svgwprop(node->aat, splineend, 5.0, 1.0, 6.0);
	  space();
	  svgwprop(node->aat, splineend, 1.0, 1.0, 2.0);
	  space();
	  svgwprop(node->aat, splineend, 1.0, 2.0, 3.0);
	  space();
	  svgwprop(node->aat, splineend, 1.0, 5.0, 6.0);
	  putchar('\n');
	} else if (spltot > 1) {
	  svgwprop(node->aat, splineend, node->UU.Uline.aradius,
		   1 - node->UU.Uline.aradius, 1.0);
	  space();
	}
	svgwpos(node->UU.Uline.endpos);
	quote();
	svgendpath();
      }
      node->ptype = XLline;
      /*D; if debuglevel > 0 then writeln(log,'node ',splcount:1) ;D*/
      splcount--;
    }
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	lth = qenv(tn, XLlinethick, tn->lthick);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  svgahead(ahnum(tn->UU.Uline.atype), &node->aat, node->UU.Uline.endpos,
		   soutline, qenv(tn, XLarrowht, tn->UU.Uline.height),
		   qenv(tn, XLarrowwid, tn->UU.Uline.width), lth,
		   node->UU.Uline.lfill);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	  svgahead(ahnum(tn->UU.Uline.atype), &tn->UU.Uline.endpos, tn->aat,
		   soutline, qenv(tn, XLarrowht, tn->UU.Uline.height),
		   qenv(tn, XLarrowwid, tn->UU.Uline.width), lth,
		   node->UU.Uline.lfill);
	if (node->son == NULL) {
	  svgfilloptions(tn, lsp, false);
	  svgcoord("x1", "y1", node->aat.xpos, node->aat.ypos);
	  svgcoord("x2", "y2", node->UU.Uline.endpos.xpos,
		   node->UU.Uline.endpos.ypos);
	  svgendpath();
	} else {
	  svgfilloptions(tn, lsp, true);
	  printf(" points=\"");
	  svgwpos(node->aat);
	  space();
	}
      } else if (node->son == NULL) {
	svgwpos(node->aat);
	putchar('\n');
	svgwpos(node->UU.Uline.endpos);
	quote();
	svgendpath();
      } else {
	svgwpos(node->aat);
	putchar('\n');
      }
    }
    svgwtext(node, node->textp,
	     0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	     0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLstring:
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
      wstring(&output, node->textp);
      putchar('\n');
    }
    break;
  }
  /*DUGHM ;flush(output) MHGUD*/
}  /* svgdraw */


/* include pst.h */
/* pst.x */
/* Output routines for PSTricks */
void pstprelude(double n, double s, double e, double w)
{
  printf("\\psset{unit=1in,cornersize=absolute,dimen=middle}%%\n");
  printf("\\begin{pspicture}");
  wcoord(&output, w, s);
  wcoord(&output, e, n);
  printf("%%\n");
  printf("%% %s option -p for PSTricks 0.93a or later\n", Version);
}


void pstpostlude(void)
{
  printf("\\end{pspicture}%%\n");
  /*D if debuglevel > 0 then writeln(log,'pstpostlude done'); D*/
}


void pstwrtext(primitive *np, nametype *tp, double x, double y)
{
  boolean A, B, L, R;

  if (tp == NULL)
    return;
  if (tp->next_ != NULL) {
    printf("\\rput");
    wcoord(&output, x, y);
    putchar('{');
    texstacktext(tp);
    printf("}\n");
    return;
  }
  checkjust(tp, &A, &B, &L, &R);
  if (A || B || L || R) {
    printf("\\uput{");
    wfloat(&output, venv(np, XLtextoffset) / scale * 72);
    printf("bp}[");
    if (B)
      putchar('d');
    else if (A)
      putchar('u');
    if (L)
      putchar('r');
    else if (R)
      putchar('l');
    putchar(']');
  } else
    printf("\\rput");
  wcoord(&output, x, y);
  putchar('{');
  wstring(&output, tp);
  printf("}\n");
}


void pstlineoptions(primitive *node, int lsp, Char sep)
{
  int TEMP;

  if (node->lthick >= 0.0) {
    printf("%clinewidth=", sep);
    wfloat(&output, node->lthick);
    printf("pt");
    sep = ',';
  }
  if (node->outlinep != NULL) {
    printf("%clinecolor=", sep);
    wstring(&output, node->outlinep);
    sep = ',';
  }
  if (node->ptype == XBLOCK)
    lsp = XLinvis;
  if (lsp == XLpath || lsp == XLinvis || lsp == XLdotted || lsp == XLdashed)
  {   /* XLsolid,*/
    printf("%clinestyle=", sep);
    switch (lsp) {

    /* XLsolid: write('solid'); */
    /* solid is the default */
    case XLdashed:
      printf("dashed");
      if (node->lparam >= 0) {
	printf(",dash=");
	wfloat(&output, node->lparam / fsc);
	putchar(' ');
	wfloat(&output, node->lparam / fsc);
      }
      break;

    case XLdotted:
      printf("dotted");
      if (node->lparam >= 0) {
	printf(",dotsep=");
	wfloat(&output, node->lparam / fsc);
      }
      break;

    case XLinvis:
    case XLpath:
      printf("none");
      break;
    }
    sep = ',';
  }
  TEMP = ahnum(node->UU.Uline.atype);
  if ((node->ptype == XLspline || node->ptype == XLline ||
       node->ptype == XLarrow) & (ahlex(node->UU.Uline.atype) != XEMPTY) &
      (!(TEMP >= 0 && TEMP < 32 && ((1L << TEMP) & 0x9) != 0))) {
    printf("%carrowsize=", sep);
    wfloat(&output, node->UU.Uline.width / scale);
    printf("in 0,arrowlength=");
    if (node->UU.Uline.width == 0.0)
      wfloat(&output, 0.0);
    else
      wfloat(&output, node->UU.Uline.height / node->UU.Uline.width);
    printf(",arrowinset=0");
    sep = ',';
  }
  if (sep == ',')
    putchar(']');
}


void pstfilloptions(primitive *node, int lsp, double a)
{
  Char sep;
  double fill;

  fill = -1.0;
  switch (node->ptype) {

  case XBLOCK:
    /* blank case */
    break;

  case XLbox:
    fill = node->UU.Ubox.boxfill;
    break;

  case XLcircle:
    fill = node->UU.Ucircle.cfill;
    break;

  case XLellipse:
    fill = node->UU.Uellipse.efill;
    break;

  case XLarc:
  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    fill = node->UU.Uline.lfill;
    break;
  }
  fill = (long)floor(fill * 1000000L + 0.5) / 1000000.0;
  if (fill > 0.0 && fill < 1.0 && fill != lastfillval && fill != 0.25 &&
      fill != 0.5 && fill != 0.75) {
    lastfillval = fill;
    printf("\\newgray{fillval}");
    wbrace(fill);
    putchar('\n');
  }
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    printf("\\psframe");
    break;

  case XLellipse:
    printf("\\psellipse");
    break;

  case XLcircle:
    printf("\\pscircle");
    break;

  case XLspline:
    printf("\\psbezier");
    break;

  case XLline:
  case XLarrow:
    printf("\\psline");
    break;

  case XLarc:
    if (a >= 0.0)
      printf("\\psarc");
    else
      printf("\\psarcn");
    break;
  }
  if (sshade != NULL) {
    printf("[fillstyle=solid,fillcolor=");
    wstring(&output, sshade);
    sep = ',';
  } else if (fill >= 0.0 && fill <= 1.0) {
    printf("[fillstyle=solid,fillcolor=");
    if (fill == 0.0)
      printf("black");
    else if (fill == 0.25)
      printf("darkgray");
    else if (fill == 0.5)
      printf("gray");
    else if (fill == 0.75)
      printf("lightgray");
    else if (fill == 1.0)
      printf("white");
    else
      printf("fillval");
    sep = ',';
  } else
    sep = '[';
  if (node->ptype == XLbox) {
    if (node->UU.Ubox.boxradius != 0.0) {
      printf("%clinearc=", sep);
      wfloat(&output, node->UU.Ubox.boxradius / fsc);
      sep = ',';
    }
  }
  pstlineoptions(node, lsp, sep);
}


void pstahead(postype *point, postype shaft, double ht, double wid,
		     double lth, int typ, nametype *sou)
{
  postype P, L, R, Px, Lx, Rx, Q;
  double x, y;

  dahead(*point, shaft, ht, wid, lth / 72 * scale, &P, &L, &R, &Px, &Lx, &Rx,
	 &Q, &x, &y);
  /*D if debuglevel > 0 then begin
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
      writeln(log) end; D*/
  if (typ == 0) {
    printf("\\pscustom[linewidth=");
    wfloat(&output, lth);
    printf("pt,linecolor=");
    if (sou != NULL)
      wstring(&output, sou);
    else
      printf("black");
    printf("]{%%\n");
    printf("\\psline");
    wprop(P, L, x - y, y, x);
    wpos(P);
    wprop(P, R, x - y, y, x);
    printf("}\n");
  } else {
    printf("\\pscustom[linewidth=0pt,fillstyle=solid,fillcolor=");
    if (sou != NULL)
      wstring(&output, sou);
    else
      printf("black");
    printf(",linecolor=");
    if (sou != NULL)
      wstring(&output, sou);
    else
      printf("black");
    printf("]{%%\n");
    printf("\\psline");
    wpos(Px);
    wpos(Rx);
    wpos(*point);
    wpos(Lx);
    printf("\\closepath}%%\n");
  }
  *point = P;
}


void pstwarc(postype C, double r, double startangle, double endangle,
		    double ccw)
{
  wpos(C);
  if (ccw > 0 && endangle < startangle)
    endangle += 2 * pi;
  else if (ccw < 0 && endangle > startangle)
    endangle -= 2 * pi;
  wbrace(r / fsc);
  wbrace(180.0 / pi * startangle);
  wbrace(180.0 / pi * endangle);
  /*D; if debuglevel > 0 then begin
     write(log,' pstwarc:');
     logpos(' C',C);
     write(log,' r='); wfloat(log,r);
     write(log,' startangle='); wfloat(log,startangle*180/pi);
     write(log,' endangle='); wfloat(log,endangle*180/pi);
     write(log,' ccw='); wfloat(log,ccw);
     writeln(log)
     end D*/
  putchar('\n');
}


void pstarc(double ccw)
{
  if (ccw >= 0.0)
    printf("\\psarc");
  else
    printf("\\psarcn");
}


void pstarcahead(postype C, postype point, int atyp, nametype *sou,
			double ht, double wid, double lth, double radius,
			double arcangle, postype *P)
{
  postype Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  /*D; if debuglevel > 0 then writeln(log,' pstarcahead:'); D*/
  arcahead(C, point, atyp, ht, wid, lth, radius, arcangle, P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  printf("\\pscustom[linewidth=0pt,fillstyle=solid,fillcolor=");
  if (sou != NULL)
    wstring(&output, sou);
  else
    printf("black");
  printf(",linecolor=");
  if (sou != NULL)
    wstring(&output, sou);
  else
    printf("black");
  printf("]{%%\n");
  pstarc(-ccw);
  pstwarc(Ci, radius, posangle(Ai, Ci), posangle(point, Ci), -ccw);
  pstarc(ccw);
  pstwarc(Co, radius, posangle(point, Co), posangle(Ao, Co), ccw);
  if (atyp == 0 && lwi < (wid - lwi) / 2) {
    pstarc(-ccw);
    pstwarc(Co, radius - lwi, posangle(Ao, Co), posangle(*P, Co), -ccw);
    pstarc(ccw);
    pstwarc(Ci, radius + lwi, posangle(*P, Ci), posangle(Ai, Ci), ccw);
  } else if (atyp == 3 && lwi < (wid - lwi) / 2) {
    pstarc(-ccw);
    pstwarc(Cox, radius, posangle(Ao, Cox), posangle(Px, Cox), -ccw);
    pstarc(ccw);
    pstwarc(Cix, radius, posangle(Px, Cix), posangle(Ai, Cix), ccw);
  }
  printf("\\closepath}%%\n");
  /*D; if debuglevel > 0 then begin
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
     end D*/
}


void pstdraw(primitive *node)
{
  int lsp;
  postype X0, X1;
  boolean v;
  double lth;
  primitive *tn;
  int TEMP, TEMP1;

  getlinespec(node, &lsp, &tn);
  sshade = node->shadedp;
  lth = qenv(node, XLlinethick, tn->lthick);   /* printobject(node); */
  /*D if debuglevel > 0 then begin
     write(log,'pstdraw[',ordp(node):1,']: ');
     snaptype(log,ptype); D*/
  /*D
           writeln(log) end;
        if linesignal > 0 then begin write(errout,'pstdraw: ');
           snaptype(errout,ptype); writeln(errout) end D*/
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    initnesw();
    nesw(node);
    if (node->ptype == XBLOCK)
      v = drawn(node, lsp, -1.0);
    else
      v = drawn(node, lsp, node->UU.Ubox.boxfill);
    if (v) {
      pstfilloptions(node, lsp, 0.0);
      wcoord(&output, west, south);
      wcoord(&output, east, north);
      putchar('\n');
    }
    pstwrtext(node, node->textp, 0.5 * (east + west), 0.5 * (north + south));
    break;

  case XLspline:
    if (drawn(node, lsp, node->UU.Uline.lfill)) {
      if (!isthen(node)) {  /* first spline */
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	if (sfill && vfill >= 0.0)
	  node->UU.Uline.lfill = vfill;
	TEMP = ahnum(tn->UU.Uline.atype);
	if (TEMP >= 0 && TEMP < 32 && ((1L << TEMP) & 0x9) != 0) {
	  TEMP1 = ahlex(tn->UU.Uline.atype);
	  if (TEMP1 == XDOUBLEHEAD || TEMP1 == XRIGHTHEAD)
	    pstahead(&tn->UU.Uline.endpos, tn->aat,
		     qenv(node, XLarrowht, node->UU.Uline.height),
		     qenv(node, XLarrowwid, node->UU.Uline.width),
		     qenv(node, XLlinethick, tn->lthick),
		     ahnum(tn->UU.Uline.atype), soutline);
	  TEMP1 = ahlex(tn->UU.Uline.atype);
	  if (TEMP1 == XDOUBLEHEAD || TEMP1 == XLEFTHEAD)
	    pstahead(&node->aat, node->UU.Uline.endpos,
		     qenv(node, XLarrowht, node->UU.Uline.height),
		     qenv(node, XLarrowwid, node->UU.Uline.width),
		     qenv(node, XLlinethick, tn->lthick),
		     ahnum(tn->UU.Uline.atype), soutline);
	}
	pstfilloptions(tn, lsp, 0.0);
	spltot = primdepth(node);
	splcount = spltot;
	TEMP = ahnum(tn->UU.Uline.atype);
	if (!(TEMP >= 0 && TEMP < 32 && ((1L << TEMP) & 0x9) != 0)) {
	  switch (ahlex(tn->UU.Uline.atype)) {

	  case XLEFTHEAD:
	    printf("{<-}");
	    break;

	  case XDOUBLEHEAD:
	    printf("{<->}");
	    break;

	  case XRIGHTHEAD:
	    printf("{->}");
	    break;
	  }
	}
	sfill = false;
	wpos(node->aat);
	putchar('\n');
      }
      if (node->UU.Uline.aradius == mdistmax) {
	if (splcount == spltot && splcount > 1) {  /* 1st seg */
	  wprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	      /* 1/6 from 1st to 2nd */
	  wprop(node->aat, node->UU.Uline.endpos, 4.0, 2.0, 6.0);
	      /* 1/3 from 1st to 2nd */
	  pprop(node->UU.Uline.endpos, &node->aat, 1.0, 1.0, 2.0);
	      /* set aat to halfway */
	  wpos(node->aat);
	  putchar('\n');
	}
	if (splcount > 1) {  /* interior segment */
	  wprop(node->aat, node->UU.Uline.endpos, 1.0, 2.0, 3.0);
	      /* 2/3 from 1st to 2nd */
	  pprop(node->son->UU.Uline.endpos, &node->son->aat, 1.0, 1.0, 2.0);
	      /* shift son^.aat */
	  wprop(node->UU.Uline.endpos, node->son->aat, 2.0, 1.0, 3.0);
	      /* 1/3 from endpos */
	  wpos(node->son->aat);
	} else {  /* last segment */
	  wprop(node->aat, node->UU.Uline.endpos, 2.0, 1.0, 3.0);
	      /* 1/3 along */
	  wprop(node->aat, node->UU.Uline.endpos, 1.0, 2.0, 3.0);
	      /* 2/3 along */
	  wpos(node->UU.Uline.endpos);
	}
      } else if (splcount > 1) {
	wprop(node->aat, node->UU.Uline.endpos, 1 - node->UU.Uline.aradius,
	      node->UU.Uline.aradius, 1.0);
	if (splcount > 2)
	  pprop(node->son->UU.Uline.endpos, &node->son->aat, 1.0, 1.0, 2.0);
	else
	  node->son->aat = node->son->UU.Uline.endpos;
	wprop(node->UU.Uline.endpos, node->son->aat, node->UU.Uline.aradius,
	      1 - node->UU.Uline.aradius, 1.0);
	wpos(node->son->aat);
      } else if (spltot == 1) {
	wprop(node->aat, node->UU.Uline.endpos, node->UU.Uline.aradius,
	      1 - node->UU.Uline.aradius, 1.0);
	wprop(node->aat, node->UU.Uline.endpos, 1 - node->UU.Uline.aradius,
	      node->UU.Uline.aradius, 1.0);
	wpos(node->UU.Uline.endpos);
      }
      node->ptype = XLline;
      if (node->UU.Uline.aradius == mdistmax || splcount > 1)
	putchar('\n');
      splcount--;
    }
    break;

  case XLline:
  case XLarrow:
    if (drawn(node, lsp, node->UU.Uline.lfill)) {
      if (!isthen(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	if (sfill && vfill >= 0.0)
	  node->UU.Uline.lfill = vfill;
	TEMP = ahnum(tn->UU.Uline.atype);
	if (TEMP >= 0 && TEMP < 32 && ((1L << TEMP) & 0x9) != 0) {
	  TEMP1 = ahlex(tn->UU.Uline.atype);
	  if (TEMP1 == XDOUBLEHEAD || TEMP1 == XLEFTHEAD)
	    pstahead(&node->aat, node->UU.Uline.endpos,
		     qenv(node, XLarrowht, node->UU.Uline.height),
		     qenv(node, XLarrowwid, node->UU.Uline.width),
		     qenv(node, XLlinethick, tn->lthick),
		     ahnum(tn->UU.Uline.atype), soutline);
	  TEMP1 = ahlex(tn->UU.Uline.atype);
	  if (TEMP1 == XDOUBLEHEAD || TEMP1 == XRIGHTHEAD)
	    pstahead(&tn->UU.Uline.endpos, tn->aat,
		     qenv(node, XLarrowht, node->UU.Uline.height),
		     qenv(node, XLarrowwid, node->UU.Uline.width),
		     qenv(node, XLlinethick, tn->lthick),
		     ahnum(tn->UU.Uline.atype), soutline);
	}
	pstfilloptions(tn, lsp, 0.0);
	TEMP = ahnum(tn->UU.Uline.atype);
	if (!(TEMP >= 0 && TEMP < 32 && ((1L << TEMP) & 0x9) != 0)) {
	  switch (ahlex(tn->UU.Uline.atype)) {

	  case XLEFTHEAD:
	    printf("{<-}");
	    break;

	  case XDOUBLEHEAD:
	    printf("{<->}");
	    break;

	  case XRIGHTHEAD:
	    printf("{->}");
	    break;
	  }
	}
	sfill = false;
	wpos(node->aat);
      }
      /* wpos( aat ); */
      wpos(node->UU.Uline.endpos);
      putchar('\n');
    }
    pstwrtext(node, node->textp,
	      0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLmove:
    pstwrtext(node, node->textp,
	      0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLellipse:
    if (drawn(node, lsp, node->UU.Uellipse.efill)) {
      pstfilloptions(node, lsp, 0.0);
      wpos(node->aat);
      wcoord(&output, 0.5 * node->UU.Uellipse.elwidth,
	     0.5 * node->UU.Uellipse.elheight);
      putchar('\n');
    }
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->UU.Ucircle.cfill)) {
      pstfilloptions(node, lsp, 0.0);
      wpos(node->aat);
      putchar('{');
      wfloat(&output, node->UU.Ucircle.radius / fsc);
      printf("}\n");
    }
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    if (drawn(node, lsp, node->UU.Uline.lfill)) {
      getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
      if (sfill && vfill >= 0.0)
	node->UU.Uline.lfill = vfill;
      X0 = arcstart(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	pstarcahead(node->aat, X0, ahnum(node->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, node->UU.Uline.height),
		    qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		    fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		    &X0);
      X1 = arcend(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	pstarcahead(node->aat, X1, ahnum(node->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, node->UU.Uline.height),
		    qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		    -fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		    &X1);
      pstfilloptions(node, lsp, node->UU.Uline.endpos.ypos);
      pstwarc(node->aat, fabs(node->UU.Uline.aradius), posangle(X0, node->aat),
	      posangle(X1, node->aat), node->UU.Uline.endpos.ypos);
    }
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLstring:
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
      wstring(&output, node->textp);
      putchar('\n');
    } else if (node->lthick >= 0.0) {
      printf("\\psset{linewidth=");
      wfloat(&output, node->lthick);
      printf("pt}%%\n");
    }
    break;
  }
}  /* pstdraw */


/* include mfp.h */
/* mfp.x */
/* Output routines for mfpic */
void mfpprelude(double n, double s, double e, double w)
{
  printf("\\begin{mfpic}[72]");
  wbrace(w / fsc);
  wbrace(e / fsc);
  wbrace(s / fsc);
  wbrace(n / fsc);
  printf("\n%% %s option -m for mfpic\n", Version);
  printf("\\dashlen=4bp\\dashspace=4bp\\dotspace=3bp\\pen{0.8bp}\n");
  printf("\\def\\mfpdefaultcolor{black}\\drawcolor{\\mfpdefaultcolor}\n");
  lastthick = 0.8;
}


void mfppostlude(void)
{
  printf("\\end{mfpic}\n");
  /*D if debuglevel > 0 then writeln(log,'mfpbpostlude done');D*/
}


void mfpwrtext(primitive *np, nametype *tp, double x, double y)
{
  boolean A, B, L, R;

  if (tp == NULL)
    return;
  if (tp->next_ != NULL) {
    printf("\\tlabel[cc]");
    wcoord(&output, x, y);
    putchar('{');
    texstacktext(tp);
    printf("}\n");
    return;
  }
  printf("\\tlabel[");
  checkjust(tp, &A, &B, &L, &R);
  if (A)
    putchar('b');
  else if (B)
    putchar('t');
  else
    putchar('c');
  if (L)
    putchar('l');
  else if (R)
    putchar('r');
  else
    putchar('c');
  putchar(']');
  wcoord(&output, x, y);
  putchar('{');
  if (A) {
    printf("\\raisebox{");
    wfloat(&output,
	   venv(np, XLtextoffset) / scale / 12 + 0.5 * venv(np, XLtextht));
    printf("ex}{");
  } else if (B) {
    printf("\\raisebox{");
    wfloat(&output,
	   venv(np, XLtextoffset) / scale / -12 - 0.5 * venv(np, XLtextht));
    printf("ex}{");
  }
  wstring(&output, tp);
  if (A || B)
    putchar('}');
  printf("}\n");
}


void comma(void)
{
  putchar(',');
}


void commacr(void)
{
  printf(",\n");
}


void mfpsetshade(double fill, nametype *shade)
{
  if (shade != NULL) {
    printf("\\gfill[");
    wstring(&output, shade);
    putchar(']');
    return;
  }
  if (fill == 0.0) {
    printf("\\gfill");
    return;
  }
  if (fill == 1)
    return;
  if (fill <= 0 || fill >= 1)
    return;
  printf("\\shadespace=");
  wfloat(&output, exp((fill - 0.3) * 2));
  printf("bp\\shade");
}


void mfpellipse(postype aat, double elwidth, double elheight)
{
  printf("\\ellipse{");
  wpos(aat);
  comma();
  wfloat(&output, 0.5 * elwidth / fsc);
  comma();
  wfloat(&output, 0.5 * elheight / fsc);
  printf("}\n");
}


void mfpcircle(postype aat, double radius)
{
  printf("\\circle{");
  wpos(aat);
  comma();
  wfloat(&output, radius / fsc);
  printf("}\n");
}


void mfsetdash(double dash)
{
  if (dash < 0.0)
    return;
  printf("\\dashlen=");
  wfloat(&output, dash);
  printf("in");
  printf("\\dashspace=");
  wfloat(&output, dash);
  printf("in");
}


void mfsetthick(double lt)
{
  if (lt < 0.0 || lt == lastthick)
    return;
  printf("\\pen{");
  wfloat(&output, lt);
  printf("bp}");
  lastthick = lt;
}


void mfpdashdot(int lsp, double param)
{
  if (lsp == XLdashed) {
    mfsetdash(param / fsc);
    printf("\\dashed");
    return;
  }
  if (lsp != XLdotted)
    return;
  if (param >= 0.0) {
    printf("\\dotspace=");
    wfloat(&output, param / fsc);
    printf("in");
  }
  printf("\\dotted");
}


void mfplineopts(double lt, double param, int lsp, nametype *sou)
{
  if (sou != NULL) {
    printf("\\drawcolor{");
    wstring(&output, sou);
    printf("}\n");
  }
  mfsetthick(lt);
  mfpdashdot(lsp, param);
}


void mfpahead(int atyp, postype *point, postype shaft, double ht,
		     double wid, double lth, nametype *sou)
{
  postype P, L, R, Px, Lx, Rx, Q;
  double lwu, x, y, cs, ss, d;

  lwu = lth / 72 * scale;
  dahead(*point, shaft, ht, wid, lwu, &P, &L, &R, &Px, &Lx, &Rx, &Q, &x, &y);
  mfsetthick(0.0);
  printf("\\draw\\gfill");
  if (sou != NULL) {
    putchar('[');
    wstring(&output, sou);
    putchar(']');
  }
  printf("\\polygon{");
  d = sqrt(ht * ht + wid * wid / 4);
  if (d == 0) {
    cs = 1.0;
    ss = 0.0;
  } else {
    cs = ht / d;
    ss = wid / 2 / d;
  }
  /*D if debuglevel > 0 then begin
     writeln(log,' mfpahead:');
     end; D*/
  wpos(Rx);
  comma();
  wpos(*point);
  comma();
  wpos(Lx);
  if (atyp == 0 && lwu < (wid - lwu) / 2) {
    comma();
    putchar('\n');
    wpos(affine(ss * lwu, cs * lwu, Lx, Q));
    comma();
    wprop(*point, P, -1.0, 2.0, 1.0);
    comma();
    wpos(affine(ss * lwu, -cs * lwu, Rx, Q));
  } else if (atyp == 3 && lwu < (wid - lwu) / 2) {
    comma();
    putchar('\n');
    wpos(Px);
  }
  printf("}\n");
  *point = P;
}


void mfpwarc(postype Ctr, double radius, double startangle,
		    double endangle, double ccw)
{
  printf("\\arc[p]{");
  wpos(Ctr);
  comma();
  if (ccw > 0 && endangle < startangle)
    endangle += 2 * pi;
  else if (ccw < 0 && endangle > startangle)
    endangle -= 2 * pi;
  wfloat(&output, 180.0 / pi * startangle);
  comma();
  wfloat(&output, 180.0 / pi * endangle);
  comma();
  wfloat(&output, fabs(radius) / fsc);
  printf("}\n");
}


void mfparcahead(postype C, postype point, int atyp, nametype *sou,
			double ht, double wid, double lth, double radius,
			double arcangle, postype *P)
{
  postype Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  /*D; if debuglevel > 0 then writeln(log,' mfparcahead:'); D*/
  arcahead(C, point, atyp, ht, wid, lth, radius, arcangle, P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  mfsetthick(0.0);
  printf("\\draw\\gfill");
  if (sou != NULL) {
    putchar('[');
    wstring(&output, sou);
    putchar(']');
  }
  printf("\\lclosed\\begin{connect}\n");
  mfpwarc(Ci, radius, posangle(Ai, Ci), posangle(point, Ci), -ccw);
  mfpwarc(Co, radius, posangle(point, Co), posangle(Ao, Co), ccw);
  if (atyp == 0 && lwi < (wid - lwi) / 2) {
    mfpwarc(Co, radius - lwi, posangle(Ao, Co), posangle(*P, Co), -ccw);
    mfpwarc(Ci, radius + lwi, posangle(*P, Ci), posangle(Ai, Ci), ccw);
  } else if (atyp == 3 && lwi < (wid - lwi) / 2) {
    mfpwarc(Cox, radius, posangle(Ao, Cox), posangle(Px, Cox), -ccw);
    mfpwarc(Cix, radius, posangle(Px, Cix), posangle(Ai, Cix), ccw);
  }
  printf("\\end{connect}\n");
  /*D; if debuglevel > 0 then begin
     write(log,' <point '); wfloat(log,posangle(point,C)*180/pi);
     write(log,' <P '); wfloat(log,posangle(P,C)*180/pi);
     write(log,' <Ao '); wfloat(log,posangle(Ao,C)*180/pi);
     write(log,' <Ai '); wfloat(log,posangle(Ai,C)*180/pi);
     write(log,' <arcangle '); wfloat(log,arcangle*180/pi);
     writeln(log)
     end D*/
}


void mfpdraw(primitive *node)
{
  /* node is always <> nil */
  int i, lsp;
  postype tmpos, X0, X1;
  primitive *tn;
  double x, y, t, r, lth;
  int TEMP;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);
  /*D if debuglevel > 0 then begin write(log,'mfpdraw: ');
        snaptype(log,node^.ptype);
        write(log,' lth='); wfloat(log,lth); writeln(log) end; D*/
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    initnesw();
    nesw(node);
    if (node->ptype == XLbox) {
      if (node->UU.Ubox.boxfill >= 0.0 && node->UU.Ubox.boxfill <= 1.0 ||
	  node->shadedp != NULL) {
	mfpsetshade(node->UU.Ubox.boxfill, node->shadedp);
	printf("\\rect{");
	wcoord(&output, west, south);
	comma();
	wcoord(&output, east, north);
	printf("}\n");
      }
      if (drawn(node, lsp, -1.0)) {
	mfplineopts(lth, node->lparam, lsp, node->outlinep);
	printf("\\rect{");
	wcoord(&output, west, south);
	comma();
	wcoord(&output, east, north);
	printf("}\n");
	if (node->outlinep != NULL)
	  printf("\\drawcolor{\\mfpdefaultcolor}\n");
      }
    }
    mfpwrtext(node, node->textp, 0.5 * (east + west), 0.5 * (north + south));
    break;

  case XLellipse:
    if (node->UU.Uellipse.efill >= 0.0 && node->UU.Uellipse.efill <= 1.0 ||
	node->shadedp != NULL) {
      mfpsetshade(node->UU.Uellipse.efill, node->shadedp);
      mfpellipse(node->aat, node->UU.Uellipse.elwidth, node->UU.Uellipse.elheight);
    }
    if (drawn(node, lsp, -1.0)) {
      mfplineopts(lth, node->lparam, lsp, node->outlinep);
      mfpellipse(node->aat, node->UU.Uellipse.elwidth, node->UU.Uellipse.elheight);
      if (node->outlinep != NULL)
	printf("\\drawcolor{\\mfpdefaultcolor}\n");
    }
    mfpwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (node->UU.Ucircle.cfill >= 0.0 && node->UU.Ucircle.cfill <= 1.0 ||
	node->shadedp != NULL) {
      mfpsetshade(node->UU.Ucircle.cfill, node->shadedp);
      mfpcircle(node->aat, node->UU.Ucircle.radius);
    }
    if (drawn(node, lsp, -1.0)) {
      mfplineopts(lth, node->lparam, lsp, node->outlinep);
      mfpcircle(node->aat, node->UU.Ucircle.radius);
      if (node->outlinep != NULL)
	printf("\\drawcolor{\\mfpdefaultcolor}\n");
    }
    mfpwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLspline:
    if (drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {  /* first spline */
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	spltot = primdepth(node);
	splcount = spltot;
	lth = qenv(node, XLlinethick, tn->lthick);
	X0 = node->aat;
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  mfpahead(ahnum(tn->UU.Uline.atype), &node->aat, node->UU.Uline.endpos,
		   qenv(node, XLarrowht, tn->UU.Uline.height),
		   qenv(node, XLarrowwid, tn->UU.Uline.width), lth, soutline);
	mfplineopts(lth, node->lparam, lsp, node->outlinep);
	if (sfill) {
	  mfpsetshade(vfill, sshade);
	  printf("\\lclosed");
	  if (lsp != XLinvis)
	    printf("\\draw");
	  putchar('\n');
	}
	printf("\\curve{");
	wpos(node->aat);
	comma();
	node->aat = X0;
	if (node->UU.Uline.aradius == mdistmax) {  /* spline x ie tensioned */
	  wprop(node->aat, node->UU.Uline.endpos, 3.0, 1.0, 4.0);
	  comma();
	  pprop(node->UU.Uline.endpos, &node->aat, 1.0, 1.0, 2.0);
	  wpos(node->aat);
	  commacr();
	}
      }
      node->ptype = XLline;   /* Do we need this anymore? */
      if (splcount > 1) {
	lth = qenv(node, XLlinethick, tn->lthick);
	if (node->UU.Uline.aradius == mdistmax || splcount > 2)
	  pprop(node->son->UU.Uline.endpos, &node->son->aat, 1.0, 1.0, 2.0);
	else {
	  X1 = node->son->aat;
	  X0 = node->son->UU.Uline.endpos;
	  TEMP = ahlex(node->UU.Uline.atype);
	  if ((ahnum(node->UU.Uline.atype) == 0) & (TEMP == XDOUBLEHEAD ||
						   TEMP == XRIGHTHEAD)) {
	    x = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		       node->UU.Uline.endpos.ypos - node->aat.ypos);
	    y = ahoffset(qenv(node, XLarrowht, tn->UU.Uline.height),
			 qenv(node, XLarrowwid, tn->UU.Uline.width),
			 lth / 72 * scale);
	    if (x > 0.0)
	      pprop(X1, &X0, y, x - y, x);
	  }
	  node->son->aat = node->son->UU.Uline.endpos;
	}
	if (node->UU.Uline.aradius == mdistmax)
	  r = 2.0 / 3;
	else
	  r = node->UU.Uline.aradius;
	for (i = 1; i <= 3; i++) {
	  t = i / 4.0;
	  tmpos = node->aat;
	  pprop(node->UU.Uline.endpos, &tmpos, 3 * r * t * (1 - t),
		(1 - t) * (1 - t) * (1 - t + 3 * t * (1 - r)), 1.0);
	  wprop(node->son->aat, tmpos, t * t * (t + 3 * (1 - t) * (1 - r)),
		1.0, 1.0);
	  comma();
	}
	if (node->UU.Uline.aradius == mdistmax || splcount > 2) {
	  wpos(node->son->aat);
	  commacr();
	} else {
	  TEMP = ahlex(node->UU.Uline.atype);
	  if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	    wpos(X0);
	    printf("}\n");
	    mfpahead(ahnum(node->UU.Uline.atype), &node->son->aat, X1,
		     qenv(node, XLarrowht, node->UU.Uline.height),
		     qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		     soutline);
	  } else {
	    wpos(node->son->aat);
	    printf("}\n");
	  }
	}
      } else if (node->UU.Uline.aradius == mdistmax) {
	wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	comma();
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	  X0 = node->UU.Uline.endpos;
	  x = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		     node->UU.Uline.endpos.ypos - node->aat.ypos);
	  y = ahoffset(qenv(node, XLarrowht, node->UU.Uline.height),
		       qenv(node, XLarrowwid, node->UU.Uline.width),
		       lth / 72 * scale);
	  if (x > 0.0)
	    pprop(node->aat, &node->UU.Uline.endpos, y, x - y, x);
	  wpos(node->UU.Uline.endpos);
	  printf("}\n");
	  mfpahead(ahnum(node->UU.Uline.atype), &X0, node->aat,
		   qenv(node, XLarrowht, node->UU.Uline.height),
		   qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		   soutline);
	} else {
	  wpos(node->UU.Uline.endpos);
	  printf("}\n");
	  if (sfill)
	    printf("\\end{connect}\n");
	  if (soutline != NULL)
	    printf("\\drawcolor{\\mfpdefaultcolor}\n");
	}
      }
      splcount--;
    }
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	if (sfill && vfill >= 0.0)
	  node->UU.Uline.lfill = vfill;
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  mfpahead(ahnum(node->UU.Uline.atype), &node->aat,
		   node->UU.Uline.endpos,
		   qenv(node, XLarrowht, tn->UU.Uline.height),
		   qenv(node, XLarrowwid, tn->UU.Uline.width), lth, soutline);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	  mfpahead(ahnum(tn->UU.Uline.atype), &tn->UU.Uline.endpos, tn->aat,
		   qenv(node, XLarrowht, tn->UU.Uline.height),
		   qenv(node, XLarrowwid, tn->UU.Uline.width), lth, soutline);
	mfplineopts(lth, node->lparam, lsp, node->outlinep);
	if (sfill) {
	  mfpsetshade(vfill, sshade);
	  printf("\\lclosed");
	  if (lsp != XLinvis)
	    printf("\\draw");
	  putchar('\n');
	}
	printf("\\polyline{");
	wpos(node->aat);
	comma();
	wpos(node->UU.Uline.endpos);
      } else {
	commacr();
	wpos(node->UU.Uline.endpos);
      }
      if (node->son == NULL) {
	printf("}\n");
	if (soutline != NULL)
	  printf("\\drawcolor{\\mfpdefaultcolor}\n");
      }
    }
    mfpwrtext(node, node->textp,
	      0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLarc:
    if (drawn(node, lsp, -1.0)) {
      getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
      X0 = arcstart(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	mfparcahead(node->aat, X0, ahnum(node->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, node->UU.Uline.height),
		    qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		    fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		    &X0);
      X1 = arcend(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	mfparcahead(node->aat, X1, ahnum(node->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, node->UU.Uline.height),
		    qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		    -fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		    &X1);
      /*D if debuglevel > 0 then begin
          write(log,' XLarc: lth='); wfloat(log,lth); writeln(log) end;D*/
      mfplineopts(lth, node->lparam, lsp, soutline);
      if (sfill) {
	mfpsetshade(vfill, sshade);
	printf("\\lclosed");
	if (lsp != XLinvis)
	  printf("\\draw");
	putchar('\n');
      }
      mfpwarc(node->aat, node->UU.Uline.aradius, posangle(X0, node->aat),
	      posangle(X1, node->aat), node->UU.Uline.endpos.ypos);
      if (soutline != NULL)
	printf("\\drawcolor{\\mfpdefaultcolor}\n");
    }
    mfpwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLstring:
    mfpwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
      wstring(&output, node->textp);
      putchar('\n');
    } else if (lth >= 0.0)
      mfsetthick(lth);
    else if (node->lparam >= 0.0) {
      mfsetdash(node->lparam);
      putchar('\n');
    }
    break;
  }
}  /* mfpdraw */


/* include mpo.h */
/* mpo.x */
/* Output routines for MetaPost */
void mpoprelude(void)
{
  /* n,s,e,w: real */
  printstate++;
  printf("%% %s option -s for MetaPost\n", Version);
  printf("beginfig(%d)\n", printstate);
  printf("def lcbutt=linecap:=butt enddef;\n");
  printf("def lcsq=linecap:=squared enddef;\n");
  printf("def lcrnd=linecap:=rounded enddef;\n");
  printf("def ljmit=linejoin:=mitered enddef;\n");
  printf("def lcolr= enddef; def drw=draw enddef; def X=;enddef;\n");
  printf("lcbutt; ljmit;\n");
}


void mpopostlude(void)
{
  printf("endfig;\n");
  /*D; if debuglevel > 0 then writeln(log,'mpobpostlude done');D*/
}


void mpowrtext(nametype *tp, double x, double y)
{
  boolean A, B, L, R;

  if (tp == NULL)
    return;
  if (tp->next_ != NULL) {
    printf("label");
    printf("(btex ");
    texstacktext(tp);
    printf(" etex,");
    wcoord(&output, x, y);
    printf(") X\n");
    return;
  }
  printf("label");
  checkjust(tp, &A, &B, &L, &R);
  if (A && L)
    printf(".urt");
  else if (A && R)
    printf(".ulft");
  else if (B && L)
    printf(".lrt");
  else if (B && R)
    printf(".llft");
  else if (A)
    printf(".top");
  else if (B)
    printf(".bot");
  else if (L)
    printf(".rt");
  else if (R)
    printf(".lft");
  printf("(btex ");
  wstring(&output, tp);
  printf(" etex,");
  wcoord(&output, x, y);
  printf(") X\n");
}


void mposetthick(double lthick)
{
  if (lthick == mdistmax) {
    if (lastthick != mdistmax)
      lthick = lastthick;
    else
      lthick = 0.8;
  }
  if (lthick < 0.0 || lthick == lastthick)
    return;
  /*D; if debuglevel > 0 then begin writeln(log,' mposetthick:');
     write(log,' lthick='); wfloat(log,lthick);
     write(log,' lastthick='); wfloat(log,lastthick);
     writeln(log)
     end D*/
  printf("pickup pencircle scaled ");
  wfloat(&output, lthick);
  printf("bp;\n");   /* Use Adobe points to conform to PSTricks */
  lastthick = lthick;
}


void mpolinecap(int lsp)
{
  switch (lsp) {

  case XLdashed:
    printf("lcsq;\n");
    break;

  case XLdotted:
    printf("lcrnd;\n");
    break;
  }
}


void addcolor(nametype *sp, double fillv)
{
  printf(" withcolor ");
  if (sp != NULL)
    wstring(&output, sp);
  else {
    wfloat(&output, fillv);
    printf("white");
  }
}


void mpodashdot(int lsp, double param, nametype *op)
{
  if (lsp == XLdashed) {
    printf(" dashed evenly");
    if (param > 0) {
      printf(" scaled ");
      wfloat(&output, param / fsc / 3);
    }
    if (op != NULL)
      addcolor(op, -1.0);
    printf(" X lcbutt;\n");
    return;
  }
  if (lsp == XLdotted) {
    printf(" dashed withdots");
    if (param > 0) {
      printf(" scaled ");
      wfloat(&output, param / 5);
    }
    if (op != NULL)
      addcolor(op, -1.0);
    printf(" X lcbutt;\n");
    return;
  }
  if (op != NULL) {
    addcolor(op, -1.0);
    printf(" X\n");
  } else
    printf(" X\n");
}


#define b               ((sqrt(2.0) - 1.0) * 4.0 / 3.0)


void mpocorner(postype ctr, double x, double y, double r, double ct,
		      double st)
{
  wcoord(&output, ctr.xpos + x + ct * r, ctr.ypos + y + st * r);
  controls();
  wcoord(&output, ctr.xpos + x + ct * r - st * r * b,
	 ctr.ypos + y + st * r + ct * r * b);
  wrand();
  wcoord(&output, ctr.xpos + x + ct * r * b - st * r,
	 ctr.ypos + y + st * r * b + ct * r);
  ddot();
  wcoord(&output, ctr.xpos + x - st * r, ctr.ypos + y + ct * r);
  ddash();
}

#undef b


void mpobox(Char *initial, postype ctr, double halfwid, double halfht,
		   double rad)
{
  double r, h, v;

  fputs(initial, stdout);
  if (rad == 0.0) {
    wcoord(&output, ctr.xpos - halfwid, ctr.ypos + halfht);
    ddash();
    wcoord(&output, ctr.xpos + halfwid, ctr.ypos + halfht);
    ddash();
    wcoord(&output, ctr.xpos + halfwid, ctr.ypos - halfht);
    ddash();
    wcoord(&output, ctr.xpos - halfwid, ctr.ypos - halfht);
    ddash();
  } else {
    r = Min(Min(rad, halfht), halfwid);
    h = halfwid - r;
    v = halfht - r;
    mpocorner(ctr, h, v, r, 1.0, 0.0);
    mpocorner(ctr, -h, v, r, 0.0, 1.0);
    mpocorner(ctr, -h, -v, r, -1.0, 0.0);
    mpocorner(ctr, h, -v, r, 0.0, -1.0);
  }
  printf("cycle");
}


#define r               ((sqrt(2.0) - 1) * 4 / 3)


void mpoellipse(Char *initial, postype ctr, double halfwid,
		       double halfht)
{
  fputs(initial, stdout);
  wcoord(&output, ctr.xpos + halfwid, ctr.ypos);
  controls();
  wcoord(&output, ctr.xpos + halfwid, ctr.ypos + halfht * r);
  wrand();
  wcoord(&output, ctr.xpos + halfwid * r, ctr.ypos + halfht);
  ddot();
  wcoord(&output, ctr.xpos, ctr.ypos + halfht);
  controls();
  wcoord(&output, ctr.xpos - halfwid * r, ctr.ypos + halfht);
  wrand();
  wcoord(&output, ctr.xpos - halfwid, ctr.ypos + halfht * r);
  ddot();
  wcoord(&output, ctr.xpos - halfwid, ctr.ypos);
  controls();
  wcoord(&output, ctr.xpos - halfwid, ctr.ypos - halfht * r);
  wrand();
  wcoord(&output, ctr.xpos - halfwid * r, ctr.ypos - halfht);
  ddot();
  wcoord(&output, ctr.xpos, ctr.ypos - halfht);
  controls();
  wcoord(&output, ctr.xpos + halfwid * r, ctr.ypos - halfht);
  wrand();
  wcoord(&output, ctr.xpos + halfwid, ctr.ypos - halfht * r);
  ddot();
  printf("cycle");
}

#undef r


void mpoahead(int atyp, postype *point, postype shaft, double ht,
		     double wid, double lth, nametype *sou)
{
  postype P, L, R, Px, Lx, Rx, Q;
  double x, y;

  dahead(*point, shaft, ht, wid, lth / 72 * scale, &P, &L, &R, &Px, &Lx, &Rx,
	 &Q, &x, &y);
  if (atyp == 0) {
    printf("drw ");
    wprop(P, L, x - y, y, x);
    ddash();
    wpos(P);
    ddash();
    wprop(P, R, x - y, y, x);
    if (sou != NULL)
      addcolor(sou, -1.0);
    printf(" X\n");
  } else {
    printf("fill ");
    wpos(Rx);
    ddash();
    wpos(*point);
    ddash();
    wpos(Lx);
    if (atyp == 3) {
      ddash();
      wpos(Px);
    }
    printf("\n --cycle\n");
    if (sou != NULL)
      addcolor(sou, -1.0);
    printf(" X\n");
  }
  *point = P;
}


void mpoarcahead(postype C, postype point, int atyp, nametype *sou,
			double ht, double wid, double lth, double radius,
			double arcangle, postype *P)
{
  postype Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  /*D; if debuglevel > 0 then writeln(log,' mpoarcahead:'); D*/
  arcahead(C, point, atyp, ht, wid, lth, radius, arcangle, P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  printf("fill ");
  wpos(Ai);
  popgwarc(Ci, radius, posangle(Ai, Ci), posangle(point, Ci), -ccw);
  popgwarc(Co, radius, posangle(point, Co), posangle(Ao, Co), ccw);
  if (atyp == 0 && lwi < (wid - lwi) / 2) {
    ddash();
    wprop(Ao, Co, radius - lwi, lwi, radius);
    popgwarc(Co, radius - lwi, posangle(Ao, Co), posangle(*P, Co), -ccw);
    popgwarc(Ci, radius + lwi, posangle(*P, Ci), posangle(Ai, Ci), ccw);
  } else if (atyp == 3 && lwi < (wid - lwi) / 2) {
    popgwarc(Cox, radius, posangle(Ao, Cox), posangle(Px, Cox), -ccw);
    popgwarc(Cix, radius, posangle(Px, Cix), posangle(Ai, Cix), ccw);
  }
  printf(" --cycle");
  addcolor(sou, 0.0);
  printf(";\n");
  /*D; if debuglevel > 0 then begin
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
     end D*/
}


void mpodraw(primitive *node)
{
  /* node is always <> nil */
  int lsp;
  postype tmpat, X0, X1, X3;
  primitive *tn;
  double x, y, lth;
  primitive *WITH, *WITH1;
  int TEMP;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);
  WITH = node;
  switch (WITH->ptype) {

  case XLbox:
  case XBLOCK:
    initnesw();
    nesw(node);
    if (WITH->ptype == XLbox) {
      if (WITH->shadedp != NULL ||
	  WITH->UU.Ubox.boxfill >= 0.0 && WITH->UU.Ubox.boxfill <= 1.0) {
	mpobox("fill ", WITH->aat, WITH->UU.Ubox.boxwidth / 2,
	       WITH->UU.Ubox.boxheight / 2, WITH->UU.Ubox.boxradius);
	addcolor(WITH->shadedp, WITH->UU.Ubox.boxfill);
	printf(";\n");
      }
      if (drawn(node, lsp, -1.0)) {
	mposetthick(WITH->lthick);
	mpolinecap(lsp);
	mpobox("drw ", WITH->aat, WITH->UU.Ubox.boxwidth / 2,
	       WITH->UU.Ubox.boxheight / 2, WITH->UU.Ubox.boxradius);
	mpodashdot(lsp, WITH->lparam, WITH->outlinep);
      }
    }
    mpowrtext(WITH->textp, 0.5 * (east + west), 0.5 * (north + south));
    break;

  case XLellipse:
    if (WITH->shadedp != NULL ||
	WITH->UU.Uellipse.efill >= 0.0 && WITH->UU.Uellipse.efill <= 1.0) {
      mpoellipse("fill ", WITH->aat, WITH->UU.Uellipse.elwidth / 2,
		 WITH->UU.Uellipse.elheight / 2);
      addcolor(WITH->shadedp, WITH->UU.Uellipse.efill);
      printf(";\n");
    }
    if (drawn(node, lsp, -1.0)) {
      mposetthick(WITH->lthick);
      mpolinecap(lsp);
      mpoellipse("drw ", WITH->aat, WITH->UU.Uellipse.elwidth / 2,
		 WITH->UU.Uellipse.elheight / 2);
      mpodashdot(lsp, WITH->lparam, WITH->outlinep);
    }
    mpowrtext(WITH->textp, WITH->aat.xpos, WITH->aat.ypos);
    break;

  case XLcircle:
    if (WITH->shadedp != NULL ||
	WITH->UU.Ucircle.cfill >= 0.0 && WITH->UU.Ucircle.cfill <= 1.0) {
      printf("fill fullcircle scaled ");
      wfloat(&output, WITH->UU.Ucircle.radius * 2 / fsc);
      printf(" shifted ");
      wpos(WITH->aat);
      addcolor(WITH->shadedp, WITH->UU.Ucircle.cfill);
      printf(";\n");
    }
    if (drawn(node, lsp, -1.0)) {
      mposetthick(WITH->lthick);
      mpolinecap(lsp);
      printf("drw fullcircle scaled ");
      wfloat(&output, WITH->UU.Ucircle.radius * 2 / fsc);
      printf(" shifted ");
      wpos(WITH->aat);
      mpodashdot(lsp, WITH->lparam, WITH->outlinep);
    }
    mpowrtext(WITH->textp, WITH->aat.xpos, WITH->aat.ypos);
    break;

  case XLspline:
    if (drawn(node, lsp, -1.0)) {
      while (node != NULL) {
	if (node->UU.Uline.aradius == mdistmax)
	  x = 2.0 / 3;
	else
	  x = node->UU.Uline.aradius;
	if (!isthen(node)) {
	  getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	  lth = qenv(node, XLlinethick, tn->lthick);
	  mposetthick(lth);
	  spltot = primdepth(node);
	  splcount = spltot;
	  tmpat = node->aat;
	  TEMP = ahlex(tn->UU.Uline.atype);
	  if ((!sfill) & (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD))
	    mpoahead(ahnum(tn->UU.Uline.atype), &node->aat,
		     node->UU.Uline.endpos,
		     qenv(node, XLarrowht, tn->UU.Uline.height),
		     qenv(node, XLarrowwid, tn->UU.Uline.width), lth,
		     soutline);
	  mpolinecap(lsp);
	  if (sfill)
	    printf("fill ");
	  else
	    printf("drw ");
	  wpos(node->aat);
	  node->aat = tmpat;
	  if (node->UU.Uline.aradius == mdistmax) {
	    ddash();
	    if (spltot > 1) {
	      wprop(node->UU.Uline.endpos, node->aat, 1.0, 1.0, 2.0);
	      controls();
	      wprop(node->aat, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
	    }
	  } else if (spltot > 1) {
	    controls();
	    wprop(node->aat, node->UU.Uline.endpos, 1 - x, x, 1.0);
	  }
	} else if (splcount > 1) {
	  wrand();
	  wprop(node->aat, node->UU.Uline.endpos, (1 + x) / 2, (1 - x) / 2,
		1.0);
	  ddot();
	  wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	  controls();
	  wprop(node->aat, node->UU.Uline.endpos, (1 - x) / 2, (1 + x) / 2,
		1.0);
	}
	if (splcount == 1) {
	  /*D; if debuglevel > 0 then writeln(log,'last node started');D*/
	  if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	    wrand();
	    wprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	    ddot();
	    wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	    ddash();
	  } else if (node->UU.Uline.aradius != distmax) {
	    wrand();
	    wprop(node->aat, node->UU.Uline.endpos, x, 1 - x, 1.0);
	    ddot();
	  }
	  tmpat = node->UU.Uline.endpos;
	  TEMP = ahlex(node->UU.Uline.atype);
	  if ((!sfill) & (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)) {
	    x = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		       node->UU.Uline.endpos.ypos - node->aat.ypos);
	    y = ahoffset(qenv(node, XLarrowht, node->UU.Uline.height),
			 qenv(node, XLarrowwid, node->UU.Uline.width),
			 lth / 72 * scale);
	    if (x > 0.0)
	      pprop(node->aat, &tmpat, y, x - y, x);
	  }
	  wpos(tmpat);
	  if (sfill) {
	    printf(" ..cycle");
	    addcolor(sshade, vfill);
	    printf(";\n");
	  } else
	    mpodashdot(lsp, node->lparam, node->outlinep);
	  TEMP = ahlex(node->UU.Uline.atype);
	  if ((!sfill) & (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD))
	    mpoahead(ahnum(node->UU.Uline.atype), &node->UU.Uline.endpos,
		     node->aat, qenv(node, XLarrowht, node->UU.Uline.height),
		     qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		     soutline);
	}
	if (!sfill)
	  node->ptype = XLline;
	/*D; if debuglevel > 0 then writeln(log,'node ',splcount:1);D*/
	splcount--;
	if (sfill)
	  node = node->son;
	else
	  node = NULL;
      }
    }
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    snode = node;
    if (drawn(node, lsp, -1.0)) {
      while (node != NULL) {
	if (!isthen(node)) {
	  getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	  mposetthick(lth);
	  tmpat = node->aat;
	  TEMP = ahlex(tn->UU.Uline.atype);
	  if ((!sfill) & (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD))
	    mpoahead(ahnum(tn->UU.Uline.atype), &node->aat,
		     node->UU.Uline.endpos,
		     qenv(node, XLarrowht, tn->UU.Uline.height),
		     qenv(node, XLarrowwid, tn->UU.Uline.width), lth,
		     soutline);
	  mpolinecap(lsp);
	  if (sfill)
	    printf("fill ");
	  else
	    printf("drw ");
	  wpos(node->aat);
	}
	TEMP = ahlex(node->UU.Uline.atype);
	if ((!sfill && node->son == NULL) & (TEMP == XDOUBLEHEAD ||
					     TEMP == XRIGHTHEAD)) {
	  getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	  lth = qenv(node, XLlinethick, node->lthick);
	  x = ahoffset(qenv(node, XLarrowht, tn->UU.Uline.height),
		       qenv(node, XLarrowwid, tn->UU.Uline.width),
		       lth / 72 * scale);
	  ddash();
	  wpos(affine(x, 0.0, node->UU.Uline.endpos,
		      affang(node->aat, node->UU.Uline.endpos)));
	  mpodashdot(lsp, node->lparam, node->outlinep);
	  mpoahead(ahnum(node->UU.Uline.atype), &node->UU.Uline.endpos,
		   node->aat, qenv(node, XLarrowht, tn->UU.Uline.height),
		   qenv(node, XLarrowwid, tn->UU.Uline.width), lth, soutline);
	} else if (node->son == NULL) {
	  /*D; if debuglevel > 0 then writeln(log,'last node started');D*/
	  ddash();
	  wpos(node->UU.Uline.endpos);
	  if (sfill) {
	    printf(" ..cycle");
	    addcolor(sshade, vfill);
	    printf(";\n");
	  } else
	    mpodashdot(lsp, node->lparam, node->outlinep);
	} else {
	  ddash();
	  wpos(node->UU.Uline.endpos);
	}
	if (sfill)
	  node = node->son;
	else {
	  node = NULL;
	  /*D; if debuglevel > 0 then writeln(log,'node ');D*/
	}
      }
    }
    if (!sfill && snode != NULL) {
      WITH1 = snode;
      mpowrtext(WITH1->textp,
		0.5 * (WITH1->UU.Uline.endpos.xpos + WITH1->aat.xpos),
		0.5 * (WITH1->UU.Uline.endpos.ypos + WITH1->aat.ypos));
    }
    break;

  case XLarc:
    if (drawn(node, lsp, -1.0)) {
      getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
      X0 = arcstart(node);
      if (WITH->shadedp != NULL ||
	  WITH->UU.Uline.lfill >= 0.0 && WITH->UU.Uline.lfill <= 1.0) {
	X3 = arcend(node);
	printf("fill ");
	wpos(X3);
	ddash();
	popgwarc(WITH->aat, fabs(WITH->UU.Uline.aradius),
		 posangle(X0, WITH->aat), posangle(X3, WITH->aat),
		 WITH->UU.Uline.endpos.ypos);
	printf(" ..cycle");
	addcolor(WITH->shadedp, WITH->UU.Uline.lfill);
	printf(";\n");
      }
      TEMP = ahlex(WITH->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	mpoarcahead(WITH->aat, X0, ahnum(WITH->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, WITH->UU.Uline.height),
		    qenv(node, XLarrowwid, WITH->UU.Uline.width), lth,
		    fabs(WITH->UU.Uline.aradius), WITH->UU.Uline.endpos.ypos,
		    &X0);
      X1 = arcend(node);
      TEMP = ahlex(WITH->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	mpoarcahead(WITH->aat, X1, ahnum(WITH->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, WITH->UU.Uline.height),
		    qenv(node, XLarrowwid, WITH->UU.Uline.width), lth,
		    -fabs(WITH->UU.Uline.aradius), WITH->UU.Uline.endpos.ypos,
		    &X1);
      mposetthick(lth);
      mpolinecap(lsp);
      printf("drw ");
      wpos(X0);
      popgwarc(WITH->aat, fabs(WITH->UU.Uline.aradius),
	       posangle(X0, WITH->aat), posangle(X1, WITH->aat),
	       WITH->UU.Uline.endpos.ypos);
      mpodashdot(lsp, WITH->lparam, WITH->outlinep);
    }
    mpowrtext(WITH->textp, WITH->aat.xpos, WITH->aat.ypos);
    break;

  case XLstring:
    mpowrtext(WITH->textp, WITH->aat.xpos, WITH->aat.ypos);
    break;

  case XLaTeX:
    if (WITH->textp != NULL) {
      wstring(&output, WITH->textp);
      putchar('\n');
    } else if (WITH->lthick >= 0.0)
      mposetthick(WITH->lthick);
    break;
  }
}  /* mpodraw */


/* include pgf.h */
/* pgf.x */
/* pgf output routines */
void pgfprelude(void)
{
  /* n,s,e,w: real */
  printf("\\begin{tikzpicture}[scale=2.54]\n");
  printf("%% %s option -g for TikZ and PGF 1.01\n", Version);
  printf("\\ifx\\dpiclw\\undefined\\newdimen\\dpiclw\\fi\n");
  printf("\\global\\def\\dpicdraw{\\draw[line width=\\dpiclw]}\n");
  printf("\\global\\def\\dpicstop{;}\n");
  printf("\\dpiclw=0.8bp\n");
}


void pgfpostlude(void)
{
  printf("\\end{tikzpicture}\n");
  /*D; if debuglevel > 0 then writeln(log,'pgfpostlude done') D*/
}


void pgfwrtext(primitive *np, nametype *tp, double x, double y)
{
  boolean A, B, L, R;

  if (tp == NULL)
    return;
  if (tp->next_ != NULL) {
    printf("\\draw ");
    wcoord(&output, x, y);
    printf(" node");
    putchar('{');
    texstacktext(tp);
    printf("};\n");
    return;
  }
  printf("\\draw ");
  wcoord(&output, x, y);
  printf(" node");
  checkjust(tp, &A, &B, &L, &R);
  if (A || B || L || R) {
    putchar('[');
    if (A && L)
      printf("above right");
    else if (A && R)
      printf("above left");
    else if (B && L)
      printf("below right");
    else if (B && R)
      printf("below left");
    else if (A)
      printf("above");
    else if (B)
      printf("below");
    else if (L)
      printf("right");
    else if (R)
      printf("left");
    putchar('=');
    /* Assume pgf built-in text offset = 4 bp */
    wfloat(&output, (venv(np, XLtextoffset) * 72 / scale - 4.0) / fsc);
    printf("bp]");
  }
  putchar('{');
  wstring(&output, tp);
  printf("};\n");
}


void pgfendpath(void)
{
  printf("\\dpicstop\n");
}


void pgfsetthick(double lthick)
{
  if (lthick == mdistmax) {
    if (lastthick != mdistmax)
      lthick = lastthick;
    else
      lthick = 0.8;
  }
  /*D if debuglevel > 0 then begin
     writeln(log);
     write(log,'lthick='); wfloat(log,lthick);
     write(log,' lastthick='); wfloat(log,lastthick); writeln(log)
     end;D*/
  if (lthick < 0.0 || lthick == lastthick)
    return;
  printf("\\dpiclw=");
  wfloat(&output, lthick);
  printf("bp\n");
  lastthick = lthick;
}


void pgfbox(postype ctr, double halfwid, double halfht, double rad)
{
  double r, h, v;

  if (rad == 0.0) {
    wcoord(&output, ctr.xpos - halfwid, ctr.ypos - halfht);
    printf(" rectangle ");
    wcoord(&output, ctr.xpos + halfwid, ctr.ypos + halfht);
    pgfendpath();
    return;
  }
  r = Min(Min(rad, halfht), halfwid);
  h = halfwid - r;
  v = halfht - r;
  mpocorner(ctr, h, v, r, 1.0, 0.0);
  mpocorner(ctr, -h, v, r, 0.0, 1.0);
  mpocorner(ctr, -h, -v, r, -1.0, 0.0);
  mpocorner(ctr, h, -v, r, 0.0, -1.0);
  printf("cycle");
  pgfendpath();
}


void pgfahead(int atyp, postype *point, postype shaft, double ht,
		     double wid, double lth)
{
  postype P, L, R, Px, Lx, Rx, Q;
  double x, y;

  dahead(*point, shaft, ht, wid, lth / 72 * scale, &P, &L, &R, &Px, &Lx, &Rx,
	 &Q, &x, &y);
      /* write(log,' ;Q:'); wcoord(log,Q.xpos,Q.ypos); */
  /*D if debuglevel > 0 then begin
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
      D*/
  /*D
         writeln(log);
         write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
         writeln(log) end; D*/
  if (atyp == 0) {
    wprop(P, L, x - y, y, x);
    ddash();
    wpos(P);
    ddash();
    wprop(P, R, x - y, y, x);
  } else if (atyp == 3) {
    wpos(Px);
    ddash();
    wpos(Rx);
    ddash();
    wpos(*point);
    ddash();
    wpos(Lx);
    printf(" --cycle");
    /* wprop(P,L,x-y,y,x); ddash; wpos(P); ddash; wprop(P,R,x-y,y,x) */
  } else {
    /* wpos(affine(x,0.0,P,Q));
    ddash; wpos(R); ddash; wpos(P); ddash; wpos(L); */
    wpos(Rx);
    ddash();
    wpos(*point);
    ddash();
    wpos(Lx);
    printf(" --cycle");
  }
  putchar('\n');
  pgfendpath();
  *point = P;
}


void pgfstartdraw(int initial, primitive *node, int lsp)
{
  Char sep;
  double fill;

  if (lsp == XLinvis)   /* initial = 1 or 3 for filled arrowheads */
    printf("\\fill");
  else if (initial == 0)
    printf("\\dpicdraw");
  else
    printf("\\filldraw");
  fill = -1.0;
  switch (node->ptype) {

  case XBLOCK:
    /* blank case */
    break;

  case XLbox:
    fill = node->UU.Ubox.boxfill;
    break;

  case XLcircle:
    fill = node->UU.Ucircle.cfill;
    break;

  case XLellipse:
    fill = node->UU.Uellipse.efill;
    break;

  case XLarc:
  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    fill = node->UU.Uline.lfill;
    break;
  }
  fill = (long)floor(fill * 1000000L + 0.5) / 1000000.0;
  if (node->shadedp != NULL || sshade != NULL) {
    printf("[fill=");
    if (node->shadedp != NULL)
      wstring(&output, node->shadedp);
    else
      wstring(&output, sshade);
    sep = ',';
  } else if (fill >= 0.0 && fill <= 1.0) {
    printf("[fill=");
    if (fill == 0.0)
      printf("black");
    else if (fill == 0.25)
      printf("darkgray");
    else if (fill == 0.5)
      printf("gray");
    else if (fill == 0.75)
      printf("lightgray");
    else if (fill == 1.0)
      printf("white");
    else {
      printf("white!");
      wfloat(&output, fill * 100);
      printf("!black");
    }
    sep = ',';
  } else
    sep = '[';
  if (initial >= 0 && initial < 32 && ((1L << initial) & 0xa) != 0) {
    printf("%cline width=0bp", sep);
    sep = ',';
  } else if (node->lthick >= 0.0 && node->lthick != lastthick) {
    printf("%cline width=", sep);
    wfloat(&output, node->lthick);
    printf("bp");
    sep = ',';
  }
  if (node->outlinep != NULL || soutline != NULL) {
    printf("%cdraw=", sep);
    if (node->outlinep != NULL)
      wstring(&output, node->outlinep);
    else
      wstring(&output, soutline);
    if (initial >= 0 && initial < 32 && ((1L << initial) & 0xa) != 0) {
      printf(",fill=");
      if (node->outlinep != NULL)
	wstring(&output, node->outlinep);
      else
	wstring(&output, soutline);
    }
    sep = ',';
  }
  if (lsp == XLdashed &&
      (initial < 0 || initial >= 32 || ((1L << initial) & 0xa) == 0)) {
    if (node->lparam <= 0)
      printf("%cdashed", sep);
    else {
      printf("%cdash pattern=on ", sep);
      wfloat(&output, node->lparam / fsc);
      printf("in off ");
      wfloat(&output, node->lparam / fsc);
      printf("in");
    }
    sep = ',';
  } else if (lsp == XLdotted) {
    if (node->lparam <= 0)
      printf("%cdotted", sep);
    else {
      printf("%ccap=round,dash pattern=on ", sep);
      printf("0pt off ");
      wfloat(&output, node->lparam * 72 / fsc);
      printf("bp");
    }
    sep = ',';
  }
  if (sep == ',')
    putchar(']');
  else
    putchar(' ');
}


void pgfarcahead(postype C, postype point, int atyp, nametype *sou,
			double ht, double wid, double lth, double radius,
			double arcangle, postype *P)
{
  postype Q, Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  /*D; if debuglevel > 0 then writeln(log,' pgfarcahead:'); D*/
  arcahead(C, point, atyp, ht, wid, lth, radius, arcangle, P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  printf("\\filldraw[line width=0bp");
  if (sou != NULL) {
    printf(",draw=");
    wstring(&output, soutline);
    printf(",fill=");
    wstring(&output, soutline);
  }
  putchar(']');
  /* Trace arrowhead outline */
  if (atyp == 0 && lwi < (wid - lwi) / 2) {
    wpos(*P);
    popgwarc(Ci, radius + lwi, posangle(*P, Ci), posangle(Ai, Ci), ccw);
    ddash();
    wpos(Ai);
    popgwarc(Ci, radius, posangle(Ai, Ci), posangle(point, Ci), -ccw);
    popgwarc(Co, radius, posangle(point, Co), posangle(Ao, Co), ccw);
    ddash();
    wprop(Ao, Co, radius - lwi, lwi, radius);
    popgwarc(Co, radius - lwi, posangle(Ao, Co), posangle(*P, Co), -ccw);
  } else if (atyp == 3 && lwi < (wid - lwi) / 2) {
    wpos(Px);
    popgwarc(Cix, radius, posangle(Px, Cix), posangle(Ai, Cix), ccw);
    popgwarc(Ci, radius, posangle(Ai, Ci), posangle(point, Ci), -ccw);
    popgwarc(Co, radius, posangle(point, Co), posangle(Ao, Co), ccw);
    popgwarc(Cox, radius, posangle(Ao, Cox), posangle(Px, Cox), -ccw);
  } else {
    Q.xpos = (Ao.xpos + Ai.xpos) / 2;
    Q.ypos = (Ao.ypos + Ai.ypos) / 2;
    wpos(Q);
    ddash();
    wpos(Ai);
    popgwarc(Ci, radius, posangle(Ai, Ci), posangle(point, Ci), -ccw);
    popgwarc(Co, radius, posangle(point, Co), posangle(Ao, Co), ccw);
    ddash();
    wpos(Q);
  }
  pgfendpath();
}


void pgfdraw(primitive *node)
{
  /* node is always <> nil */
  int lsp;
  postype X0, X1;
  primitive *tn;
  double x, y, lth;
  boolean v;
  int TEMP;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    initnesw();
    nesw(node);
    if (node->ptype == XBLOCK)
      v = drawn(node, lsp, -1.0);
    else
      v = drawn(node, lsp, node->UU.Ubox.boxfill);
    if (v) {
      pgfstartdraw(0, node, lsp);
      pgfbox(node->aat, node->UU.Ubox.boxwidth / 2,
	     node->UU.Ubox.boxheight / 2, node->UU.Ubox.boxradius);
    }
    pgfwrtext(node, node->textp, 0.5 * (east + west), 0.5 * (north + south));
    break;

  case XLellipse:
    if (drawn(node, lsp, node->UU.Uellipse.efill)) {
      pgfstartdraw(0, node, lsp);
      wpos(node->aat);
      printf(" ellipse (");
      wfloat(&output, node->UU.Uellipse.elwidth / 2 / fsc / 2.54);
      printf("in and ");
      wfloat(&output, node->UU.Uellipse.elheight / 2 / fsc / 2.54);
      printf("in)");
      pgfendpath();
    }
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->UU.Ucircle.cfill)) {
      pgfstartdraw(0, node, lsp);
      wpos(node->aat);
      printf(" circle (");
      wfloat(&output, node->UU.Ucircle.radius / fsc / 2.54);
      printf("in)");
      pgfendpath();
    }
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLspline:
    if (drawn(node, lsp, -1.0)) {
      if (node->UU.Uline.aradius == mdistmax)
	x = 2.0 / 3;
      else
	x = node->UU.Uline.aradius;
      if (!isthen(node)) {  /* first spline */
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	spltot = primdepth(node);
	splcount = spltot;
	X0 = node->aat;
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	  pgfstartdraw(ahnum(node->UU.Uline.atype), node, lsp);
	  pgfahead(ahnum(tn->UU.Uline.atype), &node->aat, node->UU.Uline.endpos,
		   qenv(node, XLarrowht, tn->UU.Uline.height),
		   qenv(node, XLarrowwid, tn->UU.Uline.width),
		   qenv(node, XLlinethick, tn->lthick));
	}
	/* pgflinecap(lsp); */
	pgfstartdraw(0, node, lsp);
	wpos(node->aat);
	node->aat = X0;
	if (node->UU.Uline.aradius == mdistmax) {
	  ddash();
	  if (spltot > 1) {
	    wprop(node->UU.Uline.endpos, node->aat, 1.0, 1.0, 2.0);
	    controls();
	    wprop(node->aat, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
	  }
	} else if (spltot > 1) {
	  controls();
	  wprop(node->aat, node->UU.Uline.endpos, 1 - x, x, 1.0);
	}
      } else if (splcount > 1) {
	wrand();
	wprop(node->aat, node->UU.Uline.endpos, (1 + x) / 2, (1 - x) / 2, 1.0);
	ddot();
	wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	controls();
	wprop(node->aat, node->UU.Uline.endpos, (1 - x) / 2, (1 + x) / 2, 1.0);
      }
      if (splcount == 1) {
	if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	  wrand();
	  wprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	  ddot();
	  wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	  ddash();
	} else if (node->UU.Uline.aradius != distmax) {
	  wrand();
	  wprop(node->aat, node->UU.Uline.endpos, x, 1 - x, 1.0);
	  ddot();
	}
	X0 = node->UU.Uline.endpos;
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	  x = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		     node->UU.Uline.endpos.ypos - node->aat.ypos);
	  switch (ahnum(node->UU.Uline.atype)) {

	  case 0:
	    y = ahoffset(qenv(node, XLarrowht, node->UU.Uline.height),
			 qenv(node, XLarrowwid, node->UU.Uline.width),
			 lth / 72 * scale);
	    break;

	  case 3:
	    y = qenv(node, XLarrowht, node->UU.Uline.height) * (2.0 / 3);
	    break;

	  default:
	    y = qenv(node, XLarrowht, node->UU.Uline.height);
	    break;
	  }
	  if (x != 0.0)
	    pprop(node->aat, &X0, y, x - y, x);
	}
	wpos(X0);
	pgfendpath();
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	  pgfstartdraw(ahnum(node->UU.Uline.atype), node, lsp);
	  pgfahead(ahnum(node->UU.Uline.atype), &node->UU.Uline.endpos,
		   node->aat, qenv(node, XLarrowht, node->UU.Uline.height),
		   qenv(node, XLarrowwid, node->UU.Uline.width), lth);
	}
      }
      node->ptype = XLline;
      /*D; if debuglevel > 0 then
              writeln(log,'pgfdraw: node ',splcount:1);D*/
      splcount--;
    }
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (drawn(node, lsp, -1.0)) {
      lth = qenv(node, XLlinethick, tn->lthick);
      if (!isthen(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	  pgfstartdraw(ahnum(node->UU.Uline.atype), node, lsp);
	  pgfahead(ahnum(tn->UU.Uline.atype), &node->aat, node->UU.Uline.endpos,
		   qenv(node, XLarrowht, tn->UU.Uline.height),
		   qenv(node, XLarrowwid, tn->UU.Uline.width), lth);
	}
	pgfstartdraw(0, node, lsp);
	wpos(node->aat);
      }
      TEMP = ahlex(tn->UU.Uline.atype);
      if ((node->son == NULL) & (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)) {
	switch (ahnum(node->UU.Uline.atype)) {

	case 0:
	  y = ahoffset(qenv(node, XLarrowht, tn->UU.Uline.height),
		       qenv(node, XLarrowwid, tn->UU.Uline.width),
		       lth / 72 * scale);
	  break;

	case 3:
	  y = qenv(node, XLarrowht, tn->UU.Uline.height) * (2.0 / 3);
	  break;

	default:
	  y = qenv(node, XLarrowht, tn->UU.Uline.height);
	  break;
	}
	/*D if debuglevel > 0 then begin
	    write(log,' right arrow: ahnum(atype)=',ahnum(atype):1,
	      ' y='); wfloat(log,y); writeln(log);
	    tmpat := affine(y,0.0,endpos,affang(aat,endpos));
	    write(log,' Line end='); wcoord(log,tmpat.xpos,tmpat.ypos);
	    writeln(log)
	    end; D*/
	ddash();
	wpos(affine(y, 0.0, node->UU.Uline.endpos,
		    affang(node->aat, node->UU.Uline.endpos)));
	pgfendpath();
	pgfstartdraw(ahnum(node->UU.Uline.atype), node, lsp);
	pgfahead(ahnum(node->UU.Uline.atype), &node->UU.Uline.endpos, node->aat,
		 qenv(node, XLarrowht, tn->UU.Uline.height),
		 qenv(node, XLarrowwid, tn->UU.Uline.width), lth);
      } else if (node->son == NULL) {
	/*D; if debuglevel > 0 then
	        writeln(log,'pgfdraw: last node started');D*/
	ddash();
	wpos(node->UU.Uline.endpos);
	pgfendpath();
      } else {
	ddash();
	wpos(node->UU.Uline.endpos);
      }
      /*D; if debuglevel > 0 then writeln(log,'pgfdraw: node ');D*/
      /* ptype := XLline */
    }
    pgfwrtext(node, node->textp,
	      0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLarc:
    if (drawn(node, lsp, -1.0)) {
      getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
      X0 = arcstart(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	pgfarcahead(node->aat, X0, ahnum(node->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, node->UU.Uline.height),
		    qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		    fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		    &X0);
      X1 = arcend(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	pgfarcahead(node->aat, X1, ahnum(node->UU.Uline.atype), soutline,
		    qenv(node, XLarrowht, node->UU.Uline.height),
		    qenv(node, XLarrowwid, node->UU.Uline.width), lth,
		    -fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		    &X1);
      pgfstartdraw(0, node, lsp);
      wpos(X0);
      popgwarc(node->aat, fabs(node->UU.Uline.aradius),
	       posangle(X0, node->aat), posangle(X1, node->aat),
	       node->UU.Uline.endpos.ypos);
    }
    pgfendpath();
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLstring:
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
      wstring(&output, node->textp);
      putchar('\n');
    } else if (node->lthick >= 0.0)
      pgfsetthick(node->lthick);
    break;
  }
}  /* pgfdraw */


/* include ps.h */
/* ps.x */
/* Output routines for Postscript, PSfrag */
void pswfloat(FILE **iou, double x)
{
  putc(' ', *iou);
  wfloat(iou, x);
  /*DUGHM ;flush(iou) MHGUD*/
}


void psprelude(double n, double s, double e, double w, double lth)
{
  double wx, ex, nx, sx;

  /* writeln('%!PS'); */
  /* writeln('%!PS-Adobe-3.0 EPSF-3.0'); */
  printf("%%!PS-Adobe-3.0\n");
  wx = w / fsc - lth / 2;
  ex = e / fsc + lth / 2;
  nx = n / fsc + lth / 2;
  sx = s / fsc - lth / 2;
  printf("%%%%BoundingBox: %d %d %d %d\n",
	 Floor((long)floor(wx * 1000000L + 0.5) / 1000000.0),
	 Floor((long)floor(sx * 1000000L + 0.5) / 1000000.0),
	 Ceil((long)floor(ex * 1000000L + 0.5) / 1000000.0),
	 Ceil((long)floor(nx * 1000000L + 0.5) / 1000000.0));
  /*D; if debuglevel > 0 then begin write(log,'lth= '); wfloat(log,lth);
     write(log,' wx='); wfloat(log,wx);
     write(log,' Floor wx='); wfloat(log,Floor(round(wx*1000000)/1000000.0));
     writeln(log)
     end; D*/
  printf("%%%%HiResBoundingBox:");
  pswfloat(&output, wx);
  pswfloat(&output, sx);
  pswfloat(&output, ex);
  pswfloat(&output, nx);
  printf("\n%%%%Creator: ");
  printf("%s option ", Version);
  switch (drawmode) {

  case PSfrag:
    printf("-f (psfrag strings)");
    break;

  case PS:
    printf("-r");
    break;
  }
  printf(" for Postscript\n");
  printf("%%%%Pages: 1\n");
  printf("%%%%EndComments\n");
  /* writeln('%%EndProlog'); */
  printf("%%%%Page: 1 1\n");
  /* writeln('userdict begin'); */
  printf("/DpicDict 20 dict def DpicDict begin\n");
  /* writeln('/setlinewid {0 exch 72.27 div 72 mul');
  writeln(' dtransform truncate idtransform setlinewidth pop} def'); */
  /* write('/setlineparms { [] 0 setdash 0 setlinecap 1 setlinejoin'); */
  printf("/setlineparms { [] 0 setdash 0 setlinecap 0 setlinejoin");
  printf(" 10 setmiterlimit} def\n");
  printf("/ostroke { stroke setlineparms } def\n");
  printf("/endstroke { ostroke } def /npath { newpath } def\n");
  printf(" setlineparms\n");
  if ((printstate & 1) != 1)
    return;
  if (drawmode == PS) {
    printf("/strsiz\n");
    printf(" {newpath 0 0 moveto true charpath flattenpath");
    printf(" pathbbox 4 1 roll pop pop pop}def\n");
    printf("/setcapht {(I) strsiz /capht exch def} def\n");
    printf("/postext {1 add baselineskip mul 1 sub 2 div capht mul add moveto} def\n");
    printf("/strwidth { dup stringwidth pop } def\n");
    printf("/ljust { labelsep } def\n");
    printf("/rjust { strwidth labelsep add neg } def\n");
    printf("/cjust { strwidth 2 div neg } def\n");
    printf("/above { capht 2 div labelsep add } def\n");
    printf("/below { capht 2 div neg labelsep sub } def\n");
    printf("/vjust {0 capht baselineskip mul neg rmoveto\n");
    printf(" currentpoint /y exch def /x exch def} def\n");
  } else {
    printf("/ljust { labelsep } def /rjust { labelsep neg } def ");
    printf("/cjust { 0 } def\n");
    printf("/above { labelsep } def /below { labelsep neg } def ");
  }
  printf("/vcenter { 0 } def\n");
  printf("/basefont {/Times-Roman findfont} def\n");
  printf(" basefont 11 scalefont setfont\n");
  printf("/labelsep 2 def\n");
  printf("/baselineskip 1.4 def\n");
}


void pspostlude(void)
{
  printf("showpage end\n");
  printf("%%%%EOF\n");
  /*D; if debuglevel > 0 then writeln(log,'pspostlude done');D*/
}


void psnewpath(void)
{
  printf("npath\n");
}


void pswcoord(FILE **iou, double x, double y)
{
  pswfloat(iou, x / fsc);
  /*DUGHM ;flush(iou) MHGUD*/
  pswfloat(iou, y / fsc);
}


void pswpos(postype pos)
{
  pswcoord(&output, pos.xpos, pos.ypos);
}


void pswstring(nametype *p)
{
  int i;
  Char c;
  boolean waswhite, iswhite;
  int FORLIM;

  waswhite = false;
  if (p == NULL)
    return;
  if (p->segmnt == NULL)
    return;
  FORLIM = p->len;
  for (i = 0; i < FORLIM; i++) {
    c = p->segmnt[p->seginx + i];
    iswhite = (c == etxch || c == nlch || c == tabch || c == ' ');
    if (!iswhite || !waswhite) {
      if (c == bslch || c == ')' || c == '(')
	putchar(bslch);
      putchar(c);
    }
    waswhite = iswhite;
  }
}


void pswtext(nametype *tp, double x, double y)
{
  int i;
  nametype *tx;
  boolean L, R, A, B;

  if (tp != NULL && drawmode == PS) {
    tx = tp;
    i = 0;
    do {
      i++;
      tx = tx->next_;
    } while (tx != NULL);
    printf(" setcapht");
    pswcoord(&output, x, y);
    printf(" %d postext\n", i);
    do {
      printf(" vjust (");
      checkjust(tp, &A, &B, &L, &R);
      pswstring(tp);
      printf(")\n");
      if (L)
	printf(" ljust");
      else if (R)
	printf(" rjust");
      else
	printf(" cjust");
      if (A)
	printf(" above");
      else if (B)
	printf(" below");
      else
	printf(" vcenter");
      printf(" rmoveto show x y moveto\n");
      tp = tp->next_;
    } while (tp != NULL);
    return;
  }
  if (tp == NULL)
    return;
  printf("(\\\\tex[");
  checkjust(tp, &A, &B, &L, &R);
  if (L)
    putchar('l');
  else if (R)
    putchar('r');
  else
    putchar('c');
  if (A)
    putchar('b');
  else if (B)
    putchar('t');
  else
    putchar('c');
  printf("][lB]{");
  if (tp->next_ != NULL)
    printf("\\\\shortstack{");
  tx = tp;
  do {
    if (L)
      printf("\\\\rlap{");
    else if (R)
      printf("\\\\llap{");
    pswstring(tx);
    if (L || R)
      putchar('}');
    tx = tx->next_;
    if (tx != NULL) {
      printf("\\\\\\\\ ");
      checkjust(tx, &A, &B, &L, &R);
    }
  } while (tx != NULL);
  if (tp->next_ != NULL)
    printf("}})\n");
  else
    printf("})\n");
  pswcoord(&output, x, y);
  printf(" moveto");
  checkjust(tp, &A, &B, &L, &R);
  if (L)
    printf(" ljust");
  else if (R)
    printf(" rjust");
  else
    printf(" cjust");
  if (A)
    printf(" above");
  else if (B)
    printf(" below");
  else
    printf(" vcenter");
  printf(" rmoveto show\n");
}


void pswprop(postype p1, postype p2, double a, double b, double c)
{
  pprop(p1, &p2, a, b, c);
  pswpos(p2);
}


void pssetthick(double lt)
{
  if (lt < 0.0 || lt == lastthick)
    return;
  pswfloat(&output, lt);
  printf(" setlinewidth\n");
  lastthick = lt;
}


void pslinearfill(double f, nametype *ss)
{
  if (f >= 0.0 && f <= 1.0) {
    printf(" gsave\n");
    pswfloat(&output, f);
    printf(" setgray fill grestore\n");
    return;
  }
  if (ss == NULL)
    return;
  printf(" gsave\n");
  wstring(&output, ss);
  printf(" setrgbcolor fill grestore\n");
}


void pssetcolor(nametype *op)
{
  if (op == NULL)
    return;
  printf(" currentrgbcolor ");
  wstring(&output, op);
  printf(" setrgbcolor\n");
}


void pslineopts(int lspec, double param, nametype *op)
{
  if (lspec == XLdashed) {
    if (param == mdistmax)
      param = 3 * fsc;
    printf(" [");
    pswfloat(&output, param / fsc);
    pswfloat(&output, param / fsc);
    printf(" ] 0 setdash\n");
  } else if (lspec == XLdotted) {
    if (param == mdistmax)
      param = 5 * fsc;
    printf(" [ 0");
    pswfloat(&output, param / fsc);
    printf(" ] 0 setdash 1 setlinecap\n");
  }
  pssetcolor(op);
}


void psendline(nametype *op)
{
  printf(" endstroke");
  if (op != NULL)
    printf(" setrgbcolor\n");
  else
    putchar('\n');
}


void pswarc(postype C, postype S, postype E, double r, double ccw)
{
  double y;

  pswpos(C);
  pswfloat(&output, r / fsc);
  y = datan(S.ypos - C.ypos, S.xpos - C.xpos);
  pswfloat(&output, 180.0 / pi * y);
  y = datan(E.ypos - C.ypos, E.xpos - C.xpos);
  pswfloat(&output, 180.0 / pi * y);
  if (ccw >= 0.0)
    printf(" arc\n");
  else
    printf(" arcn\n");
}


void psarcahead(postype C, int atyp, postype *point, double ht,
		       double wid, double lth, double radius, double angle,
		       nametype *sou)
{
  postype P, Q, Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  arcahead(C, *point, atyp, ht, wid, lth, radius, angle, &P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  /* Trace arrowhead outline */
  psnewpath();
  pswarc(Ci, Ai, *point, radius, -ccw);
  pswarc(Co, *point, Ao, radius, ccw);
  if (atyp == 0 && lwi < (wid - lwi) / 2) {
    Q = Co;
    pprop(Ao, &Q, radius - lwi, lwi, radius);
    pswpos(Q);
    printf(" lineto\n");
    pswarc(Co, Q, P, radius - lwi, -ccw);
    Q = Ci;
    pprop(Ai, &Q, radius + lwi, -lwi, radius);
    pswarc(Ci, P, Q, radius + lwi, ccw);
  }
  if (atyp == 3 && lwi < (wid - lwi) / 2) {
    pswarc(Cox, Ao, Px, radius, -ccw);
    pswarc(Cix, Px, Ai, radius, ccw);
  } else {
    pswpos(Ai);
    printf(" lineto\n");
  }
  printf(" fill\n");
  *point = P;
}


void psahead(int atyp, postype *point, postype shaft, double ht,
		    double wid, double lth, nametype *sou)
{
  postype P, L, R, Px, Lx, Rx, Q;
  double x, y;

  dahead(*point, shaft, ht, wid, lth / 72 * scale, &P, &L, &R, &Px, &Lx, &Rx,
	 &Q, &x, &y);
  printf("newpath");
  if (atyp == 0) {
    pswprop(P, R, x - y, y, x);
    printf(" moveto");
    pswpos(P);
    printf(" lineto\n");
    pswprop(P, L, x - y, y, x);
    printf(" lineto");
  } else {
    pswpos(Rx);
    printf(" moveto\n");
    pswpos(*point);
    printf(" lineto");
    pswpos(Lx);
    printf(" lineto\n");
    if (atyp == 3) {
      pswpos(Px);
      printf(" lineto\n");
    }
    printf(" closepath fill");
  }
  psendline(NULL);
  *point = P;
}


void psbox(postype aat, double halfwid, double halfht, double rad)
{
  int i;
  postype corner[4];

  psnewpath();
  corner[0].xpos = aat.xpos + halfwid;
  corner[0].ypos = aat.ypos + halfht;
  corner[1].xpos = aat.xpos - halfwid;
  corner[1].ypos = aat.ypos + halfht;
  corner[2].xpos = aat.xpos - halfwid;
  corner[2].ypos = aat.ypos - halfht;
  corner[3].xpos = aat.xpos + halfwid;
  corner[3].ypos = aat.ypos - halfht;
  pswcoord(&output, aat.xpos + halfwid, aat.ypos);
  printf(" moveto 0 setlinejoin\n");
  rad = Min(Min(fabs(rad), fabs(halfht)), fabs(halfwid));
  if (rad == 0.0) {
    for (i = 0; i <= 3; i++) {
      pswpos(corner[i]);
      printf(" lineto");
      if (i == 1 || i == 3)
	putchar('\n');
    }
  } else {
    for (i = 0; i <= 3; i++) {
      pswpos(corner[i]);
      pswpos(corner[(i + 1) & 3]);
      pswfloat(&output, rad / fsc);
      printf(" arcto 4 {pop} repeat\n");
    }
  }
  printf(" closepath");
}


void pscircle(double rd)
{
  psnewpath();
  printf(" 0 0");
  pswfloat(&output, fabs(rd) / fsc);
  printf(" 0 360 arc closepath");
}


#define r               ((sqrt(2.0) - 1) * 4 / 3)


void psellipse(double x, double y)
{
  x = fabs(x) / 2;
  y = fabs(y) / 2;
  psnewpath();
  pswcoord(&output, x, 0.0);
  printf(" moveto\n");
  pswcoord(&output, x, y * r);
  pswcoord(&output, x * r, y);
  pswcoord(&output, 0.0, y);
  printf(" curveto\n");
  pswcoord(&output, -x * r, y);
  pswcoord(&output, -x, y * r);
  pswcoord(&output, -x, 0.0);
  printf(" curveto\n");
  pswcoord(&output, -x, -y * r);
  pswcoord(&output, -x * r, -y);
  pswcoord(&output, 0.0, -y);
  printf(" curveto\n");
  pswcoord(&output, x * r, -y);
  pswcoord(&output, x, -y * r);
  pswcoord(&output, x, 0.0);
  printf(" curveto closepath\n");
}

#undef r


void psdraw(primitive *node)
{
  /* node is always <> nil */
  int lsp;
  postype X0, X1, X2;
  primitive *tn;
  double h, w, x, y, lth, fill;
  int TEMP;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);   /* printobject(node); */
  /*D if debuglevel > 0 then begin write(log,'psdraw: ');
     snaptype(log,ptype); D*/
  /*D
           write(log,' lth='); wfloat(log,lth);
           writeln(log)
           end;
        if linesignal > 0 then begin write(errout,'psdraw: ');
           snaptype(errout,ptype); writeln(errout) end D*/
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    initnesw();
    nesw(node);
    if (node->ptype == XLbox) {
      if (node->UU.Ubox.boxfill >= 0.0 && node->UU.Ubox.boxfill <= 1.0 ||
	  node->shadedp != NULL) {
	psbox(node->aat, node->UU.Ubox.boxwidth / 2,
	      node->UU.Ubox.boxheight / 2, node->UU.Ubox.boxradius);
	pssetthick(lth);
	if (drawn(node, lsp, -1.0)) {
	  printf(" gsave\n");
	  if (node->shadedp == NULL) {
	    pswfloat(&output, node->UU.Ubox.boxfill);
	    printf(" setgray");
	  } else {
	    putchar(' ');
	    wstring(&output, node->shadedp);
	    printf(" setrgbcolor");
	  }
	  printf(" fill grestore\n");
	  pslineopts(lsp, node->lparam, node->outlinep);
	  psendline(node->outlinep);
	} else if (node->shadedp == NULL) {
	  printf(" currentgray");
	  pswfloat(&output, node->UU.Ubox.boxfill);
	  printf(" setgray fill setgray setlineparms\n");
	} else {
	  printf(" currentrgbcolor ");
	  wstring(&output, node->shadedp);
	  printf(" setrgbcolor fill setrgbcolor setlineparms\n");
	}
      } else if (drawn(node, lsp, -1.0)) {
	psbox(node->aat, node->UU.Ubox.boxwidth / 2,
	      node->UU.Ubox.boxheight / 2, node->UU.Ubox.boxradius);
	pssetthick(lth);
	pslineopts(lsp, node->lparam, node->outlinep);
	psendline(node->outlinep);
      }
    }
    pswtext(node->textp, 0.5 * (east + west), 0.5 * (north + south));
    break;

  case XLellipse:
  case XLcircle:
    if (node->ptype == XLellipse) {
      x = node->UU.Uellipse.elwidth;
      y = node->UU.Uellipse.elheight;
      fill = node->UU.Uellipse.efill;
    } else
      fill = node->UU.Ucircle.cfill;
    if (fill >= 0.0 && fill <= 1.0 || node->shadedp != NULL) {
      pssetthick(lth);
      printf(" gsave ");
      pswpos(node->aat);
      printf(" translate\n");
      if (node->ptype == XLellipse)
	psellipse(x, y);
      else
	pscircle(node->UU.Ucircle.radius);
      printf(" gsave ");
      if (node->shadedp == NULL) {
	pswfloat(&output, fill);
	printf(" setgray");
      } else {
	wstring(&output, node->shadedp);
	printf(" setrgbcolor");
      }
      printf(" fill grestore\n");
      if (drawn(node, lsp, -1.0)) {
	pslineopts(lsp, node->lparam, node->outlinep);
	psendline(node->outlinep);
      }
      printf(" grestore\n");
    } else if (drawn(node, lsp, -1.0)) {
      pssetthick(lth);
      printf(" gsave ");
      pswpos(node->aat);
      printf(" translate\n");
      if (node->ptype == XLellipse)
	psellipse(x, y);
      else
	pscircle(node->UU.Ucircle.radius);
      pslineopts(lsp, node->lparam, node->outlinep);
      psendline(node->outlinep);
      printf(" grestore\n");
    }
    pswtext(node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLspline:
    if (drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {  /* first spline */
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	lth = qenv(tn, XLlinethick, tn->lthick);
	spltot = primdepth(node);
	splcount = spltot;
	X0 = node->aat;
	pssetthick(lth);
	pssetcolor(soutline);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  psahead(ahnum(tn->UU.Uline.atype), &node->aat, node->UU.Uline.endpos,
		  qenv(tn, XLarrowht, tn->UU.Uline.height),
		  qenv(tn, XLarrowwid, tn->UU.Uline.width), lth, soutline);
	psnewpath();
	pswpos(node->aat);
	node->aat = X0;
	printf(" moveto\n");
	if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	  pswprop(node->UU.Uline.endpos, node->aat, 1.0, 1.0, 2.0);
	  printf(" lineto\n");
	  pswprop(node->aat, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
	} else if (spltot > 1)
	  pswprop(node->aat, node->UU.Uline.endpos, 1 - node->UU.Uline.aradius,
		  node->UU.Uline.aradius, 1.0);
      } else if (splcount > 1 && node->UU.Uline.aradius == mdistmax) {
	pswprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	pswprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	printf(" curveto\n");
	pswprop(node->aat, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
      } else if (splcount > 1) {
	pswprop(node->aat, node->UU.Uline.endpos,
		0.5 + node->UU.Uline.aradius / 2,
		0.5 - node->UU.Uline.aradius / 2, 1.0);
	pswprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	printf(" curveto\n");
	pswprop(node->aat, node->UU.Uline.endpos,
		0.5 - node->UU.Uline.aradius / 2,
		0.5 + node->UU.Uline.aradius / 2, 1.0);
      }
      if (splcount == 1) {
	if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	  pswprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	  pswprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	  printf(" curveto\n");
	} else if (spltot > 1)
	  pswprop(node->aat, node->UU.Uline.endpos, node->UU.Uline.aradius,
		  1 - node->UU.Uline.aradius, 1.0);
	X0 = node->UU.Uline.endpos;
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	  x = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		     node->UU.Uline.endpos.ypos - node->aat.ypos);
	  y = ahoffset(qenv(node, XLarrowht, node->UU.Uline.height),
		       qenv(node, XLarrowwid, node->UU.Uline.width),
		       lth / 72 * scale);
	  if (x != 0.0)
	    pprop(node->aat, &X0, y, x - y, x);
	}
	pswpos(X0);
	if (node->UU.Uline.aradius == mdistmax)
	  printf(" lineto");
	else
	  printf(" curveto");
	pslinearfill(node->UU.Uline.lfill, sshade);
	pslineopts(lsp, node->lparam, NULL);
	printf(" endstroke \n");
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	  psahead(ahnum(node->UU.Uline.atype), &node->UU.Uline.endpos,
		  node->aat, qenv(node, XLarrowht, node->UU.Uline.height),
		  qenv(node, XLarrowwid, node->UU.Uline.width), lth, soutline);
	if (soutline != NULL)
	  printf(" setrgbcolor\n");
      }
      node->ptype = XLline;
      /*D; if debuglevel > 0 then writeln(log,'node ',splcount:1) ;D*/
      splcount--;
    }
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
	lth = qenv(tn, XLlinethick, tn->lthick);
	pssetthick(lth);
	pssetcolor(soutline);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  psahead(ahnum(tn->UU.Uline.atype), &node->aat, node->UU.Uline.endpos,
		  qenv(tn, XLarrowht, tn->UU.Uline.height),
		  qenv(tn, XLarrowwid, tn->UU.Uline.width), lth, soutline);
	psnewpath();
	pswpos(node->aat);
	printf(" moveto\n");
      }
      TEMP = ahlex(node->UU.Uline.atype);
      if ((node->son == NULL) & (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)) {
	x = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		   node->UU.Uline.endpos.ypos - node->aat.ypos);
	y = ahoffset(qenv(tn, XLarrowht, tn->UU.Uline.height),
		     qenv(tn, XLarrowwid, tn->UU.Uline.width),
		     lth / 72 * scale);
	pswprop(node->aat, node->UU.Uline.endpos, y, x - y, x);
	printf(" lineto\n");
	pslineopts(lsp, node->lparam, NULL);
	printf(" endstroke \n");
	psahead(ahnum(node->UU.Uline.atype), &node->UU.Uline.endpos, node->aat,
		qenv(node, XLarrowht, node->UU.Uline.height),
		qenv(node, XLarrowwid, node->UU.Uline.width), lth, soutline);
	if (soutline != NULL)
	  printf(" setrgbcolor\n");
      } else if (node->son == NULL) {
	pswpos(node->UU.Uline.endpos);
	printf(" lineto\n");
	pslinearfill(node->UU.Uline.lfill, sshade);
	pslineopts(lsp, node->lparam, NULL);
	psendline(soutline);
      } else {
	pswpos(node->UU.Uline.endpos);
	printf(" lineto\n");
      }
    }
    pswtext(node->textp, 0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	    0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLarc:
    pssetthick(lth);
    if (drawn(node, lsp, -1.0)) {
      getlinshade(node, &tn, &sshade, &soutline, &vfill, &sfill);
      pssetcolor(soutline);
      X1 = arcstart(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	startarc(node, X1, lth, &h, &w);
	psarcahead(node->aat, ahnum(node->UU.Uline.atype), &X1, h, w, lth,
		   fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		   soutline);
      }
      X2 = arcend(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	endarc(node, X2, lth, &h, &w);
	psarcahead(node->aat, ahnum(node->UU.Uline.atype), &X2, h, w, lth,
		   -fabs(node->UU.Uline.aradius), node->UU.Uline.endpos.ypos,
		   soutline);
      }
      psnewpath();
      pswarc(node->aat, X1, X2, node->UU.Uline.aradius,
	     node->UU.Uline.endpos.ypos);
      pssetthick(lth);
      pslinearfill(node->UU.Uline.lfill, sshade);
      pslineopts(lsp, node->lparam, NULL);
      psendline(soutline);
    }
    pswtext(node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLstring:
    pswtext(node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
      wstring(&output, node->textp);
      putchar('\n');
    } else {
      if (node->lthick >= 0.0)
	pssetthick(node->lthick);
    }
    break;
  }
  /*DUGHM ;flush(output) MHGUD*/
}  /* psdraw */


/* # include 'mps.h' */

/* include tex.h */
/* tex.x */
/* Output routines for TeX, tTeX (eepicemu), pict2e */

void texprelude(double n, double s, double e, double w)
{
  printf("\\setlength{\\unitlength}{1in}\n");
  if (s != distmax) {
    printf("\\begin{picture}");
    wcoord(&output, e - w, n - s);
    wcoord(&output, w, s);
    printf("\n\\thicklines\n");
  }
  printf("%% %s option ", Version);
  switch (drawmode) {

  case TeX:
    printf("(none)");
    break;

  case pict2e:
    printf("-e (pict2e)");
    break;

  case tTeX:
    printf("-t (eepicemu)");
    break;
  }
  printf(" for LaTeX\n");
}


void texpostlude(void)
{
  printf("\\end{picture}\n");
  /*D if debuglevel > 0 then writeln(log,'texpostlude done');D*/
}


void arrowhead(double pointx, double pointy, double tailx,
		      double taily)
{
  double x, y, r, ct;

  r = linlen(pointx - tailx, pointy - taily);
  if (r == 0.0)
    return;
  ct = Min(0.05, 0.5 * r) / r;
  x = pointx + ct * (tailx - pointx);
  y = pointy + ct * (taily - pointy);
  printf("\n\\put");
  wcoord(&output, x, y);
  printf("{\\vector");
  wrslope(pointx - x, pointy - y, true);
  putchar('}');
}


void texwrtext(nametype *tp, double x, double y)
{
  boolean A, B, L, R;

  if (tp == NULL)
    return;
  printf("\\put");
  checkjust(tp, &A, &B, &L, &R);
  if (A)
    wcoord(&output, x, y + 0.1);
  else if (B)
    wcoord(&output, x, y - 0.05);
  else
    wcoord(&output, x, y);
  checkjust(tp, &A, &B, &L, &R);
  putchar('{');
  printf("\\makebox(0,0)");
  if (L)
    printf("[l]");
  else if (R)
    printf("[r]");
  putchar('{');
  texstacktext(tp);
  printf("}}\n");
}


void p2ahead(postype *point, postype shaft, double ht)
{
  postype butx;

  /*D; if debuglevel > 0 then begin
      write(log,'p2ahead point='); wpair(log,point.xpos,point.ypos);
      write(log,' shaft='); wpair(log,shaft.xpos,shaft.ypos);
      butx := affang(shaft,point);
      write(log,' affang='); wpair(log,butx.xpos,butx.ypos);
      wlogfl('ht',ht,1) end; D*/
  /* affang contains direction cosines */
  butx = affine(ht, 0.0, *point, affang(shaft, *point));
  /*D; if debuglevel > 0 then begin
      write(log,' butx='); wpair(log,butx.xpos,butx.ypos);
      writeln(log) end ;D*/
  printf("\\put");
  wpos(butx);
  printf("{\\vector");
  wrslope(point->xpos - butx.xpos, point->ypos - butx.ypos, true);
  printf("}\n");
  *point = butx;
}


void p2setthick(double lt)
{
  if (lt < 0.0 || lt == lastthick)
    return;
  printf("\\linethickness{");
  wfloat(&output, lt);
  printf("pt}\n");
  lastthick = lt;
}


void texdraw(primitive *node)
{
  int i, npts, lsp;
  double r, x, y, x1, y1, ct, st, lgth, lth;
  primitive *tn, *p;
  postype tmpat, X0, X3;
  int TEMP;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    if (drawn(node, lsp, -1.0) || node->textp != NULL) {
      initnesw();
      nesw(node);
      if (drawmode == pict2e)
	p2setthick(lth);
      printf("\\put");
      wcoord(&output, west, south);
      if (node->ptype == XBLOCK)
	printf("{\\makebox");
      else if (lsp == XLsolid)
	printf("{\\framebox");
      else if (lsp == XLinvis)
	printf("{\\makebox");
      else if (lsp == XLdashed) {
	printf("{\\dashbox");
	if (node->lparam != mdistmax)
	  wbrace(node->lparam / fsc);
	else
	  wbrace(venv(node, XLdashwid) / fsc);
      } else if (lsp == XLdotted) {
	printf("{\\dashbox");
	printf("{0.01}");
      }
      if (node->ptype == XLbox)
	wcoord(&output, node->UU.Ubox.boxwidth, node->UU.Ubox.boxheight);
      else
	wcoord(&output, node->UU.Ublock.blockwidth, node->UU.Ublock.blockheight);
      putchar('{');
      texstacktext(node->textp);
      printf("}}\n");
    }
    break;

  case XLline:
  case XLarrow:
    if (drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {  /* first segment */
	if (drawmode == pict2e)
	  p2setthick(lth);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  p2ahead(&node->aat, node->UU.Uline.endpos,
		  qenv(node, XLarrowht, tn->UU.Uline.height));
      }
      TEMP = ahlex(node->UU.Uline.atype);
      if ((node->son == NULL) & (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD))
	p2ahead(&node->UU.Uline.endpos, node->aat,
		qenv(node, XLarrowht, tn->UU.Uline.height));
      lgth = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		    node->UU.Uline.endpos.ypos - node->aat.ypos);
      if (drawmode == pict2e ||
	  lsp == XLsolid && (lgth > 0.18 || drawmode == tTeX)) {
	if (lgth > 0) {
	  printf("\\put");
	  wpos(node->aat);
	  printf("{\\line");
	  wrslope(node->UU.Uline.endpos.xpos - node->aat.xpos,
		  node->UU.Uline.endpos.ypos - node->aat.ypos, false);
	  printf("}\n");
	}
      } else if (lsp == XLsolid) {
	npts = (long)floor(lgth / 0.005 + 0.5);
	printf("\\multiput");
	wpos(node->aat);
	wcoord(&output, (node->UU.Uline.endpos.xpos - node->aat.xpos) / npts,
	       (node->UU.Uline.endpos.ypos - node->aat.ypos) / npts);
	printf("{%d}", npts + 1);
	printf("{\\makebox(0.005555,0.00825){\\tiny .}}\n");
      } else if (lsp == XLdashed) {
	printf("\\put");
	wpos(node->aat);
	printf("{\\dashline[50]");
	if (node->lparam != mdistmax)
	  wbrace(node->lparam / fsc);
	else
	  wbrace(venv(node, XLdashwid) / fsc);
	printf("(0,0)");
	wcoord(&output, node->UU.Uline.endpos.xpos - node->aat.xpos,
	       node->UU.Uline.endpos.ypos - node->aat.ypos);
	printf("}\n");
      } else if (lsp == XLdotted) {
	printf("\\dottedline{");
	wfloat(&output, 0.05 / fsc);
	putchar('}');
	wpos(node->aat);
	wpos(node->UU.Uline.endpos);
	putchar('\n');
      }
    }
    texwrtext(node->textp, 0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLspline:
    if ((drawmode == pict2e) & drawn(node, lsp, -1.0)) {
      if (!isthen(node)) {  /* first spline */
	spltot = primdepth(node);
	splcount = spltot;
	tmpat = node->aat;
	p2setthick(lth);
	TEMP = ahlex(tn->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD)
	  p2ahead(&node->aat, node->UU.Uline.endpos,
		  qenv(node, XLarrowht, tn->UU.Uline.height));
	/* p2lineopts(lsp,lparam,outlinep); */
	if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	  printf("\\put");
	  wcoord(&output, node->aat.xpos, node->aat.ypos);
	  printf("{\\line");
	  wrslope((node->UU.Uline.endpos.xpos - node->aat.xpos) / 2,
		  (node->UU.Uline.endpos.ypos - node->aat.ypos) / 2, false);
	  printf("}\n");
	  printf("\\cbezier");
	  wprop(node->UU.Uline.endpos, node->aat, 1.0, 1.0, 2.0);
	  wprop(node->aat, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
	} else if (spltot > 1) {
	  printf("\\cbezier");
	  wpos(node->aat);
	  node->aat = tmpat;
	  wprop(node->aat, node->UU.Uline.endpos, 1 - node->UU.Uline.aradius,
		node->UU.Uline.aradius, 1.0);
	}
      } else if (splcount > 1 && node->UU.Uline.aradius == mdistmax) {
	wprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	printf("%%\n");
	printf("\\cbezier");
	wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	wprop(node->aat, node->UU.Uline.endpos, 1.0, 5.0, 6.0);
      } else if (splcount > 1) {
	wprop(node->aat, node->UU.Uline.endpos,
	      0.5 + node->UU.Uline.aradius / 2,
	      0.5 - node->UU.Uline.aradius / 2, 1.0);
	wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	printf("%%\n");
	printf("\\cbezier");
	wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	wprop(node->aat, node->UU.Uline.endpos,
	      0.5 - node->UU.Uline.aradius / 2,
	      0.5 + node->UU.Uline.aradius / 2, 1.0);
      }
      if (splcount == 1) {
	tmpat = node->UU.Uline.endpos;
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	  x = linlen(node->UU.Uline.endpos.xpos - node->aat.xpos,
		     node->UU.Uline.endpos.ypos - node->aat.ypos);
	  y = qenv(node, XLarrowht, node->UU.Uline.height);
	  pprop(node->aat, &node->UU.Uline.endpos, y, x - y, x);
	}
	if (spltot > 1 && node->UU.Uline.aradius == mdistmax) {
	  wprop(node->aat, node->UU.Uline.endpos, 5.0, 1.0, 6.0);
	  wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	  printf("%%\n");
	  printf("\\put");
	  wprop(node->aat, node->UU.Uline.endpos, 1.0, 1.0, 2.0);
	  printf("{\\line");
	  wrslope((node->UU.Uline.endpos.xpos - node->aat.xpos) / 2,
		  (node->UU.Uline.endpos.ypos - node->aat.ypos) / 2, false);
	  printf("}\n");
	} else if (spltot > 1) {
	  wprop(node->aat, node->UU.Uline.endpos, node->UU.Uline.aradius,
		1 - node->UU.Uline.aradius, 1.0);
	  wpos(node->UU.Uline.endpos);
	  printf("%%\n");
	}
	TEMP = ahlex(node->UU.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)
	  p2ahead(&tmpat, node->aat,
		  qenv(node, XLarrowht, node->UU.Uline.height));
      }
      node->ptype = XLline;
      /*D; if debuglevel > 0 then writeln(log,'node ',splcount:1) ;D*/
      splcount--;
    } else if (lsp == XLdotted || lsp == XLdashed || lsp == XLsolid) {
      if ((!isthen(node)) & ((ahlex(node->UU.Uline.atype) == XDOUBLEHEAD) |
			     (ahlex(node->UU.Uline.atype) == XLEFTHEAD)))
	    /* first spline */
	      arrowhead(node->aat.xpos, node->aat.ypos,
			node->UU.Uline.endpos.xpos, node->UU.Uline.endpos.ypos);
      printf("\\spline");
      wpos(node->aat);
      wpos(node->UU.Uline.endpos);
      if (node->son == NULL)
	putchar('\n');
      else if (node->son->son == NULL) {
	wpos(node->son->UU.Uline.endpos);
	if ((ahlex(node->UU.Uline.atype) == XRIGHTHEAD) |
	    (ahlex(node->UU.Uline.atype) == XDOUBLEHEAD))
	  arrowhead(node->son->UU.Uline.endpos.xpos,
		    node->son->UU.Uline.endpos.ypos, node->son->aat.xpos,
		    node->son->aat.ypos);
	node->ptype = XLline;
	putchar('\n');
	/* son := nil */
	deletetree(&node->son);
      } else {
	pprop(node->son->UU.Uline.endpos, &node->son->aat, 1.0, 1.0, 2.0);
	wpos(node->son->aat);
	node->ptype = XLline;
	putchar('\n');
      }
    }
    break;

  case XLmove:
    texwrtext(node->textp, 0.5 * (node->UU.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->UU.Uline.endpos.ypos));
    break;

  case XLellipse:
    if (drawmode == pict2e)
      p2setthick(lth);
    printf("\\put");
    wpos(node->aat);
    if (drawmode == TeX) {
      printf("{\\ellipse");
      wbrace(node->UU.Uellipse.elwidth / fsc);
      wbrace(node->UU.Uellipse.elheight / fsc);
      printf("}\n");
    } else {
      printf("{\\oval");
      wcoord(&output, node->UU.Uellipse.elwidth, node->UU.Uellipse.elheight);
      printf("}\n");
    }
    texwrtext(node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (lsp != XLinvis) {
      if (drawmode == pict2e)
	p2setthick(lth);
      printf("\\put");
      wpos(node->aat);
      printf("{\\circle");
      wbrace(2.0 * node->UU.Ucircle.radius / fsc);
      printf("}\n");
    }
    texwrtext(node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    if ((drawmode == pict2e) & drawn(node, lsp, -1.0)) {
      p2setthick(node->lthick);   /* p2linecap(lsp); */
      X0 = arcstart(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	if (node->UU.Uline.aradius == 0.0)
	  x = 0.0;
	else
	  x = atan(qenv(node, XLarrowht, node->UU.Uline.height) /
		   node->UU.Uline.aradius);
	if (node->UU.Uline.endpos.ypos >= 0.0) {
	  node->UU.Uline.endpos.xpos += x;
	  node->UU.Uline.endpos.ypos -= x;
	} else {
	  node->UU.Uline.endpos.xpos -= x;
	  node->UU.Uline.endpos.ypos += x;
	}
	tmpat = X0;
	X0 = arcstart(node);
	p2ahead(&tmpat, X0, qenv(node, XLarrowht, node->UU.Uline.height));
      }
      X3 = arcend(node);
      TEMP = ahlex(node->UU.Uline.atype);
      if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	if (node->UU.Uline.aradius == 0.0)
	  x = 0.0;
	else
	  x = atan(qenv(node, XLarrowht, node->UU.Uline.height) /
		   node->UU.Uline.aradius);
	if (node->UU.Uline.endpos.ypos >= 0.0)
	  node->UU.Uline.endpos.ypos -= x;
	else
	  node->UU.Uline.endpos.ypos += x;
	tmpat = X3;
	X3 = arcend(node);
	p2ahead(&tmpat, X3, qenv(node, XLarrowht, node->UU.Uline.height));
      }
      npts = (long)(1.0 + fabs(node->UU.Uline.endpos.ypos) / pi);
      node->UU.Uline.endpos.ypos /= npts;
      while (npts > 0) {
	printf("\\cbezier");
	wpos(X0);
	x = cos(node->UU.Uline.endpos.ypos / 2);
	y = sin(node->UU.Uline.endpos.ypos / 2);
	x1 = (4 - x) / 3;
	if (y != 0.0)
	  y1 = (1.0 - x * x1) / y;
	else
	  y1 = 0.0;
	tmpat.xpos = cos(
	    node->UU.Uline.endpos.xpos + node->UU.Uline.endpos.ypos / 2.0);
	tmpat.ypos = sin(
	    node->UU.Uline.endpos.xpos + node->UU.Uline.endpos.ypos / 2.0);
	wpos(affine(node->UU.Uline.aradius * x1, -node->UU.Uline.aradius * y1,
		    node->aat, tmpat));
	wpos(affine(node->UU.Uline.aradius * x1, node->UU.Uline.aradius * y1,
		    node->aat, tmpat));
	if (npts > 1) {
	  node->UU.Uline.endpos.xpos += node->UU.Uline.endpos.ypos;
	  X0 = arcstart(node);
	  wpos(X0);
	  printf("%%\n");
	}
	npts--;
      }
      wpos(X3);
      printf("%%\n");   /*; p2dashdot(lsp,lparam,outlinep) */
    } else if (iscorner(principal(node->UU.Uline.endpos.xpos, 0.5 * pi)) &&
	       fabs(fabs(node->UU.Uline.endpos.ypos) - pi / 2.0) < 0.001) {
      if (drawmode == pict2e)
	p2setthick(lth);
      printf("\\put");
      wpos(node->aat);
      printf("{\\oval");
      wcoord(&output, 2 * node->UU.Uline.aradius / fsc,
	     2 * node->UU.Uline.aradius / fsc);
      if (node->direction != XLdown && node->direction != XLup &&
	  node->direction != XLright && node->direction != XLleft) {
	p = findenv(node);
	if (p != NULL)
	  node->direction = p->direction;
      }
      if (node->direction == XLleft && node->UU.Uline.endpos.ypos < 0.0 ||
	  node->direction == XLdown && node->UU.Uline.endpos.ypos > 0.0)
	printf("[bl]}\n");
      else if (node->direction == XLleft && node->UU.Uline.endpos.ypos > 0.0 ||
	       node->direction == XLup && node->UU.Uline.endpos.ypos < 0.0)
	printf("[tl]}\n");
      else if (node->direction == XLright && node->UU.Uline.endpos.ypos < 0.0 ||
	       node->direction == XLup && node->UU.Uline.endpos.ypos > 0.0)
	printf("[tr]}\n");
      else
	printf("[br]}\n");
      texwrtext(node->textp, node->aat.xpos, node->aat.ypos);
    } else if (((1L << ((long)drawmode)) &
		((1L << ((long)TeX)) | (1L << ((long)pict2e)))) != 0 &&
	       node->UU.Uline.aradius > 0.0) {
      if (node->UU.Uline.aradius >= 0.05 / sin(pi / 18.0)) {
	npts = (long)floor(fabs(node->UU.Uline.endpos.ypos) / (pi / 18.0) + 0.5);
	if (npts < 4)
	  npts = 4;
      } else
	npts = (long)floor(fabs(node->UU.Uline.endpos.ypos) / atan(
			     0.05 / node->UU.Uline.aradius) + 0.5);
      r = node->UU.Uline.endpos.ypos / npts;
      ct = cos(r);
      st = sin(r);
      x = node->UU.Uline.aradius * cos(node->UU.Uline.endpos.xpos);
      y = node->UU.Uline.aradius * sin(node->UU.Uline.endpos.xpos);
      /*D if debuglevel = 2 then writeln(log,'x,y',x:7:4,y:7:4); D*/
      for (i = 1; i <= npts; i++) {
	x1 = ct * x - st * y;
	y1 = st * x + ct * y;
	/*D if debuglevel = 2 then writeln(log,x1:7:4,y1:7:4); D*/
	printf("\\put");
	wcoord(&output, node->aat.xpos + x, node->aat.ypos + y);
	printf("{\\line");
	wcoord(&output, x1 - x, y1 - y);
	if (fabs(x1 - x) != 0.0)
	  wbrace(fabs(x1 - x) / fsc);
	else
	  wbrace(fabs(y1 - y) / fsc);
	printf("}\n");
	if ((i == 1) & ((ahlex(node->UU.Uline.atype) == XDOUBLEHEAD) |
			(ahlex(node->UU.Uline.atype) == XLEFTHEAD)))
	  arrowhead(node->aat.xpos + x, node->aat.ypos + y,
		    node->aat.xpos + x1, node->aat.ypos + y1);
	else if ((i == npts) & ((ahlex(node->UU.Uline.atype) == XDOUBLEHEAD) |
				(ahlex(node->UU.Uline.atype) == XRIGHTHEAD)))
	  arrowhead(node->aat.xpos + x1, node->aat.ypos + y1,
		    node->aat.xpos + x, node->aat.ypos + y);
	x = x1;
	y = y1;
      }
      texwrtext(node->textp, node->aat.xpos, node->aat.ypos);
    }
    break;

  case XLstring:
    texwrtext(node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
      wstring(&output, node->textp);
      putchar('\n');
    }
    break;
  }
  /*DUGHM ; flush(output) MHGUD*/
}  /* texdraw */


void treedraw(primitive *node)
{
  while (node != NULL) {
    switch (drawmode) {

    case TeX:
    case tTeX:
    case pict2e:
      texdraw(node);
      break;

    case PGF:
      pgfdraw(node);
      break;

    case PSTricks:
      pstdraw(node);
      break;

    case MFpic:
      mfpdraw(node);
      break;

    case PS:
    case PSfrag:
      psdraw(node);
      break;

    /* PSmps: mpsdraw(node); */
    case MPost:
      mpodraw(node);
      if (sfill)
	mpodraw(node);
      break;

    case SVG:
      svgdraw(node);
      break;

    case xfig:
      xfigdraw(node);
      break;
    }
    if (node->son != NULL)
      treedraw(node->son);
    if (drawmode == xfig) {
      if (node->ptype == XBLOCK && node->direction == -1)
	printf("-6\n");
    }
    sfill = false;
    sshade = NULL;
    soutline = NULL;
    node = node->next_;
  }
  /*D; if debuglevel > 0 then writeln(log,' treedraw done') D*/
}


/*
procedure treedraw( node: primitivep ); */
/* without recursion */
/*
var nd: primitivep;
begin
   if node <> nil then nd := node^.parent else nd := nil;
   while node <> nd do if node^.ptype < XLlastenv then begin
      case drawmode of
         TeX,tTeX,pict2e: texdraw(node);
         PGF: pgfdraw(node);
         PSTricks: pstdraw(node);
         MFpic: mfpdraw(node);
         PS,PSfrag: psdraw(node);
         */
/* PSmps: mpsdraw(node); */
/*
         MPost: begin mpodraw(node); if sfill then mpodraw(node) end;
         SVG: svgdraw(node);
         xfig: xfigdraw(node)
         end;
      if node^.son <> nil then begin
         node^.ptype := node^.ptype + XLlastenv;
         node := node^.son
         end
      else if node^.next <> nil then begin
         node^.ptype := node^.ptype + 2*XLlastenv;
         node := node^.next
         end
      else node := node^.parent
      end
   else if node^.ptype < 2*XLlastenv then begin
      if drawmode <> xfig then begin end
      else if (node^.ptype = (XBLOCK + XLlastenv))
         and (node^.direction = -1) then writeln('-6');
      sfill := false; sshade := nil; soutline := nil;
      if node^.next <> nil then begin
         node^.ptype := node^.ptype + XLlastenv;
         node := node^.next
         end
      else begin
         node^.ptype := node^.ptype - XLlastenv;
         node := node^.parent
         end
      end
   else begin
      node^.ptype := node^.ptype - 2*XLlastenv;
      node := node^.parent
      end
   */
/* D; if debuglevel > 0 then writeln(log,' treedraw done') D */
/*
   end; */

void drawtree(double n, double s, double e, double w, primitive *eb)
{
  /* n,s,e,w: real; eb: primitivep */
  double fsctmp;

  sfill = false;
  sshade = NULL;
  soutline = NULL;
  switch (drawmode) {

  case SVG:
    fsctmp = fsc;
    fsc /= SVGPX;   /* output pixels */
    svgprelude(n, s, e, w, venv(eb, XLlinethick) / 72 * scale);
    treedraw(eb);
    svgpostlude();
    fsc = fsctmp;
    break;

  case PGF:   /* n,s,e,w */
    pgfprelude();
    treedraw(eb);
    pgfpostlude();
    break;

  case PSTricks:
    pstprelude(n, s, e, w);
    treedraw(eb);
    pstpostlude();
    break;

  case MFpic:
    mfpprelude(n, s, e, w);
    treedraw(eb);
    mfppostlude();
    break;

  case PS:
  case PSfrag:
    fsctmp = fsc;
    fsc /= 72;   /* output points */
    psprelude(n, s, e, w, venv(eb, XLlinethick));
    treedraw(eb);
    pspostlude();
    fsc = fsctmp;
    break;

  /* PSmps: begin
     fsctmp := fsc; fsc := fsc/72;
     mpsprelude(n,s,e,w);
     treedraw( eb );
     mpspostlude;
     fsc := fsctmp
     end; */
  case MPost:
    fsctmp = fsc;
    fsc /= 72;   /* output points */
    /* n,s,e,w */
    mpoprelude();
    treedraw(eb);
    mpopostlude();
    fsc = fsctmp;
    break;

  case xfig:
    xfigprelude();
    treedraw(eb);
    /* xfigpostlude */
    break;

  case TeX:
  case tTeX:
  case pict2e:
    if (s != distmax) {
      texprelude(n, s, e, w);
      treedraw(eb);
      texpostlude();
    }
    break;
  }
}


/*B.B*/
/* onefile */
/* G end. G */
/* onefile */
/*GH#include 'dpic2.p'HG*/

/*--------------------------------------------------------------------*/

/*DUGHM
procedure openlogfile;
begin
   rewrite(log,'log');
   writeln(log);
   writeln(log,'Dpic log file')
   end; MHGUD*/


int checkfile(Char *ifn, boolean isverbose)
{
  int cf, i, j;

  j = FILENAMELEN;
  i = 0;
  while (j > i) {
    if (ifn[j - 1] == ' ')
      j--;
    else
      i = j;
  }
  if (j < FILENAMELEN)
    j++;
  else
    fatal(1);
  ifn[j - 1] = '\0';
  cf = access(ifn, 4);
  /*G cf := 4-Access(ifn,4); G*/
  if (!(isverbose && cf != 0))
    return cf;
  fprintf(errout, " *** dpic: Searching for file \"");
  for (i = 0; i <= j - 2; i++)
    putc(ifn[i], errout);
  fprintf(errout, "\" returns %d\n", cf);
  return cf;
}


void openerror(void)
{
  errout = stderr;
}


void openfiles(void)
{
  /*DUGHM if (oflag>0) then openlogfile; MHGUD*/
  savebuf = NULL;
  output = stdout; input = stdin;

  if (argct >= P_argc)
    return;

  P_sun_argv(infname, sizeof(mstring), argct);
  /*GH if argct <= ParamCount then begin
     infname := ParamStr(argct); HG*/

  if (checkfile(infname, true) != 0)
    fatal(1);
  if (input != NULL)
    input = freopen(P_trimname(infname, sizeof(mstring)), "r", input);
  else
    input = fopen(P_trimname(infname, sizeof(mstring)), "r");
  if (input == NULL)
    _EscIO(FileNotFound);
}


/*B
procedure openerror;
begin
 rewrite(errout)
 end;
procedure openfiles;
begin
 B*/
/*DBrewrite(log);BD*/
/*B
   reset(dpictabl)
   end;
B*/

/* X
procedure openerror;
begin
   rewrite(errout,'NAME=/dev/tty')
   end;
procedure openfiles;
begin
   infname := substr(parms,1,length(parms)-1);
   reset(input,'NAME='||infname);
   reset(dpictabl,'NAME=DPICTABL')
   end;
X */

/*--------------------------------------------------------------------*/
/*D procedure wrchar( c: char );
begin
   write(log,'ch(',ord(c):1,')="');
   case ord(c) of
      ordETX: write(log,'^C');
      ordNL: writeln(log);
      ordCR: write(log,'\r');
      ordTAB: write(log,'\t');
      otherwise write(log,c)
      end;
   write(log,'"')
   end; D*/

/*D procedure wlogfl D*/
/* nm: string; v: real; cr: integer */
/*D;
begin
   write(log,' ',nm,'=');
   if v=MaxReal then write(log,'MaxReal')
   else if v= -MaxReal then write(log,'-MaxReal')
   else wfloat(log,v);
   if cr<>0 then writeln(log)
   end; D*/

/*D procedure wrbuf D*/
/* p: fbufferp; job,r: integer */
/*D;
var i,j: integer;
begin
  if p=nil then write(log,' nil buffer ')
  else with p^ do begin
     if job > 2 then begin
        write(log,' buf[');
        if p^.prevb<>nil then write(log,ordp(p^.prevb):1);
        write(log,'<'); write(log,ordp(p):1,'>');
        if p^.nextb<>nil then write(log,ordp(p^.nextb):1);
        write(log,']')
        end;
     if job > 1 then
       write(log,' readx=',readx:1,' savedlen=',savedlen:1,' attrib=',attrib:1);
     if r = 0 then j := 1 else if r < 0 then j := -r else j := readx;
     if job > 0 then write(log,'(',j:1,',',savedlen:1,')');
     writeln(log);write(log,'|');
     if carray = nil then write(log,'nil')
     else for i:=j to savedlen do case ord(carray^[i]) of
        ordETX: write(log,'^C');
        ordNL: writeln(log);
        ordCR: write(log,'\r');
        ordTAB: write(log,'\t');
        otherwise write(log,carray^[i])
        end;

     writeln(log,'|')
     end
  end; D*/
/*--------------------------------------------------------------------*/

void writeerror(void)
{
  int inx, startlin, emi, j, FORLIM, FORLIM1;

  if (errmp <= 0)
    return;
  /*D if debuglevel > 0 then begin write(log,'writeerror[');
      if inbuf=nil then write(log,'nil') else write(log,ordp(inbuf):1);
      writeln(log,']') end; D*/
  /* Write current line */
  currentline(&startlin);
  /* Put the error numbers below it */
  emi = startlin + 1;
  FORLIM = errmp;
  for (inx = 1; inx <= FORLIM; inx++) {
    FORLIM1 = errmsg[inx].col;
    for (j = emi; j < FORLIM1; j++)
      putc(' ', errout);
    fprintf(errout, "%d", inx);
    emi = errmsg[inx].col + 1;
  }
  putc('\n', errout);
  consoleflush();
  FORLIM = errmp;
  /* Error message for each error */
  for (inx = 1; inx <= FORLIM; inx++) {
    emi = errmsg[inx].no;
    fprintf(errout, "dpic: line %d ", lineno);
    if (emi < 900)
      fprintf(errout, "ERROR");
    else
      fprintf(errout, "WARNING");
    fprintf(errout, " %d: ", inx);
    if (emi < 800) {
      switch (errmsg[inx].symb_) {

      case XLname:
	fprintf(errout, "Name");
	break;

      case XLabel:
	fprintf(errout, "Label");
	break;

      case XLaTeX:
	fprintf(errout, "Backslash not followed by line end (LaTeX macro?)");
	break;

      case XLfloat:
	fprintf(errout, "Float");
	break;

      case XERROR:
	fprintf(errout, "Error");
	break;
	/* include controlerr.i */

      case 4:
	putc('<', errout);
	break;

      case 5:
	fprintf(errout, "cw");
	break;

      case 6:
	fprintf(errout, "ccw");
	break;

      case 7:
	putc('(', errout);
	break;

      case 8:
	fprintf(errout, "Possibly unbalanced parentheses: )");
	break;

      case 9:
	putc('*', errout);
	break;

      case 10:
	putc('+', errout);
	break;

      case 11:
	putc('-', errout);
	break;

      case 12:
	putc('/', errout);
	break;

      case 13:
	putc('%', errout);
	break;

      case 14:
	fprintf(errout, "; or end of line");
	break;

      case 15:
	putc('^', errout);
	break;

      case 16:
	putc('!', errout);
	break;

      case 17:
	fprintf(errout, "&&");
	break;

      case 18:
	fprintf(errout, "||");
	break;

      case 19:
	putc(',', errout);
	break;

      case 20:
	putc(':', errout);
	break;

      case 21:
	putc('[', errout);
	break;

      case 22:
	fprintf(errout, "Possibly unbalanced brackets: ]");
	break;

      case 23:
	putc('{', errout);
	break;

      case 24:
	fprintf(errout, "Possibly unbalanced braces: }");
	break;

      case 25:
	putc('.', errout);
	break;

      case 26:
	fprintf(errout, "[]");
	break;

      case 27:
	putc('`', errout);
	break;

      case 28:
	putc('\'', errout);
	break;

      case 29:
	putc('=', errout);
	break;

      case 30:
	fprintf(errout, ":=");
	break;

      case 31:
	fprintf(errout, "+=");
	break;

      case 32:
	fprintf(errout, "-=");
	break;

      case 33:
	fprintf(errout, "*=");
	break;

      case 34:
	fprintf(errout, "/=");
	break;

      case 35:
	fprintf(errout, "%%=");
	break;

      case 36:
	putc('&', errout);
	break;

      case 41:
	fprintf(errout, "\" or string");
	break;

      case 42:
	putc('#', errout);
	break;

      case 43:
	fprintf(errout, "$ or argument reference");
	break;

      case 46:
	fprintf(errout, "height or ht");
	break;

      case 47:
	fprintf(errout, "wid or width");
	break;

      case 48:
	fprintf(errout, "rad or radius");
	break;

      case 49:
	fprintf(errout, "diam or diameter");
	break;

      case 50:
	fprintf(errout, "thick or thickness");
	break;

      case 51:
	fprintf(errout, "scaled");
	break;

      case 52:
	fprintf(errout, "from");
	break;

      case 53:
	fprintf(errout, "to");
	break;

      case 54:
	fprintf(errout, "at");
	break;

      case 55:
	fprintf(errout, "with");
	break;

      case 56:
	fprintf(errout, "by");
	break;

      case 57:
	fprintf(errout, "then");
	break;

      case 58:
	fprintf(errout, "continue");
	break;

      case 59:
	fprintf(errout, "chop");
	break;

      case 60:
	fprintf(errout, "same");
	break;

      case 61:
	fprintf(errout, "of");
	break;

      case 62:
	fprintf(errout, "the");
	break;

      case 63:
	fprintf(errout, "way");
	break;

      case 64:
	fprintf(errout, "between");
	break;

      case 65:
	fprintf(errout, "and");
	break;

      case 66:
	fprintf(errout, "Here");
	break;

      case 67:
	fprintf(errout, "nd or rd or st or th");
	break;

      case 68:
	fprintf(errout, "last");
	break;

      case 69:
	fprintf(errout, "fill or filled");
	break;

      case 70:
	fprintf(errout, ".x");
	break;

      case 71:
	fprintf(errout, ".y");
	break;

      case 72:
	fprintf(errout, "print");
	break;

      case 73:
	fprintf(errout, "copy");
	break;

      case 74:
	fprintf(errout, "reset");
	break;

      case 75:
	fprintf(errout, "exec");
	break;

      case 76:
	fprintf(errout, "sh");
	break;

      case 77:
	fprintf(errout, "command");
	break;

      case 78:
	fprintf(errout, "define");
	break;

      case 79:
	fprintf(errout, "undef or undefine");
	break;

      case 80:
	fprintf(errout, "rand");
	break;

      case 81:
	fprintf(errout, "if");
	break;

      case 82:
	fprintf(errout, "else");
	break;

      case 83:
	fprintf(errout, "for");
	break;

      case 84:
	fprintf(errout, "do");
	break;

      case 86:
	fprintf(errout, "sprintf");
	break;

      case 88:
	fprintf(errout, ".ne");
	break;

      case 89:
	fprintf(errout, ".se");
	break;

      case 90:
	fprintf(errout, ".nw");
	break;

      case 91:
	fprintf(errout, ".sw");
	break;

      case 92:
	fprintf(errout, ".n or .north or .t or .top or top");
	break;

      case 93:
	fprintf(errout, ".b or .bot or .bottom or .s or .south or bottom");
	break;

      case 94:
	fprintf(errout, ".e or .east or .r or .right");
	break;

      case 95:
	fprintf(errout, ".l or .left or .w or .west");
	break;

      case 96:
	fprintf(errout, ".start or start");
	break;

      case 97:
	fprintf(errout, ".end or end");
	break;

      case 98:
	fprintf(errout, ".c or .center or .centre");
	break;

      case 100:
	fprintf(errout, "==");
	break;

      case 101:
	fprintf(errout, "!=");
	break;

      case 102:
	fprintf(errout, ">=");
	break;

      case 103:
	fprintf(errout, "<=");
	break;

      case 104:
	putc('>', errout);
	break;

      case 106:
	fprintf(errout, ".height or .ht");
	break;

      case 107:
	fprintf(errout, ".wid or .width");
	break;

      case 108:
	fprintf(errout, ".rad or .radius");
	break;

      case 109:
	fprintf(errout, ".diam or .diameter");
	break;

      case 110:
	fprintf(errout, ".thick or .thickness");
	break;

      case 111:
	fprintf(errout, ".len or .length");
	break;

      case 113:
	fprintf(errout, "abs");
	break;

      case 114:
	fprintf(errout, "acos");
	break;

      case 115:
	fprintf(errout, "asin");
	break;

      case 116:
	fprintf(errout, "cos");
	break;

      case 117:
	fprintf(errout, "exp");
	break;

      case 118:
	fprintf(errout, "expe");
	break;

      case 119:
	fprintf(errout, "int");
	break;

      case 120:
	fprintf(errout, "log");
	break;

      case 121:
	fprintf(errout, "loge");
	break;

      case 122:
	fprintf(errout, "sign");
	break;

      case 123:
	fprintf(errout, "sin");
	break;

      case 124:
	fprintf(errout, "sqrt");
	break;

      case 125:
	fprintf(errout, "tan");
	break;

      case 126:
	fprintf(errout, "floor");
	break;

      case 128:
	fprintf(errout, "atan2");
	break;

      case 129:
	fprintf(errout, "max");
	break;

      case 130:
	fprintf(errout, "min");
	break;

      case 131:
	fprintf(errout, "pmod");
	break;

      case 133:
	fprintf(errout, "solid");
	break;

      case 134:
	fprintf(errout, "dotted");
	break;

      case 135:
	fprintf(errout, "dashed");
	break;

      case 136:
	fprintf(errout, "invis or invisible");
	break;

      case 137:
	fprintf(errout, "path");
	break;

      case 139:
	fprintf(errout, "color or colored or colour or coloured");
	break;

      case 140:
	fprintf(errout, "outline or outlined");
	break;

      case 141:
	fprintf(errout, "shade or shaded");
	break;

      case 143:
	fprintf(errout, "center or centre");
	break;

      case 144:
	fprintf(errout, "ljust");
	break;

      case 145:
	fprintf(errout, "rjust");
	break;

      case 146:
	fprintf(errout, "above");
	break;

      case 147:
	fprintf(errout, "below");
	break;

      case 149:
	fprintf(errout, "<-");
	break;

      case 150:
	fprintf(errout, "->");
	break;

      case 151:
	fprintf(errout, "<->");
	break;

      case 153:
	fprintf(errout, "up");
	break;

      case 154:
	fprintf(errout, "down");
	break;

      case 155:
	fprintf(errout, "right");
	break;

      case 156:
	fprintf(errout, "left");
	break;

      case 158:
	fprintf(errout, "box");
	break;

      case 159:
	fprintf(errout, "circle");
	break;

      case 160:
	fprintf(errout, "ellipse");
	break;

      case 161:
	fprintf(errout, "arc");
	break;

      case 162:
	fprintf(errout, "line");
	break;

      case 163:
	fprintf(errout, "arrow");
	break;

      case 164:
	fprintf(errout, "move");
	break;

      case 165:
	fprintf(errout, "spline");
	break;

      case 167:
	fprintf(errout, "arcrad");
	break;

      case 168:
	fprintf(errout, "arrowht");
	break;

      case 169:
	fprintf(errout, "arrowwid");
	break;

      case 170:
	fprintf(errout, "boxht");
	break;

      case 171:
	fprintf(errout, "boxrad");
	break;

      case 172:
	fprintf(errout, "boxwid");
	break;

      case 173:
	fprintf(errout, "circlerad");
	break;

      case 174:
	fprintf(errout, "dashwid");
	break;

      case 175:
	fprintf(errout, "ellipseht");
	break;

      case 176:
	fprintf(errout, "ellipsewid");
	break;

      case 177:
	fprintf(errout, "lineht");
	break;

      case 178:
	fprintf(errout, "linewid");
	break;

      case 179:
	fprintf(errout, "moveht");
	break;

      case 180:
	fprintf(errout, "movewid");
	break;

      case 181:
	fprintf(errout, "textht");
	break;

      case 182:
	fprintf(errout, "textoffset");
	break;

      case 183:
	fprintf(errout, "textwid");
	break;

      case 184:
	fprintf(errout, "arrowhead");
	break;

      case 185:
	fprintf(errout, "fillval");
	break;

      case 186:
	fprintf(errout, "linethick");
	break;

      case 187:
	fprintf(errout, "maxpsht");
	break;

      case 188:
	fprintf(errout, "maxpswid");
	break;

      case 189:
	fprintf(errout, "scale");
	break;
	/*B%include controlerrB*/

      default:
	fprintf(errout, "Punctuation characters");
	break;
      }
      fprintf(errout, " found.\n");
      fprintf(errout, " The following were expected:\n");
    }

    switch (emi) {

    /* Expected lexical symbols */
    /* include parserr.i */
    case 1:
      fprintf(errout, " EOF .PS\n");
      break;

    case 2:
      fprintf(errout, " + - ! constant variable ( function location\n");
      break;

    case 3:
      fprintf(errout, " th\n");
      break;

    case 4:
      fprintf(errout, " of Label enumerated obj\n");
      break;

    case 5:
      fprintf(errout, " primitiv [] string [\n");
      break;

    case 6:
      fprintf(errout, " ]\n");
      break;

    case 7:
      fprintf(errout, " (\n");
      break;

    case 8:
      fprintf(errout, " ) + - ! constant variable ( function location\n");
      break;

    case 9:
      fprintf(errout, " Label enumerated obj\n");
      break;

    case 10:
      fprintf(errout, " .x .y * /\n");
      break;

    case 11:
      fprintf(errout, " ! constant variable ( function location\n");
      break;

    case 12:
      fprintf(errout,
	" variable + - ( corner Here ! string Label float function sprintf enumerated obj\n");
      break;

    case 13:
      fprintf(errout, " string sprintf\n");
      break;

    case 14:
      fprintf(errout, " )\n");
      break;

    case 15:
      fprintf(errout, " constant variable ( function location\n");
      break;

    case 16:
      fprintf(errout, " ( corner Here Label enumerated obj\n");
      break;

    case 17:
      fprintf(errout, " between of < , + -\n");
      break;

    case 18:
      fprintf(errout, " ,\n");
      break;

    case 19:
      fprintf(errout, " logic opr\n");
      break;

    case 20:
      fprintf(errout, " the\n");
      break;

    case 21:
      fprintf(errout, " way\n");
      break;

    case 22:
      fprintf(errout, " between\n");
      break;

    case 23:
      fprintf(errout, " and\n");
      break;

    case 24:
      fprintf(errout, " ) ,\n");
      break;

    case 25:
      fprintf(errout, " variable\n");
      break;

    case 26:
      fprintf(errout, " =\n");
      break;

    case 27:
      fprintf(errout, " ) ||\n");
      break;

    case 28:
      fprintf(errout, " ) + -\n");
      break;

    case 29:
      fprintf(errout, " , + -\n");
      break;

    case 30:
      fprintf(errout, " ] + -\n");
      break;

    case 31:
      fprintf(errout, " ' + -\n");
      break;

    case 32:
      fprintf(errout, " } + -\n");
      break;

    case 33:
      fprintf(errout, " ;\n");
      break;

    case 34:
      fprintf(errout, " [ []\n");
      break;

    case 35:
      fprintf(errout, " to ,\n");
      break;

    case 36:
      fprintf(errout, " + - do by\n");
      break;

    case 37:
      fprintf(errout, " do + -\n");
      break;

    case 38:
      fprintf(errout, " if\n");
      break;

    case 39:
      fprintf(errout, " then ||\n");
      break;

    case 40:
      fprintf(errout, " name Label\n");
      break;

    case 41:
      fprintf(errout,
	" at corner . + - ( Here ! Label constant variable function enumerated obj\n");
      break;

    case 42:
      fprintf(errout, " at\n");
      break;

    case 43:
      fprintf(errout, " at of Label enumerated obj\n");
      break;

    case 44:
      fprintf(errout, " {\n");
      break;

    case 45:
      fprintf(errout, " }\n");
      break;

    case 46:
      fprintf(errout, " envvar\n");
      break;

    case 47:
      fprintf(errout, " logic opr string sprintf\n");
      break;

    case 48:
      fprintf(errout, " }\n");
      break;

    case 49:
      fprintf(errout, " :\n");
      break;

    case 50:
      fprintf(errout, " .PE ;\n");
      break;
      /*B%include parserrB*/

    /* lexical error messages */
    case 800:
      fprintf(errout, "Character not recognized: ignored\n");
      break;

    case 801:
      fprintf(errout, "Null string not allowed\n");
      break;

    case 802:
      fprintf(errout, "Invalid exponent character after e in a number\n");
      break;

    case 803:
      fprintf(errout,
	      "Fill value must be non-negative and not greater than 1\n");
      break;

    case 804:
      fprintf(errout, "End of file while reading {...} contents\n");
      break;

    case 805:
      fprintf(errout, "Bad macro argument number\n");
      break;

    case 806:
      fprintf(errout, "End of file while evaluating macro argument\n");
      break;

    /* context error messages */
    case 851:
      fprintf(errout, "Variable not found\n");
      break;

    case 852:
      fprintf(errout, "Zero divisor not allowed\n");
      break;

    case 853:
      fprintf(errout, "Only one copy file may be open at any time\n");
      break;

    case 854:
      fprintf(errout, "Place name not found\n");
      break;

    case 855:
      fprintf(errout, "Internal name not found\n");
      break;

    case 856:
      fprintf(errout, "Invalid non-positive value for object count\n");
      break;

    case 857:
      fprintf(errout, "Enumerated or previous object not found\n");
      break;

    case 858:
      fprintf(errout, "This usage is inapplicable in this context\n");
      break;

    case 859:
      fprintf(errout, "File not readable\n");
      break;

    case 860:
      fprintf(errout, "Infinite looping not allowed\n");
      break;

    case 861:
      fprintf(errout, "Missing or blank string\n");
      break;

    case 862:
      fprintf(errout, "For limits must have the same sign for by *()\n");
      break;

    case 863:
      fprintf(errout, "Non-integer power of negative value\n");
      break;

    case 864:
      fprintf(errout, "Incorrect number of sprintf arguments\n");
      break;

    case 865:
      fprintf(errout, "Bad sprintf format\n");
      break;

    case 866:
      fprintf(errout, "String exceeds max length of 4095 characters\n");
      break;

    case 867:
      fprintf(errout, "Invalid log or sqrt argument\n");
      break;

    case 868:
      fprintf(errout, "Function argument out of bounds\n");
      break;

    case 869:
      fprintf(errout, "Improper use of logical operator\n");
      break;

    case 870:
      fprintf(errout, "Zero value of scale not allowed\n");
      break;

    case 871:
      fprintf(errout, "Zero second argument of pmod not allowed\n");
      break;

    case 872:
      fprintf(errout, "Buffer overflow while defining macro argument\n");
      break;

    /* lexical warning messages */
    case 901:
      fprintf(errout, "String character generated at end of line\n");
      break;

    case 902:
      fprintf(errout, "Input line exceeds buffer size of 4096 characters\n");
      break;

    case 903:
      fprintf(errout, "Picture size adjusted to maxpswid value\n");
      break;

    case 904:
      fprintf(errout, "Picture size adjusted to maxpsht value\n");
      break;

    case 906:
      fprintf(errout, "Safe mode: sh, copy, and print to file disallowed\n");
      break;
    }/* case */

    putc('\n', errout);
    consoleflush();
  }  /* for */
  if (errcount > MAXERRCOUNT)
    fatal(3);
  errmp = 0;
}  /*writeerror */


/*--------------------------------------------------------------------*/

void stackattribute(stackinx stackp)
{
  /* This is the interface routine to push terminal attributes onto the
     attribute stack */
  attribute *WITH;

  WITH = &attstack[stackp];
  /* D if debuglevel = 2 then writeln(log,
     'stackp=',stackp:1,' lexsymb=',lexsymb:1 ); D */
  WITH->chbufx = oldbufi;
  WITH->length = chbufi - oldbufi;
  WITH->prim = NULL;
  WITH->root = NULL;
  WITH->varname = NULL;
  WITH->xval = floatvalue;
  WITH->yval = 0.0;
  WITH->startchop = 0.0;
  WITH->endchop = 0.0;
  WITH->state = 0;
  WITH->lexval = lexsymb;
}  /* stackattribute */


void markerror(int n)
{
  /* n: integer */
  _REC_errmsg *WITH;

  if (n < 900)   /* Do not count warnings */
    errcount++;
  /*D if debuglevel > 0 then begin
     write(log, '*** Markerror');
     if inbuf<>nil then write(log,'[',ordp(inbuf):1,']');
     writeln(log, ': no=', n:1, ', lexsymb=', lexsymb:1 ) end; D*/
  if (errmp >= ERRMAX)
    return;
  errmp++;
  WITH = &errmsg[errmp];
  WITH->no = n;
  if (inbuf != NULL)
    WITH->col = inbuf->readx - 1;
  else
    WITH->col = 0;
  /* symb := newsymb */
  WITH->symb_ = lexsymb;
}  /*markerror*/


void initrandom(void)
{
  /* initialize random number routine */
time_t seed;

  /*GH var t: TimeStamp; HG*/
  srandom(time(&seed));
  /*GH GetTimeStamp(t);
      srandom(t.Microsecond) HG*/
}


/*--------------------------------------------------------------------*/

/* procedures for input/output of characters */

void exitmacro(void)
{
  arg *ar, *lastarg;

  /*D if debuglevel > 0 then writeln(log,' exitmacro'); D*/
  ar = args;
  if (args != NULL)   /* first get rid of args */
    args = args->higherb;
  while (ar != NULL) {   /*D,1D*/
    disposebufs(&ar->argbody);
    lastarg = ar;
    ar = ar->nextb;
    Free(lastarg);
  }
}


void newbuf(fbuffer **buf)
{
  /* var buf: fbufferp */
  fbuffer *WITH;

  /*D if debuglevel > 0 then write(log,'newbuf'); D*/
  if (freeinbuf == NULL) {
    *buf = Malloc(sizeof(fbuffer));
    (*buf)->carray = Malloc(sizeof(chbufarray));
  } else {
    *buf = freeinbuf;
    freeinbuf = freeinbuf->nextb;
  }
  WITH = *buf;
  WITH->savedlen = 0;
  WITH->readx = 1;
  WITH->attrib = 0;
  WITH->higherb = NULL;
  WITH->prevb = NULL;
  WITH->nextb = NULL;
  /*D;if debuglevel > 0 then writeln(log,'[',ordp(buf):1,']') D*/
}


void newarg(arg **arg_)
{
  /* var arg: argp */
  arg *WITH;

  /*D if debuglevel > 0 then write(log,'newarg'); D*/
  *arg_ = Malloc(sizeof(arg));
  WITH = *arg_;
  WITH->argbody = NULL;
  WITH->higherb = NULL;
  WITH->nextb = NULL;
  /*D;if debuglevel > 0 then writeln(log,'[',ordp(arg):1,']'); D*/
}


fbuffer *newinbuf(fbuffer *p)
{
  /* (p: fbufferp): fbufferp */
  if (errmp > 0)
    writeerror();
  return p;
  /*D; if debuglevel = 2 then writeln(log,'new(next)inbuf[',ordp(p):1,']') D*/
}


void disposebufs(fbuffer **buf)
{
  /* var buf: fbufferp; loc: integer */
  fbuffer *bu;

  /*D if debuglevel > 0 then begin
      write(log,'disposebufs(',loc:1,')[');
      if buf <> nil then write(log,ordp(buf):1) else write(log,'nil');
      writeln(log,']') end; D*/
  if (*buf == NULL)
    return;
  if (freeinbuf != NULL) {
    bu = *buf;
    while (bu->nextb != NULL)
      bu = bu->nextb;
    bu->nextb = freeinbuf;
  }
  freeinbuf = *buf;
  *buf = NULL;
}


void readline(FILE **infname)
{
  int ll,c;
  if (feof( *infname )) { inputeof = true; return; }
  for (ll = CHBUFSIZ-1; inbuf->savedlen < ll; ) {
     c = getc( *infname );
     if ((char)c == '\n') ll = inbuf->savedlen;
     else if ((char)c == '\r') {
        if ((char)(c=getc( *infname )) != '\n') {
           if (c != EOF) ungetc(c, *infname );
           else if (inbuf->savedlen == 0) { inputeof = true; return; }
           c = '\n'; }
        ll = inbuf->savedlen; }
     else if (c != EOF) {
        inbuf->savedlen++; inbuf->carray[inbuf->savedlen] = (char)c; }
     else if (inbuf->savedlen == 0) { inputeof = true; return; }
     else ll = inbuf->savedlen;
     }
  if ((ll == CHBUFSIZ-1) && ((char)c != '\n') && (c != EOF)) {
     inbuf->savedlen++; inbuf->carray[inbuf->savedlen] = bslch; }
  else {
     while (inbuf->carray[inbuf->savedlen] == '\r' && inbuf->savedlen > 0)
        inbuf->savedlen--;
     if (inbuf->carray[inbuf->savedlen] != bslch || lexstate != 2) {
        inbuf->savedlen++;
        inbuf->carray[inbuf->savedlen] = nlch;
        }
     }

  /*D; if debuglevel > 0 then begin write(log,'readline ');
     if inputeof then writeln(log,'inputeof') else wrbuf(inbuf,3,0) end D*/
}


void nextline(Char lastchar)
{
  /* Write out error messages, and read in a line. */
  int i;
  fbuffer *WITH;
  int FORLIM;

  writeerror();
  WITH = inbuf;
  WITH->readx = 1;
  WITH->savedlen = 0;
  do {
    lineno++;
    if (savebuf != NULL) {
      readline(&copyin);
      if (inputeof) {
	inputeof = false;
	while (inbuf->prevb != NULL)   /*D,11D*/
	  inbuf = newinbuf(inbuf->prevb);
	disposebufs(&inbuf);
	inbuf = savebuf;
	savebuf = NULL;
      }
    } else
      readline(&input);

    WITH = inbuf;

    if (WITH->carray[1] == '.') {
      if (lexstate != 2) {
	if (WITH->savedlen >= 4 && WITH->carray[2] == 'P') {
	  if (savebuf != NULL)
	    WITH->savedlen = 0;   /* skip .P* lines */
	  else if (WITH->carray[3] == 'F' || WITH->carray[3] == 'S') {
	    lexstate = 1;
	    WITH->readx = 4;
	  } else if (WITH->carray[3] == 'E') {
	    lexstate = 3;
	    WITH->readx = 4;
	  }
	}
      } else if (lastchar != bslch) {
	if (WITH->savedlen < 4 || WITH->carray[2] != 'P' || savebuf != NULL)
	  WITH->savedlen = 0;   /* skip . lines */
	else if (WITH->carray[3] == 'F' || WITH->carray[3] == 'S') {
	  lexstate = 1;
	  WITH->readx = 4;
	} else if (WITH->carray[3] == 'E') {
	  lexstate = 3;
	  WITH->readx = 4;
	} else
	  WITH->savedlen = 0;
      }
    }
    /*D if linesignal > 0 then begin
          i := lineno div (10*linesignal);
          if lineno = i*10*linesignal then writeln(errout,'lineno ',lineno:1)
          end;
       if debuglevel > 1 then begin
          writeln(log); write(log,'lineno ',lineno:1,':') end; D*/

    if (lexstate == 0 && !inputeof) {
      WITH = inbuf;
      FORLIM = WITH->savedlen;
      for (i = 1; i < FORLIM; i++)
	putchar(WITH->carray[i]);
      if (WITH->carray[WITH->savedlen] != bslch)
	putchar('\n');
      WITH->savedlen = 0;
    }
    /* D;
    if debuglevel > 0 then begin
       write(log,'nextline:');
       with inbuf^ do for i:=1 to savedlen do case ord(carray^[i]) of
          ordETX: write(log,'^C');
          ordNL: writeln(log);
          ordCR: write(log,'\r');
          ordTAB: write(log,'\t');
          otherwise write(log,carray^[i])
          end;
       end D */
  } while (!(inbuf->savedlen > 0 || inputeof));
}


void inchar(void)
{
  fbuffer *tp;
  boolean bufend;
  fbuffer *WITH;

  if (inbuf == NULL) {
    newbuf(&inbuf);
    inbuf->attrib = -1;
  }
  /*D if debuglevel = 2 then with inbuf^ do begin
     write(log,'inchar['); if prevb<>nil then write(log,ordp(prevb):1);
     write(log,'<',ordp(inbuf):1,'>');
     if nextb<>nil then write(log,ordp(nextb):1);
     write(log,']: instr=',instr) end; D*/

  WITH = inbuf;
  if (WITH->readx < WITH->savedlen)
    bufend = false;
  else if (WITH->readx > WITH->savedlen)
    bufend = true;
  else if (WITH->carray[WITH->readx] != bslch)
    bufend = false;
  else if (!instr)
    bufend = true;
  else {
    if (WITH->savedlen >= CHBUFSIZ)
      markerror(902);
    else
      WITH->savedlen++;
    WITH->carray[WITH->savedlen] = nlch;
    bufend = false;
  }
  while (bufend) {
    /*D if debuglevel = 2 then begin
        write(log,' Bufend=',bufend,', '); wrchar(ch); writeln(log) end; D*/
    if (inbuf->nextb != NULL)
      inbuf = newinbuf(inbuf->nextb);
    else if (inbuf->attrib > 0) {
      /*D if debuglevel = 2 then begin
        write(log,'For or repeat detected, '); wrchar(ch); write(log,' ')
        end; D*/
      writeerror();
      inbuf->readx = 1;
      while (inbuf->prevb != NULL) {
	inbuf = newinbuf(inbuf->prevb);
	inbuf->readx = 1;
      }
      forbufend = true;
    } else {
      writeerror();
      while (inbuf->prevb != NULL)
	inbuf = newinbuf(inbuf->prevb);
      if (inbuf->nextb != NULL)   /*D,10D*/
	disposebufs(&inbuf->nextb);
      if (inbuf->higherb != NULL) {
	tp = inbuf;
	inbuf = newinbuf(inbuf->higherb);   /*D,3D*/
	disposebufs(&tp);
      } else if (!inputeof)
	nextline(inbuf->carray[inbuf->savedlen]);
    }
    WITH = inbuf;
    if (WITH->readx < WITH->savedlen || inputeof) {
      bufend = false;
      break;
    }
    if (WITH->readx > WITH->savedlen) {
      bufend = true;
      continue;
    }
    if (WITH->carray[WITH->readx] != bslch) {
      bufend = false;
      break;
      continue;
    }
    if (!instr) {
      bufend = true;
      continue;
    }
    if (WITH->savedlen >= CHBUFSIZ)
      markerror(902);
    else
      WITH->savedlen++;
    WITH->carray[WITH->savedlen] = nlch;
    bufend = false;
  }

  if (forbufend || inputeof) {
    ch = nlch;
    return;
  }
  WITH = inbuf;
  ch = WITH->carray[WITH->readx];
  WITH->readx++;

  /*D; if debuglevel = 2 then with inbuf^ do begin
     write(log,' savedlen=',savedlen:1,' readx=',readx:1);
     if inputeof then write(log,' inputeof');
     write(log,' '); wrchar(ch); writeln(log)
     end D*/

  /* for buf */
  /* This is not a loop */
}  /* inchar */


void skiptoend(void)
{
  /* skip to end of the current input line */
  boolean skip;
  fbuffer *WITH;

  /*D if debuglevel>1 then
      writeln(log,'skiptoend: inbuf[',ordp(inbuf):1,']'); D*/
  skip = true;
  while (skip) {
    if (inbuf == NULL) {
      skip = false;
      break;
    }
    WITH = inbuf;
    /*D if (debuglevel>1) and (carray^[readx]=nlch) then
        write(log,'carray^[',readx:1,']=',ord(carray^[readx]):1); D*/
    if (WITH->carray[WITH->readx] == nlch)
      skip = false;
    else if (WITH->carray[WITH->readx] == etxch)
      exitmacro();
    WITH->readx++;
    if (WITH->readx > WITH->savedlen) {
      if (inbuf->nextb != NULL)
	inbuf = newinbuf(inbuf->nextb);
      else
	skip = false;
    }
    /*D; if (debuglevel>1) then writeln(log,' readx=',inbuf^.readx:1) D*/
  }
}


void backup(void)
{
  fbuffer *WITH;

  WITH = inbuf;
  WITH->readx--;
  ch = ' ';
}


/*--------------------------------------------------------------------*/



/*--------------------------------------------------------------------*/

void pushch(void)
{
  /* copy ch character into chbuf and get new ch */
  chbuf[chbufi] = ch;
  if (chbufi < CHBUFSIZ)   /* Leave 1 location free at the end of chbuf^ */
    chbufi++;
  else
    fatal(4);
  inchar();
}  /*pushch*/


void readlatex(void)
{
  /* read complete line into chbuf */
  while (ch != nlch)
    pushch();
  newsymb = XLaTeX;
}


boolean isprint_(Char ch)
{
  return (ch >= 32 && ch <= 126);
}


#ifndef SAFE_MODE

void pointinput(nametype *txt)
{
  /* txt: strptr */
  int i, FORLIM;

  if (txt == NULL)
    return;
  if (txt->segmnt == NULL)
    return;
  if (txt->len >= FILENAMELEN)
    txt->len = FILENAMELEN - 1;
  FORLIM = txt->len;
  for (i = 0; i < FORLIM; i++)
    infname[i] = txt->segmnt[txt->seginx + i];
  for (i = txt->len; i < FILENAMELEN; i++)
    infname[i] = ' ';
  /*DUGHM if debuglevel > 0 then begin
     write(log,'Pointinput(segmnt ',seginx:1,', len ',len:1,') ');
     for i := 1 to len do write(log,infname[i]); writeln(log) end; MHGUD*/

  if (savebuf != NULL) {
    markerror(853);
    return;
  }
  if (checkfile(infname, true) != 0) {
    markerror(859);
    return;
  }
  if (copyin != NULL)
    copyin = freopen(P_trimname(infname, sizeof(mstring)), "r", copyin);
  else
    copyin = fopen(P_trimname(infname, sizeof(mstring)), "r");
  if (copyin == NULL)
    _EscIO(FileNotFound);
  backup();
  ch = nlch;
  savebuf = inbuf;
  inbuf = NULL;
}


void pointoutput(boolean nw, nametype *txt, int *ier)
{
  /* nw: boolean; txt: strptr; var ier: integer */
  int i, FORLIM;

  *ier = 1;
  if (txt->segmnt == NULL)
    return;
  if (txt->len >= FILENAMELEN)
    txt->len = FILENAMELEN - 1;
  FORLIM = txt->len;
  for (i = 0; i < FORLIM; i++) {
    outfnam[i] = txt->segmnt[txt->seginx + i];
    if (*ier == 1) {
      if (outfnam[i] > 32 && outfnam[i] <= 126)
	*ier = 0;
    }
  }
  outfnam[txt->len] = '\0';
  /*DUGHM if debuglevel > 0 then begin
     write(log,'Pointoutput(',nw,' segmnt ',seginx:1,', len ',len:1,') "');
     for i := 1 to len do write(log,outfnam[i]); writeln(log,'"') end;
  MHGUD*/
  if (*ier != 0) {
    markerror(861);
    return;
  }
  if (nw) {
    if (redirect != NULL)
      redirect = freopen(P_trimname(outfnam, sizeof(mstring)), "w", redirect);
    else
      redirect = fopen(P_trimname(outfnam, sizeof(mstring)), "w");
    if (redirect == NULL)
      _EscIO(FileNotFound);
    return;
  }
  if (redirect != NULL)
    redirect = freopen(P_trimname(outfnam, sizeof(mstring)), "a", redirect);
  else
    redirect = fopen(P_trimname(outfnam, sizeof(mstring)), "a");
  if (redirect == NULL)
    _EscIO(FileNotFound);
}


#endif

int argcount(arg *ar)
{
  int i;

  if (ar == NULL)
    markerror(805);
  i = 0;
  while (ar != NULL) {
    if (ar->argbody != NULL)
      i++;
    ar = ar->nextb;
  }
  return i;
}


arg *findarg(arg *arlst, int k)
{
  arg *ar;
  int i, j;

  /*D if debuglevel > 0 then begin
      write(log, 'findarg(');
      if arlst = nil then write(log,'nil') else write(log,ordp(arlst):1);
      write(log,',',k:1,'):') end; D*/
  if (k > 0)
    ar = arlst;
  else
    ar = NULL;
  i = 1;
  j = k;
  while (i < j) {
    if (ar != NULL) {
      ar = ar->nextb;
      i++;
    } else
      j = i;
  }
  /*D if debuglevel > 0 then begin
     if (i<>k) or (ar=nil) then writeln(log,'not found')
     else wrbuf(ar^.argbody,2,1) end; D*/
  return ar;
}


void readstring(Char stringch)
{
  /* puts string terminal into chbuf */
  int j, n;
  boolean pushfl;
  arg *ar;
  fbuffer *abuf, *WITH;
  int FORLIM;

  /*D if debuglevel > 0 then write(log,'readstring(',stringch,') '); D*/
  j = -1;
  n = 0;
  instr = true;
  pushfl = true;
  while (instr) {
    if (inputeof)
      instr = false;
    else if (ch == bslch) {
      pushch();
      if (ch == stringch)
	chbufi--;
      else
	pushfl = false;
    } else if (ch == '$' && j < 0)
      j = chbufi;
    else if (j >= 0 && isdigit(ch) != 0)
      n = n * 10 + ch - '0';
    else if (j >= 0) {
      if (n > 0 && args != NULL)   /* delete $nn */
	chbufi = j;
      ar = findarg(args, n);
      if (ar != NULL) {
	abuf = ar->argbody;
	while (abuf != NULL) {
	  WITH = abuf;
	  FORLIM = WITH->savedlen;
	  for (n = WITH->readx; n <= FORLIM; n++) {
	    chbuf[chbufi] = WITH->carray[n];
	    if (chbufi < CHBUFSIZ)
	      chbufi++;
	    else
	      fatal(4);
	  }
	  abuf = abuf->nextb;
	}
      }
      /*D if debuglevel > 0 then begin write(log,'arg(',n:1,')|');
         for n := j to chbufi-1 do write(log,chbuf^[n]);
         write(log,'| ') end; D*/
      j = -1;
      n = 0;
      pushfl = false;
    } else if (ch == stringch)
      instr = false;
    /*D if debuglevel > 0 then begin
        write(log,' readstring2, instr=',instr,' ');
        wrchar(ch); writeln(log) end;D*/
    if (instr && pushfl)
      pushch();
    else
      pushfl = true;
  }
  if (ch != nlch)
    inchar();
  else {
    /*D; if debuglevel > 0 then writeln(log,' readstring done') D*/
    markerror(901);
  }
}


void readexponent(void)
{
  boolean neg;
  int k;

  pushch();
  if (ch == '-') {
    neg = true;
    pushch();
  } else if (ch == '+') {
    neg = false;
    pushch();
  } else
    neg = false;
  if (isdigit(ch) == 0) {
    markerror(802);
    return;
  }
  k = 0;
  do {
    k = k * 10 + ch - '0';
    pushch();
  } while (isdigit(ch) != 0);
  if (neg)
    floatvalue *= exp(-k * ln10);
  else
    floatvalue *= exp(k * ln10);
}  /* readexponent */


void readfraction(void)
{
  double x;

  x = 10.0;
  while (isdigit(ch) != 0) {
    floatvalue += (ch - '0') / x;
    x *= 10;
    pushch();
  }
}  /* readfraction */


void readconst(Char initch)
{
  /*D if debuglevel=2 then begin
     write(log,'readconst(',initch,') readx=',inbuf^.readx:1,' ');
       wrchar(ch); writeln(log) end; D*/
  if (initch == '.')
    readfraction();
  else {
    while (isdigit(ch) != 0) {
      floatvalue = 10 * floatvalue + ch - '0';
      pushch();
    }
    if (ch == '.') {
      pushch();
      readfraction();
    }
  }
  if (ch == 'e' || ch == 'E')
    readexponent();
  if (ch == 'i')   /* A rather odd way to allow inch units */
    pushch();
  newsymb = XLfloat;
}


/*D
procedure prlex(acc: boolean);
var i: integer;
begin
   if debuglevel > 0 then begin
      write(log, 'LEXICAL(oldsymb=',oldsymb:1,' newsymb=',newsymb:1,')');
      if newsymb <> XLaTeX then begin
         write(log,' chbuf(',oldbufi:1,':',chbufi-1:1,')="');
         for i := oldbufi to chbufi-1 do write(log, chbuf^[i]);
         write(log,'"') end
      else write(log,'=<LaTeX>');
      if newsymb = XLfloat then begin
         write( log,' value='); wfloat(log,floatvalue)
         end;
      write(log,' '); wrchar(ch);
      if not acc then write( log,' not accepted');
      writeln( log );
      consoleflush
      end
   end; D*/

fbuffer *nbuf(fbuffer *buf)
{
  fbuffer *WITH;

  if (buf->prevb == NULL)
    newbuf(&buf->prevb);
  WITH = buf->prevb;
  WITH->attrib = buf->attrib;
  WITH->savedlen = CHBUFSIZ;
  WITH->readx = CHBUFSIZ + 1;
  WITH->nextb = buf;
  return (buf->prevb);
}


void copyleft(fbuffer *mac, fbuffer **buf)
{
  /* mac: fbufferp; var buf: fbufferp */
  /* Push macro or arg or string from mac into the head of the input stream */
  fbuffer *ml;
  int i, k;
  boolean newflag, copied;
  fbuffer *WITH;
  int FORLIM;

  /*D if debuglevel > 0 then writeln(log, 'copyleft:'); D*/
  /*D if debuglevel > 1 then begin
     write(log,' input string'); wrbuf(mac,3,1);
     write(log,' output'); wrbuf(buf,3,1) end; D*/
  writeerror();
  ml = mac;
  while (ml != NULL) {
    mac = ml;
    ml = ml->nextb;
  }
  copied = false;
  while (mac != NULL) {
    if (mac->carray != NULL) {
      if (mac->savedlen >= mac->readx) {
	copied = true;
	if (*buf == NULL)
	  newflag = true;
	else if ((*buf)->attrib >= 0)
	  newflag = true;   /* for body */
	else
	  newflag = false;
	/*D if debuglevel > 0 then
	   writeln(log,'newflag=',newflag:1,' attrib=',buf^.attrib:1); D*/
	if (newflag) {
	  newbuf(&ml);
	  ml->attrib = -1;
	  ml->savedlen = CHBUFSIZ;
	  ml->readx = CHBUFSIZ + 1;
	  ml->higherb = *buf;
	  *buf = ml;
	}
      }
      k = mac->savedlen;
      if ((*buf)->readx < k - mac->readx + 2) {  /*not enough space*/
	WITH = *buf;
	while (WITH->readx > 1) {
	  WITH->readx--;
	  WITH->carray[WITH->readx] = mac->carray[k];
	  k--;
	}
	*buf = nbuf(*buf);
      }
      WITH = *buf;
      FORLIM = mac->readx;
      for (i = k; i >= FORLIM; i--) {
	WITH->readx--;
	WITH->carray[WITH->readx] = mac->carray[i];
      }
    }
    mac = mac->prevb;
  }
  if (!copied)
    return;
  /*D; if debuglevel > 0 then begin
     ml := buf; writeln(log,' copyleft result'); while ml <> nil do begin
        wrbuf(ml,3,1); ml := ml^.nextb end end D*/
  if ((*buf)->readx <= 1)
    *buf = nbuf(*buf);
  WITH = *buf;
  WITH->carray[WITH->readx - 1] = nlch;
}


void copyright(fbuffer *mac, fbuffer **buf)
{
  /* $n has been seen: Push the argument into the tail of the input buffer */
  fbuffer *ml;
  int i, k;
  fbuffer *WITH;
  int FORLIM;

  /*D if debuglevel > 0 then begin
     writeln(log, 'copyright:');
     write(log,' input'); wrbuf(mac,3,1);
     write(log,' output'); wrbuf(buf,3,0) end; D*/
  while (mac != NULL) {
    if (*buf == NULL) {
      newbuf(buf);
      WITH = *buf;
      WITH->attrib = -1;
      WITH->savedlen = 0;
      WITH->readx = 1;
    }
    k = mac->readx;
    if (CHBUFSIZ - (*buf)->savedlen <= mac->savedlen - k) {
      WITH = *buf;
      while (WITH->savedlen < CHBUFSIZ) {
	WITH->savedlen++;
	WITH->carray[WITH->savedlen] = mac->carray[k];
	k++;
      }
      newbuf(&ml);
      ml->attrib = (*buf)->attrib;
      ml->savedlen = 0;
      ml->readx = 1;
      ml->prevb = *buf;
      (*buf)->nextb = ml;
      *buf = ml;
    }
    WITH = *buf;
    FORLIM = mac->savedlen;
    for (i = k; i <= FORLIM; i++) {
      WITH->savedlen++;
      WITH->carray[WITH->savedlen] = mac->carray[i];
    }
    mac = mac->nextb;
  }
  /*D; if debuglevel > 0 then with buf^ do begin
     write(log,' result'); wrbuf(buf,3,0) end D*/
}


void skipcontinue(void)
{
  /* Check the current char for line continuation */
  Char c;
  fbuffer *WITH;

  /*D if debuglevel=2 then
     write(log,' skipcontinue readx=',inbuf^.readx:1,' '); D*/
  c = ch;
  while (c == bslch) {
    WITH = inbuf;
    if (WITH->readx <= WITH->savedlen)
      c = WITH->carray[WITH->readx];
    else if (WITH->nextb == NULL) {
      inchar();
      c = ch;
    } else
      c = WITH->nextb->carray[WITH->nextb->readx];
    if (c == nlch) {
      inchar();
      inchar();
      c = ch;
      continue;
    }
    if (c != '#') {
      c = ' ';
      break;
    }
    skiptoend();
    inchar();
    c = ch;
  }
}


void skipwhite(void)
{
  /* D if debuglevel = 2 then writeln(log, 'skipwhite: ' ); D */
  while (ch == etxch || ch == nlch || ch == tabch || ch == ' ') {
    if (ch == etxch)
      exitmacro();
    inchar();
    if (ch == bslch)
      skipcontinue();
  }
}


void definearg(int *parenlevel, fbuffer **p2)
{
  /* Stash the current argument into the arg struct*/
  int j, n;
  arg *ar;
  fbuffer *p, *p1;
  boolean inarg, instring;
  Char prevch;
  fbuffer *WITH;

  /*D if debuglevel > 0 then
      writeln(log, 'definearg: parenlevel=',parenlevel:1); D*/
  skipwhite();
  p1 = NULL;
  if (*parenlevel >= 0)
    inarg = true;
  else
    inarg = false;
  while (inarg) {
    newbuf(&p);
    p->attrib = -1;
    if (p1 != NULL) {
      p1->nextb = p;
      p->prevb = p1;
    }
    p1 = p;
    j = CHBUFSIZ;
    instring = false;
    prevch = ' ';
    do {
      WITH = p1;
      if (prevch != bslch) {
	if (instring && ch == '"')
	  instring = false;
	else if (ch == '"')
	  instring = true;
      }
      if (!instring) {
	if (ch == '(')
	  (*parenlevel)++;
	else if (ch == ')')
	  (*parenlevel)--;
      }
      /*D if debuglevel=2 then write(log,' instring=',instring,' '); D*/
      if (!instring && (*parenlevel < 0 || *parenlevel == 0 && ch == ',')) {
	j = WITH->savedlen;
	inarg = false;
      } else if (ch != '$') {
	prevch = ch;
	if (WITH->savedlen < CHBUFSIZ)
	  WITH->savedlen++;
	else
	  markerror(872);
	WITH->carray[WITH->savedlen] = ch;
	/*D if debuglevel = 2 then begin
	   write(log,'definearg2: savedlen=',savedlen:1,' ');
	   wrchar(ch); writeln(log,' parenlevel=',parenlevel:1)
	   end; D*/
	inchar();
      } else {
	/*D if debuglevel=2 then write(log,' definearg3'); D*/
	prevch = ch;
	inchar();
	if (isdigit(ch) != 0) {
	  n = 0;
	  do {
	    n = n * 10 + ch - '0';
	    inchar();
	  } while (isdigit(ch) != 0);
	  ar = findarg(args, n);
	  if (ar != NULL)
	    copyright(ar->argbody, &p1);
	} else {
	  if (WITH->savedlen < CHBUFSIZ)
	    WITH->savedlen++;
	  else
	    markerror(872);
	  WITH->carray[WITH->savedlen] = prevch;
	  /*D if debuglevel = 2 then begin
	      write(log,'definearg2: savedlen=',savedlen:1,' ');
	      wrchar(ch); writeln(log) end; D*/
	}
      }
      if (inputeof) {
	markerror(806);
	j = WITH->savedlen;
	inarg = false;
	*parenlevel = -1;
      }
    } while (p1->savedlen != j);
  }
  if (p1 != NULL) {
    while (p1->prevb != NULL)
      p1 = p1->prevb;
  }
  ch = ' ';
  *p2 = p1;
  /*D; if debuglevel > 0 then with p1^ do begin
     write(log,' definearg'); wrbuf(p1,3,0) end D*/
}


boolean ismacro(Char *chb, chbufinx obi, chbufinx chbi)
{
  arg *mac, *lastp, *ar, *lastarg, *firstarg;
  int level;
  boolean ism;

  /*D if debuglevel > 0 then begin
      write(log, 'ismacro[');
      for level:=obi to chbi-1 do write(log,chb^[level]);
      write(log,']:') end;
      D*/
  ism = false;
  if (oldsymb == XLdefine) {
    skipwhite();
    return ism;
  }
  if (oldsymb == XLundefine)
    return ism;
  mac = findmacro(macros, chb, obi, chbi - obi, &lastp);
  if (mac == NULL)
    return ism;
  ism = true;
  firstarg = NULL;
  lastarg = NULL;
  if (ch == '(')
    level = 0;
  else {
    backup();
    level = -1;
  }
  do {
    newarg(&ar);
    ar->higherb = args;
    if (lastarg == NULL)
      firstarg = ar;
    else
      lastarg->nextb = ar;
    if (level >= 0) {
      inchar();
      definearg(&level, &ar->argbody);
    }
    lastarg = ar;
  } while (level >= 0);
  args = firstarg;
  copyleft(mac->argbody, &inbuf);
  /*D if debuglevel > 0 then writeln(log,'ismacro returning:',ism); D*/
  return ism;
}


void copyarg(Char *chb, chbufinx chbs, chbufinx chbi)
{
  int n, i;
  arg *ar;

  /*D if debuglevel > 0 then begin write(log,'copyarg(');
     for i:= chbs to chbi-1 do write(log,chb^[i]); write(log,') ') end; D*/
  n = 0;
  for (i = chbs + 1; i < chbi; i++)
    n = n * 10 + chb[i] - '0';
  ar = findarg(args, n);
  backup();
  if (ar != NULL) {
    if (ar->argbody != NULL)
      copyleft(ar->argbody, &inbuf);
  }
}


boolean insertarg(void)
{
  int icx;
  boolean ins;

  /* j,k: integer;
  argstruct: argp;
  varptr,lastp: strptr; */
  pushch();
  ins = false;
  if (ch == '+') {
    backup();
    ch = '$';
    chbufi--;
    return ins;
  }
  icx = chbufi - 1;
  while (isdigit(ch) != 0)
    pushch();
  copyarg(chbuf, icx, chbufi);
  chbufi = icx;
  inchar();
  ins = true;
  /*
     else if isalnum(ch) then begin
        icx := chbufi-1;
        repeat pushch until not isalnum(ch);
        backup;
        varptr := glfindname(envblock,chbuf,oldbufi+1,chbufi-oldbufi-1,lastp,k);
        if varptr=nil then markerror(805) else begin
           j := round(varptr^.val); argstruct := findarg( args,j );
           if argstruct=nil then markerror(805)
           else if argstruct^.argbody<>nil then begin
              copyleft(argstruct^.argbody,inbuf);
              ins := true
              end
           end
        end
  */
  return ins;
}


void lexical(void)
{
  /* find the next terminal */
  int lxix;
  Char firstch;
  boolean terminalaccepted, looping;
  fbuffer *WITH;

  /* argstruct: argp;
  j,k: integer;
  varptr,lastp: strptr; */

  floatvalue = 0.0;
  lexsymb = -1;
  do {
    terminalaccepted = true;
    oldbufi = chbufi;   /* first char of current terminal in chbuf */
    while (ch == ' ' || ch == tabch)
      inchar();

    if (lexstate == 1) {
      newsymb = XSTART;
      lexstate = 2;
    } else if (lexstate == 3) {
      /*D if linesignal > 0 then begin
         writeln(errout,'.PE'); consoleflush end; D*/
      newsymb = XEND;
      /* ch := '.'; */
      lexstate = 4;
    } else if (lexstate == 4) {
      newsymb = XNL;
      skiptoend();
      ch = ' ';
      lexstate = 0;
    } else if (inputeof) {
      if (oldsymb == 0) {
	writeerror();
	fatal(5);
      } else if (oldsymb == 1)
	newsymb = 0;
      else
	newsymb = 1;
    } else if (forbufend) {
      newsymb = XLendfor;
      forbufend = false;
      ch = ' ';
    } else if (ch == nlch) {
      newsymb = XNL;
      ch = ' ';
      if (oldsymb == XLelse || oldsymb == XLBRACE || oldsymb == XLthen ||
	  oldsymb == XCOLON || oldsymb == XNL)
	terminalaccepted = false;
    } else if (isdigit(ch) != 0)
      readconst(ch);
    else if (ch == etxch) {
      exitmacro();
      inchar();
      terminalaccepted = false;
    } else {
      firstch = ch;
      pushch();
      WITH = inbuf;
      if (firstch == bslch) {
	if (ch == nlch || ch == '#') {
	  if (ch == '#')
	    skiptoend();
	  ch = ' ';
	  newsymb = -2;   /* flag for test below */
	  terminalaccepted = false;
	} else
	  readlatex();
      } else if (firstch == '.' && isdigit(ch) != 0)
	readconst(firstch);
      else if (firstch == '.' && WITH->readx == 3 && inbuf->prevb == NULL &&
	       newsymb != -2)
	readlatex();
      else {
	newsymb = entrytv[firstch];
	lxix = entryhp[firstch];
	while (lxix != 0) {
	  if (ch == '$') {
	    if (!insertarg())
	      lxix = 0;
	    continue;
	  }
	  if (lxch[lxix] == ch) {
	    newsymb = lxtv[lxix];
	    lxix = lxhp[lxix];
	    pushch();
	  } else
	    lxix = lxnp[lxix];
	}

	if (isupper(firstch) != 0 &&
	    (isalnum(ch) != 0 || ch == '_' || ch == '$')) {
	  looping = true;
	  while (looping) {
	    if (ch == '$')
	      looping = insertarg();
	    else if (isalnum(ch) != 0 || ch == '_')
	      pushch();
	    else
	      looping = false;
	  }
	  if (ismacro(chbuf, oldbufi, chbufi))
	    terminalaccepted = false;
	  else
	    newsymb = XLabel;
	} else if ((isalnum(firstch) != 0 || firstch == '_') &&
		   (isalnum(ch) != 0 || ch == '_' || ch == '$')) {
	  looping = true;
	  while (looping) {
	    if (ch == '$')
	      looping = insertarg();
	    else if (isalnum(ch) != 0 || ch == '_')
	      pushch();
	    else
	      looping = false;
	  }
	  if (ismacro(chbuf, oldbufi, chbufi))
	    terminalaccepted = false;
	  else
	    newsymb = XLname;
	} else if (newsymb == XLstring) {
	  chbufi--;
	  readstring(chbuf[chbufi]);
	} else if (newsymb == XCOMMENT) {
	  skiptoend();
	  ch = nlch;
	  terminalaccepted = false;
	} else if (newsymb == XNL &&
		   (oldsymb == XLelse || oldsymb == XLBRACE ||
		    oldsymb == XLthen || oldsymb == XCOLON || oldsymb == XNL))
	  terminalaccepted = false;
	else if (newsymb == XLT && inlogic) {
	  lexsymb = XLcompare;
	  newsymb = XLcompare;
	} else if (newsymb > XEQ && newsymb <= XLremeq) {
	  lexsymb = newsymb;
	  newsymb = XEQ;
	} else if (newsymb > XLcorner) {
	  lexsymb = newsymb;
	  if (newsymb > XLenvvar)
	    newsymb = XLenvvar;
	  else if (newsymb > XLprimitiv)
	    newsymb = XLprimitiv;
	  else if (newsymb > XLdirecton)
	    newsymb = XLdirecton;
	  else if (newsymb > XLarrowhd)
	    newsymb = XLarrowhd;
	  else if (newsymb > XLtextpos)
	    newsymb = XLtextpos;
	  else if (newsymb > XLcolrspec)
	    newsymb = XLcolrspec;
	  else if (newsymb > XLlinetype)
	    newsymb = XLlinetype;
	  else if (newsymb > XLfunc2)
	    newsymb = XLfunc2;
	  else if (newsymb > XLfunc1)
	    newsymb = XLfunc1;
	  else if (newsymb > XLparam)
	    newsymb = XLparam;
	  else if (newsymb > XLcompare)
	    newsymb = XLcompare;
	  else
	    newsymb = XLcorner;
	} else if (newsymb == XLarg) {
	  if (ch == '+') {  /* $+ */
	    floatvalue = argcount(args);
	    newsymb = XLfloat;
	    inchar();
	  } else if (isdigit(ch) != 0) {
	    do {
	      pushch();
	    } while (isdigit(ch) != 0);
	    copyarg(chbuf, oldbufi, chbufi);
	    terminalaccepted = false;
	  } else {
	    /* else begin
	       repeat pushch until not isalnum(ch);
	       backup;
	       varptr := glfindname(envblock,chbuf,oldbufi+1,
	          chbufi-oldbufi-1,lastp,k);
	       if varptr=nil then markerror(805) else begin
	          j := round(varptr^.val); argstruct := findarg( args,j );
	          if argstruct=nil then markerror(805)
	          else if argstruct^.argbody<>nil then
	             copyleft(argstruct^.argbody,inbuf)
	          end;
	       terminalaccepted := false
	       end */
	    markerror(805);
	  }
	} else if (newsymb == XLdo) {
	  /*D else if newsymb = XAND then begin
	     chbufi := oldbufi;
	     if debuglevel > 0 then consoleflush;
	     if ord(ch) >=ord('A') then
	        linesignal := 1+ord(ch)-ord('A')
	     else if ord(ch) > ord('0') then begin
	        openlogfile;
	        debuglevel := ord(ch)-ord('0')
	        end
	     else if ord(ch) = ord('0') then debuglevel := 0;
	     writeln(log,'Debuglevel = ',debuglevel:1,
	        ' linesignal = ',linesignal:1); consoleflush;
	     inchar;
	     terminalaccepted := false
	     end D*/
	  skipwhite();
	} else if (newsymb == 0 && isupper(firstch) != 0) {
	  if (ismacro(chbuf, oldbufi, chbufi))
	    terminalaccepted = false;
	  else
	    newsymb = XLabel;
	} else if (newsymb == 0 && (isalnum(firstch) != 0 || firstch == '_')) {
	  if (ismacro(chbuf, oldbufi, chbufi))
	    terminalaccepted = false;
	  else
	    newsymb = XLname;
	} else if (newsymb == 0) {
	  /*D if debuglevel > 0 then writeln(log,
	       'Marking 800:ord(firstch)=',ord(firstch),
	       ' ord(ch)=',ord(ch)); D*/
	  if (errmp == 0) {
	    fprintf(errout, "Char chr(%d)", firstch);
	    if (firstch > 32 && (firstch & 255) < 127)
	      fprintf(errout, "\"%c\"", firstch);
	    fprintf(errout, " unknown\n");
	  }
	  markerror(800);
	  terminalaccepted = false;
	}
      }
    }

    /*D prlex( terminalaccepted ); D*/
    if (newsymb != XLaTeX && newsymb != XLstring && newsymb != XLabel &&
	newsymb != XLname)
      chbufi = oldbufi;
  } while (!terminalaccepted);
  if (lexsymb == -1)
    lexsymb = newsymb;
  oldsymb = newsymb;



  /* search in lexical tree */
  /* $<integer> */
  /* if not isalnum(ch) then */
  /* $name */
  /* char not recognized */
  /*lookahead terminals*/
}  /* lexical */


/*--------------------------------------------------------------------*/

void skiptobrace(void)
{
  int bracelevel;
  boolean instring;
  Char prevch;

  /*D if debuglevel = 2 then writeln(log, 'skiptobrace: ' ); D*/
  bracelevel = 1;
  prevch = ' ';
  instring = false;
  while (bracelevel > 0) {
    if (ch == bslch)
      skipcontinue();
    if (ch == etxch)
      exitmacro();
    if (instring) {
      if (ch == '"' && prevch != bslch)
	instring = false;
    } else if (ch == '{')
      bracelevel++;
    else if (ch == '}')
      bracelevel--;
    else if (ch == '"')
      instring = true;
    if (bracelevel <= 0)
      break;
    if (inputeof) {
      bracelevel = 0;
      markerror(804);
    } else {
      prevch = ch;
      inchar();
    }
  }
}


void readfor(fbuffer *p0, int attx, fbuffer **p2)
{
  /* p0: fbufferp; attx: integer; var p2: fbufferp */
  /* Stuff the body of a for loop or a macro body into p2
     attx: attstack index or -(name length)
     p0 = nil: append the output to this buffer.
     Should we check for macro arguments? */
  int j, bracelevel;
  fbuffer *p, *p1;
  boolean instring;
  Char prevch;

  /*D if debuglevel = 2 then begin
      write(log,'readfor: attx(');
      if attx<0 then write(log,'-length)=') else write(log,'attstack idx)=');
      writeln(log,attx:5)
      end; D*/
  writeerror();
  prevch = ' ';
  instring = false;
  bracelevel = 1;
  p1 = NULL;
  while (bracelevel > 0) {
    if (p0 == NULL)
      newbuf(&p);
    else {
      p = p0;
      p0 = NULL;
    }
    p->attrib = attx;
    if (p1 != NULL) {
      p1->nextb = p;
      p->prevb = p1;
    }
    p->higherb = inbuf;
    p1 = p;
    j = CHBUFSIZ;
    do {
      if (ch == bslch)
	skipcontinue();
      /* D if debuglevel = 2 then begin write(log, 'readfor1: ');
          wrchar(ch); write(log,' ') end; D */
      if (instring) {  /* do not check braces in strings */
	if (ch == '"' && prevch != bslch)
	  instring = false;
      } else if (ch == '{')
	bracelevel++;
      else if (ch == '}')
	bracelevel--;
      else if (ch == '"')
	instring = true;
      if (bracelevel <= 0) {
	j = p1->savedlen + 1;
	ch = nlch;
      }
      p1->savedlen++;
      p1->carray[p1->savedlen] = ch;
      prevch = ch;
      /* D if debuglevel = 2 then begin
          write(log,' savedlen=',savedlen:1,' carray(',savedlen:1,')=');
          wrchar(ch); writeln(log) end; D */
      if (bracelevel > 0)
	inchar();
      if (inputeof) {
	markerror(804);
	j = p1->savedlen;
	bracelevel = 0;
      }
    } while (p1->savedlen != j);
  }
  while (p1->prevb != NULL)
    p1 = p1->prevb;
  backup();
  /*D if debuglevel > 0 then begin
     write(log,'For/macro buffer'); wrbuf(p1,3,0) end; D*/
  *p2 = p1;
}


/*--------------------------------------------------------------------*/

void bumptop(stackinx chk, stackinx *sttop)
{
  /* D if chk<>sttop then
     writeln(errout,'chk=',chk:4,' sttop=',sttop:4); D */
  if (chk < STACKMAX)
    (*sttop)++;
  else
    fatal(6);
}


/*D
procedure prattstack(j: integer);
var i: integer;
begin
   write(log,'attstack: ');
   for i := 1 to j do write(log,i:4); writeln(log);
   write(log,'lexval  : ');
   for i := 1 to j do write(log,attstack^[i].lexval:4); writeln(log);
   write(log,'state   : ');
   for i := 1 to j do write(log,attstack^[i].state:4) ; writeln(log);
   write(log,'chbufx  : ');
   for i := 1 to j do write(log,attstack^[i].chbufx:4); writeln(log);
   write(log,'length  : ');
   for i := 1 to j do write(log,attstack^[i].length:4); writeln(log);
   D*/
/* D
   write(log,'xval    : ');
   for i := 1 to j do write(log,attstack^[i].xval:4:1); writeln(log);
   write(log,'yval    : ');
   for i := 1 to j do write(log,attstack^[i].yval:4:1); writeln(log);
   D */
/*D
   write(log,'ptype   : ');
   for i := 1 to j do if attstack^[i].prim = nil then write(log,'    ')
      else write(log,attstack^[i].prim^.ptype:4);
   writeln(log)
   end; D*/

void doprod(int prno)
{
  /* prno: integer */
  redubuf[reduinx + REDUMAX].prod_ = prno;
  reduinx--;
}


void advance(void)
{  /* perform reductions */
  int i, j;
  reduelem *WITH;
  int FORLIM;

  /*D if (debuglevel > 1) and (redutop > 0) then begin
     write(log,'advance prod nos:');
     for i:=1 to redutop do write(log,redubuf[i].prod:4);
     writeln(log);
     write(log,'        newtop  :');
     for i:=1 to redutop do write(log,redubuf[i].newtop:4);
     writeln(log)
     end; D*/

  reduinx = 1;
  while (reduinx <= redutop) {
    WITH = &redubuf[reduinx + REDUMAX];
    /*D if debuglevel > 1 then begin
       j := newtop; if oldtop > newtop then j := oldtop;
       writeln(log,'Attstack for next production:');
       prattstack(j)
       end; D*/

    produce(WITH->newtop, WITH->prod_);   /* may change reduinx */
    /* D if debuglevel > 1 then writeln(log,
       'attstack^[', newtop:1, '].chbufx<-', attstack^[newtop-1].chbufx:1);
        D */
    /* attstack^[newtop].chbufx := attstack^[newtop-1].chbufx; */
    reduinx++;
  }

  /* update stack */
  if (validtop != top) {
    FORLIM = stacktop - validtop;
    for (i = 1; i <= FORLIM; i++)
      parsestack[validtop + i] = parsestack[top + i];
  }

  /* This maintains the input string buffer and could be removed
                                         if the buffer is not required */
  j = attstack[stacktop - 1].chbufx;

  /*D if (debuglevel > 0) and (redutop > 0) and (j < oldbufi) then begin
     write(log,' advance: stacktop=',stacktop:1);
     write(log,' attstack^[stacktop-1](chbufx,length) = ');
     with attstack^[stacktop-1] do wpair(log,chbufx,length);
     writeln(log); write(log,' oldbufi=',oldbufi:1,' ');
     snapname(chbuf,0,chbufi); writeln(log)
     end;D*/

  if (redutop > 0 && j < oldbufi &&
      redubuf[redutop + REDUMAX].prod_ != closeblock1 &&
      redubuf[redutop + REDUMAX].prod_ != suffix2 &&
      redubuf[redutop + REDUMAX].prod_ != suffix1) {
    FORLIM = chbufi - oldbufi;
    /*D if debuglevel > 0 then begin write(log,
      'chbuf(', oldbufi:1, ':', chbufi-1:1, ') =');
      snapname(chbuf,oldbufi,chbufi-oldbufi);
      writeln(log,'->(',j:1, ':', j+chbufi-oldbufi-1:1, ')') end; D*/

    for (i = 0; i < FORLIM; i++)
      chbuf[j + i] = chbuf[oldbufi + i];
    chbufi += j - oldbufi;
    oldbufi = j;
  }

  /* shift */
  bumptop(stacktop, &stacktop);
  parsestack[stacktop].table = startinx;
  stackattribute(stacktop);
  /* freeze stack, ready for new lookahead */
  top = stacktop;
  pseudotop = stacktop;
  validtop = stacktop;
  start = lri;
  redutop = 0;
}  /*advance*/


void backtrack(stackinx btop, int bstart)
{
  stacktop = btop;
  validtop = btop;
  pseudotop = btop;
  startinx = bstart;
  lri = bstart;
  redutop = 0;
}  /* backtrack */


void pseudoshift(void)
{
  bumptop(pseudotop, &stacktop);
  pseudotop = top + stacktop - validtop;
  parsestack[pseudotop].table = startinx;
  stackattribute(pseudotop);
}  /* pseudoshift */


Local void queue(int rs_, int p)
{
  reduelem *WITH;

  if (p >= 0) {
    if (redutop < REDUMAX)
      redutop++;
    else
      fatal(7);
    WITH = &redubuf[redutop + REDUMAX];
    /*D oldtop := stacktop; D*/
    stacktop -= rs_;
    WITH->newtop = stacktop;
    WITH->prod_ = p;
  } else
    stacktop -= rs_;
  if (stacktop <= validtop) {
    pseudotop = stacktop;
    validtop = stacktop;
  } else
    pseudotop -= rs_;
}  /* queue */


/*--------------------------------------------------------------------*/

boolean lookahead(symbol lsymbol)
{
  boolean Result, decided;
  int si;

  decided = false;
  /*D if trace then write(log,' lri:');D*/
  do {
    /*D if trace then write(log, lri: 4);D*/
    startinx = lri;
    switch (lr[lri + kind]) {

    case 0:
      decided = true;
      Result = true;
      parsestop = true;
      break;

    case 1:
      while (lr[lri + symb] != lsymbol && lr[lri] != 0)
	lri = lr[lri];
      decided = true;
      Result = (lr[lri + symb] == lsymbol);
      break;

    case 2:
    case 4:
    case 6:
      while (lr[lri + symb] != lsymbol && lr[lri] != 0)
	lri = lr[lri];
      if (lr[lri + kind] == 2) {
	decided = true;
	Result = true;
      } else if (lr[lri + kind] == 6) {
	pseudoshift();
	stacktop--;
	pseudotop--;
	queue(-1, lr[lri + err]);
      }
      break;

    case 3:
      queue(lr[lri + rs], lr[lri + prod]);
      break;

    case 5:
      si = parsestack[pseudotop].table;
      /*D if trace then
          writeln(log, ' SI(', si: 1, ')');D*/
      while (lr[lri + lb] != si && lr[lri] != 0)
	lri = lr[lri];
      break;
    }/* case */
    lri = lr[lri + next];
    /*D;if trace then write(log,'|')D*/
  } while (!decided);
  /*D if trace then writeln(log,' lookahead done')D*/
  return Result;
}  /* lookahead */


/*--------------------------------------------------------------------*/

void syntaxerror(void)
{
  boolean success;
  stackinx s, s1, FORLIM;

  /*D if debuglevel > 0 then write(log, ' <---SYNTAXERROR');D*/

  markerror(lr[startinx + err]);
  backtrack(top, start);
  pseudoshift();
  s = 0;
  FORLIM = top;
  for (s1 = 0; s1 <= FORLIM; s1++) {
    backtrack(s1, parsestack[s1 + 1].table);
    if (lookahead(XERROR)) {
      parsestack[s1].link = s;
      s = s1;
    }
  }
  success = false;
  do {
    s1 = s;
    do {
      backtrack(s1, parsestack[s1 + 1].table);
      if (lookahead(XERROR)) {
	pseudoshift();
	success = lookahead(newsymb);
      }
      s1 = parsestack[s1].link;
    } while (!(success || s1 == 0));
    if (!success)   /*D end D*/
      lexical();
    /*D begin if debuglevel > 0 then writeln(log, ' <---SKIPPED'); D*/
  } while (!(success || parsestop));
  if (parsestop)
    fatal(8);
}  /* syntaxerror */


void parse(void)
{
  initrandom();

  chbuf = Malloc(sizeof(chbufarray));
  entrytv[ordNL] = XNL;
  entrytv[ordCR] = XNL;   /* treat ^M as line end */

  errmp = 0;
  errcount = 0;
  /* change for debugging */
  /* linesignal := 0; */
  /*D trace := false;
  if trace then debuglevel := 1 else debuglevel := 0; D*/
  /*DUGHM if oflag > 0 then debuglevel := oflag; MHGUD*/
  /*DUGHM if trace and (oflag <= 0) then openlogfile; MHGUD*/
  /* Dwriteln('debuglevel=',debuglevel); D */

  parsestack = Malloc(sizeof(tparsestack));
  produce(1, -2);
  printstate = 0;
  /* lexical initializations, see also
     production -1 */
  ch = ' ';
  lineno = 0;
  chbufi = 0;
  oldbufi = 0;
  newsymb = XNL;
  oldsymb = XNL;
  lexstate = 0;
  macros = NULL;
  args = NULL;
  forbufend = false;
  instr = false;

  stackattribute(0);
  /* parser initializations */
  parsestop = false;
  top = 0;
  start = 5;
  parsestack[0].table = 0;
  parsestack[0].link = 0;
  backtrack(top, start);
  /*D stackhigh := top; D*/
  produce(1, -1);
  while (!parsestop) {
    lexical();
    /*D if trace then
       writeln(log, ' SYM:', newsymb: 3, ' STATE:', lri: 3);D*/
    if (!lookahead(newsymb))
      syntaxerror();
    advance();
  }
  writeerror();
}  /* parse */


Char optionchar(Char *fn)
{
  int j, k;

  j = 1;
  k = FILENAMELEN + 1;
  while (j < k) {
    if (fn[j - 1] == ' ')
      j++;
    else
      k = j;
  }
  if (j >= FILENAMELEN)
    return '\0';
  else if (fn[j - 1] == '-') {
    return (fn[j]);
    /*
    if fn[j+2] <> ' ' then for k:=j+2 to FILENAMELEN do fn[k-j-1] := fn[k]
    */
  } else
    return '\0';
}


void getoptions(void)
{
  Char cht;
  int istop;

  /*DUGHM oflag := 0; linesignal := 0; MHGUD*/
  argct = 1;
  istop = P_argc;   /*GH ParamCount+1; HG*/
  while (argct < istop) {
    P_sun_argv(infname, sizeof(mstring), argct);
	/*GH infname := ParamStr(argct); HG*/
    cht = optionchar(infname);
    if (cht == 0) {
      istop = argct;
      continue;
    }
    /* if cht = 'd' then drawmode := PSmps */
    /* Metapost-readable PS*/
    /*
    else */
    if (cht == 'e')
      drawmode = pict2e;
    else if (cht == 'f')
      drawmode = PSfrag;
    else if (cht == 'g')
      drawmode = PGF;
    else if (cht == 'm')
      drawmode = MFpic;
    else if (cht == 'p')
      drawmode = PSTricks;
    else if (cht == 'r')
      drawmode = PS;
    else if (cht == 's')
      drawmode = MPost;
    else if (cht == 't')
      drawmode = tTeX;
    else if (cht == 'v')
      drawmode = SVG;
    else if (cht == 'x')
      drawmode = xfig;
    else if (cht == 'z') {
      safemode = true;
      /*DUGHM
         else if cht = 'y' then linesignal := 2
         else if (cht >= '0') and (cht <= '9') then oflag := ord(cht)-ord('0')
         MHGUD*/
    } else if (cht == 'h' || cht == '-') {
      fprintf(errout, " *** ");
      fprintf(errout, "%s\n", Version);
      fprintf(errout, " options:\n");
      fprintf(errout, "     (none) output latex drawing commands\n");
      /* writeln(errout,'     -d pdftex graphicx output'); */
      fprintf(errout, "     -e pict2e output\n");
      fprintf(errout, "     -f Postscript output, psfrag strings\n");
      fprintf(errout, "     -g PGF-TikZ output\n");
      fprintf(errout, "     -h write this message and quit\n");
      fprintf(errout, "     -m mfpic output\n");
      fprintf(errout, "     -p PSTricks output\n");
      fprintf(errout, "     -r Postscript output\n");
      fprintf(errout, "     -s MetaPost output\n");
      fprintf(errout, "     -t eepicemu output\n");
      fprintf(errout, "     -v SVG output\n");
      fprintf(errout, "     -x xfig output\n");
      fprintf(errout,
	      "     -z safe mode (sh, copy, and print to file disabled)\n");
      fatal(0);
    } else {
      fprintf(errout, " *** dpic terminating: Unknown option \"");
      if (isprint_(cht))
	fprintf(errout, "%c\"\n", cht);
      else
	fprintf(errout, "char(%d)\"\n", cht);
      fatal(0);
    }
    argct++;
  }
}  /* getoptions */


main(int argc, Char *argv[])
{
  PASCAL_MAIN(argc, argv);
  /* dpic */
  redirect = NULL;
  copyin = NULL;
  errout = NULL;
  output = NULL;
  input = NULL;
  openerror();
  drawmode = TeX;
  safemode = false;
  getoptions();
  /* Dwriteln('oflag=',oflag); D */
  openfiles();
  inputeof = false;
  attstack = Malloc(sizeof(attstacktype));
  parse();
  epilog();
  /*X retcode(errcount); X*/

  if (input != NULL)
    fclose(input);
  if (output != NULL)
    fclose(output);
  if (errout != NULL)
    fclose(errout);
  if (copyin != NULL)
    fclose(copyin);
  if (redirect != NULL)
    fclose(redirect);
  if (errcount==0) exit(EXIT_SUCCESS); else exit(EXIT_FAILURE);
}



/* End. */
