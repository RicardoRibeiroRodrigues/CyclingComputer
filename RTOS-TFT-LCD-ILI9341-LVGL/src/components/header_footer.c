/*
 * header_footer.c
 *
 * Created: 21/11/2022 16:22:01
 *  Author: ricardorr7
 */ 
#include "header_footer.h"


void create_header(lv_obj_t * screen) {
	static lv_style_t sevenSegStyle;
	lv_style_init(&sevenSegStyle);
	lv_style_set_border_width(&sevenSegStyle, 0);
	lv_style_set_text_color(&sevenSegStyle, lv_color_white());
	// Label clock
	labelClockHeader = lv_label_create(screen);
	lv_obj_align(labelClockHeader, LV_ALIGN_TOP_RIGHT, -10, 10);
	lv_obj_add_style(labelClockHeader, &sevenSegStyle, 0);
	// 0123456789:., PesokgHrárim/hclVatu
	// lv_obj_set_style_text_font(labelClockHeader, &mont_alt_el_58, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelClockHeader, "%02d:%02d:02d", 14, 2, 10);
	
	// Linha divisora
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 2);
	lv_style_set_line_color(&style_line, lv_color_black());
	lv_style_set_line_rounded(&style_line, true);

	 
	static lv_point_t line_points[] = { {10, 0}, {10, 240} };
	lv_obj_t * line1;
	line1 = lv_line_create(screen);
	lv_line_set_points(line1, line_points, 2);     /*Set the points*/
	lv_obj_add_style(line1, &style_line, 0);
	lv_obj_center(line1);
}