#ifndef SH_DIALOGUE_H
#define SH_DIALOGUE_H

#include "sh_dialogue_paragraph.h"

#include <bn_string.h>
#include <bn_vector.h>

namespace sh
{

	enum dialogue_id
	{
		TEST,
		PRE_BTL_HOMURA,
	};

	enum dia_spkr_pos
	{
		LEFT,
		RIGHT,
		NONE
	};

	namespace dialogue
	{
		dia_spkr_pos get_speaker_side(character_id char_id);
		character_id get_speaker(dia_spkr_pos side);

		void clear_loaded_dialogue();
		dialogue_paragraph *load_dialogue(dialogue_id d_id);
	}

}

#endif //SH_DIALOGUE_H