#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
	bitmap = NULL;
	size = D2D1::SizeU(0, 0);
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
	if (bitmap) bitmap->Release();
}

bool Graphics::Init(HWND windowHandle)
{
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (result != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);
	size = D2D1::SizeU(
		rect.right - rect.left, 
		rect.bottom - rect.top
	);

	result = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, 
			size),
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

D2D1_COLOR_F Graphics::GetBrushColor()
{
	return brush->GetColor();
}

void Graphics::SetBrushColor(D2D1_COLOR_F color)
{
	brush->SetColor(color);
}

void Graphics::SetBrushColor(float r, float g, float b, float a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));
}

void Graphics::ClearScreen()
{
	//renderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
	renderTarget->Clear(D2D1::ColorF(0.2f, 0.5f, 0.8f));
}

void Graphics::DrawPoint(float x, float y)
{
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 0.5f, 0.5f), brush, 1.0f);
}

void Graphics::DrawPoints(std::vector<std::pair<float, float>> points, std::vector<D2D1::ColorF> intensity)
{
	D2D1_COLOR_F oldBrushColor = GetBrushColor();
	for (int it = 0; it < points.size(); it++)
	{
		SetBrushColor(intensity[it]);
		DrawPoint(points[it].first, points[it].second);
	}
	SetBrushColor(oldBrushColor);
}

void Graphics::LineDDA(float xa, float ya, float xb, float yb)
{
	float dx = xb - xa, dy = yb - ya, steps;
	float xInc, yInc, x = xa, y = ya;

	if (abs(dx) > abs(dy)) steps = abs(dx);
	else steps = abs(dy);
	xInc = dx / steps;
	yInc = dy / steps;
	DrawPoint(ROUND(x), ROUND(y));
	for (int k = 0; k < steps; k++)
	{
		x += xInc;
		y += yInc;
		DrawPoint(ROUND(x), ROUND(y));
	}
}

void Graphics::LineDDA_SSAA3x3(float xa, float ya, float xb, float yb)
{
	//TODO
	std::vector<std::pair<float, float>> points;
	std::vector<D2D1::ColorF> intensity;

	D2D1::ColorF pixel_intensity(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	float dx = xb - xa, dy = yb - ya, steps;
	float xInc, yInc, x = xa, y = ya;

	if (abs(dx) > abs(dy)) steps = abs(dx);
	else steps = abs(dy);
	xInc = dx / steps;
	yInc = dy / steps;
	points.push_back(std::make_pair(ROUND(x), ROUND(y)));
	intensity.push_back(pixel_intensity);
	for (int k = 0; k < steps; k++)
	{
		x += xInc;
		y += yInc;
		points.push_back(std::make_pair(ROUND(x), ROUND(y)));
		intensity.push_back(pixel_intensity);
	}
	DrawPoints(points, intensity);
}

void Graphics::LineBresenham(float xa, float ya, float xb, float yb)
{
	float dx = abs(xa - xb), dy = abs(ya - yb);
	float p = 2 * dy - dx;
	float twoDy = 2 * dy, twoDyDx = 2 * (dy - dx);
	float x, y, xEnd;

	if (xa > xb)
	{
		x = xb;
		y = yb;
		xEnd = xa;
	}
	else
	{
		x = xa;
		y = ya;
		xEnd = xb;
	}
	DrawPoint(x, y);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyDx;
		}
		DrawPoint(x, y);
	}
}

void Graphics::Swap(float& a, float& b)
{
	float temp = a;
	a = b;
	b = temp;
}

void Graphics::LineMidpoint(float xa, float ya, float xb, float yb)
{
	float dx = xb - xa;
	float dy = yb - ya;
	float x = xa, y = ya;
	float d = 0;
	bool swapped = false;

	if (dy > dx)
	{
		d = dx - (dy / 2);
		Swap(x, y);
		Swap(dx, dy);
		Swap(xb, yb);
		swapped = true;
	}
	else d = dy - (dx / 2);
	
	if(!swapped) DrawPoint(x, y);
	else DrawPoint(y, x);
	while (x < xb)
	{
		x++;
		if (d > 0)
		{
			y++;
			d = d + (dy - dx);
		}
		else d = d + dy;
		if (!swapped) DrawPoint(x, y);
		else DrawPoint(y, x);
	}
	if (!swapped) DrawPoint(xb, yb);
	else DrawPoint(yb, xb);
}

