//relies on true being 1


#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <fstream.h>
#include <conio.h>
#include <String>
const int res = 100;
const float scalefactor = 4/(float)res;
const int iters = 100;


void mesh(float angle, float xorigin, float yorigin, char* filename)
  {
  bool *sets = (bool*)malloc(res*res*2*sizeof(bool));
  int *points = (int*)malloc(res*res*2*3*sizeof(int)); //[2]x, y, index connected to
  int side = 0;
  ofstream file (filename);


  file << "mesh {\n";


  for(int z=0; z<res; z++)
    {

    gotoxy(7, wherey());
    printf("%5d", z);

    float cr = sin(angle)*((z*4/res)-2)+xorigin;
    float ci = cos(angle)*((z*4/res)-2)+yorigin;
    ++side%=2;
    memset(sets+int(res*res)*sizeof(bool)*side, 0, int(res*res)*sizeof(bool)); //backwards?
    for(int y=0; y<res; y++)
      {

      gotoxy(13, wherey());
      printf("%5d", y);

      int setsindex = res*y+res*res*side;
      for(int x=0; x<res; x++)
        {
        float zr = x*4/res-2;
        float zi = y*4/res-2;
        bool inset = 1;
        float zr2 = zr*zr, zi2 = zi*zi;
        setsindex++;
        for(int i=0;i<iters; i++)
          {
          float zrt = zr;
          zr = zr2-zi2+cr;
          zi = 2*zrt*zi+ci;
          if(sqrt((zr2=zr*zr)-(zi2=zi*zi))>=2)
            {
            inset = 0;
            break;
            }
          }
        sets[setsindex]=inset;
        }
      }
    int setsstart=res*res*side;
    int pointsstart = res*res*3*side;
    memset(&points[pointsstart], -1, res*res*3);
    for(int y=0; y<res; y++)
      {
      setsstart=setsstart+res;
      for(int x=0; x<res; x++)
        {
        setsstart++;
        for(int yy=y-!!y;yy<y+(y<res-1);yy++)
          {
          for(int xx=x-!!x; xx<x+(x<res-1); xx++)
            {
            if(!sets[setsstart+yy*3+xx])
              {
              points[pointsstart]=x;
              points[pointsstart+1]=y;
              pointsstart+=2;
              goto l;
              }
            }
          }
        l:;
        }
      }

    if(z)
      {
      for(int tside=0;tside<=1;tside++)
        {
        int pointsstart1 = tside*3*res*res;
        int pointsstart2 = (!tside)*3*res*res;
        for(int pointsindex=0;;pointsindex+=3)
          {
          if(points[pointsindex+2]==-1) break;
          for(int pointsindex2=0;;pointsindex2+=3)
            {
            if(points[pointsstart1+pointsindex2+2]==-1) break;
            if(points[pointsstart1+pointsindex2+2]==pointsindex) break;
            if(abs(points[pointsstart1+pointsindex2]-points[pointsstart1+pointsindex])<=1
            && abs(points[pointsstart1+pointsindex2+1]-points[pointsstart1+pointsindex+1])<=1)
              {
              float mind=-1;
              int minindex3;
              for(int pointsindex3=0;;pointsindex3+=3)
                {
                if(points[pointsstart2+pointsindex3+2]==-1) break;

                float d1 = points[pointsstart2+pointsindex3]
                           -(points[pointsstart1+pointsindex]
                             +points[pointsstart1+pointsindex2])
                            /(float)2;
                float d2 = points[pointsstart2+pointsindex3+1]
                           -(points[pointsstart1+pointsindex+1]
                             +points[pointsstart1+pointsindex2+1])
                            /(float)2;
                float d = sqrt(d1*d1+d2*d2);
                if(d<mind || mind==-1)
                  {
                  mind=d;
                  minindex3=pointsindex3;
                  }
                }
              if(mind!=-1)
                {

                file << "triangle {<"
                << points[pointsstart1+pointsindex]*scalefactor-2
                << ", "
                << points[pointsstart1+pointsindex+1]*scalefactor-2
                << ", "
                << (z+(side+tside)%2)*scalefactor-2
                << "><"
                << points[pointsstart1+pointsindex2]*scalefactor-2
                << ", "
                << points[pointsstart1+pointsindex2+1]*scalefactor-2
                << ", "
                << (z+(side+tside)%2)*scalefactor-2
                << "><"
                << points[pointsstart2+minindex3]*scalefactor-2
                << ", "
                << points[pointsstart2+minindex3+1]*scalefactor-2
                << ", "
                << (z+(side+tside+1)%2)*scalefactor-2
                << ">}\n";
                
                points[pointsstart1+pointsindex+2]=pointsindex2;
                }

              }
            }
          }
        }
      }
    }

  file << "}";
  file.close();


  }

int main()
  {
  for(int angle=0; angle<360; angle++)
    {
    gotoxy(1, wherey());
    printf("%5d",angle);
    char* filename = "angle                 ";
    itoa(angle, &filename[5], 10);
    mesh(angle, 0, 0, filename);

    }
  return(0);
  }







