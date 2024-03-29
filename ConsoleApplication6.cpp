// K-mean algorith used to find "groups" in the data, then hill search is used on the data
// so the optimal hub is found in the group
// int K = 2; number of groups the algorithm is trying to make (in this case it's 2)
// int max_iterations = 10000; max iterations can be change if the algorithm is running too long (in this case it is set to 10000)

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <sstream>
#include <fstream>	
#include <vector>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#define PI 3.14159265358979323846
#define R  3958.75

using namespace std;

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

class Settlement
{
	// class used to contain data about the towns, it can be assigned a to a cluster, an id is assigned to the class so it easy to manage
private:
	string name, type;
	int id_point, id_cluster;
	vector<double> values;
	int population;
	int total_values;

public:
	Settlement(int id_point, vector<double>& values, int population, string name = "", string type = "")
	{
		this->id_point = id_point;
		total_values = values.size();

		for (auto& value : values) {
			this->values.push_back(value);
		}
		this->name = name;
		this->type = type;
		this->population = population;
		id_cluster = -1;
	}// constructor to assign values for the member variables

	//set and get functions to set/get values (private members cannot be accesed from outside functions)

	int getID()
	{
		return id_point;
	}

	void setCluster(int id_cluster)
	{
		this->id_cluster = id_cluster;
	}

	int getCluster()
	{
		return id_cluster;
	}

	double getValue(int index)
	{
		return values[index];
	}

	int getTotalValues()
	{
		return total_values;
	}

	string getName()
	{
		return name;
	}

	string getType()
	{
		return type;
	}

	int getPopulation()
	{
		return population;
	}
};

class Cluster
{
	// made to contain points
private:
	int id_cluster;
	vector<double> central_values;
	vector<Settlement> points;

public:
	Cluster(int id_cluster, Settlement point)
	{
		this->id_cluster = id_cluster;

		int total_values = point.getTotalValues();

		for (int i = 0; i < total_values; i++)
			central_values.push_back(point.getValue(i));

		points.push_back(point);
	}

	void addPoint(Settlement point)
	{
		points.push_back(point);
	}

	void removePoint(int id_point)
	{
		int total_points = points.size();

		for (int i = 0; i < total_points; i++)
		{
			if (points[i].getID() == id_point)
			{
				points.erase(points.begin() + i);
				return;
			}
		}
		return;
	}

	double getCentralValue(int index)
	{
		return central_values[index];
	}

	void setCentralValue(int index, double value)
	{
		central_values[index] = value;
	}

	Settlement getPoint(int index)
	{
		return points[index];
	}

	vector<Settlement> getPointVector()
	{
		return points;
	}

	int getTotalPoints()
	{
		return points.size();
	}

	int getID()
	{
		return id_cluster;
	}
};

class KMeans
{
private:
	int K; // number of clusters
	int total_points, max_iterations;
	vector<Cluster> clusters;

	// return ID of nearest center uses haversine formula to claculate distance between the clusters and the points
	int getIDNearestCenter(Settlement point)
	{
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;

		min_dist = calculate_distance(point.getValue(0), point.getValue(1), clusters[0].getCentralValue(0), clusters[0].getCentralValue(1));

		for (int i = 1; i < K; i++)
		{
			double dist;

			dist = calculate_distance(point.getValue(0), point.getValue(1), clusters[i].getCentralValue(0), clusters[i].getCentralValue(1));

			if (dist < min_dist)
			{
				min_dist = dist;
				id_cluster_center = i;
			}
		}

		return id_cluster_center;
	}

public:
	KMeans(int K, int total_points, int max_iterations)
	{
		this->K = K;
		this->total_points = total_points;
		this->max_iterations = max_iterations;
	}

