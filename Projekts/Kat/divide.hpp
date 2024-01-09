#include <fstream>
#include <vector>
#include <Synth/audioFile.h>
#include <Synth/FFT/Clear.h>

enum Status {
	SUCCESS,
	FAILED_OPENING_FILE
};

template < typename T >
T max ( std::vector < T > val ) {
	T Max = val[0];
	for ( T v : val ) 
	{ if ( Max < v ) { Max = v; } }

	return Max;
}

template < typename T >
T min ( std::vector < T > val ) {
	T Min = val[0];
	for ( T v : val ) 
	{ if ( Min > v ) { Min = v; } }

	return Min;
}

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

size_t log2 ( size_t N ) {
	size_t k = N , i = 0 ;
	while ( k ) { k >>= 1 ; i++; }
	return i -1;
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
	for ( size_t i = 0; i < _base_size; i++ ) 
	{ _buf [i] = r[i]; std::cout << " " << i; }
	
	return _buf;	
}

int Taglia ( std::string path, std::vector<std::string> alphabet, double deltaNoise = 0.0625, double pausa = 10000 ) {
	std::cout << "carico il file\n";
	AudioFile < double > a;
	if ( ! a.load ( path ) ) 
	{ return FAILED_OPENING_FILE; }

	std::vector< double > data = a.samples[0];

	std::vector < std::vector < double > > fileTagliati;
	std::vector < double > fileTagliato;
	int paus = 0;

	std::cout << "divido il file\n";
	for ( auto el : data ) {
		if ( -deltaNoise < el && el < deltaNoise ) {
			paus += 1;
            fileTagliato.push_back ( el );
		
			if ( paus > pausa ) {
				fileTagliati.push_back ( fileTagliato );
                fileTagliato = {};
                paus = 0;
			}
		} else {
            paus = 0;
            fileTagliato.push_back ( el );
		}          
	}
	fileTagliati.push_back ( fileTagliato );

	int DurataSilenzioPreEPostLEttera = 20 ;
    std::vector < std::vector < double > > AudioSenzaSilenzi;

	std::cout << "ripulisco i files\n";
    for ( auto audio : fileTagliati ) {
		std::vector < double > audiriarrangiato;
        int silenziAllInizio = 0;
        int silenziAllFine = 0;
        bool primoSilenzio = true;
        for ( auto el : audio ) {
            if(-deltaNoise < el && el < deltaNoise && primoSilenzio)
                silenziAllInizio += 1;
            else if(primoSilenzio)
                primoSilenzio = false;
            if(-deltaNoise < el && el < deltaNoise && ! primoSilenzio)
                silenziAllFine += 1;
            else if( ! primoSilenzio)
                silenziAllFine = 0;
		}
        if ( primoSilenzio ) { continue; }

		for (size_t i = 0; i < audio.size() - ( silenziAllFine - DurataSilenzioPreEPostLEttera ); i++) {
			if ( i > (silenziAllInizio - DurataSilenzioPreEPostLEttera ) )
                audiriarrangiato.push_back ( audio [i] );
		}

        AudioSenzaSilenzi.push_back ( audiriarrangiato );
	}
    
	std::cout << "equalizzo i files\n";
	std::vector < std::vector < double > > AudiosArrayEq;
    for ( auto audio : AudioSenzaSilenzi ) {
		std::vector < double > AudioArrayEq;
        double maxvalue = max <double> ( audio );
        double minvalue = min <double> ( audio );
        double massimoAssoluto = 0.8;
		double mvalue = ( maxvalue >= -minvalue ? maxvalue : -minvalue );

        for ( auto val : audio ) {
            double valoreEq = ( massimoAssoluto * val ) / mvalue;
            AudioArrayEq.push_back ( valoreEq );
		}

        AudiosArrayEq.push_back ( AudioArrayEq );
	}

	// low pass filter
	std::vector < std::vector < double > > LowPass;
	for (size_t i = 0; i < AudiosArrayEq.size ( ); i++) {
		std::cout << "i: " << i << '\n';
		auto samples = AudiosArrayEq[i];

		size_t S = samples.size ( );
		size_t S2 = pow( 2, log2(S) + 1 );
		std::cout << "load " << S << ' ' << S2 << '\n';

		std::vector<double> sample;
		sample.resize ( S2 );
		for ( size_t i = 0; i < S; i++ ) 
		{ sample[i] = samples[i]; }

		auto FFT = FastFourierTransform( S2 );
		//std::cout << "fourier\n";
		auto FFT_1 = FFT.fft1 ( sample.data() );

		clear_signal ( FFT_1, S2, 50.0 );
		FFT_1 = Get_Tone ( FFT_1, S2 );

		auto IFFT  = FFT.ifft1 ( FFT_1 );
		//std::cout << "clear\n";
		//clear_signal ( FFT_1, S2, 10.0);
		//FFT_1 = Derviate_Main ( FFT_1, S2 );
		/*if ( i == 1 ) {
			for (size_t i = 0; i < S2; i++) {
				std::cout << FFT_1[i].imag() << ',';
			}
		}*/
		//std::cout << "inverse fourier\n";

		std::vector<double> sample2;
		sample2.resize ( S2 );
		for ( size_t i = 0; i < S2; i++ ) 
		{ sample2[i] = IFFT[i]; }
		
		LowPass.push_back( sample2 );
		std::cout << "end\n";
	}

	for (size_t i = 0; i < LowPass.size(); i++) 
	{ Save ( alphabet[i] + ".wav",  LowPass[i], a.getSampleRate ( ) ); }

	return SUCCESS;
}