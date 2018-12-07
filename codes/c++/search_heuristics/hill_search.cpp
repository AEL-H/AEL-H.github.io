#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<cmath>
#include<ctime>

#define R 3958.75	// radius of earth in miles
#define PI 3.14159	// value of pi required for converting to radians

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
			// loop over the rows
			for (int i = 0; i < (int)Cities.size(); i++)
			{
				Cities[i].printcity(outFile);		// print city values passing in file pointer for the printcity() function
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
		std::vector<GBPlace> Data;		// vector of GBPlace structs
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

double getDistance(GBPlace startcity, GBPlace destinationcity)
{
	// defining variables, making sure to convert from degrees to radians
	double Lat1 = startcity.latitude*PI / 180;
	double Lat2 = destinationcity.latitude*PI / 180;
	double dLat = (destinationcity.latitude - startcity.latitude)*PI/180;
	double dLong = (destinationcity.longitude - startcity.longitude)*PI/180;

	// applying the haversine formula
	double a = pow(sin(dLat / 2), 2) + cos(Lat1)*cos(Lat2)*pow(sin(dLong / 2), 2);
	double c = 2 * atan2(sqrt(a),sqrt(1-a));
	double distance = R*c;
	return distance; // return the great circle distance
}

double getDistance(double hubLat, double hubLong, GBPlace city)
{
	// defining variables, making sure to convert from degrees to radians
	hubLat *= PI / 180;
	double Lat2 = city.latitude*PI / 180;
	hubLong *= PI / 180;
	double Long2 = city.longitude*PI / 180;

	double dLat = Lat2-hubLat;
	double dLong = Long2-hubLong;

	// applying the haversine formula
	double a = pow(sin(dLat / 2), 2) + cos(hubLat)*cos(Lat2)*pow(sin(dLong / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double distance = R*c;
	return distance; // return the great circle distance in miles
}

// function determines the sum of distances from hub to every city
double hubCostFunction(double hubLatitude, double hubLongitude, GBPlaces Data)
{
	double cost = 0;
	for (int i = 0; i < (int)Data.Cities.size(); i++)
	{
		cost += getDistance(hubLatitude, hubLongitude, Data.Cities[i]) * Data.Cities[i].population; // cost equal to distance multiplied by population
	}
	return cost;
}

double random_number(double upper, double lower, int n) {
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

int main()
{
	// number of times to run the main program before exitting (useful for collecting data)
	for (int RESULTS = 0; RESULTS < 1; RESULTS++){

		// load the data in
		GBPlaces Data = getCities("GBplaces.csv", ',');

		double Lat, Long, glLat, glLong; // variables to keep track of coordinates
		int dLat, dLong;	// variables to update coordinates

		// step size whilst climbing
		double step = 0.01;

		// variables to keep track of solutions
		double value, newValue, oldValue;
		double globalMin = DBL_MAX;	// initialise global minimum as max value

		srand((int)time(NULL));

		// initialise these variables to zero initially
		glLat = 0;
		glLong = 0;

		// number of times we would like to repeat hill climbing in order to try find a global minima
		int NUMBER_OF_ITERATIONS = 10;


		for (int k = 0; k < NUMBER_OF_ITERATIONS; k++) {
			std::cout << "iteration : " << k + 1 << std::endl;
			// first pick a random starting point for latitude and longitude
			// bounding box found at https://wiki.ceh.ac.uk/display/cehigh/Bounding+box
			Lat = random_number(60.85, 49.84, 100);
			Long = random_number(2.69, -9.23, 100);

			// now work out the value of the hub cost function at point (Lat,Long)
			value = hubCostFunction(Lat, Long, Data);

			do {
				// now look around the current point and see if we can go somewhere where hub cost function is lower
				oldValue = value;

				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i == 0 && j == 0) {} // skip the case where we are on the same spot
						else {
							newValue = hubCostFunction(Lat + step * i, Long + step * j, Data);
							if (newValue <= value) {
								dLat = i;
								dLong = j;
								value = newValue;
							}
						}
					}
				}

				// update to new position and new value
				// value = maxValue;
				Lat += step * dLat;
				Long += step * dLong;

			} while (value < oldValue);

			if (value < globalMin){
				std::cout << "Global minimum value changing from " << globalMin << " at (" << glLat << "," << glLong << ")" << "to " << value << " at (" << Lat << "," << Long << ")" << " on iteration " << k + 1 << std::endl;
				globalMin = value;
				glLat = Lat;
				glLong = Long;
			}

		}

		std::cout << std::endl;
		std::cout << "Best result found at : ("<< glLat << "," << glLong << ") with a value of : " << globalMin << "\n";

		// append results to file
		std::ofstream Results;
		std::string filename = "hillsearch_" + std::to_string(NUMBER_OF_ITERATIONS) + "iterations_step0.01.txt";
		std::cout << "Results appended to : "<< filename << std::endl;
		Results.open(filename, std::ios::app);
		Results << globalMin << "," << glLat << "," << glLong << "\n";
	}

	return 0;
	///////////////////////////////////////////////////////////
}


//*/