#include "sh_battle_tile.h"
#include "sh_battle_board.h"
#include "sh_direction.h"
#include "sh_effects.h"
#include "sh_action_manager.h"

#include <bn_blending.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sound_items.h>
#include <bn_sprite_builder.h>
#include <bn_vector.h>

#include "bn_regular_bg_items_battle_board.h"
#include "bn_sprite_items_board_tile.h"
#include "bn_sprite_items_preview_tile.h"




namespace sh{

	battle_board::battle_board() :
		board_bg(bn::regular_bg_items::battle_board.create_bg((bn::fixed)BOARD_POS_X, (bn::fixed)BOARD_POS_Y))
	{
		current_scene = NULL;
		board_bg.set_priority(2);

		preview_orientation = direction::NORTH;
		preview_pattern = tile_pattern::SINGLE;
		selection_pos = bn::point(0,0);
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			preview_tile_offsets.push_back(bn::point(0,0));
			bn::sprite_ptr tile_sprite = bn::sprite_items::preview_tile.create_sprite((bn::fixed)BOARD_POS_X, (bn::fixed)BOARD_POS_Y);
			tile_sprite.set_bg_priority(2);
			tile_sprite.set_z_order(-500);
			tile_sprite.set_blending_enabled(true);
			//bn::fixed transparency_alpha = bn::blending::transparency_alpha();
			preview_tiles.push_back(tile_sprite);
			preview_tile_active[i] = false;
		}
		hide_preview_tiles();

		// populate board with tiles
		// set up sprite builder
		bn::sprite_builder builder(bn::sprite_items::board_tile);
		builder.set_bg_priority(TILE_SPRITE_LAYER);
		builder.set_z_order(TILE_SPRITE_PRIO_BASE);
		
		bool tile_dark = true;	// signifies whether to darken a tile or not (alternates after every placement)
		for(int row = 0; row < BOARD_HEIGHT; row++)
		{
			// builder.set_z_order(TILE_SPRITE_PRIO_BASE + (row * TILE_SPRITE_PRIO_INC));
			for(int col = 0; col < BOARD_WIDTH; col++)
			{
				tile_sprites.push_back(builder.build());
				tiles.push_back(battle_tile((col*BOARD_HEIGHT)+row));
				battle_tile *tile = &tiles.back();
				tile->set_sprite_ptr(&tile_sprites.back());
				tile->coordinates = bn::point(col,row);
				int x = (BOARD_POS_X - TILES_START) + (TILE_WIDTH * col);
				int y = (BOARD_POS_Y - TILES_START) + (TILE_WIDTH * row);
				tile->set_position(x,y);
				tile->set_dark(tile_dark);
				tile_dark = !tile_dark;
				if(col > 0)
				{
					battle_tile *neighbor = get_tile(col - 1, row);
					tile->set_neighbor(direction::WEST, neighbor);
					neighbor->set_neighbor(direction::EAST, tile);
				}
				if(row > 0)
				{
					battle_tile *neighbor = get_tile(col, row-1);
					tile->set_neighbor(direction::NORTH, neighbor);
					neighbor->set_neighbor(direction::SOUTH, tile);
				}
				tile->update_sprite();
			}
		}

