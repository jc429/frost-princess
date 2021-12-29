#include "sh_battle_card.h"
#include "sh_action_manager.h"

#include <bn_fixed.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_tiles.h>
#include <bn_sprite_tiles_ptr.h>

#include "bn_sprite_items_card_blank.h"
#include "bn_sprite_items_tile_patterns.h"

namespace sh
{


	battle_card::battle_card(bn::fixed_point pos) 
	{
		_position = pos;
		_is_faceup = false;
		_is_flipping = false;

		{
			bn::sprite_builder builder(bn::sprite_items::card_blank);
			builder.set_bg_priority(2);
			builder.set_z_order(-10);
			builder.set_position(pos);
			_sprites.push_back(builder.release_build());
		}
		{
			bn::sprite_builder builder(bn::sprite_items::tile_patterns);
			builder.set_bg_priority(2);
			builder.set_z_order(-20);
			builder.set_position(pos);
			_sprites.push_back(builder.release_build());
		}
		// _anims.push_back(bn::create_sprite_animate_action_once(_sprites.front(), 2, bn::sprite_items::card_blank.tiles_item(), 0, 0, 0, 0, 0, 0, 0, 0, 0));

		set_pattern(tile_pattern::SINGLE);
		set_pattern(tile_patterns::random_tile_pattern());

		set_facedown_immediate();
	}

	battle_card::~battle_card()
	{
		_sprites.clear();
	}
	
	void battle_card::update()
	{
		for(auto it = _anims.begin(), end = _anims.end(); it != end; ++it)
		{
			if(!it->done())
			{
				it->update();
				if(it->done())
				{
					_is_flipping = false;
					_sprites.back().set_visible(_is_faceup);
				//	it = _anims.erase(it);
				//	end = _anims.end();
				}
			}
		}
		
	}

	void battle_card::set_visible(bool visible)
	{
		_sprites.front().set_visible(visible);
		_sprites.back().set_visible(visible && _is_faceup);
	}

	void battle_card::set_position(bn::fixed_point pos)
	{
		_position = pos;
		for(auto it = _sprites.begin(), end = _sprites.end(); it != end; ++it)
		{
			it->set_position(pos);
		}
	}

	bn::fixed_point battle_card::get_position()
	{
		return _position;
	}

	void battle_card::move_to_destination(bn::fixed_point pos)
	{
		_position = pos;
		for(auto it = _sprites.begin(), end = _sprites.end(); it != end; ++it)
		{
			action_manager::register_move_action(*it, 40, pos);
		}
	}

	void battle_card::set_pattern(tile_pattern pattern)
	{
		_current_pattern = pattern;
		int tile_idx = tile_patterns::get_tile_index(pattern);
		_sprites.back().set_tiles(bn::sprite_items::tile_patterns.tiles_item().create_tiles(tile_idx));
	}

	tile_pattern battle_card::get_pattern()
	{
		return _current_pattern;
	}

	void battle_card::flip()
	{
		if(_is_flipping)
		{
			return;
		}

		_sprites.back().set_visible(false);
		_anims.clear();
		if(_is_faceup)
		{
			_anims.push_back(bn::create_sprite_animate_action_once(_sprites.front(), 2, bn::sprite_items::card_blank.tiles_item(), 0,1,2,3,4,5,6,7,8));
		}
		else
		{
			_anims.push_back(bn::create_sprite_animate_action_once(_sprites.front(), 2, bn::sprite_items::card_blank.tiles_item(), 8,7,6,5,4,3,2,1,0));
		}
		_is_faceup = !_is_faceup;
		_is_flipping = true;
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
		_is_flipping = false;
		_sprites.front().set_tiles(bn::sprite_items::card_blank.tiles_item(), 8);
		_sprites.back().set_visible(_is_faceup);
	}

	void battle_card::set_faceup_immediate()
	{
		_is_faceup = true;
		_is_flipping = false;
		_sprites.front().set_tiles(bn::sprite_items::card_blank.tiles_item(), 0);
		_sprites.back().set_visible(_is_faceup);
	}

	void battle_card::discard()
	{
		bn::fixed_point pos = _position;
		pos.set_y(pos.y() + 40);
		//move_to_destination(pos);
		set_pattern(tile_patterns::random_tile_pattern());
	}
}