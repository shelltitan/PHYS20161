// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

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

int main()
{
	vector<settlement> places;
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
}