		//put selection cursor in center
		select_tile(4,4);

	}

	battle_board::~battle_board()
	{
		tiles.clear();
		tile_sprites.clear();
		preview_tile_offsets.clear();
		preview_tiles.clear();
	}


	void battle_board::turn_update()
	{
		for(auto it = tiles.begin(), end = tiles.end(); it != end; ++it)
		{
			it->turn_update();
		}
	}

	void battle_board::refresh()
	{
		for(auto it = tiles.begin(), end = tiles.end(); it != end; ++it)
		{
			it->update_sprite();
		}
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
		int idx = x + (y * BOARD_HEIGHT);
		return &tiles.at(idx);
	}

	battle_tile* battle_board::get_tile(bn::point pos)
	{
		return get_tile(pos.x(), pos.y());
	}


	battle_tile* battle_board::move_selected_tile(int dir_x, int dir_y)
	{
		if(dir_x < 0 && selected_tile->get_neighbor(direction::WEST) != NULL)
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
				selected_tile = selected_tile->get_neighbor(direction::WEST);
			}
		}
		if(dir_x > 0 && selected_tile->get_neighbor(direction::EAST) != NULL)
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
				selected_tile = selected_tile->get_neighbor(direction::EAST);
			}
		}
		if(dir_y < 0 && selected_tile->get_neighbor(direction::NORTH) != NULL)
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
				selected_tile = selected_tile->get_neighbor(direction::NORTH);
			}
		}
		if(dir_y > 0 && selected_tile->get_neighbor(direction::SOUTH) != NULL)
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
				selected_tile = selected_tile->get_neighbor(direction::SOUTH);
			}
		}

		// update preview graphics
		select_tile(selected_tile);
		update_preview_tiles();
		return selected_tile;
	}

	battle_tile* battle_board::move_selected_tile(bn::point dir)
	{
		return move_selected_tile(dir.x(), dir.y());
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
		else{ // ???
			selection_pos = temp;
			selected_tile = get_tile(pos_x, pos_y);
			update_preview_tiles();
			for(int i = 0; i < NUM_PREVIEW_TILES; i++)
			{
				if(temp.x() + preview_tile_offsets.at(i).x() < 0)
				{
					move_selected_tile(1, 0);
				}
				if(temp.y() + preview_tile_offsets.at(i).y() < 0)
				{
					move_selected_tile(0, 1);
				}
				if(temp.x() + preview_tile_offsets.at(i).x() > 8)
				{
					move_selected_tile(-1, 0);
				}
				if(temp.y() + preview_tile_offsets.at(i).y() > 8)
				{
					move_selected_tile(0, -1);
				}
			}
			update_preview_tiles();
		}
		
		return selected_tile;
	}
	
	battle_tile* battle_board::set_selected_tile(bn::point pos)
	{
		return set_selected_tile(pos.x(), pos.y());
	}

	void battle_board::clear_tile_sprites()
	{
		for(auto it = tiles.begin(), end = tiles.end(); it != end; ++it)
		{
			it->clear_sprite_ptr();
		}
		tile_sprites.clear();
	}

	void battle_board::regen_tile_sprites()
	{
		clear_tile_sprites();
		// set up sprite builder
		bn::sprite_builder builder(bn::sprite_items::board_tile);
		builder.set_bg_priority(TILE_SPRITE_LAYER);
		builder.set_z_order(TILE_SPRITE_PRIO_BASE);
		for(auto it = tiles.begin(), end = tiles.end(); it != end; ++it)
		{
			tile_sprites.push_back(builder.build());
			it->set_sprite_ptr(&tile_sprites.back());
			it->update_sprite();
		}
	}

	
	void battle_board::create_effect_at_tile(effects::effect_id effect, bn::point pos)
	{
		effects::create_effect_at_position(effect, grid_to_world_pos(pos));
	}

	void battle_board::create_effect_at_tile(effects::effect_id effect, battle_tile* tile)
	{
		effects::create_effect_at_position(effect, grid_to_world_pos(tile->coordinates));
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
		case direction::WEST:
			return bn::point(src.y(), -1*src.x());
			break;
		case direction::SOUTH:
			return bn::point(-1*src.x(), -1*src.y());
			break;
		case direction::EAST:
			return bn::point(-1*src.y(), src.x());
			break;
		case direction::NORTH:
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

	battle_tile* battle_board::set_preview_orientation(direction orientation)
	{
		preview_orientation = orientation;
		update_preview_tiles();
		return selected_tile;
	}

	battle_tile* battle_board::rotate_preview_CW()
	{
		preview_orientation = directions::next_CW(preview_orientation);
		update_preview_tiles();
		return selected_tile;
	}

	battle_tile* battle_board::rotate_preview_CCW()
	{
		preview_orientation = directions::next_CCW(preview_orientation);
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
		case tile_pattern::SPECIAL_SINGLE:
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
				battle_tile* tile = get_tile(pos);
				if(tile->get_owner() != owner && tile->get_condition() != tile_condition::NORMAL)
				{
					return false;
				}
				// check that tile pattern is neighboring a tile we own
				for(int j = 0; j < 4; j++)
				{
					if(tile->get_neighbor(j)->get_owner() == owner)
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

	bool battle_board::use_special_action(tile_owner owner, special_action_pattern pattern)
	{
		if(owner != tile_owner::EMPTY && selected_tile->get_owner() == owner)
		{
			battle_tile *tile = selected_tile;
			int eff_duration = 4;
			int num_steps = 4;
			effects::effect_id eff_id = effects::effect_id::SHINE;
			tile_condition condition = tile_condition::FROZEN;
			int cycle_wait = 24;
			battle_tile *neighbors[4];
			for(int i = 0; i < 4; i++)
			{
				neighbors[i] = tile->get_neighbor(i);
			}
			create_effect_at_tile(eff_id, tile);
			bn::sound_items::wewewew.play();
			current_scene->wait_for_update_cycles(cycle_wait);
			tile->set_condition(condition, eff_duration);
			tile->set_owner(owner);
			for(int step = 0; step < num_steps; step++)
			{
				bool apply_effect = false;
				for(int i = 0; i < 4; i++)
				{
					if(neighbors[i] == NULL)
					{
						continue;
					}
					create_effect_at_tile(eff_id,neighbors[i]);
					apply_effect = true;
				}
				if(apply_effect)
				{
					bn::sound_items::wewewew.play();
					current_scene->wait_for_update_cycles(cycle_wait);
					for(int i = 0; i < 4; i++)
					{
						if(neighbors[i] != NULL)
						{
							neighbors[i]->set_condition(condition, eff_duration);
							neighbors[i]->set_owner(owner);
							neighbors[i] = neighbors[i]->get_neighbor(i);
						}
					}
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	// shifts a row of tiles on the board in [dir] direction by [amount] tiles
	


	// shifts a column of tiles on the board in [dir] direction by [amount] tiles
	void battle_board::shift_row_or_col(int row_col_id, direction dir)
	{
		row_col_id = bn::clamp(row_col_id,0,BOARD_WIDTH);
		bn::vector<battle_tile*,BOARD_HEIGHT> tile_list;

		switch(dir)
		{
			case direction::NORTH:
				for(int i = 0; i < BOARD_HEIGHT; i++)
				{
					tile_list.push_back(get_tile(row_col_id,i));
				}
				break;
			case direction::SOUTH:
				for(int i = 0; i < BOARD_HEIGHT; i++)
				{
					tile_list.push_back(get_tile(row_col_id,BOARD_HEIGHT-i));
				}
				break;
			case direction::WEST:
				for(int i = 0; i < BOARD_WIDTH; i++)
				{
					tile_list.push_back(get_tile(i, row_col_id));
				}
				break;
			case direction::EAST:
				for(int i = 0; i < BOARD_WIDTH; i++)
				{
					tile_list.push_back(get_tile(BOARD_WIDTH-i, row_col_id));
				}
				break;
			default:
				return;
		}
	
		{
			bn::sprite_ptr *temp_spr = tile_list.front()->get_sprite();
			bn::fixed_point temp_off = tile_list.front()->get_sprite_offset();
			tile_owner temp_owner = tile_list.front()->get_owner();
			bn::fixed_point temp_pos = tile_list.front()->get_position();
			for(auto it = tile_list.begin(), end = tile_list.end(); it != end; )
			{
				battle_tile *tile = *it;
				++it;
				battle_tile *next = *it;
				// tile->set_sprite_ptr(next->get_sprite());
				// tile->set_sprite_offset(next->get_sprite_offset());
				// tile->set_owner(next->get_owner());
				tile->copy_properties(next);
				if(it == end)
				{
					// hide + instantly move the sprite that would go off the board
					tile_list.back()->set_sprite_ptr(temp_spr);
					tile_list.back()->set_sprite_offset(temp_off);
					tile_list.back()->set_sprite_position(temp_pos);
					tile_list.back()->set_owner(temp_owner);
					tile_list.back()->get_sprite()->set_visible(false);
				}
				action_manager::register_move_action(*tile->get_sprite(), 12, tile->get_sprite_position());
			}
		}
		// animate tiles moving
		if(current_scene != NULL)
		{
			current_scene->wait_for_update_cycles(12);
		}
		// reset the sprite we turned invisible before
		tile_list.back()->get_sprite()->set_visible(true);
	//	refresh();
	}

	bool battle_board::get_preview_tile_active(int preview_tile_id)
	{
		preview_tile_id = bn::clamp(preview_tile_id, 0, NUM_PREVIEW_TILES);
		return preview_tile_active[preview_tile_id];
	}

	int battle_board::get_preview_size()
	{
		for(int i = 0; i < NUM_PREVIEW_TILES; i++)
		{
			if(!preview_tile_active[i])
				return i;
		}
		return NUM_PREVIEW_TILES;
	}
	
	bn::point battle_board::get_preview_tile_position(int preview_tile_id)
	{
		preview_tile_id = bn::clamp(preview_tile_id, 0, NUM_PREVIEW_TILES);
		return selection_pos + preview_tile_offsets.at(preview_tile_id);
	}

	bn::point battle_board::get_preview_tile_offset(int preview_tile_id)
	{
		preview_tile_id = bn::clamp(preview_tile_id, 0, NUM_PREVIEW_TILES);
		return preview_tile_offsets.at(preview_tile_id);
	}

	tile_owner battle_board::get_tile_owner(int pos_x, int pos_y)
	{
		return get_tile(pos_x, pos_y)->get_owner();
	}
	
	tile_owner battle_board::get_tile_owner(bn::point pos)
	{
		return get_tile(pos)->get_owner();
	}

	int battle_board::count_tiles_with_owner(tile_owner owner)
	{
		int count = 0;
		for(auto it = tiles.begin(), end = tiles.end(); it != end; ++it)
		{
			battle_tile& tile = *it;
			if(tile.get_owner() == owner)
				count++;
		}
		return count;
	}
}