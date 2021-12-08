#include "sh_battle.h"
#include "sh_battle_tile.h"
#include "sh_battle_deck.h"

#include "bn_core.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_random.h"


// graphics
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


	battle_scene::battle_scene() :
		battle_bg (bn::regular_bg_items::battle_bg.create_bg(0, 0)),
		cursor_card_sprite (bn::sprite_items::cursor_card.create_sprite(cards_x[0], cards_y)),
		cursor_tile_sprite (bn::sprite_items::cursor_tile.create_sprite(0, -16)),
		cursor_card_idle_action (bn::create_sprite_animate_action_forever(cursor_card_sprite, 16, bn::sprite_items::cursor_card.tiles_item(), 0, 1)),
		cursor_tile_idle_action (bn::create_sprite_animate_action_forever(cursor_tile_sprite, 16, bn::sprite_items::cursor_tile.tiles_item(), 0, 1))
	{
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

		bool game_over = false;

		current_player = tile_owner::PLAYER;

		// place hand cards
		selected_card = 0;
		//bn::vector<bn::sprite_ptr, MAX_CARDS_HAND> card_sprites;
		
		for(int i = 0; i < MAX_CARDS_HAND; i++)
		{
			//card_sprites.push_back(bn::sprite_items::card_blank.create_sprite(cards_x[i], cards_y));
			battle_cards.push_back(battle_card(cards_x[i], cards_y));
		}



		// build cursors
		
		//bn::sprite_ptr cursor_card_sprite = (bn::sprite_items::cursor_card.create_sprite(cards_x[0], cards_y));
		cursor_card_sprite.set_position(cards_x[0], cards_y);
		//cursor_card_idle_action = bn::create_sprite_animate_action_forever(cursor_card_sprite, 16, bn::sprite_items::cursor_card.tiles_item(), 0, 1);
		cursor_card_sprite.set_bg_priority(1);
		cursor_card_sprite.set_z_order(-100);
		cursor_card_sprite.set_visible(true);

		//bn::sprite_ptr cursor_tile_sprite = (bn::sprite_items::cursor_tile.create_sprite(0, -16));
		cursor_tile_sprite.set_position(0, -16);
		//cursor_tile_idle_action = bn::create_sprite_animate_action_forever(cursor_tile_sprite, 16, bn::sprite_items::cursor_tile.tiles_item(), 0, 1);
		cursor_tile_sprite.set_bg_priority(1);
		cursor_tile_sprite.set_z_order(-100);
		cursor_tile_sprite.set_visible(false);

		// build bg
		//bn::regular_bg_ptr battle_bg = bn::regular_bg_items::battle_bg.create_bg(0, 0);
		battle_bg.set_priority(3);

		
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





		bn::sprite_ptr portrait_frame_r = bn::sprite_items::portrait_frame.create_sprite(96, -56);
		bn::sprite_ptr portrait_frame_l = bn::sprite_items::portrait_frame.create_sprite(-96, 56);

		battle_deck player_deck = battle_deck(-96,15);
		battle_deck foe_deck = battle_deck(96,-15);


		while(!game_over)
		{
			// player turn

			// foe turn


			player_turn();

			swap_turns();

			foe_turn();

			swap_turns();



			

			
		}

	}

	
	void battle_scene::update()
	{
		// update animations
		cursor_card_idle_action.update();
		cursor_tile_idle_action.update();
		
		// slowly pan bg
		battle_bg.set_x(battle_bg.x() - 0.25);
		battle_bg.set_y(battle_bg.y() - 0.25);
		bn::core::update();
	}

	void battle_scene::player_turn()
	{
		turn_state turn_state = turn_state::INTRO;
		current_player = tile_owner::PLAYER;
		bool turn_over = false;

		turn_state = turn_state::PLAYER_CARD_SELECT;
		cursor_tile_sprite.set_visible(false);
		cursor_card_sprite.set_visible(true);


		select_tile(4,4);

		while(!turn_over)
		{
			int mov_x = 0;
			int mov_y = 0;
			switch(turn_state)
			{
			case turn_state::PLAYER_CARD_SELECT:
				if(bn::keypad::a_pressed())
				{
					bn::sound_items::blip_high.play();
					cursor_card_sprite.set_visible(false);
					cursor_tile_sprite.set_visible(true);
					board.update_preview_tiles();
					board.set_preview_pattern(battle_cards.at(selected_card).get_pattern());
					board.show_preview_tiles();
					turn_state = turn_state::PLAYER_TILE_PLACEMENT;
				}
				else if(bn::keypad::left_pressed())
				{
					selected_card = (selected_card + 4) % 5;
					cursor_card_sprite.set_x(cards_x[selected_card]);
				}
				else if(bn::keypad::right_pressed())
				{
					selected_card = (selected_card + 1) % 5;
					cursor_card_sprite.set_x(cards_x[selected_card]);
				}
				break;
			case turn_state::PLAYER_TILE_PLACEMENT:
				if(bn::keypad::select_pressed())
				{
					board.set_preview_pattern(tile_patterns::next_tile_pattern(board.preview_pattern));
					cursor_tile_sprite.set_position(board.selected_tile->get_position());
				}

				if(bn::keypad::l_pressed())
				{
					battle_tile *tile = board.rotate_preview_CCW();
					cursor_tile_sprite.set_position(tile->get_position());
				}
				if(bn::keypad::r_pressed())
				{
					battle_tile *tile = board.rotate_preview_CW();
					cursor_tile_sprite.set_position(tile->get_position());
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
					cursor_tile_sprite.set_position(tile->get_position());
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
						cursor_tile_sprite.set_visible(false);
						cursor_card_sprite.set_visible(false);
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
					cursor_tile_sprite.set_visible(false);
					cursor_card_sprite.set_visible(true);
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
		cursor_tile_sprite.set_visible(false);
		cursor_card_sprite.set_visible(false);
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
		cursor_tile_sprite.set_position(board.get_selected_tile()->get_position());
	}

}