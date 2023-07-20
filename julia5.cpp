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

[Project]
FileName=julia5.dev
Name=julia5
UnitCount=1
Type=0
Ver=1
ObjFiles=
Includes=
Libs=
PrivateResource=
ResourceIncludes=
MakeIncludes=
Compiler=
CppCompiler=_@@_
Linker=-lmingw32 -lSDLmain -lSDL_@@_
IsCpp=1
Icon=
ExeOutput=
ObjectOutput=
OverrideOutput=0
OverrideOutputName=julia5.exe
HostApplication=
Folders=
CommandLine=
UseCustomMakefile=0
CustomMakefile=
IncludeVersionInfo=0
SupportXPThemes=0
CompilerSet=0
CompilerSettings=0000000001001000000100

[Unit1]
FileName=julia5b.cpp
CompileCpp=1
Folder=julia5
Compile=1
Link=1
Priority=1000
OverrideBuildCmd=0
BuildCmd=

[VersionInfo]
Major=0
Minor=1
Release=1
Build=1
LanguageID=1033
CharsetID=1252
CompanyName=
FileVersion=
FileDescription=Developed using the Dev-C++ IDE
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=
AutoIncBuildNr=0

MZ�       ��  �       @                                   �   � �	�!�L�!This program cannot be run in DOS mode.
$       PE  L ��I        � 8 P   `   � @      `    @                     0    S�                                    �                                                                                                                  .text   (N      P                 `  `.data   p    `      T              @  �.rdata  ,   p      V              @  @.bss    ��  �                      �  �.idata  �         \              @  �                                                                                                                                                                                                                                                                                                                                                                                                                                                                U����]��U1ۉu��1�� =�  �wC=�  �r[�   �$   1��D$�K  ��tl��t*�$   �л�����؋u��]���]� =�  �t�=�  �t��؋u��]���]� �v =  �u��$   1��t$�J  ��t4��t��$   ����$   �   �\$�~J  ��t��e	  �������$   �   ������L$�TJ  �b�����������������U��S��$�]��$ @ �K  ���	  �E�    � �@ �U�\$�`@ �D$�T$�L$�$�@ �!J  �p�G ��tX� `@ �4"H ����   ���t �p�G �D$�4"H �K0�$��I  �4"H ���t�p�G �\$�4"H �QP�$�I  �I  � `@ ��>  ����fI  ��L$� �@ �T$��@ �$��  ���2I  �$��J  �D$�4"H �B�$�UI  �4"H �U����v ��'    U����$   �("H ��������&    U����$   �("H �������&    U�L"H ��]��t& U�<"H ��]�ᐐ��U��]��7  �������U� p@ ��1��E�E�E�E���&    ��B���Ƀ�O������������%�����������������v���������������]1������������]�   É�U1ɉ�W�   �V1�S���    �p@ �p@ ��ɉ}�1��E��]��E��E�    ������E��������ʃ}�O�������  �������������������v���������1���tƁP�@ �p@ BA��  ~������p@ F��W  �w������؋=0�@ �!  �5 �@ �$�@ �_�   �ڻ  ��P�@  t\��/}@  t?��0}@  t6��1}@  t-��O�@  t$��Q�@  t��o�@  t��p�@  t	��q�@  u��@ ��5 �@ ƁP�@ �AKy���@ G���pځ�V  �v����0�@ �$�3  ��[^_]����ظ   ��������    ��    U����0�@ �P��u� @  t�$��  ��xɸ   ���  �D$�p@ �L$�4"H ��@�$�F  �1�Ð��    ��    U����0�@ �H��u� @  t�$�  �Í�    ��    U��WV1�S���0�@ �E�P�X�]����E�]�u� @  t�$�Z  ���  �   ����   �(p@ 1��  �@�   ��U�1��E耾P�@ tb�E�    ���M��E��%���E����������ʃ}�O�E����E���   ���������������������v�����������1���tl��@ ��,p@ B� �@ FÁ�  �z������,p@ G��@ Ӂ�W  �P����ء0�@ �X��u]� @  uU�E��[^_]�S  ��[^_]�� ������ظ   �{����Q  �D$�4"H �p@ �L$��@�$�E  1�������$�5  �0�@ 뜍�&    ��'    U��S��4�$   ��  f�   f�@ f�@X�(�@ �$    ��
  ����  �$�!@ �X  �b����\$�  @�   �L$�T$�$   ��
  �0�@ ������  �X�@�P�$�@ ��� �@ �����`@ )Ӊ�@ �P�G �T�G �D$�T$�\$�A�$�
  ��@ � `@ �0�@ �`@ �`@ �T$�\$�D$�Q�$�V
  ��@ ��@  �5����=�@  ul�]����&    ��t!�=�@  uR�$�*
  �E؃�tQ߃�|ߋ(�@ �$�\
  �0�@ �$�
  �,�@ �$�
  ��@ �=�@  t���	  ��41�[]Ív �U޸X  �M�)�P�$���lp@ �pp@ �\$Q�$���lp@ �tp@ �$�����O����%	  �D$�0p@ �D$�4"H ��@�$��B  �$   �B  ��  �D$�Mp@ �ϐ����������U�   ��S��d����3  ��  ��C  �ÍE��$�C  ����u�   C��� ����	��	Шu��"t>�� ��1Ҁ�	��tL��    ��    ��t<C��� ��1Ҁ�	��u��$�t& C���"��1҄���u��"th��    �� ����	�����C��� ��	��	Шu��$    �C  ���E��
   t�U؉\$1ۉ\$�T$�$�  ���]���C�또�������U�,u@ �����    �Q����  @ ��,u@ r�]Ð�������U����]Ð��������U����0`@ ���t&����������������0`@ �Q�A�0`@ ��u��Í�&    U��S����]@ ���t)����t����'    ���]@ Ku��$@[Editor_0]
CursorCol=1
CursorRow=11
TopLine=1
LeftChar=1
Open=1
Top=1
[Editors]
Order=0
Focused=0
L	     �W S     .text           �  |  �R     c      `.data              	              @  �.bss            `q                 �  �.stab           �"  	  �V     "      B.stabstr        & �+                 B.rdata          �   �Q             @  @/4              (   {R               �.ctors             �R �W        @  �.dtors             �R �W        @  �U���(�E�$�    H�E��EH�E�E�D$�E��$�    � �E��E��E��E��E�    �E�;E�s9�}� t3�E��D$�E�$�    ���E�U���:���E�E���E�� 뿃}� t4�}� t.�E��D$�E�$�    ���E�U���:���E�E�����D$    �E�$�    ���E�U���:�E���E���E� �E�E��E���U���H�$   �    �(q �(q f�   �(q f�@  �(q f�@��(q f�@��$    �    ��y-�    �D$�D$    �    ��@�$�    �$   �    �$    �    �D$  @�D$   �D$�  �$�  �    � q �= q  u-�    �D$�D$   �    ��@�$�    �$   �    � q �@�,q � q �@�@�0q �0q ����Ѝ�    Љ����,q )У4q �    ���D$�   ���D$�    ���D$� q �@�$�    �<q �   ���D$�   ���D$�   ���D$� q �@�$�    �@q �8q  ��  �=8q  ��   �E؉$�    �E؉Eԃ}�t�}��}�|��X�}�tR���U޸�  )�P�$�d$�<   ���@   ���\$�E�P�$�d$�<   ���@   ���$�D   �u����(q �$�    � q �$�    �$q �$�    �8q �B����    �    ��U���(�E�    � q �@�E���  ��u��   �   ��E��E�    �}��  ��   �   ��E��E�    �}�  h�E�� �0q �E��E�    �8 u6�E��D$�E��D$�E�D$�E�$�E  ��t�U�<q ���E��  �E��D   ���]��E�� 돋4q �E��E��D   ���]��E�� �P����x  � q �$�    �ÐU���(� q �@�E��E�    �   ��E��E�    �}��  ��   �   ��E��E�    �}�  d�0q �E��E�� �    �D$�    �D$�E��D$�E��$��   ��~�E�@q ��E�    � �E��H   ���]��E�� 듋4q �E��E��H   ���]��E�� �T���� q �$�    ��U���(�E�M�]��E�M�]��E�    �}�]�E�E��E��e��E�]�E����M�E�]�E�M�U��E�M�U����$�    �P   �������s�	�E�    ��E�� ��E�   �E���U���(�E�M�]��E�M�]��E�   �}�\�E�E��E��e��E�]�E����M�E�]�E�M�U��E�M�U����$�    �X   �������s��E��E���E�� ��E������E��ÐU���� q �x u� q � %@  ��u�;� q �$�    ��y*�    �D$�D$`   �    ��@�$�    �E�    ��E�   �E���U���� q �x u� q � %@  ��u�� q �$�    ��U����}��  u�}u�$Hq �    �}��  u�} u�$Hq �    ��U����D$��  �$   ������U����D$��  �$    �����Ð��������������   �   �   �        �&    d       &   d       1   <       @   �       �   �       �   �       �   �       �   �       �   �       	  �       "  �       K  �       u  �       �  �       �  �       &  �       y  �       �  �       �  �       #  �       .  �       J  �       l  �       �  �       �  �       �  �         �       %  �       G  �       k  �       �  �       �  � �     �  � �         �       �  �      �  �         � &     +  � )     >  � 1     S  � 7     f  � >     {  � A     �  � H     �  � K     �  � R     �  � U     �  � \     �  � _       � e     +  � h     B  � n     Z  � s         �       q  �       �  �       �  � b        �       �  �         � ,         �       "  � �     	  � �     &	  � J        �       <	  �       X	  � x    �
  � y    :  � �    b  � �        �       �  �         � j         �       3  �       Q  �       m  �      �  �      �  �      �  �      �  �      �  �       �  � !       � "     &  � %     B  � &     `  � '     ~  � (     �  � )     �  � *     �  � +     �  � ,       � 2     2  � 3     O  � 4     l  � 5     �  � 6     �  � 7     �  � 8     �  � 9     �  � <       � =     1  � @     J  � A         �       d  �          �       �  �         � �         �       -  � P     W  � S     m  � U     �  � V     �  � W     �  � X     �  � Y     �  � Z     �  � ]     �  � _       � m     k  � n     �  � o     �  � p     �  � q     �  � r     
  � s     *  � t     J  � �     w  � �     �  � ?        �           �       �  �       �  �         � 3         �       ~  �       �  � 7     �  � Q     �  � {         �         �       7  � -         �       _  �       �  � )     �$  � \         �       �$  � *     %'  � :     C'  � Q     �)  � ]     *  � �         �       i*  �       �*  � 8     �*  � =     �,  � C     �,  � F     �.  � O         �       �.  �       /  �       B/  �       f/  �     �=  � 0        �       s>  � 6     ;@  � ;         �       U@  �       x@  �       �@  � +     ?B  � .     WB  � 0     D  � 5     /D  � 8     �E  � ;     F  � >     I  � S     5I  � X     %L  � r     BL  � �     lL  � �     hO  � �     �O  � �     6R  � �     SR  � �     �S  � i        �       PT  � &     tT  � '     �V  � .         �       �V  �       �V  � ,         �       
