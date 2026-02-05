#include "pricer_api.h"
#include "LookbackOption.h"
#include <exception>
#include <string>
#include <cstring>

// Helper that writes error message
static void write_err(char* err, int errLen, const std::string& msg)
{
    if (!err || errLen <= 0) return;

    // strncpy_s wants size_t for sizes
    strncpy_s(
        err,                         
        static_cast<size_t>(errLen), 
        msg.c_str(),                 
        _TRUNCATE                    
    );
}

// Convert an integer flag (from VBA) into the internal C++ enum
static Option toOption(int isCall)
{
    if (isCall == 1)
        return Option::Call;
    if (isCall==0 )
        return Option::Put;
    else
        throw std::invalid_argument("isCall must be 0 (Put) or 1 (Call)");
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
    double hS, double hg, double dt, double hR, double hV,
    unsigned int seed,
    double* price, double* delta, double* gamma,
    double* theta, double* rho, double* vega, char* errMsg, int errLen) {
    // clear message
    if (errMsg && errLen > 0) errMsg[0] = '\0';

    //checks
    if (!price || !delta || !gamma || !theta || !rho || !vega) {
        write_err(errMsg, errLen, "Null output pointer(s) passed from VBA.");
        return -1;
    }

    if (S0 <= 0.0 || sigma <= 0.0 || T <= 0.0) {
        write_err(errMsg, errLen, "Invalid model inputs: require S0>0, sigma>0, T>0.");
        return -3;
    }
    if (steps <= 0 || paths <= 0) {
        write_err(errMsg, errLen, "Invalid Monte Carlo settings: require steps>0, paths>0.");
        return -3;
    }
    if (dt <= 0.0 || dt >= T) {
        write_err(errMsg, errLen, "Invalid theta bump: require 0 < dt < T.");
        return -4;
    }

    try{
    const Option type = toOption(isCall);
    LookbackParams p{ type, S0, r, sigma, T, steps, paths, seed };
    GreeksParams g{ hS, hg, dt, hR, hV };
    LookbackOption pricer;
    // Price
    *price = pricer.price_lookback(p);

    // Greeks 
    *delta = pricer.delta_lookback(p,g);
    *gamma = pricer.gamma_lookback(p,g);
    *theta = pricer.theta_lookback(p,g);
    *rho = pricer.rho_lookback(p,g);
    *vega = pricer.vega_lookback(p,g);

    return 0;
}
    catch (const std::exception& e) {
        write_err(errMsg, errLen, std::string("C++ exception: ") + e.what());
        return -100;
    }
    catch(...) {
        write_err(errMsg, errLen, "Unknown C++ exception.");
        return -101;
    }
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
    const double span = Smax - Smin;
    const int n = (int)std::floor(span / dS + 1.0 + 1e-12);
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
    double hS,
    unsigned int seed,
    double* outS, double* outPrice, double* outDelta,
    int nPoints, char* errMsg, int errLen
) {
    // clear message
    if (errMsg && errLen > 0) errMsg[0] = '\0';

    if (!outS || !outPrice || !outDelta) {
        write_err(errMsg, errLen, "Null output array pointer(s) passed from VBA.");
        return -1;
    }
    if (nPoints <= 0) {
        write_err(errMsg, errLen, "Invalid nPoints: must be > 0.");
        return -2;
    }

    int expected = LookbackCountPoints(Smin, Smax, dS);
    if (expected <= 0) {
        write_err(errMsg, errLen, "Invalid grid: check Smin/Smax/dS.");
        return -3;
    }

    if (nPoints < expected) {
        write_err(errMsg, errLen, "nPoints too small for the requested grid.");
        return -4;
    }
    try{
    Option type = toOption(isCall);
    LookbackOption pricer;
    GreeksParams g{ hS, hS, 1e-4, 1e-4, 1e-4 };


    int k = 0;
    for (double S0 = Smin; S0 <= Smax + 1e-12; S0 += dS) {
        if (k >= nPoints) break;

        outS[k] = S0;
        LookbackParams p{ type, S0, r, sigma, T, steps, paths, seed };

        // price
        outPrice[k] = pricer.price_lookback(p);

        // delta
        outDelta[k] =pricer.delta_lookback(p,g);

        k++;
    }

    return 0;
}
    catch (const std::exception& e) {
        write_err(errMsg, errLen, std::string("C++ exception: ") + e.what());
        return -100;
    }
    catch (...) {
        write_err(errMsg, errLen, "Unknown C++ exception.");
        return -101;
    }
}
