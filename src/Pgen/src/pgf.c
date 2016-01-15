/* Output from p2c 1.21alpha-07.Dec.93, the Pascal-to-C translator */
/* From input file "PGf.p" */


#include "p2c.h"


/*X program pgf(input,output); X*/

/* Converts output tables from pg to a form usable in the Fortran
   form of the skeleton compiler */

/* UXM 127 MXU */
/* B 255 B */

#define ORDMAXCH        255
#define TERMLENGTH      20   /* This was changed from 10 */
#define namelen         ORDMAXCH
#define stlen           255


typedef Char namestring[namelen];

/*B  namestring = string(namelen); B*/
/*X  namestring = string(namelen);
       accessname = packed array [1..namelen] of char; X*/


Static FILE *instfile, *parscdc, *parsedit, *lexedit, *parscst, *parserr,
	    *lextab, *parstab;
Static int lrmax, lrlin, lxmax;


/*X function access( var f:char; n:integer ): integer; external; X*/
/*H name 'access' H*/
/*G; asmname '_p_Access'G */
extern int access(Char *f, int n);


#define symbollength    30


Local int numlen(int n)
{
  int len = 0;

  while (n > 0) {
    n /= 10;
    len++;
  }
  return len;
}

Local Char uppercase(Char ch)
{
  if (islower(ch))
    return _toupper(ch);
  else
    return ch;
}


Static void prodnames(void)
{
  /* Writes onto parscdc the const definitions of the nonterminals
     for use in a case statement of the semantic action routine.
     Input is in the form:   no. of alternate productions    nonterminal
                                     .                           .
                                     .                           .
                                     0                                  */
  int nalts, lgth, incrlen, lineposition;
  int prodno = -1;
  int prodrev, alternative, i;
  Char symbol[symbollength];

  if (parscdc != NULL)
    parscdc = freopen("parscdc", "w", parscdc);
  else
    parscdc = fopen("parscdc", "w");
  if (parscdc == NULL)
    _EscIO2(FileNotFound, "parscdc");
  if (parsedit != NULL)
    parsedit = freopen("parsedit", "w", parsedit);
  else
    parsedit = fopen("parsedit", "w");
  if (parsedit == NULL)
    _EscIO2(FileNotFound, "parsedit");
  scanf("%d", &nalts);

  while (nalts != 0) {
    *symbol = getchar();
    if (symbol[0] == '\n')
      symbol[0] = ' ';
    lgth = 0;
    do {
      lgth++;
      symbol[lgth-1] = getchar();
      if (symbol[lgth-1] == '\n')
	symbol[lgth-1] = ' ';
    } while (!(P_eoln(stdin) || lgth == symbollength || symbol[lgth-1] == ' '));
    if (symbol[lgth-1] == ' ')
      lgth--;
    scanf("%*[^\n]");

    getchar();
    fprintf(parscdc, "      PARAMETER(");
    lineposition = 16;
    for (alternative = 1; alternative <= nalts; alternative++) {
      prodno++;
      incrlen = lgth + numlen(prodno) + numlen(alternative) + 1;
      if (alternative > 1)
	incrlen += 2;
      if (lineposition + incrlen > 71) {
	fprintf(parscdc, ")\n");
	fprintf(parscdc, "      PARAMETER(");
	lineposition = 16;
      }
      if (lineposition != 16)
	fprintf(parscdc, ", ");
      for (i = 0; i < lgth; i++)
	fputc(uppercase(symbol[i]), parscdc);
      fprintf(parscdc, "%d=%d", alternative, prodno);
      lineposition += incrlen;
    }
    prodrev = prodno;
    for (alternative = nalts; alternative >= 1; alternative--) {
      fprintf(parsedit, "s/");
      for (i = 0; i < lgth; i++)
	putc(symbol[i], parsedit);
      fprintf(parsedit, "%d/%d/g\n", alternative, prodrev);

      prodrev--;
    }
    /*B    write(parsedit,'C /');
             for i:= 1 to lgth do write(parsedit,symbol[i]);
             writeln(parsedit,
                 alternative:1,'/',prodrev:1,'/ * *');        B*/
    /* VMS */
    /*V    write(parsedit,'C /');
             for i:= 1 to lgth do write(parsedit,symbol[i]);
             writeln(parsedit,
                 alternative:1,'/',prodrev:1,'/whole');       V*/
    fprintf(parscdc, ")\n");
    scanf("%d", &nalts);
  }
  scanf("%*[^\n]");
  getchar();
}  /* prodnames */

