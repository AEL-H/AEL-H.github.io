// Amir El-hamdy
// 30 March 2017

// C++ program to create 2D and 3D shapes
// and to create prisms from 2D shapes

#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
const double PI = M_PI;
const double P = 1.6075; // Constant used for ellipsoids

// Abstract interface class for all shapes
// We can use this class to find out basic information about the derived objects
class shape
{
private:
protected:
public:
	virtual ~shape(){}; // Virtual destructor to ensure objects are destroyed properly
	// Accessor functions
	virtual string name() const = 0;
	virtual void info(ostream &os = cout) const = 0;
};

// Abstract derived class for 2D shapes
// Contains virtual accessor functions
// to gather information and output it
class shape2D : public shape {
private:
protected:
public:
	virtual ~shape2D(){}; // Virtual destructor to ensure objects are destroyed properly
	// Accessors
	virtual double area() const = 0;
	virtual double perimeter() const = 0;
	virtual const int sides() const = 0;
	virtual const int corners() const = 0;
	virtual void info(ostream &os = cout) const
	{
		os << "A " << name() << " has " << sides() << " sides and " << corners() << " corners." << endl;
		os << "This " << name() << " has an area of " << area() << " and a perimeter of " << perimeter() << "." << endl;
		os << endl;
	}
};
// Derived class for rectangles
class rectangle : public shape2D
{
private:
	// Side lengths
	double m_a;
	double m_b;
protected:
public:
	rectangle() = delete; // No default constructor
	rectangle(const double a, const double b) : m_a(a), m_b(b) {}
	virtual ~rectangle(){} // Virtual destructor because square inherits from rectangle
	// Overrides
	virtual string name() const { return "rectangle"; }
	virtual double area() const { return m_a*m_b; }
	virtual double perimeter() const { return 2 * (m_a + m_b); }
	virtual const int sides() const { return 4; }
	virtual const int corners() const { return 4; }
};
// Derived class for squares, inherits from rectangle
class square : public rectangle
{
private:
protected:
public:
	square() = delete; // No default constructor
	square(const double a) : rectangle(a, a) {}
	~square(){}
	virtual string name() const { return "square"; } // Override rectangle name
};
// Derived class for ellipses
class ellipse : public shape2D
{
private:
	// Semi-major/minor axes
	double m_a;
	double m_b;
protected:
public:
	ellipse() = delete; // No default constructor
	ellipse(const double a, const double b) : m_a(a), m_b(b) {}
	virtual ~ellipse(){} // Virtual destructor because circle inherits from ellipse
	virtual string name() const { return "ellipse"; }
	virtual double area() const { return PI * m_a * m_b; }
	virtual double perimeter() const { return 2 * PI * sqrt(0.5 * (pow(m_a, 2) + pow(m_b, 2))); } // Approximate, simplifies to circular case for a=b
	virtual const int sides() const { return 1; }
	virtual const int corners() const { return 0; }
};
// Derived class for circles, inherits from ellipse
class circle : public ellipse
{
private:
protected:
public:
	circle() = delete; // No default constructor
	circle(const double r) : ellipse(r,r) {}
	~circle(){}
	virtual string name() const { return "circle"; } // Override ellipse name
};
// Derived class for 3D shapes
// Contains virtual accessor functions
// to gather information and output it
class shape3D : public shape
{
private:
protected:
public:
	virtual ~shape3D(){}; // Virtual destructor to ensure objects are destroyed properly
	// Accessors
	virtual double volume() const  = 0;
	virtual double surfaceArea() const  = 0;
	virtual double perimeter() const  = 0;
	virtual const int faces() const  = 0;
	virtual const int edges() const  = 0;
	virtual const int vertices() const = 0;
	virtual void info(ostream &os = cout) const
	{
		os << "A " << name() << " has " << faces() << " faces, " << edges() << " edges and " << vertices() << " vertices." << endl;
		os << "This " << name() << " has a volume of " << volume() << ", a surface area of " << surfaceArea() << " and the sum of its edges is " << perimeter() << "." << endl;
		os << endl;
	}
};
// Derived class for cuboids
class cuboid : public shape3D
{
private:
	double m_a;
	double m_b;
	double m_c;
protected:
public:
	cuboid() = delete; // No default constructor
	cuboid(const double a, const double b, const double c) : m_a(a), m_b(b), m_c(c) {}
	virtual ~cuboid(){} // Virtual destructor because cube inherits from cuboid
	// Overrides
	virtual string name() const { return "cuboid"; }
	virtual double volume() const { return m_a * m_b * m_c; }
	virtual double surfaceArea() const { return 2 * (m_a * m_b + m_a * m_c + m_b * m_c); }
	virtual double perimeter() const { return 4 * (m_a + m_b + m_c); }
	virtual const int faces() const { return 6; }
	virtual const int edges() const { return  12; }
	virtual const int vertices() const { return 8; }
};
// Derived class for cubes, inherits from cuboid
class cube : public cuboid
{
private:
protected:
public:
	cube() = delete; // No default constructor
	cube(const double a) : cuboid(a, a, a) {}
	~cube(){}
	virtual string name() const { return "cube"; } // Override cuboid name
};
// Derived class for ellipsoids
class ellipsoid : public shape3D
{
private:
	// Semi-major/minor axes
	double m_a;
	double m_b;
	double m_c;
protected:
public:
	ellipsoid() = delete; // No default constructor
	ellipsoid(const double a, const double b, const double c) : m_a(a), m_b(b), m_c(c) {}
	virtual ~ellipsoid(){} // Virtual destructor because sphere inherits from ellipsoid
	// Overrides
	virtual string name() const { return "ellipsoid"; }
	virtual double volume() const { return (4. / 3.) * PI * m_a * m_b * m_c; }
	virtual double surfaceArea() const { return 4. * PI * pow((1. / 3.)*(pow(m_a*m_b, P) + pow(m_a*m_c, P) + pow(m_b*m_c, P)), 1. / P); } // Approximate, good accuracy for spheres/ellipsoids
	virtual double perimeter() const { return 0; }
	virtual const int faces() const { return 1; }
	virtual const int edges() const { return  0; }
	virtual const int vertices() const { return 0; }
};
// Derived class for spheres, inherits from ellipsoid
class sphere : public ellipsoid
{
private:
protected:
public:
	sphere() = delete; // No default constructor
	sphere(const double a) : ellipsoid(a, a, a) {}
	~sphere(){}
	virtual string name() const { return "sphere"; } // Override ellipsoid name
};

