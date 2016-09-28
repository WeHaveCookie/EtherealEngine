#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_HiMomImBack,
  Msg_WantToPlay,
  Msg_StopPlaying,
  Msg_MomArrive
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";
	
  case Msg_HiMomImBack:
	  return "HiMomImBack";

  case Msg_WantToPlay:
	  return "WanToPlay";

  case Msg_StopPlaying:
	  return "StopPlaying";

  case Msg_MomArrive:
	  return "MomArrive";

  default:

    return "Not recognized!";
  }
}

#endif