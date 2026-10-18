#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>
using namespace std;

const double pi = 3.14159265358979323846;

// Black Scholes option premium + greeks
// standard normal cummulative distribution function
double normalCDF(double x)
{
return 0.5*erfc(-x / sqrt(2.0));
}
double normalPDF(double x)
{
return exp(-0.5*x*x)/sqrt(2.0*pi);
}

double calcD1(double S1,double K1, double t1, double r1, double sigma1)
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
	 return S1*normalPDF(d1) - K1 * exp (-r1 * t1)*normalCDF(d2);
 }
 else
 {
	 return K1 * exp(-r1 * t1) * normalCDF(-d2) - S1*normalPDF(-d1);
 }
}
// delta 
double delta(double S1, double K1, double t1, double r1, double sigma1, bool isCall)
{
	double d1 = calcD1(S1,K1,t1,r1,sigma1);
	return isCall ? normalPDF(d1) : normalPDF(d1) - 1.0;
}
// gamma
double gamma(double S1, double K1, double t1, double r1, double sigma1)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    return normalCDF(d1) / (S1 * sigma1 * sqrt(t1));
}
// vega function
double vega(double S1, double K1, double t1, double r1, double sigma1)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    return S1 * normalPDF(d1) * sqrt(t1);
}
// theta
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

// getting current prices by reading Google sheets filebuf
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
		for (char vc : priceText)
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
int main()c
{
	map<string, double> prices = readPrices("price.csv);
	cout << "Loaded " << prices.size() << prices, endl;
	
bool is_call = true;
bool is_put = false;


double K= 100.0;
double t= 30/365.0;
double r= 0.05;
double sigma= 0.25;
double call;
double put;
call = callPrice(S,K,t,r,sigma,is_call);
put = callPrice (S,K,t,r,sigma,is_put);
cout<<"Call price is: "<< call << endl;
cout<<"Put price is: "<< put << endl;
cout << "Call   Put" << endl;
cout << "Delta:" << delta(S, K, t, r, sigma, true) << "   " << delta(S,K, t, r, sigma, false) << endl;
cout << "Gamma:" << gamma(S, K, t, r, sigma) << "   " << gamma(S, K, t, r,sigma) << endl;
cout << "Vega:" << vega(S, K, t, r, sigma) << "   " << vega(S, K, t, r, sigma) << endl;
cout << "Theta:" << theta(S, K, t, r, sigma, true) << "   " << theta(S, K, t , r, sigma, false) << endl;
return 0;
}