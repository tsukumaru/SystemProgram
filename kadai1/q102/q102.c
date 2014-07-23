#include<stdio.h>

void error_func(){
	int i, *a;
	
	i = 5;
	*a = i;
}

int main(){
	error_func();
	printf("all OK\n");

	return 0;
}
