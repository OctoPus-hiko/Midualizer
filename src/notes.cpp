#include <algorithm> // for std::all_of
#include <iostream>
#include <map>
#include <set>

#include <midifile/MidiFile.h>

#include "notes.h"

using namespace smf; // for MidiFile

using std::map;
using std::pair;
using std::string;
using std::vector;



vector<Note> notes(string midi){
    MidiFile midifile;
    midifile.read(midi);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();



    //// create a list of midi events of all the notes, each represented by a Note struct
    int midi_track_count = midifile.getTrackCount();
    vector<Note> notes;

    int updated_track = 0;
    if(midi_track_count > 1){
        for(int track = 0; track < midi_track_count; track++){
            // Decrement the track count if there is a track whose events are all meta.
            vector<int> ismetas;
            for(int event = 0; event < midifile[track].getSize(); event++){
                ismetas.emplace_back(midifile[track][event].isMeta());
            }
            if(std::all_of(ismetas.begin(), ismetas.end(), [](bool v){return v;})){
                updated_track--;
                midi_track_count--;
            }

            for(int event = 0; event < midifile[track].getSize(); event++){
                // Add the event to the list only when it is a NoteOn event
                if(midifile[track][event].isNoteOn()){
                    const float start = midifile.getTimeInSeconds(midifile[track][event].tick);
                    const float end = start + midifile[track][event].getDurationInSeconds();
                    const int pitch = midifile[track][event].getP1(); // probably, midi_event[1] and midi_event.getP1() return the same thing (pitch)

                    // set is_drum to true when the channel (nibble) is 9
                    const int channel = midifile[track][event].getChannelNibble();
                    const bool is_drum = (channel == 9) ? true : false;

                    const Note note(start, end, pitch, updated_track, is_drum);
                    notes.emplace_back(note);
                }
            }
            updated_track++;
        }
    }

    else{
        // create an instrument list
        vector<int> instruments;
        for(int event = 0; event < midifile[0].getSize(); event++){
            if(midifile[0][event].isTimbre()){
                const int instrument = midifile[0][event].getChannelNibble();
                instruments.emplace_back(instrument);
            }
        }
        const std::set<int> instrument_set(instruments.begin(), instruments.end()); 
        midi_track_count = (int) instrument_set.size();

        // map the instruments to track numbers beginning with 0
        map<int, int> instrument_map;
        int i = 0;
        for(auto instrument : instrument_set){
            instrument_map.insert(pair<int, int>(instrument, i));
            i++;
        }

        for(int event = 0; event < midifile[0].getSize(); event++){
            if(midifile[0][event].isNoteOn()){
                // assign a channel (= instrument) to the event
                int track = -1;
                for(auto instrument : instrument_set){
                    if((int) midifile[0][event].getChannelNibble() == instrument){
                        track = instrument_map[instrument];
                    }
                }
                if(track == -1){
                    std::cout << "Failed to parse tracks." << std::endl;
                    exit(0);
                }

                const float start = midifile.getTimeInSeconds(midifile[0][event].tick);
                const float end = start + midifile[0][event].getDurationInSeconds();
                const int pitch = midifile[0][event].getP1();

                const int channel = midifile[0][event].getChannelNibble();
                const bool is_drum = (channel == 9) ? true : false;

                const Note note(start, end, pitch, track, is_drum);
                notes.emplace_back(note);
            }
        }
    }

    return notes;
}
