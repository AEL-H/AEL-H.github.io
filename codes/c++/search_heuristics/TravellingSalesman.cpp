// solving TSP using nearest neighbour algorithm
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<cmath>
#include<ctime>
#include<string>

#include"Matrices.h"

#define R 3958.75	// radius of earth in miles
#define PI 3.14159	// value of pi required for converting to radians
int fevals = 0;		// global variable to keep count of function evaluations

// structure to contain the information of a city
// and provide useful methods to act on a city
struct GBPlace
{
	std::string place;
	std::string type;
	int population;
	double latitude;
	double longitude;

	// print the city
	// requires no arguments
	// can also be called with a file pointer to write output to a file
	void printcity(FILE *outFile = NULL)
	{
		// if file pointer is given, send output to file instead
		if (outFile != NULL)
		{

			fprintf(outFile, "%-20s    ", place.c_str());
			fprintf(outFile, "%-4s    ", type.c_str());
			fprintf(outFile, "%-7i    ", population);
			fprintf(outFile, "%-8.5f    ", latitude);
			fprintf(outFile, "%-8.5f", longitude);
			fprintf(outFile, "\n"); // newline character when the city information has finished printing
			return;
		}

		// only reach here if file pointer is not given
		// same again, only now the output is send to standard output

		printf("%-20s    ", place.c_str());
		printf("%-4s    ", type.c_str());
		printf("%-7i    ", population);
		printf("%-8.5f    ", latitude);
		printf("%-8.5f", longitude);
		printf("\n"); // newline character when the city information has finished printing
		return;
	}

};

// structure to contain all the cities
// and provide useful methods to act on the cities
struct GBPlaces
{
	std::vector<GBPlace> Cities;

	// function to print out cities and their respective information
	// utilises the printcity() contained in the GBPlace struct
	// if a filename is given, sends output to a file
	// otherwise all output is sent to standard output
	void printCities(char * filename = "")
	{
		if (filename != "")
		{
			// only reach here if a filename was given
			FILE *outFile;
			outFile = fopen(filename, "w"); // initialise pointer to point to file
			// loop over the cities
			for (int i = 0; i < (int)Cities.size(); i++)
			{
				Cities[i].printcity(outFile);		// print city values passing in file pointer for the printcity() function
			}
			fclose(outFile);
			return;
		}

		// only reach here if no filename is given

		// loop over the cities
		for (int i = 0; i < (int)Cities.size(); i++)
		{
			Cities[i].printcity();		// print city values
		}
		return;
	}
};



