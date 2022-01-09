#ifndef SH_BATTLE_PORTRAIT_H
#define SH_BATTLE_PORTRAIT_H

#include "sh_character.h"

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>

namespace sh
{
	class battle_portrait
	{
	public:
		bn::sprite_ptr portrait_sprite;
	//	bn::sprite_ptr frame_sprite;
		// int player_id;
		bn::fixed_point _position;
		character_id char_id;

		battle_portrait(bn::fixed_point pos);
		battle_portrait(int pos_x, int pos_y);
		void set_character(character_id c_id);

	};
}


#endif //SH_BATTLE_PORTRAIT_H