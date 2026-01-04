#include "pch.h"
#include "pricer_api.h"
#include "LookbackOption.h"

// Convert an integer flag (from VBA) into the internal C++ enum
static Option toOption(int isCall)
{
    if (isCall == 1)
        return Option::Call;
    else
        return Option::Put;
}

// LookbackPriceGreeks
// Public DLL entry point: computes Monte Carlo price and Greeks for a lookback option.
// Results are returned via output pointers so that multiple values can be returned to callers like VBA/Excel.

// Return codes:
//  0  = success
// -1  = null output pointers
// -2  = invalid model inputs (S0, sigma, T)
// -3  = invalid Monte Carlo settings (steps, paths)
int DLL_CALL LookbackPriceGreeks(
    double S0, double r, double sigma, double T,
    int isCall,
    int steps, int paths,
    double h_delta, double h_gamma, double dt_theta, double h_rho, double h_vega,
    unsigned int seed,
    double* price, double* delta, double* gamma,
    double* theta, double* rho, double* vega
) {
    //checks
    if (!price || !delta || !gamma || !theta || !rho || !vega) return -1;
    if (S0 <= 0.0 || sigma <= 0.0 || T <= 0.0) return -2;
    if (steps <= 0 || paths <= 0) return -3;

    Option type = toOption(isCall);

    // Price
    *price = price_lookback(type, S0, r, sigma, T, steps, paths, seed);

    // Greeks 
    *delta = delta_lookback(type, S0, r, sigma, T, steps, paths, h_delta, seed);
    *gamma = gamma_lookback(type, S0, r, sigma, T, steps, paths, h_gamma, seed);
    *theta = theta_lookback(type, S0, r, sigma, T, steps, paths, dt_theta, seed);
    *rho = rho_lookback(type, S0, r, sigma, T, steps, paths, h_rho, seed);
    *vega = vega_lookback(type, S0, r, sigma, T, steps, paths, h_vega, seed);

    return 0;
}


// LookbackCountPoints
// Compute how many points are needed for a grid Smin..Smax with step dS.

// Return codes:
//  n>=0 = number of points
//  -1   = invalid step (dS <= 0)
//  -2   = invalid range (Smax < Smin)
int DLL_CALL LookbackCountPoints(double Smin, double Smax, double dS) {
    if (dS <= 0.0) return -1;
    if (Smax < Smin) return -2;

    // includiamo entrambi gli estremi come fai nel for (S0 <= Smax)
    double span = Smax - Smin;
    int n = (int)std::floor(span / dS + 1.0 + 1e-12);
    return std::max(n, 0);
}

// LookbackPriceDeltaCurve
// Public DLL entry point for plotting: fills pre-allocated arrays with spot values, option prices, and deltas over a spot grid.

// Return codes:
//  0  = success
// -1  = null output pointers
// -2  = invalid nPoints
// -3  = invalid grid definition (or computed expected points <= 0)
// -4  = nPoints too small for the requested grid

int DLL_CALL LookbackPriceDeltaCurve(
    double Smin, double Smax, double dS,
    double r, double sigma, double T,
    int isCall,
    int steps, int paths,
    double h_delta,
    unsigned int seed,
    double* outS, double* outPrice, double* outDelta,
    int nPoints
) {
    if (!outS || !outPrice || !outDelta) return -1;
    if (nPoints <= 0) return -2;

    int expected = LookbackCountPoints(Smin, Smax, dS);
    if (expected <= 0) return -3;
    if (nPoints < expected) return -4;

    Option type = toOption(isCall);

    int k = 0;
    for (double S0 = Smin; S0 <= Smax + 1e-12; S0 += dS) {
        if (k >= nPoints) break;

        outS[k] = S0;

        // price
        outPrice[k] = price_lookback(type, S0, r, sigma, T, steps, paths, seed);

        // delta
        unsigned int local_seed = seed + (unsigned int)(k + 1);
        double h;
        if (h_delta > 0.0)
            h = h_delta;
        else
            h=0.02 * S0;

        outDelta[k] = delta_lookback(type, S0, r, sigma, T, steps, paths, h, local_seed);

        k++;
    }

    return 0;
}
