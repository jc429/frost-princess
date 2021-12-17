#include "sh_skill_meter.h"

#include <bn_fixed.h>
#include <bn_point.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_builder.h>

#include "bn_sprite_items_skill_meter_fill.h"
#include "bn_sprite_items_skill_meter_flame.h"

namespace sh
{

	skill_meter::skill_meter(bn::fixed_point meter_pos, bool anchor_left, bn::fixed_point flame_pos, bool mirror_flame)
	{
		bn::fixed_point pos = meter_pos;
		_position = meter_pos;
		_anchor_left = anchor_left;
		_fill_amt = 0.25;
		if(anchor_left)
			pos.set_x(pos.x() + _fill_amt);
		_current_sp = 40;
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
			builder.set_horizontal_flip(mirror_flame);
			builder.set_bg_priority(1);
			builder.set_z_order(-10);
			_sprites.push_back(builder.release_build());
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
		bn::fixed fill_amt = bn::clamp(((bn::fixed)_current_sp / (bn::fixed)bn::max(_max_sp, 1)), (bn::fixed)0, (bn::fixed)1);
		bn::fixed_point pos = _position;
		pos.set_x(pos.x() + (16*fill_amt));
		_sprites.front().set_position(pos);
		_sprites.front().set_horizontal_scale(bn::max(fill_amt, (bn::fixed)0.01));
		set_flame_visible((_current_sp == _max_sp));
	}

	void skill_meter::set_flame_visible(bool visible)
	{
		_sprites.back().set_visible(visible);
	}

	bn::sprite_ptr &skill_meter::get_flame_sprite()
	{
		return _sprites.back();
	}
}