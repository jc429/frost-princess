#include "sh_character.h"


namespace sh
{


	namespace characters
	{
		static character_id player_char = character_id::FROST_PRINCESS;
		static character_id foe_char = character_id::FLAME_WITCH;

		void set_player_character(character_id char_id)
		{
			player_char = char_id;
		}

		void set_foe_character(character_id char_id)
		{
			foe_char = char_id;
		}
	}

}