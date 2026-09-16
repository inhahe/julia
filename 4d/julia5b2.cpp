#include "SDL/SDL.h"
#include <cmath>
#include <iostream>
#include <fstream>

//Inhahe (inhahe@gmail.com)

using namespace std;

const int zrres=250, zires=250, cires=250, crres=250;
const char * filename = "d:\\juliabits.aloreucrlauebin";
int xres, yres;
int iters = 80;
int rcol = 255, gcol = 0, bcol = 0;
int mbcol=255, mrcol=255, mgcol=255;
float zrs, upp, zis, cis, crs, ci = -2, zre = 2, zie, cre, cie;
SDL_Surface *screen;
SDL_Surface *pixsurf;
SDL_Rect *rect;
int pitch;
int bpp ;
int pitchadd ;
char * julia;
    
bool quit = false;
Uint32 color;
Uint32 mandelcolor;

bool getvalue(int cr, int ci, int zr, int zi);
char * mandelpix;

void update(float cr, float ci);
bool isinset(float cr, float ci, float zr, float zi);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int miniter(float cr, float ci, float zr, float zi);
void unlock_screen();
bool lock_screen();
bool loadjulia(const char * filename);
void drawmandelbrot();


int main( int argc, char* args[] ) 
  {
  yres=xres=(int)sqrt(crres*crres+cires*cires+zrres*zrres+zires*zires);
  upp = (zre-zrs)/xres;
  zis = -yres*upp/2; zie = yres*upp/2;
  cis = zis; crs = zrs; cie=zie; cre=zre;
  mandelpix = new char[xres*yres];
  loadjulia(filename);


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
  int cri = (int)((cr-crs)/upp);
  int cii = (int)((ci-cis)/upp);
  char * cp = (char *) screen->pixels;
  
  if(!lock_screen()) return;

  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      if(mandelpix[mi]!=2) {
//        if(isinset(cr, ci, zr, zi)) { 
//        if(isinset(zr, zi, cr, ci)) { 
          if(getvalue(cri, cii, x, y)) {
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
      if(getvalue(x, y, 0, 0)) { 
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




  cerr << cires << ',' << crres << ',' << zires << ',' << zrres << ',' ;  
  
  streamsize size = (long long int)cires * (long long int)crres * (long long int )zires * (long long int)zrres / 8;

cerr << '.' << size << '.';  

  julia = new char(size); // cross fingers..
  if(julia==0) {cerr << "Could not allocate "<< size <<" bytes of memory" << endl;
    exit(1);
  }
ifstream ifile;
ifile.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );

try {
  ifile.open(filename, ios::binary);

  ifile.seekg (0, ios::end);
  ifile.read(julia, size); 

  }
catch (ios_base::failure &f) {
  
  cerr << '/' << f.what() << '/' ; 
  
  exit(1);
  }

  cerr << ifile.fail() << ifile.eof() << ifile.bad();

}
  
  
  
  

bool getvalue(int cr, int ci, int zr, int zi) {
  int index = cr*cires*zrres*zires + ci*zrres*zires + zr*zires + zi;
  return (julia[index>>3]>>(index & 7)) & 1;

}



