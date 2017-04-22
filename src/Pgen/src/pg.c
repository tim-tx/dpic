/* This parser generator reads an input file in extended BNF form and
   produces language-independent ASCII output that can be converted to
   lexical tables, parse tables, error messages, and constants suitable
   for inclusion in a parser. The internal optimizations are adapted
   from a system available in the 1970s called "BOBS-System" from the
   University of Aarhus: "http://pure.au.dk/portal/files/32896922/PB_71.pdf"
   The current version has evolved through several mutations.  It could
   be simplified further. */


#include "p2c.h"
#include <time.h>


/*F uses sysutils; F*/

/* PG0 ---------------------------------------------------------------*/

/* Machine-dependent constants: -----------------------------------*/

/* smallest and largest char for parser generator
   machine: */

#define ordminch        0
#define ordmaxch        255

#define ordtabchar      9
#define ordbslchar      92

#define bslchar         ((Char)ordbslchar)

/* Maximum number of elements in a set */

#define setmax          58

/* constants for produce                    ----------------------*/
#define METAGOAL1       0
#define ebnfinput1      1
#define ebnfinput2      2
#define ebnfinput3      3
#define heading1        4
#define heading2        5
#define heading3        6
#define heading4        7
#define heading5        8
#define heading6        9
#define heading7        10
#define heading8        11
#define productions1    12
#define terminaldefs1   13
#define optiondefs1     14
#define emptydef1       15
#define emptydef2       16
#define goalsymboldef1  17
#define colon1          18
#define colon2          19
#define optionlist1     20
#define optionlist2     21
#define optionlist3     22
#define optionlist4     23
#define optionlist5     24
#define optionlist6     25
#define termdeflist1    26
#define termdeflist2    27
#define termdeflist3    28
#define termdeflist4    29
#define leftside1       30
#define leftside2       31
#define leftside3       32
#define expression1     33
#define expression2     34
#define expression3     35
#define expression4     36
#define term1           37
#define term2           38
#define term3           39
#define factor1         40
#define factor2         41
#define factor3         42
#define factor4         43
#define primary1        44
#define primary2        45
#define primary3        46
#define repeated1       47
#define repeated2       48
#define repeated3       49
#define repeated4       50
#define rightbrace1     51
#define rightbrace2     52

#define stackmax        50   /* size of attstack and parsestack */
#define stringmax       72   /* size of attribute string */
#define chbufmax        255   /* size of array chbuf */
#define redumax         15   /* size of reduction buffer */
#define recmax          100   /* max input record length */
#define errmax          9   /*max no of error messages per line */

/*include prodcon */
/* parser constants */
#define linemax         81   /* max. logical length of input lines */
#define ordeotchar      0   /* end of file marker    */

#define pchain          0
#define pnext           1
#define pkind           2
#define psymb           3
#define perr            4
#define prs             3
#define pprod           4
#define plb             3

/* Constants defining internal table sizes: ----------------------*/

#define maxPRODS        300   /* maximum number of productions */
#define maxRHSterms     15   /* maximum number of RHSterms */
#define maxSYMBS        300
    /* maximum no. of terminal and nonterminal symbols*/
#define maxTS           200   /* maximum optimizer counter table size */
#define maxRHSTS        9000
    /* maximum table size for production right sides */
#define maxBASIS        63   /* maximum number of elements in a basis set */
#define maxPT           3000   /* maximum size of parser tables */
#define maxLAH          2000
    /* maximum number of elements in the lookahead set*/
#define maxLAS          75   /* maximum size of lookahead elements */
#define LRstack         1000   /* maximum size of the lr(0) stack */
#define Nparstb         500   /* maximum number of parser tables */
#define NTtabSZ         100   /* maximum size of table for nonterminals */
#define TtabSZ          1200   /* maximum size of table for terminals */
#define BSarrSZ         6   /* BSarrSZ is maxSYMBS div (setmax+1) + 1 */
#define ParsTBS         30
    /* maximum size of a single table in parse table */
#define SYMBCNT         1500   /* notice: SYMBCNT > maxSYMBS */

#define SEGdim          1000   /* size of string buffer segments */

#define blank           "          "

#define DAYMSGlen       40   /* length of console messages         */
#define PAGElen         60   /* length of list page */
#define MSTRINGlen      54   /* length of mstrings */
#define SALFAlen        20   /* length of names */

#define symbmax         45
#define prodmax         52
#define lrmax           892
#define lxmax           66


/* finalparser types                        ----------------------*/
/* symbol = -1..maxSYMBS; */
typedef short errnum;

typedef schar rslength;

typedef short prodno;

typedef short lrinx;

typedef struct lrelm {
  unsigned chain : 11;   /*next item in this state*/
  unsigned next : 11;   /*next state*/
  signed int kind : 5, symb : 16;
  unsigned err : 9;
  signed int rs : 5;
  unsigned prodn : 9, lb : 11;
} lrelm;

typedef short lxinx;


typedef char psymbol;

typedef uchar salfa[SALFAlen];
typedef long bitset[(setmax >> 5) + 2];

typedef struct rtype {
  bitset r[BSarrSZ + 1];
} rtype;

typedef short stringindex;

typedef uchar charset[SEGdim];

typedef struct stringaddress {
  uchar *seg;
  stringindex loc;
} stringaddress;

typedef struct nontelm {
  stringaddress address;
  struct nontelm *rlink, *llink;
  unsigned nontermvalue : 9;
} nontelm;

typedef short prodtabx;

typedef struct termelm {
  struct termelm *niveaup, *higherp;
  signed int internv : 10;
  unsigned tchr : 8, defined_ : 1;
} termelm;

typedef struct entrytyp {
  termelm *higherp;
  signed int internv : 10;
  unsigned defined_ : 1;
} entrytyp;

typedef struct prodtype {
  unsigned leftside : 9, start : 14;
  struct configtype *lookbackp;
  unsigned laend : 4;
} prodtype;

typedef rtype bitmatrix[maxSYMBS + 1];
typedef char modetype;

typedef struct configtype {
  struct configtype *derivp, *nextp;
  unsigned mode : 4, inf : 9, prodnr : 9;
  struct configtype *lookbp;
} configtype;

typedef Char dayfilemsg[DAYMSGlen];
typedef Char mstring[MSTRINGlen];


typedef struct _REC_rhs {
  unsigned symbol : 9;
  configtype *parserp;
  unsigned link : 14, basisl : 6, basis : 1;
} _REC_rhs;

typedef struct _REC_help {
  unsigned afledt : 1, overgang : 1, prodindex : 9, rightp : 14;
  signed int status : 16;
  unsigned term : 1;
} _REC_help;

typedef struct _REC_state {
  unsigned errorno : 9;
  configtype *oldstart;
  unsigned newstart : 11;
} _REC_state;

typedef struct _REC_nonterms {
  nontelm *nontermp;
  unsigned isonleft : 1, isonright : 1;
} _REC_nonterms;

typedef struct _REC_frq {
  configtype *pexit;
  unsigned frequency : 12;
} _REC_frq;


/*H snapfname: mstring; H*/
mstring infilename;
int inflen;

