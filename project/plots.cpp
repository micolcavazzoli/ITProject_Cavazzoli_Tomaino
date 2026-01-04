#include "plots.h"
#include <fstream>

void plot_price_S0(Option type, double r, double sigma, double T, int steps, int paths, double Smin, double Smax, double ds, unsigned int seed)
{
	std::ofstream file;

	if(type==Option::Call) //write a file with values of prices - S0
		file.open("price_S0_call.csv"); 
	else
		file.open("price_S0_put.csv");

	file << "S0, Price\n";

	for (double S0 = Smin; S0 <= Smax; S0+=ds) {
		double P = price_lookback(type, S0, r, sigma, T , steps, paths, seed);
		file << S0 << "," << P << "\n";
	}
	file.close();
}

void plot_delta_S0(Option type, double r, double sigma, double T, int steps, int paths, double Smin, double Smax, double ds, unsigned int seed)
{
	std::ofstream file;
	
	if(type==Option::Call) //write a file with values of delta - S0
		file.open("delta_S0_call.csv");
	else
		file.open("delta_S0_put.csv");

	file << "S0, delta\n";

	for (double S0 = Smin; S0 <= Smax; S0+=ds) {
		unsigned int local_seed = seed + (int)S0;
		double h_delta = 0.02 * S0;
		double delta = delta_lookback(type,S0, r, sigma, T, steps, paths, h_delta, local_seed);
		file << S0 << "," << delta << "\n";
	}
	file.close();
}


