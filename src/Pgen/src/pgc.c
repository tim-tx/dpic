/* Output from p2c 1.21alpha-07.Dec.93, the Pascal-to-C translator */
/* From input file "PGc.p" */


/*X program pgc(input,output); X*/


#include "p2c.h"


/* Converts output tables from pg to a form usable in the C
   form of the skeleton compiler */

/* exit label for parser errors      */

/* UXM 127 MXU */
/* B 255 B */

#define ORDMAXCH        255
#define TERMLENGTH      20
#define SYMBOLLENGTH    30
#define MAXPRODS        2000


typedef Char argstr[80];
typedef Char namestring[SYMBOLLENGTH + 1];
typedef Char toknstring[SYMBOLLENGTH];


Static argstr arg;
Static FILE *instfile, *parscdc, *parsedit, *parstab;
Static int lrmax, lrlin, lxmax, prodcnt;
Static namestring smb[MAXPRODS + 1];
Static boolean rmvs[MAXPRODS + 1];


/*GH procedure system( s: cstring );
   external HG*/
/*H name 'system';H*/
/*G; asmname 'system';G*/

/*X function access( var f:char; n:integer ): integer; external; X*/
/*H name 'access' H*/
/*G; asmname '_p_Access'G */
extern int access(Char *f, int n);


Static void fatal(void)
{
  printf(" pgc error: end of file on input\n");

  exit(EXIT_SUCCESS);

}


Static int numlen(int k)
{
  int i = 1;

  while (k > 9) {
    i++;
    k /= 10;
  }
  return i;
}


Static void prodnames(void)
{
  /* Writes onto parscdc the const definitions of the nonterminals
     for use in a case statement of the semantic action routine.
     Input is in the form:   no. of alternate productions    nonterminal
                                     .                           .
                                     .                           .
                                     0                                  */
  int nalts, toklen, prodrev, alternative, i, j, k;

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
  if (P_eof(stdin))
    fatal();
  else
    scanf("%d", &nalts);

  for (i = 0; i <= MAXPRODS; i++)
    rmvs[i] = false;

  prodcnt = 0;
  while (nalts != 0) {
    toklen = -1;
    do {
      toklen++;
      smb[prodcnt][toklen] = getchar();
      if (smb[prodcnt][toklen] == '\n')
	smb[prodcnt][toklen] = ' ';
    } while (!(P_eoln(stdin) || toklen == SYMBOLLENGTH));
    while (smb[prodcnt][toklen] == ' ')
      toklen--;
    if (P_eof(stdin))
      fatal();
    else {
      scanf("%*[^\n]");
      getchar();
    }
    for (alternative = 1; alternative <= nalts; alternative++) {
      k = numlen(alternative);
      j = alternative;
      for (i = 1; i <= k; i++) {
	if (toklen + k <= SYMBOLLENGTH) {
	  smb[prodcnt][toklen + k - i + 1] = j % 10 + '0';
/* p2c: PGc.p, line 87:
 * Note: Using % for possibly-negative arguments [317] */
	}
	j /= 10;
      }
      smb[prodcnt][0] = toklen + k;
      for (i = 1; i <= toklen + k; i++)
	putc(smb[prodcnt][i], parscdc);
      fprintf(parscdc, "=%d;\n", prodcnt);
      for (i = 1; i <= toklen; i++)
	smb[prodcnt+1][i] = smb[prodcnt][i];
      prodcnt++;
    }
    prodrev = prodcnt;
    for (alternative = nalts; alternative >= 1; alternative--) {
      prodrev--;
      /*BV     write(parsedit,'C /'); VB*/
      fprintf(parsedit, "s/");
      for (i = 1; i <= toklen; i++)
	putc(smb[prodrev][i], parsedit);
      /*B      writeln(parsedit, alternative:1,'/',prodrev:1,'/ * *'); B*/
      /*V      writeln(parsedit, alternative:1,'/',prodrev:1,'/whole'); V*/
      fprintf(parsedit, "%d/%d/g\n", alternative, prodrev);
    }
    if (P_eof(stdin))
      fatal();
    else
      scanf("%d", &nalts);
  }
  prodcnt--;
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%*[^\n]");
    getchar();
  }
}  /* prodnames */


Static int strlen_(Char *s)
{
  int i = SYMBOLLENGTH, j = 0;

  while (i > j) {
    if (s[i-1] == ' ')
      i--;
    else
      j = i;
  }
  return i;
}


