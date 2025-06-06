#ifndef SID_H
#define SID_H

#include <stdint.h>

void init_sid(void);
void sid_send_note(uint8_t addr, uint8_t val);
void play_note(int note);
void stop_note(int note);

#endif
