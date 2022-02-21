#include "sh_battle_scene.h"
#include "sh_audio.h"
#include "sh_debug.h"

#include <bn_keypad.h>


/**************************************************/
/** Separate file for handling the player's turn **/
/**************************************************/

namespace sh
{

	void battle_scene::player_turn()
	{
		turn_state turn_state = turn_state::INTRO;
		current_player = tile_owner::PLAYER;
		bool turn_over = false;
		bool using_special_skill = false;

		turn_state = turn_state::PLAYER_CARD_SELECT;
		
		for(auto it = battle_cards.begin(), end = battle_cards.end(); it != end; ++it)
		{
			if(it->is_discarded())
			{

				player_deck.draw_card_with_animation(*this, battle_cards.at(selected_card));
				battle_cards.at(selected_card).flip_faceup();
			}
		}


		set_battle_cursor_card_mode();
		battle_cursor.set_visible(true);
		board.set_preview_orientation(direction::NORTH);


		while(!turn_over && !scene_done)
		{
			// int mov_x = 0;
			// int mov_y = 0;
			bn::point mov(0,0);

			if(bn::keypad::start_pressed())
			{
				open_pause_menu();
			}
			else switch(turn_state)
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
						board.set_selected_tile(player_base->coordinates);
						battle_cursor.set_position(player_base->get_position());
						board.show_preview_tiles();
						turn_state = turn_state::PLAYER_SPECIAL_PLACEMENT;
					}
					else
					{
						audio::play_sound(sound_id::UNSELECTABLE);
					}
				}

				// Debug Inputs
				if(debug::DEBUG_ENABLED)
				{
					if(bn::keypad::l_pressed())
					{
						// board.shift_row_or_col(3, direction::SOUTH);
						// _skill_meters.front().add_sp(-1);
						// _skill_meters.back().add_sp(-1);
						// apply_damage_to_player(tile_owner::PLAYER, 100);
						board.set_preview_tiles_valid(false);
					}
					if(bn::keypad::r_pressed())
					{
						// board.shift_row_or_col(3, direction::NORTH);
						// _skill_meters.front().add_sp(1);
						// _skill_meters.back().add_sp(1);
						// apply_damage_to_player(tile_owner::FOE, 100);
						board.set_preview_tiles_valid(true);
					}

					if(bn::keypad::l_held())
					{
						_skill_meters.front().add_val(-1);
						_skill_meters.back().add_val(-1);
					}

					if(bn::keypad::r_held())
					{
						_skill_meters.front().add_val(1);
						_skill_meters.back().add_val(1);
					}
					if(bn::keypad::select_held())
					{
						battle_cards.at(2).flip();
					}
				}



				break;
			case turn_state::PLAYER_TILE_PLACEMENT:
				if(debug::DEBUG_ENABLED && bn::keypad::select_pressed())
				{
					board.set_preview_pattern(tile_patterns::next_tile_pattern(board.preview_pattern));
					battle_cursor.set_position(board.selected_tile->get_position());
				}

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
					mov.set_x(-1);
				}
				else if(bn::keypad::right_pressed())
				{
					mov.set_x(1);
				}
				if(bn::keypad::up_pressed())
				{
					mov.set_y(-1);
				}
				else if(bn::keypad::down_pressed())
				{
					mov.set_y(1);
				}

				if(mov.x() != 0 || mov.y() != 0)
				{
					battle_tile *tile = board.move_selected_tile(mov);
					battle_cursor.set_position(tile->get_position());
					// _cursor_tile_sprite.set_position(tile->get_position());
					audio::play_sound(sound_id::BLIP_SOFT);
					
				}

				if(bn::keypad::a_pressed())
				{
					int sp_gain = 0;
					bool success = board.mark_tiles(current_player, sp_gain);
					if(success)
					{
						update_tile_counts();
						// gain SP dependent on .... something
						_skill_meters.front().add_val(sp_gain);
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
							wait_for_update_cycles(36);
						}
						turn_over = true;
					}
					else{	// if(!success)
						audio::play_sound(sound_id::UNSELECTABLE);
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
			case turn_state::PLAYER_SPECIAL_PLACEMENT:
				if(bn::keypad::left_pressed())
				{
					mov.set_x(-1);
				}
				else if(bn::keypad::right_pressed())
				{
					mov.set_x(1);
				}
				if(bn::keypad::up_pressed())
				{
					mov.set_y(-1);
				}
				else if(bn::keypad::down_pressed())
				{
					mov.set_y(1);
				}

				if(mov.x() != 0 || mov.y() != 0)
				{
					battle_tile *tile = board.move_selected_tile(mov, tile_owner::PLAYER);
					battle_cursor.set_position(tile->get_position());
					// _cursor_tile_sprite.set_position(tile->get_position());
					audio::play_sound(sound_id::BLIP_SOFT);
					
				}
				if(bn::keypad::a_pressed())
				{
					bool success = board.use_special_action(current_player, special_action_pattern::STAR_3);
					if(success)
					{
						_skill_meters.front().clear();
						audio::play_sound(sound_id::BLIP_HIGH);
						// switch back to card cursor
						board.hide_preview_tiles();
						battle_cursor.set_visible(false);
						turn_over = true;
					}
					else
					{
						audio::play_sound(sound_id::UNSELECTABLE);
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
}