#include <string>
#include <vector>

struct Td_Parameters{
        std::string midi, soundfont, wallpaper;
        bool fullscreen, mute, reverse, color_shuffle, lane_shuffle;
        float speed, radius, opacity, camera_x, camera_y, camera_z;
        int window_width, window_height, ngon, argc;
        char** argv;
        std::vector<float> wall_color;
        std::vector<std::vector<float>> color;
};



void td(Td_Parameters parameters);

