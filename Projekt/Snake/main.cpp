#include <engine.hpp>

// Snake == 1
// Apple == 2
// Empty == 0

enum GameState {
    PLAYING,
    PAUSED,
    LOST
};

GameState State = PLAYING;

void Spawn_Apple ( int *& Map, unsigned int dimension ) {
    assert ( dimension != 0 );
    int point = rand ( ) % dimension;
    int og = ( point == 0 ? dimension - 1 : point - 1 );
    // find empty space
    while ( Map[point] == 0 || og == point ) {
        point ++;
        point %= dimension;
    }

    // polulate empty space
    Map[point] = 2;
}

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
void Move_Snake ( int *& Map, unsigned int Width, unsigned int Heigth, Direction Dir ) {
    static int Head_X = - 1; // non inizializate
    static int Head_Y = - 1;
    static int Tail_X = - 1; // non inizializate
    static int Tail_Y = - 1;

    // inizialization
    if ( Head_X == -1 && Head_Y == -1 ) {
        Head_X = Width >> 1;
        Head_Y = Heigth >> 1;
        Tail_X = Head_X;
        Tail_Y = Head_Y;
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
        case NONE : New_Head_X ++; break; 
    }

    // out of bounds
    if ( New_Head_X == -1 || New_Head_X == Width ||
         New_Head_Y == -1 || New_Head_Y == Heigth ) {
        State = LOST;
        return;
    }

    // collisions
    bool grow = false;
    switch ( Map [ New_Head_Y * Width + New_Head_X ] ) {
        case 0: break; // empty
        case 1: State = LOST; return; break; // self collide
        case 2: grow = true; break; // mela
    }

    // re render
    Map [ New_Head_Y * Width + New_Head_X ] = 1; // new head
    if ( !grow ) // not found an apple
    { Map [ Tail_Y * Width + Tail_X ] = 0; } // remove tail
}

int load ( std::string base_dir = "" ) {
    int loaded = 0;
    // load resorces

    // load objekts
    auto Splas = Manager::Objekt_Load ( "Splash" );
    auto PostP = Manager::Objekt_Load ( "Post Processor" );
    auto Scene = Manager::Objekt_Load ( "Scene" );

    PostP->Add_Component < Framebuffer > ( )->Set ( Scene )->Set ( 800, 600 );

    Manager::Set_Active_Scene ( "Splash" );

    return loaded;
}

int main ( ) {
    // randomnes
    srand ( time (0) );
    // map size
    unsigned int Width = 10, Heigth = 10;
    // to be accesed via Map [x][y] = Map [ y * width + x ]
    int * Map = ( int * ) calloc ( Width * Heigth, sizeof ( int ) );

    ReKat::grapik::Start ( "snake", 800, 600 );
    ReKat::phisiks::Start ( 10 );
    ReKat::synth::Start ( );
    
    GameState OldState = State;
    Direction Movement = LEFT;
    float UpdateTime = 1.0f; // tempo tra i movimenti
    float UpdateTimer = Timer::current_time + UpdateTime; // inzio de conto del tempo
    float AppleSpawn = 5.0f; // tempo tra la comparsa delle mele
    float AppleTimer = Timer::current_time + AppleSpawn; // conto del tempo per le mele
    while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	
        // pause
        if ( Key_Down ( "P" ) ) {
            if ( State == PAUSED ) { State = OldState; }
            else { OldState = State; State = PAUSED; } }
        
        // exit
        if ( Key_Down ( "X" ) ) { ReKat::grapik::End ( ); }

        // movement
        if ( Key_Down ( "W" ) ) { Movement = UP; }
        if ( Key_Down ( "S" ) ) { Movement = DOWN; }
        if ( Key_Down ( "D" ) ) { Movement = LEFT; }
        if ( Key_Down ( "A" ) ) { Movement = RIGHT; }

        // update position
        if ( UpdateTimer < Timer::current_time && State == PLAYING ) {
            Move_Snake ( Map, Width, Heigth, Movement );
            UpdateTimer = Timer::current_time + UpdateTime;
        }

        // spawn apples
        if ( AppleTimer < Timer::current_time && State == PLAYING ) {
            Spawn_Apple ( Map, Width * Heigth );
            UpdateTimer = Timer::current_time + AppleSpawn;
        }

		Manager::Update ( );
		ReKat::grapik::Update ( );
		ReKat::phisiks::Update ( );
	}

	Manager::Free ( );
	ReKat::grapik::End ( );
	ReKat::synth::End ( );

    // free
    free ( Map );    
    return 0;
}
