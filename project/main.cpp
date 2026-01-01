#include "LookbackOption.h"
#include "plots.h"

int main()
{
    //Parameters
    double S0 = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    double T = 1.0;
    int steps = 252;
    int paths = 50000;
    
    double h_delta=1.0;
    double h_gamma=2.0;
    double dt_theta=1.0/52.0; //weekly in order to reduce MC variance (we can consider it daily by using dt=1.0/365.0)
    double h_rho = 0.005;
    double h_vega =0.02;

    unsigned int seed = 42;

    //Prices computation
    double call_price = price_lookback(Option::Call, S0, r, sigma, T, steps, paths, seed);
    double put_price = price_lookback(Option::Put ,S0, r, sigma, T, steps, paths, seed);

    std::cout << "Price call lookback: " << call_price << std::endl;
    std::cout << "Price put lookback: " << put_price << std::endl;

    //Greeks
    //Delta computation
    double delta_call = delta_lookback(Option::Call, S0, r, sigma, T, steps, paths, h_delta, seed);
    double delta_put = delta_lookback(Option::Put,S0, r, sigma, T, steps, paths, h_delta, seed);
    std::cout << "Delta call lookback: " << delta_call << std::endl;
    std::cout << "Delta put lookback: " << delta_put << std::endl;

    //Gamma computation
    double gamma_call = gamma_lookback(Option::Call, S0, r, sigma, T, steps, paths, h_gamma, seed);
    double gamma_put = gamma_lookback(Option::Put, S0, r, sigma, T, steps, paths, h_gamma, seed);

    std::cout << "Gamma call lookback: " << gamma_call << std::endl;
    std::cout << "Gamma put lookback: " << gamma_put << std::endl;

    //Theta computation
    double theta_call = theta_lookback(Option::Call, S0, r, sigma, T, steps, paths, dt_theta, seed);
    double theta_put = theta_lookback(Option::Put, S0, r, sigma, T, steps, paths, dt_theta, seed);

    std::cout << "Theta call lookback: " << theta_call << std::endl;
    std::cout << "Theta put lookback: " << theta_put << std::endl;

    //Rho computation
    double rho_call = rho_lookback(Option::Call, S0, r, sigma, T, steps, paths, h_rho, seed);
    double rho_put = rho_lookback(Option::Put, S0, r, sigma, T, steps, paths, h_rho, seed);

    std::cout << "Rho call lookback: " << rho_call << std::endl;
    std::cout << "Rho put lookback: " << rho_put << std::endl;

    //Vega computation
    double vega_call = vega_lookback(Option::Call, S0, r, sigma, T, steps, paths, h_vega, seed);
    double vega_put = vega_lookback(Option::Put, S0, r, sigma, T, steps, paths, h_vega, seed);

    std::cout << "Vega call lookback: " << vega_call << std::endl;
    std::cout << "Vega put lookback: " << vega_put << std::endl;

    //FILES FOR PLOTS
    double Smin=50;
    double Smax=150;
    double ds=5;

    //plots price vs S0, call and put
    plot_price_S0(Option::Call, r, sigma, T, steps, paths, Smin, Smax, ds, seed);
    plot_price_S0(Option::Put, r, sigma, T, steps, paths, Smin, Smax, ds, seed);

    //plots delta vs S0, call and put
    plot_delta_S0(Option::Call, r, sigma, T, steps, paths, Smin, Smax, ds, h_delta, seed);
    plot_delta_S0(Option::Put, r, sigma, T, steps, paths, Smin, Smax, ds, h_delta, seed);

    return 0;
}