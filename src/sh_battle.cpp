#include "sh_battle.h"

#include "bn_regular_bg_ptr.h"


#include "bn_sprite_animate_actions.h"

// graphics
// fonts
#include "common_variable_8x16_sprite_font.h"
// backgrounds
#include "bn_regular_bg_items_battle_bg.h"
#include "bn_regular_bg_items_battle_board.h"

#include "bn_sprite_items_portrait_frame.h"
#include "bn_sprite_items_card_blank.h"
#include "bn_sprite_items_cursor_card.h"
#include "bn_sprite_items_cursor_tile.h"
#include "bn_sprite_items_board_tile.h"



void battle_scene(bn::sprite_text_generator& text_generator);



void battle_scene(bn::sprite_text_generator& text_generator)
{
	// constexpr bn::string_view info_text_lines[] = {
	// 	"A: hide/show sprite",
	// 	"",
	// 	"START: go to next scene",
	// };

	bool game_over = false;


	// place hand cards
	int selected_card = 2;
	const int cards_x[] = {-48, -24, 0, 24, 48};
	const int cards_y = 64;
	bn::vector<bn::sprite_ptr, 5> card_sprites;
	for(int i = 0; i < 5; i++)
	{
		card_sprites.push_back(bn::sprite_items::card_blank.create_sprite(cards_x[i], cards_y));
	}
	// build cursors
	bn::sprite_ptr cursor_card_sprite = bn::sprite_items::cursor_card.create_sprite(cards_x[selected_card], cards_y);
	bn::sprite_animate_action<2> cursor_card_idle_action = bn::create_sprite_animate_action_forever(
			cursor_card_sprite, 16, bn::sprite_items::cursor_card.tiles_item(), 0, 1);
	cursor_card_sprite.set_bg_priority(1);
	cursor_card_sprite.set_visible(true);


	bn::sprite_ptr cursor_tile_sprite = bn::sprite_items::cursor_tile.create_sprite(0, -16);
	bn::sprite_animate_action<2> cursor_tile_idle_action = bn::create_sprite_animate_action_forever(
			cursor_tile_sprite, 16, bn::sprite_items::cursor_tile.tiles_item(), 0, 1);
	cursor_tile_sprite.set_bg_priority(1);
	cursor_tile_sprite.set_visible(false);

	// build bg
	bn::regular_bg_ptr battle_bg = bn::regular_bg_items::battle_bg.create_bg(0, 0);
	battle_bg.set_priority(3);
	bn::regular_bg_ptr battle_board = bn::regular_bg_items::battle_board.create_bg(0, -16);
	battle_board.set_priority(2);

	// build text generator
	bn::vector<bn::sprite_ptr, 32> text_sprites;
	text_generator.set_bg_priority(0);
	text_generator.set_z_order(-500);
	text_generator.generate(0, -72, "Big variable width font", text_sprites);




	bn::sprite_ptr portrait_frame_r = bn::sprite_items::portrait_frame.create_sprite(96, -56);
	bn::sprite_ptr portrait_frame_l = bn::sprite_items::portrait_frame.create_sprite(-96, 56);



	while(!game_over)
	{

		if(cursor_card_sprite.visible())
		{
			if(bn::keypad::a_pressed())
			{
				cursor_card_sprite.set_visible(false);
				cursor_tile_sprite.set_visible(true);
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
		}
		else if(cursor_tile_sprite.visible())
		{
			if(bn::keypad::a_pressed())
			{
				cursor_tile_sprite.set_visible(false);
				cursor_card_sprite.set_visible(true);
			}

			if(bn::keypad::left_pressed())
			{
				cursor_tile_sprite.set_x(cursor_tile_sprite.x() - 13);
			}
			else if(bn::keypad::right_pressed())
			{
				cursor_tile_sprite.set_x(cursor_tile_sprite.x() + 13);
			}
			if(bn::keypad::up_pressed())
			{
				cursor_tile_sprite.set_y(cursor_tile_sprite.y() - 13);
			}
			else if(bn::keypad::down_pressed())
			{
				cursor_tile_sprite.set_y(cursor_tile_sprite.y() + 13);
			}
		}





		// slowly pan bg
		battle_bg.set_x(battle_bg.x() - 0.25);
		battle_bg.set_y(battle_bg.y() - 0.25);

		// update animations
		cursor_card_idle_action.update();
		cursor_tile_idle_action.update();


		bn::core::update();
	}
}