	void run(vector<Settlement> & points)
	{
		if (K > total_points)
			return;

		vector<int> prohibited_indexes;

		// choose K distinct values for the centers of the clusters
		for (int i = 0; i < K; i++)
		{
			while (true)
			{
				int index_point = rand() % total_points;

				if (find(prohibited_indexes.begin(), prohibited_indexes.end(),
					index_point) == prohibited_indexes.end())
				{
					prohibited_indexes.push_back(index_point);
					points[index_point].setCluster(i);
					Cluster cluster(i, points[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}

		int iter = 1;

		while (true)
		{
			bool done = true;

			// associates each point to the nearest center if there is change in clusters we set the done flag to false
			// so after recalcuating the centre of the cluster the program runs one more loop and check whether some points change or no
			// we do this untill there is no change in the clusters, the clusters converge to a point if groups can be found in the data
			for (int i = 0; i < total_points; i++)
			{
				int id_old_cluster = points[i].getCluster();
				int id_nearest_center = getIDNearestCenter(points[i]);

				if (id_old_cluster != id_nearest_center)
				{
					if (id_old_cluster != -1)
						clusters[id_old_cluster].removePoint(points[i].getID());

					points[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addPoint(points[i]);
					done = false;
				}
			}

			// recalculating the centre of each cluster it's the avarage of the x,y,z,etc of the values
			for (int i = 0; i < K; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					int total_points_cluster = clusters[i].getTotalPoints();
					double sum = 0.0;

					if (total_points_cluster > 0)
					{
						for (int p = 0; p < total_points_cluster; p++)
							sum += clusters[i].getPoint(p).getValue(j);
						clusters[i].setCentralValue(j, sum / total_points_cluster);
					}
				}
			}
			// if no new points get in any cluster then the central value of the cluster doesn't change
			// so we have found the clusters that fit the points
			// there is max iteration number to ensure that the algorithm doesn't run forever 
			if (done == true || iter >= max_iterations)
			{
				cout << "Break in iteration " << iter << "\n\n";
				break;
			}

			iter++;
		}

		// prints the elements of the cluster
		for (int i = 0; i < K; i++)
		{
			int total_points_cluster = clusters[i].getTotalPoints();

			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for (int j = 0; j < total_points_cluster; j++)
			{
				cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
				for (int p = 0; p < 2; p++)
					cout << clusters[i].getPoint(j).getValue(p) << " ";

				string point_name = clusters[i].getPoint(j).getName();

				if (point_name != "")
					cout << "- " << point_name;

				cout << endl;
			}

			cout << "Cluster values: ";

			for (int j = 0; j < 2; j++)
				cout << clusters[i].getCentralValue(j) << " ";

			cout << "\n\n";
		}
	}
	vector<Cluster> getClusters()
	{
		return clusters;
	}
};

double fitness(double x, double y, vector<Settlement>& vec_data) {
	// function to return the 'value' of thing being optimized
	double sum_distance = 0;
	for (auto& s : vec_data) {
		sum_distance += calculate_distance(s.getValue(0), s.getValue(1), x, y);
	}
	double f;
	f = 1 / sum_distance;
	return f;
}

double random_number(double lower, double upper, int n) {
	// function to return a random number between two limits, lower and upper
	// n is the amount of bits to split the range into
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

void hill_search(vector<Settlement> places) {
	int dx = 0;
	int dy = 0;
	double x, y; // holds the current best values of x and y
	double step = 0.01; // step size to move in x and y
	double value, oldValue, newValue, maxValue;
	// pick a starting point at random between the highest latitude and lowest latitude
	// and highest longitude and lowest longitude
	double min_lat = places[0].getValue(0);
	double max_lat = places[0].getValue(0);
	double min_long = places[0].getValue(1);
	double max_long = places[0].getValue(1);
	for (auto& data : places) {
		if (data.getValue(0) > max_lat) max_lat = data.getValue(0);
		if (data.getValue(0) < min_lat) min_lat = data.getValue(0);
		if (data.getValue(1) < min_long) min_long = data.getValue(1);
		if (data.getValue(1) > max_long) max_long = data.getValue(1);
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

	} while (value > oldValue); // repeat all this while we can find a greater value than the previous one

	cout << "The latitude of the hub: " << x << "." << "The longitude of the hub: " << y << "\n.";
	cout << "The sum of distances to the hub is " << 1 / value << "miles." << "\n";
}

// main program

int main()
{
	srand(time(NULL));

	int K = 2;
	int max_iterations = 10000;
	int id = 0;

	vector<Settlement> points;
	string point_name;

	ifstream dataFile("GBPlaces.csv");

	// while the file is open
	if (dataFile.is_open())
	{
		// while it is not the end of file
		while (!dataFile.eof())
		{
			string line;
			string array[5];
			vector<double> values;

			getline(dataFile, line, '\n');
			// read in a line from csv file

			if (line == "" || line[0] == *"%") continue;
			// if the line is empty or starts with % we discard it
			// "%" is a const char pointer so we use the operator * to get the actual character 

			std::istringstream ss(line);
			for (int i = 0; i < 5; i++) {
				getline(ss, array[i], ',');
			}
			values.push_back(atof(array[3].c_str()));
			// the fourth member of the array is the latitude of the place and we convert it to a double using atof
			// but first we make it a c string so atof can convert it
			values.push_back(atof(array[4].c_str()));
			// the fifth member of the array is the longitude of the place and we convert it to a double using atof
			// but first we make it a c string so atof can convert it

			Settlement temp(id, values, stoi(array[2]), array[0], array[1]);
			// the first member of the array is the name of the place
			// the second member of the array is the type of the place
			// the third member of the array is the population of the place and we convert it to an integer using stoi

			points.push_back(temp);
			// puts temp in points
			id++;
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
	vector<Cluster> hubs;
	KMeans kmeans(K, points.size(), max_iterations);
	kmeans.run(points);
	hubs = kmeans.getClusters();
	//hill search used to find the best hub in the clusters
	for (auto& hub : hubs) {
		hill_search(hub.getPointVector());
	}
	return 0;
}
