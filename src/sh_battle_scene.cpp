#include "sh_battle_scene.h"
#include "sh_battle_deck.h"
#include "sh_battle_tile.h"
#include "sh_debug.h"
#include "sh_audio.h"
#include "sh_effects.h"
#include "sh_menu.h"
#include "sh_action_manager.h"
#include "sh_random.h"

#include <bn_blending_actions.h>
#include <bn_core.h>
#include <bn_camera_ptr.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_log.h>
#include <bn_regular_bg_builder.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>

// backgrounds
#include "bn_regular_bg_items_battle_bg_board_wood.h"
#include "bn_regular_bg_items_battle_ui.h"
#include "bn_regular_bg_items_btl_player_phase.h"
#include "bn_regular_bg_items_btl_enemy_phase.h"
#include "bn_regular_bg_items_btl_player_win.h"
#include "bn_regular_bg_items_btl_player_lose.h"
#include "bn_regular_bg_items_pause_menu.h"
// sprites
#include "bn_sprite_items_portrait_frame.h"
#include "bn_sprite_items_card_blank.h"
#include "bn_sprite_items_crown.h"
#include "bn_sprite_items_skill_meter_flame.h"
#include "bn_sprite_items_board_tile.h"
// text & fonts
#include "variable_8x16_sprite_font.h"



namespace sh
{
	
	#define PLAYER_SIDE	0
	#define FOE_SIDE	1

	#define BTL_PORTRAIT_X 94
	#define BTL_PORTRAIT_Y 54
	#define BTL_DECK_PLA_X -50
	#define BTL_DECK_PLA_Y 62

	bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
	bn::vector<bn::sprite_ptr, 12> text_sprites;

	static bn::point player_most_recent_tile(4,4);
	static battle_scene *_instance_ = NULL;

	battle_scene::battle_scene() :
		_battle_board_bg (bn::regular_bg_items::battle_bg_board_wood.create_bg(0, 0)),
		_battle_ui_bg(bn::regular_bg_items::battle_ui.create_bg(0,0)),
		_turn_announcement (bn::regular_bg_items::btl_player_phase.create_bg(0,0)),
		board (battle_board(this)),
		preview_transparency_action (bn::blending_transparency_alpha_loop_action(30,0.2)),
		player_portrait (battle_portrait(bn::fixed_point(-BTL_PORTRAIT_X, BTL_PORTRAIT_Y), true)),
		player_deck (battle_deck_with_sprite(bn::fixed_point(BTL_DECK_PLA_X,BTL_DECK_PLA_Y))),
		foe_portrait (battle_portrait(bn::fixed_point(BTL_PORTRAIT_X, -BTL_PORTRAIT_Y), false))
	{
		// make sure textgen is set up before writing any text
		text_generator.set_bg_priority(1);
		text_generator.set_z_order(-500);
		text_sprites.clear();
		board.current_scene = this;

		_instance_ = this;
		type = scene_type::BATTLE;
		scene_done = false;
		
		// build bg
		_battle_board_bg.set_priority(3);
		_battle_board_bg.set_camera(_camera);
		_battle_ui_bg.set_priority(2);

		player_deck.set_camera(_camera);
		// place hand cards
		bn::fixed_point card_spawn_pos = player_deck.get_card_pos();
		for(int i = 0; i < MAX_CARDS_HAND; i++)
		{
			card_positions.push_back(bn::point(cards_x[i], cards_y));
			battle_cards.push_back(battle_card(card_positions.back()));
			battle_cards.back().set_position(card_spawn_pos);
			battle_cards.back().set_camera(_camera);
		}
		battle_cards.back().set_pattern(tile_pattern::SPECIAL_SINGLE);
		battle_cards.back().set_position(battle_cards.back().get_hand_position());
		battle_cards.back().set_visible(true);
		selected_card = 0;

		// health meters
		player_health = MAX_HEALTH;
		foe_health = MAX_HEALTH;
		_health_meters.push_back(ui_meter(bn::fixed_point(-109, 18), meter_type::HP, MAX_HEALTH, true));
		_health_meters.push_back(ui_meter(bn::fixed_point(77, -25), meter_type::HP, MAX_HEALTH, true));
		// skill meters
		_skill_meters.push_back(skill_meter(bn::fixed_point(-109, 25), 40, true, bn::fixed_point(32, -5), true));
		_skill_meters.push_back(skill_meter(bn::fixed_point(77, -18), 40, true, bn::fixed_point(-0, -5), false));
		// _skill_meters.push_back(skill_meter(bn::fixed_point(77, -25), false, bn::fixed_point(0, -5)));

		// build cursors
		battle_cursor.set_visible(false);
		
		// _cursor_tile_sprite.set_visible(false);

		// set the phase announcement bgs
		_turn_announcement = bn::regular_bg_items::btl_player_phase.create_bg(0,0);
		_turn_announcement.set_priority(0); 
		_turn_announcement.set_blending_enabled(false);
		_turn_announcement.set_visible(false);
		
		// set base tiles
		{
			player_base = board.get_tile(1, BOARD_HEIGHT-2);
			// player_base->set_owner(tile_owner::PLAYER);
			// player_base->set_base(true);

			// battle_tile *tile = board.get_tile(0, BOARD_HEIGHT-2);
			// tile->set_owner(tile_owner::PLAYER);
			// tile->set_base(true);

			// tile = board.get_tile(0, BOARD_HEIGHT-1);
			// tile->set_owner(tile_owner::PLAYER);
			// tile->set_base(true);

			// tile = board.get_tile(1, BOARD_HEIGHT-1);
			// tile->set_owner(tile_owner::PLAYER);
			// tile->set_base(true);
		}

		{
			foe_base = board.get_tile(BOARD_WIDTH-2, 1);
			// foe_base->set_owner(tile_owner::FOE);
			// foe_base->set_base(true);

			// battle_tile *tile = board.get_tile(BOARD_WIDTH-2, 0);
			// tile->set_owner(tile_owner::FOE);
			// tile->set_base(true);

			// tile = board.get_tile(BOARD_WIDTH-1, 0);
			// tile->set_owner(tile_owner::FOE);
			// tile->set_base(true);

			// tile = board.get_tile(BOARD_WIDTH-1, 1);
			// tile->set_owner(tile_owner::FOE);
			// tile->set_base(true);
		}
		
		// bn::sprite_builder builder(bn::sprite_items::crown);
		// builder.set_bg_priority(player_base->get_sprite()->bg_priority());
		// builder.set_z_order(player_base->get_sprite()->z_order() - 10);
		// builder.set_camera(_camera);
		// builder.set_position(player_base->get_position());
		// bn::sprite_ptr player_crown = builder.build();
		// builder.set_position(foe_base->get_position());
		// bn::sprite_ptr foe_crown = builder.release_build();

		player_portrait.set_character(characters::player_character());
		foe_portrait.set_character(characters::foe_character());

		// init text generator
		// bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
		// bn::vector<bn::sprite_ptr, 32> text_sprites;
		
		audio::play_track(track_id::BATTLE_BGM);
		
		fade_from_black();

		battle_start();

		while(!scene_done)
		{
			turn_intro(tile_owner::PLAYER);
			player_turn();
			// check for dead zones
			end_turn();

			//check between turns too
			if(scene_done)
				break;

			turn_intro(tile_owner::FOE);
			foe_turn();
			// check for dead zones
			end_turn();

		}
		fade_to_black();
	}

