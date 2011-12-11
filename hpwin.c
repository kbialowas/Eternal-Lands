#include "actors.h"
#include "font.h"
#include "hpwin.h"
#include "translate.h"
#include "elwindows.h"
#include "gamewin.h"

int hp_win = -1;
const int step_y = (int)(SMALL_FONT_Y_LEN+0.5);
const int bar_height = (int)(SMALL_FONT_Y_LEN-3);
int hp_win_margin = 10;
int hp_win_x_len = 200;
int hp_win_y_len = 100;
int hp_win_x = 10;
int hp_win_y = 20;

#define progress_bar_width 135
#define progress_bar_height 10

// forward declaration
int display_hp_handler (window_info *win);

void display_hp_window()
{
	if(hp_win < 0)
	{		
		int our_root_win = -1;

		if (!windows_on_top) {
			our_root_win = game_root_win;
		}
		hp_win_y_len = step_y * 2 + hp_win_margin * 2;
		hp_win = create_window(win_hp, our_root_win, 0, hp_win_x, hp_win_y, hp_win_x_len, hp_win_y_len,
			ELW_WIN_DEFAULT);

		set_window_handler(hp_win, ELW_HANDLER_DISPLAY, &display_hp_handler );

		//cm_add(windows_list.window[hp_win].cm_id, cm_astro_menu_str, cm_astro_handler);
	} 
	else 
	{
		show_window(hp_win);
		select_window(hp_win);
	}
}


int display_hp_handler(window_info *win)
{
	char str[20];

	int bar_len = (hp_win_x_len - 3 * hp_win_margin) / 2;
	int numbers_x = bar_len + 2 * hp_win_margin;
	int pos_y = hp_win_margin;

	float hp_cur_len = bar_len * your_info.material_points.cur / (float)your_info.material_points.base;
	float eth_cur_len = bar_len * your_info.ethereal_points.cur / (float)your_info.ethereal_points.base;

	sprintf((char*)str,"%u/%u", your_info.material_points.cur, your_info.material_points.base);
	set_health_color( your_info.material_points.cur/(float)your_info.material_points.base, 1.0f, 1.0f);
	draw_string_small(numbers_x, pos_y, (unsigned char*)str,2);
	
	pos_y += step_y;
	sprintf((char*)str,"%u/%u", your_info.ethereal_points.cur, your_info.ethereal_points.base);
	set_ether_color( your_info.ethereal_points.cur/(float)your_info.ethereal_points.base, 1.0f, 1.0f);
	draw_string_small(numbers_x, pos_y, (unsigned char*)str,2);


	glDisable(GL_TEXTURE_2D);

	pos_y = hp_win_margin;
	glBegin(GL_QUADS);
		set_health_color( your_info.material_points.cur/(float)your_info.material_points.base, 0.5f, 1.0f);
		glVertex3i(hp_win_margin, pos_y+1,0);
		glVertex3i(hp_win_margin+hp_cur_len, pos_y+1,0);
		set_health_color( your_info.material_points.cur/(float)your_info.material_points.base, 1.0f, 1.0f);
		glVertex3i(hp_win_margin+hp_cur_len, pos_y + bar_height,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);



		pos_y += step_y;
		set_ether_color( your_info.ethereal_points.cur/(float)your_info.ethereal_points.base, 0.5f, 1.0f);
		glVertex3i(hp_win_margin, pos_y+1,0);
		glVertex3i(hp_win_margin+eth_cur_len, pos_y+1,0);
		set_ether_color( your_info.ethereal_points.cur/(float)your_info.ethereal_points.base, 1.0f, 1.0f);
		glVertex3i(hp_win_margin+eth_cur_len, pos_y + bar_height,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);
	glEnd();

	pos_y = hp_win_margin;
	glBegin(GL_LINES);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertex3i(hp_win_margin, pos_y,0);
		glVertex3i(hp_win_margin + bar_len, pos_y,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);
		glVertex3i(hp_win_margin + bar_len, pos_y + bar_height,0);

		glVertex3i(hp_win_margin, pos_y,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);
		glVertex3i(hp_win_margin + bar_len, pos_y,0);
		glVertex3i(hp_win_margin + bar_len, pos_y + bar_height,0);
		pos_y += step_y;
		glVertex3i(hp_win_margin, pos_y,0);
		glVertex3i(hp_win_margin + bar_len, pos_y,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);
		glVertex3i(hp_win_margin + bar_len, pos_y + bar_height,0);

		glVertex3i(hp_win_margin, pos_y,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);
		glVertex3i(hp_win_margin + bar_len, pos_y,0);
		glVertex3i(hp_win_margin + bar_len, pos_y + bar_height,0);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	return 1;
}
