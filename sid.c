
#include "render.h"
#include "sid.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static int sid_serial_fd = -1;
static int current_note = -1;

void init_sid(void) {
    sid_serial_fd = open("/dev/cu.usbmodem6666101", O_WRONLY | O_NOCTTY);
    if (sid_serial_fd < 0) {
        perror("Failed to open SID serial port");
    } else {
        render_log("SID serial opened OK.\n");
        tcflush(sid_serial_fd, TCIOFLUSH);
    }
}

void sid_send_note(uint8_t addr, uint8_t val) {
    if (sid_serial_fd < 0) {
        render_log("ERROR: Serial not open");
        return;
    }

    char msg[64];
    snprintf(msg, sizeof(msg), ">[%02X] [%02X]", addr, val);
    render_log("%s", msg);

    uint8_t buf[2] = { addr, val };
    ssize_t written = write(sid_serial_fd, buf, 2);

    if (written != 2) {
        //render_log("ERROR: Failed to write (only %ld bytes)", written);
    }
}
void play_note(int note) {
    if (note != current_note) {
        current_note = note;
        //render_log("Play note: %d\n", note);

        static const uint16_t sid_freqs[] = {
            0x0116, 0x012C, 0x0145, 0x015F, 0x017B, 0x0199, 0x01B9, 0x01DB,
            0x01FF, 0x0223, 0x024B, 0x0274, 0x02A0, 0x02CE, 0x02FD, 0x0330,
            0x0368, 0x03A0, 0x03DB, 0x0419, 0x0459, 0x049C, 0x04E2, 0x052B,
            0x0576, 0x05C4, 0x0616, 0x066B, 0x06C2, 0x071E, 0x077C, 0x07DD,
            0x0844, 0x08AD, 0x091A, 0x098B, 0x0A00, 0x0A78, 0x0AF3, 0x0B72,
            0x0BF4, 0x0C7A, 0x0D03, 0x0D90, 0x0E21, 0x0EB5, 0x0F4D, 0x0FE9,
            0x1089, 0x112C, 0x11D4, 0x1280, 0x1330, 0x13E4, 0x149C, 0x1559,
            0x161A, 0x16DF, 0x17A9, 0x1877, 0x194A, 0x1A21, 0x1AFE, 0x1BDF
        };

        uint16_t freq = (note < (int)(sizeof(sid_freqs) / sizeof(uint16_t)))
                        ? sid_freqs[note]
                        : sid_freqs[0];

        sid_send_note(0x00, freq & 0xFF);        // FREQ LO
        sid_send_note(0x01, (freq >> 8) & 0xFF); // FREQ HI
        sid_send_note(0x05, 0x00);               // ATTACK/DECAY
        sid_send_note(0x06, 0xF0);               // SUSTAIN/RELEASE
        sid_send_note(0x04, 0x11);               // GATE + TRIANGLE
    }
}

void stop_note(int note) {
    if (note == current_note) {
       // render_log("Stop note: %d\n", note);
        sid_send_note(0x04, 0x00); // Gate off
        current_note = -1;
    }
}
