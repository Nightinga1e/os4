#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

static struct termios stored_settings;

void myhandler();
int cutout = 0;

void MyAlarm(){
	if(cutout==0) {
		cutout = 1;
	}else if(cutout==1){
		cutout = 0;
	}
}

void set_keypress(void)
{
	struct termios new_settings;
	
	tcgetattr(0,&stored_settings);
	
	new_settings = stored_settings;
	new_settings.c_lflag &=(~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	
	tcsetattr(0,TCSANOW, &new_settings);
	return;
}

void reset_keypress(){
	tcsetattr(0,TCSANOW, &stored_settings);
	return;
}

void myhandler(){
	if(cutout==0) {
		cutout = 1;
		}else{
			cutout=0;
			}
}

void* thread_a(void* args){
	while(1){
		if(cutout==0){
			printf ("\n A \n");
		}
		sleep(1);
		}
	return 0;
}

void* thread_b(void* args){
	while(1){	
		sleep(1);
		if(cutout==1){
			printf ("\n B \n");
		}
		}
	return 0;
}

void* thread_c(void* args){
int test = 0;
	while(1){
		test = getchar();
		if(test!=0){
			signal(SIGTERM, myhandler);
			raise(SIGTERM);
		}

	}
	return 0;
}

void* thread_d(void* args){
	while(1){
		sleep(2);
		signal(SIGALRM, MyAlarm);
		alarm(1);
	}
	return 0;
}

int main()
{
	set_keypress();
	pthread_t threada;
	pthread_t threadb;
	pthread_t threadc;
	pthread_t threadd;
	int statusa;
	int statusb;
	int statusc;
	int statusd;
	int status_addra;
	int status_addrb;
	int status_addrc;
	int status_addrd;

	statusa = pthread_create(&threada, NULL, thread_a, NULL);
	if(statusa != 0){
		printf ("\n Can't create thread \n");
	return -1;
	}

	statusb = pthread_create(&threadb, NULL, thread_b, NULL);
	if(statusb != 0){
		printf ("\n Can't create thread \n");
	return -1;
	}

	statusc = pthread_create(&threadc, NULL, thread_c, NULL);
	if(statusc != 0){
		printf ("\n Can't create thread \n");
	return -1;
	}

/*	statusd = pthread_create(&threadd, NULL, thread_d, NULL);
	if(statusd != 0){
		printf ("\n Can't create thread \n");
	return -1;
	}
*///-------------------timer

	statusa = pthread_join(threada, (void**)&status_addra);
	if(statusa!= 0){
		printf("\n Can't join thread \n");
		return -1;
	}

	statusb = pthread_join(threadb, (void**)&status_addrb);
	if(statusb!= 0){
		printf("\n Can't join thread \n");
		return -1;
	}
	
	statusc = pthread_join(threadc, (void**)&status_addrc);
	if(statusc!= 0){
		printf("\n Can't join thread \n");
		return -1;
	}	

/*
	statusd = pthread_join(threadd, (void**)&status_addrd);
	if(statusd!= 0){
		printf("\n Can't join thread \n");
		return -1;
	}*/

	reset_keypress();
	return 0;
}
