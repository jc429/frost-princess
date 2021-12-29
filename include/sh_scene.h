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
		bool scene_done = false;

		scene();
		virtual ~scene() = default;
	//	[[nodiscard]]
		virtual void update() = 0;
		virtual void fade_to_black();
		virtual void fade_from_black();
		void wait_for_update_cycles(int num_updates);

	};

	
	namespace scene_management
	{
		void register_current_scene(scene *scene);
		void set_next_scene(scene_type scene_type);
		scene_type get_next_scene_type();
		void exit_current_scene();
		void exit_current_scene(scene_type next_scene);
	}
}


#endif //SH_SCENE_H