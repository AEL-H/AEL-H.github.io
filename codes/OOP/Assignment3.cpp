
// Assignment 3
// Amir El-hamdy 23 Feb 2017
// Program to store, sort and print physics courses

#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<regex>
using namespace std;

// Function declarations
bool enterCourse();
void printCourses(vector<string> &courses);
void printYear(vector<string> &courses, int year);
void sortTitle(vector<string> &courses);
void sortCode(vector<string> &courses);
int getDigit();
bool YesOrNo();

const string degreeCode("PHYS");

// vector containing all of the course information in strings
vector<string> Data;

int main()
{
	bool not_finished{ true };

	// run until we are finished entering codes
	do
	{
		cout << "Please enter a course name (course code and title on a single title) or enter 'x' to finish:" << endl;
		not_finished = enterCourse();
	} while (not_finished);

	// Finished collecting courses

	// Print courses
	cout << "List of courses:" << endl;
	printCourses(Data);
	cout << endl;

	cout << "Would you like to filter the courses by year? y/n" << endl;
	if (YesOrNo())
	{
	// Print only courses with code yearcode
	cout << "Please enter what year courses you would like to look at." << endl;
	int yearChoice{ getDigit() };
	printYear(Data, yearChoice);
	cout << endl;
	}
	cout << "Would you like to sort the courses by the course title? y/n" << endl;
	if (YesOrNo())
	{
		// Sort by course title and print
		sortTitle(Data);
		cout << endl;
	}
	cout << "Would you like to sort the courses by the course code? y/n" << endl;
	if (YesOrNo())
	{
	// Sort by course code and print
	sortCode(Data);
	cout << endl;
	}
	system("pause");
	return 0;
}
bool enterCourse()
{
	// variables to temporarily hold values
	int courseCode;
	string courseTitle;
	string line;

	// get new line and analyse the line with stringstream
	getline(cin, line);
	istringstream split(line);

	// check to see if the user is done and if not remove blank entries
	if (split.peek() == 'x'){ return false; }
	if (line == ""){ return true; }

	// Try to parse the input
	split >> courseCode; // get the course code

	// Make sure course code is valid
	if (cin.fail() || courseCode < 0 || !(to_string(courseCode).length()==5))
	{
		cout << "Make sure the course code is a positive integer 5 digits long." << endl;
		// Clear fail
		cin.clear();
		return true;
	}
	getline(split,courseTitle); // get the title
	courseTitle = regex_replace(courseTitle, regex("^ +| +$|( ) +"), "$1"); // remove leading and trailing whitespace

	ostringstream ofss;
	ofss << degreeCode << courseCode<< " " << courseTitle;
	Data.push_back(ofss.str());
	ofss.str("");
	return true;
}
// Print out the strings stored within the vector of strings
void printCourses(vector<string> &courses)
{
	for (auto iterator = courses.begin(); iterator != courses.end(); iterator++){
		cout << *iterator << endl;
	};
	return;
}
// Print out all courses with year as their year code
void printYear(vector<string> &courses, int year)
{
	// create a new vector which will contain only courses from the year we are interested in
	vector<string> result(courses.size());

	// Iterate over courses, if the leading digit of the course code == year, add it in result
	auto it = copy_if(courses.begin(), courses.end(), result.begin(), [&](string course){ return ((course[4]-'0') == year); });

	// shrink container to new size
	result.resize(distance(result.begin(), it));

	if (result.empty())
	{
		cout << "There are no courses with that year code." << endl;
	}
	// print out all courses with the given year code
	else
	{
		cout << "Printing courses in year "<<year <<"." << endl;
		printCourses(result);
	}

	return;
}
// Sort the passed in vector of strings alphabetically, and print them out
void sortTitle(vector<string> &courses)
{
	cout << "Sorting by course title:" << endl;
	// want 10th char onwards
	sort(courses.begin(), courses.end(), [](string a, string b) { return (a.substr(10)<b.substr(10)); });

	printCourses(courses);

	return;
}
// Sort the passed in vector of strings in lexicographic order, and print them out
void sortCode(vector<string> &courses)
{
	cout << "Sorting by course code." << endl;
	sort(courses.begin(), courses.end(), [](string a, string b) { return (a<b); });

	printCourses(courses);
	
	return;
}
// Get a digit 0-9 and return
int getDigit()
{
	int result;
	cin >> result;

	// make sure input is valid
	while (result > 9 || cin.fail() || result < 1 || cin.peek() != '\n')
	{
		cout << "Sorry that input is not valid, please enter a digit 1-9." << endl;
		// Clear fail and ignore bad input
		cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin >> result;
	}
	return result;
}
// Get a yes or no answer from the user and return true or false respectively
bool YesOrNo()
{
	char input;
	cin >> input;
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