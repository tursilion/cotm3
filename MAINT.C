/* Creature of the Maze III - Maintenance program */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

void fail(char *);
void main(int, char*[]);
void readdat(void);
void readplayer(int);
int threetoi(char[]);
void maint(void);
FILE *pathopen(char*,char*);
void itothree(char [],int);
char *fgets2(char *,int, FILE *);
int blank(char []);
void greet(void);
int sgn(int);
int limit(int);
int rnd(int);
int cinstr(char,char[]);

/********************** Amiga Compatibility ******************/
#ifdef AMIGA

/* prototypes*/

void randomize(void);
int random(int);
int strcmp(char *,char*);
char *strcpy(char*,char*);
char *strcat(char*,char*);

/* Turbo C compatibility functions */

void randomize()
{ /* randomize function on Amiga only */
long seed;

time(&seed);
srand(seed);
}

int random(x) int x;
{ /* random(x) function on Amiga only */
return(rand()%x);
}
#endif
/*************************************************************/

int magic, skill, strength;
int px,py,level,bossflag;
char spells[21][81];
int good, armour, numposs;
char weaponname[81];
int weaponstrength;
char playername[81];
int cursed;
char possessions[11][81];

int playernum;

char cname[81], cdesc[6][81];
int csk,cst,car;
int nth,sth,est,wst;

struct myfriend{
char name[81];
int st,sk,ma,go;
} friend[11];

char path[81];
char qw[81],opt[20];
char maze[76][85];

FILE *fp;
int a,b,c,x,y,z,i,candle;
char *flag;

void fail(x) char *x;
{ printf("Failure - %s\n",x);
  exit(10);
}

void main(argc, argv) int argc; char *argv[];
{ printf("\n\nCREATURE OF THE MAZE ]I[ MAINTENACE - by M. Brent\n");
  printf("\n(c)1995 - all rights reserved.\n");
  if ((argc!=2)&&(argc!=3))
  { printf("\n\nUsage: MAINT <options> [data path]\n");
    printf("Options: B - rest boss\n");
    printf("         L - update LOCATIONS.DAT\n");
    printf("         W - shrink NEWS.DAT (was WALL) to 20 lines\n\n");
    printf("EXAMPLE: MAINT LW C:\\COTM\\\n\n");
    exit(0);
  }
  if (argc==3) strcpy(path,argv[2]);
  else strcpy(path,"");
  strcpy(opt,argv[1]);
  if (strcmp(opt,"-greet")==0) greet();
  else maint();
}

void readplayer(z) int z;
{ /* read in the player's data file */
char fn[80];

fn[0]='P';
itothree(&fn[1],z);
fn[4]=0;
strcat(fn,".DAT");
fp=pathopen(fn,"r");
fgets2(qw,80,fp);
if (strcmp(qw,"DEAD")==0)
{ strcpy(playername,"DEAD");
  px=0; py=0; level=0;
  return; 
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

void maint()
{ /* perform daily maintenance */
FILE *f2;
int zz,yy;

yy=0;
printf("\nPerforming maintenance...\n");
while (opt[yy])
{ switch (opt[yy])
  { case 'B': printf("\nResting Boss..\n");
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
              break;

    case 'L': printf("\nBuilding LOCATION.DAT\n");
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
              break;

    case 'W': printf("\nShortening NEWS.DAT\n");
              /* player.spells[x] has enough buffer space */
              for (i=0; i<20; i++)
                spells[i][0]=0;
              fp=pathopen("NEWS.DAT","r");
              while (fgets2(qw,80,fp))
                if (!blank(qw))
                { for (i=1; i<20; i++)
                  strcpy(&spells[i-1][0],&spells[i][0]);
                  strcpy(&spells[19][0],qw);
                }
              fclose(fp);
              fp=pathopen("NEWS.DAT","w");
              for (i=0; i<20; i++)
                fprintf(fp,"%s\n",&spells[i][0]);
              fclose(fp);
              break;

    default: printf("\nUnimplemented option: %c\n",opt[yy]);
             break;
  }
yy++;
}
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

void itothree(x,y) char x[]; int y;
{ /* convert an integer into 3 digit string */
x[0]=y/100+48;
y=y%100;
x[1]=y/10+48;
x[2]=y%10+48;
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

void greet()  
{
printf("\n\nHi to The Gatekeeper again, to Riczilla who was upset not\n");
printf("to be mentioned in the original, and to anyone else who knows\n");
printf("about these.\n\n");
}

int sgn(x) int x;
{ /* sign */
if (x==0) return(0);
else if (x<0) return(-1);
     else return(1);
}

int limit(x) int x;
{ if (x<1) x=1;
  if (x>999) x=999;
  return(x);
}

int rnd(x) int x;
{ /* replacement for RANDOM, but eliminates the divide/0 error */
if (x<1) x=1;
return(random(x));
}

int cinstr(c,s) char c; char s[];
{ /* return position if character c is in string s */
int r,i;

r=i=0;
while (s[i])
  if (c==s[i++]) r=i-1;
return(r);
}
