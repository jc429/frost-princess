#ifndef SH_SCENE_H
#define SH_SCENE_H

#include "bn_optional_fwd.h"

namespace sh
{
	enum class scene_type
	{
		LOGO,
		INTRO,
		TITLE,
		LEVEL_SELECT,
		BATTLE,
		OPTIONS,
		CREDITS
	};

	class scene
	{
	public:
		scene_type type;

		virtual ~scene() = default;
	//	[[nodiscard]]
		virtual void update() = 0;
		virtual void fade_to_black();
		virtual void fade_from_black();
		void wait_for_update_cycles(int num_updates);

	protected:
		scene() = default;
	};

	void set_next_scene(sh::scene_type scene_type);
}


#endif //SH_SCENE_H