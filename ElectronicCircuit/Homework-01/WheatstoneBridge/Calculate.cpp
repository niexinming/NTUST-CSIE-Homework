#include "stdafx.h"
#include "Calculate.h"

void Calculate(int R1, int R2, int R3, int R4, int Vin,
			   double& V1, double& V2, double& V3, double& V4, double& Vo,
			   double& I1, double& I2, double& I3, double& I4)
{
	// TODO
	Vin *= 1e3; // Assume Vin is V, Convert to mV
	I1 = I2 = (double)Vin / (R1 + R2);
	I3 = I4 = (double)Vin / (R3 + R4);

	V1 = I1 * R1;
	V2 = I2 * R2;
	V3 = I3 * R3;
	V4 = I4 * R4;

	Vo = V2 - V4;
}