#ifndef SH_ACTION_MANAGER_H
#define SH_ACTION_MANAGER_H

#include <bn_sprite_actions.h>
#include <bn_vector.h>


namespace sh
{
	class action_manager
	{
	public:
		static bn::sprite_move_to_action *register_move_action(
			const bn::sprite_ptr& sprite,
			int duration_updates,
			const bn::fixed_point& final_position
		);
		static void clear_sprite_move_actions();
		static void update_sprite_move_actions();
	};
}


#endif //SH_ACTION_MANAGER_H
