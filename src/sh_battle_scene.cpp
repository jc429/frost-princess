#include "sh_battle_scene.h"
#include "sh_battle_deck.h"
#include "sh_battle_tile.h"
#include "sh_debug.h"
#include "sh_audio.h"
#include "sh_effects.h"

#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_log.h>
#include <bn_regular_bg_ptr.h>
#include <bn_string.h>

#include "sh_action_manager.h"
#include "sh_random.h"
// graphics
#include <bn_blending_actions.h>
#include <bn_sprite_animate_actions.h>
// text & fonts
#include <bn_sprite_text_generator.h>
#include "variable_8x16_sprite_font.h"
// backgrounds
#include "bn_regular_bg_items_battle_bg.h"
#include "bn_regular_bg_items_battle_bg_wood.h"
#include "bn_regular_bg_items_battle_board.h"
#include "bn_regular_bg_items_btl_player_phase.h"
#include "bn_regular_bg_items_btl_enemy_phase.h"
// sprites
#include "bn_sprite_items_portrait_frame.h"
#include "bn_sprite_items_card_blank.h"
// #include "bn_sprite_items_cursor_card.h"
// #include "bn_sprite_items_cursor_tile.h"
#include "bn_sprite_items_crown.h"
#include "bn_sprite_items_skill_meter_flame.h"



namespace sh{

	#define BTL_PORTRAIT_X 94
	#define BTL_PORTRAIT_Y 54
	#define BTL_DECK_PLA_X -50
	#define BTL_DECK_PLA_Y 62

	bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
	bn::vector<bn::sprite_ptr, 12> text_sprites;

	battle_scene::battle_scene() :
		_battle_bg (bn::regular_bg_items::battle_bg_wood.create_bg(0, 0)),
		// _cursor_card_sprite (bn::sprite_items::cursor_card.create_sprite(cards_x[0], cards_y)),
		// _cursor_tile_sprite (bn::sprite_items::cursor_tile.create_sprite(0, -16)),
		// _cursor_card_idle_action (bn::create_sprite_animate_action_forever(_cursor_card_sprite, 16, bn::sprite_items::cursor_card.tiles_item(), 0, 1)),
		// _cursor_tile_idle_action (bn::create_sprite_animate_action_forever(_cursor_tile_sprite, 16, bn::sprite_items::cursor_tile.tiles_item(), 0, 1)),
		preview_transparency_action (bn::blending_transparency_alpha_loop_action(30,0.2)),
		player_portrait (battle_portrait(-BTL_PORTRAIT_X, BTL_PORTRAIT_Y)),
		player_deck (battle_deck(BTL_DECK_PLA_X,BTL_DECK_PLA_Y)),
		foe_portrait (battle_portrait(BTL_PORTRAIT_X, -BTL_PORTRAIT_Y)),
		foe_deck (battle_deck(180,120))	// HIDE OFFSCREEN
	{
		// make sure textgen is set up before writing any text
		text_generator.set_bg_priority(1);
		text_generator.set_z_order(-500);
		board.current_scene = this;

		type = scene_type::BATTLE;
		bool game_over = false;
		
		// build bg
		_battle_bg.set_priority(3);

		// place hand cards
		for(int i = 0; i < MAX_CARDS_HAND; i++)
		{
			card_positions.push_back(bn::point(cards_x[i], cards_y));
			battle_cards.push_back(battle_card(BTL_DECK_PLA_X, BTL_DECK_PLA_Y));
		}
		battle_cards.at(MAX_CARDS_HAND - 1).set_pattern(tile_pattern::SPECIAL_SINGLE);
		selected_card = 0;

		// skill meters
		_skill_meters.push_back(skill_meter(bn::fixed_point(-110, 25), true, bn::fixed_point(-78, 20)));
		_skill_meters.push_back(skill_meter(bn::fixed_point(78, -25), false, bn::fixed_point(78, -30)));

		// build cursors
		battle_cursor.set_visible(false);
		
		//bn::sprite_ptr cursor_card_sprite = (bn::sprite_items::cursor_card.create_sprite(cards_x[0], cards_y));
		//_cursor_card_idle_action = bn::create_sprite_animate_action_forever(cursor_card_sprite, 16, bn::sprite_items::cursor_card.tiles_item(), 0, 1);
		// _cursor_card_sprite.set_position(cards_x[0], cards_y);
		// _cursor_card_sprite.set_bg_priority(1);
		// _cursor_card_sprite.set_z_order(-100);
		// _cursor_card_sprite.set_visible(false);

		//bn::sprite_ptr cursor_tile_sprite = (bn::sprite_items::cursor_tile.create_sprite(0, -16));
		//_cursor_tile_idle_action = bn::create_sprite_animate_action_forever(cursor_tile_sprite, 16, bn::sprite_items::cursor_tile.tiles_item(), 0, 1);
		// _cursor_tile_sprite.set_position(0, -16);
		// _cursor_tile_sprite.set_bg_priority(1);
		// _cursor_tile_sprite.set_z_order(-100);
		// _cursor_tile_sprite.set_visible(false);

		// set the phase announcement bgs
		// bn::regular_bg_ptr player_phase = bn::regular_bg_items::btl_player_phase.create_bg(0,0);
		// player_phase.set_priority(0); 
		// player_phase.set_blending_enabled(true);
		// player_phase.set_visible(false);
		
		// set base tiles
		bn::sprite_ptr pl_crown = bn::sprite_items::crown.create_sprite(0,0);
		player_base = board.get_tile(1, BOARD_HEIGHT-2);
		player_base->set_owner(tile_owner::PLAYER);
		player_base->set_base(true);
		pl_crown.set_position(player_base->get_position());
		pl_crown.set_bg_priority(player_base->get_sprite()->bg_priority());
		pl_crown.set_z_order(player_base->get_sprite()->z_order() - 10);

		bn::sprite_ptr foe_crown = bn::sprite_items::crown.create_sprite(0,0);
		foe_base = board.get_tile(BOARD_WIDTH-2, 1);
		foe_base->set_owner(tile_owner::FOE);
		foe_base->set_base(true);
		foe_crown.set_position(foe_base->get_position());
		foe_crown.set_bg_priority(foe_base->get_sprite()->bg_priority());
		foe_crown.set_z_order(foe_base->get_sprite()->z_order() - 10);


		player_portrait.set_player_id(0);
		foe_portrait.set_player_id(1);

		// init text generator
		// bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
		// bn::vector<bn::sprite_ptr, 32> text_sprites;
		
		audio::play_track(track_id::BATTLE_BGM);
		
		fade_from_black();

		battle_start();

		while(!game_over)
		{

			player_turn();
			// check for dead zones
			end_turn();
			swap_turns();

			foe_turn();
			// check for dead zones
			end_turn();
			swap_turns();

		}
		fade_to_black();
	}

