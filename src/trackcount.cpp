#include <functional> // std::all_of()
#include <set>

#include <midifile/MidiFile.h>

#include "trackcount.h"

using namespace smf; // for MidiFile



int trackcount(std::string midi){
    // Get the track count.
    MidiFile midifile;
    midifile.read(midi);

    int track_count = midifile.getTrackCount();
    if(midifile.getTrackCount() == 1){
        // create an instrument list
        std::vector<int> instruments;
        for(int event = 0; event < midifile[0].getSize(); event++){
            if(midifile[0][event].isTimbre()){
                int instrument = (int)midifile[0][event].getChannelNibble();
                instruments.emplace_back(instrument);
            }
        }
        std::set<int> instrument_set(instruments.begin(), instruments.end()); 
        track_count = instrument_set.size();
    }
    else{
        // Decrement the track count if there is a track whose events are all meta.
        for(int track = 0; track < track_count; track++){
            std::vector<int> ismetas;
            for(int event = 0; event < midifile[track].getSize(); event++){
                ismetas.emplace_back(midifile[track][event].isMeta());
            }
            if(std::all_of(ismetas.begin(), ismetas.end(), [](bool v){return v;})){
                track_count--;
            }
        }
    }
    return track_count;
}
