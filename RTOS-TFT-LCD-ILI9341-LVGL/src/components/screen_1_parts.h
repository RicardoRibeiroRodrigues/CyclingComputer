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
static lv_obj_t *labelViagemText;
// Div de distancia
lv_obj_t *labelDistValue;
static lv_obj_t *labelDistUnit;
static lv_obj_t *labelDistTitle;





void create_speed_section(lv_obj_t * screen, const lv_font_t *MontAltEL58);
void create_viagem_section(lv_obj_t *screen, const lv_font_t *MontAltEL20);


#endif /* SCREEN_1_PARTS_H_ */