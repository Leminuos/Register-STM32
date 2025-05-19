#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdio.h>
#include "oled.h"
#include "button.h"
#include "lcd_generic.h"

#define STATUS_COLLISION_SNAKE      0x01
#define STATUS_NO_COLLISION_SNAKE   0x00

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
    uint8_t     dir;
    Pix_Color   back;
    Pix_Color   front;
    Pix_Color   target;
} SnakePixel;

extern void delay(uint16_t mDelay);
extern const FontTypedef Font_14x15;
extern const unsigned char garfield_128x64[];
extern const unsigned char github_logo_64x64[];
extern void SnakeRun(void);
extern void SnakeInit(SnakePixel* Inf);
extern void SnakeUpdateDirection(uint8_t dir);

#endif /*__SCREEN_H__*/

