#include "sh_battle_tile.h"
#include "sh_battle_board.h"

#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"
#include "bn_fixed.h"

#include "bn_regular_bg_items_battle_board.h"

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9
#define BOARD_POS_X 0
#define BOARD_POS_Y -16
#define TILE_WIDTH 13
#define TILES_START 52

namespace sh{

	battle_board::battle_board() :
		board_bg(bn::regular_bg_items::battle_board.create_bg((bn::fixed)BOARD_POS_X, (bn::fixed)BOARD_POS_Y))
	{
		board_bg.set_priority(2);

		// signifies whether to darken a tile or not (alternates after every placement)
		bool tile_dark = true;

		// populate board with tiles
		for(int i = 0; i < BOARD_WIDTH; i++)
		{
			for(int j = 0; j < BOARD_HEIGHT; j++)
			{
				battle_tile tile = battle_tile();
				int x = (BOARD_POS_X - TILES_START) + (TILE_WIDTH * i);
				int y = (BOARD_POS_Y - TILES_START) + (TILE_WIDTH * j);
				tile.set_position(x,y);
				tile.set_dark(tile_dark);
				tile_dark = !tile_dark;
				
				tiles.push_back(tile);

				if(i > 0)
				{
					battle_tile* neighbor = get_tile(i - 1, j);
					tiles.back().neighbors[DIRECTION_W] = neighbor;
					neighbor->neighbors[DIRECTION_E] = &tiles.back();
				}

				if(j > 0)
				{
					battle_tile* neighbor = get_tile(i, j-1);
					tiles.back().neighbors[DIRECTION_N] = neighbor;
					neighbor->neighbors[DIRECTION_S] = &tiles.back();
				}
			}
		}

		// set base tiles
		get_tile(1, BOARD_HEIGHT-2)->set_owner(tile_owner::PLAYER);
		get_tile(BOARD_WIDTH-2, 1)->set_owner(tile_owner::FOE);
		selected_tile = get_tile(4,4);

	}

	battle_tile* battle_board::get_tile(int x, int y)
	{
		x = x % BOARD_WIDTH;
		y = y % BOARD_HEIGHT;
		int idx = (x * BOARD_HEIGHT) + y;

		return &tiles.at(idx);

	}

	battle_tile* battle_board::update_selected_tile(int mov_x, int mov_y)
	{
		if(mov_x < 0 && selected_tile->neighbors[DIRECTION_W] != NULL)
		{
			selected_tile = selected_tile->neighbors[DIRECTION_W];
		}
		if(mov_x > 0 && selected_tile->neighbors[DIRECTION_E] != NULL)
		{
			selected_tile = selected_tile->neighbors[DIRECTION_E];
		}
		if(mov_y < 0 && selected_tile->neighbors[DIRECTION_N] != NULL)
		{
			selected_tile = selected_tile->neighbors[DIRECTION_N];
		}
		if(mov_y > 0 && selected_tile->neighbors[DIRECTION_S] != NULL)
		{
			selected_tile = selected_tile->neighbors[DIRECTION_S];
		}

		return selected_tile;
	}

	battle_tile* battle_board::get_selected_tile()
	{
		return selected_tile;
	}
}