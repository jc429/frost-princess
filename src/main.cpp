#include <bn_core.h>
#include <bn_sprite_text_generator.h>
#include <bn_keypad.h>
#include <bn_bg_palettes.h>

#include "sh_scene.h"
#include "sh_logo_scene.h"
#include "sh_title_scene.h"
#include "sh_level_select.h"
#include "sh_battle_scene.h"


static sh::scene_type next_scene_type;

int main()
{
	// init butano
	bn::core::init();
	
	bn::bg_palettes::set_transparent_color(bn::color(16, 24, 24));

	next_scene_type = sh::scene_type::LOGO;

	while(true)
	{
		switch(next_scene_type)
		{
		case sh::scene_type::LOGO:
			sh::logo_scene();
			bn::core::update();
			break;
		case sh::scene_type::TITLE:
			sh::title_scene();
			bn::core::update();
			break;
		case sh::scene_type::LEVEL_SELECT:
			sh::level_select_scene();
			bn::core::update();
			break;
		case sh::scene_type::BATTLE:
			sh::battle_scene();
			bn::core::update();
			break;
		default:
			break;
		}
	}
}

namespace sh
{
	void set_next_scene(sh::scene_type scene_type)
	{
		next_scene_type = scene_type;
	}
}