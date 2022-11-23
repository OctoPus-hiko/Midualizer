#include <filesystem> // for std::filesystem::exists, std::filesystem::create_directories

#include <fluidsynth.h>

#include "generate_wav.h"

using std::string;



void generate_wav(string midi_file, string soundfont, string directory){
        if(!std::filesystem::exists(directory)){std::filesystem::create_directories(directory);}
        string temporary_file = directory + "/temp.wav";



        fluid_settings_t* settings = new_fluid_settings();

        // specify the file to store the audio to
        // make sure you compiled fluidsynth with libsndfile to get a real wave file
        // otherwise this file will only contain raw s16 stereo PCM
        fluid_settings_setstr(settings, "audio.file.name", temporary_file.c_str());
        
        // use number of samples processed as timing source, rather than the system timer
        fluid_settings_setstr(settings, "player.timing-source", "sample");

        // since this is a non-realtime scenario, there is no need to pin the sample data
        fluid_settings_setint(settings, "synth.lock-memory", 0);



        fluid_synth_t* synth = new_fluid_synth(settings);
        fluid_synth_sfload(synth, soundfont.c_str(), 1);



        fluid_player_t* player = new_fluid_player(synth);
        fluid_player_add(player, midi_file.c_str());
        fluid_player_play(player);



        fluid_file_renderer_t* renderer = new_fluid_file_renderer(synth);



        while(fluid_player_get_status(player) == FLUID_PLAYER_PLAYING){
            if(fluid_file_renderer_process_block(renderer) != FLUID_OK){
                break;
            }
        }
        // just for sure: stop the playback explicitly and wait until finished
        fluid_player_stop(player);
        fluid_player_join(player);



        delete_fluid_file_renderer(renderer);
        delete_fluid_player(player);
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
}
