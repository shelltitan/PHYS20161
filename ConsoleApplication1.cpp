// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Write a program that reads in the data points x, y and the error on y (sigma) from a file and computes the error-weighted line of 
//best fit through them. Your program should also calculate and display the chi-square value for the fit obtained.(Assigment week 8)

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include<cmath>


using namespace std;

struct Data
{
	double x;
	double y;
	double unc_y;
};

int main()
{
	// vector is dynamically allocated so we don't need to care how much stuff we are going to put in
	vector<Data> dataArray;

	// open the input file as an input file stream (ifstream)
	ifstream dataFile("xys_data.txt");

	// if the file is open...
	if (dataFile.is_open())
	{
		// and while it's not the end of the file
		while (!dataFile.eof())
		{
			Data dataPoint;
			// read in a line of the file and put the contents into the arrays
			dataFile >> dataPoint.x >> dataPoint.y >> dataPoint.unc_y;
			dataArray.push_back(dataPoint);
			// add one to the counter variable
		}
		// now at the file end, close it
		dataFile.close();
	}
	else 
	{
		// if the file couldn't be opened, write a message and quit
		cout << "Unable to open file\n";
		exit(1);
	}
	double Sum_w = 0; // sum of weights
	double Sum_wxy = 0; // sum of weight * x * y
	double Sum_wx = 0; // sum of weight * x
	double Sum_wy = 0; // sum of weight * y
	double Sum_wxsq = 0; // sum of weight * x^2
	for (auto const& value : dataArray) // for loop to iterate through the vector
	{
		double weight = 1 / pow(value.unc_y, 2);
		Sum_wxy += weight * value.x * value.y;
		Sum_wx += weight * value.x;
		Sum_wy += weight * value.y;
		Sum_wxsq += weight * pow(value.x, 2);
		Sum_w += weight;
	}
	/* Slope and constant calculated for the best fit line using the following formulas
	D = (Sw(1) * Sw(x ^ 2) - (Sw(x)) ^ 2)
	m = (Sw(1) * Sw(x*y) - Sw(x) * Sw(y)) / D
	dm = sqrt(Sw(1) / D)
	c = (Sw(y) * Sw(x ^ 2) - Sw(x) * Sw(x*y)) / D
	dc = sqrt(Sw(x ^ 2) / D) */
	double D = Sum_w * Sum_wxsq - pow(Sum_wx, 2);
	double m = (Sum_w * Sum_wxy - Sum_wx * Sum_wy) / D;
	double dm = sqrt(Sum_w / D);
	double c = (Sum_wy * Sum_wxsq - Sum_wx * Sum_wxy) / D;
	double dc = sqrt(Sum_wxsq / D);
	double xsq = 0;
	for (auto const& value : dataArray) // for loop to iterate through the vector
	{
		double weight = 1 / pow(value.unc_y, 2);
		xsq += weight * (m * value.x + c - value.y) * (m * value.x + c - value.y);
		//	xsq = Sw((m*x + c - y) ^ 2)
		// chi square calculated using the formula above
	}
	printf("The best fit line has a slope m = %5.2f with an uncertainty of dm = %5.2f.\0", m, dm);
	printf("\nThe line has constant c = %5.2f with and uncertainty dc = %5.2f.\0", c, dc);
	printf("\nThe chi square of the best fit line is xsq = %5.2f.\0", xsq);
	cout << "\nPress any Enter";
	cin.get();
	// printing the result out and using cin so the console doesn't close immediately
	return 0;
}

