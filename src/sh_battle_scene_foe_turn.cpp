#include "sh_battle_scene.h"
#include "sh_random.h"
#include "sh_audio.h"

#include <bn_core.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>

#include "variable_8x16_sprite_font.h"


/***************************************/
/** Separate file for handling foe ai **/
/***************************************/

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

		bn::vector<battle_tile*, 80> foe_tiles;
		foe_tiles.clear();
		bn::vector<battle_tile*, 80> neighboring_tiles;
		neighboring_tiles.clear();

		

		// identify tiles neighboring current spread
		{
			bool tile_checked[81];
			bn::vector<battle_tile*, 80> active_tiles;
			active_tiles.clear();
			for(int i = 0; i < 81; i++)
			{
				tile_checked[i] = false;
			}

			foe_tiles.push_back(foe_base);
			active_tiles.push_back(foe_base);
			while(!active_tiles.empty())
			{
				battle_tile *active_tile = active_tiles.back();
				active_tiles.pop_back();
				tile_checked[active_tile->tile_id] = true;
				for(int i = 0; i < 4; i++)
				{
					battle_tile *neighbor = active_tile->get_neighbor(i);
					if(neighbor != NULL)
					{
						if(tile_checked[neighbor->tile_id])
						{
							continue;
						}
						if(neighbor->get_owner() == tile_owner::FOE)
						{
							foe_tiles.push_back(neighbor);
							active_tiles.push_back(neighbor);
						}
						else
						{
							bool neighbor_registered = false;
							for(int n = 0; n < neighboring_tiles.size(); n++)
							{
								if(neighboring_tiles.at(n) == neighbor)
								{
									neighbor_registered = true;
									break;
								}
							}
							if(!neighbor_registered)
							{
								neighboring_tiles.push_back(neighbor);
								// neighbor->set_owner(tile_owner::DEBUG);
							}
						}
					}
				}
			}
		}
		if(neighboring_tiles.empty())
		{
			// if neighboring nothing, return
			//return;
		}
		
		// text_generator.generate(70, -10, bn::to_string<2>(foe_neighboring_tiles.size()), text_sprites);
		if(_skill_meters.back().meter_filled())
		{
			int r = random.get_int(0, foe_tiles.size());
			battle_tile *selected_tile = foe_tiles.at(r);
			board.set_selected_tile(selected_tile->coordinates);
			board.hide_preview_tiles();
			bool success = board.use_special_action(tile_owner::FOE, special_action_pattern::CROSS_5);

			if(success)
			{
				_skill_meters.back().clear();
				audio::play_sound(sound_id::BLIP_HIGH);
				// switch back to card cursor
				battle_cursor.set_visible(false);
				return;
			}
		}
		// TODO: make better than random selection maybe
		int r = random.get_int(0, neighboring_tiles.size());
		battle_tile *selected_tile = neighboring_tiles.at(r);
		// text_generator.generate(70, 0, bn::to_string<4>(selected_tile->coordinates.x()) + "," + bn::to_string<4>(selected_tile->coordinates.y()), text_sprites);
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
						int d = random.get_int(0,3);	// sample directions in a random order
						for(int x = 0; x < 4; x++)
						{
							int dir = (x + d) % 4;
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
			int sp_gain = 0;
			bool success = board.mark_tiles(current_player, sp_gain);
			if(success)
			{
				update_tile_counts();
				_skill_meters.back().add_val(sp_gain);
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

	bool battle_scene::foe_ai_check_tile_valid(battle_tile *tile)
	{
		return(tile != NULL 
			&& tile->get_owner() != tile_owner::FOE
			&& tile->get_condition() == tile_condition::NORMAL
		);
	}
}