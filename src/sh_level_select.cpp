#include "sh_scene.h"
#include "sh_level_select.h"

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


		for(int i = -1; i < 2; i++)
		{
			for(int j = -1; j < 2; j++)
			{
				portrait_frames.push_back(bn::sprite_items::portrait_frame.create_sprite(i * 56, j * 48));
			}
		}



		set_next_scene(scene_type::BATTLE);
		fade_from_black();

		while(true)
		{
			if(bn::keypad::any_pressed())
			{
				fade_to_black();
				break;
			}

			update();
		}
	}

	void level_select_scene::update()
	{
		
		bn::core::update();
	}

}