#include "stdafx.h"
#include "resource.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Gdiplus;
using namespace std;

void Calculate(int R, int L, int C, int f, double& I, double& Vo, double& Z)
{
	// for calculate the I, Vo and Z at frequency f

	double X_L = 2 * M_PI * f * L;
	double X_C = 1 / (2 * M_PI * f * C /*/ 1e6 */) * 1e6;
	Z = sqrt(pow(R, 2) + pow(X_L - X_C, 2));
	I = 100 / Z;
	Vo = I * R;

	I *= 1e3; // A to mA
}

void FequencyResponse(int R, int L, int C, int nSamplePerLogUnit,
					  vector<double>& f_res, vector<double>& I_res, vector<double>& Vo_res,
					  vector<double>& Z_res)
{
	// for calculate the samples response
	for (int i = 0; i < f_res.size(); i++)
	{
		double I = 0, Vo = 0, Z = 0;
		Calculate(R, L, C, f_res[i], I, Vo, Z);
		//const double CUT = 1e3; if (Z > CUT) Z = CUT;
		I_res.push_back(I);
		Vo_res.push_back(Vo);
		Z_res.push_back(Z);
	}
}

void Draw(HWND hWnd, int R, int L, int C)
{
	int i, j;
	double step, start, end;
	vector<double> f;
	vector<double> I;
	vector<double> Vo;
	vector<double> Z;
	double min_value, max_value;

	f.clear();
	I.clear();
	Vo.clear();
	Z.clear();

	start = 1;
	end = 10;
	for (i = 1 ; i < 7 ; ++i)
	{
		step = (end - start) / SamplePerLogUnit;

		for (j = 0 ; j < SamplePerLogUnit ; ++j)
			f.push_back(start + step * j);

		start *= 10;
		end *= 10;
	}

	FequencyResponse(R, L, C, SamplePerLogUnit, f, I, Vo, Z);

	Pt* pts = (Pt*) malloc(sizeof(struct Pt) * f.size());

	// Draw On First Picture
	min_value = 9999.0;
	max_value = -9999.0;
	for (i = 0 ; i < I.size() ; ++i)
	{
		if (I[i] < min_value)
			min_value = I[i];
		if (I[i] > max_value)
			max_value = I[i];
	}
	for (i = 0 ; i < f.size() ; ++i)
	{
		pts[i].x = int((127.0 / f.size()) * (double) i);
		pts[i].y = int(127.0 * (I[i] - min_value) / (max_value - min_value));
		pts[i].y = 127 - pts[i].y;
		if (pts[i].y < 8)
			pts[i].y = 8;
		else if (pts[i].y > 115)
			pts[i].y = 115;
	}
	DrawOnPicture(pts, f.size(), ::GetDC(::GetDlgItem(hWnd, IDC_I_PIC)));

	// Draw On Second Picture
	min_value = 9999.0;
	max_value = -9999.0;
	for (i = 0 ; i < Vo.size() ; ++i)
	{
		if (Vo[i] < min_value)
			min_value = Vo[i];
		if (Vo[i] > max_value)
			max_value = Vo[i];
	}
	for (i = 0 ; i < f.size() ; ++i)
	{
		pts[i].x = int((127.0 / f.size()) * (double) i);
		pts[i].y = int(127.0 * (Vo[i] - min_value) / (max_value - min_value));
		pts[i].y = 127 - pts[i].y;
		if (pts[i].y < 8)
			pts[i].y = 8;
		else if (pts[i].y > 115)
			pts[i].y = 115;
	}
	DrawOnPicture(pts, f.size(), ::GetDC(::GetDlgItem(hWnd, IDC_Vo_PIC)));

	// Draw On Third Picture
	min_value = 9999.0;
	max_value = -9999.0;
	for (i = 0 ; i < Z.size() ; ++i)
	{
		if (Z[i] < min_value)
			min_value = Z[i];
		if (Z[i] > max_value)
			max_value = Z[i];
	}
	for (i = 0 ; i < f.size() ; ++i)
	{
		pts[i].x = int((127.0 / f.size()) * (double) i);
		pts[i].y = int(127.0 * (Z[i] - min_value) / (max_value - min_value));
		pts[i].y = 127 - pts[i].y;
		if (pts[i].y < 8)
			pts[i].y = 8;
		else if (pts[i].y > 115)
			pts[i].y = 115;
	}
	DrawOnPicture(pts, f.size(), ::GetDC(::GetDlgItem(hWnd, IDC_Z_PIC)));

	free(pts);
}

void DrawOnPicture(Pt* data, int pt_size, HDC Target)
{
	Graphics g((HDC) Target);
	Pen BlackPen(Color(255, 0, 0, 0), 1);
	SolidBrush RedBrush(Color(255, 255, 0, 0));
	SolidBrush BkBrush(Color(255, 210, 225, 246));

	// Clear Region
	Region ClearRegion(Rect(0, 0, 128, 128));
	g.FillRegion(&BkBrush, &ClearRegion);
	g.DrawLine(&BlackPen, Point(0, 0), Point(127, 0));
	g.DrawLine(&BlackPen, Point(127, 0), Point(127, 127));
	g.DrawLine(&BlackPen, Point(0, 0), Point(0, 127));
	g.DrawLine(&BlackPen, Point(0, 127), Point(127, 127));

	// Draw Line based on data
	for (int i = 1 ; i < pt_size ; ++i)
	{
		g.DrawLine(&BlackPen, Point(data[i - 1].x, data[i - 1].y), Point(data[i].x, data[i].y));
	}
}