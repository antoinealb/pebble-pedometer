#ifndef MESSAGES_H
#define MESSAGES_H

/* All possible message types between worker and application. */
enum {
    MESSAGE_STEP_COUNT=0,
    MESSAGE_STEP_RESET=1,
};

#endif
