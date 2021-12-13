#include "sh_battle_card.h"
#include "sh_action_manager.h"

#include <bn_fixed.h>
#include <bn_sprite_tiles.h>
#include <bn_sprite_tiles_ptr.h>

#include "bn_sprite_items_card_blank.h"
#include "bn_sprite_items_tile_patterns.h"

namespace sh
{



	battle_card::battle_card(int x, int y) :
		_card_sprite (bn::sprite_items::card_blank.create_sprite(x,y)),
		_pattern_sprite (bn::sprite_items::tile_patterns.create_sprite(x,y)),
		_anim_flip (bn::create_sprite_animate_action_once(_card_sprite, 2, bn::sprite_items::card_blank.tiles_item(), 0, 0, 0, 0, 0, 0, 0, 0, 0))
	{
		_position = bn::point(x,y);
		_is_faceup = false;
		_card_sprite.set_bg_priority(1);
		_card_sprite.set_z_order(450);
		_pattern_sprite.set_bg_priority(1);
		_pattern_sprite.set_z_order(440);
		set_pattern(tile_pattern::SINGLE);
		set_pattern(tile_patterns::random_tile_pattern());

		set_facedown_immediate();
	}
	
	void battle_card::update()
	{
		if(!_anim_flip.done())
		{
			_anim_flip.update();
			if(_anim_flip.done() && _is_faceup)
			{
				_pattern_sprite.set_visible(true);
			}
		}
		
	}
	


	void battle_card::set_position(bn::fixed_point pos)
	{
		_position = pos;
		_card_sprite.set_position(pos);
		_pattern_sprite.set_position(pos);
	}

	bn::fixed_point battle_card::get_position()
	{
		return _position;
	}

	void battle_card::move_to_destination(bn::fixed_point pos)
	{
		_position = pos;
		action_manager::register_move_action(_card_sprite, 40, pos);
		action_manager::register_move_action(_pattern_sprite, 40, pos);
	}

	void battle_card::set_pattern(tile_pattern pattern)
	{
		_current_pattern = pattern;
		int tile_idx = tile_patterns::get_tile_index(pattern);
		_pattern_sprite.set_tiles(bn::sprite_items::tile_patterns.tiles_item().create_tiles(tile_idx));
	}

	tile_pattern battle_card::get_pattern()
	{
		return _current_pattern;
	}

	void battle_card::flip()
	{
		_pattern_sprite.set_visible(false);
		_anim_flip = bn::create_sprite_animate_action_once(_card_sprite, 2, bn::sprite_items::card_blank.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7, 0);
		_is_faceup = !_is_faceup;
	}

	void battle_card::flip_facedown()
	{
		if(_is_faceup)
		{
			flip();
		}
	}

	void battle_card::flip_faceup()
	{
		if(!_is_faceup)
		{
			flip();
		}
	}

	void battle_card::set_facedown_immediate()
	{
		_is_faceup = false;
		_pattern_sprite.set_visible(false);
	}

	void battle_card::set_faceup_immediate()
	{
		_is_faceup = true;
		_pattern_sprite.set_visible(true);
	}

	void battle_card::discard()
	{
		bn::fixed_point pos = _position;
		pos.set_y(pos.y() + 40);
		//move_to_destination(pos);
		set_pattern(tile_patterns::random_tile_pattern());
	}
}