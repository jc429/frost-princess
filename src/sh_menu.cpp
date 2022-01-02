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

namespace sh
{

	
	/***********************************************************/

	static bn::vector<menu_item, 8> _menu_items_;		// simple text menu items
	static bn::vector<menu_slider, 4> _menu_sliders_;		// slider items
	static bn::vector<menu_checkbox, 4> _menu_checkboxes_;	// checkboxes

	/***********************************************************/

	menu::menu(menu_type type, int layer, bn::sprite_text_generator &text_generator) :
		_cursor_sprite (bn::sprite_items::menu_arrow.create_sprite(0,0))
	{
		_current_scene = NULL;
		_type = type;
		_bg_layer = layer;
		_position = bn::fixed_point(0,0);
		_text_sprites.clear();
		_current_item = NULL;
		_cursor_sprite.set_bg_priority(layer);
		_cursor_sprite.set_z_order(-10);
		_menu_open = false;
		clear_menu_item_pool();


		switch(type)
		{
		case menu_type::PAUSE_MENU:
			{
				bn::regular_bg_builder builder(bn::regular_bg_items::pause_menu);
				builder.set_priority(layer);
				_menu_bg.push_back(builder.release_build());
				
				text_generator.set_bg_priority(layer);
				text_generator.set_center_alignment();
				bn::fixed_point pos(0,-69);		// nice
				text_generator.generate(pos, "-Paused-", _text_sprites);

				pos = bn::fixed_point(-20, -30);
				_menu_items_.push_back(menu_item(this, pos, menu_action_id::CLOSE_MENU, "Resume"));
				_item_list.push_back(&_menu_items_.back());
				pos = pos + bn::fixed_point(0,_item_offset_y);
				_menu_items_.push_back(menu_item(this, pos, menu_action_id::EXIT_SCENE, "Quit"));
				_item_list.push_back(&_menu_items_.back());

			}
			break;
		case menu_type::OPTIONS_MENU:
			{
				// bn::regular_bg_builder builder(bn::regular_bg_items::pause_menu);
				// builder.set_priority(bg_layer);
				// _menu_bgs.push_back(builder.release_build());

				text_generator.set_bg_priority(layer);
				text_generator.set_center_alignment();
				bn::fixed_point pos = bn::fixed_point(-80, -36);
				bn::fixed_point slider_offset(88,2);
				_menu_sliders_.push_back(menu_slider(this, pos, menu_action_id::NONE, "Music Volume", slider_offset, 0,9));
				_item_list.push_back(&_menu_sliders_.back());
				pos = pos + bn::fixed_point(0,_item_offset_y);
				_menu_sliders_.push_back(menu_slider(this, pos, menu_action_id::NONE, "SFX Volume", slider_offset, 0,9));
				_item_list.push_back(&_menu_sliders_.back());
				pos = pos + bn::fixed_point(0,_item_offset_y);
				_menu_checkboxes_.push_back(menu_checkbox(this, pos, menu_action_id::NONE, "Test Checkbox", slider_offset, false));
				_item_list.push_back(&_menu_checkboxes_.back());
			
			}
			break;
		default:
			break;
		}

		
		
		text_generator.set_bg_priority(layer);
		text_generator.set_left_alignment();
		// for(int i = 0; i < _menu_items.size(); i++)
		// {
		// 	menu_item &item = _menu_items.at(i);
		// 	text_generator.generate(item._position, item._text, _text_sprites);
		// }

		if(!_item_list.empty())
		{
			// generate text + link menu items
			for(int i = 0; (i+1) < _item_list.size(); i++)
			{
				_item_list.at(i)->link_below(_item_list.at(i+1));
				text_generator.generate(_item_list.at(i)->_position, _item_list.at(i)->_text, _text_sprites);
			}
			// special case for last element (or only element)
			_item_list.back()->link_below(_item_list.front());
			text_generator.generate(_item_list.back()->_position, _item_list.back()->_text, _text_sprites);

			_menu_open = true;
			highlight_menu_item(_item_list.front());
		}

		// advance one frame to flush out inputs 
		bn::core::update();
	}

	menu::~menu()
	{
		clear_menu_item_pool();
		_text_sprites.clear();
		_menu_bg.clear();
		// _menu_bg.reset();
	}
	
	void menu::clear_menu_item_pool()
	{
		_menu_items_.clear();
		_menu_sliders_.clear();
		_menu_checkboxes_.clear();
	}

	void menu::generate_menu_items()
	{

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