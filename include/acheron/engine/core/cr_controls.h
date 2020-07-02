#ifndef AR_CONTROLS_H
#define AR_CONTROLS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define AR_CONTROLS_MAX 322

// https://sdl.beuc.net/sdl.wiki/SDL_GetModState

#define AR_CONTROL_MOD_LSHIFT 0x0001
#define AR_CONTROL_MOD_RSHIFT 0x0002
#define AR_CONTROL_MOD_LCTRL  0x0040
#define AR_CONTROL_MOD_RCTRL  0x0080
#define AR_CONTROL_MOD_LALT   0x0100
#define AR_CONTROL_MOD_RALT   0x0200
#define AR_CONTROL_MOD_LMETA  0x0400
#define AR_CONTROL_MOD_RMETA  0x0800
#define AR_CONTROL_MOD_NUM    0x1000
#define AR_CONTROL_MOD_CAPS   0x2000
#define AR_CONTROL_MOD_MODE   0x4000

#define AR_CONTROL_MOD_SHIFT (AR_CONTROL_MOD_LSHIFT | AR_CONTROL_MOD_RSHIFT)
#define AR_CONTROL_MOD_CTRL  (AR_CONTROL_MOD_LCTRL  | AR_CONTROL_MOD_RCTRL)
#define AR_CONTROL_MOD_ALT   (AR_CONTROL_MOD_LALT   | AR_CONTROL_MOD_RALT)
#define AR_CONTROL_MOD_META  (AR_CONTROL_MOD_LMETA  | AR_CONTROL_MOD_RMETA)

typedef enum {
    AR_CONTROL_MODE_NORMAL,
    AR_CONTROL_MODE_TOGGLE,
} ar_control_mode_t;

typedef struct {
    int mode;
    unsigned short modifiers;
    bool pressed;

    float cooldown;
} ar_control_t;

typedef struct {
    int x, y;
    int relx, rely;
} ar_control_mouse_event_t;

typedef struct {
    ar_control_t controls[AR_CONTROLS_MAX];
} ar_controls_t;

void ar_controls_init(void);
void ar_controls_poll_events(void);
ar_control_t *ar_control_get(int controln);
int  ar_controls_get_modifiers(void);
void ar_control_set_mode(int controln, ar_control_mode_t mode);
bool ar_control_check(int controln);

#endif
