#ifndef SH_LEVEL_SELECT
#define SH_LEVEL_SELECT

#include "sh_scene.h"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	class level_select_scene : public scene
	{
	private:
		bn::regular_bg_ptr title_bg;
		bn::vector<bn::sprite_ptr, 9> portrait_frames;

	public:

		level_select_scene();
		void update();
	};

}

#endif //SH_LEVEL_SELECT