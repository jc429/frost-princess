#include "bn_core.h"
#include "bn_sprite_text_generator.h"


#include "bn_keypad.h"
#include "bn_bg_palettes.h"
// font stuff
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "sh_battle.h"

int main()
{
	// init butano
	bn::core::init();
	
	bn::bg_palettes::set_transparent_color(bn::color(16, 24, 24));

	while(true)
	{
		sh::battle_scene();
		bn::core::update();
	}
}
