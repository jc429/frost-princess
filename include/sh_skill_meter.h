#ifndef SH_SKILL_METER_H
#define SH_SKILL_METER_H

#include "sh_ui_meter.h"

#include <bn_fixed.h>
#include <bn_point.h>
#include <bn_fixed_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	class skill_meter : public ui_meter
	{
	private:
		bn::vector<bn::sprite_ptr, 2> _skill_meter_sprites;
		bn::vector<bn::sprite_animate_action<4>, 1> skill_flame_animations;
		bn::vector<bn::sprite_animate_action<2>, 1> skill_button_animations;
	protected:
		void update_sprite() override;

	public:
		skill_meter(bn::fixed_point pos, int max_val, bool anchor_left, bn::fixed_point flame_offset, bool button_prompt);
		~skill_meter();
		
		void update();

		void set_flame_visible(bool visible);
		void set_button_prompt_visible(bool visible);

	};

}

#endif //SH_SKILL_METER_H