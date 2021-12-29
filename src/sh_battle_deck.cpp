#include "sh_battle_deck.h"
#include "sh_random.h"

#include <bn_fixed_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "bn_sprite_items_card_deck.h"


namespace sh
{

	battle_deck::battle_deck()
	{
		card_ids.clear();
		card_patterns.clear();
		for(int i = 0; i < CARD_COUNT; i++)
		{
			card_ids.push_back(i);
		}
		init_card_patterns();
		shuffle();
	}

	battle_deck::~battle_deck()
	{
		card_ids.clear();
		card_patterns.clear();
	}

	void battle_deck::update()
	{

	}
	
	void battle_deck::init_card_patterns()
	{
		card_patterns.clear();
		// TODO: replace this with actual deck building for full game
		for(int i = 0; i < CARD_COUNT; i++)
		{
			card_patterns.push_back(tile_patterns::random_tile_pattern());
		}
	}

	void battle_deck::reset()
	{
		card_ids.clear();
		for(int i = 0; i < CARD_COUNT; i++)
		{
			card_ids.push_back(i);
		}
	}

	void battle_deck::randomize()
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
	}

	void battle_deck::shuffle()
	{
		randomize();
	}



	/*********************************************************/

	battle_deck_with_sprite::battle_deck_with_sprite(bn::fixed_point pos) :
		sprite(bn::sprite_items::card_deck.create_sprite(pos))
	{
		sprite.set_bg_priority(2);
		sprite.set_z_order(10);
		sprite.set_position(pos);
		anims.clear();
	}

	battle_deck_with_sprite::~battle_deck_with_sprite()
	{
		anims.clear();
	}


	void battle_deck_with_sprite::update()
	{
		battle_deck::update();

		for(auto it = anims.begin(), end = anims.end(); it != end; ++it)
		{
			if(!it->done())
			{
				it->update();
			}
			else
			{
				anims.clear();
			}
		}
	}
	
	void battle_deck_with_sprite::shuffle()
	{
		battle_deck::shuffle();
		// TODO: check if i actually need to recreate this animation each time
		anims.clear();
		anims.push_back(bn::create_sprite_animate_action_once(sprite, 2, bn::sprite_items::card_deck.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7, 0, 0));
	}

}