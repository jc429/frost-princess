#ifndef SH_CHARACTER_H
#define SH_CHARACTER_H

#include <bn_sprite_item.h>
#include <bn_string.h>

namespace sh
{
	enum class character_id
	{
		PROTAGONIST,
		FROST_PRINCESS,
		FLAME,
		WOOD,
		WATER,
		THUNDER,
		SHADOW,
		NONE
	};

	namespace characters
	{

		void set_player_character(character_id char_id);
		void set_foe_character(character_id char_id);
		character_id player_character();
		character_id foe_character();

		bn::string<16> get_character_name(character_id char_id);
		bn::sprite_item get_portrait_sprite(character_id c_id);
		bn::sprite_item get_select_sprite(character_id c_id);

		bn::sprite_palette_ptr get_portrait_palette_player();
		bn::sprite_palette_ptr get_portrait_palette_foe();
	}
}

#endif //SH_CHARACTER_H