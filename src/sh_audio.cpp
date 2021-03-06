#include "sh_audio.h"

#include <bn_audio.h>
#include <bn_music.h>
#include <bn_music_items.h>
#include <bn_sound_items.h>
#include <bn_fixed.h>

namespace sh
{
	namespace audio
	{
		static bn::fixed _music_volume_global_multiplier_ = 1;
		static bn::fixed _music_volume_song_modifier_ = 1;

		static bn::fixed _sfx_volume_global_multiplier_ = 1;

		void set_music_volume(bn::fixed volume)
		{
			_music_volume_global_multiplier_ = volume;
			if(bn::music::playing())
			{
				bn::music::set_volume(_music_volume_global_multiplier_ * _music_volume_song_modifier_);
			}
		}

		void set_sfx_volume(bn::fixed volume)
		{
			_sfx_volume_global_multiplier_ = volume;
		}
		
		void play_track(track_id track)
		{
			stop_current_track();
			switch (track)
			{
			case track_id::TITLE:
				_music_volume_song_modifier_ = 0.5;
				bn::music_items::icemoon.play(_music_volume_global_multiplier_ * _music_volume_song_modifier_, true);
				break;
			case track_id::BATTLE_BGM:
				_music_volume_song_modifier_ = 0.24;
				bn::music_items::jk_valium.play(_music_volume_global_multiplier_ * _music_volume_song_modifier_, true);
				break;
			case track_id::BATTLE_WIN:
				_music_volume_song_modifier_ = 0.8;
				stop_current_track();
				bn::sound_items::victory.play(_music_volume_global_multiplier_ * _music_volume_song_modifier_);
				break;
			case track_id::BATTLE_LOSE:
				_music_volume_song_modifier_ = 0.8;
				stop_current_track();
				bn::sound_items::failure.play(_music_volume_global_multiplier_ * _music_volume_song_modifier_);
				break;
			
			default:
				break;
			}
		}

		void stop_current_track()
		{
			if(bn::music::playing())
			{
				bn::music::stop();
			}
		}
		
		void play_sound(sound_id sound)
		{
			bn::fixed vol = _sfx_volume_global_multiplier_;
			switch (sound)
			{
			case sound_id::BLIP_HIGH:
				bn::sound_items::blip_high.play(vol);
				break;
			case sound_id::BLIP_LOW:
				bn::sound_items::blip_low.play(vol);
				break;
			case sound_id::BLIP_SOFT:
				bn::sound_items::blip_soft.play(vol);
				break;
			case sound_id::UNSELECTABLE:
				bn::sound_items::unselectable.play(vol);
				break;
			case sound_id::WEW:
				bn::sound_items::wew.play(vol);
				break;
			case sound_id::POP_1:
				bn::sound_items::pop_1.play(vol);
				break;
			case sound_id::POP_2:
				bn::sound_items::pop_2.play(vol);
				break;
			case sound_id::POP_3:
				bn::sound_items::pop_3.play(vol);
				break;
			case sound_id::POP_HIGH:
				bn::sound_items::pop_high.play(vol);
				break;
			
			default:
				break;
			}
		}

	}
}