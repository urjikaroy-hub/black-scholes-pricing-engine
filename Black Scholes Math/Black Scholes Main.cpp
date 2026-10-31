#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;
const double pi = 3.14159265358979323;
//Black-Scholes to calculate premium for options call & put, 
//added more accurate value of Pi for higher col of contracts(in millions)

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
// calculate call and put premium
double optionPrice (double S1, double K1, double t1, double r1, double sigma1, bool isCall)
{
 double d1 = calcD1(S1, K1, t1, r1, sigma1);
 double d2 = d1 - sigma1 * sqrt(t1);
 if (isCall)
 {
	 return S1*normalCDF(d1) - K1 * exp (-r1 * t1)* normalCDF(d2);
 }
 else
 {
	 return K1 * exp(-r1 * t1)*normalCDF(-d2) -S1*normalCDF(-d1);
 }
}
// delta 
double bsDelta(double S1, double K1, double t1, double r1, double sigma1, bool isCall)
{
	double d1 = calcD1(S1,K1,t1,r1,sigma1);
	return isCall ? normalCDF(d1) : normalCDF(d1) - 1.0;
}
// gamma
double bsGamma(double S1, double K1, double t1, double r1, double sigma1)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    return normalPDF(d1) / (S1 * sigma1 * sqrt(t1));
}
// vega
// Divide by 100 to get er 1%
double bsVega(double S1, double K1, double t1, double r1, double sigma1)
{
	double d1 = calcD1(S1, K1, t1, r1, sigma1);
    return S1 * normalPDF(d1) * sqrt(t1);
}
// theta
double bsTheta(double S1, double K1, double t1, double r1, double sigma1, bool isCall1)
{
	double d1 = calcD1(S1, K1,t1,r1, sigma1);
    double d2 = d1 - sigma1 * sqrt(t1);
    double decay = -S1 * normalPDF(d1) * sigma1 / (2.0 * sqrt(t1));
    if (isCall1)
        return decay - r1 * K1 * exp(-r1 * t1) * normalCDF(d2);
    else
        return decay + r1* K1 * exp(-r1 * t1) * normalCDF(-d2);
}
// getting real prices from Google sheets filebuf
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
		
		string ticker = line.substr(0,comma);
		string priceText = line.substr(comma + 1);
		
		string cleaned;
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
void printTable(const map<string, double> prices, double t, double r, double sigma)
{
	cout << "Expiry: " << fixed << setprecision(0) << t *365 << " days, rate: "
		 << setprecision(2) << r*100 << "%, vol "<< sigma *100 << "%" << endl;
	cout << "At-the-money (strike = price). Theta per day, vega per 1%" << endl << endl; 
	
	cout << left << setw(8) << "Ticker" << right
		 << setw(9) << "Price" << setw(9) << "Call" << setw(9) << "Put"
		 << setw(9) << "C.Delta" << setw(9) << "P.Delta" << setw(9) << "Gamma"
		 << setw(9) << "Vega" << setw(9) << "C.Theta" << setw (9) << "P.Theta" << endl;
		 cout << string(89, '-') << endl;
		 for (const auto& entry : prices)
		 {
			 double S = entry.second;
			 double K = S;
			 
			 cout << left << setw(8) << entry.first << right << fixed
			 << setprecision(2) << setw(9) << S
			 << setw(9) << optionPrice(S,K,t,r,sigma, true)
			 << setw(9) << optionPrice(S,K,t,r,sigma, false)
			 << setprecision(4)
			 << setw(9) << bsDelta(S,K,t,r,sigma,true)
			 << setw(9) << bsDelta(S,K,t,r,sigma,false)
			 <<setprecision(5) << setw(9) << bsGamma(S,K,t,r,sigma)
			 << setprecision(4)
			 << setw(9) << bsVega(S,K,t,r,sigma) / 100.0
			 << setw(9) << bsTheta(S,K,t,r,sigma,true)/365.0
			 << setw(9) << bsTheta(S,K,t,r,sigma,false)/365.0 << endl;
		 }
}
int main()
{
double r = 0.05;
double sigma = 0.25;
double t = 30/365.0;
	map<string, double> prices = readPrices("price.csv");
	cout << "Loaded " << prices.size() << " prices"<< endl;	
	printTable(prices, t,r,sigma);
return 0;
}
