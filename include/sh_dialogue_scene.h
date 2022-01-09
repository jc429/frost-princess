#ifndef SH_DIALOGUE_SCENE_H
#define SH_DIALOGUE_SCENE_H

#include "sh_scene.h"
#include "sh_character_sprite.h"

#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_vector.h>

namespace sh
{
	#define DIALOGUE_LINE_CT 3

	class dialogue_scene : public scene
	{
	private:
		bn::vector<bn::regular_bg_ptr, 2> backgrounds;
		bn::vector<character_sprite, 2> character_sprites;

		bn::sprite_text_generator text_generator;
		bn::vector<bn::sprite_ptr, 48> text_sprites;

		bn::vector<bn::string<64>, DIALOGUE_LINE_CT> dialogue_lines;
		bn::vector<bn::fixed_point, DIALOGUE_LINE_CT> line_pos;

	public:
		dialogue_scene();
		~dialogue_scene();
		void update() override;

		void load_text();
		void load_next_paragraph();
		void clear_text();
		void write_text();
	};

}

#endif //SH_DIALOGUE_SCENE_H
