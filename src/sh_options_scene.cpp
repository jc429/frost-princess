#include "sh_options_scene.h"
#include "sh_game_settings.h"
#include "sh_menu.h"
#include "sh_debug.h"

#include <bn_keypad.h>
#include <bn_regular_bg_builder.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>

#include "bn_regular_bg_items_options_bg.h"
#include "bn_regular_bg_items_options_fg.h"
#include "bn_sprite_items_menu_arrow.h"
#include "variable_8x16_sprite_font.h"


namespace sh
{
	options_scene::options_scene() :
		text_generator(common::variable_8x16_sprite_font)
	{
		type = scene_type::OPTIONS;
		scene_done = false;
		
		{
			bn::regular_bg_builder builder(bn::regular_bg_items::options_bg);
			bn::fixed_point pos(0,0);
			builder.set_priority(3);
			builder.set_position(pos);
			backgrounds.push_back(builder.release_build());
		}
		{
			bn::regular_bg_builder builder(bn::regular_bg_items::options_fg);
			bn::fixed_point pos(0,0);
			builder.set_priority(2);
			builder.set_position(pos);
			backgrounds.push_back(builder.release_build());
		}


		// bn::fixed_point slider_pos(11, -35);
		// menu_sliders.push_back(menu_slider(slider_pos, 0, 9));
		// menu_sliders.back().set_value(game_settings::get_music_volume());
		// slider_pos.set_y(slider_pos.y() + 16);
		// menu_sliders.push_back(menu_slider(slider_pos, 0, 9));
		// menu_sliders.back().set_value(game_settings::get_sfx_volume());



		// text_sprites.clear();
		// bn::fixed_point text_pos(-80,-37);
		// text_generator.set_bg_priority(2);
		// text_generator.set_left_alignment();
		// text_generator.generate(text_pos, "Music Volume", text_sprites);
		// text_pos.set_y(text_pos.y() + _y_inc);
		// text_generator.generate(text_pos, "SFX Volume", text_sprites);
		// text_pos.set_y(text_pos.y() + _y_inc);
		// text_generator.generate(text_pos, "Reset to Default", text_sprites);

		// text_pos = bn::fixed_point(0,40);
		// text_generator.set_center_alignment();
		// // text_generator.generate(text_pos, "Clear Data", text_sprites);

		// select_option_row(0);
		menu options_menu(menu_type::OPTIONS_MENU, 0);
		options_menu.open_menu();

		fade_from_black();

		while(!scene_done)
		{
			options_menu.update();
			if(bn::keypad::b_pressed())
			{
				// save settings
				scene_management::set_next_scene(scene_type::TITLE);
				text_sprites.clear();
				scene_done = true;
				fade_to_black();
			}

			if(debug::DEBUG_ENABLED && bn::keypad::l_pressed())
			{
				if(options_menu.is_open())
				{
					options_menu.close_menu();
				}
				else
				{
					options_menu.open_menu();
				}
			}
			update();
		}

	}

	options_scene::~options_scene()
	{
	}

	void options_scene::update()
	{
		backgrounds.front().set_x(backgrounds.front().x() - 0.25);
		backgrounds.front().set_y(backgrounds.front().y() - 0.25);
		
		scene::update();
	}

}