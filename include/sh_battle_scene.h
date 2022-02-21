#ifndef SH_BATTLE_SCENE_H
#define SH_BATTLE_SCENE_H

#include <bn_blending_actions.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_random.h>
#include <bn_vector.h>

#include "sh_battle_board.h"
#include "sh_battle_card.h"
#include "sh_battle_deck.h"
#include "sh_battle_portrait.h"
#include "sh_character.h"
#include "sh_cursor.h"
#include "sh_scene.h"
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
		PLAYER_SPECIAL_PLACEMENT,
		FOE_TILE_PLACEMENT,
		END
	};
	
	const int MAX_HEALTH = 40;
	const int MAX_CARDS_DECK = 32;
	const int MAX_CARDS_HAND = 4;
	const int cards_x[] = {/*-48,*/ -24, 0, 24, 48};
	const int cards_y = 64;
	const int card_y_raised = 56;

	class battle_scene : public scene
	{
	private:

		bn::regular_bg_ptr _battle_board_bg;
		bn::regular_bg_ptr _battle_ui_bg;
		bn::regular_bg_ptr _turn_announcement;
		cursor battle_cursor;
		battle_board board;
		bn::blending_transparency_alpha_loop_action preview_transparency_action;

		bn::vector<ui_meter, 2> _health_meters;
		bn::vector<skill_meter, 2> _skill_meters;

		character_id player_character;
		battle_portrait player_portrait;
		battle_deck_with_sprite player_deck;
		battle_tile *player_base;

		character_id foe_character;
		battle_portrait foe_portrait;
		battle_deck foe_deck;
		battle_tile *foe_base;

		tile_owner current_player;
		bn::vector<battle_card, MAX_CARDS_HAND> battle_cards;
		bn::vector<bn::point, MAX_CARDS_HAND> card_positions;

		int player_health;
		int foe_health;

		int selected_card;
		int turn_count;

		void reset_battle();
		void battle_start();
		void battle_end(tile_owner winner);

		void player_turn();
		void turn_intro(tile_owner player);
		void end_turn();
		void set_turn_number(int turn);
		void update_tile_counts(); 
		void update_text();

		//sh_battle_foe_ai.cpp
		void foe_turn();
		void foe_action_update();
		bool foe_ai_check_tile_valid(battle_tile *tile);

	public:
		battle_scene();
		~battle_scene();
		void update() override;
		
		void select_tile(int x, int y);
		void select_tile(bn::point pos);
		void set_battle_cursor_card_mode();
		void set_battle_cursor_tile_mode();

		void open_pause_menu();
		tile_owner get_current_player();
		void apply_damage_to_player(tile_owner player, int dmg);

		static bn::sprite_palette_ptr get_palette_of_player(tile_owner player);
	};



}


#endif //SH_BATTLE_SCENE_H