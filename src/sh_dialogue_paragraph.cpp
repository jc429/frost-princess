#include "sh_dialogue_paragraph.h"

namespace sh
{

	dialogue_paragraph::dialogue_paragraph()
	{
		next = NULL;
		text.clear();
	}

	dialogue_paragraph::dialogue_paragraph(character_id c_id, 
			bn::string<64> line1)
	{
		next = NULL;
		text.clear();
		speaker_id = c_id;
		text.push_back(line1);
	}

	dialogue_paragraph::dialogue_paragraph(character_id c_id, 
			bn::string<64> line1,
			bn::string<64> line2)
	{
		next = NULL;
		text.clear();
		speaker_id = c_id;
		text.push_back(line1);
		text.push_back(line2);
	}

	dialogue_paragraph::dialogue_paragraph(character_id c_id, 
			bn::string<64> line1,
			bn::string<64> line2,
			bn::string<64> line3)
	{
		next = NULL;
		text.clear();
		speaker_id = c_id;
		text.push_back(line1);
		text.push_back(line2);
		text.push_back(line3);
	}




	dialogue_paragraph::~dialogue_paragraph()
	{
		text.clear();
	}

}