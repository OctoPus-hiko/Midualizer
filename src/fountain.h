struct Particle{
    float x, y, z, movement_x, movement_y, movement_z, life;

    Particle(float radius, float particle_speed_xy, float particle_speed_z, float particle_lifetime);

    ~Particle(){}
 };



class Fountain{
    public:
        std::vector<Particle> particles;
        float offset_x, offset_y, end, r, g, b;
        int f_particle_count;
        float f_radius, f_particle_speed_xy, f_particle_speed_z, f_particle_lifetime;

        Fountain(
            float x, float y, float end_time,
            float rr, float gg, float bb,
            int particle_count,
            float radius, float particle_speed_xy, float particle_speed_z, float particle_lifetime);

        void update(float dt);

        ~Fountain(){}
};
