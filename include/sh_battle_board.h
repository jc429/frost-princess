#ifndef SH_BATTLE_BOARD_H
#define SH_BATTLE_BOARD_H

#include "sh_battle_tile.h"
#include "sh_direction.h"
#include "sh_effects.h"
#include "sh_tile_pattern.h"
#include "sh_scene.h"

#include <bn_sprite_ptr.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_fixed_point.h>
#include <bn_point.h>

namespace sh
{
		
	#define BOARD_WIDTH 9
	#define BOARD_HEIGHT 9
	#define BOARD_POS_X 0
	#define BOARD_POS_Y -16
	#define TILE_WIDTH 13
	#define TILES_START 52

	#define TILE_SPRITE_LAYER 2
	/* tile sprites should range in priority from 400 to 500 */
	#define TILE_SPRITE_PRIO_BASE 500
	#define TILE_SPRITE_PRIO_INC 10

	const int NUM_PREVIEW_TILES = 4;

	
	enum class special_action_pattern
	{
		CROSS_5,
	};

	class battle_board
	{
	private:
		bn::regular_bg_ptr board_bg;
		bn::vector<battle_tile, 81> tiles;
		bn::vector<bn::sprite_ptr, 81> tile_sprites;
		bn::vector<bn::point, NUM_PREVIEW_TILES> preview_tile_offsets;
		bn::vector<bn::sprite_ptr, NUM_PREVIEW_TILES> preview_tiles;
		bool preview_tile_active[NUM_PREVIEW_TILES];

		battle_tile* select_tile(int tile_x, int tile_y);
		battle_tile* select_tile(battle_tile *tile);
		

	public:
		bn::point selection_pos;
		battle_tile* selected_tile;
		tile_pattern preview_pattern;
		direction preview_orientation;

		scene *current_scene;

		battle_board();
		~battle_board();
		void turn_update();
		void refresh();

		battle_tile* get_tile(int x, int y);
		battle_tile* get_tile(bn::point pos);
		battle_tile* move_selected_tile(int dir_x, int dir_y);
		battle_tile* move_selected_tile(bn::point dir);
		battle_tile* get_selected_tile();
		battle_tile* set_selected_tile(int pos_x, int pos_y);
		battle_tile* set_selected_tile(bn::point pos);

		void clear_tile_sprites();
		void regen_tile_sprites();

		void create_effect_at_tile(effects::effect_id effect, bn::point pos);
		void create_effect_at_tile(effects::effect_id effect, battle_tile* tile);

		static bn::fixed_point grid_to_world_pos(bn::point pos);
		bn::point get_rotated_pos(bn::point src);

		void set_preview_pattern(tile_pattern new_pattern);
		battle_tile* set_preview_orientation(direction orientation);
		battle_tile* rotate_preview_CW();
		battle_tile* rotate_preview_CCW();
		void update_preview_tiles();
		void hide_preview_tiles();
		void show_preview_tiles();

		bool mark_tiles(tile_owner owner);
		bool use_special_action(tile_owner owner, special_action_pattern pattern);

		void shift_row_or_col(int col_id, direction dir);

		bool get_preview_tile_active(int preview_tile_id);
		int get_preview_size();
		bn::point get_preview_tile_position(int preview_tile_id);
		bn::point get_preview_tile_offset(int preview_tile_id);
		tile_owner get_tile_owner(int pos_x, int pos_y);
		tile_owner get_tile_owner(bn::point pos);
		int count_tiles_with_owner(tile_owner owner);

	};
}


#endif //SH_BATTLE_BOARD_H