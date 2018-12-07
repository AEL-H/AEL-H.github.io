
// Assignment 1

// Program to calculate transition energy using simple Bohr formula

#include<iostream>
#include<iomanip>

using namespace std;

// function declarations
bool YesOrNo();
bool eVOrJ();
int getPosInt();
double deltaE(int Z, int i, int j, bool electronvolts);

const double eV_to_J{1.602e-19}; // define conversion factor between eV and J

int main()
{
	bool not_finished{ true }; // true whilst we are not finished doing calculations

	// Do while loop for the y/n continue structure (automatically handles scope)
	do
	{
		// Ask user to enter atomic number, initial quantum number and final quantum number
		cout << "What atomic number are we dealing with?" << endl;
		int Z{ getPosInt() };

		cout << "What is the initial principal quantum number?" << endl;
		int n_i{ getPosInt() };

		cout << "What is the final principal quantum number?" << endl;
		int n_j{ getPosInt() };

		// Get rid of trivial cases
		if (n_i == n_j) // If the two numbers are the same
		{
			cout << "No transition, initial and final quantum numbers are the same." << endl;
		}
		else if (n_i < n_j) // If the initial number < final number
		{
			cout << "No photon emission, initial quantum number < final quantum number." << endl;
		}

		else // Nothing wrong, do calculation
		{
		// Ask user what units they would like to use
		cout << "Would you like your answer in electronvolts 'e' or Joules 'J'?" << endl;
		bool units{ eVOrJ() };

		// compute and output answer to 3 sf
		double Energy{ deltaE(Z, n_i, n_j, units) };
		cout << setprecision(3) << Energy;
		if (units){ cout << "eV" << endl; }
		else{ cout << "J" << endl; }
		}

		// Ask if the user would like to do another calculation
		cout << "Would you like to do another calculation? y/n" << endl;
		not_finished = YesOrNo() ;

	} 
	while (not_finished);

	return 0; // return with no issues
}

// function to handle yes or no question
bool YesOrNo()
{
	char input;
	cin >> input; // read in a character
	// make sure input is valid
	while (cin.fail() || (input != 'y' && input != 'n') || cin.peek() != '\n')
	{
		cout << "Please enter 'y' for yes, or 'n' for no." << endl;
		// Clear fail and ignore bad input
		cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin >> input;
	}
	// from here input is either 'y' or 'n'
	if (input == 'y'){ return true; }
	else { return false; }
}

// function to handle e or J question
bool eVOrJ()
{
	char input;
	cin >> input; // read in a character
	// make sure input is valid
	while (cin.fail() || (input != 'e' && input != 'J') || cin.peek() != '\n')
	{
		cout << "Please enter 'e' for electron volts, or 'J' for Joules." << endl;
		// Clear fail and ignore bad input
		cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin >> input;
	}
	// from here input is either 'e' or 'J'
	if (input == 'e'){ return true; }
	else { return false; }
}

// Function to get a positive integer (since atomic number and principal quantum numbers must be positive)
int getPosInt()
{
	int result;
	cin >> result; // read in an integer
	// make sure input is valid
	while (cin.fail() || result < 1||cin.peek()!='\n')
	{
		cout << "Sorry that input is not valid, please enter a positive integer." << endl;
		// Clear fail and ignore bad input
		cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin >> result;
	}
	return result;
}

// Function to handle calculation and return double
double deltaE(int Z, int i, int j, bool electronvolts) // Accepts atomic number, initial and final principal quantum numbers respectively as arguments along with a flag for units.
{
	// This function assumes validated inputs
	double Energy{13.6*pow(Z,2)*((1. / (pow(j,2))) - (1. / (pow(i,2)))) };
	if (electronvolts){return Energy;} // If the answer needs to be in electronvolts, simply return.
	else{ return Energy*eV_to_J; } // If the answer needs to be in Joules, apply conversion factor before returning.
}
