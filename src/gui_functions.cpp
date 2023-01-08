#include <functional> // std::all_of()
#include <filesystem>
#include <set>

#include <FL/Fl_Native_File_Chooser.H>
#include <midifile/MidiFile.h>

#include "gui.h"
#include "trackcount.h"

using namespace smf; // for MidiFile

using std::string;
using std::vector;



void GUI::midi_callback(Fl_Widget* w){
    Fl_Native_File_Chooser fnfc;
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fnfc.title("Pick a file");
    fnfc.filter("MIDI \t*.{mid,midi}");
    fnfc.directory(getenv("HOME"));

    switch(fnfc.show()){
        case -1: fl_alert(fnfc.errmsg()); break;  // ERROR
        case  1: break;                           // CANCEL
        default:                                  // FILE CHOSEN
            this->midi = fnfc.filename();

            string basename = std::filesystem::path(fnfc.filename()).filename().string();

            if((int) basename.length() <= this->label_max){
                this->midi_box->copy_label(basename.c_str());
            }else{
                string truncated_basename = basename.substr(0, this->label_max);
                this->midi_box->copy_label(truncated_basename.c_str());
            }
            break;
    }

    int track_count = trackcount(fnfc.filename());
    extern int color_count;
    if(track_count > color_count){
        for(int i = 0; i < color_count; i++){
            this->trackcolors[i]->show();
        }
    }else{
        for(int i = 0; i < track_count; i++){
            this->trackcolors[i]->show();
        }
        for(int i = track_count; i < color_count; i++){
            this->trackcolors[i]->hide();
        }
    }
}




void GUI::td_callback(Fl_Widget* w){
    if(this->wall_trackcolor->color() == fl_rgb_color(12, 12, 12) &&
        !this->is_changed
    ){
        this->wall_trackcolor->color(fl_rgb_color(123, 123, 123));
        this->wall_trackcolor->redraw();
    }
    this->is_changed = true;
    
    if(this->td_check->value() == 1){
        this->camera_x_input->deactivate();
        this->camera_y_input->deactivate();
        this->camera_z_input->deactivate();
    }else{
        this->camera_x_input->activate();
        this->camera_y_input->activate();
        this->camera_z_input->activate();
    }
}




void GUI::soundfont_callback(Fl_Widget* w){
    Fl_Native_File_Chooser fnfc;
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fnfc.title("Pick a soundfont");
    fnfc.filter("Soundfont \t*.sf2");
    fnfc.directory(getenv("HOME"));

    switch(fnfc.show()){
        case -1: fl_alert(fnfc.errmsg()); break;  // ERROR
        case  1: break;                           // CANCEL
        default:                                  // FILE CHOSEN
            this->soundfont = fnfc.filename();

            string basename = std::filesystem::path(fnfc.filename()).filename().string();

            if((int) basename.length() <= this->label_max){
                this->soundfont_box->copy_label(basename.c_str());
            }else{
                string truncated_basename = basename.substr(0, this->label_max);
                this->soundfont_box->copy_label(truncated_basename.c_str());
            }
            break;

    }
}





void GUI::wallpaper_callback(Fl_Widget* w){
    Fl_Native_File_Chooser fnfc;
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fnfc.title("Pick a wallpaper");
    fnfc.filter("Images \t*.{bmp,gif,hdr,jpg,jpeg,pic,png,pgm,ppm,psd,tga}");
    fnfc.directory(getenv("HOME"));

    switch ( fnfc.show() ) {
        case -1: fl_alert(fnfc.errmsg()); break;  // ERROR
        case  1: break;                           // CANCEL
        default:                                  // FILE CHOSEN
            this->wallpaper = fnfc.filename();

            string basename = std::filesystem::path(fnfc.filename()).filename().string();
            
            if((int) basename.length() <= this->label_max){
                this->wallpaper_box->copy_label(basename.c_str());
            }else{
                string truncated_basename = basename.substr(0, this->label_max);
                this->wallpaper_box->copy_label(truncated_basename.c_str());
            }
            break;
    }
}





void GUI::ngon_callback(Fl_Widget* w){
    const int slider_value = this->ngon_vslider->value();
    const string string_value = std::to_string(slider_value) + "-gon";
    const char* char_value = string_value.c_str();
    this->ngon_box2->copy_label(char_value);
}





void GUI::go_callback(Fl_Widget* w){
    if(this->midi == ""){
        fl_alert("Choose a MIDI file.");
        return;
    }

    if(this->soundfont == ""){
        fl_alert("Choose a soundfont.");
        return;
    }

    this->td = this->td_check->value();
    this->fullscreen = this->fullscreen_check->value();
    this->mute = this->mute_check->value();
    this->reverse = this->reverse_check->value();
    this->color_shuffle = this->color_shuffle_check->value();
    this->lane_shuffle = this->lane_shuffle_check->value();
    this->speed = this->speed_vslider->value();
    this->radius = this->radius_vslider->value();
    this->opacity = this->opacity_vslider->value();
    this->ngon = this->ngon_vslider->value();

    uchar wr, wg, wb;
    Fl::get_color(wall_trackcolor->color(), wr, wg, wb);
    vector<float> wrgb = {(float) wr / 255.0f, (float) wg / 255.0f, (float) wb / 255.0f};
    this->wall_color = wrgb;

    string camera_x_string = this->camera_x_input->value();
    camera_x = std::stof(camera_x_string);
    if(camera_x < -100.0f || camera_x > 100.0f){
        fl_alert("Incorrect value for camera position.");
        return;
    }else{
        this->camera_x = camera_x;
    }

    string camera_y_string = this->camera_y_input->value();
    camera_y = std::stof(camera_y_string);
    if(camera_y < -100.0f || camera_y > 100.0f){
        fl_alert("Incorrect value for camera position.");
        return;
    }else{
        this->camera_y = camera_y;
    }

    string camera_z_string = this->camera_z_input->value();
    camera_z = std::stof(camera_z_string);
    if(camera_z < -100.0f || camera_z > 100.f){
        fl_alert("Incorrect value for camera position.");
        return;
    }else{
        this->camera_z = camera_z;
    }

    string window_width_string = this->size_input1->value();
    window_width = std::stoi(window_width_string);
    if(window_width < 100 || window_width > 10000){
        fl_alert("Incorrect value for window size.");
        return;
    }else{
        this->window_width = window_width;
    }

    string window_height_string = this->size_input2->value();
    window_height = std::stoi(window_height_string);
    if(window_height < 100 || window_height > 10000){
        fl_alert("Incorrect value for window size.");
        return;
    }else{
        this->window_height = window_height;
    }

    for(auto trackcolor : this->trackcolors){
        if(trackcolor->visible()){
            uchar r, g, b;
            Fl::get_color(trackcolor->color(), r, g, b);
            vector<float> rgb = {(float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f};
            this->color.emplace_back(rgb);
        }
    }

    this->hide();
    //Fl::hide_all_windows(); //function only available in FLTK 1.4 or newer
    //Fl::delete_widget(this);
}





void GUI::cancel_callback(Fl_Widget* w){
    exit(0); 
}
