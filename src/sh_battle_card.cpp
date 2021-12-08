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
		pattern_sprite (bn::sprite_items::tile_patterns.create_sprite(x,y))
	{
		is_faceup = false;
		set_pattern(tile_pattern::SINGLE);
		set_pattern(tile_patterns::random_tile_pattern());
	}
	
	void battle_card::set_position(bn::point pos)
	{
		card_sprite.set_position(pos.x(), pos.y());
		pattern_sprite.set_position(pos.x(), pos.y());
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

}