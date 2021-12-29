#include "sh_options_scene.h"
#include "sh_game_settings.h"

#include <bn_core.h>
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

		bn::fixed_point slider_pos(11, -35);
		menu_sliders.push_back(menu_slider(slider_pos, 0, 9));
		menu_sliders.back().set_value(game_settings::get_music_volume());
		slider_pos.set_y(slider_pos.y() + 16);
		menu_sliders.push_back(menu_slider(slider_pos, 0, 9));
		menu_sliders.back().set_value(game_settings::get_sfx_volume());



		text_sprites.clear();
		bn::fixed_point text_pos(-80,-37);
		text_generator.set_bg_priority(2);
		text_generator.set_left_alignment();
		text_generator.generate(text_pos, "Music Volume", text_sprites);
		text_pos.set_y(text_pos.y() + _y_inc);
		text_generator.generate(text_pos, "SFX Volume", text_sprites);
		text_pos.set_y(text_pos.y() + _y_inc);
		text_generator.generate(text_pos, "Reset to Default", text_sprites);

		text_pos = bn::fixed_point(0,40);
		text_generator.set_center_alignment();
		text_generator.generate(text_pos, "Clear Data", text_sprites);

		select_option_row(0);

		fade_from_black();

		bool exit_scene = false;
		while(!exit_scene)
		{
			if(bn::keypad::b_pressed())
			{
				// save settings
				game_settings::set_music_volume(menu_sliders.at(0).get_value());
				game_settings::set_sfx_volume(menu_sliders.at(1).get_value());
				scene_management::set_next_scene(scene_type::TITLE);
				text_sprites.clear();
				exit_scene = true;
				fade_to_black();
			}

			if(bn::keypad::down_pressed())
			{
				selected_row = (selected_row + 1) % _row_ct;
			}
			else if(bn::keypad::up_pressed())
			{
				selected_row = (selected_row + _row_ct - 1) % _row_ct;
			}

			if(bn::keypad::right_pressed())
			{
				int slider_val = menu_sliders.at(selected_row).increment();
			}
			else if(bn::keypad::left_pressed())
			{
				int slider_val = menu_sliders.at(selected_row).decrement();
			}

			update();
		}
	}

	options_scene::~options_scene()
	{
		menu_sliders.clear();
	}

	void options_scene::update()
	{
		backgrounds.front().set_x(backgrounds.front().x() - 0.25);
		backgrounds.front().set_y(backgrounds.front().y() - 0.25);
		bn::core::update();
	}

	void options_scene::select_option_row(int row)
	{
		selected_row = (row + _row_ct) % _row_ct;
	}
}