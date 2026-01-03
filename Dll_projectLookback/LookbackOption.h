#pragma once
#ifndef LOOKBACKOPTION_H
#define LOOKBACKOPTION_H
#include "pch.h"
#include <random>
#include <iostream>

enum class Option {
	Call,
	Put
};

double price_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, unsigned int seed);
double delta_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed);
double gamma_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed);
double theta_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double dt, unsigned int seed);
double rho_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed);
double vega_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed);


#endif // !LOOKBACKOPRION_H

