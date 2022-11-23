struct Note{
    float start, end;
    int pitch, track;
    bool is_drum;

    Note(float p_start, float p_end, int p_pitch, int p_track, bool p_is_drum){
        start = p_start;
        end = p_end;
        pitch = p_pitch;
        track = p_track;
        is_drum = p_is_drum;
       };

    ~Note(){};
};



std::vector<Note> notes(std::string midi);
