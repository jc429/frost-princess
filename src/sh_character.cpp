#include "sh_character.h"

#include <bn_string.h>

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
			case character_id::FLAME_WITCH:
				return "Homura";
			default:
				return "";
			}
		}

	}

}