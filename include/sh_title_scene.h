#ifndef SH_TITLE_SCENE_H
#define SH_TITLE_SCENE_H

#include "sh_scene.h"

#include "bn_regular_bg_ptr.h"


namespace sh
{

	class title_scene : public scene
	{
	private:
		bn::regular_bg_ptr title_bg;
		bn::regular_bg_ptr title_snow_a;
		bn::regular_bg_ptr title_snow_b;

	public:
		title_scene();
		void update() override;
	};

}

#endif //SH_TITLE_SCENE_H