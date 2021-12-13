#ifndef SH_LEVEL_SELECT
#define SH_LEVEL_SELECT

#include "sh_scene.h"

#include <bn_regular_bg_ptr.h>


namespace sh
{
	class level_select_scene : public scene
	{

	public:
		bn::regular_bg_ptr title_bg;

		level_select_scene();
		void update();
	};

}

#endif //SH_LEVEL_SELECT