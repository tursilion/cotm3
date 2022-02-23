/* Creature of the Maze III */

#include <stdio.h>
#include <stdlib.h>

void fail(char *);
void main(int, char*[]);
void readdat(void);
void readplayer(int);
int threetoi(char[]);
void newuser(void);
void maint(void);
FILE *pathopen(char*,char*);
void saveplayer(int);
void itothree(char [],int);
void showplayer(void);
void wait2(void);
char *fgets2(char *,int, FILE *);
int blank(char []);
void game(void);
void describe(void);
void loadlev(int);
int show1(char, char*);
void showspells(void);
void showposs(void);
void greet(void);
void newpos(int,int);
int fight(int);
void addobject(int);
void store(void);
void boss(void);
void trapdoor(void);
void stairsup(void);
void stairsdown(void);
void elevator(void);
void goodtemp(void);
void eviltemp(void);
void slip(void);
void dead(char *);
void savelev(int);
void goodtemp2(void);
void eviltemp2(void);
void hurt(int,char *);
void learnspell(void);
void addgood(int);
int sgn(int);
void domagic(void);
int limit(int);
void enemyhurt(int, int);
void doitem(void);
void getwhere(int*,int*);
void showfriends(void);
void clrfriends(void);
int passok(int);
int rnd(int);
int fightmenu(int);
int fight2(int,int);
void eattack(int);
void fixenemylist(void);
int cinstr(char,char[]);
void fixfriendlist(void);
void addcurse(char*);
void checkmonster(int,int);
void docandle(void);
void checkplayer(void);
void readnews(void);
void addnews(char *);

int magic, skill, strength;
int px,py,level,bossflag;
char spells[21][81];
int good, armour, numposs;
char weaponname[81];
int weaponstrength;
char playername[81];
int cursed;
char possessions[11][81];

char spellsdat[101][81];
char monstersdat[101][81];
char possessionsdat[102][81];
char hintsdat[101][81];
char locationsdat[51][81];

int playernum;

char opname[81];
char cname[81], cdesc[6][81];
int csk,cst,car;
int nth,sth,est,wst;

struct myenemy{
char name[81];
int sk,st,ma,go,mg,ar,we,cu;
} enemy[11];

struct myfriend{
char name[81];
int st,sk,ma,go;
} friend[11];

char path[81];
char qw[81];
char maze[76][85];

FILE *fp;
int a,b,c,x,y,z,i,candle;
char *flag;

void fail(x) char *x;
{ printf("Failure - %s\n",x);
  exit(10);
}

void main(argc, argv) int argc; char *argv[];
{ printf("\n\nCREATURE OF THE MAZE ]I[ - by M. Brent\n");
  printf("\n(c)1995 - all rights reserved.\n");
  randomize();
  if ((argc!=2)&&(argc!=3))
  { printf("\nUsage: COTM3 username [datapath]\n");
    printf("   ex: COTM3 RANCID C:\\GAMES\\COTM\n");
    fail("incorrect argument count");
  }
  if (strcmp(argv[1],"-greet")==0) greet(); /* no return */
  if (strcmp(argv[1],"-maint")==0) maint(); /* no return */
  strcpy(playername,argv[1]);
  printf("\n\nSearching for player %s...",playername);
  if (argc==3) strcpy(path,argv[2]);
  else         strcpy(path,"");
  fp=pathopen("PLAYERS.DAT","r");
  playernum=-1; a=-1;
  do
  { if (strcmp(playername,qw)==0)
      playernum=a;
    a++;
    flag=fgets2(qw,80,fp);
  } while (flag);
  fclose(fp);
  qw[0]=0;
  if (playernum==-1)
    newuser();
  else
    playernum--;
  readplayer(playernum);
  readdat();
  showplayer();
  loadlev(level);
  for (i=0; i<10; i++)
    friend[i].name[0]=0;
  for (i=0; i<10; i++)
    enemy[i].name[0]=0;
  printf("\n\nOK!\n");
  bossflag=0;
  game();
  saveplayer(playernum);
}

void readdat()
{ /* read in the data files */
i=0;
fp=pathopen("spells.dat","r");
do
 flag=fgets2((char*)&spellsdat[i],80,fp);
while ((flag)&&(!blank((char*)&spellsdat[i++])));
fclose(fp);
i=0;
fp=pathopen("monsters.dat","r");
do
 flag=fgets2((char*)&monstersdat[i],80,fp);
while ((flag)&&(!blank((char*)&monstersdat[i++])));
fclose(fp);
i=0;
numposs=0;
fp=pathopen("possess.dat","r");
do
{ flag=fgets2((char*)&possessionsdat[i],80,fp);
  numposs++;
} while ((flag)&&(!blank((char*)&possessionsdat[i++])));
fclose(fp);
numposs--;
i=0;
fp=pathopen("hints.dat","r");
do
 flag=fgets2((char*)&hintsdat[i],80,fp);
while ((flag)&&(!blank((char*)&hintsdat[i++])));
fclose(fp);
fp=pathopen("creature.dat","r");
fgets2(cname,80,fp);
for (i=0; i<5; i++)
  fgets2((char*)&cdesc[i],80,fp);
fgets2(qw,80,fp);
cst=threetoi(&qw[0]);
cst=cst+1000*threetoi(&qw[9]);
csk=threetoi(&qw[3]);
car=threetoi(&qw[6]);
fclose(fp);
fp=pathopen("LOCATION.DAT","r");
i=0;
while ((fgets2(&locationsdat[i][0],80,fp))&&(!blank(&locationsdat[i++][0])));
fclose(fp);
}

void readplayer(z) int z;
{ /* read in the player's data file */
char fn[80],q2[80];

fn[0]='P';
itothree(&fn[1],z);
fn[4]=0;
strcat(fn,".DAT");
fp=pathopen(fn,"r");
fgets2(qw,80,fp);
if (strcmp(qw,"DEAD")==0)
{ fgets2(q2,80,fp);
  if (!blank(q2))
    printf("\nYou were killed by %s\n\n",q2);
  fclose(fp);
  newuser();
  fp=pathopen(fn,"r");
  fgets2(qw,80,fp);
}
magic=threetoi(&qw[0]);
skill=threetoi(&qw[3]);
strength=threetoi(&qw[6]);
if (qw[21]!=0)
  strength=strength+threetoi(&qw[21])*1000;
px=threetoi(&qw[9]);
py=threetoi(&qw[12]);
level=threetoi(&qw[15]);
candle=threetoi(&qw[18]);
for (i=0; i<20; i++)
  fgets2((char*)&spells[i],80,fp);
fgets2(qw,80,fp);
good=threetoi(&qw[0])-500;
armour=threetoi(&qw[3]);
weaponstrength=threetoi(&qw[6]);
cursed=threetoi(&qw[9]);
fgets2(weaponname,80,fp);
fgets2(playername,80,fp);
for (i=0; i<10; i++)
  fgets2((char*)&possessions[i],80,fp);
fclose(fp);
}

int threetoi(x) char x[];
{ /* convert 3 digits of string x to integer */
  return((x[0]-48)*100+(x[1]-48)*10+(x[2]-48));
}

void newuser()
{ /* set up a new user, set playernum and be prepared to continue loading
     the data */
printf("\nAh ha! A new adventurer! Do you think YOU can defeat the creature?\n");
printf("\nWell, anyway, let's set you up.\n");
if (strcmp(qw,"DEAD"))
{ playernum=a-1;
  fp=pathopen("players.dat","a");
  fprintf(fp,"%s\n",playername);
  fclose(fp);
}
printf("\nPlease enter a name for your character to use: ");
gets(playername);
magic=rnd(24)+1;
skill=rnd(24)+1;
strength=rnd(24)+1;
px=1; py=1;
level=0;
for (i=0; i<10; i++)
{ spells[i][0]=0;
  spells[i+10][0]=0;
  possessions[i][0]=0;
}
strcpy(&possessions[0][0],"005 Candles");
strcpy(&possessions[1][0],"005 Gold Coins");
good=0;
armour=0;
strcpy(weaponname,"your fists");
weaponstrength=1;
cursed=0;
candle=100;
saveplayer(playernum);
strcpy(qw,"--New player: ");
strcat(qw,playername);
addnews(qw);
}

void maint()
{ /* perform daily maintenance */
FILE *f2;
int zz;

printf("\nPerforming daily maintenance...\n");
/* let boss creature rest */
fp=pathopen("creature.dat","r");
fgets2(cname,80,fp);
for (i=0; i<5; i++)
  fgets2(&cdesc[i][0],80,fp);
fgets2(qw,80,fp);
fclose(fp);
a=threetoi(&qw[0]);
a=a+1000*threetoi(&qw[9]);
a=a+rnd(100);
if (a>29999) a=29999;
itothree(&qw[0],a%1000);
itothree(&qw[9],a/1000);
fp=pathopen("creature.dat","w");
fprintf(fp,"%s\n",cname);
for (i=0; i<5; i++)
  fprintf(fp,"%s\n",&cdesc[i][0]);
fprintf(fp,"%s\n",qw);
fclose(fp);
/* create locations.dat */
fp=pathopen("PLAYERS.DAT","r");
zz=0;
fgets2(qw,80,fp);  /* eat first blank line */
while ((fgets2(qw,80,fp))&&(!blank(qw)))
  zz++;
if (zz>50)
{ printf("\n\n* WARNING * Too many players! Max 50! List clipped!\n");
  zz=50;
}
fclose(fp);
f2=pathopen("LOCATION.DAT","w");
for (a=0; a<zz; a++)
{ strcpy(qw,"P");
  itothree(&qw[1],a);
  qw[4]=0;
  strcat(qw,".DAT");
  fp=pathopen(qw,"r");
  fgets2(qw,80,fp);
  fclose(fp);
  if (strcmp(qw,"DEAD"))
  { readplayer(a);
    itothree(&qw[0],px);
    itothree(&qw[3],py);
    itothree(&qw[6],level);
    qw[9]=0;
    playername[70]=0;  /* just in case it's long */
    strcat(qw,playername);
    fprintf(f2,"%s\n",qw);
  }
}
fclose(f2);
printf("\n** DONE **\n");
exit(0);
}

