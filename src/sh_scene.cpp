#include "sh_scene.h"
#include "sh_action_manager.h"

#include <bn_blending.h>
#include <bn_blending_fade_alpha.h>
#include <bn_core.h>
#include <bn_fixed.h>
#include <bn_math.h>
#include <bn_bg_palettes_actions.h>
#include <bn_sprite_palettes_actions.h>
#include <bn_camera_actions.h>

namespace sh
{

	scene::scene() :
		_camera (bn::camera_ptr::create(0, 0))
	{
		_camera_actions.clear();
		scene_management::register_current_scene(this);
	}
	
	scene::~scene()
	{
		_camera_actions.clear();
		action_manager::clear_sprite_move_actions();
	}

	void scene::update()
	{
		for(auto it = _camera_actions.begin(), end = _camera_actions.end(); it != end; ++it)
		{
			it->update();
			if(it->done())
			{
				_camera_actions.erase(it);
				end = _camera_actions.end();
			}
		}
		bn::core::update();
	}


	void scene::wait_for_update_cycles(int num_updates)
	{
		for(int i = 0; i < num_updates; i++)
		{
			update();
		}
	}


	void scene::fade_to_black()
	{
		bn::sprite_palettes_fade_to_action sprite_fade(10, 1);
		bn::bg_palettes_fade_to_action bg_fade(10, 1);
		while((!bg_fade.done()) || (!sprite_fade.done()))
		{
			if(!bg_fade.done())
				bg_fade.update();
			if(!sprite_fade.done())
				sprite_fade.update();
			bn::core::update();
		}
	}


	void scene::fade_from_black()
	{
		bn::sprite_palettes_fade_to_action sprite_fade(10, 0);
		bn::bg_palettes_fade_to_action bg_fade(10, 0);
		while((!bg_fade.done()) || (!sprite_fade.done()))
		{
			if(!bg_fade.done())
				bg_fade.update();
			if(!sprite_fade.done())
				sprite_fade.update();
			bn::core::update();
		}
	}

	void scene::dim_screen()
	{
		bn::sprite_palettes_fade_to_action sprite_fade(10, 0.5);
		bn::bg_palettes_fade_to_action bg_fade(10, 0.5);
		while((!bg_fade.done()) || (!sprite_fade.done()))
		{
			if(!bg_fade.done())
				bg_fade.update();
			if(!sprite_fade.done())
				sprite_fade.update();
			bn::core::update();
		}
	}

	void scene::undim_screen()
	{
		bn::sprite_palettes_fade_to_action sprite_fade(10, 0);
		bn::bg_palettes_fade_to_action bg_fade(10, 0);
		while((!bg_fade.done()) || (!sprite_fade.done()))
		{
			if(!bg_fade.done())
				bg_fade.update();
			if(!sprite_fade.done())
				sprite_fade.update();
			bn::core::update();
		}
	}

	bn::camera_ptr& scene::get_camera()
	{
		return _camera;
	}

	void scene::shake(int intensity, int duration)
	{
		bn::fixed_point left(-intensity,0);
		bn::fixed_point right(intensity,0);
		int qrtr = duration / 4;
		_camera.set_position(0,0);
		bn::camera_move_to_action shake_action(_camera, qrtr, right);
		while(!shake_action.done())
		{
			shake_action.update();
			bn::core::update();
		}
		shake_action = bn::camera_move_to_action(_camera, qrtr*2, left);
		while(!shake_action.done())
		{
			shake_action.update();
			bn::core::update();
		}
		shake_action = bn::camera_move_to_action(_camera, qrtr, 0,0);
		while(!shake_action.done())
		{
			shake_action.update();
			bn::core::update();
		}
		_camera.set_position(0, 0);
	}

	namespace scene_management
	{
		static scene_type _next_scene_type;
		static scene *_current_scene = NULL;

		void register_current_scene(scene *scene)
		{
			_current_scene = scene;
		}

		void set_next_scene(sh::scene_type scene_type)
		{
			_next_scene_type = scene_type;
		}

		scene_type get_next_scene_type()
		{
			return _next_scene_type;
		}

		void exit_current_scene()
		{
			if(_current_scene != NULL)
			{
				_current_scene->scene_done = true;
			}
		}

		void exit_current_scene(scene_type next_scene)
		{
			set_next_scene(next_scene);
			exit_current_scene();
		}

	}

}