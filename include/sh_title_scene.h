#ifndef SH_TITLE_SCENE_H
#define SH_TITLE_SCENE_H

#include "sh_scene.h"

#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>

namespace sh
{
	enum class title_state
	{
		PRESS_START,
		MAIN_MENU
	};

	

	class title_scene : public scene
	{
	private:
		#define TITLE_MENU_ITEMS 3
		bn::regular_bg_ptr title_bg;
		bn::regular_bg_ptr title_snow_a;
		bn::regular_bg_ptr title_snow_b;
		bn::regular_bg_ptr title_overlay;

		bn::sprite_text_generator text_generator;
		bn::vector<bn::sprite_ptr, 12> text_sprites;

		title_state _current_state;
		bn::vector<bn::sprite_ptr, 8> title_sprites;
		bn::sprite_ptr *title_cursor;
		int current_menu_selection;
		bn::vector<bn::fixed_point, TITLE_MENU_ITEMS>	title_menu_cursor_pos;
	public:
		title_scene();
		void update() override;

		void set_title_state(title_state state);
		void set_cursor_selection(int selection);
	};

}

#endif //SH_TITLE_SCENE_H