FILE *pathopen(x,y) char *x,*y;
{ /* open a file prepending the 'path' global */
char z[250];
FILE *q;

strcpy(z,path);
strcat(z,x);
q=fopen(z,y);
if (q==NULL) fail(z);
return(q);
}

void saveplayer(z) int z;
{ /* save the player's data file */
char fn[80];

fn[0]='P';
itothree(&fn[1],z);
fn[4]=0;
strcat(fn,".DAT");
fp=pathopen(fn,"w");
itothree(&qw[0],magic);
itothree(&qw[3],skill);
itothree(&qw[6],strength%1000);
itothree(&qw[9],px);
itothree(&qw[12],py);
itothree(&qw[15],level);
itothree(&qw[18],limit(candle));
itothree(&qw[21],strength/1000);
qw[24]=0;
fprintf(fp,"%s\n",qw);
for (i=0; i<20; i++)
  fprintf(fp,"%s\n",&spells[i]);
itothree(&qw[0],good+500);
itothree(&qw[3],armour);
itothree(&qw[6],weaponstrength);
itothree(&qw[9],cursed);
qw[12]=0;
fprintf(fp,"%s\n",qw);
fprintf(fp,"%s\n",weaponname);
fprintf(fp,"%s\n",playername);
for (i=0; i<10; i++)
  fprintf(fp,"%s\n",&possessions[i]);
fclose(fp);
}

void itothree(x,y) char x[]; int y;
{ /* convert an integer into 3 digit string */
x[0]=y/100+48;
y=y%100;
x[1]=y/10+48;
x[2]=y%10+48;
}

void showplayer()
{ /* show player stats */
printf("\n\nName: %s (",playername);
if (good/5==0) printf("neutral)\n");
if ((good>4)&&(good<15)) printf("good)\n");
if (good>14) printf("virtuous)\n");
if ((good<-4)&&(good>-15)) printf("evil)\n");
if (good<-14) printf("malevolent)\n");
printf("Level %d, (%d,%d)\n",level,px,py);
printf("Skill : %3d   Strength: %5d   Magic: %3d\n",skill, strength, magic);
printf("Armour: %3d      Curse: %5d   Candle: %3d%%\n",armour,cursed,candle);
printf("Weapon: %s - +%d\n",weaponname,weaponstrength);
showspells();
wait2();
if (cinstr(qw[0],"QqnN"))
  return;
showposs();
wait2();
if (cinstr(qw[0],"QqNn"))
  return;
showfriends();
wait2();
}

void wait2()
{
  printf("\nPress return to continue, or Q to quit...");
  gets(qw);
}

char *fgets2(x,y,z) char *x; int y; FILE *z;
{ /* get a string and strip any end junk like CR or LF */
char *a;
int i;

a=fgets(x,y,z);
i=0;
while (x[i]>31) i++;
x[i]=0;
return(a);
}

int blank(x) char x[];
{ /* return a 1 if the line contains no visible characters >32 */
int q,w;

q=0; w=1;
while (x[q])
  if (x[q++]>32)
    w=0;
return(w);
}

void game()
{ /* let's play the actual game now */
char key;

readnews();

key=0;

while (key!='Q')
{ learnspell();
  describe();
  printf("\n[N,S,E,W,I,M,U,R,P,X,Q,?,!]\n");

  gets(qw);
  if (strcmp(qw,"-greet")==0)
  { saveplayer(playernum);
    greet();
  }
  key=toupper(qw[0]);
  switch (key)
  { case '?': printf("\nCommands\n--------\n");
	      printf("(N)orth\n(S)outh\n(E)ast\n(W)est\n");
	      printf("(I)nventory/stats\n(M)agic\n(U)se item\n");
	      printf("(R)est\n(P)ray\ne(X)amine possession\n(Q)uit game\n");
	      printf("(?) This list\n(!) Instructions\n");
	      break;
    case '!': i=0;
	      fp=pathopen("HELP.TXT","r");
	      while ((fgets2(qw,80,fp))&&(i>-499))
	      { printf("%s\n",qw);
		i++;
		if (i==20)
		{ wait2();
		  i=0;
		  if (cinstr(qw[0],"QqNn")) i=-500;
		}
	      }
	      fclose(fp);
	      break;
    case 'N': if (passok(nth))
		newpos(px,py-1);
	      else
		printf("You cannot go that way.\n\n");
	      break;
    case 'S': if (passok(sth))
		newpos(px,py+1);
	      else
		printf("You cannot go that way.\n\n");
	      break;
    case 'E': if (passok(est))
		newpos(px+1,py);
	      else
		printf("You cannot go that way.\n\n");
	      break;
    case 'W': if (passok(wst))
		newpos(px-1,py);
	      else
		printf("You cannot go that way.\n\n");
	      break;
    case 'I': showplayer();
	      break;
    case 'M': domagic();
	      break;
    case 'U': doitem();
	      break;
    case 'R': printf("You rest...\n");
	      a=((level+1)*(rnd(40)+1))-strength;
	      if (a>0)
		strength=strength+rnd(a);
	      if (maze[py][px]=='l') goodtemp2();
	      if (maze[py][px]=='m') eviltemp2();
	      if (maze[py][px]=='a') checkmonster(px,py);
	      docandle();
	      break;
    case 'P': if (good/5==0)
		printf("\nYou cannot pray while you are neutral!\n\n");
	      else
	      { printf("\nYou pray for inspiration.\n");
		if ((cursed>0)&&(maze[py][px]!='l')&&(maze[py][px]!='m'))
		{ printf("You feel your curse reduced.\n");
		  cursed=cursed-rnd((level+1)*10);
		  if (cursed<0) cursed=0;
		  if (cursed==0) printf("Your curse is ended!\n\n");
		  addgood(sgn(good));
		}
		if (maze[py][px]=='l')
		{ if (good<4)
		    printf("Your prayers are not heard in this temple.\n");
		  else
		  { if (cursed>0)
		    { printf("Your curse is cured!\n\n");
		      cursed=0;
		    }
		    else
		    { printf("\nYou pray. You receive the following piece of divine inspiration:\n");
		      printf("%s\n",&hintsdat[rnd(100)][0]);
		    }
		    addgood(sgn(good)*2);
		  }
		}
		if (maze[py][px]=='m')
		{ if (good>4)
		    printf("Your prayers are not heard in this temple.\n");
		  else
		  { if (cursed>0)
		    { printf("Your curse is cured!\n\n");
		      cursed=0;
		    }
		    else
		    { printf("\nYou pray. You receive the following piece of divine inspiration:\n");
		      printf("%s\n",&hintsdat[rnd(100)][0]);
		    }
		    addgood(sgn(good)*2);
		  }
		}
	      }
	      if (maze[py][px]=='a') checkmonster(px,py);
	      docandle();
	      break;
    case 'X': printf("\nExamine which possession?\n");
	      showposs();
	      printf("\n(0-9):");
	      gets(qw);
	      x=qw[0]-48;
	      if ((x>=2)&&(x<=9))
	      { y=threetoi(possessions[x]);
		printf("\n\n%s -\n%s\n\n",&possessionsdat[y][0],&possessionsdat[y+1][0]);
	      }
	      if (x==1)
	      { printf("\n\nGold coins -\n");
		printf("Just money... used to buy items.\n\n");
	      }
	      if (x==0)
	      { printf("\n\nCandles -\n");
		printf("Used to light your way and keep nasty beings away. Run out,\n");
		printf("and you're dead!\n\n");
	      }
	      break;
    case 'Q': printf("QUIT - are you sure? (y/[N]):");
	      gets(qw);
	      if (toupper(qw[0])!='Y') key=0;
	      printf("\n\n");
	      break;
    default : printf("Invalid entry.\n\n");
  }
}}

