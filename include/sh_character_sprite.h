#ifndef SH_CHARACTER_SPRITE_H
#define SH_CHARACTER_SPRITE_H

#include "sh_character.h"

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{


	class character_sprite
	{
	private:
		const static int SPRITE_PART_CT = 6;
		bn::vector<bn::fixed_point, SPRITE_PART_CT>_sprite_offsets;
		bn::vector<bn::sprite_ptr, SPRITE_PART_CT> _sprite_parts;
		bn::fixed_point _position;

		character_id _character_id;
		bool _mirror_x;

	public:
		character_sprite(character_id c_id);
		~character_sprite();

		void update();
		void update_sprite();
		void set_position(bn::fixed_point pos);
		void set_character(character_id c_id);
		void set_mirror_x(bool mirror);
	};
}

#endif //SH_CHARACTER_SPRITE_H