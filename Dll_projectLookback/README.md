# Lookback Options Pricing Library

## Abstract

This project implements a C++ dynamic-link library (DLL) for the pricing of
lookback options using Monte Carlo simulation. Both call and put lookback
options are supported, together with the computation of the main Greeks
(Delta, Gamma, Theta, Rho, Vega) via finite difference methods.

The library is designed to be callable from external environments such as VBA,
allowing flexible integration in financial applications. Particular attention
is given to numerical stability, reproducibility through random seeds, and
clear separation between pricing logic and interface code.
