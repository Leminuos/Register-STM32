#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdio.h>
#include "oled.h"
#include "button.h"
#include "lcd_generic.h"

#define STATUS_COLLISION_SNAKE      0x01
#define STATUS_NO_COLLISION_SNAKE   0x00

#define BUTTON_KEY_UP               0x00
#define BUTTON_KEY_RIGHT            0x01
#define BUTTON_KEY_DOWN             0x02
#define BUTTON_KEY_LEFT             0x03

#define SNAKE_START_STATE           0x01
#define SNAKE_RUN_STATE             0x02
#define SNAKE_STOP_STATE            0x03

typedef struct {
    uint8_t color[2];
} Pix_Color;

typedef struct {
    uint8_t     horizontal;
    uint8_t     vertical;
} Axes;

typedef struct _SnakeChain {
    Axes axes;
    struct _SnakeChain *prev;
} SnakeChain;

typedef struct {
    Axes        axes;
    uint8_t     size;
    uint8_t     state;
    uint8_t     dir;
    uint8_t     old_dir;
    Pix_Color   back;
    Pix_Color   front;
    Pix_Color   target;
} SnakePixel;

extern void delay(uint16_t mDelay);
extern const FontTypedef Font_14x15;
extern const unsigned char garfield_128x64[];
extern const unsigned char github_logo_64x64[];
extern void SnakeRun(void);
extern uint8_t GetStateSnake(void);
extern void SetStateSnake(uint8_t state);
extern uint32_t SnakeOperation(void);
extern void SnakeInit(SnakePixel* Inf);
extern void SnakeUpdateDirection(uint8_t dir);

#endif /*__SCREEN_H__*/

