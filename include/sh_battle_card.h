#ifndef SH_BATTLE_CARD_H
#define SH_BATTLE_CARD_H

#include "sh_tile_pattern.h"

#include "bn_sprite_animate_actions.h"
#include "bn_sprite_ptr.h"
#include "bn_point.h"

namespace sh
{


	class battle_card
	{
	private:
		tile_pattern current_pattern;
		bn::sprite_ptr card_sprite;
		bn::sprite_ptr pattern_sprite;
		bn::sprite_animate_action<9> anim_flip;
		bn::point position;
		bool is_faceup;

	public:
		battle_card(int x, int y);
		virtual ~battle_card() = default;
		void update();


		void set_position(bn::point pos);
		bn::point get_position();
		void set_pattern(tile_pattern pattern);
		tile_pattern get_pattern();
		void flip();
		void flip_facedown();
		void flip_faceup();
	};

}

#endif //SH_BATTLE_CARD_H