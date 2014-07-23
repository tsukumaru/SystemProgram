#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

volatile sig_atomic_t alrm_count;

void alrm()
{
	alrm_count--;
}

int mygetchar()
{
	int c, endval;
	
	while (alrm_count > 0) { 
//		pause();
		signal(SIGINT, &alrm);
		c = getchar();
		printf("%d: %ld\n", alrm_count, time(NULL));
		
		if (c > 0) return c;
		else if (c == EOF) return -1; 
	}
	
	return -2;
}

int main(int argc, char *argv[])
{
	int c;
	char *e;
	struct sigaction sa_alarm;
	struct itimerval itimer;
	
	alrm_count = strtol(argv[1], &e, 10);
	if (*e != '\0') {
		perror("strtol");
	}

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

	c = mygetchar();

	itimer.it_value.tv_sec = itimer.it_interval.tv_sec = 0;
	itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
		perror("setitimer");
		exit(1);
	}

	return 0;
}