void describe()
{ /* print a summary of the current location */
printf("\n%s, you are at (%d,%d) on level %d\n",playername,px,py,level);
if (maze[py][px]=='d')
  printf("You are standing on a bridge over a rushing underground river\n");
if (maze[py][px]=='l')
  printf("You are in a place of good worship\n");
if (maze[py][px]=='m')
  printf("You are in a place of evil worship\n");
if (maze[py][px]=='f')
{  if (strength<400)
   { /* must have smashed it */
     printf("You smash the boulder with your sledgehammer, providing easy access.\n");
     maze[py][px]='a';
     savelev(level);
   }
   else
     printf("You push the boulder out of your way, and let it fall back into place.\n");
}
if (maze[py][px]=='c')
  { printf("You are standing in a rushing torrential river...\n");
    if (rnd(5)==1)
      if (rnd(100)>strength/20)
      { slip();
	describe();
	return;
      }
  }
nth=show1(maze[py-1][px],"north");
sth=show1(maze[py+1][px],"south");
est=show1(maze[py][px+1],"east");
wst=show1(maze[py][px-1],"west");
}

void loadlev(x) int x;
{ /* load level x into MAZE */
char n[80];

strcpy(n,"LEVELx.DAT");
n[5]=x+48;
fp=pathopen(n,"r");
for (i=0; i<75; i++)
  fgets2(&maze[i][0],80,fp);
fclose(fp);
}

int show1(x,y) char x,*y;
{ /* do a description for 'x' on the map. Return 1 if passable */
  /* 'y' is a string with the direction name to show */
int p;

p=0;
switch (x)
{ case 'h': p=1;
	    if (rnd(200)<skill)
	    {  printf("You notice a trapdoor to the %s.\n",y);
	       break;
	    }
  case 'a': printf("The barren passage continues to the %s.\n",y);
	    p=1;
	    break;
  case 'c': if (strength<75)
	      printf("There is an impassable river to the %s\n",y);
	    else
	    { if (maze[py][px]=='c')
		printf("The river continues to the %s.\n",y);
	      else
	      { printf("There is a powerful river to the %s,\n",y);
		printf("But you can probably cross it.\n");
	      }
	      p=1;
	    }
	    break;
  case 'd': printf("You can cross the river on a bridge to the %s\n",y);
	    p=1;
	    break;
  case 'e': printf("There is a store to the %s\n",y);
	    p=1;
	    break;
  case 'f': if (strength<400)
	    { printf("The path to the %s is blocked by a large boulder.\n",y);
	      p=522;
	    }
	    else
	    { printf("There's a boulder to the %s, but you can push past it.\n",y);
	      p=1;
	    }
	    break;
  case 'g': printf("Something about the %s path seems odd, and makes the\n",y);
	    printf("hairs on the back of your neck stand up.\n");
	    p=1;
	    break;
  case 'i': printf("There are stairs leading up to the %s.\n",y);
	    p=1;
	    break;
  case 'j': printf("There are stairs leading down to the %s.\n",y);
	    p=1;
	    break;
  case 'k': printf("You can see an elevator to the %s.\n",y);
	    p=1;
	    break;
  case 'l': if (good/5==0)
	    { printf("There is a temple of good to the %s.\n",y);
	      p=1;
	    }
	    if (good<-4)
	    { printf("You feel incredible distaste, fear and hatred as you see a\n");
	      printf("temple of good to the %s.\n",y);
	    }
	    if (good<-14) p=1;
	    if (good>5)
	    { printf("You feel peace and happiness as you see a temple of\n");
	      printf("good to the %s.\n",y);
	      p=1;
	    }
	    break;
  case 'm': if (good/5==0)
	    { printf("There is a temple of evil to the %s\n",y);
	      p=1;
	    }
	    if (good<-4)
	    { printf("You feel smug satisfaction as you notice a temple of\n");
	      printf("evil to the %s.\n",y);
	      p=1;
	    }
	    if (good>4)
	    { printf("You feel loss of hope and powerful evil forces as you see\n");
	      printf("a temple of evil to the %s.\n",y);
	    }
	    if (good>14) p=1;
	    break;
  case 'n': printf("There is a green door to the %s.\n",y);
	    p=500;
	    break;
  case 'o': printf("There is a red door to the %s.\n",y);
	    p=502;
	    break;
  case 'p': printf("There is a blue door to the %s.\n",y);
	    p=504;
	    break;
  case 'q': printf("There is a door with a curious and nasty-looking skull\n");
	    printf("design on it to the %s.\n",y);
	    p=530;
	    break;
  }
return(p);
}

void showspells()
{ /* display the list of spells */
printf("SPELLS\n");
for (i=0; i<10; i++)
  printf("%d. %-40s - %3.3s\n",i,&spells[i][0],&spells[i+10][0]);
}

void showposs()
{ /* display the list of possessions */
printf("POSSESSIONS\n");
for (i=0; i<10; i++)
if (blank(&possessions[i][0]))
  printf("%d.\n",i);
else
  printf("%d. %.60s\n",i,i<2?&possessions[i][0]:&possessionsdat[threetoi(&possessions[i][0])][0]);
}

void greet()
{ printf("\nSpecial greets and a big >hi< to:\n");
  printf("\nThe Gatekeeper    - see? I put it into a program, just like I\n");
  printf("                    said I would.\n");
  printf("\nRancid Instigator - whom I always mention anyway. I guess we\n");
  printf("                    have to let Phantom live now...\n");
  printf("\nCaptain C         - how's it feel to have your own greet in\n");
  printf("                    what should be a real release?\n");
  printf("\nShimrod           - you probably won't see this... but what\n");
  printf("                    the heck? Got your fingers wet lately? :)\n");
  printf("\nSome won't see this, but, more 'hi's to: Flipper (not me), Aslan,\n");
  printf("Canis, Dolphin Kiss, Kira, Sgt Young (who I forced to playtest\n");
  printf("this on midnight shifts), Chronos, Samsung, Ciao Durhed (aka Phantom),\n");
  printf("The Fishman, Buteo, Cowlover, and any dolphin lovers around the\n");
  printf("world! Note: this program is authorized for free, unmodified use\n");
  printf("on any BBS or any computer system, EXCEPT pay-for-access ones,\n");
  printf("whom I request merely ask me first. If you use this software,\n");
  printf("send me your BBS number!!\n");
  exit(0);
}

void newpos(nx,ny) int nx,ny;
{ /* check the new position for objects, encounters, etc. Update px,py */
int zz,ox,oy;

zz=0; ox=px; oy=py;
px=nx; py=ny;
checkplayer();
switch (maze[ny][nx])
{ case 'e': store(); zz=1; break;
  case 'g': boss(); /* no return */
  case 'h': trapdoor(); zz=1; break;
  case 'i': stairsup(); zz=1; break;
  case 'j': stairsdown(); zz=1; break;
  case 'k': elevator(); zz=1; break;
  case 'l': goodtemp(); zz=1; break;
  case 'm': eviltemp(); zz=1; break;
}
if (zz==0)
  checkmonster(ox,oy);
a=rnd(numposs);
if ((a<numposs/2)&&(rnd(10)>=level))
{ /* find something? */
  a=a*2;
  strcpy(qw,&possessionsdat[a][0]);
  if (qw[0]>32)
  { printf("You found a %s!\n",qw);
    qw[0]='?';
    while (qw[0]=='?')
    { printf("[T]ake, (L)eave, (?) describe\n");
      gets(qw);
      if (blank(qw)) qw[0]='T';
      qw[0]=toupper(qw[0]);
      if (qw[0]=='?')
	printf("\n%s\n",&possessionsdat[a+1][0]);
      if ((qw[0]!='T')&&(qw[0]!='L')) qw[0]='?';
    }
    if (qw[0]=='T') addobject(a);
    else printf("With a hefty kick, you send the object bouncing down the hall,\nout of sight.\n");
  }
}
docandle();
}

int fight(num) int num;
{ /* potential fight with num enemies... */
/* return true if escaped. num is num enemy loaded. First enemy is leader */
if (abs(enemy[0].go-good)<rnd(150))
{ a=fightmenu(num);
  return(a);
}
switch (sgn(good))
{ case -1:printf("Suddenly, you hear 'Kill the evil one!\n"); break;
  case 0: printf("Suddenly, you hear 'Kill the wishy-washy one!\n"); break;
  case 1: printf("Suddenly, you hear 'Kill the goodie-goodie!\n"); break;
}
  a=fight2(0,num);
  return(a);
}

