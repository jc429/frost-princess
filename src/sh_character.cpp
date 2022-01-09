#include "sh_character.h"


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
	}

}