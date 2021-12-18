#ifndef SH_BATTLE_TILE_H
#define SH_BATTLE_TILE_H

#include "sh_direction.h"

#include <bn_sprite_ptr.h>
#include <bn_point.h>
#include <bn_fixed_point.h>


namespace sh
{
	enum class tile_owner{
		EMPTY	= 0,
		PLAYER	= 1,
		FOE		= 2,
		DEBUG	= 3
	};

	class battle_tile
	{
	private:
		tile_owner owner;
		bool is_dark;
		bool is_base;
		bn::sprite_ptr *sprite_ptr;
		bn::fixed_point _position;

	public:
		int tile_id;
		bn::point coordinates;
		//bn::sprite_ptr sprite;
		battle_tile *neighbors[4];
		

		battle_tile(int id);
		void set_position(int x, int y);
		bn::fixed_point get_position();
		void set_dark(bool dark);
		void set_owner(tile_owner owner_id);
		tile_owner get_owner();
		battle_tile* get_neighbor(direction dir);
		battle_tile* get_neighbor(int dir);
		void set_base(bool base);
		void update_sprite();
		void set_sprite_ptr(bn::sprite_ptr *ptr);
		void clear_sprite_ptr();
		bn::sprite_ptr *get_sprite();
	};


}


#endif //SH_BATTLE_TILE_H