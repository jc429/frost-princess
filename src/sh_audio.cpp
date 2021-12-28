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
		
		void set_volume(bn::fixed volume)
		{
			bn::music::set_volume(volume);
		}
		
		void play_track(track_id track)
		{
			switch (track)
			{
			case track_id::TITLE:
				bn::music_items::icemoon.play(0.5, true);
				break;
			case track_id::BATTLE_BGM:
				bn::music_items::jk_valium.play(0.24, true);
				break;
			
			default:
				break;
			}
		}

		void stop_current_track()
		{
			bn::music::stop();
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