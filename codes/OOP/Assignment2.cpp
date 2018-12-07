// Assignment 2

// Program to compute mean, standard deviation and standard
// error of the mean electronic charge. Data is read from file

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<vector>
#include<string>
#include<sstream>
#include <process.h>

using namespace std;

// Functions to compute quantities
double getMean(double data[], int N);
double getStdDev(double data[], int N);
double getStdErrMean(double data[], int N);
// Functions to handle files given a filename
int getFileSize(string filename, char delim = ',');
double *getData(string filename, int N, char delim = ',');

int main()
{
	// Ask user to enter filename
	string filename;
	cout << "Please enter the name of the input file." << endl;
	cin >> filename;

	// Look through file and determine the number of readable objects in it
	int N{ getFileSize(filename) };

	// Allocate memory for data and populate it
	double* data{ getData(filename, N) };

	// Compute values and print
	const double factor{ 1e-19 };
	double Mean{ getMean(data, N) * factor};
	double StdDev{ getStdDev(data, N) * factor};
	double StdErrMean{ getStdErrMean(data, N) * factor};
	cout << "File has " << N << " elements." << endl;

	// set appropriate precision here
	cout << scientific<< setprecision(2)<<
	"Mean: " << Mean << "C"<<endl<<
	"Standard deviation: " << StdDev << "C"<<endl<<
	"Standard error in the mean: " << StdErrMean << "C." << endl;

	// free memory
	delete[] data;

	system("pause");
	return 0;
}

// Open file, read data into an array and return it
double *getData(string filename, int N, char delim)
{
	// In this function we assume the file has already
	// been read to determine the size of array needed

	// Allocate memory for data
	double* data = new double[N];

	int i = 0;

	// read data from file, ignoring any bad data
	ifstream dataFile(filename);
	if (dataFile.is_open())
	{
		string line; // string to hold line of file
		string element; // variable to temporarily hold strings

		// while not at the end of the file, get new line
		while (getline(dataFile, line))
		{
			stringstream split(line); // split the line

			// read the values from the line into element
			while (getline(split, element, delim))
			{
				if (element == ""){ continue; };
				try{
					data[i] = (stod(element)); // add element into array
					i++;
				}
				catch (...){
					// Not a double so ignore
				}
			}
		}
		// done reading the file, we can close it
		dataFile.close();
	}
	else{ // File could not be opened
		cout << "There was a problem opening the file, please check that the file is not already open or that you typed the path of the filename correctly." << endl;
		// free memory
		delete[] data;
		system("pause");
		exit(1); // exit with code 1
	}

	return data; // return the populated array
}

// Open file and read through to determine file size
// All error handling will be done in this function
int getFileSize(string filename, char delim)
{
	int N{ 0 };	// create a count variable

	ifstream dataFile(filename);
	double data; // double to try read into
	if (dataFile.is_open())
	{
		string line; // string to hold line of file
		string element; // variable to temporarily hold strings

		// while not at the end of the file, get new line
		while (getline(dataFile, line))
		{
			stringstream split(line); // split the line

			// read the values from the line into element
			while (getline(split, element, delim))
			{
				if (element == ""){ continue; };
				try{
					data = (stod(element)); // try to store element in a double
					N++;
				}
				catch (...){
					// if there is a problem reading the values
					// let user know there is a problem
					cout << "There was a problem reading value '" << element << "', ignoring value and moving on." << std::endl;
				}
			}
		}
		// done reading the file, we can close it
		dataFile.close();
	}
	else{ // File could not be opened
		cout << "There was a problem opening the file, please check that the file is not already open or that you typed the path of the filename correctly." << endl;
		system("pause");
		exit(1); // exit with code 1
	}

	return N; // Return the number of read-able elements
}
double getMean(double data[], int N)
{
	double sum{ 0 };
	for (int i{ 0 }; i < N; i++)
	{
		sum += data[i];
	}
	return sum / N;
}
double getStdDev(double data[], int N)
{
	double result{ 0 };
	double mean{ getMean(data, N) };
	for (int i{ 0 }; i < N; i++)
	{
		result += ((data[i] - mean)*(data[i] - mean));
	}
	return sqrt(result / (N - 1));
}
double getStdErrMean(double data[], int N)
{
	return (getStdDev(data, N) / sqrt(N));
}