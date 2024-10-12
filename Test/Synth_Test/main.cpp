#define DIAGNOSTIC
#include <synth/components.hpp>
#include <phisiks/timer.hpp>

int main ( ) {
    ReKat::synth::Start ( );

    Objekt listener ( "gior" );
    Objekt source ( "pier" );

    listener.Add_Child ( &source );

    Manager::Buffer_Load ( "tone", "tone.wav" );
    Manager::Source_Load ( "tone" );

    listener.Add_Component < Audio_Listener > ( );
    source.Add_Component < Audio_Source > ( )->Set ( "tone", "tone" )->Play ( );

    Manager::Set_Active_Scene ( &listener );

    Timer::Update ( );
    float time_to_wait = Timer::Get_Time ( ) + 10;
    while ( Timer::Get_Time ( ) <= time_to_wait ) {
        Timer::Update ( );
        source.Set_Pos ( { 0,sin ( Timer::Get_Time ( ) * 2 ) * 5,0 } );
        source.Get_Component < Audio_Source > ( )->Play ();
        Manager::Update ( );
    }

    Manager::Free_Audio ( );
    ReKat::synth::End ( );

}