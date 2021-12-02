#ifndef SH_BATTLE_BOARD_H
#define SH_BATTLE_BOARD_H

#include "sh_battle_tile.h"

#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"

namespace sh
{


	class battle_board
	{
	private:

	public:
		bn::regular_bg_ptr board_bg;
		bn::vector<battle_tile, 81> tiles;
		battle_tile* selected_tile;


		battle_board();
		battle_tile* get_tile(int x, int y);
		battle_tile* update_selected_tile(int mov_x, int mov_y);
		battle_tile* get_selected_tile();
	};
}


#endif //SH_BATTLE_BOARD_H