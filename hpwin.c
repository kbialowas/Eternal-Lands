#include "actors.h"
#include "asc.h"
#include "font.h"
#include "cursors.h"
#include "hpwin.h"
#include "hud.h"
#include "interface.h"
#include "item_lists.h"
#include "items.h"
#include "multiplayer.h"
#include "textures.h"
#include "translate.h"
#include "spells.h"
#include "elwindows.h"
#include "gamewin.h"

int hp_win = -1;
const int step_y = (int)(SMALL_FONT_Y_LEN+1.0);
const int bar_height = (int)(SMALL_FONT_Y_LEN-3);
const int items_bar_height = 30;
const int items_bar_slots = 6;
const int num_spells_slots = 6;
int hp_win_margin = 10;
int hp_win_x_len = 200;
int hp_win_y_len = 0;
int hp_win_x = 10;
int hp_win_y = 20;

int items_left, items_top, items_right, items_bottom;
int spells_left, spells_top, spells_right, spells_bottom;
int quickspell_over_hpwin = -1;

int action_mode_before_change = -1;
extern mqbdata * mqb_data[MAX_QUICKBAR_SLOTS+1];
// forward declaration
int display_hp_handler (window_info *win);
int	click_hp_handler(window_info *win, int mx, int my, Uint32 flags);
int mouseover_hp_handler(window_info *win, int mx, int my);
void draw_spell_icon(int id,int x_start, int y_start, int gridsize, int alpha, int grayed);
void display_hp_window()
{
	if(hp_win < 0)
	{		
		int our_root_win = -1;

		if (!windows_on_top) {
			our_root_win = game_root_win;
		}
		hp_win_y_len = step_y * 3 + hp_win_margin * 2 + 2*items_bar_height;
		
		hp_win = create_window(win_hp, our_root_win, 0, hp_win_x, hp_win_y, hp_win_x_len, hp_win_y_len,
			ELW_WIN_DEFAULT);

		set_window_handler(hp_win, ELW_HANDLER_DISPLAY, &display_hp_handler );
		set_window_handler(hp_win, ELW_HANDLER_CLICK, &click_hp_handler);
		set_window_handler(hp_win, ELW_HANDLER_MOUSEOVER, &mouseover_hp_handler );

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
	char str[80];
	int i;

	int bar_len = (hp_win_x_len - 3 * hp_win_margin) / 2;
	int numbers_x = bar_len + 2 * hp_win_margin;
	int pos_y = hp_win_margin;

	float hp_cur_percent = your_info.material_points.cur / (float)your_info.material_points.base;
	float eth_cur_percent = your_info.ethereal_points.cur / (float)your_info.ethereal_points.base;
	float hp_cur_len = bar_len * hp_cur_percent;
	float eth_cur_len = bar_len * eth_cur_percent;
	int item_bar_start = (hp_win_x_len - (items_bar_slots * 30 + 1))/2;
	Uint32 _cur_time = SDL_GetTicks(); /* grab a snapshot of current time */
	int x,y;

int quickspell_size = 23;
int startSpells = (hp_win_x_len - (num_spells_slots * (quickspell_size + 5)))/2;

	sprintf((char*)str,"%u/%u", your_info.material_points.cur, your_info.material_points.base);
	set_health_color( hp_cur_percent, 1.0f, 1.0f);
	draw_string_small(numbers_x, pos_y, (unsigned char*)str,2);
	
	pos_y += step_y;
	sprintf((char*)str,"%u/%u", your_info.ethereal_points.cur, your_info.ethereal_points.base);
	set_ether_color( eth_cur_percent, 1.0f, 1.0f);
	draw_string_small(numbers_x, pos_y, (unsigned char*)str,2);

	pos_y += step_y;
	if (your_actor != NULL && your_actor->fighting) {
		glColor4f(1.0f,0.1f,0.1f,1.0f);
		sprintf((char*)str,"Engaged in combat");
	} else {
		glColor4f(0.3f,0.3f,0.3f,0.5f);
		sprintf((char*)str,"Not engaged in combat");
	}
	draw_string_small((hp_win_x_len - ((int)(SMALL_FONT_X_LEN)*strlen(str)))/2, pos_y, (unsigned char*)str,2);

	glDisable(GL_TEXTURE_2D);
	//draw current health and mana bars
	pos_y = hp_win_margin;
	glBegin(GL_QUADS);
		set_health_color( hp_cur_percent, 0.5f, 1.0f);
		glVertex3i(hp_win_margin, pos_y+1,0);
		glVertex3i(hp_win_margin+hp_cur_len, pos_y+1,0);
		set_health_color( hp_cur_percent, 1.0f, 1.0f);
		glVertex3i(hp_win_margin+hp_cur_len, pos_y + bar_height,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);

		pos_y += step_y;
		set_ether_color( eth_cur_len, 0.5f, 1.0f);
		glVertex3i(hp_win_margin, pos_y+1,0);
		glVertex3i(hp_win_margin+eth_cur_len, pos_y+1,0);
		set_ether_color( eth_cur_len, 1.0f, 1.0f);
		glVertex3i(hp_win_margin+eth_cur_len, pos_y + bar_height,0);
		glVertex3i(hp_win_margin, pos_y + bar_height,0);
	glEnd();

	//draw health and mana bars borders
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

	//display items list - code taken from hud.c::display_quickbar_handler
	glColor3f(1.0f,1.0f,1.0f);
	//ok, now let's draw the objects...
	pos_y += 2*step_y+5;
	for(i=0;i<items_bar_slots;i++)
	{
		if(item_list[i].quantity > 0)
		{
			float u_start,v_start,u_end,v_end;
			int this_texture,cur_item;
			int x_start,x_end,y_start,y_end;

			//get the UV coordinates.
			cur_item=item_list[i].image_id%25;
#ifdef	NEW_TEXTURES
			get_item_uv(cur_item, &u_start, &v_start, &u_end,
				&v_end);
#else	/* NEW_TEXTURES */
			u_start=0.2f*(cur_item%5);
			u_end=u_start+(float)50/256;
			v_start=(1.0f+((float)50/256)/256.0f)-((float)50/256*(cur_item/5));
			v_end=v_start-(float)50/256;
#endif	/* NEW_TEXTURES */

			//get the x and 
			x_start= item_bar_start + 29*i;
			x_end= x_start+27;
			y_start= pos_y;
			y_end= y_start+27;

			//get the texture this item belongs to
			this_texture=get_items_texture(item_list[i].image_id/25);

#ifdef	NEW_TEXTURES
			bind_texture(this_texture);
#else	/* NEW_TEXTURES */
			get_and_set_texture_id(this_texture);
#endif	/* NEW_TEXTURES */
			glBegin(GL_QUADS);
				draw_2d_thing(u_start,v_start,u_end,v_end,x_start,y_start,x_end,y_end);
			glEnd();
			
			if (item_list[i].cooldown_time > _cur_time)
			{
				float cooldown = ((float)(item_list[i].cooldown_time - _cur_time)) / ((float)item_list[i].cooldown_rate);
				float x_center = (x_start + x_end)*0.5f;
				float y_center = (y_start + y_end)*0.5f;

				if (cooldown < 0.0f)
					cooldown = 0.0f;
				else if (cooldown > 1.0f)
					cooldown = 1.0f;
				
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBegin(GL_TRIANGLE_FAN);
					glColor4f(0.14f, 0.35f, 0.82f, 0.50f); 

					glVertex2f(x_center, y_center);

					if (cooldown >= 0.875f) {
						float t = tan(2.0f*M_PI*(1.0f - cooldown));
						glVertex2f(t*x_end + (1.0f - t)*x_center, y_start);
						glVertex2f(x_end, y_start);
						glVertex2f(x_end, y_end);
						glVertex2f(x_start, y_end);
						glVertex2f(x_start, y_start);
					} else if (cooldown >= 0.625f) {
						float t = 0.5f + 0.5f*tan(2.0f*M_PI*(0.75f - cooldown));
						glVertex2f(x_end, t*y_end + (1.0f - t)*y_start);
						glVertex2f(x_end, y_end);
						glVertex2f(x_start, y_end);
						glVertex2f(x_start, y_start);
					} else if (cooldown >= 0.375f) {
						float t = 0.5f + 0.5f*tan(2.0f*M_PI*(0.5f - cooldown));
						glVertex2f(t*x_start + (1.0f - t)*x_end, y_end);
						glVertex2f(x_start, y_end);
						glVertex2f(x_start, y_start);
					} else if (cooldown >= 0.125f) {
						float t = 0.5f + 0.5f*tan(2.0f*M_PI*(0.25f - cooldown));
						glVertex2f(x_start, t*y_start + (1.0f - t)*y_end);
						glVertex2f(x_start, y_start);
					} else {
						float t = tan(2.0f*M_PI*(cooldown));
						glVertex2f(t*x_start + (1.0f - t)*x_center, y_start);
					}

					glVertex2f(x_center, y_start);
				glEnd();

				glDisable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
			
			safe_snprintf(str,sizeof(str),"%i",item_list[i].quantity);
			draw_string_small_shadowed(x_start,y_end-15,(unsigned char*)str,1,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f);
		}
	}

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	//use_window_color(quickbar_win, ELW_COLOR_LINE);
	//draw the grid
	items_left = item_bar_start-1;
	items_right = item_bar_start+items_bar_slots*29-1;
	items_top = pos_y-1;
	items_bottom = pos_y + 29;
	for(i=0;i<=items_bar_slots;i++)
		{
			glVertex3i(item_bar_start+i*29-1, items_top, 0);
			glVertex3i(item_bar_start+i*29-1, items_bottom, 0);
		}
		glVertex3i(items_left, items_top, 0);
		glVertex3i(items_right, items_top, 0);
		glVertex3i(items_left, items_bottom, 0);
		glVertex3i(items_right, items_bottom, 0);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	//draw spell bar

pos_y += (quickspell_size*1.5);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.20f);
	glEnable(GL_BLEND);	// Turn Blending On
	glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);

	for(i=1;i<num_spells_slots+1;i++) {
		if(mqb_data[i] && mqb_data[i]->spell_name[0]){
			x=(i-1)*(quickspell_size+5)+startSpells;
			y=pos_y;
			
			if(quickspell_over_hpwin==i){	//highlight if we are hovering over
				glColor4f(1.0f,1.0f,1.0f,1.0f);
			} else {	//otherwise shade it a bit
				glColor4f(1.0f,1.0f,1.0f,0.6f);
			}

			draw_spell_icon(mqb_data[i]->spell_image,x,y,quickspell_size,0,0);
		}
	}
	spells_top = pos_y;
	spells_bottom = pos_y + quickspell_size;
	spells_left = startSpells;
	spells_right = (num_spells_slots)*(quickspell_size+5)+startSpells;;
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glDisable(GL_BLEND);	// Turn Blending Off
	glDisable(GL_ALPHA_TEST);

	if(quickspell_over_hpwin!=-1 && mqb_data[quickspell_over_hpwin])
		show_help(mqb_data[quickspell_over_hpwin]->spell_name,(quickspell_over_hpwin-1)*(quickspell_size+5)+startSpells,pos_y+26);
	quickspell_over_hpwin=-1;
	return 1;
}



