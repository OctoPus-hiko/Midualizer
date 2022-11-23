#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include "trackcolor.h"



void TrackColor::ok_callback(Fl_Widget* w){
    Fl_Color button_color = fl_rgb_color(color_chooser->r() * 255.0, color_chooser->g() * 255.0, color_chooser->b() * 255.0);
    this->color(button_color);
    this->color2(button_color);
    color_window->hide();
}





void TrackColor::cancel_callback(Fl_Widget* w){color_window->hide();}





void TrackColor::trackcolor_callback(Fl_Widget* w){
    const int button_width = 100;
    const int button_height = 30;
    const int window_width = 300;

    const int bbg_color = 0x11111100;
    const int font_color = 0xaaaaaa00;
    const int bg_color = 0x2a2a2a00;
    const int click_color = 0x00333300;


    color_window = new Fl_Window(window_width, 275, "Pick a color");
    color_window->color(bbg_color);

    color_chooser = new Fl_Color_Chooser(0, 0, window_width, 200);

    const int button_margin = (window_width - button_width * 2) / 3;

    Fl_Return_Button* ok_rbutton = new Fl_Return_Button(button_margin, 225, button_width, button_height, "OK");
    ok_rbutton->callback(static_ok_callback, (void*) this);
    ok_rbutton->box(FL_FLAT_BOX);
    ok_rbutton->labelcolor(font_color);
    ok_rbutton->color(bg_color);
    ok_rbutton->color2(click_color);

    Fl_Button* cancel_button = new Fl_Button(button_margin * 2 + button_width, 225, button_width, button_height, "Cancel");
    cancel_button->callback(static_cancel_callback, (void*) this);
    cancel_button->box(FL_FLAT_BOX);
    cancel_button->labelcolor(font_color);
    cancel_button->color(bg_color);
    cancel_button->color2(click_color);

    color_window->show();
}





TrackColor::TrackColor(int x, int y, int w, int h, const char* label) : Fl_Button(x, y, w, h, label){
    const int font_color = 0xaaaaaa00;
    this->callback(static_trackcolor_callback, (void*) this);
    this->labelcolor(font_color);
    this->align(FL_ALIGN_TOP);
    this->box(FL_ROUND_DOWN_BOX);
    this->down_box(FL_ROUND_DOWN_BOX);
};
