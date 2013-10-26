#include "course_alloc_alg_header.h"
#include <iomanip>
#include <unistd.h>

int array_index = 0;
sem_t mutex;

void print_statistics();

void * student_thread ( void * arg ) {

	sem_wait ( &mutex );

	struct student * s_temp = &s[ array[ array_index ] ];
	array_index++;

	for ( int i = 0 ; i < 8 ; i++ ) {
		int t_course_id = s_temp->preference[i];
		int t_branch_student = s_temp->branch;

		//cout << "preference # : " << i << endl;
		//cout << "course # : " << t_course_id << endl;
		//cout << "branch of the student # : " << t_branch_student << endl;

		/* Check if number of students of the branch in the */

		int t_stu_of_branch_in_course = c[ t_course_id ].b[ t_branch_student ].branch_students;

		//cout << "students in branch # : " << t_stu_of_branch_in_course << endl;
		//sem_getvalue ( &c[ t_course_id ].b[ t_branch_student ].branch_students , &t_stu_of_branch_in_course );

		/* If number of students of the particular branch in course are to its max limit */
		if ( t_stu_of_branch_in_course == 0 ) {
			/* Then do nothing */
		}
		else {
			/* If the branch in the course can still take up students. */

			/* If student has already taken up the course of a knowledge_spectrum then he cannot take it */

			/* Student can get any type of course */
			if ( s_temp->courses_to_be_allocated > 0 ) {

				s_temp->course_ids[4-s_temp->courses_to_be_allocated] = t_course_id;

				s_temp->courses_to_be_allocated --;
				c[ t_course_id ].b[ t_branch_student ].branch_students --;
				c[ t_course_id ].no_students ++;

				//fp << "course # " << t_course_id << " allocated to student # " << s_temp->id << " of branch ";
				//fp << s_temp->branch << endl;
			}

			/* Each type of Knowledge Spectrum should be allocated to the student */
			/*if ( s_temp->c[ c[t_course_id].know_spec ] == 0 ) {
				s_temp->c[ c[t_course_id].know_spec ] = 1;

				c[ t_course_id ].b[ t_branch_student ].branch_students --;
				c[ t_course_id ].no_students ++;

				cout << "course # " << t_course_id << " allocated to student # " << s_temp->id << endl;

				break;
			}*/

		}
	}


	sem_post ( &mutex );
	return NULL;
}

int main ( int argc , char * argv[] ) {

	if ( argc != 3 ) {
		cout << "Usage ./a.out <# students> <# course>" << endl;
		exit(0);
	}

	v.s = atoi(argv[1]);
	v.c = atoi(argv[2]);

	setup_environment();

	sem_init ( &mutex , 0 , 1 );

	for ( int i = 0 ; i < v.s ; i++ ) {
		pthread_create ( &s_thread[i] , NULL , student_thread , NULL );
	}

	for ( int i = 0 ; i < v.s ; i++ ) {
		pthread_join ( s_thread[i] , NULL );
	}

	int count = 0;
	cout << "Students who were not allocated all four courses : " << endl;
	for ( int i = 0 ; i < v.s ; i++ ) {
		/* Check for each type to knowledge_spectrum should be cover for course */

		/*
			if ( s[i].c[0] == 0 && s[i].c[1] == 0 && s[i].c[2] == 0 && s[i].c[3] == 0 ) {
				count ++;
			}
		*/

		/* 
			Check for the total number of courses to be allocated i.e. courses that can be
			allocated to the students can be from any knowledge spectrum
		*/

		if ( s[i].courses_to_be_allocated > 0 ) {
			cout << "Student #" << s[i].id << endl;
			count ++;
		}
	}

	for ( int i = 0 ; i < v.c ; i++ ) {
		fp << "Course #" << c[i].id << " has " << c[i].no_students << " students." << endl;
	}

	cout << "total : " << count << endl;

	/*for ( int i = 0 ; i < v.c ; i++ ) {
		cout << "course # " << c[i].id << " no_students = " << c[i].no_students << endl;
	}*/

	//print_statistics();


	fp.close();

	return 0;

}

