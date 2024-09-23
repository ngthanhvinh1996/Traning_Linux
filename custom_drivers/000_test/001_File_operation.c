#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int fd;
	const char write_text[] = "Hello Linux!!!\n";
	char read_text[20U];
	ssize_t read_bytes= 0U;
	ssize_t write_bytes = 0U;

	fd = open("Example_file.txt", O_CREAT | O_RDWR, 0777);
	if(-1 == fd)
	{
		printf("Open file to write failed\n");
		return -1;
	}
	printf("Size: %ld\n", sizeof(write_text));

	write_bytes = write(fd, write_text, sizeof(write_text));
	if(-1 == write_bytes)
	{
		printf("Write file failed\n");
		close(fd);
		return -1;
	}

	printf("Write bytes: %zd\n", write_bytes);

	if(-1 == close(fd))
	{
		printf("Close file after write failed\n");
		return -1;
	}
	
	fd = -1U;
	fd = open("Example_file.txt", O_RDONLY);
	if(-1U == fd)
	{
		printf("Open file to read failed\n");
		return -1;
	}

	read_bytes = read(fd, read_text, sizeof(read_text));
	if(-1 == read_bytes)
	{
		printf("Read file failed\n");
		close(fd);
		return -1;
	}

	printf("Read bytes: %zd\n", read_bytes);

	read_text[read_bytes++] = "\0";

	printf("Read buff: %s\n", read_text);

	if(-1 == close(fd))
	{
		printf("Close file after read failed\n");
		return -1;
	}

	return 0;

}

