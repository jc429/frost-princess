#include "sh_menu.h"

#include <bn_sprite_builder.h>
#include <bn_sprite_item.h>
#include <bn_sprite_tiles_item.h>

#include "bn_sprite_items_menu_slider_cursor.h"
#include "bn_sprite_items_menu_slider_bar.h"

namespace sh
{
	menu_slider::menu_slider(menu *menu_owner, int id, bn::fixed_point pos, menu_action_id action_id, bn::string<MENU_STRING_MAX_LEN> text, bn::fixed_point spr_offset, int min_val, int max_val) :
		menu_item(menu_owner, id, pos, action_id, text),
		_slider_cursor(bn::sprite_items::menu_slider_cursor.create_sprite(pos + spr_offset))
	{
		if(menu_owner != NULL)
		{
			_slider_cursor.set_bg_priority(menu_owner->get_layer());
			_slider_cursor.set_z_order(-10);
		}
		_slider_offset = spr_offset;
		
		for(int i = 0; i < 2; i++)
		{
			bn::sprite_builder builder(bn::sprite_items::menu_slider_bar, i);
			builder.set_bg_priority(menu_owner->get_layer());
			builder.set_z_order(-5);
			builder.set_position(pos + spr_offset + bn::fixed_point(7+(i*32),0));
			_slider_bar.push_back(builder.release_build());
		}

		_min_value = min_val;
		_max_value = max_val;
		_cur_value = min_val;
	}

	menu_slider::~menu_slider()
	{
		_slider_bar.clear();
	}

	void menu_slider::show_item()
	{
		for(auto it = _slider_bar.begin(), end = _slider_bar.end(); it != end; ++it)
		{
			it->set_visible(true);
		}
		_slider_cursor.set_visible(true);
	}

	void menu_slider::hide_item()
	{
		for(auto it = _slider_bar.begin(), end = _slider_bar.end(); it != end; ++it)
		{
			it->set_visible(false);
		}
		_slider_cursor.set_visible(false);
	}

	void menu_slider::interact_with_item(menu_item_interact_type interaction)
	{
		switch(interaction)
		{
		case menu_item_interact_type::LEFT_PRESS:
			decrement();
			if(click_menu_item_event != NULL)
			{
				click_menu_item_event(_cur_value);
			}
			break;
		case menu_item_interact_type::RIGHT_PRESS:
			increment();
			if(click_menu_item_event != NULL)
			{
				click_menu_item_event(_cur_value);
			}
			break;
		case menu_item_interact_type::A_PRESS:
			break;
		default:
			return menu_item::interact_with_item(interaction);
			break;
		}
	}

	void menu_slider::set_range(int min, int max)
	{
		_min_value = min;
		_max_value = max;
		set_value(_cur_value);
	}

	int menu_slider::set_value(int val)
	{
		_cur_value = bn::clamp(val, _min_value, _max_value);
		update_sprite();
		return _cur_value;
	}

	int menu_slider::get_value()
	{
		return _cur_value;
	}

	int menu_slider::increment()
	{
		return set_value(_cur_value + 1);
	}

	int menu_slider::decrement()
	{
		return set_value(_cur_value - 1);
	}

	void menu_slider::update_sprite()
	{
		_slider_cursor.set_x(_position.x() + _slider_offset.x() + (x_inc * _cur_value));
	}

}