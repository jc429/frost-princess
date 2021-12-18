#include "sh_scene.h"
#include "sh_level_select_scene.h"
#include "sh_random.h"

#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_regular_bg_ptr.h>

#include "bn_regular_bg_items_title_bg.h"
#include "bn_sprite_items_portrait_frame.h"

namespace sh
{


	level_select_scene::level_select_scene() :
		title_bg (bn::regular_bg_items::title_bg.create_bg(0, 0))
	{
		type = scene_type::LEVEL_SELECT;

		for(int y = -1; y < 2; y++)
		{
			for(int x = -1; x < 2; x++)
			{
				panel_positions.push_back(bn::fixed_point(x * 56, y * 48));
				portrait_frames.push_back(bn::sprite_items::portrait_frame.create_sprite(x * 56, y * 48));
			}
		}

		level_select_cursor.set_position(panel_positions.at(5));
		level_select_cursor.set_size(bn::point(48,40));

		set_next_scene(scene_type::BATTLE);
		fade_from_black();

		select_panel(5);

		while(true)
		{
			if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
			{
				fade_to_black();
				break;
			}

			bn::point mov;
			if(bn::keypad::left_pressed())
				{
					mov.set_x(mov.x()-1);
				}
				else if(bn::keypad::right_pressed())
				{
					mov.set_x(mov.x()+1);
				}
				if(bn::keypad::up_pressed())
				{
					mov.set_y(mov.y()-1);
				}
				else if(bn::keypad::down_pressed())
				{
					mov.set_y(mov.y()+1);
				}
				if(mov.x() != 0 || mov.y() != 0)
				{
					select_panel(selected_panel + mov.x() + (mov.y() * 3));
				}

			update();
		}
	}

	void level_select_scene::update()
	{
		unsigned int burn = random.get();
		level_select_cursor.update();
		bn::core::update();
	}

	void level_select_scene::select_panel(int panel_id)
	{
		selected_panel = (panel_id + 9) % 9;
		level_select_cursor.set_position(panel_positions.at(selected_panel));
	}


}