#ifndef SH_SCENE_H
#define SH_SCENE_H

#include <bn_optional_fwd.h>
#include <bn_camera_actions.h>
#include <bn_camera_ptr.h>
#include <bn_vector.h>

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
	protected:
		bn::camera_ptr _camera;
		bn::vector<bn::camera_move_to_action, 1> _camera_actions;
	public:
		scene_type type;
		bool scene_done = false;

		scene();
		virtual ~scene();
	//	[[nodiscard]]
		virtual void update();
		void wait_for_update_cycles(int num_updates);
		
		virtual void fade_to_black();
		virtual void fade_from_black();
		virtual void dim_screen();
		virtual void undim_screen();

		bn::camera_ptr& get_camera();
		virtual void shake(int intensity, int duration);
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