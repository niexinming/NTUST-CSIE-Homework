#ifndef CALC_H
#define CALC_H

#define DeltaT 100
#define SamplesToKeep 128
#define SINE_WAVE 0
#define SQUARE_WAVE 1
#define TRIANGLE_WAVE 2

#include <vector>
using namespace std;

struct Pt
{
	int x, y;
};

double SinWave(double Vp, double freq, double t, double theta);
double SquareWave(double Vp, double freq, double t, double theta);
double TriangleWave(double Vp, double freq, double t, double theta);

void Calculate(int type, double amplitude, double frequency, double phase,
			   double t_start, double delta_t, int nSamplesToKeep, vector<double>& wave);

void Compute(double R1, double R2, double RC, double RE, double beta,
			 int nSamplesToKeep, vector<double>& wave, vector<double>& output);

#endif