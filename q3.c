#include <errno.h>
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
int get_mode(int);
void show_perms(int, char *);
void quit();

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

    // input file
    char *in_file = argv[1];
    char *filename = last_name(in_file);

    // output files
    char *file_1 = malloc(sizeof(char) * (12 + strlen(filename)));
    sprintf(file_1, "Assignment/1_%s", filename);
    char *file_2 = malloc(sizeof(char) * (12 + strlen(filename)));
    sprintf(file_2, "Assignment/2_%s", filename);

    // file descriptors
    errno = 0;
    int dir_fd = open("Assignment", O_RDONLY | O_DIRECTORY, 0);
    if (dir_fd < 0 || errno != 0)
    {
        write_err("Directory is created: No\n");
        quit();
    }
    int fd_1 = open(file_1, O_RDONLY, 0);
    int fd_2 = open(file_2, O_RDONLY, 0);

    /* code */

    int mode_d = get_mode(dir_fd), mode_1 = get_mode(fd_1), mode_2 = get_mode(fd_2);

    if (mode_d > 0)
        write_out("Directory is created: Yes\n\n");
    show_perms(mode_d, "Assignment");
    show_perms(mode_1, file_1);
    show_perms(mode_2, file_2);

    close(fd_1);
    close(fd_2);
    close(dir_fd);

    free(file_1);
    free(file_2);

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

int get_mode(int fd)
{
    if (fd < 0)
        return -1;

    struct stat fileStat;

    if (fstat(fd, &fileStat) < 0)
        return -1;

    return (int)fileStat.st_mode;
}

void show_perms(int mode, char *name)
{
    if (mode < 0)
    {
        write_err("Error opening file/directory.\n\n");
        return;
    }

    char *ugo[] = {"User", "Group", "Others"};
    char *rwx[] = {"read", "write", "execute"};

    char *output = malloc(sizeof(char) * (40 + strlen(name)));
    char *yn = malloc(sizeof(char) * 3);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (mode & (1 << (8 - 3 * i - j)))
                yn = "Yes";
            else
                yn = "No";
            sprintf(output, "%s has %s permission on %s: %s\n", ugo[i], rwx[j], name, yn);
            write_out(output);
        }
        write_out("\n");
    }

    // free(output);
    // free(yn);
}