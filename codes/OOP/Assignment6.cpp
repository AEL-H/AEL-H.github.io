// Matrix class
// Amir El-hamdy
// 15 March 2017
#include<string>
#include<iostream>
#include<sstream>

using namespace std;
// Class to contain the matrix
class matrix
{
	// Stream overloads
	friend ostream & operator<<(ostream &os, const matrix &mat);
	friend istream & operator>>(istream &is, matrix &mat);

private:
	int nof_rows;
	int nof_columns;
	int size;
	double *data;

public:

	// Constructors
	
	// Set everything to zero
	matrix()
	{
		nof_rows = 0;
		nof_columns = 0;
		size = 0;
		data = 0;
	}
	// Given no. of rows and columns, initialise zero matrix
	matrix(int rows, int columns)
	{
		nof_rows = rows;
		nof_columns = columns;
		size = rows*columns;
		data = new double[size];
		for (int i = 0; i < size; i++)
		{
			data[i] = 0.;
		}
	}
	// Given row, columns and data, initialise a deep copy
	matrix(int rows, int columns, double *arr)
	{
		nof_rows = rows;
		nof_columns = columns;
		size = rows*columns;
		data = new double[size];
		for (int i = 0; i < size; i++)
		{
			data[i] = arr[i];
		}
	}
	// Steal the data (move constructor)
	matrix(matrix &&mat)
	{
		cout << "Move constructor." << endl;
		nof_rows = mat.nof_rows;
		nof_columns = mat.nof_columns;
		size = mat.size;
		data = mat.data;
		mat.nof_rows = 0;
		mat.nof_columns = 0;
		mat.size = 0;
		mat.data = 0;
	}
	// Copy constructor for deep copying
	matrix(matrix &mat)
	{
		cout << "Copy constructor." << endl;
		// Copy size and declare new array
		data = 0;
		size = mat.length();
		nof_rows = mat.rows();
		nof_columns = mat.columns();
		if (size>0)
		{
			data = new double[size];
			// Copy values into new array
			for (int i = 0; i<size; i++) data[i] = mat.data[i];
		}
	}
	// Destructor
	~matrix()
	{
		delete[] data;
	}
	// Access functions
	int rows() const { return nof_rows; };			// return the rows of the matrix
	int columns() const { return nof_columns; };	// return the columns of the matrix
	int length() const { return size; }
	int index(int row, int column) const    // Return position in array of element (m,n)
	{
		if (row>=0 && row < nof_rows && column>=0 && column < nof_columns) return (column) + (row)*nof_columns;
		else { cout << "Error: out of range" << endl; exit(1); }
	}
	// Operator overloads 
	double & operator[](int i);	//??//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	matrix operator*(matrix &RHSmatrix) const;		// operator to return the result of matrix multiplication
	matrix operator+(matrix &RHSmatrix) const;		// operator to return the result of matrix addition
	matrix operator-(matrix &RHSmatrix) const;		// operator to return the result of matrix subtraction
	matrix & operator=(matrix &mat);		// operator for copy assignment
	matrix & operator=(matrix&& mat);		// operator for move assignment
	// Other functions
	matrix removeRow(int position);			// remove a row within the matrix given the position
	matrix removeColumn(int position);		// remove a column within the matrix given the position
	matrix getMinor(int row, int column);   // remove a row and column and return the result
	double determinant();					// return the determinant of the matrix
	void SwapRows(int pos1, int pos2);		// swaps two rows given their positions
};

// Member functions defined outside class

// operator overloads

