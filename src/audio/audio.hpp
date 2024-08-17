#ifndef AUDIO_H
#define AUDIO_H

class Audio {
   public:
    Audio(const int channels, const int chunksize);  // open audio

    // Methods
    void loadMusic(const char *music_path);
    void playMusic();
    void changeVolume(const int music_volume);
    void freeResources();

   private:
    // Variables
    Mix_Music *music;
};

#endif  // AUDIO_H