/*BX
procedure tokn(var i:integer;
   lln:integer; line:string(100); var nam:toknstring);
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

Static boolean streq(Char *s, Char *t)
{
  int i, j;

  if (s[0] == 0)
    return false;
  else {
    j = s[0] + 1;
    i = 0;
    while (i < j) {
      if (s[i] != t[i])
	j = i;
      else
	i++;
    }
    return (i > s[0]);
  }
}


Static void chsign(Char *s)
{
  int i = 0;
  int j, FORLIM;

  j = prodcnt + 1;
  while (i < j) {
    if (streq(s, smb[i]))
      j = i;
    else
      i++;
  }
  if (i <= prodcnt) {
    rmvs[i] = true;
    return;
  }
  printf("pgc: Production \"");
  FORLIM = s[0];
  for (i = 1; i <= FORLIM; i++)
    putchar(s[i]);
  printf("\" not found\n");
}


Static void Nullprod(void)
{
  /*B args: string(100); lenargs: integer; ch: char;
     ftype,fm:toknstring; B*/
  /*X args: string(100); lenargs: integer; ch: char;
     accname: accessname; X*/
  toknstring namet;
  namestring remet;
  int i, j;

  /*B
     args := parms||str(' '); lenargs := strlen(args); i := 1;
     if lenargs > 1 then begin
        tokn(i,lenargs,args,namet);
        if (namet[1] = '-') and
           ((namet[2] = 'd') or (namet[2] = 'D')) then
           repeat
              tokn(i,lenargs,args,namet);
              if i <= lenargs then chsign(remet)
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
                 i := strlen(namet); j := 0;
                 while i > j do if namet[i]=' ' then i:=i-1 else j:=i;
                 readln(instfile);
                 chsign(remet)
                 end
              end
           end; B*/
  /*X
     args := parms||str(' '); lenargs := strlen(args); i := 1;
     if lenargs > 1 then begin
        tokn(i,lenargs,args,namet);
        if (namet[1] = '-') and (namet[2] = 'd') then
           repeat
              tokn(i,lenargs,args,namet);
              if i <= lenargs then chsign(remet)
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
                    i := strlen(namet); j := 0;
                    while i > j do if namet[i]=' ' then i:=i-1 else j:=i;
                    readln(instfile);
                    chsign(remet)
                    end
                 end
              else writeln(output,'Cannot read file ',namet:i)
              end
           end; X*/

  if (P_argc <= 2)
    return;
  P_sun_argv(namet, sizeof(toknstring), 1);
  if (namet[0] == '-' && namet[1] == 'd') {
    for (j = 2; j < P_argc; j++) {
      P_sun_argv(namet, sizeof(toknstring), j);
      chsign(remet);
    }
    return;
  }
  if (namet[0] != '-' || namet[1] != 'f')
    return;
  P_sun_argv(namet, sizeof(toknstring), 2);
  namet[strlen_(namet)] = '\0';
  if (access(namet, 4) != 0) {
    printf("Cannot read file %.*s\n", i, namet);
/* p2c: PGc.p, line 273: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    return;
  }

  /*GH
     if ParamCount > 1 then begin
        namet := ParamStr(1);
        if (namet[1] = '-') and (namet[2] = 'd') then
           for j := 2 to ParamCount do begin
              namet := ParamStr(j);
              chsign(remet)
              end
        else if (namet[1] = '-') and (namet[2] = 'f') then begin
           namet := ParamStr(2);
           namet[1+strlen(namet)] := chr(0); HG*/
  /*G if 4-Access(namet,4) = 0 then begin G*/
  /*H if access(namet,4) = 0 then begin H*/

  if (instfile != NULL)
    instfile = freopen(P_trimname(namet, sizeof(toknstring)), "r", instfile);
  else
    instfile = fopen(P_trimname(namet, sizeof(toknstring)), "r");
  if (instfile == NULL) {

    _EscIO2(FileNotFound, P_trimname(namet, sizeof(toknstring)));
  }
  while (!P_eof(instfile)) {
    i = 0;
    while (!P_eoln(instfile)) {
      if (i < SYMBOLLENGTH)
	i++;
      remet[i] = getc(instfile);
      if (remet[i] == '\n')
	remet[i] = ' ';
    }
    remet[0] = i;
    fscanf(instfile, "%*[^\n]");
    getc(instfile);
    chsign(remet);
  }
}  /* Nullprod */


