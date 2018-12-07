
// PHYS 30762 Programming in C++
// Assignment 4

// Simple demonstration of a C++ class

// Hubble types: E[0-7], S0, S[a-c], SB[a-c], Irr
// Redshift z in range [0,10]
// Total mass M_tot in range [1e7,1e12] M_sun
// Stellar mass fraction f_* in range [0,0.05]

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
using namespace std;


// Enumerate possible galaxies according to Hubble classification
enum hubbleType
{
	// Ellipticals
	E0, E1, E2, E3, E4, E5, E6, E7,
	// Spirals
	S0, Sa, Sb, Sc, SBa, SBb, SBc,
	// Irregular
	Irr
};

class Galaxy
{
private:
	string name;
	hubbleType classification;
	double redshift;
	double totalMassdouble;
	double stellarMassFraction;
	vector <Galaxy> Satellites;

public:
	// Default constructor
	Galaxy()
	{
		// Default to irregular galaxy with median values and no satellites
		Galaxy::name = "Default galaxy";
		Galaxy::classification = hubbleType::Irr;
		Galaxy::redshift = 5.0;
		Galaxy::totalMassdouble = 5e9;
		Galaxy::stellarMassFraction = 0.025;
		Galaxy::Satellites = {};
		cout << "Creating galaxy: " << Galaxy::name << "." << endl;
	}
	// Parametrised constructor
	Galaxy(string name, hubbleType classification, double redshift, double totalMassdouble, double stellarMassFraction, vector<Galaxy> Satellites = {})
	{
		Galaxy::name = name;
		Galaxy::classification = classification;
		Galaxy::redshift = redshift;
		Galaxy::totalMassdouble = totalMassdouble;
		Galaxy::stellarMassFraction = stellarMassFraction;
		Galaxy::Satellites = Satellites;
		cout << "Creating galaxy: " << Galaxy::name << "." << endl;
	}
	// Destructor
	~Galaxy()
	{
		cout << "Destroying galaxy: " << Galaxy::name << "." <<endl;
	}
	// Return name of galaxy
	string getName() const { return name; }
	// Return stellar mass (M_* = f_* x M_tot)
	double stellarMass() const { return stellarMassFraction*totalMassdouble; }
	// Change galaxy's Hubble type
	void changeType(hubbleType newType) { Galaxy::classification = newType; return; }
	// Prototype for function to print out an object's data
	void printData() const;
	// Prototype for function to add a satellite
	void addSatellite(Galaxy &satellite);
	// Prototype for function to return hubble type as string
	string getHubbleType() const;
	// Prototype for function to print satellites
	void printSatellites() const;
};
// Print out classification type
string Galaxy::getHubbleType() const
{
	switch (Galaxy::classification)
	{
	case E0: return "E0";
	case E1: return "E1";
	case E2: return "E2";
	case E3: return "E3";
	case E4: return "E4";
	case E5: return "E5";
	case E6: return "E6";
	case E7: return "E7";
	case S0: return "S0";
	case Sa: return "Sa";
	case Sb: return "Sb";
	case Sc: return "Sc";
	case SBa: return "SBa";
	case SBb: return "SBb";
	case SBc: return "SBc";
	case Irr: return "Irr";
	}
	// In theory we should never get here
	return "Unkown classification.";
}
// Print satellites
void Galaxy::printSatellites() const
{
	int i = 1;
		for (auto iterator = Galaxy::Satellites.begin(); iterator != Galaxy::Satellites.end(); iterator++){
			cout << "Satellite " << i << " of galaxy "<< Galaxy::name << ":" << endl;
			(*iterator).printData();
			i++;
		};
	return;
}
// Print out an object's data
void Galaxy::printData() const
{
	cout << "Galaxy name: " << Galaxy::name << endl;
	cout << "Galaxy type: " << Galaxy::getHubbleType() << endl;
	cout << "Redshift: " << Galaxy::redshift << endl;
	cout << "Total mass of galaxy: " << Galaxy::totalMassdouble << " solar masses" << endl;
	cout << "Stellar mass fraction: " << Galaxy::stellarMassFraction << endl;
	cout << "Stellar mass: " << Galaxy::stellarMass() << " solar masses" << endl;
	// print satellites
	if (Galaxy::Satellites.empty()){ cout << Galaxy::name << " has no satellites." << endl; }
	else
	{
		cout << Galaxy::name << " has " << Galaxy::Satellites.size() << " satellites:" << endl << endl;
		Galaxy::printSatellites(); 
	}
	cout << endl;
	return;
}
// Add a satellite to satellites
void Galaxy::addSatellite(Galaxy &satellite)
{
	Galaxy::Satellites.push_back(satellite);
	return;
}
// End of class and associated member functions

// Main program
int main()
{
	// Example using default constructor
	Galaxy g1;
	// Example using parameterised constructor
	Galaxy g2{ "Milky Way" , hubbleType::Sb, 0., 9e11, 0.05 };
	// Create two more galaxies and add them to g2 as satellites
	Galaxy sat1{ "Large Magellanic Cloud", hubbleType::Irr, 3.4, 10e10, 0.02 };
	Galaxy sat2{ "Small Magellanic Cloud", hubbleType::Irr, 0.6, 6.5e9, 0.01 };
	cout << endl;
	// Add satellite galaxies
	g2.addSatellite(sat1);
	g2.addSatellite(sat2);
	cout << endl;
	cout << "Declaring universe" << endl << endl;
	vector<Galaxy> Universe{g1,g2}; 
	cout << endl;
	for (auto galaxy = Universe.begin(); galaxy != Universe.end(); galaxy++)
	{
		(*galaxy).printData();
		// Change Hubble type to E7
		(*galaxy).changeType(hubbleType::E7);
		cout << "Changed hubble type to E7"<<endl;
		cout << "Hubble type of " << (*galaxy).getName() << " is now : " << (*galaxy).getHubbleType() << "." << endl;
		cout << "Stellar mass of " << (*galaxy).getName() << " is " << (*galaxy).stellarMass() << " solar masses." << endl << endl;
	}

	return 0;
}