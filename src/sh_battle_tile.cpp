#include "sh_battle_tile.h"
#include "sh_direction.h"

#include <bn_sprite_ptr.h>
#include <bn_sprite_tiles.h>
#include <bn_sprite_tiles_ptr.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>

#include "bn_sprite_items_board_tile.h"
#include "bn_sprite_items_crown.h"

#define COLORBLIND_MODE true

namespace sh{

	battle_tile::battle_tile(int id) :
		sprite (bn::sprite_items::board_tile.create_sprite((bn::fixed)0, (bn::fixed)0))
	{
		tile_id = id;
		owner = tile_owner::EMPTY;
		for(int i = 0; i < 4; i++)
		{
			neighbors[i] = NULL;
		}
		sprite.set_bg_priority(2);
		sprite.set_z_order(500);
	}

	void battle_tile::set_position(int x, int y)
	{
		sprite.set_position(x, y);
	}
	bn::fixed_point battle_tile::get_position()
	{
		return sprite.position();
	}

	void battle_tile::set_dark(bool dark)
	{
		is_dark = dark;
		update_sprite();
	}

	void battle_tile::set_owner(tile_owner owner_id)
	{
		owner = owner_id;
		update_sprite();
	}

	tile_owner battle_tile::get_owner()
	{
		return owner;
	}
	
	battle_tile* battle_tile::get_neighbor(direction dir)
	{
		return get_neighbor(directions::dir_to_int(dir));
	}

	battle_tile* battle_tile::get_neighbor(int dir)
	{
		dir = bn::clamp(dir, 0, 4);
		return neighbors[dir];
	}

	void battle_tile::set_base(bool base)
	{
		is_base = base;
	}

	void battle_tile::update_sprite()
	{
		int tile_idx = (int)owner * 2;
		tile_idx += (is_dark ? 1 : 0);
		tile_idx += (COLORBLIND_MODE ? 8 : 0);
		sprite.set_tiles(bn::sprite_items::board_tile.tiles_item().create_tiles(tile_idx));
	}
}