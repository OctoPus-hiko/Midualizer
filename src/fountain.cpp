#include <random> // its seems <random> includes <numbers>

#include "fountain.h"



// generator of a random float between 0.0 and 1.0
std::random_device dev;
std::default_random_engine eng(dev());
std::uniform_real_distribution<float> distr(0.0f, 1.0f);



Particle::Particle(float radius, float particle_speed_xy, float particle_speed_z, float particle_lifetime){
        const float theta = distr(eng) * 2 * std::numbers::pi_v<float>;
        x = cos(theta) * radius;
        y = sin(theta) * radius;
        z = 0.0f;
        movement_x = x * (0.025f / radius) * particle_speed_xy;
        movement_y = y * (0.025f / radius) * particle_speed_xy;
        movement_z = particle_speed_z;
        life = distr(eng) * particle_lifetime;
     }



Fountain::Fountain(
    float x, float y, float end_time,
    float rr, float gg, float bb,
    int particle_count,
    float radius, float particle_speed_xy, float particle_speed_z, float particle_lifetime
)
{
    for(int i = 0; i < particle_count; i++){
        const Particle pt(radius, particle_speed_xy, particle_speed_z, particle_lifetime);
        particles.emplace_back(pt);
    }

    offset_x = x;
    offset_y = y;
    end = end_time;
    r = rr;
    g = gg;
    b = bb;
    f_particle_count = particle_count;
    f_radius = radius;
    f_particle_speed_xy = particle_speed_xy;
    f_particle_speed_z = particle_speed_z;
    f_particle_lifetime = particle_lifetime;
}



// method to update the particles
void Fountain::update(float dt){
    for(int j = 0; j < f_particle_count; j++){
        Particle &p = particles[j];
        if(p.life > 0.0f){
            p.life -= dt;
            p.x += p.movement_x * dt;
            p.y += p.movement_y * dt;
            p.z += p.movement_z * dt;
        }
        else{
            const float theta = distr(eng) * 2 * std::numbers::pi_v<float>;
            p.x = cos(theta) * f_radius;
            p.y = sin(theta) * f_radius;
            p.z = 0.0f;
            const float velocity_xy = distr(eng) * f_particle_speed_xy;
            p.movement_x = p.x * (0.025f / f_radius) * velocity_xy;
            p.movement_y = p.y * (0.025f / f_radius) * velocity_xy;
            p.movement_z = f_particle_speed_z;
            p.life = distr(eng) * f_particle_lifetime;
        }
    }
}
