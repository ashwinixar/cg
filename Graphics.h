#pragma once

#include <d2d1.h>
#include <vector>

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;

	D2D1_COLOR_F GetBrushColor();
	void SetBrushColor(D2D1_COLOR_F color);
	void SetBrushColor(float r, float g, float b, float a);

	void Swap(float& a, float& b);

	void CirclePlotPoints(float xc, float yc, float x, float y);
	void EllipsePlotPoints(float xc, float yc, float x, float y);

	void BoundaryFill4(float x, float y, D2D1::ColorF fill, D2D1::ColorF boundary);
	void BoundaryFill8(float x, float y, D2D1::ColorF fill, D2D1::ColorF boundary);
public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }

	void ClearScreen();
	void DrawPoint(float x, float y);
	void DrawPoints(std::vector<std::pair<float, float>> points, std::vector<D2D1::ColorF> intensity);

	void LineDDA_AA(float xa, float ya, float xb, float yb);
	void LineDDA(float xa, float ya, float xb, float yb);
	void LineBresenham(float xa, float ya, float xb, float yb);
	void LineMidpoint(float xa, float ya, float xb, float yb);
	void LineMidpoint_AA(float xa, float ya, float xb, float yb);
	void CircleMidpoint(float xc, float yc, float r);
	void EllipseMidpoint(float xc, float yc, float rx, float ry);
	void Polygon(std::vector<std::pair<float, float>> points);
	void BoundaryFill(float x, float y, D2D1::ColorF fill, D2D1::ColorF boundary, bool Fill8);
};