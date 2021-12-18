#ifndef SH_SKILL_METER_H
#define SH_SKILL_METER_H

#include <bn_fixed.h>
#include <bn_point.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	class skill_meter
	{
	private:
		bool _anchor_left;
		bn::vector<bn::sprite_ptr, 2> _sprites;
		bn::fixed_point _position;
		bn::fixed _fill_amt;
		int _current_sp;
		int _max_sp;

		void update_sprite();

	public:
		skill_meter(bn::fixed_point meter_pos, bool anchor_left, bn::fixed_point flame_pos, bool mirror_flame);
		void set_position(bn::point pos);
		bn::fixed fill_percent();
		bool meter_filled();
		void set_current_sp(int current_sp);
		void add_sp(int amt);
		void set_max_sp(int max_sp);
		void clear_sp();
		void set_flame_visible(bool visible);
		bn::sprite_ptr &get_flame_sprite();

	};

}

#endif //SH_SKILL_METER_H