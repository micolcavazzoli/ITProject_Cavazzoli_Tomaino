#pragma once
#ifndef PLOTS_H
#define PLOTS_H
#include "LookbackOption.h"

void plot_price_S0(Option type, double r, double sigma, double T, int steps, int paths, double Smin, double Smax, double ds, unsigned int seed);
void plot_delta_S0(Option type, double r, double sigma, double T, int steps, int paths, double Smin, double Smax, double ds, unsigned int seed);

#endif // !PLOTS_H


