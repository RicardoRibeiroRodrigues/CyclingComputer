/*
 * screen_1_parts.c
 *
 * Created: 11/24/2022 8:34:54 PM
 *  Author: riywa
 */ 
#include "screen_1_parts.h"

LV_FONT_DECLARE(MontAltEL80);


void create_speed_section(lv_obj_t * screen, const lv_font_t *MontAltEL20) {
	static lv_style_t textStyle;
	lv_style_init(&textStyle);
	lv_style_set_border_width(&textStyle, 0);
	lv_style_set_text_color(&textStyle, lv_color_black());
	
	labelSpeedValue = lv_label_create(screen);
	lv_obj_align(labelSpeedValue, LV_ALIGN_LEFT_MID, 26, -75);
	lv_obj_add_style(labelSpeedValue, &textStyle, 0);
	lv_obj_set_style_text_font(labelSpeedValue, &MontAltEL80, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelSpeedValue, "%d", 10);
	
	
	labelSpeedUnit = lv_label_create(screen);
	lv_obj_add_style(labelSpeedUnit, &textStyle, 0);
	lv_obj_align_to(labelSpeedUnit, labelSpeedValue, LV_ALIGN_OUT_RIGHT_MID, 10, -10);
	lv_obj_set_style_text_font(labelSpeedUnit, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelSpeedUnit, "km/h");
	
	// Linha divisora
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 2);
	lv_style_set_bg_color(&style_line, lv_color_white());
	lv_style_set_line_color(&style_line, lv_color_black());
	lv_style_set_line_rounded(&style_line, true);

	// Um dos maiores misterios da humanidade -> O y = 0 está na metade da tela
	static lv_point_t line_points[] = { {0, 0}, {240, 0} };
	lv_obj_t * line1;
	line1 = lv_line_create(screen);
	lv_line_set_points(line1, line_points, 2);     /*Set the points*/
	lv_obj_add_style(line1, &style_line, 0);
	lv_obj_center(line1);
}


void create_dist_div(lv_obj_t *screen, const lv_font_t *MontAltEL20, lv_style_t *textStyle) {
	labelDistTitle = lv_label_create(screen);
	lv_obj_align(labelDistTitle, LV_ALIGN_RIGHT_MID, -10, 30);
	lv_obj_add_style(labelDistTitle, textStyle, 0);
	lv_obj_set_style_text_font(labelDistTitle, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelDistTitle, "Distância");
	
	labelDistValue = lv_label_create(screen);
	lv_obj_align(labelDistValue, LV_ALIGN_RIGHT_MID, -10, 45);
	lv_obj_add_style(labelDistValue, textStyle, 0);
	lv_obj_set_style_text_font(labelDistValue, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelDistValue, "%d", 10);
	
}

void create_viagem_section(lv_obj_t *screen, const lv_font_t *MontAltEL20) {
	static lv_style_t textStyle;
	lv_style_init(&textStyle);
	lv_style_set_border_width(&textStyle, 0);
	lv_style_set_text_color(&textStyle, lv_color_black());
	
	labelViagemText = lv_label_create(screen);
	lv_obj_align(labelViagemText, LV_ALIGN_LEFT_MID, 5, 10);
	lv_obj_add_style(labelViagemText, &textStyle, 0);
	lv_obj_set_style_text_font(labelViagemText, MontAltEL20, LV_STATE_DEFAULT);
	lv_label_set_text(labelViagemText, "Viagem atual");
	
	create_dist_div(screen, MontAltEL20, &textStyle);
}