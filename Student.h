#ifndef STUDENT_STRUCT
#define STUDENT_STRUCT

#include <cstring>
using namespace std;

struct Student {
  char firstName[100];
  char lastName[100];
  int studentID;
  float GPA;
};

#endif