#undef symbollength


Static void tableconstants(void)
{
  /*  Write onto parscst the four const definitions for
               symbmax= no. of terminals
               prodmax= no. of productions
               lrmax= lgth of lalr table
               lrlin= linear length of lalr table
               lxmax= lgth of lexical table not counting
                      initial entry table          */
  int symbmax, prodmax;

  if (parscst != NULL)
    parscst = freopen("parscst", "w", parscst);
  else
    parscst = fopen("parscst", "w");
  if (parscst == NULL)
    _EscIO2(FileNotFound, "parscst");
  scanf("%d%*[^\n]", &symbmax);
  getchar();
  fprintf(parscst, "      PARAMETER ( SYMMAX=%3d )\n", symbmax);
  scanf("%d%*[^\n]", &prodmax);
  getchar();
  fprintf(parscst, "      PARAMETER ( PRDMAX=%3d )\n", prodmax);
  scanf("%d%*[^\n]", &lrmax);
  getchar();
  scanf("%d%*[^\n]", &lrlin);
  getchar();
  scanf("%d%*[^\n]", &lxmax);
  getchar();
  fprintf(parscst, "      PARAMETER ( LRLIN=%3d )\n", lrlin);
}  /* tableconstants */


Static void Wlx(void)
{
  fprintf(parscst, "      PARAMETER ( LXMAX=%3d )\n", lxmax);
}  /* Wlx */


#define symmax          1000
#define arrmax          1000


typedef Char symbol[TERMLENGTH];


/* Local variables for parsererrors: */
struct LOC_parsererrors {
  symbol symbs[symmax];
} ;

Local int match(int *symbolp, struct LOC_parsererrors *LINK)
{
  int Result;
  int i = 1;

  while (strncmp(LINK->symbs[i-1], LINK->symbs[*symbolp - 1], sizeof(symbol)))
    i++;
  Result = i;
  if (i != *symbolp)
    (*symbolp)--;
  return Result;
}


