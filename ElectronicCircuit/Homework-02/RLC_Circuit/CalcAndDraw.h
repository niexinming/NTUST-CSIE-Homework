#ifndef _CalcAndDraw_
#define _CalcAndDraw_

#define SamplePerLogUnit 6

using namespace std;

struct Pt
{
	int x, y;
};

void Calculate(int R, int L, int C, int f, double& I, double& Vo, double& Z);

void FequencyResponse(double R, double L, double C, int nSamplePerLogUnit,
					  vector<double>& f_res, vector<double>& I_res, vector<double>& Vo_res,
					  vector<double>& Z_res);

void Draw(HWND hWnd, int R, int L, int C);

void DrawOnPicture(Pt* data, int pt_size, HDC Target);

#endif