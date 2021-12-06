#ifndef SH_BATTLE_DECK_H
#define SH_BATTLE_DECK_H

#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "sh_tile_pattern.h"
#include "sh_battle_card.h"

namespace sh
{


	class battle_deck
	{
	private:

	public:
		bn::sprite_ptr sprite;
		bn::vector<int, 20> card_ids;
		
		battle_deck(int pos_x, int pos_y);
		virtual ~battle_deck() = default;
		
	};

}

#endif //SH_BATTLE_DECK_H