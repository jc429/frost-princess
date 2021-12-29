#ifndef SH_BATTLE_CARD_H
#define SH_BATTLE_CARD_H

#include "sh_tile_pattern.h"


#include <bn_fixed_point.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{


	class battle_card
	{
	private:
		tile_pattern _current_pattern;
		// bn::sprite_ptr _card_sprite;
		// bn::sprite_ptr _pattern_sprite;
		bn::vector<bn::sprite_ptr, 2> _sprites;
		bn::vector<bn::sprite_animate_action<9>, 1> _anims;
		//bn::sprite_animate_action<9> _anim_flip;
		bn::fixed_point _position;
		bool _is_flipping;
		bool _is_faceup;

	public:
		battle_card(bn::fixed_point pos);
		virtual ~battle_card();
		void update();

		void set_visible(bool visible);
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