
/* dpic translator program. */

/* BSD Licence:

    Copyright (c) 2015, J. D. Aplevich
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

/* Comments within D ... D pairs are (Pascal) debug statements that get
   converted to C if debug is enabled. Comments within other uppercase
   pairs are activated for different operating systems or compilers. */



#include "p2c.h"
#include <time.h>
#if defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
#define __CYGMIN
#endif


/*D,log D*/

/*X program dpic(input,output); X*/

/* exit label for parser errors */

/* include dp0.h */
/* dp0.x */
/* Global definitions */

/* Some PC versions of p2c crash on Pascal const declarations at low levels.
   All consts should be in dp0.x */

/* Version= 'dpic version dpic version 2015.08.31'; p2c crashes on this */
/* UMBX distmax = 1E25; XBMU */

#define distmax         3.40282347e+38        /* assumes at least IEEE single */
/*DM MaxReal = distmax; MD*/
/*GH distmax = MaxReal; HG*/
#define mdistmax        (-distmax)
#define pi              3.1415926535897931
#define ln10            2.3025850929940459

#define SVGPX           90                             /* SVG pixels per inch */

/*UGH maxint = 2147483647; HGU*/

#define randmax         (double) LONG_MAX
#if defined(RAND_MAX)
#undef randmax
#define randmax (double) RAND_MAX
#endif

#define HASHLIM         9               /* Hash array upper val for var names */

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
#define prodmax         182
#define lrmax           4788
#define lxmax           456
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
#define systemcmd1      84
#define defhead1        85
#define defhead2        86
#define sprintf1        87
#define sprintf2        88
#define exprlist1       89
#define exprlist2       90
#define object1         91
#define object2         92
#define object3         93
#define object4         94
#define object5         95
#define object6         96
#define object7         97
#define object8         98
#define object9         99
#define object10        100
#define object11        101
#define object12        102
#define object13        103
#define object14        104
#define object15        105
#define object16        106
#define object17        107
#define object18        108
#define object19        109
#define object20        110
#define object21        111
#define object22        112
#define object23        113
#define object24        114
#define object25        115
#define object26        116
#define object27        117
#define openblock1      118
#define block1          119
#define block2          120
#define block3          121
#define block4          122
#define optexp1         123
#define optexp2         124
#define closeblock1     125
#define objectwith1     126
#define objectwith2     127
#define objectwith3     128
#define pair1           129
#define pair2           130
#define nth1            131
#define nth2            132
#define nth3            133
#define primobj1        134
#define primobj2        135
#define primobj3        136
#define primobj4        137
#define shift1          138
#define shift2          139
#define shift3          140
#define location1       141
#define location2       142
#define location3       143
#define location4       144
#define location5       145
#define place1          146
#define place2          147
#define place3          148
#define place4          149
#define place5          150
#define factor1         151
#define factor2         152
#define factor3         153
#define placename1      154
#define placename2      155
#define placename3      156
#define placename4      157
#define ncount1         158
#define ncount2         159
#define ncount3         160
#define logprod1        161
#define logprod2        162
#define logval1         163
#define logval2         164
#define logval3         165
#define lcompare1       166
#define lcompare2       167
#define lcompare3       168
#define lcompare4       169
#define primary1        170
#define primary2        171
#define primary3        172
#define primary4        173
#define primary5        174
#define primary6        175
#define primary7        176
#define primary8        177
#define primary9        178
#define primary10       179
#define primary11       180
#define primary12       181
#define primary13       182

/* include lxcst.h */
/*D XAND = 36; D*/
#define XBLOCK          26
#define XLBRACKET       21
#define XRBRACKET       22
#define XCOMMENT        42
#define XLcolrspec      137
#define XLcolour        138
#define XCOLON          20
#define XD              25
#define XDOUBLEHEAD     150
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
#define XLEFTHEAD       148
#define XLT             4
#define XLabel          39
#define XLabove         145
#define XLabs           113
#define XLacos          114
#define XLarc           160
#define XLarcrad        166
#define XLarg           43
#define XLarrow         162
#define XLarrowhd       147
#define XLarrowhead     183
#define XLarrowht       167
#define XLarrowwid      168
#define XLasin          115
#define XLat            54
#define XLatan2         128
#define XLbelow         146
#define XLbox           157
#define XLboxht         169
#define XLboxrad        170
#define XLboxwid        171
#define XLccw           6
#define XLcenter        142
#define XLchop          59
#define XLcircle        158
#define XLcirclerad     172
#define XLcoloneq       30
#define XLcontinue      58
#define XLcorner        87
#define XLcos           116
#define XLcw            5
#define XLdashed        135
#define XLdashwid       173
#define XLdefine        78
#define XLdiameter      109
#define XLdirecton      151
#define XLdiveq         34
#define XLdo            84
#define XLdotted        134
#define XLdown          153
#define XLellipse       159
#define XLellipseht     174
#define XLellipsewid    175
#define XLelse          82
#define XLendfor        85
#define XLenvvar        165
#define XLexec          75
#define XLexp           117
#define XLexpe          118
#define XLfillval       184
#define XLfloat         37
#define XLfloor         126
#define XLfrom          52
#define XLfunc1         112
#define XLfunc2         127
#define XLheight        106
#define XLint           119
#define XLinvis         136
#define XLlastenv       188
#define XLlastsc        182
#define XLleft          155
#define XLlength        111
#define XLline          161
#define XLlineht        176
#define XLlinethick     185
#define XLlinetype      132
#define XLlinewid       177
#define XLljust         143
#define XLlog           120
#define XLloge          121
#define XLcompare       99
#define XLmax           129
#define XLmaxpsht       186
#define XLmaxpswid      187
#define XLmin           130
#define XLminuseq       32
#define XLmove          163
#define XLmoveht        178
#define XLmovewid       179
#define XLmulteq        33
#define XLname          38
#define XLoutlined      139
#define XLparam         105
#define XLpluseq        31
#define XLpmod          131
#define XLprimitiv      156
#define XLradius        108
#define XLremeq         35
#define XLright         154
#define XLrjust         144
#define XLscale         188
#define XLshaded        140
#define XLsign          122
#define XLsin           123
#define XLsolid         133
#define XLspline        164
#define XLsqrt          124
#define XLstring        41
#define XLtan           125
#define XLtextht        180
#define XLtextoffset    181
#define XLtextpos       141
#define XLtextwid       182
#define XLthen          57
#define XLthickness     110
#define XLto            53
#define XLaTeX          40
#define XLundefine      79
#define XLup            152
#define XLwidth         107
#define XNEQ            101
#define XNOT            16
#define XNL             14
#define XRBRACE         24
#define XRIGHTHEAD      149
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
#define ordMINCH        0                  /* first element of type character */

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
#define CHBUFSIZ        4095            /* size of chbuf arrays, input record */
#define FILENAMELEN     1024                      /* max length of file names */

/* Lalr machine parameters */
#define STACKMAX        255                /* size of attstack and parsestack */
#define REDUMAX         128                       /* size of reduction buffer */
#define MAXERRCOUNT     3                /* max no of errors before giving up */

/* Text parameters */
#define DFONT           11            /* default svg textht, pt; should be adj*/

#define TEXTRATIO       1.6                  /* baseline to text height ratio */

/* Draw types (these could just be 1, 2, ... ) */

#define MFpic           1
#define MPost           2
#define PDF             4
#define PGF             8
#define Pict2e          16
#define PS              32
#define PSfrag          64
#define PSTricks        128
#define SVG             256
#define TeX             512
#define tTeX            1024
#define xfig            2048                                 /* PSmps = 4096; */


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

/* Production types */
/* For storing names */

typedef struct nametype {
  Char *segmnt;
  chbufinx seginx;
  int len;
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
                  endpos.xpos, endpos.ypos (uses XLline) */
      postype endpos;
      double height, width, lfill, aradius;
      int atype;
    } Uline;
  } Upr;
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
  } Upr;
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
  } Upr;
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
  } Upr;
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
  } Upr;
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
  primitive *prim, *internal;
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


/*BX def XB*/
/* Machine-dependent characters */
/* tabch, nlch, crch, etxch: char; */

/* File names */
FILE *input, *output, *errout;                 /*G asmname 'std_err'; G*/
FILE *copyin;                                  /*G asmname 'copy_in'; G*/
FILE *redirect;                              /*G asmname 'redi_rect'; G*/
/*D log: text; D*/
/*DG asmname 'log_file'; GD*/

mstring infname;                         /* name of current input file */
mstring outfnam;                        /* name of current output file */
/*X infname: string; X*/

boolean inputeof;                                 /* end of input flag */
boolean forbufend;                                /* end of for buffer */
int argct;                             /* argument counter for options */
int drawmode;                                     /* output conversion */
boolean safemode;                               /* disable sh and copy */
/*D oflag: integer; D*/

/* Lexical analyzer character buffer */
/* chbuf: strptr; */
Char *chbuf;
chbufinx chbufi, oldbufi;                    /* character buffer index */

/* Lexical variables */
Char ch;                                          /* current character */
short newsymb;                               /* current lexical symbol */
int lexstate;                    /* 0..4: <.PS; .PS; in pic; .PE; >.PE */
int lexsymb;
boolean inlogic;                      /* set < to <compare> in context */
boolean instr;                           /* set while reading a string */
fbuffer *inbuf, *savebuf, *freeinbuf;

/* Error handling */
int errcount;                     /* becomes nonzero when errors found */
int lineno;                               /* current input line number */
int currprod;                     /* current production for error msgs */

/* Production variables */
attribute *attstack;
/*D stackhigh: integer;D*/
redubufrange reduinx, redutop;
reduelem redubuf[REDUMAX + REDUMAX + 1];           /* reduction buffer */

double floatvalue;                   /* numerical value of floats read */
primitive *envblock;

double north, south, east, west;     /* compass corners of a primitive */
double xfheight;                   /* for calculating xfig coordinates */
Char *freeseg;                        /* segment open to store strings */
short freex;                                     /* next free location */
Char *tmpbuf;                        /* buffer for snprintf or sprintf */
double scale, fsc;               /* scale factor and final scale factor*/
int splcount, spltot;                          /* spline depth counter */
int pdfobjcount;                                        /* pdf objects */
primitive *snode;                            /* temporary node storage */
boolean bfill;                         /* fill flag for linear objects */
double vfill;                                            /* fill value */
nametype *sshade, *soutline;         /* temp values for linear objects */
double lastfillval;                          /* last-used fill density */
int printstate;                       /* for passing output state info */
/* graphics state parameters */
boolean gsocolor, gsfcolor, gsgcolor;
    /* stroke, fill, gray fill flags */
double gslinethick;                        /* last-used line thickness */
int gslinecap, gslinejoin;                                 /* 0 = butt */
double gsdashw, gsdashs;                /* line dash and space lengths */
nametype *stream, *cx;           /* pdf stream storage and current seg */
int pdfoffs[8];                              /* pdf output byte counts */

/* Global tables for easy C conversion.
   Alternative: use run-time space */
short lr[lrmax + 1]={
0,   5,1,   0,  0,   0,  10,6,   0, -1,  15,3765,1,   1,  1,
0,  20,1,  44,  7,  25, 585,2,  16,152,  30, 580,2, 165,170,
35, 570,2,  38,171,  40, 560,2,  37,172,  45, 415,2,   7,173,
50, 325,2,  80,177,  55, 315,2, 112,179,  60, 305,2, 127,180,
65, 285,2,  39,154,  70, 210,2,  87,148,  75, 205,2,  66,150,
80, 180,2,  68,133,  85, 175,2,  27,159,  90,  95,2,  23,160,
0,4514,4,   0,  7, 100, 675,1,  10,  2, 105, 620,1,  11, 66,
110, 585,1,  16,152, 115, 580,1, 165,170, 120, 570,1,  38,171,
125, 560,1,  37,172, 130, 415,1,   7,173, 135, 325,1,  80,177,
140, 315,1, 112,179, 145, 305,1, 127,180, 150, 285,1,  39,154,
155, 210,1,  87,148, 160, 205,1,  66,150, 165, 180,1,  68,133,
170, 175,1,  27,159,   0,  95,1,  23,160, 100, 675,1,  10,  2,
0,4314,3,   0,133,   0, 190,1,  67,  3, 195, 200,2,  68,132,
0,4519,4,   0,131,   0,4314,3,   2,132,   0, 300,3,   0,150,
215,1720,1,  61,  4, 220, 285,1,  39,154, 225, 180,1,  68,133,
165, 230,1,  37,158,   0, 185,3,   0,-158, 240, 275,1, 156,  5,
245, 270,1,  26,135, 250, 265,1,  41,136,   0, 255,1,  21,137,
0, 260,1,  22,  6,   0,4326,3,   1,137,   0,4326,3,   0,-136,
0,4326,3,   0,-135,   0,4326,3,   0,-134,   0,4466,3,   1,155,
290, 295,2,  21, 33,   0,3858,6,   0, 32, 100, 675,1,  10,  2,
0,4346,3,   0,-143,   0, 310,1,   7,  7, 100, 675,1,  10,  2,
0, 320,1,   7,  7, 100, 675,1,  10,  2,   0, 330,1,   7,  7,
95,1670,1,   8,  8, 340, 385,2, 105,176, 345, 360,2,  25,156,
350, 355,2,  87,147,   0,4524,4,   0,146,   0, 300,3,   1,147,
220, 375,1,  39,  9, 240, 275,1, 156,  5,   0,4466,3,   3,157,
290, 295,2,  21, 33,   0,4466,3,   3,156,   0,4506,3,   1,176,
395, 925,1,  70, 10, 400, 920,1,  71,175, 405, 930,1,   9,144,
0, 410,1,  12,145, 110, 585,1,  16, 11, 420,1495,1,  76, 12,
425,1295,1,  38, 60, 430,1260,1, 165, 62, 435, 675,1,  10, 65,
440, 620,1,  11, 66, 445, 415,1,   7,141, 450, 210,1,  87,148,
455, 205,1,  66,150, 460, 585,1,  16,152, 465, 520,1,  41, 58,
470, 285,1,  39,154, 475, 560,1,  37,172, 480, 325,1,  80,177,
485, 315,1, 112,179, 490, 305,1, 127,180, 160, 495,1,  86, 87,
0, 500,1,   7,  7, 505, 520,1,  41, 13,   0, 495,1,  86, 87,
0, 515,1,   8, 14,   0,4070,3,   1,-59,   0,4070,3,   0, 58,
0,3998,3,   0,-56, 535, 555,2,  19, 88, 540, 545,2,  10, 57,
0,4529,4,   0, 87, 505, 520,1,  41, 13,   0,3998,3,   2, 57,
100, 675,1,  10,  2, 565, 230,4,  67,158,   0,4534,4,   0,172,
290, 295,2,  21, 33,   0,4506,3,   1,171,   0,4506,3,   0,170,
115, 580,1, 165, 15,   0,4438,3,   1,152, 600, 605,2,  15,153,
0,4539,4,   0,151, 110, 585,1,  16, 11,   0,4438,3,   2,153,
0,3818,3,   0,-13, 110, 585,1,  16, 11, 630, 665,2,   9, 14,
635, 655,2,  12, 15, 640, 645,2,  13, 16,   0,4544,4,   0, 66,
110, 585,1,  16, 11,   0,3818,3,   2, 16, 110, 585,1,  16, 11,
0,3818,3,   2, 15, 110, 585,1,  16, 11,   0,3818,3,   2, 14,
110, 585,1,  16, 11, 685, 665,2,   9, 14, 690, 655,2,  12, 15,
695, 645,2,  13, 16,   0,4549,4,   0, 65, 705, 665,2,   9, 14,
710, 655,2,  12, 15, 715, 645,2,  13, 16,   0,4554,4,   0, 64,
725, 790,2,  19, 90, 730, 765,2,  10, 67, 735, 740,2,  11, 68,
0,4559,4,   0, 89, 110, 585,1,  16, 11, 750, 665,2,   9, 14,
755, 655,2,  12, 15, 760, 645,2,  13, 16,   0,4564,4,   0, 68,
110, 585,1,  16, 11, 775, 665,2,   9, 14, 780, 655,2,  12, 15,
785, 645,2,  13, 16,   0,4569,4,   0, 67, 100, 675,1,  10,  2,
0,4266,3,   2, 90,   0, 510,3,   4, 88, 810, 820,2,   4,164,
815, 545,2,  10, 57,   0,4574,4,   0,167, 505, 520,1,  41, 13,
830, 545,2,  10, 57,   0,4579,4,   0,164, 840, 845,2,  99,168,
0,4584,4,   0,163, 850, 675,1,  10, 16, 855, 620,1,  11, 66,
860, 520,1,  41, 58, 865, 585,1,  16,152, 110, 495,1,  86, 87,
875, 545,2,  10, 57,   0,4589,4,   0,169, 885, 765,2,  10, 67,
890, 740,2,  11, 68,   0,4594,4,   0,168, 900, 930,2,   9,144,
905, 410,2,  12,145, 910, 925,2,  70,174, 915, 920,2,  71,175,
0,4338,6,   0,138,   0,4506,3,   1,175,   0,4506,3,   1,-174,
110, 585,1,  16, 11,   0,4346,3,   2,144, 945,1140,2,  10,139,
950, 955,2,  11,140,   0,4599,4,   0,130, 960, 980,1,   7, 17,
965, 210,1,  87,148, 215, 205,1,  66,150, 975, 355,2,  87,147,
350, 360,2,  25,156, 985, 675,1,  10,  2, 990, 620,1,  11, 66,
995, 415,1,   7,141,1000, 210,1,  87,148,1005, 205,1,  66,150,
1010, 585,1,  16,152,1015, 285,1,  39,154,1020, 580,1, 165,170,
1025, 570,1,  38,171,1030, 560,1,  37,172,1035, 325,1,  80,177,
1040, 315,1, 112,179, 160, 305,1, 127,180,1050,1195,1,  64, 18,
1055,1160,1,  61, 36,1060,1095,1,   4, 37,1065,1075,1,  19,129,
1070, 765,1,  10, 67,   0, 740,1,  11, 68, 100, 675,1,  10,  2,
1085, 765,2,  10, 67,1090, 740,2,  11, 68,   0,4604,4,   0,129,
985, 675,1,  10,  2,   0,3886,3,   0,-34,   0,1110,1,  19, 19,
985, 675,1,  10,  2,   0,1120,1,  99, 20,   0,4338,6,   0,138,
1130,1140,2,  10,139,1135, 955,2,  11,140,   0,4609,4,   0, 37,
960, 980,1,   7, 17,1150, 930,2,   9,144,1155, 410,2,  12,145,
0,4614,4,   0,139,   0,1165,1,  62, 21,   0,1170,1,  63, 22,
0,1175,1,  64, 23, 985, 675,1,  10,  2,   0,1185,1,  65, 24,
985, 675,1,  10,  2,   0,3886,3,   7, 36, 985, 675,1,  10,  2,
0,1205,1,  65, 24, 985, 675,1,  10,  2,   0,3886,3,   4, 35,
1220,1240,1,   8, 25,   0,1225,1,  19,142, 985, 675,1,  10,  2,
0,1235,1,   8, 14,   0,4346,3,   4,142,   0,4346,3,   2,141,
1250, 930,2,   9,144,1255, 410,2,  12,145,   0,4619,4,   0,140,
1265,1270,2,  29, 62,   0, 580,4,   0,170,1275, 675,1,  10,  2,
1280, 620,1,  11, 66,1285,1295,1,  38, 60,1290,1260,1, 165, 62,
120, 585,1,  16,152, 290, 295,2,  21, 33,1305,1310,2,  29, 60,
0, 575,4,   0,171,1275, 675,1,  10,  2,   0,4078,3,   3, 61,
1325, 765,2,  10, 67,1330, 740,2,  11, 68,   0,4624,4,   0, 60,
0,4078,3,   2, 63,1345, 765,2,  10, 67,1350, 740,2,  11, 68,
0,4629,4,   0, 62,   0,4490,3,   0,-161,1365,1195,2,  64, 35,
1370,1160,2,  61, 36,1375,1395,2,   4, 37,1380,1075,2,  19,129,
1385, 765,2,  10, 67,1390, 740,2,  11, 68,   0,4634,4,   0,166,
1400, 675,1,  10,  2,1405, 620,1,  11, 66,1410, 415,1,   7,141,
1415, 585,1,  16,152,1420, 210,1,  87,148,1425, 205,1,  66,150,
1430, 580,1, 165,170,1435, 570,1,  38,171,1440, 560,1,  37,172,
1445, 325,1,  80,177,1450, 315,1, 112,179,1455, 305,1, 127,180,
160, 285,1,  39,154,1465,1195,2,  64, 35,1470,1160,2,  61, 36,
1475,1095,2,   4, 37,1480, 765,2,  10, 67,1485, 740,2,  11, 68,
1490,1075,2,  19,129,   0,4639,4,   0,165, 505, 520,1,  41, 13,
1505, 545,2,  10, 57,   0,4644,4,   0, 84,   0,3958,3,   0,-38,
1520,1525,2,  17,162,   0,4649,4,   0, 71,1530, 675,1,  10, 16,
1535, 620,1,  11, 66,1540, 520,1,  41, 58, 105, 495,1,  86, 87,
1380,1550,2,   4,165, 100, 675,1,  10,  2,1560, 765,2,  10, 67,
1490, 740,2,  11, 68,   0,4490,3,   2,162,   0,1575,1,   8, 14,
0,4506,3,   2,182,1585,1620,1,   8, 25,   0,1590,1,  19, 39,
1595,1605,1,  38, 26,   0,1600,1, 165, 62,   0,1270,1,  29, 27,
290, 295,2,  21, 33,   0,1310,1,  29, 27,   0,3958,3,   2, 39,
0,4506,3,   2,181,1630,1650,1,   8, 28,   0,1635,1,  18, 72,
1530, 675,1,  10, 16,1645,1525,2,  17,162,   0,4654,4,   0, 72,
0,4506,3,   2,173,   0,4346,3,   2,145,1065,1665,1,   8, 29,
0,4506,3,   3,178,   0,4506,3,   2,177,1065,1680,1,   8, 29,
0,4506,3,   3,179,1065,1690,1,  19, 30, 100, 675,1,  10,  2,
1065,1700,1,   8, 29,   0,4506,3,   5,180,1065,1710,1,  22, 31,
0,3858,3,   2, 33,   0,4466,3,   1,154, 220, 285,1,  39,  9,
1730, 360,2,  25,156,   0,4659,4,   0,149,1740, 360,2,  25,156,
0,4664,4,   0,148,1065,1750,1,  28, 32,   0, 185,3,   2,159,
1065,1760,1,  24, 33,   0, 185,3,   2,160,1770, 665,2,   9, 14,
1775, 655,2,  12, 15,1780, 645,2,  13, 16,1785, 585,2,  16,152,
1790, 580,2, 165,170,1795, 570,2,  38,171,1800, 560,2,  37,172,
1805, 415,2,   7,173,1810, 325,2,  80,177,1815, 315,2, 112,179,
1820, 305,2, 127,180,1825, 285,2,  39,154,1830, 210,2,  87,148,
1835, 205,2,  66,150,1840, 180,2,  68,133,1845, 175,2,  27,159,
1850,  95,2,  23,160,   0,4669,4,   0,  8,1860, 665,2,   9, 14,
1865, 655,2,  12, 15,1870, 645,2,  13, 16,   0,4674,4,   0,  9,
1880,1890,1,  14, 34,   0,1885,1,   3,  6,   0,3770,3,   0, -6,
0,3770,3,   0, -5,1900,3170,2,  39, 18,1905,3155,2, 151, 20,
1910,3150,2,  40, 21,1915,3075,2,  77, 27,1920,3060,2,  75, 28,
1925,2500,2,  72, 40,1930,2465,2,  74, 43,1935,2445,2,  73, 46,
1940,2390,2,  79, 48,1945,3135,6,  23, 29,1950,2260,2,  58,117,
1955,1605,2,  38, 60,1960,1600,2, 165, 62,1965,1495,2,  76, 84,
1970,2240,2,  78, 85,1975,2145,2,  83, 73,1980,2040,2, 156,119,
1985,2220,6,  81, 70,1990,2010,6,  21,118,1995,2010,6,  26,118,
2000, 520,2,  41, 58,2005, 495,2,  86, 87,   0,3790,6,   0, 10,
2015,2025,1,  21, 35,   0,2020,1,  26,122,   0,2365,3,   1,122,
1900,3170,2,  39, 18,2035, 545,2,  10, 57,   0,4679,4,   0,120,
2045, 675,2,  10, 65,2050, 620,2,  11, 66,2055, 585,2,  16,152,
2060, 580,2, 165,170,2065, 570,2,  38,171,2070, 560,2,  37,172,
2075, 415,2,   7,173,2080, 325,2,  80,177,2085, 315,2, 112,179,
2090, 305,2, 127,180,2095, 285,2,  39,154,2100, 210,2,  87,148,
2105, 205,2,  66,150,2110, 180,2,  68,133,2115, 175,2,  27,159,
2120,  95,2,  23,160,   0,4282,6,   0,123,2130, 765,2,  10, 67,
2135, 740,2,  11, 68,   0,4684,4,   0,124,   0,2365,3,   1,119,
1595,1605,1,  38, 26,1585,2155,1,  53, 36, 100, 675,1,  10,  2,
2165, 765,1,  10, 37,2170, 740,1,  11, 68,2175,2210,1,  84, 75,
0,2180,1,  56, 77,2185,2190,2,   9, 78,   0,4689,4,   0, 77,
0,2195,3,   1, 78, 100, 675,1,  10,  2,1065,2205,1,  84, 38,
0,2215,3,   2, 76,   0,2215,3,   0, 75,   0,2370,3,   4, 73,
0,2225,1,  81, 39,1530, 675,1,  10, 16,1630,2235,1,  57, 40,
0,2380,3,   3, 69,2245,2255,1,  38, 41,   0,2250,1,  39, 86,
0,2410,3,   1, 86,   0,2410,3,   1, 85,   0,4274,3,   0,117,
2270,2355,1,  54, 42,2275,2340,1,  87,115,2280,2300,1,  25,127,
2285, 675,1,  10, 65,2290, 620,1,  11, 66,1000, 415,1,   7,141,
1065,1075,1,  19, 30, 220,2315,1,  39,  9, 240, 275,1, 156,  5,
0,2265,3,   3,128, 290, 295,2,  21, 33,   0,2265,3,   3,127,
0,2330,1,  54, 43, 985, 675,1,  10,  2,   0,4274,3,   3,116,
210,2345,1,  54, 44, 985, 675,1,  10,  2,   0,4274,3,   3,115,
985, 675,1,  10,  2,   0,4274,3,   2,114,   0,4274,3,   0,-91,
0,2375,1,  23, 45,1900,3170,2,  39, 18,   0,2385,1,  23, 45,
1900,3170,2,  39, 18,2395,2405,1,  38, 41,   0,2400,1,  39, 49,
0,3145,3,   1, 49,   0,3145,3,   1, 48,2415,1890,2,  14,  5,
2420,1885,2,   3,  6,   0,3970,6,   0,-50,   0,3970,3,   0,-51,
0,2435,1,  23, 45,   0,2440,1,  24, 46,   0,3145,3,   3,-47,
505, 520,1,  41, 13,2455, 545,2,  10, 57,   0,4694,4,   0, 46,
0,3145,3,   0,-45,2470,2475,2, 165, 82,   0,4699,4,   0, 43,
0,2480,3,   0, 82,2485,2490,2,  19, 83,   0,4704,4,   0, 44,
0,2495,1, 165, 47,   0,2480,3,   2, 83,2505, 675,1,  10, 16,
2510, 620,1,  11, 66,2515, 520,1,  41, 58,2520, 585,1,  16,152,
2525, 415,1,   7,141,2530, 495,1,  86, 87,2535, 580,1, 165,170,
2540, 570,1,  38,171,2545, 560,1,  37,172,2550, 325,1,  80,177,
2555, 315,1, 112,179,2560, 305,1, 127,180,2565, 210,1,  87,148,
2570, 205,1,  66,150, 160, 285,1,  39,154,2580, 545,2,  10, 57,
2585,2590,2,  99, 80,   0,4246,6,   0, 79, 500,2595,1,  99, 48,
505, 520,1,  41, 13,2605, 545,2,  10, 57,   0,4709,4,   0, 81,
2615, 545,2,  10, 57,   0,4714,4,   0, 80,   0,3145,3,   2, 42,
2585,2590,2,  99, 80,   0,3145,3,   2, 41,2640, 765,2,  10, 67,
2645, 740,2,  11, 68,2650,1195,2,  64, 35,2655,1160,2,  61, 36,
2660,1095,2,   4, 37,2580,1075,2,  19,129,   0,3145,3,   2, 40,
2675,3040,2,  46, 92,2680,3020,2,  47, 93,2685,3000,2,  48, 94,
2690,2980,2,  49, 95,2695,2960,2,  50, 96,2700,2940,2,  51, 97,
2705,2930,2, 151, 98,2710,2920,2, 132, 99,2715,2910,2,  59,100,
2720,2900,2,  69,101,2725,2890,2, 147,102,2730,2885,2,  57,103,
2735,2880,2,   5,104,2740,2875,2,   6,105,2745,2870,2,  60,106,
2750,2850,2,  56,108,2755,2840,2,  52,109,2760,2830,2,  53,110,
2765,2820,2,  54,111,2770,2815,2, 141,112,2775,2800,2, 137,113,
2780,2795,2,  55,126,2785, 520,2,  41, 58,2790, 495,2,  86, 87,
0,4719,4,   0, 30,   0,2265,3,   1,-126, 505, 520,1,  41, 13,
2810, 545,2,  10, 57,   0,4724,4,   0,113,   0,4274,3,   1,112,
985, 675,1,  10,  2,   0,4274,3,   2,111, 985, 675,1,  10,  2,
0,4274,3,   2,110, 985, 675,1,  10,  2,   0,4274,3,   2,109,
985, 675,1,  10,  2,   0,4274,3,   2,108,2865, 545,2,  10, 57,
0,4729,4,   0,107,   0,4274,3,   1,106,   0,4274,3,   1,105,
0,4274,3,   1,104,   0,4274,3,   1,103,2045, 675,2,  10, 65,
0,4274,3,   2,102,2045, 675,2,  10, 65,   0,4274,3,   2,101,
2045, 675,2,  10, 65,   0,4274,3,   2,100,2045, 675,2,  10, 65,
0,4274,3,   2, 99,2045, 675,2,  10, 65,   0,4274,3,   2, 98,
100, 675,1,  10,  2,2950, 765,2,  10, 67,2955, 740,2,  11, 68,
0,4734,4,   0, 97, 100, 675,1,  10,  2,2970, 765,2,  10, 67,
2975, 740,2,  11, 68,   0,4739,4,   0, 96, 100, 675,1,  10,  2,
2990, 765,2,  10, 67,2995, 740,2,  11, 68,   0,4744,4,   0, 95,
100, 675,1,  10,  2,3010, 765,2,  10, 67,3015, 740,2,  11, 68,
0,4749,4,   0, 94, 100, 675,1,  10,  2,3030, 765,2,  10, 67,
3035, 740,2,  11, 68,   0,4754,4,   0, 93, 100, 675,1,  10,  2,
3050, 765,2,  10, 67,3055, 740,2,  11, 68,   0,4759,4,   0, 92,
505, 520,1,  41, 13,3070, 545,2,  10, 57,   0,4764,4,   0, 28,
505, 520,1,  41, 13,3085, 545,2,  10, 57,   0,4769,4,   0, 27,
3095,3585,2,  24, 26,   0,3100,6,   0, 74,   0,3105,1,  85, 49,
1900,3170,2,  39, 18,   0,3115,1,  23, 45,1900,3170,2,  39, 18,
3125,3130,2,  82, 53,   0,4774,4,   0, 24,   0,3110,3,   1, 53,
0,3140,1,  23, 45,1900,3170,2,  39, 18,   0,3846,3,   0, 22,
0,3846,3,   0, 21,   0,3846,3,   0, 20,3165,1590,2,  19, 39,
0,4779,4,   0, 19, 290, 295,2,  21, 33,   0,3180,1,  20, 50,
3185,2260,2,  58,117,3190, 675,2,  10, 65,3195, 620,2,  11, 66,
3200,2040,2, 156,119,3205, 415,2,   7,141,3210, 210,2,  87,148,
3215, 205,2,  66,150,3220, 585,2,  16,152,3225, 520,2,  41, 58,
3230, 285,2,  39,154,3235, 580,2, 165,170,3240, 570,2,  38,171,
3245, 560,2,  37,172,3250, 325,2,  80,177,3255, 315,2, 112,179,
3260, 305,2, 127,180,3265, 495,2,  86, 87,3270, 180,2,  68,133,
3275, 175,2,  27,159,3280,  95,2,  23,160,   0,2010,6,   0,118,
3290,3040,2,  46, 92,3295,3020,2,  47, 93,3300,3000,2,  48, 94,
3305,2980,2,  49, 95,3310,2960,2,  50, 96,3315,2940,2,  51, 97,
3320,2930,2, 151, 98,3325,2920,2, 132, 99,3330,2910,2,  59,100,
3335,2900,2,  69,101,3340,2890,2, 147,102,3345,2885,2,  57,103,
3350,2880,2,   5,104,3355,2875,2,   6,105,3360,2870,2,  60,106,
3365,2850,2,  56,108,3370,2840,2,  52,109,3375,2830,2,  53,110,
3380,2820,2,  54,111,3385,2815,2, 141,112,3390,2800,2, 137,113,
3395,2795,2,  55,126,3400, 520,2,  41, 58,3405, 495,2,  86, 87,
0,4784,4,   0, 31,   0,3846,3,   3, 18,   0,3846,3,   0, 17,
0,3790,3,   0, 11,2415,1890,2,  14,  5,3435,3170,2,  39, 18,
3440,3155,2, 151, 20,3445,3150,2,  40, 21,3450,3075,2,  77, 27,
3455,3060,2,  75, 28,3460,2500,2,  72, 40,3465,2465,2,  74, 43,
3470,2445,2,  73, 46,3475,2390,2,  79, 48,3480,3135,6,  23, 29,
3485,2260,2,  58,117,3490,1605,2,  38, 60,3495,1600,2, 165, 62,
3500,1495,2,  76, 84,3505,2240,2,  78, 85,3510,2145,2,  83, 73,
3515,2040,2, 156,119,3520,2220,6,  81, 70,3525,2010,6,  21,118,
3530,2010,6,  26,118,3535, 520,2,  41, 58,3540, 495,2,  86, 87,
0,2425,4,   0, 51,   0,3790,3,   2, 12,   0,3555,1,  24, 46,
0,3846,3,   4, 23,2415,1890,2,  14,  5,   0,3570,1,  24, 46,
0,3846,3,   4,-25,2415,1890,2,  14,  5,   0,3090,3,   4, 55,
0,3846,3,   1,-26,2415,1890,2,  14,  5,   0,3600,1,  24, 46,
0,3120,3,   4,-52,2415,1890,2,  14,  5,   0,3090,3,   3,-54,
2415,1890,2,  14,  5,   0,3625,3,   1,125,   0,3630,1,  22,  6,
0,2365,3,   3,121,1875,3750,1,  45, 51,3645,3745,2,  45,  4,
3650,3170,2,  39, 18,3655,3155,2, 151, 20,3660,3150,2,  40, 21,
3665,3075,2,  77, 27,3670,3060,2,  75, 28,3675,2500,2,  72, 40,
3680,2465,2,  74, 43,3685,2445,2,  73, 46,3690,2390,2,  79, 48,
3695,3135,6,  23, 29,3700,2260,2,  58,117,3705,1605,2,  38, 60,
3710,1600,2, 165, 62,3715,1495,2,  76, 84,3720,2240,2,  78, 85,
3725,2145,2,  83, 73,3730,2040,2, 156,119,3735,2220,6,  81, 70,
3740, 520,2,  41, 58,3280, 495,2,  86, 87,   0,3755,3,   4,  4,
0,3755,3,   3,  3,1880,1890,1,  14, 34,   0,  10,3,   2,  2,
0,   0,0,   2,  0,3774,1895,5,1875    ,3778,2425,5,2410    ,
3782,3640,5,3635    ,3786,3760,5,3755    ,   0,3430,5,   0    ,
3794,3615,5,2025    ,3798,3605,5,2375    ,3802,3590,5,2385    ,
3806,3575,5,3105    ,3810,3560,5,3115    ,3814,3425,5,3140    ,
0,3635,5,   0    ,3822,1765,5,  20    ,3826, 625,5, 620    ,
3830, 680,5, 675    ,3834, 745,5, 740    ,3838, 770,5, 765    ,
3842,1855,5,1765    ,   0, 700,5,   0    ,3850,3545,5,3430    ,
3854,3545,5,3640    ,   0,3420,5,   0    ,3862, 380,5, 375    ,
3866, 575,5, 570    ,3870,1300,5,1295    ,3874,1610,5,1605    ,
3878,2320,5,2315    ,3882,3175,5,3170    ,   0,1715,5,   0    ,
3890,1105,5,1095    ,3894,1115,5,1110    ,3898,1180,5,1175    ,
3902,1190,5,1185    ,3906,1200,5,1195    ,3910,1210,5,1205    ,
3914,1230,5,1225    ,3918,1105,5,1395    ,3922,2335,5,2330    ,
3926,2350,5,2345    ,3930,2360,5,2355    ,3934,2625,5,2500    ,
3938,2825,5,2820    ,3942,2835,5,2830    ,3946,2845,5,2840    ,
3950,2855,5,2850    ,3954,3410,5,3180    ,   0,1215,5,   0    ,
3962,1580,5, 415    ,3966,2150,5,2145    ,   0,3160,5,   0    ,
3974,3550,5,3425    ,3978,3565,5,3560    ,3982,3580,5,3575    ,
3986,3595,5,3590    ,3990,3610,5,3605    ,3994,3620,5,3615    ,
0,2430,5,   0    ,4002, 805,5, 415    ,4006, 530,5, 500    ,
4010, 825,5, 820    ,4014, 870,5, 845    ,4018,1500,5,1495    ,
4022, 805,5,1525    ,4026, 805,5,1635    ,4030, 805,5,2225    ,
4034,2450,5,2445    ,4038,2575,5,2500    ,4042,2610,5,2590    ,
4046,2600,5,2595    ,4050,2860,5,2670    ,4054,2805,5,2800    ,
4058,3065,5,3060    ,4062,3080,5,3075    ,4066,2860,5,3285    ,
0,2030,5,   0    ,4074, 550,5, 545    ,   0, 525,5,   0    ,
4082,1335,5,1270    ,4086,1315,5,1310    ,4090,1615,5,1590    ,
0,1510,5,   0    ,4098,1755,5,  95    ,4102,1745,5, 175    ,
4106,1705,5, 295    ,4110,1685,5, 310    ,4114,1675,5, 320    ,
4118,1660,5, 330    ,4122,1360,5, 415    ,4126, 720,5, 555    ,
4130, 720,5, 790    ,4134, 880,5, 845    ,4138,1080,5,1075    ,
4142,1340,5,1270    ,4146,1320,5,1310    ,4150,1460,5,1395    ,
4154,1545,5,1525    ,4158,1555,5,1550    ,4162,1545,5,1635    ,
4166,1695,5,1690    ,4170,2125,5,2040    ,4174,2160,5,2155    ,
4178,2200,5,2195    ,4182,1545,5,2225    ,4186,2295,5,2265    ,
4190,2635,5,2500    ,4194,2125,5,2890    ,4198,2125,5,2900    ,
4202,2125,5,2910    ,4206,2125,5,2920    ,4210,2125,5,2930    ,
4214,2945,5,2940    ,4218,2965,5,2960    ,4222,2985,5,2980    ,
4226,3005,5,3000    ,4230,3025,5,3020    ,4234,3045,5,3040    ,
0,1045,5,   0    ,4242,2230,5,2225    ,   0,1625,5,   0    ,
4250,2630,5,2625    ,4254,2665,5,2635    ,   0,2620,5,   0    ,
4262,1570,5, 415    ,   0,2460,5,   0    ,4270, 795,5, 790    ,
0, 800,5,   0    ,4278,3285,5,3180    ,   0,2670,5,   0    ,
4286,2895,5,2890    ,4290,2905,5,2900    ,4294,2915,5,2910    ,
4298,2925,5,2920    ,4302,2935,5,2930    ,   0,2140,5,   0    ,
4310,2325,5,2265    ,   0,1100,5,   0    ,4318, 365,5, 360    ,
4322,2305,5,2300    ,   0, 235,5,   0    ,4330, 370,5, 365    ,
4334,2310,5,2305    ,   0, 280,5,   0    ,4342,1125,5,1120    ,
0, 940,5,   0    ,4350, 895,5, 415    ,4354,1245,5, 955    ,
4358, 895,5, 980    ,4362, 895,5,1095    ,4366, 895,5,1110    ,
4370,1145,5,1140    ,4374, 895,5,1175    ,4378, 895,5,1185    ,
4382, 895,5,1195    ,4386, 895,5,1205    ,4390, 895,5,1225    ,
4394, 895,5,1395    ,4398, 895,5,2265    ,4402, 895,5,2330    ,
4406, 895,5,2345    ,4410, 895,5,2355    ,4414, 895,5,2500    ,
4418, 895,5,2820    ,4422, 895,5,2830    ,4426, 895,5,2840    ,
4430, 895,5,2850    ,4434, 895,5,3180    ,   0, 390,5,   0    ,
4442,1655,5, 410    ,4446, 610,5, 605    ,4450, 650,5, 645    ,
4454, 660,5, 655    ,4458, 670,5, 665    ,4462, 935,5, 930    ,
0, 615,5,   0    ,4470,1735,5, 210    ,4474, 970,5, 955    ,
4478, 970,5,1140    ,4482,1725,5,1720    ,4486,1735,5,2340    ,
0, 335,5,   0    ,4494,1640,5,1635    ,   0,1515,5,   0    ,
4502,1565,5,1525    ,   0,1355,5,   0    ,4510, 590,5, 585    ,
0, 595,5,   0    ,   0,1875,3,   0,  7,   0,4314,3,   1,131,
0, 300,3,   0,146,   0, 510,3,   2, 87,   0,4506,3,   0,-172,
0,4438,3,   0,-151,   0,4094,3,   1, 66,   0,4094,3,   1, 65,
0,4094,3,   0,-64,   0,4266,3,   0, 89,   0,4094,3,   2, 68,
0,4094,3,   2, 67,   0, 835,3,   0,-167,   0,4498,3,   2,164,
0,4498,3,   0,163,   0, 835,3,   2,169,   0, 835,3,   2,168,
0,4306,3,   1,130,   0,4306,3,   2,129,   0,3886,3,   6, 37,
0,4338,3,   2,139,   0,4338,3,   2,140,   0,4078,3,   3, 60,
0,4078,3,   2, 62,   0, 835,3,   0,-166,   0,4498,3,   2,165,
0,4258,3,   1, 84,   0,4238,3,   0,-71,   0,4238,3,   2, 72,
0, 300,3,   2,149,   0, 300,3,   1,148,   0,1875,3,   1,  8,
0,1875,3,   2,  9,   0,2365,3,   0,120,   0,4282,3,   0,-124,
0,2195,3,   0,-77,   0,3145,3,   1, 46,   0,3145,3,   0, 43,
0,3145,3,   1,-44,   0,4246,3,   2, 81,   0,4246,3,   1, 80,
0,3415,3,   0, 30,   0,4274,3,   2,113,   0,4274,3,   1,107,
0,4274,3,   2, 97,   0,4274,3,   2, 96,   0,4274,3,   2, 95,
0,4274,3,   2, 94,   0,4274,3,   2, 93,   0,4274,3,   2, 92,
0,3846,3,   1, 28,   0,3846,3,   1, 27,   0,3846,3,   0,-24,
0,3846,3,   0, 19,   0,3415,3,   3, 31

};
lxinx entryhp[ordMAXCH + 1]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,2,
3,0,0,0,4,5,0,6,8,100,0,0,0,0,0,0,0,0,0,0,101,0,102,105,106,0,0,0,0,0,0,0,0,
0,107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,110,0,0,0,0,0,111,144,168,213,240,
260,0,277,283,0,0,293,319,338,339,347,0,354,374,412,440,0,448,0,0,0,0,456,0,
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
341,342,343,344,345,346,0,352,349,350,351,0,353,0,368,0,365,362,359,360,361,
0,363,364,0,366,367,0,371,370,0,372,373,0,410,406,402,399,396,387,385,382,383,
384,0,386,0,393,389,390,391,392,0,394,395,0,397,398,0,401,0,0,403,404,405,0,
407,408,409,0,411,0,439,426,417,416,0,425,419,420,421,422,423,424,0,0,427,428,
438,433,431,432,0,434,435,436,437,0,0,0,442,0,443,444,445,446,447,0,455,450,
453,452,0,454,0,0,0};
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
321,0,0,0,325,0,0,0,0,0,0,0,0,334,0,0,0,0,0,340,0,0,0,0,0,0,0,348,0,0,0,0,0,
0,355,356,357,358,0,0,0,0,0,0,0,0,0,0,369,0,0,0,0,0,375,376,377,378,379,380,
381,0,0,0,0,0,0,388,0,0,0,0,0,0,0,0,0,0,0,400,0,0,0,0,0,0,0,0,0,0,0,0,413,414,
415,0,0,418,0,0,0,0,0,0,0,0,0,0,429,430,0,0,0,0,0,0,0,0,0,0,441,0,0,0,0,0,0,
0,449,0,451,0,0,0,0,0,0};
symbol lxtv[lxmax + 1]={
0,101,35,17,33,31,32,149,93,98,0,94,0,95,92,94,93,92,95,70,71,0,0,107,0,107,
0,95,0,0,92,0,0,110,0,0,0,110,89,0,0,91,0,0,96,0,0,93,0,0,0,0,94,108,0,0,108,
88,0,90,0,0,92,0,0,111,0,0,111,95,0,106,0,0,0,106,0,0,97,0,94,0,0,109,0,0,0,
109,0,0,0,0,0,98,98,0,93,0,0,93,34,30,148,103,150,100,102,0,0,66,26,0,0,0,0,
0,54,0,0,128,0,115,160,0,0,162,0,0,0,168,0,167,0,183,0,0,166,65,0,114,0,113,
0,145,0,0,56,0,157,0,0,0,0,171,0,170,169,0,0,93,0,0,0,0,0,64,0,146,0,0,0,0,0,
5,0,0,0,0,116,73,0,0,0,0,58,0,0,0,77,0,138,0,138,0,138,0,138,0,0,0,158,0,0,172,
0,59,0,0,0,0,142,142,6,0,0,0,84,0,0,153,0,0,134,0,49,0,0,0,49,0,0,0,78,0,0,0,
0,0,173,135,0,0,0,0,117,118,75,97,0,0,82,0,0,0,159,0,0,0,175,174,0,0,0,0,0,52,
83,0,0,126,0,69,0,0,0,184,69,0,46,0,0,0,46,81,0,119,0,0,136,0,0,0,136,0,0,0,
0,0,120,121,0,0,143,0,161,0,0,0,0,177,0,0,0,185,176,0,155,0,68,0,0,0,0,163,0,
0,0,179,178,130,129,0,0,0,0,0,187,186,67,61,0,0,0,0,0,139,139,0,0,0,0,72,0,131,
0,67,0,0,0,0,0,144,0,0,154,0,0,74,48,0,80,0,0,48,0,0,76,0,0,0,0,67,0,0,96,0,
124,0,0,0,0,0,86,0,0,164,0,0,133,0,123,122,0,0,140,140,0,0,188,51,0,60,0,0,67,
53,92,62,0,0,50,0,0,0,50,57,0,0,0,0,0,0,182,0,0,0,0,181,180,125,0,152,0,0,79,
0,0,79,0,0,47,0,55,0,47,63,18};
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
'l','i','n','e','d','m','r','i','n','t','o','d','a','d','e','i','j','u','s',
't','g','h','t','s','e','t','d','n','d','i','u','s','a','c','h','i','o','p',
'q','t','a','r','t','r','t','l','r','i','n','t','f','i','n','e','l','i','d',
'g','n','n','a','d','e','d','a','l','e','d','m','e','a','e','h','o','p','e',
'i','c','k','n','e','s','s','n','x','t','h','o','w','i','d','f','f','s','e',
't','t','n','n','p','d','e','f','i','n','e','a','i','d','t','h','t','h','y',
'|'};
/* integer debugging constants */
/*D debuglevel, linesignal: integer; D*/
/*D trace: boolean; D*/
/* used for debugging the parser */

/* Parser variables */
int oldsymb;                                    /* last lexical symbol */
arg *macros, *args;                   /* macro and macro argument list */
stackinx stacktop, pseudotop, validtop, top;
stackelm *parsestack;                                   /* parse stack */
boolean parsestop;                                     /* parser flags */
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

boolean firstsegment(primitive *pr);

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

void markerror(int emi);

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


/*--------------------------------------------------------------------*/

/* include sysdep.h */
/*GH#include 'sysdep.h'HG*/

double principal(double x, double r)
{ /* x,r: real): real */
  while (x > r) {
      x -= 2 * r;
  }
  while (x < -r) {
      x += 2 * r;
  }
  return x;
}


double Max(double x, double y)
{ /* x,y: real): real */
  if (y > x) {
      return y;
  }
  else {
      return x;
  }
}


double Min(double x, double y)
{ /* x,y: real): real */
  if (y < x) {
      return y;
  }
  else {
      return x;
  }
}


int Floor(double x)
{ /* x: real): integer */
  if (x >= 0 || (long)x == x) {
      return ((long)x);
  }
  else {
      return ((long)x - 1);
  }
}


int Ceil(double x)
{ /* x: real): integer */
  if (x < 0 || (long)x == x) {
      return ((long)x);
  }
  else {
      return ((long)x + 1);
  }
}


double datan(double y, double x)
{ /* y,x: real ): real */
  double r;

  /*D if debuglevel=2 then write(log,'datan(',y:8:4,',',x:8:4,')='); D*/
  r = atan2(y , x);
  /*D if debuglevel=2 then writeln(log,r:8:4); D*/
  return r;
}


void consoleflush(void)
{ /* causes immediate physical write to console, not
                               needed for most systems. */
  /*DUGHM; if debuglevel > 0 then flush(log) MHGUD*/
  fflush(errout);
  P_ioresult = 0;
}


void epilog(void)
{ /* produce(0, 0); */
  /*D if debuglevel > 0 then begin
        writeln(log,'stackhigh=',stackhigh:1);
        writeln(log,'Dpic log ends');
        writeln(log) end; D*/
  /* Seems needed for some Cygwin machines: */
  /* GH flush(errout);
  flush(stdout) HG */
}


void fatal(int t)
{ /* t: integer */
  if (t != 0) {
      fprintf(errout, " *** dpic: ");
      /* currentline( i ) */
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

/* procedure system( var s: char ); */
/*H name H*/
/*G; asmname G*/
/*GH 'system' HG*/

/* These need tweaking for different operating systems: */
/*H name H*/
/*G; asmname G*/
/*GH 'random' HG*/
#if defined(__MSDOS__) || defined(__CYGMIN) || defined(RAND) 
#undef random
#define random() rand()
#else
extern long random(void);
#if !defined(RAND_MAX)
#undef randmax
#define randmax (double) LONG_MAX
#endif
#endif

/*H name H*/
/*G; asmname G*/
/*GH 'srandom' HG*/
#if defined(_POSIX_SOURCE) || defined(__sun)
extern void srandom(unsigned s);
#elif defined(__MSDOS__) || defined(__CYGMIN) || defined(RAND) 
extern void srand(unsigned s);
#define srandom(x) srand((unsigned)(x))
#elif defined(mips)
#elif defined(__APPLE__)
#elif defined(__OpenBSD__)
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
{ return (((int) buf[0]) << 7) + (int) buf[1] ;
}


void putbval(Char *buf, int n)
{ /* D
    if debuglevel > 0 then writeln(log,'putbval[',ordp(buf):1,'](',n:1,')'); D */
  buf[0] = (Char)(n>>7); buf[1] = (Char)(n % 128);
}


void deletename(nametype **head)
{ nametype *pn, *r;
  int j, FORLIM;

  while (*head != NULL) {
      pn = *head;
      r = pn;
      while (pn->next_ != NULL) {
	  r = pn;
	  pn = pn->next_;
      }
      r->next_ = NULL;
      if (pn == *head) {
	  *head = NULL;
      }
      if (pn->segmnt != NULL) {
	  if (bval(pn->segmnt) > 1) {
	      j = bval(pn->segmnt);
	      putbval(pn->segmnt, j - 1);
	      if (pn->segmnt == freeseg && pn->seginx + pn->len == freex) {
		  freex = pn->seginx;
		  j = 3;
		  while (freex > j) {
		      if (pn->segmnt[freex - 1] == nlch) {
			  freex--;
		      }
		      else {
			  j = freex;
		      }
		  }
	      }
	      else {
		  FORLIM = pn->seginx + pn->len;
		  for (j = pn->seginx; j < FORLIM; j++) {
		      pn->segmnt[j] = nlch;
		  }
	      }
	  }
	  else if (pn->segmnt == freeseg) {
	      Free(freeseg);
	      freeseg = NULL;
	  }
	  else {
	      Free(pn->segmnt);
	  }
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
{ *specv = ((*specv) >> 3) * 8 + svalue - XLlinetype;
  /* if svalue = XLsolid then
     specv := (specv div 32)*32 + 16 + (specv mod 16) */
}


void resetspec(int *specv, int svalue)
{ *specv = 0;
  setspec(specv, svalue);
}


void setthen(int *specv)
{ *specv = ((*specv) >> 4) * 16 + ((*specv) & 7) + 8;
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
      write(log,' vars=');
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
      if ptype = XBLOCK then primp := nil else primp := son
      end;
   writeln(log); flush(log)
   end;

procedure prvars(np: integer);
var lv: strptr;
   i,x: integer;
begin
   with attstack^[np] do if envblock = nil then
      write(log,'vars=nil: nil envblock')
   else begin
      i := 0; x := HASHLIM+1;
      while i < x do if envblock^.vars[i]<>nil then x := i else i := i+1;
      writeln(log,'vars=');
      if x > HASHLIM then writeln(log,' None set') else
      for i:=0 to HASHLIM do begin write(log,i:1);
         lv := envblock^.vars[i];
         if lv = nil then writeln(log,' nil');
         while lv <> nil do begin
            write(log,' (',ordp(lv):1,',',ordp(lv^.next):1,')=');
            with lv^ do begin
snapname(segmnt,seginx,len);
write(log,'='); wfloat(log,val); writeln(log)
end;
            lv := lv^.next
            end
         end
      end
   end;
D*/

void deletetree(primitive **p)
{ /* var p: primitivep */
  primitive *r;
  int i;
  primitive *With;

  if (*p != NULL) {
      (*p)->parent = NULL;
  }
  while (*p != NULL) {
      while ((*p)->next_ != NULL || (*p)->son != NULL) {
	  r = *p;
	  if ((*p)->next_ != NULL) {
	      *p = (*p)->next_;
	      r->next_ = NULL;
	  }
	  else {
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
      With = *p;
      switch (With->ptype) {

      case XLbox:
      case XLstring:
	Free(*p);
	break;

      case XBLOCK:
	for (i = HASHLIM; i >= 0; i--) {
	    deletename(&With->Upr.Ublock.vars[i]);
	}
	if (With->Upr.Ublock.env != NULL) {
	    Free(With->Upr.Ublock.env);
	}
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
{ double ra;                       /* set arc angles given centre, start, end */

  *strtang = datan(ys - ctr.ypos, xs - ctr.xpos);
  ra = principal(datan(yf - ctr.ypos, xf - ctr.xpos) - *strtang, pi);
  if (ra < 0.0 && *arcang > 0.0) {
      ra += 2.0 * pi;
  }
  else if (ra > 0.0 && *arcang < 0.0) {
      ra -= 2.0 * pi;
  }
  *arcang = ra;
}


void eqop(double *x, int op, double y)
{ switch (op) {

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
    if (y == 0) {
	markerror(852);
    }
    else {
	*x /= y;
    }
    break;

  case XLremeq:
    if (y == 0) {
	markerror(852);
    }
    else {
	*x = (long)floor(*x + 0.5) % (long)floor(y + 0.5);
/* p2c: dpic1.p, line 375:
 * Note: Using % for possibly-negative arguments [317] */
    }
    break;
  }
}


void setstval(int *st, int value)
{ *st = value * 256 + ((*st) & 255);
}


int getstval(int st)
{ return (st >> 8);
}


void setstflag(int *st, int value)
{ switch (value) {

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
{ boolean b;

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
{ int i, j, k;

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
  if (len1 < len2) {
      j = len1;
  }
  else {
      j = len2;
  }
  while (i < j) {
      /*D if debuglevel = 2 then
           write(log,'(',seg1^[inx1+i],seg2^[inx2+i],')'); D*/
      k = seg1[inx1 + i] - seg2[inx2 + i];
      if (k == 0) {
	  i++;
      }
      else {
	  j = i;
      }
  }
  if (k != 0) {
      return k;
  }
  if (len1 > len2) {
      k = seg1[inx1 + i];
  }
  else if (len1 < len2) {
      k = -seg2[inx2 + i];
  }
  /*D if debuglevel = 2 then if (seg1 = nil) or (seg2 = nil) then
     writeln(log,' nil segment(s)')
     else writeln(log,' i=',i:1,' j=',j:1,' eqstring=',k:1); D*/
  return k;
}


int cmpstring(primitive *p1, primitive *p2)
{ if (p1 == NULL || p2 == NULL) {
      return SHORT_MAX;
  }
  else if (p1->textp == NULL) {
      return SHORT_MAX;
  }
  else if (p2->textp == NULL) {
      return (-SHORT_MAX);
  }
  else {
      return (eqstring(p1->textp->segmnt, p1->textp->seginx, p1->textp->len,
		       p2->textp->segmnt, p2->textp->seginx, p2->textp->len));
  }
}


primitive *findplace(primitive *p, Char *chb, chbufinx inx,
			    chbufinx length)
{ primitive *pj;
  nametype *With;

  pj = NULL;
  /*D if debuglevel = 2 then begin
     write(log,'findplace:'); snapname(chb,inx,length); writeln(log)
     end; D*/
  while (p != pj) {
      /*D if debuglevel = 2 then printobject(p); D*/
      if (p->name == NULL) {
	  p = p->next_;
      }
      else {
	  With = p->name;
	  if (eqstring(With->segmnt, With->seginx, With->len, chb, inx,
		       length) == 0) {
	      pj = p;
	  }
	  else {
	      p = p->next_;
	  }
      }
  }
  return p;
}


arg *findmacro(arg *p, Char *chb, chbufinx inx, chbufinx length,
		      arg **last)
{ /*H( p:argp;
                       chb: chbufp; inx,length: chbufinx;
                       var last:argp )H*/
  arg *pj, *With;

  *last = NULL;
  pj = NULL;
  /*D if debuglevel = 2 then begin
     write(log,'findmacro:'); snapname(chb,inx,length); writeln(log)
     end; D*/
  while (p != pj) {
      With = p;
      if (With->argbody == NULL) {
	  *last = p;
	  p = p->nextb;
      }
      else if (eqstring(With->argbody->carray, 1, -With->argbody->attrib, chb,
			inx, length) == 0) {
	  pj = p;
      }
      else {
	  *last = p;
	  p = p->nextb;
      }
  }
  return p;
}


int varhash(Char *chb, chbufinx chbufx, chbufinx length)
{ int idx;

  if (chb == NULL) {
      idx = 0;
      return (idx % (HASHLIM + 1));
  }
  idx = chb[chbufx];
  if (length > 2) {
      idx += chb[chbufx + length - 2];
  }
  return (idx % (HASHLIM + 1));
/* p2c: dpic1.p, line 506:
 * Note: Using % for possibly-negative arguments [317] */
}


nametype *findname(primitive *eb, Char *chb, chbufinx chbufx,
			  chbufinx length, nametype **last, int *k)
{ nametype *leftptr, *rightptr;
  int left, right, midpt, i, idx;
  nametype *With;

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
  if (eb == NULL) {
      leftptr = NULL;
  }
  else {
      leftptr = eb->Upr.Ublock.vars[idx];
      *last = leftptr;
  }
  /* Check the first (highest) name */
  if (leftptr != NULL) {
      *k = eqstring(chb, chbufx, length, leftptr->segmnt, leftptr->seginx,
		    leftptr->len);
      if (*k < 0) {
	  left = 2;
	  leftptr = leftptr->next_;
	  right = eb->Upr.Ublock.nvars[idx] + 1;
      }
      else {
	  rightptr = leftptr;
      }
  }
  while (leftptr != rightptr) {
      midpt = (left + right) >> 1;
      /*D if debuglevel > 0 then
                         write(log,' midpt=',midpt:1); D*/
      *last = leftptr;
      for (i = left + 1; i <= midpt; i++) {
	  *last = (*last)->next_;
      }
      With = *last;
      *k = eqstring(chb, chbufx, length, With->segmnt, With->seginx,
		    With->len);
      /*D if debuglevel > 0 then write(log,' k=',k:1); D*/
      if (*k < 0) {
	  left = midpt + 1;
	  leftptr = (*last)->next_;
	  continue;
      }
      if (*k == 0) {
	  leftptr = *last;
	  rightptr = leftptr;
      }
      else {
	  right = midpt;
	  rightptr = *last;
      }
  }
  if (*k == 0) {
      return leftptr;
  }
  else {
      return NULL;
      /*D; if debuglevel > 0 then writeln(log) D*/
  }
}


void marknotfound(int eno, Char *chb, chbufinx inx, chbufinx len)
{ int i;

  /*D if debuglevel > 0 then begin write(log,'Search failure ',eno:1);
      if chb <> nil then begin write(log,' for "');
         for i := inx to inx+len-1 do write(log,chb^[i]); write(log,'"') end;
      writeln(log) end; D*/
  markerror(eno);
  fprintf(errout, "\nSearch failure");
  if (chb != NULL) {
      fprintf(errout, " for \"");
      for (i = inx; i < inx + len; i++) {
	  putc(chb[i], errout);
      }
      putc('"', errout);
  }
  putc('\n', errout);
}


nametype *glfindname(primitive *eb, Char *chb, chbufinx chbufx,
			    chbufinx length, nametype **last, int *k)
{ nametype *np;
  primitive *pp;

  pp = NULL;
  np = NULL;
  *k = 1;
  while (eb != pp) {
      /* np := findname(eb^.vars,chb,chbufx,length,last); */
      np = findname(eb, chb, chbufx, length, last, k);
      if (np != NULL) {
	  pp = eb;
      }
      else {
	  eb = eb->parent;
      }
  }
  if (eb == NULL) {
      marknotfound(851, chb, chbufx, length);
  }
  return np;
}


void newstr(nametype **sp)
{ nametype *With;

  *sp = Malloc(sizeof(nametype));
  With = *sp;
  With->segmnt = NULL;
  With->seginx = 0;
  With->len = 0;
  With->val = 0.0;
  With->next_ = NULL;
}


/* copy a string into the current freeseg */
void storestring(Char **outbuf, nametype *outstr, Char *srcbuf,
			chbufinx psrc, chbufinx lsrc)
{ int i, j;
  boolean newseg;

  if (*outbuf == NULL || lsrc > CHBUFSIZ - freex + 1) {
      newseg = true;
  }
  else if (bval(*outbuf) >= maxbval) {
      newseg = true;
  }
  else {
      newseg = false;
  }
  if (newseg) {
      *outbuf = Malloc(sizeof(chbufarray));
      putbval(*outbuf, 0);
      freex = 3;
      freeseg = *outbuf;
  }
  for (i = 0; i < lsrc; i++) {
      (*outbuf)[freex + i] = srcbuf[psrc + i];
  }
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
     ' seginx=',outstr^.seginx:0, ' links=', bval(outbuf):0,
     ' freex=',freex:0 );
     snapname(outbuf,outstr^.seginx,outstr^.len);
     write(log,' from:'); snapname(srcbuf,psrc,lsrc);
     writeln( log ) end D*/
}


/* outbuf is always the current freeseg: */
void copystr(Char **outbuf, nametype **sp, nametype *ip)
{ if (ip == NULL) {
      *sp = NULL;
  }
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

  if (sp == NULL || buf == NULL) {
      return;
  }
  if (sp->segmnt == freeseg && sp->seginx + sp->len == freex &&
       freex + ll - 1 <= CHBUFSIZ) {
      /*D if debuglevel > 0 then begin
          write(log,' appending |');
          for i:=0 to ll-1 do write(log,buf^[px+i]); writeln(log,'|') end; D*/
      for (i = 0; i < ll; i++) {                                 /*D k := 1; D*/
	  freeseg[freex + i] = buf[px + i];
      }
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
  for (i = 0; i < FORLIM; i++) {
      tmpseg[i + 3] = sp->segmnt[sp->seginx + i];
  }
  j = bval(sp->segmnt);
  if (j > 1) {
      putbval(sp->segmnt, j - 1);                                /*D k := 2; D*/
      if (sp->segmnt == freeseg && sp->seginx + sp->len == freex) {
	  freex = sp->seginx;
	  j = 3;
	  while (freex > j) {
	      if (sp->segmnt[freex - 1] == nlch) {
		  freex--;
	      }
	      else {
		  j = freex;
	      }
	  }
      }
      else {
	  FORLIM = sp->seginx + sp->len;
	  for (i = sp->seginx; i < FORLIM; i++) {
	      sp->segmnt[i] = nlch;
	  }
      }
  }
  else {
      if (sp->segmnt == freeseg) {                               /*D k := 3; D*/
	  freeseg = NULL;
      }
      Free(sp->segmnt);
  }
  for (i = 0; i < ll; i++) {
      tmpseg[sp->len + i + 3] = buf[px + i];
  }
  freeseg = tmpseg;
  freex = sp->len + ll + 3;
  putbval(freeseg, 1);
  sp->segmnt = freeseg;
  sp->seginx = 3;
  sp->len += ll;
  /*D; if debuglevel > 0 then if sp<>nil then with sp^ do begin
     writeln(log,
      'appendstring to strptr ',ordp(sp):1,': segmnt=',ordp(freeseg):1,
      ' seginx:seginx+len-1=',seginx:1,':',seginx+len-1:1);
     writeln(log,
      ' len=',len:1, ' branch k=',k:1);
     snapname(segmnt,seginx,len); writeln( log )
     end D*/
}


int putstring(int ix, nametype *sp, Char *buf, chbufinx px,
		     chbufinx ll)
{ if (ix <= 0) {
      storestring(&freeseg, sp, buf, px, ll);
  }
  else {
      appendstring(sp, buf, px, ll);
  }
  return (ix + 1);
}


double pheight(primitive *pr)
{ double ph;

  if (pr == NULL) {
      ph = 0.0;
      return ph;
  }
  switch (pr->ptype) {

  case XLbox:
  case XLstring:
    ph = pr->Upr.Ubox.boxheight;
    break;

  case XBLOCK:
    ph = pr->Upr.Ublock.blockheight;
    break;

  case XLellipse:
    ph = pr->Upr.Uellipse.elheight;
    break;

  case XLcircle:
    ph = 2.0 * pr->Upr.Ucircle.radius;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    ph = fabs(pr->Upr.Uline.endpos.ypos - pr->aat.ypos);
    break;

  default:
    ph = 0.0;
    break;
  }
  return ph;
}


double pwidth(primitive *pr)
{ double ph;

  if (pr == NULL) {
      ph = 0.0;
      return ph;
  }
  switch (pr->ptype) {

  case XLbox:
  case XLstring:
    ph = pr->Upr.Ubox.boxwidth;
    break;

  case XBLOCK:
    ph = pr->Upr.Ublock.blockwidth;
    break;

  case XLellipse:
    ph = pr->Upr.Uellipse.elwidth;
    break;

  case XLcircle:
    ph = 2.0 * pr->Upr.Ucircle.radius;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    ph = fabs(pr->Upr.Uline.endpos.xpos - pr->aat.xpos);
    break;

  default:
    ph = 0.0;
    break;
  }
  return ph;
}


void neswrec(primitive *ptm)
{ while (ptm != NULL) {
      nesw(ptm);
      if (ptm->ptype != XBLOCK) {
	  neswrec(ptm->son);
      }
      ptm = ptm->next_;
  }
}


void getnesw(primitive *ptm)
{ initnesw();
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
{ if (pr == NULL) {
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
      pe->ypos = pr->aat.ypos + pr->Upr.Ubox.boxheight * 0.5;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->Upr.Ubox.boxheight * 0.5;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->Upr.Ubox.boxwidth * 0.5;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->Upr.Ubox.boxwidth * 0.5;
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
      pe->ypos = pr->aat.ypos + pr->Upr.Ublock.blockheight * 0.5;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->Upr.Ublock.blockheight * 0.5;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->Upr.Ublock.blockwidth * 0.5;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->Upr.Ublock.blockwidth * 0.5;
      break;
    }
    break;

  case XLcircle:
    switch (pr->direction) {

    case XLup:
      pe->ypos = pr->aat.ypos + pr->Upr.Ucircle.radius;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->Upr.Ucircle.radius;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->Upr.Ucircle.radius;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->Upr.Ucircle.radius;
      break;
    }
    break;

  case XLellipse:
    switch (pr->direction) {

    case XLup:
      pe->ypos = pr->aat.ypos + pr->Upr.Uellipse.elheight * 0.5;
      break;

    case XLdown:
      pe->ypos = pr->aat.ypos - pr->Upr.Uellipse.elheight * 0.5;
      break;

    case XLleft:
      pe->xpos = pr->aat.xpos - pr->Upr.Uellipse.elwidth * 0.5;
      break;

    case XLright:
      pe->xpos = pr->aat.xpos + pr->Upr.Uellipse.elwidth * 0.5;
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
    *pe = pr->Upr.Uline.endpos;
    break;

  case XLabel:
  case XLaTeX:
    /* blank case */
    break;
  }
}


void newprim(primitive **pr, int primtype, primitive *envblk)
{ int i;
  primitive *With;

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
  With = *pr;
  With->name = NULL;
  With->textp = NULL;
  With->outlinep = NULL;
  With->shadedp = NULL;
  With->son = NULL;
  With->next_ = NULL;
  if (envblk == NULL) {
      With->parent = NULL;
      With->aat.xpos = 0.0;
      With->aat.ypos = 0.0;
      With->direction = XLright;
  }
  else {
      if (envblk->son == NULL) {
	  envblk->son = *pr;
      }
      With->parent = envblk;
      With->aat = envblk->Upr.Ublock.here;
      With->direction = envblk->direction;
  }
  With->lparam = mdistmax;
  With->lthick = mdistmax;
  if (primtype == XLstring || primtype == XLspline || primtype == XLarc ||
       primtype == XLarrow || primtype == XLline || primtype == XLellipse ||
       primtype == XLcircle || primtype == XLbox) {
      resetspec(&With->spec, XLsolid);
  }
  else {
      resetspec(&With->spec, XLinvis);
  }
  With->ptype = primtype;
  switch (primtype) {

  case XLbox:
  case XLstring:
    With->Upr.Ubox.boxfill = -1.0;
    With->Upr.Ubox.boxheight = 0.0;
    With->Upr.Ubox.boxwidth = 0.0;
    With->Upr.Ubox.boxradius = 0.0;
    break;

  case XBLOCK:
    With->Upr.Ublock.blockheight = 0.0;
    With->Upr.Ublock.blockwidth = 0.0;
    With->Upr.Ublock.here = With->aat;
    for (i = 0; i <= HASHLIM; i++) {
	With->Upr.Ublock.vars[i] = NULL;
	With->Upr.Ublock.nvars[i] = 0;
    }
    With->Upr.Ublock.env = NULL;
    break;

  case XLcircle:
    With->Upr.Ucircle.cfill = -1.0;
    With->Upr.Ucircle.radius = 0.0;
    break;

  case XLellipse:
    With->Upr.Uellipse.efill = -1.0;
    With->Upr.Uellipse.elheight = 0.0;
    With->Upr.Uellipse.elwidth = 0.0;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLarc:
  case XLspline:
    With->Upr.Uline.endpos.xpos = 0.0;
    With->Upr.Uline.endpos.ypos = 0.0;
    With->Upr.Uline.height = 0.0;
    With->Upr.Uline.width = 0.0;
    With->Upr.Uline.lfill = -1.0;
    With->Upr.Uline.aradius = mdistmax;
    With->Upr.Uline.atype = pahlex(0, XEMPTY);
    break;

  case XLabel:
  case XLaTeX:
    /* blank case */
    break;
  }
}  /* newprim */


void arcenddir(primitive *pr)
{ if (pr->Upr.Uline.endpos.ypos > 0.0) {
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
{ primitive *With;

  while (pr != NULL) {
      With = pr;
      With->aat.xpos += x;
      With->aat.ypos += y;
      if (With->ptype == XLspline || With->ptype == XLmove ||
	   With->ptype == XLarrow || With->ptype == XLline) {
	  With->Upr.Uline.endpos.xpos += x;
	  With->Upr.Uline.endpos.ypos += y;
      }
      if (With->son != NULL) {
	  shift(With->son, x, y);
      }
      pr = With->next_;
  }
}


void scaleobj(primitive *pr, double s)
{ primitive *With;

  while (pr != NULL) {
      With = pr;
      With->aat.xpos *= s;
      With->aat.ypos *= s;
      if (With->ptype == XLbox) {
	  With->Upr.Ubox.boxheight *= s;
	  With->Upr.Ubox.boxwidth *= s;
	  With->Upr.Ubox.boxradius *= s;
      }
      else if (With->ptype == XBLOCK) {
	  With->Upr.Ublock.blockheight *= s;
	  With->Upr.Ublock.blockwidth *= s;
      }
      else if (With->ptype == XLcircle) {
	  With->Upr.Ucircle.radius *= s;
      }
      else if (With->ptype == XLellipse) {
	  With->Upr.Uellipse.elheight *= s;
	  With->Upr.Uellipse.elwidth *= s;
      }
      else if (With->ptype == XLarc) {
	  With->Upr.Uline.aradius *= s;
      }
      else if (With->ptype == XLspline || With->ptype == XLmove ||
	       With->ptype == XLarrow || With->ptype == XLline) {
	  With->Upr.Uline.endpos.xpos *= s;
	  With->Upr.Uline.endpos.ypos *= s;
      }
      if (With->son != NULL) {
	  scaleobj(With->son, s);
      }
      pr = With->next_;
  }
}


/* corner(prim,<corner>,xval,yval);
                           Put the corner coordinates into xval,yval */
void corner(primitive *pr, int lexv, double *x, double *y)
{ primitive *pe;
  boolean sb;

  if (pr == NULL) {
      return;
  }
  /*D if debuglevel>0 then begin write(log,
      'Corner: ptype(',ordp(pr):1,'): ptype='); printptype(ptype);
     write(log,' corner='); printcorner(lexv) end; D*/
  *x = pr->aat.xpos;
  *y = pr->aat.ypos;
  pe = pr;
  if (lexv == XEMPTY &&
      (pr->ptype == XLspline || pr->ptype == XLmove || pr->ptype == XLarrow ||
       pr->ptype == XLline)) {
      while (pe->son != NULL) {
	  pe = pe->son;
      }
      *x = 0.5 * (pr->aat.xpos + pe->Upr.Uline.endpos.xpos);
      *y = 0.5 * (pr->aat.ypos + pe->Upr.Uline.endpos.ypos);
      return;
  }
  if (lexv == XEMPTY && pr->ptype != XLaTeX) {
      return;
  }
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
	  *y = Min(pr->Upr.Ubox.boxradius,
		   Min(fabs(pr->Upr.Ubox.boxheight),
		       fabs(pr->Upr.Ubox.boxwidth)) / 2) * (1 - 1 / sqrt(2.0));
	  *x = pr->Upr.Ubox.boxwidth / 2 - *y;
	  *y = pr->Upr.Ubox.boxheight / 2 - *y;
	  break;

	case XLellipse:
	  *x = pr->Upr.Uellipse.elwidth * (0.5 / sqrt(2.0));
	  *y = pr->Upr.Uellipse.elheight * (0.5 / sqrt(2.0));
	  break;

	case XLarc:
	  *x = pr->Upr.Uline.aradius / sqrt(2.0);
	  *y = *x;
	  break;

	case XLcircle:
	  *x = pr->Upr.Ucircle.radius / sqrt(2.0);
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
    }
    else if (pr->ptype == XLarc) {
	switch (lexv) {

	case XDn:
	  *y = pr->aat.ypos + pr->Upr.Uline.aradius;
	  break;

	case XDs:
	  *y = pr->aat.ypos - pr->Upr.Uline.aradius;
	  break;

	case XDe:
	  *x = pr->aat.xpos + pr->Upr.Uline.aradius;
	  break;

	case XDw:
	  *x = pr->aat.xpos - pr->Upr.Uline.aradius;
	  break;

	case XDc:
	  /* blank case */
	  break;

	case XDstart:
	  *x = pr->aat.xpos + pr->Upr.Uline.aradius * cos(pr->Upr.Uline.endpos.xpos);
	  *y = pr->aat.ypos + pr->Upr.Uline.aradius * sin(pr->Upr.Uline.endpos.xpos);
	  break;

	case XDend:
	  *x = pr->aat.xpos + pr->Upr.Uline.aradius *
		 cos(pr->Upr.Uline.endpos.xpos + pr->Upr.Uline.endpos.ypos);
	  *y = pr->aat.ypos + pr->Upr.Uline.aradius *
		 sin(pr->Upr.Uline.endpos.xpos + pr->Upr.Uline.endpos.ypos);
	  break;
	}
    }
    else {
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
	    while (pe->son != NULL) {
		pe = pe->son;
	    }
	    *x = pe->Upr.Uline.endpos.xpos;
	    *y = pe->Upr.Uline.endpos.ypos;
	}
	else if (lexv == XDc) {
	    while (pe->son != NULL) {
		pe = pe->son;
	    }
	    *x = 0.5 * (*x + pe->Upr.Uline.endpos.xpos);
	    *y = 0.5 * (*y + pe->Upr.Uline.endpos.ypos);
	}
	else {
	    do {
		switch (lexv) {

		case XDn:
		  sb = (pe->Upr.Uline.endpos.ypos > *y);
		  break;

		case XDs:
		  sb = (pe->Upr.Uline.endpos.ypos < *y);
		  break;

		case XDe:
		  sb = (pe->Upr.Uline.endpos.xpos > *x);
		  break;

		case XDw:
		  sb = (pe->Upr.Uline.endpos.xpos < *x);
		  break;

		case XDne:
		  sb = ((pe->Upr.Uline.endpos.ypos > *y &&
			 pe->Upr.Uline.endpos.xpos >= *x) ||
			(pe->Upr.Uline.endpos.ypos >= *y &&
			 pe->Upr.Uline.endpos.xpos > *x));
		  break;

		case XDse:
		  sb = ((pe->Upr.Uline.endpos.ypos < *y &&
			 pe->Upr.Uline.endpos.xpos >= *x) ||
			(pe->Upr.Uline.endpos.ypos <= *y &&
			 pe->Upr.Uline.endpos.xpos > *x));
		  break;

		case XDsw:
		  sb = ((pe->Upr.Uline.endpos.ypos < *y &&
			 pe->Upr.Uline.endpos.xpos <= *x) ||
			(pe->Upr.Uline.endpos.ypos <= *y &&
			 pe->Upr.Uline.endpos.xpos < *x));
		  break;

		case XDnw:
		  sb = ((pe->Upr.Uline.endpos.ypos > *y &&
			 pe->Upr.Uline.endpos.xpos <= *x) ||
			(pe->Upr.Uline.endpos.ypos >= *y &&
			 pe->Upr.Uline.endpos.xpos < *x));
		  break;
		}
		if (sb) {
		    *x = pe->Upr.Uline.endpos.xpos;
		    *y = pe->Upr.Uline.endpos.ypos;
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
{ primitive *prp, *pp;
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
	  if (primp->ptype == objtype) {
	      prp = primp;
	  }
	  primp = primp->next_;
      }
      return prp;
  }
  if (nth < 0) {
      i = 0;
      pp = primp;
      while (pp != NULL) {
	  if (pp->ptype == objtype) {
	      i++;
	  }
	  pp = pp->next_;
      }
      nth += i + 1;
  }
  i = 0;
  /*D if debuglevel = 2 then write(log,' nth=',nth:1); D*/
  if (nth <= 0) {
      return prp;
  }
  while (primp != prp) {
      /*D if debuglevel = 2 then printobject(primp); D*/
      if (primp->ptype == objtype) {
	  i++;
      }
      if (i == nth) {
	  prp = primp;
      }
      else {
	  primp = primp->next_;
	  /*D;if (debuglevel = 2) and (i = nth) then
	     writeln(log,'found[nth=',nth:1,'](',ordp(prp):1,')') D*/
      }
  }
  return prp;
}


void resetenv(int envval, primitive *envbl)
{ environx i, last;

  if (envbl == NULL) {
      return;
  }
  if (envval == 0) {
      envval = XLenvvar + 1;
      last = XLlastenv;
  }
  else {
      last = envval;
  }
  if (envbl->Upr.Ublock.env == NULL) {
      envbl->Upr.Ublock.env = Malloc(sizeof(envarray));
  }
  for (i = envval - 1; i < last; i++) {
      switch (i + 1) {

      case XLarcrad:                          /* scaled environment vars (in) */
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.25;
	break;

      case XLarrowht:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.1;
	break;

      case XLarrowwid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.05;
	break;

      case XLboxht:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.5;
	break;

      case XLboxrad:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.0;
	break;

      case XLboxwid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.75;
	break;

      case XLcirclerad:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.25;
	break;

      case XLdashwid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.05;
	break;

      case XLellipseht:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.5;
	break;

      case XLellipsewid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.75;
	break;

      case XLlineht:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.5;
	break;

      case XLlinewid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.5;
	break;

      case XLmoveht:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.5;
	break;

      case XLmovewid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.5;
	break;

      case XLtextht:
	if (drawmode == PDF || drawmode == SVG) {
	    envbl->Upr.Ublock.env[i - XLenvvar] = DFONT / 72.0;
	}
	else {
	    envbl->Upr.Ublock.env[i - XLenvvar] = 0.0;
	}
	break;

      case XLtextoffset:
	envbl->Upr.Ublock.env[i - XLenvvar] = 2.5 / 72;                      /*.27*/
	break;

      case XLtextwid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.0;
	break;

      /* The following are unscaled */
      case XLarrowhead:
	envbl->Upr.Ublock.env[i - XLenvvar] = 1.0;
	break;

      case XLfillval:
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.5;
	break;

      case XLlinethick:                                             /* points */
	envbl->Upr.Ublock.env[i - XLenvvar] = 0.8;
	break;

      case XLmaxpsht:                                                   /* in */
	envbl->Upr.Ublock.env[i - XLenvvar] = 11.0;
	break;

      case XLmaxpswid:
	envbl->Upr.Ublock.env[i - XLenvvar] = 8.5;
	break;

      case XLscale:
	envbl->Upr.Ublock.env[i - XLenvvar] = 1.0;
	break;
      }
  }
}


void inheritenv(primitive *envbl)
{ environx i;
  primitive *pr;

  pr = findenv(envbl);
  if (pr == NULL) {
      resetenv(0, envbl);
      return;
  }
  envbl->Upr.Ublock.env = Malloc(sizeof(envarray));
  for (i = XLenvvar; i < XLlastenv; i++) {
      envbl->Upr.Ublock.env[i - XLenvvar] = pr->Upr.Ublock.env[i - XLenvvar];
  }
}


void resetscale(double x, int opr, primitive *envbl)
{ double r, s;
  int i;

  resetenv(0, envbl);
  r = envbl->Upr.Ublock.env[XLscale - XLenvvar - 1];
  eqop(&envbl->Upr.Ublock.env[XLscale - XLenvvar - 1], opr, x);
  s = envbl->Upr.Ublock.env[XLscale - XLenvvar - 1];
  if (s == 0.0) {
      envbl->Upr.Ublock.env[XLscale - XLenvvar - 1] = r;
      s = 1.0;
      markerror(870);
  }
  else if (r == 0.0) {
      markerror(852);
  }
  else {
      s /= r;
  }
  for (i = XLenvvar; i < XLlastsc; i++) {
      eqop(&envbl->Upr.Ublock.env[i - XLenvvar], XLmulteq, s);
  }
}


void inittwo(void)
{ /* performed for each input diagram */
  freeinbuf = NULL;
  freeseg = NULL;
  freex = 0;
  lastfillval = mdistmax;
  gslinethick = mdistmax;
  newprim(&envblock, XBLOCK, NULL);
  /*D; if debuglevel > 0 then printobject(envblock) D*/
  resetenv(0, envblock);
}


void deleteinbufs(fbuffer **p)
{ fbuffer *q;

  while (*p != NULL) {
      q = (*p)->nextb;
      Free(*p);
      *p = q;
  }
}


double intpow(double x, int k)
{ /* 0^(-k) does not occur */
  if (k == 0) {
      x = 1.0;                                             /* 0^0 returns 1.0 */
      return x;
  }
  if (x == 0.0 || k == 1) {
      return x;
  }
  if (k < 0) {
      x = intpow(1.0 / x, -k);
      return x;
  }
  if (k == 2) {
      x *= x;                                 /* common enough to test for it */
      return x;
  }
  if (k & 1) {
      x *= intpow(x, k - 1);
  }
  else {
      x = intpow(x, k >> 1);
      x *= x;
  }
  return x;
}


void getscale(double xv, double yv, primitive *lp, double *sfact,
		     double *xsc)
{ /* .PS xv yv
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
	  if (qp->Upr.Ublock.env[XLscale - XLenvvar - 1] > 0.0) {
	      *sfact = qp->Upr.Ublock.env[XLscale - XLenvvar - 1];
	  }
	  /*D if debuglevel > 0 then begin
	    write(log,'getscale: sfact='); wfloat(log,sfact);
	    write(log,' maxpswid='); wfloat(log,qp^.env^[XLmaxpswid]);
	    write(log,' maxpsht='); wfloat(log,qp^.env^[XLmaxpsht]);
	    writeln(log) end; D*/
	  if (east > west && (east - west) / *sfact >
			     qp->Upr.Ublock.env[XLmaxpswid - XLenvvar - 1] &&
	       qp->Upr.Ublock.env[XLmaxpswid - XLenvvar - 1] > 0.0) {
	      erno = 903;
	      gs = (east - west) / qp->Upr.Ublock.env[XLmaxpswid - XLenvvar - 1];
	  }
	  if (north > south &&
	       (north - south) / *sfact >
	       qp->Upr.Ublock.env[XLmaxpsht - XLenvvar - 1] &&
	       qp->Upr.Ublock.env[XLmaxpsht - XLenvvar - 1] > 0.0) {
	      erno = 904;
	      gs = Max(gs,
		  (north - south) / qp->Upr.Ublock.env[XLmaxpsht - XLenvvar - 1]);
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
  if (erno != 0) {
      markerror(erno);
  }
  /*D if debuglevel > 0 then begin write(log,' getscale=');
     wfloat(log,gs*sfact); writeln(log) end; D*/
  *xsc = gs * *sfact;
}


void addelem(primitive *prold, primitive *prnew)
{ primitive *pp, *pq;

  if (prold == NULL || prnew == NULL) {
      return;
  }
  pp = prold;
  while (pp->next_ != NULL) {
      pq = prnew;                                   /* Refuse to make a loop: */
      while (pq->next_ != NULL) {
	  if (pq->next_ == pp) {
	      pq->next_ = NULL;
	  }
	  else {
	      pq = pq->next_;
	  }
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
{ /* Needed because assignment of variant records is unreliable */
  int i;

  if (prin == NULL) {
      return;
  }
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
    (*prout)->Upr.Ubox.boxfill = prin->Upr.Ubox.boxfill;
    (*prout)->Upr.Ubox.boxheight = prin->Upr.Ubox.boxheight;
    (*prout)->Upr.Ubox.boxwidth = prin->Upr.Ubox.boxwidth;
    (*prout)->Upr.Ubox.boxradius = prin->Upr.Ubox.boxradius;
    break;

  case XBLOCK:
    (*prout)->Upr.Ublock.blockheight = prin->Upr.Ublock.blockheight;
    (*prout)->Upr.Ublock.blockwidth = prin->Upr.Ublock.blockwidth;
    (*prout)->Upr.Ublock.here = prin->Upr.Ublock.here;
    for (i = 0; i <= HASHLIM; i++) {
	(*prout)->Upr.Ublock.vars[i] = prin->Upr.Ublock.vars[i];
	(*prout)->Upr.Ublock.nvars[i] = prin->Upr.Ublock.nvars[i];
    }
    if (prin->Upr.Ublock.env != NULL) {
	(*prout)->Upr.Ublock.env = Malloc(sizeof(envarray));
	for (i = XLenvvar; i < XLlastenv; i++) {
	    (*prout)->Upr.Ublock.env[i - XLenvvar] = prin->Upr.Ublock.env[i - XLenvvar];
	}
    }
    break;

  case XLcircle:
    (*prout)->Upr.Ucircle.cfill = prin->Upr.Ucircle.cfill;
    (*prout)->Upr.Ucircle.radius = prin->Upr.Ucircle.radius;
    break;

  case XLellipse:
    (*prout)->Upr.Uellipse.efill = prin->Upr.Uellipse.efill;
    (*prout)->Upr.Uellipse.elheight = prin->Upr.Uellipse.elheight;
    (*prout)->Upr.Uellipse.elwidth = prin->Upr.Uellipse.elwidth;
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLarc:
  case XLspline:
    (*prout)->Upr.Uline.endpos = prin->Upr.Uline.endpos;
    (*prout)->Upr.Uline.height = prin->Upr.Uline.height;
    (*prout)->Upr.Uline.width = prin->Upr.Uline.width;
    (*prout)->Upr.Uline.lfill = prin->Upr.Uline.lfill;
    (*prout)->Upr.Uline.aradius = prin->Upr.Uline.aradius;
    (*prout)->Upr.Uline.atype = prin->Upr.Uline.atype;
    break;

  case XLabel:
  case XLaTeX:
    /* blank case */
    break;
  }
}


void deletestringbox(primitive **pr)
{ primitive *prx;

  if (*pr != NULL) {
      if ((*pr)->parent != NULL) {
	  if ((*pr)->parent->son != NULL) {
	      if ((*pr)->parent->son == *pr) {
		  (*pr)->parent->son = NULL;
	      }
	      else {  /* is the following ever reached? */
		  prx = (*pr)->parent->son;
		  while (prx->next_ != NULL && prx->next_ != *pr) {
		      prx = prx->next_;
		  }
		  prx->next_ = NULL;
	      }
	  }
      }
  }
  deletetree(pr);
}


/* addsuffix(chbuf,chbufx,length,attstack^[newp+1].xval); */
void addsuffix(Char *buf, chbufinx *inx, int *len, double suff)
{                                                               /*DGH ordp HGD*/
  int i, j, FORLIM;

  /*D if debuglevel <> 0 then begin writeln(log,
    'addsuffix(buf:chbufp; var inx:chbufinx; var len:integer; suff:real)');
     write(log,D*/
  /*DBUMX ord XMUBD*/
  /*D(buf):1,
         ' inx=',inx:1,' len=',len:1,' suff=');
        wfloat(log,suff); write(log,' chbufi=',chbufi:1);
        snapname(buf,inx,len); writeln(log) end; D*/

  if (chbufi + *len - 1 > CHBUFSIZ) {
      fatal(4);
  }
  if (*inx + *len != chbufi) {
      FORLIM = *len;
      for (i = 0; i < FORLIM; i++) {
	  buf[chbufi + i] = buf[*inx + i];
      }
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
  if (*inx + *len - 1 > CHBUFSIZ) {
      fatal(4);
  }
  buf[*inx + *len - 1] = ']';
  j = *len - 2;
  i = (long)floor(suff + 0.5);
  if (i < 0) {
      i = -i;
  }
  do {
      buf[*inx + j] = i % 10 + '0';
/* p2c: dpic1.p, line 1333:
 * Note: Using % for possibly-negative arguments [317] */
      j--;
      i /= 10;
  } while (i != 0);
  if ((long)floor(suff + 0.5) < 0) {
      buf[*inx + j] = '-';
      j--;
  }
  buf[*inx + j] = '[';
  chbufi = *inx + *len;                                                 /* ?? */
  /*D ; if debuglevel <> 0 then begin
     snapname(buf,inx,len); writeln(log) end D*/
}  /* addsuffix */


void appendthen(primitive **pr)
{ primitive *prp;

  copyprim(*pr, &prp);
  prp->parent = *pr;
  prp->next_ = NULL;
  prp->name = NULL;
  prp->textp = NULL;
  prp->shadedp = NULL;
  if (prp->outlinep != NULL) {  /* We have to duplicate the stored string */
      copystr(&freeseg, &prp->outlinep, (*pr)->outlinep);
  }
  pexit(*pr, &prp->aat);
  (*pr)->son = prp;
  setthen(&prp->spec);
  *pr = prp;
}


void lineardir(primitive *pr, double dy, double dx, int *state)
{ if (!(teststflag(*state, XLto) | teststflag(*state, XLdirecton))) {
      pr->Upr.Uline.endpos = pr->aat;
  }
  switch (pr->direction) {

  case XLup:
    pr->Upr.Uline.endpos.ypos += dy;
    break;

  case XLdown:
    pr->Upr.Uline.endpos.ypos -= dy;
    break;

  case XLleft:
    pr->Upr.Uline.endpos.xpos -= dx;
    break;

  case XLright:
    pr->Upr.Uline.endpos.xpos += dx;
    break;
  }
  setstflag(state, XLdirecton);
}


boolean hasoutline(int lx, boolean warn)
{ boolean hs;

  hs = (lx == XLspline || lx == XLarrow || lx == XLline || lx == XLarc ||
	lx == XLellipse || lx == XLcircle || lx == XLbox);
  if (drawmode == SVG) {
      hs = (hs || lx == XLstring);
  }
  if (!hs && warn) {
      markerror(858);
  }
  return hs;
}


boolean hasshade(int lx, boolean warn)
{ boolean hs;

  if (lx == XLellipse || lx == XLcircle || lx == XLbox) {
      hs = true;
  }
  else if (drawmode == xfig || drawmode == tTeX || drawmode == TeX ||
	   drawmode == Pict2e) {
      hs = false;
  }
  else {
      hs = (lx == XLspline || lx == XLarrow || lx == XLline || lx == XLarc);
  }
  if (drawmode == SVG) {
      hs = (hs || lx == XLstring);
  }
  if (!hs && warn) {
      markerror(858);
  }
  return hs;
}


void makevar(Char *s, int ln, int nameval)
{ nametype *vn, *lastvar, *namptr;
  int j, k;
  primitive *With;

  for (j = 0; j < ln; j++) {
      chbuf[chbufi + j] = s[j];
  }
  vn = findname(envblock, chbuf, chbufi, ln, &lastvar, &k);
  newstr(&vn);
  j = varhash(chbuf, chbufi, ln);
  storestring(&freeseg, vn, chbuf, chbufi, ln);
  With = envblock;
  /*D if debuglevel > 1 then begin
      writeln(log,'makevar: envblock=',ordp(envblock):1,' eqstr val=',k:1);
      write(log,' lastvar=',ordp(lastvar):1);
      if lastvar<>nil then with lastvar^ do snapname(segmnt,seginx,len);
      writeln(log)
      end; D*/
  /*D if debuglevel > 1 then writeln(log,' vn=',ordp(vn):1); D*/
  if (lastvar == NULL) {
      With->Upr.Ublock.vars[j] = vn;
  }
  else if (k < 0) {
      if (With->Upr.Ublock.vars[j]->next_ == NULL) {
	  With->Upr.Ublock.vars[j]->next_ = vn;
      }
      else {
	  vn->next_ = lastvar->next_;
	  lastvar->next_ = vn;
      }
  }
  else if (lastvar == With->Upr.Ublock.vars[j]) {
      vn->next_ = With->Upr.Ublock.vars[j];
      With->Upr.Ublock.vars[j] = vn;
  }
  else {
      namptr = With->Upr.Ublock.vars[j];
      while (namptr->next_ != lastvar) {
	  namptr = namptr->next_;
      }
      namptr->next_ = vn;
      vn->next_ = lastvar;
  }
  With->Upr.Ublock.nvars[j]++;
  vn->val = nameval;
}


void makeenvvars(int i)
{ int j;

  switch (drawmode) {

  case TeX:
    j = 1;
    break;

  case tTeX:
    j = 2;
    break;

  case Pict2e:
    j = 3;
    break;

  case PSTricks:
    j = 4;
    break;

  case PDF:
    j = 5;
    break;

  case PGF:
    j = 6;
    break;

  case MFpic:
    j = 7;
    break;

  case PS:
    j = 8;
    break;

  case PSfrag:
    j = 9;
    break;

  case MPost:
    j = 10;
    break;

  case SVG:
    j = 11;
    break;

  case xfig:
    j = 12;
    break;

  default:
    j = 0;
    break;
  }
  makevar("dpicopt", 7, j);
  if (safemode) {
      j = 1;
  }
  else {
      j = 0;
  }
  makevar("optsafe", 7, j);
  makevar("optTeX", 6, 1);
  makevar("opttTeX", 7, 2);
  makevar("optPict2e", 9, 3);
  makevar("optPSTricks", 11, 4);
  makevar("optPDF", 6, 5);
  makevar("optPGF", 6, 6);
  makevar("optMFpic", 8, 7);
  makevar("optPS", 5, 8);
  makevar("optPSfrag", 9, 9);
  makevar("optMpost", 8, 10);
  makevar("optSVG", 6, 11);
  makevar("optxfig", 7, 12);
}


/* This is the syntactic action routine. */
void produce(stackinx newp, int p)
{ /* newp: stackinx; p: integer */
  Char lastc;
  nametype *lastvar, *namptr;
  fbuffer *lastm;
  arg *macp, *lastp;
  primitive *primp, *prp, *eb;
  int i, j, k, kk, lj, ll, nexprs;
  double r, s, t, x1, y1, dx, dy, ts;
  boolean bswitch;
  attribute *With, *With1;
  primitive *With2;
  attribute *With3;
  nametype *With4;
  int FORLIM;
  fbuffer *With5;

  /* D if (debuglevel > 0) then begin
        writeln( 'Production(newp=',newp:1,
                     ',lexval=',attstack^[newp].lexval:1,
                     ',prod=',p:1,')' )
        end; D */
  /*D if (debuglevel > 0) then begin
        writeln(log);
        writeln(log, 'Production(newp=',newp:1,
                     ',lexval=',attstack^[newp].lexval:1,
                     ',prod=',p:1,')' );
        flush(log)
        end; D*/
  /* D writeln('Produce Step 1, debuglevel=',debuglevel);
       writeln(log,'Produce Step 1'); D */

  With = &attstack[newp];

  /*D with attstack^[newp] do if (debuglevel = 2) and (
     ((p >= block1) and (p <= block3)) or
     ((p > object1) and (p <= object26))
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
    if (envblock != NULL) {
	envblock->direction = XLright;
    }
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

    if (envblock != NULL) {
	envblock->direction = XLright;
    }
    break;

  /* picture = start NL elementlist "<END>" */
  /* | start NL elementlist NL "<END>" */
  case picture1:
  case picture2:
    if (With->prim != NULL) {
	attstack[newp + 2].prim = With->prim;
    }
    With1 = &attstack[newp + 2];
    getnesw(With1->prim);
    /*D if debuglevel > 0 then begin
      writeln(log,' Global dimensions:'); write(log,'(n,s)(e,w)=');
      wpair(log,north,south); wpair(log,east,west);
      writeln(log,' envblock<>nil:',envblock<>nil); flush(log) end; D*/
    if (envblock != NULL) {
	With2 = envblock;
	With2->aat.xpos = (east + west) * 0.5;
	With2->aat.ypos = (north + south) * 0.5;
	With2->Upr.Ublock.blockheight = north - south;
	With2->Upr.Ublock.blockwidth = east - west;
    }
    if (drawmode == xfig) {
	shift(envblock, -west, -south);
	north -= south;
	east -= west;
	west = 0.0;
	south = 0.0;
    }
    else if ((drawmode == PS || drawmode == PDF || drawmode == SVG) &&
	     envblock != NULL) {
	if (envblock->Upr.Ublock.env != NULL) {
	    r = envblock->Upr.Ublock.env[XLlinethick - XLenvvar - 1] / 2 / 72 *
		envblock->Upr.Ublock.env[XLscale - XLenvvar - 1];
	    /*D if debuglevel > 0 then begin
	      write(log,' west='); wfloat(log,west);
	      write(log,' south='); wfloat(log,south);
	      write(log,' r='); wfloat(log,r);
	      write(log,' shift=('); wfloat(log,-west+r);
	      write(log,','); wfloat(log,-south+r);
	      writeln(log,')'); flush(log) end; D*/
	    shift(envblock, 2 * r - west, 2 * r - south);
	    north += 3 * r - south;
	    east += 3 * r - west;
	    west = r;
	    south = r;
	}
    }
    xfheight = north;
    With3 = &attstack[newp];
    getscale(With3->xval, With3->yval, envblock, &scale, &fsc);
    /*D if debuglevel > 0 then begin flush(log);
      writeln(log,'After shift:'); printobject( envblock );
      printobject( envblock^.son ); writeln(log);
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
  case start1:
    /*D if debuglevel > 0 then begin writeln(log,'.PS'); flush(log); end; D*/
    makeenvvars(newp);
    break;

  case start2:
    With->xval = attstack[newp + 1].xval;
    break;

  case start3:
    With->xval = attstack[newp + 1].xval;
    With->yval = attstack[newp + 2].xval;
    break;

  /* elementlist = "<EMPTY>" */
  case elementlist1:
    With->state = 0;
    break;

  /* | element */
  case elementlist2:
    With->state = 0;
    if (With->prim != NULL && With->lexval != XLcontinue) {
	i = newp - 1;
	j = 1;
	/*D if debuglevel>0 then writeln(log,
	   ' Elementlist2: Searching for last drawn element'); D*/
	while (i > j) {
	    if ((attstack[i].lexval == XLendfor ||
		 attstack[i].lexval == XFOR ||
		 attstack[i].lexval == XLBRACE ||
		 attstack[i].lexval == XSEMICOLON ||
		 attstack[i].lexval == XNL) && attstack[i].prim == NULL)
	    {                                               /*repeat ,Xrepeat */
		i--;
	    }
	    else {
		j = i;
	    }
	}
	/*D if debuglevel > 0 then begin
	   if i = 1 then write(log,' Not found') else write(log,' Found:');
	   writeln(log,' newp=',newp:1,
	      ', attstack^[',i:1,'].lexval=',attstack^[i].lexval:1);
	   if attstack^[i].prim<>nil then begin
	      writeln(log,'attstack^[',i:1,'].prim=');
	      snaptype(log,attstack^[i].prim^.ptype)
	      end
	   end; D*/
	if (attstack[i].prim != NULL) {
	    addelem(attstack[i].prim, With->prim);
	    /*D if debuglevel > 0 then printobject(attstack^[i].prim); D*/
	    With->state = i - newp;
	}
	else if (attstack[i].lexval == XLBRACKET) {
	    attstack[i + 1].prim = With->prim;
	    /*D if debuglevel > 0 then printobject(attstack^[i+1].prim); D*/
	    With->state = i - newp + 1;
	}
	else {
	    attstack[i].prim = With->prim;
	    /*D if debuglevel > 0 then printobject(attstack^[i].prim); D*/
	    With->state = i - newp;
	}
    }
    break;

  /* | elementlist NL element */
  case elementlist3:
    if (attstack[newp + 2].prim != NULL) {
	if (attstack[newp + 2].lexval != XLcontinue) {
	    if (With->prim == NULL) {
		attstack[newp] = attstack[newp + 2];
		doprod(elementlist2);
	    }
	    else {
		addelem(attstack[newp + With->state].prim,
			attstack[newp + 2].prim);
	    }
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
    With->xval *= attstack[newp + 2].xval;
    break;

  case term3:
    if (attstack[newp + 2].xval == 0.0) {
	markerror(852);
	With->xval = 0.0;
    }
    else {
	With->xval /= attstack[newp + 2].xval;
    }
    break;

  case term4:
    if (attstack[newp + 2].xval == 0.0) {
	markerror(852);
	With->xval = 0.0;
    }
    else {
	With->xval = (long)floor(With->xval + 0.5) %
		     (long)floor(attstack[newp + 2].xval + 0.5);
/* p2c: dpic1.p, line 1663:
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
    if (With->prim != NULL) {
	prp = With->prim;
	if (With->prim->ptype == XLspline || With->prim->ptype == XLmove ||
	     With->prim->ptype == XLarrow || With->prim->ptype == XLline) {
	    if (With->startchop != 0.0) {
		With2 = With->prim;
		dx = With2->Upr.Uline.endpos.xpos - With2->aat.xpos;
		dy = With2->Upr.Uline.endpos.ypos - With2->aat.ypos;
		s = linlen(dx, dy);
		/*D if debuglevel = 2 then begin
		   write(log,' element1 startchop='); wfloat(log,startchop);
		   write(log,' (dx,dy)='); wpair(log,dx,dy);
		   write(log,' s='); wfloat(log,s); writeln(log) end; D*/
		if (s != 0.0) {
		    t = With->startchop / s;
		    With2->aat.xpos += t * dx;
		    With2->aat.ypos += t * dy;
		}
	    }
	    while (prp->son != NULL) {
		prp = prp->son;
	    }
	    if (With->endchop != 0.0) {
		dx = prp->Upr.Uline.endpos.xpos - prp->aat.xpos;
		dy = prp->Upr.Uline.endpos.ypos - prp->aat.ypos;
		s = linlen(dx, dy);
		/*D if debuglevel = 2 then begin
		   write(log,' element1 endchop='); wfloat(log,endchop);
		   write(log,' (dx,dy)='); wpair(log,dx,dy);
		   write(log,' s='); wfloat(log,s); writeln(log) end; D*/
		if (s != 0.0) {
		    t = With->endchop / s;
		    prp->Upr.Uline.endpos.xpos -= t * dx;
		    prp->Upr.Uline.endpos.ypos -= t * dy;
		}
	    }
	}
	/*D ; if debuglevel > 0 then begin
	   write(log,' element1 Here=');
	   with envblock^ do wpair(log,here.xpos,here.ypos); writeln(log);
	   printobject(prim)
	   end D*/
	pexit(prp, &envblock->Upr.Ublock.here);
    }
    break;

  /* | "<Label>" suffix ":" position */
  case element2:
    if (attstack[newp + 1].lexval != XEMPTY) {
	addsuffix(chbuf, &With->chbufx, &With->length,
		  attstack[newp + 1].xval);
    }
    prp = findplace(envblock->son, chbuf, With->chbufx, With->length);
    if (prp == NULL) {
	newprim(&With->prim, XLabel, envblock);
	newstr(&With->prim->name);
	storestring(&freeseg, With->prim->name, chbuf, With->chbufx,
		    With->length);
	prp = With->prim;
    }
    else if (prp->ptype != XLabel) {
	newprim(&With->prim, XLabel, envblock);
	With->prim->name = prp->name;
	prp->name = NULL;
	prp = With->prim;
    }
    prp->aat.xpos = attstack[newp + 3].xval;
    prp->aat.ypos = attstack[newp + 3].yval;
    /*D; if (debuglevel > 0) and (prim<>nil) then printobject(prim)D*/
    break;

  /* | assignlist */
  case element3:
    With->lexval = XLBRACE;
    break;

  /* | "<directon>" */
  case element4:
    envblock->direction = With->lexval;
    With->lexval = XLBRACE;
    break;

  /* | "<LaTeX>" */
  case element5:
    newprim(&With->prim, XLaTeX, envblock);
    newstr(&With->prim->textp);
    storestring(&freeseg, With->prim->textp, chbuf, With->chbufx,
		With->length);
    break;

  /* | command */
  case element6:
    With->lexval = XLBRACE;
    break;

  /* | lbrace "lbrace" elementlist optnl "rbrace" */
  case element7:
    envblock->Upr.Ublock.here.xpos = With->xval;
    envblock->Upr.Ublock.here.ypos = With->yval;
    if (With->state == XLright || With->state == XLleft ||
	 With->state == XLdown || With->state == XLup) {
	envblock->direction = With->state;
    }
    if (attstack[newp + 2].state == 0) {
	attstack[newp] = attstack[newp + 2];
    }
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
	newprim(&With->prim, XLaTeX, envblock);
	With->prim->textp = attstack[newp + 1].prim->textp;
	attstack[newp + 1].prim->textp = NULL;
	if (envblock->son == attstack[newp + 1].prim) {
	    envblock->son = With->prim;
	}
	deletestringbox(&attstack[newp + 1].prim);
    }
    break;

  /* | "exec" stringexpr */
  case element12:
    if (attstack[newp + 1].prim != NULL) {
	if (attstack[newp + 1].prim->textp != NULL) {
	    if (attstack[newp + 1].prim->textp->segmnt != NULL) {
		With4 = attstack[newp + 1].prim->textp;
		newbuf(&lastm);
		lastm->carray[1] = nlch;
		lastm->savedlen = 1;
		copyleft(lastm, &inbuf);
		FORLIM = With4->len;
		for (i = 1; i <= FORLIM; i++) {
		    lastm->carray[i] = With4->segmnt[With4->seginx + i - 1];
		}
		lastm->savedlen = With4->len;
		copyleft(lastm, &inbuf);
		lastm->carray[1] = nlch;
		lastm->savedlen = 1;
		copyleft(lastm, &inbuf);
		deletestringbox(&attstack[newp + 1].prim);              /*D,2D*/
		disposebufs(&lastm);
	    }
	}
    }
    With->lexval = XLBRACE;
    break;
    /* | loop "}" */
    /* | break */
    /*repeat element13,element14: ; */

  /* lbrace = "<EMPTY>" */
  case lbrace1:
    With->xval = envblock->Upr.Ublock.here.xpos;
    With->yval = envblock->Upr.Ublock.here.ypos;
    With->state = envblock->direction;
    break;

  /* namedobj = object */
  case namedobj1:
    if (With->prim != NULL) {  /* then, arc, deferred shift */
	prp = With->prim;
	while (isthen(With->prim)) {
	    With->prim = With->prim->parent;
	}
	if (prp != With->prim) {
	    With->prim->name = prp->name;
	    prp->name = NULL;
	}
	if (With->prim->ptype == XLarc) {
	    arcenddir(prp);
	}
	if (teststflag(With->state, XLat)) {
	    With2 = With->prim;
	    i = getstval(With->state);
	    if (i == XLfloat) {  /* pair at */
		getnesw(With->prim);
		dx = west + With->startchop;
		dy = south + With->endchop;
	    }
	    else if (With->internal != NULL) {
		corner(With->internal, i, &dx, &dy);
	    }
	    else {
		corner(With->prim, i, &dx, &dy);
	    }
	    With->internal = NULL;
	    if (With2->ptype != XLarc) {
		shift(With->prim, With->xval - dx, With->yval - dy);
	    }
	    else {
		x1 = With2->aat.xpos +
		     With2->Upr.Uline.aradius * cos(With2->Upr.Uline.endpos.xpos);
		    /* from */
		y1 = With2->aat.ypos +
		     With2->Upr.Uline.aradius * sin(With2->Upr.Uline.endpos.xpos);
		if (teststflag(With->state, XLto))
		{                                 /* to X from Here|Y implied */
		    if (i != XEMPTY && i != XDc) {
			markerror(858);
		    }
		    r = With2->aat.xpos + With2->Upr.Uline.aradius * cos(
			    With2->Upr.Uline.endpos.xpos + With2->Upr.Uline.endpos.ypos);
			/* to */
		    s = With2->aat.ypos + With2->Upr.Uline.aradius * sin(
			    With2->Upr.Uline.endpos.xpos + With2->Upr.Uline.endpos.ypos);
		    With2->aat.xpos = With->xval;
		    With2->aat.ypos = With->yval;
		    With2->Upr.Uline.aradius = linlen(r - With2->aat.xpos,
						    s - With2->aat.ypos);
		    setangles(&With2->Upr.Uline.endpos.xpos,
			      &With2->Upr.Uline.endpos.ypos, With2->aat, x1, y1,
			      r, s);
		}
		else if (teststflag(With->state, XLfrom)) {
		    if (i != XEMPTY && i != XDc) {
			markerror(858);
		    }
		    With2->aat.xpos = With->xval;
		    With2->aat.ypos = With->yval;
		    t = datan(y1 - With2->aat.ypos, x1 - With2->aat.xpos);
		    r = With2->aat.xpos + With2->Upr.Uline.aradius *
					  cos(t + With2->Upr.Uline.endpos.ypos);
		    s = With2->aat.ypos + With2->Upr.Uline.aradius *
					  sin(t + With2->Upr.Uline.endpos.ypos);
		    With2->Upr.Uline.aradius = linlen(x1 - With2->aat.xpos,
						    y1 - With2->aat.ypos);
		    setangles(&With2->Upr.Uline.endpos.xpos,
			      &With2->Upr.Uline.endpos.ypos, With2->aat, x1, y1,
			      r, s);
		}
		else {
		    shift(With->prim, With->xval - dx, With->yval - dy);
		}
	    }
	}
    }
    break;

  /* | "<Label>" suffix ":" object */
  case namedobj2:
    if (attstack[newp + 3].prim != NULL) {
	if (attstack[newp + 1].lexval != XEMPTY) {
	    addsuffix(chbuf, &With->chbufx, &With->length,
		      attstack[newp + 1].xval);
	}
	primp = findplace(envblock->son, chbuf, With->chbufx, With->length);
	if (primp != NULL) {
	    attstack[newp + 3].prim->name = primp->name;
	    primp->name = NULL;
	}
	else {
	    With2 = attstack[newp + 3].prim;
	    /*D if debuglevel = 2 then
	       writeln(log,'Label not found, creating it'); D*/
	    newstr(&With2->name);
	    storestring(&freeseg, With2->name, chbuf, With->chbufx,
			With->length);
	}
	attstack[newp] = attstack[newp + 3];
	doprod(namedobj1);
    }
    break;

  /* suffix = "<EMPTY>" */
  /* | "[" expression "]" */
  case suffix1:
    With->lexval = XEMPTY;
    break;

  case suffix2:
    With->xval = attstack[newp + 1].xval;
    break;

  /* position = pair */
  case position1:
    break;

  /* | expression "between" position "and" position */
  /* | expression "<" position "," position "<compare>" shift */
  case position2:
  case position4:
    r = With->xval;
    With->xval = attstack[newp + 2].xval +
		 r * (attstack[newp + 4].xval - attstack[newp + 2].xval);
    With->yval = attstack[newp + 2].yval +
		 r * (attstack[newp + 4].yval - attstack[newp + 2].yval);
    if (p == position4) {
	if (attstack[newp + 5].lexval != XGT) {
	    markerror(869);
	}
	else if (attstack[newp + 6].lexval != XEMPTY) {
	    With->xval += attstack[newp + 6].xval;
	    With->yval += attstack[newp + 6].yval;
	}
    }
    break;

  /* | expression "of" "the" "way" "between" position "and" position*/
  case position3:
    r = With->xval;
    With->xval = attstack[newp + 5].xval +
		 r * (attstack[newp + 7].xval - attstack[newp + 5].xval);
    With->yval = attstack[newp + 5].yval +
		 r * (attstack[newp + 7].yval - attstack[newp + 5].yval);
    break;

  /* assignlist = assignment */
  /* | assignlist "," assignment */
  case assignlist1:
    break;

  case assignlist2:
    With->xval = attstack[newp + 2].xval;
    break;

  /* command = "print" expression redirect */
  /* | "print" position redirect */
  /* | "print" stringexpr redirect */
  case command1:
  case command2:
  case command3:
    With1 = &attstack[newp + 1];
    if (attstack[newp + 2].lexval == XEMPTY) {
	switch (p) {

	case command1:
	  wfloat(&errout, With1->xval);
	  break;

	case command2:
	  wpair(&errout, With1->xval, With1->yval);
	  break;

	case command3:
	  if (With1->prim != NULL) {
	      wstring(&errout, With1->prim->textp);
	  }
	  break;
	}
	putc('\n', errout);
	consoleflush();
    }
    else if (attstack[newp + 2].state == 0) {
#ifndef SAFE_MODE
	switch (p) {

	case command1:
	  wfloat(&redirect, With1->xval);
	  break;

	case command2:
	  wpair(&redirect, With1->xval, With1->yval);
	  break;

	case command3:
	  if (With1->prim != NULL) {
	      wstring(&redirect, With1->prim->textp);
	  }
	  break;
	}
	putc('\n', redirect);
	if (redirect != NULL) {
	    fclose(redirect);
	}
	redirect = NULL;
#endif
    }
    if (p == command3) {
	deletestringbox(&With1->prim);
    }
    break;

  /* | "reset" */
  case command4:
    resetenv(0, envblock);
    break;

  /* | "reset" resetlist */
  case command5:
    break;

  /* | systemcmd */
  case command6:
    break;

  /* | "copy" stringexpr */
  case command7:
    With1 = &attstack[newp + 1];
    if (With1->prim != NULL) {
#ifdef SAFE_MODE
	markerror(906);
#else
	pointinput(With1->prim->textp);
#endif
	deletestringbox(&With1->prim);
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
    macp = findmacro(macros, chbuf, With->chbufx, With->length, &lastp);
    if (macp != NULL) {
	if (lastp != NULL) {
	    lastp->nextb = macp->nextb;
	}
	if (macp == macros) {                                           /*D,5D*/
	    macros = macp->nextb;
	}
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
    if (With->xval == 0.0) {
	attstack[newp + 1].lexval = XLBRACE;
	With->lexval = XLBRACE;
    }
    else {
	currprod = elsehead1;
	skiptobrace();
    }
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
	With2 = With->prim;
	prp = attstack[newp + 2].prim;
	With2->Upr.Ubox.boxwidth += prp->Upr.Ubox.boxwidth;
	With2->Upr.Ubox.boxheight = Max(With2->Upr.Ubox.boxheight,
				       prp->Upr.Ubox.boxheight);
	if (prp->textp != NULL) {
	    if (With2->textp == NULL) {
		With2->textp = prp->textp;
		prp->textp = NULL;
	    }
	    else if (With2->textp->segmnt == prp->textp->segmnt &&
		     With2->textp->seginx + With2->textp->len ==
		     prp->textp->seginx) {
		/*D if debuglevel > 0 then writeln(log,' stringexpr2 branch 1,',
		   ' seginx,length=',textp^.seginx:1,',',textp^.len:1,
		   ' seginx,length=',prp^.textp^.seginx:1,',',prp^.textp^.len:1); D*/
		With2->textp->len += prp->textp->len;
		putbval(With2->textp->segmnt, bval(With2->textp->segmnt) - 1);
		prp->textp->segmnt = NULL;
	    }
	    else {
		appendstring(With2->textp, prp->textp->segmnt,
			     prp->textp->seginx, prp->textp->len);
	    }
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
    newprim(&With->prim, XLstring, envblock);
    eb = findenv(envblock);
    With2 = With->prim;
    With2->Upr.Ubox.boxheight = eb->Upr.Ublock.env[XLtextht - XLenvvar - 1];
    With2->Upr.Ubox.boxwidth = eb->Upr.Ublock.env[XLtextwid - XLenvvar - 1];
    if (drawmode == xfig && With2->Upr.Ubox.boxwidth == 0.0) {
	/* To keep xfig from crashing, assume text height is 0.1
	   and a character is 0.1*0.75 wide */
	eb = findenv(envblock);
	if (With2->Upr.Ubox.boxheight == 0.0) {
	    With2->Upr.Ubox.boxheight = 0.1 * eb->Upr.Ublock.env[XLscale - XLenvvar - 1];
	}
	With2->Upr.Ubox.boxwidth = With2->Upr.Ubox.boxheight * With->length * 0.75;
    }
    if (drawmode == PDF && With2->Upr.Ubox.boxwidth == 0.0) {
	With2->Upr.Ubox.boxwidth = With2->Upr.Ubox.boxheight * With->length * 0.6;
    }
    With2->Upr.Ubox.boxradius = 0.0;
    newstr(&With2->textp);
    storestring(&freeseg, With2->textp, chbuf, With->chbufx, With->length);
    break;

  /* | sprintf ")" */
  case string2:
    break;

  /* assignment = "<name>" suffix "=" expression */
  /* | "<name>" suffix "=" assignment */
  case assignment1:
  case assignment2:
    if (attstack[newp + 1].lexval != XEMPTY) {
	addsuffix(chbuf, &With->chbufx, &With->length,
		  attstack[newp + 1].xval);
    }
    With->varname = findname(envblock, chbuf, With->chbufx, With->length,
			     &lastvar, &k);
    if (With->varname == NULL && attstack[newp + 2].lexval != XEQ) {
	With->varname = glfindname(envblock->parent, chbuf, With->chbufx,
				   With->length, &namptr, &kk);
    }
    if (With->varname == NULL) {
	newstr(&With->varname);
	j = varhash(chbuf, With->chbufx, With->length);
	storestring(&freeseg, With->varname, chbuf, With->chbufx,
		    With->length);
	With2 = envblock;
	/*D if debuglevel > 1 then begin
	    writeln(log,'assignment1: envblock=',ordp(envblock):1,
	            ' eqstr val=',k:1);
	    write(log,' lastvar=',ordp(lastvar):1);
	            if lastvar<>nil then with lastvar^ do
	              snapname(segmnt,seginx,len); writeln(log) end; D*/
	/*D if debuglevel > 1 then begin writeln(log,
	             ' varname=',ordp(varname):1); prvars(newp) end; D*/
	if (lastvar == NULL) {
	    With2->Upr.Ublock.vars[j] = With->varname;
	}
	else if (k < 0) {
	    if (With2->Upr.Ublock.vars[j]->next_ == NULL) {
		With2->Upr.Ublock.vars[j]->next_ = With->varname;
	    }
	    else {
		With->varname->next_ = lastvar->next_;
		lastvar->next_ = With->varname;
	    }
	}
	else if (lastvar == With2->Upr.Ublock.vars[j]) {
	    With->varname->next_ = With2->Upr.Ublock.vars[j];
	    With2->Upr.Ublock.vars[j] = With->varname;
	}
	else {
	    namptr = With2->Upr.Ublock.vars[j];
	    /* while (namptr^.next<>nil) and (namptr^.next<>lastvar) do */
	    while (namptr->next_ != lastvar) {
		namptr = namptr->next_;
	    }
	    namptr->next_ = With->varname;
	    With->varname->next_ = lastvar;
	}
	With2->Upr.Ublock.nvars[j]++;
	With->varname->val = 0.0;
    }
    if (attstack[newp + 2].lexval == XEQ) {
	With->varname->val = attstack[newp + 3].xval;
    }
    else {
	eqop(&With->varname->val, attstack[newp + 2].lexval,
	     attstack[newp + 3].xval);
    }
    With->xval = With->varname->val;
    break;

  /* | "<envvar>" "=" expression */
  /* | "<envvar>" "=" assignment */
  case assignment3:
  case assignment4:
    if (envblock != NULL) {
	if (With->lexval == XLscale) {
	    resetscale(attstack[newp + 2].xval, attstack[newp + 1].lexval,
		       envblock);
	}
	else if (With->lexval == XLarrowhead && drawmode == TeX &&
		 attstack[newp + 2].xval == 0.0) {
	    markerror(858);
	}
	else {
	    if (envblock->Upr.Ublock.env == NULL) {
		inheritenv(envblock);
	    }
	    eqop(&envblock->Upr.Ublock.env[With->lexval - XLenvvar - 1],
		 attstack[newp + 1].lexval, attstack[newp + 2].xval);
	}
	With->xval = envblock->Upr.Ublock.env[With->lexval - XLenvvar - 1];
	/*D if debuglevel > 0 then begin
	   write(log,
	   ' Assignment3or4 envblock[',ordp(envblock),']: lexval=',lexval:1,
	   ' value='); wfloat(log,envblock^.env^[lexval]); writeln(log) end; D*/
	With->startchop = With->lexval;
	if (With->lexval == XLdashwid || With->lexval == XLlinethick) {
	    newprim(&With->prim, XLaTeX, envblock);
	    if (With->lexval == XLlinethick) {
		With->prim->lthick = With->xval;
	    }
	    else {
		With->prim->lparam = With->xval;
	    }
	}
	else {
	    With->lexval = XLBRACE;
	}
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
    With->xval = attstack[newp + 1].xval;
    break;

  case expression3:
    With->xval = -attstack[newp + 1].xval;
    break;

  case expression4:
    With->xval += attstack[newp + 2].xval;
    break;

  case expression5:
    With->xval -= attstack[newp + 2].xval;
    break;

  /* ifhead = setlogic "if" logexpr "then" */
  case ifhead1:
    inlogic = false;
    With->xval = attstack[newp + 2].xval;
    /*D if debuglevel > 0 then begin
        if xval <> 0.0 then writeln(log,' true') else writeln(log,' false')
        end; D*/
    if (With->xval != 0.0) {
	for (i = 1; i <= 3; i++) {
	    attstack[newp + i].lexval = XLBRACE;
	}
    }
    else {
	currprod = ifhead1;
	skiptobrace();
    }
    With->lexval = XLBRACE;
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
    if (With->xval != 0.0 || attstack[newp + 2].xval != 0.0) {
	With->xval = 1.0;
    }
    else {
	With->xval = 0.0;
    }
    break;

  /* forhead = "for" assignlist "to" expression do */
  case forhead1:
    With->xval = attstack[newp + 1].xval;                    /* initial value */
    With->yval = attstack[newp + 4].xval;                        /* increment */
    With->endchop = attstack[newp + 3].xval;                   /* final value */
    With->length = 0;
    if (With->xval == With->endchop) {
	With->length = -1;
	With->yval = 1.0;
    }
    else if (attstack[newp + 4].lexval == XSTAR) {
	t = 0.0;
	if (With->xval == 0.0) {
	    With->length = 860;
	}
	else {
	    t = With->endchop * With->xval;
	}
	if (t < 0.0) {                                     /* or (yval < 0.0) */
	    With->length = 862;
	}
	else if (t == 0.0 && fabs(With->yval * With->xval) != 0.0) {
	    With->length = 860;
	}
	else if (With->yval == 0.0 && t != 0.0) {
	    With->length = 860;
	}
	else if (fabs(With->yval) == 1 &&
		 fabs(With->xval) != fabs(With->endchop)) {
	    With->length = 860;
	}
	else {
	    With->length = 1;
	}
    }
    else if (With->yval == 0.0) {
	With->length = 860;
    }
    else if ((With->endchop - With->xval) * With->yval > 0) {
	With->length = -1;
    }
    if (With->length > 800) {
	markerror(With->length);
	With->length = 0;
    }
    /* store loop variable */
    With->startchop = attstack[newp + 1].startchop;
    if (With->startchop == 0.0) {
	With->varname = attstack[newp + 1].varname;
	With->varname->val = With->xval;
    }
    else if ((long)floor(With->startchop + 0.5) != XLscale) {
	if (envblock->Upr.Ublock.env == NULL) {
	    inheritenv(envblock);
	}
	envblock->Upr.Ublock.env[(int)((long)floor(With->startchop + 0.5)) -
			     XLenvvar - 1] = With->xval;
    }
    else {
	resetscale(With->xval, XEQ, envblock);
    }

    currprod = forhead1;
    if (With->length == 0) {
	skiptobrace();
    }
    else {
	for (i = 0; i <= 4; i++) {
	    attstack[newp + i].lexval = XLBRACE;
	}
	readfor(NULL, newp, &inbuf);
    }
    break;

  /* forincr = "<EMPTY>" */
  case forincr1:
    With->lexval = XLBRACE;
    With1 = &attstack[newp - 1];
    if (With1->varname != NULL) {
	With1->xval = With1->varname->val;
    }
    else {
	With1->xval =
	  envblock->Upr.Ublock.env[(int)((long)floor(With1->startchop + 0.5)) -
			       XLenvvar - 1];
    }
    bswitch = false;
    if (With1->length < 0) {
	if (With1->yval == 0) {
	    bswitch = true;
	}
	else {
	    With1->xval += With1->yval;
	    if (With1->yval > 0 && With1->xval > With1->endchop ||
		 With1->yval < 0 && With1->xval < With1->endchop) {
		bswitch = true;
	    }
	}
    }
    else if (With1->xval == 0) {
	bswitch = true;
    }
    else {
	With1->xval *= With1->yval;
	if ((fabs(With1->yval) >= 1.0 &&
	     fabs(With1->xval) > fabs(With1->endchop)) ||
	     (fabs(With1->yval) < 1.0 &&
	      fabs(With1->xval) < fabs(With1->endchop))) {
	    bswitch = true;
	}
    }
    if (With1->varname != NULL) {
	With1->varname->val = With1->xval;
    }
    else if ((long)floor(With1->startchop + 0.5) != XLscale) {
	envblock->Upr.Ublock.env[(int)((long)floor(With1->startchop + 0.5)) -
			     XLenvvar - 1] = With1->xval;
    }
    else {
	resetscale(With1->xval, XEQ, envblock);
    }
    /*D if debuglevel > 0 then begin write(log,'forincr1: xval=');
        wfloat(log,xval); writeln(log,' bswitch=',bswitch) end; D*/
    if (bswitch) {
	while (inbuf->attrib < 0) {
	    lastm = inbuf;
	    inbuf = inbuf->higherb;                                     /*D,4D*/
	    /*D; if debuglevel > 0 then wrbuf(inbuf,3,0) D*/
	    disposebufs(&lastm);
	}
	lastm = inbuf;
	inbuf = inbuf->higherb;                                         /*D,4D*/
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
    With->xval = 1.0;
    break;

  case do2:
    With->xval = attstack[newp + 1].xval;
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
    With->lexval = attstack[newp + 1].lexval;
    break;

  /* redirect = "<EMPTY>" */
  case redirect1:
    With->lexval = XEMPTY;
    break;

  /* | "<compare>" stringexpr */
  case redirect2:
    With1 = &attstack[newp + 1];
    attstack[newp].state = 1;
    bswitch = false;
    if (With1->prim != NULL) {
	if (With1->prim->textp == NULL) {
	    markerror(861);
	}
	else if (p == redirect2 && attstack[newp].lexval != XGT) {
	    markerror(869);
	}
	else if (safemode) {
	    markerror(906);
	}
	else {
	    bswitch = true;
	}
    }
#ifdef SAFE_MODE
    if (bswitch) {
	markerror(906);
    }
#else
    if (bswitch) {
	pointoutput(true, With1->prim->textp, &attstack[newp].state);
    }
#endif
    deletestringbox(&With1->prim);
    break;

  /* | "<compare>" "<compare>" stringexpr */
  case redirect3:
    With1 = &attstack[newp + 2];
    attstack[newp].state = 1;
    bswitch = false;
    if (With1->prim != NULL) {
	if (With1->prim->textp == NULL) {
	    markerror(861);
	}
	else if (attstack[newp].lexval != XGT ||
		 attstack[newp + 1].lexval != XGT) {
	    markerror(869);
	}
	else if (safemode) {
	    markerror(906);
	}
	else {
	    bswitch = true;
	}
    }
#ifdef SAFE_MODE
    if (bswitch) {
	markerror(906);
    }
#else
    if (bswitch) {
	pointoutput(false, With1->prim->textp, &attstack[newp].state);
    }
#endif
    deletestringbox(&With1->prim);
    break;

  /* resetlist = "<envvar>" */
  /* | resetlist "," "<envvar>" */
  case resetlist1:
    resetenv(With->lexval, envblock);
    break;

  case resetlist2:
    resetenv(attstack[newp + 2].lexval, envblock);
    break;

  /* systemcmd = "sh" stringexpr */
  case systemcmd1:
    With1 = &attstack[newp + 1];
    if (With1->prim != NULL) {
	if (With1->prim->textp != NULL) {
	    With4 = With1->prim->textp;
	    if (With4->segmnt != NULL) {
		if (With4->seginx + With4->len >= CHBUFSIZ) {
		    markerror(866);
		}
		else {
#ifdef SAFE_MODE
		    markerror(906);
#else
		    if (safemode) {
			markerror(906);
		    }
		    else {
			With4->segmnt[With4->seginx + With4->len] = '\0';
			attstack[newp].xval = system(&With4->segmnt[With4->seginx]);
		    }
#endif
		}
	    }
	}
	deletestringbox(&With1->prim);
    }
    break;

  /* defhead = "define" "<name>" */
  /* | "define" "<Label>" */
  case defhead1:
  case defhead2:
    attstack[newp] = attstack[newp + 1];
    macp = findmacro(macros, chbuf, With->chbufx, With->length, &lastp);
    if (macp == NULL) {                                                 /*D,6D*/
	newarg(&macp);
	if (lastp == NULL) {
	    macros = macp;
	}
	else {
	    lastp->nextb = macp;
	}
    }
    disposebufs(&macp->argbody);
    newbuf(&macp->argbody);                            /* copy the macro name */
    With5 = macp->argbody;
    FORLIM = With->length;
    for (i = 1; i <= FORLIM; i++) {
	With5->carray[i] = chbuf[With->chbufx + i - 1];
    }
    With5->savedlen = With->length;
    With5->readx = With->length + 1;
    currprod = p;
    readfor(macp->argbody, -With->length, &macp->argbody);
	/* append the body */
    lastm = macp->argbody;
    while (lastm->nextb != NULL) {
	lastm = lastm->nextb;
    }
    lastm->carray[lastm->savedlen] = etxch;
    /*D; if debuglevel > 1 then begin writeln(log);
       if prod=defhead1 then write(log,'defhead1') else write(log,'defhead2');
       lastm := macp^.argbody; while lastm<> nil do begin
          wrbuf(lastm,3,0); lastm := lastm^.nextb end
       end D*/
    break;

  /* sprintf = "sprintf" "(" stringexpr */
  /* | "sprintf" "(" stringexpr "," exprlist */
  case sprintf1:
  case sprintf2:
    newprim(&With->prim, XLstring, envblock);
    With2 = With->prim;
    eb = findenv(envblock);
    /*D if debuglevel <= 0 then begin end
        else if eb=nil then writeln(log,' ! sprintf2: eb=nil')
        else if eb^.env=nil then writeln(log,' ! sprintf2: env=nil'); D*/
    With2->Upr.Ubox.boxheight = eb->Upr.Ublock.env[XLtextht - XLenvvar - 1];
    With2->Upr.Ubox.boxwidth = eb->Upr.Ublock.env[XLtextwid - XLenvvar - 1];
    With2->Upr.Ubox.boxradius = 0.0;
    newstr(&With2->textp);
    if (p == sprintf1) {
	nexprs = 0;                             /* no of expression arguments */
    }
    else {
	nexprs = attstack[newp + 4].state;
    }
    if (tmpbuf == NULL) {
	tmpbuf = Malloc(sizeof(chbufarray));
    }
    i = 0;                                                /* expression count */
    j = 0;                                        /* end of current substring */
    lj = j;                                     /* start of current substring */
    kk = 0;                                                /* substring count */
    if (attstack[newp + 2].prim != NULL) {
	if (attstack[newp + 2].prim->textp != NULL) {
	    With4 = attstack[newp + 2].prim->textp;
	    while (j < With4->len) {                            /* end; below */
		if (With4->segmnt[With4->seginx + j] != '%') {
		    j++;
		    if (j == With4->len) {
			kk = putstring(kk, attstack[newp].prim->textp,
				       With4->segmnt, With4->seginx + lj,
				       j - lj);
		    }
		    continue;
		}
		if (With4->segmnt[With4->seginx + j + 1] == '%')
		{  /* %% prints % */
		    kk = putstring(kk, attstack[newp].prim->textp,
				   With4->segmnt, With4->seginx + lj,
				   j - lj + 1);
		    j += 2;
		    lj = j;
		    continue;
		}
		if (i >= nexprs) {  /* not enough exprs */
		    markerror(864);
		    j = With4->len;
		    continue;
		}
		if (j > lj) {
		    kk = putstring(kk, attstack[newp].prim->textp,
				   With4->segmnt, With4->seginx + lj, j - lj);
		    lj = j;
		}
		k = With4->len;
		j++;
		if (With4->segmnt[With4->seginx + j] == '-') {
		    j++;
		}
#ifdef NO_SNPRINTF
		attstack[newp + 1].state = j;
#endif
		while (j < k) {
		    if (With4->segmnt[With4->seginx + j] == 'g' ||
			 With4->segmnt[With4->seginx + j] == 'f' ||
			 With4->segmnt[With4->seginx + j] == 'e') {
			k = j;
		    }
		    else if (With4->segmnt[With4->seginx + j] == '.' ||
			     isdigit(With4->segmnt[With4->seginx + j])) {
			j++;
		    }
		    else {
			j = k;
		    }
		}

		if (k == With4->len) {
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
		    if (With4->segmnt[With4->seginx + k] == '.') {
			ll = k;
		    }
		    else {
			s = 10.0 * s + With4->segmnt[With4->seginx + k] - '0';
			k++;
		    }
		}
		r = 0.0;
		for (ll = k + 1; ll < j; ll++) {
		    r = 10.0 * r + With4->segmnt[With4->seginx + ll] - '0';
		}
		ll = 1;
		if (s >= CHBUFSIZ || r >= CHBUFSIZ) {
		    markerror(865);
		    j = With4->len;
		}
		else {
		    lastc = With4->segmnt[With4->seginx + j];
		    With4->segmnt[With4->seginx + j] = '\0';
		    sprintf(&tmpbuf[1], &With4->segmnt[With4->seginx + lj],
			     attstack[newp + i * 2 + 4].xval);
		    With4->segmnt[With4->seginx + j] = lastc;
		    k = CHBUFSIZ;
		    while (ll < k) {
			if (tmpbuf[ll] == '\0') {
			    k = ll;
			}
			else {
			    ll++;
			}
		    }
		    ll--;
		}
#else
		lastc = With4->segmnt[With4->seginx + j];
		With4->segmnt[With4->seginx + j] = '\0';
		k = snprintf(&tmpbuf[1], CHBUFSIZ,
			     &With4->segmnt[With4->seginx + lj],
			     attstack[newp + i * 2 + 4].xval);
		With4->segmnt[With4->seginx + j] = lastc;
		if (k > CHBUFSIZ - 2) {
		    markerror(865);
		    ll = CHBUFSIZ - 2;
		}
		else if (k > 0) {
		    ll = k;
		}
		else {
		    ll = 0;
		}
#endif
		/*D if debuglevel > 0 then begin
		   write(log,' tmpbuf(1:',ll:1,')=');
		   for lj := 1 to ll do write(log,tmpbuf^[lj]);
		   writeln(log); flush(log) end; D*/
		/* Copy tmpbuf to the string */
		if (ll > 0 && ll <= CHBUFSIZ - 2) {
		    kk = putstring(kk, attstack[newp].prim->textp, tmpbuf, 1,
				   ll);
		}
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
    if (nexprs > i) {                                       /* Too many exprs */
	markerror(864);
    }
    if (drawmode == xfig) {
	With2 = With->prim;
	if (With2->Upr.Ubox.boxwidth == 0.0) {
	    if (With2->Upr.Ubox.boxheight == 0.0) {
		With2->Upr.Ubox.boxheight =
		  0.1 * eb->Upr.Ublock.env[XLscale - XLenvvar - 1];
	    }
	    if (With2->textp != NULL) {
		With2->Upr.Ubox.boxwidth = With2->Upr.Ubox.boxheight *
					  With2->textp->len * 0.75;
	    }
	}
	else if (drawmode == PDF && With2->textp != NULL) {
	    With2->Upr.Ubox.boxwidth = With2->Upr.Ubox.boxheight *
				      With2->textp->len * 0.6;
	}
    }
    if (envblock->son == attstack[newp + 2].prim) {
	envblock->son = With->prim;
    }
    deletestringbox(&attstack[newp + 2].prim);
    break;

  /* exprlist = expression */
  case exprlist1:
    With->state = 1;
    break;

  /* | expression "," exprlist */
  case exprlist2:
    With->state = attstack[newp + 2].state + 1;
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
    if (With->prim != NULL) {
	With2 = With->prim;
	switch (With2->ptype) {

	case XLbox:
	case XBLOCK:
	  if (With2->ptype == XBLOCK) {
	      r = 0.5 * (attstack[newp + 2].xval - With2->Upr.Ublock.blockheight);
	      With2->Upr.Ublock.blockheight = attstack[newp + 2].xval;
	  }
	  else {
	      r = 0.5 * (attstack[newp + 2].xval - With2->Upr.Ubox.boxheight);
	      With2->Upr.Ubox.boxheight = attstack[newp + 2].xval;
	  }
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLleft:
	      case XLright:
		/* blank case */
		break;

	      case XLup:
		shift(With->prim, 0.0, r);
		break;

	      case XLdown:
		shift(With->prim, 0.0, -r);
		break;
	      }
	  }
	  break;

	case XLstring:
	  With2->Upr.Ubox.boxheight = attstack[newp + 2].xval;
	  if (drawmode == PDF && With2->textp != NULL) {
	      With2->Upr.Ubox.boxwidth = With2->Upr.Ubox.boxheight *
					With2->textp->len * 0.6;
	  }
	  break;

	case XLcircle:
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLleft:
	      case XLright:
		/* blank case */
		break;

	      case XLup:
		With2->aat.ypos +=
		  0.5 * attstack[newp + 2].xval - With2->Upr.Ucircle.radius;
		break;

	      case XLdown:
		With2->aat.ypos +=
		  With2->Upr.Ucircle.radius - 0.5 * attstack[newp + 2].xval;
		break;
	      }
	  }
	  With2->Upr.Ucircle.radius = attstack[newp + 2].xval * 0.5;
	  break;

	case XLellipse:
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLleft:
	      case XLright:
		/* blank case */
		break;

	      case XLup:
		With2->aat.ypos +=
		  0.5 * (attstack[newp + 2].xval - With2->Upr.Uellipse.elheight);
		break;

	      case XLdown:
		With2->aat.ypos +=
		  0.5 * (With2->Upr.Uellipse.elheight - attstack[newp + 2].xval);
		break;
	      }
	  }
	  With2->Upr.Uellipse.elheight = attstack[newp + 2].xval;
	  break;

	case XLline:
	case XLarrow:
	case XLmove:
	case XLarc:
	case XLspline:
	  prp = With->prim;
	  while (prp != NULL) {
	      prp->Upr.Uline.height = attstack[newp + 2].xval;
	      if (isthen(prp)) {
		  prp = prp->parent;
	      }
	      else {
		  prp = NULL;
	      }
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
    if (With->prim != NULL) {
	With2 = With->prim;
	switch (With2->ptype) {

	case XLbox:
	case XBLOCK:
	  if (With2->ptype == XBLOCK) {
	      r = 0.5 * (attstack[newp + 2].xval - With2->Upr.Ublock.blockwidth);
	      With2->Upr.Ublock.blockwidth = attstack[newp + 2].xval;
	  }
	  else {
	      r = 0.5 * (attstack[newp + 2].xval - With2->Upr.Ubox.boxwidth);
	      With2->Upr.Ubox.boxwidth = attstack[newp + 2].xval;
	  }
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLup:
	      case XLdown:
		/* blank case */
		break;

	      case XLright:
		shift(With->prim, r, 0.0);
		break;

	      case XLleft:
		shift(With->prim, -r, 0.0);
		break;
	      }
	  }
	  break;

	case XLstring:
	  With2->Upr.Ubox.boxwidth = attstack[newp + 2].xval;
	  break;

	case XLcircle:
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLup:
	      case XLdown:
		/* blank case */
		break;

	      case XLright:
		With2->aat.xpos +=
		  0.5 * attstack[newp + 2].xval - With2->Upr.Ucircle.radius;
		break;

	      case XLleft:
		With2->aat.xpos +=
		  With2->Upr.Ucircle.radius - 0.5 * attstack[newp + 2].xval;
		break;
	      }
	  }
	  With2->Upr.Ucircle.radius = attstack[newp + 2].xval * 0.5;
	  break;

	case XLellipse:
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLup:
	      case XLdown:
		/* blank case */
		break;

	      case XLright:
		With2->aat.xpos +=
		  0.5 * (attstack[newp + 2].xval - With2->Upr.Uellipse.elwidth);
		break;

	      case XLleft:
		With2->aat.xpos +=
		  0.5 * (With2->Upr.Uellipse.elwidth - attstack[newp + 2].xval);
		break;
	      }
	  }
	  With2->Upr.Uellipse.elwidth = attstack[newp + 2].xval;
	  break;

	case XLline:
	case XLarrow:
	case XLmove:
	case XLarc:
	case XLspline:
	  prp = With->prim;
	  while (prp != NULL) {
	      prp->Upr.Uline.width = attstack[newp + 2].xval;
	      if (isthen(prp)) {
		  prp = prp->parent;
	      }
	      else {
		  prp = NULL;
	      }
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
    if (With->prim != NULL) {
	With2 = With->prim;
	switch (With2->ptype) {

	case XLbox:
	  With2->Upr.Ubox.boxradius = attstack[newp + 2].xval;
	  break;

	case XLarc:
	  t = attstack[newp + 2].xval;
	  if (With2->direction == 0) {
	      attstack[newp + 2].xval = With2->aat.xpos +
		  With2->Upr.Uline.aradius * cos(
		    With2->Upr.Uline.endpos.xpos + With2->Upr.Uline.endpos.ypos);
	      attstack[newp + 2].yval = With2->aat.ypos +
		  With2->Upr.Uline.aradius * sin(
		    With2->Upr.Uline.endpos.xpos + With2->Upr.Uline.endpos.ypos);
	      /*D if debuglevel = 2 then begin write(log, 'to ');
	        with attstack^[newp+2] do wpair(log,xval,yval) end; D*/
	      doprod(object20);
	  }
	  r = cos(With2->Upr.Uline.endpos.xpos);
	  s = sin(With2->Upr.Uline.endpos.xpos);
	  With2->aat.xpos += With2->Upr.Uline.aradius * r;
	  With2->aat.ypos += With2->Upr.Uline.aradius * s;
	  With2->Upr.Uline.aradius = t;
	  With2->aat.xpos -= With2->Upr.Uline.aradius * r;
	  With2->aat.ypos -= With2->Upr.Uline.aradius * s;
	  setstflag(&With->state, XLradius);
	  break;

	case XLcircle:
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLleft:
		With2->aat.xpos += With2->Upr.Ucircle.radius - attstack[newp + 2].xval;
		break;

	      case XLright:
		With2->aat.xpos += attstack[newp + 2].xval - With2->Upr.Ucircle.radius;
		break;

	      case XLup:
		With2->aat.ypos += attstack[newp + 2].xval - With2->Upr.Ucircle.radius;
		break;

	      case XLdown:
		With2->aat.ypos += With2->Upr.Ucircle.radius - attstack[newp + 2].xval;
		break;
	      }
	  }
	  With2->Upr.Ucircle.radius = attstack[newp + 2].xval;
	  setstflag(&With->state, XLradius);
	  break;

	default:
	  markerror(858);
	  break;
	}
    }
    break;

  /* | object "diameter" expression */
  case object5:
    if (With->prim != NULL) {
	With2 = With->prim;
	if (With2->ptype == XLcircle) {
	    r = 0.5 * attstack[newp + 2].xval;
	    if (!teststflag(With->state, XLat)) {
		switch (With2->direction) {

		case XLleft:
		  With2->aat.xpos += With2->Upr.Ucircle.radius - r;
		  break;

		case XLright:
		  With2->aat.xpos += r - With2->Upr.Ucircle.radius;
		  break;

		case XLup:
		  With2->aat.ypos += r - With2->Upr.Ucircle.radius;
		  break;

		case XLdown:
		  With2->aat.ypos += With2->Upr.Ucircle.radius - r;
		  break;
		}
	    }
	    With2->Upr.Ucircle.radius = r;
	}
	else {
	    markerror(858);
	}
    }
    break;

  /* | object "thickness" expression */
  case object6:
    if (With->prim != NULL) {
	With2 = With->prim;
	if (attstack[newp + 2].xval < 0.0) {
	    eb = findenv(envblock);
	    With2->lthick = eb->Upr.Ublock.env[XLlinethick - XLenvvar - 1];
	}
	else {
	    With2->lthick = attstack[newp + 2].xval;
	}
    }
    break;

  /* | object "scaled" expression */
  case object7:
    if (With->prim != NULL && attstack[newp + 2].lexval != XEMPTY) {
	With2 = With->prim;
	r = attstack[newp + 2].xval - 1;
	corner(With->prim, XDc, &x1, &y1);
	switch (With2->ptype) {

	case XLbox:
	case XBLOCK:
	case XLstring:
	  if (teststflag(With->state, XLat)) {
	      dx = 0.0;
	      dy = 0.0;
	  }
	  else if (With2->ptype == XBLOCK) {
	      dx = With2->Upr.Ublock.blockwidth * r / 2;
	      dy = With2->Upr.Ublock.blockheight * r / 2;
	  }
	  else {
	      dx = With2->Upr.Ubox.boxwidth * r / 2;
	      dy = With2->Upr.Ubox.boxheight * r / 2;
	  }
	  scaleobj(With->prim, attstack[newp + 2].xval);
	  switch (With2->direction) {

	  case XLright:
	    shift(With->prim, x1 - With2->aat.xpos + dx, y1 - With2->aat.ypos);
	    break;

	  case XLleft:
	    shift(With->prim, x1 - With2->aat.xpos - dx, y1 - With2->aat.ypos);
	    break;

	  case XLup:
	    shift(With->prim, x1 - With2->aat.xpos, y1 - With2->aat.ypos + dy);
	    break;

	  case XLdown:
	    shift(With->prim, x1 - With2->aat.xpos, y1 - With2->aat.ypos - dy);
	    break;
	  }
	  break;

	case XLcircle:
	  With2->Upr.Ucircle.radius = attstack[newp + 2].xval * With2->Upr.Ucircle.radius;
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLup:
		With2->aat.xpos = x1;
		With2->aat.ypos = y1 + With2->Upr.Ucircle.radius;
		break;

	      case XLdown:
		With2->aat.xpos = x1;
		With2->aat.ypos = y1 - With2->Upr.Ucircle.radius;
		break;

	      case XLright:
		With2->aat.xpos = x1 + With2->Upr.Ucircle.radius;
		With2->aat.ypos = y1;
		break;

	      case XLleft:
		With2->aat.xpos = x1 - With2->Upr.Ucircle.radius;
		With2->aat.ypos = y1;
		break;
	      }
	  }
	  break;

	case XLellipse:
	  With2->Upr.Uellipse.elwidth *= attstack[newp + 2].xval;
	  With2->Upr.Uellipse.elheight *= attstack[newp + 2].xval;
	  if (!teststflag(With->state, XLat)) {
	      switch (With2->direction) {

	      case XLup:
		With2->aat.xpos = x1;
		With2->aat.ypos = y1 + With2->Upr.Uellipse.elheight / 2;
		break;

	      case XLdown:
		With2->aat.xpos = x1;
		With2->aat.ypos = y1 - With2->Upr.Uellipse.elheight / 2;
		break;

	      case XLright:
		With2->aat.xpos = x1 + With2->Upr.Uellipse.elwidth / 2;
		With2->aat.ypos = y1;
		break;

	      case XLleft:
		With2->aat.xpos = x1 - With2->Upr.Uellipse.elwidth / 2;
		With2->aat.ypos = y1;
		break;
	      }
	  }
	  break;

	case XLline:
	case XLarrow:
	case XLmove:
	case XLarc:
	case XLspline:
	  scaleobj(With->prim, attstack[newp + 2].xval);
	  corner(With->prim, XDc, &r, &s);
	  shift(With->prim, x1 - r, y1 - s);
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
    if (With->prim != NULL) {
	With2 = With->prim;
	if (With2->ptype == XLspline || With2->ptype == XLarrow ||
	     With2->ptype == XLmove || With2->ptype == XLline ||
	     With2->ptype == XLarc) {
	    i = attstack[newp + 1].lexval;
	    envblock->direction = i;
	    eb = findenv(envblock);
	    switch (With2->ptype) {

	    case XLarc:
	      r = cos(With2->Upr.Uline.endpos.xpos);
	      s = sin(With2->Upr.Uline.endpos.xpos);
	      With2->aat.xpos += With2->Upr.Uline.aradius * r;
	      With2->aat.ypos += With2->Upr.Uline.aradius * s;
	      if (With2->direction == XLup && i == XLleft ||
		   With2->direction == XLdown && i == XLright ||
		   With2->direction == XLright && i == XLup ||
		   With2->direction == XLleft && i == XLdown) {
		  With2->Upr.Uline.endpos.ypos = pi * 0.5;
	      }
	      else if (With2->direction == XLup && i == XLright ||
		       With2->direction == XLdown && i == XLleft ||
		       With2->direction == XLright && i == XLdown ||
		       With2->direction == XLleft && i == XLup) {
		  With2->Upr.Uline.endpos.ypos = -pi * 0.5;
	      }
	      if (attstack[newp + 2].lexval != XEMPTY) {
		  With2->Upr.Uline.aradius = attstack[newp + 2].lexval;
	      }
	      With2->aat.xpos -= With2->Upr.Uline.aradius * r;
	      With2->aat.ypos -= With2->Upr.Uline.aradius * s;
	      With2->direction = i;
	      break;

	    case XLline:
	    case XLmove:
	    case XLarrow:
	    case XLspline:
	      With2->direction = i;
	      if (attstack[newp + 2].lexval != XEMPTY) {
		  r = attstack[newp + 2].xval;
		  s = r;
	      }
	      else {
		  switch (With2->ptype) {

		  case XLline:
		  case XLarrow:
		  case XLspline:
		    r = eb->Upr.Ublock.env[XLlineht - XLenvvar - 1];
		    s = eb->Upr.Ublock.env[XLlinewid - XLenvvar - 1];
		    break;

		  case XLmove:
		    r = eb->Upr.Ublock.env[XLmoveht - XLenvvar - 1];
		    s = eb->Upr.Ublock.env[XLmovewid - XLenvvar - 1];
		    break;
		  }
	      }
	      lineardir(With->prim, r, s, &With->state);
	      break;
	    }
	}
	else {
	    markerror(858);
	}
    }
    break;

  /* | object "<linetype>" optexp */
  case object9:
    if (With->prim != NULL) {
	setspec(&With->prim->spec, attstack[newp + 1].lexval);
	if (attstack[newp + 2].lexval != XEMPTY) {
	    if ((attstack[newp + 1].lexval == XLsolid ||
		 attstack[newp + 1].lexval == XLinvis) &&
		(With->prim->ptype == XLmove ||
		 With->prim->ptype == XLspline ||
		 With->prim->ptype == XLarrow ||
		 With->prim->ptype == XLline)) {
		lineardir(With->prim, attstack[newp + 2].xval,
			  attstack[newp + 2].xval, &With->state);
	    }
	    else if (attstack[newp + 1].lexval == XLsolid ||
		     attstack[newp + 1].lexval == XLinvis) {
		markerror(858);
	    }
	    else {
		With->prim->lparam = attstack[newp + 2].xval;
	    }
	}
    }
    break;

  /* | object "chop" optexp */
  case object10:
    if (With->prim != NULL) {
	if (With->prim->ptype != XLspline && With->prim->ptype != XLmove &&
	     With->prim->ptype != XLarrow && With->prim->ptype != XLline) {
	    markerror(858);
	}
	else {
	    With2 = With->prim;
	    if (attstack[newp + 2].lexval != XEMPTY) {
		r = attstack[newp + 2].xval;
	    }
	    else {
		eb = findenv(envblock);
		r = eb->Upr.Ublock.env[XLcirclerad - XLenvvar - 1];
	    }
	    if (teststflag(With->state, XLchop)) {
		With->endchop = r;
	    }
	    else {
		With->startchop = r;
		With->endchop = r;
		setstflag(&With->state, XLchop);
	    }
	}
    }
    break;

  /* | object "fill" optexp */
  case object11:
    if (With->prim != NULL) {
	With2 = With->prim;
	if (attstack[newp + 2].lexval != XEMPTY) {
	    s = attstack[newp + 2].xval;
	}
	else {
	    eb = findenv(envblock);
	    s = eb->Upr.Ublock.env[XLfillval - XLenvvar - 1];
	}
	prp = With->prim;
	while (prp != NULL) {
	    switch (With2->ptype) {

	    case XLbox:
	      prp->Upr.Ubox.boxfill = s;
	      break;

	    case XLcircle:
	      prp->Upr.Ucircle.cfill = s;
	      break;

	    case XLellipse:
	      prp->Upr.Uellipse.efill = s;
	      break;

	    default:
	      if (drawmode == Pict2e || drawmode == tTeX || drawmode == TeX) {
		  markerror(858);
	      }
	      else {
		  switch (With2->ptype) {

		  case XLline:
		  case XLarrow:
		  case XLmove:
		  case XLspline:
		  case XLarc:
		    prp->Upr.Uline.lfill = s;
		    break;

		  default:
		    markerror(858);
		    break;
		  }
	      }
	      break;
	    }
	    if (isthen(prp)) {
		prp = prp->parent;
	    }
	    else {
		prp = NULL;
	    }
	}
    }
    break;

  /* | object "<arrowhd>" optexp */
  case object12:
    if (With->prim != NULL) {
	With2 = With->prim;
	if (With2->ptype != XLspline && With2->ptype != XLarc &&
	     With2->ptype != XLarrow && With2->ptype != XLline) {
	    markerror(858);
	}
	else {
	    With2->Upr.Uline.atype = pahlex(With2->Upr.Uline.atype,
					  attstack[newp + 1].lexval);
	    if (attstack[newp + 2].lexval != XEMPTY) {
		lj = (long)floor(attstack[newp + 2].xval + 0.5);
	    }
	    else {
		eb = findenv(envblock);
		lj = (long)floor(eb->Upr.Ublock.env[XLarrowhead - XLenvvar - 1] + 0.5);
	    }
	    With2->Upr.Uline.atype = pahnum(With2->Upr.Uline.atype, lj);
	}
    }
    break;

  /* | object "then" */
  case object13:
    if (With->prim != NULL) {
	/*D if debuglevel > 0 then begin write(log,'lexical "then" found, ');
	    prtstval(state); writeln(log) end; D*/
	appendthen(&With->prim);
	if (With->prim->ptype != XLarc) {
	    setstflag(&With->state, XEMPTY);
	}
    }
    break;

  /* | object "cw" */
  case object14:
    if (With->prim != NULL) {
	With2 = With->prim;
	if (With2->ptype != XLarc) {
	    markerror(858);
	}
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
	    if (With2->Upr.Uline.endpos.ypos > 0.0 && With2->direction == 0) {
		With2->Upr.Uline.endpos.ypos = -fabs(
		    principal(2.0 * pi - With2->Upr.Uline.endpos.ypos, 2.0 * pi));
	    }
	    else if (With2->direction != 0) {
		With2->aat = arcstart(With->prim);
		switch (With2->direction) {

		case XLup:
		  With2->aat.xpos += With2->Upr.Uline.aradius;
		  break;

		case XLdown:
		  With2->aat.xpos -= With2->Upr.Uline.aradius;
		  break;

		case XLleft:
		  With2->aat.ypos += With2->Upr.Uline.aradius;
		  break;

		case XLright:
		  With2->aat.ypos -= With2->Upr.Uline.aradius;
		  break;
		}
		if (With2->Upr.Uline.endpos.ypos > 0.0) {
		    With2->Upr.Uline.endpos.xpos = principal(
			With2->Upr.Uline.endpos.xpos + pi, pi);
		}
		With2->Upr.Uline.endpos.ypos = -fabs(With2->Upr.Uline.endpos.ypos);
	    }
	    setstflag(&With->state, XLcw);
	}
    }
    break;

  /* | object "ccw" */
  case object15:
    if (With->prim != NULL) {
	With2 = With->prim;
	if (With2->ptype != XLarc) {
	    markerror(858);
	}
	else {
	    /* if teststflag(state,XLat) then begin
	       if endpos.ypos<0.0 then endpos.ypos := 2*pi+endpos.ypos
	       end
	    else */
	    if (With2->Upr.Uline.endpos.ypos < 0.0 && With2->direction == 0) {
		With2->Upr.Uline.endpos.ypos = fabs(
		    principal(With2->Upr.Uline.endpos.ypos - 2.0 * pi, 2.0 * pi));
	    }
	    else if (With2->direction != 0) {
		With2->aat = arcstart(With->prim);
		switch (With2->direction) {

		case XLup:
		  With2->aat.xpos -= With2->Upr.Uline.aradius;
		  break;

		case XLdown:
		  With2->aat.xpos += With2->Upr.Uline.aradius;
		  break;

		case XLleft:
		  With2->aat.ypos -= With2->Upr.Uline.aradius;
		  break;

		case XLright:
		  With2->aat.ypos += With2->Upr.Uline.aradius;
		  break;
		}
		if (With2->Upr.Uline.endpos.ypos < 0.0) {
		    With2->Upr.Uline.endpos.xpos = principal(
			With2->Upr.Uline.endpos.xpos + pi, pi);
		}
		With2->Upr.Uline.endpos.ypos = fabs(With2->Upr.Uline.endpos.ypos);
	    }
	    setstflag(&With->state, XLccw);
	}
    }
    break;

  /* | object "same" */
  case object16:
    if (With->prim != NULL) {
	prp = nthprimobj(envblock->son, 0, With->prim->ptype);
	if (prp == NULL) {
	    markerror(857);
	}
	else {
	    With2 = With->prim;
	    With2->lparam = prp->lparam;
	    With2->lthick = prp->lthick;
	    With2->direction = prp->direction;
	    With2->spec = prp->spec;
	    if (hasoutline(With->lexval, false)) {
		copystr(&freeseg, &With2->outlinep, prp->outlinep);
	    }
	    if (hasshade(With->lexval, false)) {
		copystr(&freeseg, &With2->shadedp, prp->shadedp);
	    }
	}
	if (prp != NULL) {
	    With2 = With->prim;
	    switch (With2->ptype) {

	    case XLbox:
	    case XLstring:
	      if (With2->ptype == XLbox) {
		  switch (With2->direction) {

		  case XLup:
		    With2->aat.ypos += 0.5 *
			(prp->Upr.Ubox.boxheight - With2->Upr.Ubox.boxheight);
		    break;

		  case XLdown:
		    With2->aat.ypos -= 0.5 *
			(prp->Upr.Ubox.boxheight - With2->Upr.Ubox.boxheight);
		    break;

		  case XLleft:
		    With2->aat.xpos -=
		      0.5 * (prp->Upr.Ubox.boxwidth - With2->Upr.Ubox.boxwidth);
		    break;

		  case XLright:
		    With2->aat.xpos +=
		      0.5 * (prp->Upr.Ubox.boxwidth - With2->Upr.Ubox.boxwidth);
		    break;
		  }
	      }
	      With2->Upr.Ubox.boxfill = prp->Upr.Ubox.boxfill;
	      With2->Upr.Ubox.boxheight = prp->Upr.Ubox.boxheight;
	      With2->Upr.Ubox.boxwidth = prp->Upr.Ubox.boxwidth;
	      With2->Upr.Ubox.boxradius = prp->Upr.Ubox.boxradius;
	      break;

	    case XBLOCK:
	      markerror(858);
	      break;

	    case XLcircle:
	      switch (With2->direction) {

	      case XLup:
		With2->aat.ypos += prp->Upr.Ucircle.radius - With2->Upr.Ucircle.radius;
		break;

	      case XLdown:
		With2->aat.ypos += With2->Upr.Ucircle.radius - prp->Upr.Ucircle.radius;
		break;

	      case XLleft:
		With2->aat.xpos += With2->Upr.Ucircle.radius - prp->Upr.Ucircle.radius;
		break;

	      case XLright:
		With2->aat.xpos += prp->Upr.Ucircle.radius - With2->Upr.Ucircle.radius;
		break;
	      }
	      With2->Upr.Ucircle.cfill = prp->Upr.Ucircle.cfill;
	      With2->Upr.Ucircle.radius = prp->Upr.Ucircle.radius;
	      break;

	    case XLellipse:
	      switch (With2->direction) {

	      case XLup:
		With2->aat.ypos +=
		  0.5 * (prp->Upr.Uellipse.elheight - With2->Upr.Uellipse.elheight);
		break;

	      case XLdown:
		With2->aat.ypos -=
		  0.5 * (prp->Upr.Uellipse.elheight - With2->Upr.Uellipse.elheight);
		break;

	      case XLleft:
		With2->aat.xpos -=
		  0.5 * (prp->Upr.Uellipse.elwidth - With2->Upr.Uellipse.elwidth);
		break;

	      case XLright:
		With2->aat.xpos +=
		  0.5 * (prp->Upr.Uellipse.elwidth - With2->Upr.Uellipse.elwidth);
		break;
	      }
	      With2->Upr.Uellipse.efill = prp->Upr.Uellipse.efill;
	      With2->Upr.Uellipse.elheight = prp->Upr.Uellipse.elheight;
	      With2->Upr.Uellipse.elwidth = prp->Upr.Uellipse.elwidth;
	      break;

	    case XLarc:
	      x1 = With2->aat.xpos +
		   With2->Upr.Uline.aradius * cos(With2->Upr.Uline.endpos.xpos);
	      y1 = With2->aat.ypos +
		   With2->Upr.Uline.aradius * sin(With2->Upr.Uline.endpos.xpos);
	      With2->Upr.Uline.aradius = prp->Upr.Uline.aradius;
	      With2->Upr.Uline.endpos.xpos = prp->Upr.Uline.endpos.xpos;
	      With2->aat.xpos =
		x1 - With2->Upr.Uline.aradius * cos(With2->Upr.Uline.endpos.xpos);
	      With2->aat.ypos =
		y1 - With2->Upr.Uline.aradius * sin(With2->Upr.Uline.endpos.xpos);
	      With2->Upr.Uline.endpos.ypos = prp->Upr.Uline.endpos.ypos;
	      break;

	    case XLline:
	    case XLarrow:
	    case XLmove:
	    case XLspline:
	      With2->Upr.Uline.endpos.xpos =
		With2->aat.xpos + prp->Upr.Uline.endpos.xpos - prp->aat.xpos;
	      With2->Upr.Uline.endpos.ypos =
		With2->aat.ypos + prp->Upr.Uline.endpos.ypos - prp->aat.ypos;
	      With2->Upr.Uline.height = prp->Upr.Uline.height;
	      With2->Upr.Uline.width = prp->Upr.Uline.width;
	      With2->Upr.Uline.atype = prp->Upr.Uline.atype;
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
	if (With->prim != NULL) {
	    With2 = With->prim;
	    if (With2->textp == NULL) {
		With2->textp = attstack[newp + 1].prim->textp;
	    }
	    else {
		namptr = With2->textp;
		i = 1;
		while (namptr->next_ != NULL) {
		    namptr = namptr->next_;
		    i++;
		}
		namptr->next_ = attstack[newp + 1].prim->textp;
		if (With2->ptype == XLstring) {
		    With2->Upr.Ubox.boxheight = With2->Upr.Ubox.boxheight *
					       (i + 1) / i;
		    /* boxheight/((i-1)*TEXTRATIO+1)*(i*TEXTRATIO+1) */
		}
	    }
	    if (drawmode == PSfrag || drawmode == PDF || drawmode == PS)
	    {                                                         /*,PSmps*/
		/* output contains text */
		printstate = (printstate >> 1) * 2 + 1;
	    }
	}
	attstack[newp + 1].prim->textp = NULL;
	deletestringbox(&attstack[newp + 1].prim);
    }
    break;

  /* | object "by" position */
  case object18:
    if (With->prim != NULL) {
	if (With->prim->ptype == XLmove || With->prim->ptype == XLspline ||
	     With->prim->ptype == XLarrow || With->prim->ptype == XLline ||
	     With->prim->ptype == XLarc) {
	    With2 = With->prim;
	    x1 = attstack[newp + 2].xval + With2->aat.xpos;
	    y1 = attstack[newp + 2].yval + With2->aat.ypos;
	    if (With2->ptype == XLarc) {
		x1 += With2->Upr.Uline.aradius * cos(With2->Upr.Uline.endpos.xpos);
		y1 += With2->Upr.Uline.aradius * sin(With2->Upr.Uline.endpos.xpos);
	    }
	    attstack[newp + 2].xval = x1;
	    attstack[newp + 2].yval = y1;
	    doprod(object20);
	}
	else {
	    markerror(858);
	}
    }
    break;

  /* | object "from" position */
  case object19:
    if (With->prim != NULL) {
	With2 = With->prim;
	if (With2->ptype == XLmove || With2->ptype == XLspline ||
	     With2->ptype == XLarrow || With2->ptype == XLline ||
	     With2->ptype == XLarc) {
	    if (With2->ptype == XLarc) {
		r = attstack[newp + 2].xval;
		s = attstack[newp + 2].yval;
		if (teststflag(With->state, XLto)) {
		    attstack[newp + 2].xval = With2->aat.xpos +
			With2->Upr.Uline.aradius * cos(
			  With2->Upr.Uline.endpos.xpos + With2->Upr.Uline.endpos.ypos);
		    attstack[newp + 2].yval = With2->aat.ypos +
			With2->Upr.Uline.aradius * sin(
			  With2->Upr.Uline.endpos.xpos + With2->Upr.Uline.endpos.ypos);
		    doprod(object20);
		}
		With2->aat.xpos = r - With2->Upr.Uline.aradius *
				      cos(With2->Upr.Uline.endpos.xpos);
		With2->aat.ypos = s - With2->Upr.Uline.aradius *
				      sin(With2->Upr.Uline.endpos.xpos);
	    }
	    else if (!teststflag(With->state, XLto)) {
		prp = With->prim;
		while (isthen(prp)) {
		    prp = prp->parent;
		}
		shift(prp, attstack[newp + 2].xval - prp->aat.xpos,
		      attstack[newp + 2].yval - prp->aat.ypos);
	    }
	    else {
		With2->aat.xpos = attstack[newp + 2].xval;
		With2->aat.ypos = attstack[newp + 2].yval;
	    }
	    setstflag(&With->state, XLfrom);
	}
	else {
	    markerror(858);
	}
    }
    break;

  /* | object "to" position */
  case object20:
    if (With->prim != NULL) {
	if (With->prim->ptype == XLmove || With->prim->ptype == XLspline ||
	     With->prim->ptype == XLarrow || With->prim->ptype == XLline ||
	     With->prim->ptype == XLarc) {
	    if ((With->prim->ptype != XLarc) & teststflag(With->state, XLto)) {
		/* (teststflag(state,XLto) or teststflag(state,XLdirecton)) */
		/*D if debuglevel > 0 then begin writeln(log,'"then" inserted, ');
		    prtstval(state); writeln(log) end; D*/
		appendthen(&With->prim);
		setstflag(&With->state, XEMPTY);
	    }
	    With2 = With->prim;
	    if (With2->ptype != XLarc) {
		With2->Upr.Uline.endpos.xpos = attstack[newp + 2].xval;
		With2->Upr.Uline.endpos.ypos = attstack[newp + 2].yval;
	    }
	    else {
		x1 = With2->aat.xpos +
		     With2->Upr.Uline.aradius * cos(With2->Upr.Uline.endpos.xpos);
		    /* from position */
		y1 = With2->aat.ypos +
		     With2->Upr.Uline.aradius * sin(With2->Upr.Uline.endpos.xpos);
		dx = attstack[newp + 2].xval - x1;
		dy = attstack[newp + 2].yval - y1;
		ts = dx * dx + dy * dy;                            /* chord^2 */
		if (With2->direction != 0) {
		    i = With2->direction;
		}
		else {
		    i = With->length;
		}
		/*D if debuglevel = 2 then begin
		   write(log,' (x1,y1)='); wpair(log,x1,y1);
		   write(log,' (dx,dy)='); wpair(log,dx,dy);
		   write(log,' ts='); wfloat(log,ts);
		   write(log,' i=',i:1 ) end; D*/
		if (ts == 0.0) {
		    With2->Upr.Uline.endpos.ypos = 0.0;
		}
		else {  /* ratio centre-to-chord/half-chord */
		    t = sqrt(Max(0.0, 4.0 * With2->Upr.Uline.aradius * With2->
					Upr.Uline.aradius - ts) / ts);
		    /*D if debuglevel = 2 then begin
		       write(log,' t='); wfloat(log,t);
		       write(log,' |arcangle','|='); wfloat(log,endpos.ypos*180/pi);
		       writeln(log) end; D*/
		    r = sqrt(ts);
		    if (t <= 0.0) {                /* t is always nonnegative */
			With2->Upr.Uline.aradius = 0.5 * r;
		    }
		    switch (i) {
			/* Determine which of the two default arcs to draw: */

		    case XLup:
		      if (With2->Upr.Uline.endpos.ypos * (-dx - t * dy) < 0.0) {
			  t = -t;
		      }
		      break;

		    case XLdown:
		      if (With2->Upr.Uline.endpos.ypos * (-dx - t * dy) > 0.0) {
			  t = -t;
		      }
		      break;

		    case XLright:
		      if (With2->Upr.Uline.endpos.ypos * (dy - t * dx) < 0.0) {
			  t = -t;
		      }
		      break;

		    case XLleft:
		      if (With2->Upr.Uline.endpos.ypos * (dy - t * dx) > 0.0) {
			  t = -t;
		      }
		      break;
		    }
		    With2->aat.xpos = x1 + 0.5 * (dx + t * dy);
		    With2->aat.ypos = y1 + 0.5 * (dy - t * dx);
		    /*D if debuglevel = 2 then begin
		       write(log,' t='); wfloat(log,t);
		       write(log,' aradius='); wfloat(log,aradius);
		       write(log,' aat='); wpair(log,aat.xpos,aat.ypos);
		       writeln(log) end; D*/
		    setangles(&With2->Upr.Uline.endpos.xpos,
			      &With2->Upr.Uline.endpos.ypos, With2->aat, x1, y1,
			      attstack[newp + 2].xval,
			      attstack[newp + 2].yval);
		}
		if (With2->direction != 0) {
		    With->length = With2->direction;
		}
		With2->direction = 0;
	    }
	    /*D ; if debuglevel > 0 then printobject(prim) D*/
	    setstflag(&With->state, XLto);
	}
	else {
	    markerror(858);
	}
    }
    break;

  /* | object "at" position */
  case object21:
    if (With->prim != NULL) {
	With2 = With->prim;
	With->xval = attstack[newp + 2].xval;
	With->yval = attstack[newp + 2].yval;
	setstval(&With->state, XDc);
	setstflag(&With->state, XLat);
    }
    break;

  /* | object "<textpos>" */
  /* This might be altered (in the case of strings, not other objects with text),
       so that aat is changed as a function of textpos. Then
       the output routines have to take that into account. The alternative
       is to alter nesw for strings, as now. */
  case object22:
    if (With->prim != NULL) {
	namptr = With->prim->textp;
	if (namptr != NULL) {
	    while (namptr->next_ != NULL) {
		namptr = namptr->next_;
	    }
	    i = (long)floor(namptr->val + 0.5);
	    if (i > 8) {
		i = 0;
	    }
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
	}
	else {
	    markerror(861);
	}
    }
    break;

  /* | object ("colour"|"outline"|"shade") stringexpr */
  case object23:
    if (drawmode != SVG && drawmode != PSTricks && drawmode != PSfrag &&
	 drawmode != PS && drawmode != PDF && drawmode != PGF &&
	 drawmode != MFpic && drawmode != MPost) {
	/*,PSmps*/
	markerror(858);
    }
    else if (attstack[newp + 2].prim != NULL && With->prim != NULL) {
	With2 = With->prim;
	switch (attstack[newp + 1].lexval) {

	case XLshaded:
	  if (hasshade(With->lexval, true)) {
	      deletename(&With2->shadedp);
	      With2->shadedp = attstack[newp + 2].prim->textp;
	      attstack[newp + 2].prim->textp = NULL;
	  }
	  break;

	case XLoutlined:
	  if (hasoutline(With->lexval, true)) {
	      deletename(&With2->outlinep);
	      With2->outlinep = attstack[newp + 2].prim->textp;
	      attstack[newp + 2].prim->textp = NULL;
	  }
	  break;

	case XLcolour:
	  if (hasoutline(With->lexval, true)) {
	      deletename(&With2->outlinep);
	      With2->outlinep = attstack[newp + 2].prim->textp;
	      attstack[newp + 2].prim->textp = NULL;
	      /* if (attstack^[newp+1].lexval=XLcolour) and */
	      if ((With2->ptype != XLspline && With2->ptype != XLarrow &&
		   With2->ptype != XLline &&
		   With2->ptype != XLarc) & hasshade(With->lexval, false)) {
		  deletename(&With2->shadedp);
		  copystr(&freeseg, &With2->shadedp, With2->outlinep);
	      }
	  }
	  break;
	}
    }
    deletestringbox(&attstack[newp + 2].prim);
    break;

  /* | objectwith "at" position */
  case object24:
    if (With->prim != NULL) {
	With->xval = attstack[newp + 2].xval;
	With->yval = attstack[newp + 2].yval;
	setstval(&With->state, XEMPTY);
	setstflag(&With->state, XLat);
    }
    break;

  /* | objectwith "<corner>" "at" position */
  /* | objectwith pair "at" position */
  case object25:
  case object26:
    if (With->prim != NULL) {
	With->xval = attstack[newp + 3].xval;
	With->yval = attstack[newp + 3].yval;
	if (p == object25) {
	    setstval(&With->state, attstack[newp + 1].lexval);
	}
	else if (With->prim->ptype != XLarc &&
		 With->prim->ptype != XLellipse &&
		 With->prim->ptype != XLcircle &&
		 With->prim->ptype != XBLOCK &&
		 With->prim->ptype != XLstring &&
		 With->prim->ptype != XLbox) {
	    markerror(858);
	}
	else {
	    With->startchop = attstack[newp + 1].xval;
	    With->endchop = attstack[newp + 1].yval;
	    setstval(&With->state, XLfloat);
	}
	setstflag(&With->state, XLat);
    }
    break;

  /* | "continue" */
  case object27:
    primp = NULL;
    prp = envblock->son;
    while (prp != NULL) {
	if (prp->ptype != XLaTeX && prp->ptype != XLstring &&
	     prp->ptype != XBLOCK && prp->ptype != XLabel) {
	    primp = prp;
	}
	prp = prp->next_;
    }
    if (primp == NULL) {
	markerror(857);
    }
    else {
	while (primp->son != NULL) {
	    primp = primp->son;
	}
	With->prim = primp;
	doprod(object13);
    }
    break;

  /* openblock = "<EMPTY>" */
  case openblock1:
    /*D if debuglevel > 0 then writeln(log,
       'Calling newprim(prim,',XBLOCK:1,'):'); D*/
    newprim(&With->prim, XBLOCK, envblock);
    /* prim^.here := envblock^.here; */
    /* Bah! gpic compatibility: */
    /* with prim^.here do begin xpos := 0.0; ypos := 0.0 end; */
    With->prim->Upr.Ublock.here.xpos = 0.0;
    With->prim->Upr.Ublock.here.ypos = 0.0;
    envblock = With->prim;
    With->lexval = 0;
    /* ; resetenv(0,envblock) */
    /*D; if (debuglevel > 0) and (prim <> nil) then printobject(prim)D*/
    break;

  /* block = "<primitiv>" optexp */
  /* | stringexpr */
  /* | openblock "[" closeblock "]" */
  /* | openblock "[]" */
  /* set parameters for primitive object */
  case block1:
    if (With->lexval > XLprimitiv && With->lexval < XLenvvar) {
	newprim(&With->prim, With->lexval, envblock);
	eb = findenv(envblock);
	if (((drawmode == PSfrag || drawmode == SVG || drawmode == PS ||
	      drawmode == PDF || drawmode == Pict2e ||
	      drawmode == MPost) && With->lexval != XLmove) ||
	    With->lexval == XLarc)
	{                                                             /*,PSmps*/
	    With->prim->lthick = eb->Upr.Ublock.env[XLlinethick - XLenvvar - 1];
	    /*D; if debuglevel > 0 then begin write(log,
	        ' envblock[',ordp(eb),'] setting linethick=');
	        wfloat(log,prim^.lthick); writeln(log) end D*/
	}
	/* (drawmode in [PGF,PSTricks]) and */
	if (attstack[newp + 1].lexval != XEMPTY && With->lexval != XLmove &&
	     With->lexval != XLspline && With->lexval != XLarrow &&
	     With->lexval != XLline) {
	    markerror(858);
	}
	With2 = With->prim;
	switch (With->lexval) {

	case XLbox:
	  With2->Upr.Ubox.boxheight = eb->Upr.Ublock.env[XLboxht - XLenvvar - 1];
	  With2->Upr.Ubox.boxwidth = eb->Upr.Ublock.env[XLboxwid - XLenvvar - 1];
	  With2->Upr.Ubox.boxradius = eb->Upr.Ublock.env[XLboxrad - XLenvvar - 1];
	  switch (With2->direction) {

	  case XLup:
	    With2->aat.ypos += With2->Upr.Ubox.boxheight * 0.5;
	    break;

	  case XLdown:
	    With2->aat.ypos -= With2->Upr.Ubox.boxheight * 0.5;
	    break;

	  case XLleft:
	    With2->aat.xpos -= With2->Upr.Ubox.boxwidth * 0.5;
	    break;

	  case XLright:
	    With2->aat.xpos += With2->Upr.Ubox.boxwidth * 0.5;
	    break;
	  }
	  break;

	case XLcircle:
	  With2->Upr.Ucircle.radius = eb->Upr.Ublock.env[XLcirclerad - XLenvvar - 1];
	  switch (With2->direction) {

	  case XLup:
	    With2->aat.ypos += With2->Upr.Ucircle.radius;
	    break;

	  case XLdown:
	    With2->aat.ypos -= With2->Upr.Ucircle.radius;
	    break;

	  case XLleft:
	    With2->aat.xpos -= With2->Upr.Ucircle.radius;
	    break;

	  case XLright:
	    With2->aat.xpos += With2->Upr.Ucircle.radius;
	    break;
	  }
	  break;

	case XLellipse:
	  With2->Upr.Uellipse.elheight = eb->Upr.Ublock.env[XLellipseht - XLenvvar - 1];
	  With2->Upr.Uellipse.elwidth = eb->Upr.Ublock.env[XLellipsewid - XLenvvar - 1];
	  switch (With2->direction) {

	  case XLup:
	    With2->aat.ypos += With2->Upr.Uellipse.elheight * 0.5;
	    break;

	  case XLdown:
	    With2->aat.ypos -= With2->Upr.Uellipse.elheight * 0.5;
	    break;

	  case XLleft:
	    With2->aat.xpos -= With2->Upr.Uellipse.elwidth * 0.5;
	    break;

	  case XLright:
	    With2->aat.xpos += With2->Upr.Uellipse.elwidth * 0.5;
	    break;
	  }
	  break;

	case XLarc:
	  With2->Upr.Uline.aradius = eb->Upr.Ublock.env[XLarcrad - XLenvvar - 1];
	  switch (With2->direction) {

	  case XLup:
	    With2->Upr.Uline.endpos.xpos = 0.0;
	    With2->aat.xpos -= With2->Upr.Uline.aradius;
	    break;

	  case XLdown:
	    With2->Upr.Uline.endpos.xpos = pi;
	    With2->aat.xpos += With2->Upr.Uline.aradius;
	    break;

	  case XLleft:
	    With2->Upr.Uline.endpos.xpos = 0.5 * pi;
	    With2->aat.ypos -= With2->Upr.Uline.aradius;
	    break;

	  case XLright:
	    With2->Upr.Uline.endpos.xpos = -0.5 * pi;
	    With2->aat.ypos += With2->Upr.Uline.aradius;
	    break;
	  }
	  With2->Upr.Uline.height = eb->Upr.Ublock.env[XLarrowht - XLenvvar - 1];
	  With2->Upr.Uline.width = eb->Upr.Ublock.env[XLarrowwid - XLenvvar - 1];
	      /* atype := XEMPTY; */
	  With2->Upr.Uline.atype = pahnum(pahlex(0, XEMPTY),
	      (long)floor(eb->Upr.Ublock.env[XLarrowhead - XLenvvar - 1] + 0.5));
	  With2->Upr.Uline.endpos.ypos = pi * 0.5;
	  break;

	case XLline:
	case XLarrow:
	case XLspline:
	case XLmove:
	  With2->Upr.Uline.endpos = With2->aat;
	  if (With2->ptype == XLspline && attstack[newp + 1].lexval != XEMPTY) {
	      /* This implements adjusted splines */
	      With2->Upr.Uline.aradius = attstack[newp + 1].xval;
	      attstack[newp + 1].lexval = XEMPTY;
	  }  /* */
	  if (attstack[newp + 1].lexval != XEMPTY) {
	      r = attstack[newp + 1].xval;
	  }
	  else if (With2->ptype == XLmove) {
	      switch (With2->direction) {

	      case XLup:
	      case XLdown:
		r = eb->Upr.Ublock.env[XLmoveht - XLenvvar - 1];
		break;

	      case XLleft:
	      case XLright:
		r = eb->Upr.Ublock.env[XLmovewid - XLenvvar - 1];
		break;
	      }
	  }
	  else {
	      switch (With2->direction) {

	      case XLup:
	      case XLdown:
		r = eb->Upr.Ublock.env[XLlineht - XLenvvar - 1];
		break;

	      case XLleft:
	      case XLright:
		r = eb->Upr.Ublock.env[XLlinewid - XLenvvar - 1];
		break;
	      }
	  }
	  switch (With2->direction) {

	  case XLup:
	    With2->Upr.Uline.endpos.ypos = With2->aat.ypos + r;
	    break;

	  case XLdown:
	    With2->Upr.Uline.endpos.ypos = With2->aat.ypos - r;
	    break;

	  case XLleft:
	    With2->Upr.Uline.endpos.xpos = With2->aat.xpos - r;
	    break;

	  case XLright:
	    With2->Upr.Uline.endpos.xpos = With2->aat.xpos + r;
	    break;
	  }
	  With2->Upr.Uline.height = eb->Upr.Ublock.env[XLarrowht - XLenvvar - 1];
	  With2->Upr.Uline.width = eb->Upr.Ublock.env[XLarrowwid - XLenvvar - 1];
	  if (With2->ptype == XLarrow) {
	      With2->Upr.Uline.atype = pahlex(0, XRIGHTHEAD);
	  }
	  else {
	      With2->Upr.Uline.atype = pahlex(0, XEMPTY);
	  }
	  With2->Upr.Uline.atype = pahnum(With2->Upr.Uline.atype,
	      (long)floor(eb->Upr.Ublock.env[XLarrowhead - XLenvvar - 1] + 0.5));
	  break;
	}
    }
    break;

  /* | stringexpr */
  case block2:
    if (drawmode == PSfrag || drawmode == PDF || drawmode == PS) {    /*,PSmps*/
	/* flag text in output */
	printstate = (printstate >> 1) * 2 + 1;
    }
    break;

  /* | openblock "[" closeblock "]" */
  case block3:
    if (With->prim != NULL) {
	With->prim->son = attstack[newp + 2].prim;
	envblock = With->prim->parent;
	getnesw(With->prim->son);
	With2 = With->prim;
	With2->Upr.Ublock.blockwidth = east - west;
	With2->Upr.Ublock.blockheight = north - south;
	With2->aat.xpos = (east + west) * 0.5;
	With2->aat.ypos = (north + south) * 0.5;
	dx = envblock->Upr.Ublock.here.xpos - With2->aat.xpos;
	dy = envblock->Upr.Ublock.here.ypos - With2->aat.ypos;
	switch (envblock->direction) {

	case XLright:
	  dx += With2->Upr.Ublock.blockwidth * 0.5;
	  break;

	case XLleft:
	  dx -= With2->Upr.Ublock.blockwidth * 0.5;
	  break;

	case XLup:
	  dy += With2->Upr.Ublock.blockheight * 0.5;
	  break;

	case XLdown:
	  dy -= With2->Upr.Ublock.blockheight * 0.5;
	  break;
	}
	With2->direction = envblock->direction;         /* gpic compatibility */
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
	shift(With->prim, dx, dy);
    }
    break;

  /* | openblock "[]" */
  case block4:
    if (With->prim != NULL) {
	envblock = With->prim->parent;
    }
    break;

  /* optexp = "<EMPTY>" */
  /* | expression */
  case optexp1:
    With->lexval = XEMPTY;
    break;

  case optexp2:
    break;

  /* closeblock = elementlist optnl */
  /* Add latex to reset env variables changed within a block */
  case closeblock1:
    if (With->prim != NULL && envblock->Upr.Ublock.env != NULL) {
	eb = findenv(envblock->parent);
	if (eb != NULL) {  /* check and reset variables: */
	    if (envblock->Upr.Ublock.env[XLlinethick - XLenvvar - 1] !=
		 eb->Upr.Ublock.env[XLlinethick - XLenvvar - 1]) {
		newprim(&attstack[newp + 2].prim, XLaTeX, envblock);
		attstack[newp + 2].prim->lthick =
		  eb->Upr.Ublock.env[XLlinethick - XLenvvar - 1];
		addelem(attstack[newp + With->state].prim,
			attstack[newp + 2].prim);
	    }
	    if (envblock->Upr.Ublock.env[XLdashwid - XLenvvar - 1] !=
		 eb->Upr.Ublock.env[XLdashwid - XLenvvar - 1]) {
		newprim(&attstack[newp + 2].prim, XLaTeX, envblock);
		attstack[newp + 2].prim->lparam =
		  eb->Upr.Ublock.env[XLdashwid - XLenvvar - 1];
		addelem(attstack[newp + With->state].prim,
			attstack[newp + 2].prim);
	    }
	}
    }
    break;

  /* pair = expression "," expression */
  /* | location shift */
  case pair1:
    With->yval = attstack[newp + 2].xval;
    break;

  case pair2:
    if (attstack[newp + 1].lexval != XEMPTY) {
	With->xval += attstack[newp + 1].xval;
	With->yval += attstack[newp + 1].yval;
    }
    break;

  /* objectwith = object "with" */
  /* | objectwith "." "<Label>" suffix */
  case objectwith1:
    break;

  case objectwith2:
    if (With->prim != NULL) {
	if (attstack[newp + 3].lexval != XEMPTY) {
	    With1 = &attstack[newp + 2];
	    addsuffix(chbuf, &With1->chbufx, &With1->length,
		      attstack[newp + 3].xval);
	}
	if (With->internal == NULL) {
	    prp = With->prim;
	}
	else {
	    prp = With->internal;
	}
	With->internal = findplace(prp->son, chbuf, attstack[newp + 2].chbufx,
				   attstack[newp + 2].length);
	if (With->internal == NULL) {
	    marknotfound(855, chbuf, attstack[newp + 2].chbufx,
			 attstack[newp + 2].length);
	    deletetree(&With->prim);
	}
    }
    break;

  /* | objectwith "." nth primobj */
  case objectwith3:
    if (With->prim != NULL) {
	if (With->internal == NULL) {
	    prp = With->prim;
	}
	else {
	    prp = With->internal;
	}
	With->internal = nthprimobj(prp->son, attstack[newp + 2].length,
				    attstack[newp + 3].lexval);
	if (With->internal == NULL) {
	    markerror(857);
	    deletetree(&With->prim);
	}
	/* else lexval := attstack^[newp+1].lexval */
    }
    break;

  /* shift = "<EMPTY>" */
  /* | shift "+" location */
  /* | shift "-" location */
  case shift1:
    With->xval = 0.0;
    With->yval = 0.0;
    With->lexval = XEMPTY;
    break;

  case shift2:
    With->xval += attstack[newp + 2].xval;
    With->yval += attstack[newp + 2].yval;
    With->lexval = XLfloat;
    break;

  case shift3:
    With->xval -= attstack[newp + 2].xval;
    With->yval -= attstack[newp + 2].yval;
    With->lexval = XLfloat;
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
    With->xval = attstack[newp + 1].xval;
    With->yval = attstack[newp + 3].yval;
    break;

  case location3:
    /* blank case */
    break;

  case location4:
    With->xval *= attstack[newp + 2].xval;
    With->yval *= attstack[newp + 2].xval;
    break;

  case location5:
    if (attstack[newp + 2].xval == 0.0) {
	markerror(852);
    }
    else {
	With->xval /= attstack[newp + 2].xval;
	With->yval /= attstack[newp + 2].xval;
    }
    break;

  /* place = placename */
  /* | placename "<corner>" */
  /* | "<corner>" placename */
  /* | "<corner>" "of" placename */
  /* | "Here" */
  case place1:
    corner(With->prim, XEMPTY, &With->xval, &With->yval);
    break;

  case place2:
    corner(With->prim, attstack[newp + 1].lexval, &With->xval, &With->yval);
    break;

  case place3:
    corner(attstack[newp + 1].prim, With->lexval, &With->xval, &With->yval);
    break;

  case place4:
    corner(attstack[newp + 2].prim, With->lexval, &With->xval, &With->yval);
    break;

  case place5:
    With->xval = envblock->Upr.Ublock.here.xpos;
    With->yval = envblock->Upr.Ublock.here.ypos;
    break;

  /* factor = primary */
  /* | "!" primary */
  /* | primary "^" factor */
  case factor1:
    break;

  case factor2:
    if (attstack[newp + 1].xval == 0.0) {
	With->xval = 1.0;
    }
    else {
	With->xval = 0.0;
    }
    break;

  case factor3:
    if (With->xval == 0.0 && attstack[newp + 2].xval < 0.0) {
	markerror(852);
    }
    else {
	j = (long)floor(attstack[newp + 2].xval + 0.5);
	r = j;
	if (r == attstack[newp + 2].xval) {
	    With->xval = intpow(With->xval, j);
	}
	else if (With->xval < 0.0) {
	    markerror(863);
	    With->xval = 0.0;
	}
	else if (With->xval != 0.0) {
	    With->xval = exp(attstack[newp + 2].xval * log(With->xval));
	}
    }
    break;

  /* placename = "<Label>" suffix */
  case placename1:
    if (attstack[newp + 1].lexval != XEMPTY) {
	addsuffix(chbuf, &With->chbufx, &With->length,
		  attstack[newp + 1].xval);
    }
    prp = NULL;
    primp = envblock;
    while (primp != prp) {
	prp = findplace(primp->son, chbuf, With->chbufx, With->length);
	if (prp != NULL) {
	    primp = prp;
	}
	else {
	    primp = primp->parent;
	}
    }
    if (prp == NULL) {
	/*D else if debuglevel > 0 then with prp^ do begin
	    write(log,'placename: type ', ptype:1,', ');
	    wpair(log,aat.xpos,aat.ypos); writeln(log) end D*/
	marknotfound(854, chbuf, With->chbufx, With->length);
    }
    With->prim = prp;
    break;

  /* | nth primobj */
  case placename2:
    With->prim = nthprimobj(envblock->son, With->length,
			    attstack[newp + 1].lexval);
    if (With->prim == NULL) {
	markerror(857);
    }
    break;

  /* | placename "." "<Label>" suffix */
  case placename3:
    if (With->prim != NULL) {
	if (attstack[newp + 3].lexval != XEMPTY) {
	    With1 = &attstack[newp + 2];
	    addsuffix(chbuf, &With1->chbufx, &With1->length,
		      attstack[newp + 3].xval);
	}
	/*D if debuglevel > 0 then begin
	   write(log,'Searching:'); printobject(prim);
	   with attstack^[newp+2] do snapname(chbuf,chbufx,length);
	   writeln(log) end; D*/
	primp = findplace(With->prim->son, chbuf, attstack[newp + 2].chbufx,
			  attstack[newp + 2].length);
	if (primp == NULL) {
	    /*D else if debuglevel > 0 then begin
	       write(log,'placename:'); printobject(primp) end D*/
	    marknotfound(855, chbuf, attstack[newp + 2].chbufx,
			 attstack[newp + 2].length);
	}
	With->prim = primp;
    }
    break;

  /* | placename "." nth primobj */
  case placename4:
    if (With->prim != NULL) {
	With->prim = nthprimobj(With->prim->son, attstack[newp + 2].length,
				attstack[newp + 3].lexval);
	if (With->prim == NULL) {
	    markerror(857);
	}
    }
    break;

  /* nth = ncount "th" */
  /* | ncount "th" "last" */
  /* | "last" */
  case nth1:
    if (With->xval <= 0.0) {
	markerror(856);
    }
    else {
	With->length = (long)floor(With->xval + 0.5);
    }
    break;

  case nth2:
    if (With->xval <= 0.0) {
	markerror(856);
    }
    else {
	With->length = -(long)floor(With->xval + 0.5);
    }
    break;

  case nth3:
    With->length = 0;
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
    With->lexval = XBLOCK;
    break;

  /* ncount = "<float>" */
  case ncount1:
    break;

  /* | "`" expression "'" */
  /* | "lbrace" expression "rbrace" */
  case ncount2:
  case ncount3:
    With->xval = attstack[newp + 1].xval;
    break;

  /* logprod = logval */
  case logprod1:
    break;

  /* | logprod "&&" logval */
  case logprod2:
    if (With->xval == 0.0 || attstack[newp + 2].xval == 0.0) {
	With->xval = 0.0;
    }
    else {
	With->xval = 1.0;
    }
    break;

  /* logval = lcompare */
  case logval1:
    if (With->lexval == XLstring) {
	markerror(869);
	With->lexval = XLfloat;
	deletestringbox(&With->prim);
    }
    break;

  /* | stringexpr "<" stringexpr */
  case logval2:
    i = cmpstring(With->prim, attstack[newp + 2].prim);
    if (i < 0) {
	With->xval = 1.0;
    }
    else {
	With->xval = 0.0;
    }
    With->lexval = XLfloat;
    deletestringbox(&attstack[newp + 2].prim);
    deletestringbox(&With->prim);
    break;

  /* | expression "<" expression */
  case logval3:
    if (With->xval < attstack[newp + 2].xval) {
	With->xval = 1.0;
    }
    else {
	With->xval = 0.0;
    }
    break;

  /* lcompare = expression */
  /* | stringexpr */
  case lcompare1:
  case lcompare2:
    break;

  /* | lcompare "<compare>" expression */
  /* "<" "==" "!=" ">=" "<=" ">" */
  case lcompare3:
    if (With->lexval == XLstring) {
	markerror(869);
	bswitch = false;
	deletestringbox(&With->prim);
    }
    else {
	switch (attstack[newp + 1].lexval - XLcompare) {

	case 0:
	  bswitch = (With->xval < attstack[newp + 2].xval);
	  break;

	case 1:
	  bswitch = (With->xval == attstack[newp + 2].xval);
	  break;

	case 2:
	  bswitch = (With->xval != attstack[newp + 2].xval);
	  break;

	case 3:
	  bswitch = (With->xval >= attstack[newp + 2].xval);
	  break;

	case 4:
	  bswitch = (With->xval <= attstack[newp + 2].xval);
	  break;

	case 5:
	  bswitch = (With->xval > attstack[newp + 2].xval);
	  break;
	}
    }
    if (bswitch) {
	With->xval = 1.0;
    }
    else {
	With->xval = 0.0;
    }
    break;

  /* | lcompare "<compare>" stringexpr */
  case lcompare4:
    if (With->lexval != XLstring) {
	markerror(869);
	bswitch = false;
    }
    else {
	i = cmpstring(With->prim, attstack[newp + 2].prim);
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
	deletestringbox(&With->prim);
    }
    if (bswitch) {
	With->xval = 1.0;
    }
    else {
	With->xval = 0.0;
    }
    With->lexval = XLfloat;
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
  /* | "(" systemcmd ")" */
  case primary1:
    if (envblock != NULL) {
	eb = findenv(envblock);
	With->xval = eb->Upr.Ublock.env[With->lexval - XLenvvar - 1];
    }
    break;

  /* | "<name>" suffix */
  case primary2:
    if (attstack[newp + 1].lexval != XEMPTY) {
	addsuffix(chbuf, &With->chbufx, &With->length,
		  attstack[newp + 1].xval);
    }
    namptr = glfindname(envblock, chbuf, With->chbufx, With->length, &lastvar,
			&k);
    if (namptr == NULL) {
	With->xval = 0.0;
    }
    else {
	With->xval = namptr->val;
    }
    break;

  /* | "<float>" */
  case primary3:
    break;

  /* | "(" logexpr ")" */
  case primary4:
    With->xval = attstack[newp + 1].xval;
    break;

  case primary5:                                              /* | place ".x" */
    break;

  case primary6:                                              /* | place ".y" */
    With->xval = With->yval;
    break;

  /* | placename "<param>" */
  case primary7:
    if (With->prim != NULL) {
	switch (attstack[newp + 1].lexval) {

	case XLheight:
	  With->xval = pheight(With->prim);
	  break;

	case XLwidth:
	  With->xval = pwidth(With->prim);
	  break;

	case XLradius:
	case XLdiameter:
	  With2 = With->prim;
	  if (With2->ptype == XLcircle) {
	      With->xval = With2->Upr.Ucircle.radius;
	  }
	  else if (With2->ptype == XLarc) {
	      With->xval = With2->Upr.Uline.aradius;
	  }
	  else {
	      markerror(858);
	  }
	  if (attstack[newp + 1].lexval == XLdiameter) {
	      With->xval *= 2.0;
	  }
	  break;

	case XLthickness:
	  With2 = With->prim;
	  if (With2->ptype == XLarc || With2->ptype == XLspline ||
	       With2->ptype == XLarrow || With2->ptype == XLline ||
	       With2->ptype == XLcircle || With2->ptype == XLellipse ||
	       With2->ptype == XLbox) {
	      With->xval = With2->lthick;
	  }
	  else {
	      markerror(858);
	  }
	  break;

	case XLlength:
	  With2 = With->prim;
	  if (With2->ptype == XLspline || With2->ptype == XLmove ||
	       With2->ptype == XLarrow || With2->ptype == XLline) {
	      primp = With->prim;
	      while (primp->son != NULL) {
		  primp = primp->son;
	      }
	      r = fabs(primp->Upr.Uline.endpos.xpos - With2->aat.xpos);
	      s = fabs(primp->Upr.Uline.endpos.ypos - With2->aat.ypos);
	      if (r == 0.0) {
		  With->xval = s;
	      }
	      else if (s == 0.0) {
		  With->xval = r;
	      }
	      else {
		  With->xval = sqrt(r * r + s * s);
	      }
	  }
	  else {
	      markerror(858);
	  }
	  break;
	}
    }
    break;

  /* | "rand" "(" ")" random number between 0 and 1 */
  case primary8:
    With->xval = (double)random() / randmax;
    break;

  /* | "rand" "(" expression ")" */
  case primary9:
    srandom((long)floor(attstack[newp + 2].xval + 0.5));
    With->xval = (double)random() / randmax;
    break;

  /* | "<func1>" "(" expression ")" */
  case primary10:
    switch (With->lexval) {

    case XLabs:
      With->xval = fabs(attstack[newp + 2].xval);
      break;

    case XLacos:
      t = attstack[newp + 2].xval;
      if (fabs(t) > 1.0) {
	  markerror(868);
      }
      else {
	  With->xval = datan(sqrt(1 - t * t), t);
      }
      break;

    case XLasin:
      t = attstack[newp + 2].xval;
      if (fabs(t) > 1.0) {
	  markerror(868);
      }
      else {
	  With->xval = datan(t, sqrt(1 - t * t));
      }
      break;

    case XLcos:
      With->xval = cos(attstack[newp + 2].xval);
      break;

    case XLexp:
      With->xval = exp(log(10.0) * attstack[newp + 2].xval);
      break;

    case XLexpe:
      With->xval = exp(attstack[newp + 2].xval);
      break;

    case XLint:
      With->xval = (long)attstack[newp + 2].xval;
      break;

    case XLlog:
      if (attstack[newp + 2].xval <= 0.0) {
	  markerror(867);
      }
      else {
	  With->xval = log(attstack[newp + 2].xval) / log(10.0);
      }
      break;

    case XLloge:
      if (attstack[newp + 2].xval <= 0.0) {
	  markerror(867);
      }
      else {
	  With->xval = log(attstack[newp + 2].xval);
      }
      break;

    case XLsign:
      if (attstack[newp + 2].xval >= 0.0) {
	  With->xval = 1.0;
      }
      else {
	  With->xval = -1.0;
      }
      break;

    case XLsin:
      With->xval = sin(attstack[newp + 2].xval);
      break;

    case XLsqrt:
      if (attstack[newp + 2].xval < 0.0) {
	  markerror(867);
      }
      else {
	  With->xval = sqrt(attstack[newp + 2].xval);
      }
      break;

    case XLtan:
      t = cos(attstack[newp + 2].xval);
      if (t == 0.0) {
	  markerror(868);
	  With->xval = distmax;
      }
      else {
	  With->xval = sin(attstack[newp + 2].xval) / t;
      }
      break;

    case XLfloor:
      With->xval = Floor(attstack[newp + 2].xval);
      break;
    }
    break;

  /* | "<func2>" "(" expression "," expression ")" */
  case primary11:
    switch (With->lexval) {

    case XLatan2:
      With->xval = datan(attstack[newp + 2].xval, attstack[newp + 4].xval);
      break;

    case XLmax:
      With->xval = Max(attstack[newp + 2].xval, attstack[newp + 4].xval);
      break;

    case XLmin:
      With->xval = Min(attstack[newp + 2].xval, attstack[newp + 4].xval);
      break;

    case XLpmod:
      t = attstack[newp + 2].xval;
      s = attstack[newp + 4].xval;
      if (s == 0) {
	  markerror(871);
      }
      else {
	  With->xval = t - s * Floor(t / s);
	  if (With->xval < 0) {
	      With->xval += fabs(s);
	  }
      }
      break;
    }
    break;

  /* | "(" assignlist ")" */
  case primary12:
    With->xval = attstack[newp + 1].xval;
    break;

  /* | "(" systemcmd ")" */
  case primary13:
    With->xval = attstack[newp + 1].xval;

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
{ printf("\n ..controls ");
}


void wrand(void)
{ printf(" and ");
}


void ddot(void)
{ printf("\n ..");
}


void ddash(void)
{ printf("\n --");
}


void space(void)
{ putchar(' ');
}


void quote(void)
{ putchar('"');
}


void getlinshade(primitive *n, primitive **tn, nametype **ss,
			nametype **so, double *fillval, boolean *hshade)
{ primitive *With;

  *tn = n;
  *ss = NULL;
  *so = NULL;
  *fillval = -1.0;
  while (*tn != NULL) {
      With = *tn;
      if (With->outlinep != NULL) {
	  *so = With->outlinep;
      }
      *tn = (*tn)->son;
  }
  *tn = n;
  if (*hshade) {
      *hshade = false;
  }
  else {
      while (n != NULL) {
	  With = n;
	  if (With->shadedp != NULL) {
	      *ss = With->shadedp;
	  }
	  if (With->Upr.Uline.lfill >= 0.0 && With->Upr.Uline.lfill <= 1.0) {
	      *fillval = With->Upr.Uline.lfill;
	  }
	  *tn = n;
	  n = n->son;
      }
  }
  if (*ss != NULL || *fillval >= 0.0) {
      *hshade = true;
  }
}


/* Arrowhead location (lex value) and number */
int ahlex(int atyp)
{ return (atyp >> 3);
}


int ahnum(int atyp)
{ return (atyp & 7);
}


int pahlex(int atyp, int alex)
{ return ((atyp & 7) + alex * 8);
}


int pahnum(int atyp, int anum)
{ return ((atyp >> 3) * 8 + (anum & 7));                      /* 0 < anum < 7 */
}


void wfloat(FILE **iou, double y)
{ /* var iou: text; y: real */
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
{ /* var iou: text; x,y: real */
  putc('(', *iou);
  wfloat(iou, x);
  putc(',', *iou);
  wfloat(iou, y);
  putc(')', *iou);
}


void wcoord(FILE **iou, double x, double y)
{ /* var iou: text; x,y: real */
  putc('(', *iou);
  wfloat(iou, x / fsc);
  putc(',', *iou);
  wfloat(iou, y / fsc);
  putc(')', *iou);
  /*DUGHM ;flush(iou) MHGUD*/
}


void wpos(postype pos)
{ /* pos: postype */
  wcoord(&output, pos.xpos, pos.ypos);
}


void wstring(FILE **iou, nametype *p)
{ /* var iou: text; p: strptr */
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
  if (p == NULL) {
      return;
  }
  if (p->segmnt != NULL) {
      FORLIM = p->seginx + p->len;
      for (i = p->seginx; i < FORLIM; i++) {
	  putc(p->segmnt[i], *iou);
      }
  }
}


void wbrace(double x)
{ putchar('{');
  wfloat(&output, x);
  putchar('}');
}


boolean testjust(int n, int flag)
{ boolean Result;

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
{ /* tp: strptr; var A,B,L,R: boolean */
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
{ /* n: integer): integer */
  /* if ((n div 16) mod 2) <> 0 then lspec := XLsolid
  else */
  return ((n & 7) + XLlinetype);
}


void getlinespec(primitive *nd, int *lsp, primitive **lastnd)
{ /* nd: primitivep;
     var lsp: integer; var lastnd: primitivep */
  primitive *tn;

  tn = nd;
  if (nd->ptype == XLarc || nd->ptype == XLarrow || nd->ptype == XLline ||
       nd->ptype == XLspline) {
      while (tn->son != NULL) {
	  tn = tn->son;
      }
  }
  *lastnd = tn;
  *lsp = lspec(tn->spec);
}


primitive *findenv(primitive *p)
{ /* p: primitivep ): primitivep; external */
  primitive *q;

  q = NULL;
  while (p != q) {
      if (p->ptype != XBLOCK) {
	  p = p->parent;
      }
      else if (p->Upr.Ublock.env == NULL) {
	  p = p->parent;
      }
      else {
	  q = p;
      }
  }
  /*D if debuglevel > 0 then begin
     if p = nil then writeln(log,'findenv: p=nil')
     else if p^.env = nil then writeln(log,'findenv: p^.env=nil');
     flush(log) end; D*/
  return p;
}


double venv(primitive *p, int ind)
{ double v;

  v = 0.0;
  if (ind <= XLenvvar || ind > XLlastenv) {
      return v;
  }
  p = findenv(p);
  if (p != NULL) {
      v = p->Upr.Ublock.env[ind - XLenvvar - 1];
  }
  return v;
}


double qenv(primitive *p, int ind, double localval)
{ double noval;

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
  if (localval != noval) {
      return localval;
  }
  else {
      return (venv(p, ind));
  }
}


/* orig + mat(cs) * [x,y] */
postype affine(double x, double y, postype orig, postype cs)
{ postype tpos;

  tpos.xpos = orig.xpos + cs.xpos * x - cs.ypos * y;
  tpos.ypos = orig.ypos + cs.ypos * x + cs.xpos * y;
  return tpos;
}


postype affang(postype point, postype shaft)
{ double lgth;
  postype tpos;

  lgth = linlen(shaft.xpos - point.xpos, shaft.ypos - point.ypos);
  if (lgth == 0.0) {
      tpos.xpos = 1.0;
      tpos.ypos = 0.0;
  }
  else {
      tpos.xpos = (point.xpos - shaft.xpos) / lgth;
      tpos.ypos = (point.ypos - shaft.ypos) / lgth;
  }
  return tpos;
}


double posangle(postype V, postype C)
{ return (datan(V.ypos - C.ypos, V.xpos - C.xpos));
}


void initnesw(void)
{ south = distmax;
  north = -south;
  west = south;
  east = -west;
}


void neswstring(primitive *pmp, double ht, double wd)
{ boolean A, B, L, R;
  double x, y, offst;

  if (pmp == NULL) {
      return;
  }
  checkjust(pmp->textp, &A, &B, &L, &R);
  offst = venv(pmp, XLtextoffset);
  y = pmp->aat.ypos;
  if (A) {
      y += ht * 0.5 + offst;
  }
  else if (B) {
      y += -ht * 0.5 - offst;
  }
  if (drawmode == SVG) {
      offst = pmp->Upr.Ubox.boxheight / 3;
  }
  x = pmp->aat.xpos;
  if (R) {
      x += -wd * 0.5 - offst;
  }
  else if (L) {
      x += wd * 0.5 + offst;
  }
  north = Max(north, y + ht * 0.5);
  south = Min(south, y - ht * 0.5);
  west = Min(west, x - wd * 0.5);
  east = Max(east, x + wd * 0.5);
  /*D; if debuglevel>0 then begin
     write(log,' neswstring:');
     wlogfl('aat.xpos',aat.xpos,0); wlogfl('x',x,0);
     wlogfl('east',east,0); wlogfl('west',west,0); wlogfl('wd',wd,1)
     end D*/
}


void neswline(primitive *pmp)
{ double aht, awd;
  postype cs, cc, cd;
  int TEMP;

  if (pmp == NULL) {
      return;
  }
  west = Min(west, Min(pmp->aat.xpos, pmp->Upr.Uline.endpos.xpos));
  east = Max(east, Max(pmp->aat.xpos, pmp->Upr.Uline.endpos.xpos));
  south = Min(south, Min(pmp->aat.ypos, pmp->Upr.Uline.endpos.ypos));
  north = Max(north, Max(pmp->aat.ypos, pmp->Upr.Uline.endpos.ypos));
  TEMP = ahlex(pmp->Upr.Uline.atype);
  if (TEMP == XLEFTHEAD || TEMP == XDOUBLEHEAD) {
      cs = affang(pmp->Upr.Uline.endpos, pmp->aat);
      awd = qenv(pmp, XLarrowht, pmp->Upr.Uline.width);
      aht = qenv(pmp, XLarrowwid, pmp->Upr.Uline.height);
      cc = affine(aht, awd / 2, pmp->aat, cs);
      cd = affine(aht, awd / -2, pmp->aat, cs);
      west = Min(west, Min(cc.xpos, cd.xpos));
      east = Max(east, Max(cc.xpos, cd.xpos));
      south = Min(south, Min(cc.ypos, cd.ypos));
      north = Max(north, Max(cc.ypos, cd.ypos));
  }
  TEMP = ahlex(pmp->Upr.Uline.atype);
  if (!(TEMP == XRIGHTHEAD || TEMP == XDOUBLEHEAD)) {
      return;
  }
  cs = affang(pmp->aat, pmp->Upr.Uline.endpos);
  awd = qenv(pmp, XLarrowht, pmp->Upr.Uline.width);
  aht = qenv(pmp, XLarrowwid, pmp->Upr.Uline.height);
  cc = affine(aht, awd / 2, pmp->Upr.Uline.endpos, cs);
  cd = affine(aht, awd / -2, pmp->Upr.Uline.endpos, cs);
  west = Min(west, Min(cc.xpos, cd.xpos));
  east = Max(east, Max(cc.xpos, cd.xpos));
  south = Min(south, Min(cc.ypos, cd.ypos));
  north = Max(north, Max(cc.ypos, cd.ypos));
}


boolean inarc(double strt, double fin, double ang, double arcang)
{ boolean inarctmp;

  /*D if debuglevel > 0 then begin
     write(log,'Inarc strt,fin='); wfloat(log,strt*180.0/pi);
        write(log,' ' ); wfloat(log,fin*180.0/pi);
     write(log,' ang,arcang='); wfloat(log,ang*180.0/pi);
        write(log,' ' ); wfloat(log,arcang*180.0/pi) end; D*/
  if (arcang >= 0.0) {
      while (fin < strt) {
	  fin += 2.0 * pi;
      }
      while (ang < strt) {
	  ang += 2.0 * pi;
      }
      if (ang <= fin) {
	  inarctmp = true;
      }
      else {
	  inarctmp = false;
      }
      return inarctmp;
  }
  while (fin > strt) {
      fin -= 2.0 * pi;
  }
  while (ang > strt) {
      ang -= 2.0 * pi;
  }
  if (ang >= fin) {
      inarctmp = true;
  }
  else {
      inarctmp = false;
  }
  return inarctmp;
  /*D; if debuglevel > 0 then writeln(log,' Inarc =',inarctmp) D*/
}


void nesw(primitive *ptmp)
{ /* ptmp: primitivep */
  double hight, wdth, sang, eang;

  if (ptmp == NULL) {
      return;
  }
  switch (ptmp->ptype) {

  case XLbox:
  case XLstring:
    hight = ptmp->Upr.Ubox.boxheight;
    wdth = ptmp->Upr.Ubox.boxwidth;
    break;

  case XBLOCK:
    hight = ptmp->Upr.Ublock.blockheight;
    wdth = ptmp->Upr.Ublock.blockwidth;
    break;

  case XLcircle:
    hight = 2.0 * ptmp->Upr.Ucircle.radius;
    wdth = hight;
    break;

  case XLellipse:
    hight = ptmp->Upr.Uellipse.elheight;
    wdth = ptmp->Upr.Uellipse.elwidth;
    break;
  }
  switch (ptmp->ptype) {

  case XLbox:
  case XBLOCK:
  case XLcircle:
  case XLellipse:
    north = Max(north, ptmp->aat.ypos + hight * 0.5);
    south = Min(south, ptmp->aat.ypos - hight * 0.5);
    west = Min(west, ptmp->aat.xpos - wdth * 0.5);
    east = Max(east, ptmp->aat.xpos + wdth * 0.5);
    break;

  case XLstring:
    neswstring(ptmp, hight, wdth);
    break;

  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    neswline(ptmp);
    break;

  case XLaTeX:
  case XLabel:
    /* blank case */
    break;

  case XLarc:
    sang = principal(ptmp->Upr.Uline.endpos.xpos, pi);
    eang = ptmp->Upr.Uline.endpos.xpos + ptmp->Upr.Uline.endpos.ypos;
    /*D if debuglevel > 0 then begin write(log,'(sang,eang)(deg)=');
       wpair(log,sang*180/pi,eang*180/pi); writeln(log) end; D*/
    if (inarc(sang, eang, 0.5 * pi, ptmp->Upr.Uline.endpos.ypos)) {
	north = Max(north, ptmp->aat.ypos + ptmp->Upr.Uline.aradius);
    }
    else {
	north = Max(north,
	    ptmp->aat.ypos + ptmp->Upr.Uline.aradius * Max(sin(sang), sin(eang)));
    }
    if (inarc(sang, eang, -0.5 * pi, ptmp->Upr.Uline.endpos.ypos)) {
	south = Min(south, ptmp->aat.ypos - ptmp->Upr.Uline.aradius);
    }
    else {
	south = Min(south,
	    ptmp->aat.ypos + ptmp->Upr.Uline.aradius * Min(sin(sang), sin(eang)));
    }
    if (inarc(sang, eang, pi, ptmp->Upr.Uline.endpos.ypos)) {
	west = Min(west, ptmp->aat.xpos - ptmp->Upr.Uline.aradius);
    }
    else {
	west = Min(west,
	    ptmp->aat.xpos + ptmp->Upr.Uline.aradius * Min(cos(sang), cos(eang)));
    }
    if (inarc(sang, eang, 0.0, ptmp->Upr.Uline.endpos.ypos)) {
	east = Max(east, ptmp->aat.xpos + ptmp->Upr.Uline.aradius);
    }
    else {
	east = Max(east,
	    ptmp->aat.xpos + ptmp->Upr.Uline.aradius * Max(cos(sang), cos(eang)));
    }
    break;
  }
  /*D;if debuglevel > 0 then with ptmp^ do begin
     write(log, 'nesw(',ordp(ptmp):1,') ptype=',ptype:1);
     wlogfl('W',west,0); wlogfl('S',south,0);
     wlogfl('E',east,0); wlogfl('N',north,1)
     end D*/
}


double linlen(double x, double y)
{ /* x,y: real ): real */
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
  }
  else {
      xm = fabs(y);
      ym = x / xm;
  }
  return (xm * sqrt(1.0 + ym * ym));
}


void texstacktext(primitive *np, nametype *tp)
{ nametype *tx;
  boolean A, B, L, R;
  double toff;

  if (tp == NULL) {
      return;
  }
  tx = tp->next_;
  if (tx != NULL) {
      printf("\\shortstack{");
  }
  toff = venv(np, XLtextoffset) / scale * 72;
  do {
      checkjust(tp, &A, &B, &L, &R);
      if (L) {
	  printf("\\rlap{\\hbox to ");
	  wfloat(&output, toff);
	  printf("bp{}");
      }
      else if (R) {
	  printf("\\llap{");
      }
      wstring(&output, tp);
      if (R) {
	  printf("\\hbox to ");
	  wfloat(&output, toff);
	  printf("bp{}");
      }
      if (L || R) {
	  putchar('}');
      }
      tp = tp->next_;
      if (tp != NULL) {
	  printf("\\\\%%\n");
      }
  } while (tp != NULL);
  if (tx != NULL) {
      putchar('}');
  }
}


int primdepth(primitive *ptmp)
{ int dep;

  dep = 0;
  while (ptmp != NULL) {
      dep++;
      ptmp = ptmp->son;
  }
  return dep;
}


void pprop(postype p1, postype *p2, double a, double b, double c)
{ if (c != 0.0) {
      p2->xpos = (a * p1.xpos + b * p2->xpos) / c;
      p2->ypos = (a * p1.ypos + b * p2->ypos) / c;
  }
}


void wprop(postype p1, postype p2, double a, double b, double c)
{ pprop(p1, &p2, a, b, c);                             /* Note: p2 is not var */
  wpos(p2);
}


boolean iscorner(double theta)
{ /*D if debuglevel = 2 then
    writeln(log,'iscorner(',theta*180/pi:7:4,')=',
      (abs(theta) < 0.001) or (abs(0.5*pi-abs(theta)) < 0.001)); D*/
  return (fabs(theta) < 0.001 || fabs(0.5 * pi - fabs(theta)) < 0.001);
}


int hcf(int x, int y)
{ int i, small, large;

  if (x < 0) {
      x = -x;
  }
  if (y < 0) {
      y = -y;
  }
  if (y < x) {
      small = y;
      large = x;
  }
  else {
      small = x;
      large = y;
  }
  while (small > 0) {
      i = small;
      small = large % small;
/* p2c: dpic2.p, line 494:
 * Note: Using % for possibly-negative arguments [317] */
      large = i;
  }
  if (large == 0) {
      return 1;
  }
  else {
      return large;
  }
}


int iabs(int i)
{ if (i < 0) {
      return (-i);
  }
  else {
      return i;
  }
}


void wrslope(double xp, double yp, boolean arrow)
{ int i, ix, iy;
  double r;

  /*D if debuglevel > 0 then begin
     write(log,'wrslope xp,yp: '); wpair(log,xp,yp) end; D*/
  if (xp == 0.0 && yp == 0.0) {
      xp = 1.0;
      yp = 0.0;
  }
  r = linlen(xp, yp);
  if (drawmode == Pict2e) {
      i = 1000;                                                         /*4096*/
  }
  else if (drawmode == tTeX) {
      i = 453;
  }
  else if (arrow) {
      i = 4;
  }
  else {
      i = 6;
  }
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
  if (ix == 0) {
      wbrace(fabs(yp) / fsc);
  }
  else {
      wbrace(fabs(xp) / fsc);
  }
}


boolean isthen(primitive *pr)
{ /* pr: primitivep ): boolean */
  if (pr == NULL) {
      return false;
  }
  else {
      return (((pr->spec >> 3) & 1) != 0);
  }
}


boolean firstsegment(primitive *pr)
{ /* pr: primitivep ): boolean */
  if (pr == NULL) {
      return false;
  }
  else {
      return (((pr->spec >> 3) & 1) == 0);
  }
}


boolean drawn(primitive *node, int linesp, double fill)
{ if (node == NULL) {
      return false;
  }
  else if (node->shadedp != NULL) {
      return true;
  }
  else if (linesp == XLdotted || linesp == XLdashed || linesp == XLsolid ||
	   fill >= 0.0 && fill <= 1.0) {
      return true;
  }
  else {
      return false;
  }
}


double ahoffset(double ht, double wid, double lti)
{ /* distance to P control point */
  if (wid == 0.0) {
      return 0.0;
  }
  else {
      return (lti * sqrt(ht * ht + wid * wid / 4) / wid);
  }
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
		   postype *Px, postype *Lx, postype *Rx, postype *C,
		   double *x, double *y)
{ /* arrowhead ht and wid, user units */
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
  *C = affang(shaft, point);                       /* shaft direction cosines */
  po = ahoffset(ht, wid, ltu);
  if (po > ht) {
      po = ht;
  }
  *P = affine(po, 0.0, point, *C);        /* point adjusted by line thickness */
  h = ht - ltu / 2;
  *x = h - po;
  if (ht == 0.0) {
      v = 0.0;
  }
  else {
      v = wid / 2 * *x / ht;
  }
  *R = affine(h, v, point, *C);
  *L = affine(h, -v, point, *C);
  if (*x == 0.0) {
      t = 1.0;
  }
  else {
      t = ht / *x;
  }
  Rx->xpos = point.xpos + (R->xpos - P->xpos) * t;            /* right corner */
  Rx->ypos = point.ypos + (R->ypos - P->ypos) * t;
  Lx->xpos = point.xpos + (L->xpos - P->xpos) * t;             /* left corner */
  Lx->ypos = point.ypos + (L->ypos - P->ypos) * t;
  Px->xpos = (point.xpos + Lx->xpos + Rx->xpos) / 3;      /* type 3 center pt */
  Px->ypos = (point.ypos + Lx->ypos + Rx->ypos) / 3;
  if (ht == 0.0) {
      *y = ht;
  }
  else {
      *y = ht - po + ltu * wid / ht / 4;
      /*D; if debuglevel > 0 then begin
          write(log,' dahead out: po='); wfloat(log,po);
          logpos('P',P); logpos('L',L); logpos('R',R); logpos('C',C);
          logpos('Px',Px); logpos('Lx',Lx); logpos('Rx',Rx); writeln(log);
          write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
          writeln(log) end D*/
  }
}


void popgwarc(postype Ctr, double radius, double startangle,
		     double endangle, double ccw)
{ int narcs, i;
  double c, s, cc, ss, arcangle;
  postype Q;

  if (ccw > 0 && endangle < startangle) {
      endangle += 2 * pi;
  }
  else if (ccw < 0 && endangle > startangle) {
      endangle -= 2 * pi;
  }
  narcs = (long)(1.0 + fabs(endangle - startangle) / pi);
  arcangle = (endangle - startangle) / narcs;
  c = cos(arcangle / 2);
  s = sin(arcangle / 2);
  cc = (4 - c) / 3;
  if (s != 0.0) {
      ss = (1.0 - c * cc) / s;
  }
  else {
      ss = 0.0;
  }
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
{ double lw, aa, bb, cc, s, v, d, b, t;
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
  if (radius * angle > 0) {
      *ccw = 1.0;
  }
  else {
      *ccw = -1.0;
  }
  *startarrow = (radius >= 0);
  ht = fabs(ht);
  wid = fabs(wid);
  *lwi = lth / 72 * scale;                 /* line thickness in diagram units */
  lw = Min(fabs(*lwi), Min(wid / 2, ht / 2));
  wid = Max(wid, lw);
  radius = fabs(radius);
  d = sqrt(ht * ht + wid * wid / 4);
  /* Centres of the wing arcs */
  if (d == 0) {
      Q.xpos = 1.0;
      Q.ypos = 0.0;
  }
  else {
      Q.xpos = ht / d;
      Q.ypos = *ccw * wid / 2 / d;
  }
  *Ci = affine(C.xpos - point.xpos, C.ypos - point.ypos, point, Q);
  Q.ypos = -Q.ypos;
  *Co = affine(C.xpos - point.xpos, C.ypos - point.ypos, point, Q);
  /* Outer corner */
  if (radius == 0) {
      t = 0.0;
  }
  else {
      t = Min(pi / 2, d / radius);
  }
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
  if (s < 0) {
      v = aa;
  }
  else {
      v = (sqrt(s) - bb) / 2;
  }
  *Ai = *Ao;
  pprop(C, Ai, aa - v, v, aa);
  /* Point adjusted for line thickness */
  if (d == 0) {
      *P = point;
  }
  else if (radius == 0) {
      *P = C;
  }
  else if (ht == d) {
      *P = *Ao;
  }
  else {
      b = 2 * radius * sqrt((1 - ht / d) / 2);            /* distance C to Co */
      /* Angle from Co-C to P, center C */
      Q.xpos = (b * b - lw * lw + 2 * lw * radius) / (2 * b * radius);
      if (fabs(Q.xpos) > 1) {
	  P->xpos = (Ao->xpos + Ai->xpos) / 2;
	  P->ypos = (Ao->ypos + Ai->ypos) / 2;
      }
      else {
	  Q.ypos = -*ccw * sqrt(1 - Q.xpos * Q.xpos);
	  *P = affine(radius * (Co->xpos - C.xpos) / b,
		      radius * (Co->ypos - C.ypos) / b, C, Q);
      }
  }
  /* Type 3 center and corners */
  if (radius == 0) {
      t = 0.0;
  }
  else {
      t = Min(pi / 2, ht / radius * 2 / 3);
  }
  Q.xpos = cos(t);
  Q.ypos = *ccw * sin(t);
  *Px = affine(point.xpos - C.xpos, point.ypos - C.ypos, C, Q);

  v = radius * radius;
  TEMP = Ao->xpos - Px->xpos;
  TEMP1 = Ao->ypos - Px->ypos;
  d = TEMP * TEMP + TEMP1 * TEMP1;
  if (d == 0) {
      s = sqrt(v);
  }
  else if (v / d < 0.25) {
      s = 0.0;
  }
  else {
      s = sqrt(v / d - 0.25);
  }
  Cox->xpos = (Px->xpos + Ao->xpos) / 2 - *ccw * (Ao->ypos - Px->ypos) * s;
  Cox->ypos = (Px->ypos + Ao->ypos) / 2 + *ccw * (Ao->xpos - Px->xpos) * s;

  TEMP = Ai->xpos - Px->xpos;
  TEMP1 = Ai->ypos - Px->ypos;
  d = TEMP * TEMP + TEMP1 * TEMP1;
  if (d == 0) {
      s = sqrt(v);
  }
  else if (v / d < 0.25) {
      s = 0.0;
  }
  else {
      s = sqrt(v / d - 0.25);
  }
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
{ double x, y;

  *h = qenv(n, XLarrowht, n->Upr.Uline.height);
  *w = qenv(n, XLarrowwid, n->Upr.Uline.width);
  y = ahoffset(*h, *w, lth / 72 * scale);
  if (n->Upr.Uline.aradius * n->Upr.Uline.aradius - y * y <= 0.0) {
      x = 0.0;
  }
  else {
      x = 2 * atan(y / sqrt(n->Upr.Uline.aradius * n->Upr.Uline.aradius - y * y));
  }
  if (n->Upr.Uline.endpos.ypos >= 0.0) {
      n->Upr.Uline.endpos.xpos += x;
      n->Upr.Uline.endpos.ypos -= x;
  }
  else {
      n->Upr.Uline.endpos.xpos -= x;
      n->Upr.Uline.endpos.ypos += x;
  }
}


void endarc(primitive *n, postype X0, double lth, double *h, double *w)
{ double x, y;

  *h = qenv(n, XLarrowht, n->Upr.Uline.height);
  *w = qenv(n, XLarrowwid, n->Upr.Uline.width);
  y = ahoffset(*h, *w, lth / 72 * scale);
  if (n->Upr.Uline.aradius * n->Upr.Uline.aradius - y * y <= 0.0) {
      x = 0.0;
  }
  else {
      x = 2 * atan(y / sqrt(n->Upr.Uline.aradius * n->Upr.Uline.aradius - y * y));
  }
  if (n->Upr.Uline.endpos.ypos >= 0.0) {
      n->Upr.Uline.endpos.ypos -= x;
  }
  else {
      n->Upr.Uline.endpos.ypos += x;
  }
}


postype arcstart(primitive *n)
{ postype X;

  X.xpos = n->aat.xpos + n->Upr.Uline.aradius * cos(n->Upr.Uline.endpos.xpos);
  X.ypos = n->aat.ypos + n->Upr.Uline.aradius * sin(n->Upr.Uline.endpos.xpos);
  return X;
}


postype arcend(primitive *n)
{ postype X;

  X.xpos = n->aat.xpos + n->Upr.Uline.aradius *
			 cos(n->Upr.Uline.endpos.xpos + n->Upr.Uline.endpos.ypos);
  X.ypos = n->aat.ypos + n->Upr.Uline.aradius *
			 sin(n->Upr.Uline.endpos.xpos + n->Upr.Uline.endpos.ypos);
  return X;
}


/* include xfig.h */
/* xfig.x */
/* Output routines for xfig */

#define xfigres         1200
#define dispres         80
#define points          72


void xfigprelude(void)
{ /* writeln('#FIG 3.1');
     writeln('Landscape');
     writeln('Center');
     writeln('Inches');
     writeln(xfigres:1,' 2');
     writeln('# dpic version 2015.08.31 option -x for Fig 3.1')
     */

  printf("#FIG 3.2\n");
  printf("Landscape\n");
  printf("Center\n");
  printf("Inches\n");
  printf("Letter\n");
  printf("100.00\n");
  printf("Single\n");
  printf("-2\n");
  printf("# dpic version 2015.08.31 option -x for Fig 3.2\n");
  printf("%ld 2\n", (long)xfigres);

}


void wfigpt(double x)
{ printf(" %ld", (long)floor(x / fsc * xfigres + 0.5));
}


void wfigcoord(double x, double y)
{ wfigpt(x);
  wfigpt(xfheight - y);
}


void arrowline(int atype, double wid, double ht, double lth)
{ /* arrowtype,arrowstyle: integer;
     arrowthickness,arrowwidth,arrowheight: real */
  if (ahnum(atype) == 0) {
      printf("%c0 0 ", tabch);
  }
  else {
      printf("%c1 1 ", tabch);
  }
  wfloat(&output, lth * dispres / points);
  putchar(' ');
  wfloat(&output, wid * xfigres);
  putchar(' ');
  wfloat(&output, ht * xfigres);
  putchar('\n');
}


int linstyle(int i)
{ if (i == XLsolid) {
      i = 0;
      return i;
  }
  if (i == XLdashed) {
      i = 1;
      return i;
  }
  if (i == XLdotted) {
      i = 2;
  }
  else {
      i = -1;
  }
  return i;
}


void hdrline(int object_code, int sub_type, int line_style, double lth,
		    double gfill)
{ /* first 10 values object_code .. style_val */
  printf("%d %d %d ", object_code, sub_type, linstyle(line_style));

  if (line_style == XLinvis) {
      printf("0 ");
  }
  else {
      printf("%ld ", (long)floor(lth * dispres / points + 0.5));
  }
  printf("0 -1 0 -1 ");               /* pencolor, fillcolor, depth, penstyle */
  if (gfill == -1.0) {
      printf("-1 ");                                             /* area fill */
  }
  else {
      printf("%ld ", (long)floor((1.0 - gfill) * 20 + 0.5));
  }

  /* style_val */
  if (line_style == XLdashed) {
      wfloat(&output, 5.0 / points * dispres);
  }
  else if (line_style == XLdotted) {
      wfloat(&output, 3.0 / points * dispres);
  }
  else {
      putchar('0');
  }
  putchar(' ');
}


int fwdarrow(int i)
{ if ((ahlex(i) == XRIGHTHEAD) | (ahlex(i) == XDOUBLEHEAD)) {
      return 1;
  }
  else {
      return 0;
  }
}


int bckarrow(int i)
{ if ((ahlex(i) == XLEFTHEAD) | (ahlex(i) == XDOUBLEHEAD)) {
      return 1;
  }
  else {
      return 0;
  }
}


void polyline(int object_code, int sub_type, int line_style,
		     double lth, double gfill, double lrad, int atype,
		     double lwid, double lht, int npoints)
{ hdrline(object_code, sub_type, line_style, lth, gfill);
  if (object_code == 3) {
      printf("0 ");                           /* cap_style = butt for splines */
      /* join_style = miter, cap_style = butt, radius */
  }
  else {
      printf("0 0 %ld ", (long)floor(lrad * dispres + 0.5));
  }
  printf("%d %d %d\n", fwdarrow(atype), bckarrow(atype), npoints);
  if (fwdarrow(atype) == 1) {
      arrowline(atype, lwid, lht, lth);
  }
  if (bckarrow(atype) == 1) {
      arrowline(atype, lwid, lht, lth);
  }
}


void xfigwrtext(primitive *np, nametype *tp, double bxht, double bxwid,
		       double x, double y)
{ double ydisp;
  int istr, nstr, figjust, i;
  boolean A, B, L, R;
  nametype *p;
  int FORLIM;

  if (bxht == 0.0) {
      bxht = venv(np, XLtextht);
  }
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
      if (L) {
	  figjust = 0;
      }
      else if (R) {
	  figjust = 2;
      }
      else {
	  figjust = 1;
      }
      printf("%d ", figjust);
      printf("-1 0 -1 ");                           /* color, depth, penstyle */
      printf("0 10 0.0 2");             /* font, font_size, angle, font_flags */
      wfigpt(bxht);
      if (bxwid == 0) {
	  wfigpt(tp->len * bxht * 2 / 3);
      }
      else {
	  wfigpt(bxwid);
      }
      /*D if debuglevel > 0 then writeln(log,
        ' bxht=',bxht:9:2,' bxwid=',bxwid:9:2,' length=',tp^.len:1); D*/
      if (A) {
	  ydisp = bxht / 5 + venv(np, XLtextoffset);
      }
      else if (B) {
	  ydisp = -bxht - venv(np, XLtextoffset);
      }
      else {
	  ydisp = bxht / -3;
      }
      wfigcoord(x, y + ydisp + ((nstr + 1.0) / 2 - istr) * bxht);
      putchar(' ');
      FORLIM = tp->len;
      for (i = 0; i < FORLIM; i++) {
	  if (tp->segmnt[tp->seginx + i] == bslch) {
	      putchar(bslch);
	  }
	  putchar(tp->segmnt[tp->seginx + i]);
      }
      printf("\\001\n");
      tp = tp->next_;
  }
}


void farc(int object_code, int sub_type, int line_style, double lth,
		 double gfill, int atype, double radius, double strtang,
		 double arcang, double x, double y, double lwid, double lht)
{ hdrline(object_code, sub_type, line_style, lth, gfill);
  printf("0 ");
  if (arcang < 0.0) {
      printf("0 ");
  }
  else {
      printf("1 ");
  }
  printf("%d %d ", fwdarrow(atype), bckarrow(atype));
  wfigcoord(x, y);
  wfigcoord(x + radius * cos(strtang), y + radius * sin(strtang));
  wfigcoord(x + radius * cos(strtang + arcang / 2),
	    y + radius * sin(strtang + arcang / 2));
  wfigcoord(x + radius * cos(strtang + arcang),
	    y + radius * sin(strtang + arcang));
  putchar('\n');
  if (fwdarrow(atype) == 1) {
      arrowline(atype, lwid, lht, lth);
  }
  if (bckarrow(atype) == 1) {
      arrowline(atype, lwid, lht, lth);
  }
}


void fellipse(int object_code, int sub_type, int line_style,
		     double lth, double gfill, double center_x,
		     double center_y, double radius_x, double radius_y)
{ hdrline(object_code, sub_type, line_style, lth, gfill);
  printf("1 0.0");
  wfigcoord(center_x, center_y);
  wfigpt(radius_x);
  wfigpt(radius_y);
  wfigcoord(center_x + radius_x, center_y);
  wfigcoord(center_x + radius_x, center_y);
  putchar('\n');
}


boolean rdrawn(primitive *np)
{ boolean rv, v;
  primitive *With;

  rv = false;
  while (rv == false && np != NULL) {
      With = np;
      if (With->ptype == XLbox) {
	  v = drawn(np, lspec(With->spec), With->Upr.Ubox.boxfill);
      }
      else if (With->ptype == XLcircle) {
	  v = drawn(np, lspec(With->spec), With->Upr.Ucircle.cfill);
      }
      else if (With->ptype == XLellipse) {
	  v = drawn(np, lspec(With->spec), With->Upr.Uellipse.efill);
      }
      else if (With->ptype == XLspline || With->ptype == XLarrow ||
	       With->ptype == XLline || With->ptype == XLarc) {
	  /* v := drawn(np,lspec(spec),-1.0) */
	  v = drawn(np, lspec(With->spec), With->Upr.Uline.lfill);
      }
      else {
	  v = false;
      }
      if (v || With->textp != NULL) {
	  rv = true;
	  break;
      }
      if (rdrawn(With->son)) {
	  rv = true;
      }
      else {
	  np = With->next_;
      }
  }
  return rv;
}


void xfigdraw(primitive *node)
{ int i, lsp;
  double fill;
  primitive *tn;
  int FORLIM;

  getlinespec(node, &lsp, &tn);
  if (node->lthick < 0.0) {
      node->lthick = venv(node, XLlinethick);
  }
  /*D if debuglevel > 0 then with node^ do begin
     write(log,'xfigdraw(',ordp(node):1,'): lthick=',lthick:9:2,' ');
     snaptype(log,ptype); writeln(log)
     end; D*/
  switch (node->ptype) {

  case XLarc:
    if (drawn(node, lsp, node->Upr.Uline.lfill)) {
	farc(5, 1, lsp, node->lthick, node->Upr.Uline.lfill,
	     node->Upr.Uline.atype, node->Upr.Uline.aradius,
	     node->Upr.Uline.endpos.xpos, node->Upr.Uline.endpos.ypos,
	     node->aat.xpos, node->aat.ypos, node->Upr.Uline.width,
	     node->Upr.Uline.height);
    }
    xfigwrtext(node, node->textp, 0.0, 0.0, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
  case XLspline:
    if (drawn(node, lsp, node->Upr.Uline.lfill)) {
	if (firstsegment(node)) {
	    spltot = primdepth(node);
	    if (node->ptype == XLspline) {
		polyline(3, 0, lsp, node->lthick, node->Upr.Uline.lfill, 0.0,
			 node->Upr.Uline.atype, node->Upr.Uline.width,
			 node->Upr.Uline.height, spltot + 1);
	    }
	    else {
		polyline(2, 1, lsp, node->lthick, node->Upr.Uline.lfill, 0.0,
			 node->Upr.Uline.atype, node->Upr.Uline.width,
			 node->Upr.Uline.height, spltot + 1);
	    }
	    putchar(tabch);
	    wfigcoord(node->aat.xpos, node->aat.ypos);
	}
	wfigcoord(node->Upr.Uline.endpos.xpos, node->Upr.Uline.endpos.ypos);
	if (node->son == NULL && node->ptype == XLspline) {
	    printf("\n%c 0.0", tabch);
	    FORLIM = spltot;
	    for (i = 2; i <= FORLIM; i++) {
		printf(" 1.0");
	    }
	    printf(" 0.0\n");
	}
	else if (node->son == NULL) {
	    putchar('\n');
	}
    }
    xfigwrtext(node, node->textp, 0.0, 0.0,
	       0.5 * (node->Upr.Uline.endpos.xpos + node->aat.xpos),
	       0.5 * (node->aat.ypos + node->Upr.Uline.endpos.ypos));
    break;

  case XLbox:
  case XBLOCK:
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
    if (node->ptype == XBLOCK) {
	fill = -1.0;
    }
    else {
	fill = node->Upr.Ubox.boxfill;
    }
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
	       0.5 * (node->Upr.Uline.endpos.xpos + node->aat.xpos),
	       0.5 * (node->aat.ypos + node->Upr.Uline.endpos.ypos));
    break;

  case XLellipse:
    if (drawn(node, lsp, node->Upr.Uellipse.efill)) {
	fellipse(1, 1, lsp, node->lthick, node->Upr.Uellipse.efill, node->aat.xpos,
		 node->aat.ypos, 0.5 * node->Upr.Uellipse.elwidth,
		 0.5 * node->Upr.Uellipse.elheight);
    }
    xfigwrtext(node, node->textp, 0.0, 0.0, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->Upr.Ucircle.cfill)) {
	fellipse(1, 3, lsp, node->lthick, node->Upr.Ucircle.cfill, node->aat.xpos,
		 node->aat.ypos, node->Upr.Ucircle.radius, node->Upr.Ucircle.radius);
    }
    xfigwrtext(node, node->textp, 0.0, 0.0, node->aat.xpos, node->aat.ypos);
    break;

  case XLstring:
    xfigwrtext(node, node->textp, node->Upr.Ubox.boxheight,
	       node->Upr.Ubox.boxwidth, node->aat.xpos, node->aat.ypos);
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
{ double hsize, vsize;

  /*D if debuglevel > 0 then begin
     writeln(log,' svgprelude:');
     write(log,' n='); wfloat(log,n); write(log,' s='); wfloat(log,s);
     write(log,' e='); wfloat(log,e); write(log,' w='); wfloat(log,w);
     write(log,' fsc='); wfloat(log,fsc); write(log,' lth='); wfloat(log,lth);
     writeln(log) end; D*/
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
  printf("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
  printf("\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  printf("<!-- Creator: dpic version 2015.08.31 option -v for SVG 1.1 -->\n");
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
  gslinethick = 0.8;
}


void svgpostlude(void)
{ printf("</g></svg>\n");
  /*D; if debuglevel > 0 then writeln(log,'svgpostlude done') D*/
}


void svgsetstroke(double lth)
{ if (lth == gslinethick) {
      return;
  }
  printf(" stroke-width=\"");
  wfloat(&output, lth / 72 * SVGPX);
  printf("\"\n");
}


void svgsoutline(nametype *so)
{ printf(" stroke=\"");
  wstring(&output, so);
  printf("\"\n");
}


void fillgray(double fll)
{ printf(" fill=\"");
  if (fll == 0.0) {
      printf("black");
  }
  else if (fll == 0.25) {
      printf("darkgray");
  }
  else if (fll == 0.5) {
      printf("gray");
  }
  else if (fll == 0.75) {
      printf("lightgray");
  }
  else if (fll == 1.0) {
      printf("white");
  }
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
{ double param;

  if (node->lthick >= 0.0) {
      svgsetstroke(node->lthick);
  }
  if (soutline != NULL) {
      svgsoutline(soutline);
  }
  if (node->ptype == XBLOCK) {
      lnspec = XLinvis;
  }
  switch (lnspec) {

  case XLdashed:
    param = qenv(node, XLdashwid, node->lparam) / fsc;
    printf(" stroke-dasharray=\"");
    wfloat(&output, param * 7 / 6);
    putchar(',');
    wfloat(&output, param * 5 / 6);
    printf("\"\n");
    break;

  case XLdotted:
    /* if lparam = mdistmax then param := 5/72*scale
       else param := lparam; */
    if (node->lparam != mdistmax) {
	param = node->lparam;
    }
    else {
	param = qenv(node, XLlinethick, node->lthick) / 72 * 5 * scale;
    }
    printf(" stroke-linecap=\"round\"");
    printf(" stroke-dasharray=\"0.5,");
    wfloat(&output, param / fsc);
    printf("\"\n");
    break;

  case XLinvis:
    printf(" stroke=\"none\"");
    break;
  }
}


void svgfilloptions(primitive *node, double fill, nametype *sh,
			   int lnspec, boolean poly)
{ fill = (long)floor(fill * 1000000L + 0.5) / 1000000.0;
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
    if (poly) {
	printf("<polyline");
    }
    else {
	printf("<line");
    }
    break;

  case XLarc:
    printf("<path");
    break;
  }
  if (sh != NULL) {
      printf(" fill=\"");
      wstring(&output, sh);
      printf("\"\n");
  }
  else if (fill >= 0.0 && fill <= 1.0) {
      fillgray(fill);
  }
  else if (node->ptype == XLstring) {
      fillgray(0.0);
  }
  svglineoptions(node, lnspec);
}


void svgendpath(void)
{ printf(" />\n");
}


void svgparam(Char *p, double x)
{ printf(" %s=\"", p);
  wfloat(&output, x / fsc);
  quote();
}


void svgcoord(Char *s1, Char *s2, double x, double y)
{ svgparam(s1, x);
  /*DUGHM ;flush(output) MHGUD*/
  svgparam(s2, xfheight - y);
}


void svgwpos(postype p)
{ wfloat(&output, p.xpos / fsc);
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
{ pprop(p1, &p2, a, b, c);
  svgwpos(p2);
}


void svgwstring(nametype *p)
{ int i;
  Char c;
  boolean waswhite, iswhite;
  int FORLIM;

  if (p == NULL) {
      return;
  }
  if (p->segmnt == NULL) {
      return;
  }
  waswhite = false;
  /* kludge: precede string with a blank to add offset */
  if (p->len > 0) {
      putchar(' ');
  }
  FORLIM = p->len;
  for (i = 0; i < FORLIM; i++) {
      c = p->segmnt[p->seginx + i];
      iswhite = (c == etxch || c == nlch || c == tabch || c == ' ');
      if (!iswhite || !waswhite) {
	  putchar(c);
      }
      waswhite = iswhite;
  }
}


void svgwtext(primitive *node, nametype *tp, double x, double y)
{ int nstr;
  nametype *tx;
  boolean L, R, A, B;
  double textht, textoff, dx, dy;

  if (tp == NULL) {
      return;
  }
  tx = tp;
  nstr = 0;
  textht = venv(node, XLtextht);
  while (tx != NULL) {
      nstr++;
      tx = tx->next_;
  }
  /* boxheight = nstrings * textht */
  if (node->ptype == XLstring && nstr > 0) {
      textht = node->Upr.Ubox.boxheight / nstr;
  }
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
      if (node->ptype != XLstring) {
	  printf(" stroke-width=\"0.2pt\" fill=\"black\"");
      }
      else {
	  printf(" font-size=\"");
	  wfloat(&output, textht / scale * 72);
	  printf("pt\"");
	  svgfilloptions(node, node->Upr.Ubox.boxfill, node->shadedp,
			 lspec(node->spec), false);
	  if (node->lthick < 0) {
	      svgsetstroke(0.2);
	  }
      }
      if (L) {
	  printf(" text-anchor=\"start\"");
      }
      else if (R) {
	  printf(" text-anchor=\"end\"");
      }
      if (L) {
	  dx = textoff - textht * 2 / 5;
      }
      else if (R) {
	  dx = textht / 7 - textoff;
      }
      else {
	  dx = textht / -7;
      }
      dy = textht / -20;
      if (A) {
	  dy += textoff + textht / 2;
      }
      else if (B) {
	  dy += textht * (1 - TEXTRATIO) / 2 - textoff;
      }
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
{ postype p;

  p.xpos = Q.xpos * V.xpos - Q.ypos * V.ypos;
  p.ypos = Q.ypos * V.xpos + Q.xpos * V.ypos;
  return p;
}


void svgwarc(postype E, double r, double angle, double ccw)
{ printf(" A ");
  wfloat(&output, fabs(r) / fsc);
  space();
  wfloat(&output, fabs(r) / fsc);
  printf(" 0 ");
  if (fabs(angle) > pi) {
      putchar('1');
  }
  else {
      putchar('0');
  }
  if (ccw > 0) {
      printf(" 0 ");
  }
  else {
      printf(" 1 ");
  }
  svgwpos(E);
  if (angle == 0) {
      putchar('\n');
  }
}


void svgarcahead(postype C, int atyp, postype *point, nametype *sou,
			double ht, double wid, double lth, double radius,
			double angle)
{ postype P, Q, Co, Ci, Px, Cox, Cix, Ao, Ai;
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
  }
  else {
      printf(" fill=\"black\"\n");
  }
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
  }
  else if (atyp == 3 && lwi < (wid - lwi) / 2) {
      printf(" d=\"M ");
      svgwpos(Px);
      putchar('\n');
      svgwarc(Ai, radius, 0.0, ccw);
      svgwarc(*point, radius, 0.0, -ccw);
      svgwarc(Ao, radius, 0.0, ccw);
      svgwarc(Px, radius, 1.0, -ccw);
  }
  else {
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
{ postype P, L, R, Px, Lx, Rx, Q;
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
      logpos('P',P); logpos('L',L); logpos('R',R); logpos('C',Q);
      logpos('Px',Px); logpos('Lx',Lx); logpos('Rx',Rx);
      writeln(log);
      write(log,' x='); wfloat(log,x); write(log,' y='); wfloat(log,y);
      writeln(log) end; D*/
  if (atyp == 0) {
      printf("<polyline");
      if (lth > 0.0) {
	  svgsetstroke(lth);
      }
      if (sou != NULL) {
	  svgsoutline(sou);
      }
      printf(" points=\"");
      svgwprop(P, L, x - y, y, x);
      space();
      svgwpos(P);
      putchar('\n');
      svgwprop(P, R, x - y, y, x);
  }
  else if (atyp == 3) {
      printf("<polygon stroke-width=\"0\"");
      if (sou != NULL) {
	  svgsoutline(sou);
	  printf(" fill=\"");
	  wstring(&output, sou);
	  printf("\"\n");
      }
      else if (fill >= 0.0 && fill <= 1.0) {
	  fillgray(fill);
      }
      else {
	  printf(" fill=\"black\"\n");
      }
      printf(" points=\"");
      svgwpos(Px);
      space();
      svgwpos(Lx);
      putchar('\n');
      svgwpos(*point);
      space();
      svgwpos(Rx);
  }
  else {
      printf("<polygon stroke-width=\"0\"");
      if (sou != NULL) {
	  svgsoutline(sou);
	  printf(" fill=\"");
	  wstring(&output, sou);
	  printf("\"\n");
      }
      else if (fill >= 0.0 && fill <= 1.0) {
	  fillgray(fill);
      }
      else {
	  printf(" fill=\"black\"\n");
      }
      printf(" points=\"");
      svgwpos(Lx);
      space();
      svgwpos(*point);
      putchar('\n');
      svgwpos(Rx);
  }
  quote();
  svgendpath();
  *point = P;
}


void svgsplinesegment(primitive *tv, int splc, int splt)
{ if (tv == NULL) {
      return;
  }
  if (splt == 1) {
      printf("<line x1=\"");
      wfloat(&output, tv->aat.xpos / fsc);
      printf("\" y1=\"");
      wfloat(&output, tv->aat.ypos / fsc);
      printf("\" x2=\"");
      wfloat(&output, tv->Upr.Uline.endpos.xpos / fsc);
      printf("\" y2=\"");
      wfloat(&output, tv->Upr.Uline.endpos.ypos / fsc);
      svgendpath();
      return;
  }
  if (tv->Upr.Uline.aradius == mdistmax) {
      if (splc == splt) {  /* 1st seg */
	  printf(" d=\"M ");
	  svgwpos(tv->aat);
	  printf("\n C ");
	  svgwprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	  space();
	  svgwprop(tv->aat, tv->Upr.Uline.endpos, 2.0, 1.0, 3.0);
	  space();
	  svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  space();
	  svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  putchar('\n');
	  return;
      }
      if (splc > 1) {  /* interior segment */
	  svgwprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	  space();
	  svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  space();
	  svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  putchar('\n');
	  return;
      }
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
      space();
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      space();
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 2.0, 3.0);
      space();
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
      putchar('\n');
      svgwpos(tv->Upr.Uline.endpos);
      quote();
      svgendpath();
      return;
  }
  if (splc == splt && splc > 1) {
      printf(" d=\"M ");
      svgwpos(tv->aat);
      printf("\n C ");
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	       tv->Upr.Uline.aradius, 1.0);
      space();
      return;
  }
  if (splc > 1) {
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 0.5 + tv->Upr.Uline.aradius / 2,
	       0.5 - tv->Upr.Uline.aradius / 2, 1.0);
      space();
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      space();
      svgwprop(tv->aat, tv->Upr.Uline.endpos, 0.5 - tv->Upr.Uline.aradius / 2,
	       0.5 + tv->Upr.Uline.aradius / 2, 1.0);
      space();
      return;
  }
  svgwprop(tv->aat, tv->Upr.Uline.endpos, tv->Upr.Uline.aradius,
	   1 - tv->Upr.Uline.aradius, 1.0);
  space();
  svgwpos(tv->Upr.Uline.endpos);
  quote();
  svgendpath();

  /* last segment */
}


void svgdraw(primitive *node)
{ /* node is always <> nil */
  int lsp;
  postype X1, X2;
  primitive *tn, *tx;
  double h, w, lth;
  int TEMP;
  primitive *With1;

  getlinespec(node, &lsp, &tn);
  sshade = node->shadedp;
  soutline = node->outlinep;
  lth = qenv(node, XLlinethick, node->lthick);          /* printobject(node); */
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
    initnesw();
    nesw(node);
    if (drawn(node, lsp, node->Upr.Ubox.boxfill)) {
	svgfilloptions(node, node->Upr.Ubox.boxfill, node->shadedp, lsp, false);
	svgcoord("x", "y", node->aat.xpos - fabs(node->Upr.Ubox.boxwidth) / 2,
		 node->aat.ypos + fabs(node->Upr.Ubox.boxheight) / 2);
	if (node->Upr.Ubox.boxradius > 0.0) {
	    svgparam("rx", node->Upr.Ubox.boxradius);
	    svgparam("ry", node->Upr.Ubox.boxradius);
	}
	svgparam("width", fabs(node->Upr.Ubox.boxwidth));
	svgparam("height", fabs(node->Upr.Ubox.boxheight));
	svgendpath();
    }
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XBLOCK:
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLellipse:
    if (drawn(node, lsp, node->Upr.Uellipse.efill)) {
	svgfilloptions(node, node->Upr.Uellipse.efill, node->shadedp, lsp, false);
	svgcoord("cx", "cy", node->aat.xpos, node->aat.ypos);
	svgparam("rx", 0.5 * fabs(node->Upr.Uellipse.elwidth));
	svgparam("ry", 0.5 * fabs(node->Upr.Uellipse.elheight));
	svgendpath();
    }
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->Upr.Ucircle.cfill)) {
	svgfilloptions(node, node->Upr.Ucircle.cfill, node->shadedp, lsp, false);
	svgcoord("cx", "cy", node->aat.xpos, node->aat.ypos);
	svgparam("r", fabs(node->Upr.Ucircle.radius));
	svgendpath();
    }
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (firstsegment(node)) {
	snode = node;
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    printf("<polyline stroke-width=\"0\"");
	    if (soutline != NULL) {
		svgsoutline(soutline);
	    }
	    if (vfill >= 0 && vfill <= 1) {
		fillgray(vfill);
	    }
	    else {
		printf(" fill=\"");
		wstring(&output, sshade);
		printf("\"\n");
	    }
	    printf(" points=\"");
	    svgwpos(node->aat);
	    tx = node;
	    while (tx != NULL) {
		putchar('\n');
		svgwpos(tx->Upr.Uline.endpos);
		tx = tx->son;
	    }
	    printf("\" />\n");
	    vfill = -1.0;
	    sshade = NULL;
	}
	if (lsp != XLinvis) {
	    lth = qenv(tn, XLlinethick, tn->lthick);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		svgahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos, soutline,
			 qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth, vfill);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		svgahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, soutline,
			 qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth, vfill);
	    }
	    if (node->son == NULL) {
		svgfilloptions(tn, vfill, sshade, lsp, false);
		svgcoord("x1", "y1", node->aat.xpos, node->aat.ypos);
		svgcoord("x2", "y2", node->Upr.Uline.endpos.xpos,
			 node->Upr.Uline.endpos.ypos);
		svgendpath();
	    }
	    else {
		svgfilloptions(tn, vfill, sshade, lsp, true);
		printf(" points=\"");
		svgwpos(node->aat);
		space();
	    }
	}
    }
    else if (lsp != XLinvis) {
	svgwpos(node->aat);
	putchar('\n');
	if (node->son == NULL) {
	    svgwpos(node->Upr.Uline.endpos);
	    quote();
	    svgendpath();
	}
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		svgwtext(node, With1->textp,
		  0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
		  0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLspline:
    if (firstsegment(node)) {
	snode = node;
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	node->lthick = tn->lthick;
	node->lparam = tn->lparam;
	if (bfill) {
	    printf("<path stroke-width=\"0\"");
	    if (soutline != NULL) {
		svgsoutline(soutline);
	    }
	    if (vfill >= 0 && vfill <= 1) {
		fillgray(vfill);
	    }
	    else {
		printf(" fill=\"");
		wstring(&output, sshade);
		printf("\"\n");
	    }
	    spltot = primdepth(node);
	    splcount = spltot;
	    tx = node;
	    while (tx != NULL) {
		svgsplinesegment(tx, splcount, spltot);
		splcount--;
		tx = tx->son;
	    }
	    vfill = -1.0;
	    sshade = NULL;
	}
	if (lsp != XLinvis) {
	    lth = qenv(tn, XLlinethick, tn->lthick);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		svgahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos, soutline,
			 qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 node->Upr.Uline.lfill);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		svgahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, soutline,
			 qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 node->Upr.Uline.lfill);
	    }
	    spltot = primdepth(node);
	    splcount = spltot;
	    svgfilloptions(node, vfill, sshade, lsp, false);
	}
    }
    if (lsp != XLinvis) {
	svgsplinesegment(node, splcount, spltot);
    }
    splcount--;
    break;

  case XLarc:
    X1 = arcstart(node);
    X2 = arcend(node);
    getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
    if (bfill) {
	printf("<path stroke-width=\"0\"");
	if (vfill >= 0 && vfill <= 1) {
	    fillgray(vfill);
	}
	else {
	    printf(" fill=\"");
	    wstring(&output, sshade);
	    printf("\"\n");
	}
	printf(" d=\"M ");
	svgwpos(X1);
	putchar('\n');
	svgwarc(X2, node->Upr.Uline.aradius, node->Upr.Uline.endpos.ypos,
		node->Upr.Uline.endpos.ypos);
	quote();
	svgendpath();
	vfill = -1.0;
	sshade = NULL;
    }
    if (lsp != XLinvis) {
	lth = qenv(tn, XLlinethick, tn->lthick);
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	    startarc(node, X1, lth, &h, &w);
	    svgarcahead(node->aat, ahnum(node->Upr.Uline.atype), &X1, soutline,
			h, w, lth, fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos);
	}
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	    endarc(node, X2, lth, &h, &w);
	    svgarcahead(node->aat, ahnum(node->Upr.Uline.atype), &X2, soutline,
			h, w, lth, -fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos);
	}
	printf("<path");
	svglineoptions(node, lsp);
	printf(" d=\"M ");
	svgwpos(X1);
	putchar('\n');
	svgwarc(X2, node->Upr.Uline.aradius, node->Upr.Uline.endpos.ypos,
		node->Upr.Uline.endpos.ypos);
	quote();
	svgendpath();
    }
    svgwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
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
{ printf("\\psset{unit=1in,cornersize=absolute,dimen=middle}%%\n");
  printf("\\begin{pspicture}");
  wcoord(&output, w, s);
  wcoord(&output, e, n);
  printf("%%\n");
  printf("%% dpic version 2015.08.31 option -p for PSTricks 0.93a or later\n");
}


void pstpostlude(void)
{ printf("\\end{pspicture}%%\n");
  /*D if debuglevel > 0 then writeln(log,'pstpostlude done'); D*/
}


void pstwrtext(primitive *np, nametype *tp, double x, double y)
{ boolean A, B, L, R;

  if (tp == NULL) {
      return;
  }
  if (tp->next_ != NULL) {
      printf("\\rput");
      wcoord(&output, x, y);
      putchar('{');
      texstacktext(np, tp);
      printf("}\n");
      return;
  }
  checkjust(tp, &A, &B, &L, &R);
  if (A || B || L || R) {
      printf("\\uput{");
      wfloat(&output, venv(np, XLtextoffset) / scale * 72);
      printf("bp}[");
      if (B) {
	  putchar('d');
      }
      else if (A) {
	  putchar('u');
      }
      if (L) {
	  putchar('r');
      }
      else if (R) {
	  putchar('l');
      }
      putchar(']');
  }
  else {
      printf("\\rput");
  }
  wcoord(&output, x, y);
  putchar('{');
  wstring(&output, tp);
  printf("}\n");
}


void pstlineoptions(primitive *node, int lsp, Char sep)
{ int ahn;

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
  if (node->ptype == XBLOCK) {
      lsp = XLinvis;
  }
  if (lsp == XLinvis || lsp == XLdotted || lsp == XLdashed) {      /* XLsolid,*/
      printf("%c%%\n", sep);
      printf("linestyle=");
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
	printf("none");
	break;
      }
      sep = ',';
  }
  ahn = ahnum(node->Upr.Uline.atype);
  if (((node->ptype == XLspline || node->ptype == XLline ||
	node->ptype == XLarrow) & (ahlex(node->Upr.Uline.atype) != XEMPTY)) &&
       ahn != 0 && ahn != 3) {
      printf("%c%%\n", sep);
      printf("arrowsize=");
      wfloat(&output, node->Upr.Uline.width / scale);
      printf("in 0,arrowlength=");
      if (node->Upr.Uline.width == 0.0) {
	  wfloat(&output, 0.0);
      }
      else {
	  wfloat(&output, node->Upr.Uline.height / node->Upr.Uline.width);
      }
      printf(",arrowinset=0");
      sep = ',';
  }
  if (sep == ',') {
      putchar(']');
  }
}


void pstfilloptions(primitive *node, int lsp, double a)
{ Char sep;
  double fill;

  fill = -1.0;
  switch (node->ptype) {

  case XBLOCK:
    /* blank case */
    break;

  case XLbox:
    fill = node->Upr.Ubox.boxfill;
    break;

  case XLcircle:
    fill = node->Upr.Ucircle.cfill;
    break;

  case XLellipse:
    fill = node->Upr.Uellipse.efill;
    break;

  case XLarc:
  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    fill = node->Upr.Uline.lfill;
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
    if (a >= 0.0) {
	printf("\\psarc");
    }
    else {
	printf("\\psarcn");
    }
    break;
  }
  if (sshade != NULL) {
      printf("[fillstyle=solid,fillcolor=");
      wstring(&output, sshade);
      sep = ',';
  }
  else if (fill >= 0.0 && fill <= 1.0) {
      printf("[fillstyle=solid,fillcolor=");
      if (fill == 0.0) {
	  printf("black");
      }
      else if (fill == 0.25) {
	  printf("darkgray");
      }
      else if (fill == 0.5) {
	  printf("gray");
      }
      else if (fill == 0.75) {
	  printf("lightgray");
      }
      else if (fill == 1.0) {
	  printf("white");
      }
      else {
	  printf("fillval");
      }
      sep = ',';
  }
  else {
      sep = '[';
  }
  if (node->ptype == XLbox) {
      if (node->Upr.Ubox.boxradius != 0.0) {
	  printf("%clinearc=", sep);
	  wfloat(&output, node->Upr.Ubox.boxradius / fsc);
	  sep = ',';
      }
  }
  pstlineoptions(node, lsp, sep);
}


void pstahead(postype *point, postype shaft, double ht, double wid,
		     double lth, int typ, nametype *sou)
{ postype P, L, R, Px, Lx, Rx, Q;
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
      if (sou != NULL) {
	  wstring(&output, sou);
      }
      else {
	  printf("black");
      }
      printf("]{%%\n");
      printf("\\psline");
      wprop(P, L, x - y, y, x);
      wpos(P);
      wprop(P, R, x - y, y, x);
      printf("}\n");
  }
  else {
      printf("\\pscustom[linewidth=0pt,fillstyle=solid,fillcolor=");
      if (sou != NULL) {
	  wstring(&output, sou);
      }
      else {
	  printf("black");
      }
      printf(",linecolor=");
      if (sou != NULL) {
	  wstring(&output, sou);
      }
      else {
	  printf("black");
      }
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
{ wpos(C);
  if (ccw > 0 && endangle < startangle) {
      endangle += 2 * pi;
  }
  else if (ccw < 0 && endangle > startangle) {
      endangle -= 2 * pi;
  }
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
{ if (ccw >= 0.0) {
      printf("\\psarc");
  }
  else {
      printf("\\psarcn");
  }
}


void pstarcahead(postype C, postype point, int atyp, nametype *sou,
			double ht, double wid, double lth, double radius,
			double arcangle, postype *P)
{ postype Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  /*D; if debuglevel > 0 then writeln(log,' pstarcahead:'); D*/
  arcahead(C, point, atyp, ht, wid, lth, radius, arcangle, P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  printf("\\pscustom[linewidth=0pt,fillstyle=solid,fillcolor=");
  if (sou != NULL) {
      wstring(&output, sou);
  }
  else {
      printf("black");
  }
  printf(",linecolor=");
  if (sou != NULL) {
      wstring(&output, sou);
  }
  else {
      printf("black");
  }
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
  }
  else if (atyp == 3 && lwi < (wid - lwi) / 2) {
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


void pstsplinesegment(primitive *tv, int splc, int splt)
{ if (tv != NULL) {
      if (tv->Upr.Uline.aradius == mdistmax) {
	  if (splc == splt && splc > 1) {  /* 1st seg */
	      wpos(tv->aat);
	      wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
		  /* 1/6 from 1st to 2nd */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 4.0, 2.0, 6.0);
		  /* 1/3 from 1st to 2nd */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
		  /* 1/2 from 1st to 2nd */
	      /* 5/6 from 1st to 2nd */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  }
	  else if (splc > 1) {
	      wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
		  /* 1/6 from 1st to 2nd */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
		  /* 1/2 from 1st to 2nd */
	      /* 5/6 from 1st to 2nd */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  }
	  else {
	      wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
		  /* 1/6 along */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
		  /* 1/2 along */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 2.0, 4.0, 6.0);
		  /* 2/3 along */
	      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
		  /* 5/6 along */
	      wpos(tv->Upr.Uline.endpos);
	  }
      }
      else if (splc == splt && splc > 1) {
	  wpos(tv->aat);
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
		tv->Upr.Uline.aradius, 1.0);
      }
      else if (splc > 1) {
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1 + tv->Upr.Uline.aradius,
		1 - tv->Upr.Uline.aradius, 2.0);
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
		1 + tv->Upr.Uline.aradius, 2.0);
      }
      else if (splt == 1) {
	  wprop(tv->aat, tv->Upr.Uline.endpos, tv->Upr.Uline.aradius,
		1 - tv->Upr.Uline.aradius, 1.0);
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
		tv->Upr.Uline.aradius, 1.0);
	  wpos(tv->Upr.Uline.endpos);
      }
      else {
	  wprop(tv->aat, tv->Upr.Uline.endpos, tv->Upr.Uline.aradius,
		1 - tv->Upr.Uline.aradius, 1.0);
	  wpos(tv->Upr.Uline.endpos);
      }
  }
  putchar('\n');

  /* interior segment */
  /* last segment */
}


void pstdraw(primitive *node)
{ int lsp, ahn;
  postype X0, X1;
  boolean v;
  double s, c, lth;
  primitive *tn, *tx;
  nametype *sx;
  int TEMP;
  primitive *With1;

  getlinespec(node, &lsp, &tn);
  sshade = node->shadedp;
  lth = qenv(node, XLlinethick, tn->lthick);
  /*D if debuglevel > 0 then begin
     write(log,'pstdraw[',ordp(node):1,']: ');
     snaptype(log,ptype); printobject(node);
     writeln(log) end;
  if linesignal > 0 then begin write(errout,'pstdraw: ');
     snaptype(errout,ptype); writeln(errout) end D*/
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    initnesw();
    nesw(node);
    if (node->ptype == XBLOCK) {
	v = drawn(node, lsp, -1.0);
    }
    else {
	v = drawn(node, lsp, node->Upr.Ubox.boxfill);
    }
    if (v) {
	pstfilloptions(node, lsp, 0.0);
	wcoord(&output, west, south);
	wcoord(&output, east, north);
	putchar('\n');
    }
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLellipse:
    if (drawn(node, lsp, node->Upr.Uellipse.efill)) {
	pstfilloptions(node, lsp, 0.0);
	wpos(node->aat);
	wcoord(&output, 0.5 * node->Upr.Uellipse.elwidth,
	       0.5 * node->Upr.Uellipse.elheight);
	putchar('\n');
    }
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->Upr.Ucircle.cfill)) {
	pstfilloptions(node, lsp, 0.0);
	wpos(node->aat);
	putchar('{');
	wfloat(&output, node->Upr.Ucircle.radius / fsc);
	printf("}\n");
    }
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    if (drawn(node, lsp, node->Upr.Uline.lfill)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill && vfill >= 0.0) {
	    node->Upr.Uline.lfill = vfill;
	}
	X0 = arcstart(node);
	X1 = arcend(node);
	if (bfill) {
	    s = node->lthick;
	    node->lthick = 0.0;
	    pstfilloptions(node, lsp, node->Upr.Uline.endpos.ypos);
	    pstwarc(node->aat, fabs(node->Upr.Uline.aradius),
		    posangle(X0, node->aat), posangle(X1, node->aat),
		    node->Upr.Uline.endpos.ypos);
	    node->lthick = s;
	}
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	    pstarcahead(node->aat, X0, ahnum(node->Upr.Uline.atype), soutline,
			qenv(node, XLarrowht, node->Upr.Uline.height),
			qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, &X0);
	}
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	    pstarcahead(node->aat, X1, ahnum(node->Upr.Uline.atype), soutline,
			qenv(node, XLarrowht, node->Upr.Uline.height),
			qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			-fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, &X1);
	}
	sx = sshade;
	sshade = NULL;
	c = node->Upr.Uline.lfill;
	node->Upr.Uline.lfill = -1.0;
	pstfilloptions(node, lsp, node->Upr.Uline.endpos.ypos);
	sshade = sx;
	node->Upr.Uline.lfill = c;
	pstwarc(node->aat, fabs(node->Upr.Uline.aradius),
		posangle(X0, node->aat), posangle(X1, node->aat),
		node->Upr.Uline.endpos.ypos);
    }
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
    if (firstsegment(node)) {
	snode = node;
    }
    if (drawn(node, lsp, node->Upr.Uline.lfill)) {
	if (firstsegment(node)) {
	    getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	    if (bfill && vfill >= 0.0) {
		node->Upr.Uline.lfill = vfill;
	    }
	    if (bfill) {
		s = tn->lthick;
		tn->lthick = 0.0;
		pstfilloptions(tn, lsp, 0.0);
		tn->lthick = s;
		wpos(node->aat);
		tx = node;
		while (tx != NULL) {
		    wpos(tx->Upr.Uline.endpos);
		    putchar('\n');
		    tx = tx->son;
		}
	    }
	    ahn = ahnum(tn->Upr.Uline.atype);
	    if (ahn == 0 || ahn == 3) {
		TEMP = ahlex(tn->Upr.Uline.atype);
		if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		    pstahead(&node->aat, node->Upr.Uline.endpos,
			     qenv(node, XLarrowht, tn->Upr.Uline.height),
			     qenv(node, XLarrowwid, tn->Upr.Uline.width),
			     qenv(node, XLlinethick, tn->lthick),
			     ahnum(tn->Upr.Uline.atype), soutline);
		}
		TEMP = ahlex(tn->Upr.Uline.atype);
		if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		    pstahead(&tn->Upr.Uline.endpos, tn->aat,
			     qenv(node, XLarrowht, tn->Upr.Uline.height),
			     qenv(node, XLarrowwid, tn->Upr.Uline.width),
			     qenv(node, XLlinethick, tn->lthick),
			     ahnum(tn->Upr.Uline.atype), soutline);
		}
	    }
	    sx = sshade;
	    sshade = NULL;
	    pstfilloptions(tn, lsp, 0.0);
	    sshade = sx;
	    if (ahn != 0 && ahn != 3) {
		switch (ahlex(tn->Upr.Uline.atype)) {

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
	    bfill = false;
	    wpos(node->aat);
	}
	wpos(node->Upr.Uline.endpos);
	putchar('\n');
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		pstwrtext(snode, With1->textp,
		  0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
		  0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLmove:
    if (firstsegment(node)) {
	snode = node;
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		pstwrtext(snode, With1->textp,
		  0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
		  0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLspline:
    if (firstsegment(node)) {  /* first spline */
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill && vfill >= 0.0) {
	    node->Upr.Uline.lfill = vfill;
	}
	if (bfill) {
	    s = tn->lthick;
	    tn->lthick = 0.0;
	    pstfilloptions(tn, lsp, 0.0);
	    tn->lthick = s;
	    spltot = primdepth(node);
	    splcount = spltot;
	    printf("%%\n");
	    tx = node;
	    while (tx != NULL) {
		pstsplinesegment(tx, splcount, spltot);
		splcount--;
		tx = tx->son;
	    }
	    bfill = false;
	}
	spltot = primdepth(node);
	splcount = spltot;
	ahn = ahnum(tn->Upr.Uline.atype);
	if (ahn == 0 || ahn == 3) {
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		pstahead(&tn->Upr.Uline.endpos, tn->aat,
			 qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width),
			 qenv(node, XLlinethick, tn->lthick),
			 ahnum(tn->Upr.Uline.atype), soutline);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		pstahead(&node->aat, node->Upr.Uline.endpos,
			 qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width),
			 qenv(node, XLlinethick, tn->lthick),
			 ahnum(tn->Upr.Uline.atype), soutline);
	    }
	}
	if (lsp != XLinvis) {
	    sx = sshade;
	    sshade = NULL;
	    pstfilloptions(tn, lsp, 0.0);
	    sshade = sx;
	    if (ahn != 0 && ahn != 3) {
		switch (ahlex(tn->Upr.Uline.atype)) {

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
	    printf("%%\n");
	}
    }
    if (lsp != XLinvis) {
	pstsplinesegment(node, splcount, spltot);
    }
    splcount--;
    break;

  case XLstring:
    pstwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
	wstring(&output, node->textp);
	putchar('\n');
    }
    else if (node->lthick >= 0.0) {
	printf("\\psset{linewidth=");
	wfloat(&output, node->lthick);
	printf("pt}%%\n");
    }
    break;
  }
  /*DUGHM ;flush(output) MHGUD*/
}  /* pstdraw */


/* include mfp.h */
/* mfp.x */
/* Output routines for mfpic */
void mfpprelude(double n, double s, double e, double w)
{ printf("\\begin{mfpic}[72]");
  wbrace(w / fsc);
  wbrace(e / fsc);
  wbrace(s / fsc);
  wbrace(n / fsc);
  printf("\n%% dpic version 2015.08.31 option -m for mfpic\n");
  printf("\\dashlen=4bp\\dashspace=4bp\\dotspace=3bp\\pen{0.8bp}\n");
  printf("\\def\\mfpdefaultcolor{black}\\drawcolor{\\mfpdefaultcolor}\n");
  gslinethick = 0.8;
}


void mfppostlude(void)
{ printf("\\end{mfpic}\n");
  /*D if debuglevel > 0 then writeln(log,'mfpbpostlude done');D*/
}


void mfpwrtext(primitive *np, nametype *tp, double x, double y)
{ boolean A, B, L, R;

  if (tp == NULL) {
      return;
  }
  if (tp->next_ != NULL) {
      printf("\\tlabel[cc]");
      wcoord(&output, x, y);
      putchar('{');
      texstacktext(np, tp);
      printf("}\n");
      return;
  }
  printf("\\tlabel[");
  checkjust(tp, &A, &B, &L, &R);
  if (A) {
      putchar('b');
  }
  else if (B) {
      putchar('t');
  }
  else {
      putchar('c');
  }
  if (L) {
      putchar('l');
  }
  else if (R) {
      putchar('r');
  }
  else {
      putchar('c');
  }
  putchar(']');
  wcoord(&output, x, y);
  putchar('{');
  if (A) {
      printf("\\raisebox{");
      wfloat(&output,
	     venv(np, XLtextoffset) / scale / 12 + 0.5 * venv(np, XLtextht));
      printf("ex}{");
  }
  else if (B) {
      printf("\\raisebox{");
      wfloat(&output,
	     venv(np, XLtextoffset) / scale / -12 - 0.5 * venv(np, XLtextht));
      printf("ex}{");
  }
  wstring(&output, tp);
  if (A || B) {
      putchar('}');
  }
  printf("}\n");
}


void comma(void)
{ putchar(',');
}


void commacr(void)
{ printf(",\n");
}


void mfpsetshade(double fill, nametype *shade)
{ if (shade != NULL) {
      printf("\\gfill[");
      wstring(&output, shade);
      putchar(']');
      return;
  }
  if (fill <= 0.0) {
      printf("\\gfill");
      return;
  }
  if (fill >= 0.99) {
      return;
  }
  printf("\\shadespace=");
  wfloat(&output, 0.44 / sqrt(1 - fill));
  printf("bp\\shade");
}


void mfpellipse(postype aat, double elwidth, double elheight)
{ printf("\\ellipse{");
  wpos(aat);
  comma();
  wfloat(&output, 0.5 * elwidth / fsc);
  comma();
  wfloat(&output, 0.5 * elheight / fsc);
  printf("}\n");
}


void mfpcircle(postype aat, double radius)
{ printf("\\circle{");
  wpos(aat);
  comma();
  wfloat(&output, radius / fsc);
  printf("}\n");
}


void mfpsetdash(double dash)
{ if (dash < 0.0) {
      return;
  }
  printf("\\dashlen=");
  wfloat(&output, dash);
  printf("in");
  printf("\\dashspace=");
  wfloat(&output, dash);
  printf("in");
}


void mfsetthick(double lthk)
{ if (lthk < 0.0 || lthk == gslinethick) {
      return;
  }
  printf("\\pen{");
  wfloat(&output, lthk);
  printf("bp}");
  gslinethick = lthk;
}


void mfpdashdot(int lsp, double param, double lth)
{ if (lsp == XLdashed) {
      mfpsetdash(param / fsc);
      printf("\\dashed");
      return;
  }
  if (lsp != XLdotted) {
      return;
  }
  printf("\\dotted[");
  wfloat(&output, lth);
  printf("pt,");
  if (param >= 0.0) {
      wfloat(&output, param / fsc);
      printf("in]");
  }
  else {
      wfloat(&output, lth * 3);
      printf("pt]");
  }
}


void mfplineopts(double lth, double param, int lsp, nametype *sou)
{ if (sou != NULL) {
      printf("\\drawcolor{");
      wstring(&output, sou);
      printf("}\n");
  }
  mfsetthick(lth);
  mfpdashdot(lsp, param, lth);
}


void mfpahead(int atyp, postype *point, postype shaft, double ht,
		     double wid, double lth, nametype *sou)
{ postype P, L, R, Px, Lx, Rx, Q;
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
  }
  else {
      cs = ht / d;
      ss = wid / 2 / d;
  }
  /*D if debuglevel > 0 then begin
     writeln(log,' mfpahead:');
     end; D*/
  wpos(Rx);
  commacr();
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
  }
  else if (atyp == 3 && lwu < (wid - lwu) / 2) {
      comma();
      putchar('\n');
      wpos(Px);
  }
  printf("}\n");
  *point = P;
}


void mfpwarc(postype Ctr, double radius, double startangle,
		    double endangle, double ccw)
{ printf("\\arc[p]{");
  wpos(Ctr);
  comma();
  if (ccw > 0 && endangle < startangle) {
      endangle += 2 * pi;
  }
  else if (ccw < 0 && endangle > startangle) {
      endangle -= 2 * pi;
  }
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
{ postype Co, Ci, Px, Cox, Cix, Ao, Ai;
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
  }
  else if (atyp == 3 && lwi < (wid - lwi) / 2) {
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


void mfpsplinesegment(primitive *tv, int splc, int splt)
{ if (tv == NULL) {
      return;
  }
  if (splt == 1) {
      printf("\\polyline{");
      wpos(tv->aat);
      comma();
      wpos(tv->Upr.Uline.endpos);
      printf("}\n");
      return;
  }
  if (tv->Upr.Uline.aradius == mdistmax) {
      if (splc == splt) {  /* 1st seg */
	  printf("\\cbeziers{");
	  wpos(tv->aat);
	  comma();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	  comma();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 2.0, 1.0, 3.0);
	  comma();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  commacr();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  comma();
	  return;
      }
      if (splc > 1) {  /* interior segment */
	  wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	  comma();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  comma();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  commacr();
	  return;
      }
      wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
      comma();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      comma();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 2.0, 3.0);
      comma();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
      commacr();
      wpos(tv->Upr.Uline.endpos);
      printf("}\n");
      return;
  }
  if (splc == splt && splc > 1) {
      printf("\\cbeziers{");
      wpos(tv->aat);
      comma();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	    tv->Upr.Uline.aradius, 1.0);
      commacr();
      return;
  }
  if (splc > 1) {
      wprop(tv->aat, tv->Upr.Uline.endpos, 0.5 + tv->Upr.Uline.aradius / 2,
	    0.5 - tv->Upr.Uline.aradius / 2, 1.0);
      comma();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      comma();
      wprop(tv->aat, tv->Upr.Uline.endpos, 0.5 - tv->Upr.Uline.aradius / 2,
	    0.5 + tv->Upr.Uline.aradius / 2, 1.0);
      commacr();
      return;
  }
  wprop(tv->aat, tv->Upr.Uline.endpos, tv->Upr.Uline.aradius,
	1 - tv->Upr.Uline.aradius, 1.0);
  comma();
  wpos(tv->Upr.Uline.endpos);
  printf("}\n");

  /* last segment */
}


#define r               (1 - (sqrt(2.0) - 1) * 4 / 3)


void mfpbox(double x, double y, double n, double s, double e, double w,
		   double rad)
{ double h, v, rr;

  rad = Min(Min(fabs(rad), fabs((n - s) / 2)), fabs((e - w) / 2));
  if (rad == 0.0) {
      printf("\\rect{");
      wcoord(&output, w, s);
      comma();
      wcoord(&output, e, n);
  }
  else {
      rr = r * rad;
      h = (e - w) / 2 - rad;
      v = (n - s) / 2 - rad;
      printf("\\lclosed\\cbeziers{");
      wcoord(&output, e, y + v);
      comma();
      wcoord(&output, e, n - rr);
      comma();
      wcoord(&output, e - rr, n);
      comma();
      wcoord(&output, x + h, n);
      commacr();
      wcoord(&output, x + h / 3, n);
      comma();
      wcoord(&output, x - h / 3, n);
      comma();
      wcoord(&output, x - h, n);
      comma();
      wcoord(&output, w + rr, n);
      comma();
      wcoord(&output, w, n - rr);
      comma();
      wcoord(&output, w, y + v);
      commacr();
      wcoord(&output, w, y + v / 3);
      comma();
      wcoord(&output, w, y - v / 3);
      comma();
      wcoord(&output, w, y - v);
      comma();
      wcoord(&output, w, s + rr);
      comma();
      wcoord(&output, w + rr, s);
      comma();
      wcoord(&output, x - h, s);
      commacr();
      wcoord(&output, x - h / 3, s);
      comma();
      wcoord(&output, x + h / 3, s);
      comma();
      wcoord(&output, x + h, s);
      comma();
      wcoord(&output, e - rr, s);
      comma();
      wcoord(&output, e, s + rr);
      comma();
      wcoord(&output, e, y - v);
      commacr();
      wcoord(&output, e, y - v / 3);
      comma();
      wcoord(&output, e, y + v / 3);
  }
  printf("}\n");
}

#undef r


void mfpdraw(primitive *node)
{ /* node is always <> nil */
  int lsp;
  postype X0, X1;
  primitive *tn, *tx;
  double lth;
  int TEMP;
  primitive *With1;

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
	if (node->Upr.Ubox.boxfill >= 0.0 && node->Upr.Ubox.boxfill <= 1.0 ||
	     node->shadedp != NULL) {
	    mfpsetshade(node->Upr.Ubox.boxfill, node->shadedp);
	    mfpbox(node->aat.xpos, node->aat.ypos, north, south, east, west,
		   node->Upr.Ubox.boxradius);
	}
	if (lsp != XLinvis) {
	    mfplineopts(lth, node->lparam, lsp, node->outlinep);
	    mfpbox(node->aat.xpos, node->aat.ypos, north, south, east, west,
		   node->Upr.Ubox.boxradius);
	    if (node->outlinep != NULL) {
		printf("\\drawcolor{\\mfpdefaultcolor}\n");
	    }
	}
    }
    mfpwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLellipse:
    if (node->Upr.Uellipse.efill >= 0.0 && node->Upr.Uellipse.efill <= 1.0 ||
	 node->shadedp != NULL) {
	mfpsetshade(node->Upr.Uellipse.efill, node->shadedp);
	mfpellipse(node->aat, node->Upr.Uellipse.elwidth, node->Upr.Uellipse.elheight);
    }
    if (lsp != XLinvis) {
	mfplineopts(lth, node->lparam, lsp, node->outlinep);
	mfpellipse(node->aat, node->Upr.Uellipse.elwidth, node->Upr.Uellipse.elheight);
	if (node->outlinep != NULL) {
	    printf("\\drawcolor{\\mfpdefaultcolor}\n");
	}
    }
    mfpwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (node->Upr.Ucircle.cfill >= 0.0 && node->Upr.Ucircle.cfill <= 1.0 ||
	 node->shadedp != NULL) {
	mfpsetshade(node->Upr.Ucircle.cfill, node->shadedp);
	mfpcircle(node->aat, node->Upr.Ucircle.radius);
    }
    if (lsp != XLinvis) {
	mfplineopts(lth, node->lparam, lsp, node->outlinep);
	mfpcircle(node->aat, node->Upr.Ucircle.radius);
	if (node->outlinep != NULL) {
	    printf("\\drawcolor{\\mfpdefaultcolor}\n");
	}
    }
    mfpwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLspline:
    if (firstsegment(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    mfsetthick(0.0);
	    printf("\\draw");
	    mfpsetshade(vfill, sshade);
	    printf("%%\n");
	    printf("\\lclosed");
	    spltot = primdepth(node);
	    splcount = spltot;
	    tx = node;
	    while (tx != NULL) {
		mfpsplinesegment(tx, splcount, spltot);
		splcount--;
		tx = tx->son;
	    }
	    vfill = -1.0;
	    sshade = NULL;
	}
	if (lsp != XLinvis) {
	    lth = qenv(tn, XLlinethick, tn->lthick);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		mfpahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		mfpahead(ahnum(node->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    spltot = primdepth(node);
	    splcount = spltot;
	    mfplineopts(lth, node->lparam, lsp, soutline);
	}
    }
    if (lsp != XLinvis) {
	mfpsplinesegment(node, splcount, spltot);
    }
    splcount--;
    if (node->son == NULL && node->outlinep != NULL) {
	printf("\\drawcolor{\\mfpdefaultcolor}\n");
    }
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (firstsegment(node)) {
	snode = node;
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    mfsetthick(0.0);
	    printf("\\draw");
	    mfpsetshade(vfill, sshade);
	    printf("%%\n");
	    printf("\\lclosed\\polyline{");
	    wpos(node->aat);
	    tx = node;
	    while (tx != NULL) {
		commacr();
		wpos(tx->Upr.Uline.endpos);
		tx = tx->son;
	    }
	    printf("}\n");
	    vfill = -1.0;
	    sshade = NULL;
	}
	if (lsp != XLinvis) {
	    lth = qenv(tn, XLlinethick, tn->lthick);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		mfpahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		mfpahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    mfplineopts(lth, node->lparam, lsp, soutline);
	    printf("\\polyline{");
	    wpos(node->aat);
	}
    }
    if (lsp != XLinvis) {
	commacr();
	wpos(node->Upr.Uline.endpos);
	if (node->son == NULL) {
	    printf("}\n");
	    if (soutline != NULL) {
		printf("\\drawcolor{\\mfpdefaultcolor}\n");
	    }
	}
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		mfpwrtext(snode, With1->textp,
		  0.5 * (With1->aat.xpos + With1->Upr.Uline.endpos.xpos),
		  0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLarc:
    X0 = arcstart(node);
    X1 = arcend(node);
    getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
    if (bfill) {
	mfsetthick(0.0);
	printf("\\draw");
	mfpsetshade(vfill, sshade);
	printf("%%\n");
	printf("\\lclosed");
	mfpwarc(node->aat, node->Upr.Uline.aradius, posangle(X0, node->aat),
		posangle(X1, node->aat), node->Upr.Uline.endpos.ypos);
	vfill = -1.0;
	sshade = NULL;
    }
    if (lsp != XLinvis) {
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	    mfparcahead(node->aat, X0, ahnum(node->Upr.Uline.atype), soutline,
			qenv(node, XLarrowht, node->Upr.Uline.height),
			qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, &X0);
	}
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	    mfparcahead(node->aat, X1, ahnum(node->Upr.Uline.atype), soutline,
			qenv(node, XLarrowht, node->Upr.Uline.height),
			qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			-fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, &X1);
	}
	/*D if debuglevel > 0 then begin write(log,' XLarc: lth=');
	    wfloat(log,lth); writeln(log) end;D*/
	mfplineopts(lth, node->lparam, lsp, soutline);
	mfpwarc(node->aat, node->Upr.Uline.aradius, posangle(X0, node->aat),
		posangle(X1, node->aat), node->Upr.Uline.endpos.ypos);
	if (soutline != NULL) {
	    printf("\\drawcolor{\\mfpdefaultcolor}\n");
	}
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
    }
    else if (lth >= 0.0) {
	mfsetthick(lth);
    }
    else if (node->lparam >= 0.0) {
	mfpsetdash(node->lparam);
	putchar('\n');
    }
    break;
  }
}  /* mfpdraw */


/* include mpo.h */
/* mpo.x */
/* Output routines for MetaPost */
void mpoprelude(void)
{ /* n,s,e,w: real */
  printstate++;
  printf("%% dpic version 2015.08.31 option -s for MetaPost\n");
  printf("beginfig(%d)\n", printstate);
  printf("def lcbutt=linecap:=butt enddef;\n");
  printf("def lcsq=linecap:=squared enddef;\n");
  printf("def lcrnd=linecap:=rounded enddef;\n");
  printf("def ljmit=linejoin:=mitered enddef;\n");
  printf("def lcolr= enddef; def drw=draw enddef; def X=;enddef;\n");
  printf("lcbutt; ljmit;\n");
}


void mpopostlude(void)
{ printf("endfig;\n");
  /*D; if debuglevel > 0 then writeln(log,'mpobpostlude done');D*/
}


void mpowrtext(primitive *np, nametype *tp, double x, double y)
{ boolean A, B, L, R;

  if (tp == NULL) {
      return;
  }
  if (tp->next_ != NULL) {
      printf("label");
      printf("(btex ");
      texstacktext(np, tp);
      printf(" etex,");
      wcoord(&output, x, y);
      printf(") X\n");
      return;
  }
  printf("label");
  checkjust(tp, &A, &B, &L, &R);
  if (A && L) {
      printf(".urt");
  }
  else if (A && R) {
      printf(".ulft");
  }
  else if (B && L) {
      printf(".lrt");
  }
  else if (B && R) {
      printf(".llft");
  }
  else if (A) {
      printf(".top");
  }
  else if (B) {
      printf(".bot");
  }
  else if (L) {
      printf(".rt");
  }
  else if (R) {
      printf(".lft");
  }
  printf("(btex ");
  wstring(&output, tp);
  printf(" etex,");
  wcoord(&output, x, y);
  printf(") X\n");
}


void mposetthick(double lthick)
{ if (lthick == mdistmax) {
      if (gslinethick != mdistmax) {
	  lthick = gslinethick;
      }
      else {
	  lthick = 0.8;
      }
  }
  if (lthick < 0.0 || lthick == gslinethick) {
      return;
  }
  /*D; if debuglevel > 0 then begin writeln(log,' mposetthick:');
     write(log,' lthick='); wfloat(log,lthick);
     write(log,' gslinethick='); wfloat(log,gslinethick);
     writeln(log)
     end D*/
  printf("pickup pencircle scaled ");
  wfloat(&output, lthick);
  printf("bp;\n");
  gslinethick = lthick;
}


void mpolinecap(int lsp)
{ switch (lsp) {

  case XLdashed:
    printf("lcsq;\n");
    break;

  case XLdotted:
    printf("lcrnd;\n");
    break;
  }
}


void addcolor(nametype *sp, double fillv)
{ printf(" withcolor ");
  if (sp != NULL) {
      wstring(&output, sp);
  }
  else {
      wfloat(&output, fillv);
      printf("white");
  }
}


void mpodashdot(int lsp, double param, nametype *op)
{ if (lsp == XLdashed) {
      printf(" dashed evenly");
      if (param > 0) {
	  printf(" scaled ");
	  wfloat(&output, param / fsc / 3);
      }
      if (op != NULL) {
	  addcolor(op, -1.0);
      }
      printf(" X lcbutt;\n");
      return;
  }
  if (lsp == XLdotted) {
      printf(" dashed withdots scaled ");
      if (param <= 0) {
	  printf("0.8");
      }
      else {
	  wfloat(&output, param / fsc / 5);
      }
      if (op != NULL) {
	  addcolor(op, -1.0);
      }
      printf(" X lcbutt;\n");
      return;
  }
  if (op != NULL) {
      addcolor(op, -1.0);
      printf(" X\n");
  }
  else {
      printf(" X\n");
  }
}


#define b               ((sqrt(2.0) - 1.0) * 4.0 / 3.0)


void mpocorner(postype ctr, double x, double y, double r, double ct,
		      double st)
{ wcoord(&output, ctr.xpos + x + ct * r, ctr.ypos + y + st * r);
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
{ double r, h, v;

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
  }
  else {
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
{ fputs(initial, stdout);
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
{ postype P, L, R, Px, Lx, Rx, Q;
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
      if (sou != NULL) {
	  addcolor(sou, -1.0);
      }
      printf(" X\n");
  }
  else {
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
      printf("\n --cycle");
      if (sou != NULL) {
	  addcolor(sou, -1.0);
      }
      printf(" X\n");
  }
  *point = P;
}


void mpoarcahead(postype C, postype point, int atyp, nametype *sou,
			double ht, double wid, double lth, double radius,
			double arcangle, postype *P)
{ postype Co, Ci, Px, Cox, Cix, Ao, Ai;
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
  }
  else if (atyp == 3 && lwi < (wid - lwi) / 2) {
      popgwarc(Cox, radius, posangle(Ao, Cox), posangle(Px, Cox), -ccw);
      popgwarc(Cix, radius, posangle(Px, Cix), posangle(Ai, Cix), ccw);
  }
  printf(" --cycle");
  addcolor(sou, 0.0);
  printf(" X\n");
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


void splinesegment(primitive *tv, int splc, int splt)
{ if (tv == NULL) {
      return;
  }
  if (splt == 1) {
      wpos(tv->aat);
      ddash();
      wpos(tv->Upr.Uline.endpos);
      return;
  }
  if (tv->Upr.Uline.aradius == mdistmax) {
      if (splc == splt) {  /* 1st seg */
	  wpos(tv->aat);
	  ddash();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  controls();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  return;
      }
      if (splc > 1) {  /* interior segment */
	  wrand();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	  ddot();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  controls();
	  wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  return;
      }
      wrand();
      wprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
      ddot();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      ddash();
      wpos(tv->Upr.Uline.endpos);
      return;
  }
  if (splc == splt && splc > 1) {
      wpos(tv->aat);
      controls();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	    tv->Upr.Uline.aradius, 1.0);
      return;
  }
  if (splc > 1) {
      wrand();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1 + tv->Upr.Uline.aradius,
	    1 - tv->Upr.Uline.aradius, 2.0);
      ddot();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      controls();
      wprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	    1 + tv->Upr.Uline.aradius, 2.0);
      return;
  }
  wrand();
  wprop(tv->aat, tv->Upr.Uline.endpos, tv->Upr.Uline.aradius,
	1 - tv->Upr.Uline.aradius, 1.0);
  ddot();
  wpos(tv->Upr.Uline.endpos);

  /* last segment */
}


void mpodraw(primitive *node)
{ /* node is always <> nil */
  int lsp;
  postype X0, X1;
  primitive *tn, *tx;
  double lth;
  int TEMP;
  primitive *With1;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    if (node->ptype == XLbox) {
	if (node->shadedp != NULL ||
	     node->Upr.Ubox.boxfill >= 0.0 && node->Upr.Ubox.boxfill <= 1.0) {
	    mpobox("fill ", node->aat, node->Upr.Ubox.boxwidth / 2,
		   node->Upr.Ubox.boxheight / 2, node->Upr.Ubox.boxradius);
	    addcolor(node->shadedp, node->Upr.Ubox.boxfill);
	    printf(" X\n");
	}
	if (lsp != XLinvis) {
	    mposetthick(node->lthick);
	    mpolinecap(lsp);
	    mpobox("drw ", node->aat, node->Upr.Ubox.boxwidth / 2,
		   node->Upr.Ubox.boxheight / 2, node->Upr.Ubox.boxradius);
	    mpodashdot(lsp, node->lparam, node->outlinep);
	}
    }
    mpowrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLellipse:
    if (node->shadedp != NULL ||
	 node->Upr.Uellipse.efill >= 0.0 && node->Upr.Uellipse.efill <= 1.0) {
	mpoellipse("fill ", node->aat, node->Upr.Uellipse.elwidth / 2,
		   node->Upr.Uellipse.elheight / 2);
	addcolor(node->shadedp, node->Upr.Uellipse.efill);
	printf(" X\n");
    }
    if (lsp != XLinvis) {
	mposetthick(node->lthick);
	mpolinecap(lsp);
	mpoellipse("drw ", node->aat, node->Upr.Uellipse.elwidth / 2,
		   node->Upr.Uellipse.elheight / 2);
	mpodashdot(lsp, node->lparam, node->outlinep);
    }
    mpowrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (node->shadedp != NULL ||
	 node->Upr.Ucircle.cfill >= 0.0 && node->Upr.Ucircle.cfill <= 1.0) {
	printf("fill fullcircle scaled ");
	wfloat(&output, node->Upr.Ucircle.radius * 2 / fsc);
	printf(" shifted ");
	wpos(node->aat);
	addcolor(node->shadedp, node->Upr.Ucircle.cfill);
	printf(" X\n");
    }
    if (lsp != XLinvis) {
	mposetthick(node->lthick);
	mpolinecap(lsp);
	printf("drw fullcircle scaled ");
	wfloat(&output, node->Upr.Ucircle.radius * 2 / fsc);
	printf(" shifted ");
	wpos(node->aat);
	mpodashdot(lsp, node->lparam, node->outlinep);
    }
    mpowrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    X0 = arcstart(node);
    X1 = arcend(node);
    getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
    if (bfill) {
	mposetthick(0.0);
	printf("fill ");
	wpos(X0);
	popgwarc(node->aat, fabs(node->Upr.Uline.aradius),
		 posangle(X0, node->aat), posangle(X1, node->aat),
		 node->Upr.Uline.endpos.ypos);
	printf(" --cycle");
	addcolor(sshade, vfill);
	printf(" X\n");
	vfill = -1.0;
	sshade = NULL;
    }
    if (lsp != XLinvis) {
	mposetthick(lth);
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	    mpoarcahead(node->aat, X0, ahnum(node->Upr.Uline.atype), soutline,
			qenv(node, XLarrowht, node->Upr.Uline.height),
			qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, &X0);
	}
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	    mpoarcahead(node->aat, X1, ahnum(node->Upr.Uline.atype), soutline,
			qenv(node, XLarrowht, node->Upr.Uline.height),
			qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			-fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, &X1);
	}
	mpolinecap(lsp);
	printf("drw ");
	wpos(X0);
	popgwarc(node->aat, fabs(node->Upr.Uline.aradius),
		 posangle(X0, node->aat), posangle(X1, node->aat),
		 node->Upr.Uline.endpos.ypos);
	mpodashdot(lsp, node->lparam, node->outlinep);
    }
    mpowrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
    if (firstsegment(node)) {
	snode = node;
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    mposetthick(0.0);
	    printf("fill ");
	    wpos(node->aat);
	    tx = node;
	    while (tx != NULL) {
		ddash();
		wpos(tx->Upr.Uline.endpos);
		tx = tx->son;
	    }
	    printf(" --cycle");
	    addcolor(sshade, vfill);
	    printf(" X\n");
	    vfill = -1.0;
	    sshade = NULL;
	}
	lth = qenv(node, XLlinethick, tn->lthick);
	if (lsp != XLinvis) {
	    mposetthick(lth);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		mpoahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		mpoahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    mpolinecap(lsp);
	    printf("drw ");
	    wpos(node->aat);
	}
    }
    if (lsp != XLinvis) {
	ddash();
	wpos(node->Upr.Uline.endpos);
	if (node->son == NULL) {
	    mpodashdot(lsp, node->lparam, node->outlinep);
	}
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		mpowrtext(node, With1->textp,
		  0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
		  0.5 * (With1->Upr.Uline.endpos.ypos + With1->aat.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLmove:
    if (firstsegment(node)) {
	snode = node;
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		mpowrtext(node, With1->textp,
		  0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
		  0.5 * (With1->Upr.Uline.endpos.ypos + With1->aat.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLspline:
    if (firstsegment(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    spltot = primdepth(node);
	    splcount = spltot;
	    mposetthick(0.0);
	    printf("fill ");
	    tx = node;
	    while (tx != NULL) {
		splinesegment(tx, splcount, spltot);
		splcount--;
		tx = tx->son;
	    }
	    printf(" --cycle");
	    addcolor(sshade, vfill);
	    printf(" X\n");
	    vfill = -1.0;
	    sshade = NULL;
	}
	lth = qenv(tn, XLlinethick, tn->lthick);
	if (lsp != XLinvis) {
	    spltot = primdepth(node);
	    splcount = spltot;
	    mposetthick(lth);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		mpoahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		mpoahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
	    }
	    node->shadedp = NULL;
	    sshade = NULL;
	    node->Upr.Uline.lfill = -1.0;
	    vfill = -1.0;
	    mpolinecap(lsp);
	    printf("drw ");
	}
    }
    if (lsp != XLinvis) {
	splinesegment(node, splcount, spltot);
	if (splcount == 1) {
	    mpodashdot(lsp, node->lparam, node->outlinep);
	}
    }
    splcount--;
    /*D; if debuglevel > 0 then
            writeln(log,'pgfdraw: node ',splcount:1);D*/
    break;

  case XLstring:
    mpowrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
	wstring(&output, node->textp);
	putchar('\n');
    }
    else if (node->lthick >= 0.0) {
	mposetthick(node->lthick);
    }
    break;
  }
}  /* mpodraw */


/* include pgf.h */
/* pgf.x */
/* pgf output routines */
void pgfprelude(void)
{ /* n,s,e,w: real */
  printf("\\begin{tikzpicture}[scale=2.54]\n");
  printf("%% dpic version 2015.08.31 option -g for TikZ and PGF 1.01\n");
  printf("\\ifx\\dpiclw\\undefined\\newdimen\\dpiclw\\fi\n");
  printf("\\global\\def\\dpicdraw{\\draw[line width=\\dpiclw]}\n");
  printf("\\global\\def\\dpicstop{;}\n");
  printf("\\dpiclw=0.8bp\n");
}


void pgfpostlude(void)
{ printf("\\end{tikzpicture}\n");
  /*D; if debuglevel > 0 then writeln(log,'pgfpostlude done') D*/
}


void pgfwrtext(primitive *np, nametype *tp, double x, double y)
{ boolean A, B, L, R;

  if (tp == NULL) {
      return;
  }
  if (tp->next_ != NULL) {
      printf("\\draw ");
      wcoord(&output, x, y);
      printf(" node");
      putchar('{');
      texstacktext(np, tp);
      printf("};\n");
      return;
  }
  printf("\\draw ");
  wcoord(&output, x, y);
  printf(" node");
  checkjust(tp, &A, &B, &L, &R);
  if (A || B || L || R) {
      putchar('[');
      if (A && L) {
	  printf("above right");
      }
      else if (A && R) {
	  printf("above left");
      }
      else if (B && L) {
	  printf("below right");
      }
      else if (B && R) {
	  printf("below left");
      }
      else if (A) {
	  printf("above");
      }
      else if (B) {
	  printf("below");
      }
      else if (L) {
	  printf("right");
      }
      else if (R) {
	  printf("left");
      }
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
{ printf("\\dpicstop\n");
}


void pgfsetthick(double lthick)
{ if (lthick == mdistmax) {
      if (gslinethick != mdistmax) {
	  lthick = gslinethick;
      }
      else {
	  lthick = 0.8;
      }
  }
  /*D if debuglevel > 0 then begin
     writeln(log);
     write(log,'lthick='); wfloat(log,lthick);
     write(log,' gslinethick='); wfloat(log,gslinethick); writeln(log)
     end;D*/
  if (lthick < 0.0 || lthick == gslinethick) {
      return;
  }
  printf("\\dpiclw=");
  wfloat(&output, lthick);
  printf("bp\n");
  gslinethick = lthick;
}


void pgfbox(postype ctr, double halfwid, double halfht, double rad)
{ double r, h, v;

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
{ postype P, L, R, Px, Lx, Rx, Q;
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
      putchar('\n');
  }
  else if (atyp == 3) {
      wpos(Px);
      ddash();
      wpos(Rx);
      ddash();
      wpos(*point);
      ddash();
      wpos(Lx);
      printf(" --cycle");
      /* wprop(P,L,x-y,y,x); ddash; wpos(P); ddash; wprop(P,R,x-y,y,x) */
  }
  else {
      /* wpos(affine(x,0.0,P,Q));
      ddash; wpos(R); ddash; wpos(P); ddash; wpos(L); */
      wpos(Rx);
      ddash();
      wpos(*point);
      ddash();
      wpos(Lx);
      printf(" --cycle");
  }
  pgfendpath();
  *point = P;
}


void pgfstartdraw(int initial, primitive *node, int lsp)
{ Char sep;
  double fill;

  if (lsp == XLinvis) {             /* initial = 1 or 3 for filled arrowheads */
      printf("\\fill");
  }
  else if (initial == 0) {
      printf("\\dpicdraw");
  }
  else {
      printf("\\filldraw");
  }
  fill = -1.0;
  switch (node->ptype) {

  case XBLOCK:
    /* blank case */
    break;

  case XLbox:
    fill = node->Upr.Ubox.boxfill;
    break;

  case XLcircle:
    fill = node->Upr.Ucircle.cfill;
    break;

  case XLellipse:
    fill = node->Upr.Uellipse.efill;
    break;

  case XLarc:
  case XLline:
  case XLarrow:
  case XLmove:
  case XLspline:
    fill = node->Upr.Uline.lfill;
    break;
  }
  fill = (long)floor(fill * 1000000L + 0.5) / 1000000.0;
  if (node->shadedp != NULL || sshade != NULL) {
      printf("[fill=");
      if (node->shadedp != NULL) {
	  wstring(&output, node->shadedp);
      }
      else {
	  wstring(&output, sshade);
      }
      sep = ',';
  }
  else if (fill >= 0.0 && fill <= 1.0) {
      printf("[fill=");
      if (fill == 0.0) {
	  printf("black");
      }
      else if (fill == 0.25) {
	  printf("darkgray");
      }
      else if (fill == 0.5) {
	  printf("gray");
      }
      else if (fill == 0.75) {
	  printf("lightgray");
      }
      else if (fill == 1.0) {
	  printf("white");
      }
      else {
	  printf("white!");
	  wfloat(&output, fill * 100);
	  printf("!black");
      }
      sep = ',';
  }
  else {
      sep = '[';
  }
  if (initial == 1 || initial == 3) {
      printf("%cline width=0bp", sep);
      sep = ',';
  }
  else if (node->lthick >= 0.0 && node->lthick != gslinethick) {
      printf("%cline width=", sep);
      wfloat(&output, node->lthick);
      printf("bp");
      sep = ',';
  }
  if ((node->outlinep != NULL || soutline != NULL) && lsp != XLinvis) {
      printf("%cdraw=", sep);
      if (node->outlinep != NULL) {
	  wstring(&output, node->outlinep);
      }
      else {
	  wstring(&output, soutline);
      }
      if (initial == 1 || initial == 3) {
	  printf(",fill=");
	  if (node->outlinep != NULL) {
	      wstring(&output, node->outlinep);
	  }
	  else {
	      wstring(&output, soutline);
	  }
      }
      sep = ',';
  }
  if (lsp == XLdashed && initial != 1 && initial != 3) {
      if (node->lparam <= 0) {
	  printf("%cdashed", sep);
      }
      else {
	  printf("%cdash pattern=on ", sep);
	  wfloat(&output, node->lparam / fsc);
	  printf("in off ");
	  wfloat(&output, node->lparam / fsc);
	  printf("in");
      }
      sep = ',';
  }
  else if (lsp == XLdotted) {
      if (node->lparam <= 0) {
	  printf("%cdotted", sep);
      }
      else {
	  printf("%ccap=round,dash pattern=on ", sep);
	  printf("0pt off ");
	  wfloat(&output, node->lparam * 72 / fsc);
	  printf("bp");
      }
      sep = ',';
  }
  if (sep == ',') {
      putchar(']');
  }
  else {
      putchar(' ');
  }
}


void pgfarcahead(postype C, postype point, int atyp, nametype *sou,
			double ht, double wid, double lth, double radius,
			double arcangle, postype *P)
{ postype Q, Co, Ci, Px, Cox, Cix, Ao, Ai;
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
  }
  else if (atyp == 3 && lwi < (wid - lwi) / 2) {
      wpos(Px);
      popgwarc(Cix, radius, posangle(Px, Cix), posangle(Ai, Cix), ccw);
      popgwarc(Ci, radius, posangle(Ai, Ci), posangle(point, Ci), -ccw);
      popgwarc(Co, radius, posangle(point, Co), posangle(Ao, Co), ccw);
      popgwarc(Cox, radius, posangle(Ao, Cox), posangle(Px, Cox), -ccw);
  }
  else {
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
{ /* node is always <> nil */
  int lsp;
  postype X0, X1;
  primitive *tn, *tx;
  double s, c, lth;
  boolean v;
  nametype *sf, *sg;
  int TEMP;
  primitive *With1;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);
  switch (node->ptype) {

  case XLbox:
  case XBLOCK:
    if (node->ptype == XBLOCK) {
	v = drawn(node, lsp, -1.0);
    }
    else {
	v = drawn(node, lsp, node->Upr.Ubox.boxfill);
    }
    if (v) {
	pgfstartdraw(0, node, lsp);
	pgfbox(node->aat, node->Upr.Ubox.boxwidth / 2,
	       node->Upr.Ubox.boxheight / 2, node->Upr.Ubox.boxradius);
    }
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLellipse:
    if (drawn(node, lsp, node->Upr.Uellipse.efill)) {
	pgfstartdraw(0, node, lsp);
	wpos(node->aat);
	printf(" ellipse (");
	wfloat(&output, node->Upr.Uellipse.elwidth / 2 / fsc / 2.54);
	printf("in and ");
	wfloat(&output, node->Upr.Uellipse.elheight / 2 / fsc / 2.54);
	printf("in)");
	pgfendpath();
    }
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (drawn(node, lsp, node->Upr.Ucircle.cfill)) {
	pgfstartdraw(0, node, lsp);
	wpos(node->aat);
	printf(" circle (");
	wfloat(&output, node->Upr.Ucircle.radius / fsc / 2.54);
	printf("in)");
	pgfendpath();
    }
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    if (drawn(node, lsp, -1.0)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill && vfill >= 0.0) {
	    node->Upr.Uline.lfill = vfill;
	}
	X0 = arcstart(node);
	X1 = arcend(node);
	if (bfill) {
	    s = node->lthick;
	    node->lthick = 0.0;
	    pgfstartdraw(0, node, XLinvis);
	    node->lthick = s;
	    wpos(X0);
	    popgwarc(node->aat, fabs(node->Upr.Uline.aradius),
		     posangle(X0, node->aat), posangle(X1, node->aat),
		     node->Upr.Uline.endpos.ypos);
	    printf("--cycle\n");
	    pgfendpath();
	}
	if (lsp != XLinvis) {
	    TEMP = ahlex(node->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		sf = node->shadedp;
		node->shadedp = NULL;
		sg = sshade;
		sshade = soutline;
		pgfarcahead(node->aat, X0, ahnum(node->Upr.Uline.atype),
			    soutline,
			    qenv(node, XLarrowht, node->Upr.Uline.height),
			    qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			    fabs(node->Upr.Uline.aradius),
			    node->Upr.Uline.endpos.ypos, &X0);
		node->shadedp = sf;
		sshade = sg;
	    }
	    TEMP = ahlex(node->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		sf = node->shadedp;
		node->shadedp = NULL;
		sg = sshade;
		sshade = soutline;
		pgfarcahead(node->aat, X1, ahnum(node->Upr.Uline.atype),
			    soutline,
			    qenv(node, XLarrowht, node->Upr.Uline.height),
			    qenv(node, XLarrowwid, node->Upr.Uline.width), lth,
			    -fabs(node->Upr.Uline.aradius),
			    node->Upr.Uline.endpos.ypos, &X1);
		node->shadedp = sf;
		sshade = sg;
	    }
	    sf = node->shadedp;
	    node->shadedp = NULL;
	    sg = sshade;
	    sshade = NULL;
	    c = node->Upr.Uline.lfill;
	    node->Upr.Uline.lfill = -1.0;
	    pgfstartdraw(0, node, lsp);
	    wpos(X0);
	    popgwarc(node->aat, fabs(node->Upr.Uline.aradius),
		     posangle(X0, node->aat), posangle(X1, node->aat),
		     node->Upr.Uline.endpos.ypos);
	    node->shadedp = sf;
	    sshade = sg;
	    node->Upr.Uline.lfill = c;
	    pgfendpath();
	}
    }
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (firstsegment(node)) {
	snode = node;
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	lth = qenv(node, XLlinethick, tn->lthick);
	if (bfill) {
	    s = node->lthick;
	    node->lthick = 0.0;
	    pgfstartdraw(0, node, XLinvis);
	    node->lthick = s;
	    wpos(node->aat);
	    tx = node;
	    while (tx != NULL) {
		ddash();
		wpos(tx->Upr.Uline.endpos);
		tx = tx->son;
	    }
	    printf("--cycle\n");
	    pgfendpath();
	}
	if (lsp != XLinvis) {
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		sf = node->shadedp;
		node->shadedp = NULL;
		sg = sshade;
		sshade = soutline;
		pgfstartdraw(ahnum(node->Upr.Uline.atype), node, lsp);
		pgfahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth);
		node->shadedp = sf;
		sshade = sg;
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		sf = node->shadedp;
		node->shadedp = NULL;
		sg = sshade;
		sshade = soutline;
		pgfstartdraw(ahnum(node->Upr.Uline.atype), node, lsp);
		pgfahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width), lth);
		node->shadedp = sf;
		sshade = sg;
	    }
	    sf = node->shadedp;
	    node->shadedp = NULL;
	    sg = sshade;
	    sshade = NULL;
	    s = node->Upr.Uline.lfill;
	    node->Upr.Uline.lfill = -1.0;
	    c = vfill;
	    vfill = -1.0;
	    pgfstartdraw(0, node, lsp);
	    node->shadedp = sf;
	    sshade = sg;
	    node->Upr.Uline.lfill = s;
	    vfill = c;
	    wpos(node->aat);
	}
    }
    if (lsp != XLinvis) {
	ddash();
	wpos(node->Upr.Uline.endpos);
	if (node->son == NULL) {
	    pgfendpath();
	}
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		pgfwrtext(snode, With1->textp,
		  0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
		  0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLspline:
    if (firstsegment(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	node->lparam = tn->lparam;
	node->lthick = tn->lthick;
	if (bfill) {
	    c = node->lthick;
	    node->lthick = 0.0;
	    sg = soutline;
	    soutline = sshade;
	    pgfstartdraw(0, node, XLinvis);
	    node->lthick = c;
	    soutline = sg;
	    spltot = primdepth(node);
	    splcount = spltot;
	    tx = node;
	    while (tx != NULL) {
		splinesegment(tx, splcount, spltot);
		splcount--;
		tx = tx->son;
	    }
	    printf(" --cycle");
	    pgfendpath();
	}
	if (lsp != XLinvis) {
	    spltot = primdepth(node);
	    splcount = spltot;
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		sf = node->shadedp;
		node->shadedp = NULL;
		sg = sshade;
		sshade = soutline;
		pgfstartdraw(ahnum(tn->Upr.Uline.atype), node, lsp);
		pgfahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width),
			 qenv(node, XLlinethick, tn->lthick));
		node->shadedp = sf;
		sshade = sg;
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		sf = tn->shadedp;
		tn->shadedp = NULL;
		sg = sshade;
		sshade = soutline;
		pgfstartdraw(ahnum(tn->Upr.Uline.atype), tn, lsp);
		pgfahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(node, XLarrowht, tn->Upr.Uline.height),
			 qenv(node, XLarrowwid, tn->Upr.Uline.width),
			 qenv(node, XLlinethick, tn->lthick));
		node->shadedp = sf;
		sshade = sg;
	    }
	    node->shadedp = NULL;
	    sshade = NULL;
	    node->Upr.Uline.lfill = -1.0;
	    vfill = -1.0;
	    pgfstartdraw(0, node, lsp);
	}
    }
    if (lsp != XLinvis) {
	splinesegment(node, splcount, spltot);
	if (splcount == 1) {
	    pgfendpath();
	}
    }
    splcount--;
    /*D; if debuglevel > 0 then
            writeln(log,'pgfdraw: node ',splcount:1);D*/
    break;

  case XLstring:
    pgfwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
	wstring(&output, node->textp);
	putchar('\n');
    }
    else if (node->lthick >= 0.0) {
	pgfsetthick(node->lthick);
    }
    break;
  }
}  /* pgfdraw */


/* include ps.h */
/* ps.x */
/* Output routines for Postscript, PSfrag */
void pswfloat(FILE **iou, double x)
{ putc(' ', *iou);
  wfloat(iou, x);
  /*DUGHM ;flush(iou) MHGUD*/
}


void psprelude(double n, double s, double e, double w, double lth)
{ double wx, ex, nx, sx;

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
  printf("\n%%%%Creator: dpic version 2015.08.31 option ");
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
  printf("/DpicDict 100 dict def DpicDict begin\n");
  /* writeln('/setlinewid {0 exch 72.27 div 72 mul');
  writeln(' dtransform truncate idtransform setlinewidth pop} def'); */
  /* write('/setlineparms { [] 0 setdash 0 setlinecap 1 setlinejoin'); */
  printf("/setlineparms { [] 0 setdash 0 setlinecap 0 setlinejoin");
  printf(" 10 setmiterlimit} def\n");
  printf("/ostroke { stroke setlineparms } def\n");
  printf("/endstroke { ostroke } def /npath { newpath } def\n");
  printf(" setlineparms\n");
  if ((printstate & 1) != 1) {
      return;
  }
  if (drawmode == PS) {
      printf("/strsiz\n");
      printf(" {newpath 0 0 moveto true charpath flattenpath");
      printf(" pathbbox 4 1 roll pop pop pop}def\n");
      printf("/setcapht {gsave (I) strsiz /capht exch def grestore} def\n");
      printf(
	"/postext {1 add baselineskip mul 1 sub 2 div capht mul add moveto} def\n");
      printf("/strwidth { dup stringwidth pop } def\n");
      printf("/ljust { labelsep } def\n");
      printf("/rjust { strwidth labelsep add neg } def\n");
      printf("/cjust { strwidth 2 div neg } def\n");
      printf("/above { capht 2 div labelsep add } def\n");
      printf("/below { capht 2 div neg labelsep sub } def\n");
      printf("/vjust {0 capht baselineskip mul neg rmoveto\n");
      printf(" currentpoint /y exch def /x exch def} def\n");
  }
  else {
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
{ printf("showpage end\n");
  printf("%%%%EOF\n");
  /*D; if debuglevel > 0 then writeln(log,'pspostlude done');D*/
}


void psnewpath(void)
{ printf("npath\n");
}


void pswcoord(FILE **iou, double x, double y)
{ pswfloat(iou, x / fsc);
  /*DUGHM ;flush(iou) MHGUD*/
  pswfloat(iou, y / fsc);
}


void pswpos(postype pos)
{ pswcoord(&output, pos.xpos, pos.ypos);
}


void pswstring(nametype *p)
{ int i;
  Char c;
  boolean waswhite, iswhite;
  int FORLIM;

  waswhite = false;
  if (p == NULL) {
      return;
  }
  if (p->segmnt == NULL) {
      return;
  }
  FORLIM = p->len;
  for (i = 0; i < FORLIM; i++) {
      c = p->segmnt[p->seginx + i];
      iswhite = (c == etxch || c == nlch || c == tabch || c == ' ');
      if (!iswhite || !waswhite) {
	  if (c == bslch || c == ')' || c == '(') {
	      putchar(bslch);
	  }
	  putchar(c);
      }
      waswhite = iswhite;
  }
}


void pswtext(primitive *np, nametype *tp, double x, double y)
{ int i;
  nametype *tx;
  boolean L, R, A, B;
  double toff, theight;

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
	  if (L) {
	      printf(" ljust");
	  }
	  else if (R) {
	      printf(" rjust");
	  }
	  else {
	      printf(" cjust");
	  }
	  if (A) {
	      printf(" above");
	  }
	  else if (B) {
	      printf(" below");
	  }
	  else {
	      printf(" vcenter");
	  }
	  printf(" rmoveto show x y moveto\n");
	  tp = tp->next_;
      } while (tp != NULL);
      return;
  }
  if (tp == NULL) {
      return;
  }
  toff = venv(np, XLtextoffset) / scale * 72;
  theight = venv(np, XLtextht) / scale * 72;
  printf("(\\\\tex[");
  checkjust(tp, &A, &B, &L, &R);
  if (L) {
      putchar('l');
  }
  else if (R) {
      putchar('r');
  }
  else {
      putchar('c');
  }
  if (A) {
      putchar('b');
  }
  else if (B) {
      putchar('t');
  }
  else {
      putchar('c');
  }
  printf("][lB]");
  if (theight != 0) {
      putchar('[');
      wfloat(&output, theight / 11);
      putchar(']');
  }
  putchar('{');
  if (tp->next_ != NULL) {
      printf("\\\\shortstack{");
  }
  tx = tp;
  do {
      if (L) {
	  printf("\\\\rlap{\\\\hbox to ");
	  wfloat(&output, toff);
	  printf("bp{}");
      }
      else if (R) {
	  printf("\\\\llap{");
      }
      pswstring(tx);
      if (R) {
	  printf("\\\\hbox to ");
	  wfloat(&output, toff);
	  printf("bp{}");
      }
      if (L || R) {
	  putchar('}');
      }
      tx = tx->next_;
      if (tx != NULL) {
	  printf("\\\\\\\\ ");
	  checkjust(tx, &A, &B, &L, &R);
      }
  } while (tx != NULL);
  if (tp->next_ != NULL) {
      printf("}})\n");
  }
  else {
      printf("})\n");
  }
  pswcoord(&output, x, y);
  printf(" moveto");
  checkjust(tp, &A, &B, &L, &R);
  if (L) {
      printf(" ljust");
  }
  else if (R) {
      printf(" rjust");
  }
  else {
      printf(" cjust");
  }
  if (A) {
      printf(" above");
  }
  else if (B) {
      printf(" below");
  }
  else {
      printf(" vcenter");
  }
  printf(" rmoveto show\n");
}


void pswprop(postype p1, postype p2, double a, double b, double c)
{ pprop(p1, &p2, a, b, c);
  pswpos(p2);
}


void pssetthick(double lthk)
{ if (lthk < 0.0 || lthk == gslinethick) {
      return;
  }
  pswfloat(&output, lthk);
  printf(" setlinewidth\n");
  gslinethick = lthk;
}


void pslinearfill(double f, nametype *ss)
{ if (f >= 0.0 && f <= 1.0) {
      printf(" currentrgbcolor");
      pswfloat(&output, f);
      printf(" setgray fill setrgbcolor\n");
      return;
  }
  if (ss == NULL) {
      return;
  }
  printf(" currentrgbcolor");
  putchar(' ');
  wstring(&output, ss);
  printf(" setrgbcolor fill setrgbcolor\n");
}


void pssetcolor(nametype *op)
{ if (op == NULL) {
      return;
  }
  printf(" currentrgbcolor ");
  wstring(&output, op);
  printf(" setrgbcolor\n");
}


void psdashdot(int lspec, double param)
{ if (lspec == XLdashed) {
      if (param == mdistmax) {
	  param = 3 * fsc;
      }
      printf(" [");
      pswfloat(&output, param / fsc);
      pswfloat(&output, param / fsc);
      printf(" ] 0 setdash\n");
      return;
  }
  if (lspec != XLdotted) {
      return;
  }
  if (param == mdistmax) {
      param = 5 * fsc;
  }
  printf(" [ 0");
  pswfloat(&output, param / fsc);
  printf(" ] 0 setdash 1 setlinecap\n");
}


void psendline(nametype *op)
{ printf(" endstroke");
  if (op != NULL) {
      printf(" setrgbcolor\n");
  }
  else {
      putchar('\n');
  }
}


void pswarc(postype C, postype S, postype E, double r, double ccw)
{ double y;

  pswpos(C);
  pswfloat(&output, r / fsc);
  y = datan(S.ypos - C.ypos, S.xpos - C.xpos);
  pswfloat(&output, 180.0 / pi * y);
  y = datan(E.ypos - C.ypos, E.xpos - C.xpos);
  pswfloat(&output, 180.0 / pi * y);
  if (ccw >= 0.0) {
      printf(" arc\n");
  }
  else {
      printf(" arcn\n");
  }
}


void psarcahead(postype C, int atyp, postype *point, double ht,
		       double wid, double lth, double radius, double angle)
{ postype P, Q, Co, Ci, Px, Cox, Cix, Ao, Ai;
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
  }
  else {
      pswpos(Ai);
      printf(" lineto\n");
  }
  printf(" fill\n");
  *point = P;
}


void psahead(int atyp, postype *point, postype shaft, double ht,
		    double wid, double lth)
{ postype P, L, R, Px, Lx, Rx, Q;
  double x, y;

  dahead(*point, shaft, ht, wid, lth / 72 * scale, &P, &L, &R, &Px, &Lx, &Rx,
	 &Q, &x, &y);
  psnewpath();
  if (atyp == 0) {
      pswprop(P, R, x - y, y, x);
      printf(" moveto");
      pswpos(P);
      printf(" lineto\n");
      pswprop(P, L, x - y, y, x);
      printf(" lineto");
      psendline(NULL);
  }
  else {
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
      printf(" closepath fill\n");
  }
  *point = P;
}


void psbox(postype aat, double halfwid, double halfht, double rad)
{ int i;
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
	  if (i == 1 || i == 3) {
	      putchar('\n');
	  }
      }
  }
  else {
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
{ psnewpath();
  printf(" 0 0");
  pswfloat(&output, fabs(rd) / fsc);
  printf(" 0 360 arc closepath");
}


#define r               ((sqrt(2.0) - 1) * 4 / 3)


void psellipse(double x, double y)
{ x = fabs(x) / 2;
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


void pssplinesegment(primitive *tv, int splc, int splt)
{ if (tv == NULL) {
      return;
  }
  if (splt == 1) {
      pswpos(tv->aat);
      printf(" moveto");
      pswpos(tv->Upr.Uline.endpos);
      printf(" lineto\n");
      return;
  }
  if (tv->Upr.Uline.aradius == mdistmax) {
      if (splc == splt && splc > 1) {  /* 1st seg */
	  pswpos(tv->aat);
	  printf(" moveto\n");
	  pswprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  printf(" lineto\n");
	  pswprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  return;
      }
      if (splc > 1) {  /* interior segment */
	  pswprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	  pswprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  printf(" curveto\n");
	  pswprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  return;
      }
      pswprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
      pswprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      printf(" curveto\n");
      pswpos(tv->Upr.Uline.endpos);
      printf(" lineto\n");
      return;
  }
  if (splc == splt && splc > 1) {
      pswpos(tv->aat);
      printf(" moveto\n");
      pswprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	      tv->Upr.Uline.aradius, 1.0);
      return;
  }
  if (splc > 1) {
      pswprop(tv->aat, tv->Upr.Uline.endpos, 1 + tv->Upr.Uline.aradius,
	      1 - tv->Upr.Uline.aradius, 2.0);
      pswprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      printf(" curveto\n");
      pswprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	      1 + tv->Upr.Uline.aradius, 2.0);
      return;
  }
  pswprop(tv->aat, tv->Upr.Uline.endpos, tv->Upr.Uline.aradius,
	  1 - tv->Upr.Uline.aradius, 1.0);
  pswpos(tv->Upr.Uline.endpos);
  printf(" curveto\n");

  /* last segment */
}


void psdraw(primitive *node)
{ /* node is always <> nil */
  int lsp;
  postype X1, X2;
  primitive *tn, *tx;
  double h, w, lth, fill;
  int TEMP;
  primitive *With1;

  getlinespec(node, &lsp, &tn);
  lth = qenv(node, XLlinethick, node->lthick);          /* printobject(node); */
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
    if (node->Upr.Ubox.boxfill >= 0.0 && node->Upr.Ubox.boxfill <= 1.0 ||
	 node->shadedp != NULL) {
	psbox(node->aat, node->Upr.Ubox.boxwidth / 2,
	      node->Upr.Ubox.boxheight / 2, node->Upr.Ubox.boxradius);
	pssetthick(lth);
	if (lsp != XLinvis) {
	    printf(" gsave\n");
	}
	if (node->shadedp == NULL) {
	    printf(" currentrgbcolor");
	    pswfloat(&output, node->Upr.Ubox.boxfill);
	    printf(" setgray");
	}
	else {
	    pssetcolor(node->shadedp);
	}
	printf(" fill");
	if (lsp != XLinvis) {
	    printf(" grestore\n");
	}
	else {
	    putchar('\n');
	}
	psdashdot(lsp, node->lparam);
	pssetcolor(node->outlinep);
	psendline(node->outlinep);
	printf(" setrgbcolor");
	printf(" setlineparms\n");
    }
    else if (lsp != XLinvis) {
	psbox(node->aat, node->Upr.Ubox.boxwidth / 2,
	      node->Upr.Ubox.boxheight / 2, node->Upr.Ubox.boxradius);
	pssetthick(lth);
	psdashdot(lsp, node->lparam);
	pssetcolor(node->outlinep);
	psendline(node->outlinep);
	printf(" setlineparms\n");
    }
    pswtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XBLOCK:
    pswtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLellipse:
  case XLcircle:
    if (node->ptype == XLellipse) {
	fill = node->Upr.Uellipse.efill;
    }
    else {
	fill = node->Upr.Ucircle.cfill;
    }
    if (fill >= 0.0 && fill <= 1.0 || node->shadedp != NULL) {
	pssetthick(lth);
	printf(" gsave ");
	pswpos(node->aat);
	printf(" translate\n");
	if (node->ptype == XLellipse) {
	    psellipse(node->Upr.Uellipse.elwidth, node->Upr.Uellipse.elheight);
	}
	else {
	    pscircle(node->Upr.Ucircle.radius);
	}
	printf(" gsave ");
	if (node->shadedp == NULL) {
	    pswfloat(&output, fill);
	    printf(" setgray");
	}
	else {
	    wstring(&output, node->shadedp);
	    printf(" setrgbcolor");
	}
	printf(" fill grestore\n");
	if (lsp != XLinvis) {
	    psdashdot(lsp, node->lparam);
	    pssetcolor(node->outlinep);
	    psendline(node->outlinep);
	    printf(" setlineparms\n");
	}
	printf(" grestore\n");
    }
    else if (lsp != XLinvis) {
	pssetthick(lth);
	printf(" gsave ");
	pswpos(node->aat);
	printf(" translate\n");
	if (node->ptype == XLellipse) {
	    psellipse(node->Upr.Uellipse.elwidth, node->Upr.Uellipse.elheight);
	}
	else {
	    pscircle(node->Upr.Ucircle.radius);
	}
	psdashdot(lsp, node->lparam);
	pssetcolor(node->outlinep);
	psendline(node->outlinep);
	printf(" setlineparms\n");
	printf(" grestore\n");
    }
    pswtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
  case XLspline:
    if (firstsegment(node)) {
	snode = node;
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    psnewpath();
	    if (node->ptype != XLspline) {
		pswpos(node->aat);
		printf(" moveto\n");
	    }
	    spltot = primdepth(node);
	    splcount = spltot;
	    tx = node;
	    while (tx != NULL) {
		if (node->ptype == XLspline) {
		    pssplinesegment(tx, splcount, spltot);
		    splcount--;
		}
		else {
		    pswpos(tx->Upr.Uline.endpos);
		    printf(" lineto\n");
		}
		tx = tx->son;
	    }
	    pssetthick(0.0);
	    pslinearfill(vfill, sshade);
	    vfill = -1.0;
	    sshade = NULL;
	}
	if (lsp != XLinvis) {
	    lth = qenv(tn, XLlinethick, tn->lthick);
	    spltot = primdepth(node);
	    splcount = spltot;
	    pssetthick(lth);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		pssetcolor(soutline);
		psahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			node->Upr.Uline.endpos,
			qenv(tn, XLarrowht, tn->Upr.Uline.height),
			qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth);
		if (soutline != NULL) {
		    printf(" setrgbcolor\n");
		}
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		pssetcolor(soutline);
		psahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			tn->aat, qenv(tn, XLarrowht, tn->Upr.Uline.height),
			qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth);
		if (soutline != NULL) {
		    printf(" setrgbcolor\n");
		}
	    }
	    if (node->ptype != XLspline) {
		psnewpath();
		pswpos(node->aat);
		printf(" moveto\n");
	    }
	}
    }
    if (lsp != XLinvis) {
	if (node->ptype == XLspline) {
	    pssplinesegment(node, splcount, spltot);
	}
	else {
	    pswpos(node->Upr.Uline.endpos);
	    printf(" lineto\n");
	}
	if (node->son == NULL) {
	    psdashdot(lsp, node->lparam);
	    pssetcolor(soutline);
	    psendline(soutline);
	}
    }
    splcount--;
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		pswtext(node, With1->textp,
			0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
			0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLmove:
    if (firstsegment(node)) {
	snode = node;
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		pswtext(node, With1->textp,
			0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
			0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLarc:
    if (drawn(node, lsp, node->Upr.Uline.lfill)) {
	pssetthick(lth);
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	X1 = arcstart(node);
	X2 = arcend(node);
	if (bfill) {
	    printf(" currentrgbcolor\n");
	    psnewpath();
	    pswarc(node->aat, X1, X2, node->Upr.Uline.aradius,
		   node->Upr.Uline.endpos.ypos);
	    pssetthick(0.0);
	    pslinearfill(vfill, sshade);
	    vfill = -1.0;
	    sshade = NULL;
	    printf(" setrgbcolor\n");
	}
	if (lsp != XLinvis) {
	    pssetthick(lth);
	    TEMP = ahlex(node->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		pssetcolor(soutline);
		startarc(node, X1, lth, &h, &w);
		psarcahead(node->aat, ahnum(node->Upr.Uline.atype), &X1, h, w,
			   lth, fabs(node->Upr.Uline.aradius),
			   node->Upr.Uline.endpos.ypos);
		if (soutline != NULL) {
		    printf(" setrgbcolor\n");
		}
	    }
	    TEMP = ahlex(node->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		pssetcolor(soutline);
		endarc(node, X2, lth, &h, &w);
		psarcahead(node->aat, ahnum(node->Upr.Uline.atype), &X2, h, w,
			   lth, -fabs(node->Upr.Uline.aradius),
			   node->Upr.Uline.endpos.ypos);
		if (soutline != NULL) {
		    printf(" setrgbcolor\n");
		}
	    }
	    psnewpath();
	    pswarc(node->aat, X1, X2, node->Upr.Uline.aradius,
		   node->Upr.Uline.endpos.ypos);
	    psdashdot(lsp, node->lparam);
	    pssetcolor(soutline);
	    psendline(soutline);
	}
	printf(" setlineparms\n");
    }
    pswtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLstring:
    pswtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
	wstring(&output, node->textp);
	putchar('\n');
    }
    else if (node->lthick >= 0.0) {
	pssetthick(node->lthick);
    }
    break;
  }
  /*DUGHM ;flush(output) MHGUD*/
}  /* psdraw */


/* include pdf.h */
/* pdf.x */
/* Output routines for PDF */

void pdfstream(Char *s, int ls, nametype **strm)
{ int i, ll, s0;
  nametype *ns, *With;

  s0 = 1;
  while (ls > 0) {
      With = *strm;
      if (With->len + ls - 1 <= CHBUFSIZ) {
	  ll = ls;
      }
      else {
	  ll = CHBUFSIZ - With->len + 1;
      }
      With = *strm;
      for (i = 0; i < ll; i++) {
	  With->segmnt[With->len + i] = s[s0 + i - 1];
      }
      s0 += ll;
      With->len += ll;
      if ((*strm)->len > CHBUFSIZ) {
	  newstr(&ns);
	  ns->segmnt = Malloc(sizeof(chbufarray));
	  (*strm)->next_ = ns;
	  *strm = ns;
      }
      ls -= ll;
  }
}


void pdfwln(Char *s, int ln, nametype **strm)
{ Char STR1[256];

  pdfstream(s, ln, strm);
  sprintf(STR1, "%c", nlch);
  pdfstream(STR1, 1, strm);
}


void addbytes(int n)
{ if (pdfobjcount == 0) {
      pdfoffs[0] = n;
  }
  else {
      pdfoffs[pdfobjcount] = pdfoffs[pdfobjcount - 1] + n;
  }
  pdfobjcount++;
}


void pdfprelude(double n, double s, double e, double w, double lth)
{ double wx, ex, nx, sx;

  pdfobjcount = 0;
  printf("%%PDF-1.4\n");
  printf("%% Creator: dpic version 2015.08.31 option -d for PDF\n");
  addbytes(62);                                 /* pdfobjcount must be 1 here */

  /* 123456789 123456789 123456789 123456789 123456789 123456789 12345*/
  /* 1. 2. 3. 4. 5. 6. */
  printf("%d 0 obj << /Type /Catalog /Pages 2 0 R >> endobj\n", pdfobjcount);
  addbytes(49);

  printf("%d 0 obj << /Type /Pages /Kids [ 3 0 R ] /Count 1 >> endobj\n",
	 pdfobjcount);
  addbytes(59);

  printf("%d 0 obj << /Type /Page /Parent 2 0 R\n", pdfobjcount);
  wx = w / fsc - lth / 2;
  ex = e / fsc + lth / 2;
  nx = n / fsc + lth / 2;
  sx = s / fsc - lth / 2;
  printf(" /MediaBox [ ");
  printf("%5d %5d %5d %5d ]\n",
	 Floor((long)floor(wx * 1000000L + 0.5) / 1000000.0),
	 Floor((long)floor(sx * 1000000L + 0.5) / 1000000.0),
	 Ceil((long)floor(ex * 1000000L + 0.5) / 1000000.0),
	 Ceil((long)floor(nx * 1000000L + 0.5) / 1000000.0));
  /*D; if debuglevel > 0 then begin
     write(log,'lth= '); wfloat(log,lth);
     write(log,' wx='); wfloat(log,wx);
     write(log,' Floor wx='); wfloat(log,Floor(round(wx*1000000)/1000000.0));
     writeln(log)
     end; D*/
  printf(" /Contents 6 0 R\n");
  printf(" /Resources <<\n");
  printf("   /ProcSet %d 0 R\n", pdfobjcount + 1);
  printf("   /Font << /F1 %d 0 R >>\n", pdfobjcount + 2);
  printf("   >>\n");
  printf(" >> endobj\n");
  addbytes(168);

  printf("%d 0 obj [/PDF /Text] endobj\n", pdfobjcount);
  addbytes(28);

  printf("%d 0 obj << /Type /Font /Subtype /Type1\n", pdfobjcount);
  printf(" /Name /F1 /BaseFont /Courier              \n");
  printf(" /Encoding /WinAnsiEncoding\n");
  printf(" >> endobj\n");
  addbytes(122);

  /* writeln(pdfobjcount:0,
              ' 0 obj << /Type /Font /Subtype /Type1');
     writeln(' /Name /F2 /BaseFont /Helvetica-BoldOblique');
     writeln(' /Encoding /WinAnsiEncoding');
     writeln(' >> endobj');
       addbytes(39+44+28+11); */

  /* initialize system parameters */
  gsocolor = false;
  gsfcolor = false;
  gsgcolor = false;
  gslinecap = 0;
  gslinejoin = 0;
  gsdashw = 0.0;
  gsdashs = 0.0;
  newstr(&stream);
  stream->segmnt = Malloc(sizeof(chbufarray));
  cx = stream;
  pdfwln(" 0.8 w", 6, &cx);
  gslinethick = 0.8;
}


void pdfwfloat(double y)
{ int ix, j, k, ln;
  Char ts[10];
  boolean nz;
  Char STR1[256];

  pdfstream(" ", 1, &cx);
  if (y < 0) {
      pdfstream("-", 1, &cx);
      y = -y;
  }
  if (y == (long)y) {
      j = 7;
      ix = (long)floor(y + 0.5);
      nz = true;
  }
  else {
      j = 0;
      ix = (long)floor(y * 1000000L + 0.5);
      nz = false;
  }
  ln = 0;
  do {
      j++;
      k = ix % 10;
/* p2c: pdf.h, line 115:
 * Note: Using % for possibly-negative arguments [317] */
      if (j == 7 && nz) {
	  ln++;
	  ts[ln - 1] = '.';
      }
      else if (j == 7) {
	  nz = true;
      }
      else if (!nz) {
	  nz = (k != 0);
      }
      if (nz) {
	  ln++;
	  ts[ln - 1] = k + '0';
      }
      ix /= 10;
  } while (ix != 0 || j <= 6);
  for (k = 1; k <= ln; k++) {
      sprintf(STR1, "%c", ts[ln - k]);
      pdfstream(STR1, 1, &cx);
  }
}


void resetgs(primitive *node)
{ double x;

  if (gsocolor) {
      pdfwln(" 0 0 0 RG", 9, &cx);
      gsocolor = false;
  }
  if (gsfcolor) {
      pdfwln(" 0 0 0 rg", 9, &cx);
      gsfcolor = false;
  }
  if (gsgcolor) {
      pdfwln(" 0 g", 4, &cx);
      gsgcolor = false;
  }
  x = venv(node, XLlinethick);
  if (gslinethick != x) {
      pdfwfloat(x);
      pdfwln(" w", 2, &cx);
      gslinethick = x;
  }
  if (gslinecap != 0) {
      pdfwln(" 0 J", 4, &cx);
      gslinecap = 0;
  }
  if (gslinejoin != 0) {
      pdfwln(" 0 j", 4, &cx);
      gslinejoin = 0;
  }
  if (gsdashw == 0 && gsdashs == 0) {
      return;
  }
  pdfwln(" [] 0 d", 7, &cx);
  gsdashw = 0.0;
  gsdashs = 0.0;
}


void pdfwlz(int n)
{ Char s[10];
  int i, j;

  j = 10;
  while (n > 0) {
      s[j - 1] = n % 10 + '0';
/* p2c: pdf.h, line 147:
 * Note: Using % for possibly-negative arguments [317] */
      j--;
      n /= 10;
  }
  for (i = 1; i <= j; i++) {
      putchar('0');
  }
  for (i = j; i <= 9; i++) {
      putchar(s[i]);
  }
  printf(" 00000 n \n");
}


void pdfpostlude(void)
{ nametype *s, *t;
  int i, streamlen;
  nametype *With;
  int FORLIM;

  streamlen = 0;
  s = stream;
  while (s != NULL) {
      streamlen += s->len;
      s = s->next_;
  }
  printf("%d 0 obj << /Length %6d >>\n", pdfobjcount, streamlen);
  printf("stream\n");
  s = stream;
  while (s != NULL) {
      With = s;
      FORLIM = With->len;
      for (i = 0; i < FORLIM; i++) {
	  putchar(With->segmnt[i]);
      }
      s = With->next_;
  }
  printf("endstream\n");
  printf("endobj\n");
  addbytes(streamlen + 53);

  printf("xref\n");
  printf("0 %d\n", pdfobjcount);
  printf("0000000000 65535 f \n");
  FORLIM = pdfobjcount - 2;
  for (i = 0; i <= FORLIM; i++) {
      pdfwlz(pdfoffs[i]);
  }
  printf("trailer\n");
  printf("   << /Size %d\n", pdfobjcount);
  printf("      /Root 1 0 R\n");
  printf("   >>\n");
  printf("startxref\n");
  printf("%d\n", pdfoffs[pdfobjcount - 1]);
  printf("%%%%EOF\n");
  /* delete output buffers */
  while (stream != NULL) {
      s = stream;
      t = NULL;
      while (s->next_ != NULL) {
	  t = s;
	  s = s->next_;
      }
      Free(s->segmnt);
      Free(s);
      if (t == NULL) {
	  stream = NULL;
      }
      else {
	  t->next_ = NULL;
      }
  }
  /*D; if debuglevel > 0 then writeln(log,'pdfpostlude done');D*/
}


void pdfwcoord(double x, double y)
{ pdfwfloat(x / fsc);
  pdfwfloat(y / fsc);
}


void pdfwpos(postype pos)
{ pdfwcoord(pos.xpos, pos.ypos);
}


void pdfwstring(nametype *p)
{ int i;
  Char c;
  boolean waswhite, iswhite;
  int FORLIM;
  Char STR1[256];

  if (p == NULL) {
      return;
  }
  if (p->segmnt == NULL) {
      return;
  }
  waswhite = false;
  FORLIM = p->len;
  for (i = 0; i < FORLIM; i++) {
      c = p->segmnt[p->seginx + i];
      iswhite = (c == etxch || c == nlch || c == tabch || c == ' ');
      if (!iswhite || !waswhite) {
	  if (c == bslch || c == ')' || c == '(') {
	      sprintf(STR1, "%c", bslch);
	      pdfstream(STR1, 1, &cx);
	  }
	  sprintf(STR1, "%c", c);
	  pdfstream(STR1, 1, &cx);
      }
      waswhite = iswhite;
  }
}


void pdfwtext(primitive *node, nametype *tp, double x, double y)
{ int nstr;
  nametype *tx;
  boolean L, R, A, B;
  double textht, textwid, textoff, dx, dy, ratio;

  ratio = 13.0 / 11;
  resetgs(node);
  if (tp == NULL) {
      return;
  }
  tx = tp;
  nstr = 0;
  while (tx != NULL) {
      nstr++;
      tx = tx->next_;
  }
  textht = venv(node, XLtextht);
  textwid = venv(node, XLtextwid);
  if (node->ptype == XLstring && nstr > 0) {
      textht = node->Upr.Ubox.boxheight / nstr;         /*/((nstr-1)*ratio+1); */
      textwid = node->Upr.Ubox.boxwidth;
  }
  textoff = venv(node, XLtextoffset);
  /*D if debuglevel>0 then begin
    write(log,' pdfwtext: x=',x:8:3,'[',x/fsc:8:3,']');
    writeln(log, ' y=',y:8:3,'[',y/fsc:8:3,']');
    writeln(log,' textoff=',textoff:8:3,'[',textoff/fsc:8:3,']');
    writeln(log,' textht=',textht:8:3,'[',textht/fsc:8:3,']',
                ' textwid=',textwid:8:3,'[',textwid/fsc:8:3,']') end; D*/
  y += (nstr / 2.0 - 4.0 / 5) * textht;
  nstr = 1;
  do {
      if (textwid <= 0 || nstr > 1) {
	  textwid = tp->len * textht * 0.6;
      }
      checkjust(tp, &A, &B, &L, &R);
      pdfwln("BT", 2, &cx);
      pdfstream("/F1", 3, &cx);
      pdfwfloat(textht / scale * 72);
      pdfwln(" Tf", 3, &cx);
      if (L) {
	  dx = textoff;
      }
      else if (R) {
	  dx = -textoff - tp->len * textht * 0.6;
      }
      else if (node->ptype == XLstring) {
	  dx = textwid / -2;
      }
      else {
	  dx = -tp->len * textht * 0.6 / 2;
      }
      dy = textht / 10;
      if (A) {
	  dy += textoff + textht * 0.3;
      }
      else if (B) {
	  dy += textht * (1 - ratio) * 0.7 - textoff;
      }
      /*D if debuglevel>0 then begin
        writeln(log,' A=',A,' B=',B,' L=',L,' R=',R);
        writeln(log,' dx=',dx:8:3,'[',dx/fsc:8:3,']',
                    ' dy=',dy:8:3,'[',dy/fsc:8:3,']');
        writeln(log,' x=',x:8:3,'[',x/fsc:8:3,']',
                    ' y=',y:8:3,'[',y/fsc:8:3,']');
        writeln(log,' x+dx=',(x+dx):8:3,'[',(x+dx)/fsc:8:3,']',
                    ' y+dy=',(y+dy):8:3,'[',(y+dy)/fsc:8:3,']') end; D*/
      pdfwcoord(Max(0.0, x + dx), Max(0.0, y + dy));
      pdfwln(" Td", 3, &cx);
      pdfstream("(", 1, &cx);
      pdfwstring(tp);
      pdfwln(") Tj", 4, &cx);
      pdfwln("ET", 2, &cx);
      y -= textht;
      nstr++;
      tp = tp->next_;
  } while (tp != NULL);
}


void pdfwprop(postype p1, postype p2, double a, double b, double c)
{ pprop(p1, &p2, a, b, c);
  pdfwpos(p2);
}


void pdfsetthick(double lt)
{ if (lt < 0.0 || lt == gslinethick) {
      return;
  }
  pdfwfloat(lt);
  pdfwln(" w", 2, &cx);
  gslinethick = lt;
}


void pdflinearfill(double f, nametype *ss)
{ if (f >= 0.0 && f <= 1.0) {
      pdfwfloat(f);
      pdfwln(" g", 2, &cx);
      gsgcolor = true;
      return;
  }
  if (ss == NULL) {
      return;
  }
  pdfstream(" ", 1, &cx);
  pdfwstring(ss);
  pdfwln(" rg", 3, &cx);
  gsfcolor = true;
}


void pdfsetcolor(nametype *op, boolean f)
{ if (op == NULL) {
      return;
  }
  pdfstream(" ", 1, &cx);
  pdfwstring(op);
  if (f) {
      pdfwln(" rg", 3, &cx);
      gsfcolor = true;
  }
  else {
      pdfwln(" RG", 3, &cx);
      gsocolor = true;
  }
}


void pdflineopts(int lspec, double param, double thck, nametype *op)
{ pdfsetthick(thck);
  if (lspec == XLdashed) {
      if (param == mdistmax) {
	  param = 6 * fsc;
      }
      pdfstream(" [", 2, &cx);
      pdfwfloat(param * 7 / 6 / fsc);
      pdfwfloat(param * 5 / 6 / fsc);
      pdfwln(" ] 0 d 0 j", 10, &cx);
      gsdashw = param;
      gsdashs = param;
  }
  else if (lspec == XLdotted) {
      if (param == mdistmax) {
	  param = 4 * fsc;
      }
      pdfstream(" [", 2, &cx);
      pdfwfloat(0.0);
      pdfwfloat(param / fsc);
      pdfwln("] 0 d 1 J", 9, &cx);
      gsdashw = 0.0;
      gsdashs = param;
  }
  pdfsetcolor(op, false);
}


void pdfwarc(postype Ctr, postype St, postype En, double radius,
		    double ccw)
{ int narcs, i;
  double c, s, cc, ss, startangle, endangle, arcangle;
  postype Q;

  startangle = datan(St.ypos - Ctr.ypos, St.xpos - Ctr.xpos);
  endangle = datan(En.ypos - Ctr.ypos, En.xpos - Ctr.xpos);
  if (ccw > 0 && endangle < startangle) {
      endangle += 2 * pi;
  }
  else if (ccw < 0 && endangle > startangle) {
      endangle -= 2 * pi;
  }
  narcs = (long)(1.0 + fabs(endangle - startangle) / (pi / 2));
  arcangle = (endangle - startangle) / narcs;
  c = cos(arcangle / 2);
  s = sin(arcangle / 2);
  cc = (4 - c) / 3;
  if (s != 0.0) {
      ss = (1.0 - c * cc) / s;
  }
  else {
      ss = 0.0;
  }
  for (i = 1; i <= narcs; i++) {
      Q.xpos = cos(startangle + (i - 0.5) * arcangle);
      Q.ypos = sin(startangle + (i - 0.5) * arcangle);
      pdfwpos(affine(radius * cc, -radius * ss, Ctr, Q));
      pdfwpos(affine(radius * cc, radius * ss, Ctr, Q));
      Q.xpos = Ctr.xpos + radius * cos(startangle + i * arcangle);
      Q.ypos = Ctr.ypos + radius * sin(startangle + i * arcangle);
      pdfwpos(Q);
      pdfwln(" c", 2, &cx);
  }
}


void pdfarcahead(postype C, int atyp, postype *point, double ht,
			double wid, double lth, double radius, double angle,
			nametype *sou)
{ postype P, Q, Co, Ci, Px, Cox, Cix, Ao, Ai;
  double ccw, lwi;
  boolean startarrow;

  arcahead(C, *point, atyp, ht, wid, lth, radius, angle, &P, &Co, &Ci, &Px,
	   &Cox, &Cix, &Ao, &Ai, &ccw, &lwi, &startarrow);
  radius = fabs(radius);
  /* Trace arrowhead outline */
  pdfwpos(Ai);
  pdfwln(" m", 2, &cx);
  pdfwarc(Ci, Ai, *point, radius, -ccw);
  pdfwarc(Co, *point, Ao, radius, ccw);
  if (atyp == 0 && lwi < (wid - lwi) / 2) {
      Q = Co;
      pprop(Ao, &Q, radius - lwi, lwi, radius);
      pdfwpos(Q);
      pdfwln(" l", 2, &cx);
      pdfwarc(Co, Q, P, radius - lwi, -ccw);
      Q = Ci;
      pprop(Ai, &Q, radius + lwi, -lwi, radius);
      pdfwarc(Ci, P, Q, radius + lwi, ccw);
  }
  if (atyp == 3 && lwi < (wid - lwi) / 2) {
      pdfwarc(Cox, Ao, Px, radius, -ccw);
      pdfwarc(Cix, Px, Ai, radius, ccw);
  }
  else {
      pdfwpos(Ai);
      pdfwln(" l", 2, &cx);
  }
  pdfwln(" f", 2, &cx);
  *point = P;
}


void pdfahead(int atyp, postype *point, postype shaft, double ht,
		     double wid, double lth, nametype *sou)
{ postype P, L, R, Px, Lx, Rx, Q;
  double x, y;

  dahead(*point, shaft, ht, wid, lth / 72 * scale, &P, &L, &R, &Px, &Lx, &Rx,
	 &Q, &x, &y);
  if (atyp == 0) {
      pdfsetthick(lth);
      pdfsetcolor(sou, false);
      pdfwprop(P, R, x - y, y, x);
      pdfstream(" m", 2, &cx);
      pdfwpos(P);
      pdfwln(" l", 2, &cx);
      pdfwprop(P, L, x - y, y, x);
      pdfstream(" l", 2, &cx);
      pdfwln(" S", 2, &cx);
  }
  else {
      pdfsetcolor(sou, true);
      pdfwpos(Rx);
      pdfwln(" m", 2, &cx);
      pdfwpos(*point);
      pdfstream(" l", 2, &cx);
      pdfwpos(Lx);
      pdfstream(" l", 2, &cx);
      if (atyp == 3) {
	  pdfwpos(Px);
	  pdfstream(" l", 2, &cx);
      }
      pdfwln(" h f", 4, &cx);
  }
  *point = P;
}


#define r               (1 - (sqrt(2.0) - 1) * 4 / 3)


/* function pdfpos(x,y:real): postype;
var
  Q: postype;
begin
   Q.xpos := x; Q.ypos := y;
   pdfpos := Q
   end; */

void pdfbox(postype aat, double halfwid, double halfht, double rad)
{ double n, s, e, w;

  rad = Min(Min(fabs(rad), fabs(halfht)), fabs(halfwid));
  n = aat.ypos + halfht;
  s = aat.ypos - halfht;
  e = aat.xpos + halfwid;
  w = aat.xpos - halfwid;
  if (rad == 0.0) {
      pdfwcoord(w, s);
      pdfwcoord(2 * halfwid, 2 * halfht);
      pdfwln(" re", 3, &cx);
      return;
  }
  pdfwcoord(e, n - rad);
  pdfwln(" m", 2, &cx);
  pdfwcoord(e, n - rad * r);
  pdfwcoord(e - rad * r, n);
  pdfwcoord(e - rad, n);
  pdfwln(" c", 2, &cx);
  pdfwcoord(w + rad, n);
  pdfwln(" l", 2, &cx);
  pdfwcoord(w + rad * r, n);
  pdfwcoord(w, n - rad * r);
  pdfwcoord(w, n - rad);
  pdfwln(" c", 2, &cx);
  pdfwcoord(w, s + rad);
  pdfwln(" l", 2, &cx);
  pdfwcoord(w, s + rad * r);
  pdfwcoord(w + rad * r, s);
  pdfwcoord(w + rad, s);
  pdfwln(" c", 2, &cx);
  pdfwcoord(e - rad, s);
  pdfwln(" l", 2, &cx);
  pdfwcoord(e - rad * r, s);
  pdfwcoord(e, s + rad * r);
  pdfwcoord(e, s + rad);
  pdfstream(" c h", 4, &cx);
}

#undef r


#define r               ((sqrt(2.0) - 1) * 4 / 3)


void pdfellipse(postype Ctr, double x, double y)
{ x = fabs(x) / 2;
  y = fabs(y) / 2;
  pdfwcoord(Ctr.xpos + x, Ctr.ypos);
  pdfwln(" m", 2, &cx);
  pdfwcoord(Ctr.xpos + x, Ctr.ypos + y * r);
  pdfwcoord(Ctr.xpos + x * r, Ctr.ypos + y);
  pdfwcoord(Ctr.xpos, Ctr.ypos + y);
  pdfwln(" c", 2, &cx);
  pdfwcoord(Ctr.xpos - x * r, Ctr.ypos + y);
  pdfwcoord(Ctr.xpos - x, Ctr.ypos + y * r);
  pdfwcoord(Ctr.xpos - x, Ctr.ypos);
  pdfwln(" c", 2, &cx);
  pdfwcoord(Ctr.xpos - x, Ctr.ypos - y * r);
  pdfwcoord(Ctr.xpos - x * r, Ctr.ypos - y);
  pdfwcoord(Ctr.xpos, Ctr.ypos - y);
  pdfwln(" c", 2, &cx);
  pdfwcoord(Ctr.xpos + x * r, Ctr.ypos - y);
  pdfwcoord(Ctr.xpos + x, Ctr.ypos - y * r);
  pdfwcoord(Ctr.xpos + x, Ctr.ypos);
  pdfstream(" c h", 4, &cx);
}

#undef r


void pdfsplinesegment(primitive *tv, int splc, int splt)
{ if (tv == NULL) {
      return;
  }
  if (splt == 1) {
      pdfwpos(tv->aat);
      pdfwln(" m", 2, &cx);
      pdfwpos(tv->Upr.Uline.endpos);
      pdfstream(" l", 2, &cx);
      return;
  }
  if (tv->Upr.Uline.aradius == mdistmax) {
      if (splc == splt) {  /* 1st seg */
	  pdfwpos(tv->aat);
	  pdfwln(" m", 2, &cx);
	  pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  pdfwln(" l", 2, &cx);
	  pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  return;
      }
      if (splc > 1) {  /* interior segment */
	  pdfwprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	  pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	  pdfwln(" c", 2, &cx);
	  pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	  return;
      }
      pdfwprop(tv->aat, tv->Upr.Uline.endpos, 5.0, 1.0, 6.0);
      pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      pdfwln(" c", 2, &cx);
      pdfwpos(tv->Upr.Uline.endpos);
      pdfstream(" l", 2, &cx);
      return;
  }
  if (splc == splt) {
      pdfwpos(tv->aat);
      pdfwln(" m", 2, &cx);
      pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	       tv->Upr.Uline.aradius, 1.0);
      return;
  }
  if (splc > 1) {
      pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1 + tv->Upr.Uline.aradius,
	       1 - tv->Upr.Uline.aradius, 2.0);
      pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1.0, 1.0, 2.0);
      pdfwln(" c", 2, &cx);
      pdfwprop(tv->aat, tv->Upr.Uline.endpos, 1 - tv->Upr.Uline.aradius,
	       1 + tv->Upr.Uline.aradius, 2.0);
      return;
  }
  pdfwprop(tv->aat, tv->Upr.Uline.endpos, tv->Upr.Uline.aradius,
	   1 - tv->Upr.Uline.aradius, 1.0);
  pdfwpos(tv->Upr.Uline.endpos);
  pdfstream(" c", 2, &cx);

  /* last segment */
}


void pdfdraw(primitive *node)
{ /* node is always <> nil */
  int lsp;
  postype X1, X2;
  primitive *tn, *tx;
  double h, w, x, y, lth, fill;
  boolean fll;
  int TEMP;
  Char STR1[256];
  primitive *With1;

  getlinespec(node, &lsp, &tn);                   /* lsp = dotted, dashed ... */
  lth = qenv(node, XLlinethick, node->lthick);          /* printobject(node); */
  /*D if debuglevel > 0 then begin write(log,'pdfdraw: ');
     snaptype(log,ptype); D*/
  /*D
           write(log,' lth='); wfloat(log,lth);
           writeln(log)
           end;
        if linesignal > 0 then begin write(errout,'pdfdraw: ');
           snaptype(errout,ptype); writeln(errout) end D*/
  switch (node->ptype) {

  case XBLOCK:
    pdfwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLbox:
    if (drawn(node, lsp, node->Upr.Ubox.boxfill)) {
	fll = (node->Upr.Ubox.boxfill >= 0.0 && node->Upr.Ubox.boxfill <= 1.0 ||
	       node->shadedp != NULL);
	pdflinearfill(node->Upr.Ubox.boxfill, node->shadedp);
	pdflineopts(lsp, node->lparam, lth, node->outlinep);
	pdfbox(node->aat, node->Upr.Ubox.boxwidth / 2,
	       node->Upr.Ubox.boxheight / 2, node->Upr.Ubox.boxradius);
	if (fll && lsp != XLinvis) {
	    pdfwln(" B", 2, &cx);
	}
	else if (fll) {
	    pdfwln(" f", 2, &cx);
	}
	else {
	    pdfwln(" S", 2, &cx);
	}
    }
    pdfwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLellipse:
  case XLcircle:
    if (node->ptype == XLellipse) {
	x = node->Upr.Uellipse.elwidth;
	y = node->Upr.Uellipse.elheight;
	fill = node->Upr.Uellipse.efill;
    }
    else {
	x = 2 * node->Upr.Ucircle.radius;
	y = x;
	fill = node->Upr.Ucircle.cfill;
    }
    if (drawn(node, lsp, fill)) {
	fll = (fill >= 0.0 && fill <= 1.0 || node->shadedp != NULL);
	pdflinearfill(fill, node->shadedp);
	pdflineopts(lsp, node->lparam, lth, node->outlinep);
	pdfellipse(node->aat, x, y);
	if (fll && lsp != XLinvis) {
	    pdfwln(" B", 2, &cx);
	}
	else if (fll) {
	    pdfwln(" f", 2, &cx);
	}
	else {
	    pdfwln(" S", 2, &cx);
	}
    }
    pdfwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
    X1 = arcstart(node);
    X2 = arcend(node);
    if (bfill) {
	pdflinearfill(vfill, sshade);
	pdfwpos(X1);
	pdfwln(" m", 2, &cx);
	pdfwarc(node->aat, X1, X2, node->Upr.Uline.aradius,
		node->Upr.Uline.endpos.ypos);
	pdfwln(" f", 2, &cx);
	resetgs(node);
    }
    if (lsp != XLinvis) {
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	    pdfsetcolor(soutline, true);
	    startarc(node, X1, lth, &h, &w);
	    pdfarcahead(node->aat, ahnum(node->Upr.Uline.atype), &X1, h, w, lth,
			fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, soutline);
	    resetgs(node);
	}
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	    pdfsetcolor(soutline, true);
	    endarc(node, X2, lth, &h, &w);
	    pdfarcahead(node->aat, ahnum(node->Upr.Uline.atype), &X2, h, w, lth,
			-fabs(node->Upr.Uline.aradius),
			node->Upr.Uline.endpos.ypos, soutline);
	    resetgs(node);
	}
	pdflineopts(lsp, node->lparam, lth, soutline);
	pdfwpos(X1);
	pdfwln(" m", 2, &cx);
	pdfwarc(node->aat, X1, X2, node->Upr.Uline.aradius,
		node->Upr.Uline.endpos.ypos);
	pdfwln(" S", 2, &cx);
    }
    pdfwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLline:
  case XLarrow:
  case XLmove:
    if (firstsegment(node)) {
	snode = node;
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    pdflinearfill(vfill, sshade);
	    tx = node;
	    pdfwpos(node->aat);
	    pdfwln(" m", 2, &cx);
	    while (tx != NULL) {
		pdfwpos(tx->Upr.Uline.endpos);
		pdfwln(" l", 2, &cx);
		tx = tx->son;
	    }
	    pdfwln(" f", 2, &cx);
	    resetgs(node);
	}
	lth = qenv(tn, XLlinethick, tn->lthick);
	if (lsp != XLinvis) {
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		pdfsetcolor(soutline, true);
		pdfahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
		resetgs(node);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		pdfsetcolor(soutline, true);
		pdfahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
		resetgs(node);
	    }
	    pdflineopts(lsp, node->lparam, lth, soutline);
	    pdfwpos(node->aat);
	    pdfwln(" m", 2, &cx);
	}
    }
    if (lsp != XLinvis) {
	pdfwpos(node->Upr.Uline.endpos);
	pdfstream(" l", 2, &cx);
	if (node->son != NULL) {
	    sprintf(STR1, "%c", nlch);
	    pdfstream(STR1, 1, &cx);
	}
	else {
	    pdfwln(" S", 2, &cx);
	}
    }
    if (node->son == NULL) {
	while (snode != NULL) {
	    With1 = snode;
	    if (With1->textp != NULL) {
		pdfwtext(snode, With1->textp,
		  0.5 * (With1->Upr.Uline.endpos.xpos + With1->aat.xpos),
		  0.5 * (With1->aat.ypos + With1->Upr.Uline.endpos.ypos));
	    }
	    snode = snode->son;
	}
    }
    break;

  case XLspline:
    if (firstsegment(node)) {
	getlinshade(node, &tn, &sshade, &soutline, &vfill, &bfill);
	if (bfill) {
	    spltot = primdepth(node);
	    splcount = spltot;
	    pdflinearfill(vfill, sshade);
	    tx = node;
	    while (tx != NULL) {
		With1 = tx;
		pdfsplinesegment(tx, splcount, spltot);
		splcount--;
		tx = tx->son;
	    }
	    pdfwln(" f", 2, &cx);
	    resetgs(node);
	}
	if (lsp != XLinvis) {
	    spltot = primdepth(node);
	    splcount = spltot;
	    lth = qenv(tn, XLlinethick, tn->lthick);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		pdfsetcolor(soutline, true);
		pdfahead(ahnum(tn->Upr.Uline.atype), &node->aat,
			 node->Upr.Uline.endpos,
			 qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
		resetgs(node);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		pdfsetcolor(soutline, true);
		pdfahead(ahnum(tn->Upr.Uline.atype), &tn->Upr.Uline.endpos,
			 tn->aat, qenv(tn, XLarrowht, tn->Upr.Uline.height),
			 qenv(tn, XLarrowwid, tn->Upr.Uline.width), lth,
			 soutline);
		resetgs(node);
	    }
	    pdflineopts(lsp, tn->lparam, lth, soutline);
	}
    }
    if (lsp != XLinvis) {
	pdfsplinesegment(node, splcount, spltot);
	if (splcount == 1) {
	    pdfwln(" S", 2, &cx);
	    resetgs(node);
	}
    }
    /*D; if debuglevel > 0 then writeln(log,'node ',splcount:1) ;D*/
    splcount--;
    break;

  case XLstring:
    pdfwtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLaTeX:
    if (node->textp != NULL) {
	pdfwstring(node->textp);
	sprintf(STR1, "%c", nlch);
	pdfstream(STR1, 1, &cx);
    }
    break;
  }
  /*DUGHM ;flush(output) MHGUD*/
}  /* pdfdraw */


/* # include 'mps.h' */

/* include tex.h */
/* tex.x */
/* Output routines for TeX, tTeX (eepicemu), Pict2e */

void texprelude(double n, double s, double e, double w)
{ printf("\\setlength{\\unitlength}{1in}\n");
  if (s != distmax) {
      printf("\\begin{picture}");
      wcoord(&output, e - w, n - s);
      wcoord(&output, w, s);
      printf("\n\\thicklines\n");
  }
  printf("%% dpic version 2015.08.31 option ");
  switch (drawmode) {

  case TeX:
    printf("(none, LaTeX picture assumed)");
    break;

  case Pict2e:
    printf("-e (Pict2e)");
    break;

  case tTeX:
    printf("-t (eepicemu)");
    break;
  }
  printf(" for LaTeX\n");
}


void texpostlude(void)
{ printf("\\end{picture}\n");
  /*D if debuglevel > 0 then writeln(log,'texpostlude done');D*/
}


void arrowhead(double pointx, double pointy, double tailx,
		      double taily)
{ double x, y, r, ct;

  r = linlen(pointx - tailx, pointy - taily);
  if (r == 0.0) {
      return;
  }
  ct = Min(0.05, 0.5 * r) / r;
  x = pointx + ct * (tailx - pointx);
  y = pointy + ct * (taily - pointy);
  printf("\n\\put");
  wcoord(&output, x, y);
  printf("{\\vector");
  wrslope(pointx - x, pointy - y, true);
  putchar('}');
}


void texwrtext(primitive *np, nametype *tp, double x, double y)
{ boolean A, B, L, R;

  if (tp == NULL) {
      return;
  }
  printf("\\put");
  checkjust(tp, &A, &B, &L, &R);
  if (A) {
      wcoord(&output, x, y + 0.1);
  }
  else if (B) {
      wcoord(&output, x, y - 0.05);
  }
  else {
      wcoord(&output, x, y);
  }
  checkjust(tp, &A, &B, &L, &R);
  putchar('{');
  printf("\\makebox(0,0)");
  if (L) {
      printf("[l]");
  }
  else if (R) {
      printf("[r]");
  }
  putchar('{');
  texstacktext(np, tp);
  printf("}}\n");
}


void p2ahead(postype *point, postype shaft, double ht)
{ postype butx;

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
{ if (lt < 0.0 || lt == gslinethick) {
      return;
  }
  printf("\\linethickness{");
  wfloat(&output, lt);
  printf("pt}\n");
  gslinethick = lt;
}


void texdraw(primitive *node)
{ int i, npts, lsp;
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
	if (drawmode == Pict2e) {
	    p2setthick(lth);
	}
	printf("\\put");
	wcoord(&output, west, south);
	if (node->ptype == XBLOCK) {
	    printf("{\\makebox");
	}
	else if (lsp == XLsolid) {
	    printf("{\\framebox");
	}
	else if (lsp == XLinvis) {
	    printf("{\\makebox");
	}
	else if (lsp == XLdashed) {
	    printf("{\\dashbox");
	    if (node->lparam != mdistmax) {
		wbrace(node->lparam / fsc);
	    }
	    else {
		wbrace(venv(node, XLdashwid) / fsc);
	    }
	}
	else if (lsp == XLdotted) {
	    printf("{\\dashbox");
	    printf("{0.01}");
	}
	if (node->ptype == XLbox) {
	    wcoord(&output, node->Upr.Ubox.boxwidth, node->Upr.Ubox.boxheight);
	}
	else {
	    wcoord(&output, node->Upr.Ublock.blockwidth, node->Upr.Ublock.blockheight);
	}
	putchar('{');
	texstacktext(node, node->textp);
	printf("}}\n");
    }
    break;

  case XLline:
  case XLarrow:
    if (drawn(node, lsp, -1.0)) {
	if (firstsegment(node)) {  /* first segment */
	    if (drawmode == Pict2e) {
		p2setthick(lth);
	    }
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		p2ahead(&node->aat, node->Upr.Uline.endpos,
			qenv(node, XLarrowht, tn->Upr.Uline.height));
	    }
	}
	TEMP = ahlex(node->Upr.Uline.atype);
	if ((node->son == NULL) & (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD)) {
	    p2ahead(&node->Upr.Uline.endpos, node->aat,
		    qenv(node, XLarrowht, tn->Upr.Uline.height));
	}
	lgth = linlen(node->Upr.Uline.endpos.xpos - node->aat.xpos,
		      node->Upr.Uline.endpos.ypos - node->aat.ypos);
	if (drawmode == Pict2e ||
	     lsp == XLsolid && (lgth > 0.18 || drawmode == tTeX)) {
	    if (lgth > 0) {
		printf("\\put");
		wpos(node->aat);
		printf("{\\line");
		wrslope(node->Upr.Uline.endpos.xpos - node->aat.xpos,
			node->Upr.Uline.endpos.ypos - node->aat.ypos, false);
		printf("}\n");
	    }
	}
	else if (lsp == XLsolid) {
	    npts = (long)floor(lgth / 0.005 + 0.5);
	    printf("\\multiput");
	    wpos(node->aat);
	    wcoord(&output,
		   (node->Upr.Uline.endpos.xpos - node->aat.xpos) / npts,
		   (node->Upr.Uline.endpos.ypos - node->aat.ypos) / npts);
	    printf("{%d}", npts + 1);
	    printf("{\\makebox(0.005555,0.00825){\\tiny .}}\n");
	}
	else if (lsp == XLdashed) {
	    printf("\\put");
	    wpos(node->aat);
	    printf("{\\dashline[50]");
	    if (node->lparam != mdistmax) {
		wbrace(node->lparam / fsc);
	    }
	    else {
		wbrace(venv(node, XLdashwid) / fsc);
	    }
	    printf("(0,0)");
	    wcoord(&output, node->Upr.Uline.endpos.xpos - node->aat.xpos,
		   node->Upr.Uline.endpos.ypos - node->aat.ypos);
	    printf("}\n");
	}
	else if (lsp == XLdotted) {
	    printf("\\dottedline{");
	    wfloat(&output, 0.05 / fsc);
	    putchar('}');
	    wpos(node->aat);
	    wpos(node->Upr.Uline.endpos);
	    putchar('\n');
	}
    }
    texwrtext(node, node->textp,
	      0.5 * (node->Upr.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->Upr.Uline.endpos.ypos));
    break;

  case XLspline:
    if ((drawmode == Pict2e) & drawn(node, lsp, -1.0)) {
	if (firstsegment(node)) {  /* first spline */
	    spltot = primdepth(node);
	    splcount = spltot;
	    tmpat = node->aat;
	    p2setthick(lth);
	    TEMP = ahlex(tn->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
		p2ahead(&node->aat, node->Upr.Uline.endpos,
			qenv(node, XLarrowht, tn->Upr.Uline.height));
	    }
	    /* p2lineopts(lsp,lparam,outlinep); */
	    if (spltot > 1 && node->Upr.Uline.aradius == mdistmax) {
		printf("\\put");
		wcoord(&output, node->aat.xpos, node->aat.ypos);
		printf("{\\line");
		wrslope((node->Upr.Uline.endpos.xpos - node->aat.xpos) / 2,
			(node->Upr.Uline.endpos.ypos - node->aat.ypos) / 2,
			false);
		printf("}\n");
		printf("\\cbezier");
		wprop(node->Upr.Uline.endpos, node->aat, 1.0, 1.0, 2.0);
		wprop(node->aat, node->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	    }
	    else if (spltot > 1) {
		printf("\\cbezier");
		wpos(node->aat);
		node->aat = tmpat;
		wprop(node->aat, node->Upr.Uline.endpos,
		      1 - node->Upr.Uline.aradius, node->Upr.Uline.aradius, 1.0);
	    }
	}
	else if (splcount > 1 && node->Upr.Uline.aradius == mdistmax) {
	    wprop(node->aat, node->Upr.Uline.endpos, 5.0, 1.0, 6.0);
	    wprop(node->aat, node->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	    printf("%%\n");
	    printf("\\cbezier");
	    wprop(node->aat, node->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	    wprop(node->aat, node->Upr.Uline.endpos, 1.0, 5.0, 6.0);
	}
	else if (splcount > 1) {
	    wprop(node->aat, node->Upr.Uline.endpos,
		  0.5 + node->Upr.Uline.aradius / 2,
		  0.5 - node->Upr.Uline.aradius / 2, 1.0);
	    wprop(node->aat, node->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	    printf("%%\n");
	    printf("\\cbezier");
	    wprop(node->aat, node->Upr.Uline.endpos, 1.0, 1.0, 2.0);
	    wprop(node->aat, node->Upr.Uline.endpos,
		  0.5 - node->Upr.Uline.aradius / 2,
		  0.5 + node->Upr.Uline.aradius / 2, 1.0);
	}
	if (splcount == 1) {
	    tmpat = node->Upr.Uline.endpos;
	    TEMP = ahlex(node->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		x = linlen(node->Upr.Uline.endpos.xpos - node->aat.xpos,
			   node->Upr.Uline.endpos.ypos - node->aat.ypos);
		y = qenv(node, XLarrowht, tn->Upr.Uline.height);
		pprop(node->aat, &node->Upr.Uline.endpos, y, x - y, x);
	    }
	    if (spltot > 1 && node->Upr.Uline.aradius == mdistmax) {
		wprop(node->aat, node->Upr.Uline.endpos, 5.0, 1.0, 6.0);
		wprop(node->aat, node->Upr.Uline.endpos, 1.0, 1.0, 2.0);
		printf("%%\n");
		printf("\\put");
		wprop(node->aat, node->Upr.Uline.endpos, 1.0, 1.0, 2.0);
		printf("{\\line");
		wrslope((node->Upr.Uline.endpos.xpos - node->aat.xpos) / 2,
			(node->Upr.Uline.endpos.ypos - node->aat.ypos) / 2,
			false);
		printf("}\n");
	    }
	    else if (spltot > 1) {
		wprop(node->aat, node->Upr.Uline.endpos, node->Upr.Uline.aradius,
		      1 - node->Upr.Uline.aradius, 1.0);
		wpos(node->Upr.Uline.endpos);
		printf("%%\n");
	    }
	    TEMP = ahlex(node->Upr.Uline.atype);
	    if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
		p2ahead(&tmpat, node->aat,
			qenv(node, XLarrowht, tn->Upr.Uline.height));
	    }
	}
	/*D if debuglevel > 0 then writeln(log,'node ',splcount:1) ;D*/
	splcount--;
    }
    else if (lsp == XLdotted || lsp == XLdashed || lsp == XLsolid) {
	if (firstsegment(node) & ((ahlex(node->Upr.Uline.atype) == XDOUBLEHEAD) |
				  (ahlex(node->Upr.Uline.atype) == XLEFTHEAD)))
	{                                                     /* first spline */
	    arrowhead(node->aat.xpos, node->aat.ypos,
		      node->Upr.Uline.endpos.xpos, node->Upr.Uline.endpos.ypos);
	}
	printf("\\spline");
	wpos(node->aat);
	wpos(node->Upr.Uline.endpos);
	if (node->son == NULL) {
	    putchar('\n');
	}
	else if (node->son->son == NULL) {
	    wpos(node->son->Upr.Uline.endpos);
	    if ((ahlex(node->Upr.Uline.atype) == XRIGHTHEAD) |
		(ahlex(node->Upr.Uline.atype) == XDOUBLEHEAD)) {
		arrowhead(node->son->Upr.Uline.endpos.xpos,
			  node->son->Upr.Uline.endpos.ypos, node->son->aat.xpos,
			  node->son->aat.ypos);
	    }
	    node->ptype = XLline;
	    putchar('\n');
	    /* son := nil */
	    deletetree(&node->son);
	}
	else {
	    pprop(node->son->Upr.Uline.endpos, &node->son->aat, 1.0, 1.0, 2.0);
	    wpos(node->son->aat);
	    node->ptype = XLline;
	    putchar('\n');
	}
    }
    break;

  case XLmove:
    texwrtext(node, node->textp,
	      0.5 * (node->Upr.Uline.endpos.xpos + node->aat.xpos),
	      0.5 * (node->aat.ypos + node->Upr.Uline.endpos.ypos));
    break;

  case XLellipse:
    if (drawmode == Pict2e) {
	p2setthick(lth);
    }
    printf("\\put");
    wpos(node->aat);
    if (drawmode == TeX) {
	printf("{\\ellipse");
	wbrace(node->Upr.Uellipse.elwidth / fsc);
	wbrace(node->Upr.Uellipse.elheight / fsc);
	printf("}\n");
    }
    else {
	printf("{\\oval");
	wcoord(&output, node->Upr.Uellipse.elwidth, node->Upr.Uellipse.elheight);
	printf("}\n");
    }
    texwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLcircle:
    if (lsp != XLinvis) {
	if (drawmode == Pict2e) {
	    p2setthick(lth);
	}
	printf("\\put");
	wpos(node->aat);
	printf("{\\circle");
	wbrace(2.0 * node->Upr.Ucircle.radius / fsc);
	printf("}\n");
    }
    texwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    break;

  case XLarc:
    if ((drawmode == Pict2e) & drawn(node, lsp, -1.0)) {
	p2setthick(node->lthick);                          /* p2linecap(lsp); */
	X0 = arcstart(node);
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XLEFTHEAD) {
	    if (node->Upr.Uline.aradius == 0.0) {
		x = 0.0;
	    }
	    else {
		x = atan(qenv(node, XLarrowht, node->Upr.Uline.height) /
			 node->Upr.Uline.aradius);
	    }
	    if (node->Upr.Uline.endpos.ypos >= 0.0) {
		node->Upr.Uline.endpos.xpos += x;
		node->Upr.Uline.endpos.ypos -= x;
	    }
	    else {
		node->Upr.Uline.endpos.xpos -= x;
		node->Upr.Uline.endpos.ypos += x;
	    }
	    tmpat = X0;
	    X0 = arcstart(node);
	    p2ahead(&tmpat, X0, qenv(node, XLarrowht, node->Upr.Uline.height));
	}
	X3 = arcend(node);
	TEMP = ahlex(node->Upr.Uline.atype);
	if (TEMP == XDOUBLEHEAD || TEMP == XRIGHTHEAD) {
	    if (node->Upr.Uline.aradius == 0.0) {
		x = 0.0;
	    }
	    else {
		x = atan(qenv(node, XLarrowht, node->Upr.Uline.height) /
			 node->Upr.Uline.aradius);
	    }
	    if (node->Upr.Uline.endpos.ypos >= 0.0) {
		node->Upr.Uline.endpos.ypos -= x;
	    }
	    else {
		node->Upr.Uline.endpos.ypos += x;
	    }
	    tmpat = X3;
	    X3 = arcend(node);
	    p2ahead(&tmpat, X3, qenv(node, XLarrowht, node->Upr.Uline.height));
	}
	npts = (long)(1.0 + fabs(node->Upr.Uline.endpos.ypos) / pi);
	node->Upr.Uline.endpos.ypos /= npts;
	while (npts > 0) {
	    printf("\\cbezier");
	    wpos(X0);
	    x = cos(node->Upr.Uline.endpos.ypos / 2);
	    y = sin(node->Upr.Uline.endpos.ypos / 2);
	    x1 = (4 - x) / 3;
	    if (y != 0.0) {
		y1 = (1.0 - x * x1) / y;
	    }
	    else {
		y1 = 0.0;
	    }
	    tmpat.xpos = cos(
		node->Upr.Uline.endpos.xpos + node->Upr.Uline.endpos.ypos / 2.0);
	    tmpat.ypos = sin(
		node->Upr.Uline.endpos.xpos + node->Upr.Uline.endpos.ypos / 2.0);
	    wpos(affine(node->Upr.Uline.aradius * x1,
			-node->Upr.Uline.aradius * y1, node->aat, tmpat));
	    wpos(affine(node->Upr.Uline.aradius * x1,
			node->Upr.Uline.aradius * y1, node->aat, tmpat));
	    if (npts > 1) {
		node->Upr.Uline.endpos.xpos += node->Upr.Uline.endpos.ypos;
		X0 = arcstart(node);
		wpos(X0);
		printf("%%\n");
	    }
	    npts--;
	}
	wpos(X3);
	printf("%%\n");                    /*; p2dashdot(lsp,lparam,outlinep) */
    }
    else if (iscorner(principal(node->Upr.Uline.endpos.xpos, 0.5 * pi)) &&
	     fabs(fabs(node->Upr.Uline.endpos.ypos) - pi / 2.0) < 0.001) {
	if (drawmode == Pict2e) {
	    p2setthick(lth);
	}
	printf("\\put");
	wpos(node->aat);
	printf("{\\oval");
	wcoord(&output, 2 * node->Upr.Uline.aradius / fsc,
	       2 * node->Upr.Uline.aradius / fsc);
	if (node->direction != XLdown && node->direction != XLup &&
	     node->direction != XLright && node->direction != XLleft) {
	    p = findenv(node);
	    if (p != NULL) {
		node->direction = p->direction;
	    }
	}
	if (node->direction == XLleft && node->Upr.Uline.endpos.ypos < 0.0 ||
	     node->direction == XLdown && node->Upr.Uline.endpos.ypos > 0.0) {
	    printf("[bl]}\n");
	}
	else if (node->direction == XLleft && node->Upr.Uline.endpos.ypos > 0.0 ||
		 node->direction == XLup && node->Upr.Uline.endpos.ypos < 0.0) {
	    printf("[tl]}\n");
	}
	else if (node->direction == XLright && node->Upr.Uline.endpos.ypos < 0.0 ||
		 node->direction == XLup && node->Upr.Uline.endpos.ypos > 0.0) {
	    printf("[tr]}\n");
	}
	else {
	    printf("[br]}\n");
	}
	texwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    }
    else if ((drawmode == Pict2e || drawmode == TeX) &&
	     node->Upr.Uline.aradius > 0.0) {
	if (node->Upr.Uline.aradius >= 0.05 / sin(pi / 18.0)) {
	    npts = (long)floor(fabs(node->Upr.Uline.endpos.ypos) / (pi / 18.0) + 0.5);
	    if (npts < 4) {
		npts = 4;
	    }
	}
	else {
	    npts = (long)floor(fabs(node->Upr.Uline.endpos.ypos) / atan(
				 0.05 / node->Upr.Uline.aradius) + 0.5);
	}
	r = node->Upr.Uline.endpos.ypos / npts;
	ct = cos(r);
	st = sin(r);
	x = node->Upr.Uline.aradius * cos(node->Upr.Uline.endpos.xpos);
	y = node->Upr.Uline.aradius * sin(node->Upr.Uline.endpos.xpos);
	/*D if debuglevel = 2 then writeln(log,'x,y',x:7:4,y:7:4); D*/
	for (i = 1; i <= npts; i++) {
	    x1 = ct * x - st * y;
	    y1 = st * x + ct * y;
	    /*D if debuglevel = 2 then writeln(log,x1:7:4,y1:7:4); D*/
	    printf("\\put");
	    wcoord(&output, node->aat.xpos + x, node->aat.ypos + y);
	    printf("{\\line");
	    wcoord(&output, x1 - x, y1 - y);
	    if (fabs(x1 - x) != 0.0) {
		wbrace(fabs(x1 - x) / fsc);
	    }
	    else {
		wbrace(fabs(y1 - y) / fsc);
	    }
	    printf("}\n");
	    if ((i == 1) & ((ahlex(node->Upr.Uline.atype) == XDOUBLEHEAD) |
			    (ahlex(node->Upr.Uline.atype) == XLEFTHEAD))) {
		arrowhead(node->aat.xpos + x, node->aat.ypos + y,
			  node->aat.xpos + x1, node->aat.ypos + y1);
	    }
	    else if ((i == npts) & ((ahlex(node->Upr.Uline.atype) == XDOUBLEHEAD) |
		       (ahlex(node->Upr.Uline.atype) == XRIGHTHEAD))) {
		arrowhead(node->aat.xpos + x1, node->aat.ypos + y1,
			  node->aat.xpos + x, node->aat.ypos + y);
	    }
	    x = x1;
	    y = y1;
	}
	texwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
    }
    break;

  case XLstring:
    texwrtext(node, node->textp, node->aat.xpos, node->aat.ypos);
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
{ while (node != NULL) {
      switch (drawmode) {

      case TeX:
      case tTeX:
      case Pict2e:
	texdraw(node);
	break;

      case PDF:
	pdfdraw(node);
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
	break;

      case SVG:
	svgdraw(node);
	break;

      case xfig:
	xfigdraw(node);
	break;
      }
      if (node->son != NULL) {
	  treedraw(node->son);
      }
      if (drawmode == PDF) {
	  resetgs(node);
      }
      else if (drawmode == xfig && node->ptype == XBLOCK &&
	       node->direction == -1) {
	  printf("-6\n");
      }
      bfill = false;
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
         TeX,tTeX,Pict2e: texdraw(node);
         PDF: pdfdraw(node);
         PGF: pgfdraw(node);
         PSTricks: pstdraw(node);
         MFpic: mfpdraw(node);
         PS,PSfrag: psdraw(node);
         */
/* PSmps: mpsdraw(node); */
/*
         MPost: begin mpodraw(node); if bfill then mpodraw(node) end;
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
      bfill := false; sshade := nil; soutline := nil;
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
{ /* n,s,e,w: real; eb: primitivep */
  double fsctmp;

  bfill = false;
  sshade = NULL;
  soutline = NULL;
  switch (drawmode) {

  case SVG:
    fsctmp = fsc;
    fsc /= SVGPX;                                            /* output pixels */
    svgprelude(n, s, e, w, venv(eb, XLlinethick) / 72 * scale);
    treedraw(eb);
    svgpostlude();
    fsc = fsctmp;
    break;

  case PGF:                                                        /* n,s,e,w */
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
    fsc /= 72;                                               /* output points */
    psprelude(n, s, e, w, venv(eb, XLlinethick));
    treedraw(eb);
    pspostlude();
    fsc = fsctmp;
    break;

  case PDF:
    fsctmp = fsc;
    fsc /= 72;                                               /* output points */
    pdfprelude(n, s, e, w, venv(eb, XLlinethick));
    treedraw(eb);
    pdfpostlude();
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
    fsc /= 72;                                               /* output points */
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
  case Pict2e:
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
{ int cf, i, j;

  j = FILENAMELEN;
  i = 0;
  while (j > i) {
      if (ifn[j - 1] == ' ') {
	  j--;
      }
      else {
	  i = j;
      }
  }
  if (j < FILENAMELEN) {
      j++;
  }
  else {
      fatal(1);
  }
  ifn[j - 1] = '\0';
  cf = access(ifn, 4);
  /*G cf := 4-Access(ifn,4); G*/
  if (!(isverbose && cf != 0)) {
      return cf;
  }
  fprintf(errout, " *** dpic: Searching for file \"");
  for (i = 0; i <= j - 2; i++) {
      putc(ifn[i], errout);
  }
  fprintf(errout, "\" returns %d\n", cf);
  return cf;
}


void openerror(void)
{ errout = stderr;
}


void openfiles(void)
{ /*DUGHM if (oflag>0) then openlogfile; MHGUD*/
  savebuf = NULL;
  output = stdout; input = stdin;

  if (argct >= P_argc) {
      return;
  }

  P_sun_argv(infname, sizeof(mstring), argct);
  /*GH if argct <= ParamCount then begin
     infname := ParamStr(argct); HG*/

  if (checkfile(infname, true) != 0) {
      fatal(1);
  }
  if (input != NULL) {
      input = freopen(P_trimname(infname, sizeof(mstring)), "r", input);
  }
  else {
      input = fopen(P_trimname(infname, sizeof(mstring)), "r");
  }
  if (input == NULL) {
      _EscIO(FileNotFound);
  }
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

void markerror(int emi)
{ /* emi: integer */
  int inx, j, k;
  fbuffer *thisbuf, *lastbuf, *With;

  if (emi < 900) {                                   /* Do not count warnings */
      errcount++;
  }
  /*D if debuglevel > 0 then begin
      write(log,'*** Markerror[');
      if inbuf=nil then write(log,'nil') else write(log,ordp(inbuf):1);
      writeln(log,'] emi=', emi:1, ', lexsymb=', lexsymb:1,':');
      wrbuf(inbuf,3,0) end; D*/
  /* Write out the current and prev line */
  thisbuf = inbuf;
  lastbuf = NULL;
  while (thisbuf != NULL) {
      With = thisbuf;
      lastbuf = thisbuf;
      j = 0;
      if (With->readx - 1 < With->savedlen) {
	  inx = With->readx - 1;
      }
      else {
	  inx = With->savedlen;
      }
      while (inx > j) {
	  if (With->carray[inx] == etxch || With->carray[inx] == tabch ||
	       With->carray[inx] == ' ' || With->carray[inx] == crch ||
	       With->carray[inx] == nlch) {
	      inx--;
	  }
	  else {
	      j = inx;
	  }
      }
      j = 0;
      while (inx > j) {
	  if (With->carray[inx] != nlch) {
	      inx--;
	  }
	  else {
	      j = inx;
	  }
      }
      if (inx == 0) {
	  thisbuf = With->prevb;
      }
      else {
	  thisbuf = NULL;
      }
  }
  while (lastbuf != NULL) {
      if (lastbuf == inbuf) {
	  k = lastbuf->readx - 1;
      }
      else {
	  k = lastbuf->savedlen;
      }
      for (j = inx; j <= k; j++) {
	  putc(lastbuf->carray[j], errout);
      }
      if (lastbuf != inbuf) {
	  lastbuf = lastbuf->nextb;
      }
      else {
	  lastbuf = NULL;
      }
      if (lastbuf != NULL) {
	  inx = lastbuf->readx;
      }
  }
  fprintf(errout, "\n*** dpic: line %d ", lineno);
  if (emi < 900) {
      fprintf(errout, "ERROR: ");
  }
  else {
      fprintf(errout, "WARNING: ");
  }
  if (emi < 800) {
      switch (lexsymb) {

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

      case 138:
	fprintf(errout, "color or colored or colour or coloured");
	break;

      case 139:
	fprintf(errout, "outline or outlined");
	break;

      case 140:
	fprintf(errout, "shade or shaded");
	break;

      case 142:
	fprintf(errout, "center or centre");
	break;

      case 143:
	fprintf(errout, "ljust");
	break;

      case 144:
	fprintf(errout, "rjust");
	break;

      case 145:
	fprintf(errout, "above");
	break;

      case 146:
	fprintf(errout, "below");
	break;

      case 148:
	fprintf(errout, "<-");
	break;

      case 149:
	fprintf(errout, "->");
	break;

      case 150:
	fprintf(errout, "<->");
	break;

      case 152:
	fprintf(errout, "up");
	break;

      case 153:
	fprintf(errout, "down");
	break;

      case 154:
	fprintf(errout, "right");
	break;

      case 155:
	fprintf(errout, "left");
	break;

      case 157:
	fprintf(errout, "box");
	break;

      case 158:
	fprintf(errout, "circle");
	break;

      case 159:
	fprintf(errout, "ellipse");
	break;

      case 160:
	fprintf(errout, "arc");
	break;

      case 161:
	fprintf(errout, "line");
	break;

      case 162:
	fprintf(errout, "arrow");
	break;

      case 163:
	fprintf(errout, "move");
	break;

      case 164:
	fprintf(errout, "spline");
	break;

      case 166:
	fprintf(errout, "arcrad");
	break;

      case 167:
	fprintf(errout, "arrowht");
	break;

      case 168:
	fprintf(errout, "arrowwid");
	break;

      case 169:
	fprintf(errout, "boxht");
	break;

      case 170:
	fprintf(errout, "boxrad");
	break;

      case 171:
	fprintf(errout, "boxwid");
	break;

      case 172:
	fprintf(errout, "circlerad");
	break;

      case 173:
	fprintf(errout, "dashwid");
	break;

      case 174:
	fprintf(errout, "ellipseht");
	break;

      case 175:
	fprintf(errout, "ellipsewid");
	break;

      case 176:
	fprintf(errout, "lineht");
	break;

      case 177:
	fprintf(errout, "linewid");
	break;

      case 178:
	fprintf(errout, "moveht");
	break;

      case 179:
	fprintf(errout, "movewid");
	break;

      case 180:
	fprintf(errout, "textht");
	break;

      case 181:
	fprintf(errout, "textoffset");
	break;

      case 182:
	fprintf(errout, "textwid");
	break;

      case 183:
	fprintf(errout, "arrowhead");
	break;

      case 184:
	fprintf(errout, "fillval");
	break;

      case 185:
	fprintf(errout, "linethick");
	break;

      case 186:
	fprintf(errout, "maxpsht");
	break;

      case 187:
	fprintf(errout, "maxpswid");
	break;

      case 188:
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
      " sh variable + - ( corner Here ! string Label float function sprintf enumerated obj\n");
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
    fprintf(errout,
	    " + - string ! sprintf constant variable ( function location\n");
    break;

  case 17:
    fprintf(errout, " ( corner Here Label enumerated obj\n");
    break;

  case 18:
    fprintf(errout, " between of < , + -\n");
    break;

  case 19:
    fprintf(errout, " ,\n");
    break;

  case 20:
    fprintf(errout, " logic opr\n");
    break;

  case 21:
    fprintf(errout, " the\n");
    break;

  case 22:
    fprintf(errout, " way\n");
    break;

  case 23:
    fprintf(errout, " between\n");
    break;

  case 24:
    fprintf(errout, " and\n");
    break;

  case 25:
    fprintf(errout, " ) ,\n");
    break;

  case 26:
    fprintf(errout, " variable\n");
    break;

  case 27:
    fprintf(errout, " =\n");
    break;

  case 28:
    fprintf(errout, " ) ||\n");
    break;

  case 29:
    fprintf(errout, " ) + -\n");
    break;

  case 30:
    fprintf(errout, " , + -\n");
    break;

  case 31:
    fprintf(errout, " ] + -\n");
    break;

  case 32:
    fprintf(errout, " ' + -\n");
    break;

  case 33:
    fprintf(errout, " } + -\n");
    break;

  case 34:
    fprintf(errout, " ;\n");
    break;

  case 35:
    fprintf(errout, " [ []\n");
    break;

  case 36:
    fprintf(errout, " to ,\n");
    break;

  case 37:
    fprintf(errout, " + - do by\n");
    break;

  case 38:
    fprintf(errout, " do + -\n");
    break;

  case 39:
    fprintf(errout, " if\n");
    break;

  case 40:
    fprintf(errout, " then ||\n");
    break;

  case 41:
    fprintf(errout, " name Label\n");
    break;

  case 42:
    fprintf(errout,
      " at corner . + - ( Here ! Label constant variable function enumerated obj\n");
    break;

  case 43:
    fprintf(errout, " at\n");
    break;

  case 44:
    fprintf(errout, " at of Label enumerated obj\n");
    break;

  case 45:
    fprintf(errout, " {\n");
    break;

  case 46:
    fprintf(errout, " }\n");
    break;

  case 47:
    fprintf(errout, " envvar\n");
    break;

  case 48:
    fprintf(errout, " logic opr string sprintf\n");
    break;

  case 49:
    fprintf(errout, " }\n");
    break;

  case 50:
    fprintf(errout, " :\n");
    break;

  case 51:
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
  case 807:
    fprintf(errout, "End of file while reading ");
    if (emi == 807) {
	fprintf(errout, "string in ");
    }
    switch (currprod) {

    case elsehead1:
      fprintf(errout, "else");
      break;

    case ifhead1:
      fprintf(errout, "if");
      break;

    case forhead1:
      fprintf(errout, "for");
      break;

    case defhead1:
    case defhead2:
      fprintf(errout, "define");
      break;
    }
    fprintf(errout, " {...} contents\n");
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

  case 903:
    fprintf(errout, "Picture size adjusted to maxpswid value\n");
    break;

  case 904:
    fprintf(errout, "Picture size adjusted to maxpsht value\n");
    break;

  /*905: writeln(errout,'Operating system command returns nonzero value'); */
  case 906:
    fprintf(errout, "Safe mode: sh, copy, and print to file disallowed\n");
    break;
  }/* case */

  /* writeln(errout); */
  consoleflush();
  if (errcount > MAXERRCOUNT) {
      fatal(3);
  }
}  /* markerror */


/*--------------------------------------------------------------------*/

void stackattribute(stackinx stackp)
{ /* This is the interface routine to push terminal attributes onto the
     attribute stack */
  attribute *With;

  With = &attstack[stackp];
  /* D if debuglevel = 2 then writeln(log,
     'stackp=',stackp:1,' lexsymb=',lexsymb:1 ); D */
  With->chbufx = oldbufi;
  With->length = chbufi - oldbufi;
  With->prim = NULL;
  With->internal = NULL;
  With->varname = NULL;
  With->xval = floatvalue;
  With->yval = 0.0;
  With->startchop = 0.0;
  With->endchop = 0.0;
  With->state = 0;
  With->lexval = lexsymb;
}  /* stackattribute */


void newbuf(fbuffer **buf)
{ /* var buf: fbufferp */
  fbuffer *With;

  /*D if debuglevel > 0 then write(log,'newbuf'); D*/
  if (freeinbuf == NULL) {
      *buf = Malloc(sizeof(fbuffer));
      (*buf)->carray = Malloc(sizeof(chbufarray));
  }
  else {
      *buf = freeinbuf;
      freeinbuf = freeinbuf->nextb;
  }
  With = *buf;
  With->savedlen = 0;
  With->readx = 1;
  With->attrib = 0;
  With->higherb = NULL;
  With->prevb = NULL;
  With->nextb = NULL;
  /*D;if debuglevel > 0 then writeln(log,'[',ordp(buf):1,']') D*/
}


void initrandom(void)
{ /* initialize random number routine */
time_t seed;

  /*GH var t: TimeStamp; HG*/
  srandom(time(&seed));
  /*GH GetTimeStamp(t);
      srandom(t.Microsecond) HG*/
}


/*--------------------------------------------------------------------*/

/* procedures for input/output of characters */

void exitmacro(void)
{ arg *ar, *lastarg;

  /*D if debuglevel > 0 then writeln(log,' exitmacro'); D*/
  ar = args;
  if (args != NULL) {                                /* first get rid of args */
      args = args->higherb;
  }
  while (ar != NULL) {                                                  /*D,1D*/
      disposebufs(&ar->argbody);
      lastarg = ar;
      ar = ar->nextb;
      Free(lastarg);
  }
}


void newarg(arg **arg_)
{ /* var arg: argp */
  arg *With;

  /*D if debuglevel > 0 then write(log,'newarg'); D*/
  *arg_ = Malloc(sizeof(arg));
  With = *arg_;
  With->argbody = NULL;
  With->higherb = NULL;
  With->nextb = NULL;
  /*D;if debuglevel > 0 then writeln(log,'[',ordp(arg):1,']'); D*/
}


void disposebufs(fbuffer **buf)
{ /* var buf: fbufferp; loc: integer */
  fbuffer *bu;

  /*D if debuglevel > 0 then begin
      write(log,'disposebufs(',loc:1,')[');
      if buf <> nil then write(log,ordp(buf):1) else write(log,'nil');
      writeln(log,']') end; D*/
  if (*buf == NULL) {
      return;
  }
  if (freeinbuf != NULL) {
      bu = *buf;
      while (bu->nextb != NULL) {
	  bu = bu->nextb;
      }
      bu->nextb = freeinbuf;
  }
  freeinbuf = *buf;
  *buf = NULL;
}


void readline(FILE **infname)
{ int ll,c;
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

  /*D; if debuglevel > 0 then begin writeln(log);
     write(log,'readline ');
     if inputeof then writeln(log,'inputeof') else wrbuf(inbuf,3,0) end D*/
}


void nextline(Char lastchar)
{ /* Write out error messages, and read in a line. */
  int i;
  fbuffer *With;
  int FORLIM;

  With = inbuf;
  With->readx = 1;
  With->savedlen = 0;
  do {
      lineno++;
      if (savebuf != NULL) {
	  readline(&copyin);
	  if (inputeof) {
	      inputeof = false;
	      while (inbuf->prevb != NULL) {                           /*D,11D*/
		  inbuf = inbuf->prevb;
	      }
	      disposebufs(&inbuf);
	      inbuf = savebuf;
	      savebuf = NULL;
	  }
      }
      else {
	  readline(&input);
      }

      With = inbuf;

      if (With->carray[1] == '.') {
	  if (lexstate != 2) {
	      if (With->savedlen >= 4 && With->carray[2] == 'P') {
		  if (savebuf != NULL) {
		      With->savedlen = 0;                   /* skip .P* lines */
		  }
		  else if (With->carray[3] == 'F' || With->carray[3] == 'S') {
		      lexstate = 1;
		      With->readx = 4;
		  }
		  else if (With->carray[3] == 'E') {
		      lexstate = 3;
		      With->readx = 4;
		  }
	      }
	  }
	  else if (lastchar != bslch) {
	      if (With->savedlen < 4 || With->carray[2] != 'P' ||
		   savebuf != NULL) {
		  With->savedlen = 0;                         /* skip . lines */
	      }
	      else if (With->carray[3] == 'F' || With->carray[3] == 'S') {
		  lexstate = 1;
		  With->readx = 4;
	      }
	      else if (With->carray[3] == 'E') {
		  lexstate = 3;
		  With->readx = 4;
	      }
	      else {
		  With->savedlen = 0;
	      }
	  }
      }
      /*D if linesignal > 0 then begin
            i := lineno div (10*linesignal);
            if lineno = i*10*linesignal then writeln(errout,'lineno ',lineno:1)
            end;
         if debuglevel > 1 then begin
            writeln(log); writeln(log,'lineno ',lineno:1,':') end; D*/

      if (lexstate == 0 && !inputeof) {
	  With = inbuf;
	  FORLIM = With->savedlen;
	  for (i = 1; i < FORLIM; i++) {
	      putchar(With->carray[i]);
	  }
	  if (With->carray[With->savedlen] != bslch) {
	      putchar('\n');
	  }
	  With->savedlen = 0;
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
{ fbuffer *tp;
  boolean bufend;
  fbuffer *With;

  if (inbuf == NULL) {
      newbuf(&inbuf);
      inbuf->carray[0] = ' ';
      inbuf->attrib = -1;
  }
  /*D if debuglevel = 2 then with inbuf^ do begin
     writeln(log);
     write(log,' inchar['); if prevb<>nil then write(log,ordp(prevb):1);
     write(log,'<',ordp(inbuf):1,'>');
     if nextb<>nil then write(log,ordp(nextb):1);
     write(log,']: instr=',instr) end; D*/

  bufend = (inbuf->readx >= inbuf->savedlen);
  while (bufend) {
      With = inbuf;
      if (With->readx < With->savedlen || inputeof) {
	  bufend = false;
      }
      else if (With->readx <= With->savedlen) {
	  if (With->carray[With->readx] != bslch || With->nextb != NULL) {
	      bufend = false;
	  }
	  else if (instr) {
	      if (With->savedlen < CHBUFSIZ) {
		  With->savedlen++;
		  With->carray[With->savedlen] = nlch;
	      }
	      else if (With->attrib > 0) {
		  newbuf(&tp);
		  tp->attrib = inbuf->attrib;
		  tp->prevb = inbuf;
		  tp->savedlen = 1;
		  tp->carray[1] = nlch;
		  With->nextb = tp;
	      }
	      else {
		  With->readx--;
		  With->carray[With->readx] = bslch;
		  With->carray[With->savedlen] = nlch;
	      }
	      bufend = false;
	  }
      }
      if (!bufend) {
	  break;
      }
      if (inbuf->nextb != NULL) {
	  inbuf = inbuf->nextb;
	  continue;
      }
      if (inbuf->attrib > 0) {  /* for buf */
	  /*D if debuglevel = 2 then begin
	    write(log,' For detected, '); wrchar(ch); write(log,' ')
	    end; D*/
	  inbuf->readx = 1;
	  while (inbuf->prevb != NULL) {
	      inbuf = inbuf->prevb;
	      inbuf->readx = 1;
	  }
	  forbufend = true;
	  continue;
      }
      while (inbuf->prevb != NULL) {
	  inbuf = inbuf->prevb;
      }
      if (inbuf->nextb != NULL) {                                      /*D,10D*/
	  disposebufs(&inbuf->nextb);
      }
      if (inbuf->higherb != NULL) {
	  tp = inbuf;
	  inbuf = inbuf->higherb;                                       /*D,3D*/
	  disposebufs(&tp);
      }
      else if (!inputeof) {
	  nextline(inbuf->carray[inbuf->savedlen]);
      }
  }

  if (forbufend || inputeof) {
      ch = nlch;
      return;
  }
  With = inbuf;
  ch = With->carray[With->readx];
  With->readx++;

  /*D; if debuglevel = 2 then with inbuf^ do begin
     write(log,' savedlen=',savedlen:1);
     if inputeof then write(log,' inputeof');
     write(log,' '); wrchar(ch);
     write(log,' readx=',readx:1)
     end D*/

  /* This is not a loop */
}  /* inchar */


void skiptoend(void)
{ /* skip to end of the current input line */
  boolean skip;
  fbuffer *With;

  /*D if debuglevel>1 then
      writeln(log,'skiptoend: inbuf[',ordp(inbuf):1,']'); D*/
  skip = true;
  while (skip) {
      if (inbuf == NULL) {
	  skip = false;
	  break;
      }
      With = inbuf;
      /*D if (debuglevel>1) and (carray^[readx]=nlch) then
          write(log,'carray^[',readx:1,']=',ord(carray^[readx]):1); D*/
      if (With->carray[With->readx] == nlch) {
	  skip = false;
      }
      else if (With->carray[With->readx] == etxch) {
	  exitmacro();
      }
      With->readx++;
      if (With->readx > With->savedlen) {
	  if (inbuf->nextb != NULL) {
	      inbuf = inbuf->nextb;
	  }
	  else {
	      skip = false;
	  }
      }
      /*D; if (debuglevel>1) then writeln(log,' readx=',inbuf^.readx:1) D*/
  }
}


void backup(void)
{ fbuffer *With;

  With = inbuf;
  With->readx--;
  ch = ' ';
}


/*--------------------------------------------------------------------*/



/*--------------------------------------------------------------------*/

void pushch(void)
{ /* copy ch character into chbuf and get new ch */
  chbuf[chbufi] = ch;
  if (chbufi < CHBUFSIZ) {      /* Leave 1 location free at the end of chbuf^ */
      chbufi++;
  }
  else {
      fatal(4);
  }
  inchar();
}  /*pushch*/


void readlatex(void)
{ /* read complete line into chbuf */
  while (ch != nlch) {
      pushch();
  }
  newsymb = XLaTeX;
}


boolean isprint_(Char ch)
{ return (ch >= 32 && ch <= 126);
}


#ifndef SAFE_MODE

void pointinput(nametype *txt)
{ /* txt: strptr */
  int i, FORLIM;

  if (txt == NULL) {
      return;
  }
  if (txt->segmnt == NULL) {
      return;
  }
  if (txt->len >= FILENAMELEN) {
      txt->len = FILENAMELEN - 1;
  }
  FORLIM = txt->len;
  for (i = 0; i < FORLIM; i++) {
      infname[i] = txt->segmnt[txt->seginx + i];
  }
  for (i = txt->len; i < FILENAMELEN; i++) {
      infname[i] = ' ';
  }
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
  if (copyin != NULL) {
      copyin = freopen(P_trimname(infname, sizeof(mstring)), "r", copyin);
  }
  else {
      copyin = fopen(P_trimname(infname, sizeof(mstring)), "r");
  }
  if (copyin == NULL) {
      _EscIO(FileNotFound);
  }
  backup();
  ch = nlch;
  savebuf = inbuf;
  inbuf = NULL;
}


void pointoutput(boolean nw, nametype *txt, int *ier)
{ /* nw: boolean; txt: strptr; var ier: integer */
  int i, FORLIM;

  *ier = 1;
  if (txt->segmnt == NULL) {
      return;
  }
  if (txt->len >= FILENAMELEN) {
      txt->len = FILENAMELEN - 1;
  }
  FORLIM = txt->len;
  for (i = 0; i < FORLIM; i++) {
      outfnam[i] = txt->segmnt[txt->seginx + i];
      if (*ier == 1) {
	  if (outfnam[i] > 32 && outfnam[i] <= 126) {
	      *ier = 0;
	  }
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
      if (redirect != NULL) {
	  redirect = freopen(P_trimname(outfnam, sizeof(mstring)), "w",
			     redirect);
      }
      else {
	  redirect = fopen(P_trimname(outfnam, sizeof(mstring)), "w");
      }
      if (redirect == NULL) {
	  _EscIO(FileNotFound);
      }
      return;
  }
  if (redirect != NULL) {
      redirect = freopen(P_trimname(outfnam, sizeof(mstring)), "a", redirect);
  }
  else {
      redirect = fopen(P_trimname(outfnam, sizeof(mstring)), "a");
  }
  if (redirect == NULL) {
      _EscIO(FileNotFound);
  }
}


#endif

int argcount(arg *ar)
{ int i;

  if (ar == NULL) {
      markerror(805);
  }
  i = 0;
  while (ar != NULL) {
      if (ar->argbody != NULL) {
	  i++;
      }
      ar = ar->nextb;
  }
  return i;
}


arg *findarg(arg *arlst, int k)
{ arg *ar;
  int i, j;

  /*D if debuglevel > 0 then begin
      write(log, 'findarg(');
      if arlst = nil then write(log,'nil') else write(log,ordp(arlst):1);
      write(log,',',k:0,'):') end; D*/
  if (k > 0) {
      ar = arlst;
  }
  else {
      ar = NULL;
  }
  i = 1;
  j = k;
  while (i < j) {
      if (ar != NULL) {
	  ar = ar->nextb;
	  i++;
      }
      else {
	  j = i;
      }
  }
  /*D if debuglevel > 0 then begin
     if (i<>k) or (ar=nil) then writeln(log,'not found')
     else wrbuf(ar^.argbody,2,1) end; D*/
  return ar;
}


void readstring(Char stringch)
{ /* puts string terminal into chbuf */
  int j, n;
  boolean pushfl;
  arg *ar;
  fbuffer *abuf, *With;
  int FORLIM;

  /*D if debuglevel > 0 then writeln(log,'readstring(',stringch,')'); D*/
  j = -1;
  n = 0;
  instr = true;
  pushfl = true;
  while (instr) {
      if (inputeof) {
	  instr = false;
      }
      else if (ch == bslch) {
	  /*D if debuglevel > 0 then begin
	      write(log,' readstring3, instr=',instr,' ');
	      wrchar(ch); writeln(log) end; D*/
	  pushch();
	  if (ch == stringch) {
	      chbufi--;
	  }
	  else {
	      pushfl = false;
	  }
      }
      else if (ch == '$' && j < 0) {
	  j = chbufi;
      }
      else if (j >= 0 && isdigit(ch) != 0) {
	  n = n * 10 + ch - '0';
      }
      else if (j >= 0) {
	  if (n > 0 && args != NULL) {                          /* delete $nn */
	      chbufi = j;
	  }
	  if (n > 0) {
	      ar = findarg(args, n);
	      if (ar != NULL) {
		  abuf = ar->argbody;
		  while (abuf != NULL) {
		      With = abuf;
		      FORLIM = With->savedlen;
		      for (n = With->readx; n <= FORLIM; n++) {
			  chbuf[chbufi] = With->carray[n];
			  if (chbufi < CHBUFSIZ) {
			      chbufi++;
			  }
			  else {
			      fatal(4);
			  }
		      }
		      abuf = abuf->nextb;
		  }
	      }
	      /*D if debuglevel > 0 then begin write(log,'arg(',n:1,')|');
	         for n := j to chbufi-1 do write(log,chbuf^[n]);
	         write(log,'| ') end; D*/
	  }
	  j = -1;
	  n = 0;
	  pushfl = false;
      }
      else if (ch == stringch) {
	  instr = false;
      }
      /*D if debuglevel > 0 then begin
          write(log,' readstring2, instr=',instr,' j=',j:0,' pushfl=',pushfl);
          wrchar(ch); writeln(log) end;D*/
      if (instr && pushfl) {
	  pushch();
      }
      else {
	  pushfl = true;
      }
  }
  if (ch != nlch) {
      inchar();
  }
  else {
      /*D; if debuglevel > 0 then writeln(log,' readstring done') D*/
      markerror(901);
  }
}


void readexponent(void)
{ boolean neg;
  int k;

  pushch();
  if (ch == '-') {
      neg = true;
      pushch();
  }
  else if (ch == '+') {
      neg = false;
      pushch();
  }
  else {
      neg = false;
  }
  if (isdigit(ch) == 0) {
      markerror(802);
      return;
  }
  k = 0;
  do {
      k = k * 10 + ch - '0';
      pushch();
  } while (isdigit(ch) != 0);
  if (neg) {
      floatvalue *= exp(-k * ln10);
  }
  else {
      floatvalue *= exp(k * ln10);
  }
}  /* readexponent */


void readfraction(void)
{ double x;

  x = 10.0;
  while (isdigit(ch) != 0) {
      floatvalue += (ch - '0') / x;
      x *= 10;
      pushch();
  }
}  /* readfraction */


void readconst(Char initch)
{ /*D if debuglevel=2 then begin
     write(log,'readconst(',initch,') readx=',inbuf^.readx:1,' ');
       wrchar(ch); writeln(log) end; D*/
  if (initch == '.') {
      readfraction();
  }
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
  if (ch == 'e' || ch == 'E') {
      readexponent();
  }
  if (ch == 'i') {                    /* A rather odd way to allow inch units */
      pushch();
  }
  newsymb = XLfloat;
}


/*D
procedure prlex(acc: boolean);
var i: integer;
begin
   if debuglevel > 0 then begin
      writeln(log);
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
{ fbuffer *With;

  if (buf->prevb == NULL) {
      newbuf(&buf->prevb);
  }
  With = buf->prevb;
  With->attrib = buf->attrib;
  With->savedlen = CHBUFSIZ;
  With->readx = CHBUFSIZ + 1;
  With->nextb = buf;
  return (buf->prevb);
}


void copyleft(fbuffer *mac, fbuffer **buf)
{ /* mac: fbufferp; var buf: fbufferp */
  /* Push macro or arg or string from mac into the head of the input stream */
  fbuffer *ml;
  int i, k;
  boolean newflag, copied;
  fbuffer *With;
  int FORLIM;

  /*D if debuglevel > 0 then writeln(log, 'copyleft:'); D*/
  /*D if debuglevel > 1 then begin
     write(log,' input string'); wrbuf(mac,3,1);
     write(log,' output'); wrbuf(buf,3,1) end; D*/
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
	      if (*buf == NULL) {
		  newflag = true;
	      }
	      else if ((*buf)->attrib >= 0) {
		  newflag = true;                                 /* for body */
	      }
	      else {
		  newflag = false;
	      }
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
	      With = *buf;
	      while (With->readx > 1) {
		  With->readx--;
		  With->carray[With->readx] = mac->carray[k];
		  k--;
	      }
	      *buf = nbuf(*buf);
	  }
	  With = *buf;
	  FORLIM = mac->readx;
	  for (i = k; i >= FORLIM; i--) {
	      With->readx--;
	      With->carray[With->readx] = mac->carray[i];
	  }
      }
      mac = mac->prevb;
  }
  if (!copied) {
      return;
  }
  /*D; if debuglevel > 0 then begin
     ml := buf; writeln(log,' copyleft result'); while ml <> nil do begin
        wrbuf(ml,3,1); ml := ml^.nextb end end D*/
  if ((*buf)->readx <= 1) {
      *buf = nbuf(*buf);
  }
  With = *buf;
  With->carray[With->readx - 1] = nlch;
}


void copyright(fbuffer *mac, fbuffer **buf)
{ /* $n has been seen: Push the argument into the tail of the input buffer */
  fbuffer *ml;
  int i, k;
  fbuffer *With;
  int FORLIM;

  /*D if debuglevel > 0 then begin
     writeln(log, 'copyright:');
     write(log,' input'); wrbuf(mac,3,1);
     write(log,' output'); wrbuf(buf,3,0) end; D*/
  while (mac != NULL) {
      if (*buf == NULL) {
	  newbuf(buf);
	  With = *buf;
	  With->attrib = -1;
	  With->savedlen = 0;
	  With->readx = 1;
      }
      k = mac->readx;
      if (CHBUFSIZ - (*buf)->savedlen <= mac->savedlen - k) {
	  With = *buf;
	  while (With->savedlen < CHBUFSIZ) {
	      With->savedlen++;
	      With->carray[With->savedlen] = mac->carray[k];
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
      With = *buf;
      FORLIM = mac->savedlen;
      for (i = k; i <= FORLIM; i++) {
	  With->savedlen++;
	  With->carray[With->savedlen] = mac->carray[i];
      }
      mac = mac->nextb;
  }
  /*D; if debuglevel > 0 then with buf^ do begin
     write(log,' result'); wrbuf(buf,3,0) end D*/
}


void skipcontinue(void)
{ /* Check the current char for line continuation */
  Char c;
  fbuffer *With;

  /*D if debuglevel=2 then
     write(log,' skipcontinue readx=',inbuf^.readx:1,' '); D*/
  c = ch;
  while (c == bslch) {
      With = inbuf;
      if (With->readx <= With->savedlen) {
	  c = With->carray[With->readx];
      }
      else if (With->nextb == NULL) {
	  inchar();
	  c = ch;
      }
      else {
	  c = With->nextb->carray[With->nextb->readx];
      }
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
{ /* D if debuglevel = 2 then writeln(log, 'skipwhite: ' ); D */
  while (ch == etxch || ch == nlch || ch == tabch || ch == ' ') {
      if (ch == etxch) {
	  exitmacro();
      }
      inchar();
      if (ch == bslch) {
	  skipcontinue();
      }
  }
}


void definearg(int *parenlevel, fbuffer **p2)
{ /* Stash the current argument into the arg struct*/
  int j, n;
  arg *ar;
  fbuffer *p, *p1;
  boolean inarg, instring;
  Char prevch;
  fbuffer *With;

  /*D if debuglevel > 0 then
      writeln(log, 'definearg: parenlevel=',parenlevel:1); D*/
  skipwhite();
  p1 = NULL;
  if (*parenlevel >= 0) {
      inarg = true;
  }
  else {
      inarg = false;
  }
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
	  With = p1;
	  if (prevch != bslch) {
	      if (instring && ch == '"') {
		  instring = false;
	      }
	      else if (ch == '"') {
		  instring = true;
	      }
	  }
	  if (!instring) {
	      if (ch == '(') {
		  (*parenlevel)++;
	      }
	      else if (ch == ')') {
		  (*parenlevel)--;
	      }
	  }
	  /*D if debuglevel=2 then write(log,' instring=',instring,' '); D*/
	  if (!instring && (*parenlevel < 0 || *parenlevel == 0 && ch == ',')) {
	      j = With->savedlen;
	      inarg = false;
	  }
	  else if (ch != '$') {
	      prevch = ch;
	      if (With->savedlen < CHBUFSIZ) {
		  With->savedlen++;
	      }
	      else {
		  markerror(872);
	      }
	      With->carray[With->savedlen] = ch;
	      /*D if debuglevel = 2 then begin
	         write(log,'definearg2: savedlen=',savedlen:1,' ');
	         wrchar(ch); writeln(log,' parenlevel=',parenlevel:1)
	         end; D*/
	      inchar();
	  }
	  else {
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
		  if (ar != NULL) {
		      copyright(ar->argbody, &p1);
		  }
	      }
	      else {
		  if (With->savedlen < CHBUFSIZ) {
		      With->savedlen++;
		  }
		  else {
		      markerror(872);
		  }
		  With->carray[With->savedlen] = prevch;
		  /*D if debuglevel = 2 then begin
		      write(log,'definearg2: savedlen=',savedlen:1,' ');
		      wrchar(ch); writeln(log) end; D*/
	      }
	  }
	  if (inputeof) {
	      markerror(806);
	      j = With->savedlen;
	      inarg = false;
	      *parenlevel = -1;
	  }
      } while (p1->savedlen != j);
  }
  if (p1 != NULL) {
      while (p1->prevb != NULL) {
	  p1 = p1->prevb;
      }
  }
  ch = ' ';
  *p2 = p1;
  /*D; if debuglevel > 0 then with p1^ do begin
     write(log,' definearg'); wrbuf(p1,3,0) end D*/
}


boolean ismacro(Char *chb, chbufinx obi, chbufinx chbi)
{ arg *mac, *lastp, *ar, *lastarg, *firstarg;
  int level;
  boolean ism;

  /*D if debuglevel > 0 then begin
      writeln(log); write(log, 'ismacro[');
      for level:=obi to chbi-1 do write(log,chb^[level]);
      write(log,']:') end;
      D*/
  ism = false;
  if (oldsymb == XLdefine) {
      skipwhite();
      return ism;
  }
  if (oldsymb == XLundefine) {
      return ism;
  }
  mac = findmacro(macros, chb, obi, chbi - obi, &lastp);
  if (mac == NULL) {
      return ism;
  }
  ism = true;
  firstarg = NULL;
  lastarg = NULL;
  if (ch == '(') {
      level = 0;
  }
  else {
      backup();
      level = -1;
  }
  do {
      newarg(&ar);
      ar->higherb = args;
      if (lastarg == NULL) {
	  firstarg = ar;
      }
      else {
	  lastarg->nextb = ar;
      }
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
{ int n, i;
  arg *ar;

  /*D if debuglevel > 0 then begin write(log,'copyarg(');
     for i:= chbs to chbi-1 do write(log,chb^[i]); write(log,') ') end; D*/
  n = 0;
  for (i = chbs + 1; i < chbi; i++) {
      n = n * 10 + chb[i] - '0';
  }
  ar = findarg(args, n);
  backup();
  if (ar != NULL) {
      if (ar->argbody != NULL) {
	  copyleft(ar->argbody, &inbuf);
      }
  }
}


boolean insertarg(void)
{ int icx;
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
  while (isdigit(ch) != 0) {
      pushch();
  }
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
{ /* find the next terminal */
  int lxix;
  Char firstch;
  boolean terminalaccepted, looping;
  fbuffer *With;

  /* argstruct: argp;
  j,k: integer;
  varptr,lastp: strptr; */

  floatvalue = 0.0;
  lexsymb = -1;
  do {
      terminalaccepted = true;
      oldbufi = chbufi;            /* first char of current terminal in chbuf */
      while (ch == ' ' || ch == tabch) {
	  inchar();
      }

      if (lexstate == 1) {
	  newsymb = XSTART;
	  lexstate = 2;
      }
      else if (lexstate == 3) {
	  /*D if linesignal > 0 then begin
	     writeln(errout,'.PE'); consoleflush end; D*/
	  newsymb = XEND;
	  /* ch := '.'; */
	  lexstate = 4;
      }
      else if (lexstate == 4) {
	  newsymb = XNL;
	  skiptoend();
	  ch = ' ';
	  lexstate = 0;
      }
      else if (inputeof) {
	  if (oldsymb == 0) {
	      fatal(5);
	  }
	  else if (oldsymb == 1) {
	      newsymb = 0;
	  }
	  else {
	      newsymb = 1;
	  }
      }
      else if (forbufend) {
	  newsymb = XLendfor;
	  forbufend = false;
	  ch = ' ';
      }
      else if (ch == nlch) {
	  newsymb = XNL;
	  ch = ' ';
	  if (oldsymb == XLelse || oldsymb == XLBRACE || oldsymb == XLthen ||
	       oldsymb == XCOLON || oldsymb == XNL) {
	      terminalaccepted = false;
	  }
      }
      else if (isdigit(ch) != 0) {
	  readconst(ch);
      }
      else if (ch == etxch) {
	  exitmacro();
	  inchar();
	  terminalaccepted = false;
      }
      else {
	  firstch = ch;
	  pushch();
	  With = inbuf;
	  if (firstch == bslch) {
	      if (ch == nlch || ch == '#') {
		  if (ch == '#') {
		      skiptoend();
		  }
		  ch = ' ';
		  newsymb = -2;                        /* flag for test below */
		  terminalaccepted = false;
	      }
	      else {
		  readlatex();
	      }
	  }
	  else if (firstch == '.' && isdigit(ch) != 0) {
	      readconst(firstch);
	  }
	  else if (firstch == '.' && With->readx == 3 &&
		   inbuf->prevb == NULL && newsymb != -2) {
	      readlatex();
	  }
	  else {
	      newsymb = entrytv[firstch];
	      lxix = entryhp[firstch];
	      while (lxix != 0) {
		  if (ch == '$') {
		      if (!insertarg()) {
			  lxix = 0;
		      }
		      continue;
		  }
		  if (lxch[lxix] == ch) {
		      newsymb = lxtv[lxix];
		      lxix = lxhp[lxix];
		      pushch();
		  }
		  else {
		      lxix = lxnp[lxix];
		  }
	      }

	      if (isupper(firstch) != 0 &&
		  (isalnum(ch) != 0 || ch == '_' || ch == '$')) {
		  looping = true;
		  while (looping) {
		      if (ch == '$') {
			  looping = insertarg();
		      }
		      else if (isalnum(ch) != 0 || ch == '_') {
			  pushch();
		      }
		      else {
			  looping = false;
		      }
		  }
		  if (ismacro(chbuf, oldbufi, chbufi)) {
		      terminalaccepted = false;
		  }
		  else {
		      newsymb = XLabel;
		  }
	      }
	      else if ((isalnum(firstch) != 0 || firstch == '_') &&
		       (isalnum(ch) != 0 || ch == '_' || ch == '$')) {
		  looping = true;
		  while (looping) {
		      if (ch == '$') {
			  looping = insertarg();
		      }
		      else if (isalnum(ch) != 0 || ch == '_') {
			  pushch();
		      }
		      else {
			  looping = false;
		      }
		  }
		  if (ismacro(chbuf, oldbufi, chbufi)) {
		      terminalaccepted = false;
		  }
		  else {
		      newsymb = XLname;
		  }
	      }
	      else if (newsymb == XLstring) {
		  chbufi--;
		  readstring(chbuf[chbufi]);
	      }
	      else if (newsymb == XCOMMENT) {
		  skiptoend();
		  ch = nlch;
		  terminalaccepted = false;
	      }
	      else if (newsymb == XNL &&
		       (oldsymb == XLelse || oldsymb == XLBRACE ||
			oldsymb == XLthen || oldsymb == XCOLON ||
			oldsymb == XNL)) {
		  terminalaccepted = false;
	      }
	      else if (newsymb == XLT && inlogic) {
		  lexsymb = XLcompare;
		  newsymb = XLcompare;
	      }
	      else if (newsymb > XEQ && newsymb <= XLremeq) {
		  lexsymb = newsymb;
		  newsymb = XEQ;
	      }
	      else if (newsymb > XLcorner) {
		  lexsymb = newsymb;
		  if (newsymb > XLenvvar) {
		      newsymb = XLenvvar;
		  }
		  else if (newsymb > XLprimitiv) {
		      newsymb = XLprimitiv;
		  }
		  else if (newsymb > XLdirecton) {
		      newsymb = XLdirecton;
		  }
		  else if (newsymb > XLarrowhd) {
		      newsymb = XLarrowhd;
		  }
		  else if (newsymb > XLtextpos) {
		      newsymb = XLtextpos;
		  }
		  else if (newsymb > XLcolrspec) {
		      newsymb = XLcolrspec;
		  }
		  else if (newsymb > XLlinetype) {
		      newsymb = XLlinetype;
		  }
		  else if (newsymb > XLfunc2) {
		      newsymb = XLfunc2;
		  }
		  else if (newsymb > XLfunc1) {
		      newsymb = XLfunc1;
		  }
		  else if (newsymb > XLparam) {
		      newsymb = XLparam;
		  }
		  else if (newsymb > XLcompare) {
		      newsymb = XLcompare;
		  }
		  else {
		      newsymb = XLcorner;
		  }
	      }
	      else if (newsymb == XLarg) {
		  if (ch == '+') {  /* $+ */
		      floatvalue = argcount(args);
		      newsymb = XLfloat;
		      inchar();
		  }
		  else if (isdigit(ch) != 0) {
		      do {
			  pushch();
		      } while (isdigit(ch) != 0);
		      copyarg(chbuf, oldbufi, chbufi);
		      terminalaccepted = false;
		  }
		  else {
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
	      }
	      else if (newsymb == XLdo) {
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
	      }
	      else if (newsymb == 0 && isupper(firstch) != 0) {
		  if (ismacro(chbuf, oldbufi, chbufi)) {
		      terminalaccepted = false;
		  }
		  else {
		      newsymb = XLabel;
		  }
	      }
	      else if (newsymb == 0 &&
		       (isalnum(firstch) != 0 || firstch == '_')) {
		  if (ismacro(chbuf, oldbufi, chbufi)) {
		      terminalaccepted = false;
		  }
		  else {
		      newsymb = XLname;
		  }
	      }
	      else if (newsymb == 0) {
		  fprintf(errout, "Char chr(%d)", firstch);
		  if (firstch > 32 && (firstch & 255) < 127) {
		      fprintf(errout, "\"%c\"", firstch);
		  }
		  fprintf(errout, " unknown\n");
		  markerror(800);
		  terminalaccepted = false;
	      }
	  }
      }

      /*D prlex( terminalaccepted ); D*/
      if (newsymb != XLaTeX && newsymb != XLstring && newsymb != XLabel &&
	   newsymb != XLname) {
	  chbufi = oldbufi;
      }
  } while (!terminalaccepted);
  if (lexsymb == -1) {
      lexsymb = newsymb;
  }
  oldsymb = newsymb;



  /* search in lexical tree */
  /* $<integer> */
  /* if not isalnum(ch) then */
  /* $name */
  /* char not recognized */
  /*D if debuglevel > 0 then writeln(log,
       'Marking 800:ord(firstch)=',ord(firstch),
       ' ord(ch)=',ord(ch)); D*/
  /*lookahead terminals*/
}  /* lexical */


/*--------------------------------------------------------------------*/

void skiptobrace(void)
{ int bracelevel;
  boolean instring;
  Char prevch;

  /*D if debuglevel = 2 then writeln(log, 'skiptobrace: ' ); D*/
  bracelevel = 1;
  prevch = ' ';
  instring = false;
  while (bracelevel > 0) {
      if (ch == bslch) {
	  skipcontinue();
      }
      if (ch == etxch) {
	  exitmacro();
      }
      if (instring) {
	  if (ch == '"' && prevch != bslch) {
	      instring = false;
	  }
      }
      else if (ch == '{') {
	  bracelevel++;
      }
      else if (ch == '}') {
	  bracelevel--;
      }
      else if (ch == '"') {
	  instring = true;
      }
      if (bracelevel <= 0) {
	  break;
      }
      if (inputeof) {
	  bracelevel = 0;
	  if (instring) {
	      markerror(807);
	  }
	  else {
	      markerror(804);
	  }
      }
      else {
	  prevch = ch;
	  inchar();
      }
  }
}


void readfor(fbuffer *p0, int attx, fbuffer **p2)
{ /* p0: fbufferp; attx: integer; var p2: fbufferp */
  /* Stuff the body of a for loop or a macro body into p2
     attx: attstack index or -(name length)
     p0 = nil: append the output to this buffer.
     Should we check for macro arguments? */
  int j, bracelevel;
  fbuffer *p, *p1;
  boolean instring;
  Char prevch;

  /*D if debuglevel > 0 then begin
      write(log,'readfor: attx(');
      if attx<0 then write(log,'-length)=') else write(log,'attstack idx)=');
      writeln(log,attx:5)
      end; D*/
  prevch = ' ';
  instring = false;
  bracelevel = 1;
  p1 = NULL;
  while (bracelevel > 0) {
      if (p0 == NULL) {
	  newbuf(&p);
      }
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
	  if (ch == bslch) {
	      skipcontinue();
	  }
	  /* D if debuglevel = 2 then begin write(log, 'readfor1: ');
	      wrchar(ch); write(log,' ') end; D */
	  if (instring) {  /* do not check braces in strings */
	      if (ch == '"' && prevch != bslch) {
		  instring = false;
	      }
	  }
	  else if (ch == '#') {
	      skiptoend();
	      ch = nlch;
	  }
	  else if (ch == '{') {
	      bracelevel++;
	  }
	  else if (ch == '}') {
	      bracelevel--;
	  }
	  else if (ch == '"') {
	      instring = true;
	  }
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
	  if (bracelevel > 0) {
	      inchar();
	  }
	  if (inputeof) {
	      if (instring) {
		  markerror(807);
	      }
	      else {
		  markerror(804);
	      }
	      j = p1->savedlen;
	      bracelevel = 0;
	  }
      } while (p1->savedlen != j);
  }
  while (p1->prevb != NULL) {
      p1 = p1->prevb;
  }
  backup();
  /*D if debuglevel > 0 then begin
     write(log,'readfor: for/macro buffer'); wrbuf(p1,3,0) end; D*/
  *p2 = p1;
}


/*--------------------------------------------------------------------*/

void bumptop(stackinx chk, stackinx *sttop)
{ /* D if chk<>sttop then
     writeln(errout,'chk=',chk:4,' sttop=',sttop:4); D */
  if (chk < STACKMAX) {
      (*sttop)++;
  }
  else {
      fatal(6);
  }
}


/*D
procedure prattstack(j: integer);
var i,k: integer;
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
      else write(log,attstack^[i].prim^.ptype:4); writeln(log);
   k := j+1; i := 1;
   while i < k do if attstack^[i].internal=nil then i := i+1 else k := i;
   if k <= j then begin
      write(log,'intrtype: ');
      for i := 1 to j do if attstack^[i].internal = nil then write(log,'    ')
         else write(log,attstack^[i].internal^.ptype:4); writeln(log) end
   end; D*/

void doprod(int prno)
{ /* prno: integer */
  redubuf[reduinx + REDUMAX].prod_ = prno;
  reduinx--;
}


void advance(void)
{  /* perform reductions */
  int i, j;
  reduelem *With;
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
      With = &redubuf[reduinx + REDUMAX];
      /*D if debuglevel > 1 then begin
         j := newtop; if oldtop > newtop then j := oldtop;
         writeln(log,'Attstack for next production:');
         prattstack(j)
         end; D*/

      produce(With->newtop, With->prod_);               /* may change reduinx */
      /* D if debuglevel > 1 then writeln(log,
         'attstack^[', newtop:1, '].chbufx<-', attstack^[newtop-1].chbufx:1);
          D */
      /* attstack^[newtop].chbufx := attstack^[newtop-1].chbufx; */
      reduinx++;
  }

  /* update stack */
  if (validtop != top) {
      FORLIM = stacktop - validtop;
      for (i = 1; i <= FORLIM; i++) {
	  parsestack[validtop + i] = parsestack[top + i];
      }
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

      for (i = 0; i < FORLIM; i++) {
	  chbuf[j + i] = chbuf[oldbufi + i];
      }
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
{ stacktop = btop;
  validtop = btop;
  pseudotop = btop;
  startinx = bstart;
  lri = bstart;
  redutop = 0;
}  /* backtrack */


void pseudoshift(void)
{ bumptop(pseudotop, &stacktop);
  pseudotop = top + stacktop - validtop;
  parsestack[pseudotop].table = startinx;
  stackattribute(pseudotop);
}  /* pseudoshift */


/*--------------------------------------------------------------------*/

void queue(int rs_, int p)
{ reduelem *With;

  if (p >= 0) {
      if (redutop < REDUMAX) {
	  redutop++;
      }
      else {
	  fatal(7);
      }
      With = &redubuf[redutop + REDUMAX];
      /*D oldtop := stacktop; D*/
      stacktop -= rs_;
      With->newtop = stacktop;
      With->prod_ = p;
  }
  else {
      stacktop -= rs_;
  }
  if (stacktop <= validtop) {
      pseudotop = stacktop;
      validtop = stacktop;
  }
  else {
      pseudotop -= rs_;
  }
}  /* queue */


boolean lookahead(symbol lsymbol)
{ boolean Result, decided;
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
	while (lr[lri + symb] != lsymbol && lr[lri] != 0) {
	    lri = lr[lri];
	}
	decided = true;
	Result = (lr[lri + symb] == lsymbol);
	break;

      case 2:
      case 4:
      case 6:
	while (lr[lri + symb] != lsymbol && lr[lri] != 0) {
	    lri = lr[lri];
	}
	if (lr[lri + kind] == 2) {
	    decided = true;
	    Result = true;
	}
	else if (lr[lri + kind] == 6) {
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
	while (lr[lri + lb] != si && lr[lri] != 0) {
	    lri = lr[lri];
	}
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
{ boolean success;
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
      if (!success) {                                                /*D end D*/
	  lexical();
      }
      /*D begin if debuglevel > 0 then writeln(log, ' <---SKIPPED'); D*/
  } while (!(success || parsestop));
  if (parsestop) {
      fatal(8);
  }
}  /* syntaxerror */


void parse(void)
{ initrandom();

  chbuf = Malloc(sizeof(chbufarray));
  entrytv[ordNL] = XNL;
  entrytv[ordCR] = XNL;                               /* treat ^M as line end */

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
      if (!lookahead(newsymb)) {
	  syntaxerror();
      }
      advance();
  }
}  /* parse */


Char optionchar(Char *fn)
{ int j, k;

  j = 1;
  k = FILENAMELEN + 1;
  while (j < k) {
      if (fn[j - 1] == ' ') {
	  j++;
      }
      else {
	  k = j;
      }
  }
  if (j >= FILENAMELEN) {
      return '\0';
  }
  else if (fn[j - 1] == '-') {
      return (fn[j]);
      /*
      if fn[j+2] <> ' ' then for k:=j+2 to FILENAMELEN do fn[k-j-1] := fn[k]
      */
  }
  else {
      return '\0';
  }
}


void getoptions(void)
{ Char cht;
  int istop;

  /*DUGHM oflag := 0; linesignal := 0; MHGUD*/
  argct = 1;
  istop = P_argc;                                        /*GH ParamCount+1; HG*/
  while (argct < istop) {
      P_sun_argv(infname, sizeof(mstring), argct);
	  /*GH infname := ParamStr(argct); HG*/
      cht = optionchar(infname);
      if (cht == 0) {
	  istop = argct;
	  continue;
      }
      /* if cht = 'q' then drawmode := PSmps */
      /* Metapost-readable PS*/
      /*
      else */
      if (cht == 'd') {
	  drawmode = PDF;
      }
      else if (cht == 'e') {
	  drawmode = Pict2e;
      }
      else if (cht == 'f') {
	  drawmode = PSfrag;
      }
      else if (cht == 'g') {
	  drawmode = PGF;
      }
      else if (cht == 'm') {
	  drawmode = MFpic;
      }
      else if (cht == 'p') {
	  drawmode = PSTricks;
      }
      else if (cht == 'r') {
	  drawmode = PS;
      }
      else if (cht == 's') {
	  drawmode = MPost;
      }
      else if (cht == 't') {
	  drawmode = tTeX;
      }
      else if (cht == 'v') {
	  drawmode = SVG;
      }
      else if (cht == 'x') {
	  drawmode = xfig;
      }
      else if (cht == 'z') {
	  safemode = true;
	  /*DUGHM
         else if cht = 'y' then linesignal := 2
         else if (cht >= '0') and (cht <= '9') then oflag := ord(cht)-ord('0')
         MHGUD*/
      }
      else if (cht == 'h' || cht == '-') {
	  fprintf(errout, " *** dpic version 2015.08.31\n");
	  fprintf(errout, " options:\n");
	  fprintf(errout, "     (none) LaTeX picture output\n");
	  fprintf(errout, "     -d PDF output\n");
	  fprintf(errout, "     -e Pict2e output\n");
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
      }
      else {
	  fprintf(errout, " *** dpic terminating: Unknown option \"");
	  if (isprint_(cht)) {
	      fprintf(errout, "%c\"\n", cht);
	  }
	  else {
	      fprintf(errout, "char(%d)\"\n", cht);
	  }
	  fatal(0);
      }
      argct++;
  }
}  /* getoptions */


main(int argc, Char *argv[])
{ P_argc = argc; P_argv = argv; __top_jb = NULL;
  /* dpic */
  redirect = NULL;
  copyin = NULL;
  errout = NULL;
  output = NULL;
  input = NULL;
  openerror();
  drawmode = TeX;
#ifdef SAFE_MODE
  safemode = true;
#else
  safemode = false;
#endif
  getoptions();
  /* D writeln('oflag=',oflag); D */
  openfiles();
  inputeof = false;
  attstack = Malloc(sizeof(attstacktype));
  parse();
  epilog();
  /*X retcode(errcount); X*/

  if (input != NULL) {
      fclose(input);
  }
  if (output != NULL) {
      fclose(output);
  }
  if (errout != NULL) {
      fclose(errout);
  }
  if (copyin != NULL) {
      fclose(copyin);
  }
  if (redirect != NULL) {
      fclose(redirect);
  }
  if (errcount==0) exit(EXIT_SUCCESS); else exit(EXIT_FAILURE);
}



/* End. */
