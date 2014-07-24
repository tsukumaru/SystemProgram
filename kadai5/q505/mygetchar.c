#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define TIMEOUT -2

int timeOutFlag = 0;
volatile sig_atomic_t alrm_count;

void print_resut(int res)
{
	if (res == -1) 
		printf("input is \"EOF\"\n");
	else if (res == -2) 
		printf("timeout\n");
	else if (res == -3) 
		printf("other\n");
	else 
		printf("input is \'%c\'\n", res);
}

int get_resut(int c)
{
	if (c == EOF)
		return -1;
	else if (c == TIMEOUT)
		return -2;
	else if (c >= 0)
		return c;
	else 
		return -3;
}

void alrm()
{
//	if (alrm_count > 0) 
		alrm_count--;
//	printf("alrm_count is %d\n", alrm_count);
	 if (alrm_count == 0) {
		timeOutFlag = 1;
		print_resut(TIMEOUT);
		exit(1);
	}
}

int mygetchar(int sec)
{
	int c = -3;
	struct sigaction sa_alarm;
	struct itimerval itimer;
	time_t timer;

	alrm_count = sec;

	memset(&sa_alarm, 0, sizeof(sa_alarm));
	sa_alarm.sa_handler = alrm;
	sa_alarm.sa_flags = SA_RESTART;

	if (sigaction(SIGALRM, &sa_alarm, NULL) < 0) {
		perror("sigaction");
		exit(1);
	}

	itimer.it_value.tv_sec = itimer.it_interval.tv_sec = 1;
	itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
		perror("setitimer");
		exit(1);
	}
	
	time(&timer);
	printf("current time is %s\n", ctime(&timer));
	c = getchar();

	itimer.it_value.tv_sec = itimer.it_interval.tv_sec = 0;
	itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
		perror("setitimer");
		exit(1);
	}
	
	return get_resut(c);
}

int main(int argc, char *argv[])
{
	int c, sec;
	char *e;
	
	sec = strtol(argv[1], &e, 10);
	if (*e != '\0') {
		perror("strtol");
	}

	c = mygetchar(sec);
	
//	printf("input is \'%c\'\n", c);
	print_resut(c);
	
	return 0;
}
