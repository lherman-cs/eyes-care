#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "common.h"

void convert_to_lower(char *sentence)
{
    size_t len = strlen(sentence);
    for (int i = 0; i < len; i++)
        sentence[i] = tolower(sentence[i]);
}

pid_t get_prog_pid()
{
    FILE *fp = fopen(LOCK_PATH, "r");
    pid_t pid;
    if (fscanf(fp, "%d", &pid) <= 0)
        die(-1, "Can't find the program's pid");
    fclose(fp);

    return pid;
}

static void stop_handler()
{
    pid_t pid = get_prog_pid();
    kill(pid, SIGTERM);
    printf("Stopped %s\n", PROG_NAME);
}

static void pause_handler()
{
    pid_t pid = get_prog_pid();
    kill(pid, SIGINT);

    printf("Paused/resumed %s\n", PROG_NAME);
}

int hash(const char *s)
{
    size_t len = strlen(s);
    int h = 1;
    for (int i = len - 1; i >= 0; i--)
        h <<= h * s[i];

    return h;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./controller [command]\n");
        exit(1);
    }

    if (!is_root())
        die(-1, "You have to be a root user");

    convert_to_lower(argv[1]);

    int command_hash = hash(argv[1]);
    if (command_hash == hash("stop"))
        stop_handler();
    else if (command_hash == hash("pause"))
        pause_handler();

    return 0;
}