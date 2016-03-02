#include <stdio.h>
#include <conio.h>
#include <math.h>

main()
  {
  int angle = 90;
  int xorg = 0;
  int yorg = 0;
  int iters = 100;
  int xres = 600;
  int yres = 600;
  int s = 0;
  bool slice[600][600][2];
  double xc = 0;
  double yc = 0;
  FILE * pFile;
  pFile = fopen("julia.txt","wt");
  if(pFile==NULL)
    {
    printf("Could not open file.\n");
    }
  else
    {
    for(double x=-2; x=+4/xres; x<2)
      {
      for(double y=-2; y+=4/yres; y<2)
        {
        double zx = x;
        double zy = y;
        slice[(int)(x+2)*xres/4][(int)(x+2)*yres/4][s] = 1;
        for(int i=1; i<iters; i++)
          {
          zx = x*x-y*y+cos(angle)*zx;
          zy = 2*x*y+sin(angle)*zy;
          if(sqrt(zx*zx+zy*zy)>=2)
            {
            slice[(int)(x+2)*xres/4][(int)(x+2)*yres/4][s] = 0;
            break;
            }
          }
        }
      }
    }
  }


  