// Operator overload of insertion operator
ostream & operator<<(ostream &os, const matrix &mat)
{
	// make sure output starts on a newline
	os << endl;
	for (int i = 0; i < mat.nof_rows; i++)
	{
		for (int j = 0; j < mat.nof_columns; j++)
		{
			os << mat.data[j+i*mat.nof_columns] << "\t";
		}
		os << endl;
	}

	return os;
}
// Operator overload of extraction operator
istream & operator>>(istream &is, matrix &mat)
{
	for (int i = 0; i < mat.size; i++)
	{
		is>>mat.data[i];
	}
	return is;
}
// operator overload to do matrix multiplication
matrix matrix::operator*(matrix &RHSmatrix) const
{
	if (nof_columns != RHSmatrix.nof_rows)
	{
		// check matrices can multiply
		cout << "Cannot multiply a " << nof_rows << "x" << nof_columns << " with a " << RHSmatrix.nof_rows << "x" << RHSmatrix.nof_columns << " matrix." << endl;
		exit(1);
	}
	// populate the new matrix row by row
	matrix result{ nof_rows, RHSmatrix.nof_columns };
	int entry = 0;
	for (int i = 0; i < nof_rows;i++)
	{
		for (int j = 0; j < RHSmatrix.nof_columns; j++)
		{
			double sum = 0.;
			for (int k = 0; k < nof_columns; k++)
			{
				sum += data[k + i*nof_columns] * RHSmatrix.data[j+k*RHSmatrix.nof_columns];
			}
			result.data[entry] = sum;
			entry++;
		}
	}
	return result;
}
// operator overload for matrix addition
matrix matrix::operator+(matrix &RHSmatrix) const
{
	if (nof_columns != RHSmatrix.nof_columns || nof_rows != RHSmatrix.nof_rows)
	{
		// check matrices match
		cout << "Matrix dimensions don't match." << endl;
		exit(1);
	}
	matrix result{nof_rows,nof_columns};
	// add matrices row by row
	for (int i = 0; i < size; i++)
	{
		result.data[i] = data[i] + RHSmatrix.data[i];
	}
	return result;
}
// operator overload for matrix subtraction
matrix matrix::operator-(matrix &RHSmatrix) const
{
	if (nof_columns != RHSmatrix.nof_columns || nof_rows != RHSmatrix.nof_rows)
	{
		// check matrices match
		cout << "Matrix dimensions don't match." << endl;
		exit(1);
	}
	matrix result{ nof_rows, nof_columns };
	// add matrices row by row
	for (int i = 0; i < size; i++)
	{
		result.data[i] = data[i] - RHSmatrix.data[i];
	}
	return result;
}
// assignment operator overload for deep copying??????????????????????????????
// Copy assignment operator for deep copying
matrix & matrix::operator=(matrix &mat)
{
	cout << "Copy assignment operator." << endl;
	if (&mat == this) return *this; // no self assignment
	// First delete this object's array
	delete[] data; data = 0; size = 0; nof_rows = 0; nof_columns = 0;
	// Now copy size and declare new array
	size = mat.length();
	nof_rows = mat.rows();
	nof_columns = mat.columns();
	if (size>0)
	{
		data = new double[size];
		// Copy values into new array
		for (int i = 0; i<size; i++) data[i] = mat.data[i];
	}
	return *this;
}
// Move assignment operator
matrix & matrix::operator=(matrix&& mat)
{
	cout << "Move assignment operator." << endl;
	swap(nof_rows, mat.nof_rows);
	swap(nof_columns, mat.nof_columns);
	swap(size, mat.size);
	swap(data, mat.data);
	return *this;
}
// function to return matrix with row removed
matrix matrix::removeRow(int position)
{
	if (position<0 || position >= nof_rows)  { cout << "Error: row out of range" << endl; exit(1); }


	matrix result{ nof_rows - 1, nof_columns };
	int entry = 0;
	for (int i = 0; i < nof_rows; i++)
	{
		for (int j = 0; j < nof_columns; j++)
		{
			if (i == position){ continue; }
			else
			{
				result.data[entry] = data[j + nof_columns*i];
				entry++;
			}
		}
	}
	return result;
}
// function to return matrix with column removed
matrix matrix::removeColumn(int position)
{

	if (position<0||position>=nof_columns)  { cout << "Error: column out of range" << endl; exit(1); }

	matrix result{ nof_rows, nof_columns - 1 };
	int entry = 0;
	for (int i = 0; i < nof_rows; i++)
	{
		for (int j = 0; j < nof_columns; j++)
		{
			if (j == position){ continue; }
			else
			{
				result.data[entry] = data[j + nof_columns*i];
				entry++;
			}
		}
	}
	return result;
}
// function to return matrix with row and column removed
matrix matrix::getMinor(int row, int column)
{

	if (column<0 || column >= nof_columns)  { cout << "Error: column out of range" << endl; exit(1); }
	if (row<0 || row >= nof_rows)  { cout << "Error: row out of range" << endl; exit(1); }


	matrix result{ nof_rows - 1, nof_columns - 1 };
	int entry = 0;
	for (int i = 0; i < nof_rows; i++)
	{
		for (int j = 0; j < nof_columns; j++)
		{
			if (j == column || i == row){ continue; }
			else
			{
				result.data[entry] = data[j + nof_columns*i];
				entry++;
			}
		}
	}
	return result;
}
// function to return determinant
double matrix::determinant()
{
	// check we have a square matrix
	if (nof_rows != nof_columns)
	{
		cout << "Matrix is not square. Cannot calculate determinant." << endl;
		exit(1);
	}

	double result = 0.;

	// we are finished when we reach a 2x2 matrix, return the determinant of this
	if (nof_rows == 2)
	{
		double result = data[0] * data[3] - data[1] * data[2];

		return result;
	}

	// calculate determinant across top row
	int i = 0;
	// loop across each column in first row
	for (int j = 0; j<nof_columns; j++)
	{
		// make a submatrix removing the ith row and jth column
		matrix tempmatrix{getMinor(i,j)};

		// determine if the coefficient is +1 or -1 depending on position
		// then make the recursive call to find the determinant of the submatrix
		if (j % 2 == 0)
		{
			int entry{ (j + i*nof_rows) };
			result += data[entry] * tempmatrix.determinant();
		}
		else
		{
			int entry{ (j + i*nof_rows) };
			result -= data[entry] * tempmatrix.determinant();
		}
	}

	return result;
}
// function to swap rows of the matrix, given the two positions
void matrix::SwapRows(int row1, int row2)
{
	double *temprow = new double[nof_columns];
	for (int i = 0; i < nof_columns; i++)
	{
		temprow[i] = data[nof_columns*row1 + i];
		data[nof_columns*row1 + i] = data[nof_columns*row2 + i];
		data[nof_columns*row2 + i] = temprow[i];
	}
	delete[] temprow;
	return;
}

