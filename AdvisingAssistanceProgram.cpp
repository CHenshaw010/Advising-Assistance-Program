//============================================================================
// Name        : AdvisingAssistanceProgram.cpp
// Author      : Christian Henshaw
// Version     : 1.0
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseID;
    string courseName;
    vector<string> prereqs;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void printCourses(Node* node);

public:
    BinarySearchTree();
    void PrintCourses();
    void Insert(Course course);
    Course Search(string courseID);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::PrintCourses() {
    // call inOrder fuction and pass root 
    printCourses(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr then the tree is empty
    if (root == nullptr) {
        // assign root as the new course
        root = new Node(course);
    }
    else {
        // call method addNode and pass root and course
        this->addNode(root, course);
    }
}

/**
 * Add a new course node. Recursive if necessary.
 *
 * @param node is the current node in the tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if new course < node then add new course to left
    if (node != nullptr && node->course.courseID.compare(course.courseID) > 0) {
        // if no left node is present
        if (node->left == nullptr) {
            // new course now assigned to left node
            node->left = new Node(course);
            return;
        } // else recurse down left node
        else {
            this->addNode(node->left, course);
        }
    } // if new course > node then add new course to right
    else if (node != nullptr && node->course.courseID.compare(course.courseID) < 0) {
        // if no right node is present
        if (node->right == nullptr) {
            // new course now assigned to right node
            node->right = new Node(course);
            return;
        } // else recurse down right node
        else {
            this->addNode(node->right, course);
        }
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseID) {
    // set current node equal to root
    Node* cur = root;

    // loop along tree until the end is reached or matching courseID is found
    while (cur != nullptr) {
        // match found, return current bid
        if (cur->course.courseID.compare(courseID) == 0) {
            return cur->course;
        }
        // if course < current node then traverse left
        else if (courseID.compare(cur->course.courseID) < 0) {
            cur = cur->left;
        }
        // else course > current node then traverse right
        else {
            cur = cur->right;
        }
    }
    // dummy course incase course is not found
    Course course;
    return course;
}

void BinarySearchTree::printCourses(Node* node) {
    if (node != nullptr) {
        // Recursively call method and pass left child node
        printCourses(node->left);
        //output courseID, courseName
        cout << node->course.courseID << ", " << node->course.courseName << endl;
        // Recursively call method and pass right child node
        printCourses(node->right);
    }
}

// method to open and read the file and calls other methods as necessary to perform tasks on text lines
void readFile(string filename, BinarySearchTree* bst) {
    // variable holds string token after being broken at delimeter
    int temp;
    // variable holds a line of text from the file
    string lineFromFile; 
    // variable holds a word from the line of text
    string wordFromLine;
    // opens file with passed filename
    ifstream file(filename);
    // if the file is open
    if (file.is_open()) {
        cout << "Reading file..." << endl << endl;
        // runs until end of file
        while (getline(file, lineFromFile)) {
            // initialize token to zero
            temp = 0;
            // create new course instance
            Course course;
            // convert lineFromFile to line to be used to get separate pieces of course information
            stringstream line(lineFromFile);
            // runs until end of line
            while (temp <= count(lineFromFile.begin(), lineFromFile.end(), ',')) {
                // breaks apart line at delimeter into strings containing pieces of course information
                getline(line, wordFromLine, ',');
                // if token is 0, content is courseID
                if (temp == 0) {
                    // assign the new course.courseID with the token
                    course.courseID = wordFromLine;
                }
                // if token is 1, content is courseName
                else if (temp == 1) {
                    // assign the new course.courseName with the token
                    course.courseName = wordFromLine;
                }
                // else, the content is prerequisites
                else {
                    // pushback token to prereq vector
                    course.prereqs.push_back(wordFromLine);
                }
                // increment temp
                temp++;
            }
            // after the line is completed, call the Insert method to add new course to tree
            bst->Insert(course);           
        }
    }
    // else, file cannot be opened
    else {
        // print error
        cout << "Error loading file." << endl;
        return;
    }
    // close file once all lines have been read
    file.close();
}

// method to print a specific course
void displayCourse(Course course) {
    // print course information
    cout << course.courseID << ", " << course.courseName << endl;
    cout << "Prerequisites: ";
    // if the course contains prereqs (some do not)
    if (!course.prereqs.empty()) {
        // loop through prereqs printing information
        for (string prereq : course.prereqs) {
            cout << prereq << " ";
        }
    }
    // else, used if no prereqs are present in course
    else {
        cout << "none";
    }
    return;
}

int main() {
    // create new tree
    BinarySearchTree* bst = new BinarySearchTree();
    // initialize new course instance
    Course course;
    // variable used to hold filename from user
    string filename;
    // variable used to hold user choice in menu
    int userChoice = 0;
    // variable used to hold course from user
    string userCourse;

    cout << "Welcome to the course planner." << endl << endl;
    // runs until user selects to exit program
    while (userChoice != 9) {
        // print program menu
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl << endl;
        cout << "What would you like to do? ";
        // get user input
        cin >> userChoice;
        cout << endl;

        // if user selects to load data structure
        if (userChoice == 1) {
            cout << "Please enter the name of the file: " << endl;
            // default file name below. Is the file in the folder, can be used if the cin below is commented out
            // filename = "ABCU_Advising_Program_Input.txt";
            // obtain filename from user
            cin >> filename;
            // call method to read the file and pass the filename and tree
            readFile(filename, bst);
        }
        // if user selects to print all courses
        else if (userChoice == 2) {
            cout << "Here is a sample schedule:" << endl << endl;
            // call method to print all courses in the tree
            bst->PrintCourses();
            cout << endl;
        }
        // if user selects to print specific course
        else if (userChoice == 3) {
            cout << "What course do you want to know about? ";
            // obtain course from user
            cin >> userCourse;
            // transform user input so all input forms (caps, lowercase, camelcase) can be inputted
            std::transform(userCourse.begin(), userCourse.end(), userCourse.begin(), std::toupper);
            // assign course with the returned course from calling Search method in the bst for the passed course
            // either specific course is found and returned or a dummy course is returned
            course = bst->Search(userCourse);
            // if course is not empty, the specified course was found
            if (!course.courseID.empty()) {
                // call method displayCourse to print course information
                displayCourse(course);
                cout << endl << endl;
            }
            // else dummy course was returned
            else {
                cout << "Course not found." << endl << endl;
            }
        }
        // if user selects to exit program
        else if (userChoice == 9) {
            cout << "Thank you for using the course planner!" << endl;
            // break, ending the while loop
            break;
        }
        // else, user selected invalid option
        else {
            cout << userChoice << " is not a valid option." << endl << endl;
        }
    }
    return 0;
}