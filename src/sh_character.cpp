#include "sh_character.h"

#include <bn_sprite_item.h>
#include <bn_string.h>

#include "bn_sprite_items_protag_portrait.h"
#include "bn_sprite_items_flame_portrait.h"
#include "bn_sprite_items_wood_portrait.h"

#include "bn_sprite_items_protag_select.h"
#include "bn_sprite_items_flame_select.h"
#include "bn_sprite_items_wood_select.h"

namespace sh
{


	namespace characters
	{
		
		static character_id _player_char_ = character_id::PROTAGONIST;
		static character_id _foe_char_ = character_id::FLAME_WITCH;

		void set_player_character(character_id char_id)
		{
			_player_char_ = char_id;
		}

		void set_foe_character(character_id char_id)
		{
			_foe_char_ = char_id;
		}

		character_id player_character()
		{
			return _player_char_;
		}
		
		character_id foe_character()
		{
			return _foe_char_;
		}

		bn::string<16> get_character_name(character_id char_id)
		{
			switch(char_id)
			{
			case character_id::PROTAGONIST:
				return "Protagonist";
			case character_id::FROST_PRINCESS:
				return "Frost Princess";
			case character_id::WOOD_DRUID:
				return "Woody";
			case character_id::FLAME_WITCH:
				return "Homura";
			default:
				return "";
			}
		}

		bn::sprite_item get_portrait_sprite(character_id c_id)
		{
			switch(c_id)
			{
			case character_id::FLAME_WITCH:
				return bn::sprite_items::flame_portrait;
				break;
			case character_id::WOOD_DRUID:
				return bn::sprite_items::wood_portrait;
				break;
			case character_id::PROTAGONIST:
			default:
				return bn::sprite_items::protag_portrait;
				break;
			}
		}

		bn::sprite_item get_select_sprite(character_id c_id)
		{
			switch(c_id)
			{
			case character_id::FLAME_WITCH:
				return bn::sprite_items::flame_select;
				break;
			case character_id::WOOD_DRUID:
				return bn::sprite_items::wood_select;
				break;
			case character_id::PROTAGONIST:
			default:
				return bn::sprite_items::protag_select;
				break;
			}
		}

	}

}