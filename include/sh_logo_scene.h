#ifndef SH_LOGO_SCENE_H
#define SH_LOGO_SCENE_H

#include "sh_scene.h"

#include "bn_regular_bg_ptr.h"

namespace sh
{

	class logo_scene : public scene
	{
	public:
		bn::regular_bg_ptr logo_bg;

		logo_scene();
	};

}

#endif //SH_LOGO_SCENE_H