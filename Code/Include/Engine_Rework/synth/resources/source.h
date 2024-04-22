#include "../synth.hpp"

#include <glm/glm.hpp>

class Source {
public:
	ALuint source;

	Source ( ) { };
	~Source ( ) { End( ); };

	int Make ( glm::vec3 _pos = {0,0,0} ) {
		int error;
		alGenSources((ALuint)1, &source);
		AL_CHECK_ERROR;
		// confinguration
		alSourcef(source, AL_PITCH, 1);
		AL_CHECK_ERROR;
		alSourcef(source, AL_GAIN, 1);
		AL_CHECK_ERROR;
		alSource3f(source, AL_POSITION, _pos.x, _pos.y, _pos.z );
		AL_CHECK_ERROR;
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		AL_CHECK_ERROR;
		alSourcei(source, AL_LOOPING, AL_FALSE);
		AL_CHECK_ERROR;
		std::cout << "source: " << source << '\n';
		return 0;
	}
	void End ( ) 
	{ alDeleteSources ( 1, &source ); }
};