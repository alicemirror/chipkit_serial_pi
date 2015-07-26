/**
 \file MessageStrings.h
 \brief Message strings of the controller, used to manage the audio speeches

 The strings defined in this header are the short sentences and phrases that will be
 played by the RPIslave3 device on the speakers as voice messages.
 When the program is launched with the '-v' option instead of running the controller
 loop it creates the wav files for every defined string saved in a folder that can
 be accessed by the remote player via a NFS shared folder.
 */

#include <iomanip>
#include <locale>
#include <sstream>
#include <string.h>

#ifndef MESSAGE_STRINGS_H
#define	MESSAGE_STRINGS_H

#define SSH_PATH "/home/pi/remote_message.sh"
#define SSH_COMMAND "remote_message.sh"

//! The audio files folder, under the pi home folder
#define TTS_FOLDER	"/home/pi/tts_audio_messages/"

//! Shell script to generate the wav file
#define TTS_SHELL_COMMAND "create_ttsText.sh"
//! Shell script path
#define TTS_SHELL_PATH "/home/pi/create_ttsText.sh"

//! The audio format (audio file extension)
#define TTS_FORMAT "meditech"

//! The max number of message strings
#define TTS_MAX_MESSAGES 27

//! The shell command string length (max)
#define MAX_SHELL_CMD_LEN 1024

//! Wrong parameter error message
#define MAINEXIT_WRONGPARAM "\n\nWrong parameter.\n"
//! Wrong number of parameters error message.
#define MAINEXIT_WRONGNUMPARAM "\n\nWrong number of parameters.\n"
//! TTS process completion message
#define MAINEXIT_DONE "\n\n*** TTS completed ***\n"

//! TTS process start message
#define TTS_START_PROCESS "\n*** TTS Creation started. Please wait ***\n"

//! Error message when spawning the process to start festival
#define TTS_SPAWN_ERROR "\n*** ERROR Spawining the main process ***\n"

// Strings array IDs
#define TTS_SYSTEM_RESTARTED 0
#define TTS_POWER_OFF 1
#define TTS_SHUTDOWN 2
#define TTS_VOICE_ACTIVE 3
#define TTS_MUTED 4
#define TTS_STETHOSCOPE_OFF 5
#define TTS_STETHOSCOPE_RUNNING 6
#define TTS_STETHOSCOPE_ON 7
#define TTS_BLOOD_PRESSURE_OFF 8
#define TTS_BLOOD_PRESSURE_RUNNING 9
#define TTS_BLOOD_PRESSURE_ON 10
#define TTS_HEARTBEAT_OFF 11
#define TTS_HEARTBEAT_RUNNING 12
#define TTS_HEATBEAT_ON 13
#define TTS_TEMPERATURE_OFF 14
#define TTS_TEMPERATURE_RUNNING 15
#define TTS_TEMPERATURE_ON 16
#define TTS_ECG_OFF 17
#define TTS_ECG_RUNNING 18
#define TTS_ECG_ON 19
#define TTS_INFORMATION 20
#define TTS_TESTING 21
#define TTS_TESTING_END 22
#define TTS_SYSTEM_READY 23
#define TTS_START_PROBE 24
#define TTS_PROBE_STOPPED 25
#define TTS_CONTINUOUS_ON 26

#endif	/* MESSAGE_STRINGS_H */

