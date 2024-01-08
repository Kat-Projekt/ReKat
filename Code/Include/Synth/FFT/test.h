#include "Clear.h"
#include <Synth/audioFile.h>
#include <Synth/synth.hpp>
#include <Synth/fft.h>

int test ( ) {
	AudioFile < float > a;
	a.load ( "a.wav" );
	int S = a.samples [0].size ( );
	int S2 = pow( 2, log2(S) + 1 );
	std::cout << "load " << S << ' ' << S2 << '\n';

	double * samples = ( double * ) calloc ( S2, sizeof(double) );
	for ( size_t i = 0; i < S; i++ ) 
	{ samples[i] = (double) a.samples[0][i]; }

	auto FFT = FastFourierTransform( S2 );
	std::cout << "\n\nfourier\n";
	auto FFT_1 = FFT.fft1 ( samples );
	clear_signal ( FFT_1, S2, 30.0 );

	/*for ( size_t i = 0; i < S; i++ ) 
	{ std::cout << FFT_1[i].real() << ','; }*/

	std::cout << "\n\ninverse fourier\n";
	auto IFFT  = FFT.ifft1 ( FFT_1 );

	/*for ( size_t i = 0; i < S; i++ ) 
	{ std::cout << IFFT[i] << ','; }*/

	synth::Create_Buffer ( "AA", IFFT, S2 );

	return 0;
}