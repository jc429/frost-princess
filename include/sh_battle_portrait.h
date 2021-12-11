#ifndef SH_BATTLE_PORTRAIT_H
#define SH_BATTLE_PORTRAIT_H

#include "bn_sprite_ptr.h"

namespace sh
{
	class battle_portrait
	{
	public:
		bn::sprite_ptr portrait_sprite;
		bn::sprite_ptr frame_sprite;
		int player_id;

		battle_portrait(int pos_x, int pos_y);
		void set_player_id(int id);

		void set_blending_enabled(bool blending_enabled);
	};
}


#endif //SH_BATTLE_PORTRAIT_H