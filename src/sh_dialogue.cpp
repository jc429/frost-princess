#include "sh_dialogue.h"


namespace sh
{

	namespace dialogue
	{
		static bn::vector<dialogue_paragraph, 16> _current_dialogue_;
		static character_id _left_speaker_;
		static character_id _right_speaker_;


		dia_spkr_pos get_speaker_side(character_id char_id)
		{
			if(_left_speaker_ == char_id)
				return dia_spkr_pos::LEFT;
			else if(_right_speaker_ == char_id)
				return dia_spkr_pos::RIGHT;
			else
				return dia_spkr_pos::NONE;
		}

		character_id get_speaker(dia_spkr_pos side)
		{
			switch (side)
			{
			case dia_spkr_pos::LEFT:
				return _left_speaker_;
				break;
			case dia_spkr_pos::RIGHT:
				return _right_speaker_;
				break;
			default:
				return character_id::NONE;
				break;
			}
		}

		void clear_loaded_dialogue()
		{
			_current_dialogue_.clear();
			_left_speaker_ = character_id::NONE;
			_right_speaker_ = character_id::NONE;
		}

		dialogue_paragraph *load_dialogue(dialogue_id d_id)
		{
			clear_loaded_dialogue();
			switch (d_id)
			{
			case dialogue_id::TEST:
				_left_speaker_ = character_id::FROST_PRINCESS;
				_right_speaker_ = character_id::FLAME_WITCH;
				_current_dialogue_.push_back(dialogue_paragraph(
						character_id::FROST_PRINCESS,
						"Hey! This is some sample dialogue!",
						".....................................................",
						"....................................................."
					));
				_current_dialogue_.push_back(dialogue_paragraph(
						character_id::FLAME_WITCH,
						"Wow! Two whole paragraphs!",
						"Localized entirely within your kitchen!"
					));
				
				break;

			case dialogue_id::PRE_BTL_HOMURA:
				_left_speaker_ = character_id::PROTAGONIST;
				_right_speaker_ = character_id::FLAME_WITCH;
				_current_dialogue_.push_back(dialogue_paragraph(
						character_id::PROTAGONIST,
						"...."
					));
				_current_dialogue_.push_back(dialogue_paragraph(
						character_id::FLAME_WITCH,
						"Well then. Let's get started!"
					));
				break;

			default:
				return NULL;
				break;
			}
			for(int i = 0; i+1 < _current_dialogue_.size(); i++)
			{
				_current_dialogue_.at(i).next = &_current_dialogue_.at(i+1);
			}

			if(_current_dialogue_.empty())
			{
				return NULL;
			}
			else
			{
				return &_current_dialogue_.front();
			}
		}

	}


}