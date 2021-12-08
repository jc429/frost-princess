#ifndef SH_BATTLE_H
#define SH_BATTLE_H

#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_random.h"
#include "bn_vector.h"

#include "sh_scene.h"
#include "sh_battle_board.h"
#include "sh_battle_card.h"

namespace sh
{

	enum class battle_state{
		INTRO,
		PLAYER_TURN,
		FOE_TURN,
		RESULTS
	};
	

	enum class turn_state{
		INTRO,
		PLAYER_CARD_SELECT,
		PLAYER_TILE_PLACEMENT,
		FOE_TILE_PLACEMENT,
		END
	};
	
	const int MAX_CARDS_HAND = 5;
	const int MAX_CARDS_DECK = 20;

	//void battle_scene(bn::sprite_text_generator& text_generator);
	const int cards_x[] = {-48, -24, 0, 24, 48};
	const int cards_y = 64;

	class battle_scene
	{
	private:
		bn::regular_bg_ptr battle_bg;
		bn::sprite_ptr cursor_card_sprite;
		bn::sprite_ptr cursor_tile_sprite;
		bn::sprite_animate_action<2> cursor_card_idle_action;
		bn::sprite_animate_action<2> cursor_tile_idle_action;
		battle_board board;
			
		battle_tile *player_base;
		battle_tile *foe_base;
		
		tile_owner current_player;
		int selected_card = 2;
		bn::random random;
		bn::vector<battle_card, MAX_CARDS_HAND> battle_cards;

		void update();
		void player_turn();
		void foe_turn();
		void swap_turns();

	public:
		battle_scene();
		void select_tile(int x, int y);

	};

}


#endif //SH_BATTLE_H