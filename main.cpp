#include <iostream>
#include "Node.h"
#include "Student.h"
#include <cstring>
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
Hashtable* rehash(Hashtable* table);

int main() {
  Student* newStudent = new Student();
  strcpy(newStudent->firstName, "Aditya");
  strcpy(newStudent->lastName, "Chandrashekaran");
  newStudent->studentID = 514957;
  newStudent->GPA = 4.00;

  Student* student2 = new Student();
  strcpy(student2->firstName, "ddeeee");
  strcpy(student2->lastName, "Chandrashekaran");
  student2->studentID = 101010;
  student2->GPA = 4.00;

  Student* student3 = new Student();
  strcpy(student3->firstName, "ddeeee");
  strcpy(student3->lastName, "Chandrashekaran");
  student3->studentID = 101010;
  student3->GPA = 4.00;

  Student* student4 = new Student();
  strcpy(student4->firstName, "ddeeee");
  strcpy(student4->lastName, "Chandrashekaran");
  student4->studentID = 101010;
  student4->GPA = 4.00;


  //cout << newStudent->firstName << endl;

  //Node* node = new Node();
  //node->setValue(newStudent);

  Hashtable* table = new Hashtable();
  makeHashtable(table, 100);
  bool out = insert(newStudent, table);
  cout << out << endl;

  bool out2 = insert(student2, table);
  cout << out2 << endl;

  bool out3 = insert(student3, table);
  cout << out3 << endl;

  bool out4 = insert(student4, table);
  cout << out4 << endl;

  print(table);
  //table->arr[hasher(newStudent, table->size)] = node;
  //cout << table->arr[hasher(newStudent, table->size)]->getValue()->studentID << endl;
  //cout << (table->arr[61] == NULL) << endl;
  return 0;
}

void makeHashtable(Hashtable* table, int size) {
  table->size = size;
  table->arr = new Node*[size];
  for(int i = 0; i < size+1; i++) {
    table->arr[i] = NULL;
  }
}

int hasher(Student* student, int size) {
  int key = 0;
  for(int i = 0; i < strlen(student->firstName); i++) {
    key += (int)student->firstName[i];
  }
  key = key+=student->studentID;
  key = key%size;
  //cout << key << endl;
  return key;
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
  bool rehash = addNode(student, table->arr[hasher(student, table->size)]);
  return rehash;
}

void printNode(Node* next, Node* head) {
  if (next == head) {
    cout << "LIST: ";
  }
  if (next != NULL) {
    cout << next->getValue()->firstName << " ";
    printNode(next->getNext(), head);
  }
}

void print(Hashtable* table) {
  int num = 0;
  while (num < table->size) {
    cout << "" << endl;
    cout << num << " - ";
    printNode(table->arr[num], table->arr[num]);
    num +=1;
  }
}

Hashtable* rehash(Hashtable* table) {
  Hashtable* newTable = new Hashtable();
  makeHashtable(newTable, (table->size)*2);

  int num = 0;
  while (num < table->size) {
    Node* node = table->arr[num];
    while (node != NULL) {
      insert(node->getValue(), newTable);
    }
    num += 1;
  }
  
  return newTable;
}

//void generateStudents(int number) {

//}
