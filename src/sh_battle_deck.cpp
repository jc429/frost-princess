#include "sh_battle_deck.h"

#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "bn_sprite_items_card_deck.h"

namespace sh
{

	battle_deck::battle_deck(int pos_x, int pos_y) :
		sprite(bn::sprite_items::card_deck.create_sprite((bn::fixed)pos_x,(bn::fixed)pos_y))
	{
		sprite.set_bg_priority(0);
		
	}

}