void Graphics::LineMidpoint_GuptaSproullAA(float xa, float ya, float xb, float yb)
{
	//TODO
	float dx = xb - xa;
	float dy = yb - ya;
	float x = xa, y = ya;
	float d = 0;
	float D = 0, Dlower = 0, Dupper = 0;
	float num = 0;
	float denom = 2 * (sqrtf(dx * dx + dy * dy));
	bool swapped = false;

	std::vector<std::pair<float, float>> points;
	std::vector<D2D1::ColorF> intensity;

	D2D1::ColorF pixel_intensity(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));

	if (dy > dx)
	{
		d = dx - (dy / 2);
		Swap(x, y);
		Swap(dx, dy);
		Swap(xb, yb);
		swapped = true;
	}
	else d = dy - (dx / 2);

	if (!swapped) points.push_back(std::make_pair(ROUND(x), ROUND(y)));
	else points.push_back(std::make_pair(ROUND(y), ROUND(x)));
	intensity.push_back(pixel_intensity);

	while (x < xb)
	{
		x++;
		if (d >= 0)
		{
			y++;
			d = d + (dy - dx);
		}
		else d = d + dy;

		if (!swapped) points.push_back(std::make_pair(ROUND(x), ROUND(y)));
		else points.push_back(std::make_pair(ROUND(y), ROUND(x)));
		intensity.push_back(pixel_intensity);
	}

	if (!swapped) points.push_back(std::make_pair(ROUND(x), ROUND(y)));
	else points.push_back(std::make_pair(ROUND(y), ROUND(x)));
	intensity.push_back(pixel_intensity);

	DrawPoints(points, intensity);
}

void Graphics::CirclePlotPoints(float xc, float yc, float x, float y)
{
	DrawPoint(xc + x, yc + y);
	DrawPoint(xc - x, yc + y);
	DrawPoint(xc + x, yc - y);
	DrawPoint(xc - x, yc - y);
	DrawPoint(xc + y, yc + x);
	DrawPoint(xc - y, yc + x);
	DrawPoint(xc + y, yc - x);
	DrawPoint(xc - y, yc - x);
}

void Graphics::CircleMidpoint(float xc, float yc, float r)
{
	float x = 0;
	float y = r;
	float p = 1 - r;
	
	CirclePlotPoints(xc, yc, x, y);

	while (x < y)
	{
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else
		{
			y--;
			p += 2 * (x - y) + 1;
		}
		CirclePlotPoints(xc, yc, x, y);
	}
}

void Graphics::EllipsePlotPoints(float xc, float yc, float x, float y)
{
	DrawPoint(xc + x, yc + y);
	DrawPoint(xc - x, yc + y);
	DrawPoint(xc + x, yc - y);
	DrawPoint(xc - x, yc - y);
}

void Graphics::EllipseMidpoint(float xc, float yc, float rx, float ry)
{
	float rx2 = rx * rx;
	float ry2 = ry * ry;
	float twoRx2 = 2 * rx2;
	float twoRy2 = 2 * ry2;
	float p;
	float x = 0;
	float y = ry;
	float px = 0;
	float py = twoRx2 * y;

	EllipsePlotPoints(xc, yc, x, y);

	/*Region 1*/
	p = ROUND(ry2 - (rx2 * ry) + (0.25 * rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += ry2 + px - py;
		}
		EllipsePlotPoints(xc, yc, x, y);
	}

	/*Region 2*/
	p = ROUND(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
	while (y > 0)
	{
		y--;
		py -= twoRx2;
		if (p > 0)
			p += rx2 - py;
		else
		{
			x++;
			px += twoRy2;
			p += rx2 - py + px;
		}
		EllipsePlotPoints(xc, yc, x, y);
	}
}

void Graphics::Polygon(std::vector<std::pair<float, float>> points)
{
	for (int it = 1; it < points.size(); it++)
	{
		LineDDA(points[it - 1].first, points[it - 1].second, points[it].first, points[it].second);
	}
	LineDDA(points.back().first, points.back().second, points[0].first, points[0].second);
}

void Graphics::BoundaryFill(float x, float y, D2D1::ColorF fill, D2D1::ColorF boundary, bool Fill8)
{
	D2D1_COLOR_F oldBrushColor = GetBrushColor();

	SetBrushColor(fill);
	if (Fill8) BoundaryFill8(x, y, fill, boundary);
	else BoundaryFill4(x, y, fill, boundary);

	SetBrushColor(oldBrushColor);
}

void Graphics::BoundaryFill4(float x, float y, D2D1::ColorF fill, D2D1::ColorF boundary)
{
	//TODO
	//D2D1_COLOR_F current;
}

void Graphics::BoundaryFill8(float x, float y, D2D1::ColorF fill, D2D1::ColorF boundary)
{
	//TODO
}

void Graphics::CreateBitmap()
{
	D2D1_BITMAP_PROPERTIES bitmapProperties = D2D1::BitmapProperties(
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
	);
	renderTarget->CreateBitmap(
		size,
		nullptr, 
		0, 
		bitmapProperties, 
		&bitmap
	);
	
	ID3D10Texture2D* d3dTexture;
	IDXGISurface* sharedSurface;
	
	//TODO
}