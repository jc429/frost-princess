#include "sh_effects.h"

#include <bn_sprite_animate_actions.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "bn_sprite_items_effect_shine.h"

namespace sh
{

	namespace effects
	{

		static bn::vector<bn::sprite_animate_action<8>, 12> effect_animations;


		bn::sprite_ptr create_effect_at_position(effect_id effect, bn::fixed_point pos)
		{
			bn::sprite_builder builder(bn::sprite_items::effect_shine);
			builder.set_bg_priority(1);
			builder.set_z_order(-500);
			builder.set_position(pos);
			bn::sprite_ptr spr = builder.release_build();
			effect_animations.push_back(bn::create_sprite_animate_action_once(spr, 2, bn::sprite_items::effect_shine.tiles_item(), 0,1,2,3,4,5,6,7));
			return spr;
		}

		void update_effect_animations()
		{
			for(auto it = effect_animations.begin(), end = effect_animations.end(); it != end; )
			{
				bn::sprite_animate_action<8> &action = *it;
				action.update();

				if(action.done())
				{
					it = effect_animations.erase(it);
					end = effect_animations.end();
				}
				else
				{
					++it;
				}
			}
		}
	}
}