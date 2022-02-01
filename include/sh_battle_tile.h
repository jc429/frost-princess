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

	enum class tile_condition{
		NORMAL	= 0,
		FROZEN	= 1,
		BURNED	= 2,
		SHOCKED	= 3,
		VINE_H	= 4,
		VINE_V	= 5,
		BLIND	= 6,
	};

	class battle_tile
	{
	private:
		tile_owner owner;
		bool _is_dark;
		bool _is_base;
		bn::sprite_ptr *sprite_ptr;
		bn::fixed_point _position;
		bn::fixed_point _sprite_offset;
		battle_tile *neighbors[4];
		tile_condition _current_condition;
		int _condition_timer;

	public:
		int tile_id;
		bn::point coordinates;

		battle_tile(int id);
		void turn_update();
		void update_sprite();

		void copy_properties(battle_tile *other);

		void set_dark(bool dark);
		bool is_dark();
		void set_base(bool base);
		bool is_base();
		
		void set_position(int x, int y);
		void set_position(bn::fixed_point pos);
		bn::fixed_point get_position();

		void set_sprite_offset(bn::fixed_point offset);
		bn::fixed_point get_sprite_offset();

		void set_sprite_position(bn::fixed_point pos);
		bn::fixed_point get_sprite_position();

		void set_owner(tile_owner owner_id);
		tile_owner get_owner();

		void set_neighbor(direction dir, battle_tile *tile);
		battle_tile* get_neighbor(direction dir);
		battle_tile* get_neighbor(int dir);

		void set_condition(tile_condition condition, int duration);
		void clear_condition();
		tile_condition get_condition();

		void set_sprite_ptr(bn::sprite_ptr *ptr);
		void clear_sprite_ptr();
		bn::sprite_ptr *get_sprite();
	};


}


#endif //SH_BATTLE_TILE_H