Static void tableconstants(void)
{
  /*  Write onto parscst the four const definitions for
               symbmax= no. of terminals
               prodmax= no. of productions
               lrmax= length of lalr table
               lrlin= linear length of lalr table
               lxmax= length of lexical table not counting
                      initial entry table          */
  int symbmax, prodmax;

  if (P_eof(stdin))
    fatal();
  else {
    scanf("%d%*[^\n]", &symbmax);
    getchar();
  }
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%d%*[^\n]", &prodmax);
    getchar();
  }
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%d%*[^\n]", &lrmax);
    getchar();
  }
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%d%*[^\n]", &lrlin);
    getchar();
  }
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%d%*[^\n]", &lxmax);
    getchar();
  }
}  /* tableconstants */


#define ESYMMAX         1000
#define EARRMAX         1000


typedef Char esymbol[TERMLENGTH];


/* Local variables for parsererrors: */
struct LOC_parsererrors {
  esymbol esymbs[ESYMMAX + 1];   /* unique symbols */
} ;

Local int match(int *esymbolp, struct LOC_parsererrors *LINK)
{
  int Result;
  int i = 1;

  while (strncmp(LINK->esymbs[i], LINK->esymbs[*esymbolp], sizeof(esymbol)))
    i++;
  Result = i;
  if (i != *esymbolp)
    (*esymbolp)--;
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
  int i, j, k, errorno, nterminals, termno;
  int plen = 0, errorcount = 0, esymbolp = 0;
  int eptrs[EARRMAX];   /* ptrs to symbols */

  if (P_eof(stdin))
    fatal();
  else
    scanf("%d", &errorno);
  while (errorno != 0) {
    if (P_eof(stdin))
      fatal();
    else {
      scanf("%d%*[^\n]", &nterminals);
      getchar();
    }
    plen++;
    eptrs[plen-1] = nterminals;
    errorcount = errorno;
    for (termno = 1; termno <= nterminals; termno++) {
      esymbolp++;
      i = 0;
      while (!P_eoln(stdin) && i < TERMLENGTH) {
	i++;
	if (P_eof(stdin))
	  fatal();
	else {
	  V.esymbs[esymbolp][i-1] = getchar();
	  if (V.esymbs[esymbolp][i-1] == '\n')
	    V.esymbs[esymbolp][i-1] = ' ';
	}
      }
      if (P_eof(stdin))
	fatal();
      else {
	scanf("%*[^\n]");
	getchar();
      }
      for (j = i; j < TERMLENGTH; j++)
	V.esymbs[esymbolp][j] = ' ';
      plen++;
      eptrs[plen-1] = match(&esymbolp, &V);
    }
    if (P_eof(stdin))
      fatal();
    else
      scanf("%d", &errorno);
  }
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%*[^\n]");
    getchar();
  }
  i = 0;
  while (i < esymbolp) {
    j = i + 5;
    if (j > esymbolp)
      j = esymbolp;
    for (k = i + 1; k <= j; k++) ;  /* incomplete write out */
    i = j;
  }
  i = 0;
  nterminals = errorcount + plen;
  plen = 1;
  while (i < nterminals) {  /* incomplete write out */
    j = i + 60;
    if (j > nterminals)
      j = nterminals;
    for (termno = i + 1; termno <= j; termno++) {
      if (termno <= errorcount)
	plen += eptrs[plen-1] + 1;
    }
    i = j;
  }
}  /* parsererrors */

#undef ESYMMAX
#undef EARRMAX


typedef int lxtable[ORDMAXCH + 1];


typedef struct lxelm {
  int np, hp, tv;
  Char ch;
  struct lxelm *next;
} lxelm;


/* Local variables for lxTables: */
struct LOC_lxTables {
  lxtable hp, tv;
} ;

