#include "sh_title_scene.h"
#include "sh_audio.h"
#include "sh_random.h"

#include <bn_blending_fade_alpha.h>
#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_regular_bg_item.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_vector.h>

#include "bn_regular_bg_items_title_bg.h"
#include "bn_regular_bg_items_title_overlay.h"
#include "bn_regular_bg_items_title_snow.h"
#include "variable_8x16_sprite_font.h"

#include "bn_sprite_items_title_cursor.h"


namespace sh
{
	


	title_scene::title_scene() :
		title_bg (bn::regular_bg_items::title_bg.create_bg(0, 0)),
		title_snow_a (bn::regular_bg_items::title_snow.create_bg(0, 0)),
		title_snow_b (bn::regular_bg_items::title_snow.create_bg(0, 0)),
		title_overlay (bn::regular_bg_items::title_overlay.create_bg(0, 0)),
		text_generator(common::variable_8x16_sprite_font)
	{
		type = scene_type::TITLE;
		_current_state = title_state::PRESS_START;

		// set up bgs
		title_snow_a.set_priority(0);
		title_overlay.set_priority(1);
		title_snow_b.set_priority(2);
		title_bg.set_priority(3);
		title_snow_a.set_x(24);
		title_snow_a.set_y(8);

		// set up text
		text_generator.set_bg_priority(0);
		text_generator.set_z_order(0);
		text_generator.set_center_alignment();

		bn::sprite_builder builder(bn::sprite_items::title_cursor);
		bn::fixed_point pos(-35, 40);
		builder.set_bg_priority(0);
		builder.set_z_order(-500);
		builder.set_position(pos);
		title_sprites.push_back(builder.release_build());
		title_cursor = &title_sprites.back();
		title_cursor->set_visible(false);
		
		// Skip Level Select for Demo
		scene_management::set_next_scene(scene_type::BATTLE);
		// scene_management::set_next_scene(scene_type::LEVEL_SELECT);

		audio::play_track(track_id::TITLE);
		
		fade_from_black();

		wait_for_update_cycles(12);
		set_title_state(title_state::PRESS_START);

		scene_done = false;
		static unsigned int burn;
		bn::fixed_point debug_pos(0,0);
		while(!scene_done)
		{
			burn = random.get();
			
			
			switch(_current_state)
			{
			case title_state::PRESS_START:
				if(bn::keypad::start_pressed() || bn::keypad::a_pressed())
				{
					set_title_state(title_state::MAIN_MENU);
				}
				break;
				
			case title_state::MAIN_MENU:
				if(bn::keypad::down_pressed())
				{
					set_cursor_selection(current_menu_selection + 1);
				}
				else if(bn::keypad::up_pressed())
				{
					set_cursor_selection(current_menu_selection - 1);
				}
				if(bn::keypad::a_pressed())
				{

					switch(current_menu_selection)
					{
					case 0:
						scene_management::set_next_scene(scene_type::BATTLE);
						scene_done = true;
						break;
					case 1:
						scene_management::set_next_scene(scene_type::OPTIONS);
						scene_done = true;
						break;
					case 2:
						scene_management::set_next_scene(scene_type::CREDITS);
						scene_done = true;
						break;

					default:
						break;
					}
					if(scene_done)
					{
						text_sprites.clear();
						audio::stop_current_track();
						fade_to_black();
					}
				}
				else if(bn::keypad::b_pressed())
				{
					set_title_state(title_state::PRESS_START);
				}
				break;
			default:
				break;
			}

			

			update();
		}
	}


	void title_scene::update()
	{
		
		title_snow_a.set_y(title_snow_a.y() + 0.5);
		title_snow_a.set_x(title_snow_a.x() - random.get_fixed(0,1));
		title_snow_b.set_y(title_snow_b.y() + 0.25);
		title_snow_b.set_x(title_snow_b.x() - 2 + random.get_fixed(0, 1));
		
		scene::update();
	}

	void title_scene::set_title_state(title_state state)
	{
		_current_state = state;
		bn::fixed_point text_pos;
		switch (_current_state)
		{
		case title_state::PRESS_START:
			title_cursor->set_visible(false);
			text_sprites.clear();
			text_generator.set_center_alignment();
			text_pos = bn::fixed_point(0, 60);
			text_generator.generate(text_pos, "Press Start", text_sprites);
			break;
		case title_state::MAIN_MENU:
			text_sprites.clear();
			title_menu_cursor_pos.clear();
			text_generator.set_left_alignment();
			text_pos = bn::fixed_point(-25, 36);
			// title_menu_cursor_pos.push_back(bn::fixed_point(text_pos.x() - 10, text_pos.y()));
			// text_generator.generate(text_pos, "Continue", text_sprites);
			// text_pos += bn::fixed_point(0,11);
			title_menu_cursor_pos.push_back(bn::fixed_point(text_pos.x() - 10, text_pos.y()));
			text_generator.generate(text_pos, "New Game", text_sprites);
			text_pos += bn::fixed_point(0,11);
			title_menu_cursor_pos.push_back(bn::fixed_point(text_pos.x() - 10, text_pos.y()));
			text_generator.generate(text_pos, "Options", text_sprites);
			text_pos += bn::fixed_point(0,11);
			title_menu_cursor_pos.push_back(bn::fixed_point(text_pos.x() - 10, text_pos.y()));
			text_generator.generate(text_pos, "Credits", text_sprites);
			set_cursor_selection(0);
			title_cursor->set_visible(true);
			break;
		default:
			break;
		}
	}

	void title_scene::set_cursor_selection(int selection)
	{
		current_menu_selection = ((selection + TITLE_MENU_ITEMS) % TITLE_MENU_ITEMS);
		if(current_menu_selection < title_menu_cursor_pos.size())
		{
			title_cursor->set_position(title_menu_cursor_pos.at(current_menu_selection));
		}

	}

}
