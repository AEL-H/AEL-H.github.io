// Amir El-hamdy
// 24 March 2017
// Classes for a general vector, a four vector and a particle
#include<string>
#include<iostream>
#include<sstream>
#include<cmath>
#include<cassert>
using namespace std;

// Class to contain the nVector
// Allows for an n-dimensional cartesian vector with basic maths operator overloads
// with friend functions to allow for extraction and insertion with input and output streams
class nVector
{
	// Stream overloads
	friend ostream & operator<<(ostream &os, const nVector &vec);
	friend istream & operator>>(istream &is, nVector &vec);

private:
protected:
	int m_dimension;
	double *m_elements;
public:

	// Constructors and destructor

	// Empty nVector
	nVector(){ m_dimension = 0; m_elements = 0; }
	// Zero nVector
	nVector(const int dimension)
	{
		m_dimension = dimension;
		m_elements = new double[dimension];
		for (int i = 0; i < dimension; i++) m_elements[i] = 0;
	}
	// nVector from array
	nVector(const int dimension, const double *source)
	{
		m_dimension = dimension;
		m_elements = new double[dimension];
		for (int i = 0; i < dimension; i++) m_elements[i] = source[i];
	}
	// Move constructor
	nVector(nVector &&vec)
	{
		m_dimension = vec.m_dimension;
		m_elements = vec.m_elements;
		vec.m_dimension = 0;
		vec.m_elements = 0;
	}
	// Copy constructor for deep copying
	nVector(const nVector &vec)
	{
		m_elements = 0;
		m_dimension = vec.m_dimension;
		if (m_dimension>0)
		{
			m_elements = new double[m_dimension];
			// Copy values into new array
			for (int i = 0; i<m_dimension; i++) m_elements[i] = vec.m_elements[i];
		}
	}
	// Destructor
	~nVector(){ delete[] m_elements; }
	// Access functions
	int dimension() const { return m_dimension; }
	// Operator overloads 
	double & operator[](const int i) const;
	double operator*(const nVector &RHSvec) const;	// operator to return the result of nVector dot product
	nVector operator*(const double RHSscalar) const;// operator to return the result of multiplication of nVector with a scalar
	nVector operator+(const nVector &RHSvec) const;	// operator to return the result of nVector addition
	nVector operator-(const nVector &RHSvec) const;	// operator to return the result of nVector subtraction
	nVector & operator=(const nVector &vec);		// operator for copy assignment
	nVector & operator=(nVector&& vec);				// operator for move assignment
};
// Member functions defined outside class
// Operator overloads

// Operator overload of insertion operator
ostream & operator<<(ostream &os, const nVector &vec)
{
	if (vec.m_dimension == 0){ os << "()"; return os; }
	os << "(";
	for (int i = 0; i < vec.m_dimension-1; i++) os << vec.m_elements[i] << ", ";
	os << vec.m_elements[vec.m_dimension - 1] << ")";
	return os;
}
// Operator overload of extraction operator
istream & operator>>(istream &is, nVector &vec)
{
	for (int i = 0; i < vec.m_dimension; i++) is >> vec.m_elements[i];
	return is;
}
// Operator overload to access nVector components
double & nVector::operator[](const int i) const 
{
	if (i >= m_dimension || i < 0){ cout << "Index out of range" << endl; exit(1); }
	return m_elements[i];
}
// Operator overload to do nVector dot product
double nVector::operator*(const nVector &RHSvec) const
{
	if (m_dimension!=RHSvec.m_dimension)
	{
		cout << "Vector dimensions do not match." << endl;
		exit(1);
	}
	double result = 0;
	for (int i = 0; i < m_dimension; i++) result += m_elements[i] * RHSvec.m_elements[i];
	return result;
}
// Operator overload to multiply nVector by scalar
nVector nVector::operator*(const double RHSscalar) const
{
	nVector result(m_dimension);
	for (int i = 0; i < m_dimension; i++) result.m_elements[i] = m_elements[i] * RHSscalar;
	return result;
}
// Operator overload for nVector addition
nVector nVector::operator+(const nVector &RHSvec) const
{
	if (m_dimension != RHSvec.m_dimension)
	{
		cout << "Vector dimensions do not match." << endl;
		exit(1);
	}
	nVector result{ m_dimension };
	for (int i = 0; i < m_dimension; i++) result.m_elements[i] = m_elements[i] + RHSvec.m_elements[i];
	return result;
}
// Operator overload for nVector subtraction
nVector nVector::operator-(const nVector &RHSvec) const
{
	if (m_dimension != RHSvec.m_dimension)
	{
		cout << "Vector dimensions do not match." << endl;
		exit(1);
	}
	nVector result{ m_dimension };
	for (int i = 0; i < m_dimension; i++) result.m_elements[i] = m_elements[i] - RHSvec.m_elements[i];
	return result;
}
// Copy assignment operator for deep copying
nVector & nVector::operator=(const nVector &vec)
{
	if (&vec == this) return *this; // no self assignment
	// First delete this object's array
	delete[] m_elements; m_elements = 0; m_dimension = 0;
	// Now copy size and declare new array
	m_dimension = vec.m_dimension;
	if (m_dimension>0)
	{
		m_elements = new double[m_dimension];
		// Copy values into new array
		for (int i = 0; i<m_dimension; i++) m_elements[i] = vec.m_elements[i];
	}
	return *this;
}
// Move assignment operator
nVector & nVector::operator=(nVector&& vec)
{
	swap(m_dimension, vec.m_dimension);
	swap(m_elements, vec.m_elements);
	return *this;
}

