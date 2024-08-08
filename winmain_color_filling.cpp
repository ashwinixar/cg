#include <Windows.h>
#include <wincodec.h>
#include <vector>
#include "Graphics.h"

#define WIDTH 640
#define HEIGHT 480

HDC hdc;

Graphics* graphics;

/*
float r = 20.0f;
float rChange = 1.0f;
float y = 0.0f;
float ySpeed = 0.0f;
float x = 0.0f;
float xLeftSpeed = -15.0f;
float xRightSpeed = 15.0f;
float xDir = 2.0f;
bool jump = false;
bool left = false;
bool right = false;
*/
void update()
{
	/*
	ySpeed += 1;
	y += ySpeed;
	if (y > 460)
	{
		y = 460;
		ySpeed = -20.0f;
	}
	x += xDir;
	if (x > 620)
	{
		x = 620;
		xDir = -2.0f;
	}
	else if (x < 20)
	{
		x = 20;
		xDir = 2.0f;
	}
	*/
}

void floodFill(int x, int y, COLORREF oldColor, COLORREF newColor)
{
	if (x < 0 || x >= WIDTH) return;
	if (y < 0 || y >= HEIGHT) return;
	if (GetPixel(hdc, x, y) == oldColor)
	{
		SetPixel(hdc, x, y, newColor);
		floodFill(x + 1, y, oldColor, newColor);
		floodFill(x - 1, y, oldColor, newColor);
		floodFill(x, y + 1, oldColor, newColor);
		floodFill(x, y - 1, oldColor, newColor);
	}
}

void render()
{
	graphics->BeginDraw();
	graphics->ClearScreen();

	//Graphics coding starts here

	graphics->CircleMidpoint(320, 240, 20);
	
	//Graphics coding ends here

	graphics->EndDraw();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	if (uMsg == WM_PAINT)
	{
		render();
		floodFill(320, 240, GetPixel(hdc, 320, 240), RGB(255, 0, 0));
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = L"MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

	RECT rect = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND windowHandle = CreateWindowEx(
		0,
		L"MainWindow",
		L"Computer Graphics Lab",
		WS_OVERLAPPEDWINDOW,
		100,
		50,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		0);

	if (!windowHandle) return -1;

	graphics = new Graphics();
	if (!graphics->Init(windowHandle))
	{
		delete graphics;
		return -1;
	}

	hdc = GetDC(windowHandle);

	ShowWindow(windowHandle, nShowCmd);
	
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		DispatchMessage(&message);
	}

	/*
	MSG message;
	message.message = WM_NULL;
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&message);
		}
		else
		{
			update(); //Update Graphics
			render(); //Render Graphics
		}

	}
	*/
	
	delete graphics;

	return 0;
}