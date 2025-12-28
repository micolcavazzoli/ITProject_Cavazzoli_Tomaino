#include <iostream>
#include <cmath>
#include <random>
#include <vector>

using namespace std;

double simulate_price(double S0, double r, double sigma,
    double T, int steps, bool isCall)
{
    int N = steps;
    double dt = T / N;

    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> Z(0.0, 1.0);

    double S = S0;
    double minS = S0;
    double maxS = S0;

    for (int i = 0; i < N; i++) {
        double z = Z(gen);
        S = S * exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * z);
        minS = min(minS, S);
        maxS = max(maxS, S);
    }

    if (isCall)
        return S - minS;
    else
        return maxS - S;
}

double price_lookback(double S0, double r, double sigma,
    double T, int steps, int paths, bool isCall)
{
    double sum = 0.0;

    for (int i = 0; i < paths; i++)
        sum += simulate_price(S0, r, sigma, T, steps, isCall);

    double mean = sum / paths;

    return exp(-r * T) * mean;
}

int main()
{
    double S0 = 100;
    double r = 0.05;
    double sigma = 0.2;
    double T = 1.0;
    int steps = 252;
    int paths = 50000;
    bool isCall = true;

    double P = price_lookback(S0, r, sigma, T, steps, paths, isCall);

    cout << "Prezzo lookback: " << P << endl;
}
