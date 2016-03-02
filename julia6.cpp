#include "SDL/SDL.h"
#include <iostream>

  const int xres = 800;
  const int yres = 600;
  const int iters = 80; // should probably lower 
  int rcol = 0, gcol = 255, bcol = 0;
  int mrcol = 255, mgcol = 255, mbcol = 255;
  const float  zrs = -2, zre = 2;
  const float upp = (zre-zrs)/xres;
  const float zis = -yres*upp/2, zie = yres*upp/2;
  const float cis = zis, crs = zrs, cie=zie, cre=zre;
  char mandelpix[xres*yres];
  SDL_Surface *screen;
  SDL_Rect *rect;
  int pitch;
  int bpp ;
  int pitchadd ;
  Uint8* sp;
    
bool quit = false;
  Uint32 color;
  Uint32 mandelcolor;

void update(float cr, float ci);
bool isinset(float cr, float ci, float zr, float zi);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void line(int x0, int y0, int x1, int y1);
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
   sp = (Uint8 *)screen->pixels;
  
  color = SDL_MapRGB(screen->format, rcol, gcol, bcol);
  mandelcolor = SDL_MapRGB(screen->format, mrcol, mgcol, mbcol);
  
  SDL_Event event;
  quit = false;
 
  drawmandelbrot();
  if(!lock_screen()) return -1;
  line(0,0,xres, yres); //debug
//  putpixel(screen, xres/2, yres/2, mandelcolor);
  
  unlock_screen();
  SDL_Flip(screen);



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
       quit = true;
       break;

     }
   }
  
  SDL_Quit(); 
  return 0;  
  } 

void update(float cr, float ci) 
  {

  if(!lock_screen()) return;

  float zi=0, zr=0, ozi=0, ozr=0, zr2, zi2;
  for (int i=0; i<iters; i++) {
    zr2 = zr*zr; zi2 = zi*zi;
    ozi=zi; ozr=zr;
    zr = zr2-zi2+cr;
    zi = 2*ozr*zi+ci; 
    int x0 = (int)((ozr-zrs)/upp);
    int y0 = yres-(int)((ozi-zis)/upp);
    int x1 = (int)((zr-zrs)/upp);
    int y1 = yres-(int)((zi-zis)/upp);
    
    std::cerr << x0 << ',' << y0 << ',' << x1 << ',' << y1 << std::endl;

    if((x1>=xres) || (x1<0) || (y1>=yres) || (y1<0)) break;
    line(x0, y0, x1, y1);

}

  unlock_screen();  

  SDL_Flip(screen);
  }

void drawmandelbrot()  {
 
  if (!lock_screen()) return;

  float zi, zr;
  int x, y;
  int mi = 0;
  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      if(isinset(zr, zi, 0, 0)) { 
        mandelpix[mi] = 1; } 
      mi++; } }  

  char * cp = (char *) sp;
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

  unlock_screen();

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

void line(int x0, int y0, int x1, int y1) {
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  int tx0, tx1, ty0, ty1;
  if(steep) {
    tx0 = x0; tx1 = x1;
    x0 = y0; x1 = y1;
    y0 = tx0; y1 = tx1; }
  if (x0 > x1) {
    tx0 = x0; ty0 = y0;
    x0 = x1; y0 = y1;
    x1 = tx0; y1 = ty0; }
  int deltax = x1 - x0;
  int deltay = abs(y1 - y0);
  int error = deltax / 2;
  int ystep;
  int y = y0;
  if (y0 < y1) {ystep = 1;}
  else {ystep = -1;}
  for (int x=x0; x<=x1; x++) {
  if (steep) { putpixel(screen, y,x, color);}
  else {putpixel(screen, x,y, color);}
    error -= deltay;
    if (error < 0) {
      y += ystep;
      error += deltax; } } }

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