W  � N     //can work with 30 fps or more if resolution is lowered.  iters can also be lowered for lower 
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

L     @  1     .text           P  �   �      ^      `.data                            @  �.bss            `S                 �  �.rdata          �                 @  @U�    ��1��E�E�E�E���&    ��B���Ƀ�O������������%�����������������v���������������]1������������]�   É�U1ɉ�W�   �V1�S���    �   �   ��ɉ}�1��E��]��E��E�    ������E��������ʃ}�O�������  �������������������v���������1���tƁ@   �   BA��  ~������   F��W  �w������؋=    �!  �5   �   �_�   �ڻ  ��@    t\����� t?�� ��� t6��!��� t-��?    t$��A    t��_   t��`   t	��a   u�    ��5   Ɓ@   �AKy��   G���pځ�V  �v����    �$�    ��[^_]����ظ   ��������    ��    U����    �P��u� @  t�$�    ��xɸ   ��    �D$�   �L$�    ��@�$�    �1�Ð��    ��    U����    �H��u� @  t�$�    �Í�    ��    U��WV1�S���    �E�P�X�]����E�]�u� @  t�$�    ���  �   ����   �(   1��  �@�   ��U�1��E耾@   tb�E�    ���M��E��%���E����������ʃ}�O�E����E���   ���������������������v�����������1���tl�   ��,   B�   FÁ�  �z������,   G�   Ӂ�W  �P����ء    �X��u]� @  uU�E��[^_]�    ��[^_]�� ������ظ   �{����    �D$�    �   �L$��@�$�    1�������$�    �    뜍�&    ��'    U��S��4�$   �    f�   f�@ f�@X�   �$    �    ����  �$    �X  �    �\$�  @�   �L$�T$�$   �    �    ������  �X�@�P�   ���   �����   )Ӊ   �@S �DS �D$�T$�\$�A�$�    �   �    �    �   �   �T$�\$�D$�Q�$�    �    �    �5����=    ul�]����&    ��t!�=    uR�$�    �E؃�tQ߃�|ߋ   �$�    �    �$�    �   �$�    �   �=    t���    ��41�[]Ív �U޸X  �M�)�P�$���l   �p   �\$Q�$���l   �t   �$�����O����    �D$�0   �D$�    ��@�$�    �$   �    �    �D$�M   �ϐ�����������   �   �   �     �@  ��  �@
ף;Can't lock screen: %s
    ��
ף;Couldn't initialize SDL: %s
 Couldn't set video mode: %s
   
ף;  ��   �               �       �       �       �             $      /      5      L      U      ^      g      p      y      �      �      �      �      �      �      �      �      �  !            "    .  #    7      @  $    K  /    g      ~  %    �      �  "    �      �      Z      b      h            �      �      �  #    �  $    �      �  /      %          /  0    E      Q  &    `  '    j  -    �  (    �      �      �      �      �      �      �      �  )    �      �                        &  )    +      1      =      W      b  *    x      �  ,    �      �  +    �      �  +    �      �      �  '    �      �      �      �        #          # Project: julia5
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++.exe
CC   = gcc.exe
WINDRES = windres.exe
RES  = 
OBJ  = julia5b.o $(RES)
LINKOBJ  = julia5b.o $(RES)
LIBS =  -L"d:/Dev-Cpp/lib" -mwindows -lmingw32 -lSDLmain -lSDL  -s 
INCS =  -I"d:/Dev-Cpp/include" 
CXXINCS =  -I"d:/Dev-Cpp/lib/gcc/mingw32/3.4.2/include"  -I"d:/Dev-Cpp/include/c++/3.4.2/backward"  -I"d:/Dev-Cpp/include/c++/3.4.2/mingw32"  -I"d:/Dev-Cpp/include/c++/3.4.2"  -I"d:/Dev-Cpp/include" 
BIN  = julia5.exe
CXXFLAGS = $(CXXINCS)    -fexpensive-optimizations -O3
CFLAGS = $(INCS)   -fexpensive-optimizations -O3
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before julia5.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "julia5.exe" $(LIBS)

julia5b.o: julia5b.cpp
	$(CPP) -c julia5b.cpp -o julia5b.o $(CXXFLAGS)
MZ�       ��  �       @                                   �   � �	�!�L�!This program cannot be run in DOS mode.
$       PE  L .xG        � #8 �  �  �  `         h                    �    �T                         P e   p 0   �                    � �                                                                                   .text   t�     �                `  `.data   P     