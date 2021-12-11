#include "sh_battle_scene.h"
#include "sh_battle_tile.h"
#include "sh_battle_deck.h"

#include "bn_core.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_random.h"


// graphics
#include "bn_blending_actions.h"
#include "bn_sprite_animate_actions.h"
// text & fonts
#include "common_variable_8x16_sprite_font.h"
// backgrounds
#include "bn_regular_bg_items_battle_bg.h"
#include "bn_regular_bg_items_battle_board.h"
// sound
#include "bn_sound_items.h"
// sprites
#include "bn_sprite_items_portrait_frame.h"
#include "bn_sprite_items_card_blank.h"
#include "bn_sprite_items_cursor_card.h"
#include "bn_sprite_items_cursor_tile.h"
#include "bn_sprite_items_crown.h"


#include "bn_log.h"


namespace sh{

	#define BTL_PORTRAIT_X 94
	#define BTL_PORTRAIT_Y 58
	#define BTL_DECK_PLA_X -50
	#define BTL_DECK_PLA_Y 62
	#define BTL_DECK_FOE_X 96
	#define BTL_DECK_FOE_Y -15

	battle_scene::battle_scene() :
		_battle_bg (bn::regular_bg_items::battle_bg.create_bg(0, 0)),
		_cursor_card_sprite (bn::sprite_items::cursor_card.create_sprite(cards_x[0], cards_y)),
		_cursor_tile_sprite (bn::sprite_items::cursor_tile.create_sprite(0, -16)),
		_cursor_card_idle_action (bn::create_sprite_animate_action_forever(_cursor_card_sprite, 16, bn::sprite_items::cursor_card.tiles_item(), 0, 1)),
		_cursor_tile_idle_action (bn::create_sprite_animate_action_forever(_cursor_tile_sprite, 16, bn::sprite_items::cursor_tile.tiles_item(), 0, 1)),
		preview_transparency_action (bn::blending_transparency_alpha_loop_action(30,0.2)),
		player_portrait (battle_portrait(-BTL_PORTRAIT_X, BTL_PORTRAIT_Y)),
		player_deck (battle_deck(BTL_DECK_PLA_X,BTL_DECK_PLA_Y)),
		foe_portrait (battle_portrait(BTL_PORTRAIT_X, -BTL_PORTRAIT_Y)),
		foe_deck (battle_deck(180,120))	// HIDE OFFSCREEN
	{
		type = scene_type::BATTLE;
		
		bool game_over = false;

		current_player = tile_owner::PLAYER;

		// place hand cards
		//bn::vector<bn::sprite_ptr, MAX_CARDS_HAND> card_sprites;
		for(int i = 0; i < MAX_CARDS_HAND; i++)
		{
			//card_sprites.push_back(bn::sprite_items::card_blank.create_sprite(cards_x[i], cards_y));
			battle_cards.push_back(battle_card(cards_x[i], cards_y));
		}
		battle_cards.at(MAX_CARDS_HAND - 1).set_pattern(tile_pattern::SPECIAL_SINGLE);
		selected_card = 0;

		// skill meters
		_skill_meters.push_back(skill_meter(-110, 33, true));
		_skill_meters.push_back(skill_meter(78, -33, true));

		// build cursors
		
		//bn::sprite_ptr cursor_card_sprite = (bn::sprite_items::cursor_card.create_sprite(cards_x[0], cards_y));
		//_cursor_card_idle_action = bn::create_sprite_animate_action_forever(cursor_card_sprite, 16, bn::sprite_items::cursor_card.tiles_item(), 0, 1);
		_cursor_card_sprite.set_position(cards_x[0], cards_y);
		_cursor_card_sprite.set_bg_priority(1);
		_cursor_card_sprite.set_z_order(-100);
		_cursor_card_sprite.set_visible(false);

		//bn::sprite_ptr cursor_tile_sprite = (bn::sprite_items::cursor_tile.create_sprite(0, -16));
		//_cursor_tile_idle_action = bn::create_sprite_animate_action_forever(cursor_tile_sprite, 16, bn::sprite_items::cursor_tile.tiles_item(), 0, 1);
		_cursor_tile_sprite.set_position(0, -16);
		_cursor_tile_sprite.set_bg_priority(1);
		_cursor_tile_sprite.set_z_order(-100);
		_cursor_tile_sprite.set_visible(false);

		// build bg
		//bn::regular_bg_ptr battle_bg = bn::regular_bg_items::battle_bg.create_bg(0, 0);
		_battle_bg.set_priority(3);

		
		// set base tiles
		bn::sprite_ptr pl_crown = bn::sprite_items::crown.create_sprite(0,0);
		player_base = board.get_tile(1, BOARD_HEIGHT-2);
		player_base->set_owner(tile_owner::PLAYER);
		player_base->set_base(true);
		pl_crown.set_position(player_base->get_position());
		pl_crown.set_bg_priority(player_base->sprite.bg_priority());
		pl_crown.set_z_order(player_base->sprite.z_order() - 10);

		bn::sprite_ptr foe_crown = bn::sprite_items::crown.create_sprite(0,0);
		foe_base = board.get_tile(BOARD_WIDTH-2, 1);
		foe_base->set_owner(tile_owner::FOE);
		foe_base->set_base(true);
		foe_crown.set_position(foe_base->get_position());
		foe_crown.set_bg_priority(foe_base->sprite.bg_priority());
		foe_crown.set_z_order(foe_base->sprite.z_order() - 10);


		player_portrait.set_player_id(0);
		foe_portrait.set_player_id(1);


		//bn::sprite_ptr portrait_frame_r = bn::sprite_items::portrait_frame.create_sprite(96, -56);
		//bn::sprite_ptr portrait_frame_l = bn::sprite_items::portrait_frame.create_sprite(-96, 56);

		// player_deck = battle_deck(-96,15);
		// foe_deck = battle_deck(96,-15);

		// init text generator
		//	bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
		// bn::vector<bn::sprite_ptr, 32> text_sprites;
		// text_generator.set_bg_priority(0);
		// text_generator.set_z_order(-500);
		// text_generator.generate(0, -72, "Look ma, I'm on a GBA!", text_sprites);

		//	text_generator.set_center_alignment();
		// constexpr bn::string_view info_text_lines[] = {
		// 	"A: hide/show sprite",
		// 	"",
		// 	"START: go to next scene",
		// };

		
		fade_from_black();


		while(!game_over)
		{

			player_turn();
			// check for dead zones
			swap_turns();

			foe_turn();
			// check for dead zones
			swap_turns();

		}
		fade_to_black();

	}

	
	void battle_scene::update()
	{
		// update animations
		_cursor_card_idle_action.update();
		_cursor_tile_idle_action.update();

		if(!player_deck.anim_shuffle.done())
		{
			player_deck.anim_shuffle.update();
		}
		if(!foe_deck.anim_shuffle.done())
		{
			foe_deck.anim_shuffle.update();
		}
		
		// slowly pan bg
		_battle_bg.set_x(_battle_bg.x() - 0.25);
		_battle_bg.set_y(_battle_bg.y() - 0.25);

		// update transparency 
		preview_transparency_action.update();
		
		bn::core::update();
	}

