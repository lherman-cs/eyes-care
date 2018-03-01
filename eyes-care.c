#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include "common.h"

#define DELAY_1 1200 // 20 min
#define BREAK_1 20   // 20 sec

static volatile int paused = 0;

static void pause_handler(int dummy);
static void break_handler(int dummy);
static void terminate_handler(int dummy);
static unsigned get_screen_brightness();
static void set_screen_brightness(unsigned level);
int lock_exist();

int main()
{
    if (!is_root())
        die(-1, "You have to be a root user");

    signal(SIGINT, pause_handler);
    signal(SIGALRM, break_handler);
    signal(SIGTERM, terminate_handler);

    if (lock_exist())
        die(-1, "Lock exists. You might have run the program in the background.");

    FILE *fp = fopen(LOCK_PATH, "w");
    (void)fprintf(fp, "%d", getpid());
    fclose(fp);

    fprintf(stderr, "%s is running\n", PROG_NAME);
    for (;;)
    {
        while (!paused)
        {
            alarm(DELAY_1);
            pause();
        }
        pause();
    }

    return 0;
}

static void pause_handler(int dummy)
{
    // Do something when the pause signal is called
    paused ^= 1; // Flip the switch

    if (paused)
    {
        fprintf(stderr, "Paused\n");
        alarm(0);
    }
    else
        fprintf(stderr, "Resumed\n");
}

static void break_handler(int dummy)
{
    // Do something when it's break time
    fprintf(stderr, "Taking a break for %d secs\n", BREAK_1);
    unsigned cur_level = get_screen_brightness();
    set_screen_brightness(0);
    sleep(BREAK_1);
    set_screen_brightness(cur_level);
}

static void terminate_handler(int dummy)
{
    // Do some cleanups
    fprintf(stderr, "Deleting the lock at %s\n", LOCK_PATH);
    unlink(LOCK_PATH);
    exit(0);
}

static unsigned get_screen_brightness()
{
    unsigned level;
    FILE *fp = fopen("/sys/class/backlight/intel_backlight/brightness", "r");
    if (fscanf(fp, "%u", &level) <= 0)
        die(-1, "Can't get the current brightness");
    fclose(fp);
    return level;
}

static void set_screen_brightness(unsigned level)
{
    FILE *fp = fopen("/sys/class/backlight/intel_backlight/brightness", "w");
    if (fp == NULL)
        die(errno, "Failed to set brightness");
    fprintf(fp, "%u", level);
    fclose(fp);
}

int lock_exist()
{
    return access(LOCK_PATH, F_OK) != -1;
}
