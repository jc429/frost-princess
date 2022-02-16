#include <bn_core.h>
#include <bn_sprite_text_generator.h>
#include <bn_keypad.h>
#include <bn_bg_palettes.h>

#include "sh_character.h"
#include "sh_scene.h"
#include "sh_logo_scene.h"
#include "sh_title_scene.h"
#include "sh_level_select_scene.h"
#include "sh_dialogue_scene.h"
#include "sh_battle_scene.h"
#include "sh_options_scene.h"
#include "sh_credits_scene.h"



int main()
{
	// init butano
	bn::core::init();
	
	bn::bg_palettes::set_transparent_color(bn::color(16, 24, 24));

	sh::characters::set_player_character(sh::character_id::PROTAGONIST);
	sh::characters::set_foe_character(sh::character_id::PROTAGONIST);


	while(true)
	{
		switch(sh::scene_management::get_next_scene_type())
		{
		case sh::scene_type::LOGO:
			sh::logo_scene();
			break;
		case sh::scene_type::TITLE:
			sh::title_scene();
			break;
		case sh::scene_type::LEVEL_SELECT:
			sh::level_select_scene();
			break;
		case sh::scene_type::DIALOGUE:
			sh::dialogue_scene();
			break;
		case sh::scene_type::BATTLE:
			sh::battle_scene();
			break;
		case sh::scene_type::OPTIONS:
			sh::options_scene();
			break;
		case sh::scene_type::CREDITS:
			sh::credits_scene();
			break;
		default:
			break;
		}

		bn::core::update();
	}
}

namespace sh
{

}