#include "sh_battle_scene.h"
#include "sh_random.h"

#include <bn_core.h>
#include <bn_string.h>
#include "variable_8x16_sprite_font.h"

namespace sh
{
	extern bn::sprite_text_generator text_generator;
	extern bn::vector<bn::sprite_ptr, 32> text_sprites;

	void battle_scene::foe_turn()
	{
		current_player = tile_owner::FOE;

		//tile_pattern pattern = tile_pattern::SINGLE;
		tile_pattern pattern = tile_patterns::random_tile_pattern();
		board.set_preview_pattern(pattern);
		board.set_preview_orientation(directions::random_direction());

		bn::vector<battle_tile*, 80> foe_neighboring_tiles;
		foe_neighboring_tiles.clear();
		// identify tiles neighboring current spread
		{
			bool tile_checked[81];
			bn::vector<battle_tile*, 80> foe_active_tiles;
			foe_active_tiles.clear();
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
								// neighbor->set_owner(tile_owner::DEBUG);
							}
						}
					}
				}
			}
		}
		if(foe_neighboring_tiles.empty())
		{
			// if neighboring nothing, return
			return;
		}
		
		text_generator.generate(70, -10, bn::to_string<2>(foe_neighboring_tiles.size()), text_sprites);

		// TODO: make better than random selection maybe
		int r = random.get_int(0, foe_neighboring_tiles.size());
		battle_tile *selected_tile = foe_neighboring_tiles.at(r);
		text_generator.generate(70, 0, bn::to_string<4>(selected_tile->coordinates.x()) + "," + bn::to_string<4>(selected_tile->coordinates.y()), text_sprites);
		if(selected_tile != NULL)
		{
			board.show_preview_tiles();
			board.set_selected_tile(selected_tile->coordinates);
			foe_action_update();
			for(int rep = 0; rep < 10; rep++)
			{
				int preview_size = board.get_preview_size();
				bool preview_moved = false;
				for(int i = 0; i < preview_size; i++)
				{
					//FIXME: sometimes choosing a spot in player range
					bn::point pos = board.get_preview_tile_position(i);
					tile_owner o = board.get_tile_owner(pos);
					// if overlapping with our own tiles
					if(o == tile_owner::FOE)
					{
						bn::point mov(0,0);
						battle_tile *tile = board.get_tile(pos);
						for(int dir = 0; dir < 4; dir++)
						{
							if(tile->get_neighbor(dir) != NULL && tile->get_neighbor(dir)->get_owner() != tile_owner::FOE)
							{
								direction move_dir = directions::int_to_dir(dir);
								mov = directions::dir_to_vector(move_dir);
								break;
							}
						}
						selected_tile = board.move_selected_tile(mov);
						continue;
					}
				}

				foe_action_update();

				if(!preview_moved)
					break;
			}

			

			select_tile(selected_tile->coordinates);
			bool success = board.mark_tiles(current_player);
			if(success)
			{
				update_tile_counts();
				_skill_meters.back().add_sp(5);
			}
			else
			{
				// uh oh ¯\_(ツ)_/¯
				//foe_base->set_owner(tile_owner::DEBUG);
			}
		}
		else	// if we somehow pulled a null neighbor out of the neighbor list
		{
			// uh oh ¯\_(ツ)_/¯
			// text_generator.generate(70, 10, "???", text_sprites);
		}
	}

	void battle_scene::foe_action_update()
	{
		int num_cycles = 24;
		for(int i = 0; i < num_cycles; i++)
		{
			update();
		}
	}
}