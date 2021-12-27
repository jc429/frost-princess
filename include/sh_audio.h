#ifndef SH_AUDIO_H
#define SH_AUDIO_H

#include <bn_audio.h>
#include <bn_music.h>
#include <bn_music_items.h>
#include <bn_sound_items.h>
#include <bn_fixed.h>

namespace sh
{
	enum class track_id
	{
		TITLE,
		BATTLE_BGM
	};

	enum class sound_id
	{
		BLIP_HIGH,
		BLIP_LOW,
		BLIP_SOFT,
		UNSELECTABLE,
		WEWEWEW,
	};

	namespace audio
	{
		void set_volume(bn::fixed volume);
		void play_track(track_id track);
		void stop_current_track();
		void play_sound(sound_id sound);

	}
}


#endif //SH_AUDIO_H