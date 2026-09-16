#include <iostream>
#include <fstream>
#include <conio.h>
#include <cmath>
// calculates the 4-d julia fractal. for every y column, rle encodes it with two-byte lengths
// stores the whole thing into two files: one with a [crres][cires][xres] array of indices to 
// rle encoded y columns, and .one with all the rle encoded y columns delimited by -1's
// ways to speed up:
//   make isinset inline
//   unwrap iters loop

union floatint {
  float floatv;
  unsigned long int intv; } what;
  
using namespace std;

bool isinset(float cr, float ci, float zr, float zi, int iters);
void rleencode(bool ycol[], int yres, short int ycolrles[], long int * curidx);

int main()   {
  const int iters = 200;
  const float crstart = -1, crend = 2, cistart = -1, ciend = 1;
  const float xstart = -1, xend = 2, ystart = -1, yend = 1;
  const int xres = 320;
  const float ppu = (xend-xstart)/xres;
  const int yres = int((yend-ystart)/(xend-xstart)*xres);
  const int cires = int((ciend-cistart)/(ciend-cistart)*xres);
  const int crres = int((crend-crstart)/(crend-cistart)*xres);
  long int ycolrleidxs[xres];
  bool ycol[yres];
  long int curidx = 0, curciidx = 0;
  short int ycolrles[yres*(xres+1)]; // +1 for the -1 ending values
  int ycolrlelen;
  floatint cr, ci;
  float x, y;
  int crpix=0, cipix=0, xpix=0, ypix=0;
  cout << crres << "x" << cires << "x" << xres << "x" << yres << endl;
  ofstream idxfile("index.bin", ios::out|ios::binary|ios::app);
  ofstream rlefile(  "rle.bin", ios::out|ios::binary|ios::app);
  for (cr.floatv=crstart, crpix=0; crpix<crres; cr.floatv+=ppu, crpix++) {
    for (ci.floatv=cistart, cipix=0; cipix<cires; ci.floatv+=ppu, cipix++) {
      cout << "cr:" << hex << cr.intv << " (" << dec << crpix << ") " <<
              "ci:" << hex << ci.intv << " (" << dec << cipix << ")" << endl;
      if(kbhit() and getch() == '\27') {
        idxfile.close(); rlefile.close();
        return 1; }
      curidx = 0;  
      for (x=xstart, xpix=0; xpix<xres; x+=ppu, xpix++) {
        for (y=ystart, ypix=0; ypix<yres; y+=ppu, ypix++) {
          ycol[ypix] = isinset(cr.floatv, ci.floatv, x, y, iters); }
        ycolrleidxs[xpix] = curidx+curciidx;
        rleencode(ycol, yres, ycolrles, &curidx); }
      idxfile.write((char *)ycolrleidxs, sizeof(ycolrleidxs));
      rlefile.write((char *)ycolrles, sizeof(short int) * (curidx));
      curciidx += curidx; } }
  }    
  
bool isinset(float cr, float ci, float zr, float zi, const int iters) {
  float zr2 = zr*zr, zi2 = zi*zi, zrt;
  for(int i=0;i<iters; i++) {
    zrt = zr;
    zr = zr2-zi2+cr;
    zi = 2*zrt*zi+ci;
    if(sqrt((zr2=zr*zr)+(zi2=zi*zi))>=2) return false; }
    // if(zr2=zr*zr)>=2 || zr2<=-1 || (zi2=zi*zi) >=1 || zi2<=-1) return false; //might be faster, prolly not.   fsqrt probably takes beteen 5 and 17 clock cycles.
  return true; }
  
void rleencode(bool ycol[], int yres, short int ycolrles[], long int * curidx) {
  bool lvalue = 0, value;
  short int ccount = 0;
  for(int ypix=0; ypix<yres; ypix++) {
    value = ycol[ypix];
    if(value != lvalue) 
      {
      ycolrles[*curidx] = ccount;
      (*curidx)++;
      lvalue = value;
      ccount = 0; 
      }
    ccount++; }
  ycolrles[*curidx] = -1;
  (*curidx)++;  
  }
  