void addobject(num) int num;
{ /* add object type num to the player's inventory */
int z;

if (num==6)
{ z=threetoi(&possessions[0][0])+1;
  if (z>999) z=999;
  itothree(&possessions[0][0],z);
  printf("\nYou can *never* have too many candles!\n");
  printf("You now have %s\n",&possessions[0][0]);
  return;
}
if (num==8)
{ z=threetoi(&possessions[1][0])+rnd((level+1)*10)+1;
  if (z>999) z=999;
  itothree(&possessions[1][0],z);
  printf("\nGold! Wealth! Fame! Fortune! Babes!\n(if you live)\n");
  printf("You now have %s\n",&possessions[1][0]);
  return;
}
z=0;
while (possessionsdat[num+1][z])
  if (possessionsdat[num+1][z++]=='+')
  { y=threetoi(&possessionsdat[num+1][z]);
    if (y<weaponstrength)
    { printf("\n*Really* replace your +%d weapon with a +%d one? (y/[N]):",weaponstrength,y);
      gets(qw);
      if (toupper(qw[0])!='Y')
      { printf("\n\nYou toss the weaker, useless weapon away.\n");
	return;
      }
    }
    printf("\nYour attack strength is now at +%d\n",y);
    weaponstrength=y;
    strcpy(weaponname,&possessionsdat[num][0]);
    addcurse(weaponname);
    return;
  }
z=0;
while (possessionsdat[num+1][z])
  if (possessionsdat[num+1][z++]=='@')
  { y=threetoi(&possessionsdat[num+1][z]);
    if (y<armour)
    { printf("\n*Really* replace your @%d armour with @%d? (y/[N]):",armour,y);
      gets(qw);
      if (toupper(qw[0])!='Y')
      { printf("\n\nYou discard the weaker armour and it clatters away.\n");
	return;
      }
    }
    printf("\nYour armour is now @%d\n",y);
    armour=y;
    addcurse(&possessionsdat[num][0]);
    return;
  }
z=-1;
for (i=0; i<10; i++)
  if ((blank(&possessions[i][0]))&&(z==-1)) z=i;
if (z==-1)
{ printf("Your inventory is *full*\n\n");
  qw[0]='?';
  while (((qw[0]<50)||(qw[0]>57))&&(qw[0]!=0))
  { if (qw[0]=='?')
      showposs();
    printf("\nEnter the number of the object to drop, or press return to cancel.\n");
    printf(":");
    gets(qw);
    if (qw[0]=='0')
      printf("You can't drop your candles! You need the light to survive!\n");
    if (qw[0]=='1')
    { printf("You should hold on to your gold... it'll make a nice souvenir when\n");
      printf("you finally get out of here.\n");
    }
  }
  if ((qw[0]<50)||(qw[0]>57))
  { printf("You drop the object, deciding it's not as important as you thought.\n");
    printf("It bounces away, out of sight of the candlelight.\n\n");
  }
  else
  { z=qw[0]-48;
    printf("You drop the %s, and it bounces out of sight.\n",&possessionsdat[threetoi(&possessions[z][0])][0]);
  }
}
if (z!=-1)
{ printf("You take the %s.\n\n",&possessionsdat[num][0]);
  itothree(&possessions[z][0],num);
  possessions[z][3]=0;
  addcurse(&possessionsdat[num][0]);
}}

void store()
{ printf("\nYou enter the store. A small creature pops up and greets you.\n");
  printf("'Hello there, brave adventurer. Perhaps I may be able to sell you\n");
  printf("something?' [Y]/n:");
  gets(qw);
  if (blank(qw)) qw[0]='Y';
  if (toupper(qw[0])=='Y')
  { for (i=0; i<3; i++)
    { do
      { a=rnd(50)*2;
	if (blank(&possessionsdat[a][0])) a=-1;
	if (a==8) a=-1;  /* can't buy gold!! */
      } while (a==-1);
      printf("\nWould you like to buy a %s for %d? (y/[N]/?)\n",&possessionsdat[a][0],b=(rnd(100)+1)*(level+1));
      do
      { gets(qw);
	if (qw[0]=='?')
	{ printf("%s\n",&possessionsdat[a+1][0]);
	  printf("\nWould you like to buy the %s for %d? (y/[N]/?)\n",&possessionsdat[a][0],b);
	}
      } while (qw[0]=='?');
      if (toupper(qw[0])=='Y')
      { c=threetoi(&possessions[1][0]);
	if (c<b) printf("You don't have enough money! Forget it!\n");
	else { c=c-b;
	       printf("You bought it, buddy! No refunds!\n");
	       addobject(a);
	     }
	itothree(&possessions[1][0],c);
      }
    }
  }
  printf("\n'Come back anytime!' calls the man, and the store vanishes.\n\n");
}

void boss()
{ char c,fn[80];

  for (i=0; i<5; i++)
    printf("%s\n",cdesc[i]);
  clrfriends();
  printf("\nYou have found %s, the boss!!\nNow you might fight to the death!\n",cname);
  bossflag=1;
  strcpy(enemy[0].name,cname);
  enemy[0].sk=csk;
  enemy[0].st=cst;
  enemy[0].ma=20000;
  enemy[0].go=0;
  enemy[0].mg=0;
  enemy[0].ar=car;
  enemy[0].we=0;
  enemy[0].cu=0;
  strcpy(qw,playername);
  strcat(qw," battled the boss!");
  addnews(qw);
  fight2(0,1);
  addnews("...AND WON!!");
  printf("\n\n\nYou have defeated the Creature of the Maze!!\n");
  printf("\nYou have won the game!!\n\n");
  printf("You feel a stange compulsion come over you to stay here, and\n");
  printf("guard the maze in place of the creature. Now YOU are the\n");
  printf("Creature of the Maze.\n\n");
  printf("You may now enter a brief description of your entry. This will\n");
  printf("be displayed when a player enters your room. You have five lines\n\n");
  strcpy(cname,playername);
  cst=strength;
  csk=skill;
  car=armour;
  do
  { for (i=0; i<5; i++)
    { printf(">");
      gets(cdesc[i]);
    }
    printf("\n\nYou entered:\n");
    for (i=0; i<5; i++)
      printf("%s\n",cdesc[i]);
    printf("\nIs this okay? [Y]/n\n");
    gets(qw);
  } while (toupper(qw[0])=='N');
  printf("\n\nNow saving Creature stats!\n");
  fp=pathopen("CREATURE.DAT","w");
  fprintf(fp,"%s\n",cname);
  for (i=0; i<5; i++)
    fprintf(fp,"%s\n",cdesc[i]);
  itothree(&qw[0],cst%1000);
  itothree(&qw[3],csk);
  itothree(&qw[6],car);
  itothree(&qw[9],cst/1000);
  qw[12]=0;
  fprintf(fp,"%s\n",qw);
  fclose(fp);
  printf("Now killing off your old character...\n");
  strcpy(fn,"P");
  itothree(&fn[1],playernum);
  fn[4]=0;
  strcat(fn,".DAT");
  fp=pathopen(fn,"w");
  fprintf(fp,"DEAD\n");
  fclose(fp);
  printf("\n\nCongratulations, and thanks for playing!\n");
  exit(0);
}

void trapdoor()
{ /* fall down a level */
printf("\n*AHHH* You have just stepped on a hidden trapdoor and fallen down a level!\n");
level++;
loadlev(level);
if (rnd(10)>5)
  hurt(rnd(10*(level+1)),"trapdoor");
}

void stairsup()
{ printf("There is a flight of stairs leading up here. Take them? (y/[N])\n");
  gets(qw);
  if (toupper(qw[0])=='Y')
  { printf("\nYou climb the stairs...\n");
    clrfriends();
    level--;
    loadlev(level);
  }
}

void stairsdown()
{ printf("There is a flight of stairs leading down here. Take them? (y/[N])\n");
  gets(qw);
  if (toupper(qw[0])=='Y')
  { printf("\nYou descend the stairs...\n");
    clrfriends();
    level++;
    loadlev(level);
  }
}

void elevator()
{ printf("Before you, you see a shiny, new elevator. It can take you to any\n");
  printf("level of the maze. Do you want to use it? (y/[N])\n");
  gets(qw);
  if (toupper(qw[0])=='Y')
  { printf("You enter the elevator. Inside are buttons labelled from 0-9.\n");
    printf("Please enter the destination level from 0 to 9:");
    gets(qw);
    if ((qw[0]>'9')||(qw[0]<'0')) printf("You decide not to use the elevator.\n");
    else
    { clrfriends();
      printf("The elevator hums slightly, and then the doors open on level %d\n",qw[0]-48);
      level=qw[0]-48;
      loadlev(level);
    }
  }
}

void goodtemp()
{ if (good>4)
    printf("You enter the wondrous Temple of Good.\n");
  else
    if (good<-4)
      printf("You enter the accursed Temple of Good.\n");
    else printf("You enter the Temple of Good.\n");
  goodtemp2();
}

void goodtemp2()
{ if (good>4)
  { if (cursed>0)
    { cursed=cursed-rnd(10);
      printf("You feel your curse reduced...\n");
      if (cursed<0) cursed=0;
    }
  } else
  { if (good<-14)
    { printf("The mere presence of one so evil as yourself causes the Temple\n");
      printf("to shake, then crumble, before finally becoming nothing more than\n");
      printf("rubble, quickly fading into the drabness of the passages.\n\n");
      maze[py][px]='a';
      savelev(level);
    } else
      if (good<-4)
      { printf("You feel disgust, and a desire to smash something, but you\n");
	printf("dare not act against the Temple.\n");
      }
    if (rnd(500)*level>(-skill*good))
    { printf("\nYou feel some of the goodness creep into your being.\n");
      addgood(1);
    }
  }
}

void eviltemp()
{ if (good<-4)
    printf("You enter the dark, yet comforting, Temple of Evil.\n");
  else
    if (good>4)
      printf("You enter the blasphemous Temple of Evil.\n");
    else printf("You enter the Temple of Evil.\n");
  eviltemp2();
}

void eviltemp2()
{ if (good<-4)
  { if (cursed>0)
    { cursed=cursed-rnd(10);
      printf("You feel your curse reduced...\n");
      if (cursed<0) cursed=0;
    }
  } else
  { if (good>14)
    { printf("The mere presence of one so pure as yourself causes the evil\n");
      printf("temple to crumble and collapse to dust around you.\n");
      maze[py][px]='a';
      savelev(level);
    } else
      if (good>4)
      { printf("You wish you could see the end of this evil place, but you\n");
	printf("dare not act against the powerful forces which guard it.\n");
      }
    if (rnd(500)*(level+1)>skill*good)
    { printf("\nYou feel the evil seeping into your soul.\n");
      addgood(-1);
    }
  }
}

