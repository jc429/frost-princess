#include "sh_battle_portrait.h"

#include "bn_fixed.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_portrait_frame.h"
#include "bn_sprite_items_portrait_test.h"
#include "bn_sprite_items_tile_patterns.h"
#include "bn_sprite_tiles_ptr.h"

namespace sh
{

	battle_portrait::battle_portrait(int pos_x, int pos_y) :
		portrait_sprite (bn::sprite_items::portrait_test.create_sprite(pos_x, pos_y))
	//	frame_sprite (bn::sprite_items::portrait_frame.create_sprite(pos_x, pos_y))
	{
		// frame_sprite.set_bg_priority(1);
		// frame_sprite.set_z_order(450);
		portrait_sprite.set_bg_priority(1);
		portrait_sprite.set_z_order(440);

	}

	void battle_portrait::set_player_id(int id)
	{
		player_id = id;
		portrait_sprite.set_tiles(bn::sprite_items::portrait_test.tiles_item().create_tiles(id));
	}

}