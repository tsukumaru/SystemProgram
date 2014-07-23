#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

char * getString(char *c, int num, int size){
	int count;
	
	c += size;
	//num個の改行コードを見つけるまでループをまわす
	count = 0;
	while(count <= num){
		if(*c == '\n'){
			count++;
		}
		c--;
	}
	c += 2;
//	printf("%s", c);
	return c;
}

char * my_tail(int fd, int num){
	int size;
	char *c;
	struct stat fs;

	if(fstat(fd, &fs) < 0){
		perror("fstat");
		exit(-1);
	}
	
	size = fs.st_size;
	c = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(c == MAP_FAILED){
		perror("mmap");
		exit(-1);
	}
		
	return getString(c, num, size);
}

int main(int argc, char *argv[]){
	int fd, line_num;
	char *e, *s;

	if(argc != 3){
		printf("Usage: %s filename line_num\n", argv[0]);
		exit(1);
	}
	
	fd = open(argv[2], O_RDONLY);
	if(fd < 0){
		perror(argv[1]);
		exit(-1);
	}

	line_num = strtol(argv[1], &e, 10);
	if(*e != '\0'){
		printf("error at \'*e\'\n");
	}
//	printf("%d\n", line_num);
	s = my_tail(fd, line_num);
	printf("%s\n", s);

	return 0;
}
