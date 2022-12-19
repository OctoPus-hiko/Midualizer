const char* bar_vertex_shader_code = R"glsl(
# version 330 core

layout(location = 0) in vec3 attribute_position;
layout(location = 1) in vec3 attribute_normal;
layout(location = 2) in vec3 attribute_offset;
layout(location = 3) in vec3 attribute_scale;
layout(location = 4) in vec3 attribute_color;
layout(location = 5) in vec2 attribute_start_end;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec3 fragment_color;
out vec2 fragment_start_end;

uniform mat4 view;
uniform mat4 projection;
uniform float movement;

void main(){ 
    gl_Position = projection * view * vec4(attribute_position * attribute_scale + attribute_offset + vec3(0.0, 0.0, movement), 1.0);
    fragment_position = vec3(attribute_position * attribute_scale + attribute_offset + vec3(0.0, 0.0, movement));
    fragment_normal = attribute_normal;
    fragment_color = attribute_color;
    fragment_start_end = attribute_start_end;
}
)glsl";





const char* bar_fragment_shader_code = R"glsl(
# version 330 core

in vec3 fragment_position;
in vec3 fragment_normal;
in vec3 fragment_color;
in vec2 fragment_start_end;

out vec4 out_color;

uniform vec3 camera_position;

uniform float time;

vec3 calculate_pointlight(vec3 l_position, vec3 f_color, vec3 normal, vec3 f_position, vec3 v_direction);

void main(){
    vec3 light_position = vec3(-3.0, 3.0, -1.0);

    float glowup_duration = 0.2;
    float dim_speed = 3.0;

    vec3 norm = normalize(fragment_normal);
    vec3 view_direction = normalize(camera_position - fragment_position);

    vec3 result = calculate_pointlight(light_position, fragment_color, norm, fragment_position, view_direction);
    result += calculate_pointlight(vec3(-light_position.x, light_position.y, light_position.z), fragment_color, norm, fragment_position, view_direction);

    if(time < fragment_start_end.x){
        out_color = vec4(result, 1.0);

    // non-short note bars
    }else if(fragment_start_end.y - fragment_start_end.x > glowup_duration && time < fragment_start_end.x + glowup_duration){
        vec3 color_increment = vec3((vec3(1.0) - result) / glowup_duration);
        out_color = vec4(result + color_increment * (time - fragment_start_end.x), 1.0);

    }else if(time < fragment_start_end.y){
        out_color = vec4(1.0);

    }else if(time >= fragment_start_end.y && time < fragment_start_end.y + glowup_duration * dim_speed){
        vec3 color_decrement = vec3((vec3(1.0) - result) / (glowup_duration * dim_speed));
        out_color = vec4(vec3(1.0) - color_decrement * (time - fragment_start_end.y), 1.0);

    }else{
        out_color = vec4(result, 1.0);
    }
}

vec3 calculate_pointlight(vec3 l_position, vec3 f_color, vec3 normal, vec3 f_position, vec3 v_direction){
    vec3 light_color = vec3(1.0);
    float shininess = 64.0;
    float ambient_intensity = 0.1;
    float specular_intensity = 0.5;

    // distance 200
    float light_constant = 1.0;
    float light_linear = 0.022;
    float light_quadratic = 0.0014;


    vec3 light_direction = normalize(l_position - f_position);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 ambient = ambient_intensity * light_color;

    vec3 reflection_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(v_direction, reflection_direction), 0.0), shininess);
    vec3 specular = specular_intensity * spec * light_color;

    float distance = length(l_position - f_position);
    float attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * distance * distance);

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular) * f_color;
    return result;
}
)glsl";





const char* wall_vertex_shader_code = R"glsl(
# version 330 core

layout(location = 0) in vec3 attribute_position;

out vec3 fragment_position;

uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * vec4(attribute_position, 1.0);
    fragment_position = attribute_position;
}
)glsl";





const char* wall_fragment_shader_code = R"glsl(
# version 330 core

out vec4 out_color;

uniform float opacity;
uniform vec3 color;

void main(){
    out_color = vec4(color, opacity);
}
)glsl";





const char* wallpaper_vertex_shader_code = R"glsl(
# version 330 core

layout(location = 0) in vec3 attribute_position;
layout(location = 1) in vec2 attribute_texture_coord;

out vec2 fragment_texture_coord;

uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * vec4(attribute_position, 1.0);
    fragment_texture_coord = attribute_texture_coord;
}
)glsl";





const char* wallpaper_fragment_shader_code = R"glsl(
# version 330 core

in vec2 fragment_texture_coord;

out vec4 out_color;

uniform sampler2D piyo_texture;
uniform float opacity;

void main(){
    out_color = vec4(1.0, 1.0, 1.0, opacity) * texture(piyo_texture, fragment_texture_coord);
}
)glsl";





const char* fountain_vertex_shader_code = R"glsl(
#version 330 core

layout(location = 0) in vec3 attribute_position;

out vec3 fragment_color;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

void main(){
    gl_Position = projection * view * vec4(attribute_position, 1.0);
    fragment_color = color;
}
)glsl";





const char* fountain_fragment_shader_code = R"glsl(
# version 330 core

in vec3 fragment_color;

out vec4 out_color;

void main(){
    out_color = vec4((fragment_color.x + 1.0) / 2, (fragment_color.y + 1.0) / 2, (fragment_color.z + 1.0) / 2, 0.5);
}
)glsl";





const char* tdbar_fragment_shader_code = R"glsl(
# version 330 core

in vec3 fragment_position;
in vec3 fragment_normal;
in vec3 fragment_color;
in vec2 fragment_start_end;

out vec4 out_color;

uniform vec3 camera_position;

uniform float time;

vec3 calculate_pointlight(vec3 l_position, vec3 f_color, vec3 normal, vec3 v_direction);

void main(){
    vec3 light_direction = normalize(vec3(-1.0, 1.0, 0.0));

    float glowup_duration = 0.2;
    float dim_speed = 3.0;

    vec3 norm = normalize(fragment_normal);
    vec3 view_direction = normalize(camera_position - fragment_position);

    vec3 result = calculate_pointlight(light_direction, fragment_color, norm, view_direction);
    result = result * 0.5; // make note bars darker.


    if(time < fragment_start_end.x){
        out_color = vec4(result, 1.0);

    // non-short note bars
    }else if(fragment_start_end.y - fragment_start_end.x > glowup_duration && time < fragment_start_end.x + glowup_duration){
        vec3 color_increment = vec3((vec3(1.0) - result) / glowup_duration);
        out_color = vec4(result + color_increment * (time - fragment_start_end.x), 1.0);

    }else if(time < fragment_start_end.y){
        out_color = vec4(1.0);

    }else if(time >= fragment_start_end.y && time < fragment_start_end.y + glowup_duration * dim_speed){
        vec3 color_decrement = vec3((vec3(1.0) - result) / (glowup_duration * dim_speed));
        out_color = vec4(vec3(1.0) - color_decrement * (time - fragment_start_end.y), 1.0);

    }else{
        out_color = vec4(result, 1.0);
    }
}

vec3 calculate_pointlight(vec3 light_direction, vec3 f_color, vec3 normal, vec3 v_direction){
    vec3 light_color = vec3(1.0);
    float shininess = 64.0;
    float ambient_intensity = 0.1;
    float specular_intensity = 0.5;

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 ambient = ambient_intensity * light_color;

    vec3 reflection_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(v_direction, reflection_direction), 0.0), shininess);
    vec3 specular = specular_intensity * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * f_color;
    return result;
}
)glsl";