void slip()
{ /* player slips in a river! Where does he come up? */
int xd,yd;

if (level/2==level%2)
  xd=yd=1;
else
  xd=yd=-1;
printf("\nYou lose your footing and fall into the raging river!\n");
a=rnd(3*(level+1));
b=c=0;
for (i=0; i<a; i++)
{ c=rnd(10*level+1);
  b=b+c;
  if (maze[py+yd][px]=='c') py=py+yd;
  if (maze[py][px+xd]=='c') px=px+xd;
}
hurt(b,"the river");
printf("Finally, you manage to regain your footing and stand up again.\n");
if (rnd(level+1)!=0)
{ for (i=0; i<a; i++)
  { b=rnd(8)+2;
    if (possessions[b][0]!=0)
    { printf("You lost a %s\n",&possessionsdat[threetoi(&possessions[b][0])][0]);
      possessions[b][0]=0;
    }
  }
}
}

void dead(x) char *x;
{ /* player has died... game over */
char fn[80];

printf("\n\nYOU ARE *DEAD*!\n");
printf("\n\nAh, what an untimely demise. Next game, you will start over\n");
printf("again as a new user. Better luck next time!\n\n");
strcpy(fn,"P");
itothree(&fn[1],playernum);
fn[4]=0;
strcat(fn,".DAT");
fp=pathopen(fn,"w");
fprintf(fp,"DEAD\n");
if (!blank(x))
  fprintf(fp,"%s\n",x);
fclose(fp);
strcpy(qw,playername);
strcat(qw," was killed");
if (!blank(x))
{ strcat(qw," by a ");
  strcat(qw,x);
}
addnews(qw);
exit(0);
}

void savelev(x) int x;
{ /* save level x from MAZE */
char n[80];

strcpy(n,"LEVELx.DAT");
n[5]=x+48;
fp=pathopen(n,"w");
for (i=0; i<75; i++)
  fprintf(fp,"%s\n",&maze[i][0]);
fclose(fp);
}

void hurt(zz,x) int zz; char *x;
{ /* player has taken zz points of damage */
if (armour>=zz)
{ printf("\nYour armour has absorbed the blow!\n");
  armour=armour-zz;
  return;
}
if (armour>0)
  printf("\nYour armour has been destroyed!\n");
zz=zz-armour;
armour=0;
printf("You have sustained %d points of damage!\n",zz);
strength=strength-zz;
if (strength<1) dead(x);
}

void learnspell()
{ int list[10];

  a=-1;
  for (i=0; i<10; i++)
    if ((blank(&spells[i][0]))&&(a==-1)) a=i;
  /* now we have the first empty spell location */
  if ((a!=-1)&&(a<skill/50))
  { printf("\nYou may now learn a new spell!\n\n");
    /* choose 10 spells to choose from */
    for (i=0; i<9; i++)
    { list[i]=rnd(50)*2;
      printf("%c - %s\n",i+65,&spellsdat[list[i]][0]);
    }
    do
    { printf("\nChoose your level %d spell (can NOT be changed!) :",a);
      gets(qw);
      qw[0]=toupper(qw[0]);
    } while ((qw[0]<65)||(qw[0]>74));
    printf("\nYou have now learned %s!\n",&spellsdat[list[qw[0]-65]][0]);
    strcpy(&spells[a][0],&spellsdat[list[qw[0]-65]][0]);
    strcpy(&spells[a+10][0],&spellsdat[list[qw[0]-65]+1][0]);
  }
}

void addgood(x) int x;
{ /* adjust player's 'good' factor */
int d;

  good=good+x;
  for (i=0; i<10; i++)
  { if (!blank((char*)&friend[i].name))
    { d=friend[i].go-good;
      if (abs(d)>rnd(30)+5)
      if (sgn(d)==-1)
      { printf("\n%s turns to you. 'You are becoming too pure for my tastes.'\n",friend[i].name);
	printf("'I shall leave you now.'\nHe vanishes into the dark.\n\n");
	friend[i].name[0]=0;
      }
      else
      { printf("\n%s turns to you. 'You are too cruel for my tastes.'\n",friend[i].name);
	printf("'I cannot bear to be in your party any longer.'\n\n");
	printf("He then vanishes into the dark.\n");
	friend[i].name[0]=0;
      }
    }
  }
  fixfriendlist();
}

int sgn(x) int x;
{ /* sign */
if (x==0) return(0);
else if (x<0) return(-1);
     else return(1);
}

void domagic()
{ /* player wants to try a magic spell */
int zz,sn,pm,ef,p,e,n;

do
{ showspells();
  printf("\nMagic points available: %d",magic);
  printf("\nWhat spell would you like to try? (0-9):");
  gets(qw);
  if (blank(qw)) return;
} while ((qw[0]<'0')||(qw[0]>'9'));
sn=qw[0]-48;
if (blank(&spells[sn][0]))
{ printf("\n\nThere is no spell learned for level %d\n",sn);
  return;
}
zz=threetoi(&spells[sn+10][0]);
if (zz>magic)
{ printf("\n\nYou haven't enough magic power for that spell!\n");
  return;
}
p=spells[sn+10][10];
if ((p=='+')&&(good<-4))
{ printf("\nOnly GOOD people can cast that spell!\n");
  return;
}
if ((p=='-')&&(good>4))
{ printf("\nOnly EVIL people can cast that spell!\n");
  return;
}
printf("\n\nYou cast level %d %s!\n",sn,&spells[sn][0]);
magic=magic-zz;
zz=0;
pm=spells[sn+10][6]=='+' ? 1 : -1;
ef=threetoi(&spells[sn+10][7]);
e=spells[sn+10][11];
for (i=3; i<6; i++)
{ n=pm*rnd(ef*(sn+1));
  switch (spells[sn+10][i])
  { case 'G': printf("Gold %s %d!\n",n>0?"up":"down",abs(n));
	      itothree(&possessions[1][0],limit(threetoi(&possessions[1][0])+n));
	      zz=1;
	      break;
    case 'S': printf("Skill %s %d!\n",n>0?"up":"down",abs(n));
	      skill=limit(skill+n);
	      zz=1;
	      break;
    case 'T': if (n<0) hurt(-n,"his own magic");
	      else { printf("Strength up %d!\n",n);
		     strength=strength+n;
		   }
	      zz=1;
	      break;
    case 'A': printf("Armour %s %d!\n",n>0?"up":"down",abs(n));
	      armour=limit(armour+n);
	      zz=1;
	      break;
    case 'W': printf("Weapon %s %d!\n",n>0?"up":"down",abs(n));
	      weaponstrength=limit(weaponstrength+n);
	      zz=1;
	      break;
    case 'E': for (a=0; a<10; a++)
	      { if (!blank((char*)&enemy[a].name))
		{ if ((e=='0')||((enemy[a].go>4)&&(e=='+'))||((enemy[a].go<-4)&&(e=='-')))
		  { zz=1;
		    printf("%s skill %s %d!\n",&enemy[a].name,n>0?"up":"down",abs(n));
		    enemy[a].sk=limit(enemy[a].sk+n);
		    n=pm*rnd(ef*(level+1));
		  }
		}
	      }
	      break;
    case 'F': for (a=0; a<10; a++)
	      { if (!blank((char*)&enemy[a].name))
		{ if ((e=='0')||((enemy[a].go>4)&&(e=='+'))||((enemy[a].go<-4)&&(e=='-')))
		  { zz=1;
		    if (n>0)
		    { printf("%s strength up %d!\n",&enemy[a].name,abs(n));
		      enemy[a].st=limit(enemy[a].st+n);
		    } else enemyhurt(a,-n);
		    n=pm*rnd(ef*(level+1));
		  }
		}
	      }
	      break;
    case 'B': for (a=0; a<10; a++)
	      { if (!blank((char*)&enemy[a].name))
		{ if ((e=='0')||((enemy[a].go>4)&&(e=='+'))||((enemy[a].go<-4)&&(e=='-')))
		  { zz=1;
		    printf("%s armour %s %d!\n",&enemy[a].name,n>0?"up":"down",abs(n));
		    enemy[a].ar=limit(enemy[a].ar+n);
		  }
		  n=pm*rnd(ef*(level+1));
		}
	      }
	      break;
    case 'X': for (a=0; a<10; a++)
	      { if (!blank((char*)&enemy[a].name))
		{ if ((e=='0')||((enemy[a].go>4)&&(e=='+'))||((enemy[a].go<-4)&&(e=='-')))
		  { zz=1;
		    printf("%s weapon %s %d!\n",&enemy[a].name,n>0?"up":"down",abs(n));
		    enemy[a].we=limit(enemy[a].we+n);
		  }
		  n=pm*rnd(ef*(level+1));
		}
	      }
	      break;
    case 'D': for (a=0; a<10; a++)
	      { if (!blank((char*)&enemy[a].name))
		{ if ((e=='0')||((enemy[a].go>4)&&(e=='+'))||((enemy[a].go<-4)&&(e=='-')))
		  { zz=1;
		    printf("%s curse %s %d!\n",&enemy[a].name,n>0?"up":"down",abs(n));
		    enemy[a].cu=limit(enemy[a].cu+n);
		  }
		  n=pm*rnd(ef*(level+1));
		}
	      }
	      break;
    case 'C': printf("Your curse has %s %d!\n",n>0?"increased":"decreased",abs(n));
	      cursed=limit(cursed+n);
	      if (cursed==1) cursed=0;
	      zz=1;
	      break;
  }
}
if (zz==0) printf("No effect!!\n");
}

