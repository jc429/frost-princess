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
#include "bn_sprite_items_board_tile_burned.h"
#include "bn_sprite_items_crown.h"

#define COLORBLIND_MODE true
#define COLORBLIND_TILE_OFFSET 8


namespace sh{

	battle_tile::battle_tile(int id)
	{
		tile_id = id;
		owner = tile_owner::EMPTY;
		_current_condition = tile_condition::NORMAL;
		_is_base = false;
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
	
	void battle_tile::update_sprite()
	{
		const bn::sprite_tiles_item *tile_set;
		const bn::sprite_palette_item *palette;
		int tile_idx = (int)owner * 2;
		tile_idx += (_is_dark ? 1 : 0);
		tile_idx += (COLORBLIND_MODE ? COLORBLIND_TILE_OFFSET : 0);
		switch(_current_condition)
		{
		case tile_condition::BURNED:
			_sprite_offset = bn::fixed_point(0,0);
			tile_set = &bn::sprite_items::board_tile_burned.tiles_item();
			palette =  &bn::sprite_items::board_tile_burned.palette_item();
			tile_idx = 0;
			break;
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

	void battle_tile::copy_properties(battle_tile *other)
	{
		sprite_ptr = other->get_sprite();
		_sprite_offset = other->get_sprite_offset();
		owner = other->get_owner();
	}
	
	void battle_tile::set_dark(bool dark)
	{
		_is_dark = dark;
		update_sprite();
	}
	bool battle_tile::is_dark()
	{
		return _is_dark;
	}

	void battle_tile::set_base(bool base)
	{
		_is_base = base;
		update_sprite();
	}

	bool battle_tile::is_base()
	{
		return _is_base;
	}

	void battle_tile::set_position(int x, int y)
	{
		bn::fixed_point pos = bn::fixed_point(x,y);
		set_position(pos);
	}

	void battle_tile::set_position(bn::fixed_point pos)
	{
		_position = pos;
		set_sprite_position(pos);
	}
	
	bn::fixed_point battle_tile::get_position()
	{
		return _position;
	}

	void battle_tile::set_sprite_offset(bn::fixed_point offset)
	{
		_sprite_offset = offset;
	}

	bn::fixed_point battle_tile::get_sprite_offset()
	{
		return _sprite_offset;
	}
	
	void battle_tile::set_sprite_position(bn::fixed_point pos)
	{
		if(sprite_ptr != NULL)
			sprite_ptr->set_position(pos + _sprite_offset);
	}
	
	bn::fixed_point battle_tile::get_sprite_position()
	{
		return _position + _sprite_offset;
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

	void battle_tile::set_neighbor(direction dir, battle_tile *tile)
	{
		neighbors[directions::dir_to_int(dir)] = tile;
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

	void battle_tile::set_condition(tile_condition condition, int duration)
	{
		if(!_is_base)
		{
			_current_condition = condition;
			_condition_timer = duration;
		}
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