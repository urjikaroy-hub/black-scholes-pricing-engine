#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>
using namespace std;

const double pi = 3.14159265358979323846;



// Black Scholes functions models that calculate the options call and put prices

// standard normal cummulative distribution function
double normalCDF(double x)
{
	return 0.5*erfc(-x / sqrt(2.0));
}
double normalPDF(double x)
{
	return exp(-0.5*x*x) / sqrt(2.0*pi);
}

double calcD1(double S1, double K1, double t1, double r1, double sigma1)
{
	return (log(S1 / K1) + (r1 + 0.5 * sigma1 * sigma1) * t1) / (sigma1 * sqrt(t1));
}
// calculates call and put price
double callPrice (double S1, double K1, double t1, double r1, double sigma1, bool isCall)
{
 double d1 = calcD1(S1, K1, t1, r1, sigma1);
 double d2 = d1 - sigma1 * sqrt(t1);
 if (isCall)
 {
	 return S1*normalPDF(d1) - K1 * exp (-r1 * t1) * normalCDF(d2);
 }
 else
 {
	 return K1 * exp(-r1 * t1) * normalCDF(-d2) - S1*normalPDF(-d1);
 }
}
// delta function: the change in option price for a $1 move in the stock 
double delta(double S1, double K1, double t1, double r1, double sigma1, bool isCall)
{
	double d1 = calcD1(S1,K1,t1,r1,sigma1);
	return isCall ? normalPDF(d1) : normalPDF(d1) - 1.0;
}
// gamma function: how much delta changes for a $1 move in the stock
double gamma(double S1, double K1, double t1, double r1, double sigma1)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    return normalCDF(d1) / (S1 * sigma1 * sqrt(t1));
}
// vega function: the change in price for a 1.00 (100 percentage point) change in volatility.
// Divide by 100 to get the more common "per 1%" figure (0.1139 here).
double vega(double S1, double K1, double t1, double r1, double sigma1)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    return S1 * normalPDF(d1) * sqrt(t1);
}
// theta function: the price change per year as time passes. Divide by 365 for per day (about -0.0544 for the call).
double theta(double S1, double K1, double t1, double r1, double sigma1, bool isCall1)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    double d2 = d1 - sigma1 * sqrt(t1);
    double decay = -S1 * normalPDF(d1) * sigma1 / (2.0 * sqrt(t1));
    if (isCall1)
        return decay - r1 * K1 * exp(-r1 * t1) * normalCDF(d2);
    else
        return decay + r1 * K1 * exp(-r1 * t1) * normalCDF(-d2);
}
// rho function: the change in price for a 1.00 change in the interest rate. Divide by 100 for per 1%.
double rho(double S1, double K1, double t1, double r1, double sigma1, bool isCall)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    double d2 = d1 - sigma1 * sqrt(t1);
    if (isCall)
	{return K1 * t1 * exp(-r1 * t1) * normalCDF(d2);}
    else
	{return -K1 * t1 * exp(-r1 * t1) * normalCDF(-d2);}
}

// getting real world prices by reading Google sheets filebuf
map<string, double> readPrices(const string& filename)
{
	map<string, double> prices;
	ifstream file(filename);
	string line;
	getline(file, line);
	
	while (getline(file, line))
	{
		size_t comma = line.find(',');
		if (comma == string::npos) continue;
		
		string ticker = line.subtr(0,comma);
		string priceText = line.substr(comma + 1);
		
		string cleanedl
		for (char c : priceText)
			if(c!= '"' && c!= ',' && c!= '\r') cleaned +=c;
		
		try 
		{
			prices[ticker] = stod(cleaned);
		}
		catch (...)
		{
			
		}
		
	} return prices;
}
int main()
{
	map<string, double> prices = readPrices("price.csv);
	cout << "Loaded " << prices.size() << prices, endl;
	
bool is_call = true;
bool is_put = false;


double K = 100.0;
double t = 30/365.0;
double r = 0.05;
double sigma = 0.25;
double call;
double put;
call = callPrice(S,K,t,r,sigma,is_call);
put = callPrice (S,K,t,r,sigma,is_put);
cout<<"The call price is: "<< call << endl;
cout<<"The put price is: "<< put << endl;
cout << "           Call        Put" << endl;
cout << "Delta:  " << delta(S, K, t, r, sigma, true) << "   " << delta(S, K, t, r, sigma, false) << endl;
cout << "Gamma:  " << gamma(S, K, t, r, sigma) << "   " << gamma(S, K, t, r, sigma) << endl;
cout << "Vega:   " << vega(S, K, t, r, sigma) << "   " << vega(S, K, t, r, sigma) << endl;
cout << "Theta:  " << theta(S, K, t, r, sigma, true) << "   " << theta(S, K, t, r, sigma, false) << endl;
cout << "Rho:    " << rho(S, K, t, r, sigma, true) << "   " << rho(S, K, t, r, sigma, false) << endl;
return 0;
}