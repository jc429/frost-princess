#include "sh_action_manager.h"

#include <bn_sprite_actions.h>
#include <bn_vector.h>

namespace sh{

	static bn::vector<bn::sprite_move_to_action, 16> sprite_move_actions;

	bn::sprite_move_to_action *action_manager::register_move_action(
		const bn::sprite_ptr& sprite,
		int duration_updates,
		const bn::fixed_point& final_position
	)
	{
		sprite_move_actions.push_back(bn::sprite_move_to_action(sprite, duration_updates, final_position));
		return &sprite_move_actions.back();
	}

	void action_manager::clear_sprite_move_actions()
	{
		sprite_move_actions.clear();
	}
	
	void action_manager::update_sprite_move_actions()
	{
		for(auto it = sprite_move_actions.begin(), end = sprite_move_actions.end(); it != end; )
		{
			bn::sprite_move_to_action &action = *it;
			action.update();

			if(action.done())
			{
				it = sprite_move_actions.erase(it);
				end = sprite_move_actions.end();
			}
			else
			{
				++it;
			}
		}
	}
}