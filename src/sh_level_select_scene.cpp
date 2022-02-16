#include "sh_scene.h"
#include "sh_level_select_menu.h"
#include "sh_level_select_scene.h"
#include "sh_random.h"
#include "sh_character.h"

#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_builder.h>

#include "bn_regular_bg_items_char_sel_bg.h"
#include "bn_regular_bg_items_char_sel_fg.h"
#include "bn_sprite_items_level_select_frame.h"
#include "bn_sprite_items_nametags.h"

namespace sh
{
	void update_char_display(int item_id);
	void update_selected_character(int item_id);

	static level_select_scene *_instance_ = NULL;

	level_select_scene::level_select_scene() :
		_lev_sel_bg (bn::regular_bg_items::char_sel_bg.create_bg(0, 0)),
		_lev_sel_fg (bn::regular_bg_items::char_sel_fg.create_bg(0, 0)),
		_char_sprite(character_id::SHADOW, char_sprite_type::FULLBODY)
	{
		_instance_ = this;
		type = scene_type::LEVEL_SELECT;
		_lev_sel_bg.set_z_order(3);
		_lev_sel_fg.set_z_order(2);
		{
			panel_positions.push_back(lev_sel_pos::top_left);
			panel_positions.push_back(lev_sel_pos::top_right);
			panel_positions.push_back(lev_sel_pos::mid_left);
			panel_positions.push_back(lev_sel_pos::mid_right);
			panel_positions.push_back(lev_sel_pos::bot_left);
			panel_positions.push_back(lev_sel_pos::bot_right);
	
			bn::sprite_builder builder(bn::sprite_items::level_select_frame);
			builder.set_bg_priority(1);
			builder.set_z_order(10);
			for(int i = 0; i < 6; i++)
			{
				builder.set_position(panel_positions.at(i));
				portrait_frames.push_back(builder.build());
			}
			portrait_sprites.push_back(characters::get_select_sprite(character_id::PROTAGONIST).create_sprite(panel_positions.at(0)));
			portrait_sprites.back().set_bg_priority(1);
			portrait_sprites.push_back(characters::get_select_sprite(character_id::FLAME).create_sprite(panel_positions.at(1)));
			portrait_sprites.back().set_bg_priority(1);
			portrait_sprites.push_back(characters::get_select_sprite(character_id::WOOD).create_sprite(panel_positions.at(2)));
			portrait_sprites.back().set_bg_priority(1);
			portrait_sprites.push_back(characters::get_select_sprite(character_id::SHADOW).create_sprite(panel_positions.at(3)));
			portrait_sprites.back().set_bg_priority(1);
			portrait_sprites.push_back(characters::get_select_sprite(character_id::NONE).create_sprite(panel_positions.at(4)));
			portrait_sprites.back().set_bg_priority(1);
			portrait_sprites.push_back(characters::get_select_sprite(character_id::NONE).create_sprite(panel_positions.at(5)));
			portrait_sprites.back().set_bg_priority(1);
		}

		{
			bn::sprite_builder builder(bn::sprite_items::nametags);
			builder.set_bg_priority(1);
			builder.set_position(bn::fixed_point(68,60));
			_nametag_sprites.push_back(builder.build());
			_nametag_sprites.back().set_tiles(bn::sprite_items::nametags.tiles_item(), 4);
		}

		_char_sprite.set_position(bn::fixed_point(68,48));

		scene_management::set_next_scene(scene_type::BATTLE);
		
		for(int i = 0; i < _lev_sel_menu.item_list()->size(); i++)
		{
			_lev_sel_menu.item_list()->at(i)->hover_menu_item_event = update_char_display;
			_lev_sel_menu.item_list()->at(i)->click_menu_item_event = update_selected_character;
		}
		_lev_sel_menu.open_menu();

		// select_panel(4);

		scene_done = false;
		fade_from_black();

		while(!scene_done)
		{
			_lev_sel_menu.update();

			if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
			{
				if(_lev_sel_menu.get_active_cursor_id() == (NUM_CURSORS - 1))
				{
					scene_management::set_next_scene(scene_type::BATTLE);
					scene_done = true;
					fade_to_black();
					break;
				}
				else
				{
					_lev_sel_menu.increase_active_cursor();
				}
			}

			if(bn::keypad::b_pressed())
			{
				if(_lev_sel_menu.get_active_cursor_id() == 0)
				{
					scene_management::set_next_scene(scene_type::TITLE);
					scene_done = true;
					fade_to_black();
				}
				else
				{
					_lev_sel_menu.decrease_active_cursor();
				}
			}
			// bn::point mov;
			// if(bn::keypad::left_pressed())
			// {
			// 	mov.set_x(mov.x()-1);
			// }
			// else if(bn::keypad::right_pressed())
			// {
			// 	mov.set_x(mov.x()+1);
			// }
			// if(bn::keypad::up_pressed())
			// {
			// 	mov.set_y(mov.y()-1);
			// }
			// else if(bn::keypad::down_pressed())
			// {
			// 	mov.set_y(mov.y()+1);
			// }
			// if(mov.x() != 0 || mov.y() != 0)
			// {
			// 	select_panel(selected_panel + mov.x() + (mov.y() * 3));
			// }

			update();
		}
	}

	level_select_scene::~level_select_scene()
	{
		panel_positions.clear();
		portrait_frames.clear();
		portrait_sprites.clear();
		_nametag_sprites.clear();
	}

	void level_select_scene::update()
	{
		unsigned int burn = random.get();
		_lev_sel_bg.set_x(_lev_sel_bg.x() - 0.25);
		_lev_sel_bg.set_y(_lev_sel_bg.y() - 0.25);
		
		scene::update();
	}

	void level_select_scene::set_char_sprite(character_id char_id)
	{
		_char_sprite.set_character(char_id);
	}

	level_select_menu *level_select_scene::get_level_select_menu()
	{
		return &_lev_sel_menu;
	}




	/*******************************************************/



	void update_char_display(int item_id)
	{
		_instance_->set_char_sprite(characters::get_character(item_id));
		
	}
	

	void update_selected_character(int item_id)
	{
		int cursor = _instance_->get_level_select_menu()->get_active_cursor_id();
		if(cursor == 0)
		{
			characters::set_player_character(characters::get_character(item_id));
		}
		else
		{
			characters::set_foe_character(characters::get_character(item_id));
		}
	}


}