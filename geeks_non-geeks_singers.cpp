#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <iomanip>

#define BUCKET 0

using namespace std;

struct people {
	int g;
	int n;
	int s;
};

/* Functions */
void 	random_event();
void * 	geek_arrives( void * );
void * 	non_geek_arrives( void * );
void * 	singer_arrives( void * );
void	print_statistics( int );
void	print_trip( int , int , int );
int 	send_bucket();

/* Global Variables */
struct people people_count;
struct people bucket;

sem_t mutex ;
pthread_t *thread;

int k = 0;

int main ( int argc , char * argv[] ) {

	int l = 0;

	if ( argc != 4 ) {
		cout << "Usage : ./a.out <# geeks> <# non_geeks> <# singers>" << endl;
		exit(0);
	}

	people_count.g = atoi ( argv[1] );
	people_count.n = atoi ( argv[2] );
	people_count.s = atoi ( argv[3] );

	l = people_count.g + people_count.n + people_count.s;

	thread = ( pthread_t * ) new pthread_t[l*2];

	srand( time(0) );

	sem_init(&mutex,0,1);

	while ( 1 ) {
		random_event();
		int s = send_bucket();

		sem_wait(&mutex);

		if ( s == 0 && people_count.g == 0 && people_count.n == 0 && people_count.s == 0 ) {
			break;
		}

		sem_post(&mutex);
	
	}

	/*
		for ( int i = 0 ; i < l ; i++ ) {
			cout << "i = " << i << endl;
		pthread_join ( thread[i] , NULL );
		}
	*/

	print_statistics( BUCKET );

	return 0;
}

void reduce_count ( int g , int n , int s ) {

	sem_wait(&mutex);

	bucket.g -= g;
	bucket.n -= n;
	bucket.s -= s;

	print_trip( g , n , s );

	sem_post(&mutex);
}

int can_send() {

	int g = bucket.g;
	int n = bucket.n;
	int s = bucket.s;

	if ( g >= 4 && n >= 0 && s >= 0 ) {
		reduce_count(4,0,0);
		return 1;
	}
	else if ( g >= 0 && n >= 4 && s >= 0 ) {
		reduce_count(0,4,0);
		return 1;
	}
	else if ( g >= 2 && n >= 2 && s >= 0 ) {
		reduce_count(2,2,0);
		return 1;
	}
	else if ( g >= 3 && n >= 0 && s >= 1 ) {
		reduce_count(3,0,1);
		return 1;
	}
	else if ( g >= 0 && n >= 3 && s >= 1 ) {
		reduce_count(0,3,1);
		return 1;
	}
	else if ( g >= 2 && n >= 1 && s >= 1 ) {
		reduce_count(2,1,1);
		return 1;
	}
	else if ( g >= 1 && n >= 2 && s >= 1 ) {
		reduce_count(1,2,1);
		return 1;
	}

	return 0;

}

void send_the_people() {
	sleep(1);
}

int send_bucket() {

	int can_send_val = can_send();

	if ( can_send_val == 1 ) {
		send_the_people();
	}
	return can_send_val;

}

void random_event() {

	int n = rand() % 3;

	switch ( n ) {

		case 0:
			if ( people_count.g > 0 ) {
				pthread_create( &thread[k++] , NULL , geek_arrives , NULL );
			}
			break;
		case 1:
			if ( people_count.n > 0 ) {
				pthread_create( &thread[k++] , NULL , non_geek_arrives , NULL );
			}
			break;
		case 2:
			if ( people_count.s > 0 ) {
				pthread_create( &thread[k++] , NULL , singer_arrives , NULL );
			}
			break;
	}

}

void * geek_arrives( void * arg ) {

	int flag = 0;
	sem_wait(&mutex);
	if ( people_count.g > 0 ) {
		people_count.g --;
		bucket.g ++;
	}
	sem_post(&mutex);

	return NULL;
}

void * non_geek_arrives ( void * arg ) {

	int flag = 0;
	sem_wait(&mutex);
	if ( people_count.n > 0 ) {
		people_count.n --;
		bucket.n ++;
	}
	sem_post(&mutex);
	return NULL;
}

void * singer_arrives ( void * arg ) {

	int flag = 0;
	sem_wait(&mutex);
	if ( people_count.s > 0 ) {
		people_count.s --;
		bucket.s ++;
	}
	sem_post(&mutex);
	return NULL;
}

/*
void print_header() {
	cout << endl << setw(46) << "Statistics" << endl;
	cout << endl << setw(46) << "Trips" << endl;
	cout << "+-----------------------------------------------------------------------------------------------------------+" << endl;
	cout << "|           # Trip         |          # Geek         |        # Non-Geek        |          # Singer         |" << endl;
	cout << "+-----------------------------------------------------------------------------------------------------------+" << endl;
}
*/

void print_trip ( int g , int n , int s ) {
	cout << "# Geeks : "  << g << "      # Non Geek : " << n << "      # Singers : " << s << endl;
}

void print_statistics( int type ) {
	sleep(1);

	cout << endl << setw(46) << "People left" << endl;
	cout << "+--------------------------------------------------------------------------------+" << endl;
	cout << "|          # Geek         |        # Non-Geek        |          # Singer         |" << endl;
	cout << "+--------------------------------------------------------------------------------+" << endl;
		cout << "|" << setw(14) << bucket.g << setw(12)  << "|" << setw(14) << bucket.n << setw(13) << "|" << setw(15) << bucket.s << setw(13) << "|" << endl;
	cout << "+--------------------------------------------------------------------------------+" << endl;

}
