#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

/* This structure will hold all the parameters that we need to pass to a thread */
struct parameters {
	int val_int;
	char val_char;
};

void * thread_function ( void * arg );

int main ( int argc , char * argv[] ) {

	int result = 0;

	pthread_t a_thread;

	struct parameters param;

	/* Populating the parameters in sturct parameters */
	param.val_int = 1024;
	param.val_char = 'A';

	/* Creating the thread with parameters 'param'*/
	result = pthread_create ( &a_thread , NULL , thread_function , &param );

	/* This function will make the main() wait untill the thread 'a_thread' terminates 
		Try commenting the line pthread_join ( a_thread , NULL );
	*/
	pthread_join ( a_thread , NULL );

	cout << "a_thread terminated so main will continue its execution." << endl;

	return 0;
}

void * thread_function ( void * arg ) {

	struct parameters * p = ( struct parameters *) arg;

	cout << "In thread_function the parameters passed are " << endl;
	cout << "1. val_int = " << p -> val_int << endl;
	cout << "2. val_char = " << p -> val_char << endl;

	return NULL;
}
