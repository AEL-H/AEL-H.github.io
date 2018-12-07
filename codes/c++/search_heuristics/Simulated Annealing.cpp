#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<cmath>
#include<ctime>
#include<string>

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
			Data.push_back({ city }); // store the contents of the city into the data

		}



		// done reading the file, we can close it
		dataFile.close();
		result.Cities = Data;
		return result;	// return the matrix
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

// determines the distance from hub coordinate to city
double getDistance(double hubLat, double hubLong, GBPlace city)
{
	// defining variables, making sure to convert from degrees to radians
	hubLat *= PI / 180;
	double Lat2 = city.latitude*PI / 180;
	hubLong *= PI / 180;
	double Long2 = city.longitude*PI / 180;

	double dLat = Lat2 - hubLat;
	double dLong = Long2 - hubLong;

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

// generate random number
double random_number(double upper, double lower, int n) {
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

// function to calculate probability to accept solution
double acceptanceProbability(double Energy, double newEnergy, double temperature)
{
	// if new solution is better, accept it unconditionally
	if (newEnergy < Energy)
	{
		return 1.0;
	}
	else
	{
		return exp((Energy - newEnergy)/temperature);
	}
}

// main program
int main()
{
	// number of times we wish to run the main program before exitting (useful for collecting data)
	for (int i = 0; i < 1; i++){
		///////////////////////////

		// load in the data
		GBPlaces Data = getCities("GBplaces.csv", ',');
		//Data.printCities("G_B_Places.txt"); //output the cities to a textfile

		// Temperature settings
		double Temperature = 10000;
		double coolingRate = 0.01;

		// variables to store coordinates
		double Lat, Long, bestLat, bestLong;
		int dLat = 0;
		int	dLong = 0;

		// variables for handling solutions
		double currentEnergy;
		double newEnergy;
		double bestEnergy = DBL_MAX;

		// initialise initial solution
		// first pick a random starting point for latitude and longitude
		// bounding box found at https://wiki.ceh.ac.uk/display/cehigh/Bounding+box
		Lat = random_number(60.85, 49.84, 100);
		Long = random_number(2.69, -9.23, 100);
		// now work out the value of the hub cost function at initial point (Lat,Long)
		currentEnergy = hubCostFunction(Lat, Long, Data);

		// while we are cooling down
		while (Temperature > 1)
		{
			std::cout << "Temperature : " << Temperature << std::endl;
			double step = 0.00001*Temperature; // make stepsize proportional to temperature
			// check neighbours
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) {} // skip the case where we are on the same spot
					else {
						newEnergy = hubCostFunction(Lat + step * i, Long + step * j, Data);
						
						// maths to determine if we accept the new solution
						if (acceptanceProbability(currentEnergy, newEnergy, Temperature) > random_number(1, 0, 100000)) {
							dLat = i;
							dLong = j;
							currentEnergy = newEnergy;
						}
					}
				}
			}

			// update to new position and new value
			// value = maxValue;
			Lat += step * dLat;
			Long += step * dLong;

			// keep track of best solution
			if (currentEnergy < bestEnergy)
			{
				bestEnergy = currentEnergy;
				bestLat = Lat;
				bestLong = Long;
			}

			Temperature *= 1 - coolingRate; // update temperature
		}

		// let user know program finished and give the data they want to see
		std::cout << "Finished cooling!" << std::endl;
		std::cout << "Best solution : " << bestEnergy << std::endl;
		std::cout << "Occurred at : (" << bestLat << "," << bestLong << ")." << std::endl;

		// append results to file
		std::ofstream Results;
		std::string filename = "cooling_rate" + std::to_string(coolingRate) + "_stepsize0.00001Temperature.txt";
		std::cout << "Data appended to : "<< filename << std::endl;
		Results.open(filename, std::ios::app);
		Results << bestEnergy << "," << bestLat << "," << bestLong << "\n";
		/////////////////////////////
	}

	return 0;
}