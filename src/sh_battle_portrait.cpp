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

	battle_portrait::battle_portrait(bn::fixed_point pos) :
		portrait_sprite (bn::sprite_items::protag_portrait.create_sprite(pos))
	{
		_position = pos;
		portrait_sprite.set_bg_priority(1);
	}


	battle_portrait::battle_portrait(int pos_x, int pos_y) :
		portrait_sprite (bn::sprite_items::protag_portrait.create_sprite(pos_x, pos_y))
	//	frame_sprite (bn::sprite_items::portrait_frame.create_sprite(pos_x, pos_y))
	{
		_position = bn::fixed_point(pos_x,pos_y);
		// frame_sprite.set_bg_priority(1);
		// frame_sprite.set_z_order(450);
		portrait_sprite.set_bg_priority(1);
	}

	void battle_portrait::set_character(character_id c_id)
	{
		char_id = c_id;
		portrait_sprite = characters::get_portrait_sprite(c_id).create_sprite(_position);
		portrait_sprite.set_bg_priority(1);
	}




}