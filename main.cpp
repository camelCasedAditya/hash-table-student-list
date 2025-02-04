#include <iostream>
#include "Node.h"
#include "Student.h"
#include <cstring>
#include <fstream>
#include <ctime>
using namespace std;


struct Hashtable {
  int size;
  Node** arr;
};

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
  int studentCount = 0;
  srand(time(NULL));
  Hashtable* table = new Hashtable();
  makeHashtable(table, 100);
    
  bool running = true;
  while(running == 1) {
    char input[100];
    cout << "Enter one of the following commands: \"ADD\", \"DELETE\", \"GENERATE\", \"PRINT\", or \"QUIT\"" << endl;
    cin >> input;
    if(strcmp(input, "ADD") == 0) {
      Student* student = new Student();
      cout << "Enter the first name" << endl;
      cin>>student->firstName;
      cout << "Enter the last name" << endl;
      cin >> student->lastName;
      cout << "Enter the GPA" << endl;
      cin >> student->GPA;

      // CHANGE TO USER INPUT WITH ID UNIQUENESS VERIFICATION
      int id = -1;
      cout << "Enter the student id" << endl;
      cin >> id;
      bool idExists = searchID(id, table);
      while (idExists == 1) {
	cout << "That id already exists. Enter a new student ID" << endl;
	cin >> id;
	idExists = searchID(id, table);
      }
      student->studentID = id;
      studentCount+=1;
      bool collision = insert(student, table);
      if (collision == true) {
        table = rehash(table);
      }

    }
    else if (strcmp(input, "PRINT") == 0) {
      print(table);
    }
    else if (strcmp(input, "DELETE") == 0) {
      remove(table);
    }
    else if (strcmp(input, "GENERATE") == 0) {
      int number = 0;
      cout << "Enter the number of student to generate" << endl;
      cin >> number;
      bool collision = generate(number, table, studentCount);
      if (collision == true) {
	table = rehash(table);
      }
    }
    else if (strcmp(input, "QUIT") == 0) {
      running = false;
    }
    else {
      cout << "Invalid command" << endl;
    }
  }

  return 0;
}

void makeHashtable(Hashtable* table, int size) {
  table->size = size;
  table->arr = new Node*[size];
  for(int i = 0; i < size+1; i++) {
    table->arr[i] = NULL;
  }
}

int hasher(int studentID, int size) {
  int key = 0;
  key = studentID*7;
  key = key%size;
  return key;
}

void remove(Hashtable* table) {
  int id = -1;
  cout << "Enter the student ID: " << endl;
  cin >> id;
  int index = hasher(id, table->size);
  deleteNode(id, table->arr[index], table->arr[index], NULL);
}

bool addNode(Student* newStudent, Node*& head) {
  Node* current = head;
  int num = 0;
  if (current == NULL) {
    head = new Node();
    head->setValue(newStudent);
    return false;
  }
  else {
    num += 1;
    while (current->getNext() != NULL) {
      current = current->getNext();
      num += 1;
    }
    current->setNext(new Node());
    current->getNext()->setValue(newStudent);
    num += 1;
    cout << "LENGTH: " << num << endl;
    if (num > 3) {
      return true;
    }
    else {
      return false;
    }
  }
}

bool insert(Student* student, Hashtable* table) {
  //Node* node = new Node();
  //node->setValue(student);
  bool rehash = addNode(student, table->arr[hasher(student->studentID, table->size)]);
  return rehash;
}

void printNode(Node* next, Node* head) {
  if (next != NULL) {
    cout << next->getValue()->firstName << " " << next->getValue()->lastName << ", " << next->getValue()->studentID << ", " << next->getValue()->GPA << endl;
    printNode(next->getNext(), head);
  }
}

void print(Hashtable* table) {
  int num = 0;
  while (num < table->size) {
    printNode(table->arr[num], table->arr[num]);
    num +=1;
  }
}

Hashtable* rehash(Hashtable* table) {
  Hashtable* newTable = new Hashtable();
  makeHashtable(newTable, ((table->size)*2)+7);

  int num = 0;
  while (num < table->size) {
    Node* node = table->arr[num];
    while (node != NULL) {
      insert(node->getValue(), newTable);
      node = node->getNext();
    }
    num += 1;
  }
  return newTable;
}

bool generate(int num, Hashtable* table, int& studentCount) {
  int FIRSTNAMECOUNTS = 164;
  int LASTNAMECOUNTS = 295;
  bool temp = false;
  for (int i = 0; i < num; i++) {
    ifstream file("firstnames.txt");
    int line = rand()%(FIRSTNAMECOUNTS+1);
    char firstname[200];
    int loop = 0;

    while (file.getline(firstname, sizeof(firstname))) {
      if (loop == line) {
        cout << firstname << endl;
        break;
      }
      loop += 1;
    }
    ifstream file2("lastnames.txt");
    line = rand()%(LASTNAMECOUNTS+1);
    char lastname[200];
    loop = 0;
    while (file2.getline(lastname, sizeof(lastname))) {
      if (loop == line) {
        break;
      }
      loop += 1;
    }
    Student* student = new Student();
    strcpy(student->firstName, firstname);
    strcpy(student->lastName, lastname);
    while(searchID(studentCount, table) == 1) {
      studentCount += 1;
    }
    student->studentID = studentCount;
    float random_float = static_cast<float>(std::rand()) / RAND_MAX;
    student->GPA = random_float*4;
    bool val = insert(student, table);
    if (val == true) {
      temp = true;
    }
    studentCount+=1;
    cout << "ADDED" << endl;
  }
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

bool searchID(int id, Hashtable* table) {
  int index = hasher(id, table->size);
  Node* node = table->arr[index];
  while (node != NULL) {
    if (node->getValue()->studentID == id) {
      return true;
    }
    node = node->getNext();
  }
  return false;
}
