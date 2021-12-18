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
#define COLORBLIND_TILE_OFFSET 8


namespace sh{

	battle_tile::battle_tile(int id)
	{
		tile_id = id;
		owner = tile_owner::EMPTY;
		for(int i = 0; i < 4; i++)
		{
			neighbors[i] = NULL;
		}
		// if(sprite_ptr != NULL)
		// {
		// 	sprite_ptr->set_bg_priority(2);
		// 	sprite_ptr->set_z_order(500);
		// }
	}

	void battle_tile::set_position(int x, int y)
	{
		_position = bn::fixed_point(x,y);
		if(sprite_ptr != NULL)
			sprite_ptr->set_position(x, y);
	}
	bn::fixed_point battle_tile::get_position()
	{
		return _position;
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
		if(sprite_ptr != NULL)
		{
			sprite_ptr->set_position(_position);
			int tile_idx = (int)owner * 2;
			tile_idx += (is_dark ? 1 : 0);
			tile_idx += (COLORBLIND_MODE ? COLORBLIND_TILE_OFFSET : 0);
			sprite_ptr->set_tiles(bn::sprite_items::board_tile.tiles_item().create_tiles(tile_idx));
		}
	}
	
	void battle_tile::set_sprite_ptr(bn::sprite_ptr *ptr)
	{
		sprite_ptr = ptr;
	}

	void battle_tile::clear_sprite_ptr()
	{
		sprite_ptr = NULL;
	}

	bn::sprite_ptr *battle_tile::get_sprite()
	{
		return sprite_ptr;
	}
}