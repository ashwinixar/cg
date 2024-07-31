#include <Windows.h>
#include <wincodec.h>
#include <vector>
#include "Graphics.h"

#define WIDTH 640
#define HEIGHT 480

Graphics* graphics;

float y = 0.0f;
float ySpeed = 0.0f;
void update()
{
	ySpeed += 1.0f;
	y += ySpeed;
	if (y > 600) {
		y = 600;
		ySpeed = -30.0f;
	}
}

void render()
{
	graphics->BeginDraw();
	graphics->ClearScreen();

	//Graphics coding starts here
	
	//graphics->DrawPoint(300.0f, y); //Call along with update()
	//graphics->LineDDA(400, 100, 500, 200);
	//graphics->LineBresenham(200, 200, 300, 300);
	//graphics->LineMidpoint(100, 200, 200, 300);
	//graphics->CircleMidpoint(200, 200, 100);
	//graphics->EllipseMidpoint(300, 300, 50, 20);
	/*
	std::vector<std::pair<float, float>> points;
	points.push_back({ 100, 100 });
	points.push_back({ 200, 100 });
	points.push_back({ 150, 150 });
	graphics->Polygon(points);
	*/
	graphics->LineDDA_AA(50, 50, 80, 150);
	//graphics->LineMidpoint_AA(80, 50, 110, 150);
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
	/*
	if (uMsg == WM_PAINT)
	{
		graphics->BeginDraw();
		graphics->ClearScreen();

		//Graphics coding starts here
		//graphics->LineBresenham(200, 200, 300, 300);
		//graphics->CircleMidpoint(200, 200, 100);
		//graphics->EllipseMidpoint(300, 300, 50, 20);
		std::vector<std::pair<int, int>> points;
		points.push_back({ 100, 100 });
		points.push_back({ 200, 100 });
		points.push_back({ 150, 150 });
		graphics->Polygon(points);
		//Graphics coding ends here

		graphics->EndDraw();
	}
	*/
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

	ShowWindow(windowHandle, nShowCmd);

	/*
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		DispatchMessage(&message);
	}
	*/
	
	MSG message;
	message.message = WM_NULL;
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&message);
		else
		{
			//update(); //Update Graphics
			render(); //Render Graphics
		}
			
	}

	delete graphics;

	return 0;
}