int limit(x) int x;
{ if (x<1) x=1;
  if (x>999) x=999;
  return(x);
}

void enemyhurt(n,d)
{ if (enemy[n].ar>=d)
  { printf("%s's armour absorbs the damage!\n",enemy[n].name);
    enemy[n].ar=enemy[n].ar-d;
    return;
  }
  if (enemy[n].ar>0)
    printf("%s's armour is destroyed!\n",enemy[n].name);
  d=d-enemy[n].ar;
  enemy[n].ar=0;
  printf("%s takes %d damage!\n",enemy[n].name,d);
  enemy[n].st=enemy[n].st-d;
  if (enemy[n].st<1)
  { printf("%s is DEAD!\n",&enemy[n].name);
    enemy[n].name[0]=0;
  }
}

void doitem()
{ /* player wants to use an item */
int pn,it,tx,ty;

do
{ showposs();
  printf("\nUse what item? (0-9):");
  gets(qw);
  if (blank(qw)) return;
} while ((qw[0]<'0')||(qw[0]>'9'));
if (blank(&possessions[qw[0]-48][0]))
{ printf("\n\nThere is no item in that slot!\n");
  return;
}
if (qw[0]==48)
{ printf("\n\nYou are already using your candles.\n");
  return;
}
if (qw[0]==49)
{ printf("\n\nYou don't have to use your gold here.\n");
  return;
}
pn=qw[0]-48;
it=threetoi(&possessions[pn][0]);
switch (it)
{ case 0:  /* green, red, blue, skull keys and sledgehammer */
  case 2:
  case 4:
  case 30:
  case 22: printf("\n\nThat will be used automatically when the need arises.\n");
	   break;
  case 10: /* trapdoor scroll */
	   getwhere(&tx,&ty);
	   if (tx==0) break;
	   if (maze[ty][tx]!='a')
	     printf("\n\nYou may only create a trapdoor on an empty passage!\n");
	   else
	     if (level==9)
	       printf("\n\nYou may not create a trapdoor on the bottom level!\n");
	     else
	     { maze[ty][tx]='h';
	       savelev(level);
	       printf("\n\nTrapdoor created!\n");
	       possessions[pn][0]=0;
	     }
	   break;
  case 12: /* makepath scroll */
	   getwhere(&tx,&ty);
	   if (tx==0) break;
	   if (maze[ty][tx]!='b')
	     printf("\n\nYou may only create a path through a wall.\n");
	   else
	     if ((tx==0)||(tx==74)||(ty==0)||(ty==74))
	       printf("\n\nYou may not create a path there.\n");
	     else
	     { maze[ty][tx]='a';
	       savelev(level);
	       printf("\n\nPath created!\n");
	       possessions[pn][0]=0;
	     }
	   break;
  case 14: /* summon friend */
	   a=-1;
	   for (i=0; i<10; i++)
	     if ((blank(friend[i].name))&&(a==-1)) a=i;
	   if (a==-1)
	     printf("\n\nYou have the maximum number of friends!\n");
	   else
	   { strcpy(friend[a].name,"Friendly magic being");
	     friend[a].st=rnd(30*(level+1))+10;
	     friend[a].sk=rnd(30*(level+1))+10;
	     friend[a].go=good;
	     printf("\n\nFriendly magic being summoned!\n");
	     possessions[pn][0]=0;
	   }
	   break;
  case 16: /* teleport up */
	   if (level==0)
	     printf("\n\nCannot teleport up from level 0!\n");
	   else
	   { level--;
	     loadlev(level);
	     possessions[pn][0]=0;
	     clrfriends();
	     printf("\n\nYou begin to fade away, and reappear a level up.\n");
	     while (maze[py][px]!='a')
	     { py=rnd(75);
	       px=rnd(75);
	     }
	   }
	   break;
  case 18: /* teleport down */
	   if (level==9)
	     printf("\n\nCannot teleport down from level 9!\n");
	   else
	   { level++;
	     loadlev(level);
	     possessions[pn][0]=0;
	     clrfriends();
	     printf("\n\nYou begin to fade away, and reappear a level down.\n");
	     while (maze[py][px]!='a')
	     { py=rnd(75);
	       px=rnd(75);
	     }
	   }
	   break;
  case 20: /* map scroll */
	   printf("\n\nThe scroll magically draws a partial map of the level on itself...\n");
	   printf("\n# = wall, + = river, ! = other, X = you are here!\n\n");
	   for (a=(py>6?py-6:0); a<(py>68?75:py+6); a++)
	   { for (b=0; b<75; b++)
	     { if ((a==py)&&(b==px))
		 putchar('X');
	       else
		 switch(maze[a][b])
		 { case 'b': putchar('#'); break;
		   case 'c': putchar('+'); break;
		   case 'a': putchar(' '); break;
		   default : putchar('!'); break;
		 }
	     }
	     putchar('\n');
	   }
	   break;
  case 26: /* magic up potion */
	   printf("\n\nYou drink the potion, and your magic is increased.\n");
	   magic=limit(magic+rnd(20*(level+1))+level*2);
	   possessions[pn][0]=0;
	   break;
  case 28: /* mirror image */
	   a=-1;
	   for (i=0; i<10; i++)
	     if ((blank(friend[i].name))&&(a==-1)) a=i;
	   if (a==-1)
	     printf("\n\nYou have the maximum number of friends!\n");
	   else
	   { strcpy(friend[a].name,playername);
	     strcat(friend[a].name," Image");
	     friend[a].st=strength;
	     friend[a].sk=skill;
	     friend[a].go=good;
	     printf("\n\nMirror Image created!\n");
	     possessions[pn][0]=0;
	   }
	   break;
  case 100: /* magic light scroll */
	    printf("\n\nYour candle flame suddenly begins to burn very\n");
	    printf("brightly, but it is flickering in a very slow motion,\n");
	    printf("almost not at all. It will now burn for a much longer\n");
	    printf("time than it would have normally.\n");
	    candle=candle*10;
	    possessions[pn][0]=0;
	    break;
  default: printf("\n\nThat item is of no practical value.\n");
	   break;
}
}

void getwhere(x,y) int *x, *y;
{ /* get a maze location from north, south, east, west */
char q2[80];

do
{ printf("\nWhat direction? (N)orth, (S)outh, (E)ast, (W)est?\n");
  gets(q2);
  if (blank(q2))
  { *x=0;
    return;
  }
  q2[0]=toupper(q2[0]);
} while ((q2[0]!='N')&&(q2[0]!='S')&&(q2[0]!='E')&&(q2[0]!='W'));
switch (q2[0])
{ case 'N': *x=px; *y=py-1; break;
  case 'S': *x=px; *y=py+1; break;
  case 'E': *x=px+1; *y=py; break;
  case 'W': *x=px-1; *y=py; break;
}
}

void showfriends()
{ /* display current friends */
if (friend[0].name[0]==0)
{ printf("\nNo companions at present.\n");
  return;
}
printf("\nCURRENT COMPANIONS:\n\n");
for (i=0; i<10; i++)
  if (friend[i].name[0]!=0)
    printf("%-25s - ST: %d  SK: %d\n",friend[i].name,friend[i].st, friend[i].sk);
}

void clrfriends()
{ /* clear all friends */
if (friend[0].name[0]!=0)
{ printf("Your current companions wave goodbye, unable to follow you further.\n");
  for (i=0; i<10; i++)
    friend[i].name[0]=0;
}}

int passok(x) int x;
{ /* return if passage is ok for code x */
int zz;

if (x<500) return(x);
x=x-500;
zz=0;
for (i=2; i<10; i++)
  if (possessions[i][0]!=0)
    if (threetoi(&possessions[i][0])==x) zz=1;
if (zz==0) printf("\nYou need a %s!\n",&possessionsdat[x][0]);
return(zz);
}

int rnd(x) int x;
{ /* replacement for RANDOM, but eliminates the divide/0 error */
if (x<1) x=1;
return(random(x));
}

