#include "sh_battle_deck.h"

#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "bn_sprite_items_card_deck.h"

#include "sh_random.h"

namespace sh
{

	battle_deck::battle_deck(int pos_x, int pos_y) :
		sprite(bn::sprite_items::card_deck.create_sprite((bn::fixed)pos_x,(bn::fixed)pos_y)),
		anim_shuffle(bn::create_sprite_animate_action_once(sprite, 2, bn::sprite_items::card_deck.tiles_item(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
	{
		sprite.set_bg_priority(2);
		sprite.set_z_order(10);
	}

	void battle_deck::shuffle()
	{
		int m = card_ids.size()-1;
		while(m > 0)
		{
			int r = random.get_int(0, m);
			int temp = card_ids.at(m);
			card_ids.at(m) = card_ids.at(r);
			card_ids.at(r) = temp;
			m--;
		}
		// TODO: check if i actually need to recreate this animation each time
		anim_shuffle = bn::create_sprite_animate_action_once(sprite, 2, bn::sprite_items::card_deck.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7, 0, 0);
	}

	void battle_deck::update()
	{
		if(!anim_shuffle.done())
		{
			anim_shuffle.update();
		}
	}
}