void setup_environment() {

	srand ( time(0) );

	fp.open ( "allocation.txt" , ios::out );

	c = new struct course[v.c];
	s = new struct student[v.s];
	s_thread = new pthread_t[v.s];

	for ( int i = 0 ; i < v.c ; i++ ) {

		c[i].id = i;
		c[i].no_students = 0;
		sem_init ( &c[i].student , 0 , 60 );

		/*
			sem_init ( &c[i].b[MCOM].branch_students_sem , 0 , 12 );
			sem_init ( &c[i].b[PHD] .branch_students_sem , 0 , 12 );
			sem_init ( &c[i].b[BCOM].branch_students_sem , 0 , 24 );
			sem_init ( &c[i].b[ARTS].branch_students_sem , 0 , 12 );
		*/

		c[i].b[MCOM].branch_students = 12;
                c[i].b[PHD] .branch_students = 12;
                c[i].b[BCOM].branch_students = 24;
                c[i].b[ARTS].branch_students = 12;

		switch ( i % 4 ) {
			case 0:
				c[i].know_spec = KNOWLEDGE_SPEC_1;
				break;
			case 1:
				c[i].know_spec = KNOWLEDGE_SPEC_2;
				break;
			case 2:
				c[i].know_spec = KNOWLEDGE_SPEC_3;
				break;
			case 3:
				c[i].know_spec = KNOWLEDGE_SPEC_4;
				break;
		}

		/* Dumping data to file 'allocation.txt' all info about the course*/

		/*int sem_value = 0;
		sem_getvalue ( &c[i].student , &sem_value );

		fp << "Course # " << c[i].id << endl;
		fp << "\tknow_spec : " << c[i].know_spec << endl;
		fp << "\tno. of students : " << c[i].no_students << endl;
		fp << "\tsem_val of student " << sem_value << endl;

		for ( int j = 0 ; j < 4 ; j++ ) {
			//sem_getvalue( &c[i].b[j].branch_students , &sem_value);
			fp << "\t\tbranch # " << j << " sem_val_branch_student : " << c[i].b[j].branch_students << endl;
		}
		*/

	}

	int know_spec_1[v.c] , know_spec_2[v.c] , know_spec_3[v.c] , know_spec_4[v.c];
	int i1 = 0 , i2 = 0 , i3 = 0 , i4 = 0;

	for ( int i = 0 ; i < v.c ; i++ ) {
		switch ( c[i].know_spec ) {
			case  KNOWLEDGE_SPEC_1:
				know_spec_1[i1++] = c[i].id;
				break;
			case  KNOWLEDGE_SPEC_2:
				know_spec_2[i2++] = c[i].id;
				break;
			case  KNOWLEDGE_SPEC_3:
				know_spec_3[i3++] = c[i].id;
				break;
			case  KNOWLEDGE_SPEC_4:
				know_spec_4[i4++] = c[i].id;
				break;
		}
	}

	for ( int i = 0 ; i < v.s ; i++ ) {
		s[i].id = i;
		s[i].courses_to_be_allocated = 4;
		s[i].c[0] = s[i].c[1] = s[i].c[2] = s[i].c[3] = 0;
		switch ( i % 4 ) {
		case 0:
			s[i].branch = MCOM;
			break;
		case 1:
			s[i].branch = PHD;
			break;
		case 2:
			s[i].branch = BCOM;
			break;
		case 3:
			s[i].branch = ARTS;
			break;
		}

		shuffle ( know_spec_1 , i1 );
		shuffle ( know_spec_2 , i2 );
		shuffle ( know_spec_3 , i3 );
		shuffle ( know_spec_4 , i4 );

		s[i].preference[0] = know_spec_1[0];
		s[i].preference[1] = know_spec_1[1];
		s[i].preference[2] = know_spec_2[0];
		s[i].preference[3] = know_spec_2[1];
		s[i].preference[4] = know_spec_3[0];
		s[i].preference[5] = know_spec_3[1];
		s[i].preference[6] = know_spec_4[0];
		s[i].preference[7] = know_spec_4[1];

		shuffle ( s[i].preference , 8 );

		s[i].course_ids[0] = s[i].course_ids[1] = s[i].course_ids[2] = s[i].course_ids[3] = -1;

		/* Dumping data to file 'allocation.txt' all info about the students */

		/*int sem_value = 0;
		sem_getvalue ( &c[i].student , &sem_value );

		fp << "Student # " << s[i].id << endl;
		fp << "\tbranch : " << s[i].branch << endl;
		fp << "\tc[4] : " << s[i].c[0] << " " << s[i].c[1] << " " <<s[i].c[2] << " " << s[i].c[3] << " " << endl;
		fp << "\tpreference : " << endl;
		for ( int j = 0 ; j < 8 ; j++ ) {
			fp << "\t\t" << j+1 << ". " << " course # : " << s[i].preference[j] << endl;
		}*/

	}


	array = new int[v.s];

	for( int i = 0 ; i < v.s ; i++ ) {
		array[i] = i;
	}

	shuffle( array , v.s );

	/* Dumping students order into allocation.txt */

	/*fp << "Students order : " << endl;
	for( int i = 0 ; i < v.s ; i++ ) {
		fp << "Student # " << array[i] << endl;
	}*/

}

