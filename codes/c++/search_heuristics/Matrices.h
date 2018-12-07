#pragma once		// header guard

// includes we will need for the project

#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

// Declarations

// structure to hold a row of the matrix
// and provide useful methods for manipulating and printing rows
struct row
{
	std::vector<double> column;				// vector to contain the row
	void operator +=(row addend);			// operator to add a row onto current row
	void operator -=(row subtractant);		// operator to subtract a row from the current row
	row operator +(row addend);				// operator to return the result of adding two rows
	row operator -(row subtractant);		// operator to return the result of subtracting two rows
	void operator *=(double multiplier);	// operator to multipy current row by given multiplier
	void printrow(FILE *outFile = NULL);	// method to print the current row to stdout or a file if given a file pointer
	int columns();
};

// structure to contain the matrix
struct matrix
{
	std::vector<row> row;					// vector to contain the rows
	matrix operator*(matrix RHSmatrix);		// operator to return the result of matrix multiplication
	matrix operator+(matrix RHSmatrix);		// operator to return the result of matrix addition
	matrix operator-(matrix RHSmatrix);		// operator to return the result of matrix subtraction
	void removeRow(int position);			// remove a row within the matrix given the position
	void removeColumn(int position);		// remove a column within the matrix given the position
	matrix inverse();						// return the inverse of the matrix
	void printMatrix(char * filename = "");	// print out the matrix to stdout or a file if given a filename
	matrix transpose();						// return the transpose of the matrix
	int rows();								// return the rows of the matrix
	int columns();							// return the columns of the matrix
	double determinant();					// return the determinant of the matrix
	double determinant(matrix subMatrix);	// returns the determinant given a submatrix, this is used by the determinant() declared above
	void SwapRows(int pos1, int pos2);		// swaps two rows given their positions
};
matrix getMatrix(char * fileName, char delim = ' ');	// reads text file and returns matrix, optionally can be given a delimiter
matrix identity(int size);								// returns identity matrix given size