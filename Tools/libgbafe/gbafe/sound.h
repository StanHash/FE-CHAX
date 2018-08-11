#ifndef GBAFE_SOUND_H
#define GBAFE_SOUND_H

#include "common.h"
#include "chapterdata.h"

#pragma long_calls

// TODO
// SET_ABS_FUNC Sound_GetCurrentSong, 0x8002259
// SET_ABS_FUNC Sound_IsSongPlaying, 0x8002265
// SET_ABS_FUNC Sound_SetVolume_8002274, 0x8002275
// SET_ABS_FUNC Sound_SetSongVolume, 0x80022ED
// SET_ABS_FUNC Sound_FadeSongOut, 0x800231D
// SET_ABS_FUNC Sound_FadeOut_800237C, 0x800237D
// SET_ABS_FUNC Sound_FadeOut_80023E0, 0x80023E1
// SET_ABS_FUNC Sound_PlaySong, 0x8002449
// SET_ABS_FUNC Sound_PlaySongSmoothCommon, 0x8002479
// SET_ABS_FUNC Sound_PlaySongSmooth, 0x80024D5
// SET_ABS_FUNC Sound_PlaySongSmoothExt, 0x80024E5
// SET_ABS_FUNC Sound_SongVolumeTransitionAProc_Loop, 0x80024F1
// SET_ABS_FUNC StartSongVolumeTransitionB, 0x8002731
// SET_ABS_FUNC Sound_SongVolumeTransitionBProc_Loop, 0x8002789
// SET_ABS_FUNC Sound_PlaySoundDelayedProc_Loop, 0x800281D
// SET_ABS_FUNC PlaySongDelayed, 0x8002859
// SET_ABS_FUNC Sound_PlaySongCore, 0x8002891
// SET_ABS_FUNC SetSoundDefaultMaxChannels, 0x80028D1
// SET_ABS_FUNC SetSoundMaxChannels, 0x80028E9
// SET_ABS_FUNC Sound_SetupMaxChannelsForSong, 0x80028FD
// SET_ABS_FUNC CancelPlaySongDelayed, 0x8002AB9

// TODO: m4a header
void m4aSongNumStart(int);

#pragma long_calls_off

#define PlaySfx(aSongId) do { \
	if (!gChapterData.muteSfxOption) \
		m4aSongNumStart(aSongId); \
} while (0)

#endif // GBAFE_SOUND_H