void shuffle( int * array , int n ) {


	for ( int i = 0 ; i < n ; i++ ) {
		int r = i + rand() % (n - i);
		int temp = array[i];
		array[i] = array[r];
		array[r] = temp;
	}

}

void print_statistics () {

	fp << setw(50) <<"Student V/S Courses" << endl;
	fp << "+------------------------------------------------------------------------------------+" << endl;
	fp << "+    Student     |    Course 1    |    Course 2    |    Course 3    |    Course 1    |" << endl;
	fp << "+------------------------------------------------------------------------------------+" << endl;
	for ( int i = 0 ; i < v.s ; i++ ) {
		fp << "|" << setw(8) << i << setw(8) << "";
		for ( int j = 0 ; j < 4 ; j++ )
			if ( s[i].course_ids[j] != -1 )
				fp << "|" << setw(9) << s[i].course_ids[j] << setw(7) << "";
			else
				fp << "|" << setw(9) << "-" << setw(7) << "";

		fp << "|" << endl;
	}
	fp << "+------------------------------------------------------------------------------------+" << endl;

	int mapping[v.c][4] ;

	for ( int i = 0 ; i < v.c ; i++ ) {
		mapping[i][0] = mapping[i][1] = mapping[i][2] = mapping[i][3] = 0;
	}

	for ( int i = 0 ; i < v.s ; i++ ) {
		for ( int j = 0 ; j < 4 ; j++ )
			if ( s[i].course_ids[j] != -1 ) {
				mapping[ s[i].course_ids[j] ][ s[i].branch ] ++;
				//cout << "Napping : " <<  s[i].course_ids[j]  << " and " << s[i].branch << endl;
			}

	}

	int count2 = 0;

	fp << setw(50) <<"Courses V/S Branch" << endl;
	fp << "+------------------------------------------------------------------------------------+" << endl;
	fp << "+     Courses    |    Branch 1    |    Branch 2    |    Branch 3    |    Branch 4    |" << endl;
	fp << "+------------------------------------------------------------------------------------+" << endl;
	for ( int i = 0 ; i < v.c ; i++ ) {
		fp << "|" << setw(8) << i << setw(8) << "";
		for ( int j = 0 ; j < 4 ; j++ )
			if ( s[i].course_ids[j] != -1 ) {
				fp << "|" << setw(9) << mapping[i][j] << setw(7) << "";
				count2 += mapping[i][j];
			}
			else
				fp << "|" << setw(9) << "-" << setw(7) << "";

		fp << "|" << endl;
	}
	fp << "+------------------------------------------------------------------------------------+" << endl;

	//cout << "Count2 = " << count2 << endl;

	for ( int i = 0 ; i < v.c ; i++ ) {
		//cout << c[i].no_students << endl;
	}

}
