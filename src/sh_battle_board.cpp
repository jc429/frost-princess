#include "sh_battle_tile.h"
#include "sh_battle_board.h"

#include "bn_blending.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"

#include "bn_regular_bg_items_battle_board.h"
#include "bn_sprite_items_preview_tile.h"




namespace sh{

	battle_board::battle_board() :
		board_bg(bn::regular_bg_items::battle_board.create_bg((bn::fixed)BOARD_POS_X, (bn::fixed)BOARD_POS_Y))
	{
		board_bg.set_priority(2);
		

		preview_orientation = DIRECTION_N;
		preview_pattern = tile_pattern::SINGLE;
		selection_pos = bn::point(0,0);
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			preview_tile_offsets.push_back(bn::point(0,0));
			bn::sprite_ptr tile_sprite = bn::sprite_items::preview_tile.create_sprite((bn::fixed)BOARD_POS_X, (bn::fixed)BOARD_POS_Y);
			tile_sprite.set_bg_priority(1);
			tile_sprite.set_z_order(500);
			tile_sprite.set_blending_enabled(true);
			//bn::fixed transparency_alpha = bn::blending::transparency_alpha();
			preview_tiles.push_back(tile_sprite);
			preview_tile_active[i] = false;
		}
		hide_preview_tiles();

		// populate board with tiles
		// signifies whether to darken a tile or not (alternates after every placement)
		bool tile_dark = true;
		for(int i = 0; i < BOARD_WIDTH; i++)
		{
			for(int j = 0; j < BOARD_HEIGHT; j++)
			{
				tiles.push_back(battle_tile((i*BOARD_HEIGHT)+j));
				battle_tile *tile = &tiles.back();
				tile->coordinates = bn::point(i,j);
				int x = (BOARD_POS_X - TILES_START) + (TILE_WIDTH * i);
				int y = (BOARD_POS_Y - TILES_START) + (TILE_WIDTH * j);
				tile->set_position(x,y);
				tile->set_dark(tile_dark);
				tile_dark = !tile_dark;
				if(i > 0)
				{
					battle_tile* neighbor = get_tile(i - 1, j);
					tile->neighbors[DIRECTION_W] = neighbor;
					neighbor->neighbors[DIRECTION_E] = tile;
				}
				if(j > 0)
				{
					battle_tile* neighbor = get_tile(i, j-1);
					tile->neighbors[DIRECTION_N] = neighbor;
					neighbor->neighbors[DIRECTION_S] = tile;
				}
			}
		}