// Class to contain the fourVector
// Inherits publically from nVector
// Overrides dot product function, and overloads hard-coding dimension to be 4
// Contains function to calculate Lorentz boost with beta as parameter
class fourVector : public nVector
{
	// Stream overloads
	friend ostream & operator<<(ostream &os, const fourVector &fourvec);
	friend istream & operator>>(istream &is, fourVector &fourvec);

private:
protected:
public:
	// zero vector
	fourVector() : nVector(4) {}
	// fourVector from components
	fourVector(const double ct, const double x, const double y, const double z)
		: nVector(4) 
	{
		m_elements[0] = ct;
		m_elements[1] = x;
		m_elements[2] = y;
		m_elements[3] = z;
	}
	// fourVector from time component and spatial 3-vector
	fourVector(const double ct, const nVector r)
		: nVector(4)
	{
		// assert r is 3 dimensional
		assert(r.dimension()==3);
		m_elements[0] = ct; 
		m_elements[1] = r[0];
		m_elements[2] = r[1];
		m_elements[3] = r[2];
	}
	// Move constructor & copy constructor chained
	fourVector(fourVector &&fourvec) : nVector(move(fourvec)){}
	fourVector(const fourVector &fourvec) : nVector(fourvec){}
	~fourVector(){}

	// Operator overloads 
	double & operator[](const int i) const;
	double operator*(const fourVector &RHSfourvec) const;		// operator to return the result of fourVector dot product
	fourVector operator*(const double RHSscalar) const;		// operator to return the result of fourVector scalar multiplication
	fourVector operator+(const fourVector &RHSfourvec) const;	// operator to return the result of fourVector addition
	fourVector operator-(const fourVector &RHSfourvec) const;	// operator to return the result of fourVector subtraction
	fourVector & operator=(const fourVector &fourvec);		// operator for copy assignment
	fourVector & operator=(fourVector&& fourvec);		// operator for move assignment

	// other functions
	fourVector lorentzBoost(const nVector beta) const;
};

// Member functions defined outside class
// Operator overloads

