#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int len;

int file_open_check(FILE *fp){
	if(fp == NULL){
		printf("file open error\n");
		return -1;
	}
	return 0;
}

void set_string(char *s, FILE *fp){
	int c, i;
	char a[] = "aiueo";
	
//	t = s; //sの先頭アドレスを確保
	while((c = fgetc(fp)) != EOF){
		*s = (char)c;
//		printf("%c", *s);	
		s++;
	}
	
	for(i=0; i < 10000; i++){
		s += strlcat(s, a, 5);
	}
//	printf("%s\n", s);
//	s = t; //sに先頭アドレスを渡す
//	printf("%s", s);
//	return s;
}

int my_strlen2(char *s){
	int len = 0;
	
	while(*s != 0){
		len++;
		s++;
	}
	return len;	
}

int my_strlen1(char *s){
	int i, len = 0;

	for(i = 0; ; i++){
		if(*s == 0) break;
		s = s + 1;
		len = len + 1;
	}
	return len;
}

double strlen_time(char *s, int key){
	clock_t start, end;
	int i;
	switch(key){
		case 0:
			start = clock();
			for(i = 0; i < 300; i++)
				len = strlen(s);
//			printf("words length = %d\n", len);
			end = clock();
			break;
		case 1:
			start = clock();
			for(i = 0; i < 300; i++)
				len = my_strlen1(s);
//			printf("words length = %d\n", len);
			end = clock();
			break;
		case 2:
			start = clock();
			for(i = 0; i < 300; i++)
				len = my_strlen2(s);
//			printf("words length = %d\n", len);
			end = clock();
			break;
		default: return -1;
	}
	return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(){
	int i;
	double time[3];
	char *s;
	FILE *fp;
	
	s = (char *)malloc(sizeof(char) * 10000000);
	if (s == NULL) {
		perror("malloc");
		exit(1);
	}

	fp = fopen("/usr/share/dict/words", "r");
	if(file_open_check(fp) == -1) return -1;
	
	set_string(s, fp);
//	printf("%s", s);
	for(i = 0; i < 3; i++){
		time[i] = strlen_time(s, i);
		printf("time of my_strlen%d is %f\n", i, time[i]);
	}

	free(s);
	return 0;
}
