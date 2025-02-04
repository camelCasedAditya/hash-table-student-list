#include <iostream>
#include "Node.h"
#include "Student.h"
#include <cstring>
#include <fstream>
#include <ctime>
using namespace std;

// Hashtable stuct which contains size and pointer to start of array
struct Hashtable {
  int size;
  Node** arr;
};

// Initalizing functions
void makeHashtable(Hashtable* table, int size);
int hasher(Student* student, int size);
bool insert(Student* student, Hashtable* table);
bool addNode(Student* newStudent, Node*& head);
void print(Hashtable* table);
void printNode(Node* next, Node* head);
bool generate(int num, Hashtable* table, int& studentCount);
Hashtable* rehash(Hashtable* table);
void remove(Hashtable* table);
void deleteNode(int studentID, Node*& head, Node* current, Node* previous);
bool searchID(int id, Hashtable* table);

int main() {
  // Set student count to 0. This variable is used to increment the student id
  int studentCount = 0;
  srand(time(NULL));

  // Create hash table and set all slots to null.
  Hashtable* table = new Hashtable();
  makeHashtable(table, 100);
    
  bool running = true;
  // While loop to get user input
  while(running == 1) {
    char input[100];
    cout << "Enter one of the following commands: \"ADD\", \"DELETE\", \"GENERATE\", \"PRINT\", or \"QUIT\"" << endl;
    cin >> input;
    // If user want to add a student
    if(strcmp(input, "ADD") == 0) {
      Student* student = new Student();
      cout << "Enter the first name" << endl;
      cin>>student->firstName;
      cout << "Enter the last name" << endl;
      cin >> student->lastName;
      cout << "Enter the GPA" << endl;
      cin >> student->GPA;
      
      int id = -1;
      cout << "Enter the student id" << endl;
      cin >> id;
      // makes sure that the student id does not already exist
      bool idExists = searchID(id, table);
      while (idExists == 1) {
	cout << "That id already exists. Enter a new student ID" << endl;
	cin >> id;
	idExists = searchID(id, table);
      }
      student->studentID = id;
      studentCount+=1;
      // Insert the student and if there is a collision, then rehash the table
      bool collision = insert(student, table);
      if (collision == true) {
        table = rehash(table);
      }

    }
    else if (strcmp(input, "PRINT") == 0) {
      // Call print function to print table
      print(table);
    }
    else if (strcmp(input, "DELETE") == 0) {
      // Call function to remove a student from the table
      remove(table);
    }
    else if (strcmp(input, "GENERATE") == 0) {
      int number = 0;
      cout << "Enter the number of student to generate" << endl;
      cin >> number;

      // Generate the students and if there is a collision, then rehash the table
      bool collision = generate(number, table, studentCount);
      if (collision == true) {
	table = rehash(table);
      }
    }
    else if (strcmp(input, "QUIT") == 0) {
      // Change running variable to false to end program
      running = false;
    }
    else {
      cout << "Invalid command" << endl;
    }
  }

  return 0;
}

void makeHashtable(Hashtable* table, int size) {
  // Go through each index in array and set it to a null Node
  table->size = size;
  table->arr = new Node*[size];
  for(int i = 0; i < size+1; i++) {
    table->arr[i] = NULL;
  }
}

// hash function to take a student ID and return the index of the array
int hasher(int studentID, int size) {
  int key = 0;
  key = studentID*7;
  key = key%size;
  return key;
}

// Function to remove node from hash table
void remove(Hashtable* table) {
  int id = -1;
  cout << "Enter the student ID: " << endl;
  cin >> id;
  // get the index of the student in the table and calls the delete node function
  int index = hasher(id, table->size);
  deleteNode(id, table->arr[index], table->arr[index], NULL);
}

// Function to add node in linked list
bool addNode(Student* newStudent, Node*& head) {
  Node* current = head;
  int num = 0;
  // If the head is NULL then add the node as head
  if (current == NULL) {
    head = new Node();
    head->setValue(newStudent);
    return false;
  }
  else {
    // Loop to the end of the linked list and add the new node at the end
    num += 1;
    while (current->getNext() != NULL) {
      current = current->getNext();
      num += 1;
    }
    current->setNext(new Node());
    current->getNext()->setValue(newStudent);
    num += 1;
    // if the number of node in a linked list is greater than 3 we then return true to indicate a collision
    if (num > 3) {
      return true;
    }
    else {
      return false;
    }
  }
}

