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

#include "bn_regular_bg_items_title_bg.h"
#include "bn_sprite_items_level_select_frame.h"

namespace sh
{


	level_select_scene::level_select_scene() :
		title_bg (bn::regular_bg_items::title_bg.create_bg(0, 0))
	{
		type = scene_type::LEVEL_SELECT;
		{
			bn::fixed_point pos(0,0);
			panel_positions.push_back(pos);
			pos = bn::fixed_point(-CORNER_X, -CORNER_Y);
			panel_positions.push_back(pos);
			pos = bn::fixed_point(CORNER_X, -CORNER_Y);
			panel_positions.push_back(pos);
			pos = bn::fixed_point(-MIDDLE_X, MIDDLE_Y);
			panel_positions.push_back(pos);
			pos = bn::fixed_point(MIDDLE_X, MIDDLE_Y);
			panel_positions.push_back(pos);
			pos = bn::fixed_point(-CORNER_X, CORNER_Y);
			panel_positions.push_back(pos);
			pos = bn::fixed_point(CORNER_X, CORNER_Y);
			panel_positions.push_back(pos);
		}
		{
			bn::sprite_builder builder(bn::sprite_items::level_select_frame);
			builder.set_z_order(10);
			for(int i = 0; i < 7; i++)
			{
				builder.set_position(panel_positions.at(i));
				portrait_frames.push_back(builder.build());
			}
			portrait_sprites.push_back(characters::get_select_sprite(character_id::PROTAGONIST).create_sprite(panel_positions.at(0)));
			portrait_sprites.push_back(characters::get_select_sprite(character_id::FLAME_WITCH).create_sprite(panel_positions.at(1)));
			portrait_sprites.push_back(characters::get_select_sprite(character_id::WOOD_DRUID).create_sprite(panel_positions.at(2)));
		}


		scene_management::set_next_scene(scene_type::BATTLE);
		fade_from_black();

		// select_panel(4);

		level_select_menu lev_sel_menu;
		lev_sel_menu.open_menu();

		while(true)
		{
			lev_sel_menu.update();

			if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
			{
				fade_to_black();
				break;
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
	}

	void level_select_scene::update()
	{
		unsigned int burn = random.get();
		bn::core::update();
	}

	// void level_select_scene::select_panel(int panel_id)
	// {
	// 	selected_panel = (panel_id + 9) % 9;
	// 	level_select_cursor.set_position(panel_positions.at(selected_panel));
	// }


}