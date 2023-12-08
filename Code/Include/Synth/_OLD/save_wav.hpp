#pragma once
#include "audioFile.h"

int save_wav ( std::vector<double> data, std::string path, int sample_rate = 44100 ) {
    // create file
    AudioFile<double> audioFile;
    audioFile.setBitDepth ( 16 );
    audioFile.setSampleRate ( sample_rate );

    // create buffer
    AudioFile<double>::AudioBuffer buffer;
    buffer.resize ( 1 );
    buffer[0].resize ( data.size ( ) );

    for (size_t i = 0; i < data.size ( ); i++) { buffer [0][i] = data [i]; }
    
    // copy buffer
    if ( !audioFile.setAudioBuffer (buffer)) { std::cerr << "COPY_BUFFER_ERROR"; return 1; }
    // save buffer
    if ( !audioFile.save ( path )) { std::cerr << "SAVE_ERROR"; return 1;}

    return 0;
}