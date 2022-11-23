#include <FL/Fl.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Window.H>

#include "trackcolor.h"



class GUI : public Fl_Window{
        Fl_Tabs* tabs;
        Fl_Group* main_group;
        Fl_Group* advanced_group;
        Fl_Group* color_group;
        Fl_Button* midi_button;
        Fl_Box* midi_box;
        Fl_Check_Button* td_check;
        Fl_Check_Button* fullscreen_check;
        Fl_Check_Button* mute_check;
        Fl_Check_Button* reverse_check;
        Fl_Check_Button* color_shuffle_check;
        Fl_Check_Button* lane_shuffle_check;
        Fl_Box* wall_box;
        TrackColor* wall_trackcolor;
        Fl_Box* speed_box;
        Fl_Value_Slider* speed_vslider;
        Fl_Box* radius_box;
        Fl_Value_Slider* radius_vslider;
        Fl_Box* size_box1;
        Fl_Input* size_input1;
        Fl_Box* size_box2;
        Fl_Input* size_input2;
        Fl_Button* wallpaper_button;
        Fl_Box* wallpaper_box;
        Fl_Button* soundfont_button;
        Fl_Value_Slider* opacity_vslider;
        Fl_Box* opacity_box;
        Fl_Box* soundfont_box;
        Fl_Box* camera_box;
        Fl_Box* camera_x_box;
        Fl_Input* camera_x_input;
        Fl_Box* camera_y_box;
        Fl_Input* camera_y_input;
        Fl_Box* camera_z_box;
        Fl_Input* camera_z_input;
        Fl_Box* ngon_box;
        Fl_Value_Slider* ngon_vslider;
        Fl_Box* ngon_box2;
        Fl_Return_Button* go_rbutton;
        Fl_Button* cancel_button;
        Fl_Return_Button* go_rbutton2;
        Fl_Button* cancel_button2;
        Fl_Return_Button* go_rbutton3;
        Fl_Button* cancel_button3;

        bool is_changed = false;

        std::vector <TrackColor*> trackcolors;
        int label_max;

        void midi_callback(Fl_Widget* w);
        static void static_midi_callback(Fl_Widget* w, void* data){((GUI*) data)->midi_callback(w);};

        void td_callback(Fl_Widget* w);
        static void static_td_callback(Fl_Widget* w, void* data){((GUI*) data)->td_callback(w);};

        void soundfont_callback(Fl_Widget* w);
        static void static_soundfont_callback(Fl_Widget* w, void* data){((GUI*) data)->soundfont_callback(w);};

        void wallpaper_callback(Fl_Widget* w);
        static void static_wallpaper_callback(Fl_Widget* w, void* data){((GUI*) data)->wallpaper_callback(w);};

        void go_callback(Fl_Widget* w);
        static void static_go_callback(Fl_Widget *w, void* data){((GUI*) data)->go_callback(w);};

        void cancel_callback(Fl_Widget* w);
        static void static_cancel_callback(Fl_Widget *w, void* data){((GUI*) data)->cancel_callback(w);};

        void ngon_callback(Fl_Widget* w);
        static void static_ngon_callback(Fl_Widget *w, void* data){((GUI*) data)->ngon_callback(w);};



    public:
        std::string soundfont = "";
        std::string midi = "";
        std::string wallpaper = "";
        bool td;
        bool fullscreen;
        bool mute;
        bool reverse;
        bool color_shuffle;
        bool lane_shuffle;
        float speed;
        float radius;
        float opacity;
        float camera_x;
        float camera_y;
        float camera_z;
        int window_width;
        int window_height;
        int ngon;
        std::vector<float> wall_color;
        std::vector<std::vector<float>> color;

        GUI(int width, int height, const char* title);

        ~GUI(){};
};
