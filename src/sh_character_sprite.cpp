#include "sh_character_sprite.h"

#include <bn_sprite_builder.h>
#include <bn_sprite_item.h>

#include "bn_sprite_items_fullbody_test.h"

namespace sh
{
	#define CHARACTER_LAYER 2
	bn::sprite_item get_char_sprite(character_id c_id);

	character_sprite::character_sprite(character_id c_id)
	{
		_sprite_offsets.clear();
		_sprite_parts.clear();
		_position = bn::fixed_point(0,0);
		{
			_sprite_offsets.push_back(bn::fixed_point(-32,-64));
			_sprite_offsets.push_back(bn::fixed_point(-32, 0));
			_sprite_offsets.push_back(bn::fixed_point(-32, 64));
			_sprite_offsets.push_back(bn::fixed_point(32, -64));
			_sprite_offsets.push_back(bn::fixed_point(32, 0));
			_sprite_offsets.push_back(bn::fixed_point(32, 64));
		}
		_mirror_x = false;
		
		set_character(c_id);
	}

	character_sprite::~character_sprite()
	{
		_sprite_offsets.clear();
		_sprite_parts.clear();
	}



	void character_sprite::update()
	{
	}

	void character_sprite::update_sprite()
	{
		for(int i = 0; i < SPRITE_PART_CT; i++)
		{
			int off_id = _mirror_x ? ((i+3)%SPRITE_PART_CT) : i;
			_sprite_parts.at(i).set_position(_position + _sprite_offsets.at(off_id));
		}
	}




	void character_sprite::set_position(bn::fixed_point pos)
	{
		_position = pos;
		update_sprite();
	}

	void character_sprite::set_character(character_id c_id)
	{
		_character_id = c_id;
		const bn::sprite_item char_sprite = get_char_sprite(c_id);
		bn::sprite_builder builder(char_sprite);
		builder.set_bg_priority(CHARACTER_LAYER);
		builder.set_z_order(0);
		for(int i = 0; i < SPRITE_PART_CT; i++)
		{
			int off_id = _mirror_x ? (i+3)%SPRITE_PART_CT : i; 
			builder.set_position(_position + _sprite_offsets.at(off_id));
			_sprite_parts.push_back(builder.build());
			_sprite_parts.back().set_tiles(char_sprite.tiles_item().create_tiles(i));
		}
		update_sprite();
	}

	void character_sprite::set_mirror_x(bool mirror)
	{
		_mirror_x = mirror;
		for(auto it = _sprite_parts.begin(), end = _sprite_parts.end(); it != end; ++it)
		{
			it->set_horizontal_flip(_mirror_x);
		}
		update_sprite();
	}


	/**********************************************************************************/

	bn::sprite_item get_char_sprite(character_id c_id)
	{
		switch(c_id)
		{
		default:
			return bn::sprite_items::fullbody_test;
			break;
		}
	}
}