// Derived class for a prism
class prism : public shape3D
{
private:
	shape2D *m_surface;
	double m_depth;
protected:
public:
	prism() = delete; // No default constructor
	prism(shape2D *surface, double depth) : m_surface(surface), m_depth(depth) {}
	~prism(){}
	// Overrides
	virtual string name() const { return m_surface->name() + "-prism"; }
	virtual double volume() const { return m_surface->area()*m_depth; }
	virtual double surfaceArea() const { return m_surface->perimeter()*m_depth + 2 * m_surface->area(); }
	virtual double perimeter() const { return 2 * m_surface->perimeter() + m_depth*m_surface->corners(); }
	virtual const int faces() const { return 2 + m_surface->sides(); }
	virtual const int edges() const { return  2 * m_surface->sides() + m_surface->corners(); }
	virtual const int vertices() const { return 2 * m_surface->corners(); }

};
int main()
{
	cout << endl << endl;
	// 8 base class pointers, each to a different shape
	vector<shape*> shapes;
	shapes.push_back(new rectangle(3.,4.));
	shapes.push_back(new square(4.));
	shapes.push_back(new ellipse(3.,4.));
	shapes.push_back(new circle(4.));
	shapes.push_back(new cuboid(3., 4., 5.));
	shapes.push_back(new cube(4.));
	shapes.push_back(new ellipsoid(3., 4., 5.));
	shapes.push_back(new sphere(4.));
	// Output info for each case,
	// clean up by deleting objects
	for (auto it = shapes.begin(); it < shapes.end(); ++it)
	{
		(*it)->info();
		delete *it;
	}
	shapes.clear();
	cout << "shapes vector now has size: " << shapes.size() << endl << endl;

	// Demonstrate prism
	// Make some 2D shapes
	shape2D *shape1 = new rectangle(3,4);
	shape2D *shape2 = new square(4);
	shape2D *shape3 = new ellipse(3,4);
	shape2D *shape4 = new circle(4);
	// Get information about their prisms
	prism(shape1, 10.).info();
	prism(shape2, 10.).info();
	prism(shape3, 10.).info();
	prism(shape4, 10.).info();
	// Clean up
	delete shape1;
	delete shape2;
	delete shape3;
	delete shape4;
}