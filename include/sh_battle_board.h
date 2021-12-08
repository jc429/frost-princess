#ifndef SH_BATTLE_BOARD_H
#define SH_BATTLE_BOARD_H

#include "sh_battle_tile.h"
#include "sh_tile_pattern.h"

#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"
#include "bn_fixed_point.h"
#include "bn_point.h"

namespace sh
{
		
	#define BOARD_WIDTH 9
	#define BOARD_HEIGHT 9
	#define BOARD_POS_X 0
	#define BOARD_POS_Y -16
	#define TILE_WIDTH 13
	#define TILES_START 52
	const int NUM_PREVIEW_TILES = 5;

	class battle_board
	{
	private:
		bn::regular_bg_ptr board_bg;
		bn::vector<battle_tile, 81> tiles;
		bn::vector<bn::point, NUM_PREVIEW_TILES> preview_tile_offsets;
		bn::vector<bn::sprite_ptr, NUM_PREVIEW_TILES> preview_tiles;
		bool preview_tile_active[NUM_PREVIEW_TILES];

		battle_tile* select_tile(int tile_x, int tile_y);
		battle_tile* select_tile(battle_tile *tile);

	public:
		bn::point selection_pos;
		battle_tile* selected_tile;
		tile_pattern preview_pattern;
		int preview_orientation;


		battle_board();
		battle_tile* get_tile(int x, int y);
		battle_tile* move_selected_tile(int mov_x, int mov_y);
		battle_tile* get_selected_tile();
		battle_tile* set_selected_tile(int pos_x, int pos_y);

		static bn::fixed_point grid_to_world_pos(bn::point pos);
		bn::point get_rotated_pos(bn::point src);

		void set_preview_pattern(tile_pattern new_pattern);
		battle_tile* set_preview_orientation(int rotation);
		battle_tile* rotate_preview_CW();
		battle_tile* rotate_preview_CCW();
		void update_preview_tiles();
		void hide_preview_tiles();
		void show_preview_tiles();

		bool mark_tiles(tile_owner owner);
	};
}


#endif //SH_BATTLE_BOARD_H