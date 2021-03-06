#include "sh_character_sprite.h"

#include <bn_sprite_builder.h>
#include <bn_sprite_item.h>

// #include "bn_sprite_items_fullbody_test.h"
// #include "bn_sprite_items_protag_fullbody.h"

#include "bn_sprite_items_protag_bust.h"
#include "bn_sprite_items_protag_fullbody.h"
#include "bn_sprite_items_flame_bust.h"
#include "bn_sprite_items_flame_fullbody.h"
#include "bn_sprite_items_wood_bust_1.h"
#include "bn_sprite_items_wood_bust_2.h"
#include "bn_sprite_items_wood_fullbody.h"
#include "bn_sprite_items_shadow_bust_1.h"
#include "bn_sprite_items_shadow_bust_2.h"
#include "bn_sprite_items_shadow_bust_3.h"
#include "bn_sprite_items_shadow_fullbody.h"

namespace sh
{
	bn::sprite_item get_char_sprite(character_id c_id);
	bn::sprite_item get_fullbody_extension(character_id c_id);
	void assemble_sprite(character_sprite& sprite, character_id c_id);
	void load_sprite_offsets(bn::vector<bn::fixed_point, CHR_SPRITE_PART_CT> &offsets, character_id c_id);


	character_sprite::character_sprite(character_id c_id, char_sprite_type spr_type)
	{
		_sprite_offsets.clear();
		_sprite_parts.clear();
		
		_mirror_x = false;
		_sprite_type = spr_type;
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

	void character_sprite::set_sprite_type(char_sprite_type spr_type)
	{
		_sprite_type = spr_type;
		build_sprite();
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
		bn::sprite_item char_sprite = get_char_sprite(_character_id);
		int sheet_tiles = bn::min(_sprite_offsets.size(), char_sprite.tiles_item().graphics_count());
		// all chars have at least one sprite sheet, so build that out here
		{
			bn::sprite_builder builder(char_sprite);
			builder.set_bg_priority(CHARACTER_LAYER);
			builder.set_z_order(0);
			for(int i = 0; i < sheet_tiles; i++)
			{
				builder.set_position(_position + _sprite_offsets.at(i));
				_sprite_parts.push_back(builder.build());
				_sprite_parts.back().set_tiles(char_sprite.tiles_item().create_tiles(i));
			}
		}
		// special cases for chars that need more than one sheet
		switch(_character_id)
		{
		case character_id::FLAME:
		case character_id::PROTAGONIST:
			break;
		case character_id::WOOD:
			{
				char_sprite = bn::sprite_items::wood_bust_2;
				bn::sprite_builder builder(char_sprite);
				builder.set_bg_priority(CHARACTER_LAYER);
				builder.set_z_order(0);
				int sheet_tiles_2 = 2;
				for(int i = 0; i < sheet_tiles_2; i++)
				{
					builder.set_position(_position + _sprite_offsets.at(i+sheet_tiles));
					_sprite_parts.push_back(builder.build());
					_sprite_parts.back().set_tiles(char_sprite.tiles_item().create_tiles(i));
				}
				sheet_tiles += sheet_tiles_2;
			}
			break;
		case character_id::SHADOW:
			{
				char_sprite = bn::sprite_items::shadow_bust_2;
				bn::sprite_builder builder(char_sprite);
				builder.set_bg_priority(CHARACTER_LAYER);
				builder.set_z_order(0);
				int sheet_tiles_2 = 2;
				for(int i = 0; i < sheet_tiles_2; i++)
				{
					builder.set_position(_position + _sprite_offsets.at(i+sheet_tiles));
					_sprite_parts.push_back(builder.build());
					_sprite_parts.back().set_tiles(char_sprite.tiles_item().create_tiles(i));
				}
				sheet_tiles += sheet_tiles_2;
			}
			{
				char_sprite = bn::sprite_items::shadow_bust_3;
				bn::sprite_builder builder(char_sprite);
				builder.set_bg_priority(CHARACTER_LAYER);
				builder.set_z_order(0);
				int sheet_tiles_3 = 2;
				for(int i = 0; i < sheet_tiles_3; i++)
				{
					builder.set_position(_position + _sprite_offsets.at(i+sheet_tiles));
					_sprite_parts.push_back(builder.build());
					_sprite_parts.back().set_tiles(char_sprite.tiles_item().create_tiles(i));
				}
				sheet_tiles += sheet_tiles_3;
			}

		default:
			break;
		}
		
		// add extension segments for fullbody 
		if(_sprite_type == char_sprite_type::FULLBODY)
		{
			bn::sprite_item fb_sprite = get_fullbody_extension(_character_id);
			int sheet_tiles = bn::min(_sprite_offsets.size(), fb_sprite.tiles_item().graphics_count());
			bn::sprite_builder builder(fb_sprite);
			builder.set_bg_priority(CHARACTER_LAYER);
			builder.set_z_order(0);
			for(int i = 0; i < sheet_tiles; i++)
			{
				builder.set_position(_position + _sprite_offsets.at(i));
				_sprite_parts.push_back(builder.build());
				_sprite_parts.back().set_tiles(fb_sprite.tiles_item().create_tiles(i));
			}
		}
		


		update_sprite();
	}

	bn::sprite_item get_char_sprite(character_id c_id)
	{
		switch(c_id)
		{
		case character_id::FLAME:
			return bn::sprite_items::flame_bust;
			break;
		case character_id::WOOD:
			return bn::sprite_items::wood_bust_1;
			break;
		case character_id::SHADOW:
			return bn::sprite_items::shadow_bust_1;
			break;
		case character_id::PROTAGONIST:
		default:
			return bn::sprite_items::protag_bust;
			break;
		}
	}

	bn::sprite_item get_fullbody_extension(character_id c_id)
	{
		switch(c_id)
		{
		case character_id::FLAME:
			return bn::sprite_items::flame_fullbody;
			break;
		case character_id::WOOD:
			return bn::sprite_items::wood_fullbody;
			break;
		case character_id::SHADOW:
			return bn::sprite_items::shadow_fullbody;
			break;
		case character_id::PROTAGONIST:
		default:
			return bn::sprite_items::protag_fullbody;
			break;
		}
	}

	void load_sprite_offsets(bn::vector<bn::fixed_point, CHR_SPRITE_PART_CT> &offsets, character_id c_id)
	{
		offsets.clear();
		switch (c_id)
		{
		case character_id::FLAME:
			offsets.push_back(bn::fixed_point(0,-96));
			offsets.push_back(bn::fixed_point(0,-64));
			offsets.push_back(bn::fixed_point(0,-32));
			offsets.push_back(bn::fixed_point(-16,0));
			offsets.push_back(bn::fixed_point(48,0));
			//fb 
			offsets.push_back(bn::fixed_point(-32,24));
			offsets.push_back(bn::fixed_point(0,24));
			offsets.push_back(bn::fixed_point(32,24));
			break;
		case character_id::WOOD:
			// sheet 1
			offsets.push_back(bn::fixed_point(-8,-96));
			offsets.push_back(bn::fixed_point(-8,-64));
			offsets.push_back(bn::fixed_point(-8,-32));
			offsets.push_back(bn::fixed_point(-8,0));
			// sheet 2
			offsets.push_back(bn::fixed_point(40,-32));
			offsets.push_back(bn::fixed_point(40,0));
			//fb 
			offsets.push_back(bn::fixed_point(-40,24));
			offsets.push_back(bn::fixed_point(-8,24));
			offsets.push_back(bn::fixed_point(24,24));
			break;
		case character_id::SHADOW:
			// sheet 1
			offsets.push_back(bn::fixed_point(0,-80));
			offsets.push_back(bn::fixed_point(0,-48));
			offsets.push_back(bn::fixed_point(0,-16));
			// sheet 2
			offsets.push_back(bn::fixed_point(-48,-32));
			offsets.push_back(bn::fixed_point(48,-32));
			// sheet 3
			offsets.push_back(bn::fixed_point(0,-104));
			offsets.push_back(bn::fixed_point(48,-72));
			//fb 
			offsets.push_back(bn::fixed_point(-16,16));
			offsets.push_back(bn::fixed_point(16,16));
			offsets.push_back(bn::fixed_point(48,16));
			break;
		case character_id::PROTAGONIST:
			offsets.push_back(bn::fixed_point(0,-80));
			offsets.push_back(bn::fixed_point(0,-48));
			offsets.push_back(bn::fixed_point(0,-16));
			//fb 
			offsets.push_back(bn::fixed_point(-32,16));
			offsets.push_back(bn::fixed_point(0,16));
			offsets.push_back(bn::fixed_point(32,16));

			break;
		
		default:
			break;
		}
	}

}