#include <engine.hpp>
#include "synth.h"
#include <Synth/FFT/Clear.h>

int Save ( std::string path, double* data, long size, int sample_rate = 44100 ) {
    // create file
    AudioFile<double> audioFile;
    audioFile.setBitDepth ( 16 );
    audioFile.setSampleRate ( sample_rate );

    // create buffer
    AudioFile<double>::AudioBuffer buffer;
    buffer.resize ( 1 );
    buffer[0].resize ( size );

	ofstream o ( "out.md" );
    for (size_t i = 0; i < size; i++) { buffer [0][i] = data [i]; o << i << '\n'; }
    
    // copy buffer
    if ( !audioFile.setAudioBuffer (buffer)) { std::cerr << "COPY_BUFFER_ERROR"; return 1; }
    // save buffer
    if ( !audioFile.save ( path )) { std::cerr << "SAVE_ERROR"; return 1;}

    return 0;
}

class FFT_CLEAR : public Behaviour {
	void Start ( ) {
		AudioFile < double > a;
		a.load ( "audio.wav" );
		long S = a.getNumSamplesPerChannel ( );
		std::cout << "loaded\n";

		auto FFT = FastFourierTransform ( S );
		auto Trans = FFT.fft1 ( a.samples[0].data( ) );
		std::cout << "transformed\n";

		clamp_freq ( Trans, S, 1000 );
		auto Rectified = FFT.ifft1 ( Trans );
		Save ( "AUDIO.wav", Rectified, S );
		std::cout << "exported\n";
	}
};

int Load ( ) {
    ReKat::grapik::Start ( "Kat", 800, 600, false, false, true );
	Synth *Audio_Drivers = new Synth;
	Audio_Drivers->_Start ( );
	int result = 0;
	Objekt * scene = new Objekt ( "Scene" );
	scene->Add_Component ( Audio_Drivers );
	scene->Add_Component < Fps > ( );
	scene->Add_Component ( Audio_Drivers );
	scene->Add_Component < synth > ( );

	scene->Add_Component < FFT_CLEAR > ( );

	Manager::Set_Active_Scene ( scene );
	return result;
}