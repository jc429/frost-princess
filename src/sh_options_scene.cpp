#include "sh_options_scene.h"

#include <bn_core.h>
#include <bn_keypad.h>
#include <bn_regular_bg_ptr.h>
#include <bn_regular_bg_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>

#include "bn_regular_bg_items_options_bg.h"
#include "bn_regular_bg_items_options_fg.h"
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






		fade_from_black();

		bool exit_scene = false;
		while(!exit_scene)
		{
			if(bn::keypad::b_pressed())
			{
				set_next_scene(scene_type::TITLE);
				exit_scene = true;
				fade_to_black();
			}

			update();
		}
	}



	void options_scene::update()
	{
		backgrounds.front().set_x(backgrounds.front().x() - 0.25);
		backgrounds.front().set_y(backgrounds.front().y() - 0.25);
		bn::core::update();
	}
}