#include <FL/Fl_Color_Chooser.H>



class TrackColor : public Fl_Button{
        Fl_Window* color_window;
        Fl_Color_Chooser* color_chooser;

        void ok_callback(Fl_Widget* w);
        static void static_ok_callback(Fl_Widget* w, void* data){((TrackColor*) data)->ok_callback(w);}
        void cancel_callback(Fl_Widget* w);
        static void static_cancel_callback(Fl_Widget* w, void* data){((TrackColor*) data)->cancel_callback(w);}
        void trackcolor_callback(Fl_Widget* w);
        static void static_trackcolor_callback(Fl_Widget* w, void* data){((TrackColor*) data)->trackcolor_callback(w);}

    public:
        TrackColor(int x, int y, int w, int h, const char* label);
        ~TrackColor(){};
};
