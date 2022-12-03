/*
 * screen_1_parts.h
 *
 * Created: 11/24/2022 8:34:41 PM
 *  Author: riywa
 */ 


#ifndef SCREEN_1_PARTS_H_
#define SCREEN_1_PARTS_H_

#include "lvgl.h"

// Seção de velocidade
lv_obj_t *labelSpeedValue;
static lv_obj_t *labelSpeedUnit;
// Viagem
static lv_obj_t *labelViagemText;
lv_obj_t *labelViagemClock;
// Div de distancia
lv_obj_t *labelDistValue;
static lv_obj_t *labelDistUnit;
static lv_obj_t *labelDistTitle;
// Div de vel media.
lv_obj_t *labelVelMValue;
static lv_obj_t *labelVelMUnit;
static lv_obj_t *labelVelMTitle;




void create_speed_section(lv_obj_t * screen, const lv_font_t *MontAltEL58);
void create_viagem_section(lv_obj_t *screen, const lv_font_t *MontAltEL20);


#endif /* SCREEN_1_PARTS_H_ */