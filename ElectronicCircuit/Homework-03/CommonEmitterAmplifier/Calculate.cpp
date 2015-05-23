#include "stdafx.h"
#include "Calculate.h"
#include "resource.h"

// Use constant: M_PI
#define _USE_MATH_DEFINES 
#include <math.h>

using namespace Gdiplus;
using namespace std;

//****************************************************************************
//
// * Generate the input voltage
//   Type: 0 for sine, 1 for square wave (duality cycle 50%) and
//         2 for triangle wave
//   amplitude is the amplitude of the function
//   phase: control the offset in radian (0 ~ 2 pi)
//   t: the current time
//   delta_t: the time interval between two consecutive samples
//   nSamplesToKeep: how many samples in the array
//   wave: the wave samples
//============================================================================

typedef double(*WaveGenerator)(double, double, double, double);

void Calculate(int type, double amplitude, double frequency, double phase,
			   double t_start, double delta_t, int nSamplesToKeep, vector<double>& wave)
{
	const WaveGenerator generators[] = { SinWave, SquareWave, TriangleWave };
	WaveGenerator generator = generators[type % 3];

	double t, Vo;
	for (int i = 0; i < nSamplesToKeep; ++i)
	{
		t = t_start + delta_t * i;
		wave.push_back(generator(amplitude, frequency / 1e7 /* Hack! */, t, phase));
	}
}

double SinWave(double Vp, double freq, double t, double theta)
{
	return Vp * sin(2 * M_PI * freq * t + theta);
}

double SquareWave(double Vp, double freq, double t, double theta)
{
	double offset = theta / 2 / M_PI;
	double pos = fmod((t*freq + offset) * 100, 100);
	return pos < 50 ? Vp : -Vp;
}

double TriangleWave(double Vp, double freq, double t, double theta)
{
	// The origin wave:       [ A single wave ]
    // +1.0k⢠⠞⢄                  |     ⡴⠣⡀                      ⢠⠞⢄                       
    //     ⡠⠃ ⠈⢦                 |   ⢀⠜  ⠱⡄                    ⡠⠃ ⠈⢦                    ⢀⠜
    //    ⡴⠁    ⠣⡀               |  ⢠⠎    ⠘⢄                  ⡴⠁    ⠣⡀                 ⢠⠎ 
    //  ⢀⠜       ⠱⡄              | ⡠⠃      ⠈⢦               ⢀⠜       ⠱⡄               ⡠⠃  
    // ⢠⠎         ⠘⢄             |⡴⠁         ⠣⡀            ⢠⠎         ⠘⢄             ⡴⠁   
    // 0            ⠣⡀         ⢠⠎|            ⠘⢄          ⡴⠁            ⠣⡀         ⢠⠎     
    //               ⠱⡄       ⡠⠃ |             ⠈⢦       ⢀⠜               ⠱⡄       ⡠⠃      
    //                ⠘⢄     ⡴⠁  |               ⠣⡀    ⢠⠎                 ⠘⢄     ⡴⠁       
    //                 ⠈⢦  ⢀⠜    |                ⠱⡄  ⡠⠃                   ⠈⢦  ⢀⠜         
    // -1.0k             ⠣⣠⠎     |                 ⠘⢄⡴⠁                      ⠣⣠⠎          
	// Adjust to this and it will be more easily to generate
	//                      [ A single wave ]
    // +1.0k                   ⢠⠎|⢦                       ⡴⠱⡄                      ⢠⠎⢦      
    // ⡇⠘⢄                    ⡠⠃ | ⠣⡀                   ⢀⠜  ⠘⢄                    ⡠⠃  ⠣⡀    
    // ⡇ ⠈⢦                  ⡴⠁  |  ⠱⡄                 ⢠⠎    ⠈⢦                  ⡴⠁    ⠱⡄   
    // ⡇   ⠣⡀              ⢀⠜    |   ⠘⢄               ⡠⠃       ⠣⡀              ⢀⠜       ⠘⢄  
    // ⡇    ⠱⡄            ⢠⠎     |    ⠈⢦             ⡴⠁         ⠱⡄            ⢠⠎         ⠈⢦ 
    // 0     ⠘⢄          ⡠⠃      |      ⠣⡀         ⢀⠜            ⠘⢄          ⡠⠃            ⠣
    //        ⠈⢦        ⡴⠁       |       ⠱⡄       ⢠⠎              ⠈⢦        ⡴⠁              
    //          ⠣⡀    ⢀⠜         |        ⠘⢄     ⡠⠃                 ⠣⡀    ⢀⠜                
    //           ⠱⡄  ⢠⠎          |         ⠈⢦   ⡴⠁                   ⠱⡄  ⢠⠎                 
    // -1.0k      ⠘⢄⡠⠃           |           ⠣⣀⠜                      ⠘⢄⡠⠃                  
	// f( 0%) =  Vp
	// f(50%) = -Vp
	//   Vp =  0% x + b ; b = Vp
	//  -Vp = 50% x + Vp
	// -2Vp = 50% x ; x = -4Vp

	double offset = theta / 2 / M_PI;

	// [0..100), stands for position of a single wave
	double pos = fmod((t * freq + offset) * 100 + 25, 100);
	double deltaY = (pos < 50) ? 25 - pos : pos - 75;
	return Vp * (-4) * deltaY / 100;
}

void Compute(double R1, double R2, double RC, double RE, double beta,
	int nSamplesToKeep, vector<double>& wave, vector<double>& output)
{
	double Av = -beta * RC / (1 + beta) / RE;
	for (int i = 0; i < nSamplesToKeep; ++i)
	{
		output.push_back(wave[i] * Av);
	}
}