int	click_hp_handler(window_info *win, int mx, int my, Uint32 flags){
	char str[10];
	int i;
	if (mx > items_left && mx < items_right && my > items_top && my < items_bottom) {
		i=(mx - items_left) / 29;
		if(item_list[i].use_with_inventory)
			{
				str[0]=USE_INVENTORY_ITEM;
				str[1]=item_list[i].pos;
				my_tcp_send(my_socket,(Uint8 *)str,2);
#ifdef NEW_SOUND
				item_list[i].action = USE_INVENTORY_ITEM;
#endif // NEW_SOUND
				return 1;
			}
		return 1;
	}
	if (mx > spells_left && mx < spells_right && my > spells_top && my < spells_bottom) {
		i = ((mx - spells_left) / 28)+1;
		send_spell(mqb_data[i]->spell_str, mqb_data[i]->spell_str[1]+2);
		return 1;
	}
	return 0;
};

int mouseover_hp_handler(window_info *win, int mx, int my) {
	if (mx > items_left && mx < items_right && my > items_top && my < items_bottom) {
		elwin_mouse=CURSOR_USE;
		return 1;
	}

	if (mx > spells_left && mx < spells_right && my > spells_top && my < spells_bottom) {
		quickspell_over_hpwin=((mx - spells_left) / 28)+1;
		elwin_mouse=CURSOR_WAND;
		return 1;
	}
	return 0;
};
