
//this program is probably cross-platform but i can't say for sure. i compiled it under mingw. 
//this is called julia6 but it's actually only about the mandelbrot. 

//Richard A. Nichols III (inhahe@gmail.com)

#include "SDL/SDL.h"

const int xres = 800;
const int yres = 600;
const int iters = 80; 
int rcol = 0, gcol = 255, bcol = 0;
int mrcol = 0, mgcol = 0, mbcol = 255;
const float  zrs = -2, zre = 2;
const float upp = (zre-zrs)/xres;
const float zis = -yres*upp/2, zie = yres*upp/2;
const float cis = zis, crs = zrs, cie=zie, cre=zre;
SDL_Surface *screen;
SDL_Surface *mandelsurf;
Uint32 color;
Uint32 mandelcolor;

void update(float cr, float ci);
bool isinset(float cr, float ci, float zr, float zi);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void line(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 color);
void unlock_surface(SDL_Surface *surface);
bool lock_surface(SDL_Surface *surface);
void drawmandelbrot(SDL_Surface *surface, Uint32 color);
Uint32 getpixel(SDL_Surface *surface, int x, int y);

int main( int argc, char* args[] ) 
  {
  /* Initialize the SDL library */
  if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);    }
  /* Clean up on exit */
  atexit(SDL_Quit);
  /*
   * Initialize the display */
  screen = SDL_SetVideoMode(xres, yres, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  if ( screen == NULL ) {
    fprintf(stderr, "Couldn't set video mode: %s\n",
      SDL_GetError());
      exit(1); 
 }

  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      mandelsurf = SDL_CreateRGBSurface(SDL_HWSURFACE, xres, yres, 32, 
                                        0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
  #else
      mandelsurf = SDL_CreateRGBSurface(SDL_HWSURFACE, xres, yres, 32, 
                                        0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
  #endif

  if(mandelsurf == NULL) {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      exit(1);
  } 
  SDL_SetAlpha(mandelsurf, 0, 255);

  color = SDL_MapRGB(screen->format, rcol, gcol, bcol);
  mandelcolor = SDL_MapRGB(mandelsurf->format, mrcol, mgcol, mbcol);
  drawmandelbrot(mandelsurf, mandelcolor);
  SDL_BlitSurface(mandelsurf, NULL, screen, NULL);
  SDL_Flip(screen);  

  SDL_Event event;
  bool quit = false;
  while(!quit)   {  
    SDL_PollEvent(&event);
    switch(event.type)      {  
      case SDL_MOUSEMOTION:
        update(crs+event.motion.x*upp, cis+(yres-event.motion.y)*upp);   
        break;
      case SDL_KEYDOWN: //in case we compile it with full-screen mode.
      case SDL_KEYUP:   //i was too lazy to have it check for a specific key. 
      case SDL_QUIT:
        SDL_FreeSurface(screen);
        SDL_FreeSurface(mandelsurf);
        quit = true;
        break;    } }
  SDL_Quit(); 
  return 0;    } 

void update(float cr, float ci)   {

  SDL_BlitSurface(mandelsurf, NULL, screen, NULL);

  if(!lock_surface(screen)) return;

  float zi=ci, zr=cr, ozi=0, ozr=0, zr2, zi2;
  for (int i=0; i<iters; i++) {
    zr2 = zr*zr; zi2 = zi*zi;
    ozi=zi; ozr=zr;
    zr = zr2-zi2+cr;
    zi = 2*ozr*zi+ci; 
    int x0 = (int)((ozr-zrs)/upp);
    int y0 = yres-(int)((ozi-zis)/upp);
    int x1 = (int)((zr-zrs)/upp);
    int y1 = yres-(int)((zi-zis)/upp);
    
    if((x1>=xres) || (x1<0) || (y1>=yres) || (y1<0)) break;
    line(screen, x0, y0, x1, y1, color);
}

  unlock_surface(screen);  

  SDL_Flip(screen);
  }

void drawmandelbrot(SDL_Surface *surface, Uint32 color)  {
 
  if (!lock_surface(surface)) return;
  float zi, zr;
  int x, y;
  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      if(isinset(zr, zi, 0, 0)) { 
        putpixel(surface, x, y, color); } } }
  unlock_surface(surface);
  SDL_UpdateRect(surface, 0,0,0,0);
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

bool lock_surface(SDL_Surface *surface) {
/* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(surface) ) {
        if ( SDL_LockSurface(surface) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return false; } } 
    return true; }

void unlock_surface(SDL_Surface *surface) {
  if(SDL_MUSTLOCK(surface)) {
    SDL_UnlockSurface(surface); } }

void line(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 color) { 
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
    if (steep) { putpixel(surface, y,x, color);}
    else {putpixel(surface, x,y, color);}
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

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
