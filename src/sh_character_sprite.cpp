#include "sh_character_sprite.h"

#include <bn_sprite_builder.h>
#include <bn_sprite_item.h>

// #include "bn_sprite_items_fullbody_test.h"
// #include "bn_sprite_items_protag_fullbody.h"

#include "bn_sprite_items_protag_bust.h"
#include "bn_sprite_items_flame_bust.h"

namespace sh
{
	
	bn::sprite_item get_char_sprite(character_id c_id);
	void assemble_sprite(character_sprite& sprite, character_id c_id);
	void load_sprite_offsets(bn::vector<bn::fixed_point, CHR_SPRITE_PART_CT> &offsets, character_id c_id);


	character_sprite::character_sprite(character_id c_id)
	{
		_sprite_offsets.clear();
		_sprite_parts.clear();
		
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
		for(int i = 0; i < _sprite_offsets.size(); i++)
		{
			_sprite_parts.at(i).set_position(_position + _sprite_offsets.at(i));
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
		build_sprite();
	}

	void character_sprite::set_mirror_x(bool mirror)
	{
		if(_mirror_x != mirror)
		{
			for(auto it = _sprite_offsets.begin(), end = _sprite_offsets.end(); it != end; ++it)
			{
				it->set_x(it->x() * -1);
			}
		}
		_mirror_x = mirror;
		for(auto it = _sprite_parts.begin(), end = _sprite_parts.end(); it != end; ++it)
		{
			it->set_horizontal_flip(_mirror_x);
		}
		update_sprite();
	}


	/**********************************************************************************/

	void character_sprite::build_sprite()
	{
		load_sprite_offsets(_sprite_offsets, _character_id);
		_sprite_parts.clear();
		const bn::sprite_item char_sprite = get_char_sprite(_character_id);
		bn::sprite_builder builder(char_sprite);
		builder.set_bg_priority(CHARACTER_LAYER);
		builder.set_z_order(0);
		for(int i = 0; i < _sprite_offsets.size(); i++)
		{
			builder.set_position(_position + _sprite_offsets.at(i));
			_sprite_parts.push_back(builder.build());
			_sprite_parts.back().set_tiles(char_sprite.tiles_item().create_tiles(i));
		}
		update_sprite();
	}

	bn::sprite_item get_char_sprite(character_id c_id)
	{
		switch(c_id)
		{
		case character_id::FLAME_WITCH:
			return bn::sprite_items::flame_bust;
			break;
		case character_id::FROST_PRINCESS:
		default:
			return bn::sprite_items::protag_bust;
			break;
		}
	}

	void load_sprite_offsets(bn::vector<bn::fixed_point, CHR_SPRITE_PART_CT> &offsets, character_id c_id)
	{
		offsets.clear();
		switch (c_id)
		{
		case character_id::FLAME_WITCH:
			offsets.push_back(bn::fixed_point(0,-96));
			offsets.push_back(bn::fixed_point(0,-64));
			offsets.push_back(bn::fixed_point(0,-32));
			offsets.push_back(bn::fixed_point(-8,0));
			offsets.push_back(bn::fixed_point(56,0));
			break;

		case character_id::FROST_PRINCESS:
			offsets.push_back(bn::fixed_point(0,-80));
			offsets.push_back(bn::fixed_point(0,-48));
			offsets.push_back(bn::fixed_point(0,-16));
			break;
		
		default:
			break;
		}
	}

}