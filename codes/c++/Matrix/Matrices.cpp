// Matrices
#include "Matrices.h"
// Contains tools to read in a matrix and perform operations on the matrices

	// operator overload to add a row onto current row
	void row::operator +=(row addend)
	{
		// if rows do not match, print this and exit
		if (addend.columns() != columns()){ std::cout << "Rows do not match\n"; exit(1); }
		for (int i = 0; i < (int)column.size(); i++)
		{
			column[i] += addend.column[i];
		}

		return;
	}

	// operator overload to subtract a row onto current row
	void row::operator -=(row subtractant)
	{
		// if rows do not match, print this and exit
		if (subtractant.columns() != columns()){ std::cout << "Rows do not match\n"; exit(1); }
		for (int i = 0; i < (int)column.size(); i++)
		{
			column[i] -= subtractant.column[i];
		}

		return;
	}

	// operator overload to add two rows together
	row row::operator +(row addend)
	{
		// if rows do not match, print this and exit
		if (addend.columns() != columns()){ std::cout << "Rows do not match\n"; exit(1); }
		row result;
		for (int i = 0; i < (int)column.size(); i++)
		{
			result.column.push_back(column[i] + addend.column[i]);
		}

		return result;
	}

	// operator overload to subtract a row from another
	row row::operator -(row subtractant)
	{
		// if rows do not match, print this and exit
		if (subtractant.columns() != columns()){ std::cout << "Rows do not match\n"; exit(1); }
		row result;
		for (int i = 0; i < (int)column.size(); i++)
		{
			result.column.push_back(column[i] - subtractant.column[i]);
		}

		return result;
	}

	// operator overload to multiply a row
	void row::operator *=(double multiplier)
	{
		for (int i = 0; i < (int)column.size(); i++)
		{
			column[i] *= multiplier;
		}
		return;
	}

	// print the row
	// requires no arguments
	// can also be called with a file pointer to write output to a file
	void row::printrow(FILE *outFile)
	{
		// if file pointer is given, send output to file instead
		if (outFile != NULL)
		{
			for (int i = 0; i < (int)column.size(); i++)
			{
				fprintf(outFile,"%5.2f	", column[i]);
			}
			fprintf(outFile,"\n"); // newline character when the row has finished printing
			return;
		}

		// only reach here if file pointer is not given
		// same again, only now the output is sent to standard output

		for (int i = 0; i < (int)column.size();i++)
		{
			printf("%5.2f	",column[i]);
		}
		printf("\n");
		return;
	}

	// function that returns the number of columns in the row
	int row::columns()
	{
		return (int)column.size();
	}




	// operator overload to do matrix multiplication
	matrix matrix::operator*(matrix RHSmatrix)
	{
		matrix LHSmatrix;
		matrix result;
		LHSmatrix.row = this->row;
		if (LHSmatrix.columns()!=RHSmatrix.rows())
		{
			// check matrices can multiply
			std::cout << "Cannot multiply a " << LHSmatrix.rows() << "x" << LHSmatrix.columns() << " with a " << RHSmatrix.rows() <<  "x" << RHSmatrix.columns() << " matrix." << std::endl;
			exit(1);
		}

		
		// populate the new matrix row by row
		for (int i = 0; i < LHSmatrix.rows(); i++)
		{
			::row temprow;
			for (int k = 0; k < RHSmatrix.columns(); k++)
			{
				double sum = 0.;
				for (int j = 0; j < LHSmatrix.columns(); j++)
				{
					sum += LHSmatrix.row[i].column[j]*RHSmatrix.row[j].column[k];
				}
				temprow.column.push_back(sum);
			}
			result.row.push_back(temprow);
		}
		return result;
	}

	// operator overload for matrix addition
	matrix matrix::operator+(matrix RHSmatrix)
	{

		matrix LHSmatrix;
		LHSmatrix.row = this->row;

		if (LHSmatrix.columns()!=RHSmatrix.columns()||LHSmatrix.rows()!=RHSmatrix.rows())
		{
			// check matrices match
			std::cout << "Matrices don't match." << std::endl;
			exit(1);
		}
		matrix result;
		// add matrices row by row
		for (int i = 0; i < LHSmatrix.rows(); i++)
		{
			result.row.push_back(LHSmatrix.row[i]+RHSmatrix.row[i]);
		}
		return result;
	}

	// operator overload for matrix subtraction
	matrix matrix::operator-(matrix RHSmatrix)
	{

		matrix LHSmatrix;
		LHSmatrix.row = this->row;

		if (LHSmatrix.columns() != RHSmatrix.columns() || LHSmatrix.rows() != RHSmatrix.rows())
		{
			// check matrices match
			std::cout << "Matrices don't match." << std::endl;
			exit(1);
		}
		matrix result;

		for (int i = 0; i < LHSmatrix.rows(); i++)
		{
			// subtract matrices row by row
			result.row.push_back(LHSmatrix.row[i] - RHSmatrix.row[i]);
		}
		return result;
	}

	// function to remove row of matrix, given position
	void matrix::removeRow(int position)
	{
		this->row.erase(this->row.begin() + position);
	}
	
	// function to remove column of matrix, given position
	void matrix::removeColumn(int position){
		for (int i = 0; i < rows();i++)
		{
			this->row[i].column.erase(this->row[i].column.begin()+position);
		}
	}

	




	// calculates the inverse of a matrix using gaussian elimination
	matrix matrix::inverse()
	{
		// if matrix is not square, print this and exit
		if (columns()!=rows())
		{
			std::cout << "Matrix is not square!\n";
			exit(1);
		}
		matrix matrixcopy; // temporary matrix to hold current values
		matrixcopy.row = this->row; // copy the current matrix into the temporary one
		int n = rows(), m = rows(); // n,m are rows and columns respectively
		matrix result = identity(n); // initialise result as identity matrix
		
		// first part of gaussian elimination

		// bottom half of matrix

		// iterate over the columns
		for (int i = 0; i < m; i++)
		{
			// iterate over the rows
			for (int j = i; j < m; j++)
			{
				if (i == j)
				{
					// check if leading diagonal element is 0
					if (matrixcopy.row[j].column[i]==0.)
					{
						// if we are on the last row, the matrix is not invertible
						if (j==m-1)
						{
							std::cout << "Cannot find inverse" << std::endl;
							exit(1);
						}
						// otherwise swap with the rows below
						for (int k = j + 1; k < m; k++)
						{
							if (matrixcopy.row[k].column[i] != 0.)
							{
								matrixcopy.SwapRows(k,j);
								result.SwapRows(k,j);
								break;
							}
							// until we reach the last row, then declare there is no inverse
							if (k == m - 1){
								std::cout << "Cannot find inverse" << std::endl << "Determinant is : " << determinant() << std::endl;
								exit(1);
							}
						}

					}

					// if we are on the leading diagonal and it is not zero, multiply the row to make the entry equal to 1
					double multiplier = 1. / matrixcopy.row[j].column[i];
					matrixcopy.row[j] *= multiplier;
					result.row[j] *= multiplier;
				}
				else
				{
					// if we are not on the leading diagonal, subtract to make the entries equal to zero
					
						::row addend = matrixcopy.row[i];
						double multiplier = matrixcopy.row[j].column[i];
						addend *= multiplier;
						matrixcopy.row[j] -= addend;

						addend = result.row[i];
						addend *= multiplier;
						result.row[j] -= addend;
					
				}

			}
		}

		// we have reached row echelon form

		// second part of gaussian elimination
		for (int i = m-1; i > 0; i--)
		{
			for (int j = i-1; j > -1; j--)
			{
				// subtract the rest of the rows to make them equal to zero
				::row addend = matrixcopy.row[i];
				double multiplier = matrixcopy.row[j].column[i];
				addend *= multiplier;
				matrixcopy.row[j] -= addend;

				addend = result.row[i];
				addend *= multiplier;
				result.row[j] -= addend;
			}
		}

		return result;
	}

	// function to print out matrix
	// utilises the printrow() contained in the row struct
	// if a filename is given, sends output to a file
	// otherwise all output is sent to standard output
	void matrix::printMatrix(char * filename )
	{
		if (filename != "")
		{
			// only reach here if a filename was given
			FILE *outFile;
			outFile = fopen(filename, "w"); // initialise pointer to point to file
			// loop over the rows
			for (int i = 0; i < (int)row.size(); i++)
			{
				row[i].printrow(outFile);		// print row values passing in file pointer for the printrow() function
			}
			fclose(outFile);
			return;
		}

		// only reach here is no filename is given

		// loop over the rows
		for (int i = 0; i < (int)row.size(); i++)
		{
			row[i].printrow();		// print row values
		}
		return;
	}

	// function to calculate the transpose of the matrix
	matrix matrix::transpose()
	{ 
		matrix result; // matrix to contain the returned value

		// temporary variables to construct the result
		std::vector<::row> matrixrows;
		::row temprow;

		// loop over columns
		for (int j = 0; j < columns(); j++)
		{
			std::vector<double> temp;
			// loop over rows
			for (int i = 0; i < rows(); i ++)
			{
				temp.push_back(row[i].column[j]); // each row of result matrix is constructed from the column of the existing matrix
			}
			temprow.column = temp;
			matrixrows.push_back(temprow); // add the row, then move onto the next column and repeat
		}
		result.row = matrixrows;
		return result;
	}

	// function that returns the number of rows
	int matrix::rows()
	{
		return (int)row.size();
	}
	// function that returns the number of columns
	int matrix::columns()
	{
		if (rows() == 0){ return 0; } // if row doesn't exist, return 0
		return row[row.size() - 1].columns();
	}

	// recursive function to determine the determinant
	// this should be called with no arguments
	double matrix::determinant()
	{
		matrix subMatrix;
		subMatrix.row = this->row;
		// if rows and columns do not match, it is not a square matrix
		if (rows() != columns())
		{
			std::cout << "Matrix is not square. Cannot calculate determinant." << std::endl;
			exit(1);
		}
		// recursive call to function that takes in a matrix
		return determinant(subMatrix);

	}

	// should only be called by the determinant() function above
	double matrix::determinant(matrix subMatrix)
	{
		double result = 0.;

		// we are finished when we reach a 2x2 matrix, return the determinant of this
		if (subMatrix.rows() == 2)
		{
			double result = subMatrix.row[0].column[0] * subMatrix.row[1].column[1] - subMatrix.row[0].column[1] * subMatrix.row[1].column[0];
			return result;
		}

		// calculate determinant across top row
		int i = 0;
			// loop across each column in first row
			for (int j = 0; j<subMatrix.columns(); j++)
			{
				// make a submatrix removing the ith row and jth column
				matrix tempmatrix;
				tempmatrix.row = subMatrix.row;
				tempmatrix.removeColumn(j);
				tempmatrix.removeRow(i);

				// determine if the coefficient is +1 or -1 depending on position
				// then make the recursive call to find the determinant of the submatrix
				if (j % 2 == 0)
				{
					result += subMatrix.row[i].column[j] * determinant(tempmatrix);
				}
				else
				{
					result -= subMatrix.row[i].column[j] * determinant(tempmatrix);
				}
			}
		
		return result;
	}

	// function to swap rows of the matrix, given the two positions
	void matrix::SwapRows(int pos1, int pos2)
	{
		::row temp = this->row[pos1];
		this->row[pos1] = this->row[pos2];
		this->row[pos2] = temp;
		return;
	}


