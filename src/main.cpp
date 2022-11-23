#include <chrono> // milliseconds
#include <iostream>
#include <sys/signal.h> // kill
#include <thread> // sleep_for

#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Audio.hpp>

#include "2d.h"
#include "attributes.h"
#include "fountain.h"
#include "generate_wav.h"
#include "gui.h"
#include "prism.h"
#include "stb_image.h"

using std::vector;
using std::string;



void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}





// Quit the application when a callback to hide the window is called
void gui_callback(Fl_Widget*, void*){
    exit(0);
}





// an argument can be used to play a WAVE file instead of a MIDI file
int main(int argc, char** argv){
    // launch the GUI to get the parameters
    GUI* gui = new GUI(500, 600, "Midualizer");
    gui->callback(gui_callback);
    gui->show();
    Fl::run();



    if(gui->td){
        Td_Parameters parameters;

        parameters.midi = gui->midi;
        parameters.soundfont = gui->soundfont;
        parameters.wallpaper = gui->wallpaper;
        parameters.fullscreen = gui->fullscreen;
        parameters.mute = gui->mute;
        parameters.reverse = gui->reverse;
        parameters.color_shuffle = gui->color_shuffle;
        parameters.lane_shuffle = gui->lane_shuffle;
        parameters.speed = gui->speed;
        parameters.radius = gui->radius;
        parameters.opacity = gui->opacity;
        parameters.window_width = gui->window_width;
        parameters.window_height = gui->window_height;
        parameters.ngon = gui->ngon;
        parameters.wall_color = gui->wall_color;
        parameters.color = gui->color;

        parameters.argc = argc;
        parameters.argv = argv;

        td(parameters);
        return 0;
    }

    // adjust some parameters from the GUI
    const float radius = gui->radius / 100.0f;
    const float speed = 0.6f * gui->speed / 3.0f;
    const float opacity = gui->opacity / 100.0f;



    // sound playback parameters
    const float before_time = 11.0f * 3.0f / gui->speed;
    float after_time;
    if(gui->reverse){after_time = before_time + 5.0f;}
    else{after_time = 45.0f * 3.0f / gui->speed;}

    // camera parameters
    const glm::vec3 camera_position = glm::vec3{gui->camera_x, gui->camera_y, gui->camera_z};
    const glm::vec3 camera_direction = glm::vec3{0.0f, 0.0f, 0.0f};

    // fountain effect parameters
    const float particle_lifetime = 0.4f;
    const float particle_speed_xy = 10.0f;
    const float particle_speed_z = -0.12f;
    const int particle_count = 150;

    // wall & note bar parameters
    const float horizontal_interval = 0.2f;
    const float vertical_interval = 8.0f / 127.0f;





    //// GLFW setup
    glfwInit();

    // create a fullscreen or non-fullscreen window
    GLFWwindow* window;
    if(gui->fullscreen){
        GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);
        window = glfwCreateWindow(mode->width, mode->height, "Midualizer", primary_monitor , NULL);
    }else{
        window = glfwCreateWindow(gui->window_width, gui->window_height, "Midualizer", NULL, NULL);
    }

    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //OpenGL x.3
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);





    //// view and projection matrices
    glm::mat4 view = glm::mat4(1.0f); // maybe, this variable should be initialized as a identity matrix
    view = glm::lookAt(camera_position, camera_direction, glm::vec3{0.0f, 1.0f, 0.0f});
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    glm::mat4 projection = glm::mat4(1.0f); // maybe, this variable should be initiated as a identity matrix
    projection = glm::perspective(glm::radians(45.0f), (float) window_width / (float) window_height, 0.1f, 100.0f);





    extern const char* bar_vertex_shader_code;
    extern const char* bar_fragment_shader_code;
    extern const char* wall_vertex_shader_code;
    extern const char* wall_fragment_shader_code;
    extern const char* wallpaper_vertex_shader_code;
    extern const char* wallpaper_fragment_shader_code;
    extern const char* fountain_vertex_shader_code;
    extern const char* fountain_fragment_shader_code;

    //// compile and link shaders
    const GLuint bar_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(bar_vertex_shader, 1, &bar_vertex_shader_code, NULL);
    glCompileShader(bar_vertex_shader);
    const GLuint bar_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(bar_fragment_shader, 1, &bar_fragment_shader_code, NULL);
    glCompileShader(bar_fragment_shader);
    const GLuint bar_shader = glCreateProgram();
    glAttachShader(bar_shader, bar_vertex_shader);
    glAttachShader(bar_shader, bar_fragment_shader);
    glLinkProgram(bar_shader);

    const GLuint wall_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(wall_vertex_shader, 1, &wall_vertex_shader_code, NULL);
    glCompileShader(wall_vertex_shader);
    const GLuint wall_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(wall_fragment_shader, 1, &wall_fragment_shader_code, NULL);
    glCompileShader(wall_fragment_shader);
    const GLuint wall_shader = glCreateProgram();
    glAttachShader(wall_shader, wall_vertex_shader);
    glAttachShader(wall_shader, wall_fragment_shader);
    glLinkProgram(wall_shader);

    const GLuint wallpaper_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(wallpaper_vertex_shader, 1, &wallpaper_vertex_shader_code, NULL);
    glCompileShader(wallpaper_vertex_shader);
    const GLuint wallpaper_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(wallpaper_fragment_shader, 1, &wallpaper_fragment_shader_code, NULL);
    glCompileShader(wallpaper_fragment_shader);
    const GLuint wallpaper_shader = glCreateProgram();
    glAttachShader(wallpaper_shader, wallpaper_vertex_shader);
    glAttachShader(wallpaper_shader, wallpaper_fragment_shader);
    glLinkProgram(wallpaper_shader);

    const GLuint fountain_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fountain_vertex_shader, 1, &fountain_vertex_shader_code, NULL);
    glCompileShader(fountain_vertex_shader);
    const GLuint fountain_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fountain_fragment_shader, 1, &fountain_fragment_shader_code, NULL);
    glCompileShader(fountain_fragment_shader);
    const GLuint fountain_shader = glCreateProgram();
    glAttachShader(fountain_shader, fountain_vertex_shader);
    glAttachShader(fountain_shader, fountain_fragment_shader);
    glLinkProgram(fountain_shader);





    // create all the VAOs and VBOs
    GLuint vaos[3];
    glGenVertexArrays(3, vaos);

    GLuint vbos[7];
    glGenBuffers(7, vbos);





    //// note bars
    // get the attributes
    const Attributes attributes(
        gui->midi,
        speed, radius, horizontal_interval, vertical_interval, before_time,
        gui->reverse, gui->color_shuffle, gui->lane_shuffle, false,
        gui->color
    );
    const vector<float> bar_vertices = prism(gui->ngon);

    /// set the attributes
    glBindVertexArray(vaos[0]);

    // vertices and normals
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, bar_vertices.size() * sizeof(float), &bar_vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) (0));
    glEnableVertexAttribArray(0);
    glVertexAttribDivisor(0, 0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 0);

    // offsets
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, attributes.offsets.size() * sizeof(float), &attributes.offsets[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) (0));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // scales
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, attributes.scales.size() * sizeof(float), &attributes.scales[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) (0));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // colors
    glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ARRAY_BUFFER, attributes.colors.size() * sizeof(float), &attributes.colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) (0));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    // start and end times
    glBindBuffer(GL_ARRAY_BUFFER, vbos[4]);
    glBufferData(GL_ARRAY_BUFFER, attributes.starts_ends.size() * sizeof(float), &attributes.starts_ends[0], GL_STATIC_DRAW);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) (0));
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);


    /// shader stuff
    glUseProgram(bar_shader);

    const GLint bar_view_location = glGetUniformLocation(bar_shader, "view");
    const GLint bar_projection_location = glGetUniformLocation(bar_shader, "projection");
    const GLint bar_camera_position_location = glGetUniformLocation(bar_shader, "camera_position");
    const GLint movement_location = glGetUniformLocation(bar_shader, "movement");
    const GLint time_location = glGetUniformLocation(bar_shader, "time");

    glUniformMatrix4fv(bar_view_location, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(bar_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(bar_camera_position_location, 1, glm::value_ptr(camera_position));





    //// wall
    glBindVertexArray(vaos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[5]);

    // if a wallpaper is specified
    if(gui->wallpaper != (string) ""){
        /// texture
        // set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image using the stb image library
        stbi_set_flip_vertically_on_load(true);
        int image_width, image_height, nr_channels;
        unsigned char* texture_data = stbi_load(gui->wallpaper.c_str(), &image_width, &image_height, &nr_channels, 0); 

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);


        if(nr_channels == 3){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        }
        else if(nr_channels == 4){         // if the loaded image has four channels, that is, r, g, b and alpha
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(texture_data); // romove the loaded image from the memory



        /// vertices
        const float half_height = 128.0f * vertical_interval / 2.0f + radius;
        const float half_width =  (float) image_width / (float) image_height * half_height;

        // the texture is horizontally flipped because in this application, it is seen from behind by default
        // the wall is in the z = 0 plane
        const float wall_vertices[] = {// position                          // texture coordinate
                                         half_width,  half_height, 0.0f,    0.0f, 1.0f,
                                         half_width, -half_height, 0.0f,    0.0f, 0.0f,
                                        -half_width, -half_height, 0.0f,    1.0f, 0.0f,
                                        -half_width,  half_height, 0.0f,    1.0f, 1.0f,
                                         half_width,  half_height, 0.0f,    0.0f, 1.0f,
                                        -half_width, -half_height, 0.0f,    1.0f, 0.0f};

        // vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) 0);
        glEnableVertexAttribArray(0);

        // texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBufferData(GL_ARRAY_BUFFER, sizeof(wall_vertices), wall_vertices, GL_STATIC_DRAW);

        glUseProgram(wallpaper_shader);

        const GLint wallpaper_view_location = glGetUniformLocation(wallpaper_shader, "view");
        const GLint wallpaper_projection_location = glGetUniformLocation(wallpaper_shader, "projection");
        const GLint wallpaper_opacity_location = glGetUniformLocation(wallpaper_shader, "opacity");

        glUniformMatrix4fv(wallpaper_view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(wallpaper_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(wallpaper_opacity_location, opacity);



    // when no wallpaper is specified
    }else{
        const float half_height = 128.0f * vertical_interval / 2.0f;
        const float half_width = (float) attributes.track_count * horizontal_interval / 2.0f + 0.5f; // the last figure represernts the margin

        const float wall_vertices[] = {half_width,  half_height, 0.0f,
                                     half_width, -half_height, 0.0f,
                                    -half_width, -half_height, 0.0f,
                                    -half_width,  half_height, 0.0f,
                                     half_width,  half_height, 0.0f,
                                    -half_width, -half_height, 0.0f};

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) (0));
        glEnableVertexAttribArray(0);

        glBufferData(GL_ARRAY_BUFFER, sizeof(wall_vertices), wall_vertices, GL_STATIC_DRAW);

        glUseProgram(wall_shader);

        const GLint wall_view_location = glGetUniformLocation(wall_shader, "view");
        const GLint wall_projection_location = glGetUniformLocation(wall_shader, "projection");
        const GLint wall_opacity_location = glGetUniformLocation(wall_shader, "opacity");
        const GLint wall_color_location = glGetUniformLocation(wall_shader, "color");

        glUniformMatrix4fv(wall_view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(wall_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(wall_opacity_location, opacity);
        glUniform3fv(wall_color_location, 1, (GLfloat*) &gui->wall_color[0]);
    }





    //// fountain effect
    glBindVertexArray(vaos[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[6]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) (0));
    glEnableVertexAttribArray(0);

    glUseProgram(fountain_shader);

    const GLint fountain_view_location = glGetUniformLocation(fountain_shader, "view");
    const GLint fountain_projection_location = glGetUniformLocation(fountain_shader, "projection");
    const GLint fountain_color_location = glGetUniformLocation(fountain_shader, "color");

    glUniformMatrix4fv(fountain_view_location, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(fountain_projection_location, 1, GL_FALSE, glm::value_ptr(projection));





    //// play sound if the "Mute" option is OFF
    float launch_time;
    pid_t pid;
    if(!gui->mute){
        const string temporary_directory = getenv("XDG_RUNTIME_DIR") + (string) "/app/com.octo_translation.Midualizer";

        /// convert the MIDI file to wav with fluidsynth. directly playing a MIDI file is slow.
        generate_wav(gui->midi, gui->soundfont, temporary_directory);

        /// fork a child process to play the wav file
        pid = fork();
        launch_time = glfwGetTime();
        if(pid == 0){
            // sleep in order to time the sound playback to the video
            int sleep_duration = (launch_time + before_time - glfwGetTime()) * 1000; // in milliseconds
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));

            sf::Music music;
            if(argc == 1){music.openFromFile(temporary_directory + "/temp.wav");}
            else{music.openFromFile(argv[1]);} // play a file passed as a command line argument
            music.play();

            // maintain the buffer memory? until the song ends
            sleep_duration = (launch_time + before_time + attributes.end_time - glfwGetTime()) * 1000;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));
            std::cout << "Audio playback finished." << std::endl;
        }
    }else{
        pid = 1;
        launch_time = glfwGetTime();
    }





    //// main loop and its preparation
    vector<std::map<string, float>> fountain_list = attributes.fountain_list;
    vector<Fountain> active_fountains;
    float now = 0.0f;
    float dt;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // alpha
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // background color

    if(pid != 0){ // if this is not the child process
        // the window is dismissed if the escape key is pressed or when a specified time has passed
        while(
            !glfwWindowShouldClose(window) &&
            glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwGetTime() - launch_time < attributes.end_time + before_time + after_time
        )
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            dt = glfwGetTime() - launch_time - now;
            now = glfwGetTime() - launch_time;


            // note bars
            glUseProgram(bar_shader);
            if(gui->reverse){glUniform1f(movement_location, now * -speed);}
            else{glUniform1f(movement_location, now * speed);}
            glUniform1f(time_location, now);
            glBindVertexArray(vaos[0]);
            glDrawArraysInstanced(GL_TRIANGLES, 0, bar_vertices.size(), attributes.note_count);



            // wall
            if(gui->wallpaper != (string) ""){glUseProgram(wallpaper_shader);}
            else{glUseProgram(wall_shader);}
            glBindVertexArray(vaos[1]);
            glDrawArrays(GL_TRIANGLES, 0, 6);



            //// fountains
            glBlendFunc(GL_SRC_ALPHA, GL_ONE); // special alpha for fountain effect
            glUseProgram(fountain_shader);
            glBindVertexArray(vaos[2]);

            // spawn fountains (= add them to the active_fountain list) according to fountain_list and time
            // spawned fountains are removed from fountain_list
            for(size_t i = 0; i < fountain_list.size(); i++){
                if(now >= fountain_list[i]["start"]){
                    Fountain fountain(fountain_list[i]["x"], fountain_list[i]["y"], fountain_list[i]["end"],
                                      fountain_list[i]["r"], fountain_list[i]["g"], fountain_list[i]["b"],
                                      particle_count, radius, particle_speed_xy, particle_speed_z, particle_lifetime);
                    active_fountains.emplace_back(fountain);
                    fountain_list.erase(fountain_list.begin() + i);
                }
            }

            // update and draw fountains
            for(size_t i = 0; i < active_fountains.size(); i++){
                active_fountains[i].update(dt);

                // prapare the vertices of the updated fountain
                vector<float> fountain_vertices;
                for(auto particle : active_fountains[i].particles){
                    fountain_vertices.emplace_back(particle.x + active_fountains[i].offset_x);
                    fountain_vertices.emplace_back(particle.y + active_fountains[i].offset_y);
                    fountain_vertices.emplace_back(particle.z);
                }

                // draw the fountain
                glBufferData(GL_ARRAY_BUFFER, fountain_vertices.size() * sizeof(float), &fountain_vertices[0], GL_DYNAMIC_DRAW);
                vector<float> fountain_color{active_fountains[i].r, active_fountains[i].g, active_fountains[i].b};
                glUniform3fv(fountain_color_location, 1, &fountain_color[0]);
                glDrawArrays(GL_POINTS, 0, fountain_vertices.size());

                // despawn old fountains = remove them from the active_fountains list
                if(now > active_fountains[i].end){
                    active_fountains.erase(active_fountains.begin() + i);
                }
            }



            glfwPollEvents(); // for ESC or global hot key termination
            glfwSwapBuffers(window); // required for smooth rendering
        }





        glfwTerminate();
        kill(0, SIGKILL); // kill the child process for sound playback if it's still alive
        return 0;
    }
}