short plr[lrmax + 1]={
/*    0*/   0,   5,1,   0,  0,  10, 350,1,   5,  1,  15, 130,1,  10, 13,
/*   15*/  20,  55,1,   7, 14,   0,  25,1,   8, 15,  30,  40,1,  13,  2,
/*   30*/   0,  35,1,  15, 19,   0, 755,3,   0, 19,   0, 755,3,   0, 18,
/*   45*/   0,  50,1,   4,  3,   0, 731,3,   2, 15,  30,  40,1,  13,  2,
/*   60*/  65,  75,1,   3,  4,   0,  70,1,   5, 21,   0,  80,3,   0, 21,
/*   75*/   0,  80,3,   0, 20,  85, 125,2,   3, 22,  90, 105,2,  14, 23,
/*   90*/  95, 100,2,   5, 24,   0, 843,4,   0, 14,   0,  80,3,   1, 24,
/*  105*/ 110, 120,1,   3,  4,   0, 115,1,   5, 25,   0,  80,3,   2, 25,
/*  120*/   0,  80,3,   2, 23,   0,  80,3,   1, 22,  30,  40,1,  13,  2,
/*  135*/   0, 140,1,   4,  3,   0, 145,3,   0, 26, 150, 185,2,   4, 27,
/*  150*/ 155, 175,2,  14, 28, 160, 165,2,  15, 29,   0, 848,4,   0, 13,
/*  165*/   0, 170,1,   4,  3,   0, 145,3,   2, 29,   0, 180,1,   4,  3,
/*  180*/   0, 145,3,   2, 28,   0, 145,3,   1, 27,   0, 195,1,  25,  5,
/*  195*/   0, 130,1,  10,  6,   0, 205,1,  25,  5, 210, 215,2,   9, 17,
/*  210*/   0, 853,4,   0,  6,  30,  40,1,  13,  2,   0, 225,1,   2,  7,
/*  225*/   0, 739,3,   2, 17,   0, 235,1,  25,  5,   0, 360,3,   5, 10,
/*  240*/   0, 245,1,  25,  5, 250, 130,1,  10,  8, 255,  25,1,   8, 15,
/*  255*/   0, 260,1,   5, 16,   0, 731,3,   0, 16,   0, 270,1,  25,  5,
/*  270*/   0, 130,1,  10,  6,   0, 280,1,  25,  5, 285, 215,2,   9, 17,
/*  285*/   0, 858,4,   0,  7,   0, 295,1,  25,  5,   0, 360,3,   7, 11,
/*  300*/   0, 305,1,  25,  5, 310, 215,2,   9, 17,   0, 863,4,   0,  5,
/*  315*/   0, 320,1,  25,  5,   0, 360,3,   5,  9,   0, 330,1,  25,  5,
/*  330*/ 335, 215,2,   9, 17,   0, 868,4,   0,  4,   0, 345,1,  25,  5,
/*  345*/   0, 360,3,   3,  8, 355, 260,4,  25, 16,   0, 873,4,   0,  3,
/*  360*/ 365, 700,1,  25,  9,   0, 370,1,   2, 30,   0, 375,3,   0, 30,
/*  375*/   0, 380,1,  15, 10, 385, 470,1,   5, 11, 390, 455,1,  19, 41,
/*  390*/ 395, 425,1,   2, 43, 400, 420,1,   4, 45, 405, 415,1,  17, 46,
/*  405*/   0, 410,1,  21, 47, 380, 590,1,  24, 12, 385, 470,1,   5, 11,
/*  420*/   0, 460,3,   0, 45, 430,  40,2,  13, 18, 435,  35,2,  15, 19,
/*  435*/   0, 878,4,   0, 44,   0, 410,1,  21, 13,   0, 807,3,   2, 43,
/*  450*/   0, 807,3,   0, 42, 385, 470,1,   5, 11,   0, 807,3,   0, 40,
/*  465*/   0, 799,3,   0, 37,   0, 775,3,   0, 34, 480, 510,2,   5, 39,
/*  480*/ 485, 455,2,  19, 41, 490, 425,2,   2, 43, 495, 420,2,   4, 45,
/*  495*/ 500, 415,2,  17, 46, 505, 410,2,  21, 47,   0, 883,4,   0, 33,
/*  510*/   0, 799,3,   1, 39,   0, 799,3,   1, 38, 525, 575,1,  20, 14,
/*  525*/   0, 530,1,  16, 35, 385, 535,1,   5, 11,   0, 775,3,   2, 36,
/*  540*/ 545, 510,2,   5, 39, 550, 455,2,  19, 41, 555, 425,2,   2, 43,
/*  555*/ 560, 420,2,   4, 45, 565, 415,2,  17, 46, 570, 410,2,  21, 47,
/*  570*/   0, 888,4,   0, 35,   0, 807,3,   2, 41, 525, 585,1,  18, 15,
/*  585*/   0, 460,3,   2, 46, 385, 470,1,   5, 11, 600, 530,1,  16, 16,
/*  600*/ 605, 615,1,  22, 51,   0, 610,1,  23, 52,   0, 827,3,   0, 52,
/*  615*/   0, 827,3,   0, 51,   0, 819,3,   3, 50, 595, 630,1,  24, 17,
/*  630*/ 635, 615,1,  22, 18, 380, 610,1,  23, 52, 600, 530,1,  16, 16,
/*  645*/   0, 819,3,   4, 49,   0, 819,3,   3, 48,   0, 819,3,   2, 47,
/*  660*/ 525, 665,1,  25, 19,   0, 670,3,   3, 12, 675, 695,1,  25, 20,
/*  675*/ 680, 690,1,   2, 31,   0, 685,1,   5, 32,   0, 375,3,   1, 32,
/*  690*/   0, 375,3,   1, 31,   0, 705,3,   2,  2,   0, 705,3,   1,  1,
/*  705*/   0, 710,1,   1, 21,   0,   0,0,   2,  0, 719, 200,5, 195    ,
/*  719*/ 723, 300,5, 245    , 727, 275,5, 270    ,   0, 325,5,   0    ,
/*  731*/ 735, 265,5, 245    ,   0, 190,5,   0    , 743, 290,5, 280    ,
/*  743*/ 747, 315,5, 305    , 751, 340,5, 330    ,   0, 230,5,   0    ,
/*  755*/ 759,  60,5,  55    , 763, 135,5, 130    , 767, 220,5, 215    ,
/*  767*/ 771, 440,5, 425    ,   0,  45,5,   0    , 779, 625,5, 410    ,
/*  779*/ 783, 580,5, 415    , 787, 520,5, 455    , 791, 595,5, 590    ,
/*  791*/ 795, 640,5, 630    ,   0, 660,5,   0    , 803, 540,5, 530    ,
/*  803*/   0, 475,5,   0    , 811, 515,5, 475    , 815, 515,5, 540    ,
/*  815*/   0, 465,5,   0    , 823, 445,5, 440    ,   0, 450,5,   0    ,
/*  827*/ 831, 655,5, 625    , 835, 650,5, 630    , 839, 645,5, 640    ,
/*  839*/   0, 620,5,   0    ,   0, 240,3,   2, 14,   0, 715,3,   2, 13,
/*  853*/   0, 360,3,   3,  6,   0, 360,3,   5,  7,   0, 360,3,   3,  5,
/*  868*/   0, 360,3,   1,  4,   0, 705,3,   0,  3,   0, 460,3,   0, 44,
/*  883*/   0, 775,3,   0, 33,   0, 775,3,   2, 35

};
lxinx pentryhp[ordmaxch + 1]={
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 3, 0, 11, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0,
 0, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 0,
 42, 0, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 57, 0, 0, 0, 0, 0, 0,
 0, 0, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0
};
psymbol pentrytv[ordmaxch + 1]={
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 24, 0, 0, 12, 17,
 18, 0, 0, 14, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0,
 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 20, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 21, 16, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* plx: array [0..lxmax] of lxelm; */
/*lexical tables*/
lxinx plxnp[lxmax + 1]={
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 66, 0
};
lxinx plxhp[lxmax + 1]={
 0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 0, 12, 13, 14, 15, 16, 17, 18,
 19, 0, 21, 22, 23, 24, 25, 0, 27, 28, 29, 30, 31, 32, 33, 0,
 35, 36, 37, 38, 39, 40, 41, 0, 43, 44, 45, 46, 47, 48, 49, 50,
 0, 52, 53, 54, 55, 56, 0, 58, 59, 60, 61, 62, 63, 64, 0, 0, 0

};
psymbol plxtv[lxmax + 1]={
 0, 26, 27, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 9,
 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0,
 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0,
 0, 0, 0, 10, 22, 23
};
uchar plxch[lxmax + 1]={
 '0', '*', '-', 'M', 'P', 'T', 'Y', 'T', 'E', 'R', 'M', 'O', 'A',
 'L', 'S', 'Y', 'M', 'B', 'O', 'L', 'P', 'T', 'I', 'O', 'N', 'S',
 'E', 'R', 'M', 'I', 'N', 'A', 'L', 'S', 'm', 'p', 't', 'y', 't',
 'e', 'r', 'm', 'o', 'a', 'l', 's', 'y', 'm', 'b', 'o', 'l', 'p',
 't', 'i', 'o', 'n', 's', 'e', 'r', 'm', 'i', 'n', 'a', 'l', 's',
 'L', 'R'
};

uchar *symbtab;
stringindex freesp;
prodtabx rhsnil;

entrytyp entry_[ordmaxch + 1];

_REC_rhs rhs[maxRHSTS + 1];
prodtabx lefthelp[maxSYMBS + 1];
prodtype prod[maxPRODS + 1];
_REC_help help[maxSYMBS + 1];
_REC_state state[Nparstb + 1];
stringaddress stringarray[maxSYMBS + 2];
int symbols[maxSYMBS];
_REC_nonterms nonterms[maxSYMBS + 1];
int emptyvalue;
nontelm *last, *first, *dynp, *statp;
prodtabx point, symbp, helpp, top;
short internv;
int prodcount, statecount, lalrend, statenum, lextabcount, lastnont,
	   firstnont, cnt, ntcount, goalsymbol, lasttermval, termrep, symb,
	   termg;
boolean firstnonterm, goalknown, emptyinexpr, goalnotentered,
	       errorsininput, leftflag, goalnotdefined, stopflag, isfirst;
configtype *rodp, *beginstate, *lqbep, *classtop, *oldp, *freeptr;
FILE *input, *output, *list, *console, *logfile;
_REC_frq frq[maxTS];
int frqi;
bitmatrix pt1s, ph1s;
int lineno;   /* listing counters*/
/*D trace, debug: boolean; D*/
/* for debug */
lrelm lr[maxLAH + 1];
int smax;


/*H
function  access( var f:mstring; n:integer ): integer; external name 'access';
procedure unlink( var s:mstring ); external name 'unlink';
 H*/

void newline(int n)
{
  int i;

  for (i = n; i >= 1; i--)
    putc('\n', list);
}


void bar(Char *mg)
{
  int msglen = DAYMSGlen, i = 0;
  int asterlen;

  putc('\n', list);
  while (msglen > i) {
    if (mg[msglen-1] == ' ')
      msglen--;
    else
      i = msglen;
  }
  asterlen = (77 - msglen) >> 1;
  putc(' ', list);
  for (i = 1; i <= asterlen; i++)
    putc('*', list);
  if (msglen != 0)
    putc(' ', list);
  else
    putc('*', list);
  for (i = 0; i < msglen; i++)
    putc(mg[i], list);
  if (msglen != 0)
    putc(' ', list);
  else
    putc('*', list);
  for (i = 1; i <= asterlen; i++)
    putc('*', list);
  if (!(msglen & 1))
    putc('*', list);
  fprintf(list, "\n\n");
}


void consoleflush(void)
{
  /* causes immediate physical write to console, not needed
             for most systems. */
  fflush(console);
  P_ioresult = 0;
}


void listterm(stringaddress a, int *len)
{
  int i, c, FORLIM;

  *len = a.seg[a.loc - 1];
  FORLIM = *len;
  for (i = 0; i < FORLIM; i++) {
    c = a.seg[a.loc + i];
    if (c >= ' ' && c < 128)
      putc(c, list);
    else
      fprintf(list, "%c%d%d%d", bslchar, c >> 6, (c & 63) >> 3, c & 7);
  }
}


void writenont(int i, int *t)
{
  int m;

  listterm(nonterms[i].nontermp->address, &m);
  *t += m;
}


void listgrammar(void);

jmp_buf _JL999;


void stop(int n)
{
  /* all abnormal termination is through this procedure*/
  switch (n) {

  case 0:   /*miscellaneous*/
    break;

  case 1:   /*consterror*/
    break;

  case 2:   /*error(s) in input*/
    break;

  case 3:  /*error(s) in grammar*/
    listgrammar();
    fprintf(list, "  Error(s) in grammar \n");
    fprintf(console, " Error(s) in grammar\n");
    consoleflush();
    break;

  case 4:   /*not slr/lalr(1) */
    break;

  case 5:
    fprintf(console, " Input file not readable\n");
    consoleflush();
    break;

  case 6:
    fprintf(console, " An input file must be specified\n");
    consoleflush();
    break;
  }
  longjmp(_JL999, 1);
  /*F halt(1) F*/
}


void consterror(uchar *s1, uchar *s2, int cc)
{
  newline(2);
  fprintf(list, " Errort in %.*s%.*s exceeded", SALFAlen, s1, SALFAlen, s2);
  fprintf(console, "%.*s exceeded (%d)\n", SALFAlen, s2, cc);
  consoleflush();
  stop(0);
}


void incrtabx(prodtabx *p)
{
  if (*p >= maxRHSTS)
    consterror("allocate            ", "maxRHSTS            ", maxRHSTS);
  (*p)++;
}


void writeterm(stringaddress a, int *t)
{
  int j;

  putc('"', list);
  listterm(a, &j);
  putc('"', list);
  *t += j + 2;
}


void listgrammar(void)
{
  prodtabx p;
  int i, k, l, t, tt;
  int prodno_ = -1;
  int plocal, FORLIM, FORLIM1;
  stringaddress *WITH;

  newline(2);
  bar("Expanded Grammar                        ");
  FORLIM = lastnont;
  for (i = firstnont; i <= FORLIM; i++) {  /*productions*/
    p = help[i].rightp;
    if (p != rhsnil) {  /*main production i*/
      prodno_++;
      fprintf(list, "\n%5d   ", prodno_);
      t = 3;
      if (i == firstnont) {
	fprintf(list, "METAGOAL");
	t += 8;
      } else
	writenont(i, &t);
      plocal = 1;
      fprintf(list, " = ");
      t++;
      while (p != rhsnil) {
	tt = t;
	FORLIM1 = rhs[p].basisl;
	for (k = 1; k <= FORLIM1; k++) {
	  incrtabx(&p);
	  if (i != firstnont || k > 1) {
	    if (tt > 70) {
	      putc('\n', list);
	      tt = t;
	      for (l = 1; l <= t + 7; l++)
		putc(' ', list);
	    }
	    symb = rhs[p].symbol;
	    if (symb <= termg)
	      writeterm(stringarray[symb+1], &tt);
	    else
	      writenont(symb, &tt);
	    putc(' ', list);
	    tt++;
	  }
	}
	p = rhs[p].link;   /*alternative productions*/
	if (p == rhsnil) {
	  putc('\n', list);
	  continue;
	}
	prodno_++;
	fprintf(list, "\n%5d", prodno_);
	for (l = 1; l <= t; l++)
	  putc(' ', list);
	fprintf(list, "| ");
	plocal++;
      }
    }
    if (!stopflag) {
      fprintf(output, "%5d ", plocal);
      if (i == firstnont)
	fprintf(output, "METAGOAL");
      else {
	WITH = &nonterms[i].nontermp->address;
	FORLIM1 = WITH->seg[WITH->loc - 1];
	for (k = 0; k < FORLIM1; k++)
	  putc(WITH->seg[WITH->loc + k], output);
      }
      putc('\n', output);
    }
  }
  if (!stopflag)
    fprintf(output, "%5d\n", 0);
  putc('\n', list);
  bar("                                        ");
}


/*H
procedure openfiles;
begin
   rewrite(console,'/dev/tty');
   if ParamCount<1 then stop(6) else infilename := ParamStr(1);
   inflen := MSTRINGlen; while infilename[inflen]=' ' do inflen := inflen-1;
   if inflen >= MSTRINGlen then stop(5)
   else infilename[inflen+1] := chr(0);
   if access(infilename,4) < 0 then stop(5) else reset(input,infilename);
   reset(tables,ParamStr(0) + '.dat');
   rewrite(list,'Pgen.list'); H*/
/*DH rewrite(logfile,'logfile') HD*/
/*H end; H*/


void openfiles(void)
{
  output = stdout; input = stdin; console = stderr;
  if (list != NULL)
    list = freopen("pg.list", "w", list);
  else
    list = fopen("pg.list", "w");
  if (list == NULL)
    _EscIO2(FileNotFound, "pg.list");
  if (P_argc < 2)
    stop(6);
  else
    P_sun_argv(infilename, sizeof(mstring), 1);
  inflen = 1;
  while (infilename[inflen-1] > ' ' && (infilename[inflen-1] & (~127)) == 0)
    inflen++;
  inflen--;
  if (input != NULL)
    input = freopen(P_trimname(infilename, sizeof(mstring)), "r", input);
  else {
    /*DU rewrite(logfile,'logfile') UD*/
    input = fopen(P_trimname(infilename, sizeof(mstring)), "r");
  }
  if (input == NULL)
    _EscIO2(FileNotFound, P_trimname(infilename, sizeof(mstring)));
}


/*F
procedure openfiles;
begin
   assign(list,'pg.list'); rewrite(list); if IOResult<>0 then halt(1);
   assign(console,'/dev/tty'); rewrite(console); if IOResult<>0 then halt(1);
   if ParamCount<1 then stop(6) else begin
      infilename := ParamStr(1);
      assign(input,infilename); reset(input); if IOResult<>0 then halt(1) end;
   F*/
/*DF if debug then begin
      assign(logfile,'logfile'); rewrite(logfile);
      if IOResult<>0 then halt(1) end; FD*/
/*F
   assign(tables,concat(ParamStr(0),'.dat')); reset(tables);
   if IOResult<>0 then halt(1)
   end; F*/

void headline(void)
{
  int i = MSTRINGlen, j = 0;

  /*H t: TimeStamp; H*/
  /*F YY,MM,DD: word; F*/
  fprintf(list, " Parser Generator (Version 5). File \"");
  while (i > j) {
    if (infilename[i-1] == ' ' || infilename[i-1] == '\0')
      i--;
    else
      j = i;
  }
  for (i = 0; i < j; i++)
    putc(infilename[i], list);
  fprintf(list, "\" \n");
  /*H GetTimeStamp(t); dt[1] := Date(t); dt[2] := Time(t); H*/
  /* U date(dt[1]); time(dt[2]); U */
  /* UH write(list, dt[1]:11,' ',dt[2]:11); HU */
  /*H write(list, dt[1]:11,' ',dt[2]:11); H*/
  /*F DeCodeDate(Date,YY,MM,DD); write(list,' ',YY,':',MM:1,':',DD); F*/
}


void asterline(void)
{
  int i;

  fprintf(list, "\n ");
  for (i = 1; i <= 79; i++)
    putc('*', list);
  newline(2);
}


void m(Char *s)
{
  fprintf(list, "      %.*s\n", MSTRINGlen, s);
  fprintf(console, " %.*s\n", MSTRINGlen, s);
  consoleflush();
}


void newelm(prodtabx *top, uchar *rname)
{
  if (*top >= maxRHSTS)
    consterror(rname, "maxRHSTS            ", maxRHSTS);
  (*top)++;
}


void gramerror(int i)
{
  fprintf(list, "\n  ***  ");
  switch (i) {

  case 1:
    fprintf(list, " Too many new names generated");
    fprintf(console, "Too many new names generated\n");
    consoleflush();
    break;

  case 2:
    fprintf(list,
      " The empty terminal must be defined when the right hand side is optional");
    fprintf(console, "Definition of 8 required\n");
    consoleflush();
    break;

  case 4:
    fprintf(list, " Empty is not the only right-side symbol");
    fprintf(console, "8 appears with other symbols\n");
    consoleflush();
    break;

  case 7:
    consterror("inputgrammar        ", "maxSYMBS            ", maxSYMBS);
    fprintf(console, "Installation-dependent constant exceeded\n");
    consoleflush();
    break;

  case 9:
    fprintf(list, " Invalid alternative");
    fprintf(console, "Invalid alternative\n");
    consoleflush();
    break;

  case 14:
    consterror("inputgrammar        ", "NTtabSZ             ", NTtabSZ);
    fprintf(console, "Installation-dependent constant exceeded\n");
    consoleflush();
    break;

  case 16:
    fprintf(list, " Undefined option number");
    fprintf(console, "An option number is not defined\n");
    consoleflush();
    break;

  case 18:
    fprintf(list, " Nonterminal too long ");
    fprintf(console, "A nonterminal is too long\n");
    consoleflush();
    break;

  case 19:
    fprintf(list, " Right-hand side too long (max 15) ");
    fprintf(console, "Too many terms in expression\n");
    consoleflush();
    break;

  default:
    fprintf(list, " Error: details unavailable");
    fprintf(console, "Error: details unavailable\n");
    consoleflush();
    break;
  }
  fprintf(list, "\n          ");
  errorsininput = true;
  if (i == 4)
    stopflag = true;
  else if (i < 0 || i >= 32 || ((1L << i) & 0xd0200L) == 0)
    stop(0);
}


/*---------------------------------------------------------------*/

void consoleterm(stringaddress a, int *len)
{
  int i, FORLIM;

  *len = a.seg[a.loc - 1];
  FORLIM = *len;
  for (i = 0; i < FORLIM; i++)
    putc(a.seg[a.loc + i], console);
}


void listtab(int l, int i)
{
  int hop;

  hop = i - (i + l - 1) % i - 1;
/* p2c: pg.p, line 536:
 * Note: Using % for possibly-negative arguments [317] */
  if (hop > 0)
    fprintf(list, "%*c", hop, ' ');
}


void termination(void)
{
  prodtabx p;
  boolean nonew, termboo, tb1;
  int i, j, k, FORLIM;
  _REC_rhs *WITH;

  FORLIM = lastnont;
  for (i = firstnont; i <= FORLIM; i++)
    help[i].term = false;
  do {
    nonew = true;
    FORLIM = lastnont;
    for (i = firstnont; i <= FORLIM; i++) {
      if (!help[i].term) {
	p = help[i].rightp;
	while (p != rhsnil) {
	  k = rhs[p].basisl;
	  termboo = true;
	  for (j = 1; j <= k; j++) {
	    incrtabx(&p);
	    WITH = &rhs[p];
	    if (WITH->symbol > termg) {
	      tb1 = help[WITH->symbol].term;
	      termboo = (termboo && tb1);
	    }
	  }
	  if (termboo) {
	    nonew = false;
	    help[i].term = true;
	    p = rhsnil;
	  } else
	    p = rhs[p].link;
	}
      }
    }
  } while (!nonew);
  termboo = false;
  k = 0;
  FORLIM = lastnont;
  for (i = firstnont + 1; i <= FORLIM; i++) {
    if (help[i].rightp != rhsnil && !help[i].term) {
      newline(2);
      putc(' ', list);
      writenont(i, &k);
      termboo = true;
    }
  }
  if (!termboo)
    return;
  newline(2);
  fprintf(list, " The above nonterminals cannot produce\n");
  fprintf(list, " a string of only terminal symbols \n");
  stop(3);
}


void allocp(configtype **p, int m)
{
  if (freeptr != NULL) {
    *p = freeptr;
    freeptr = freeptr->derivp;
  } else {
    switch (m) {

    case 1:
    case 2:
    case 3:
    case 4:
      *p = Malloc(sizeof(configtype));
      (*p)->inf = 0;
      (*p)->prodnr = 0;
      break;

    case 5:
      *p = Malloc(sizeof(configtype));
      (*p)->lookbp = NULL;
      break;
    }
  }
  (*p)->derivp = NULL;
  (*p)->nextp = NULL;
  (*p)->mode = m;
}


void countnextp(configtype *p)
{
  /* countnextp counts the frequency of nextp values in a lookback state */
  int i = 0;

  while (i < frqi) {
    i++;
    if (frq[i-1].pexit == p) {
      frq[i-1].frequency++;
      i = frqi + 1;
    }
  }
  if (i != frqi)
    return;
  frqi++;
  if (frqi > maxTS)
    consterror(" remember           ", "maxts               ", maxTS);
  frq[frqi-1].pexit = p;
  frq[frqi-1].frequency = 1;
}


void optimize(configtype **maxnextp, int *md)
{
  /* optimize finds the nextp that is most often used in
  the state, and deletes the state elements where it occurs*/
  configtype *p;
  int max, i, v;
  boolean brk;
  int FORLIM;
  _REC_frq *WITH;

  *maxnextp = frq[0].pexit;
  max = frq[0].frequency;
  FORLIM = frqi;
  for (i = 1; i < FORLIM; i++) {
    WITH = &frq[i];
    if (WITH->frequency > max) {
      max = WITH->frequency;
      *maxnextp = WITH->pexit;
    }
  }
  if (*md == 4) {
    p = beginstate;
    while (p->nextp != *maxnextp)
      p = p->derivp;
    v = p->prodnr;
  }
  while (beginstate->nextp == *maxnextp && beginstate->mode == *md)
    beginstate = beginstate->derivp;
  p = beginstate;
  do {
    oldp = p;
    brk = false;
    do {
      p = p->derivp;
      if (p == NULL)
	brk = true;
      else
	brk = (p->nextp != *maxnextp || p->mode != *md);
    } while (!brk);
    oldp->derivp = p;
  } while (p != NULL);
  if (*md == 4)
    *md = v;
}


/* PG5----------------------------------------------------------------*/

void emitconstants(void)
{
  lrinx si;
  int ix = 0;
  lrinx FORLIM;
  lrelm *WITH;

  FORLIM = smax;
  for (si = 0; si <= FORLIM; si++) {
    WITH = &lr[si];
    if (WITH->kind == 5)
      ix += 4;
    else
      ix += 5;
  }
  fprintf(output, "%5d symbmax \n", lastnont);
  fprintf(output, "%5d prodmax \n", prodcount);
  fprintf(output, "%5d lrmax   \n", smax);
  fprintf(output, "%5d lrlin   \n", ix);
  fprintf(output, "%5d lxmax   \n", lextabcount);
  newline(2);
  fprintf(list, " Number of terminal symbols      = %5d\n", lastnont);
  fprintf(list, " Number of productions           = %5d\n", prodcount);
  fprintf(list, " Number of LALR table entries    = %5d\n", ix);
  fprintf(list, " Number of lexical table entries = %5d\n", lextabcount);
  newline(2);
}


lxinx incrtinx(lxinx *ttmp)
{
  if (*ttmp >= TtabSZ)
    consterror("emitlex             ", "TtabSZ              ", TtabSZ);
  else
    (*ttmp)++;
  return (*ttmp);
}


typedef struct lxelm {
  unsigned np : 11, hp : 11;
  short tv;
  uchar ch;
} lxelm;


/* Local variables for emitlex: */
struct LOC_emitlex {
  lxelm lx[TtabSZ + 257];
  lxinx tinx;
} ;

Local void visit(termelm *p, struct LOC_emitlex *LINK)
{
  termelm *nmin, *tix;
  lxelm *WITH1;

  WITH1 = &LINK->lx[LINK->tinx+256];
  /* make sure lexigraphic first char is in this node */
  tix = p->niveaup;
  WITH1->ch = p->tchr;
  nmin = p;
  while (tix != NULL) {
    if (tix->tchr < WITH1->ch) {
      WITH1->ch = tix->tchr;
      nmin = tix;
    }
    tix = tix->niveaup;
  }
  if (nmin != p) {
    nmin->tchr = p->tchr;
    p->tchr = WITH1->ch;
    WITH1->tv = nmin->internv;
    nmin->internv = p->internv;
    p->internv = WITH1->tv;
    tix = nmin->higherp;
    nmin->higherp = p->higherp;
    p->higherp = tix;
  }
  WITH1->tv = p->internv;
  WITH1->ch = p->tchr;
  if (p->niveaup != NULL) {
    WITH1->np = incrtinx(&LINK->tinx);
    visit(p->niveaup, LINK);
  } else
    WITH1->np = 0;
  if (p->higherp != NULL) {
    WITH1->hp = incrtinx(&LINK->tinx);
    visit(p->higherp, LINK);
  } else
    WITH1->hp = 0;
}

Local void convertlex(struct LOC_emitlex *LINK)
{
  int i;
  lxelm *WITH;
  entrytyp *WITH1;

  /*transform class termtree into array lx*/
  LINK->tinx = 0;
  LINK->lx[256].ch = ' ';
  LINK->lx[256].hp = 0;
  LINK->lx[256].np = 0;
  LINK->lx[256].tv = 0;
  /* lx[0] is not used */
  for (i = ordminch; i <= ordmaxch; i++) {
    WITH = &LINK->lx[i - ordmaxch + 255];
    WITH1 = &entry_[i];
    WITH->tv = WITH1->internv;
    WITH->ch = i;
    WITH->np = 0;
    if (WITH1->higherp != NULL) {
      WITH->hp = incrtinx(&LINK->tinx);
      visit(WITH1->higherp, LINK);
    } else
      WITH->hp = 0;
  }
}


void emitlex(void)
{
  struct LOC_emitlex V;
  lxinx t;
  uchar chlastused = ' ';
  lxinx FORLIM;
  lxelm *WITH;

  convertlex(&V);
  FORLIM = V.tinx;
  for (t = ordminch - ordmaxch - 1; t <= FORLIM; t++) {
    WITH = &V.lx[t+256];
    if (t == 0)
      fprintf(output, " %c0 0 0\n", chlastused);
    if (t >= 0 || WITH->tv > 0 || WITH->hp > 0) {
      fprintf(output, " %c%d %d ", WITH->ch, WITH->np, WITH->hp);
      if (WITH->tv < 0)
	fprintf(output, "%d\n", 0);
      else
	fprintf(output, "%d\n", WITH->tv);
      chlastused = WITH->ch;
    }
  }
  putc('\n', list);
}


void writlr(lrinx si)
{
  lrelm *WITH;

  WITH = &lr[si];
  fprintf(list, " %3d%6d%5d%5d", si, WITH->chain, WITH->next, WITH->kind);
  switch (WITH->kind) {

  case 1:
  case 2:
  case 4:
  case 6:
    fprintf(list, "%5d%5d     ", WITH->symb, WITH->err);
    break;

  case 0:
  case 3:
    fprintf(list, "%5d%5d     ", WITH->rs, WITH->prodn);
    break;

  case 5:
    fprintf(list, "%5d          ", WITH->lb);
    break;
  }
}


void emitpars(void)
{
  lrinx si;
  int ix = 0;
  lrinx FORLIM;
  lrelm *WITH;

  FORLIM = smax;
  for (si = 0; si <= FORLIM; si++) {
    WITH = &lr[si];
    fprintf(output, "%d\n", ix);
    if (WITH->kind == 5)
      ix += 4;
    else
      ix += 5;
  }
  FORLIM = smax;
  for (si = 0; si <= FORLIM; si++) {
    WITH = &lr[si];
    fprintf(output, "%d %d %d ", WITH->chain, WITH->next, WITH->kind);
    switch (WITH->kind) {

    case 1:
    case 2:
    case 4:
    case 6:
      fprintf(output, "%d %d\n", WITH->symb, WITH->err);
      break;

    case 0:
    case 3:
      fprintf(output, "%d %d\n", WITH->rs, WITH->prodn);
      break;

    case 5:
      fprintf(output, "%d\n", WITH->lb);
      break;
    }
  }
}


void lapres(configtype **start)
{
  configtype *sp;
  int m = 4;
  configtype *WITH;

  sp = *start;
  beginstate = sp;
  frqi = 0;   /*beginstate and frqi are used in optimize*/
  while (sp != NULL) {
    WITH = sp;
    if (WITH->mode == 4)
      countnextp(WITH->nextp);
    sp = WITH->derivp;
  }
  optimize(&sp, &m);
  allocp(&classtop, 1);
  oldp->derivp = classtop;   /*oldp is set in optimize*/
  WITH = classtop;
  WITH->inf = 0;
  WITH->prodnr = m;
  WITH->mode = 4;
  WITH->nextp = sp;
  WITH->derivp = NULL;
  *start = beginstate;
}


int stateno(configtype *pt)
{
  int k;

  if (pt == NULL)
    return 0;
  else {
    k = 0;
    while (state[k].oldstart != pt)
      k++;
    return (state[k].newstart);
  }
}


void deletetail(int s, lrinx xstart, lrinx *xsi)
{
  int sinx;
  int t = 1;
  int u, v;
  lrinx tinx, sx, tx, oldsinx;
  lrinx smatch = maxLAH;
  configtype *sptr, *tptr, *sp, *tp;
  boolean maxmatch = false;
  lrelm *WITH;
  configtype *WITH1;

  while (t < s && !maxmatch) {
    sinx = lr[xstart].chain;
    oldsinx = xstart;
    sptr = state[s].oldstart->derivp;
    while (sinx > 0 && sinx < smatch) {
      tinx = state[t].newstart;
      tptr = state[t].oldstart;
      v = 0;
      while (tinx > v) {
	if (lr[sinx].symb == lr[tinx].symb) {
	  sx = sinx;
	  tx = tinx;
	  sp = sptr;
	  tp = tptr;
	  u = 0;
	  while (sx > u && tx > 0) {
	    WITH = &lr[sx];
	    WITH1 = sp;
	    if (WITH->symb == lr[tx].symb && WITH->kind == lr[tx].kind &&
		WITH1->nextp == tp->nextp) {
	      sx = WITH->chain;
	      sp = WITH1->derivp;
	      tx = lr[tx].chain;
	      tp = tp->derivp;
	    } else
	      u = sx + 1;
	  }
	  if (u == 0 && sx == tx) {
	    /*sx=tx=0*/
	    /*successful match*/
	    maxmatch = (oldsinx == xstart);
	    smatch = oldsinx;
	    *xsi = smatch + 1;
	    lr[smatch].chain = tinx;
	    v = tinx + 1;
	  }
	}
	if (v == 0) {
	  tinx = lr[tinx].chain;
	  tptr = tptr->derivp;
	}
      }  /*while tinx>0 do*/
      if (v != 0) {
	sinx = -1;
	break;
      }
      oldsinx = sinx;
      sinx = lr[sinx].chain;
      sptr = sptr->derivp;
    }  /*while sinx>0....*/
    t++;
  }
}


void compress(void)
{
  int i;
  configtype *statep;
  lrinx si = 0;
  lrinx sstart;
  int FORLIM;
  configtype *WITH;
  lrelm *WITH1;

  FORLIM = statecount;
  /*copy parser to lr and merge identical tails of states*/
  for (i = 0; i <= FORLIM; i++) {
    statep = state[i].oldstart;
    sstart = si;
    state[i].newstart = sstart;
    if (((1L << statep->mode) & 0x14) != 0) {
      lapres(&statep);
      WITH = state[i].oldstart;
      WITH->derivp = statep->derivp;
      WITH->nextp = statep->nextp;
      WITH->mode = statep->mode;
      WITH->inf = statep->inf;
      WITH->prodnr = statep->prodnr;
    }
    while (statep != NULL) {
      WITH1 = &lr[si];
      WITH = statep;
      if (WITH->derivp == NULL)
	WITH1->chain = 0;
      else
	WITH1->chain = si + 1;
      if (((1L << WITH->mode) & 0x18) != 0) {   /*prodnr is undef if mode=5*/
	if (prod[WITH->prodnr].laend == 0)
	  WITH->mode = 6;
      }
      WITH1->kind = WITH->mode;
      switch (WITH->mode) {

      case 1:
      case 2:
      case 4:
      case 6:
	WITH1->symb = WITH->inf;
	WITH1->err = WITH->prodnr;
	break;

      case 3:
	WITH1->rs = WITH->inf;
	WITH1->prodn = WITH->prodnr;
	if (WITH1->prodn == 0)
	  WITH1->kind = 0;
	break;

      case 5:
	/* blank case */
	break;
      }
      statep = WITH->derivp;
      if (si >= maxLAH - 1)
	consterror("compress            ", "maxLAH              ", maxLAH);
      si++;
    }
    if (lr[sstart].kind >= 0 && lr[sstart].kind < 32 &&
	((1L << lr[sstart].kind) & 0x16) != 0)
      deletetail(i, sstart, &si);
  }
  smax = si - 1;   /*index of last element used in lr*/
  FORLIM = statecount;
  for (i = 0; i <= FORLIM; i++) {
    sstart = state[i].newstart;
    statep = state[i].oldstart;
    si = sstart;
    while (statep != NULL) {
      WITH1 = &lr[si];
      WITH = statep;
      WITH1->next = stateno(WITH->nextp);
      if (WITH->mode == 5)
	WITH1->lb = stateno(WITH->lookbp);
      si = WITH1->chain;
      if (si < sstart)
	statep = NULL;
      else
	statep = WITH->derivp;
    }
  }
}


void errortable(void)
{
  int i, i5, j, kz;
  int no = 0;
  int ct, ctmax;
  lrinx lqbep, testp, gemp;
  int FORLIM;
  _REC_state *WITH;
  lrelm *WITH1;
  stringaddress *WITH2;
  int FORLIM1;
  _REC_state *WITH3;

  putc('\n', list);
  bar("Syntax error strings                    ");
  fprintf(list, "     Error :       Expected Symbols");
  FORLIM = statecount;
  for (i = 1; i <= FORLIM; i++)
    state[i].errorno = 0;
  FORLIM = statecount;
  for (i = 1; i <= FORLIM; i++) {
    WITH = &state[i];
    if (lr[WITH->newstart].kind == 1 && WITH->errorno == 0) {
      if (no < maxPRODS)
	no++;
      else
	consterror("errortable          ", "maxprods            ", maxPRODS);
      WITH->errorno = no;
      lr[WITH->newstart].err = no;
      newline(2);
      fprintf(list, "%10d :        ", no);
      kz = 1;
      i5 = -1;
      lqbep = WITH->newstart;
      while (lqbep != 0) {
	kz++;
	lqbep = lr[lqbep].chain;
      }
      fprintf(output, "%5d%5d\n", no, kz - 1);
      lqbep = WITH->newstart;
      while (lqbep != 0) {
	WITH1 = &lr[lqbep];
	listterm(stringarray[WITH1->symb + 1], &j);
	listtab(j, 10);
	putc(' ', list);
	WITH2 = &stringarray[WITH1->symb + 1];
	FORLIM1 = WITH2->seg[WITH2->loc - 1];
	for (j = 0; j < FORLIM1; j++)
	  putc(WITH2->seg[WITH2->loc + j], output);
	putc('\n', output);
	if (i5 == -1)
	  i5 = 0;
	lqbep = WITH1->chain;
	i5++;
	if (i5 == 5) {
	  fprintf(list, "\n%s%s", blank, blank);
	  i5 = 0;
	}
      }
      FORLIM1 = statecount;
      for (j = i + 1; j <= FORLIM1; j++) {
	if (lr[state[j].newstart].kind == 1 && state[j].errorno == 0) {
	  lqbep = WITH->newstart;
	  gemp = state[j].newstart;
	  ctmax = 0;
	  i5 = 0;
	  while (lqbep != i5) {
	    WITH1 = &lr[lqbep];
	    testp = gemp;
	    ct = 2;
	    while (lqbep != i5 && WITH1->symb != lr[testp].symb) {
	      testp = lr[testp].chain;
	      ct++;
	      if (testp == 0)
		i5 = lqbep;
	    }
	    if (i5 == 0) {
	      lqbep = WITH1->chain;
	      if (lr[testp].chain == 0)
		ctmax = ct;
	    }
	  }
	  if (i5 == 0 && kz == ctmax) {
	    WITH3 = &state[j];
	    WITH3->errorno = no;
	    lr[WITH3->newstart].err = no;
	  }
	}
      }
    }
  }
  fprintf(output, "%5d%5d\n", 0, 0);
  newline(4);
  asterline();
}


#define newentry        0
#define newvej          1
#define udenom          2


typedef struct _REC_lookant {
  unsigned ant : 7;
  configtype *point;
} _REC_lookant;

typedef struct _REC_stack {
  unsigned symb : 9, pnr : 9, token : 4;
} _REC_stack;

typedef struct _REC_ll {
  unsigned mrk : 1, lamrk : 1, lnk : 11;
  configtype *hidep;
} _REC_ll;

/* Local variables for verify: */
struct LOC_verify {
  int prodcnt, termcnt;
  configtype *start, *helpp, *lqbep1, *lastok;
  _REC_lookant lookant[ParsTBS];
  int la[maxLAS + 1];
  _REC_stack stack[maxSYMBS];
} ;

/* Local variables for lalrlookahead: */
struct LOC_lalrlookahead {
  struct LOC_verify *LINK;
  int l, info, lltop;   /*lltop is last used element of ll */
  _REC_ll ll[SYMBCNT + 1];
  prodtabx r;
} ;

Local void lalr1(configtype *statep, configtype *testp, int prodno_,
		 struct LOC_lalrlookahead *LINK)
{
  int leftinf, plngd, hinx;
  prodtabx rightp, prodp;   /*pointer to rightside */
  configtype *runp, *transp, *loopp, *chckp;
  boolean looping;
  prodtype *WITH;
  _REC_ll *WITH1;
  configtype *WITH2;

  WITH = &prod[prodno_];
  leftinf = WITH->leftside;
  plngd = WITH->laend;
  LINK->r = WITH->start;
  if (WITH->laend > 0)   /*rhs not empty*/
    LINK->r++;
  rightp = LINK->r;
  prodp = rightp;
  if (testp != NULL && plngd > 0) {
    plngd--;
    chckp = testp;
  } else
    chckp = statep;
  while (prodp != rhsnil) {
    runp = rhs[prodp].parserp;
    loopp = runp;
    LINK->r = rightp;
    for (hinx = 1; hinx <= plngd; hinx++) {
      LINK->info = rhs[LINK->r].symbol;
      while (loopp->inf != LINK->info)
	loopp = loopp->derivp;
      loopp = loopp->nextp;
      LINK->r++;
    }
    looping = true;
    if (loopp == chckp) {
      if (LINK->ll[leftinf].mrk) {
	hinx = leftinf;
	do {
	  WITH1 = &LINK->ll[hinx];
	  if (runp == WITH1->hidep) {
	    looping = false;
	    hinx = 0;
	  } else {
	    LINK->l = hinx;
	    hinx = WITH1->lnk;
	  }
	} while (hinx != 0);
	if (looping) {
	  LINK->lltop++;
	  if (LINK->lltop > SYMBCNT)
	    consterror("lalrlookah          ", "symbcnt             ",
		       SYMBCNT);
	  hinx = LINK->lltop;
	  LINK->ll[LINK->l].lnk = hinx;
	}
      } else
	hinx = leftinf;
      if (looping) {
	WITH1 = &LINK->ll[hinx];
	WITH1->mrk = true;
	WITH1->hidep = runp;
	WITH1->lnk = 0;
	transp = runp;
	while (transp->inf != leftinf)
	  transp = transp->derivp;
	transp = transp->nextp;
	loopp = transp;
	while (loopp != NULL) {
	  WITH2 = loopp;
	  if (WITH2->mode == 0)
	    lalr1(transp, runp, WITH2->prodnr, LINK);
	  else
	    LINK->ll[WITH2->inf].lamrk = true;
	  loopp = WITH2->derivp;
	}
      }
    }
    prodp = rhs[prodp].link;
  }
}

Local void lalrlookahead(configtype *pointp, int prodno_,
			 struct LOC_verify *LINK)
{
  struct LOC_lalrlookahead V;
  int j, FORLIM;
  _REC_ll *WITH;

  V.LINK = LINK;
  V.lltop = maxSYMBS;
  for (j = 0; j <= maxSYMBS; j++) {
    WITH = &V.ll[j];
    WITH->mrk = false;
    WITH->lamrk = false;
  }
  lalr1(pointp, NULL, prodno_, &V);
  lalrend = 0;
  FORLIM = termg;
  for (j = 0; j <= FORLIM; j++) {
    if (V.ll[j].lamrk) {
      LINK->la[lalrend] = j;
      if (lalrend < maxLAS)
	lalrend++;
      else
	consterror("lalrlookah          ", "maxLAS              ", maxLAS);
    }
  }
  if (lalrend != 0)
    return;
  newline(2);
  fprintf(list, " ***** prod%12d", prodno_);
  newline(2);
}

Local void lookahead(int *a, struct LOC_verify *LINK)
{
  prodtabx begindp, p1, symbp;
  int i;
  int j = 0;
  int k, pnr, cnt, prodnr;
  int lax = 0;
  int symbol;
  int llimit = 1;
  int stack[maxLAS + 1], buffer[maxLAS + 1];
  boolean laloop, repeatla, exitla;
  int FORLIM;
  _REC_help *WITH;

  FORLIM = lastnont;
  for (pnr = firstnont; pnr <= FORLIM; pnr++)
    help[pnr].status = 0;
  for (i = 0; i <= maxLAS; i++)
    LINK->la[i] = 0;
  begindp = prod[0].start;
  lalrend = 0;
  do {
    laloop = true;
    prodnr = 0;
    i = 1;
    p1 = begindp;
    do {
      repeatla = false;
      exitla = false;
      incrtabx(&p1);
      if (i > prod[prodnr].laend) {
	prodnr++;
	i = 1;
	if (prodnr > prodcount)
	  exitla = true;
	else {
	  p1 = prod[prodnr].start;
	  incrtabx(&p1);
	}
      }
      if (!exitla) {
	while (rhs[p1].symbol != *a && p1 < top) {
	  incrtabx(&p1);
	  i++;
	  if (i <= prod[prodnr].laend)
	    continue;
	  prodnr++;
	  i = 1;
	  if (prodnr > prodcount)
	    exitla = true;
	  else {
	    p1 = prod[prodnr].start;
	    incrtabx(&p1);
	  }
	}
	if (!exitla) {
	  if (p1 < top) {
	    if (i < prod[prodnr].laend) {
	      incrtabx(&p1);
	      symbol = rhs[p1].symbol;
	      symbp = p1;
	      i++;
	    } else {
	      pnr = prod[prodnr].leftside;
	      /* to stack */
	      WITH = &help[pnr];
	      if (WITH->status == 0 && pnr != *a) {
		if (j < maxLAS) {
		  j++;
		  WITH->status = 1;
		  stack[j] = pnr;
		} else
		  consterror("lookahead           ", "maxLAS              ",
			     maxLAS);
	      }
	      i++;
	      repeatla = true;
	    }
	  } else
	    exitla = true;
	  if (!exitla && !repeatla) {
	    if (symbol > termg) {
	      FORLIM = firstnont;
	      for (k = 0; k < FORLIM; k++) {
		if (P_inset(k % (setmax + 1),
			    ph1s[symbol].r[k / (setmax + 1)])) {
/* p2c: pg.p, line 1219:
 * Note: Using % for possibly-negative arguments [317] */
		  buffer[lax] = k;
		  lax++;
		  if (lax > maxLAS)
		    consterror("lookahead           ", "maxLAS              ",
			       maxLAS);
		  lalrend++;
		}
	      }
	    } else {
	      buffer[lax] = symbol;
	      lalrend++;
	      lax++;
	      if (lax > maxLAS)
		consterror("lookahead           ", "maxLAS              ",
			   maxLAS);
	    }
	    p1 = symbp;
	    i++;
	    repeatla = true;
	  }
	}
      }
    } while (repeatla);
    if (j > 0) {
      *a = stack[j];
      j--;
    } else
      laloop = false;
  } while (laloop);
  if (lalrend > 0)
    LINK->la[0] = buffer[0];
  j = 1;
  i = 0;
  cnt = lalrend;
  while (j < cnt) {
    while (LINK->la[i] != buffer[j] && i < llimit)
      i++;
    if (i == llimit) {
      LINK->la[llimit] = buffer[j];
      llimit++;
    } else
      lalrend--;
    j++;
    i = 0;
  }
}

Local void voptimize(char x, struct LOC_verify *LINK)
{
  switch (x) {

  case newentry:
    LINK->lastok->derivp = LINK->helpp->derivp;
    LINK->lastok->nextp = LINK->helpp->nextp;
    LINK->lastok->mode = LINK->helpp->mode;
    if (LINK->helpp->mode == 5)
      LINK->lastok->lookbp = LINK->helpp->lookbp;
    else if (LINK->helpp->mode > 0 && LINK->helpp->mode < 5) {
      LINK->lastok->inf = LINK->helpp->inf;
      LINK->lastok->prodnr = LINK->helpp->prodnr;
    }
    break;

  case newvej:
    LINK->lastok->derivp = LINK->helpp;
    LINK->lastok = LINK->helpp;
    break;

  case udenom:
    LINK->lastok->derivp = LINK->helpp;
    break;
  }
}

Local void lalrtext(int x, struct LOC_verify *LINK)
{
  asterline();
  fprintf(list, "         ");
  switch (x) {

  case 1:
    fprintf(list, "The grammar is LALR1 \n");
    fprintf(console, " The grammar is LALR1\n");
    consoleflush();
    asterline();
    break;

  case 2:
    fprintf(list, "The grammar is not LALR1 \n");
    fprintf(list, "         ");
    fprintf(console, " The grammar is not LALR1\n");
    consoleflush();
    fprintf(list, "Processing stops \n");
    asterline();
    stop(0);
    break;
  }
}

Local void adeqwrite(int x, struct LOC_verify *LINK)
{
  int i, j, k, l, kk, m;
  configtype *helpp;
  int FORLIM;

  switch (x) {

  case 1:
    bar("A list of non-slr1 states               ");
    fprintf(list, "           ");
    fprintf(list, "in each of the following states the \n");
    fprintf(list, "           ");
    fprintf(list, "sets of symbols are not disjoint \n");
    break;

  case 2:
    bar("End of non-slr1 states                  ");
    break;

  case 3:
    fprintf(list, "\n           ");
    for (j = 1; j <= 61; j++)
      putc('-', list);
    putc('\n', list);
    if (LINK->termcnt > 0) {
      fprintf(list, "           ");
      fprintf(list, "Reading continues if the next input symbol\n");
      fprintf(list, "                      ");
      fprintf(list, "is one of the following: \n");
      fprintf(list, "                      ");
      fprintf(list, "Symbol       Production    Symbol no.\n");
      FORLIM = LINK->termcnt;
      for (j = 0; j < FORLIM; j++) {
	kk = LINK->stack[j].pnr;
	l = LINK->stack[j].token;
	k = LINK->stack[j].symb;
	fprintf(list, "                       ");
	listterm(stringarray[k+1], &i);
	fprintf(list, "%12d", kk);
	fprintf(list, "%12d\n", l);
      }
      putc('\n', list);
    }
    FORLIM = LINK->prodcnt;
    for (j = 0; j < FORLIM; j++) {
      l = LINK->lookant[j].ant;
      helpp = LINK->lookant[j].point;
      fprintf(list, "           ");
      fprintf(list, "Reduction  no.");
      m = helpp->prodnr;
      fprintf(list, "%4d", m);
      fprintf(list, " is performed if the next input symbol\n");
      fprintf(list, "                      ");
      fprintf(list, "is one of the following : \n");
      kk = 0;
      do {
	kk++;
	fprintf(list, "                      ");
	k = helpp->inf;
	listterm(stringarray[k+1], &i);
	putc('\n', list);
	helpp = helpp->derivp;
      } while (kk != l);
      putc('\n', list);
    }
    break;

  case 4:   /* earlier user for writing warning states */
    break;

  case 5:
    bar("A list of non-lalr1 states              ");
    fprintf(list, "           ");
    fprintf(list, "In each of the following states the\n");
    fprintf(list, "           ");
    fprintf(list, "sets of symbols are not disjoint\n");
    break;

  case 6:
    bar("End of non-lalr1 states                 ");
    break;
  }
}

Local int mendcheck(struct LOC_verify *LINK)
{
  int iboo = 0, count = 0;
  int checksymb, k, step1, step2, l1, l2, j, l;
  configtype *step1p, *step2p, *stepp1, *stepp2;

  LINK->helpp = LINK->start;
  if (LINK->termcnt > 0) {
    count = 0;
    while (count < LINK->termcnt) {
      count++;
      checksymb = LINK->stack[count-1].symb;
      j = 0;
      do {
	j++;
	k = LINK->lookant[j-1].ant;
	LINK->lqbep1 = LINK->lookant[j-1].point;
	l = 0;
	do {
	  l++;
	  if (LINK->lqbep1->inf == checksymb) {
	    iboo = 1;
	    l = k;
	    j = LINK->prodcnt;
	    count = LINK->termcnt;
	  }
	  LINK->lqbep1 = LINK->lqbep1->derivp;
	} while (l != k);
      } while (j != LINK->prodcnt);
    }
  }
  if (iboo != 0 || LINK->prodcnt <= 1)
    return iboo;
  k = 0;
  do {
    k++;
    step1 = LINK->lookant[k-1].ant;
    step1p = LINK->lookant[k-1].point;
    j = k;
    do {
      stepp1 = step1p;
      j++;
      step2 = LINK->lookant[j-1].ant;
      step2p = LINK->lookant[j-1].point;
      l1 = 0;
      do {
	l1++;
	l2 = 0;
	stepp2 = step2p;
	do {
	  l2++;
	  if (stepp1->inf == stepp2->inf) {
	    l2 = step2;
	    l1 = step1;
	    j = LINK->prodcnt;
	    iboo = 1;
	    k = LINK->prodcnt - 1;
	  }
	  stepp2 = stepp2->derivp;
	} while (l2 != step2);
	stepp1 = stepp1->derivp;
      } while (l1 != step1);
    } while (j != LINK->prodcnt);
  } while (k != LINK->prodcnt - 1);
  return iboo;
}

Local configtype *findreduce(int pnr, struct LOC_verify *LINK)
{
  prodtabx p1, p2;
  configtype *pt;
  prodtype *WITH;

  WITH = &prod[pnr];
  if (WITH->laend == 0)
    return (WITH->lookbackp);
  else {
    p1 = WITH->start;
    while (rhs[p1].parserp->derivp != NULL) {
      p2 = p1;
      p1 = rhs[p1].link;
      if (p1 != rhsnil)
	continue;
      allocp(&pt, 1);
      pt->mode = 3;
      pt->inf = WITH->laend - 1;
      pt->prodnr = pnr;
      pt->nextp = WITH->lookbackp;
      pt->derivp = NULL;
      statecount++;
      if (statecount > Nparstb)
	consterror("findreduce          ", "Nparstb             ", Nparstb);
      state[statecount].oldstart = pt;
      newelm(&top, "findreduce          ");
      p1 = top;
      rhs[p1].parserp = pt;
      rhs[p1].link = rhsnil;
      rhs[p2].link = p1;
    }
    return (rhs[p1].parserp);
  }
}


/* PG4----------------------------------------------------------------*/
void verify(void)
{
  struct LOC_verify V;
  int iboo, i, j, k, pnr, l, count;
  boolean slr1 = true, headwrite = true;
  configtype *lqbep2, *lqbep3, *stopp;
  int FORLIM;
  configtype *WITH;
  prodtype *WITH1;

  FORLIM = statenum;
  for (i = 1; i <= FORLIM; i++) {
    V.start = state[i].oldstart;
    V.prodcnt = 0;
    V.termcnt = 0;
    V.helpp = V.start;
    do {
      k = V.helpp->mode;
      if (k == 0)
	V.prodcnt++;
      else if (V.helpp->inf <= termg && V.helpp->nextp != NULL)
	V.termcnt++;
      V.helpp = V.helpp->derivp;
    } while (V.helpp != NULL);
    if (V.prodcnt > ParsTBS)
      consterror("verify              ", "ParsTBS             ", ParsTBS);
    if (V.prodcnt > 1 || V.prodcnt == 1 && V.termcnt > 0) {
      V.prodcnt = 0;
      V.termcnt = 0;
      V.helpp = V.start;
      do {
	k = V.helpp->mode;
	if (k == 0) {
	  pnr = V.helpp->prodnr;
	  lalrlookahead(V.start, pnr, &V);
	  l = lalrend - 1;
	  allocp(&V.lqbep1, 1);
	  V.helpp->nextp = V.lqbep1;
	  V.prodcnt++;
	  V.lookant[V.prodcnt-1].ant = lalrend;
	  V.lookant[V.prodcnt-1].point = V.lqbep1;
	  lqbep3 = findreduce(pnr, &V);
	  for (j = 0; j <= l; j++) {
	    lqbep2 = V.lqbep1;
	    lqbep2->nextp = lqbep3;
	    lqbep2->inf = V.la[j];
	    lqbep2->prodnr = pnr;
	    lqbep2->mode = 4;
	    if (j == l)
	      lqbep2->derivp = V.helpp->derivp;
	    else {
	      allocp(&V.lqbep1, 1);
	      lqbep2->derivp = V.lqbep1;
	    }
	  }
	} else {
	  WITH = V.helpp;
	  if (WITH->inf <= termg && WITH->nextp != NULL) {
	    V.termcnt++;
	    if (V.termcnt > maxSYMBS)
	      consterror("verify              ", "maxsymbs            ",
			 maxSYMBS);
	    else {
	      V.stack[V.termcnt-1].symb = WITH->inf;
	      V.stack[V.termcnt-1].pnr = WITH->prodnr;
	      V.stack[V.termcnt-1].token = WITH->mode;
	    }
	  }
	  WITH->mode = 2;
	}
	V.helpp = V.helpp->derivp;
      } while (V.helpp != NULL);
      iboo = mendcheck(&V);
      if (iboo != 0) {
	if (slr1 && headwrite) {
	  adeqwrite(5, &V);
	  if (iboo == 1)
	    slr1 = false;
	  else
	    headwrite = false;
	}
	adeqwrite(3, &V);
      }
    } else if (V.prodcnt == 0) {
      V.helpp = V.start;
      while (V.helpp != NULL) {
	V.helpp->mode = 1;
	V.helpp = V.helpp->derivp;
      }
    }
  }
  if (!slr1) {
    adeqwrite(6, &V);
    lalrtext(2, &V);
    stop(0);
    return;
  }
  FORLIM = statenum;
  for (i = 1; i <= FORLIM; i++) {
    V.start = state[i].oldstart;
    V.lastok = V.start;
    V.prodcnt = 0;
    V.termcnt = 0;
    count = 0;
    V.lqbep1 = V.start;
    do {
      V.helpp = V.lqbep1;
      count++;
      WITH = V.lqbep1;
      stopp = WITH->derivp;
      if (WITH->mode == 0 && WITH->nextp != NULL) {
	if (V.prodcnt == 0 && V.termcnt == 0) {
	  V.helpp = WITH->nextp;
	  voptimize(newentry, &V);
	} else {
	  V.helpp = WITH->nextp;
	  voptimize(newvej, &V);
	}
	V.prodcnt++;
	do {
	  lqbep3 = V.lastok->derivp;
	  if (lqbep3 != stopp)
	    V.lastok = lqbep3;
	} while (lqbep3 != stopp);
      } else if (WITH->mode == 0) {
	WITH1 = &prod[WITH->prodnr];
	WITH->mode = 3;
	WITH->nextp = WITH1->lookbackp;
	V.prodcnt++;
	if (WITH1->laend == 0)
	  WITH->inf = 0;
	else
	  WITH->inf = WITH1->laend - 1;
	if (count > 1)
	  voptimize(newentry, &V);
      } else if (WITH->inf <= termg) {
	if (count > 1) {
	  if (WITH->nextp != NULL) {
	    V.helpp = V.lqbep1;
	    if (V.prodcnt == 0 && V.termcnt == 0)
	      voptimize(newentry, &V);
	    else
	      voptimize(newvej, &V);
	  } else {
	    V.helpp = stopp;
	    voptimize(udenom, &V);
	    V.termcnt--;
	  }
	}
	V.termcnt++;
      } else {
	V.helpp = WITH->derivp;
	if (V.termcnt == 0 && V.prodcnt == 0)
	  voptimize(newentry, &V);
	else {
	  V.helpp = stopp;
	  voptimize(udenom, &V);
	}
      }
      V.lqbep1 = stopp;
    } while (V.lqbep1 != NULL);
  }
  lalrtext(1, &V);
}

#undef newentry
#undef newvej
#undef udenom


/* PG3----------------------------------------------------------------*/
void freelist(configtype **pt)
{
  configtype *p;

  p = *pt;
  while (p->derivp != NULL)
    p = p->derivp;
  p->derivp = freeptr;
  freeptr = *pt;
  *pt = NULL;
}


typedef struct _REC_stack_ {
  unsigned symbovg : 9;
  configtype *stp;
} _REC_stack_;

/* Local variables for lr0: */
struct LOC_lr0 {
  _REC_stack_ stack[LRstack];
  prodtabx lqbep2;
  int baslae, stacktop, stacktael;
} ;

Local void align(configtype *startp, boolean boo, struct LOC_lr0 *LINK)
{
  int i;
  configtype *WITH;
  _REC_rhs *WITH1;

  lqbep = startp;
  while (lqbep != NULL) {
    WITH = lqbep;
    LINK->lqbep2 = prod[WITH->prodnr].start;
    for (i = WITH->mode; i >= 1; i--)
      incrtabx(&LINK->lqbep2);
    if (rhs[LINK->lqbep2].parserp != NULL) {
      while (rhs[LINK->lqbep2].link != rhsnil)
	LINK->lqbep2 = rhs[LINK->lqbep2].link;
      rhs[LINK->lqbep2].link = top;
      LINK->lqbep2 = top;
      newelm(&top, "align               ");
    }
    WITH1 = &rhs[LINK->lqbep2];
    WITH1->basis = boo;
    if (boo)
      WITH1->basisl = LINK->baslae;
    WITH1->parserp = beginstate;
    WITH1->link = rhsnil;
    lqbep = WITH->derivp;
  }
}

Local void stackellerej(struct LOC_lr0 *LINK)
{
  configtype *WITH;
  _REC_help *WITH1;
  _REC_stack_ *WITH2;

  WITH = classtop;
  WITH1 = &help[WITH->inf];
  if (WITH1->overgang)
    return;
  LINK->stacktael++;
  if (LINK->stacktop >= LRstack)
    consterror("lr0                 ", "LRstack             ", LRstack);
  LINK->stacktop++;
  WITH1->overgang = true;
  WITH2 = &LINK->stack[LINK->stacktop-1];
  WITH2->symbovg = WITH->inf;
  WITH2->stp = beginstate;
}


void lr0(void)
{
  struct LOC_lr0 V;
  configtype *helpp, *closp, *oldp;
  int modeh, i, symb, prodno_;
  boolean copy, loopx, finlq;
  int goalprod = 0;
  configtype *WITH;
  _REC_stack_ *WITH1;
  _REC_help *WITH2;
  _REC_rhs *WITH3;
  prodtype *WITH4;
  configtype *WITH5;

  top = point;
  newelm(&top, "lr0                 ");
  statecount = 0;
  allocp(&rodp, 1);
  classtop = rodp;
  WITH = rodp;
  WITH->prodnr = goalprod;
  WITH->mode = 1;
  V.lqbep2 = prod[goalprod].start;
  incrtabx(&V.lqbep2);
  WITH->inf = rhs[V.lqbep2].symbol;
  WITH->derivp = NULL;
  V.baslae = 1;
  V.stacktop = 1;
  WITH1 = V.stack;
  WITH1->stp = rodp;
  WITH1->symbovg = rodp->inf;
  statenum = 0;
  state[statenum].oldstart = rodp;
  align(rodp, true, &V);
  allocp(&classtop, 1);
  beginstate = classtop;
  while (V.stacktop > 0) {
    WITH1 = &V.stack[V.stacktop-1];
    oldp = WITH1->stp;
    symb = WITH1->symbovg;
    lqbep = oldp;
    V.stacktael = 0;
    V.baslae = 0;
    V.stacktop--;
    for (i = 0; i <= maxSYMBS; i++) {
      WITH2 = &help[i];
      WITH2->afledt = false;
      WITH2->overgang = false;
    }
    while (lqbep->inf != symb)
      lqbep = lqbep->derivp;
    closp = lqbep;
    loopx = true;
    while (loopx) {
      if (V.baslae == maxBASIS)
	consterror("lr0                 ", "maxBASIS            ", maxBASIS);
      V.baslae++;
      WITH = classtop;
      WITH->nextp = NULL;
      WITH->prodnr = lqbep->prodnr;
      modeh = lqbep->mode + 1;
      if (prod[WITH->prodnr].laend < modeh) {
	WITH->mode = 0;
	WITH->inf = 0;
      } else {
	WITH->mode = modeh;
	V.lqbep2 = prod[WITH->prodnr].start;
	for (i = WITH->mode; i >= 1; i--)
	  incrtabx(&V.lqbep2);
	WITH->inf = rhs[V.lqbep2].symbol;
	stackellerej(&V);
      }
      do {
	lqbep = lqbep->derivp;
	if (lqbep == NULL) {
	  loopx = false;
	  finlq = true;
	} else
	  finlq = (lqbep->inf == symb);
      } while (!finlq);
      if (!loopx)
	break;
      helpp = classtop;
      allocp(&classtop, 1);
      helpp->derivp = classtop;
    }
    classtop->derivp = NULL;
    copy = false;
    V.lqbep2 = prod[beginstate->prodnr].start;
    for (i = beginstate->mode; i >= 1; i--)
      incrtabx(&V.lqbep2);
    if (rhs[V.lqbep2].parserp != NULL) {
      while (V.lqbep2 != rhsnil && !copy) {
	WITH3 = &rhs[V.lqbep2];
	if (WITH3->basis) {
	  if (V.baslae == WITH3->basisl) {
	    copy = true;
	    lqbep = beginstate;
	    while (lqbep != NULL && copy) {
	      WITH = lqbep;
	      copy = false;
	      helpp = WITH3->parserp;
	      while (helpp != NULL) {
		copy = ((WITH->prodnr == helpp->prodnr &&
			 WITH->mode == helpp->mode) || copy);
		helpp = helpp->derivp;
	      }
	      lqbep = WITH->derivp;
	    }
	  }
	}
	if (!copy)
	  V.lqbep2 = WITH3->link;
      }
    }
    if (copy) {
      closp->nextp = rhs[V.lqbep2].parserp;
      V.stacktop -= V.stacktael;
      freelist(&beginstate);
      allocp(&classtop, 1);
      beginstate = classtop;
      continue;
    }
    closp->nextp = beginstate;
    if (statenum == Nparstb)
      consterror("lr0                 ", "Nparstb             ", Nparstb);
    statenum++;
    state[statenum].oldstart = beginstate;
    align(beginstate, true, &V);
    closp = classtop;
    lqbep = beginstate;
    while (lqbep != NULL) {
      WITH = lqbep;
      WITH2 = &help[WITH->inf];
      if (WITH->mode > 0) {
	if (WITH->inf > termg) {
	  if (!WITH2->afledt) {
	    WITH2->afledt = true;
	    prodno_ = WITH2->prodindex;
	    while (prod[prodno_].leftside == WITH->inf) {
	      WITH4 = &prod[prodno_];
	      helpp = classtop;
	      allocp(&classtop, 1);
	      helpp->derivp = classtop;
	      WITH5 = classtop;
	      V.lqbep2 = WITH4->start;
	      incrtabx(&V.lqbep2);
	      WITH5->inf = rhs[V.lqbep2].symbol;
	      if (prod[prodno_].laend == 0)
		WITH5->mode = 0;
	      else
		WITH5->mode = 1;
	      WITH5->prodnr = prodno_;
	      WITH5->derivp = NULL;
	      WITH5->nextp = NULL;
	      if (classtop->mode > 0)
		stackellerej(&V);
	      prodno_++;
	    }
	  }
	}
      }
      lqbep = WITH->derivp;
    }
    if (closp->derivp != NULL)
      align(closp->derivp, false, &V);
    allocp(&classtop, 1);
    beginstate = classtop;
  }
}


void lookb(void)
{
  int prodnr = 0;
  int symb, md, r1;
  prodtabx point;
  configtype *stp;
  prodtype *WITH;
  configtype *WITH1;

  prod[0].lookbackp = NULL;
  statecount = statenum;
  while (prodnr < prodcount) {
    prodnr++;
    WITH = &prod[prodnr];
    frqi = 0;
    allocp(&classtop, 5);
    beginstate = classtop;
    symb = WITH->leftside;
    point = WITH->start;
    if (WITH->laend > 0)
      incrtabx(&point);
    while (point != rhsnil) {
      WITH1 = classtop;
      WITH1->mode = 5;
      stp = rhs[point].parserp;
      if (stp == NULL) {
	fprintf(console, " Error(s) in grammar\n");
	consoleflush();
	newline(2);
	fprintf(list, " The grammar cannot produce a string containing\n");
	putc(' ', list);
	r1 = 1;
	writenont(WITH->leftside, &r1);
	newline(2);
	stop(0);
      }
      WITH1->lookbp = stp;
      while (stp->inf != symb)
	stp = stp->derivp;
      stp = stp->nextp;
      countnextp(stp);
      WITH1->nextp = stp;
      oldp = classtop;
      allocp(&classtop, 5);
      oldp->derivp = classtop;
      point = rhs[point].link;
    }
    oldp->derivp = NULL;
    if (frqi == 1) {
      WITH->lookbackp = beginstate->nextp;
      freelist(&beginstate);
    } else {
      md = 5;
      optimize(&stp, &md);
      oldp->derivp = classtop;
      WITH1 = classtop;
      WITH1->derivp = NULL;
      WITH1->nextp = stp;
      WITH1->lookbp = NULL;
      WITH1->mode = 5;
      statecount++;
      if (statecount > Nparstb)
	consterror("lookb               ", "Nparstb             ", Nparstb);
      state[statecount].oldstart = beginstate;
      WITH->lookbackp = beginstate;
    }
    while (prod[prodnr+1].leftside == symb) {
      prodnr++;
      prod[prodnr].lookbackp = WITH->lookbackp;
    }
  }
}


/* PG2----------------------------------------------------------------*/
void initprod(void)
{
  prodtabx p;
  int k = -1;
  int i, l, FORLIM;
  prodtype *WITH;
  int FORLIM1;

  FORLIM = lastnont;
  for (i = firstnont; i <= FORLIM; i++) {
    p = help[i].rightp;
    while (p != rhsnil) {
      k++;
      if (k > maxPRODS)
	consterror("initprod            ", "maxprods            ", maxPRODS);
      WITH = &prod[k];
      WITH->leftside = i;
      WITH->start = p;
      WITH->laend = rhs[p].basisl;
      FORLIM1 = WITH->laend;
      for (l = 1; l <= FORLIM1; l++)
	incrtabx(&p);
      if (WITH->laend == 1 && rhs[WITH->start + 1].symbol == emptyvalue)
	WITH->laend = 0;
      p = rhs[p].link;
    }
  }
  prod[k+1].leftside = 0;
  prodcount = k;
  i = 0;
  while (i <= prodcount) {
    symb = prod[i].leftside;
    help[symb].prodindex = i;
    do {
      i++;
    } while (symb == prod[i].leftside);
  }
}


void iterer(rtype *matrix)
{
  int n, up1, up2, nn, m, k, kk, l, i, j;
  boolean done;
  bitset gem, boo;
  bitmatrix trans;
  rtype rfresh;
  int FORLIM;
  long SET[3];
  bitset SET1;
  rtype *WITH;

  n = lastnont / (setmax + 1);
  nn = lastnont - n * (setmax + 1);
  do {
    up1 = setmax;
    done = true;
    k = -1;
    for (m = 0; m <= n; m++) {
      if (m == n)
	up1 = nn;
      for (kk = 0; kk <= up1; kk++) {
	k++;
	j = -1;
	up2 = setmax;
	for (l = 0; l <= n; l++) {
	  P_expset(boo, 0);
	  if (l == n)
	    up2 = nn;
	  for (i = 0; i <= up2; i++) {
	    j++;
	    if (P_inset(kk, matrix[j].r[m]))
	      P_addset(boo, i);
	  }
	  P_setcpy(trans[k].r[l], boo);
	}
      }
    }
    for (i = 0; i <= BSarrSZ; i++)
      P_expset(rfresh.r[i], 0);
    FORLIM = lastnont;
    for (k = firstnont; k <= FORLIM; k++) {
      WITH = &matrix[k];
      i = -1;
      up2 = setmax;
      for (l = 0; l <= n; l++) {
	P_expset(gem, 0);
	if (l == n)
	  up2 = nn;
	for (m = 0; m <= up2; m++) {
	  i++;
	  P_expset(boo, 0);
	  for (j = 0; j <= n; j++)
	    P_setunion(boo, boo, P_setint(SET1, WITH->r[j], trans[i].r[j]));
	  if (*boo != 0)
	    P_addset(gem, m);
	}
	P_setcpy(rfresh.r[l], gem);
      }
      for (j = 0; j <= n; j++) {
	P_setunion(gem, WITH->r[j], rfresh.r[j]);
	if (!P_setequal(gem, WITH->r[j])) {
	  P_setcpy(WITH->r[j], gem);
	  done = false;
	}
      }
    }
  } while (!done);
}


void vhrecurs(void)
{
  prodtabx p;
  boolean boo = false;
  int ii, j, ll, i, l, start, FORLIM;
  long SET[3];
  bitset SET1, SET2;

  i = lastnont / (setmax + 1);
  if (i > BSarrSZ) {
    newline(2);
    consterror("vhrecurs            ", "BSarrSZ             ", BSarrSZ);
  }
  for (i = 0; i <= maxSYMBS; i++) {
    for (j = 0; j <= BSarrSZ; j++) {
      P_expset(ph1s[i].r[j], 0);
      P_expset(pt1s[i].r[j], 0);
    }
  }
  FORLIM = lastnont;
  for (i = firstnont; i <= FORLIM; i++) {
    p = help[i].rightp;
    while (p != rhsnil) {
      l = rhs[p].basisl;
      ll = rhs[p+1].symbol;
      ii = ll / (setmax + 1);
      P_addset(ph1s[i].r[ii], ll % (setmax + 1));
/* p2c: pg.p, line 2012:
 * Note: Using % for possibly-negative arguments [317] */
      ll = rhs[p + l].symbol;
      ii = ll / (setmax + 1);
      P_addset(pt1s[i].r[ii], ll % (setmax + 1));
/* p2c: pg.p, line 2015:
 * Note: Using % for possibly-negative arguments [317] */
      p = rhs[p + l].link;
    }
  }
  iterer(ph1s);
  iterer(pt1s);
  l = 1;
  start = firstnont + 1;
  FORLIM = lastnont;
  for (ii = start; ii <= FORLIM; ii++) {
    i = ii / (setmax + 1);
    if (*P_setint(SET2, P_setint(SET1, ph1s[ii].r[i], pt1s[ii].r[i]),
		  P_addset(P_expset(SET, 0), ii % (setmax + 1))) != 0) {
/* p2c: pg.p, line 2026:
 * Note: Using % for possibly-negative arguments [317] */
      boo = true;
      fprintf(list, "\n ");
      writenont(ii, &l);
    }
  }
  if (!boo)
    return;
  fprintf(list,
    "\n The above nonterminal(s) is(are) both left and right recursive.\n");
  fprintf(console, " A nonterminal is both left and right recursive\n");
  stop(3);
}


/*--------------------------------------------------------------------*/

void writesub(int rem, int ind)
{
  int z = 1;

  putc(' ', list);
  writenont(rem, &z);
  fprintf(list, "  has been substituted by  ");
  writenont(ind, &z);
  putc('\n', list);
}


boolean removechain(boolean t, boolean *modb)
{
  boolean Result = false;
  int i, k, l, m, n, s, remove, indsat;
  prodtabx j;

  k = lastnont;
  i = firstnont - 1;
  do {
    i++;
    j = help[i].rightp;
    if (j != rhsnil && rhs[j].basisl == 1) {
      incrtabx(&j);
      if (rhs[j].link == rhsnil && rhs[j].symbol > termg) {
	indsat = rhs[j].symbol;
	remove = i;
	help[i].rightp = rhsnil;
	Result = true;
	*modb = true;
	if (t)
	  writesub(remove, indsat);
	s = firstnont - 1;
	do {
	  s++;
	  j = help[s].rightp;
	  if (j != rhsnil) {
	    l = rhs[j].basisl;
	    m = 0;
	    do {
	      for (n = 1; n <= l; n++) {
		incrtabx(&j);
		if (rhs[j].symbol == remove)
		  rhs[j].symbol = indsat;
	      }
	      if (rhs[j].link == rhsnil)
		m = 1;
	      else {
		j = rhs[j].link;
		l = rhs[j].basisl;
	      }
	    } while (m != 1);
	  }
	} while (s != k);
      }
    }
  } while (i != k);
  return Result;
}


void writeip(int *x, int ju, int lae, prodtabx stp2)
{
  int l;
  int jj = 1;
  int ll;
  prodtabx stepp3 = stp2;

  if (*x == 1) {
    newline(2);
    fprintf(list, " Duplicate productions occur\n");
    fprintf(list, " and the grammar is modified.\n");
    fprintf(list, " The following productions exist more than once:\n");
  }
  putc(' ', list);
  writenont(ju, &jj);
  fprintf(list, " ::= ");
  for (l = 1; l <= lae; l++) {
    incrtabx(&stepp3);
    ll = rhs[stepp3].symbol;
    if (ll <= termg)
      writeterm(stringarray[ll+1], &jj);
    else
      writenont(ll, &jj);
    putc(' ', list);
  }
  putc('\n', list);
}


boolean identprod(boolean *t)
{
  int laend1, laend2, i, j;
  int kk = 0;
  boolean ens;
  boolean idboo = false;
  prodtabx startp1, startp2, stepp1, stepp2, lastok;
  int FORLIM;

  FORLIM = lastnont;
  for (j = firstnont; j <= FORLIM; j++) {
    startp1 = help[j].rightp;
    if (startp1 != rhsnil) {
      do {
	laend1 = rhs[startp1].basisl;
	startp2 = startp1;
	for (i = 1; i <= laend1; i++)
	  incrtabx(&startp2);
	lastok = startp2;
	startp2 = rhs[startp2].link;
	if (startp2 != rhsnil) {
	  do {
	    ens = false;
	    stepp1 = startp1;
	    stepp2 = startp2;
	    laend2 = rhs[stepp2].basisl;
	    if (laend1 == laend2) {
	      ens = true;
	      for (i = 1; i <= laend1; i++) {
		incrtabx(&stepp1);
		incrtabx(&stepp2);
		if (rhs[stepp1].symbol != rhs[stepp2].symbol)
		  ens = false;
	      }
	      if (ens) {
		kk++;
		if (*t)
		  writeip(&kk, j, laend2, startp2);
		idboo = true;
		rhs[lastok].link = rhs[stepp2].link;
	      }
	    } else {
	      for (i = 1; i <= laend2; i++)
		incrtabx(&stepp2);
	    }
	    if (!ens)
	      lastok = stepp2;
	    stepp2 = rhs[stepp2].link;
	    if (stepp2 != rhsnil)
	      startp2 = stepp2;
	  } while (stepp2 != rhsnil);
	  for (i = 1; i <= laend1; i++)
	    incrtabx(&startp1);
	  startp1 = rhs[startp1].link;
	  if (startp1 == rhsnil)
	    startp1 = startp2;
	} else
	  startp2 = startp1;
      } while (startp1 != startp2);
    }
  }
  if (*t)
    newline(2);
  return idboo;
}


void removeid(boolean dowrites)
{
  boolean dolist = dowrites, idch = true;
  boolean idb;
  boolean hasmods = false;

  while (idch) {
    idb = identprod(&dolist);
    idch = removechain(dowrites, &hasmods);
    idch = (idch || idb);
    dolist = false;
  }
  if (hasmods && dowrites) {
    fprintf(list, " The grammar has been modified for simple chains\n");
    fprintf(list, " using the above substitutions\n");
  }
}


/* exit label for parser errors */

typedef uchar chbufinx;

typedef uchar chbuftype[chbufmax + 1];
typedef char stackinx;

typedef struct treenode {
  int nodetype;
  stringaddress address;
  struct treenode *left, *right;
} treenode;

typedef struct attributes {
  chbufinx chbufp;
  int prodint;
  treenode *node;
  stringaddress address;
} attributes;

typedef uchar chstring[stringmax];
typedef char errmsginx;


typedef struct _REC_errmsg {
  int no, col, symb;
} _REC_errmsg;


/* prodno = 0..prodmax; */
/* rslength = -1..symbmax; */
/* lrinx = 0..lrmax; */
typedef char reduinx;

typedef struct reduelem {
  unsigned oldtop : 6, newtop : 6, prod : 9;
} reduelem;

typedef struct stackelm {
  unsigned link : 6, table : 11;
} stackelm;


/* Local variables for inputgrammar: */
struct LOC_inputgrammar {
  int symbcount;
  attributes attstack[stackmax + 1];
  chbuftype chbuf;
  chbufinx chbufi;   /* character buffer index */
  int integervalue;   /* the numerical value of integers read in */
  int errcount;   /* becomes nonzero when errors found */
  int maxerrcount;   /* max no or errors before giving up*/
  uchar line[recmax];   /*contains current line*/
  int linelength;   /*length of current input line */
  _REC_errmsg errmsg[errmax + 1];
  errmsginx errmp;   /* index of error messages in errmsg */
  boolean stoperror, proderror;
  stringaddress saddr, niladdress, emptyaddress;
  treenode *treeroot;
  uchar suffix;
  int numberoftermsymbs;
} ;

Local void produce(stackinx oldtop, stackinx newtop, int prod,
		   struct LOC_inputgrammar *LINK);

/*--------------------------------------------------------------------*/

Local void parserstop(int n, struct LOC_inputgrammar *LINK)
{
  newline(2);
  switch (n) {

  case 0:
    /* blank case */
    break;

  case 1:
    m(" *** Installation-dependent const \"stackmax\" exceeded ");
    break;

  case 2:
    m(" *** End of file encountered on input                 ");
    break;

  case 3:
    m(" *** Syntax error recovery abandoned                  ");
    break;

  case 4:
    m(" *** Installation-dependent const \"redumax\" exceeded  ");
    break;

  case 5:
    m(" *** Overflow of character buffer: \"chbufmax\" exceeded");
    break;

  case 6:
    m(" *** Maximum error count exceeded                     ");
    break;
  }
  switch (n) {

  case 0:
    /* blank case */
    break;

  case 1:
  case 4:
  case 5:
    m(" *** Translation stopped by translator error          ");
    break;

  case 2:
  case 3:
  case 6:
    m(" *** Translation stopped by source error(s)           ");
    break;
  }
  /*H goto 10 H*/
  /*F halt(1) F*/
  /*exit*/
}

/* Local variables for writeerror: */
struct LOC_writeerror {
  struct LOC_inputgrammar *LINK;
  int inx, emi;
} ;

Local void e(Char *s, struct LOC_writeerror *LINK)
{
  fprintf(list, "      *** ERROR%4d (%d): %.*s\n",
	  LINK->emi, LINK->inx, MSTRINGlen, s);
  fprintf(console, " *** ERROR%4d (%d): %.*s\n",
	  LINK->emi, LINK->inx, MSTRINGlen, s);
}

Local void w(Char *s, struct LOC_writeerror *LINK)
{
  fprintf(list, "      *** WARNING%4d (%d): %.*s\n",
	  LINK->emi, LINK->inx, MSTRINGlen, s);
  fprintf(console, " *** WARNING%4d (%d): %.*s\n",
	  LINK->emi, LINK->inx, MSTRINGlen, s);
}

/*--------------------------------------------------------------------*/

Local void writeerror(struct LOC_inputgrammar *LINK)
{
  struct LOC_writeerror V;
  int symb, FORLIM, FORLIM1;

  V.LINK = LINK;
  FORLIM = LINK->linelength;
  for (V.emi = 1; V.emi <= FORLIM; V.emi++)
    putc(LINK->line[V.emi-1], console);
  putc('\n', console);
  fprintf(list, "            ");
  V.emi = 1;
  FORLIM = LINK->errmp;
  for (V.inx = 1; V.inx <= FORLIM; V.inx++) {
    for (FORLIM1 = LINK->errmsg[V.inx].col; V.emi < FORLIM1; V.emi++) {
      putc(' ', list);
      putc(' ', console);
    }
    fprintf(list, "%d", V.inx);
    fprintf(console, "%d", V.inx);
    V.emi += 2;
  }
  putc('\n', list);
  putc('\n', console);
  FORLIM = LINK->errmp;
  for (V.inx = 1; V.inx <= FORLIM; V.inx++) {
    V.emi = LINK->errmsg[V.inx].no;
    if (V.emi < 800) {
      symb = LINK->errmsg[V.inx].symb;
      if (symb == 2)
	e("A name was found but the following were expected:     ", &V);
      else if (symb == 4)
	e("A string was found but the following were expected:   ", &V);
      else if (symb == 3)
	e("An integer was found but the following were expected: ", &V);
      else if (symb == 5)
	e("An error was found but the following were expected:   ", &V);
      else if (symb > 6 && symb < 11)
	e("Reserved word found.  The following were expected:    ", &V);
      else
	e("Punctuation characters found; expected symbols were:  ", &V);
    }

    switch (V.emi) {

    case 1:
      m("<ERROR>    TERMINALS  OPTIONS    EMPTYTERM            ");
      break;

    case 2:
      m(":          =                                          ");
      break;

    case 3:
      m("<STRING>                                              ");
      break;

    case 4:
      m("<INTEGER>  <ERROR>                                    ");
      break;

    case 5:
      m(".                                                     ");
      break;

    case 6:
      m("TERMINALS                                             ");
      break;

    case 7:
      m("<NAME>                                                ");
      break;

    case 8:
      m("TERMINALS  EMPTYTERM  <ERROR>                         ");
      break;

    case 9:
      m(".          <NAME>                                     ");
      break;

    case 10:
      m("=                                                     ");
      break;

    case 11:
      m("<ERROR>    [          <NAME>     <STRING>   (         ");
      m("{                                                     ");
      break;

    case 12:
      m("$          <ERROR>    [          <NAME>     <STRING>  ");
      m("(          {                                          ");
      break;

    case 13:
      m("{                                                     ");
      break;

    case 14:
      m("]          |                                          ");
      break;

    case 15:
      m(")          |                                          ");
      break;

    case 16:
      m("|          }L         }R                              ");
      break;

    case 17:
      m("$          |          }L         }R                   ");
      break;

    case 18:
      m("}L         }R         <ERROR>    [          <NAME>    ");
      m("<STRING>   (          {                               ");
      break;

    case 19:
      m(".          |                                          ");
      break;

    case 20:
      m(".          <NAME>     <ERROR>                         ");
      break;

    case 21:
      m("<EOF>                                                 ");
      break;

    /* lexical error messages */
    case 800:
      e("Character not recognized: ignored                     ", &V);
      break;

    case 801:
      e("Null string not allowed                               ", &V);
      break;

    /* lexical warning messages */
    case 901:
      w("A pending comment was not terminated                  ", &V);
      break;

    case 902:
      w("String end character generated at end of line         ", &V);
      break;

    case 903:
      w("An identifier must not begin or end with an underscore", &V);
      break;

    case 904:
      w("Input line may exceed maximum alloable length         ", &V);
      break;

    case 905:
      w("Incorrect octal character representation              ", &V);
      break;
    }

    putc('\n', console);
    consoleflush();
  }
  if (LINK->errcount > LINK->maxerrcount)
    parserstop(6, LINK);
}

/* Local variables for produce: */
struct LOC_produce {
  struct LOC_inputgrammar *LINK;
  stackinx oldtop, newtop;
  int prod, i, wrj;
  treenode *np;
  boolean carry, primaryfound;
} ;

Local void listterminals(struct LOC_produce *LINK)
{
  /* list the internal values of terminals */
  int i, j, middle;

  newline(2);
  fprintf(list, " Terminal Symbols:\n");
  fprintf(list, " Value       Terminal        Value       Terminal");
  newline(2);
  middle = LINK->LINK->numberoftermsymbs >> 1;
  for (i = 0; i <= middle; i++) {
    fprintf(list, " %5d       ", i);
    listterm(stringarray[i+1], &j);
    listtab(j, 10);
    if (middle + i < LINK->LINK->numberoftermsymbs) {
      fprintf(list, "      %5d       ", middle + i + 1);
      listterm(stringarray[middle + i + 2], &j);
    }
    putc('\n', list);
  }
  newline(2);
}

Local chbufinx startofsymbol(int index, struct LOC_produce *LINK)
{
  return (LINK->LINK->attstack[LINK->newtop + index - 2].chbufp);
}

Local chbufinx endofsymbol(int index, struct LOC_produce *LINK)
{
  return (LINK->LINK->attstack[LINK->newtop + index - 1].chbufp - 1);
}

Local void getstring(int sy, uchar *st, int *length, struct LOC_produce *LINK)
{
  int i, t, FORLIM;

  if (sy >= 0)
    t = LINK->newtop + sy - 1;
  else
    t = LINK->oldtop + sy - 1;
  *length = 0;
  FORLIM = LINK->LINK->attstack[t].chbufp;
  for (i = LINK->LINK->attstack[t-1].chbufp; i < FORLIM; i++) {
    (*length)++;
    st[*length - 1] = LINK->LINK->chbuf[i];
  }
}

Local void snapprocess(struct LOC_produce *LINK)
{
  chstring s;
  int i, j, l;
  Char ch;
  int FORLIM;

  if (LINK->prod != 0) {
    fprintf(logfile, "   Prod:%3d oldtop:%3d newtop:%d\n",
	    LINK->prod, LINK->oldtop, LINK->newtop);
    FORLIM = LINK->oldtop - LINK->newtop + 1;
    for (i = 1; i <= FORLIM; i++) {
      getstring(i, s, &l, LINK);
      if (l > 0) {
	fprintf(logfile, "  symb%d ", i);
	for (j = 0; j < l; j++)
	  putc(s[j], logfile);
	fprintf(logfile, "\"\n");
      }
    }
    return;
  }
  putc('\n', logfile);
  putc('\n', list);
  if (logfile != NULL)
    logfile = freopen("logfile", "r", logfile);
  else
    logfile = fopen("logfile", "r");
  if (logfile == NULL)
    _EscIO2(FileNotFound, "logfile");
  fprintf(list, " Snapshots:\n");
  while (!P_eof(logfile)) {
    while (!P_eoln(logfile)) {
      ch = getc(logfile);
      if (ch == '\n')
	ch = ' ';
      putc(ch, list);
    }
    fscanf(logfile, "%*[^\n]");
    getc(logfile);
    putc('\n', list);
  }
}

Local void inputerror(int nr, struct LOC_produce *LINK)
{
  /* error-procedure local for terminput*/
  int j, k, kch, FORLIM;

  if (nr & 1)
    fprintf(list, "\n ");
  /*D if debug then begin
     writeln(logfile);
     writeln(logfile,'*** Inputerror ',nr)
     end; D*/
  switch (nr) {

  case 1:
    fprintf(list, "   *** Expression too complicated ");
    break;

  case 2:
    fprintf(console, "   ***  Terminal \"");
    k = stringarray[lasttermval+1].loc;
    FORLIM = symbtab[k-1];
    for (j = 0; j < FORLIM; j++) {
      kch = symbtab[k + j];
      if (kch >= ' ' && kch < 128)
	putc(symbtab[k + j], console);
      else {
	putc(bslchar, console);
	fprintf(console, "%d%d%d", kch >> 6, (kch & 63) >> 3, kch & 7);
      }
    }
    fprintf(console, "\" duplicated.\n");
    fprintf(list, "\n   ***  Terminal \"");
    FORLIM = symbtab[k-1];
    for (j = 0; j < FORLIM; j++) {
      kch = symbtab[k + j];
      if (kch >= ' ' && kch < 128)
	putc(symbtab[k + j], list);
      else {
	putc(bslchar, list);
	kch = symbtab[k + j];
	fprintf(list, "%d%d%d", kch >> 6, (kch & 63) >> 3, kch & 7);
      }
    }
    fprintf(list, "\" duplicated.\n");
    break;

  case 3:
    consterror("terminput           ", "TtabSZ              ", TtabSZ);
    break;

  case 4:
    LINK->LINK->stoperror = true;
    putc('\n', list);
    fprintf(console, "   ***  Illegal concatenation.\n");
    fprintf(list, "   ***  Illegal concatenation.");
    break;

  case 6:
    LINK->LINK->stoperror = true;
    putc('\n', list);
    fprintf(console, "   ***  Terminal \"");
    k = stringarray[lasttermval+1].loc;
    FORLIM = symbtab[k-1];
    for (j = 0; j < FORLIM; j++)
      putc(symbtab[k + j], console);
    fprintf(console, "\" more than %2ld characters.\n", (long)ordmaxch);
    fprintf(list, "   ***  Terminal \"");
    k = stringarray[lasttermval+1].loc;
    FORLIM = symbtab[k-1];
    for (j = 0; j < FORLIM; j++)
      putc(symbtab[k + j], list);
    fprintf(list, "\" more than %2ld characters.", (long)ordmaxch);
    putc(' ', list);
    break;

  case 7:
    consterror("terminput           ", "maxsymbs            ", maxSYMBS);
    break;
  }
  if (nr >= 0 && nr < 32 && ((1L << nr) & 0x56) != 0) {
    fprintf(console, " Error(s) in input\n");
    consoleflush();
  }
  fprintf(list, "\n          ");
  if (nr & 1)
    stop(0);
}

Local void writenonterms(nontelm *p, struct LOC_produce *LINK)
{
  int i;
  nontelm *WITH;

  while (p != NULL) {
    WITH = p;
    writenonterms(WITH->llink, LINK);
    fprintf(list, " %5d ", WITH->nontermvalue);
    listterm(WITH->address, &i);
    if (LINK->wrj & 1)
      putc('\n', list);
    else
      listtab(i, 30);
    LINK->wrj++;
    p = WITH->rlink;
  }
}

Local void putsym(int start, int fin, stringaddress *add,
		  struct LOC_produce *LINK)
{
  int i, len;

  len = fin - start + 1;
  if (freesp + len + 1 > SEGdim) {
    symbtab = Malloc(sizeof(charset));
    freesp = 1;
  }
  symbtab[freesp-1] = len;
  for (i = 0; i < len; i++)
    symbtab[freesp + i] = LINK->LINK->chbuf[start + i];
  add->seg = symbtab;
  add->loc = freesp;
  /*D if debug then begin
     writeln(logfile); write(logfile,'freesp=',freesp:3,' symbol "');
     for i := 1 to len do write(logfile,symbtab^[freesp+i]);
     writeln(logfile,'", len=',len:3)
     end; D*/
  freesp += len + 1;
}

Local int strcompare(stringaddress str1, stringaddress str2,
		     struct LOC_produce *LINK)
{
  int m;
  int i = 0;
  int j;

  m = str2.seg[str2.loc - 1];
  j = m + 1;
  while (i <= m) {
    if (str1.seg[str1.loc + i - 1] == str2.seg[str2.loc + i - 1])
      i++;
    else if (str1.seg[str1.loc + i - 1] < str2.seg[str2.loc + i - 1])
      m = i - 1;
    else {
      m = i - 1;
      j = m;
    }
  }
  if (i == j)
    return 0;
  else if (i < j)
    return -1;
  else
    return 1;
}

Local void indord(stringaddress addr, struct LOC_produce *LINK)
{
  int scompval;
  boolean notfound = true;
  nontelm *WITH;

  dynp = first;
  while (dynp != NULL && notfound) {
    WITH = dynp;
    scompval = strcompare(addr, WITH->address, LINK);
    if (scompval != 0) {
      last = dynp;
      if (scompval < 0)
	dynp = WITH->llink;
      else
	dynp = WITH->rlink;
      continue;
    }
    notfound = false;
    ntcount = WITH->nontermvalue;
    if (!leftflag)
      continue;
    LINK->i = 1;
    while (LINK->i < LINK->LINK->symbcount && symbols[LINK->i-1] != ntcount)
      LINK->i++;
    if (LINK->i < LINK->LINK->symbcount)
      rhs[lefthelp[ntcount]].link = point;
  }
  if (!notfound)
    return;
  statp = Malloc(sizeof(nontelm));
  if (statp == NULL)
    gramerror(14);
  WITH = statp;
  WITH->address = addr;
  WITH->llink = NULL;
  WITH->rlink = NULL;
  WITH->nontermvalue = internv;
  ntcount = internv;
  if (internv >= maxSYMBS)
    gramerror(7);
  internv++;
  nonterms[ntcount].nontermp = statp;
  WITH = last;
  if (strcompare(WITH->address, addr, LINK) > 0)
    WITH->llink = statp;
  else
    WITH->rlink = statp;
}

Local void nonterminal(int stringptr, int endmarker, stringaddress addr,
		       struct LOC_produce *LINK)
{
  nontelm *WITH;

  if (firstnonterm) {
    if (addr.seg == NULL)
      putsym(stringptr, endmarker, &addr, LINK);
    first = Malloc(sizeof(nontelm));
    WITH = first;
    WITH->address = addr;
    WITH->llink = NULL;
    WITH->rlink = NULL;
    ntcount = internv;
    nonterms[ntcount].nontermp = first;
    firstnonterm = false;
    WITH->nontermvalue = internv;
    internv++;
    return;
  }
  if (addr.seg == NULL)
    putsym(stringptr, endmarker, &addr, LINK);
  if (errorsininput == false)
    indord(addr, LINK);
}

Local void initialprod(struct LOC_produce *LINK)
{
  int i;
  entrytyp *WITH;

  rhsnil = 0;
  for (i = 0; i <= maxSYMBS; i++) {
    help[i].rightp = rhsnil;
    nonterms[i].isonright = false;
    nonterms[i].isonleft = false;
  }
  emptyinexpr = false;
  errorsininput = false;
  goalknown = false;
  LINK->LINK->proderror = false;
  lextabcount = 0;
  for (i = ordminch; i <= ordmaxch; i++) {
    WITH = &entry_[i];
    WITH->higherp = NULL;
    WITH->defined_ = false;
    WITH->internv = -1;
  }
  emptyvalue = 0;
  lasttermval = 1;
  LINK->LINK->stoperror = false;
  putc('\n', list);
  bar("Input File                              ");
  memcpy(LINK->LINK->chbuf,
    "METAGOAL                                                                                                                                                                                                                                                        ",
    sizeof(chbuftype));
  putsym(0, 7, &stringarray[1], LINK);
  memcpy(LINK->LINK->chbuf,
    "<EOF>                                                                                                                                                                                                                                                           ",
    sizeof(chbuftype));
  putsym(0, 4, &stringarray[2], LINK);
}

Local void newnode(int leftattr, int rightattr, int localprod,
		   struct LOC_produce *LINK)
{
  treenode *np;

  np = Malloc(sizeof(treenode));
  if (leftattr != 0)
    np->left = LINK->LINK->attstack[LINK->newtop + leftattr - 1].node;
  else
    np->left = NULL;
  if (rightattr != 0)
    np->right = LINK->LINK->attstack[LINK->newtop + rightattr - 1].node;
  else
    np->right = NULL;
  np->address.seg = NULL;
  np->nodetype = localprod;
  LINK->LINK->attstack[LINK->newtop].node = np;
}

Local void getnewname(treenode *node, struct LOC_produce *LINK)
{
  uchar *inseg;
  stringindex inloc;
  int length, i;
  attributes *WITH;

  inseg = node->address.seg;
  inloc = node->address.loc;
  length = inseg[inloc-1];
  if (freesp + length + 2 > SEGdim) {
    symbtab = Malloc(sizeof(charset));
    freesp = 1;
  }
  symbtab[freesp-1] = length + 1;
  for (i = 0; i < length; i++)
    symbtab[freesp + i] = inseg[inloc + i];
  symbtab[freesp + length] = LINK->LINK->suffix;
  do {
    LINK->LINK->suffix++;
    if (LINK->LINK->suffix > 'Z')
      gramerror(1);
  } while (isalnum(LINK->LINK->suffix) == 0 && LINK->LINK->suffix != '_');
  WITH = &LINK->LINK->attstack[LINK->newtop];
  WITH->address.seg = symbtab;
  WITH->address.loc = freesp;
  WITH->node->address = WITH->address;
  freesp += length + 2;
}

Local void copytree(treenode *intree, treenode **outtree,
		    struct LOC_produce *LINK)
{
  treenode *WITH;

  if (intree == NULL) {
    *outtree = NULL;
    return;
  }
  *outtree = Malloc(sizeof(treenode));
  WITH = *outtree;
  WITH->nodetype = intree->nodetype;
  WITH->address = intree->address;
  copytree(intree->left, &WITH->left, LINK);
  copytree(intree->right, &WITH->right, LINK);
}

Local void termerror(stringaddress addr, int n, struct LOC_produce *LINK)
{
  int i;

  errorsininput = true;
  termrep = 0;
  fprintf(list, "\n   *** Illegal terminal: \"");
  fprintf(console, " Illegal terminal: \"");
  listterm(addr, &i);
  consoleterm(addr, &i);
  putc('"', list);
  putc('"', console);
  switch (n) {

  case 0:
    /* blank case */
    break;

  case 1:
    fprintf(list, " longer than %2ld characters", (long)ordmaxch);
    fprintf(console, " longer than %2ld characters", (long)ordmaxch);
    break;

  case 2:
    fprintf(list, " not defined");
    fprintf(console, " not defined");
    break;
  }
  putc('\n', list);
  putc('\n', console);
  consoleflush();
}

Local void terminal(stringaddress addr, struct LOC_produce *LINK)
{
  termelm *currentp = NULL, *runp = NULL;
  int length;
  int i = 2;
  entrytyp *WITH;
  termelm *WITH1;

  termrep = -1;
  if (addr.seg == NULL) {
    fprintf(list, "nil address given to terminal");
    fprintf(console, "nil address given to terminal");
  } else
    length = addr.seg[addr.loc - 1];
  while (i <= length && termrep != 0) {
    if (i == 2) {
      WITH = &entry_[addr.seg[addr.loc]];
      if (WITH->higherp == NULL)
	termerror(addr, 2, LINK);
      else
	runp = WITH->higherp;
    } else {
      if (currentp->higherp == NULL)
	termerror(addr, 2, LINK);
      else
	runp = currentp->higherp;
    }
    if (runp != NULL) {
      while (termrep != 0 && runp->tchr != addr.seg[addr.loc + i - 1]) {
	WITH1 = runp;
	if (WITH1->niveaup == NULL)
	  termerror(addr, 2, LINK);
	else
	  runp = WITH1->niveaup;
      }
    }
    currentp = runp;
    i++;
  }
  if (termrep == 0)
    return;
  if (length != 1) {
    if (currentp->defined_)
      termrep = currentp->internv;
    else
      termerror(addr, 0, LINK);
    return;
  }
  WITH = &entry_[addr.seg[addr.loc]];
  if (WITH->defined_)
    termrep = WITH->internv;
  else
    termerror(addr, 0, LINK);
}

Local void workon(treenode *node, struct LOC_produce *LINK)
{
  LINK->primaryfound = true;
  if (node->nodetype == primary1) {
    nonterminal(0, 0, node->address, LINK);
    cnt++;
    if (cnt > maxRHSterms) {
      gramerror(19);
      return;
    }
    nonterms[ntcount].isonright = true;
    rhs[point].parserp = NULL;
    rhs[point].link = rhsnil;
    rhs[point].symbol = ntcount;
    symbp = point;
    newelm(&point, "inputgrammar        ");
    isfirst = false;
    return;
  }
  terminal(node->address, LINK);
  if (emptyvalue == termrep)
    emptyinexpr = true;
  rhs[point].parserp = NULL;
  rhs[point].link = rhsnil;
  cnt++;
  if (cnt > maxRHSterms) {
    gramerror(19);
    return;
  }
  rhs[point].symbol = termrep;
  symbp = point;
  newelm(&point, "inputgrammar        ");
  isfirst = false;
}

Local void scan(treenode *node, struct LOC_produce *LINK)
{
  if (node == NULL)
    return;
  switch (node->nodetype) {

  case term2:
    scan(node->left, LINK);
    scan(node->right, LINK);
    break;

  case expression3:
    scan(node->left, LINK);
    if (LINK->carry) {
      node->nodetype = -expression3;
      LINK->carry = false;
    }
    break;

  case -expression3:
    scan(node->right, LINK);
    if (LINK->carry)
      node->nodetype = expression3;
    break;

  case primary1:
  case primary2:
    workon(node, LINK);
    break;
  }
}

Local void outputleftside(treenode *node, struct LOC_produce *LINK)
{
  int i;
  _REC_rhs *WITH;

  leftflag = true;
  nonterminal(0, 0, node->address, LINK);
  symbols[LINK->LINK->symbcount-1] = ntcount;
  LINK->LINK->symbcount++;
  if (goalnotentered) {
    WITH = &rhs[point];
    WITH->basisl = 3;
    symbp = point;
    newelm(&point, "Goalprod            ");
    rhs[point].symbol = 0;
    newelm(&point, "Goalprod            ");
    WITH = &rhs[point];
    if (goalknown)
      WITH->symbol = goalsymbol;
    else
      WITH->symbol = ntcount;
    newelm(&point, "Goalprod            ");
    rhs[point].symbol = 1;
    for (i = 1; i <= 4; i++) {
      WITH = &rhs[symbp];
      incrtabx(&symbp);
      WITH->link = rhsnil;
      WITH->parserp = NULL;
    }
    goalnotentered = false;
    point = symbp;
  }
  nonterms[ntcount].isonleft = true;
  if (help[ntcount].rightp == rhsnil)
    help[ntcount].rightp = point;
  rhs[point].parserp = NULL;
  rhs[point].link = rhsnil;
  helpp = point;
  symbp = point;
  newelm(&point, "inputgrammar        ");
  leftflag = false;
}

Local void outputoption(struct LOC_produce *LINK)
{
  if (isfirst) {
    gramerror(9);
    return;
  }
  if (emptyinexpr) {
    if (cnt > 1) {
      gramerror(4);
      emptyinexpr = false;
    } else if (cnt == 1)
      emptyinexpr = false;
  }
  rhs[helpp].basisl = cnt;
  cnt = 0;
  helpp = point;
  rhs[symbp].link = point;
  rhs[point].link = rhsnil;
  rhs[point].parserp = NULL;
  newelm(&point, "inputgrammar        ");
}

Local void purgetree(treenode **node, struct LOC_produce *LINK)
{
  /* This leaves a potential memory leak of the stringaddress */
  if (*node == NULL)
    return;
  purgetree(&(*node)->right, LINK);
  purgetree(&(*node)->left, LINK);
  Free(*node);
}

Local void checkbothsides(struct LOC_produce *LINK)
{
  int i, k, m;
  _REC_nonterms *WITH;
  nontelm *WITH1;

  if (goalnotdefined)
    goalsymbol = symbols[0];
  k = internv - 1;
  for (i = lasttermval; i <= k; i++) {
    WITH = &nonterms[i];
    if (!(WITH->isonleft && WITH->isonright)) {
      WITH1 = WITH->nontermp;
      if (WITH1->nontermvalue != goalsymbol) {
	fprintf(list, "\n ");
	listterm(WITH1->address, &m);
	consoleterm(WITH1->address, &m);
	if (WITH->isonleft) {
	  fprintf(list, " appears only on the left side");
	  fprintf(console, " appears only on left side\n");
	} else {
	  fprintf(list, " appears only on the right side");
	  fprintf(console, " appears only on right side\n");
	}
	consoleflush();
	putc('\n', list);
	stopflag = true;
      }
    }
  }
  lastnont = k;
  if (stopflag)
    stop(3);
}

Local void printouttree(treenode *node, int indent, struct LOC_produce *LINK)
{
  int maxlen, i;

  if (node == NULL)
    return;
  fprintf(list, "%3d", node->nodetype);
  if (node->address.seg == NULL) {
    if (node->nodetype == expression3)
      fprintf(list, "16 ");
    else if (node->nodetype == term2)
      fprintf(list, "25 ");
    else
      fprintf(list, "    ");
  } else {
    putc(' ', list);
    maxlen = node->address.seg[node->address.loc - 1];
    if (maxlen > 3)
      maxlen = 3;
    for (i = 0; i < maxlen; i++)
      putc(node->address.seg[node->address.loc + i], list);
    listtab(maxlen, 3);
  }
  if (node->right != NULL)
    printouttree(node->right, indent + 7, LINK);
  if (node->left == NULL)
    return;
  putc('\n', list);
  for (i = 1; i <= indent; i++)
    putc(' ', list);
  printouttree(node->left, indent, LINK);
}

Local termelm *mknode(uchar ch, int *termcount, struct LOC_produce *LINK)
{
  termelm *t;

  t = Malloc(sizeof(termelm));
  t->defined_ = false;
  t->tchr = ch;
  t->internv = -1;
  t->niveaup = NULL;
  t->higherp = NULL;
  (*termcount)++;
  return t;
}

Local void readterminal(chbufinx stringptr, chbufinx endmarker,
			struct LOC_produce *LINK)
{
  termelm *currentp, *runp;
  int m, i;
  boolean run, rum;
  entrytyp *WITH;
  termelm *WITH1;

  /*                    It is a good question why one should complicate things by
                        building a tree of terminals here instead of just listing
                        them separately and checking for equality by other means*/
  if (LINK->prod != termdeflist4) {
    if (lasttermval < maxSYMBS)
      lasttermval++;
    else
      inputerror(7, LINK);
  }
  m = endmarker - stringptr + 1;
  if (m > ordmaxch)
    m = ordmaxch;
  putsym(stringptr, stringptr + m - 1, &stringarray[lasttermval+1], LINK);
  if (endmarker - stringptr + 1 > ordmaxch)
    inputerror(6, LINK);

  for (i = 2; i <= m; i++) {
    run = true;
    if (i == 2) {
      WITH = &entry_[LINK->LINK->chbuf[stringptr]];
      if (WITH->higherp == NULL) {
	WITH->higherp = mknode(LINK->LINK->chbuf[stringptr+1], &lextabcount,
			       LINK);
	currentp = WITH->higherp;
	run = false;
      } else
	runp = WITH->higherp;
    } else {
      WITH1 = currentp;
      if (WITH1->higherp == NULL) {
	WITH1->higherp = mknode(LINK->LINK->chbuf[stringptr + i - 1],
				&lextabcount, LINK);
	currentp = WITH1->higherp;
	run = false;
      } else
	runp = WITH1->higherp;
    }
    rum = false;
    while (run != rum) {
      if (runp->tchr == LINK->LINK->chbuf[stringptr + i - 1]) {
	rum = run;
	continue;
      }
      WITH1 = runp;
      if (WITH1->niveaup == NULL) {
	WITH1->niveaup = mknode(LINK->LINK->chbuf[stringptr + i - 1],
				&lextabcount, LINK);
	currentp = WITH1->niveaup;
	run = false;
      } else
	runp = WITH1->niveaup;
    }
    if (run)
      currentp = runp;
  }
  if (m == 1) {
    WITH = &entry_[LINK->LINK->chbuf[stringptr]];
    /* if debug then begin
       write(list,'stringptr=',stringptr:1,
                  ' entry[ord(',chbuf[stringptr]:1,')]');
       writeln(list, ' lasttermval=',lasttermval,' defined=',defined)
       end; */
    WITH->internv = lasttermval;
    if (WITH->defined_)
      inputerror(2, LINK);
    else
      WITH->defined_ = true;
    return;
  }
  currentp->internv = lasttermval;
  if (currentp->defined_)
    inputerror(2, LINK);
  else
    currentp->defined_ = true;
}

Local boolean canbenil(treenode *node, struct LOC_produce *LINK)
{  /* find out if tree contains a nil right hand side */
  boolean Result;

  if (node == NULL)
    return true;
  switch (node->nodetype) {

  case term2:
    Result = canbenil(node->left, LINK) & canbenil(node->right, LINK);
    break;

  case expression3:
    Result = canbenil(node->left, LINK) | canbenil(node->right, LINK);
    break;

  case primary1:
  case primary2:
    Result = false;
    break;
  }
  return Result;
}

Local void makenonnil(treenode **node, struct LOC_produce *LINK)
{  /* replace first nil right hand side with 8 */
  if (*node == NULL) {
    if (emptyvalue == 0) {
      produce(0, 0, 0, LINK->LINK);
      gramerror(2);
      return;
    }
    newnode(0, 0, primary2, LINK);
    *node = LINK->LINK->attstack[LINK->newtop].node;
    (*node)->address = LINK->LINK->emptyaddress;
    return;
  }
  switch ((*node)->nodetype) {

  case term2:
    makenonnil(&(*node)->left, LINK);
    break;

  case expression3:
    if (canbenil((*node)->left, LINK))
      makenonnil(&(*node)->left, LINK);
    else
      makenonnil(&(*node)->right, LINK);
    break;
  }
}

Local void changeref(treenode *node, stringaddress oldaddr,
		     stringaddress newaddr, struct LOC_produce *LINK)
{
  stringaddress *WITH;

  if (node == NULL)
    return;
  changeref(node->left, oldaddr, newaddr, LINK);
  WITH = &node->address;
  if (WITH->seg == oldaddr.seg && WITH->loc == oldaddr.loc)
    node->address = newaddr;
  changeref(node->right, oldaddr, newaddr, LINK);
}

Local void testrepeated(struct LOC_produce *LINK)
{
  /* get rid of <NAME> = <NAME1>; <NAME1> = <repeated> */
  boolean found = false;
  treenode *tp;

  if (LINK->LINK->treeroot->left == NULL)   /* break */
    return;
  if (LINK->LINK->treeroot->left->nodetype != primary1)   /* break */
    return;
  LINK->np = LINK->LINK->treeroot->right;
  while (!found && LINK->np != NULL) {
    if (LINK->LINK->treeroot->left->address.seg != LINK->np->address.seg ||
	LINK->LINK->treeroot->left->address.loc != LINK->np->address.loc) {
      LINK->np = LINK->np->right;
      continue;
    }
    found = true;
    changeref(LINK->LINK->treeroot->right,
	      LINK->LINK->treeroot->left->address,
	      LINK->LINK->treeroot->address, LINK);
    tp = LINK->LINK->treeroot->right;
    LINK->LINK->treeroot->right = NULL;
    purgetree(&LINK->LINK->treeroot, LINK);
    LINK->LINK->treeroot = tp;
  }
  if (found)
    testrepeated(LINK);
}

/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/


Local void produce(stackinx oldtop_, stackinx newtop_, int prod_,
		   struct LOC_inputgrammar *LINK)
{
  struct LOC_produce V;
  treenode *saveroot, *WITH;
  attributes *WITH1;

  /*D if debug then snapprocess; D*/

  V.LINK = LINK;
  V.oldtop = oldtop_;
  V.newtop = newtop_;
  V.prod = prod_;
  switch (V.prod) {

  case -1:
    initialprod(&V);
    break;

  /* 0 METAGOAL = ebnfinput "<EOF>"       */
  case METAGOAL1:   /* last production call */
    break;

  /* 1 ebnfinput = heading "."       */
  /* 2           | heading productions "."       */
  /* 3           | "<ERROR>"       */
  case ebnfinput1:
  case ebnfinput2:
    newline(3);
    bar("End of input                            ");
    listterminals(&V);
    if (errorsininput) {
      fprintf(console, " Error(s) in input\n");
      consoleflush();
      stop(0);
    } else {
      newline(2);
      fprintf(list, " Non-terminal symbols:\n");
      fprintf(list, " Value,non-terminal");
      fprintf(list, "                  ");
      fprintf(list, " Value,non-terminal");
      newline(2);
      V.wrj = 0;
      writenonterms(first, &V);
      checkbothsides(&V);
    }
    break;

  /* 4 heading = terminaldefs "."       */
  /* 5         | optiondefs "." terminaldefs "."       */
  /* 6         | emptydef "." terminaldefs "."       */
  /* 7         | optiondefs "." emptydef "." terminaldefs "."       */
  /* 8         | terminaldefs "." goalsymboldef "."       */
  /* 9         | optiondefs "." terminaldefs "." goalsymboldef "."       */
  /*10         | emptydef "." terminaldefs "." goalsymboldef "."       */
  /*11         | optiondefs "." emptydef "." terminaldefs "." goalsymboldef "." */
  case heading1:
  case heading2:
  case heading3:
  case heading4:
  case heading5:
  case heading6:
  case heading7:
  case heading8:
    break;

  /*12 productions = leftside "=" expression "."       */
  case productions1:
    if (LINK->treeroot == NULL)
      purgetree(&LINK->attstack[V.newtop+2].node, &V);
    else {
      LINK->treeroot->left = LINK->attstack[V.newtop+2].node;
      if (!LINK->proderror) {
	/*D if debug then begin
	   writeln( list, 'before reduction:' );
	   printouttree(treeroot,0);
	   writeln(list) end; D*/
	testrepeated(&V);
	V.np = LINK->treeroot;
	while (V.np != NULL) {
	  if (canbenil(V.np->left, &V))
	    makenonnil(&V.np->left, &V);
	  V.np = V.np->right;
	}
	/*D if debug then begin
	   writeln( list, 'after reduction:' );
	   printouttree(treeroot,0);
	   writeln(list) end; D*/
	V.np = LINK->treeroot;
	while (V.np != NULL) {
	  isfirst = true;
	  cnt = 0;
	  outputleftside(V.np, &V);
	  do {
	    V.carry = true;
	    V.primaryfound = false;
	    scan(V.np->left, &V);
	    if (V.primaryfound && !V.carry)
	      outputoption(&V);
	  } while (!V.carry);
	  rhs[helpp].basisl = cnt;
	  lefthelp[symbols[LINK->symbcount-2]] = symbp;
	  if (emptyinexpr && cnt > 1)
	    gramerror(4);
	  emptyinexpr = false;
	  V.np = V.np->right;
	}
      }
      purgetree(&LINK->treeroot, &V);
    }
    LINK->proderror = false;
    break;

  /*13 terminaldefs = "TERMINALS" colon termdeflist       */
  case terminaldefs1:
    goalnotentered = true;
    LINK->numberoftermsymbs = lasttermval;
    if (LINK->stoperror) {
      listterminals(&V);
      fprintf(console, " Error(s) in input\n");
      consoleflush();
      stop(0);
    }
    LINK->symbcount = 1;
    top = 1;
    termg = lasttermval;
    leftflag = false;
    lasttermval++;
    firstnont = lasttermval;
    help[firstnont].rightp = top;
    lasttermval++;
    internv = lasttermval;
    firstnonterm = true;
    point = top;
    goalnotdefined = true;
    stopflag = false;
    break;

  /*14 optiondefs = "OPTIONS" colon optionlist       */
  case optiondefs1:
    break;

  /*15 emptydef = "EMPTYTERM" colon "<STRING>"       */
  /*16          | "<ERROR>"       */
  case emptydef1:
    readterminal(startofsymbol(3, &V) + 1, endofsymbol(3, &V), &V);
    emptyvalue = lasttermval;
    LINK->emptyaddress = stringarray[emptyvalue+1];
    break;

  /*17 goalsymboldef = "GOALSYMBOL" colon "<NAME>"       */
  case goalsymboldef1:
    nonterminal(startofsymbol(3, &V), endofsymbol(3, &V), LINK->niladdress,
		&V);
    goalsymbol = ntcount;
    goalknown = true;
    goalnotdefined = false;
    break;

  /*18 colon = ":"       */
  /*19       | "="       */
  case colon1:
  case colon2:
    break;

  /*20 optionlist = "<INTEGER>"       */
  /*21            | "<ERROR>"       */
  /*22            | optionlist "<INTEGER>"       */
  /*23            | optionlist "," "<INTEGER>"       */
  /*24            | optionlist "<ERROR>"       */
  /*25            | optionlist "," "<ERROR>"       */
  case optionlist1:
  case optionlist3:
  case optionlist4:
    break;

  /*26 termdeflist = "<STRING>"       */
  /*27             | termdeflist "<STRING>"       */
  /*28             | termdeflist "," "<STRING>"       */
  /*29             | termdeflist "=" "<STRING>"       */
  case termdeflist1:
    readterminal(startofsymbol(1, &V) + 1, endofsymbol(1, &V), &V);
    break;

  case termdeflist2:
    readterminal(startofsymbol(2, &V) + 1, endofsymbol(2, &V), &V);
    break;

  case termdeflist3:
  case termdeflist4:
    readterminal(startofsymbol(3, &V) + 1, endofsymbol(3, &V), &V);
    break;

  /*30 leftside = "<NAME>"       */
  /*31          | productions "<NAME>"       */
  /*32          | productions "<ERROR>"       */
  case leftside1:
  case leftside2:
    newnode(0, 0, leftside1, &V);
    if (V.prod == leftside1)
      V.i = 0;
    else
      V.i = 1;
    LINK->treeroot = LINK->attstack[V.newtop].node;
    WITH = LINK->treeroot;
    WITH->address = LINK->attstack[V.newtop + V.i].address;
    LINK->suffix = 'A';
    break;

  case leftside3:
    LINK->proderror = true;
    break;

  /*33 expression = term       */
  /*34            | "<ERROR>"       */
  /*35            | expression "|" term       */
  /*36            | expression "|" "<ERROR>"       */
  case expression1:
    break;

  case expression2:
    LINK->proderror = true;
    break;

  case expression3:
    newnode(1, 3, V.prod, &V);
    break;

  /*37 term = factor       */
  /*38      | term factor       */
  /*39      | term "<ERROR>"       */
  case term1:
    break;

  case term2:
    newnode(1, 2, V.prod, &V);
    break;

  case term3:
    LINK->proderror = false;
    break;

  /*40 factor = primary       */
  /*41        | "[" expression "]"       */
  /*42        | repeated       */
  /*43        | "<NAME>" colon repeated       */
  case factor1:
    break;

  case factor2:
    newnode(0, 2, expression3, &V);
    break;

  case factor3:
    if (LINK->treeroot == NULL)
      purgetree(&LINK->attstack[V.newtop].node, &V);
    else {
      newnode(1, 0, leftside1, &V);
      V.np = LINK->treeroot;
      while (V.np->right != NULL)
	V.np = V.np->right;
      V.np->right = LINK->attstack[V.newtop].node;
      newnode(0, 0, primary1, &V);
      getnewname(LINK->treeroot, &V);
      WITH = V.np->right;
      WITH->address = LINK->attstack[V.newtop].address;
      V.np = WITH->left->right;
      copytree(LINK->attstack[V.newtop].node, &V.np->left, &V);
      if (LINK->attstack[V.newtop].prodint == rightbrace2) {
	WITH = V.np;
	V.np = WITH->left;
	WITH->left = WITH->right;
	WITH->right = V.np;
      }
    }
    break;

  case factor4:
    newnode(3, 0, leftside1, &V);
    V.np = LINK->treeroot;
    while (V.np->right != NULL)
      V.np = V.np->right;
    V.np->right = LINK->attstack[V.newtop].node;
    newnode(0, 0, primary1, &V);
    WITH1 = &LINK->attstack[V.newtop];
    WITH1->node->address = WITH1->address;
    V.np->right->address = WITH1->address;
    V.np = V.np->right->left->right;
    copytree(LINK->attstack[V.newtop].node, &V.np->left, &V);
    if (LINK->attstack[V.newtop+2].prodint == rightbrace2) {
      WITH = V.np;
      V.np = WITH->left;
      WITH->left = WITH->right;
      WITH->right = V.np;
    }
    break;

  /*44 primary = "<NAME>"       */
  /*45         | "<STRING>"       */
  /*46         | "(" expression ")"       */
  case primary1:
  case primary2:
    newnode(0, 0, V.prod, &V);
    V.np = LINK->attstack[V.newtop].node;
    V.np->address = LINK->attstack[V.newtop].address;
    break;

  case primary3:
    LINK->attstack[V.newtop] = LINK->attstack[V.newtop+1];
    break;

  /*47 repeated = "{" expression rightbrace       */
  /*48          | "{" expression "$" rightbrace       */
  /*49          | "{" expression "$" expression rightbrace       */
  /*50          | "{" "$" expression rightbrace       */
  case repeated1:
  case repeated2:
  case repeated3:
  case repeated4:
    saveroot = LINK->attstack[V.newtop+1].node;
    switch (V.prod) {

    case repeated1:
      LINK->attstack[V.newtop].prodint = LINK->attstack[V.newtop+2].prodint;
      newnode(0, 2, term2, &V);
      break;

    case repeated2:
      LINK->attstack[V.newtop].prodint = LINK->attstack[V.newtop+3].prodint;
      newnode(0, 2, term2, &V);
      break;

    case repeated3:
      WITH1 = &LINK->attstack[V.newtop];
      LINK->attstack[V.newtop].prodint = LINK->attstack[V.newtop+4].prodint;
      if (LINK->attstack[V.newtop].prodint == rightbrace1)
	newnode(4, 2, term2, &V);
      else
	newnode(2, 4, term2, &V);
      newnode(0, 1, term2, &V);
      break;

    case repeated4:
      saveroot = NULL;
      LINK->attstack[V.newtop].prodint = LINK->attstack[V.newtop+3].prodint;
      newnode(0, 3, term2, &V);
      break;
    }
    newnode(0, 1, expression3, &V);
    copytree(saveroot, &LINK->attstack[V.newtop].node->left, &V);
    break;

  /*51 rightbrace = "rbraceL"       */
  /*52            | "rbraceR"       */
  case rightbrace1:
  case rightbrace2:
    LINK->attstack[V.newtop].prodint = V.prod;
    break;

  case ebnfinput3:
  case optionlist2:
  case optionlist5:
  case optionlist6:
  case emptydef2:
  case expression4:
    break;

  }

}  /* produce */

/* Local variables for parser: */
struct LOC_parser {
  struct LOC_inputgrammar *LINK;
  uchar eotchar;

  stackelm parsestack[stackmax + 1];   /*parse stack*/

  /* pentry: array [0..ordmaxch] of lxentry; */
  /* pentryhp: array [0..ordmaxch] of lxinx;
  pentrytv: array [0..ordmaxch] of psymbol; */

  /* plx: array [0..lxmax] of lxelm; */
  /*lexical tables*/
  /* plxnp: array [0..lxmax] of lxinx;
  plxhp: array [0..lxmax] of lxinx;
  plxtv: array [0..lxmax] of psymbol;
  plxch: array [0..lxmax] of {UNSIGNED} char; */

  /*  exponentchars: set of char;   chars in floating point numbers */
  psymbol newsymb;   /*current terminal symbol*/
  int oldsymb;   /*last terminal symbol*/
  uchar ch;   /*current char*/
  chbufinx oldbufi;   /* first token char in chbuf */
  boolean ok, moreinput;   /*parser flag */
  /* becomes false when input is exhausted */
  int reclength, lineinx;   /*length of current input record */
  reduelem redubuf[redumax + 1];   /* reduction buffer */
  reduinx redutop;
  stackinx stacktop, pseudotop, validtop, top;
  lrinx startinx, lri, start;
} ;

/* ----------------------------------------------------------------- */

/* procedures for input/output of characters */

Local void markerror(int n, int offset, struct LOC_parser *LINK)
{
  _REC_errmsg *WITH;

  LINK->LINK->errcount++;
  if (LINK->LINK->errmp < errmax) {
    LINK->LINK->errmp++;
    WITH = &LINK->LINK->errmsg[LINK->LINK->errmp];
    WITH->no = n;
    WITH->col = LINK->lineinx + offset;
    WITH->symb = LINK->newsymb;
  }
  LINK->LINK->proderror = true;
}

Local void readline(struct LOC_parser *LINK)
{
  LINK->lineinx = 1;
  LINK->LINK->linelength = 1;
  LINK->LINK->errmp = 0;
  if (P_eof(input)) {
    LINK->moreinput = false;
    LINK->ch = LINK->eotchar;
    LINK->reclength = 0;
    return;
  }
  while (!P_eoln(input) && LINK->LINK->linelength < linemax) {
    LINK->LINK->line[LINK->LINK->linelength-1] = getc(input);
    if (LINK->LINK->line[LINK->LINK->linelength-1] == '\n')
      LINK->LINK->line[LINK->LINK->linelength-1] = ' ';
    LINK->LINK->linelength++;
  }
  LINK->reclength = LINK->LINK->linelength - 1;
  if (P_eoln(input))
    LINK->LINK->line[LINK->LINK->linelength-1] = ' ';
  else {
    if (LINK->LINK->linelength == linemax)
      markerror(904, linemax - 2, LINK);
    while (!P_eoln(input) && LINK->reclength < recmax) {
      LINK->reclength++;
      LINK->LINK->line[LINK->reclength-1] = getc(input);
      if (LINK->LINK->line[LINK->reclength-1] == '\n')
	LINK->LINK->line[LINK->reclength-1] = ' ';
    }
  }
  LINK->ch = LINK->LINK->line[LINK->lineinx-1];
  fscanf(input, "%*[^\n]");
  getc(input);
}  /*readline*/

Local void printline(struct LOC_parser *LINK)
{
  int i, FORLIM;

  lineno++;
  fprintf(list, "%9d | ", lineno);
  FORLIM = LINK->reclength;
  for (i = 0; i < FORLIM; i++)
    putc(LINK->LINK->line[i], list);
  putc('\n', list);
}

Local void dumpline(struct LOC_parser *LINK)
{
  printline(LINK);
  if (LINK->LINK->errmp > 0)
    writeerror(LINK->LINK);
  readline(LINK);
}

Local void inchar(struct LOC_parser *LINK)
{
  if (LINK->lineinx < LINK->LINK->linelength) {
    LINK->lineinx++;
    LINK->ch = LINK->LINK->line[LINK->lineinx-1];
  } else if (LINK->lineinx == LINK->LINK->linelength) {
    LINK->lineinx++;
    LINK->ch = ' ';
  } else
    dumpline(LINK);
  if (LINK->ch == ordtabchar)
    LINK->ch = ' ';
}

/*end of input/output procedures
---------------------------------------------------------------------*/


Local void initialize(struct LOC_parser *LINK)
{
  /* var i: integer; */
  /*D if trace then debug := true; D*/
  LINK->LINK->errcount = 0;

  /* if debug then begin
      for i := ordminch to ordmaxch do
         writeln(list,'pentryhp[',i:1,']=',pentryhp[i]:1);
         for i := ordminch to ordmaxch do
            writeln(list,'pentrytv[',i:1,']=',pentrytv[i]:1);
         for i := 0 to lxmax do writeln(list,'plx ch,np,hp,tv',
             plxch[i]:2, plxnp[i]:4, plxhp[i]:4, plxtv[i]:4);
         i := 0;
         while i <= lrmax do begin
            write(list, 'plr ', plr[i]:4, plr[i+pnext]:4, plr[i+pkind]:4);
            case plr[i+pkind] of
               1, 2, 4, 6: writeln(list, plr[i+psymb]:4, plr[i+perr]:4);
               0, 3: writeln(list, plr[i+prs]:4, plr[i+pprod]:4);
               5: begin writeln(list, plr[i+plb]:4); i := i-1 end
               end;
            i := i+5
            end
      end; */


  /* symbol table */
  freesp = SEGdim;
  LINK->LINK->niladdress.seg = NULL;

  LINK->moreinput = true;
  LINK->LINK->chbufi = 0;
  readline(LINK);
  LINK->oldsymb = 0;
  LINK->eotchar = (Char)ordeotchar;
  LINK->oldsymb = 27;

  /* parser initializations */
  LINK->ok = true;
  LINK->LINK->maxerrcount = 10;
  LINK->parsestack[0].table = 0;
  LINK->LINK->attstack[0].chbufp = 0;
  LINK->parsestack[0].link = 0;
}

/*--------------------------------------------------------------------*/

Local void skipcomment(uchar controlchar, struct LOC_parser *LINK)
{
  /* read next char on input until comend is recognized */
  uchar endchar;

  if (controlchar == '{') {
    while (LINK->ch != '}' && LINK->moreinput) {
      if (LINK->ch == '{')
	markerror(901, 0, LINK);
      inchar(LINK);
    }
  } else {
    if (controlchar == '(')
      endchar = ')';
    else
      endchar = '/';
    do {
      while (LINK->ch != '*' && LINK->moreinput) {
	if (LINK->ch == controlchar) {
	  do {
	    inchar(LINK);
	  } while (LINK->ch == controlchar);
	  if (LINK->ch == '*')
	    markerror(901, 0, LINK);
	} else
	  inchar(LINK);
      }
      inchar(LINK);
    } while (LINK->ch != endchar && LINK->moreinput);
  }
  if (!LINK->moreinput)
    markerror(901, 0, LINK);
  inchar(LINK);
}

Local void pushch(struct LOC_parser *LINK)
{
  /* copy ch character into chbuf and get new ch */
  LINK->LINK->chbuf[LINK->LINK->chbufi] = LINK->ch;
  if (LINK->LINK->chbufi < chbufmax)
    LINK->LINK->chbufi++;
  else
    parserstop(5, LINK->LINK);
  inchar(LINK);
}

Local void readinteger(struct LOC_parser *LINK)
{
  LINK->LINK->integervalue = LINK->ch - '0';
  inchar(LINK);
  while (isdigit(LINK->ch) != 0) {
    LINK->LINK->integervalue = LINK->LINK->integervalue * 10 + LINK->ch - '0';
    inchar(LINK);
  }
  LINK->newsymb = 3;
}

Local void nextstrchar(int *i, struct LOC_parser *LINK)
{
  int iv, len;

  if (LINK->ch == bslchar) {
    inchar(LINK);
    if (LINK->ch == bslchar)
      pushch(LINK);
    else {
      iv = 0;
      len = 0;
      while (isdigit(LINK->ch) != 0 && len < 3) {
	len++;
	iv = iv * 8 + LINK->ch - '0';
	inchar(LINK);
      }
      if (len == 0 || iv < ordminch || iv > ordmaxch)
	markerror(905, 1, LINK);
      else {
	LINK->LINK->chbuf[LINK->LINK->chbufi] = iv;
	/* if debug then writeln(list,' chbufi=',chbufi:1,' chbuf[chbufi]=',
	   chbuf[chbufi]:1,'(',ord(chbuf[chbufi]):1,')'); */
	if (LINK->LINK->chbufi < chbufmax)
	  LINK->LINK->chbufi++;
	else
	  parserstop(5, LINK->LINK);
      }
    }
  } else
    pushch(LINK);
  (*i)++;
  if (LINK->lineinx > LINK->LINK->linelength)
    markerror(902, 1, LINK);
}

Local void readstring(uchar lastch, struct LOC_parser *LINK)
{
  /* puts string terminal into chbuf */
  int i = 0;
  int stop;

  stop = LINK->LINK->linelength + 1;
  while (LINK->lineinx < stop) {
    if (LINK->ch == lastch) {
      inchar(LINK);
      if (LINK->ch == lastch)
	nextstrchar(&i, LINK);
      else
	stop = LINK->lineinx;
    } else
      nextstrchar(&i, LINK);
  }
  if (i == 0)
    markerror(801, -1, LINK);
  LINK->newsymb = 4;
}

Local void installstring(int firstch, int length, stringaddress *symbaddr,
			 struct LOC_parser *LINK)
{
  int i;

  if (freesp + length + 1 > SEGdim) {
    symbtab = Malloc(sizeof(charset));
    freesp = 1;
  }
  symbtab[freesp-1] = length;
  for (i = 0; i < length; i++)
    symbtab[freesp + i] = LINK->LINK->chbuf[firstch + i];
  symbaddr->seg = symbtab;
  symbaddr->loc = freesp;
  freesp += length + 1;
}

Local void lexical(struct LOC_parser *LINK)
{
  /* sets newsymb to value of next acceptable terminal */
  /*D, i D*/
  int newi;
  uchar lastch, firstch;
  boolean terminalaccepted;

  do {
    terminalaccepted = true;
    while (LINK->ch == ' ')
      inchar(LINK);
    LINK->oldbufi = LINK->LINK->chbufi;
	/*first char of current terminal in chbuf*/
    if (!LINK->moreinput) {
      if (LINK->oldsymb == 0) {
	dumpline(LINK);
	parserstop(2, LINK->LINK);
      } else if (LINK->oldsymb == 1)
	LINK->newsymb = 0;
      else
	LINK->newsymb = 1;
    } else if (isdigit(LINK->ch) != 0)
      readinteger(LINK);
    else {
      firstch = LINK->ch;
      pushch(LINK);
      LINK->newsymb = pentrytv[firstch];   /* search in termtree */
      newi = pentryhp[firstch];
      while (newi != 0) {
	/* if trace then writeln(logfile,
	   ' newi=',newi:1,' ch:',ch,':',plxch[newi]); */
	if (plxch[newi] == LINK->ch) {
	  LINK->newsymb = plxtv[newi];
	  newi = plxhp[newi];
	  pushch(LINK);
	} else
	  newi = plxnp[newi];
      }
      lastch = LINK->LINK->chbuf[LINK->LINK->chbufi-1];
      if ((isalnum(lastch) != 0 || lastch == '_') &&
	  (isalnum(LINK->ch) != 0 || LINK->ch == '_')) {
	/*read trailing characters in name terminals*/
	do {
	  pushch(LINK);
	} while (isalnum(LINK->ch) != 0 || LINK->ch == '_');
	if (firstch == '_' || LINK->LINK->chbuf[LINK->LINK->chbufi-1] == '_')
	  markerror(903, -1, LINK);
	LINK->newsymb = 2;
      } else if (LINK->newsymb > 0) {
	if (LINK->newsymb == 12)
	  readstring(lastch, LINK);
	else if (LINK->newsymb == 26) {
	  if (lastch == '/') {  /* -/ */
	    dumpline(LINK);
	    LINK->oldsymb = 27;
	  } else
	    skipcomment(firstch, LINK);
	  terminalaccepted = false;
	} else if (LINK->newsymb == 27) {
	  /* if lastch = '-' then dumpline; */
	  /* -- */
	  dumpline(LINK);
	  terminalaccepted = false;
	}
      } else if (isalnum(lastch) != 0 || lastch == '_') {
	if (firstch == '_' || lastch == '_')
	  markerror(903, -1, LINK);
	LINK->newsymb = 2;
      }
      /*D else if firstch = '&' then begin D*/
      /* switch for logfile */
      /*D
         chbufi := oldbufi;
         debug := not debug;
         terminalaccepted := false
         end D*/
      else {
	markerror(800, -1, LINK);
	terminalaccepted = false;
	/* character not recognized */
      }
    }
    /*lookahead terminals*/
    /*D if debug then begin
       write(logfile, 'Lexical: ');
       for i := oldbufi to chbufi - 1 do write(logfile, chbuf[i]);
       writeln(logfile)
       end; D*/
    if (LINK->newsymb == 4)
      installstring(LINK->oldbufi + 1, LINK->LINK->chbufi - LINK->oldbufi - 1,
		    &LINK->LINK->saddr, LINK);
    else
      installstring(LINK->oldbufi, LINK->LINK->chbufi - LINK->oldbufi,
		    &LINK->LINK->saddr, LINK);
    if (LINK->newsymb < 0 || LINK->newsymb >= 32 ||
	((1L << LINK->newsymb) & 0x1c) == 0)
      LINK->LINK->chbufi = LINK->oldbufi;
  } while (!terminalaccepted);
  LINK->oldsymb = LINK->newsymb;
}

/*--------------------------------------------------------------------*/

Local void advance(struct LOC_parser *LINK)
{  /* perform reductions */
  int i, j, FORLIM;
  reduelem *WITH;
  attributes *WITH1;

  FORLIM = LINK->redutop;
  for (i = 1; i <= FORLIM; i++) {
    WITH = &LINK->redubuf[i];
    produce(WITH->oldtop, WITH->newtop, WITH->prod, LINK->LINK);
    LINK->LINK->attstack[WITH->newtop].chbufp =
      LINK->LINK->attstack[WITH->newtop - 1].chbufp;
  }
  /*D if debug then writeln(logfile); D*/

  /*update stack*/
  if (LINK->validtop != LINK->top) {
    FORLIM = LINK->stacktop - LINK->validtop;
    for (i = 1; i <= FORLIM; i++)
      LINK->parsestack[LINK->validtop + i] = LINK->parsestack[LINK->top + i];
  }

  if (LINK->redutop > 0) {
    j = LINK->LINK->attstack[LINK->stacktop-1].chbufp;
    FORLIM = LINK->LINK->chbufi - LINK->oldbufi;
    for (i = 0; i < FORLIM; i++)
      LINK->LINK->chbuf[j + i] = LINK->LINK->chbuf[LINK->oldbufi + i];
    LINK->LINK->chbufi += j - LINK->oldbufi;
    LINK->oldbufi = j;
  }

  /* shift */
  if (LINK->stacktop == stackmax)
    parserstop(1, LINK->LINK);
  LINK->stacktop++;
  LINK->parsestack[LINK->stacktop].table = LINK->startinx;
  WITH1 = &LINK->LINK->attstack[LINK->stacktop];

  WITH1->chbufp = LINK->LINK->chbufi;
  WITH1->address = LINK->LINK->saddr;
  /* freeze new stack situation, ready for new lookahead */
  LINK->top = LINK->stacktop;
  LINK->pseudotop = LINK->stacktop;
  LINK->validtop = LINK->stacktop;
  LINK->start = LINK->lri;
  LINK->redutop = 0;
}

Local void backtrack(stackinx btop, lrinx bstart, struct LOC_parser *LINK)
{
  LINK->stacktop = btop;
  LINK->validtop = btop;
  LINK->pseudotop = btop;
  LINK->startinx = bstart;
  LINK->lri = bstart;
  LINK->redutop = 0;
}

Local void pseudoshift(struct LOC_parser *LINK)
{
  attributes *WITH;

  if (LINK->pseudotop == stackmax)
    parserstop(1, LINK->LINK);
  LINK->stacktop++;
  LINK->pseudotop = LINK->top + LINK->stacktop - LINK->validtop;
  LINK->parsestack[LINK->pseudotop].table = LINK->startinx;
  WITH = &LINK->LINK->attstack[LINK->pseudotop];
  WITH->chbufp = LINK->LINK->chbufi;
  WITH->address = LINK->LINK->saddr;
}

/*--------------------------------------------------------------------*/

Local void queue(rslength rs, prodno p, struct LOC_parser *LINK)
{
  reduelem *WITH;

  if (p >= 0) {
    if (LINK->redutop < redumax)
      LINK->redutop++;
    else
      parserstop(4, LINK->LINK);
    WITH = &LINK->redubuf[LINK->redutop];
    WITH->oldtop = LINK->stacktop;
    LINK->stacktop -= rs;
    WITH->newtop = LINK->stacktop;
    WITH->prod = p;
  } else
    LINK->stacktop -= rs;
  if (LINK->stacktop <= LINK->validtop) {
    LINK->pseudotop = LINK->stacktop;
    LINK->validtop = LINK->stacktop;
  } else
    LINK->pseudotop -= rs;
}

Local boolean lookahead_(psymbol lsymbol, struct LOC_parser *LINK)
{
  boolean Result;
  boolean decided = false;
  lrinx si;

  /*D if trace then write(logfile, ' lri: ',lri:1); D*/
  do {
    /*D if trace then write(logfile, ' lri ',lri:1); D*/
    LINK->startinx = LINK->lri;
    switch (plr[LINK->lri + pkind]) {

    case 0:
      decided = true;
      Result = true;
      LINK->ok = false;
      break;

    case 1:
      while (plr[LINK->lri + psymb] != lsymbol && plr[LINK->lri] != 0)
	LINK->lri = plr[LINK->lri];
      decided = true;
      Result = (plr[LINK->lri + psymb] == lsymbol);
      break;

    case 2:
    case 4:
    case 6:
      while (plr[LINK->lri + psymb] != lsymbol && plr[LINK->lri] != 0)
	LINK->lri = plr[LINK->lri];
      if (plr[LINK->lri + pkind] == 2) {
	decided = true;
	Result = true;
      } else if (plr[LINK->lri + pkind] == 6) {
	pseudoshift(LINK);
	LINK->stacktop--;
	LINK->pseudotop--;
	queue(-1, plr[LINK->lri + perr], LINK);
      }
      break;

    case 3:
      queue(plr[LINK->lri + prs], plr[LINK->lri + pprod], LINK);
      break;

    case 5:
      si = LINK->parsestack[LINK->pseudotop].table;
      /*D if trace then write(logfile, ' si(', si: 1, ')'); D*/
      while (plr[LINK->lri + plb] != si && plr[LINK->lri] != 0)
	LINK->lri = plr[LINK->lri];
      break;
    }
    LINK->lri = plr[LINK->lri + pnext];
    /*D; if trace then write(logfile, lri: 4) D*/
  } while (!decided);
  /*D if trace then writeln(logfile) D*/
  return Result;
}

/*--------------------------------------------------------------------*/

Local void syntaxerror(struct LOC_parser *LINK)
{
  boolean success = false;
  stackinx s = 0;
  stackinx s1, FORLIM;

  /*D if debug then write(logfile, ' <---SYNTAXERROR'); D*/
  markerror(plr[LINK->startinx + perr], -1, LINK);
  backtrack(LINK->top, LINK->start, LINK);
  pseudoshift(LINK);
  FORLIM = LINK->top;
  for (s1 = 0; s1 <= FORLIM; s1++) {
    backtrack(s1, LINK->parsestack[s1+1].table, LINK);
    if (lookahead_(5, LINK)) {
      LINK->parsestack[s1].link = s;
      s = s1;
    }
  }
  do {
    s1 = s;
    do {
      backtrack(s1, LINK->parsestack[s1+1].table, LINK);
      if (lookahead_(5, LINK)) {
	pseudoshift(LINK);
	success = lookahead_(LINK->newsymb, LINK);
      }
      s1 = LINK->parsestack[s1].link;
    } while (!(s1 == 0 || success));
    if (!success) {
      /* mark previous symbol skipped */
      /*D if debug then writeln(logfile, ' <---SKIPPED'); D*/
      lexical(LINK);
    }
  } while (!success && LINK->ok);
  if (!LINK->ok)
    parserstop(3, LINK->LINK);
}

/*--------------------------------------------------------------------*/

Local void parser(struct LOC_inputgrammar *LINK)
{
  struct LOC_parser V;

  /*--------------------------------------------------------------------*/

  V.LINK = LINK;
  initialize(&V);
  produce(0, 1, -1, LINK);
  V.top = 0;
  V.start = 5;
  backtrack(V.top, V.start, &V);
  while (V.ok) {
    lexical(&V);
    /*D if trace then write(logfile,' Sym:',newsymb: 3,' State:', lri: 3); D*/
    if (!lookahead_(V.newsymb, &V))
      syntaxerror(&V);
    advance(&V);
  }
}


/* PG1----------------------------------------------------------------*/
void inputgrammar(void)
{
  struct LOC_inputgrammar V;

  parser(&V);
_L10:
  produce(2, 0, 0, &V);
  if (V.errcount == 0)
    return;
  fprintf(list, "\n      *** %2d Source errors detected\n", V.errcount);
  fprintf(console, " *** %2d Source errors detected\n", V.errcount);
  stop(0);
}


void main(int argc, Char *argv[])
{
  P_argc = argc; P_argv = argv; __top_jb = NULL;
  if (setjmp(_JL999))
    goto _L999;
  logfile = NULL;
  console = NULL;
  list = NULL;
  output = NULL;
  input = NULL;
  /*D trace := false; D*/
  /* change for debugging */
  /*D debug := true;
  if trace then debug := true; D*/
  openfiles();
  headline();
  /*HF writeln(console,' ',ParamStr(0),' (Version 5)'); FH*/
  fputc(' ',console); char *cp = P_argv[0]; int len = MSTRINGlen;
  while (*cp && --len >= 0) fputc(*cp++,console);
  fprintf(console," (Version 5)\n");
  freeptr = NULL;
  inputgrammar();
  removeid(true);
  termination();
  removeid(false);
  vhrecurs();
  initprod();
  listgrammar();
  lr0();
  lookb();
  verify();
  compress();
  emitconstants();
  errortable();
  emitlex();
  emitpars();
_L999:
  putc('\n', list);
  if (input != NULL)
    fclose(input);
  if (output != NULL)
    fclose(output);
  if (list != NULL)
    fclose(list);
  if (console != NULL)
    fclose(console);
  if (logfile != NULL)
    fclose(logfile);
  exit(EXIT_SUCCESS);
}



/* End. */