	battle_scene::~battle_scene()
	{
		action_manager::clear_sprite_move_actions();
	}
	
	void battle_scene::update()
	{
		action_manager::update_sprite_move_actions();
		// burn a random number each update
		unsigned int burn = random.get();

		update_skill_meter_animations();
		effects::update_effect_animations();
		

		// update animations
		battle_cursor.update();
		// _cursor_card_idle_action.update();
		// _cursor_tile_idle_action.update();

		player_deck.update();
		foe_deck.update();

		for(auto it = battle_cards.begin(), end = battle_cards.end(); it != end; ++it)
		{
			battle_card& card = *it;
			card.update();
		}
		
		// slowly pan bg
		// _battle_bg.set_x(_battle_bg.x() - 0.25);
		// _battle_bg.set_y(_battle_bg.y() - 0.25);

		// update transparency 
		preview_transparency_action.update();
		
		bn::core::update();
	}

	void battle_scene::battle_start()
	{
		turn_count = 1;
		current_player = tile_owner::PLAYER;
		set_turn_number(1);

		for(int i = 0; i < battle_cards.size(); i++)
		{
			battle_cards.at(i).move_to_destination(card_positions.at(i));
		}
		for(int t = 0; t < 45; t++)
		{
			update();
		}
		for(auto it = battle_cards.begin(), end = battle_cards.end(); it != end; ++it)
		{
			battle_card& card = *it;
			card.flip_faceup();
		}
	}

