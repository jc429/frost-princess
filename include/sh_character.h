#ifndef SH_CHARACTER_H
#define SH_CHARACTER_H

namespace sh
{
	enum class character_id
	{
		FROST_PRINCESS,
		FLAME_WITCH,
		ICE_QUEEN
	};

	namespace characters
	{

		void set_player_character(character_id char_id);
		void set_foe_character(character_id char_id);
	}
}

#endif //SH_CHARACTER_H