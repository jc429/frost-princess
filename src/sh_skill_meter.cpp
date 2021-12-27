#include "sh_skill_meter.h"

#include <bn_fixed.h>
#include <bn_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "bn_sprite_items_skill_meter_button.h"
#include "bn_sprite_items_skill_meter_fill.h"
#include "bn_sprite_items_skill_meter_flame.h"

namespace sh
{
	#define SPR_METER 0
	#define SPR_FLAME 1
	#define SPR_BUTTON 2

	static bn::vector<bn::sprite_animate_action<4>, 2> skill_flame_animations;
	static bn::vector<bn::sprite_animate_action<2>, 2> skill_button_animations;
	

	skill_meter::skill_meter(bn::fixed_point meter_pos, bool player_side, bn::fixed_point flame_pos)
	{
		bn::fixed_point pos = meter_pos;
		_position = meter_pos;
		_anchor_left = player_side;
		_fill_amt = 0.25;
		if(player_side)
			pos.set_x(pos.x() + (16*_fill_amt));
		else
			pos.set_x(pos.x() + 40 - (16*_fill_amt));
		_current_sp = 0;
		_max_sp = 40;
		{
			bn::sprite_builder builder(bn::sprite_items::skill_meter_fill);
			builder.set_position(pos);
			builder.set_horizontal_scale(0.5);
			builder.set_bg_priority(1);
			builder.set_z_order(0);
			_sprites.push_back(builder.release_build());
		}
		{
			bn::sprite_builder builder(bn::sprite_items::skill_meter_flame);
			builder.set_position(flame_pos);
			builder.set_horizontal_flip(!player_side);
			builder.set_bg_priority(1);
			builder.set_z_order(-10);
			_sprites.push_back(builder.release_build());
			skill_flame_animations.push_back(bn::create_sprite_animate_action_forever(_sprites.at(SPR_FLAME), 4, bn::sprite_items::skill_meter_flame.tiles_item(), 0, 1, 2, 3));
		}
		if(player_side){
			bn::sprite_builder builder(bn::sprite_items::skill_meter_button);
			builder.set_position(pos + bn::fixed_point(-8,-1));
			builder.set_bg_priority(1);
			builder.set_z_order(-10);
			_sprites.push_back(builder.release_build());
			skill_button_animations.push_back(bn::create_sprite_animate_action_forever(_sprites.at(SPR_BUTTON), 16, bn::sprite_items::skill_meter_button.tiles_item(), 0, 1));
		}
		
		update_sprite();
	}

	void update_skill_meter_animations()
	{
		for(auto it = skill_flame_animations.begin(), end = skill_flame_animations.end(); it != end; ++it)
		{
			it->update();
		}
		for(auto it = skill_button_animations.begin(), end = skill_button_animations.end(); it != end; ++it)
		{
			it->update();
		}
	}

	void skill_meter::set_position(bn::point pos)
	{
		_position = pos;
	}


	bn::fixed skill_meter::fill_percent()
	{
		return _fill_amt;
	}

	bool skill_meter::meter_filled()
	{
		return (_current_sp >= _max_sp);
	}

	void skill_meter::set_current_sp(int current_sp)
	{
		_current_sp = bn::clamp(current_sp, 0, _max_sp);
		update_sprite();

	}

	void skill_meter::add_sp(int amt)
	{
		_current_sp = bn::clamp(_current_sp + amt, 0, _max_sp);
		update_sprite();
	}

	void skill_meter::set_max_sp(int max_sp)
	{
		_max_sp = bn::max(max_sp, 1);
		_current_sp = bn::min(max_sp, _current_sp);
		update_sprite();
	}

	void skill_meter::clear_sp()
	{
		_current_sp = 0;
		update_sprite();
	}

	void skill_meter::update_sprite()
	{
		bn::fixed_point pos = _position;
		bn::fixed sp_ratio = bn::clamp(((bn::fixed)_current_sp / (bn::fixed)bn::max(_max_sp, 1)), (bn::fixed)0, (bn::fixed)1);
		// remap sp_ratio from [0-1] to [1-32] (pixel width of meter)
		bn::fixed fill_width = (sp_ratio * 16) * (bn::fixed)0.0625;
		_sprites.at(SPR_METER).set_horizontal_scale(bn::max(fill_width, (bn::fixed)0.03125));
		// bn::max((sp_ratio * 32).floor_integer(), 1) * (bn::fixed)0.03125;
		// bn::fixed fill_amt = (sp_ratio / (bn::fixed)0.0625) * (bn::fixed)0.0625;
	 
		
		int fill_offset;
		if(_anchor_left)
		{
			fill_offset = (sp_ratio * 16).floor_integer();
		}
		else
		{
			fill_offset = 32 - (sp_ratio * 16).ceil_integer();
		}
	
		pos.set_x(pos.x() + fill_offset);
		_sprites.at(SPR_METER).set_position(pos);
		set_flame_visible((_current_sp == _max_sp));
		set_button_prompt_visible((_current_sp == _max_sp));
	}

	void skill_meter::set_flame_visible(bool visible)
	{
		_sprites.at(SPR_FLAME).set_visible(visible);
	}

	void skill_meter::set_button_prompt_visible(bool visible)
	{
		if(_sprites.size() > SPR_BUTTON)
			_sprites.at(SPR_BUTTON).set_visible(visible);
	}

}