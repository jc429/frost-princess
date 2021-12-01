#include "bn_core.h"
#include "bn_sprite_text_generator.h"


#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_items_red_sprite.h"
// font stuff
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"


extern void battle_scene(bn::sprite_text_generator& text_generator);


int main()
{
	// init butano
	bn::core::init();
	
	// init text generator
	bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
	text_generator.set_center_alignment();
	bn::bg_palettes::set_transparent_color(bn::color(16, 24, 24));

	while(true)
	{
		battle_scene(text_generator);
		bn::core::update();
	}
}
