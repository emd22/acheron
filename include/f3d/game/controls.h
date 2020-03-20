#ifndef CONTROLS_H
#define CONTROLS_H

#define CONTROLS_MOUSE_CAPTURED 0x01

typedef struct {
    unsigned flags;
} controls_info_t;

enum {
    CONTROL_FORWARD,
    CONTROL_BACKWARD,
    CONTROL_LEFT,
    CONTROL_RIGHT,
    
    CONTROLS_SIZE
};

extern int keys_pressed[CONTROLS_SIZE];

void controls_init(void);
void controls_handle_keydown(int key);
void controls_handle_keyup(int key);

#endif
