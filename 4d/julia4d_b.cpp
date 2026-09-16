#include <iostream>
#include <fstream>
#include <conio.h>
#include <cmath>
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
void byteadd();
ofstream bitsfile;

int main()   {
  const int iters = 200;
  const float crstart = -1, crend = 2, cistart = -1, ciend = 1;
  const float xstart = -1, xend = 2, ystart = -1, yend = 1;
  const int xres = 320;
  const float ppu = (xend-xstart)/xres;
  const int yres = int((yend-ystart)/(xend-xstart)*xres);
  const int cires = int((ciend-cistart)/(ciend-cistart)*xres);
  const int crres = int((crend-crstart)/(crend-cistart)*xres);
  int count = 0; //debug
  floatint cr, ci;
  float x, y;
  int crpix=0, cipix=0, xpix=0, ypix=0;
  cout << crres << "x" << cires << "x" << xres << "x" << yres << endl;
  bitsfile.open("d:\\juliabits.bin", ios::out|ios::binary|ios::app);
  for (cr.floatv=crstart, crpix=0; crpix<crres; cr.floatv+=ppu, crpix++) {
    for (ci.floatv=cistart, cipix=0; cipix<cires; ci.floatv+=ppu, cipix++) {
      cout << "cr: " << hex << cr.intv << " (" << dec << crpix << ") " <<
              "ci: " << hex << ci.intv << " (" << dec << cipix << ")" << endl;
      cout << count << endl;              //debug
      if(kbhit() and getch() == '\27') {
        bitsfile.close();
        return 1; }
      for (x=xstart, xpix=0; xpix<xres; x+=ppu, xpix++) {
        for (y=ystart, ypix=0; ypix<yres; y+=ppu, ypix++) {
          bitadd(isinset(cr.floatv, ci.floatv, x, y, iters)); 
          count++; /*debug*/ } } } }
  flush();        
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
  
int bitplace = 0; 
char curbyte = 0;
void bitadd(bool bit) {
  curbyte |= bit<<bitplace;
  bitplace++;
  bitplace &= 7;
  if (bitplace==0) {
    byteadd();
    curbyte = 0; } }

const int bufsize = 64000;
char buf[bufsize];
int bufplace = 0;
void byteadd() {

  
  buf[bufplace] = curbyte;
  
  if(curbyte) cout << buf[bufplace] << '-' << bufplace << endl; //debug

  
  bufplace++;
  bufplace %= bufsize;
  if(bufplace==0) {
    
    bitsfile.write(buf, bufsize); } }
    
void flush() {
  if(bitplace!=0) byteadd(); // i hate singletons.
  if(bufplace!=0) bitsfile.write(buf, bufplace); }
  