// Function that inserts a node into the table
bool insert(Student* student, Hashtable* table) {
  // Calls the add node function to add the node in the right index of the table
  bool rehash = addNode(student, table->arr[hasher(student->studentID, table->size)]);
  return rehash;
}

void printNode(Node* next, Node* head) {
  // loops through linked list and prints each student out
  if (next != NULL) {
    cout << next->getValue()->firstName << " " << next->getValue()->lastName << ", " << next->getValue()->studentID << ", " << next->getValue()->GPA << endl;
    printNode(next->getNext(), head);
  }
}

// Loops through each index in array and calls print node to print out all the students in each of the linked lists
void print(Hashtable* table) {
  int num = 0;
  while (num < table->size) {
    printNode(table->arr[num], table->arr[num]);
    num +=1;
  }
}

// Rehash function if there is more than 3 collisions
Hashtable* rehash(Hashtable* table) {
  // Make a new larger table
  Hashtable* newTable = new Hashtable();
  makeHashtable(newTable, ((table->size)*2)+7);

  int num = 0;
  // Copy all of the nodes to the new table with new hashing
  while (num < table->size) {
    Node* node = table->arr[num];
    while (node != NULL) {
      insert(node->getValue(), newTable);
      node = node->getNext();
    }
    num += 1;
  }
  // output the new table
  return newTable;
}

// Function to generate student and add them to the table;
bool generate(int num, Hashtable* table, int& studentCount) {
  // Variables to hold the ammount of names in both files
  int FIRSTNAMECOUNTS = 164;
  int LASTNAMECOUNTS = 295;
  bool temp = false;
  
  // Loop that generates the number of students the user wants
  for (int i = 0; i < num; i++) {

    // Open file with first names
    ifstream file("firstnames.txt");

    // Generate random line to stop at
    int line = rand()%(FIRSTNAMECOUNTS+1);
    char firstname[200];
    int loop = 0;

    // Loop through file and get name at the random generated line
    while (file.getline(firstname, sizeof(firstname))) {
      if (loop == line) {
        cout << firstname << endl;
        break;
      }
      loop += 1;
    }

    // Open file with last names
    ifstream file2("lastnames.txt");

    // Randomly pick link to get last name from
    line = rand()%(LASTNAMECOUNTS+1);
    char lastname[200];
    loop = 0;

    // Loops through the last names and gets the name from the random line
    while (file2.getline(lastname, sizeof(lastname))) {
      if (loop == line) {
        break;
      }
      loop += 1;
    }

    // Creates a new student with the names
    Student* student = new Student();
    strcpy(student->firstName, firstname);
    strcpy(student->lastName, lastname);

    // Increments student id until it does not already exist in the table
    while(searchID(studentCount, table) == 1) {
      studentCount += 1;
    }

    // set the student ID
    student->studentID = studentCount;

    // Generate a random float for the GPA
    float random_float = static_cast<float>(std::rand()) / RAND_MAX;
    student->GPA = random_float*4;

    // Add the student to the table
    bool val = insert(student, table);
    if (val == true) {
      temp = true;
    }
    studentCount+=1;
  }
  // Return if rehashing is needed
  return temp;
}


void deleteNode(int studentID, Node*& head, Node* current, Node* previous) {
  // If the head is null we exit the function since there is nothing to delete
  cout << "Start" << endl;
  if (head == NULL) {
    return;
  }
  // If the current node is null this means we have gotten to the end of the linked list without finding a node that has a student with the inputted id
  else if (current == NULL) {
    return;
  }
  // If the inputted id and student id match on a node we delete the node and connect the previous node to the next node
  else if (current->getValue()->studentID == studentID) {
    cout << "Check 2" << endl;
    cout << previous << endl;
    if (previous != NULL) {
      previous->setNext(current->getNext());
    }
    cout << "here" << endl;
    if (current == head) {
      cout << "SAME NODE" << endl;
      head = NULL;
    }
    delete current;
    return;
  }
  // If no conditions are met the function is called with the next node
  deleteNode(studentID, head, current->getNext(), current);
  return;
}

// Function to search if student id already exists
bool searchID(int id, Hashtable* table) {
  // Hash the student ID and find the correct index in the table
  int index = hasher(id, table->size);
  Node* node = table->arr[index];
  // Loop through the linked list and return true if the ID exists
  while (node != NULL) {
    if (node->getValue()->studentID == id) {
      return true;
    }
    node = node->getNext();
  }
  // If id does not exist then return false
  return false;
}