	void battle_scene::player_turn()
	{
		turn_state turn_state = turn_state::INTRO;
		current_player = tile_owner::PLAYER;
		bool turn_over = false;
		bool using_special_skill = false;

		turn_state = turn_state::PLAYER_CARD_SELECT;
		select_tile(4,4);
		
		set_battle_cursor_card_mode();
		battle_cursor.set_visible(true);
		// _cursor_tile_sprite.set_visible(false);
		// _cursor_card_sprite.set_visible(true);
		board.set_preview_orientation(direction::NORTH);


		while(!turn_over)
		{
			if(bn::keypad::start_pressed())
			{
				// fade_to_black();
				// fade_from_black();
				player_deck.shuffle();
				
				for(auto it = battle_cards.begin(), end = battle_cards.end(); it != end; ++it)
				{
					battle_card& card = *it;
					card.flip();
				}
			}

			int mov_x = 0;
			int mov_y = 0;
			
			switch(turn_state)
			{
			case turn_state::PLAYER_CARD_SELECT:
				if(bn::keypad::a_pressed())
				{
					audio::play_sound(sound_id::BLIP_HIGH);
					set_battle_cursor_tile_mode();
					// _cursor_card_sprite.set_visible(false);
					// _cursor_tile_sprite.set_visible(true);
					board.update_preview_tiles();
					board.set_preview_pattern(battle_cards.at(selected_card).get_pattern());
					board.show_preview_tiles();

					bn::fixed_point pos = battle_cards.at(selected_card).get_position();
					pos.set_y(card_y_raised);
					battle_cards.at(selected_card).set_position(pos);
					turn_state = turn_state::PLAYER_TILE_PLACEMENT;
				}
				else if(bn::keypad::left_pressed())
				{
					selected_card = (selected_card + (MAX_CARDS_HAND - 1)) % MAX_CARDS_HAND;
					battle_cursor.set_position(card_positions.at(selected_card));
					// _cursor_card_sprite.set_x(cards_x[selected_card]);
				}
				else if(bn::keypad::right_pressed())
				{
					selected_card = (selected_card + 1) % MAX_CARDS_HAND;
					battle_cursor.set_position(card_positions.at(selected_card));
					// _cursor_card_sprite.set_x(cards_x[selected_card]);
				}
				else if(bn::keypad::l_pressed())
				{
					if(_skill_meters.front().meter_filled())
					{
						// use player skill
						using_special_skill = true;
						set_battle_cursor_tile_mode();
						board.set_preview_pattern(tile_pattern::SPECIAL_SINGLE);
						board.show_preview_tiles();
						turn_state = turn_state::PLAYER_TILE_PLACEMENT;
					}
				}

				// Debug Inputs
				if(debug::DEBUG_ENABLED)
				{
					if(bn::keypad::l_pressed())
					{
						board.shift_row_or_col(3, direction::SOUTH);
						// _skill_meters.front().add_sp(-1);
						// _skill_meters.back().add_sp(-1);

					}
					if(bn::keypad::r_pressed())
					{
						board.shift_row_or_col(3, direction::NORTH);
						// _skill_meters.front().add_sp(1);
						// _skill_meters.back().add_sp(1);
					}

					if(bn::keypad::l_held())
					{
						_skill_meters.front().add_sp(-1);
						_skill_meters.back().add_sp(-1);
					}

					if(bn::keypad::r_held())
					{
						_skill_meters.front().add_sp(1);
						_skill_meters.back().add_sp(1);
					}
				}



				break;
			case turn_state::PLAYER_TILE_PLACEMENT:
				// if(bn::keypad::select_pressed())
				// {
				// 	board.set_preview_pattern(tile_patterns::next_tile_pattern(board.preview_pattern));
				// 	battle_cursor.set_position(board.selected_tile->get_position());
				// 	// _cursor_tile_sprite.set_position(board.selected_tile->get_position());
				// }

				if(bn::keypad::l_pressed() && !using_special_skill)
				{
					battle_tile *tile = board.rotate_preview_CCW();
					battle_cursor.set_position(tile->get_position());
					// _cursor_tile_sprite.set_position(tile->get_position());
				}
				if(bn::keypad::r_pressed() && !using_special_skill)
				{
					battle_tile *tile = board.rotate_preview_CW();
					battle_cursor.set_position(tile->get_position());
					// _cursor_tile_sprite.set_position(tile->get_position());
				}

				if(bn::keypad::left_pressed())
				{
					mov_x--;
				}
				else if(bn::keypad::right_pressed())
				{
					mov_x++;
				}
				if(bn::keypad::up_pressed())
				{
					mov_y--;
				}
				else if(bn::keypad::down_pressed())
				{
					mov_y++;
				}

				if(mov_x != 0 || mov_y != 0)
				{
					battle_tile *tile = board.move_selected_tile(mov_x, mov_y);
					battle_cursor.set_position(tile->get_position());
					// _cursor_tile_sprite.set_position(tile->get_position());
					audio::play_sound(sound_id::BLIP_SOFT);
					
				}

				if(bn::keypad::a_pressed())
				{
					if(using_special_skill)
					{
						bool success = board.use_special_action(current_player);
						if(success)
						{
							_skill_meters.front().clear_sp();
							audio::play_sound(sound_id::BLIP_HIGH);
							// switch back to card cursor
							board.hide_preview_tiles();
							battle_cursor.set_visible(false);
							turn_over = true;
						}
						else
						{
							audio::play_sound(sound_id::BLIP_LOW);
						}
					}
					else	// if(!using_special_skill)
					{
						bool success = board.mark_tiles(current_player);
						if(success)
						{
							update_tile_counts();
							// gain SP dependent on .... something
							_skill_meters.front().add_sp(5);
							audio::play_sound(sound_id::BLIP_HIGH);
							// switch back to card cursor
							board.hide_preview_tiles();
							battle_cursor.set_visible(false);
							// _cursor_tile_sprite.set_visible(false);
							// _cursor_card_sprite.set_visible(false);

							// TODO: replace later with discard + draw next turn
							bn::fixed_point pos = battle_cards.at(selected_card).get_position();
							pos.set_y(cards_y);
							battle_cards.at(selected_card).set_position(pos);
							// do not discard if special single
							if(battle_cards.at(selected_card).get_pattern() != tile_pattern::SPECIAL_SINGLE)
							{
								battle_cards.at(selected_card).discard();

							}
							//
							turn_over = true;
						}
						else{	// if(!success)
							audio::play_sound(sound_id::BLIP_LOW);
						}
					}
				}
				else if(bn::keypad::b_pressed())
				{
					// cancel back to card cursor
					audio::play_sound(sound_id::BLIP_LOW);
					board.hide_preview_tiles();
					using_special_skill = false;
					set_battle_cursor_card_mode();
					// _cursor_tile_sprite.set_visible(false);
					// _cursor_card_sprite.set_visible(true);
					bn::fixed_point pos = battle_cards.at(selected_card).get_position();
					pos.set_y(cards_y);
					battle_cards.at(selected_card).set_position(pos);
					turn_state = turn_state::PLAYER_CARD_SELECT;
				}
				break;
			default:
				break;
			}
			update();
		}
	}

