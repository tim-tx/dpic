/* Output from p2c 1.21alpha-07.Dec.93, the Pascal-to-C translator */
/* From input file "PGp.p" */


/* program pgp(input,output); */

#include "p2c.h"


/* Converts output tables from pg to a form usable in the Pascal
   version of the skeleton compiler */

/* UXM 127 MXU */
/* B 255 B */

#define ORDMAXCH        255
#define namelen         ORDMAXCH


typedef Char namestring[namelen];

/*B  namestring = string(namelen); B*/
/*X  namestring = string(namelen);
     accessname = packed array [1..namelen] of char; X*/


Static FILE *instfile, *parscdc, *parscst, *parserr, *lextable, *lexedit,
	    *parstabl;
Static int lrmax, lxmax;
Static Char symbol[ORDMAXCH];


/*X function access( var f:char; n:integer ): integer; external; X*/
/*H name 'access' H*/
/*G; asmname '_p_Access'G */
extern int access(Char *f, int n);


Static void prodnames(void)
{
  /* Writes onto parscdc the const definitions of the nonterminals
     for use in a case statement of the semantic action routine.
     Input is in the form:   no. of alternate productions    nonterminal
                                     .                           .
                                     .                           .
                                     0                                  */
  int nalts, length, incrlen;
  int lineposition = 0, prodno = -1;
  int alternative, i;
  Char blank;

  if (parscdc != NULL)
    parscdc = freopen("parscdc", "w", parscdc);
  else
    parscdc = fopen("parscdc", "w");
  if (parscdc == NULL)
    _EscIO2(FileNotFound, "parscdc");
  scanf("%d", &nalts);
  while (nalts != 0) {
    blank = getchar();
    if (blank == '\n')
      blank = ' ';
    length = 0;
    do {
      length++;
      symbol[length-1] = getchar();
      if (symbol[length-1] == '\n')
	symbol[length-1] = ' ';
    } while (!P_eoln(stdin));
    scanf("%*[^\n]");
    getchar();
    while (symbol[length-1] == ' ')
      length--;
    for (alternative = 1; alternative <= nalts; alternative++) {
      prodno++;
      if (prodno < 10)
	incrlen = length + 5;
      else
	incrlen = length + 6;
      if (lineposition + incrlen > 72) {
	putc('\n', parscdc);
	lineposition = 0;
      }
      for (i = 0; i < length; i++)
	putc(symbol[i], parscdc);
      fprintf(parscdc, "%d=%d; ", alternative, prodno);
      lineposition += incrlen;
    }
    scanf("%d", &nalts);
  }
  putc('\n', parscdc);
  scanf("%*[^\n]");
  getchar();
}  /* prodnames */


Static void tableconstants(void)
{
  /*  Write onto parscst the four const definitions for
               symbmax= no. of terminals
               prodmax= no. of productions
               lrmax= length of lalr table
               lrlin= linear length of lalr table (not used)
               lxmax= length of lexical table not counting
                      initial entry table          */
  int symbmax, prodmax, lrlin;

  if (parscst != NULL)
    parscst = freopen("parscst", "w", parscst);
  else
    parscst = fopen("parscst", "w");
  if (parscst == NULL)
    _EscIO2(FileNotFound, "parscst");
  scanf("%d%*[^\n]", &symbmax);
  getchar();
  fprintf(parscst, "   symbmax=%d;\n", symbmax);
  scanf("%d%*[^\n]", &prodmax);
  getchar();
  fprintf(parscst, "   prodmax=%d;\n", prodmax);
  scanf("%d%*[^\n]", &lrmax);
  getchar();
  fprintf(parscst, "   lrmax  =%d;\n", lrmax);
  scanf("%d%*[^\n]", &lrlin);
  getchar();
  scanf("%d%*[^\n]", &lxmax);
  getchar();
  /*; writeln(parscst,'   lxmax  =',lxmax:1,';') */
}  /* tableconstants */


Static void Wlx(void)
{
  fprintf(parscst, "   lxmax  =%d;\n", lxmax);
}  /* Wlx */


Static void parsererrors(void)
{
  /* Writes onto parserr the pascal message statements corresponding
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
  int errorno, elements, elementno, i5, i, j, k, FORLIM1;

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

    /*  The following relies on terminals being 10 chars or less to make
        up a message of exactly the right length.  It should be modified
        to handle longer terminals */

    if (elements < 6)
      fprintf(parserr, "%4d: m('", errorno);
    else
      fprintf(parserr, "%4d: begin m('", errorno);
    i5 = -1;
    for (elementno = 1; elementno <= elements; elementno++) {
      if (i5 == 0) {
	fprintf(parserr, "');\n");
	fprintf(parserr, "      m('");
      } else if (i5 > 0)
	putc(' ', parserr);
      else
	i5 = 0;
      j = 0;
      k = 0;
      do {
	j++;
	symbol[j-1] = getchar();
	if (symbol[j-1] == '\n')
	  symbol[j-1] = ' ';
	if (symbol[j-1] == '\'') {
	  symbol[j] = symbol[j-1];
	  j++;
	  k++;
	}
      } while (!P_eoln(stdin));
      scanf("%*[^\n]");
      getchar();
      i = 0;
      while (j > i) {
	if (symbol[j-1] == ' ')
	  j--;
	else
	  i = j;
      }
/* p2c: PGp.p, line 143: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
      fprintf(parserr, "%.*s", j, symbol);
      FORLIM1 = 9 - (j - k + 9) % 10;
      for (i = 1; i <= FORLIM1; i++)
	putc(' ', parserr);
/* p2c: PGp.p, line 144:
 * Note: Using % for possibly-negative arguments [317] */
      i5 = (i5 + 1) % 5;
/* p2c: PGp.p, line 146:
 * Note: Using % for possibly-negative arguments [317] */
    }
    if (i5 > 0) {
      for (j = i5 + 1; j <= 5; j++)
	fprintf(parserr, "           ");
    }
    if (elements > 5)
      fprintf(parserr, "');  end;\n");
    else
      fprintf(parserr, "');\n");
    scanf("%d", &errorno);
  }
  scanf("%*[^\n]");
  getchar();
}  /* parsererrors */


