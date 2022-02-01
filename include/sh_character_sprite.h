#ifndef SH_CHARACTER_SPRITE_H
#define SH_CHARACTER_SPRITE_H

#include "sh_character.h"

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	#define CHR_SPRITE_PART_CT 8
	#define CHARACTER_LAYER 2

	class character_sprite
	{
	private:
		
		bn::vector<bn::fixed_point, CHR_SPRITE_PART_CT>_sprite_offsets;
		bn::vector<bn::sprite_ptr, CHR_SPRITE_PART_CT> _sprite_parts;
		/* sprites are anchored at their bottom middle pixel*/
		bn::fixed_point _position;

		character_id _character_id;
		bool _mirror_x;

	public:
		character_sprite(character_id c_id);
		~character_sprite();

		void build_sprite();

		void update();
		void update_sprite();
		void set_position(bn::fixed_point pos);
		void set_character(character_id c_id);
		void set_mirror_x(bool mirror);
	};
}

#endif //SH_CHARACTER_SPRITE_H