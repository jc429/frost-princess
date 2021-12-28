#ifndef SH_MENU_SLIDER_H
#define SH_MENU_SLIDER_H

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>

#include "bn_sprite_items_slider_cursor.h"

namespace sh
{
	struct menu_slider
	{
		bn::fixed_point base_pos;
		bn::sprite_ptr slider_cursor;
		const int x_inc = 5;
		int min_value;
		int max_value;
		int cur_value;

		menu_slider(bn::fixed_point pos, int min, int max);
		~menu_slider();
		
		void set_range(int min, int max);
		int set_value(int val);
		int get_value();
		int increment();
		int decrement();

		void update_sprite();
	};
}

#endif //SH_MENU_SLIDER_H