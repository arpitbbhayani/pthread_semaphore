#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <iomanip>

using namespace std;

struct value {
	int h;
	int o;
	int s;
	int e;
};

struct site_data {
	int e;
};

/* Global Variables */

sem_t	    allow_site , energy_threshold , reaction;
sem_t 	  * site_sem;
pthread_t * site_threads;
site_data * s_d;
struct value v;

/* Functions */

void * site_thread ( void * );
void print_statistics();
void setup_environment();


void setup_environment() {

	site_sem	= ( sem_t * ) 	  malloc ( v.s * sizeof(sem_t) );
	site_threads    = ( pthread_t * ) malloc ( v.s * sizeof(pthread_t) );
	s_d	    	= ( site_data * ) malloc ( v.s * sizeof(site_data) );

	sem_init ( &allow_site , 0 , 0 );
	sem_init ( &energy_threshold , 0 , v.e );
	sem_init ( &reaction , 0 , 1 );

	/* Initially at all sites reaction can happen and energy at each site = 0 */
	for ( int i = 0 ; i < v.s ; i++ ) {
		sem_init ( &site_sem[i] , 0 , 1 );
		s_d[i].e = 0;
		
	}

	/* Create thread for each site */
	for ( int i = 0 ; i < v.s ; i++ ) {
		pthread_create ( &site_threads[i] , NULL , site_thread , NULL );
	}

	srand( time(0) );

}

void * site_thread ( void * arg ) {

	while ( 1 ) {

		sem_wait ( &allow_site );

		int site_selected = rand() % v.s;

		if ( v.s == 1 ) {

		}
		else if ( site_selected == 0 ) {
			/* First site selected */
			sem_wait ( &site_sem[1] );
			sem_post ( &site_sem[1] );
		}
		else if ( site_selected == v.s - 1 ) {
			/* Last site selected */
			sem_wait ( &site_sem[v.s - 2] );
			sem_post ( &site_sem[v.s - 2] );
		}
		else {
			/* Any of the non-extreme site selected */
			sem_wait ( &site_sem[site_selected - 1] );
			sem_post ( &site_sem[site_selected - 1] );
			sem_wait ( &site_sem[site_selected + 1] );
			sem_post ( &site_sem[site_selected + 1] );
		}

		/* Locking current site */
		sem_wait ( &site_sem[site_selected] );

		/* Allowing other site to be selected */
		sem_post ( &allow_site );

		/* Reaction Start - { Critical Region } */
		sem_wait ( &reaction );

		/* If hydrogen and oxygen atoms are inadequate for any further reaction then terminate the code */
		if ( v.h < 2 || v.o < 1 ) {
			print_statistics();
			exit (0);
		}

		sem_wait ( &energy_threshold );

		v.h -= 2;
		v.o -= 1;

		s_d[site_selected].e += 1;

		/*
			int simul_reaction = 0;
			sem_getvalue( &energy_threshold , &simul_reaction );
			simul_reaction = v.e - simul_reaction;
		*/

		cout << "Reaction at site # : " << setw(3) << site_selected + 1 ;
		cout << "    # H-atoms : " << setw(3) << v.h ;
		cout << "    # O-atoms : " << setw(3) << v.o ;
		cout << "    # energy released : " << setw(3) << s_d[site_selected].e << " EMj " << endl;

		sem_post ( &reaction );

		/* Time for reaction @ current site */
		sleep(1);

		sem_post ( &energy_threshold );
		sem_post ( &site_sem[site_selected] );

		/* {End of Critical Region} */

	}

	return NULL;
}

int main ( int argc , char * argv[] ) {

	if ( argc != 5 ) {
		cout << "Usage : ./a.out <# h atoms> <# o atoms> <# sites> <energy>" << endl;
		exit(0);
	}

	v.h = atoi ( argv[1] );
	v.o = atoi ( argv[2] );
	v.s = atoi ( argv[3] );
	v.e = atoi ( argv[4] );

	if ( v.e == 0 ) {
		cout << "Energy threshold cannot be zero." << endl;
		exit(0);
	}

	setup_environment();

	/*
		Allow any one site to enter the critical region of REACTION.
		This will start the process of generating water.
	*/
	sem_post ( &allow_site );

	for ( int i = 0 ; i < v.s ; i++ ) {
		pthread_join ( site_threads[i] , NULL );
	}

	return 0;
}

void print_statistics() {
	sleep(1);
	cout << endl << setw(32) << "Statistics\n" << endl;
	cout << setw(33) << "Site information" << endl;
	cout << "+---------------------------------------------------------+" << endl;
	cout << "|" << setw(25) << "Site #         |" << "          Energy Released       |" << endl;
	cout << "+---------------------------------------------------------+" << endl;
	for ( int i = 0 ; i < v.s ; i++ ) {
		cout << "|" << setw(13) << i+1 << setw(12)  << "|" << setw(14) << s_d[i].e << " EMJ" << setw(15) << "|" << endl;
	}
	cout << "+---------------------------------------------------------+" << endl << endl;
	cout << endl << setw(36) << "H-atoms & O-atoms Left" << endl;
	cout << "+---------------------------------------------------------+" << endl;
	cout << "|" << setw(25) << "# H-atoms        |" << "           # O-atoms            |" << endl;
	cout << "+---------------------------------------------------------+" << endl;
	cout << "|" << setw(13) << v.h << setw(12)  << "|" << setw(16) << v.o << "" << setw(17) << "|" << endl;
	cout << "+---------------------------------------------------------+" << endl << endl;
}
