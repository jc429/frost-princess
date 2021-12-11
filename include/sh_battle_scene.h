#ifndef SH_BATTLE_SCENE_H
#define SH_BATTLE_SCENE_H

#include "bn_blending_actions.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_random.h"
#include "bn_vector.h"

#include "sh_scene.h"
#include "sh_battle_board.h"
#include "sh_battle_card.h"
#include "sh_battle_deck.h"
#include "sh_battle_portrait.h"
#include "sh_skill_meter.h"


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
	

	const int MAX_CARDS_DECK = 20;
	const int MAX_CARDS_HAND = 4;
	const int cards_x[] = {/*-48,*/ -24, 0, 24, 48};
	const int cards_y = 64;
	const int card_y_raised = 56;

	class battle_scene : public scene
	{
	private:
		bn::regular_bg_ptr _battle_bg;
		bn::sprite_ptr _cursor_card_sprite;
		bn::sprite_ptr _cursor_tile_sprite;
		bn::sprite_animate_action<2> _cursor_card_idle_action;
		bn::sprite_animate_action<2> _cursor_tile_idle_action;
		battle_board board;
		bn::blending_transparency_alpha_loop_action preview_transparency_action;

		bn::vector<skill_meter, 2> _skill_meters;

		battle_portrait player_portrait;
		battle_deck player_deck;
		battle_tile *player_base;

		battle_portrait foe_portrait;
		battle_deck foe_deck;	
		battle_tile *foe_base;
		

		bn::random random;

		tile_owner current_player;
		bn::vector<battle_card, MAX_CARDS_HAND> battle_cards;
		int selected_card;


		void update();
		void player_turn();
		void foe_turn();
		void swap_turns();

	public:
		battle_scene();
		void select_tile(int x, int y);

	};

}


#endif //SH_BATTLE_SCENE_H