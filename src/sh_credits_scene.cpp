#include "sh_credits_scene.h"

#include <bn_keypad.h>
#include <bn_regular_bg_ptr.h>
#include <bn_regular_bg_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>

#include "bn_regular_bg_items_options_bg.h"
#include "bn_regular_bg_items_credits_fg.h"
#include "variable_8x16_sprite_font.h"

namespace sh
{
	credits_scene::credits_scene() :
		text_generator(common::variable_8x16_sprite_font)
	{
		type = scene_type::CREDITS;
		
		{
			bn::regular_bg_builder builder(bn::regular_bg_items::options_bg);
			bn::fixed_point pos(0,0);
			builder.set_priority(3);
			builder.set_position(pos);
			backgrounds.push_back(builder.release_build());
		}
		{
			bn::regular_bg_builder builder(bn::regular_bg_items::credits_fg);
			bn::fixed_point pos(0,0);
			builder.set_priority(2);
			builder.set_position(pos);
			backgrounds.push_back(builder.release_build());
		}
		{
			const int x_base = -80;
			const int x_indent = 24;
			const int y_base = -48;
			const int y_inc = 12;
			const int new_line = 18;

			text_sprites.clear();
			bn::fixed_point text_pos(0,-68);
			text_generator.set_bg_priority(2);
			text_generator.set_center_alignment();
			text_generator.generate(text_pos, "Credits", text_sprites);

			text_pos = bn::fixed_point(0,y_base);
			text_generator.generate(text_pos, "Created by SquishyFrogs", text_sprites);
			text_pos += bn::fixed_point(0, y_inc);
			text_generator.generate(text_pos, "squishyfrogs.itch.io", text_sprites);

			text_pos += bn::fixed_point(0, new_line);
			text_generator.generate(text_pos, "Powered by Butano 8.6.0", text_sprites);
			text_pos += bn::fixed_point(0, y_inc);
			text_generator.generate(text_pos, "github.com/GValiente/butano", text_sprites);

			text_pos += bn::fixed_point(0, new_line);
			text_generator.generate(text_pos, "Ice Blue Moon - Skyline", text_sprites);
			text_pos += bn::fixed_point(0, y_inc);
			text_generator.generate(text_pos, "https://modarchive.org", text_sprites);

			text_pos += bn::fixed_point(0, new_line);
			text_generator.generate(text_pos, "Valium candies - Jakim", text_sprites);
			text_pos += bn::fixed_point(0, y_inc);
			text_generator.generate(text_pos, "https://modarchive.org", text_sprites);


		}






		fade_from_black();

		bool exit_scene = false;
		while(!exit_scene)
		{
			if(bn::keypad::b_pressed())
			{
				scene_management::set_next_scene(scene_type::TITLE);
				exit_scene = true;
				fade_to_black();
			}

			update();
		}
	}


	credits_scene::~credits_scene()
	{
		text_sprites.clear();
	}


	void credits_scene::update()
	{
		backgrounds.front().set_x(backgrounds.front().x() - 0.25);
		backgrounds.front().set_y(backgrounds.front().y() - 0.25);
		
		scene::update();
	}
}