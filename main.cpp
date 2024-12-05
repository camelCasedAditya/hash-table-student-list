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
void insert(Student* student, Hashtable* table);


int main() {
  Student* newStudent = new Student();
  strcpy(newStudent->firstName, "Aditya");
  strcpy(newStudent->lastName, "Chandrashekaran");
  newStudent->studentID = 514957;
  newStudent->GPA = 4.00;

  cout << newStudent->firstName << endl;

  Node* node = new Node();
  node->setValue(newStudent);

  Hashtable* table = new Hashtable();
  makeHashtable(table, 100);
  insert(newStudent, table);
  //table->arr[hasher(newStudent, table->size)] = node;
  cout << table->arr[hasher(newStudent, table->size)]->getValue()->studentID << endl;
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
  key += student->studentID;
  key = key%size;
  return key;
}

void insert(Student* student, Hashtable* table) {
  Node* node = new Node();
  node->setValue(student);
  table->arr[hasher(student, table->size)] = node;
}

