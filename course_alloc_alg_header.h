#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <cstring>

#define	KNOWLEDGE_SPEC_1 0
#define	KNOWLEDGE_SPEC_2 1
#define	KNOWLEDGE_SPEC_3 2
#define	KNOWLEDGE_SPEC_4 3

#define MCOM 0
#define PHD  1
#define BCOM 2
#define ARTS 3

using namespace std;

/*
	Input parameter
		s: no. of students
		c: no. of courses
*/
struct value {
	int s;
	int c;
};

/*
	name : Name of the branch
	branch_students : This semaphore contains the value of max number of students allowed in a course of a particular branch.

*/
struct branch {
	char name[8];
	int branch_students;
	//sem_t branch_students_sem;
};

/*

	id : id of the course
	no_students : Max value : 60
	b[4] : contains the 4 instances of branch structure each for one branch MCOM , PHD , BCOM , ARTS.
	student : Semaphore that ensures max no. of students = 60
	know_spec :
		1. KNOWLEDGE_SPEC_1
		2. KNOWLEDGE_SPEC_2
		3. KNOWLEDGE_SPEC_3
		4. KNOWLEDGE_SPEC_4
*/
struct course {
	int id;
	int know_spec;
	int no_students;
	sem_t student;
	struct branch b[4];
};

/*
	id: id of the student
	c[4] : the four courses allocated to student - boolean value
		if : KNOWLEDGE_SPEC_1 -> c[0] = TRUE
	preference[8] : 8 preferences of the student.
*/
struct student {
	int id;
	int branch;
	int c[4];
	int preference[8];
	int courses_to_be_allocated;
	int course_ids[4];
};

/* Functions */
void   setup_environment();
void * student_thread ( void * );
void   shuffle( int * , int );

/* Global variable */
struct value v;
struct course * c;
struct student * s;
int    * array;

pthread_t * s_thread;
ofstream  fp;
