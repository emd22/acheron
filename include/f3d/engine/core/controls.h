#ifndef SB_CONTROLS_H
#define SB_CONTROLS_H

#include <stdbool.h>

#define SB_CONTROLS_MAX 322

#define SB_CONTROLS_MOUSE_CAPTURED 0x01

typedef enum {
    SB_CONTROL_MODE_NORMAL,
    SB_CONTROL_MODE_TOGGLE,
} sb_control_mode_t;

typedef struct {
    int mode;
    bool pressed;
    int modifier;

    float cooldown;
} sb_control_t;

typedef struct {
    int flags;
    sb_control_t controls[CONTROLS_SIZE];
} sb_controls_t;

void controls_init(void);
void controls_update(void);
bool controls_check_toggle(int key);
void controls_handle_keydown(int key);
void controls_handle_keyup(int key);

#endif
