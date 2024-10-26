#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>;
using namespace std;

struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
};

struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course course) {
		this->course = course;
		left = nullptr;
		right = nullptr;
	}
};

class BinaryTree {
private:
	Node* root;
	void inOrder(Node* node);
	int size = 0;

public:
	BinaryTree();
	void Insert(Course course);
	void Search(string courseNumber);
	void InOrderPrint();
};

/*
* This function converts a string to all lowercase. This will help in allowing the user to input courses without the need for strict input rules. Code help from https://www.geeksforgeeks.org/how-to-convert-std-string-to-lower-case-in-cpp/
*/
string ConvertToLowercase(const string& str) {
	string lowerCaseStr = ""; // initializes a string to hold the converted string.

	for (char c : str) { // go through each character in the string and convert into lowercase, adding each lowercase character into the converted string variable.
		lowerCaseStr += tolower(c);
	}

	return lowerCaseStr; // return the converted string variable. 
}

/*
*	constructor
*/
BinaryTree::BinaryTree() {
	this->root = nullptr; // assigns the tree's root to nullptr.
}

/*
 *	insert a course into the tree.
 */
void BinaryTree::Insert(Course course) {
	Node* currNode = root;

	if (root == nullptr) { // if there is no data in the tree, the root will be null.
		root = new Node(course); // so we assign the course to the root and exit the function.
		return;
	}

	while (currNode != nullptr) { // if the root is not null ptr, we traverse through the tree until we find a spot to insert the course. 
		if (currNode->course.courseNumber.compare(course.courseNumber) < 0) { // this checks if the currNode's course number is alphanumerically lower than the course number of the course passed in, if yes, we insert into the right.
			if (currNode->right == nullptr) { // if there is no node stored on the right of the currNode,
				currNode->right = new Node(course); // we simply store the new course in the right child node.
				return; // then exit the function.
			}
			else {
				currNode = currNode->right; // if there is a node to the right of currNode, then we continue to traverse. 
			}
			
		}
		else if (currNode->course.courseNumber.compare(course.courseNumber) > 0) { // this checks if the currNode's course number is alphanumerically higher than the course number of the course passed in; if yes, we insert into the left.
			if (currNode->left == nullptr) { // if there is no left child node, 
				currNode->left = new Node(course); // we simply store the new course in the left child node. 
				return; // then exit the function.
			}
			else {
				currNode = currNode->left; // if there is a node to the left of currNode, then we continue to traverse.
			}
		}
			
		else { // if neither of these conditions are met, it means we are trying to insert a node we already have. Since there are no duplicates...
			return; // we simply exit the function. 
		}
	}
}
/*
* Function searches the tree for the course number passed. 
*/
void BinaryTree::Search(string courseNumber){
	Node* currNode = root;
	string lowerCaseCourseNum = ConvertToLowercase(courseNumber); // converting to lowercase to check against valid user input that was input in all caps or all lowercase. removes a validation barrier and is easier for user.

	while (currNode != nullptr) { // if tree has data, the following statements will execute.
		string lowerNodeCourseNum = ConvertToLowercase(currNode->course.courseNumber); // the currNode's course number is converted to lowercase to check against the courseNumber passed.
		if (lowerNodeCourseNum == lowerCaseCourseNum) { // if the strings are equal, print out the course name, number, and prerequisites.
			cout << currNode->course.courseNumber << ": " << currNode->course.courseName << endl;
			cout << "Prerequisites: " << endl;
			for (int i = 0; i < currNode->course.prerequisites.size(); i++) { // have to loop for the prerequisites because it is a vector. 
				cout << currNode->course.prerequisites[i] << endl;
			}
			cout << endl;
			return; // exit the function.
		}
		else if (lowerNodeCourseNum < lowerCaseCourseNum) { // if the strings are not equal, and the node's course number is less than the course number passed, check the right child node. 
			currNode = currNode->right;
		}
		else if (lowerNodeCourseNum > lowerCaseCourseNum) { // if the strings are not equal, and the node's course number is greater than the course number passed, check the left child node.
			currNode = currNode->left;
		}
	}
	
	cout << courseNumber << " not found." << endl; // if the course number is not found in the tree, the error statement is printed. 
}


