#pragma once
#ifndef LOOKBACKOPTION_H
#define LOOKBACKOPTION_H
#include <cmath>
#include <algorithm>
#include <random>
#include <iostream>

enum class Option {
	Call,
	Put
};

struct LookbackParams {
	Option type;
	double S0;
	double r;
	double sigma;
	double T;

	//Monte Carlo
	int steps;
	int paths;
	unsigned int seed;
};

struct GreeksParams {
	double hS; //delta
	double hg; //gamma
	double dt; //theta
	double hR; //rho
	double hV; //vega
};

class LookbackOption {
private:

	//Function to simulate a single path of the option lookback
	double simulate_lookback(Option type, double S0, double r, double sigma, double T, int steps, std::mt19937& gen, std::normal_distribution<>& Z) const;
	
public:
	//Price simulation with Monte Carlo
	double price_lookback(const LookbackParams& p) const;

	//Greeks

    //Delta of the option
	double delta_lookback(const LookbackParams& p, const GreeksParams& g) const;

	//Gamma of the option
	double gamma_lookback(const LookbackParams& p, const GreeksParams& g) const;

	//Theta of the option
	double theta_lookback(const LookbackParams& p, const GreeksParams& g) const;

	//Rho of the option
	double rho_lookback(const LookbackParams& p, const GreeksParams& g) const;

	//Vega of the option
	double vega_lookback(const LookbackParams& p, const GreeksParams& g) const;
};

#endif // !LOOKBACKOPRION_H