// Operator overload of insertion operator
ostream & operator<<(ostream &os, const fourVector &fourvec)
{
	if (fourvec.m_dimension == 0){ os << "()"; return os; }
	os << "(";
	for (int i = 0; i < fourvec.m_dimension-1; i++) os << fourvec.m_elements[i] << ", ";
	os << fourvec.m_elements[fourvec.m_dimension - 1] << ")";
	return os;
}
// Operator overload of extraction operator
istream & operator>>(istream &is, fourVector &fourvec)
{
	for (int i = 0; i << fourvec.m_dimension; i++) is >> fourvec.m_elements[i];
	return is;
}
// Operator overload to access vector components
double & fourVector::operator[](const int i) const 
{
	if (i >= 4 || i < 0){ cout << "Index out of range" << endl; exit(1); }
	return m_elements[i];
}
// Operator overload to do four-vector dot product
double fourVector::operator*(const fourVector &RHSfourvec) const
{
	double result = 0;
	result += m_elements[0] * RHSfourvec.m_elements[0];
	for (int i = 1; i < 4; i++) result -= m_elements[i] * RHSfourvec.m_elements[i];
	return result;
}
// Operator overload to multiply vector by scalar
fourVector fourVector::operator*(const double RHSscalar) const
{
	fourVector result;
	for (int i = 0; i < 4; i++) result.m_elements[i] = m_elements[i] * RHSscalar;
	return result;
}
// Operator overload for vector addition
fourVector fourVector::operator+(const fourVector &RHSvec) const
{
	fourVector result;
	for (int i = 0; i < 4; i++) result.m_elements[i] = m_elements[i] + RHSvec.m_elements[i];
	return result;
}
// Operator overload for vector subtraction
fourVector fourVector::operator-(const fourVector &RHSvec) const
{
	fourVector result;
	for (int i = 0; i < 4; i++) result.m_elements[i] = m_elements[i] - RHSvec.m_elements[i];
	return result;
}
// Copy assignment operator for deep copying
fourVector & fourVector::operator=(const fourVector &fourvec)
{
	nVector::operator=(fourvec);
	return *this;
}
// Move assignment operator
fourVector & fourVector::operator=(fourVector&& fourvec)
{
	nVector::operator=(move(fourvec));
	return *this;
}
// Function to return Lorentz boosted fourVector
fourVector fourVector::lorentzBoost(const nVector beta) const
{
	assert(beta*beta != 0.);
	// split fourVector to time and spatial components, calculate gamma
	double m_ct = m_elements[0];
	nVector m_r(3, m_elements + 1);
	double gamma{1. / sqrt(1-beta*beta)};
	// construct boosted components
	double result_ct{gamma*(m_ct-beta*m_r)};
	nVector result_r{ m_r + beta*((gamma - 1)*((m_r*beta) / (beta*beta)) - gamma*m_ct) };
	return fourVector(result_ct, result_r);
}