// function to create a GBPlaces struct from input file given the filename
// optional second parameter to include the delimiter of the input file
GBPlaces getCities(char * fileName, char delim = ' ')
{
	std::ifstream dataFile(fileName);	// open the file
	if (dataFile.is_open())
	{
		GBPlaces result;
		GBPlace city;
		std::vector<GBPlace> Data;		// vector of cities
		std::string line;			// string to hold line of file
		std::string header;			// string to hold the header

		// variables to temporarily hold values
		std::string element;

		// skip first line
		getline(dataFile, header);

		// while not at the end of the file, get new line
		while (std::getline(dataFile, line))
		{
			std::stringstream split(line); // split the line

			// read the values from the line into element
			std::vector<std::string> temp;
			while (std::getline(split, element, delim))
			{
				try{
					temp.push_back(element); // add element onto temporary row
				}
				catch (...){
					// if there is a problem reading the values
					// let user know there is a problem and quit
					std::cout << "There is a problem reading the values." << std::endl;
					exit(1);
				}
			}
			city.place = temp[0];
			city.type = temp[1];
			city.population = std::stoi(temp[2]);
			city.latitude = std::stod(temp[3]);
			city.longitude = std::stod(temp[4]);
			Data.push_back({ city }); // store the contents of the row into the data

		}



		// done reading the file, we can close it
		dataFile.close();
		result.Cities = Data;
		return result;	// return the struct
	}
	else
	{
		// only arrive here if file is not open
		// let the user know what is happening
		// then quit the program
		std::cout << "Problem opening the file.\n";
		exit(1);
	}
}
// distance between two cities
double getDistance(GBPlace startcity, GBPlace destinationcity)
{
	// defining variables, making sure to convert from degrees to radians
	double Lat1 = startcity.latitude*PI / 180;
	double Lat2 = destinationcity.latitude*PI / 180;
	double dLat = (destinationcity.latitude - startcity.latitude)*PI / 180;
	double dLong = (destinationcity.longitude - startcity.longitude)*PI / 180;

	// applying the haversine formula
	double a = pow(sin(dLat / 2), 2) + cos(Lat1)*cos(Lat2)*pow(sin(dLong / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double distance = R*c;
	return distance; // return the great circle distance in miles
}

// useful for travelling salesman problem
matrix makecostmatrix(GBPlaces Data)
{
	matrix result;

	double cost;
	for (int i = 0; i < (int)Data.Cities.size(); i++)
	{
		row temprow;
		for (int j = 0; j < (int)Data.Cities.size(); j++)
		{
			if (i == j)
			{
				// then we are looking at the distance between city A and city A, so make cost infinity
				cost = DBL_MAX;
				temprow.column.push_back(cost);
			}
			else
			{
				cost = getDistance(Data.Cities[i], Data.Cities[j])*Data.Cities[j].population; // assume cost is proportional to distance * population
				temprow.column.push_back(cost);
			}
		}
		result.row.push_back(temprow);
	}
	return result;
}


struct Tour
{
	std::vector<GBPlace> Cities;

	// function to print out cities and their respective information
	// utilises the printcity() contained in the GBPlace struct
	// if a filename is given, sends output to a file
	// otherwise all output is sent to standard output
	void printCities(char * filename = "")
	{
		if (filename != "")
		{
			// only reach here if a filename was given
			FILE *outFile;
			outFile = fopen(filename, "w"); // initialise pointer to point to file
			// loop over the cities
			for (int i = 0; i < (int)Cities.size(); i++)
			{
				Cities[i].printcity(outFile);		// print city values passing in file pointer for the printrow() function
			}
			fclose(outFile);
			return;
		}

		// only reach here is no filename is given

		// loop over the cities
		for (int i = 0; i < (int)Cities.size(); i++)
		{
			Cities[i].printcity();		// print city values
		}
		return;
	}

	// work out the cost of a tour
	// cost is proportional to distance * population
	double cost()
	{
		double result = 0;
		// loop over the tour
		for (int i = 0; i < (int)Cities.size(); i++)
		{
			if (i == ((int)Cities.size() - 1))
			{
				result += getDistance(Cities[i], Cities[0])*Cities[0].population; // from end city to beginning
			}
			else{
				result += getDistance(Cities[i], Cities[i + 1])*Cities[i+1].population; // intermediate cities
			}
		}

		return result;
	}
};

// generate random number
double random_number(double upper, double lower, int n) {
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

// returns a tour
Tour NearestNeighbour(GBPlaces Data,matrix costmatrix, int startCityIndex)
{
	Tour result;

	// loop over the number of cities
	for (int N = 0; N < costmatrix.rows(); N++)
	{
		// add city on to the tour
		result.Cities.push_back(Data.Cities[startCityIndex]);
		double temp;
		int NearestNeighbourIndex;
		double NearestNeighbour = DBL_MAX;
		// make entries in the column equal to infinity as the city has now been visited
		for (int k = 0; k < costmatrix.rows(); k++){ costmatrix.row[k].column[startCityIndex] = DBL_MAX; }
		// loop over the different cities from current city
		for (int i = 0; i < costmatrix.row[startCityIndex].columns(); i++)
		{
			temp = costmatrix.row[startCityIndex].column[i];
			if (temp < NearestNeighbour)// if shortest path
			{
				// update values
				NearestNeighbour = temp;
				NearestNeighbourIndex = i;
			}
		}
		// If we are not at the end, move on to next city
		if (N<costmatrix.rows()-1){
			startCityIndex = NearestNeighbourIndex;
		}
	}
	return result;
}

int main()
{
	GBPlaces Data = getCities("GBplaces.csv", ',');
	Data.printCities("G_B_Places.txt");
	makecostmatrix(Data).printMatrix("costmatrix.txt");
	// for each city
	int startCityIndex;
	double minimumdistance = DBL_MAX;
	for (int i = 0; i < (int)Data.Cities.size(); i++)
	{
		// apply the nearestneighbour algorithm with the ith city as the startpoint
		std::cout << "Considering city " << Data.Cities[i].place << " as a starting point." << std::endl;
		Tour nearestneighbourTour = NearestNeighbour(Data, makecostmatrix(Data), i);

		if (nearestneighbourTour.cost()<minimumdistance)
		{
			minimumdistance = nearestneighbourTour.cost();
			startCityIndex = i;
		}

	}

	std::cout << std::endl;

	Tour nearestneighbourTour = NearestNeighbour(Data,makecostmatrix(Data),startCityIndex); // create the minimum distance tour
	nearestneighbourTour.printCities("Tour.txt"); // send this output to a file where successive destinations are listed beneath eachother
	std::cout <<"Tour Cost : " << nearestneighbourTour.cost() << " Starting at : " << Data.Cities[startCityIndex].place << std::endl;
	return 0;
}

//*/