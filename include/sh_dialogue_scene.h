#ifndef SH_DIALOGUE_SCENE_H
#define SH_DIALOGUE_SCENE_H

#include "sh_scene.h"
#include "sh_dialogue_paragraph.h"
#include "sh_dialogue.h"
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
	

	class dialogue_scene : public scene
	{
	private:
		bn::vector<bn::regular_bg_ptr, 3> backgrounds;
		bn::vector<character_sprite, 2> character_sprites;

		bn::sprite_text_generator text_generator;
		bn::vector<bn::sprite_ptr, 48> text_sprites;

		// bn::vector<bn::string<64>, DIALOGUE_LINE_CT> dialogue_lines;
		bn::vector<bn::fixed_point, DIALOGUE_LINE_CT> line_pos;

		bn::vector<bn::sprite_ptr, 4> speaker_sprites;

		dialogue_paragraph *current_paragraph;

	public:
		dialogue_scene();
		~dialogue_scene();
		void update() override;

		void set_speaker_name(character_id char_id, dia_spkr_pos side);
		void clear_speaker_name();

		void load_dialogue(dialogue_id dia_id);
		void load_paragraph(dialogue_paragraph *paragraph);
		void clear_text();
		void write_text();

		void open_pause_menu();
	};

}

#endif //SH_DIALOGUE_SCENE_H
