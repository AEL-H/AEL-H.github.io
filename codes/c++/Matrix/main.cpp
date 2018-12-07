// main program
#include "Matrices.h"
int main()
{
	matrix Data = getMatrix("testinverse.txt", ','); // create matrix from input file (pass filename and delimiter as arguments)

	std::cout << "The Matrix : " << std::endl;
	Data.printMatrix();					// to view the matrix in standard output
	Data.printMatrix("The_Matrix.txt");	// to output matrix to file "The_Matrix.txt"
	std::cout << std::endl;

	std::cout << "The Inverse Matrix : " << std::endl;
	Data.inverse().printMatrix();							// to view the inverse matrix in standard output
	Data.inverse().printMatrix("The_Inverse_Matrix.txt");	// to output the matrix to file "The_Inverse_Matrix.txt"
	std::cout << std::endl;

	std::cout << "The determinant is : " << Data.determinant() << std::endl << std::endl;

	std::cout << "This should produce a matrix of zeroes if inverse is found : " << std::endl;
	(Data * Data.inverse() - identity(Data.rows())).printMatrix(); // multiply matrix with its inverse and subtract the identity matrix | should equal square matrix of zeros

	return 0;
}