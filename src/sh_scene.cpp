#include "sh_scene.h"

#include "bn_blending.h"
#include "bn_blending_fade_alpha.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_math.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

namespace sh
{


	
	void scene::fade_to_black()
	{
		bn::sprite_palettes_fade_to_action sprite_fade(10, 1);
		bn::bg_palettes_fade_to_action bg_fade(10, 1);
		while((!bg_fade.done()) || (!sprite_fade.done()))
		{
			if(!bg_fade.done())
				bg_fade.update();
			if(!sprite_fade.done())
				sprite_fade.update();
			bn::core::update();
		}
	}


	void scene::fade_from_black()
	{
		bn::sprite_palettes_fade_to_action sprite_fade(10, 0);
		bn::bg_palettes_fade_to_action bg_fade(10, 0);
		while((!bg_fade.done()) || (!sprite_fade.done()))
		{
			if(!bg_fade.done())
				bg_fade.update();
			if(!sprite_fade.done())
				sprite_fade.update();
			bn::core::update();
		}
	}

	void scene::wait_for_update_cycles(int num_updates)
	{
		for(int i = 0; i < num_updates; i++)
		{
			update();
		}
	}

}