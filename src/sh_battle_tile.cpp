#include "sh_battle_tile.h"
#include "sh_direction.h"

#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_tiles.h>
#include <bn_sprite_tiles_ptr.h>
#include <bn_sprite_palette_ptr.h>

#include "bn_sprite_items_board_tile.h"
#include "bn_sprite_items_board_tile_frozen.h"
#include "bn_sprite_items_crown.h"

#define COLORBLIND_MODE true
#define COLORBLIND_TILE_OFFSET 8


namespace sh{

	battle_tile::battle_tile(int id)
	{
		tile_id = id;
		owner = tile_owner::EMPTY;
		_current_condition = tile_condition::NORMAL;
		_condition_timer = 0;
		_sprite_offset = bn::fixed_point(0,0);
		for(int i = 0; i < 4; i++)
		{
			neighbors[i] = NULL;
		}
	}

	void battle_tile::turn_update()
	{
		if(_condition_timer > 0)
		{
			_condition_timer--;
			if(_condition_timer == 0)
			{
				clear_condition();
			}
		}
		update_sprite();
	}

	void battle_tile::set_position(int x, int y)
	{
		_position = bn::fixed_point(x,y);
		if(sprite_ptr != NULL)
			sprite_ptr->set_position(_position + _sprite_offset);
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

	void battle_tile::set_neighbor(direction dir, battle_tile *tile)
	{
		neighbors[directions::dir_to_int(dir)] = tile;
	}

	void battle_tile::set_base(bool base)
	{
		is_base = base;
	}

	void battle_tile::set_condition(tile_condition condition, int duration)
	{
		_current_condition = condition;
		_condition_timer = duration;
		update_sprite();
	}

	void battle_tile::clear_condition()
	{
		_current_condition = tile_condition::NORMAL;
		_condition_timer = 0;
		update_sprite();
	}

	tile_condition battle_tile::get_condition()
	{
		return _current_condition;
	}

	void battle_tile::update_sprite()
	{
		const bn::sprite_tiles_item *tile_set;
		const bn::sprite_palette_item *palette;
		int tile_idx = (int)owner * 2;
		tile_idx += (is_dark ? 1 : 0);
		tile_idx += (COLORBLIND_MODE ? COLORBLIND_TILE_OFFSET : 0);
		switch(_current_condition)
		{
		case tile_condition::FROZEN:
			_sprite_offset = bn::fixed_point(0,-2);
			tile_set = &bn::sprite_items::board_tile_frozen.tiles_item();
			palette =  &bn::sprite_items::board_tile_frozen.palette_item();
			tile_idx = bn::max((3 - _condition_timer), 0);
			break;
		default:
			_sprite_offset = bn::fixed_point(0,0);
			tile_set = &bn::sprite_items::board_tile.tiles_item();
			palette =  &bn::sprite_items::board_tile.palette_item();
			break;
		}

		if(sprite_ptr != NULL)
		{
			sprite_ptr->set_tiles_and_palette(tile_set->create_tiles(tile_idx), palette->create_palette());
			sprite_ptr->set_position(_position + _sprite_offset);
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