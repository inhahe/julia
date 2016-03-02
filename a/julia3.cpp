#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
int main()
  {


  const double dres = 600;
  const int ires = (int)dres;
  int angle = 90;
  double xorg = ires/2;
  double yorg = ires/2;
  double xc = 0;
  double yc = 0;
  int pixel;
  int iters = 100;
  int s = 0;
  double zx, zy, lzx;

  bool *slice;
  slice = (bool *)malloc(ires*ires*2*sizeof(bool));
  memset(slice, 0, ires*ires*2*sizeof(bool));
    
  FILE * pFile;
  pFile = fopen("julia.txt","wt");

  FILE * targa;
  targa = fopen("targa.tga","wb");
  if(targa==NULL) {
    printf("Could not open targa file\r\n");
    exit(0);
    }
   putc(0, targa);
   putc(0, targa);
   putc(2, targa);
   putc(0, targa);
   putc(0, targa);
   putc(0, targa);
   putc(0, targa);
   putc(0, targa);
   putc(0, targa);
   putc(0, targa);
   putc(0, targa);
   putc(0, targa);
   putc(ires%256, targa);
   putc(ires/256, targa);
   putc(ires%256, targa);
   putc(ires/256, targa);
   putc(24, targa);
   putc(16, targa);



   //0 0 2 0 0 0 0 0 0 0 0 0 width height 24 00010000 or00001000



  if(pFile==NULL)
    {
    printf("Could not open file.\r\n");
    }
  else
    {
    s=0;
//    for(double z=0; z<res; z++)
    for(double z=300; z<301; z++)
      {
      s=(s+1)%2;
      printf("1\r\n");
      for(double x=0; x<dres; x++)
        {
        for(double y=0;  y<dres; y++)
          {
          zx = x*4/dres-2;
          zy = y*4/dres-2;
          pixel = 1;
          for(int i=1; i<iters; i++)
            {
            lzx=zx;
            zx = zx*zx-zy*zy+cos(angle)*(((z-xorg)*4/dres)-2);
            zy = 2*lzx*zy+sin(angle)*(((z-yorg)*4/dres)-2);
            if(sqrt(zx*zx+zy*zy)>=2)
              {
              pixel = 0;
              break;
              }
            }
          slice[(int)x*ires*s+(int)y*s+s]=pixel;
          putc(pixel*255, targa);
          putc(pixel*255, targa);
          putc(pixel*255, targa);
          }
        }
      }
    }
  }

/*

gdb
gxx -d


make an array of border pixels
for each border pixel search for nearest border pixel on other level
if two on current level have same other , 3 triangles instead of 2
do same for other level without duplicate triangles

nearest pixel must not be separated by non-solid
in all possible connecting lines

does not cover flat tops and bottoms
c:\program files\povray ..\smooth.zip

optimize 
            zx = zx*zx-zy*zy+cos(angle)*((z-xorg)*4/res)-2);
            zy = 2*lzx*zy+sin(angle)*((z-yorg)*4/res)-2);

printf z\r


*/




