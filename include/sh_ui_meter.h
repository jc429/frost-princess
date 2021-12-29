#ifndef SH_UI_METER_H
#define SH_UI_METER_H

#include <bn_fixed.h>
#include <bn_point.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	enum class meter_type
	{
		HP,
		SP
	};

	class ui_meter
	{
	protected:
		meter_type _type;
		bool _anchor_left;
		bn::fixed_point _position;
		int _current_val;
		int _max_val;
		bn::vector<bn::sprite_ptr, 2> _fill_sprites;

		virtual void update_sprite();

	public:
		ui_meter(bn::fixed_point pos, meter_type type, int max_val, bool anchor_left);
		virtual ~ui_meter();
		
		void set_position(bn::point pos);
		bool meter_filled();
		bn::fixed get_fill_percent();

		void set_current_val(int current);
		int get_current_val();
		void add_val(int val_add);
		void set_max_val(int max_val);
		void fill();
		void clear();

	};
}

#endif //SH_UI_METER_H