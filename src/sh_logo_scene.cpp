#include "sh_logo_scene.h"

#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_items_logo_bg.h"

namespace sh
{

	logo_scene::logo_scene() :
		logo_bg (bn::regular_bg_items::logo_bg.create_bg(0, 0))
	{
		type = scene_type::LOGO;

		set_next_scene(scene_type::TITLE);
		
		logo_bg.set_blending_enabled(true);
		fade_from_black();
		while(true)
		{
			if(bn::keypad::any_pressed())
			{
				break;
			}

			bn::core::update();
		}
		fade_to_black();
	}

}
