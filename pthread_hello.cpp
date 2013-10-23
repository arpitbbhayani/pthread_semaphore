#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

void * thread_function ( void * arg );

int main ( int argc , char * argv[] ) {

	int result = 0;

	pthread_t a_thread;

	/* Creating the thread */
	result = pthread_create ( &a_thread , NULL , thread_function , NULL );

	/* This function will make the main() wait untill the thread 'a_thread' terminates 
		Try commenting the line pthread_join ( a_thread , NULL );
	*/
	pthread_join ( a_thread , NULL );

	cout << "a_thread terminated so main will continue its execution." << endl;

	return 0;
}

void * thread_function ( void * arg ) {

	cout << "a_thread created" << endl;
	cout << "a_thread sleeping for 4 sec" << endl;
	sleep(4);
	cout << "a_thread woke" << endl;
	return NULL;
}