int main()
{
	// Parametrised constructor
	matrix m1(3, 3);
	// Extraction operator overload
	stringstream("1 0 1 0 1 0 1 0 1") >> m1;
	// Insertion operator overload
	cout << "m1:" << m1;
	matrix m2(3, 3);
	stringstream("1 0 1 0 1 0 1 0 1") >> m2;
	cout << "m2:" << m2;
	// Addition, subtraction and multiplication overload
	cout << "m1+m2:" << m1 + m2 << endl;
	cout << "m1-m2:" << m1 - m2 << endl;
	cout << "m1*m2:" << m1 * m2 << endl;
	// Move constructor
	matrix m3{m1+m2};
	cout << "m3:" << m3;
	// Move assignment operator
	matrix m4;
	m4 = move(m1);
	cout << "m4:" << m4;
	cout << "m1:" << m1;
	// Copy constructor
	matrix m5{m1};
	// Copy assignment
	matrix m6;
	m6 = m2;
	matrix m7(4, 4);
	stringstream("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16") >> m7;
	cout << "m7:" << m7;
	cout << m7.getMinor(0,0);
	cout << "Determinant of m7 is: " << m7.determinant() << endl;
	// Demonstrate an invalid case of matrix multiplication
	//cout << m3*m7 << endl;
	//cout << m3+m7 << endl;
	//cout << m3-m7 << endl;
	return 0;
}