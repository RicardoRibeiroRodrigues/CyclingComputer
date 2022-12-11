/*
 * screen_2_parts.h
 *
 * Created: 10/12/2022 12:20:48
 *  Author: guifl
 */ 


#ifndef SCREEN_2_PARTS_H_
#define SCREEN_2_PARTS_H_

#include "lvgl.h"
#include "asf.h"

// se��o de peso
lv_obj_t *labelPesoValue;
static lv_obj_t *labelPesoUnit;
static lv_obj_t *labelPesoText;
// se��o de aro
static lv_obj_t *labelPolegadaText;
static lv_obj_t *labelmmText;
static lv_obj_t *labelAroText;
lv_obj_t *labelAroValue;
// se��o de horario
static lv_obj_t *labelHoraText;
static lv_obj_t *labelMinText;
static lv_obj_t *labelMinTitle;
static lv_obj_t *labelSegundoText;
static lv_obj_t *labelSegundoTitle;
static lv_obj_t *labelHorarioText;
lv_obj_t *labelHoraValue;
lv_obj_t *labelMinValue;
lv_obj_t *labelSegundoValue;


static lv_obj_t *img_wheel;

typedef struct{
	const lv_img_dsc_t *horario;
	const lv_img_dsc_t *minus_button;
	const lv_img_dsc_t *plus_button;
} horario_imgs;

void create_horario_section(lv_obj_t * screen, const lv_font_t *MontAltEL20, const horario_imgs imgs);
void create_peso_section(lv_obj_t * screen, const lv_font_t *MontAltEL20, const lv_img_dsc_t *img_peso);
void create_aro_section(lv_obj_t *screen, const lv_font_t *MontAltEL20, lv_img_dsc_t *img_rodalogo);



#endif /* SCREEN_2_PARTS_H_ */