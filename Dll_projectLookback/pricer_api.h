#pragma once

// Cross-platform export/calling-convention macros.
// On Windows we must:
//  - export symbols from the DLL (__declspec(dllexport))
//  - use C linkage (extern "C") to avoid C++ name mangling (important for VBA/C/Python bindings)
//  - specify a calling convention (__stdcall) commonly expected by Windows/Excel/VBA

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#define DLL_CALL __stdcall
#else
#define DLL_EXPORT extern "C"
#define DLL_CALL
#endif

// Computes the Monte Carlo price and Greeks for a lookback option

// Inputs: S0, r, sigma, T, isCall, steps, paths, h_delta, h_gamma, h_theta, h_rho, h_vega, seed
// isCall: option type selector (1=Call, 0=Put)

// Outputs: price, delta, gamma, theta, rho, vega

// Return value: integer status code (0 = success, non-zero = error)
DLL_EXPORT int DLL_CALL LookbackPriceGreeks(
    double S0, double r, double sigma, double T,
    int isCall,
    int steps, int paths,
    double h_delta, double h_gamma, double dt_theta, double h_rho, double h_vega,
    unsigned int seed,
    double* price, double* delta, double* gamma,
    double* theta, double* rho, double* vega
);

// Utility function for plotting (fills arrays) 
// Computes price(S) and delta(S) over a grid of underlying prices

// Inputs: Smin, Smax, ds, r, sigma, T, isCall, steps, paths,  h_delta, h_gamma, h_theta, h_rho, h_vega, seed, n_poins
// nPoints = number of points allocated in output arrays

// Outputs: S[i], Price[i], Delta[i]

// Return value: integer status code (0 = success, non-zero = error)
DLL_EXPORT int DLL_CALL LookbackPriceDeltaCurve(
    double Smin, double Smax, double dS,
    double r, double sigma, double T,
    int isCall,
    int steps, int paths,
    double h_delta,
    unsigned int seed,
    double* outS, double* outPrice, double* outDelta,
    int nPoints
);

// Helper to compute how many grid points are needed for Smin..Smax with step dS.

// Return value: number of points required (or a negative value on invalid inputs)
DLL_EXPORT int DLL_CALL LookbackCountPoints(double Smin, double Smax, double dS);
