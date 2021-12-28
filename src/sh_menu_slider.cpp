#include "sh_menu_slider.h"


#include <bn_fixed_point.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include "bn_sprite_items_slider_cursor.h"

namespace sh
{

	menu_slider::menu_slider(bn::fixed_point pos, int min, int max) :
		slider_cursor(bn::sprite_items::slider_cursor.create_sprite(pos))
	{
		base_pos = pos;
		min_value = min;
		max_value = max;
		cur_value = min;

		slider_cursor.set_bg_priority(2);
		slider_cursor.set_z_order(-10);
	}
	
	menu_slider::~menu_slider()
	{
	}

	
	void menu_slider::set_range(int min, int max)
	{
		min_value = min;
		max_value = max;
		set_value(cur_value);
	}

	int menu_slider::set_value(int val)
	{
		cur_value = bn::clamp(val, min_value, max_value);
		update_sprite();
		return cur_value;
	}

	int menu_slider::get_value()
	{
		return cur_value;
	}

	int menu_slider::increment()
	{
		return set_value(cur_value + 1);
	}

	int menu_slider::decrement()
	{
		return set_value(cur_value - 1);
	}

	void menu_slider::update_sprite()
	{
		slider_cursor.set_x(base_pos.x() + (x_inc * cur_value));
	}

}