// function to create a matrix from input file given the filename
// optional second parameter to include the delimiter of the input file
matrix getMatrix(char * fileName, char delim)
{
	std::ifstream dataFile(fileName);	// open the file
	if (dataFile.is_open())
	{
		matrix result;
		std::vector<row> Data;		// vector of rows
		std::string line;			// string to hold line of file
		// variables to temporarily hold values
		std::string element;

			// while not at the end of the file, get new line
			while (std::getline(dataFile, line))
			{
				std::stringstream split(line); // split the line

				// read the values from the line into element
				std::vector<double> temp;
				while (std::getline(split, element, delim))
				{
					if (element == ""){ continue; };
					try{
						temp.push_back(stod(element)); // add element onto temporary row
					}
					catch(...){
						// if there is a problem reading the values
						// let user know there is a problem and quit
						std::cout << "There is a problem reading the values." << std::endl;
						std::cout << "Please ensure the input file is separated by '" << delim << "'," << std::endl;
						std::cout << "and any absence of value is denoted by a '0'." << std::endl;
						exit(1);
					}
					}
				Data.push_back({ temp }); // store the contents of the row into the data

			}
			
		

		// done reading the file, we can close it
		dataFile.close();
		result.row = Data;
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

// returns the identity matrix of a given size
matrix identity(int size)
{
	matrix result;
	// temporary variables to construct the result
	std::vector<::row> matrixrows;
	::row temprow;

	// loop over columns
	for (int j = 0; j < size; j++)
	{
		std::vector<double> temp;
		// loop over rows
		for (int i = 0; i < size; i++)
		{
			if (i == j){ temp.push_back(1); }
			else{ temp.push_back(0); }
		}
		temprow.column = temp;
		matrixrows.push_back(temprow); // add the row, then move onto the next column and repeat
	}
	result.row = matrixrows;
	return result;
}
