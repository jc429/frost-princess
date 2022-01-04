#include "sh_menu.h"
#include "sh_scene.h"
#include "sh_game_settings.h"

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
#include "variable_8x16_sprite_font.h"

namespace sh
{

	
	/***********************************************************/

	static bn::vector<menu_item, 8> _menu_items_;		// simple text menu items
	static bn::vector<menu_slider, 4> _menu_sliders_;		// slider items
	static bn::vector<menu_checkbox, 4> _menu_checkboxes_;	// checkboxes

	/***********************************************************/

	menu::menu(menu_type type, int layer) :
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
		_header_text = "";
		

		clear_menu_item_pool();

		bn::fixed_point pos;
		

		generate_menu_items();
		
		
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
			}
			// special case for last element (or only element)
			_item_list.back()->link_below(_item_list.front());

			_menu_open = true;
			highlight_menu_item(_item_list.front());
		}

		close_menu();

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
		bn::fixed_point pos(0,0);
		switch(_type)
		{
		case menu_type::PAUSE_MENU:
			{
				_header_text = "-Paused-";

				bn::regular_bg_builder builder(bn::regular_bg_items::pause_menu);
				builder.set_priority(_bg_layer);
				_menu_bg.push_back(builder.release_build());
				
				pos = bn::fixed_point(-20, -30);
				{
					_menu_items_.push_back(menu_item(this, pos, menu_action_id::CLOSE_MENU, "Resume"));
					_item_list.push_back(&_menu_items_.back());
				}
				pos = pos + bn::fixed_point(0,_item_offset_y);
				{
					_menu_items_.push_back(menu_item(this, pos, menu_action_id::EXIT_SCENE, "Quit"));
					_item_list.push_back(&_menu_items_.back());
				}

			}
			break;
		case menu_type::OPTIONS_MENU:
			{
				// text_generator.set_center_alignment();
				pos = bn::fixed_point(-76, -44);
				bn::fixed_point slider_offset(88,2);
				{
					_menu_sliders_.push_back(menu_slider(this, pos, menu_action_id::NONE, "Music Volume", slider_offset, 0,9));
					_menu_sliders_.back().set_value(game_settings::get_music_volume());
					_menu_sliders_.back().update_menu_item_event = &game_settings::set_music_volume;
					_item_list.push_back(&_menu_sliders_.back());
				}
				pos = pos + bn::fixed_point(0,_item_offset_y);
				{
					_menu_sliders_.push_back(menu_slider(this, pos, menu_action_id::NONE, "SFX Volume", slider_offset, 0,9));
					_menu_sliders_.back().set_value(game_settings::get_sfx_volume());
					_menu_sliders_.back().update_menu_item_event = &game_settings::set_sfx_volume;
					_item_list.push_back(&_menu_sliders_.back());
				}
				pos = pos + bn::fixed_point(0,_item_offset_y);
				bn::fixed_point checkbox_offset(100,0);
				{
					_menu_checkboxes_.push_back(menu_checkbox(this, pos, menu_action_id::NONE, "Colorblind Mode", checkbox_offset, game_settings::colorblind_mode_enabled()));
					_menu_checkboxes_.back().update_menu_item_event = &game_settings::set_colorblind_mode_enabled;
					_item_list.push_back(&_menu_checkboxes_.back());
				}
				pos = pos + bn::fixed_point(0,_item_offset_y);
				pos = pos + bn::fixed_point(0,_item_offset_y);
				pos = pos + bn::fixed_point(0,_item_offset_y);
				pos = pos + bn::fixed_point(0,_item_offset_y);
				{
					_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, "Reset Settings to Default"));
					_menu_items_.back().update_menu_item_event = &game_settings::reset_settings_to_default;
					_item_list.push_back(&_menu_items_.back());
				}
			
			}
			break;
		case menu_type::TITLE_MENU:
			{
				pos = bn::fixed_point(-25, 36);
				{
					_menu_items_.push_back(menu_item(this, pos, menu_action_id::GO_TO_BATTLE, "Continue"));
					_item_list.push_back(&_menu_items_.back());
				}
				pos += bn::fixed_point(0,_item_offset_y);
				{
					_menu_items_.push_back(menu_item(this, pos, menu_action_id::GO_TO_OPTIONS, "Options"));
					_item_list.push_back(&_menu_items_.back());
				}
				pos += bn::fixed_point(0,_item_offset_y);
				{
					_menu_items_.push_back(menu_item(this, pos, menu_action_id::GO_TO_CREDITS, "Credits"));
					_item_list.push_back(&_menu_items_.back());
				}
			}
			break;
		default:
			break;
		}
	}

	void menu::generate_menu_text()
	{
		_text_sprites.clear();
		bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
		text_generator.set_bg_priority(_bg_layer);
		text_generator.set_center_alignment();
		bn::fixed_point pos(0,-69);		// nice
		text_generator.generate(pos, _header_text, _text_sprites);

		text_generator.set_left_alignment();
		for(int i = 0; i < _item_list.size(); i++)
		{
			text_generator.generate(_item_list.at(i)->_position, _item_list.at(i)->_text, _text_sprites);
		}
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
		case menu_action_id::GO_TO_TITLE:
			scene_management::exit_current_scene(scene_type::TITLE);
			close_menu();
			break;
		case menu_action_id::GO_TO_BATTLE:
			scene_management::exit_current_scene(scene_type::BATTLE);
			close_menu();
			break;
		case menu_action_id::GO_TO_OPTIONS:
			scene_management::exit_current_scene(scene_type::OPTIONS);
			close_menu();
			break;
		case menu_action_id::GO_TO_CREDITS:
			scene_management::exit_current_scene(scene_type::CREDITS);
			close_menu();
			break;
		default:
			break;
		}
	}

	void menu::open_menu()
	{
		_menu_open = true;
		for(int i= 0; i < _item_list.size(); i++)
		{
			_item_list.at(i)->show_item();
		}
		generate_menu_text();
		_cursor_sprite.set_visible(true);
	}

	void menu::close_menu()
	{
		_menu_open = false;
		for(int i= 0; i < _item_list.size(); i++)
		{
			_item_list.at(i)->hide_item();
		}
		_text_sprites.clear();
		_cursor_sprite.set_visible(false);
	}

	bool menu::is_open()
	{
		return _menu_open;
	}

	int menu::get_layer()
	{
		return _bg_layer;
	}

}