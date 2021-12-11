#include "sh_title_scene.h"

#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_regular_bg_item.h"
#include "bn_sin_lut.h"

#include "bn_regular_bg_items_title_bg.h"
#include "bn_regular_bg_items_title_snow.h"

#include "bn_blending_fade_alpha.h"

namespace sh
{

	title_scene::title_scene() :
		title_bg (bn::regular_bg_items::title_bg.create_bg(0, 0)),
		title_snow (bn::regular_bg_items::title_snow.create_bg(0, 0))
	{
		type = scene_type::TITLE;

		title_bg.set_priority(3);
		title_bg.set_blending_enabled(false);

		title_snow.set_priority(2);
		title_snow.set_blending_enabled(false);
		

		set_next_scene(scene_type::BATTLE);
		
		fade_from_black();

		while(true)
		{
			if(bn::keypad::any_pressed())
			{
				fade_to_black();
				break;
			}

			bn::core::update();
		}
	}


}
