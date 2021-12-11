#include "sh_skill_meter.h"

#include "bn_fixed.h"
#include "bn_point.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_builder.h"

#include "bn_sprite_items_skill_meter_fill.h"

namespace sh
{

	skill_meter::skill_meter(int pos_x, int pos_y, bool anchor_left) 
	{
		bn::fixed_point pos(pos_x, pos_y);
		_position = pos;
		_anchor_left = anchor_left;
		_fill_amt = 0.25;
		if(anchor_left)
			pos.set_x(pos.x() + _fill_amt);
		_current_sp = 0;
		_max_sp = 40;
		{
			
			bn::sprite_builder builder(bn::sprite_items::skill_meter_fill);
			builder.set_position(pos);
			builder.set_horizontal_scale(0.5);
			builder.set_bg_priority(0);
			builder.set_z_order(0);
			_fill_sprites.push_back(builder.release_build());
		}
		update_sprite();
	}

	void skill_meter::set_position(bn::point pos)
	{
		_position = pos;
	}


	bn::fixed skill_meter::fill_percent()
	{
		return _fill_amt;
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
		update_sprite();
	}

	void skill_meter::clear_sp()
	{
		_current_sp = 0;
		update_sprite();
	}

	void skill_meter::update_sprite()
	{
		bn::fixed fill_amt = bn::clamp(((bn::fixed)_current_sp / (bn::fixed)bn::max(_max_sp, 1)), (bn::fixed)0, (bn::fixed)1);
		bn::fixed_point pos = _position;
		pos.set_x(pos.x() + (16*fill_amt));
		_fill_sprites.back().set_position(pos);
		_fill_sprites.back().set_horizontal_scale(bn::max(fill_amt, (bn::fixed)0.01));
	}

	
}