int fightmenu(num) int num;
{ /* menu options against num enemies */
printf("\nSuddenly facing you are %d %ss!\n\n",num,enemy[0].name);
printf("They appear willing to talk to you. Do you want to\n");
x=rnd(numposs);
do
{ printf("(A)ttack, [T]alk, (B)ribe or (I)gnore? ");
  gets(qw);
  if (blank(qw)) qw[0]='T';
  qw[0]=toupper(qw[0]);
  if ((qw[0]=='B')&&(!blank(friend[9].name)))
  {  printf("\nYou already have the maximum number of companions!\n\n");
     qw[0]='Z';
  }
  if (qw[0]=='B')
  { printf("\n\nWhat would you like to try to bribe them with?\n");
    showposs();
    printf("\n(0-9, return to abort):");
    gets(qw);
    if ((qw[0]>='0')&&(qw[0]<='9'))
    { c=qw[0]-48;
      if (c==0)
	printf("\nThey are not interested in your candles.\n");
      if (c==1)
      { printf("How much gold do you want to give them?\n");
	y=threetoi(&possessions[1][0]);
	printf("(0-%d):",y);
	gets(qw);
	z=atoi(qw);
	if ((z<1)||(z>y))
	  printf("You give them nothing.\n");
	else
	{ printf("You give them the gold...\n");
	  y=y-z;
	  itothree(&possessions[1][0],y);
	  x=x-z*(20/num);
	}
      }
      if ((c>=2)&&(c<=9))
      { if (blank(&possessions[c][0]))
	  printf("\nNothing there to give!\n");
	else
	{ y=threetoi(&possessions[c][0]);
	  x=x-y;
	  printf("\nThey take the %s.\n",&possessionsdat[y][0]);
	  possessions[qw[0]-48][0]=0;
	}
      }
      if (x<=0)
      { printf("\nThey have agreed to join you!\n\n");
	a=-1;
	for (i=0; i<10; i++)
	  if ((blank(friend[i].name))&&(a==-1)) a=i;
	if (a+num>10)
	{ printf("%d of them merely slink off into the darkness.\n",a+num-10);
	  num=num-(a+num-10);
	}
	for (i=0; i<num; i++)
	{ strcpy(friend[i+a].name,enemy[i].name);
	  friend[i+a].st=enemy[i].st;
	  friend[i+a].sk=enemy[i].sk;
	  friend[i+a].go=enemy[i].go+(rnd(abs(good))*sgn(good));
	  enemy[i].name[0]=0;
	}
	return(0);
      } else
      { printf("\nThey are not satisfied, and wait to see what you will\n");
	printf("do next...\n\n");
      }
    }
  }
} while ((qw[0]!='A')&&(qw[0]!='T')&&(qw[0]!='I'));
if (qw[0]=='I')
{ if (rnd(enemy[0].go)<rnd(enemy[0].go/2))
  { printf("\nYou ignore them, and they wander off.\n");
    return(0);
  }
  printf("The creatures glare at you. 'We will not be ignored!'\n");
  return(fight2(0,num));
}
if (qw[0]=='A')
{ printf("\nYou launch into the attack!\n");
  good=good-sgn(enemy[0].go);
  return(fight2(1,num));
}
printf("\nYou attempt to talk to them.\n");
strcpy(qw,&hintsdat[rnd(100)][0]);
if (blank(qw)) printf("They claim to have no useful information.\n");
else printf("They say:\n'%s'\n\n",qw);
a=-1;
for (i=0; i<10; i++)
  if ((blank(friend[i].name))&&(a==-1)) a=i;
if ((a!=-1)&&(num<=10-a))
  if ((sgn(good)==sgn(enemy[0].go))&&(abs(good-enemy[0].go)<10)&&(rnd(50)>35))
  { printf("\nThe %ss step forward, and offer to join you on your mission.\n",enemy[0].name);
    printf("Do you accept? [Y]/n : ");
    gets(qw);
    if (toupper(qw[0])=='N')
    { printf("\nThey accept your decision, and leave you.\n");
      for (i=0; i<num; i++)
	enemy[i].name[0]=0;
      return(0);
    }
    printf("\nThey happily join your group!\n");
    for (i=0; i<num; i++)
    { strcpy(friend[a+i].name,enemy[i].name);
      friend[a+i].st=enemy[i].st;
      friend[a+i].sk=enemy[i].st;
      friend[a+i].go=enemy[i].go+(rnd(abs(good))*sgn(good));
      enemy[i].name[0]=0;
    }
    return(0);
  } else
  { printf("They then turn, and vanish into the dark.\n\n");
    for (i=0; i<num; i++)
      enemy[i].name[0]=0;
    return(0);
  }
return(0);  /* should not get here, but if it does... */
}

int fight2(fa,num) int fa,num;
{ /* actual fight routine. fa=first attack (1=player), num=number enemies */
  /* return true if player escaped */
int numfr,mnum,mgol;

  mnum=num; mgol=enemy[0].mg;
  c=0;
  while (c==0)
  { if (fa==0)
      for (a=0; a<num; a++)
	eattack(a);
    if (strength<1) dead(enemy[0].name);
    if (fa==1)
    { fa=0;
      addgood(-rnd(enemy[0].go));
    }
    numfr=0;
    while (!blank(friend[numfr].name)) numfr++;
    printf("\n%s and %d friends vs %d %ss\n\n",playername,numfr,num,enemy[0].name);
    printf("Armour at %d, Strength at %d\n",armour,strength);
    do
    { printf("Do you want to [A]ttack, (M)agic, (I)nv/stats or (E)scape? ([A]/M/I/E):");
      gets(qw);
      if (blank(qw)) qw[0]='A';
      qw[0]=toupper(qw[0]);
      if (qw[0]=='I')
	showplayer();
    } while ((qw[0]!='A')&&(qw[0]!='M')&&(qw[0]!='E'));
    switch (qw[0])
    { case 'A': printf("\nYou attack!\n");
		a=rnd(num);
		if (rnd(skill+rnd(12*level)-cursed)>rnd(enemy[a].sk+rnd(12*level)))
		{ printf("You hit!! ");
		  enemyhurt(a,rnd(weaponstrength+strength)+1);
		} else
		  printf("You missed.\n");
		break;
      case 'M': domagic();
		break;
      case 'E': if (bossflag)
		  printf("There is no way to escape!!\n\n");
		else
		{ if (rnd(abs(skill-enemy[0].sk)*2)/(level+1)>rnd(level*10+1))
		  { printf("You escaped!\n\n");
		    c=2;
		  }
		  else printf("You are blocked from escaping!\n\n");
		}
		break;
    }
    fixenemylist();
    num=0;
    while (!blank(enemy[num].name)) num++;
    if (num==0) c=1;
    if (c==0)
    { for (i=0; i<10; i++)
	if ((!blank(friend[i].name))&&(!blank(enemy[0].name)))
	{ a=rnd(num);
	  printf("\n%s attacks %s!\n",friend[i].name,enemy[a].name);
	  if (rnd(friend[i].sk+rnd(12*level))>rnd(enemy[a].sk+rnd(12*level)))
	  { printf("%s hits!! ",friend[i].name);
	    enemyhurt(a,rnd(friend[i].st)+1);
	    fixenemylist();
	    num=0;
	    while (!blank(enemy[num].name)) num++;
	    if (num==0) c=1;
	  } else
	    printf("%s misses.\n",friend[i].name);
	}
    }
  }
  if (c==1)
  { printf("\nYou have defeated the enemies!\n");
    z=rnd(((level+1)*10+1)*mnum);
    printf("Skill up %d!\n",z);
    skill=limit(skill+z);
    z=rnd(((level+1)*15+1)*mnum);
    printf("Strength up %d!\n",z);
    strength=strength+z;
    z=rnd(((level+1)*mgol)*mnum);
    printf("You got %d gold!\n\n",z);
    itothree(&possessions[1][0],limit(threetoi(&possessions[1][0])+z));
  }
  if (c==2) return(1);
  else return(0);
}

