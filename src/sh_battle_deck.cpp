#include "sh_battle_deck.h"
#include "sh_random.h"
#include "sh_scene.h"
#include "sh_character.h"

#include <bn_deque.h>
#include <bn_fixed_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_palette_ptr.h>
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
		card_patterns.push_back(tile_pattern::SINGLE);
		card_patterns.push_back(tile_pattern::SINGLE);
		card_patterns.push_back(tile_pattern::SINGLE);
		card_patterns.push_back(tile_pattern::SINGLE);
		card_patterns.push_back(tile_pattern::LINE_2);
		card_patterns.push_back(tile_pattern::LINE_2);
		card_patterns.push_back(tile_pattern::LINE_2);
		card_patterns.push_back(tile_pattern::LINE_2);

		card_patterns.push_back(tile_pattern::LINE_3);
		card_patterns.push_back(tile_pattern::LINE_3);
		card_patterns.push_back(tile_pattern::LINE_3);
		card_patterns.push_back(tile_pattern::LINE_3);
		card_patterns.push_back(tile_pattern::L_SMALL);
		card_patterns.push_back(tile_pattern::L_SMALL);
		card_patterns.push_back(tile_pattern::L_SMALL);
		card_patterns.push_back(tile_pattern::L_SMALL);
		
		card_patterns.push_back(tile_pattern::L_NORMAL);
		card_patterns.push_back(tile_pattern::L_NORMAL);
		card_patterns.push_back(tile_pattern::L_NORMAL);
		card_patterns.push_back(tile_pattern::L_REVERSE);
		card_patterns.push_back(tile_pattern::L_REVERSE);
		card_patterns.push_back(tile_pattern::L_REVERSE);
		card_patterns.push_back(tile_pattern::SQUARE);
		card_patterns.push_back(tile_pattern::SQUARE);

		card_patterns.push_back(tile_pattern::T_4);
		card_patterns.push_back(tile_pattern::T_4);
		card_patterns.push_back(tile_pattern::Z_4);
		card_patterns.push_back(tile_pattern::Z_4);
		card_patterns.push_back(tile_pattern::S_4);
		card_patterns.push_back(tile_pattern::S_4);
		card_patterns.push_back(tile_pattern::LINE_4);
		card_patterns.push_back(tile_pattern::LINE_4);

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

	tile_pattern battle_deck::draw_card()
	{
		if(card_ids.empty())
		{
			reset();
		}
		int card_id = card_ids.front();
		card_ids.pop_front();
		return card_patterns.at(card_id);
	}

	/*********************************************************/

	battle_deck_with_sprite::battle_deck_with_sprite(bn::fixed_point pos) :
		_sprite(bn::sprite_items::card_deck.create_sprite(pos))
	{
		_position = pos;
		_top_card_offset = bn::fixed_point(0,-3);
		_sprite.set_bg_priority(2);
		_sprite.set_z_order(10);
		_sprite.set_position(pos);
		_sprite.set_palette(characters::get_portrait_palette_player());
		_anims.clear();
	}

	battle_deck_with_sprite::~battle_deck_with_sprite()
	{
		_anims.clear();
	}


	void battle_deck_with_sprite::update()
	{
		battle_deck::update();

		for(auto it = _anims.begin(), end = _anims.end(); it != end; ++it)
		{
			if(!it->done())
			{
				it->update();
			}
			else
			{
				_anims.clear();
			}
		}
	}

	void battle_deck_with_sprite::set_camera(bn::camera_ptr camera)
	{
		_sprite.set_camera(camera);
	}
	
	void battle_deck_with_sprite::shuffle()
	{
		battle_deck::shuffle();
		// TODO: check if i actually need to recreate this animation each time
		_anims.clear();
		_anims.push_back(bn::create_sprite_animate_action_once(_sprite, 2, bn::sprite_items::card_deck.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7, 0, 0));
	}

	tile_pattern battle_deck_with_sprite::draw_card_with_animation(scene &scene, battle_card &card)
	{
		card.reset();
		tile_pattern pattern = draw_card();
		card.set_facedown_immediate();
		card.set_position(get_card_pos());
		int dist_x = (card.get_hand_position().x() - get_card_pos().x()).floor_integer();
		dist_x = bn::max(dist_x, -dist_x) / 2;
		int duration = bn::min(dist_x, 30);
		card.move_to_destination(card.get_hand_position(), duration);
		scene.wait_for_update_cycles(duration+5);
		card.set_raised(false);
		return pattern;
	}

	bn::fixed_point battle_deck_with_sprite::get_card_pos()
	{
		return _position + _top_card_offset;
	}
}