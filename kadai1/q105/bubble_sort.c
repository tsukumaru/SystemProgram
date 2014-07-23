#include<stdio.h>
#include<stdlib.h>

#define SAMPLE_COUNT 1000
int sample[SAMPLE_COUNT];
int cmp = 0, swp = 0;

void print_data(int a[], int count)
{
	int i;

	for(i = 0; i < count; i++)
		printf("%2d ", a[i]);
	printf("\n");
}


void swap_array(int a[], int i, int j)
{
	int tmp;

	swp++;
	tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

int compare(int i, int j){
	cmp++;
	if(i > j)
		return 1;
	else if(i == j)
		return 0;
	else 
		return -1;
}

void sort(int data[], int count)
{
	int i, j;
	int n = count - 1;

	for(i = 0; i < n; i++){
//		printf("%d: ", i);
//		print_data(data, count);

		for(j = 0; j < n-i; j++){
//			printf("\t[%d]=%d > [%d]=%d",
//					j, data[j], j + 1, data[j + 1]);

			if(compare(data[j], data[j + 1]) == 1){
//				printf(" ... swap!!");

				swap_array(data, j, j + 1);
			}

//			printf("\n");
		}
	}
}

void init_array(){
	int i;

	srand(10);
	for(i = 0; i < SAMPLE_COUNT; i++){
		sample[i] = rand() % 100;
	}
}

int main()
{
	init_array();
//	printf("ソート前：");
//	print_data(sample, SAMPLE_COUNT);

	sort(sample, SAMPLE_COUNT);
	
//	printf("ソート後：");
//	print_data(sample, SAMPLE_COUNT);
	printf("比較回数：%d\n", cmp);
	printf("交換回数：%d\n", swp);

	return 0;
}