	battle_scene::~battle_scene()
	{
		_health_meters.clear();
		_skill_meters.clear();
		text_sprites.clear();
		card_positions.clear();
		battle_cards.clear();
		action_manager::clear_sprite_move_actions();
	}
	
	void battle_scene::update()
	{
		action_manager::update_sprite_move_actions();
		
		for(auto it = _skill_meters.begin(), end = _skill_meters.end(); it != end; ++it)
		{
			it->update();
		}
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

		// update transparency 
		preview_transparency_action.update();
		
		update_text();

		scene::update();
	}

	void battle_scene::reset_battle()
	{
		turn_count = 0;
		for(auto it = _health_meters.begin(), end = _health_meters.end(); it != end; ++it)
		{
			it->fill();
		}
		for(auto it = _skill_meters.begin(), end = _skill_meters.end(); it != end; ++it)
		{
			it->clear();
		}
		player_deck.reset();
		foe_deck.reset();
		player_health = MAX_HEALTH;
		foe_health = MAX_HEALTH;
	}

	void battle_scene::battle_start()
	{
		reset_battle();
		player_deck.randomize();
		foe_deck.randomize();
		battle_cards.back().set_position(battle_cards.back().get_hand_position());
		battle_cards.back().set_visible(true);
		
		auto second_last = battle_cards.end();
		--second_last;
		for(auto it = battle_cards.begin(); it != second_last; ++it)
		{
			it->set_visible(false);
		}

		for(int i = 0; i < 3; i++)
		{
			player_deck.shuffle();
			wait_for_update_cycles(28);
		}
		wait_for_update_cycles(12);

		turn_count = 0;
		current_player = tile_owner::PLAYER;
		player_most_recent_tile = bn::point(4,4);
		select_tile(player_most_recent_tile); // start tile cursor at center of board

		for(auto it = battle_cards.begin(); it != second_last; ++it)
		{
			it->set_visible(true);
			// it->move_to_destination(it->get_hand_position());
			player_deck.draw_card_with_animation(*this, *it);
		}

		for(auto it = battle_cards.begin(), end = battle_cards.end(); it != end; ++it)
		{
			it->flip_faceup();
		}

		
		wait_for_update_cycles(30);
	}