		//put selection cursor in center
		select_tile(4,4);

	}

	battle_tile* battle_board::select_tile(int tile_x, int tile_y)
	{
		selection_pos = bn::point(tile_x, tile_y);
		selected_tile = get_tile(tile_x, tile_y);
		return selected_tile;
	}
	battle_tile* battle_board::select_tile(battle_tile *tile)
	{
		selection_pos = tile->coordinates;
		selected_tile = tile;
		return selected_tile;
	}

	battle_tile* battle_board::get_tile(int x, int y)
	{
		x = x % BOARD_WIDTH;
		y = y % BOARD_HEIGHT;
		int idx = (x * BOARD_HEIGHT) + y;

		return &tiles.at(idx);

	}

	battle_tile* battle_board::move_selected_tile(int mov_x, int mov_y)
	{
		if(mov_x < 0 && selected_tile->neighbors[DIRECTION_W] != NULL)
		{
			bool safe = true;
			for(int i = 0; i < NUM_PREVIEW_TILES; i++)
			{
				if(selection_pos.x() + preview_tile_offsets.at(i).x() <= 0)
				{
					safe = false;
					break;
				}
			}
			if(safe)
			{
				selected_tile = selected_tile->neighbors[DIRECTION_W];
			}
		}
		if(mov_x > 0 && selected_tile->neighbors[DIRECTION_E] != NULL)
		{
			bool safe = true;
			for(int i = 0; i < NUM_PREVIEW_TILES; i++)
			{
				if(selection_pos.x() + preview_tile_offsets.at(i).x() >= 8)
				{
					safe = false;
					break;
				}
			}
			if(safe)
			{
				selected_tile = selected_tile->neighbors[DIRECTION_E];
			}
		}
		if(mov_y < 0 && selected_tile->neighbors[DIRECTION_N] != NULL)
		{
			bool safe = true;
			for(int i = 0; i < NUM_PREVIEW_TILES; i++)
			{
				if(selection_pos.y() + preview_tile_offsets.at(i).y() <= 0)
				{
					safe = false;
					break;
				}
			}
			if(safe)
			{
				selected_tile = selected_tile->neighbors[DIRECTION_N];
			}
		}
		if(mov_y > 0 && selected_tile->neighbors[DIRECTION_S] != NULL)
		{
			bool safe = true;
			for(int i = 0; i < NUM_PREVIEW_TILES; i++)
			{
				if(selection_pos.y() + preview_tile_offsets.at(i).y() >= 8)
				{
					safe = false;
					break;
				}
			}
			if(safe)
			{
				selected_tile = selected_tile->neighbors[DIRECTION_S];
			}
		}

		// update preview graphics
		select_tile(selected_tile);
		update_preview_tiles();
		return selected_tile;
	}

	battle_tile* battle_board::get_selected_tile()
	{
		return selected_tile;
	}

	battle_tile* battle_board::set_selected_tile(int pos_x, int pos_y)
	{
		bn::point temp(pos_x, pos_y);
		bool safe = true;
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			if((temp.x() + preview_tile_offsets.at(i).x() < 0) || (temp.x() + preview_tile_offsets.at(i).x() >= BOARD_WIDTH))
			{
				safe = false;
				break;
			}
			if((temp.y() + preview_tile_offsets.at(i).y() < 0) || (temp.y() + preview_tile_offsets.at(i).y() >= BOARD_HEIGHT))
			{
				safe = false;
				break;
			}
		}
		if(safe)
		{
			selection_pos = temp;
			selected_tile = get_tile(pos_x, pos_y);
			update_preview_tiles();
		}
		
		return selected_tile;
	}
	
	bn::fixed_point battle_board::grid_to_world_pos(bn::point pos)
	{
		pos.set_x((BOARD_POS_X - TILES_START) + (TILE_WIDTH * pos.x()));
		pos.set_y((BOARD_POS_Y - TILES_START) + (TILE_WIDTH * pos.y()));
		return pos;
	}

	bn::point battle_board::get_rotated_pos(bn::point src)
	{
		switch(preview_orientation)
		{
		case DIRECTION_W:
			return bn::point(src.y(), -1*src.x());
			break;
		case DIRECTION_S:
			return bn::point(-1*src.x(), -1*src.y());
			break;
		case DIRECTION_E:
			return bn::point(-1*src.y(), src.x());
			break;
		case DIRECTION_N:
		default:
			return src;
			break;
		}
	}

	void battle_board::set_preview_pattern(tile_pattern new_pattern)
	{
		preview_pattern = new_pattern;
		update_preview_tiles();
		show_preview_tiles();
	}

	battle_tile* battle_board::set_preview_orientation(int rotation)
	{
		preview_orientation = rotation;
		update_preview_tiles();
		return selected_tile;
	}

	battle_tile* battle_board::rotate_preview_CW()
	{
		preview_orientation = (preview_orientation + 1) % 4;
		update_preview_tiles();
		return selected_tile;
	}

	battle_tile* battle_board::rotate_preview_CCW()
	{
		preview_orientation = (preview_orientation + 3) % 4;
		update_preview_tiles();
		return selected_tile;
	}



	void battle_board::update_preview_tiles()
	{
		bn::point cursor_pos = selected_tile->coordinates;
		bn::point adj = bn::point(0,0);
		bn::point offset;
		// reset all tiles
		selection_pos = cursor_pos;
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			preview_tile_offsets.at(i) = bn::point(0,0);
			preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos));
			preview_tile_active[i] = false;
		}
		switch (preview_pattern)
		{
		case tile_pattern::EMPTY:
			break;
		case tile_pattern::SINGLE:
			preview_tile_offsets.at(0) = bn::point(0,0);
			preview_tiles.at(0).set_position(grid_to_world_pos(cursor_pos));
			preview_tile_active[0] = true;
			break;
		case tile_pattern::LINE_2:
			adj = bn::point(0,1);
			for(int i = 0; i < 2; i++)
			{
				offset = bn::point(0,0);
				offset.set_x(i * adj.x());
				offset.set_y(i * adj.y());
				offset = get_rotated_pos(offset);
				preview_tile_offsets.at(i) = offset;
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;
		case tile_pattern::LINE_3:
			adj = bn::point(0,1);
			for(int i = 0; i < 3; i++)
			{
				offset = get_rotated_pos(adj);
				preview_tile_offsets.at(i) = offset;
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
				adj.set_y(adj.y()-1);
			}
			break;
		case tile_pattern::LINE_4:
			adj = bn::point(0,1);
			for(int i = 0; i < 4; i++)
			{
				offset = get_rotated_pos(adj);
				preview_tile_offsets.at(i) = offset;
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
				adj.set_y(adj.y()-1);
			}
			break;
		case tile_pattern::L_SMALL:
			preview_tile_offsets.at(0) = bn::point(0,0);
			adj = bn::point(0,-1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(1) = offset;
			adj = bn::point(1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(2) = offset;
			for(int i = 0; i < 3; i++)
			{
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;
		case tile_pattern::L_NORMAL:
			preview_tile_offsets.at(0) = bn::point(0,0);
			adj = bn::point(1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(1) = offset;
			adj = bn::point(0,-1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(2) = offset;
			adj = bn::point(0,-2);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(3) = offset;
			for(int i = 0; i < 4; i++)
			{
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;
		case tile_pattern::L_REVERSE:
			preview_tile_offsets.at(0) = bn::point(0,0);
			adj = bn::point(-1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(1) = offset;
			adj = bn::point(0,-1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(2) = offset;
			adj = bn::point(0,-2);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(3) = offset;
			for(int i = 0; i < 4; i++)
			{
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;
		case tile_pattern::T_4:
			preview_tile_offsets.at(0) = bn::point(0,0);
			adj = bn::point(-1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(1) = offset;
			adj = bn::point(0,1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(2) = offset;
			adj = bn::point(1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(3) = offset;
			for(int i = 0; i < 4; i++)
			{
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;
		case tile_pattern::S_4:
			preview_tile_offsets.at(0) = bn::point(0,0);
			adj = bn::point(1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(1) = offset;
			adj = bn::point(0,1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(2) = offset;
			adj = bn::point(-1,1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(3) = offset;
			for(int i = 0; i < 4; i++)
			{
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;
		case tile_pattern::Z_4:
			preview_tile_offsets.at(0) = bn::point(0,0);
			adj = bn::point(-1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(1) = offset;
			adj = bn::point(0,1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(2) = offset;
			adj = bn::point(1,1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(3) = offset;
			for(int i = 0; i < 4; i++)
			{
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;
		case tile_pattern::SQUARE:
			preview_tile_offsets.at(0) = bn::point(0,0);
			adj = bn::point(1,0);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(1) = offset;
			adj = bn::point(0,1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(2) = offset;
			adj = bn::point(1,1);
			offset = get_rotated_pos(adj);
			preview_tile_offsets.at(3) = offset;
			for(int i = 0; i < 4; i++)
			{
				preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos + preview_tile_offsets.at(i)));
				preview_tile_active[i] = true;
			}
			break;



		default:
			break;
		}

		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			if(cursor_pos.x() + preview_tile_offsets.at(i).x() < 0)
			{
				move_selected_tile(1, 0);
			}
			if(cursor_pos.y() + preview_tile_offsets.at(i).y() < 0)
			{
				move_selected_tile(0, 1);
			}
			if(cursor_pos.x() + preview_tile_offsets.at(i).x() > 8)
			{
				move_selected_tile(-1, 0);
			}
			if(cursor_pos.y() + preview_tile_offsets.at(i).y() > 8)
			{
				move_selected_tile(0, -1);
			}
		}


	}
	
	void battle_board::hide_preview_tiles()
	{
		bn::point cursor_pos = selected_tile->coordinates;
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			preview_tiles.at(i).set_position(grid_to_world_pos(cursor_pos));
			preview_tiles.at(i).set_visible(false);
			preview_tile_active[i] = false;
		}
	}

	void battle_board::show_preview_tiles()
	{
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			preview_tiles.at(i).set_visible(preview_tile_active[i]);
		}
	}

	
	bool battle_board::mark_tiles(tile_owner owner)
	{
		if(owner != tile_owner::EMPTY)
		{
			bool neighbors_self = false;
			for(int i = 0; i < NUM_PREVIEW_TILES; i++)
			{
				if(!preview_tile_active[i])
				{
					continue;
				}
				bn::point pos = selection_pos + preview_tile_offsets.at(i);
				battle_tile* tile = get_tile(pos.x(), pos.y());
				for(int j = 0; j < 4; j++)
				{
					if(tile->neighbors[j]->get_owner() == owner)
					{
						neighbors_self = true;
						break;
					}
				}
			}
			if(!neighbors_self)
			{
				return false;
			}
		}
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			if(!preview_tile_active[i])
			{
				continue;
			}
			bn::point pos = selection_pos + preview_tile_offsets.at(i);
			get_tile(pos.x(), pos.y())->set_owner(owner);
		}
		return true;
	}


	
	void battle_board::set_blending_enabled(bool blending_enabled)
	{
		board_bg.set_blending_enabled(blending_enabled);
		for(int i = 0; i < tiles.size(); i++)
		{
			tiles.at(i).sprite.set_blending_enabled(blending_enabled);
		}
	}
}