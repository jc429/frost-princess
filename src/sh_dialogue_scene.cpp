#include "sh_dialogue_scene.h"
#include "sh_menu.h"
#include "sh_dialogue_paragraph.h"

#include <bn_core.h>
#include <bn_keypad.h>

#include "variable_8x16_sprite_font.h"
#include "bn_regular_bg_items_options_bg.h"
#include "bn_regular_bg_items_dialogue_fg.h"
#include "bn_regular_bg_items_dialogue_spk_box.h"

namespace sh
{
	static const bn::fixed_point char_pos_left_default(-56,24);
	static const bn::fixed_point char_pos_right_default(56,24);
	static const bn::fixed_point speaker_pos_left_default(-56,20);
	static const bn::fixed_point speaker_pos_right_default(56,20);

	dialogue_scene::dialogue_scene() :
		text_generator(common::variable_8x16_sprite_font)
	{
		current_paragraph = NULL;
		// dialogue_lines.clear();
		backgrounds.clear();
		character_sprites.clear();
		bn::regular_bg_ptr bg = bn::regular_bg_items::options_bg.create_bg(0, 0);
		bg.set_priority(3);
		backgrounds.push_back(bg);
		bg = bn::regular_bg_items::dialogue_fg.create_bg(0, 0);
		bg.set_priority(1);
		backgrounds.push_back(bg);
		bg = bn::regular_bg_items::dialogue_spk_box.create_bg(0, 0);
		bg.set_priority(1);
		backgrounds.push_back(bg);
		backgrounds.back().set_visible(false);

		text_generator.set_left_alignment();
		text_generator.set_bg_priority(0);

		{
			bn::fixed_point pos(-112, 36);
			line_pos.push_back(pos);
			pos = pos + bn::fixed_point(0,16);
			line_pos.push_back(pos);
			pos = pos + bn::fixed_point(0,16);
			line_pos.push_back(pos);
		}


		load_dialogue(dialogue_id::TEST);
		fade_from_black();
		scene_done = false;
		write_text();
		while(!scene_done)
		{
			if(bn::keypad::a_pressed() || bn::keypad::b_pressed())
			{
				if(current_paragraph->next != NULL)
				{
					load_paragraph(current_paragraph->next);
					write_text();
				}
				else
				{
					current_paragraph = NULL;
				}
			}

			if(current_paragraph == NULL)
			{
				scene_management::exit_current_scene(scene_type::TITLE);
			}

			update();
		}
		fade_to_black();
	}


	dialogue_scene::~dialogue_scene()
	{
		backgrounds.clear();
		character_sprites.clear();
		text_sprites.clear();
		line_pos.clear();
		speaker_sprites.clear();
	}



	void dialogue_scene::update()
	{
		backgrounds.front().set_position(backgrounds.front().position() + bn::fixed_point(0.25,0.25));

		if(bn::keypad::start_pressed())
		{
			open_pause_menu();
		}

		for(auto it = character_sprites.begin(), end = character_sprites.end(); it != end; ++it)
		{
			it->update();
		}

		bn::core::update();
	}

	void dialogue_scene::set_speaker_name(character_id char_id, dia_spkr_pos side)
	{
		clear_speaker_name();
		bn::fixed_point pos = (side == dia_spkr_pos::LEFT) ? speaker_pos_left_default : speaker_pos_right_default;
		backgrounds.back().set_position(pos);
		backgrounds.back().set_visible(true);
		text_generator.set_center_alignment();
		text_generator.generate(pos, characters::get_character_name(char_id), speaker_sprites);
	}

	void dialogue_scene::clear_speaker_name()
	{
		speaker_sprites.clear();
		backgrounds.back().set_visible(false);
	}

	void dialogue_scene::load_dialogue(dialogue_id dia_id)
	{
		clear_text();
		character_sprites.clear();
		load_paragraph(dialogue::load_dialogue(dialogue_id::TEST));

		character_sprites.push_back(character_sprite(dialogue::get_speaker(dia_spkr_pos::LEFT), char_sprite_type::BUST));
		character_sprites.front().set_position(char_pos_left_default);
		character_sprites.push_back(character_sprite(dialogue::get_speaker(dia_spkr_pos::RIGHT), char_sprite_type::BUST));
		character_sprites.back().set_position(char_pos_right_default);


	}

	void dialogue_scene::load_paragraph(dialogue_paragraph *paragraph)
	{
		clear_text();
		current_paragraph = paragraph;
		if(paragraph == NULL)
		{
			return;
		}
		character_id char_id = paragraph->speaker_id;
		set_speaker_name(char_id, dialogue::get_speaker_side(char_id));
		// for(auto it = paragraph->text.begin(), end = paragraph->text.end(); it != end; ++it)
		// {
		// 	dialogue_lines.push_back(*it);
		// }
	}


	void dialogue_scene::clear_text()
	{
		text_sprites.clear();
		current_paragraph = NULL;
	}

	void dialogue_scene::write_text()
	{
		static bn::vector<bn::string<64>, DIALOGUE_LINE_CT> working_lines;
		working_lines.clear();
		int line_idx = 0;
		int char_idx = 0;
		text_sprites.clear();
		if(current_paragraph == NULL)
		{
			return;
		}
		working_lines.push_back("");
		text_generator.set_left_alignment();
		while(line_idx < current_paragraph->text.size())
		{
			if((bn::keypad::a_pressed() || bn::keypad::b_pressed()) && (char_idx > 0 || line_idx > 0))
			{
				text_sprites.clear();
				working_lines.clear();
				for(int i = 0; i < current_paragraph->text.size(); i++)
				{
					working_lines.push_back(current_paragraph->text.at(i));
					text_generator.generate(line_pos.at(i), working_lines.at(i), text_sprites);
				}
				break;
			}
			
			if(current_paragraph->text.at(line_idx).length() > 0)
			{
				working_lines.at(line_idx).append(current_paragraph->text.at(line_idx).at(char_idx));
			}

			for(int i = 0; i < line_idx; i++)
			{
				text_generator.generate(line_pos.at(i), current_paragraph->text.at(i), text_sprites);
			}
			

			text_sprites.clear();
			for(int i = 0; i <= line_idx; i++)
			{
				text_generator.generate(line_pos.at(i), working_lines.at(i), text_sprites);
			}

			char_idx++;
			if(char_idx >= current_paragraph->text.at(line_idx).length())
			{
				working_lines.at(line_idx) = current_paragraph->text.at(line_idx);
				line_idx++;
				char_idx = 0;
				if(line_idx < current_paragraph->text.size())
				{
					working_lines.push_back("");
				}
			}

			update();
		}

	}


	void dialogue_scene::open_pause_menu()
	{
		text_sprites.clear();

		menu pause_menu(menu_type::PAUSE_MENU, 0);
		pause_menu.open_menu();
		
		while(pause_menu.is_open())
		{
			pause_menu.update();
			bn::core::update();
		}

	}

}