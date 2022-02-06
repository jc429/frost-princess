#include "sh_battle_tile.h"
#include "sh_battle_board.h"
#include "sh_battle_scene.h"
#include "sh_direction.h"
#include "sh_effects.h"
#include "sh_audio.h"
#include "sh_action_manager.h"

#include <bn_blending.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_builder.h>
#include <bn_vector.h>

#include "bn_regular_bg_items_battle_board.h"
#include "bn_sprite_items_board_tile.h"
#include "bn_sprite_items_preview_tile.h"

/*******************************************************/
/** Separate file for handling special move sequences **/
/*******************************************************/

namespace sh
{

	bool battle_board::use_special_action(tile_owner owner, special_action_pattern pattern)
	{
		effects::effect_id eff_id = effects::effect_id::NONE;
		tile_condition condition = tile_condition::NORMAL;
		switch(owner)
		{
		case tile_owner::PLAYER:
			eff_id = effects::effect_id::SHINE;
			condition = tile_condition::FROZEN;
			break;
		case tile_owner::FOE:
			eff_id = effects::effect_id::EXPLOSION;
			condition = tile_condition::BURNED;
			break;

		default:
			break;
		}


		if(owner != tile_owner::EMPTY && selected_tile->get_owner() == owner)
		{
			battle_tile *tile = selected_tile;
			int eff_duration = 4;
			
			int cycle_wait = 24;
			bn::vector<battle_tile*, 8> apply_tiles;
			battle_tile *neighbors[8];
			for(int i = 0; i < 8; i++)
			{
				neighbors[i] = NULL;
			}

			create_effect_at_tile(eff_id, tile);
			
			audio::play_sound(sound_id::WEW);
			current_scene->wait_for_update_cycles(cycle_wait);
			tile->set_condition(condition, eff_duration);
			tile->set_owner(owner);
			
			switch(pattern)
			{
			case special_action_pattern::CROSS_5:
				for(int i = 0; i < 4; i++)
				{
					neighbors[i] = tile->get_neighbor(i);
				}
				for(int step = 0; step < 4; step++)
				{
					apply_tiles.clear();

					for(int i = 0; i < 4; i++)
					{
						if(neighbors[i] != NULL)
						{
							apply_tiles.push_back(neighbors[i]);
						}
					}
					if(!apply_tiles.empty())
					{
						audio::play_sound(sound_id::WEW);
						for(auto it = apply_tiles.begin(), end = apply_tiles.end(); it != end; ++it)
						{
							create_effect_at_tile(eff_id,*it);
						}
						current_scene->wait_for_update_cycles(cycle_wait);
						for(int i = 0; i < 4; i++)
						{
							if(neighbors[i] != NULL)
							{
								neighbors[i]->set_condition(condition, eff_duration);
								if(neighbors[i]->is_base() && (neighbors[i]->get_owner() != owner))
								{
									current_scene->apply_damage_to_player(neighbors[i]->get_owner(), 5);
								}
								else
								{
									neighbors[i]->set_owner(owner);
								}
								neighbors[i] = neighbors[i]->get_neighbor(i);
							}
						}
					}
				}
				return true;
				break;

			case special_action_pattern::STAR_3:
			
				for(int i = 0; i < 4; i++)
				{
					neighbors[i] = tile->get_neighbor(i);
					if(neighbors[i] != NULL)
					{
						neighbors[i+4] = neighbors[i]->get_neighbor((i+1)%4);
					}
				}
				for(int step = 0; step < 2; step++)
				{
					apply_tiles.clear();
					for(int i = 0; i < 8; i++)
					{
						if(neighbors[i] != NULL)
						{
							apply_tiles.push_back(neighbors[i]);
						}
					}
					if(!apply_tiles.empty())
					{
						audio::play_sound(sound_id::WEW);
						for(auto it = apply_tiles.begin(), end = apply_tiles.end(); it != end; ++it)
						{
							create_effect_at_tile(eff_id,*it);
						}
						current_scene->wait_for_update_cycles(cycle_wait);
						for(int i = 0; i < 8; i++)
						{
							if(neighbors[i] != NULL)
							{
								neighbors[i]->set_condition(condition, eff_duration);
								if(neighbors[i]->is_base() && (neighbors[i]->get_owner() != owner))
								{
									current_scene->apply_damage_to_player(neighbors[i]->get_owner(), 5);
								}
								else{
									neighbors[i]->set_owner(owner);
								}
								neighbors[i] = neighbors[i]->get_neighbor(i%4);
								if(neighbors[i] != NULL && i >= 4)
								{
									neighbors[i] = neighbors[i]->get_neighbor((i+1)%4);
								}
							}
						}
					}
				}
				return true;
				break;

			default:
				break;
			}
			
			return true;
		}
		else
		{
			return false;
		}
	}
}