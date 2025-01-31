// #define DIAGNOSTIC
// #define EXTEND
#include <engine.hpp>

// Snake >= 3
// Time Trial == 2
// Apple == 1
// Empty == 0

namespace Core {
    enum GameState {
        PLAYING,
        PAUSED,
        LOST
    };

    float Thing_Timer_End = 0;
    float Thing_Time = 5;
    unsigned int Thing_Pos = 1;

    GameState State = PLAYING;

    inline int coord ( int*& Map, ivec2 coords, ivec2 size ) {
        if ( coords.y == size.y ) { coords.y = 0; }
        if ( coords.x == size.x ) { coords.x = 0; }
        if ( coords.y == - 1 ) { coords.y = size.y - 1; }
        if ( coords.x == - 1 ) { coords.x = size.y - 1; }
        return Map[ (coords.y % size.y) * size.x + (coords.x % size.x)];
    }

    int Update_Thing ( int *& Map ) {
        if ( Thing_Timer_End < Timer::current_time ) {
            if ( Map[Thing_Pos] == 2 ) { Map[Thing_Pos] = 0; }
            if ( Map[Thing_Pos-1] == 2 ) { Map[Thing_Pos-1] = 0; }
            return 0;
        }

        return (int)( Thing_Timer_End - Timer::current_time + 1 );
    }

    void Spawn_Thing ( int *& Map, unsigned int dimension, unsigned int width ) {
        Thing_Timer_End = 0;
        Update_Thing ( Map );
        assert ( dimension != 0 );
        int point = rand ( ) % dimension;
        if ( point == 0 ) { point++; }
        int og = ( point == 0 ? dimension - 1 : point - 1 );
        // find empty space
        while ( Map[point] != 0 || og == point ) {
            point ++;
            point %= dimension;
            if ( point % width == 0 ) { point++; }
        }

        // polulate empty space
        Map[point] = 2;
        Map[point-1] = 2;
        Thing_Timer_End = Timer::current_time + Thing_Time;
        Thing_Pos = point;
    }

    void Spawn_Apple ( int *& Map, unsigned int dimension, unsigned int width ) {
        assert ( dimension != 0 );
        int point = rand ( ) % dimension;
        int og = ( point == 0 ? dimension - 1 : point - 1 );
        // find empty space
        while ( Map[point] != 0 || og == point ) {
            point ++;
            point %= dimension;
        }

        // polulate empty space
        Map[point] = 1;

        if ( rand ( ) % 5 == 0 ) { Spawn_Thing ( Map, dimension, width ); }
    }

    enum Direction { UP, DOWN, LEFT, RIGHT, STAY };
    int Move_Snake ( int *& Map, unsigned int Width, unsigned int Heigth, Direction Dir ) {
        static int Head_X = - 1; // non inizializate
        static int Head_Y = - 1;
        static int Max = 5;
        static int Min = 3;
        static int Pointes = 0;

        // inizialization
        if ( Head_X == -1 && Head_Y == -1 ) {
            // put the head and the tail
            Head_X = Width >> 1;
            Head_Y = Heigth >> 1;
            Map [ Head_Y * Width + Head_X - 1 ] = Min;
            Map [ Head_Y * Width + Head_X ] = Min + 1;
            std::cout << "inialize";
            return 0;
        }

        // Se nel movimento tocca tocca una mela size ++
        // se nel movimento supera i confini game over
        // se nel movimento tocca se stesso game over

        int New_Head_X = Head_X;
        int New_Head_Y = Head_Y;
        switch ( Dir ) {
            case UP   : New_Head_Y ++; break; 
            case DOWN : New_Head_Y --; break; 
            case LEFT : New_Head_X ++; break; 
            case RIGHT: New_Head_X --; break; 
            case STAY : break; 
        }

        // loop borders
        New_Head_X = ( New_Head_X == -1 ? Width - 1 : 
                     ( New_Head_X == Width ? 0 : New_Head_X ) );
        New_Head_Y = ( New_Head_Y == -1 ? Heigth - 1 : 
                     ( New_Head_Y == Heigth ? 0 : New_Head_Y ) );
        

        // collisions
        int grow = false;
        switch ( Map [ New_Head_Y * Width + New_Head_X ] ) {
            case 0: break; // empty
            case 1: grow = true; Pointes += 7; break; // mela
            case 2: grow = 2; Pointes += 30; Thing_Timer_End = 0; break; // thing
            default: State = LOST; return Pointes; break; // self collide
        }

        // re render
        Map [ New_Head_Y * Width + New_Head_X ] = Max; // new head
        Max ++;
        if ( grow == 0) { // not found an apple // remove tail
            for ( unsigned long i = 0; i < Width*Heigth; i++ ) {
                if ( Map[i] == Min ) { Map[i] = 0;  }
            }
            Min ++;
        } 

        // re position
        Head_X = New_Head_X;
        Head_Y = New_Head_Y;

        if ( grow == 1 ) // spawn a new apple
        { Spawn_Apple ( Map, Width * Heigth, Width ); }

        return Pointes;
    }

