// The program claculates the optimal distance from towns to the hub using hill search alogorithm
// it expects a file name GBplaces.cvs

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <sstream>
#include <fstream>
#include <vector>

int f_evals = 0; // keep a count function evaluations - note, global variable
#define PI 3.14159265358979323846
#define R  3958.75

using namespace std;

struct settlement
{
	string name;
	string type;
	double latitude;
	double longitude;
	int population;
};
// struct made to store data

// functions

double calculate_distance(double lat1, double long1, double lat2, double long2)
{
	// Haversine formula used to calculate the distance between two points

	double dLat = lat2 - lat1;

	dLat = dLat * PI / 180;

	double dLong = long2 - long1;

	dLong = dLong * PI / 180;

	double a = pow(sin(dLat / 2), 2) + cos(lat1 * PI / 180)*cos(lat2 * PI / 180)*pow(sin(dLong / 2), 2);

	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return R * c;
}

double fitness(double x, double y, vector<settlement>& vec_data) {
	// function to return the 'value' of thing being optimized
	double sum_distance = 0;
	for (auto const& s : vec_data) {
		sum_distance += calculate_distance(s.latitude, s.longitude, x, y);
	}
	// calculates the sum of the Great Circle distances from point x,y to each place
	double f;
	f = 1 / sum_distance;
	f_evals++;
	return f;
}

double random_number(double lower, double upper, int n) {
	// function to return a random number between two limits, lower and upper
	// n is the amount of bits to split the range into
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

// main program

int main() {
	// declare variables
	vector<settlement> places;
	int dx = 0;
	int dy = 0;
	double x, y; // holds the current best values of x and y
	double step = 0.01; // step size to move in x and y
	double value, oldValue, newValue, maxValue;
	int iteration = 1;

	srand(time(NULL)); // seeds random number generator

	// read in GBplaces.csv
	// input file is opened as ifstream
	ifstream dataFile("GBPlaces.csv");

	// while the file is open
	if (dataFile.is_open())
	{
		// while it is not the end of file
		while (!dataFile.eof())
		{
			string line;
			settlement temp;
			string array[5];

			getline(dataFile, line, '\n');
			// read in a line from csv file

			if (line == "" || line[0] == *"%") continue;
			// if the line is empty or starts with % we discard it
			// "%" is a const char pointer so we use the operator * to get the actual character 

			std::istringstream ss(line);
			for (int i = 0; i < 5; i++) {
				getline(ss, array[i], ',');
			}

			temp.name = array[0];  // the first member of the array is the name of the place
			temp.type = array[1];  // the second member of the array is the type of the place
			temp.population = stoi(array[2]);
			// the third member of the array is the population of the place and we convert it to an integer using stoi
			temp.latitude = atof(array[3].c_str());
			// the fourth member of the array is the latitude of the place and we convert it to a double using atof
			// but first we make it a c string so atof can convert it
			temp.longitude = atof(array[4].c_str());
			// the fifth member of the array is the longitude of the place and we convert it to a double using atof
			// but first we make it a c string so atof can convert it
			places.push_back(temp);
			// puts temp in places
		}
		// close at the end of file
		dataFile.close();
	}
	else
	{
		// if fail to open file, send error message
		cout << "Unable to open file\n";
		exit(1);
	}
	// pick a starting point at random between the highest latitude and lowest latitude
	// and highest longitude and lowest longitude
	double min_lat = places[0].latitude;
	double max_lat = places[0].latitude;
	double min_long = places[0].longitude;
	double max_long = places[0].longitude;
	for (auto const& data : places) {
		if (data.latitude > max_lat) max_lat = data.latitude;
		if (data.latitude < min_lat) min_lat = data.latitude;
		if (data.longitude < min_long) min_long = data.longitude;
		if (data.longitude > max_long) max_long = data.longitude;
	}
	x = random_number(min_lat, max_lat, 100);
	y = random_number(min_long, max_long, 100);
	// optimize the position x,y

	// output results

	// work out value of function at that point - how 'good' is point x,y?
	value = fitness(x, y, places);

	// main loop to continually 'improve' x, y
	do {

		// save the current value
		oldValue = value;
		maxValue = oldValue; // set the maxValue for the local search to be the current value

		// now look around the current point to see if there's a better one nearby
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				// this gives 9 points including the current point (when i=0, j=0)
				if (i == 0 && j == 0) {
					// so maybe you want to miss that one and save some function evaluations
				}
				else {
					newValue = fitness(x + step * i, y + step * j, places); // value at neighbouring point
					if (newValue >= maxValue) { // is it bigger than maxValue?
					  // yes so set maxValue and save point i,j values
						dx = i;
						dy = j;
						maxValue = newValue;
					}
				}
			}
		}

		// update x and y to new point with higher 'value'
		x += step * dx;
		y += step * dy;
		value = maxValue;

		iteration++; // add one to the iteration counter

	} while (value > oldValue); // repeat all this while we can find a greater value than the previous one

	cout << "Function evaluations: " << f_evals << ".\n"; // write out the total number of function evaluations
	cout << "The latitude of the hub: " << x << "." << "The longitude of the hub: " << y << ".\n";
	cout << "The sum of distances to the hub is " << 1/value << "miles." << "\n";

	return 0;
}
