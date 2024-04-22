#ifndef _synth
#define _synth

#include <engine.hpp>
#include <math.h>
using namespace ReKat::grapik::Input;

class synth : public Behaviour {
	short * _Buf;
	Audio_Source A;
public:
	virtual void Start ( ) {
		_Buf = (short*) calloc (1024*128,sizeof(short));
		for (size_t i = 0; i < 1024*128; i++) 
		{ _Buf[i] = sin ( i / 440 ) * 255; }
		Manager::Buffer_Load ( "carlo", _Buf, 1024*128 );
		Manager::Source_Load ( "carlo", {0,0,0} );
		obj->Add_Component ( &A ) -> Set ( "carlo", "carlo" );
		A._Start ( );
		A.Play ( );
	}
	virtual void Update ( ) {  }
	virtual void Delete ( ) { delete _Buf; }
};

#endif