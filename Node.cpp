#include "Node.h"
#include "Student.h"

// Constructor for Node object
Node::Node() {
  value = new Student();
  next = NULL;
}

// Destructor for Node object
Node::~Node() {
  delete value;
  next = NULL;
}

// Set the value for the node
void Node::setValue(Student* newStudent) {
  value = newStudent;
}

// get the value of the node
Student* Node::getValue() {
  return value;
}

// set the pointer to the next node
void Node::setNext(Node* newNext) {
  next = newNext;
}

// Get the pointer to the next node
Node* Node::getNext() {
  return next;
}
