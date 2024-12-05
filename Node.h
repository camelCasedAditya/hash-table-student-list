#ifndef NODE_HEADER
#define NODE_HEADER
#include <iostream>
#include "Student.h"
using namespace std;

class Node {
 public:
  // Initialize functions for Node class
  Node();
  ~Node();
  void setValue(Student* newStudent);
  Student* getValue();
  void setNext(Node* newNext);
  Node* getNext();
 private:
  // Node object attributes
  Student* value;
  Node* next;
};
#endif