/*
* Prints the tree in alphanumeric order staring from root using inOrder traversal.
*/
void BinaryTree::InOrderPrint() {
	inOrder(root);
}
/*
* Traverses the tree in order and prints out each course and its prerequisites. 
*/
void BinaryTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left); //traverses the left-most node first.

		// this is the print statement for the node. NOTE: I didn't put any prerequisites because there were none listed in the sample output.
		cout << "     " << node->course.courseNumber << ": " << node->course.courseName << endl << endl; 
		inOrder(node->right); //then traverse the right-most node.
	}
}
/*
* Reads a csv file and inserts into a binary tree. code help sourced from https://cppbyexample.com/parse_csv.html 
*/
void ReadCSV(string filename, BinaryTree* courseTree) {
	ifstream fileIn(filename); 
	vector<vector<string>> csvRows; // a super vector that contains other vectors of strings allowing for easier parsing. 

	if (!fileIn) { // if file has an issue being opened, it will print the error and nothing will be stored. 
		cout << "File could not be opened!" << endl;
		return;
	}

	for (string line; getline(fileIn, line);) { // this is reading each line of the file.
		istringstream ss(line); // the string stream allows us to manipulate the string and find each element separated by ',' later on.
		vector<string> row; // creating a vector to store each collection of words/inputs from the csv file.

		for (string value; getline(ss, value, ',');) { // here, i am splitting the string element into parts separated by ','. 
			row.push_back(value); //then I assign each value into a row vector.
		}

		csvRows.push_back(row); // this row vector for a single line is then added into the super vector, and the process loops for the next row in the file.
	}

	for (const vector<string>& row : csvRows) { // this iterates through the super vector, csvRows, and allows me to store the csv file data into the binary tree efficiently. 
		if (row.size() < 2) { // the data input cannot be less than 2, as each row needs to have a course name and number.
			continue;
		}

		Course course; // creating a new course for row elements to be assigned to respective variables and vectors.
		// elements 0 and 1 will always be the course number and name respectively. 
		course.courseNumber = row[0]; 
		course.courseName = row[1];

		if (row.size() > 2) { // this only executes if the row's size is greater than 2, meaning there is atleast one prerequisite. 
			for (int i = 2; i < row.size(); i++) {
				//if (!row[i].empty()) { // I used this for debugging issues but realized it's redundant considering the if statement before the loop. 
				course.prerequisites.push_back(row[i]);
			}
		}
		courseTree->Insert(course); // after the data is added into the course structure, it is then inserted into our binary tree.
	}
	cout << "file loaded successfully!" << endl; 
}

/*
* Function validates user input's formatting. Code help from https://www.geeksforgeeks.org/how-to-validate-user-input-in-cpp/ & https://www.geeksforgeeks.org/write-regular-expressions/ & https://cplusplus.com/reference/regex/regex_match/
*/
bool isValidCourseCode(const string& input) { 
	regex pattern("[A-Za-z]+[0-9]+"); //makes sure user inputs in the format [alpha][num] instead of [alpha] [num]. lowercase is allowed as it is all being checked against lowercase in the search operation.
	return regex_match(input, pattern); // returns true if the input matches the pattern, and false if it does not.
}

int main() {
	int selection = 0;
	BinaryTree* course = new BinaryTree();
	string fileName;
	string courseNumber;
	
	while (selection != 9){ 
		cout << "COURSE SEARCH MENU:" << endl;
		cout << "     1. Load CSV file." << endl;
		cout << "     2. Print list of all courses." << endl;
		cout << "     3. Print individual course and its prerequisites." << endl;
		cout << "     9. Exit program." << endl;
		cout << "ENTER A SELECTION: " << endl; 
		cin >> selection;
		cout << endl;

		switch (selection) {
		case 1: // if 1 is entered, this case runs. 
			cout << "Enter the name of the CSV file: " << endl;
			cin.ignore(); // ignores the endl space. 
			getline(cin, fileName); // reads the entire line input. if only cin was used, on the first input before a whitespace would be inserted. 
			ReadCSV(fileName, course); // reads and loads the file into the binaryTree data structure.
			cout << endl; // formatting is king.
			break; // breaks out of the case and repeats the menu options.

		case 2: // if 2 is entered, this case runs.
			cout << "FULL COURSE LIST:" << endl; // formatting is king.
			course->InOrderPrint(); // prints out the tree in ascending order using inOrder traversal. 
			break; //breaks out of the case and repeats the menu options.

		case 3: // if 3 is entered, this case runs.
			cout << "Enter the course ID you want to search without spaces (ex. CSCI300): " << endl; // prompting user for input.
			cin.ignore(); // ignores the endl.
			while (true) { // this while statement allows me to run a check in the input without exiting out of the case it's already in. so instead of looping back to the menu, it prompts the user for proper formatting.
				getline(cin, courseNumber); // reads the entire input, so if a user enters input with spacing (which is invalid), it will be able to be handled.
				cout << endl; // formatting is king.
				if (isValidCourseCode(courseNumber) == true) { // makes sure the input is valid input, meaning no spaces between the letters and numbers in the course ID.
					course->Search(courseNumber); // searches the tree for the course number input given by user. if not found, a print statement is given saying the course cannot be found.
					cout << endl; // formatting is king.
					break; // breaks out of the case and repeats the menu options.
				}
				else {
					cout << "Invalid format, please enter the course ID without spaces." << endl; // if the input is not valid, this will be repeated until input is taken.
				}
			}
			break; // breaks out of the case and repeats the menu options.

		case 9: // if 9 is entered, this case runs.
			cout << "Goodbye!" << endl; // gives user feedback that the program is in fact being exited.
			return 0; // exits the program.

		default: // if any number other than 1, 2, 3, or 9 is entered, this case runs.
			cout << "Invalid Number! Please choose from the options on the menu." << endl << endl; // if numbers other than those defined in the cases is entered, this will print, and the prompt will start again. 
		} 
	} 
	cout << "Goodbye!" << endl; // in case 9 is selected at the very start, meaning the while loop will not execute, this is printed out to signify that they have exited the program.
	return 0; // exits the program.
}