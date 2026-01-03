#pragma once

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#define DLL_CALL __stdcall
#else
#define DLL_EXPORT extern "C"
#define DLL_CALL
#endif

// isCall: 1=Call, 0=Put
DLL_EXPORT int DLL_CALL LookbackPriceGreeks(
    double S0, double r, double sigma, double T,
    int isCall,
    int steps, int paths,
    double h_delta, double h_gamma, double dt_theta, double h_rho, double h_vega,
    unsigned int seed,
    double* price, double* delta, double* gamma,
    double* theta, double* rho, double* vega
);

// Per i grafici (riempie array): S[i], Price[i], Delta[i]
// nPoints = numero punti richiesti
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

// utility: calcola quanti punti servono per Smin..Smax step dS
DLL_EXPORT int DLL_CALL LookbackCountPoints(double Smin, double Smax, double dS);
