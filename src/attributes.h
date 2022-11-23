#include <map>



struct Attributes{
    std::vector<float> offsets, scales, starts_ends, colors;
    float end_time;
    int track_count, note_count;
    std::vector<std::map<std::string, float>> fountain_list;

    Attributes(
        std::string midi_file,
        float speed, float radius, float horizontal_interval, float vertical_interval, float before_time,
        bool reverse, bool color_shuffle, bool lane_shuffle, bool is_td,
        std::vector<std::vector<float>> colors_parameter);
    ~Attributes(){};
};
