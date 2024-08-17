#ifndef ASSIGN2_STUDENTINFO_H
#define ASSIGN2_STUDENTINFO_H

#include <string>
#include "Student.h"

// Define student information
const Student students[] = {
    {"Rebecca Watson", "s3903758", "s3903758@student.rmit.edu.au"},
    {"Alan Donovan", "s3255561", "s3255561@student.rmit.edu.au"},
    {"Nathan Christos", "s3788418", "s3788418@student.rmit.edu.au"},
    {"Mathew Craig", "s3824227", "s3824227@student.rmit.edu.au"}
};

// Define the number of students in the array
const size_t numStudents = sizeof(students) / sizeof(Student);

#endif // ASSIGN2_STUDENTINFO_H
