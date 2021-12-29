#include "sh_ui_meter.h"

#include <bn_fixed.h>
#include <bn_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>


#include "bn_sprite_items_hp_meter_fill.h"
#include "bn_sprite_items_skill_meter_fill.h"

namespace sh
{
	#define SPR_METER_FILL 0

	bn::sprite_item get_fill_sprite(meter_type type);

	ui_meter::ui_meter(bn::fixed_point pos, meter_type type, int max_val, bool anchor_left)
	{
		_type = type;
		_anchor_left = anchor_left;
		_position = pos;
		_max_val = max_val;
		_current_val = max_val;
		
		// build fill sprite
		{
			const bn::sprite_item meter_fill = get_fill_sprite(type);
			bn::sprite_builder builder(meter_fill);
			builder.set_position(pos);
			builder.set_horizontal_scale(0.5);
			builder.set_bg_priority(1);
			builder.set_z_order(0);
			_fill_sprites.push_back(builder.release_build());
		}
		update_sprite();
	}

	ui_meter::~ui_meter()
	{
		_fill_sprites.clear();
	}
	
	void ui_meter::update_sprite()
	{
		if(_fill_sprites.empty())
		{
			return;
		}
		bn::fixed fill_center_x = _position.x();
		bn::fixed val_ratio = get_fill_percent();
		// remap sp_ratio from [0-1] to [1-32] (pixel width of meter)
		bn::fixed fill_width = (val_ratio * 16) * (bn::fixed)0.0625;
		_fill_sprites.at(SPR_METER_FILL).set_horizontal_scale(bn::max(fill_width, (bn::fixed)0.03125));
		int fill_offset;
		// if(_anchor_left)
		{
			fill_offset = (val_ratio * 16).floor_integer();
		}
		// TODO: add right facing meter
		fill_center_x = (fill_center_x + fill_offset);
		_fill_sprites.at(SPR_METER_FILL).set_x(fill_center_x);

	}
	
	void ui_meter::set_position(bn::point pos)
	{
		_position = pos;
	}

	bool ui_meter::meter_filled()
	{
		return (_current_val >= _max_val);
	}

	bn::fixed ui_meter::get_fill_percent()
	{
		return bn::clamp(((bn::fixed)_current_val / (bn::fixed)bn::max(_max_val, 1)), (bn::fixed)0, (bn::fixed)1);
	}

	
	void ui_meter::set_current_val(int current)
	{
		_current_val = bn::clamp(current, 0, _max_val);
		update_sprite();
	}

	int ui_meter::get_current_val()
	{
		return _current_val;
	}

	void ui_meter::add_val(int val_add)
	{
		_current_val = bn::clamp(_current_val + val_add, 0, _max_val);
		update_sprite();
	}
	
	void ui_meter::set_max_val(int max_val)
	{
		_max_val = bn::max(max_val, 1);
		_current_val = bn::min(_max_val, _current_val);
		update_sprite();
	}
	
	void ui_meter::fill()
	{
		_current_val = _max_val;
		update_sprite();
	}
	
	void ui_meter::clear()
	{
		_current_val = 0;
		update_sprite();
	}

	/**********************************************/

	bn::sprite_item get_fill_sprite(meter_type type)
	{
		switch(type)
		{
		case meter_type::HP:
			return bn::sprite_items::hp_meter_fill;
			break;
		case meter_type::SP:
			return bn::sprite_items::skill_meter_fill;
			break;
		default:
			return bn::sprite_items::skill_meter_fill;
			break;
		}
	}
}