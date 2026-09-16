#include <conio.h>
#include <iostream>
// ways to speed up:
//   make isinset inline
//   unwrap iters loop


union floatint {
  float floatv;
  unsigned long int intv; } what;
  
using namespace std;

bool isinset(float cr, float ci, float zr, float zi, int iters);
void bitadd(bool bit);
void flush();
FILE * ofile;

int main()   {
  const int iters = 200;
  const float crstart = -2, crend = 2, cistart = -2, ciend = 2;
  const float xstart = -2, xend = 2, ystart = -2, yend = 2;
  const int xres = 250;
  const float ppu = (xend-xstart)/xres;
  const int yres = int((yend-ystart)/(xend-xstart)*xres);
  const int cires = int((ciend-cistart)/(ciend-cistart)*xres);
  const int crres = int((crend-crstart)/(crend-cistart)*xres);
  floatint cr, ci;
  float x, y;
  int crpix=0, cipix=0, xpix=0, ypix=0;
  cout << crres << "x" << cires << "x" << xres << "x" << yres << endl;
  ofile = fopen("d:\juliabits2.bin", "wb");
  for (cr.floatv=crstart, crpix=0; crpix<crres; cr.floatv+=ppu, crpix++) {
    for (ci.floatv=cistart, cipix=0; cipix<cires; ci.floatv+=ppu, cipix++) {
      cout << "cr: " << hex << cr.intv << " (" << dec << crpix << ") " 
           << "ci: " << hex << ci.intv << " (" << dec << cipix << ")" << endl;
      if(kbhit() and getch() == '\27') {
        return 1; }
      for (x=xstart, xpix=0; xpix<xres; x+=ppu, xpix++) {
        for (y=ystart, ypix=0; ypix<yres; y+=ppu, ypix++) {
          bitadd(isinset(cr.floatv, ci.floatv, x, y, iters)); } } } }
  flush();        
  }    
  
bool isinset(float cr, float ci, float zr, float zi, const int iters) {
  float zr2 = zr*zr, zi2 = zi*zi, zrt;
  for(int i=0;i<iters; i++) {
    zrt = zr;
    zr = zr2-zi2+cr;
    zi = 2*zrt*zi+ci;
    if(((zr2=zr*zr)+(zi2=zi*zi))>=4) return false; }
  return true; }
  
int bitplace = 0;  // because singletons are ugly.
char curbyte = 0;
void bitadd(bool bit) {
  curbyte |= bit<<bitplace;
  bitplace++;
  bitplace &= 7;
  if (bitplace==0) {
    putc(curbyte, ofile);
    curbyte = 0; } }

void flush() {
  if(bitplace!=0) putc(curbyte, ofile); }
  
