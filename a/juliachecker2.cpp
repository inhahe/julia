// juliachecker2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>




LRESULT CALLBACK event_handler(HWND hWind, UINT msg, WPARAM wParam, LPARAM lParam)
{
return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	
	
	WNDCLASS wndclass;
	wndclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = event_handler;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "WINCLASS1";
	
	HWND handle = CreateWindow("WNDCLASS1", "whatever", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	DDSURFACEDESC surface;
    surface.dwSize = sizeof(surface);
	surface.dwFlags = DDSD_CAPS;
	surface.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	LPDIRECTDRAWSURFACE surface2;

	LPDIRECTDRAW lpdd;
	int n;
	char nn[10];
	n = DirectDrawCreate(NULL, &lpdd,NULL);
	
	itoa(n, nn, 10);
    MessageBox( NULL, nn, "Hi!", MB_OK );

	n = lpdd->SetCooperativeLevel(handle, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);

    itoa(n, nn, 10);
    MessageBox( NULL, nn, "Hi!", MB_OK );

	n = lpdd->SetDisplayMode(640,480,8);
    
	itoa(n, nn, 10);
    MessageBox( NULL, nn, "Hi!", MB_OK );

	
	n = lpdd->CreateSurface(&surface, &surface2, NULL);

    itoa(n, nn, 10);
    MessageBox( NULL, nn, "Hi!", MB_OK );

	n = surface2->Release();

    itoa(n, nn, 10);
    MessageBox( NULL, nn, "Hi!", MB_OK );


	n = lpdd->Release();

    itoa(n, nn, 10);
    MessageBox( NULL, nn, "Hi!", MB_OK );
	
	return 0;
}



