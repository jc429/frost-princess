#ifndef SH_SCENE_H
#define SH_SCENE_H

#include "bn_optional_fwd.h"

namespace sh
{
	enum class scene_type
	{
		LOGO,
		TITLE,
		MAIN_MENU,
		BATTLE
	};

	class scene
	{
	public:
		scene_type type;

		virtual ~scene() = default;
	//	[[nodiscard]]
	//	virtual void update() = 0;

	protected:
		scene() = default;
	};

	
	void set_next_scene(sh::scene_type scene_type);
}


#endif //SH_SCENE_H