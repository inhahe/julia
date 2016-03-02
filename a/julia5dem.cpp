//todo: multicore

//Richard A. Nichols III (inhahe@gmail.com)
//with code from http://en.wikipedia.org/wiki/File:Demj.jpg

#include "SDL/SDL.h"
#include <math.h>

  const int xres = 800;
  const int yres = 600;
  const int iters = 80;
  int rcol = 0, gcol = 0, bcol = 255;
  int mrcol = 255, mgcol = 255, mbcol = 255;
  const double  zrs = -2, zre = 2;
  const double upp = (zre-zrs)/xres;
  const double zis = -yres*upp/2, zie = yres*upp/2;
  const double cis = zis, crs = zrs, cie=zie, cre=zre;
  char mandelpix[xres*yres];
  SDL_Surface *screen;
  SDL_Surface *pixsurf;
  SDL_Rect *rect;
  int pitch;
  int bpp ;
  int pitchadd ;
  double distanceMax;
    
bool quit = false;
  Uint32 color;
  Uint32 mandelcolor;

void update(double cr, double ci);
bool isinset(double Cx, double Cy, double Z0x, double Z0y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int miniter(double cr, double ci, double zr, double zi);
void unlock_screen();
bool lock_screen();
void drawmandelbrot();

#ifdef main
#undef main
#endif

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

void update(double cr, double ci) 
  {

  double zi, zr;
  int x, y, mi = 0;
  char * cp = (char *) screen->pixels;
  
  if(!lock_screen()) return;

  for(zi=zis, y=0; y<yres; zi+=upp, y++) {
    for(zr=zrs, x=0; x<xres; zr+=upp, x++) {
      if(mandelpix[mi]!=2) {
        if(isinset(cr, ci, zr, zi)) { 
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
 
  double zi, zr;
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

double jdist(double Zx, double Zy, double Cx, double Cy, int iter_max) { 
  int i;
  double x = Zx,            // Z = x+y*i  
         y = Zy,            // Zp = xp+yp*1 = 1  
         xp =  1, 
         yp = 0,            
         nz,                // temporary 
         nzp,
        a;                  // a = abs(z) 
  for (i = 1; i <= iter_max; i++)   {  
                            // first derivative    zp = 2*z*zp + 1 = xp + yp*i; 
    nz = 2*(x*xp - y*yp) + 1;  
    yp = 2*(x*yp + y*xp); 
    xp = nz;                //z = z*z + c = x+y*i 
    nz = x*x - y*y + Cx; 
    y = 2*x*y + Cy; 
    x = nz;   
    nz = x*x + y*y; 
    nzp = xp*xp + yp*yp;
    if (nzp > 1e60 || nz > 1e60) {
      break; }}
  a=sqrt(nz);              // distance = 2 * |Zn| * log|Zn| / |dZn| 
  return 2* a*log(a)/sqrt(nzp); }

 
bool isinset(double Cx, double Cy, double Z0x, double Z0y) {
  int Iteration;
  const int IterationMax = 2000;
  const int EscapeRadius = 400;
  int ER2 = EscapeRadius * EscapeRadius;
  int iX, iY;
  distanceMax = jdist(0,0,Cx,Cy,IterationMax);
  double Zx, Zy, Zx2, Zy2;
  Zx=Z0x;
  Zy=Z0y;
  Zx2=Zx*Zx;
  Zy2=Zy*Zy;
  for (Iteration=0; Iteration<IterationMax && ((Zx2+Zy2)<ER2); Iteration++) {
    Zy=2*Zx*Zy + Cy;
    Zx=Zx2-Zy2 + Cx;
    Zx2=Zx*Zx;
    Zy2=Zy*Zy; };                // compute  pixel color (24 bit = 3 bytes) 
  if (Iteration==IterationMax) { // interior of Julia set  
    return true; }
  else {                           // exterior of Filled-in Julia set 
    double distance=jdist(Z0x,Z0y,Cx,Cy,IterationMax);
    if (distance<distanceMax)  
    {                        /*  Julia set */
      return true; }
    else {                       // exterior of Julia set
      return false; } } }
      
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

