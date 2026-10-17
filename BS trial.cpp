#include <iostream>
#include <cmath>
using namespace std;
double N(double x) (return 0.5 * erfc{-x / sqrt(2.0)); }// normal CDF
int main() {
    double S, K, t, r, v;
    cout << "S K t r sigma: "; cin>>S>>K>> t>>r >>v;
    double d1 =(log(S/K)+(r+ 0.5*v*v)*t)/(v*sqrt(t));
    double d2 =d1-v*sqrt(t);
    cout << "Call:"<< S*N(d1)-K*exp(-r*t)*N(d2)<<"\n";
    cout << "Put:"<< K*exp(-r*t)*N(-d2)-S*N(-d1) << "\n";
}