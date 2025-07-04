#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <locale.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{              
	int in = open("./OS09_05.txt",O_RDONLY);
 	char buffer[128];
	ssize_t bytesRead;
	
	//первые 30 байт
 	lseek(in, 0, SEEK_SET); //  ставим позицию в начало
 	bytesRead = read(in, buffer, 30);
 	buffer[bytesRead] = '\0';
 	printf("[30 bytes]: %s\n", buffer);

	//следующие 20
 	lseek(in, 20, SEEK_CUR);
 	bytesRead = read(in, buffer, 20);
	buffer[bytesRead] = '\0';
 	printf("[Next 20 bytes]: %s\n", buffer);
	
	close(in);
	return 0;
}