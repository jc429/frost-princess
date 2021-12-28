#ifndef SH_OPTIONS_SCENE_H
#define SH_OPTIONS_SCENE_H

#include "sh_scene.h"
#include "sh_direction.h"
#include "sh_menu_slider.h"

#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>

namespace sh
{

	class options_scene : public scene
	{
	private:
		bn::vector<bn::regular_bg_ptr, 2> backgrounds;

		bn::sprite_text_generator text_generator;
		bn::vector<bn::sprite_ptr, 24> text_sprites;
		bn::vector<menu_slider, 2> menu_sliders;

		const int _row_ct = 2;
		const int _y_inc = 16;
		int selected_row = 0;

	public:
		options_scene();
		~options_scene();
		void update() override;

		void select_option_row(int row);
	};

}

#endif //SH_OPTIONS_SCENE_H