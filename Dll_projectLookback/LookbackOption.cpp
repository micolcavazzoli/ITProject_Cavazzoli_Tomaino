#include "LookbackOption.h"


//Function to simulate a single path of the option lookback
double LookbackOption::simulate_lookback(Option type, double S0, double r, double sigma, double T, int steps, std::mt19937& gen, std::normal_distribution<>& Z) const{

    double dt = T / steps;
    double S = S0;
    double minS = S0;
    double maxS = S0;

    for (int i = 0; i < steps; i++) {
        double z = Z(gen);
        S *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * z);
        minS = std::min(minS, S);
        maxS = std::max(maxS, S);
    }
    if (type == Option::Call)
        return S - minS;  //payoff call lookback
    else
        return maxS - S;  //payoff put lookback
}


//Price simulation with Monte Carlo
double LookbackOption::price_lookback(const LookbackParams& p) const{

    std::mt19937 gen(p.seed);
    std::normal_distribution<> Z(0.0, 1.0);

    double sum = 0.0;
    for (int i = 0; i < p.paths; i++)
        sum += simulate_lookback(p.type, p.S0, p.r, p.sigma, p.T, p.steps, gen, Z);

    return std::exp(-p.r * p.T) * (sum / p.paths);
}

//Greeks

//Delta of the option
double LookbackOption::delta_lookback(const LookbackParams& p, const GreeksParams& g) const{

    LookbackParams p_plus = p;
    LookbackParams p_minus = p;
    p_plus.S0 += g.hS;
    p_minus.S0 -= g.hS;

    const double P_plus = price_lookback(p_plus);
    const double P_minus = price_lookback(p_minus);

    return (P_plus - P_minus) / (2.0 * g.hS);
}

//Gamma of the option
double LookbackOption::gamma_lookback(const LookbackParams& p, const GreeksParams& g) const{

    LookbackParams p_plus = p;
    LookbackParams p_minus = p;
    p_plus.S0 += g.hg;
    p_minus.S0 -= g.hg;

    const double P_plus = price_lookback(p_plus);
    const double P = price_lookback(p);
    const double P_minus = price_lookback(p_minus);

    return (P_plus - 2.0 * P + P_minus) / (g.hg * g.hg);
}

//Theta of the option
double  LookbackOption::theta_lookback(const LookbackParams& p, const GreeksParams& g) const{

    LookbackParams p_today = p;
    LookbackParams p_future = p;
    p_future.T = p.T - g.dt;

    const double P_today = price_lookback(p_today);
    const double P_future = price_lookback(p_future);

    return (P_future - P_today) / g.dt; //negative if the price decreses wrt time
}

//Rho of the option
double  LookbackOption::rho_lookback(const LookbackParams& p, const GreeksParams& g) const{

    LookbackParams p_plus = p;
    LookbackParams p_minus = p;
    p_plus.r += g.hR;
    p_minus.r -= g.hR;

    double P_plus = price_lookback(p_plus);
    double P_minus = price_lookback(p_minus);

    return (P_plus - P_minus) / (2.0 * g.hR);
}

//Vega of the option
double  LookbackOption::vega_lookback(const LookbackParams& p, const GreeksParams& g) const{

    LookbackParams p_plus = p;
    LookbackParams p_minus = p;
    p_plus.sigma += g.hV;
    p_minus.sigma -= g.hV;

    double P_plus = price_lookback(p_plus);
    double P_minus = price_lookback(p_minus);

    return (P_plus - P_minus) / (2.0 * g.hV);
}
