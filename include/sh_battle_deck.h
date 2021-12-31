#ifndef SH_BATTLE_DECK_H
#define SH_BATTLE_DECK_H

#include <bn_deque.h>
#include <bn_fixed_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "sh_battle_card.h"
#include "sh_tile_pattern.h"
#include "sh_scene.h"

namespace sh
{
	#define CARD_COUNT 32
	class battle_deck
	{
	protected:
		// the actual "deck" that gets shuffled and drawn from
		bn::deque<int, CARD_COUNT> card_ids;
		// maps card ids to their associated tile patterns
		bn::vector<tile_pattern, CARD_COUNT> card_patterns;

	public:
		
		battle_deck();
		virtual ~battle_deck();
		virtual void update();

		void init_card_patterns();

		void reset();
		void randomize();
		virtual void shuffle();
		virtual tile_pattern draw_card();
	};


	class battle_deck_with_sprite : public battle_deck
	{
	private:
		bn::fixed_point _position;
		bn::fixed_point _top_card_offset;
		bn::sprite_ptr _sprite;
		bn::vector<bn::sprite_animate_action<10>, 1> _anims;

	public:
		battle_deck_with_sprite(bn::fixed_point pos);
		virtual ~battle_deck_with_sprite();
		void update() override;
		
		void set_camera(bn::camera_ptr camera);
		void shuffle() override;
		tile_pattern draw_card_with_animation(scene &scene, battle_card &card);

		bn::fixed_point get_card_pos();
	};
}

#endif //SH_BATTLE_DECK_H