    void Print ( int* Map, unsigned int Width, unsigned int Heigth ) {
        std::string line = "";
        for ( unsigned int y = 0; y < Heigth; y++ ) {
            line = "[ ";
            for ( unsigned int x = 0; x < Width; x++ ) {
                switch ( Map[y*Heigth + x] ) {
                    case 0: line += "0"; break;
                    case 1: line += "\033[1;31m0\033[0m"; break;
                    default: line += "\033[1;32m0\033[0m"; break;
                }
                line += ( x != Width - 1 ? ", " : " ]" );
            }
            std::cout << line << '\n';
        }
        std::cout << '\n';
    }
}
using namespace Core;

class Map_Render : public Behaviour {
public:
    int * _Map = nullptr;
    int _Width = 10, _Heigth = 10;
    
    struct Dis_Data {
        float x;
        float y;
        float sprite;
    };
    void Start ( ) {
        // create buffer
        auto John = Manager::Get < Instance > ( "Jonh" );
        std::vector < Dis_Data > Display_Data;
        Display_Data.reserve (_Width*_Heigth);

        for ( unsigned int i = 0; i < _Width * _Heigth; i++ ) {
            Dis_Data p = {(float)(i%_Width - _Width/2), (float)(i/_Width -_Heigth/2), (float)_Map[i]};
            Display_Data.push_back ( p );
        }

        John->Configure_Atributes ( {{3,GL_FLOAT,false,3*sizeof(float)}}, 1 );
        John->Add_Data ( Display_Data.data ( ), Display_Data.size ( ) );
    }

    void Update ( ) {
        std::vector < Dis_Data > Display_Data;
        Display_Data.reserve (_Width*_Heigth);
        
        int Max = 0;
        int Min = 0;
        for ( int i = 0; i < _Width * _Heigth; i++ ) {
            float frame = _Map[i];
            if ( frame == 2 ) {
                // choose criket
                if ( coord( _Map, ivec2( i%_Width - 1, i/_Width ), {_Width,_Heigth} ) == 2 ) {
                    frame = 2 + ( Thing_Pos % 4 ) * 2 + 1;
                } else {
                    frame = 2 + ( Thing_Pos % 4 ) * 2;
                }
            } else
            if ( frame > 2 ) {
                // find configuration           // 00001   00010  00100  01000
                std::vector < ivec2 > neibours = { {1,0}, {-1,0}, {0,1}, {0,-1} };
                int direction = 16; // 10000
                for ( size_t _nb = 0; _nb < neibours.size(); _nb++ ) {
                    ivec2 nb = neibours[_nb] + ivec2( i%_Width, i/_Width );
                    auto Coo = coord ( _Map, nb, {_Width,_Heigth} );
                    if ( Coo == frame - 1 || Coo == frame + 1 ) // the is snake
                    { direction += 1 << _nb; } // do the wired bit shift
                }
                if ( _Map[Max] < frame ) { Max = i; }
                if ( _Map[Min] > frame || _Map[Min] == 0 ) { Min = i; }
                frame = direction;
            }
            Dis_Data p = {(float)(i%_Width - _Width/2), (float)(i/_Width -_Heigth/2),frame};
            Display_Data.push_back ( p );
        }
        
        // the one on Max is the head
        Display_Data[Max].sprite += 32; // 0100000
        // yhe one on Min is the tail
        Display_Data[Min].sprite += 64; // 1000000

        Manager::Get < Instance > ( "Jonh" )->Update_Data ( 0,_Width*_Heigth, Display_Data.data ( ) );
    }

    Map_Render * Set ( int * Map, unsigned int Width = 10, unsigned int Heigth = 10 ) 
    { _Map = Map; _Width = Width; _Heigth = Heigth; return this; }
};


#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
#endif

int load ( std::string base_dir = "" ) {

#ifdef __APPLE__
	CFURLRef appUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("Font"), CFSTR("ttf"), CFSTR("Data"));
	CFStringRef filePathRef = CFURLCopyPath(appUrlRef);
	const char* filePath = CFStringGetCStringPtr(filePathRef, kCFStringEncodingUTF8);
	base_dir += filePath;

	for ( int i = 0; i < 13; i++) 
	{ base_dir.pop_back ( ); }
	base_dir.push_back ( '/' );

	// Release references
	CFRelease(filePathRef);
	CFRelease(appUrlRef);
