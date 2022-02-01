#include "sh_battle_portrait.h"
#include "sh_character.h"

#include <bn_fixed_point.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_tiles_ptr.h>

#include "bn_sprite_items_portrait_frame.h"
#include "bn_sprite_items_protag_portrait.h"

namespace sh
{
	bn::sprite_item get_portrait_sprite(character_id c_id);

	battle_portrait::battle_portrait(bn::fixed_point pos, bool is_player) :
		_portrait_sprite (bn::sprite_items::protag_portrait.create_sprite(pos))
	{
		_is_player = is_player;
		_position = pos;
		_portrait_sprite.set_bg_priority(1);
	}


	void battle_portrait::set_character(character_id c_id)
	{
		char_id = c_id;
		bn::sprite_item spr = characters::get_portrait_sprite(c_id);
		_portrait_sprite = spr.create_sprite(_position);
		_portrait_sprite.set_tiles(spr.tiles_item().create_tiles(_is_player ? 1 : 0));
		_portrait_sprite.set_bg_priority(1);
	}




}