typedef int lxtable[ORDMAXCH + 1];

typedef struct lxelm {
  int np, hp, tv;
  Char ch;
  struct lxelm *hpp, *npp, *prev;
} lxelm;


/* Local variables for lxTables: */
struct LOC_lxTables {
  lxtable hp, tv;
  lxelm *hp0[ORDMAXCH + 1];
  Char cc;
} ;

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

Local void wltr(lxelm *lx)
{
  lxelm *WITH;

  while (lx != NULL) {
    WITH = lx;
    fprintf(lextable, " %c%d %d %d\n", lx->ch, lx->np, lx->hp, lx->tv);
    wltr(WITH->npp);
    lx = WITH->hpp;
  }
}  /* wltr */

Local void tpurge(lxelm **lx)
{
  if (*lx == NULL)
    return;
  tpurge(&(*lx)->hpp);
  tpurge(&(*lx)->npp);
  Free(*lx);
}  /* tpurge */

Local void Sltable(struct LOC_lxTables *LINK)
{
  int j, k;

  for (k = 0; k <= ORDMAXCH; k++) {
    if (LINK->tv[k] != 0 || LINK->hp0[k] != NULL) {
      fprintf(lextable, " %c%d %d %d\n", k, 0, LINK->hp[k], LINK->tv[k]);
      j = k;
    }
  }
  fprintf(lextable, " %c%d %d %d\n", j, 0, 0, 0);
  fprintf(lextable, "  %d %d %d\n", 0, 0, 0);
  for (k = 0; k <= ORDMAXCH; k++)
    wltr(LINK->hp0[k]);
  for (k = 0; k <= ORDMAXCH; k++)
    tpurge(&LINK->hp0[k]);
}  /* Sltable */

/* Local variables for Sendlexedit: */
struct LOC_Sendlexedit {
  struct LOC_lxTables *LINK;
  Char namebuffer[100];
} ;

/*BX    namebuffer: string(100); XB*/

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
    "METAGOAL                                                                                            ",
    100);
      /*X namebuffer := str('METAGOAL'); X*/
  Sendeditstr(8, 0, &V);
  memcpy(V.namebuffer,
    "<EOF>                                                                                               ",
    100);
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
    printf("pgp: Terminal \"%.*s\" not found.\n", len, name);
/* p2c: PGp.p, line 327: Note:
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
  /*B args: string(100); lenargs: integer; ch: char;
     ftype,fm:namestring; B*/
  /*X args: string(100); lenargs: integer; ch: char;
     accname: accessname; X*/
  namestring namet;
  int j;

  /*BX
procedure tokn(var i:integer;
     lln:integer; line:string(100); var nam:namestring);
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

    else {
      printf("Cannot read file %.*s\n", V.i, namet);
/* p2c: PGp.p, line 469: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    }
  }

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
  if (lextable != NULL)
    lextable = freopen("lextable", "w", lextable);
  else
    lextable = fopen("lextable", "w");
  if (lextable == NULL)
    _EscIO2(FileNotFound, "lextable");
  Sltable(&V);
  Wlx();
}  /* lxTables */


Static void lrTables(void)
{
  Char ch;
  int i, FORLIM;

  FORLIM = lrmax;
  for (i = 0; i < FORLIM; i++) {   /* Skip over input data */
    scanf("%*[^\n]");
    getchar();
  }
  if (parstabl != NULL)
    parstabl = freopen("parstabl", "w", parstabl);
  else
    parstabl = fopen("parstabl", "w");
  if (parstabl == NULL)
    _EscIO2(FileNotFound, "parstabl");
  FORLIM = lrmax;
  for (i = 0; i <= FORLIM; i++) {
    scanf("%*[^\n]");
    getchar();
    do {
      ch = getchar();
      if (ch == '\n')
	ch = ' ';
      putc(ch, parstabl);
    } while (!P_eoln(stdin));
    putc('\n', parstabl);
  }
}  /* lrTables */


main(int argc, Char *argv[])
{
  PASCAL_MAIN(argc, argv);
  parstabl = NULL;
  lexedit = NULL;
  lextable = NULL;
  parserr = NULL;
  parscst = NULL;
  parscdc = NULL;
  instfile = NULL;
  prodnames();
  tableconstants();
  parsererrors();
  lxTables();
  lrTables();
  /*X retcode(0) X*/
  if (instfile != NULL)
    fclose(instfile);
  if (parscdc != NULL)
    fclose(parscdc);
  if (parscst != NULL)
    fclose(parscst);
  if (parserr != NULL)
    fclose(parserr);
  if (lextable != NULL)
    fclose(lextable);
  if (lexedit != NULL)
    fclose(lexedit);
  if (parstabl != NULL)
    fclose(parstabl);
  exit(EXIT_SUCCESS);
}  /* pgp */



/* End. */
