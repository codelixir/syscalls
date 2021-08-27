#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int const STD_IN = 0;
int const STD_OUT = 1;
int const STD_ERR = 2;

int write_out(char *);
int write_err(char *);
char *last_name(char *);
void reverse_string(char *, int, char *);
void show_loading(float);
void quit();

int const CHUNK_SIZE = 1 << 16;

int main(int argc, char *argv[])
{

	// Handling Number of Arguments
	if (argc < 2)
	{
		write_err("Missing argument: inputfile.\n");
		quit();
	}
	else if (argc > 2)
	{
		write_err("Too many arguments. Ignoring additional arguments.\n");
		// program will still execute in this case, considering the first argument
	}

	// make directory
	mkdir("Assignment", S_IRUSR | S_IWUSR | S_IXUSR);
	/* will give error if already exists, but that isn't a problem
	since we have to use the existing directory in that case */

	// input file
	char *in_file = argv[1];
	char *filename = last_name(in_file);

	// output file
	char *out_file = malloc(sizeof(char) * (12 + strlen(filename)));
	sprintf(out_file, "Assignment/1_%s", filename);

	// file descriptors
	int in_fd = open(in_file, O_RDONLY | O_APPEND, 0);
	int out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	// Handling invalid path
	if (in_fd < 0)
	{
		perror("Input File Error");
		quit();
	}
	if (out_fd < 0)
	{
		perror("Output File Error");
		quit();
	}

	char *buffer = malloc(sizeof(char) * CHUNK_SIZE);
	char *reffub = malloc(sizeof(char) * CHUNK_SIZE);

	long long int file_end = lseek(in_fd, 0, SEEK_END);
	long long int file_read = 0;
	long long int offset = file_end;
	long long int offtemp = lseek(in_fd, -1 * CHUNK_SIZE, SEEK_END);

	do
	{
		int chunk_read = read(in_fd, buffer, CHUNK_SIZE);
		file_read += chunk_read;
		offset = lseek(in_fd, -1 * chunk_read, SEEK_CUR);
		offtemp = lseek(in_fd, -1 * CHUNK_SIZE, SEEK_CUR);
		reverse_string(buffer, chunk_read, reffub);
		write(out_fd, reffub, chunk_read);
		show_loading((double)file_read / file_end);
	} while (offtemp > 0);

	lseek(in_fd, 0, SEEK_SET);
	int chunk_read = read(in_fd, buffer, offset);
	reverse_string(buffer, chunk_read, reffub);
	write(out_fd, reffub, chunk_read);
	show_loading(1);
	write_out("\n");

	/*test*/
	/*for (float i=0; i<=1.1; i+=0.0001) {
		show_loading(i);
		sleep(0.01);
	}*/

	close(in_fd);
	close(out_fd);

	free(out_file);
	free(buffer);
	free(reffub);

	return 0;
}

// write to stdout
int write_out(char *buf)
{
	int cnt = strlen(buf);
	return write(STD_OUT, buf, cnt);
}

// write to stderr
int write_err(char *buf)
{
	int cnt = strlen(buf);
	return write(STD_ERR, buf, cnt);
}

// exit with a message and code 1
void quit()
{
	write_out("Terminating program...\n");
	exit(1);
}

// get the substring after '/' (in case of abs path)
char *last_name(char *path)
{
	int idx = -1;
	int n = strlen(path);

	for (int i = n - 1; i >= 0; i--)
	{
		if (path[i] == '/')
		{
			idx = i;
			break;
		}
	}

	if (idx == -1)
		return path;

	char *filename = malloc(sizeof(char) * (n - idx));

	for (int i = 0; i < n - idx; i++)
	{
		filename[i] = path[idx + i + 1];
	}

	return (char *)filename;
}

// reverse a string and add a null character at the end
void reverse_string(char *str, int lim, char *rts)
{
	for (int i = lim - 1; i >= 0; i--)
	{
		rts[i] = str[lim - 1 - i];
	}
	if (lim < CHUNK_SIZE)
	{
		rts[lim] = '\0';
	}
}

// show % progress
void show_loading(float progress)
{
	char loading[32];
	if (progress > 0.9999)
		sprintf(loading, "\r####################   100%%");
	else
	{
		float pc = 100 * progress;
		sprintf(loading, "\r.................... %.2f%%", pc);
		for (int i = 1; i <= 20; i++)
		{
			if (pc > 5)
				loading[i] = '#';
			pc -= 5;
		}
	}

	write_out(loading);
}
