
// PHYS 30762 Programming in C++
// Class for complex numbers
// Amir El-hamdy
// 9th March

#include<iostream>
#include<sstream>
#include<cmath>
using namespace std;

class complex
{
	// Make function to overload operator<< a friend
	friend ostream & operator<<(ostream &os, const complex &z);
	friend istream & operator>>(istream &is, complex &z);
private:
	// number in the form x+iy
	double x; // real part
	double y; // imaginary part
public:
	// Constructors and destructors

	// Default constructors
	complex(){ x = y = 0.; }
	// Parametrised constructor
	complex(double r, double i){ x = r; y = i; }
	// Destructor
	~complex(){}

	// Member functions

	// Return real component
	double real() const { return x; }
	// Return imaginary component
	double imaginary() const { return y; }
	// Return modulus
	double modulus() const { return sqrt(pow(x, 2) + pow(y, 2)); }
	// Return argument
	double argument() const { return atan2(y, x); }
	// Return modulus squared
	double modsq() const { return (pow(x, 2) + pow(y, 2)); }
	// Return complex conjugate
	complex conjugate() const { return complex(x, -y); }

	// Operator overloads

	// Overload + operator for addition 
	complex operator+(const complex &z) const
	{ // is it neater to put this all on one line like above?
		complex temp(x + z.real(), y + z.imaginary());
		return temp;
	}
	// Overload - operator for subtraction
	complex operator-(const complex &z) const
	{
		complex temp(x - z.real(), y - z.imaginary());
		return temp;
	}
	// Overload * operator with scalars
	complex operator*(const double &scalar) const
	{
		complex temp(x * scalar, y * scalar);
		return temp;
	}
	// Overload / operator with scalars
	complex operator/(const double &scalar) const
	{
		complex temp(x / scalar, y / scalar);
		return temp;
	}
	// Overload * operator for multiplication, z1*z2
	complex operator*(const complex &z) const
	{
		double temp_x{x * z.real() - y * z.imaginary()};
		double temp_y{x * z.imaginary() + y * z.real()};
		complex temp(temp_x, temp_y);
		return temp;
	}
	// Overload / operator for division, z1/z2
	complex operator/(const complex &z) const
	{
		 
		double temp_x{ (x*z.real() + y*z.imaginary())/z.modsq() };
		double temp_y{ (y*z.real() - x*z.imaginary())/z.modsq() };
		complex temp(temp_x, temp_y);
		return temp;
	}
};
// Friend functions

// Non-member function to overload << operator for complex numbers
ostream & operator<<(ostream &os, const complex &z)
{
	if ((os.flags() & ios_base::showpos)==0)
	{
		os << showpos << z.real() << z.imaginary() << "i" << noshowpos;
	}
	else
	{
	os << z.real() << z.imaginary() << "i";
	}
	return os;
}
// Non-member function to overload >> operator for complex numbers
istream & operator>>(istream &is, complex &z)
{
	std::streamoff pos{is.tellg()};
	double real;
	double imaginary;
	char sign;
	is >> real;
	is >> sign;
	if (is.get()!='i')
	{
	//  incorrect format of input, reset stream, flag fail, return
		is.seekg(pos);
		is.setstate(ios::failbit);
		return is;
	}
	is >> imaginary;
	// check the sign on the imaginary number
	switch (sign)
	{
	case '+':
		z.x = real;
		z.y = imaginary;
		return is;

	case '-':
		z.x = real;
		z.y = -imaginary;
		return is;

	default : // do nothing
		break;
	}
	// if we reach here, then there was no + or - sign
	// incorrect format of input, reset stream, flag fail, return
	is.seekg(pos);
	is.setstate(ios::failbit);
	return is;
}

int main()
{
	cout.precision(3);

	// Create two complex numbers
	complex a(3, 4);
	complex b(1, -2);

	// Get real and imaginary components, modulus and argument
	double a_real{ a.real() };
	double a_imaginary{ a.imaginary() };
	double a_mod{ a.modulus() };
	double a_arg{ a.argument() };

	double b_real{ b.real() };
	double b_imaginary{ b.imaginary() };
	double b_mod{ b.modulus() };
	double b_arg{ b.argument() };
	// Get conjugates
	complex a_conj{ a.conjugate() };
	complex b_conj{ b.conjugate() };
	// Get sum, difference, product and quotient of a and b
	complex sum{ a + b };
	complex diff{ a - b };
	complex prod{ a * b };
	complex quot{ a / b };
	// Print out results, also demonstrating use of overloaded operator<<
	cout << "a: " << a << endl;
	cout << "Real part: " << a_real << endl;
	cout << "Imaginary part: " << a_imaginary << endl;
	cout << "Modulus: " << a_mod << endl;
	cout << "Argument: " << a_arg << endl;
	cout << "Conjugate: " << a_conj << endl << endl;

	cout << "b: " << b << endl;
	cout << "Real part: " << b_real << endl;
	cout << "Imaginary part: " << b_imaginary << endl;
	cout << "Modulus: " << b_mod << endl;
	cout << "Argument: " << b_arg << endl;
	cout << "Conjugate: " << b_conj << endl << endl;
	// Show results of overloading arithmetic operators
	cout << "a + b: " << sum << endl;
	cout << "a - b: " << diff << endl;
	cout << "a * b: " << prod << endl;
	cout << "a / b: " << quot << endl << endl;

	// Show results of reading in complex numbers from istream
	complex input;
	cin >> input;
	cout << input << endl;
	return 0;
}