void eattack(en) int en;
{ /* enemy en attacks */
int sp,fr,i,j,d,sk,aa,ab,ac;

if ((rnd(5)>2)&&(enemy[en].ma>0))
{ sp=rnd(50)*2;
  strcpy(qw,&spellsdat[sp+1][0]);
  if ((qw[6]=='-')&&(cinstr(qw[5],"EFBX"))&&(cinstr(qw[4],"EFBX"))&&(cinstr(qw[3],"EFBX")))
  { printf("\n%s casts %s!!\n",enemy[en].name,&spellsdat[sp][0]);
    enemy[en].ma=enemy[en].ma-threetoi(&qw[0])*level;
    switch(qw[10])
    { case '+': aa=1; break;
      case '0': aa=0; break;
      case '-': aa=-1; break;
    }
    switch(qw[11])
    { case '+': ab=1; break;
      case '0': ab=0; break;
      case '-': ab=-1; break;
    }
    ac=1;
    if ((sgn(enemy[en].go)!=aa)&&(aa!=0))
    { ac=0;
      printf("%s is unable to perform the spell!\n",enemy[en].name);
    }
    if ((sgn(good)!=ab)&&(ab!=0))
    { ac=0;
      printf("The spell has no effect!\n");
    }
    if (ac)
    { for (i=3; i<6; i++)
	for (j=0; j<11; j++)
	{ d=rnd((level+1)*threetoi(&qw[7]));
	  if ((!blank(friend[j].name))||(j==10))
	    switch (qw[i])
	    { case 'E': printf("%ss skill down %d!\n",j==10?playername:friend[j].name,d);
			if (j==10) skill=limit(skill-d);
			else friend[j].sk=limit(friend[j].sk-d);
			break;
	      case 'F': if (j==10)
			  hurt(d,"an enemy's spell");
			else
			{ printf("%ss takes %d damage!\n",friend[j].name,d);
			  friend[j].st=friend[j].st-d;
			  if (friend[j].st<1)
			  { printf("%s is DEAD!\n",friend[j].name);
			    friend[j].name[0]=0;
			    fixfriendlist();
			  }
			}
			break;
	      case 'B': if (j==10)
			  if (armour>0)
			  { if (armour<=d)
			    { printf("Your armour has been DESTROYED!\n");
			      armour=0;
			    }
			    else
			    { printf("Your armour takes %d points of damage!\n",d);
			      armour=armour-d;
			    }
			  }
			  else
			    printf("You have no armour!\n");
			break;
	      case 'X': if (j==10)
			{ if (weaponstrength<=d)
			  { printf("Your weapon has been DESTROYED! Back to your fists\n");
			    weaponstrength=1;
			    strcpy(weaponname,"Bare fists");
			  }
			  else
			  { printf("Your weapon loses %d points of strength!\n",d);
			    weaponstrength=weaponstrength-d;
			  }
			}
			break;
	    }  /* switch */
	}  /* for */
    } /* ac */
      return;
  }  /* if (type of magic) */
  else
  { printf("\n%s casts CURSE!\n",enemy[en].name);
    cursed=cursed+rnd((level+1)*10);
    if (cursed>skill-5) cursed=skill-5;
    printf("\nYour curse is increased to %d!\n",cursed);
    return;
  } /* else */
} /* if (magic) */
fr=rnd(11);
do
{ i=0;
  if (fr<10)
    if (blank(friend[fr].name))
    { fr++;
      i=1;
    }
} while(i);
printf("\n%s attacks %s!\n",enemy[en].name,fr==10?playername:friend[fr].name);
if (fr==10)
  sk=skill;
else
  sk=friend[fr].sk;
if (rnd(enemy[en].sk+rnd(12*level)-enemy[en].cu)>rnd(sk+rnd(12*level)))
{ printf("It hits!\n");
  if (fr==10)
    hurt(rnd(enemy[en].st+enemy[en].we)+1,enemy[en].name);
  else
  { i=rnd(enemy[en].st+enemy[en].we)+1;
    printf("%s takes %d points of damage!\n",friend[fr].name,i);
    friend[fr].st=friend[fr].st-i;
    if (friend[fr].st<1)
    { printf("%s is DEAD!\n\n",friend[fr].name);
      friend[fr].name[0]=0;
      fixfriendlist();
    }
  }
}
else
  printf("It misses.\n");
}

void fixenemylist()
{ /* rebuild the enemy list to avoid spaces */
int f,i,x;

for (f=0; f<10; f++)   /* 10 passes will always fix it */
{ for (i=0; i<10; i++)
    if (blank(enemy[i].name))
    {  for (x=i; x<9; x++)
       {  strcpy(enemy[x].name,enemy[x+1].name);
	  enemy[x].sk=enemy[x+1].sk;
	  enemy[x].st=enemy[x+1].st;
	  enemy[x].ma=enemy[x+1].ma;
	  enemy[x].go=enemy[x+1].go;
	  enemy[x].mg=enemy[x+1].mg;
	  enemy[x].ar=enemy[x+1].ar;
	  enemy[x].we=enemy[x+1].we;
	  enemy[x].cu=enemy[x+1].cu;
       }
       enemy[9].name[0]=0;
    }
}}

int cinstr(c,s) char c; char s[];
{ /* return position if character c is in string s */
int r,i;

r=i=0;
while (s[i])
  if (c==s[i++]) r=i-1;
return(r);
}

void fixfriendlist()
{ /* rebuild friend list to avoid spaces */
int f,i,x;

for (f=0; f<10; f++)   /* 10 passes will always fix it */
{ for (i=0; i<10; i++)
    if (blank(friend[i].name))
    {  for (x=i; x<9; x++)
       {  strcpy(friend[x].name,friend[x+1].name);
	  friend[x].sk=friend[x+1].sk;
	  friend[x].st=friend[x+1].st;
	  friend[x].go=friend[x+1].go;
       }
       friend[9].name[0]=0;
    }
}}

void addcurse(x) char *x;
{ /* randomly tell the player the item in x was cursed! */

if (rnd(10)<=level)
{ printf("\nThe %s was CURSED!\n",x);
  a=rnd((level+1)*10);
  cursed=cursed+a;
  if (cursed>skill-5)
    cursed=skill-5;
  printf("Your curse is increased to %d!\n\n",cursed);
}}

void checkmonster(ox,oy) int ox,oy;
{ a=rnd(100);
  if (a<50)
  { /* encounter monster (?) */
    a=a*2; /* get line # offset */
    strcpy(qw,&monstersdat[a+1][0]);
    if (qw[0]>47)
      if (qw[0]-48<=level)
      { c=rnd(level+1)+1;
	for (i=0; i<10; i++)
	  enemy[i].name[0]=0;
	for (i=0; i<c; i++)
	{ strcpy(enemy[i].name,&monstersdat[a][0]);
	  enemy[i].sk=rnd(threetoi(&qw[4])*(level+1))+1;
	  enemy[i].st=rnd(threetoi(&qw[1])*(level+1))+1;
	  switch (qw[7])
	  { case '0': enemy[i].go=0; break;
	    case '+': enemy[i].go=rnd(5*(level+1))+1; break;
	    case '-': enemy[i].go=-(rnd(5*(level+1))+1); break;
	  }
	  if (qw[8]=='Y')
	    enemy[i].ma=rnd(20*(level+1))+5;
	  else
	    enemy[i].ma=0;
	  b=threetoi(&qw[9]);
	  if (b==0) enemy[i].mg=0;
	  else enemy[i].mg=rnd(b)+1;
	  enemy[i].ar=rnd(threetoi(&qw[12])*(level+1));
	  enemy[i].we=rnd(threetoi(&qw[15])*(level+1))+1;
	  enemy[i].cu=0;
	}
	if (fight(c))
	{ px=ox;
	  py=oy;
	}
      }
  }
}

void docandle()
{ /* update candle */
candle--;
if (candle<1)
{ printf("\nYour candle begins to sputter as it nears the end of it's wick.\n");
  a=threetoi(&possessions[0][0]);
  if (a==0)
  { printf("You have no more candles. Soon you are in the dark. But you are\n");
    printf("far from alone. New creatures, once kept at bay by the light, are\n");
    printf("now closing in. Your end is certainly near, so we'll avoid the nasty\n");
    printf("details and end your story now.\n");
    dead("a lack of light"); /* no return */
  }
  a--;
  printf("You quickly light a new one to avoid being at the mercy of the dark.\n");
  printf("You have %d candles left.\n",a);
  itothree(&possessions[0][0],a);
  candle=100;
}
}

void checkplayer()
{ /* see if player has run into anyone else */
char fn[80]; int epn,i;

for (i=0; i<50; i++)
  if ((!blank(&locationsdat[i][0]))&&(i!=playernum))
  { x=threetoi(&locationsdat[i][0]);
    y=threetoi(&locationsdat[i][3]);
    z=threetoi(&locationsdat[i][6]);
    if ((x==px)&&(y==py)&&(z==level))
    { printf("\n%s is here! Attack? (y/[N]): ",&locationsdat[i][9]);
      gets(qw);
      if (blank(qw)) qw[0]='N';
      qw[0]=toupper(qw[0]);
      if (qw[0]=='Y')
      { epn=i;
	strcpy(fn,"P");
	itothree(&fn[1],i);
	fn[4]=0;
	strcat(fn,".DAT");
	fp=pathopen(fn,"r");
	fgets2(qw,80,fp);
	enemy[0].ma=threetoi(&qw[0]);
	enemy[0].sk=threetoi(&qw[3]);
	enemy[0].st=threetoi(&qw[6])+1000*threetoi(&qw[21]);
	for (z=0; z<21; z++)
	  fgets2(qw,80,fp);
	enemy[0].go=threetoi(&qw[0]);
	enemy[0].ar=threetoi(&qw[3]);
	enemy[0].we=threetoi(&qw[6]);
	enemy[0].cu=threetoi(&qw[9]);
	fgets2(qw,80,fp);
	fgets2(enemy[0].name,80,fp);
	fgets2(qw,80,fp);
	fgets2(qw,80,fp);
	enemy[0].mg=threetoi(&qw[0]);
	fclose(fp);
	strcpy(opname,enemy[0].name);
	z=fight2(1,1);
	if (z==0)
	{ /* did not escape, must have killed the enemy */
	  fp=pathopen(fn,"w");
	  fprintf(fp,"DEAD\n%s\n",playername);
	  fclose(fp);
	  locationsdat[epn][0]=0;
	  strcpy(qw,playername);
	  strcat(qw," mercilessly killed ");
	  strcat(qw,opname);
	  addnews(qw);
	}
      }
    }
  }
}

void readnews()
{ /* read the news.dat file. It should be kept around 20 lines. */

printf("\nRECENT EVENTS\n-------------\n");
fp=pathopen("news.dat","r");
while (fgets2(qw,80,fp))
  printf("%s\n",qw);
fclose(fp);
}

void addnews(x) char *x;
{ /* add a line to news.dat */
fp=pathopen("news.dat","a");
fprintf(fp,"%s\n",x);
fclose(fp);
}