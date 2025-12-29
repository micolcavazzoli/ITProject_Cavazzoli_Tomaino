#include <iostream>
#include <cmath>
#include <random>
#include <vector>

using namespace std;

// Funzione generica per simulare un percorso della call lookback
double simulate_call_lookback(double S0, double r, double sigma, double T, int steps) {
    int N = steps;
    double dt = T / N;

    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> Z(0.0, 1.0);

    double S = S0;
    double minS = S0;  // per la call guardiamo il minimo

    for (int i = 0; i < N; i++) {
        double z = Z(gen);
        S = S * exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * z);
        minS = min(minS, S);
    }

    return S - minS;  // payoff call lookback
}

// Funzione generica per simulare un percorso della put lookback
double simulate_put_lookback(double S0, double r, double sigma, double T, int steps) {
    int N = steps;
    double dt = T / N;

    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> Z(0.0, 1.0);

    double S = S0;
    double maxS = S0;  // per la put guardiamo il massimo

    for (int i = 0; i < N; i++) {
        double z = Z(gen);
        S = S * exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * z);
        maxS = max(maxS, S);
    }

    return maxS - S;  // payoff put lookback
}

// Prezzo call lookback con Monte-Carlo
double price_call_lookback(double S0, double r, double sigma, double T, int steps, int paths) {
    double sum = 0.0;
    for (int i = 0; i < paths; i++)
        sum += simulate_call_lookback(S0, r, sigma, T, steps);

    return exp(-r * T) * (sum / paths);
}

// Prezzo put lookback con Monte-Carlo
double price_put_lookback(double S0, double r, double sigma, double T, int steps, int paths) {
    double sum = 0.0;
    for (int i = 0; i < paths; i++)
        sum += simulate_put_lookback(S0, r, sigma, T, steps);

    return exp(-r * T) * (sum / paths);
}

double delta_call_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.01) {
    double P_plus = price_call_lookback(S0 + h, r, sigma, T, steps, paths);
    double P_minus = price_call_lookback(S0 - h, r, sigma, T, steps, paths);

    return (P_plus - P_minus) / (2.0 * h);
}

double delta_put_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.01) {
    double P_plus = price_put_lookback(S0 + h, r, sigma, T, steps, paths);
    double P_minus = price_put_lookback(S0 - h, r, sigma, T, steps, paths);

    return (P_plus - P_minus) / (2.0 * h);
}

double gamma_call_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.01) {
    double P_plus = price_call_lookback(S0 + h, r, sigma, T, steps, paths);
    double P = price_call_lookback(S0, r, sigma, T, steps, paths);
    double P_minus = price_call_lookback(S0 - h, r, sigma, T, steps, paths);

    return (P_plus - 2.0 * P + P_minus) / (h * h);
}

double gamma_put_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.01) {
    double P_plus = price_put_lookback(S0 + h, r, sigma, T, steps, paths);
    double P = price_put_lookback(S0, r, sigma, T, steps, paths);
    double P_minus = price_put_lookback(S0 - h, r, sigma, T, steps, paths);

    return (P_plus - 2.0 * P + P_minus) / (h * h);
}

double theta_call_lookback(double S0, double r, double sigma, double T, int steps, int paths, double dt = 1.0 / 365.0) {
    double P_today = price_call_lookback(S0, r, sigma, T, steps, paths);
    double P_future = price_call_lookback(S0, r, sigma, T - dt, steps, paths);
    return (P_future - P_today) / dt;  // negativo se il prezzo scende col tempo
}

double theta_put_lookback(double S0, double r, double sigma, double T, int steps, int paths, double dt = 1.0 / 365.0) {
    double P_today = price_put_lookback(S0, r, sigma, T, steps, paths);
    double P_future = price_put_lookback(S0, r, sigma, T - dt, steps, paths);
    return (P_future - P_today) / dt;
}

double rho_call_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.001) {
    double P_plus = price_call_lookback(S0, r + h, sigma, T, steps, paths);
    double P_minus = price_call_lookback(S0, r - h, sigma, T, steps, paths);
    return (P_plus - P_minus) / (2.0 * h);
}

double rho_put_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.001) {
    double P_plus = price_put_lookback(S0, r + h, sigma, T, steps, paths);
    double P_minus = price_put_lookback(S0, r - h, sigma, T, steps, paths);
    return (P_plus - P_minus) / (2.0 * h);
}

double vega_call_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.01) {
    double P_plus = price_call_lookback(S0, r, sigma + h, T, steps, paths);
    double P_minus = price_call_lookback(S0, r, sigma - h, T, steps, paths);
    return (P_plus - P_minus) / (2.0 * h);
}

double vega_put_lookback(double S0, double r, double sigma, double T, int steps, int paths, double h = 0.01) {
    double P_plus = price_put_lookback(S0, r, sigma + h, T, steps, paths);
    double P_minus = price_put_lookback(S0, r, sigma - h, T, steps, paths);
    return (P_plus - P_minus) / (2.0 * h);
}




int main()
{
    // Parametri di esempio
    double S0 = 100;
    double r = 0.05;
    double sigma = 0.2;
    double T = 1.0;
    int steps = 252;
    int paths = 50000;

    // Calcolo prezzi
    double call_price = price_call_lookback(S0, r, sigma, T, steps, paths);
    double put_price = price_put_lookback(S0, r, sigma, T, steps, paths);

    cout << "Price call lookback: " << call_price << endl;
    cout << "Price put lookback: " << put_price << endl;

    double delta_call = delta_call_lookback(S0, r, sigma, T, steps, paths);
    double delta_put = delta_put_lookback(S0, r, sigma, T, steps, paths);

    cout << "Delta call lookback: " << delta_call << endl;
    cout << "Delta put lookback: " << delta_put << endl;

    double gamma_call = gamma_call_lookback(S0, r, sigma, T, steps, paths);
    double gamma_put = gamma_put_lookback(S0, r, sigma, T, steps, paths);

    cout << "Gamma call lookback: " << gamma_call << endl;
    cout << "Gamma put lookback: " << gamma_put << endl;

    double theta_call = theta_call_lookback(S0, r, sigma, T, steps, paths);
    double theta_put = theta_put_lookback(S0, r, sigma, T, steps, paths);

    cout << "Theta call lookback: " << theta_call << endl;
    cout << "Theta put lookback: " << theta_put << endl;

    double rho_call = rho_call_lookback(S0, r, sigma, T, steps, paths);
    double rho_put = rho_put_lookback(S0, r, sigma, T, steps, paths);

    cout << "Rho call lookback: " << rho_call << endl;
    cout << "Rho put lookback: " << rho_put << endl;

    double vega_call = vega_call_lookback(S0, r, sigma, T, steps, paths);
    double vega_put = vega_put_lookback(S0, r, sigma, T, steps, paths);

    cout << "Vega call lookback: " << vega_call << endl;
    cout << "Vega put lookback: " << vega_put << endl;

    return 0;
}