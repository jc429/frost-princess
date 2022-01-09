#include "sh_dialogue_scene.h"

#include <bn_core.h>
#include <bn_keypad.h>

#include "variable_8x16_sprite_font.h"
#include "bn_regular_bg_items_options_bg.h"
#include "bn_regular_bg_items_dialogue_fg.h"

namespace sh
{
	static const bn::fixed_point char_pos_left_default(-56,24);
	static const bn::fixed_point char_pos_right_default(56,24);

	dialogue_scene::dialogue_scene() :
		text_generator(common::variable_8x16_sprite_font)
	{
		dialogue_lines.clear();
		backgrounds.clear();
		character_sprites.clear();
		bn::regular_bg_ptr bg = bn::regular_bg_items::options_bg.create_bg(0, 0);
		bg.set_priority(3);
		backgrounds.push_back(bg);
		bg = bn::regular_bg_items::dialogue_fg.create_bg(0, 0);
		bg.set_priority(1);
		backgrounds.push_back(bg);

		text_generator.set_left_alignment();
		text_generator.set_bg_priority(1);

		{
			bn::fixed_point pos(-112, 36);
			line_pos.push_back(pos);
			pos = pos + bn::fixed_point(0,16);
			line_pos.push_back(pos);
			pos = pos + bn::fixed_point(0,16);
			line_pos.push_back(pos);
		}

		character_sprites.push_back(character_sprite(character_id::PROTAGONIST));
		character_sprites.front().set_position(char_pos_left_default);
		character_sprites.push_back(character_sprite(character_id::FLAME_WITCH));
		character_sprites.back().set_position(char_pos_right_default);

		load_text();

		fade_from_black();
		scene_done = false;
		while(!scene_done)
		{

			if(bn::keypad::a_pressed() || bn::keypad::b_pressed())
			{
				// load next line
				// load_next_paragraph();
				write_text();
				// load_text();
			}



			update();
		}

	}


	dialogue_scene::~dialogue_scene()
	{
		backgrounds.clear();
		character_sprites.clear();
		dialogue_lines.clear();
	}



	void dialogue_scene::update()
	{
		backgrounds.front().set_position(backgrounds.front().position() + bn::fixed_point(0.25,0.25));


		for(auto it = character_sprites.begin(), end = character_sprites.end(); it != end; ++it)
		{
			it->update();
		}

		bn::core::update();
	}

	void dialogue_scene::load_text()
	{
		clear_text();
		dialogue_lines.push_back("Hey! This is some sample dialogue!");
		dialogue_lines.push_back(".....................................................");
		dialogue_lines.push_back(".....................................................");
		
	}

	void dialogue_scene::load_next_paragraph()
	{
		clear_text();
	}

	void dialogue_scene::clear_text()
	{
		text_sprites.clear();
		dialogue_lines.clear();
	}

	void dialogue_scene::write_text()
	{
		static bn::vector<bn::string<64>, DIALOGUE_LINE_CT> working_lines;
		working_lines.clear();
		int line_idx = 0;
		int char_idx = 0;
		text_sprites.clear();
		working_lines.push_back("");
		while(line_idx < dialogue_lines.size())
		{
			if((bn::keypad::a_pressed() || bn::keypad::b_pressed()) && (char_idx > 0 || line_idx > 0))
			{
				text_sprites.clear();
				working_lines.clear();
				for(int i = 0; i < dialogue_lines.size(); i++)
				{
					working_lines.push_back(dialogue_lines.at(i));
					text_generator.generate(line_pos.at(i), working_lines.at(i), text_sprites);
				}
				break;
			}
			
			if(dialogue_lines.at(line_idx).length() > 0)
			{
				working_lines.at(line_idx).append(dialogue_lines.at(line_idx).at(char_idx));
			}

			for(int i = 0; i < line_idx; i++)
			{
				text_generator.generate(line_pos.at(i), dialogue_lines.at(i), text_sprites);
			}
			

			text_sprites.clear();
			for(int i = 0; i <= line_idx; i++)
			{
				text_generator.generate(line_pos.at(i), working_lines.at(i), text_sprites);
			}

			char_idx++;
			if(char_idx >= dialogue_lines.at(line_idx).length())
			{
				working_lines.at(line_idx) = dialogue_lines.at(line_idx);
				line_idx++;
				char_idx = 0;
				if(line_idx < dialogue_lines.size())
				{
					working_lines.push_back("");
				}
			}

			update();
		}

	}

}