	void battle_scene::battle_end(tile_owner winner)
	{
		wait_for_update_cycles(12);
		dim_screen();
		switch(winner)
		{
		case tile_owner::PLAYER:
			audio::play_track(track_id::BATTLE_WIN);
			_turn_announcement = bn::regular_bg_items::btl_player_win.create_bg(0,0);
			break;
		case tile_owner::FOE:
			audio::play_track(track_id::BATTLE_LOSE);
			_turn_announcement = bn::regular_bg_items::btl_player_lose.create_bg(0,0);
			break;
		default:
			break;
		}
		_turn_announcement.set_palette(bn::regular_bg_items::btl_player_win.palette_item());
		_turn_announcement.set_priority(0); 
		_turn_announcement.set_visible(true);
		wait_for_update_cycles(60);
		while(!bn::keypad::any_pressed())
		{
			update();
		}
		
		scene_management::set_next_scene(scene_type::TITLE);
		scene_done = true;
		_turn_announcement.set_visible(false);

	}

	

	
	void battle_scene::turn_intro(tile_owner player)
	{
		dim_screen();
		current_player = player;
		set_turn_number(turn_count+1);
		switch(player)
		{
		case tile_owner::PLAYER:
			select_tile(player_most_recent_tile);
			_turn_announcement = bn::regular_bg_items::btl_player_phase.create_bg(0,0);
			break;
		case tile_owner::FOE:
			_turn_announcement = bn::regular_bg_items::btl_enemy_phase.create_bg(0,0);
			break;
		default:
			break;
		}
		_turn_announcement.set_priority(0); 
		_turn_announcement.set_palette(bn::regular_bg_items::btl_player_phase.palette_item());
		_turn_announcement.set_visible(true);
		wait_for_update_cycles(60);
		_turn_announcement.set_visible(false);
		undim_screen();
	}


	void battle_scene::end_turn()
	{
		board.hide_preview_tiles();
		battle_cursor.set_visible(false);
		board.turn_update();
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

		
		// burn a random number each update
		volatile unsigned int burn = random.get();
		// text_generator.generate(100, -6, bn::to_string<4>(burn % 10000), text_sprites);
	}

	void battle_scene::select_tile(int x, int y)
	{
		board.set_selected_tile(x, y);
		battle_cursor.set_position(board.get_selected_tile()->get_position());
		// _cursor_tile_sprite.set_position(board.get_selected_tile()->get_position());
		if(current_player == tile_owner::PLAYER)
		{
			player_most_recent_tile = bn::point(x,y);
		}
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

	void battle_scene::open_pause_menu()
	{
		text_sprites.clear();
		board.clear_tile_sprites();

		menu pause_menu(menu_type::PAUSE_MENU, 0);
		pause_menu.open_menu();
		
		while(pause_menu.is_open())
		{
			pause_menu.update();
			bn::core::update();
		}

		board.regen_tile_sprites();
		update_text();
	}

	tile_owner battle_scene::get_current_player()
	{
		return current_player;
	}

	void battle_scene::apply_damage_to_player(tile_owner player, int dmg)
	{
		switch (player)
		{
		case tile_owner::PLAYER:
			player_health = bn::clamp(player_health-dmg, 0, MAX_HEALTH);
			_health_meters.at(PLAYER_SIDE).set_current_val(player_health);
			break;
		case tile_owner::FOE:
			foe_health = bn::clamp(foe_health-dmg, 0, MAX_HEALTH);
			_health_meters.at(FOE_SIDE).set_current_val(foe_health);
			break;
		default:
			break;
		}
		audio::play_sound(sound_id::WEW);
		shake(6, 16);

		// TODO: check if zero hp 
		if(player_health <= 0)
		{
			battle_end(tile_owner::FOE);
		}
		else if(foe_health <= 0)
		{
			battle_end(tile_owner::PLAYER);
		}
	}


	bn::sprite_palette_ptr battle_scene::get_palette_of_player(tile_owner player)
	{
		switch(player)
		{
		case tile_owner::PLAYER:
			return _instance_->player_portrait._portrait_sprite.palette();
			break;
		case tile_owner::FOE:
			return _instance_->foe_portrait._portrait_sprite.palette();
			break;
		case tile_owner::EMPTY:
		default:
			const bn::sprite_palette_item *pal_itm = &bn::sprite_items::board_tile.palette_item();
			return pal_itm->create_palette();
			break;
		}
	}

}