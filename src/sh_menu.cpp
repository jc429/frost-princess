#include "sh_menu.h"
#include "sh_scene.h"

#include <bn_core.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_optional.h>
#include <bn_optional_fwd.h>
#include <bn_regular_bg_builder.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_vector.h>

#include "bn_regular_bg_items_pause_menu.h"
#include "bn_sprite_items_menu_arrow.h"
#include "bn_sprite_items_slider_cursor.h"

namespace sh
{

	menu_item::menu_item(menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<16> text)
	{
		_position = pos;
		_action_id = action_id;
		_text = text;
		_menu = menu_owner;
		_above = NULL;
		_below = NULL;
	}
	
	// void menu_item::set_menu_action(void (*func)())
	// {
	// 	_menu_action = func;
	// }
	
	void menu_item::link_above(menu_item *other)
	{
		_above = other;
		if(other != NULL)
			other->_below = this;
	}

	void menu_item::link_below(menu_item *other)
	{
		_below = other;
		if(other != NULL)
			other->_above = this;
	}

	void menu_item::select()
	{
		if(_menu == NULL)
		{
			return;
		}
		_menu->highlight_menu_item(this);

	}

	void menu_item::interact_with_item(menu_item_interact_type interaction)
	{
		switch(interaction)
		{
		case menu_item_interact_type::UP_PRESS:
			if(_above != NULL)
			{
				_above->select();
			}
			break;
		case menu_item_interact_type::DOWN_PRESS:
			if(_below != NULL)
			{
				_below->select();
			}
			break;
		case menu_item_interact_type::A_PRESS:
			_menu->perform_action(_action_id);
			break;
		default:
			break;
		}
	}

	/***********************************************************/

	menu_slider::menu_slider(menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<16> text, int min_val, int max_val) :
		menu_item(menu_owner, pos, action_id,text),
		_slider_cursor(bn::sprite_items::slider_cursor.create_sprite(pos))
	{
		if(menu_owner != NULL)
		{
			_slider_cursor.set_bg_priority(menu_owner->get_layer());
			_slider_cursor.set_z_order(-10);
		}
		_min_value = min_val;
		_max_value = max_val;
		_cur_value = min_val;
	}

	menu_slider::~menu_slider()
	{

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
		_slider_cursor.set_x(_position.x() + (x_inc * _cur_value));
	}

	/***********************************************************/

	menu::menu(menu_type type, int bg_layer, bn::sprite_text_generator &text_generator) :
		_cursor_sprite (bn::sprite_items::menu_arrow.create_sprite(0,0))
	{
		_current_scene = NULL;
		_type = type;
		_bg_layer = bg_layer;
		_position = bn::fixed_point(0,0);
		_text_sprites.clear();
		_current_item = NULL;
		_menu_items.clear();
		_cursor_sprite.set_bg_priority(bg_layer);
		_cursor_sprite.set_z_order(-10);
		_menu_open = false;







		switch(type)
		{
		case menu_type::PAUSE_MENU:
			{
				bn::regular_bg_builder builder(bn::regular_bg_items::pause_menu);
				builder.set_priority(bg_layer);
				_menu_bg.push_back(builder.release_build());
				
				text_generator.set_bg_priority(bg_layer);
				text_generator.set_center_alignment();
				bn::fixed_point pos(0,-69);		// nice
				text_generator.generate(pos, "-Paused-", _text_sprites);

				pos = bn::fixed_point(-20, -30);
				_menu_items.push_back(menu_item(this, pos, menu_action_id::CLOSE_MENU, "Resume"));
				pos = pos + bn::fixed_point(0,_item_offset_y);
				_menu_items.push_back(menu_item(this, pos, menu_action_id::EXIT_SCENE, "Quit"));

			}
			break;
		case menu_type::OPTIONS_MENU:
			{
				// bn::regular_bg_builder builder(bn::regular_bg_items::pause_menu);
				// builder.set_priority(bg_layer);
				// _menu_bgs.push_back(builder.release_build());

				text_generator.set_bg_priority(bg_layer);
				text_generator.set_center_alignment();
				bn::fixed_point pos = bn::fixed_point(-80, -36);
				_menu_items.push_back(menu_slider(this, pos, menu_action_id::NONE, "Music Volume",0,9));
				pos = pos + bn::fixed_point(0,_item_offset_y);
				_menu_items.push_back(menu_slider(this, pos, menu_action_id::NONE, "SFX Volume",0,9));
			
			}
			break;
		default:
			break;
		}

		
		
		text_generator.set_bg_priority(bg_layer);
		text_generator.set_left_alignment();
		// for(int i = 0; i < _menu_items.size(); i++)
		// {
		// 	menu_item &item = _menu_items.at(i);
		// 	text_generator.generate(item._position, item._text, _text_sprites);
		// }

		if(!_menu_items.empty())
		{
			// generate text + link menu items
			if(_menu_items.size() > 1)
			{
				for(auto it = _menu_items.begin(), end = _menu_items.end(); it+1 != end; ++it)
				{
					it->link_below(it+1);
					text_generator.generate(it->_position, it->_text, _text_sprites);
				}
			}
			// special case for last element (or only element)
			_menu_items.back().link_below(&_menu_items.front());
			text_generator.generate(_menu_items.back()._position, _menu_items.back()._text, _text_sprites);

			_menu_open = true;
			highlight_menu_item(&_menu_items.front());
		}

		// advance one frame to flush out inputs 
		bn::core::update();
	}

	menu::~menu()
	{
		_text_sprites.clear();
		_menu_items.clear();
		_menu_bg.clear();
		// _menu_bg.reset();
	}
	
	void menu::update()
	{
		
		if(_current_item != NULL)
		{
			if(bn::keypad::a_pressed())
			{
				_current_item->interact_with_item(menu_item_interact_type::A_PRESS);
			}
			if(bn::keypad::down_pressed())
			{
				_current_item->interact_with_item(menu_item_interact_type::DOWN_PRESS);
			}
			else if(bn::keypad::up_pressed())
			{
				_current_item->interact_with_item(menu_item_interact_type::UP_PRESS);
			}
			if(bn::keypad::right_pressed())
			{
				_current_item->interact_with_item(menu_item_interact_type::RIGHT_PRESS);
			}
			else if(bn::keypad::left_pressed())
			{
				_current_item->interact_with_item(menu_item_interact_type::LEFT_PRESS);
			}

		}
		if(bn::keypad::b_pressed())
		{
			close_menu();
		}

	
	}
	


	void menu::highlight_menu_item(menu_item *item)
	{
		_current_item = item;	
		if(item == NULL)
		{
			_cursor_sprite.set_visible(false);
		}
		else{
			_cursor_sprite.set_position(item->_position + _cursor_offset);
			_cursor_sprite.set_visible(true);
		}
	}

	void menu::perform_action(menu_action_id action_id)
	{
		switch (action_id)
		{
		case menu_action_id::CLOSE_MENU:
			close_menu();
			break;
		case menu_action_id::EXIT_SCENE:
			scene_management::exit_current_scene(scene_type::TITLE);
			close_menu();
			break;
		default:
			break;
		}
	}

	bool menu::is_open()
	{
		return _menu_open;
	}

	void menu::close_menu()
	{
		_menu_open = false;
	}

	int menu::get_layer()
	{
		return _bg_layer;
	}

}