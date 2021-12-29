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
	#define SPR_FLAME 0
	#define SPR_BUTTON 1

	static bn::vector<bn::sprite_animate_action<4>, 2> skill_flame_animations;
	static bn::vector<bn::sprite_animate_action<2>, 2> skill_button_animations;
	

	skill_meter::skill_meter(bn::fixed_point pos, int max_val, bool anchor_left, bn::fixed_point flame_offset, bool button_prompt)
		: ui_meter(pos, meter_type::SP, max_val, anchor_left)
	{
		_current_val = 0;
		{
			bn::sprite_builder builder(bn::sprite_items::skill_meter_flame);
			bn::fixed_point flame_pos = pos + flame_offset;
			builder.set_position(flame_pos);
			builder.set_horizontal_flip(!button_prompt); // hacky temp fix
			builder.set_bg_priority(1);
			builder.set_z_order(-10);
			_skill_meter_sprites.push_back(builder.release_build());
			skill_flame_animations.push_back(bn::create_sprite_animate_action_forever(_skill_meter_sprites.at(SPR_FLAME), 4, bn::sprite_items::skill_meter_flame.tiles_item(), 0, 1, 2, 3));
		}
		if(button_prompt){
			bn::sprite_builder builder(bn::sprite_items::skill_meter_button);
			builder.set_position(pos + bn::fixed_point(-7,-1));
			builder.set_bg_priority(1);
			builder.set_z_order(-10);
			_skill_meter_sprites.push_back(builder.release_build());
			skill_button_animations.push_back(bn::create_sprite_animate_action_forever(_skill_meter_sprites.at(SPR_BUTTON), 16, bn::sprite_items::skill_meter_button.tiles_item(), 0, 1));
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



	void skill_meter::update_sprite()
	{
		ui_meter::update_sprite();

		set_flame_visible(meter_filled());
		set_button_prompt_visible(meter_filled());
	}

	void skill_meter::set_flame_visible(bool visible)
	{
		if(_skill_meter_sprites.size() > SPR_FLAME)
			_skill_meter_sprites.at(SPR_FLAME).set_visible(visible);
	}

	void skill_meter::set_button_prompt_visible(bool visible)
	{
		if(_skill_meter_sprites.size() > SPR_BUTTON)
			_skill_meter_sprites.at(SPR_BUTTON).set_visible(visible);
	}

}