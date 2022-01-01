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

		void set_volume(bn::fixed volume)
		{
			_music_volume_global_multiplier_ = volume;
			bn::music::set_volume(_music_volume_global_multiplier_ * _music_volume_song_modifier_);
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
			switch (sound)
			{
			case sound_id::BLIP_HIGH:
				bn::sound_items::blip_high.play();
				break;
			case sound_id::BLIP_LOW:
				bn::sound_items::blip_low.play();
				break;
			case sound_id::BLIP_SOFT:
				bn::sound_items::blip_soft.play();
				break;
			case sound_id::UNSELECTABLE:
				bn::sound_items::unselectable.play(1);
				break;
			case sound_id::WEWEWEW:
				bn::sound_items::wewewew.play(1);
				break;
			
			default:
				break;
			}
		}

	}
}