#ifndef HANDLER_H
#define HANDLER_H

#include <signal.h>
/**
 * Handles child processes when they are in background and are finished so they're don't become zombies
 */
void sigchld_handler(int sig);

/**
 * Sets up the basic for the signal-handling that will be used for handling child processes signals
 */
void sig_setup(void);

#endif