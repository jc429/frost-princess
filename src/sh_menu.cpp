#include "sh_menu.h"

#include <bn_core.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
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

	menu_item::menu_item(bn::fixed_point pos, menu_action_id action_id, bn::string<16> text)
	{
		_position = pos;
		_action_id = action_id;
		_text = text;
	}

	menu::menu(menu_type type, int bg_layer, bn::sprite_text_generator &text_generator) :
		_cursor_sprite (bn::sprite_items::menu_arrow.create_sprite(0,0))
	{
		_type = type;
		_text_sprites.clear();
		_selection_id = 0;
		_menu_open = true;
		_cursor_sprite.set_bg_priority(bg_layer);
		_cursor_sprite.set_z_order(-10);
		

		switch(type)
		{
		case menu_type::PAUSE_MENU:
			{
				bn::regular_bg_builder builder(bn::regular_bg_items::pause_menu);
				builder.set_priority(bg_layer);
				_menu_bgs.push_back(builder.release_build());

				text_generator.set_bg_priority(bg_layer);
				text_generator.set_center_alignment();
				bn::fixed_point pos(0,-69);		// nice
				text_generator.generate(pos, "-Paused-", _text_sprites);
				pos = bn::fixed_point(-40, -30);
				_menu_items.push_back(menu_item(pos, menu_action_id::CLOSE_MENU, "Resume"));
				pos.set_y(pos.y() + _item_offset_y);
				_menu_items.push_back(menu_item(pos, menu_action_id::CLOSE_MENU, "Quit"));

			}
			break;
		default:
			break;
		}
		
		text_generator.set_bg_priority(bg_layer);
		text_generator.set_left_alignment();
		for(int i = 0; i < _menu_items.size(); i++)
		{
			menu_item &item = _menu_items.at(i);
			text_generator.generate(item._position, item._text, _text_sprites);
		}

		select_item(0);

		// advance one frame to flush out inputs 
		bn::core::update();
		while(_menu_open)
		{
			update();
		}
	}

	menu::~menu()
	{
		_text_sprites.clear();
		_menu_items.clear();
		_menu_bgs.clear();
	}
	
	void menu::update()
	{
		if(bn::keypad::b_pressed())
		{
			close_menu();
		}
		else if(bn::keypad::a_pressed())
		{
			
		}
		if(bn::keypad::down_pressed())
		{
			move_cursor(direction::SOUTH);
		}
		else if(bn::keypad::up_pressed())
		{
			move_cursor(direction::NORTH);
		}


		bn::core::update();
	}
	
	void menu::move_cursor(direction dir)
	{
		switch(dir)
		{
		case direction::SOUTH:
			_selection_id = (_selection_id + 1) % _menu_items.size();
			break;
		case direction::NORTH:
			_selection_id = (_selection_id + _menu_items.size() - 1) % _menu_items.size();
			break;
		default:
			break;
		}
		_cursor_sprite.set_position(_menu_items.at(_selection_id)._position + _cursor_offset);
	}

	void menu::select_item(int item_index)
	{
		_selection_id = bn::clamp(item_index, 0, _menu_items.size());
		_cursor_sprite.set_position(_menu_items.at(_selection_id)._position + _cursor_offset);
	}

	void menu::close_menu()
	{
		_menu_open = false;
	}
}