#include <filesystem>
#include <random>

#include "gui.h"

using std::string;
using std::vector;



int color_row_count = 6;
int color_column_count = 7;
int color_count = color_row_count * color_column_count;

GUI::GUI(int width, int height, const char* title) : Fl_Window(width, height, title){
    const int gui_width = 500;
    const int gui_height = 600;
    const int widget_width = 200;
    const int widget_height = 30;
    const int margin = 25;
    const int gap = 10;
    const int tab_height = 30;
    const int slider_width = 400;
    const float nob_size = 0.04f;
    const int go_gap = (gui_width - widget_width * 2) / 3;

    const int bbg_color = 0x11111100;
    const int inactive_color = 0x70707000;
    const int font_color = 0xaaaaaa00;
    const int bg_color = 0x2a2a2a00;
    const int input_color = 0xeeeeee00;
    const int text_color = 0x11111100;
    const int click_color = 0x00333300;
    const int check_color = 0x28666600;
    const int select_color = 0x28666600;

    label_max = 28;

    tabs = new Fl_Tabs(0, 0, gui_width, gui_height);
    tabs->selection_color(bbg_color);
    tabs->labelcolor(font_color);



    main_group = new Fl_Group(0, tab_height, gui_width, gui_height - tab_height, "Basic");{

        const int td_position = 110;
        const int mute_position = 150;
        const int fullscreen_position = 190;
        const int reverse_position = 230;
        const int color_shuffle_position = 270;
        const int lane_shuffle_position = 310;
        const int speed_position = 360;
        const int radius_position = 435;

        midi_button = new Fl_Button(margin, margin + tab_height, widget_width, widget_height, "@fileopen  Choose MIDI file");
        midi_button->callback(static_midi_callback, (void*) this);
        midi_button->box(FL_FLAT_BOX);
        midi_button->color(bg_color);
        midi_button->color2(click_color);
        midi_button->labelcolor(font_color);

        midi_box = new Fl_Box(margin + widget_width + gap, margin + tab_height, gui_width - margin - widget_width - gap, widget_height, "No file chosen");
        midi_box->labelcolor(font_color);


        td_check = new Fl_Check_Button(margin * 2, td_position, widget_width, widget_height, "2D mode");
        td_check->callback(static_td_callback, (void*) this);
        td_check->when(FL_WHEN_CHANGED);
        td_check->down_box(FL_GTK_THIN_DOWN_BOX);
        td_check->color2(check_color);
        td_check->labelcolor(font_color);

        wall_box = new Fl_Box(gui_width / 2, td_position, widget_width, widget_height, "3D wall/2D line color");
        wall_box->labelcolor(font_color);

        wall_trackcolor = new TrackColor(gui_width * 2 / 3, td_position + 40, 50, 50, "");
        wall_trackcolor->color(fl_rgb_color(12, 12, 12));

        mute_check = new Fl_Check_Button(margin * 2, mute_position, widget_width, widget_height, "Mute");
        mute_check->down_box(FL_GTK_THIN_DOWN_BOX);
        mute_check->color2(check_color);
        mute_check->labelcolor(font_color);

        fullscreen_check = new Fl_Check_Button(margin * 2, fullscreen_position, widget_width, widget_height, "Fullscreen");
        fullscreen_check->down_box(FL_GTK_THIN_DOWN_BOX);
        fullscreen_check->color2(check_color);
        fullscreen_check->labelcolor(font_color);

        reverse_check = new Fl_Check_Button(margin * 2, reverse_position, widget_width, widget_height, "Reverse the moving direction");
        reverse_check->down_box(FL_GTK_THIN_DOWN_BOX);
        reverse_check->color2(check_color);
        reverse_check->labelcolor(font_color);

        color_shuffle_check = new Fl_Check_Button(margin * 2, color_shuffle_position, widget_width, widget_height, "Shuffle track colors");
        color_shuffle_check->down_box(FL_GTK_THIN_DOWN_BOX);
        color_shuffle_check->color2(check_color);
        color_shuffle_check->labelcolor(font_color);

        lane_shuffle_check = new Fl_Check_Button(margin * 2, lane_shuffle_position, widget_width, widget_height, "Shuffle track lanes");
        lane_shuffle_check->down_box(FL_GTK_THIN_DOWN_BOX);
        lane_shuffle_check->color2(check_color);
        lane_shuffle_check->labelcolor(font_color);

        speed_box = new Fl_Box(0, speed_position, gui_width, widget_height, "Speed");
        speed_box->labelcolor(font_color);

        speed_vslider = new Fl_Value_Slider((gui_width - slider_width) / 2, speed_position + 35, slider_width, widget_height - 6);
        speed_vslider->bounds(1, 10);
        speed_vslider->step(1);
        speed_vslider->value(3);
        speed_vslider->type(FL_HORIZONTAL);
        speed_vslider->slider_size(nob_size);
        speed_vslider->color(bg_color);
        speed_vslider->color2(click_color);
        speed_vslider->textcolor(font_color);
        speed_vslider->textsize(15);

        radius_box = new Fl_Box(0, radius_position, gui_width, widget_height, "Note bar thickness");
        radius_box->labelcolor(font_color);

        radius_vslider = new Fl_Value_Slider((gui_width - slider_width) / 2, radius_position + 35, slider_width, widget_height - 6);
        radius_vslider->bounds(1, 10);
        radius_vslider->step(1);
        radius_vslider->value(3);
        radius_vslider->type(FL_HORIZONTAL);
        radius_vslider->slider_size(nob_size);
        radius_vslider->color(bg_color);
        radius_vslider->color2(click_color);
        radius_vslider->textcolor(font_color);
        radius_vslider->textsize(15);

        go_rbutton = new Fl_Return_Button(go_gap, gui_height - margin - widget_height, widget_width, widget_height, "Go!");
        go_rbutton->callback(static_go_callback, (void*) this);
        go_rbutton->box(FL_FLAT_BOX);
        go_rbutton->color(bg_color);
        go_rbutton->color2(click_color);
        go_rbutton->labelcolor(font_color);

        cancel_button = new Fl_Button(go_gap * 2 + widget_width, gui_height - margin - widget_height, widget_width, widget_height, "Cancel");
        cancel_button->callback(static_cancel_callback, (void*) this);
        cancel_button->box(FL_FLAT_BOX);
        cancel_button->color(bg_color);
        cancel_button->color2(click_color);
        cancel_button->labelcolor(font_color);
    }main_group->end();
    main_group->color(bbg_color);
    main_group->color2(inactive_color);
    main_group->labelcolor(font_color);



    advanced_group = new Fl_Group(0, tab_height, gui_width, gui_height - tab_height, "Advanced");{
        const int camera_position = tab_height + 10;
        const int camera_inputs_position = camera_position + 35;
        const int size_position = tab_height + 110;
        const int size_inputs_position = size_position + 35;
        const int soundfont_position = tab_height + 220;
        const int wallpaper_position = tab_height + 280;
        const int opacity_position = tab_height + 330;
        const int ngon_position = tab_height + 410;

        wallpaper_button = new Fl_Button(margin, wallpaper_position, widget_width, widget_height, "@fileopen  Specify a wallpaper");
        wallpaper_button->callback(static_wallpaper_callback, (void*) this);
        wallpaper_button->box(FL_FLAT_BOX);
        wallpaper_button->color(bg_color);
        wallpaper_button->color2(click_color);
        wallpaper_button->labelcolor(font_color);

        wallpaper_box = new Fl_Box(margin + widget_width + gap, wallpaper_position, gui_width - margin - widget_width - gap, widget_height, "No wallpaper specified");
        wallpaper_box->labelcolor(font_color);

        opacity_vslider = new Fl_Value_Slider((gui_width - slider_width) / 2, opacity_position + 35, slider_width, widget_height - 6);
        opacity_vslider->bounds(0.0f, 100.0f);
        opacity_vslider->step(1.0f);
        opacity_vslider->value(95.0f);
        opacity_vslider->type(FL_HORIZONTAL);
        opacity_vslider->slider_size(nob_size);
        opacity_vslider->color(bg_color);
        opacity_vslider->color2(click_color);
        opacity_vslider->textcolor(font_color);
        opacity_vslider->textsize(15);

        opacity_box = new Fl_Box(0, opacity_position, gui_width, widget_height, "3D mode wall/2D mode line opacity");
        opacity_box->labelcolor(font_color);

        soundfont_button = new Fl_Button(margin, soundfont_position, widget_width, widget_height, "@fileopen  Change soundfont");
        soundfont_button->callback(static_soundfont_callback, (void*) this);
        soundfont_button->box(FL_FLAT_BOX);
        soundfont_button->color(bg_color);
        soundfont_button->color2(click_color);
        soundfont_button->labelcolor(font_color);


        const vector<std::filesystem::path> paths = {
            "/usr/share/soundfonts/default.sf2",
            "/usr/local/share/soundfonts/default.sf2",
            (string) getenv("HOME") + (string) "/.local/share/soundfonts/default.sf2",
            "/usr/share/soundfonts/default.sf2",
            "/usr/share/soundfonts/GeneralUser_GS_v1.471.sf2",
            "/app/share/soundfonts/GeneralUser_GS_v1.471.sf2"
        };

        string soundfont_label = "No soundfont chosen";
        for(auto path : paths){
            if(std::filesystem::exists(path)){
                soundfont = path.string();
                soundfont_label = path.filename().string();
            }
        }


        soundfont_box = new Fl_Box(margin + widget_width + gap, soundfont_position, gui_width - margin - widget_width - gap, widget_height);
        soundfont_box->copy_label(soundfont_label.c_str());
        soundfont_box->labelcolor(font_color);

        camera_box = new Fl_Box(0, camera_position, gui_width, widget_height, "Camera position (-100.0–100.0 for each)");
        camera_box->labelcolor(font_color);

        camera_x_box = new Fl_Box(margin, camera_inputs_position, margin, widget_height, "x:");
        camera_x_box->labelcolor(font_color);

        camera_x_input = new Fl_Input(margin + 25, camera_inputs_position, widget_width / 2, widget_height);
        camera_x_input->value("-7.0");
        camera_x_input->box(FL_GTK_DOWN_BOX);
        camera_x_input->color(input_color);
        camera_x_input->textcolor(text_color);
        camera_x_input->selection_color(select_color);

        camera_y_box = new Fl_Box(margin * 3 + widget_width / 2, camera_inputs_position, margin, widget_height, "y:");
        camera_y_box->labelcolor(font_color);

        camera_y_input = new Fl_Input(margin * 4 + widget_width / 2, camera_inputs_position, widget_width / 2, widget_height);
        camera_y_input->value("0.0");
        camera_y_input->box(FL_GTK_DOWN_BOX);
        camera_y_input->color(input_color);
        camera_y_input->textcolor(text_color);
        camera_y_input->selection_color(select_color);

        camera_z_box = new Fl_Box(margin * 5 + widget_width / 2 * 2, camera_inputs_position, margin, widget_height, "z:");
        camera_z_box->labelcolor(font_color);

        camera_z_input = new Fl_Input(margin * 6 + widget_width / 2 * 2, camera_inputs_position, widget_width / 2, widget_height);
        camera_z_input->value("-6.0");
        camera_z_input->box(FL_GTK_DOWN_BOX);
        camera_z_input->color(input_color);
        camera_z_input->textcolor(text_color);
        camera_z_input->selection_color(select_color);

        size_box1 = new Fl_Box(0, size_position, gui_width, widget_height, "Window width and height in px (100–10000 for each)");
        size_box1->labelcolor(font_color);

        size_input1 = new Fl_Input(margin + widget_width / 3, size_inputs_position, widget_width * 2 / 3 - 10, widget_height);
        size_input1->value("960");
        size_input1->box(FL_GTK_DOWN_BOX);
        size_input1->color(input_color);
        size_input1->textcolor(text_color);
        size_input1->selection_color(select_color);

        size_box2 = new Fl_Box(margin + widget_width, size_inputs_position, margin, widget_height, "X");
        size_box2->labelcolor(font_color);

        size_input2 = new Fl_Input(margin + widget_width + margin + 10, size_inputs_position, widget_width * 2 / 3 - 10, widget_height);
        size_input2->value("540");
        size_input2->box(FL_GTK_DOWN_BOX);
        size_input2->color(input_color);
        size_input2->textcolor(text_color);
        size_input2->selection_color(select_color);

        ngon_box = new Fl_Box(margin + 50, ngon_position, 250 , widget_height, "Note bar cross section:");
        ngon_box->labelcolor(font_color);

        ngon_box2 = new Fl_Box(300, ngon_position, 65, widget_height, "16-gon");
        ngon_box2->labelcolor(text_color);
        ngon_box2->box(FL_GTK_DOWN_BOX);
        ngon_box2->color(input_color);

        ngon_vslider = new Fl_Value_Slider((gui_width - slider_width) / 2, ngon_position + 35, slider_width, widget_height - 6);
        ngon_vslider->bounds(3, 32);
        ngon_vslider->step(1);
        ngon_vslider->value(16);
        ngon_vslider->type(FL_HORIZONTAL);
        ngon_vslider->slider_size(nob_size);
        ngon_vslider->color(bg_color);
        ngon_vslider->color2(click_color);
        ngon_vslider->textcolor(font_color);
        ngon_vslider->textsize(15);
        ngon_vslider->callback(static_ngon_callback, (void*) this);
        ngon_vslider->when(FL_WHEN_CHANGED);

        go_rbutton2 = new Fl_Return_Button(go_gap, gui_height - margin - widget_height, widget_width, widget_height, "Go!");
        go_rbutton2->callback(static_go_callback, (void*) this);
        go_rbutton2->box(FL_FLAT_BOX);
        go_rbutton2->color(bg_color);
        go_rbutton2->color2(click_color);
        go_rbutton2->labelcolor(font_color);

        cancel_button2 = new Fl_Button(go_gap * 2 + widget_width, gui_height - margin - widget_height, widget_width, widget_height, "Cancel");
        cancel_button2->callback(static_cancel_callback, (void*) this);
        cancel_button2->box(FL_FLAT_BOX);
        cancel_button2->color(bg_color);
        cancel_button2->color2(click_color);
        cancel_button2->labelcolor(font_color);
    }advanced_group->end();
    advanced_group->color(bbg_color);
    advanced_group->color2(inactive_color);
    advanced_group->labelcolor(font_color);


    color_group = new Fl_Group(0, tab_height, gui_width, gui_height - tab_height, "Track colors");{
        // prepare colors to assign to the tracks
        const int minimum_brightness = 255;
        const int maximum_brightness = 561;

        // generator of a random integer between 0 and 255
        std::random_device dev;
        std::mt19937 eng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distr(0, 255); // distribution in range [0, 255]

        vector<Fl_Color> track_colors = {
            fl_rgb_color(255, 100, 180),    // hot pink
            fl_rgb_color(50, 50, 255),      // blue
            fl_rgb_color(230, 230, 50),     // yellow
            fl_rgb_color(255, 50, 50),      // red
            fl_rgb_color(102, 77, 204),     // blue-violet
            fl_rgb_color(50, 255, 50),      // green
            fl_rgb_color(255, 150, 0),      // orange
            fl_rgb_color(230, 50, 230),     // purple
            fl_rgb_color(150, 255, 100),    // light green
            fl_rgb_color(215, 0, 64),       // carmine
            fl_rgb_color(25, 127, 156),      // turquoise
            fl_rgb_color(50, 230, 230)     // cyan
        };

        while((int) track_colors.size() < color_count){
            const int proposed_r = distr(eng);
            const int proposed_g = distr(eng);
            const int proposed_b = distr(eng);
            const int sum = proposed_r + proposed_g + proposed_b;

            if(sum >= minimum_brightness && sum <= maximum_brightness){
                track_colors.emplace_back(fl_rgb_color(proposed_r, proposed_g, proposed_b));
            }
        }

        const int cc_gap_h = 15;
        const int cc_gap_v = 25;
        const int cc_width = (gui_width - margin * 2 - cc_gap_h * (color_column_count - 1)) / color_column_count;
        const int cc_height = (gui_height - tab_height - margin * 3 - cc_gap_v * color_row_count - widget_height) / color_row_count;
        const int cc_interval_h = cc_width + cc_gap_h;
        const int cc_interval_v = cc_height + cc_gap_v;

        // generate track color buttons
        for(int i = 0; i < color_count; i++){
            string label = (string) "Track " + std::to_string(i + 1);
            const int row = i / color_column_count;
            const int column = i % color_column_count;
            TrackColor* trackcolor = new TrackColor(
                margin + cc_interval_h * column,
                margin + cc_gap_v + tab_height + cc_interval_v * row,
                cc_width, cc_height, ""
            );
            trackcolor->copy_label(label.c_str());
            trackcolor->color(track_colors[i]);
            trackcolor->hide();
            this->trackcolors.emplace_back(trackcolor);
        }

        go_rbutton3 = new Fl_Return_Button(go_gap, gui_height - margin - widget_height, widget_width, widget_height, "Go!");
        go_rbutton3->callback(static_go_callback, (void*) this);
        go_rbutton3->box(FL_FLAT_BOX);
        go_rbutton3->color(bg_color);
        go_rbutton3->color2(click_color);
        go_rbutton3->labelcolor(font_color);

        cancel_button3 = new Fl_Button(go_gap * 2 + widget_width, gui_height - margin - widget_height, widget_width, widget_height, "Cancel");
        cancel_button3->callback(static_cancel_callback, (void*) this);
        cancel_button3->box(FL_FLAT_BOX);
        cancel_button3->color(bg_color);
        cancel_button3->color2(click_color);
        cancel_button3->labelcolor(font_color);
    }color_group->end();
    color_group->color(bbg_color);
    color_group->color2(inactive_color);
    color_group->labelcolor(font_color);
}
