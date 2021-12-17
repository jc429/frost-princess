#ifndef SH_BATTLE_SCENE_H
#define SH_BATTLE_SCENE_H

#include <bn_blending_actions.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_vector.h>

#include "sh_scene.h"
#include "sh_battle_board.h"
#include "sh_battle_card.h"
#include "sh_battle_deck.h"
#include "sh_battle_portrait.h"
#include "sh_cursor.h"
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
		// bn::sprite_ptr _cursor_card_sprite;
		// bn::sprite_ptr _cursor_tile_sprite;
		// bn::sprite_animate_action<2> _cursor_card_idle_action;
		// bn::sprite_animate_action<2> _cursor_tile_idle_action;
		cursor battle_cursor;
		battle_board board;
		bn::blending_transparency_alpha_loop_action preview_transparency_action;

		bn::vector<skill_meter, 2> _skill_meters;

		battle_portrait player_portrait;
		battle_deck player_deck;
		battle_tile *player_base;

		battle_portrait foe_portrait;
		battle_deck foe_deck;	
		battle_tile *foe_base;

		tile_owner current_player;
		bn::vector<battle_card, MAX_CARDS_HAND> battle_cards;
		bn::vector<bn::point, MAX_CARDS_HAND> card_positions;

		int selected_card;
		int turn_count;

		void update();
		void battle_start();
		void player_turn();
		void swap_turns();
		void set_turn_number(int turn);
		void update_tile_counts();
		void update_text();

		//sh_battle_foe_ai.cpp
		void foe_turn();
		void foe_action_update();

	public:
		battle_scene();
		~battle_scene();
		void select_tile(int x, int y);
		void select_tile(bn::point pos);
		void set_battle_cursor_card_mode();
		void set_battle_cursor_tile_mode();
	};

}


#endif //SH_BATTLE_SCENE_H