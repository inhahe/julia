#include "SDL/SDL.h"
#include <cmath>
#include <iostream>

  const int xres = 400;
  const int yres = 400;
  const int iters = 20; // should probably lower 
  int rcol = 0, gcol = 0, bcol = 255;
  int mrcol = 255, mgcol = 255, mbcol = 255;
  const float cis = -2, cie = 2, crs = -2, cre = 2;
  const float zis = -2, zie = 2, zrs = -2, zre = 2;
  char mandelpix[xres*yres];
  const float upp = (cie-cis)/xres;
  SDL_Surface *screen;
  SDL_Surface *pixsurf;
  SDL_Rect *rect;
  int pitch;
  int bpp ;
  int pitchadd ;
    
bool quit = false;
  Uint32 color;
  Uint32 mandelcolor;

void update(float cr, float ci);
bool isinset(float cr, float ci, float zr, float zi);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int miniter(float cr, float ci, float zr, float zi);
void unlock_screen();
bool lock_screen();
void drawmandelbrot();

int main( int argc, char* args[] ) 
  {

    rect = new SDL_Rect;
    rect->x = 0;
    rect->y = 0;
    rect->w = xres;
    rect->y = yres;
   
    /* Initialize the SDL library */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Clean up on exit */
    atexit(SDL_Quit);
    
    /*
     * Initialize the display */
    screen = SDL_SetVideoMode(xres, yres, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set video mode: %s\n",
                        SDL_GetError());
        exit(1);
    }
    pitch = screen->pitch;
 bpp = screen->format->BytesPerPixel;
 pitchadd = pitch - xres*bpp;
  
  color = SDL_MapRGB(screen->format, rcol, gcol, bcol);
  mandelcolor = SDL_MapRGB(screen->format, mrcol, mgcol, mbcol);
  
  SDL_Event event;
  quit = false;
 
  drawmandelbrot();


 while(!quit) 
   {  
   SDL_PollEvent(&event);
   switch(event.type)
     {  
     case SDL_MOUSEMOTION:
       update(crs+event.motion.x*upp, cis+(yres-event.motion.y)*upp);   
       break;
     case SDL_KEYDOWN:
     case SDL_KEYUP:
     case SDL_QUIT:
       delete rect;
       SDL_FreeSurface(screen);
       SDL_FreeSurface(pixsurf);
       quit = true;
       break;

     }
   }
  
  SDL_Quit(); 
  return 0;  
  } 

void update(float cr, float ci) 
  {

  float zi, zr;
  int x, y, mi = 0;
  char * cp = (char *) screen->pixels;
//  SDL_FillRect(screen, NULL, 0);  //whatever. doesn't work.

  if(!lock_screen()) return;

  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      mi++;
      cp+=bpp;
      if(!mandelpix[mi]==1)
{
        if(isinset(cr, ci, zr, zi)) { 
//        if(isinset(zr, zi, cr, ci)) { 
          *cp = color; }
        else { //because fillrect won't work
          *cp = 0;  } } } 
    cp+=pitchadd;
    } 
  
  unlock_screen();

  SDL_Flip(screen);
  }

void drawmandelbrot()  {
 
  float zi, zr;
  int x, y;
  char * cp = (char *) screen->pixels;
  int mi = 0;
  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      cp+=bpp;
      mi++;
      if(miniter(zr, zi, 0, 0)>=iters-8) { 
        *cp = mandelcolor; 
        mandelpix[mi] = 1; } } 
    cp+=pitchadd; }  
  SDL_Flip(screen);  
  
}
 
  


bool isinset(float cr, float ci, float zr, float zi) {
  float zr2 = zr*zr, zi2 = zi*zi, zrt;
  for(int i=0;i<iters; i++) {
    zrt = zr;
    zr = zr2-zi2+cr;
    zi = 2*zrt*zi+ci;
    if(sqrt((zr2=zr*zr)+(zi2=zi*zi))>=2) return false; }
    // if(zr2=zr*zr)>=2 || zr2<=-1 || (zi2=zi*zi) >=1 || zi2<=-1) return false; //might be faster, prolly not.   fsqrt probably takes beteen 5 and 17 clock cycles.
  return true; }

int miniter(float cr, float ci, float zr, float zi) {
  float zr2 = zr*zr, zi2 = zi*zi, zrt;
  for(int i=1;i<=iters; i++) {
    zrt = zr;
    zr = zr2-zi2+cr;
    zi = 2*zrt*zi+ci;
    if(sqrt((zr2=zr*zr)+(zi2=zi*zi))>=2) return i; }
    // if(zr2=zr*zr)>=2 || zr2<=-1 || (zi2=zi*zi) >=1 || zi2<=-1) return false; //might be faster, prolly not.   fsqrt probably takes beteen 5 and 17 clock cycles.
  return -1; }



bool lock_screen() {
/* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return false; } } 
    return true; }

void unlock_screen() {
  if(SDL_MUSTLOCK(screen)) {
    SDL_UnlockSurface(screen); } }

