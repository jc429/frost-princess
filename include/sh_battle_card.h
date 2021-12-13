#ifndef SH_BATTLE_CARD_H
#define SH_BATTLE_CARD_H

#include "sh_tile_pattern.h"


#include <bn_sprite_animate_actions.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_ptr.h>
#include <bn_point.h>
#include <bn_fixed_point.h>

namespace sh
{


	class battle_card
	{
	private:
		tile_pattern _current_pattern;
		bn::sprite_ptr _card_sprite;
		bn::sprite_ptr _pattern_sprite;
		bn::sprite_animate_action<9> _anim_flip;
		bn::fixed_point _position;
		bool _is_faceup;

	public:
		battle_card(int x, int y);
		virtual ~battle_card() = default;
		void update();

		void set_position(bn::fixed_point pos);
		bn::fixed_point get_position();
		void move_to_destination(bn::fixed_point pos);

		void set_pattern(tile_pattern pattern);
		tile_pattern get_pattern();
		void flip();
		void flip_facedown();
		void flip_faceup();
		void set_facedown_immediate();
		void set_faceup_immediate();
		void discard();
	};

}

#endif //SH_BATTLE_CARD_H