#include "sh_battle_card.h"

#include "bn_fixed.h"
#include "bn_sprite_tiles.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_items_card_blank.h"
#include "bn_sprite_items_tile_patterns.h"

namespace sh
{



	battle_card::battle_card(int x, int y) :
		card_sprite (bn::sprite_items::card_blank.create_sprite(x,y)),
		pattern_sprite (bn::sprite_items::tile_patterns.create_sprite(x,y)),
		anim_flip (bn::create_sprite_animate_action_once(card_sprite, 2, bn::sprite_items::card_blank.tiles_item(), 0, 0, 0, 0, 0, 0, 0, 0, 0))
	{
		position.set_x(x);
		position.set_y(y);
		is_faceup = false;
		card_sprite.set_bg_priority(1);
		card_sprite.set_z_order(450);
		pattern_sprite.set_bg_priority(1);
		pattern_sprite.set_z_order(440);
		set_pattern(tile_pattern::SINGLE);
		set_pattern(tile_patterns::random_tile_pattern());

		pattern_sprite.set_visible(is_faceup);
	}
	
	void battle_card::update()
	{
		if(!anim_flip.done())
		{
			anim_flip.update();
			if(anim_flip.done() && is_faceup)
			{
				pattern_sprite.set_visible(true);
			}
		}
	}
	
	void battle_card::set_position(bn::point pos)
	{
		position = pos;
		card_sprite.set_position(pos.x(), pos.y());
		pattern_sprite.set_position(pos.x(), pos.y());
	}

	bn::point battle_card::get_position()
	{
		return position;
	}

	void battle_card::set_pattern(tile_pattern pattern)
	{
		current_pattern = pattern;
		int tile_idx = tile_patterns::get_tile_index(pattern);
		pattern_sprite.set_tiles(bn::sprite_items::tile_patterns.tiles_item().create_tiles(tile_idx));
	}

	tile_pattern battle_card::get_pattern()
	{
		return current_pattern;
	}

	void battle_card::flip()
	{
		pattern_sprite.set_visible(false);
		anim_flip = bn::create_sprite_animate_action_once(card_sprite, 2, bn::sprite_items::card_blank.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7, 0);
		is_faceup = !is_faceup;
	}

	void battle_card::flip_facedown()
	{
		if(is_faceup)
		{
			flip();
		}
	}

	void battle_card::flip_faceup()
	{
		if(!is_faceup)
		{
			flip();
		}
	}


}