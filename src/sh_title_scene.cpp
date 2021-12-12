#include "sh_title_scene.h"

#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_regular_bg_item.h"
#include "bn_sin_lut.h"

#include "bn_regular_bg_items_title_bg.h"
#include "bn_regular_bg_items_title_snow.h"

#include "bn_blending_fade_alpha.h"

#include "sh_random.h"

namespace sh
{

	title_scene::title_scene() :
		title_bg (bn::regular_bg_items::title_bg.create_bg(0, 0)),
		title_snow_a (bn::regular_bg_items::title_snow.create_bg(0, 0)),
		title_snow_b (bn::regular_bg_items::title_snow.create_bg(0, 0))
	{
		type = scene_type::TITLE;

		title_snow_a.set_priority(1);
		title_snow_b.set_priority(2);
		title_bg.set_priority(3);
		title_snow_a.set_x(24);
		title_snow_a.set_y(8);

		

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


	void title_scene::update()
	{
		
		title_snow_a.set_y(title_snow_a.y() + 0.5);
		title_snow_a.set_x(title_snow_a.x() - random.get_fixed(0,1));
		title_snow_b.set_y(title_snow_b.y() + 0.25);
		title_snow_b.set_x(title_snow_b.x() - 2 + random.get_fixed(0, 1));
		
		bn::core::update();
	}

}
