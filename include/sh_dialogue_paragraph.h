#ifndef SH_DIALOGUE_PARAGRAPH_H
#define SH_DIALOGUE_PARAGRAPH_H

#include "sh_character.h"

#include <bn_string.h>
#include <bn_vector.h>

namespace sh
{
	#define DIALOGUE_LINE_CT 3
	
	struct dialogue_paragraph
	{
		sh::character_id speaker_id;
		bn::vector<bn::string<64>, DIALOGUE_LINE_CT> text;

		dialogue_paragraph *next;

		dialogue_paragraph();
		dialogue_paragraph(character_id c_id, bn::string<64> line1);
		dialogue_paragraph(character_id c_id, bn::string<64> line1, bn::string<64> line2);
		dialogue_paragraph(character_id c_id, bn::string<64> line1, bn::string<64> line2, bn::string<64> line3);
		~dialogue_paragraph();

	};
	
} // namespace sh


#endif //SH_DIALOGUE_PARAGRAPH_H