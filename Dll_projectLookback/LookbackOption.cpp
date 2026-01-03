#include "pch.h"
#include "LookbackOption.h"
#include <algorithm>
#include <cmath>

//Function to simulate a single path of the option lookback
double simulate_lookback(double S0, double r, double sigma, double T, int steps, Option type, std::mt19937& gen, std::normal_distribution<>& Z) {

    double dt = T / steps;
    double S = S0;
    double minS = S0;
    double maxS = S0;

    for (int i = 0; i < steps; i++) {
        double z = Z(gen);
        S *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * z);
        minS = std::min(minS, S);
        maxS = std::max(maxS, S);
    }
    if (type == Option::Call)
        return S - minS;  //payoff call lookback
    else
        return maxS - S;  //payoff put lookback
}


//Price simulation with Monte Carlo
double price_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, unsigned int seed) {

    std::mt19937 gen(seed);
    std::normal_distribution<> Z(0.0, 1.0);

    double sum = 0.0;
    for (int i = 0; i < paths; i++)
        sum += simulate_lookback(S0, r, sigma, T, steps, type, gen, Z);

    return std::exp(-r * T) * (sum / paths);
}

//Greeks

//Delta of the option
double delta_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed) {

    double P_plus = price_lookback(type, S0 + h, r, sigma, T, steps, paths, seed);
    double P_minus = price_lookback(type, S0 - h, r, sigma, T, steps, paths, seed);

    return (P_plus - P_minus) / (2.0 * h);
}

//Gamma of the option
double gamma_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed) {

    double P_plus = price_lookback(type, S0 + h, r, sigma, T, steps, paths, seed);
    double P = price_lookback(type, S0, r, sigma, T, steps, paths, seed);
    double P_minus = price_lookback(type, S0 - h, r, sigma, T, steps, paths, seed);

    return (P_plus - 2.0 * P + P_minus) / (h * h);
}

//Theta of the option
double theta_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double dt, unsigned int seed) {

    double P_today = price_lookback(type, S0, r, sigma, T, steps, paths, seed);
    double P_future = price_lookback(type, S0, r, sigma, T - dt, steps, paths, seed);

    return (P_future - P_today) / dt; //negative if the price decreses wrt time
}

//Rho of the option
double rho_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed) {

    double P_plus = price_lookback(type, S0, r + h, sigma, T, steps, paths, seed);
    double P_minus = price_lookback(type, S0, r - h, sigma, T, steps, paths, seed);

    return (P_plus - P_minus) / (2.0 * h);
}

//Vega of the option
double vega_lookback(Option type, double S0, double r, double sigma, double T, int steps, int paths, double h, unsigned int seed) {

    double P_plus = price_lookback(type, S0, r, sigma + h, T, steps, paths, seed);
    double P_minus = price_lookback(type, S0, r, sigma - h, T, steps, paths, seed);

    return (P_plus - P_minus) / (2.0 * h);
}
