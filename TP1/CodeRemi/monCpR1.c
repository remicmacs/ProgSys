#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

#define NMAX 10

int main(int argc, char** argv){
	
	if(argc < 3){
		write(1,"command monCp expect 2 arguments : 'monCp <sourceFile> <destFile> [<bufferSize|1>]'\n",84);
		exit(1);
	}

	int chunk = -1, timer = 0;
	struct timeval stop, start;
	

	if(argc == 4){
		if(*argv[3] == 't'){
			gettimeofday(&start, NULL);
			timer = 1;
			chunk = 1;
		}
		else{
			chunk = ((int) *argv[3]) - 48;
			if(chunk <= 0)
				chunk = 1;
			timer = 0;
		}
	}
	else if(argc == 5){
		if(*argv[4] == 't'){
			gettimeofday(&start, NULL);
			timer = 1;
			chunk = 1;
		}
		else{
			chunk = ((int) *argv[3]) - 48;
			if(chunk <= 0)
				chunk = 1;
			timer = 0;
		}
	}
	else{
		chunk = 1;
	}

	char *content = malloc(sizeof(char)*chunk);

	int source, dest, contentLength;

	source = open(argv[1],O_RDONLY);
	if(source < 0){
		perror("An error has occured while openning sourceFile");
		exit(1);
	}

	dest = open(argv[2],O_WRONLY);
	if(dest < 0){
		write(1,"destFile provided does not exists, creating it\n",47);
		dest = open(argv[2],O_CREAT,S_IRWXU);
		if(dest < 0){
			perror("An error has occured while creating destFile");
			exit(1);
		}
		close(dest);
		dest = open(argv[2],O_WRONLY);
	}

	
	do{
		contentLength = read(source,content,chunk);
		if(contentLength < 0){
			perror("An error has occured while reading in sourceFile");
			exit(1);
		}
		if(write(dest,content,contentLength) != contentLength){
			perror("An error has occured while writing in destFile");
			exit(1);
		}
	}while(contentLength == chunk);

	close(source);
	close(dest);

	if(timer == 1){
		gettimeofday(&stop, NULL);
		long int time_took = stop.tv_usec - start.tv_usec;
		printf("time : %lu microseconds\n",time_took);
	}
	
	return 0;
}
