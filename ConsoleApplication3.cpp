#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>

//I think the assigment was to transpose the matrice given in a file. I don't remember if this is the right solution.
//I had a solution that worked for few cases and a general one. This was week 9.

using namespace std;
int main()
{
	// open the input file as an input file stream (ifstream)
	ifstream dataFile("xys_data.txt");

	// dynamic arrays made for storing the matrices
	double *matrix;
	double *transpose_matrix;
	int width = 0;
	int height = 0;
	int k = 0;

	// if the file is open...
	if (dataFile.is_open())
	{
		// and while it's not the end of the file
		while (!dataFile.eof())
		{
			string Line;
			getline(dataFile, Line);
			stringstream line(Line);
			double i;
			if (height == 0)
			{
				while (line >> i)
				{
					if (line.peek() == ',')
					{
						line.ignore();
					}
					width++;
				}
			}
			height++;
		}
		matrix = new double[width*height];
		transpose_matrix = new double[width*height];
		dataFile.clear();
		dataFile.seekg(0, dataFile.beg);
		while (!dataFile.eof())
		{
			string Line;
			getline(dataFile, Line);
			stringstream line(Line);
			double i;
			while (line >> i)
			{
				matrix[k] = i;
				if (line.peek() == ',' || '\n')
				{
					line.ignore();
				}
				k++;
			}
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
	for (int n = 0; n < width*height; n++) {
		int i = n / height;
		int j = n % height;
		transpose_matrix[n] = matrix[width*j + i];
	}
	//transposing
	ofstream outFile("transposed_matrix.txt");

	if (outFile.is_open())
	{
		for (int r = 1; r <= width * height; r++)
		{	
			cout << width << endl;
			cout << height << endl;
			outFile << transpose_matrix[r - 1];
			cout << transpose_matrix[r - 1] << endl;
			cout << r << endl;
			if (r % height == 0)
			{
				outFile << endl;
				continue;
			}
			if (r == width * height)
			{
				continue;
			}
			outFile << ",";
		}
		// now at the file end, close it
		outFile.close();
	}
	else
	{
		// if the file couldn't be opened, write a message and quit
		cout << "Unable to open file for writing!\n";
		exit(1);
	}
	//deleting pointers
	delete[] matrix;
	delete[] transpose_matrix;
	return 0;
}