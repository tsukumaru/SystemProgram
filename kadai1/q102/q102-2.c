#include<stdio.h>
#include<stdlib.h> //変更箇所

void bus_error(){
	int i;
	int *a = (int *)malloc(sizeof(int)); //変更箇所
	
	i = 5;
	*a = i;
	free(a);
}

int main(){
	bus_error();
	printf("all OK\n");

	return 0;
}
