#ifndef COMMON_H
#define COMMON_H

#define PROG_NAME "eyes-care"
#define LOCK_PATH "/tmp/eyes-care.lock"

int is_root();
void die(int err, const char *msg);

#endif