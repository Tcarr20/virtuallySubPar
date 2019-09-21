/// \file SndList.h
/// \brief Enumerated type for sounds.

#pragma once

#include "Sound.h"

/// \brief Game sound enumerated type. 
///
/// These are the sounds used in actual gameplay. 
/// The sounds must be listed here in the same order that
/// they are in the sound settings XML file.

enum eSoundType{ 
  DOSERCRY, BOSSHIT, DOSERHIT, BALLBOUNCE, BUGBUZZ, BOSSLOSE, CLANG_SOUND, BOSSSTART, DOSERDIE, BUGDIE, LEONEDIE,
  UMPH_SOUND, FORTHECOURSE, HOLEINONE, SINKBALL,
  SWINGCLUB, LEONEMEOW, LEONESPIN, UNDERPAR, SHOOTSNOT,
  WINLEVEL, WINGAME, LOSELEVEL,BOSSCHUNK
}; //eSoundType