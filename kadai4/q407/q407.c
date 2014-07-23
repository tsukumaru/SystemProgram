#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pipe_fd[2];

/*サンプルを双方向にして、childとparentでwriteとreadをして出力のタイミングをはかる*/
void do_child(){
	int count;
	char c;
	char *p = "0123456789\n";

	printf("this is child\n");

	close(pipe_fd[0]);

	printf("write start\n");
	if(write(pipe_fd[1], p, 1) < 0){
		perror("child_write");
		exit(1);
	}

	close(1);
	if(dup2(pipe_fd[1], 0) < 0){
		perror("parent_dup2");
		close(pipe_fd[1]);
		exit(1);
	}
	count = read(pipe_fd[0], &c, 1);
	putchar(c);

	if(count < 0){
		perror("child_read");
		exit(1);
	}


	//	putchar(*p);
	//	p++;
	printf("child end\n");
}

void do_parent(int num){
	char *p = "ABCDEFGHIJKLMNOPQRSU\n";
	char c;
	int count, status;

	close(pipe_fd[1]);

	printf("this is parent\n");

	printf("read start\n");
	count = read(pipe_fd[0], &c, 1);
	putchar(c);

	close(0);
	if(dup2(pipe_fd[0], 1) < 0){
		perror("parent_dup2");
		close(pipe_fd[0]);
		exit(1);
	}

	if(count < 0){
		perror("parent_read");
		exit(1);
	}

	if(write(pipe_fd[1], p, 1) < 0){
		perror("parent_write");
		exit(1);
	}

	printf("wait now\n");
	if(wait(&status) < 0){
		perror("wait");
		exit(1);
	}

}

int main(int argc, char *argv[]){
	int i, child, num;
	char *e;

	if(argc != 2){
		printf("Usage: %s num\n", argv[0]);
		exit(1);
	}

	if(pipe(pipe_fd) < 0){
		perror("pipe");
		exit(1);
	}

	if((child = fork()) < 0){
		perror("fork");
		exit(1);
	}

	num = strtol(argv[1], &e, 10);
	if(*e != '\0'){
		printf("error at \'*e\'\n");
	}

	//	for(i = 0; i < num; i++){
	//		printf("%d周目\n", i);
	if(child == 0)
		do_child();
	else
		do_parent(num);
	//	}

	return 0;
}
