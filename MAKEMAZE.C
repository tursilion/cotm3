/* COTM3 Maze generation utility */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char maze[80][80];
int h,v,w1[76][76],v1[76][76],q,z,x,i,c,r,s;
int yy,xx,j;
int level;
char qw[80];
FILE *fp,*f2;

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

int rnd(int);
void main(int,char*[]);
void mazegen(int);

int rnd(x) int x;
{ if (x==0) return(0);
  return(random(x));
}

void main(argc,argv) int argc; char *argv[];
{ printf("Creature of the Maze 3 Maze Generation Utility\n\n");
  printf("(based on a routine by Jack Hauber)\n");
  if (argc!=2)
  { printf("\nUsage: MAKEMAZE <level (0-9)>\n");
    exit(10);
  }
  if (strcmp(argv[1],"-greet")==0)
  { printf("\n\nBlah... this one doesn't NEED a greet screen. C'mon.\n");
    printf("But, I did it anyway. Happy, Gatie?\n");
    exit(0);
  }
  randomize();
  level=atoi(argv[1]);
  printf("Creating level %d...\n",level);
  mazegen(level);
}

void mazegen(level) int level;
{ /* generate one maze */
  yy=0; xx=0;
  /* Height and width */
  h=37; v=37;
  /********************/
  q=0; z=0; x=rnd(h)+1;
  for (i=1; i<=h; i++)
  { maze[yy][xx++]='b';
    maze[yy][xx++]='b';
    printf("%s",i==x?". ":".-");
  } /* top line */
  printf(":\n");
  maze[yy][xx++]='b';
  xx=0; yy++;
  c=1; w1[x][1]=c; c++;
  r=x; s=1; goto l260;
l210:
  if (r!=h) goto l240;
  if (s!=v) goto l230;
  r=1; s=1; goto l250;
l230:
  r=1; s=s+1; goto l250;
l240:
  r=r+1;
l250:
  if (w1[r][s]==0) goto l210;
l260:
  if (r-1==0) goto l530;
  if (w1[r-1][s]!=0) goto l530;
  if (s-1==0) goto l390;
  if (w1[r][s-1]!=0) goto l390;
  if (r==h) goto l330;
  if (w1[r+1][s]!=0) goto l330;
  x=rnd(3)+1;
  switch (x)
  { case 1: goto l790;
    case 2: goto l820;
    case 3: goto l860;
  }
l330:
  if (s!=v) goto l340;
  if (z==1) goto l370;
  q=1; goto l350;
l340:
  if (w1[r][s+1]!=0) goto l370;
l350:
  x=rnd(3)+1;
  switch(x)
  { case 1: goto l790;
    case 2: goto l820;
    case 3: goto l910;
  }
l370:
  x=rnd(2)+1;
  switch (x)
  { case 1: goto l790;
    case 2: goto l820;
  }
l390:
  if (r==h) goto l470;
  if (w1[r+1][s]!=0) goto l470;
  if (s!=v) goto l420;
  if (z==1) goto l450;
  q=1; goto l430;
l420:
  if (w1[r][s+1]!=0) goto l450;
l430:
  x=rnd(3)+1;
  switch(x)
  { case 1: goto l790;
    case 2: goto l860;
    case 3: goto l910;
  }
l450:
  x=rnd(2)+1;
  switch(x)
  { case 1: goto l790;
    case 2: goto l860;
  }
l470:
  if (s!=v) goto l490;
  if (z==1) goto l520;
  q=1; goto l500;
l490:
  if (w1[r][s+1]!=0) goto l520;
l500:
  x=rnd(2)+1;
  switch (x)
  { case 1: goto l790;
    case 2: goto l910;
  }
l520:
  goto l790;
l530:
  if (s-1==0) goto l670;
  if (w1[r][s-1]!=0) goto l670;
  if (r==h) goto l610;
  if (w1[r+1][s]!=0) goto l610;
  if (s!=v) goto l560;
  if (z==1) goto l590;
  q=1; goto l570;
l560:
  if (w1[r][s+1]!=0) goto l590;
l570:
  x=rnd(3)+1;
  switch(x)
  { case 1: goto l820;
    case 2: goto l860;
    case 3: goto l910;
  }
l590:
  x=rnd(2)+1;
  switch(x)
  { case 1: goto l820;
    case 2: goto l860;
  }
l610:
  if (s!=v) goto l630;
  if (z==1) goto l660;
  q=1; goto l640;
l630:
  if (w1[r][s+1]!=0) goto l660;
l640:
  x=rnd(2)+1;
  switch(x)
  { case 1: goto l820;
    case 2: goto l910;
  }
l660:
  goto l820;
l670:
  if (r==h) goto l740;
  if (w1[r+1][s]!=0) goto l740;
  if (s!=v) goto l700;
  if (z==1) goto l730;
  q=1; goto l830;
l700:
  if (w1[r][s+1]!=0) goto l730;
  x=rnd(2)+1;
  switch(x)
  { case 1: goto l860;
    case 2: goto l910;
  }
l730:
  goto l860;
l740:
  if (s!=v) goto l760;
  if (z==1) goto l780;
  q=1; goto l770;
l760:
  if (w1[r][s+1]!=0) goto l780;
l770:
  goto l910;
l780:
  goto l1000;
l790:
  w1[r-1][s]=c;
  c=c+1; v1[r-1][s]=2; r=r-1;
  if (c==h*v+1) goto l1010;
  q=0; goto l260;
l820:
  w1[r][s-1]=c;
l830:
  c=c+1;
  v1[r][s-1]=1; s=s-1; if (c==h*v+1) goto l1010;
  q=0; goto l260;
l860:
  w1[r+1][s]=c;
  c=c+1; if (v1[r][s]==0) goto l880;
  v1[r][s]=3; goto l890;
l880:
  v1[r][s]=2;
l890:
  r=r+1;
  if (c==h*v+1) goto l1010;
  goto l530;
l910:
  if (q==1) goto l960;
  w1[r][s+1]=c; c=c+1; if (v1[r][s]==0) goto l940;
  v1[r][s]=3; goto l950;
l940:
  v1[r][s]=1;
l950:
  s=s+1;  if (c==h*v+1) goto l1010;
  goto l260;
l960:
  z=1;
  if (v1[r][s]==0) goto l980;
  v1[r][s]=3; q=0; goto l1000;
l980:
  v1[r][s]=1; q=0; r=1; s=1; goto l250;
l1000:
  goto l210;
l1010:
  /* save output!! fix later to store in MAZE and save */
  for (j=1; j<=v; j++)
  { maze[yy][xx++]='b';
    for (i=1; i<=h; i++)
    { if (v1[i][j]<2)
      { maze[yy][xx++]='a';
	maze[yy][xx++]='b';
      }
      else
      { maze[yy][xx++]='a';
	maze[yy][xx++]='a';
      }
    }
    maze[yy][xx]=0;
    yy++; xx=0;
    for (i=1; i<=h; i++)
    { if ((v1[i][j]==0)||(v1[i][j]==2))
      { maze[yy][xx++]='b';
	maze[yy][xx++]='b';
      }
      else
      { maze[yy][xx++]='b';
	maze[yy][xx++]='a';
      }
    }
    maze[yy][xx++]='b';
    maze[yy][xx]=0;
    xx=0; yy++;
  }
/* now back to my own code.. finish fixing up the maze */
/* bottom line, first */
yy--;
for (i=0; i<75; i++)
  maze[yy][i]='b';
maze[yy][i]=0;
/* now add the random stuff, based on the level */
/* first a river, with bridges */
xx=rnd(73)+1;
for (yy=1; yy<74; yy++)
{ if (rnd(10)>7)
    maze[yy][xx]='d';
  else
    maze[yy][xx]='c';
  while (rnd(5)==0)
  { xx=xx+(rnd(3)-1);
    if (xx>73) xx=73;
    if (xx<1) xx=1;
    if (rnd(10)>7)
      maze[yy][xx]='d';
    else
      maze[yy][xx]='c';
  }
}
/* now, a few stores */
for (i=0; i<10-level; i++)
{ getcoord(&xx,&yy);
  maze[yy][xx]='e';
}
/* some boulders to make life annoying */
for (i=0; i<=level*3; i++)
{ getcoord(&xx,&yy);
  maze[yy][xx]='f';
}
/* now some nasty trapdoors */
if (level!=9)
{ for (i=0; i<=level*3; i++)
  { getcoord(&xx,&yy);
    maze[yy][xx]='h';
  }
}
/* some downward stairs */
if (level!=9)
{ for (i=0; i<7; i++)
  { getcoord(&xx,&yy);
    maze[yy][xx]='j';
  }
}
/* elevator? */
if (level==0)
{ getcoord(&xx,&yy);
  maze[yy][xx]='k';
}
/* good temples */
for (i=0; i<10-level; i++)
{ getcoord(&xx,&yy);
  maze[yy][xx]='l';
}
/* evil temples */
for (i=0; i<10-level; i++)
{ getcoord(&xx,&yy);
  maze[yy][xx]='m';
}
/* doors, to make things miserable */
for (j='n'; j<='p'; j++)
  for (i=0; i<3*(level+1); i++)
  { getcoord(&xx,&yy);
    maze[yy][xx]=j;
  }
/* check previous level to match stairs up and elevator */
if (level>0)
{ strcpy(qw,"LEVEL");
  qw[5]=level+47; /* level-1 in ASCII */
  qw[6]=0;
  strcat(qw,".DAT");
  f2=fopen(qw,"r");
  if (f2==NULL)
  { printf("Can't open %s!\n",qw);
    exit(10);
  }
  for (i=0; i<75; i++)
  { fgets(qw,80,f2);
    for (j=0; j<75; j++)
      switch(qw[j])
      { case 'h': /* trapdoor */ maze[i][j]='a'; break;
	case 'j': /* downstair*/ maze[i][j]='i'; break;
	case 'k': /* elevator */ maze[i][j]='k'; break;
      }
  }
fclose(f2);
}
/* boss and skull door */
if (level==9)
{ getcoord(&xx,&yy);
  maze[yy][xx]='g';
  if (maze[yy-1][xx]=='a') maze[yy-1][xx]='q';
  if (maze[yy+1][xx]=='a') maze[yy+1][xx]='q';
  if (maze[yy][xx-1]=='a') maze[yy][xx-1]='q';
  if (maze[yy][xx+1]=='a') maze[yy][xx+1]='q';
}
/* DONE! Save it.. */
strcpy(qw,"LEVEL");
qw[5]=level+48;
qw[6]=0;
strcat(qw,".DAT");
fp=fopen(qw,"w");
if (fp==NULL)
{ printf("Can't save %s!\n",qw);
  exit(10);
}
for (i=0; i<75; i++)
  fprintf(fp,"%s\n",&maze[i][0]);
fclose(fp);
}

getcoord(xx,yy) int *xx,*yy;
{ /* choose random coordinates on empty maze */
do
{ *xx=rnd(73)+1;
  *yy=rnd(73)+1;
} while (maze[*yy][*xx]!='a');
}