Static void parsererrors(void)
{
  /* Writes error constants onto parscst and
            onto parserr the tables corresponding
     to the expected terminals for each error number.  Input is in
     the form:           error number      no. of terminals
                         terminal1
                         terminal2
                             .
                             .
                         error no.        no. of terminals
                                    .
                                    .
                             0                                    */
  struct LOC_parsererrors V;
  int i, j, k;
  int maxsl = 0;
  int errorno, elements, elementno;
  int plen = 0, errorcount = 0, symbolp = 0;
  int ptrs[arrmax];

  if (parserr != NULL)
    parserr = freopen("parserr", "w", parserr);
  else
    parserr = fopen("parserr", "w");
  if (parserr == NULL)
    _EscIO2(FileNotFound, "parserr");
  scanf("%d", &errorno);
  while (errorno != 0) {
    scanf("%d%*[^\n]", &elements);
    getchar();
    plen++;
    ptrs[plen-1] = elements;
    errorcount = errorno;
    for (elementno = 1; elementno <= elements; elementno++) {
      symbolp++;
      i = 0;
      do {
	i++;
	V.symbs[symbolp-1][i-1] = getchar();
	if (V.symbs[symbolp-1][i-1] == '\n')
	  V.symbs[symbolp-1][i-1] = ' ';
      } while (!(P_eoln(stdin) || i >= TERMLENGTH));
      scanf("%*[^\n]");
      getchar();
      j = 0;
      while (i > j) {
	if (V.symbs[symbolp-1][i-1] == ' ')
	  i--;
	else
	  j = i;
      }
      if (i > maxsl)
	maxsl = i;
      for (j = i; j < TERMLENGTH; j++)
	V.symbs[symbolp-1][j] = ' ';
      plen++;
      ptrs[plen-1] = match(&symbolp, &V);
    }
    scanf("%d", &errorno);
  }
  scanf("%*[^\n]");
  getchar();
  fprintf(parserr, "      DIMENSION EPTR(%d)\n", errorcount + plen);
  fprintf(parserr, "      CHARACTER*%d ERRTRM(%d)\n", maxsl, symbolp);
  i = 0;
  while (i < symbolp) {
    j = i + 66 / (maxsl + 3);
    if (j > symbolp)
      j = symbolp;
    fprintf(parserr, "      DATA (ERRTRM(I),I=%d,%d)/\n", i + 1, j);
    fprintf(parserr, "     *");
    for (k = i + 1; k <= j; k++) {
      fprintf(parserr, "'%.*s'", maxsl, V.symbs[k-1]);
      if (k != j)
	putc(',', parserr);
      else
	fprintf(parserr, "/\n");
    }
/* p2c: PGf.p, line 215: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    i = j;
  }
  i = 0;
  elements = errorcount + plen;
  plen = 1;
  while (i < elements) {
    j = i + 60;
    if (j > elements)
      j = elements;
    fprintf(parserr, "      DATA (EPTR(I),I=%d,%d)/\n", i + 1, j);
    fprintf(parserr, "     *");
    k = 0;
    for (elementno = i + 1; elementno <= j; elementno++) {
      if (elementno <= errorcount) {
	fprintf(parserr, "%4d", errorcount + plen);
	plen += ptrs[plen-1] + 1;
      } else
	fprintf(parserr, "%4d", ptrs[elementno - errorcount - 1]);
      k++;
      if (elementno != j) {
	if (k != 12)
	  putc(',', parserr);
	else {
	  fprintf(parserr, ",\n");
	  fprintf(parserr, "     *");
	  k = 0;
	}
      } else
	fprintf(parserr, "/\n");
    }
    i = j;
  }
}  /* parsererrors */

#undef symmax
#undef arrmax


typedef int lxtable[ORDMAXCH + 1];

typedef struct lxelm {
  int np, hp, tv;
  Char ch;
  struct lxelm *hpp, *npp, *prev;
} lxelm;


typedef Char datalabel[4];


/* Local variables for lxTables: */
struct LOC_lxTables {
  lxtable hp, tv;
  lxelm *hp0[ORDMAXCH + 1];
  Char cc;
} ;

Local void Sendlx(int *table, struct LOC_lxTables *LINK)
{
  int i = 0;
  int j, k;

  while (i <= ORDMAXCH) {
    fprintf(lextab, "\n     * ");
    k = i + 15;
    if (k < ORDMAXCH) {
      for (j = i; j <= k; j++)
	fprintf(lextab, "%3d,", table[j]);
    } else {
      for (j = i; j < ORDMAXCH; j++)
	fprintf(lextab, "%3d,", table[j]);
      fprintf(lextab, "%3d", table[ORDMAXCH]);
    }
    i = k + 1;
  }
  fprintf(lextab, "/\n");
}  /* Sendlx */

Local void Rltable(lxelm **lx, struct LOC_lxTables *LINK)
{
  /* Read in tree */
  lxelm *WITH;

  *lx = Malloc(sizeof(lxelm));
  scanf("%c%c%d%d%d%*[^\n]", &LINK->cc, &(*lx)->ch, &(*lx)->np, &(*lx)->hp,
	&(*lx)->tv);
  getchar();
  if (LINK->cc == '\n')
    LINK->cc = ' ';
  if ((*lx)->ch == '\n')
    (*lx)->ch = ' ';
  WITH = *lx;
  if (WITH->np > 0)
    Rltable(&WITH->npp, LINK);
  else
    WITH->npp = NULL;
  if (WITH->hp > 0)
    Rltable(&WITH->hpp, LINK);
  else
    WITH->hpp = NULL;
}  /* Rltable */

Local void Readlx(struct LOC_lxTables *LINK)
{
  Char cc, ch1;
  int i;

  for (i = 0; i <= ORDMAXCH; i++) {
    LINK->hp0[i] = NULL;
    LINK->hp[i] = 0;
    LINK->tv[i] = 0;
  }
  scanf("%c%c%d", &cc, &cc, &i);
  if (cc == '\n')
    cc = ' ';
  if (cc == '\n')
    cc = ' ';
  do {
    scanf("%d%d%*[^\n]", &LINK->hp[cc], &LINK->tv[cc]);
    getchar();
    ch1 = cc;
    scanf("%c%c%d", &cc, &cc, &i);
    if (cc == '\n')
      cc = ' ';
    if (cc == '\n')
      cc = ' ';
  } while (cc != ch1);
  scanf("%*[^\n]");
  getchar();
  scanf("%*[^\n]");
  getchar();
  for (i = 0; i <= ORDMAXCH; i++) {
    if (LINK->hp[i] > 0)
      Rltable(&LINK->hp0[i], LINK);
  }
}  /* Readlx */

/* Local variables for Wltable: */
struct LOC_Wltable {
  struct LOC_lxTables *LINK;
  int i, j;
} ;

Local void wltr(Char *lab, lxelm *lx, struct LOC_Wltable *LINK)
{
  lxelm *WITH;

  while (lx != NULL && LINK->i <= lxmax) {
    WITH = lx;
    if (LINK->i > LINK->j) {
      if (LINK->i + 15 <= lxmax)
	LINK->j = LINK->i + 15;
      else
	LINK->j = lxmax;
      fprintf(lextab, "      DATA (%.4s(I),I=%3d,%3d)/\n",
	      lab, LINK->i, LINK->j);
      fprintf(lextab, "     * ");
    }
    if (!strncmp(lab, "LXCH", sizeof(datalabel))) {
      putc('\'', lextab);
      if (WITH->ch == '\\')
	putc('\\', lextab);
      fprintf(lextab, "%c'", WITH->ch);
    } else if (!strncmp(lab, "LXNP", sizeof(datalabel)))
      fprintf(lextab, "%3d", WITH->np);
    else if (!strncmp(lab, "LXHP", sizeof(datalabel)))
      fprintf(lextab, "%3d", WITH->hp);
    else if (!strncmp(lab, "LXTV", sizeof(datalabel)))
      fprintf(lextab, "%3d", WITH->tv);
    if (LINK->i < LINK->j)
      putc(',', lextab);
    else
      fprintf(lextab, "/\n");
    LINK->i++;
    wltr(lab, WITH->npp, LINK);
    lx = WITH->hpp;
    /* wltr( lab,hpp ) */
  }
}  /* wltr */

Local void tpurge(lxelm **lx, struct LOC_Wltable *LINK)
{
  if (*lx == NULL)
    return;
  tpurge(&(*lx)->hpp, LINK);
  tpurge(&(*lx)->npp, LINK);
  Free(*lx);
}  /* tpurge */

Local void Wltable(struct LOC_lxTables *LINK)
{
  struct LOC_Wltable V;
  int k;

  V.LINK = LINK;
  V.i = 1;
  V.j = 0;
  for (k = 0; k <= ORDMAXCH; k++)
    wltr("LXCH", LINK->hp0[k], &V);
  V.i = 1;
  V.j = 0;
  for (k = 0; k <= ORDMAXCH; k++)
    wltr("LXNP", LINK->hp0[k], &V);
  V.i = 1;
  V.j = 0;
  for (k = 0; k <= ORDMAXCH; k++)
    wltr("LXHP", LINK->hp0[k], &V);
  V.i = 1;
  V.j = 0;
  for (k = 0; k <= ORDMAXCH; k++)
    wltr("LXTV", LINK->hp0[k], &V);
  for (k = 0; k <= ORDMAXCH; k++)
    tpurge(&LINK->hp0[k], &V);
}  /* Wltable */

/* Local variables for Sendlexedit: */
struct LOC_Sendlexedit {
  struct LOC_lxTables *LINK;
  Char namebuffer[stlen];
} ;

/*BX    namebuffer: string(stlen); XB*/

Local void Sendeditstr(int len, int val, struct LOC_Sendlexedit *LINK)
{
  int i;

  /*B    write(lexedit,'C /<');
           for i:= 1 to len do write(lexedit,namebuffer[i]);
           writeln(lexedit,'>/',val:1,'/ * *')        B*/
  /*V    write(lexedit,'C /<');
           for i:= 1 to len do write(lexedit,namebuffer[i]);
           writeln(lexedit,'>/',val:1,'/whole')       V*/
  fprintf(lexedit, "s/<");
  for (i = 0; i < len; i++) {
    if (LINK->namebuffer[i] == '\\' || LINK->namebuffer[i] == '[' ||
	LINK->namebuffer[i] == '*' ||
	LINK->namebuffer[i] == '.' || LINK->namebuffer[i] == '/')
      putc('\\', lexedit);
    putc(LINK->namebuffer[i], lexedit);
  }
  fprintf(lexedit, ">/%d/g\n", val);
}  /* Sendeditstr */

Local void sendleaf(lxelm *p, int ind, struct LOC_Sendlexedit *LINK)
{
  while (p != NULL) {
    /*BX if ind >= length(namebuffer) then
    namebuffer := namebuffer||str(' '); XB*/
    LINK->namebuffer[ind] = p->ch;
    if (p->hpp != NULL)
      sendleaf(p->hpp, ind + 1, LINK);
    if (p->tv != 0)
      Sendeditstr(ind + 1, p->tv, LINK);
    LINK->namebuffer[ind] = p->ch;
    p = p->npp;
    /* if p^.npp <> nil then sendleaf(p^.npp,ind) */
  }
}  /* sendleaf */

Local void Sendlexedit(struct LOC_lxTables *LINK)
{
  struct LOC_Sendlexedit V;
  int i = '<';
  int j;

  V.LINK = LINK;
  memcpy(V.namebuffer,
    "METAGOAL                                                                                                                                                                                                                                                       ",
    stlen);
      /*X namebuffer := str('METAGOAL'); X*/
  Sendeditstr(8, 0, &V);
  memcpy(V.namebuffer,
    "<EOF>                                                                                                                                                                                                                                                          ",
    stlen);
      /*X namebuffer := str('<EOF>')   ; X*/
  Sendeditstr(5, 1, &V);

  /* Do '<' first to avoid editor conflicts */
  V.namebuffer[0] = '<';
  if (LINK->hp0[i] != NULL)
    sendleaf(LINK->hp0[i], 1, &V);
  if (LINK->tv[i] != 0)
    Sendeditstr(1, LINK->tv[i], &V);

  for (j = 0; j <= ORDMAXCH; j++) {
    if (j != i) {
      V.namebuffer[0] = j;
      if (LINK->hp0[j] != NULL)
	sendleaf(LINK->hp0[j], 1, &V);
      if (LINK->tv[j] != 0)
	Sendeditstr(1, LINK->tv[j], &V);
    }
  }
}  /* Sendlexedit */

/* Local variables for Checkdelete: */
struct LOC_Checkdelete {
  struct LOC_lxTables *LINK;
  int i;
} ;

Local void lxdelete(Char *name, int len, struct LOC_Checkdelete *LINK)
{
  int i = 1;
  lxelm *ap;
  lxelm *app = NULL;
  lxelm *WITH;

  if (len == 1) {
    LINK->LINK->tv[name[0]] = 0;
    return;
  }
  if (len <= 1)
    return;
  ap = LINK->LINK->hp0[name[0]];
  while (i < len && ap != NULL) {
    ap->prev = app;
    app = ap;
    if (name[i] == ap->ch) {
      i++;
      ap = ap->hpp;
    } else
      ap = ap->npp;
  }
  if (i != len || app == NULL) {
    printf("pgf: Terminal \"%.*s\" not found.\n", len, name);
/* p2c: PGf.p, line 454: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    return;
  }
  app->tv = 0;
  while (i > 1) {
    WITH = app;
    if (WITH->tv != 0 || WITH->hpp != NULL) {
      i = 0;
      break;
    }
    ap = WITH->prev;
    if (ap == NULL)
      LINK->LINK->hp0[name[0]] = WITH->npp;
    else if (ap->hpp == app)
      ap->hpp = WITH->npp;
    else
      ap->npp = WITH->npp;
    lxmax--;
    Free(app);
    app = ap;
    i--;
  }
}  /* lxdelete */

Local void fixtablerefs(lxelm *lx, struct LOC_Checkdelete *LINK)
{
  if (lx == NULL)
    return;
  if (lx->npp == NULL)
    lx->np = 0;
  else {
    LINK->i++;
    lx->np = LINK->i;
    fixtablerefs(lx->npp, LINK);
  }
  if (lx->hpp == NULL) {
    lx->hp = 0;
    return;
  }
  LINK->i++;
  lx->hp = LINK->i;
  fixtablerefs(lx->hpp, LINK);
}  /* fixtablerefs */


Local int length_(Char *s, struct LOC_Checkdelete *LINK)
{
  int i = namelen, j = 0;

  while (i > j) {
    if (s[i-1] == ' ')
      i--;
    else
      j = i;
  }
  return i;
}

Local void Checkdelete(struct LOC_lxTables *LINK)
{
  struct LOC_Checkdelete V;
  /*B args: string(stlen); lenargs: integer; ch: char;
     ftype,fm:namestring; B*/
  /*X args: string(stlen); lenargs: integer; ch: char;
     accname: accessname; X*/
  namestring namet;
  int j;

  /*BX
procedure tokn(var i:integer;
     lln:integer; line:string(stlen); var nam:namestring);
var j: integer;
begin
     j := lln+1;
     while i < j do if line[i] = ' ' then i := i+1 else j := i;
     if j <= lln then begin nam := str(line[j]); j := j+1 end;
     i := lln+1;
     while j < i do if line[j] = ' ' then i := j else begin
        nam := nam||str(line[j]);
        j := j+1
        end
     end; XB*/

  /*B
     args := parms||str(' '); lenargs := length(args); i := 1;
     if lenargs > 1 then begin
        tokn(i,lenargs,args,namet);
        if (namet[1] = '-') and
           ((namet[2] = 'd') or (namet[2] = 'D')) then
           repeat
              tokn(i,lenargs,args,namet);
              if i <= lenargs then lxdelete(namet,length(namet))
           until i > lenargs
        else if (namet[1] = '-') and
          ((namet[2] = 'f') or (namet[2] = 'F')) then begin
           if i > lenargs then
              writeln(output,'-f fname ftype [fmode]')
           else begin
              tokn(i,lenargs,args,namet);
              if i > lenargs then ftype := str('*')
              else tokn(i,lenargs,args,ftype);
              if i > lenargs then fm := str('*')
              else tokn(i,lenargs,args,fm);
              reset(instfile,'name='||namet||'.'||ftype||'.'||fm);
              while not eof(instfile) do begin
                 read(instfile,ch); namet := str(ch);
                 while not eoln(instfile) do begin
                    read(instfile,ch);
                    namet := namet||str(ch)
                    end;
                 i := length(namet); j := 0;
                 while i > j do if namet[i]=' ' then i:=i-1 else j:=i;
                 readln(instfile);
                 lxdelete(namet,i)
                 end
              end
           end; B*/
  /*X
     args := parms||str(' '); lenargs := length(args); i := 1;
     if lenargs > 1 then begin
        tokn(i,lenargs,args,namet);
        if (namet[1] = '-') and (namet[2] = 'd') then
           repeat
              tokn(i,lenargs,args,namet);
              if i <= lenargs then lxdelete(namet,length(namet))
           until i > lenargs
        else if (namet[1] = '-') and (namet[2] = 'f') then begin
           if i > lenargs then writeln(output,'-f fname')
           else begin
              tokn(i,lenargs,args,namet);
              accname := namet||chr(0);
              if access(accname[1],4) = 0 then begin
                 reset(instfile,'NAME='||namet);
                 while not eof(instfile) do begin
                    read(instfile,ch); namet := str(ch);
                    while not eoln(instfile) do begin
                       read(instfile,ch);
                       namet := namet||str(ch)
                       end;
                    i := length(namet); j := 0;
                    while i > j do if namet[i]=' ' then i:=i-1 else j:=i;
                    readln(instfile);
                    lxdelete(namet,i)
                    end
                 end
              else writeln(output,'Cannot read file ',namet:i)
              end
           end; X*/

  V.LINK = LINK;
  if (P_argc <= 2)
    return;
  P_sun_argv(namet, sizeof(namestring), 1);
  if (namet[0] == '-' && namet[1] == 'd') {
    for (j = 2; j < P_argc; j++) {
      P_sun_argv(namet, sizeof(namestring), j);
      lxdelete(namet, length_(namet, &V), &V);
    }
  } else if (namet[0] == '-' && namet[1] == 'f') {
    P_sun_argv(namet, sizeof(namestring), 2);
    namet[length_(namet, &V)] = '\0';
    if (access(namet, 4) == 0) {
      if (instfile != NULL)
	instfile = freopen(P_trimname(namet, sizeof(namestring)), "r",
			   instfile);
      else
	instfile = fopen(P_trimname(namet, sizeof(namestring)), "r");
      if (instfile == NULL)
	_EscIO2(FileNotFound, P_trimname(namet, sizeof(namestring)));
      while (!P_eof(instfile)) {
	V.i = 0;
	while (!P_eoln(instfile)) {
	  if (V.i < namelen)
	    V.i++;
	  namet[V.i-1] = getc(instfile);
	  if (namet[V.i-1] == '\n')
	    namet[V.i-1] = ' ';
	}
	fscanf(instfile, "%*[^\n]");
	getc(instfile);
	lxdelete(namet, V.i, &V);
      }
    } else {
      printf("Cannot read file %.*s\n", V.i, namet);
/* p2c: PGf.p, line 581: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    }
  }

  /*GH
     if ParamCount > 1 then begin
        namet := ParamStr(1);
        if (namet[1] = '-') and (namet[2] = 'd') then
           for j := 2 to ParamCount do begin
              namet := ParamStr(j);
              lxdelete(namet,length(namet))
              end
        else if (namet[1] = '-') and (namet[2] = 'f') then begin
           namet := ParamStr(2);
           namet[1+length(namet)] := chr(0); HG*/
  /*G if 4-Access(namet,4) = 0 then begin G*/
  /*H if access(namet,4) = 0 then begin H*/
  /*GH
     reset(instfile,namet);
     while not eof(instfile) do begin
        i := 0;
        while not eoln(instfile) do begin
           if i < namelen then i := i+1;
           read(instfile,namet[i])
           end;
        readln(instfile);
        lxdelete(namet,i)
        end
     end
  else writeln(output,'Cannot read file ',namet:i)
  end;
   HG*/
  V.i = 0;
  for (j = 0; j <= ORDMAXCH; j++) {
    if (LINK->hp0[j] != NULL) {
      V.i++;
      LINK->hp[j] = V.i;
      fixtablerefs(LINK->hp0[j], &V);
    } else
      LINK->hp[j] = 0;
  }
}  /* Checkdelete */


Static void lxTables(void)
{
  struct LOC_lxTables V;

  Readlx(&V);
  if (lexedit != NULL)
    lexedit = freopen("lexedit", "w", lexedit);
  else
    lexedit = fopen("lexedit", "w");
  if (lexedit == NULL)
    _EscIO2(FileNotFound, "lexedit");
  Sendlexedit(&V);
  Checkdelete(&V);
  if (lextab != NULL)
    lextab = freopen("lextab", "w", lextab);
  else
    lextab = fopen("lextab", "w");
  if (lextab == NULL)
    _EscIO2(FileNotFound, "lextab");
  fprintf(lextab, "      DATA HP1 /");
  Sendlx(V.hp, &V);
  fprintf(lextab, "      DATA TV1 /");
  Sendlx(V.tv, &V);
  Wltable(&V);
  Wlx();
}  /* lxTables */


#define MaxContin       4   /* Max no. of continuation lines used */
#define ixmax           10000   /* Handles lr table of up to 10,000 rows */


typedef short ixix;


/* Local variables for lrTables: */
struct LOC_lrTables {
  int lrix[ixmax + 1];
} ;

Local void ReadIndex(struct LOC_lrTables *LINK)
{
  ixix i, FORLIM;

  FORLIM = lrmax;
  for (i = 0; i <= FORLIM; i++) {
    scanf("%d%*[^\n]", &LINK->lrix[i]);
    getchar();
  }
}

/* Local variables for SendData: */
struct LOC_SendData {
  struct LOC_lrTables *LINK;
  int c, n, k, s, e;
} ;

Local void inputentry(struct LOC_SendData *LINK)
{
  LINK->e = 0;
  scanf("%d%d%d%d", &LINK->c, &LINK->n, &LINK->k, &LINK->s);
  if (LINK->k == 5) {
    scanf("%*[^\n]");
    getchar();
  } else {
    scanf("%d%*[^\n]", &LINK->e);
    getchar();
  }
  LINK->c = LINK->LINK->lrix[LINK->c];
  LINK->n = LINK->LINK->lrix[LINK->n];
  if (LINK->k == 5)
    LINK->s = LINK->LINK->lrix[LINK->s];
}

Local void outputentry(struct LOC_SendData *LINK)
{
  fprintf(parstab, "%4d,%4d,%d,%4d", LINK->c, LINK->n, LINK->k, LINK->s);
  if (LINK->k != 5)
    fprintf(parstab, ",%3d", LINK->e);
  else
    fprintf(parstab, "    ");
}

Local void SendData(int start, int stop, struct LOC_lrTables *LINK)
{
  struct LOC_SendData V;
  int i;

  /*
      write( parstab , '      DATA ((LRD(J,I),I=1,5),J=' );
      write( parstab , start:3, ',', stop:3, ') /');
   */
  V.LINK = LINK;
  fprintf(parstab, "      DATA (LRD(J),J=");
  if (stop < lrmax)
    i = LINK->lrix[stop+1] - 1;
  else
    i = lrlin - 1;
  fprintf(parstab, "%3d,%3d) /", LINK->lrix[start], i);
  inputentry(&V);
  outputentry(&V);

  for (i = start + 1; i <= stop; i++) {
    putc(',', parstab);
    if ((i - start) % 3 == 1)
      fprintf(parstab, "\n     * ");
/* p2c: PGf.p, line 683:
 * Note: Using % for possibly-negative arguments [317] */
    inputentry(&V);
    outputentry(&V);
  }

  fprintf(parstab, " /\n");
}


Static void lrTables(void)
{
  struct LOC_lrTables V;
  int max = MaxContin * 3 + 1;
  int n, NN;

  ReadIndex(&V);
  if (parstab != NULL)
    parstab = freopen("parstab", "w", parstab);
  else
    parstab = fopen("parstab", "w");
  if (parstab == NULL)
    _EscIO2(FileNotFound, "parstab");
  NN = (lrmax + 1) / max;
  for (n = 1; n <= NN; n++)
    SendData(max * (n - 1), max * n - 1, &V);
  if ((lrmax + 1) % max != 0) {
/* p2c: PGf.p, line 700:
 * Note: Using % for possibly-negative arguments [317] */
    SendData(NN * max, lrmax, &V);
  }
}  /* lrTables */

#undef MaxContin
#undef ixmax


main(int argc, Char *argv[])
{
  PASCAL_MAIN(argc, argv);
  parstab = NULL;
  lextab = NULL;
  parserr = NULL;
  parscst = NULL;
  lexedit = NULL;
  parsedit = NULL;
  parscdc = NULL;
  instfile = NULL;
  prodnames();
  tableconstants();
  parsererrors();
  lxTables();
  /*X ; retcode(0) X*/
  lrTables();
  if (instfile != NULL)
    fclose(instfile);
  if (parscdc != NULL)
    fclose(parscdc);
  if (parsedit != NULL)
    fclose(parsedit);
  if (lexedit != NULL)
    fclose(lexedit);
  if (parscst != NULL)
    fclose(parscst);
  if (parserr != NULL)
    fclose(parserr);
  if (lextab != NULL)
    fclose(lextab);
  if (parstab != NULL)
    fclose(parstab);
  exit(EXIT_SUCCESS);
}  /* pgf */



/* End. */
