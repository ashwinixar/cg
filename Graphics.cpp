#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
}

bool Graphics::Init(HWND windowHandle)
{
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (result != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	result = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)),
		&renderTarget);

	if (result != S_OK) return false;

	result = renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(
			0.0f, //r
			0.0f, //g
			0.0f, //b
			1.0f), //a
		&brush);

	if (result != S_OK) return false;

	return true;
}

void Graphics::ClearScreen()
{
	renderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
}

void Graphics::DrawPoint(float x, float y)
{
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 0.5f, 0.5f), brush, 1.0f);
}