//can work with 30 fps or more if resolution is lowered.  iters can also be lowered for lower 
//resolutions, or maybe even with this resolution, i'm not sure.  

//since new computers are multi-core, we should probably break the julias up into a 
//section for each core, but i don't have multi-core so i didn't bother to try.

//adding out-coloring based on number of iterations would probably not have a noticeable
//effect on speed.

//Richard A. Nichols III (inhahe@gmail.com)

#include "SDL/SDL.h"

  const int xres = 800;
  const int yres = 600;
  const int iters = 80;
  int rcol = 0, gcol = 0, bcol = 255;
  int mrcol = 255, mgcol = 255, mbcol = 255;
  const float  zrs = -2, zre = 2;
  const float upp = (zre-zrs)/xres;
  const float zis = -yres*upp/2, zie = yres*upp/2;
  const float cis = zis, crs = zrs, cie=zie, cre=zre;
  char mandelpix[xres*yres];
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
  
  if(!lock_screen()) return;

  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      if(mandelpix[mi]!=2) {
        if(isinset(cr, ci, zr, zi)) { 
//        if(isinset(zr, zi, cr, ci)) { 
          *cp = color; }
        else { 
          *cp = 0; } }
      mi++;
      cp+=bpp; }
      cp+=pitchadd;  } 
  
  unlock_screen();

  SDL_Flip(screen);
  }

void drawmandelbrot()  {
 
  float zi, zr;
  int x, y;
  int mi = 0;
  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      if(isinset(zr, zi, 0, 0)) { 
        mandelpix[mi] = 1; } 
      mi++; } }  

  char * cp = (char *) screen->pixels;
  mi = xres+1;
  cp+=pitch+bpp;
  for(y=1; y<yres-1; y++) {
    for(x=1; x<xres-1; x++) {
      if(mandelpix[mi]) {
        if (!(mandelpix[mi-xres-1] && mandelpix[mi-xres] && mandelpix[mi-xres+1] && 
            mandelpix[mi-1] && mandelpix[mi+1] &&
            mandelpix[mi+xres-1] && mandelpix[mi+xres] && mandelpix[mi+xres+1])) {
          *cp = mandelcolor; 
          mandelpix[mi] = 2; } }
      cp+=bpp; 
      mi++; }
    cp+=pitchadd+2*bpp; 
    mi+=2; }  

  SDL_Flip(screen);  
  
}
 
bool isinset(float cr, float ci, float zr, float zi) {
  float zr2, zi2, zrt;
  for(int i=0;i<iters; i++) {
    zr2 = zr*zr; zi2 = zi*zi;
    if((zr2+zi2)>4) return false;
    zrt = zr;
    zr = zr2-zi2+cr;
    zi = 2*zrt*zi+ci; }
  return true; }



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