	void battle_scene::end_turn()
	{
		board.hide_preview_tiles();
		battle_cursor.set_visible(false);
		board.turn_update();
	}

	void battle_scene::swap_turns()
	{
		if(current_player == tile_owner::FOE)
		{
			current_player = tile_owner::PLAYER;
		}
		else if(current_player == tile_owner::PLAYER)
		{
			current_player = tile_owner::FOE;
		}
		set_turn_number(turn_count+1);
	}


	void battle_scene::set_turn_number(int turn)
	{
		turn_count = turn;
		update_text();
		
	}

	void battle_scene::update_tile_counts()
	{

		update_text();
	}

	void battle_scene::update_text()
	{
		text_sprites.clear();

		// turn indicator
		text_generator.set_left_alignment();
		text_generator.generate(-117, -72, "Turn", text_sprites);
		text_generator.generate(-85, -72, bn::to_string<2>(bn::min(turn_count, 99)), text_sprites);

		// tile counts
		text_generator.set_center_alignment();
		text_generator.generate(-94, -60, "/", text_sprites);
		text_generator.set_right_alignment();
		int player_count = board.count_tiles_with_owner(tile_owner::PLAYER);
		text_generator.generate(-100, -60, bn::to_string<2>(bn::min(player_count, 99)), text_sprites);
		text_generator.set_left_alignment();
		int foe_count = board.count_tiles_with_owner(tile_owner::FOE);
		text_generator.generate(-88, -60, bn::to_string<2>(bn::min(foe_count, 99)), text_sprites);
	}

	void battle_scene::select_tile(int x, int y)
	{
		board.set_selected_tile(x, y);
		battle_cursor.set_position(board.get_selected_tile()->get_position());
		// _cursor_tile_sprite.set_position(board.get_selected_tile()->get_position());
	}

	void battle_scene::select_tile(bn::point pos)
	{
		select_tile(pos.x(), pos.y());
	}

	
	void battle_scene::set_battle_cursor_card_mode()
	{
		battle_cursor.set_size(bn::point(16,24));
		battle_cursor.set_position(card_positions.at(selected_card));
	}

	void battle_scene::set_battle_cursor_tile_mode()
	{
		battle_cursor.set_size(bn::point(8,8));
		battle_cursor.set_position(board.get_selected_tile()->get_position());
	}

}