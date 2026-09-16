#include "SDL/SDL.h"
#include <cmath>
#include <iostream>
#include <fstream>

//Inhahe (inhahe@gmail.com)

using namespace std;

const int zrres=250, zires=250, cires=250, crres=250;
const char * filename = "d:\\juliabits.bin";
int xres, yres;
int iters = 80;
int rcol = 255, gcol = 0, bcol = 0;
int mbcol=255, mrcol=255, mgcol=255;
float zrs, upp, zis, cis, crs, ci = -2, zre = 2, zie, cre, cie;
SDL_Surface *screen;
SDL_Surface *pixsurf;
int pitch;
int bpp ;
int pitchadd ;
char * julia;
    
bool quit = false;
Uint32 color;
Uint32 mandelcolor;

bool getvalue(long long int cr, long long int ci, long long int zr, long long int zi);
char * mandelpix;

bool isinset(float cr, float ci, float zr, float zi);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int miniter(float cr, float ci, float zr, float zi);
void unlock_screen();
bool lock_screen();
bool loadjulia(const char * filename);

int main( int argc, char* args[] ) 
  {
  yres=xres=(int)sqrt(crres*crres+cires*cires+zrres*zrres+zires*zires);
  upp = (zre-zrs)/xres;
  zis = -yres*upp/2; zie = yres*upp/2;
  cis = zis; crs = zrs; cie=zie; cre=zre;
  mandelpix = new char[xres*yres];

  /* Initialize the SDL library */
  if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr,
              "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
  }
  /* Clean up on exit */
  atexit(SDL_Quit);
  /* Initialize the display */
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

  loadjulia(filename);

  drawmandelbrot();

  while(!quit) 
    {  
    SDL_PollEvent(&event);
    switch(event.type)
      {  
      case SDL_MOUSEMOTION:
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
      case SDL_QUIT:
        SDL_FreeSurface(screen);
        SDL_FreeSurface(pixsurf);
        quit = true;
        delete julia;
        break;
      }
    }

  SDL_Quit(); 
  return 0;  
  } 

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

bool loadjulia(const char * filename) {
  
  int size = (long long int)cires * (long long int)crres * (long long int )zires * (long long int)zrres / 8;

  julia = new char[size]; 
  if(julia==0) {
      cerr << "Could not allocate "<< size <<" bytes of memory" << endl;
      exit(1);    }

  FILE * pFile;
  pFile = fopen(filename, "r");
  if(pFile==NULL) {
    cerr << "null" ;
    exit(1);}
  
  fread(julia, 1, size, pFile);
  if(ferror(pFile)) perror("Error while reading file: ");
}
  
bool getvalue(long long int cr, long long int ci, long long int zr, long long int zi) {
  long long int index = cr*cires*zrres*zires + ci*zrres*zires + zr*zires + zi;
  return (bool)((julia[(int)(index>>3)]>>(index & 7)) & 1);
}



