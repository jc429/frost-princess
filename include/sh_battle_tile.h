#ifndef SH_BATTLE_TILE_H
#define SH_BATTLE_TILE_H

#include "bn_sprite_ptr.h"

#define DIRECTION_N 0
#define DIRECTION_E 1
#define DIRECTION_S 2
#define DIRECTION_W 3


namespace sh
{
	enum class tile_owner{
		EMPTY	= 0,
		PLAYER	= 1,
		FOE		= 2
	};


	class battle_tile
	{
	private:
		tile_owner owner;
		bool is_dark;


	public:
		bn::sprite_ptr sprite;
		battle_tile* neighbors[4];
		

		battle_tile();
		void set_position(int x, int y);
		bn::fixed_point get_position();
		void set_dark(bool dark);
		void set_owner(tile_owner owner_id);
		void update_sprite();
	};


}


#endif //SH_BATTLE_TILE_H