#include "sh_scene.h"
#include "sh_level_select.h"

#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_regular_bg_ptr.h>

#include "bn_regular_bg_items_title_bg.h"

namespace sh
{


	level_select_scene::level_select_scene() :
		title_bg (bn::regular_bg_items::title_bg.create_bg(0, 0))
	{
		type = scene_type::LEVEL_SELECT;
		
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