#endif

    int loaded = 0;
    // load resorces
    loaded += Manager::Shader_Load ( "instanze", ( base_dir + "Data/instancer.vs" ).c_str( ), ( base_dir + "Data/instancer.fs" ).c_str( ) );
    loaded += Manager::Shader_Load ( "text", ( base_dir + "Data/text.vs" ).c_str( ), ( base_dir + "Data/text.fs" ).c_str( ) );
    loaded += Manager::Texture_Load ( "Tiles", ( base_dir + "Data/Snake original.png" ).c_str( ) );
    loaded += Manager::Font_Load ( "font", ( base_dir + "Data/Font.ttf" ).c_str( ), 90, 13 );

    Manager::Instance_Load ( "Jonh", sizeof(Map_Render::Dis_Data),100 );

    // load objekts
    auto Splas = Manager::Objekt_Load ( "Splash" );
    auto PostP = Manager::Objekt_Load ( "Post Processor" );
    auto Scene = Manager::Objekt_Load ( "Scene" );
    auto Infos = Manager::Objekt_Load ( "Infos display" );
    auto Point = Manager::Objekt_Load ( "Point display", { -300,400,0}  );
    auto Timer = Manager::Objekt_Load ( "Timer display", { 300,400,0} );
    auto Mapper = Manager::Objekt_Load ( "Map", {0,0,0}, {50,50,50} );

 
    PostP->Add_Component < Framebuffer > ( )->Set ( Scene )->Set ( 800, 600 );
    Mapper->Add_Component < Map_Render > ( );
    Mapper->Add_Component < Sprite > ( )->Set ( "Tiles", "instanze", "cam", {6,4},0,{1,1,1,1},true,"Jonh");
    Point->Add_Component < Text > ( )->Set ( "font", "text", "",{0,1,1,1} )->Set ( "0" );
    Timer->Add_Component < Text > ( )->Set ( "font", "text", "",{1,0,1,1} );

    Scene->Add_Child ( Mapper );
    Scene->Add_Child ( Infos );

    Infos->Add_Child ( Point );
    Infos->Add_Child ( Timer );

    return loaded;
}


int main ( ) {
    // randomnes
    srand ( time (0) );
    // map size
    unsigned int Width = 26, Heigth = 18;
    // to be accesed via Map [x][y] = Map [ y * width + x ]
    int * Map = ( int * ) calloc ( Width * Heigth, sizeof ( int ) );
    Move_Snake ( Map, Width, Heigth, STAY );
    Spawn_Apple ( Map, Width * Heigth, Width ); // spawn first apple

    ReKat::grapik::Start ( "snake", 800, 600 );
    ReKat::synth::Start ( );

    if ( load ( ) ) { DEBUG( 1,"Error Loading" ); }

    Manager::Objekt_Get ( "Map" )->Get_Component < Map_Render > ( )->Set ( Map, Width, Heigth );
    Manager::Set_Active_Scene ( "Scene" );

    auto _Punti = Manager::Objekt_Get ( "Point display" )->Get_Component < Text > ( );
    auto _Timer = Manager::Objekt_Get ( "Timer display" )->Get_Component < Text > ( );

    GameState OldState = State;
    State = PAUSED;
    Direction Movement = LEFT;
    float UpdateTime = 0.07f; // tempo tra i movimenti
    float UpdateTimer = Timer::current_time + UpdateTime; // inzio de conto del tempo
    while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(1.0, 1.0, 1.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	
        // pause
        if ( Key_Down ( "P" ) ) {
            if ( State == PAUSED ) { State = OldState; }
            else { OldState = State; State = PAUSED; } }
        
        // exit
        if ( Key_Down ( "X" ) ) { ReKat::grapik::End ( ); }

        // movement
        if ( Key_Down ( "W" ) && Movement != DOWN ) { Movement = UP; }
        if ( Key_Down ( "S" ) && Movement != UP ) { Movement = DOWN; }
        if ( Key_Down ( "D" ) && Movement != RIGHT ) { Movement = LEFT; }
        if ( Key_Down ( "A" ) && Movement != LEFT ) { Movement = RIGHT; }

        // update position
        if ( UpdateTimer < Timer::current_time && State == PLAYING ) {
            _Punti->Set ( std::to_string ( Move_Snake ( Map, Width, Heigth, Movement ) ) );
            UpdateTimer = Timer::current_time + UpdateTime;
        }
        auto U = Update_Thing ( Map );
        if ( U == 0 ) { _Timer->Set ( "_" ); }
        else { _Timer->Set ( std::to_string (U) ); }

		Manager::Update ( );
		ReKat::grapik::Update ( );
		Timer::Update ( );
	}

	Manager::Free ( );
	ReKat::grapik::End ( );
	ReKat::synth::End ( );

    // free
    free ( Map );    
    return 0;
}
