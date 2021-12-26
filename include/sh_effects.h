#ifndef SH_EFFECT_H
#define SH_EFFECT_H

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>

namespace sh
{
	namespace effects
	{

		enum class effect_id
		{
			NONE,
			SHINE,
		};

		bn::sprite_ptr create_effect_at_position(effect_id effect, bn::fixed_point pos);
		void update_effect_animations();
	}
}

#endif //SH_EFFECT_H