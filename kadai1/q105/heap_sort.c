#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_COUNT 1000
int sample[SAMPLE_COUNT]; 
int cmp = 0, swp = 0;

/*出力*/
void print_data(int a[], int n){
	int i;

	for(i = 0; i < n; i++)
		printf("%2d ", a[i]);
	printf("\n");
}

/* 要素の大小を比較して、対応した値を返す*/
int compare(int i, int j)
{
	cmp++;
	if (i > j)
		return 1;
	else if (i == j)
		return 0;
	else
		return -1;
}

/* 配列 a の i 番目と j 番目の要素を入れ換える */
void swap(int a[], int i, int j)
{
	int tmp;

	swp++;
	tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

/* 配列 a の i 番目の要素に値 v を代入する */
void assign(int a[], int i, int v)
{
	a[i] = v;
}

static void sift(int a[], int root, int leaf)
{
	int i, j, w;

	i = root;
	j = 2 * root + 1;
	w = a[root];
	
	while(compare(j, leaf) <= 0){
		if(compare(j, leaf) < 0 && compare(a[j], a[j+1]) < 0)
			 j++;
		if(compare(w, a[j]) >= 0)
			 break;
		assign(a, i, a[j]);
		i = j;
		j = 2 * i + 1;
	}
	assign(a, i, w);

//	printf("半順序木：");
//	print_data(a, SAMPLE_COUNT);
}

void heap_sort(int a[], int n)
{
	int root, leaf;

	leaf = n - 1;
	root = (n-1) / 2; //leafに対する根
	
	//半順序木の作成
	while(compare(root, 0) >= 0){
		sift(a, root, leaf);
		--root;
	}
//	printf("〜半順序木完成〜\n");	
	  
	//ソート
//	printf("〜ヒープソート〜\n");
	while(compare(leaf, 0) > 0){
//		printf("compare(leaf, 0) == 1...swap!\n");
//		printf("swap(0, leaf)\n");
		swap(a, 0, leaf);
		leaf--;
		sift(a, 0, leaf);
	}
//	printf("end!\n");
}

void init_array(){
	int i;

	srand(10);
	for(i = 0; i < SAMPLE_COUNT; i++){
		sample[i] = rand() % 100;
	}
}

int main(){
	init_array();
//	printf("ソート前：");
//	print_data(sample, SAMPLE_COUNT);
	
	heap_sort(sample, SAMPLE_COUNT);
	
//	printf("ソート後：");
//	print_data(sample, SAMPLE_COUNT);
	printf("比較回数：%d\n", cmp);
	printf("交換回数：%d\n", swp);
}