	void battle_scene::player_turn()
	{
		turn_state turn_state = turn_state::INTRO;
		current_player = tile_owner::PLAYER;
		bool turn_over = false;

		turn_state = turn_state::PLAYER_CARD_SELECT;
		_cursor_tile_sprite.set_visible(false);
		_cursor_card_sprite.set_visible(true);
		board.set_preview_orientation(DIRECTION_N);

		select_tile(4,4);

		while(!turn_over)
		{
			if(bn::keypad::start_pressed())
			{
				fade_to_black();
				fade_from_black();
			}

			int mov_x = 0;
			int mov_y = 0;
			switch(turn_state)
			{
			case turn_state::PLAYER_CARD_SELECT:
				if(bn::keypad::a_pressed())
				{
					bn::sound_items::blip_high.play();
					_cursor_card_sprite.set_visible(false);
					_cursor_tile_sprite.set_visible(true);
					board.update_preview_tiles();
					board.set_preview_pattern(battle_cards.at(selected_card).get_pattern());
					board.show_preview_tiles();

					bn::point pos = battle_cards.at(selected_card).get_position();
					pos.set_y(card_y_raised);
					battle_cards.at(selected_card).set_position(pos);
					turn_state = turn_state::PLAYER_TILE_PLACEMENT;
				}
				else if(bn::keypad::left_pressed())
				{
					selected_card = (selected_card + (MAX_CARDS_HAND - 1)) % MAX_CARDS_HAND;
					_cursor_card_sprite.set_x(cards_x[selected_card]);
				}
				else if(bn::keypad::right_pressed())
				{
					selected_card = (selected_card + 1) % MAX_CARDS_HAND;
					_cursor_card_sprite.set_x(cards_x[selected_card]);
				}
				if(bn::keypad::l_held())
				{
					_skill_meters.front().add_sp(-1);
				}
				if(bn::keypad::r_held())
				{
					_skill_meters.front().add_sp(1);
				}



				break;
			case turn_state::PLAYER_TILE_PLACEMENT:
				if(bn::keypad::select_pressed())
				{
					board.set_preview_pattern(tile_patterns::next_tile_pattern(board.preview_pattern));
					_cursor_tile_sprite.set_position(board.selected_tile->get_position());
				}

				if(bn::keypad::l_pressed())
				{
					battle_tile *tile = board.rotate_preview_CCW();
					_cursor_tile_sprite.set_position(tile->get_position());
				}
				if(bn::keypad::r_pressed())
				{
					battle_tile *tile = board.rotate_preview_CW();
					_cursor_tile_sprite.set_position(tile->get_position());
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
					_cursor_tile_sprite.set_position(tile->get_position());
					bn::sound_items::blip_soft.play();
				}

				if(bn::keypad::a_pressed())
				{
					bool success = board.mark_tiles(current_player);
					if(success)
					{
						bn::sound_items::blip_high.play();
						// switch back to card cursor
						board.hide_preview_tiles();
						_cursor_tile_sprite.set_visible(false);
						_cursor_card_sprite.set_visible(false);

						// TODO: replace later with discard + draw next turn
						bn::point pos = battle_cards.at(selected_card).get_position();
						pos.set_y(cards_y);
						battle_cards.at(selected_card).set_position(pos);
						//
						turn_over = true;
					}
					else{
						bn::sound_items::blip_low.play();
					}
				}
				else if(bn::keypad::b_pressed())
				{
					// cancel back to card cursor
					bn::sound_items::blip_low.play();
					board.hide_preview_tiles();
					_cursor_tile_sprite.set_visible(false);
					_cursor_card_sprite.set_visible(true);
					bn::point pos = battle_cards.at(selected_card).get_position();
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



	void battle_scene::foe_turn()
	{
		// placeholder AI: randomly slap down a tile until something fits 
		current_player = tile_owner::FOE;

		//tile_pattern pattern = tile_pattern::SINGLE;
		tile_pattern pattern = tile_patterns::random_tile_pattern();
		board.set_preview_pattern(pattern);

		bn::vector<battle_tile*, 80> foe_active_tiles;
		bn::vector<battle_tile*, 80> foe_neighboring_tiles;
		bool tile_checked[81];

		foe_active_tiles.clear();
		foe_neighboring_tiles.clear();
		for(int i = 0; i < 81; i++)
		{
			tile_checked[i] = false;
		}

		foe_active_tiles.push_back(foe_base);
		while(!foe_active_tiles.empty())
		{
			battle_tile *active_tile = foe_active_tiles.back();
			foe_active_tiles.pop_back();
			tile_checked[active_tile->tile_id] = true;
			for(int i = 0; i < 4; i++)
			{
				battle_tile *neighbor = active_tile->neighbors[i];
				if(neighbor != NULL)
				{
					if(tile_checked[neighbor->tile_id])
					{
						continue;
					}
					if(neighbor->get_owner() == tile_owner::FOE)
					{
						foe_active_tiles.push_back(neighbor);
					}
					else
					{
						bool neighbor_registered = false;
						for(int n = 0; n < foe_neighboring_tiles.size(); n++)
						{
							if(foe_neighboring_tiles.at(n) == neighbor)
							{
								neighbor_registered = true;
								break;
							}
						}
						if(!neighbor_registered)
						{
							foe_neighboring_tiles.push_back(neighbor);
						}
					}
				}
			}
		}
		if(foe_neighboring_tiles.empty())
		{
			// uh oh ¯\_(ツ)_/¯
			return;
		}
		int r = random.get_int(0, foe_neighboring_tiles.size());
		battle_tile *selected_tile = foe_neighboring_tiles.at(r);
		if(selected_tile != NULL)
		{
			select_tile(selected_tile->coordinates.x(), selected_tile->coordinates.y());
			bool success = board.mark_tiles(current_player);
			if(!success)
			{
				// uh oh ¯\_(ツ)_/¯
			}
		}
	}






	void battle_scene::swap_turns()
	{
		board.hide_preview_tiles();
		_cursor_tile_sprite.set_visible(false);
		_cursor_card_sprite.set_visible(false);
		if(current_player == tile_owner::FOE)
		{
			current_player = tile_owner::PLAYER;
		}
		else if(current_player == tile_owner::PLAYER)
		{
			current_player = tile_owner::FOE;
		}
	}

	void battle_scene::select_tile(int x, int y)
	{
		board.set_selected_tile(x, y);
		_cursor_tile_sprite.set_position(board.get_selected_tile()->get_position());
	}

	

}