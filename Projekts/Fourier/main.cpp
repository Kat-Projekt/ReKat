#include <iostream>
#include <fstream>
#include <Synth/audioFile.h>
#include <Synth/FFT/Clear.h>
#include <Synth/synth.hpp>

int Save ( std::string path, std::vector<double> data, int sample_rate = 44100 ) {
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

ofstream p ( "data.py", std::ios_base::app );

int __data = 0;
template < typename T >
void print_data ( std::string name, T _data, size_t size ) {
	ofstream p ( "data.py", std::ios_base::app );
	__data++;
	std::cout << " printing: data " << name << '\n';
	p << "data" << __data << " = [ \"" << name << "\", [\n";
	// comma separated values
	for ( size_t i = 0; i < size; i++ ) {
		p << _data[i];
		if ( i == size-1 ) 
		{ p << '\n'; } 
		else 
		{ p << ','; }
	}

	p << "] ]\n";
	std::cout << "  data printed\n";
}

void print_complex_data ( std::string name, std::complex<double> * _data, size_t size, int real = 0 ) {
	__data++;
	std::cout << " printing: data " << name << '\n';
	p << "data" << __data << " = [ \"" << name << "\", [\n";
	// comma separated values
	for ( size_t i = 0; i < size; i++ ) {
		switch ( real ) {
			case 0: p << _data[i].real(); break;
			case 1: p << _data[i].imag(); break;
			case 2: p << abs(_data[i]); break;
			
			default: p << _data[i]; break;
		}
		if ( i == size-1 ) 
		{ p << '\n'; } 
		else 
		{ p << ','; }
	}

	p << "] ]\n";
	std::cout << "  data printed\n";
}

std::complex<double> * Get_Tone ( std::complex<double>* _base, size_t _base_size ) {
	std::cout << "#CALL\n";
	int block_size = 10;

	std::vector < std::vector < std::complex<double> > > fileTagliati;
	std::vector < std::complex<double> > fileTagliato;
	int paus = 0;

	std::cout << "#DIVISION\n";
	int sizzze = 0;
	for (size_t i = 0; i < _base_size; i++ ) {
		auto el = _base[i];
		if ( abs(el) == 0 ) {
			paus += 1;
            fileTagliato.push_back ( el );
		
			if ( paus == 10 ) {
				std::cout << "\t sample size: " << fileTagliato.size();
				sizzze += fileTagliato.size();
				fileTagliati.push_back ( fileTagliato );
                fileTagliato.clear();
                paus = 0;
			}
		} else {
            paus = 0;
            fileTagliato.push_back ( el );
		}
	}
	std::cout << "\nto size: " << sizzze + fileTagliato.size();
	std::cout << "\n#DIVIDED\n";
	fileTagliati.push_back ( fileTagliato );
	std::vector < std::complex<double> > r;
	r.reserve ( _base_size );
	// per ogni intervallo prendi il massimo
	int porceseded = 0;
	for ( auto el : fileTagliati ) {
		auto max = el[0];
		for ( auto e : el ) { if ( abs ( e ) > abs ( max ) ) { max = e; } }
		for ( auto e : el ) {
			if ( e != max ) { r.push_back ( 0 ); }
			else { r.push_back ( max ); }
			porceseded++;
		}
	}
	std::cout << "\tsamples procesed: " << porceseded << '\n';
	std::cout << "\t size of procesed: " << r.size() << '\n';

	std::cout << "#COPY\n";
	std::cout << "base :: " << _base_size << '\n'; 
	std::complex<double> * _buf = ( std::complex<double> * ) calloc ( _base_size, sizeof(std::complex<double>) );
	for ( size_t i = 0; i < _base_size; i++ ) { 
		try {
			/* code */
		_buf [i] = r[i]; std::cout << " " << i; 
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		
	}
	
	return _buf;	
}

int main() {
	std::ofstream o ( "data.py" );
	o << "# Kat data\n";
	o.close();

	ReKat::synth::Start ();
	std::cout << "START\n"; 
	AudioFile <double> a;
	a.load ( "a.wav" );

	auto samples = a.samples[0];

	size_t S = samples.size ( ) / 2;

	std::vector<double> sample;
	sample.resize (S);
	for (size_t i = 0; i < S; i++) 
	{ sample[i] = samples[ 1000 + i]; }
	// print_data < std::vector<double>& > ( "original", sample, S );

	auto FFT = FastFourierTransform( S );
	
	auto FFT_1 = FFT.fft1 ( sample.data() );
	std::cout << "# computed fourier's transform\n";
	print_complex_data ( "original", FFT_1, S, 2 );


	clear_signal ( FFT_1, S, 50.0 );
	// print_complex_data ( "abs", FFT_1, S, 2 );

		// FFT_1 = Derviate_Main ( FFT_1, S );
		/*if ( i == 1 ) {
			for (size_t i = 0; i < S; i++) {
				std::cout << FFT_1[i].imag() << ',';
			}
	}*/
	//std::cout << "inverse fourier\n";
	std::cout << "#CALLING\n";
	FFT_1 = Get_Tone ( FFT_1, S );
	std::cout << "#CALLED\n";

	print_complex_data( "max", FFT_1, S, 2);
	print_complex_data( "imag", FFT_1, S,1);

	auto IFFT  = FFT.ifft1 ( FFT_1 );
	std::cout << "# computed inverse fourier's transform\n";

	print_data < double* > ( "recomposition", IFFT, S );

	std::cout << "END\n";

	ReKat::synth::Start ();
	ReKat::synth::Create_Buffer ( "fft", IFFT, S );
	ReKat::synth::Play( "fft" );
	ReKat::synth::End();
	system ( "python3 plot.py" );

	std::vector<double> sampleff;
	sampleff.resize (S);
	for (size_t i = 0; i < S; i++) 
	{ sampleff[i] = IFFT[i]; }
	Save("test.wav", sampleff );
	
	p.close();
}