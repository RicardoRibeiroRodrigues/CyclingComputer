/*
 * screen_2_parts.c
 *
 * Created: 10/12/2022 12:20:10
 *  Author: guifl
 */ 
#include "screen_2_parts.h"

LV_FONT_DECLARE(MontAltEL80);


void create_aro_section(lv_obj_t * screen, const lv_font_t *MontAltEL20, lv_img_dsc_t *img_rodalogo) {
	static lv_style_t textStyle;
	lv_style_init(&textStyle);
	lv_style_set_border_width(&textStyle, 0);
	lv_style_set_text_color(&textStyle, lv_color_black());
	
	labelAroText = lv_label_create(screen);
	lv_obj_align(labelAroText, LV_ALIGN_LEFT_MID, 40, 25);
	lv_obj_add_style(labelAroText, &textStyle, 0);
	lv_obj_set_style_text_font(labelAroText, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelAroText, "aro");
	
	
	labelAroValue = lv_label_create(screen);
	lv_obj_add_style(labelAroValue, &textStyle, 0);
	lv_obj_align_to(labelAroValue, labelAroText, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
	lv_obj_set_style_text_font(labelAroValue, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelAroValue, "%i",14);
	
	// Roda do logo.
	img_wheel = lv_img_create(screen);
	lv_img_set_src(img_wheel, img_rodalogo);
	lv_img_set_zoom(img_wheel, 1024);
	lv_obj_align_to(img_wheel, labelAroValue, LV_ALIGN_OUT_TOP_MID, 0, -100);
	
}

void create_peso_section(lv_obj_t * screen, const lv_font_t *MontAltEL20, const lv_img_dsc_t *img_peso) {
	static lv_style_t textStyle;
	lv_style_init(&textStyle);
	lv_style_set_border_width(&textStyle, 0);
	lv_style_set_text_color(&textStyle, lv_color_black());
	
	
	labelPesoValue = lv_label_create(screen);
	lv_obj_align(labelPesoValue, LV_ALIGN_TOP_RIGHT, -55, 60);
	lv_obj_add_style(labelPesoValue, &textStyle, 0);
	lv_obj_set_style_text_font(labelPesoValue, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelPesoValue, "%i", 80);
	
	labelPesoText = lv_label_create(screen);
	lv_obj_align(labelPesoText, LV_ALIGN_RIGHT_MID, -45, -120);
	lv_obj_add_style(labelPesoText, &textStyle, 0);
	lv_obj_set_style_text_font(labelPesoText, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelPesoText, "Peso");
	
	labelPesoUnit = lv_label_create(screen);
	lv_obj_add_style(labelPesoUnit, &textStyle, 0);
	lv_obj_align_to(labelPesoUnit, labelPesoValue, LV_ALIGN_OUT_RIGHT_MID, 10, -6);
	lv_obj_set_style_text_font(labelPesoUnit, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelPesoUnit, "kg");
}

void create_horario_section(lv_obj_t *screen, const lv_font_t *MontAltEL20, horario_imgs imgs) {
	static lv_style_t textStyle;
	lv_style_init(&textStyle);
	lv_style_set_border_width(&textStyle, 0);
	lv_style_set_text_color(&textStyle, lv_color_black());
	
	
	labelHorarioText = lv_label_create(screen);
	lv_obj_align(labelHorarioText, LV_ALIGN_RIGHT_MID, -40, -30);
	lv_obj_add_style(labelHorarioText, &textStyle, 0);
	lv_obj_set_style_text_font(labelHorarioText, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelHorarioText, "Horario");
	
	labelHoraValue = lv_label_create(screen);
	lv_obj_align_to(labelHoraValue, labelHorarioText,  LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	lv_obj_add_style(labelHoraValue, &textStyle, 0);
	lv_obj_set_style_text_font(labelHoraValue, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelHoraValue, "%d", 20);
	
	labelHoraText = lv_label_create(screen);
	lv_obj_align_to(labelHoraText, labelHoraValue, LV_ALIGN_OUT_RIGHT_MID, 8, -5);
	lv_obj_add_style(labelHoraText, &textStyle, 0);
	lv_obj_set_style_text_font(labelHoraText, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelHoraText, "hora");
	
	labelMinTitle = lv_label_create(screen);
	lv_obj_align(labelMinTitle, LV_ALIGN_RIGHT_MID, -75, 15);
	lv_obj_add_style(labelMinTitle, &textStyle, 0);
	lv_obj_set_style_text_font(labelMinTitle, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelMinTitle, "");
	
	labelMinValue = lv_label_create(screen);
	lv_obj_align_to(labelMinValue, labelMinTitle,  LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_obj_add_style(labelMinValue, &textStyle, 0);
	lv_obj_set_style_text_font(labelMinValue, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelMinValue, "%d", 20);
	
	labelMinText = lv_label_create(screen);
	lv_obj_align_to(labelMinText, labelMinValue, LV_ALIGN_OUT_RIGHT_MID, 8, -5);
	lv_obj_add_style(labelMinText, &textStyle, 0);
	lv_obj_set_style_text_font(labelMinText, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelMinText, "min");
	
	labelSegundoTitle = lv_label_create(screen);
	lv_obj_align(labelSegundoTitle, LV_ALIGN_RIGHT_MID, -75, 45);
	lv_obj_add_style(labelSegundoTitle, &textStyle, 0);
	lv_obj_set_style_text_font(labelSegundoTitle, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelSegundoTitle, "");
	
	labelSegundoValue = lv_label_create(screen);
	lv_obj_align_to(labelSegundoValue,labelSegundoTitle,  LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_obj_add_style(labelSegundoValue, &textStyle, 0);
	lv_obj_set_style_text_font(labelSegundoValue, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelSegundoValue, "%d", 20);
	
	labelSegundoText = lv_label_create(screen);
	lv_obj_align_to(labelSegundoText, labelSegundoValue, LV_ALIGN_OUT_RIGHT_MID, 12, -2);
	lv_obj_add_style(labelSegundoText, &textStyle, 0);
	lv_obj_set_style_text_font(labelSegundoText, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelSegundoText, "seg");
	
	// --------------------------- Botoes do lado ---------------------------
	static lv_style_t style_def;
	lv_style_init(&style_def);
	lv_style_set_bg_color(&style_def, lv_color_white());
	lv_style_set_text_color(&style_def, lv_color_white());
	static lv_style_t style_pr;
	lv_style_init(&style_pr);
	lv_style_set_img_recolor_opa(&style_pr, LV_OPA_30);
	lv_style_set_img_recolor(&style_pr, lv_color_black());
	
	
	
	// --------------------------- Linhas de divisao ---------------------------
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 2);
	lv_style_set_bg_color(&style_line, lv_color_white());
	lv_style_set_line_color(&style_line, lv_color_black());
	lv_style_set_line_rounded(&style_line, true);
	
	// Linha para dividir a parte da velocidade da parte da viagem
	static lv_point_t line_points[] = { {0, 20}, {120, 20} };
	lv_obj_t * line1;
	line1 = lv_line_create(screen);
	lv_line_set_points(line1, line_points, 2);     /*Set the points*/
	lv_obj_add_style(line1, &style_line, 0);
	lv_obj_center(line1);
	lv_obj_align(line1, LV_ALIGN_RIGHT_MID, 0, -53);
	
	// Linha vertical de todas as caixas
	static lv_point_t line_boxes_points[] = { {0, 0}, {0, 240} };
	lv_obj_t *line_boxes;
	line_boxes = lv_line_create(screen);
	lv_line_set_points(line_boxes, line_boxes_points, 2);     /*Set the points*/
	lv_obj_add_style(line_boxes, &style_line, 0);
	lv_obj_center(line_boxes);
	
}
