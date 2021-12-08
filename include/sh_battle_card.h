#ifndef SH_BATTLE_CARD_H
#define SH_BATTLE_CARD_H

#include "sh_tile_pattern.h"
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
		bool is_faceup;

	public:
		battle_card(int x, int y);
		virtual ~battle_card() = default;
		void set_position(bn::point pos);
		void set_pattern(tile_pattern pattern);
		tile_pattern get_pattern();
		void set_facedown();
		void set_faceup();
	};

}

#endif //SH_BATTLE_CARD_H