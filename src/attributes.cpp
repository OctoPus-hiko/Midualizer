#include <algorithm> // for std::shuffle() and std::max_element
#include <random>
#include <set>

#include "attributes.h"
#include "notes.h"
#include "trackcount.h"

using std::map;
using std::pair;
using std::string;
using std::vector;



Attributes::Attributes(
    string midi,
    float speed, float radius, float horizontal_interval, float vertical_interval, float before_time,
    bool reverse, bool color_shuffle, bool lane_shuffle, bool is_td,
    vector<vector<float>> colors_parameter
){
    const float minimum_brightness = 1.0f;
    const float maximum_brightness = 2.2f;
    const float drum_length = 0.05f;

    extern int color_count;


    int midi_track_count = trackcount(midi);
    vector<Note> midi_notes = notes(midi);


    //// prepare colors to assign to the tracks
    vector<vector<float>> track_colors = colors_parameter;

    // generator of a random float between 0.0 and 1.0
    std::random_device dev;
    std::default_random_engine eng(dev());
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);

    if(midi_track_count > color_count){
        while((int) track_colors.size() < midi_track_count){
            const vector<float> proposed_color = {distr(eng), distr(eng), distr(eng)};
            const float sum = std::accumulate(proposed_color.begin(), proposed_color.end(), 0.0);

            if(sum >= minimum_brightness && sum <= maximum_brightness){
                track_colors.emplace_back(proposed_color);
            }
        }
    }

    if(color_shuffle){std::shuffle(std::begin(track_colors), std::end(track_colors), eng);} // shuffle the track colors



    //// create non-flat vectors of attributes
    vector<vector<float>> nonflat_offsets;
    vector<vector<float>> nonflat_scales;
    vector<vector<float>> nonflat_starts_ends;
    vector<vector<float>> nonflat_colors;
    vector<map<string, float>> a_fountain_list;


    // prepare horizontal lanes to assign to the tracks
    vector<float> lanes;
    for(int i = 0; i < midi_track_count; i++){lanes.emplace_back((float) i);}
    if(lane_shuffle){std::shuffle(std::begin(lanes), std::end(lanes), eng);} // shuffle track lanes


    for(auto note : midi_notes){
        const float start = (float)note.start + before_time;

        float end;
        float length;
        //  every drum note bar has the same physical and temporal lengths
        if(note.is_drum){
            length = drum_length;
            end = start + drum_length / speed;
        }else{
            length = ((float)note.end - (float)note.start) * speed;
            end = (float)note.end + before_time;
        }

        const vector<float> start_end = {start, end};

        float offset_x;
        if(is_td){offset_x = 0.0f;}
        else{offset_x = (lanes[note.track] - (float)midi_track_count / 2.0f) * horizontal_interval;}
        const float offset_y = ((float)note.pitch - 63.0f) * vertical_interval;
        const float offset_z = (reverse) ? start * speed : - start * speed - length;
        const vector<float> offset = {offset_x, offset_y, offset_z};
        const vector<float> scale = {radius, radius, length};
        const vector<float> color = track_colors[note.track];

        nonflat_offsets.emplace_back(offset);
        nonflat_scales.emplace_back(scale);
        nonflat_colors.emplace_back(color);
        nonflat_starts_ends.emplace_back(start_end);

        map<string, float> fountain;
        fountain.insert(pair<string, float>("x", offset_x));
        fountain.insert(pair<string, float>("y", offset_y));
        fountain.insert(pair<string, float>("start", start));
        fountain.insert(pair<string, float>("end", end));
        fountain.insert(pair<string, float>("r", color[0]));
        fountain.insert(pair<string, float>("g", color[1]));
        fountain.insert(pair<string, float>("b", color[2]));
        a_fountain_list.emplace_back(fountain);
    }





    //// flatten the non-flat vectors
    vector<float> flat_offsets;
    for(auto offset : nonflat_offsets){
        flat_offsets.emplace_back(offset[0]);
        flat_offsets.emplace_back(offset[1]);
        flat_offsets.emplace_back(offset[2]);
    }

    vector<float> flat_scales;
    for(auto scale : nonflat_scales){
        flat_scales.emplace_back(scale[0]);
        flat_scales.emplace_back(scale[1]);
        flat_scales.emplace_back(scale[2]);
    }

    vector<float> flat_colors;
    for(auto color : nonflat_colors){
        flat_colors.emplace_back(color[0]);
        flat_colors.emplace_back(color[1]);
        flat_colors.emplace_back(color[2]);
    }

    vector<float> flat_starts_ends;
    for(auto start_end : nonflat_starts_ends){
        flat_starts_ends.emplace_back(start_end[0]);
        flat_starts_ends.emplace_back(start_end[1]);
    }


    // assign values to struct members
    vector<float> ends;
    for(auto note : midi_notes){ends.emplace_back(note.end);}
    auto max = *std::max_element(std::begin(ends), std::end(ends));

    end_time = max;
    offsets = flat_offsets;
    scales = flat_scales;
    starts_ends = flat_starts_ends;
    colors = flat_colors;
    track_count = midi_track_count;
    note_count = (int) midi_notes.size();
    fountain_list = a_fountain_list;
}