// Class to contain the particle
// Contains basic information of a particle in Minkowski space
// Other useful functions to return derived quantities which can be displayed using info()
class particle
{
private:
	fourVector m_position;//MeV^-1
	double m_mass;//MeV/c^2
	nVector m_beta;

protected:
public:
	// stationary massless particle at origin
	particle(){ m_position = fourVector(); m_mass = 0; m_beta = nVector(3); }
	// massive particle with a velocity and position
	particle(const fourVector position, const double mass, const nVector beta)
	{
		// assert beta is 3 dimensional and physical
		assert(beta.dimension()==3);
		assert(beta*beta<1.);
		m_position = position; m_mass = mass; m_beta = beta;
	}
	~particle(){}
	double gamma() const { return 1. / sqrt(1 - m_beta*m_beta); }
	double energy() const { return m_mass * gamma(); } // MeV
	double momentum() const { return gamma()*m_mass*sqrt(m_beta*m_beta); }
	fourVector fourMomentum() const { return fourVector(energy(),m_beta*gamma()*m_mass); } // MeV/c 
	fourVector fourPosition() const { return m_position; }// hbar*c/MeV
	double mass() const { return m_mass; }
	void info()
	{
		cout << "Gamma: " << gamma() << endl;
		cout << "Beta: " << m_beta << endl;
		cout << "Mass: " << m_mass << "MeV/c^2" << endl;
		cout << "Energy: " << energy() << "MeV" << endl;
		cout << "Momentum: " << momentum() << "MeV" << endl;
		cout << "Four-position: "<< fourPosition() << "hbar*c/MeV" << endl;
		cout << "Four-momentum: "<< fourMomentum() << "MeV/c" << endl;
		return;
	}
};
int main()
{
	// Demonstrate classes
	cout << "-----------------------------------------------------------------------------------------------" << endl;
	// nVector
	nVector defaultVec; // Default constructor
	cout << "Default Vector: " << defaultVec << endl;
	double data[] = {1,2,3};
	nVector zeroVec(3); // 3D Zero vector
	cout << "Zero Vector: " << zeroVec << endl;
	nVector sourceVec(3,data); // nVector initialised from source array
	cout << "Source Vector: " << sourceVec << endl;
	nVector copyConVec(sourceVec); // Copy constructor
	stringstream("4 5 6") >> sourceVec;
	cout << "Source Vector: " << sourceVec << endl;
	cout << "Copy constructed Vector: " << copyConVec << endl;
	nVector moveConVec(move(sourceVec)); // Move constructor
	cout << "Source Vector: " << sourceVec << endl;
	cout << "Move constructed Vector: " << moveConVec << endl;
	nVector copyAssigVec; // Copy assignment
	copyAssigVec = copyConVec;
	stringstream("7 8 9") >> copyConVec;
	cout << "Copy constructed Vector: " << copyConVec << endl;
	cout << "Copy assigned Vector: " << copyAssigVec << endl;
	nVector moveAssigVec; // Move assignment
	moveAssigVec = move(copyAssigVec);
	cout << "Copy assigned Vector: " << copyAssigVec << endl;
	cout << "Move assigned Vector: " << moveAssigVec << endl;
	cout << "3rd component of move assigned vector: "<< moveAssigVec[2] << endl; // Accessor - in range
	//cout << "4th component of move assigned vector: " << moveAssigVec[3] << endl; // Accessor - out of range
	cout << "Copy constructed vector dot Move assigned vector: " << copyConVec*moveAssigVec << endl;// dot product
	cout << "-----------------------------------------------------------------------------------------------" << endl;
	// fourVector
	fourVector defaultFourVec; // Default constructor
	cout << "Default Four-Vector: " << defaultFourVec << endl;
	fourVector fourVec1(1.,2.,3.,4.); // four doubles constructor
	cout << "Four-Vector 1: " << fourVec1 << endl;
	fourVector fourVec2(500.,moveAssigVec); // double and 3-vector constructor
	cout << "Four-Vector 2: " << fourVec2 << endl;
	cout << "Four-Vector 1 dot Four-Vector 2: " << fourVec1*fourVec2 << endl; // dot product
	fourVector copyConFourVec(fourVec1); // Copy constructor
	stringstream("5 6 7 8") >> fourVec1;
	cout << "Four-Vector 1: " << fourVec1 << endl;
	cout << "Copy constructed Four-Vector: " << copyConFourVec << endl;
	fourVector moveConFourVec(move(fourVec2)); // Move  constructor
	cout << "Four-Vector 2: " << fourVec2 << endl;
	cout << "Move constructed Four-Vector: " << moveConFourVec << endl;
	fourVector copyAssigFourVec; // Copy assignment
	copyAssigFourVec = fourVec1;
	stringstream("9 10 11 12") >> fourVec1;
	cout << "Four-Vector 1: " << fourVec1 << endl;
	cout << "Copy assigned Four-Vector: " << copyAssigFourVec << endl;
	fourVector moveAssigFourVec; // Move assignment
	moveAssigFourVec = move(fourVec1);
	cout << "Four-Vector 1: " << fourVec1 << endl;
	cout << "Move assigned Four-Vector: " << moveAssigFourVec << endl;
	cout << "4th component of move assigned fourvector: " << moveAssigFourVec[3] << endl; // Accessor - in range
	//cout << "5th component of move assigned fourvector: " << moveAssigFourVec[4] << endl; // Accessor - out of range
	nVector beta(3);
	stringstream("0.9 0 0") >> beta;
	cout << "Lorentz boosted move assigned fourvector: " << moveAssigFourVec.lorentzBoost(beta) << endl;// Lorentz boost

	cout << "-----------------------------------------------------------------------------------------------" << endl;
	// particle
	particle defaultParticle;
	cout << "Default particle info:" << endl;
	defaultParticle.info();
	fourVector p1pos(4,1,2,1);
	double p1mass(0.511);
	nVector p1beta(3);
	stringstream("0.9 0 0") >> p1beta;
	particle p1{p1pos,p1mass,p1beta};
	cout << endl << "Parametrised particle info:" << endl;
	p1.info();
	cout << "-----------------------------------------------------------------------------------------------" << endl;
	return 0;
}
