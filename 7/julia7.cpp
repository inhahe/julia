#include "SDL/SDL.h"
#include <math.h>

const int xres = 1000;
const int yres = 800;
const int iters = 200; //shouldn't make this > 255.
int rcol = 0, gcol = 0, bcol = 255;
int mrcol = 255, mgcol = 255, mbcol = 255;
const float  zrs = -1.5, zre = 1.5;
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
Uint32  mandelcolor;

void update(float cr, float ci);
inline int outcolor(float cr, float ci, float zr, float zi);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int miniter(float cr, float ci, float zr, float zi);
Uint32* makepal();
void unlock_screen();
bool lock_screen();
void drawmandelbrot();
Uint32* pal;

#ifdef main
#undef main
#endif
int main( int argc, char* args[] )   {

  rect = new SDL_Rect;
  rect->x = 0;
  rect->y = 0;
  rect->w = xres;
  rect->y = yres;
 
  if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr,  "Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1); }

  /* Clean up on exit */
  atexit(SDL_Quit);
  
  screen = SDL_SetVideoMode(xres, yres, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
  if ( screen == NULL ) {
    fprintf(stderr, "Couldn't set video mode: %s\n",
    SDL_GetError());
    exit(1);  }

  pal = makepal(); // must come after setvideomode

  pitch = screen->pitch;
  bpp = screen->format->BytesPerPixel;
  pitchadd = pitch - xres*bpp;

  color = SDL_MapRGB(screen->format, rcol, gcol, bcol);
  mandelcolor = SDL_MapRGB(screen->format, mrcol, mgcol, mbcol);

  
  SDL_Event event;
  quit = false;
 
  drawmandelbrot(); 

  while(!quit)    {  
    SDL_PollEvent(&event);
    switch(event.type)     {  
      case SDL_MOUSEMOTION:
       update(crs+event.motion.x*upp, cis+(yres-event.motion.y)*upp);   

//       printf("%d %d  ", event.motion.x, event.motion.y);

  //     update(crs+625*upp, cis+(yres-360)*upp);   
//       printf("%d %d", event.motion.xrel, event.motion.yrel);
        break;
      case SDL_KEYDOWN:
       break;
      case SDL_KEYUP:
       break;
      case SDL_QUIT:
       delete rect;
       SDL_FreeSurface(screen);
       SDL_FreeSurface(pixsurf);
       quit = true; } }

  SDL_Quit(); 
  return 0;    } 

Uint32* makepal() {
  pal = new Uint32[256];
  int wl = 256/5;
  int ro = 0, go = wl/3, bo = wl/3*2;
  int r, g, b;
  int i;
  for (i=0; i<256; i++) {
    r = (int)(sin((i+ro)/255.0*wl)*128+128);
    g = (int)(sin((i+go)/255.0*wl)*128+128);
    b = (int)(sin((i+bo)/255.0*wl)*128+128);
    
//    printf("%d %d %d  ", r, g, b);
    
    pal[i] = SDL_MapRGB(screen->format, r, g, b); }
  return pal; }

void update(float cr, float ci)   {
  float zi, zr;
  int x, y, mi = 0;
  char * cp = (char *) screen->pixels;
  
  if(!lock_screen()) return;
 
//  SDL_FillRect(screen, 0, 0);

  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      //if(mandelpix[mi]!=2) *cp = pal[outcolor(cr, ci, zr, zi)]; 
      
      
      int s = outcolor(cr, ci, zr, zi);
//      printf("%d ", s);
      
      *cp = pal[s]; 
      mi++;
      cp+=bpp; }
    cp+=pitchadd;  } 
  
  unlock_screen();
  SDL_Flip(screen);  }


void drawmandelbrot()  {
  float zi, zr;
  int x, y;
  int mi = 0;
  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      if(outcolor(zr, zi, 0, 0) == iters) { 
        mandelpix[mi] = 1; } 
      mi++; } }  

  Uint32* cp = (Uint32 *) screen->pixels;
  mi = xres+1;
  cp+=pitch+bpp;
  for(x=1; x<xres-1; x++) {
    for(y=1; y<yres-1; y++) {
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

  SDL_Flip(screen);  }
 
inline int outcolor(float cr, float ci, float zr, float zi) {
  float zr2, zi2, zrt;
  int i;
  for(i=0;i<iters; i++) {
    zr2 = zr*zr; zi2 = zi*zi;
    if((zr2+zi2)>4) return i;
    zrt = zr;
    zr = zr2-zi2+cr;
    zi = 2*zrt*zi+ci; }
  return 0; }

bool lock_screen() {  // Lock the screen for direct access to the pixels
  if ( SDL_MUSTLOCK(screen) ) {
    if ( SDL_LockSurface(screen) < 0 ) {
      fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
      return false; } } 
  return true; }

void unlock_screen() {
  if(SDL_MUSTLOCK(screen)) {
    SDL_UnlockSurface(screen); } }

