#include <math.h>
#include <stdio.h>

main()

  {
  double xz, yz, xzl;
  int iters=100;
  double dres=600;
  int ires = (int)dres;
  int pixel;
  
/*  FILE * targa;
  targa = fopen("julia.tga", "wb");


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

   */


  for(double y=0; y<dres; y++)
    {
    for(double x=0; x<dres; x++)
      {
      yz=y*4/dres-2;
      xz=x*4/dres-2;
      pixel=1;
      for(int i=1; i<=iters; i++)
        {
        xzl=xz;
        xz=xz*xz-yz*yz;
        yz=2*xzl*yz;
        if(sqrt(xz*xz+yz*yz)>=2)
          {
          pixel=0;
          break;
          }
        }
//        putc(pixel*255, targa);
//        putc(pixel*255, targa);
//        putc(pixel*255, targa);



      }
    }
//  printf("%d\r\n",wtf);


  }




/*
for each ring of pixels
  for each outside pixel
    find closest linked not found already
    if no more linked not found, link to first one, which should be close
    draw triangle betwen  n, n+1, and nearest to center on other slice
  go to other slice and do the same





