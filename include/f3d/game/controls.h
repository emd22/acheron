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
    
    CONTROL_0,
    CONTROL_1,
    CONTROL_2,
    CONTROL_3,
    CONTROL_4,
    CONTROL_5,
    CONTROL_6,
    CONTROL_7,
    CONTROL_8,
    CONTROL_9,
    
    CONTROLS_SIZE
};

extern int keys_pressed[CONTROLS_SIZE];

void controls_init(void);
void controls_handle_keydown(int key);
void controls_handle_keyup(int key);

#endif