Local void Readlx(struct LOC_lxTables *LINK)
{
  Char cc, ch1;
  int i;

  for (i = 0; i <= ORDMAXCH; i++) {
    LINK->hp[i] = 0;
    LINK->tv[i] = 0;
  }
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%c%c%d", &cc, &cc, &i);
    if (cc == '\n')
      cc = ' ';
    if (cc == '\n')
      cc = ' ';
  }
  do {
    if (P_eof(stdin))
      fatal();
    else {
      scanf("%d%d%*[^\n]", &LINK->hp[cc], &LINK->tv[cc]);
      getchar();
    }
    ch1 = cc;
    if (P_eof(stdin))
      fatal();
    else {
      scanf("%c%c%d", &cc, &cc, &i);
      if (cc == '\n')
	cc = ' ';
      if (cc == '\n')
	cc = ' ';
    }
  } while (cc != ch1);
  if (P_eof(stdin))
    fatal();
  else {
    scanf("%*[^\n]");
    getchar();
  }
}

Local void Sltable(struct LOC_lxTables *LINK)
{
  Char cc;
  int i;
  lxelm *lx, *lx0;
  int FORLIM;

  lx0 = Malloc(sizeof(lxelm));
  lx = lx0;
  FORLIM = lxmax;
  for (i = 0; i <= FORLIM; i++) {
    if (P_eof(stdin))
      fatal();
    else {
      scanf("%c%c%d%d%d%*[^\n]", &cc, &lx->ch, &lx->np, &lx->hp, &lx->tv);
      getchar();
      if (cc == '\n')
	cc = ' ';
      if (lx->ch == '\n')
	lx->ch = ' ';
    }
    lx->next = Malloc(sizeof(lxelm));
    lx = lx->next;
  }
}


Static void lxTables(void)
{
  struct LOC_lxTables V;

  Readlx(&V);
  Sltable(&V);
}  /* lxTables */


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
    if (P_eof(stdin))
      fatal();
    else {
      scanf("%d%*[^\n]", &LINK->lrix[i]);
      getchar();
    }
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
  if (P_eof(stdin))
    fatal();
  else
    scanf("%d%d%d%d", &LINK->c, &LINK->n, &LINK->k, &LINK->s);
  if (LINK->k == 5) {
    if (P_eof(stdin))
      fatal();
    else {
      scanf("%*[^\n]");
      getchar();
    }
  } else if (P_eof(stdin))
    fatal();
  else {
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
  if (LINK->k == 5) {
    fprintf(parstab, "    ");
    return;
  }
  if ((LINK->k == 3 || LINK->k == 6) && rmvs[LINK->e])
    fprintf(parstab, ",%3d", -LINK->e);
  else
    fprintf(parstab, ",%3d", LINK->e);
}

Local void SendData(int stop, struct LOC_lrTables *LINK)
{
  struct LOC_SendData V;
  int i;
  int j = 0;

  V.LINK = LINK;
  fprintf(parstab, "/*    0*/");
  for (i = 0; i <= stop; i++) {
    inputentry(&V);
    outputentry(&V);
    j++;
    if (i == stop)
      putc('\n', parstab);
    else if (j == 3) {
      j = 0;
      fprintf(parstab, ",\n");
      fprintf(parstab, "/*%5d*/", LINK->lrix[i+1]);
    } else
      putc(',', parstab);
  }

  putc('\n', parstab);
}


Static void lrTables(void)
{
  struct LOC_lrTables V;

  ReadIndex(&V);
  if (parstab != NULL)
    parstab = freopen("parstab", "w", parstab);
  else
    parstab = fopen("parstab", "w");
  if (parstab == NULL)
    _EscIO2(FileNotFound, "parstab");
  SendData(lrmax, &V);
}  /* lrTables */

#undef ixmax



/*X procedure system(var s: char); external; X*/

main(int argc, Char *argv[])
{
  PASCAL_MAIN(argc, argv);
  parstab = NULL;
  parsedit = NULL;
  parscdc = NULL;
  instfile = NULL;
  prodnames();
  Nullprod();
  tableconstants();
  parsererrors();
  lxTables();
  lrTables();
  /*              lazy way of creating only parstab */
  memcpy(arg,
    "rm -f parsedit parscdc                                                          ",
    sizeof(argstr));
  system(arg);
  /*GH system('rm -f parsedit parscdc'); HG*/
  /*X
      arg := str('rm -f parsedit parscdc')||chr(0);
      system(arg[1]);
      retcode(0)
   X*/

  if (instfile != NULL)
    fclose(instfile);
  if (parscdc != NULL)
    fclose(parscdc);
  if (parsedit != NULL)
    fclose(parsedit);
  if (parstab != NULL)
    fclose(parstab);
  exit(EXIT_SUCCESS);
}  /* pgc */



/* End. */
