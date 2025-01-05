#define DIAGNOSTIC
#include <engine.hpp>

int largezza_campo = 1300;
int heigth = 340;
int half_lunghezza = largezza_campo>>1;

vec4 unbrekable_color = { 0.7,0.7,0.7,1 };

enum STATE {
    START,
    LIFELOST,
    PLAYING,
    WIN,
    LOSE,
    COMPLETED,
    REBOOT
};

STATE current_state = START;

std::vector < vec4 > colors = {
    {1,0,0,1},
    {1,1,0,1},
    {0,1,0,1},
    {0,1,1,1},
    {0,0,1,1},
    {1,0,1,1},
};

class Level : public Resource {
    public: 
    int width = 0;
    int heigth = 0;
    std::vector < int > cells;

    int Make ( const char * path ) {
        std::vector < std::string > Data;
        std::ifstream Data_stream;
        // ensure ifstream objects can throw exceptions:
        Data_stream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            Data_stream.open ( path );
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << Data_stream.rdbuf();
            Data_stream.close ();
            // Tokenizing 
            heigth = 0;
            while ( getline ( sData_stream, intermediate, '\n' ) ) 
            { heigth++; Data.push_back(intermediate); }
        }
        catch(const std::exception& e) { DEBUG ( 1, e.what ( ) ); }
        
		std::vector < int > D;
		D.reserve ( Data.size( ) );
        for ( std::string s : Data ) {
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << s;
            DEBUG ( 4, s );
            // Tokenizing 
            width = 0;
            while ( getline ( sData_stream, intermediate, ',' ) ) {
                width++;
                DEBUG ( 5, intermediate );
                D.push_back( std::stoi(intermediate));
            }
        }
        cells = D;
        return 0;
    }

    friend std::ostream& operator << ( std::ostream& os, const Level& level ) {
        os << "H: " << level.heigth << " W: " << level.width << '\n';
        for (size_t y = 0; y < level.heigth; y++) {
            os << "{ ";
            for (size_t x = 0; x < level.width; x++) 
            { os << level.cells[ y*level.width + x ] << ' '; }
            os << "}\n";
        }
        
        return os;
    }
};

namespace Manager {
    static int Level_Load ( std::string name, const char* levelPath ) {
        Level *s = new Level ();
        _resources.insert( { (name + std::string(typeid(Level).name())) , s } );
        return (*s).Make( levelPath );
    }
};

class Splash_Screen : public Behaviour {
	Text* titolo;
	Text* sottot;
public:
	void Start ( ) {
		obj->Set_Pos ( {0,60,0} );
		titolo = obj->Add_Component < Text > ( );
		titolo->Set ( "BreakOut", Text::CENTER );
		titolo->Set ( "font", "text" );

		auto Sottot = Manager::Objekt_Load ( "sotto titolo", {0,-80,0}, {30,30,100} );
		sottot = Sottot->Add_Component < Text > ( );
		sottot->Set ( "made with 4RealEngine5", Text::CENTER );
		sottot->Set ( "font", "text" );

		obj->Add_Child ( Sottot );

		// animation
		// 3 secondi Titolo
		// 2 secondi fade
		auto titolo_a = new Animation ( titolo->Expose_Color ( ), ONCE );
		auto sottot_a = new Animation ( sottot->Expose_Color ( ), ONCE );
		titolo_a->Add_Frame ( {1,1,1,1}, {1,1,1,1}, 3 );
		sottot_a->Add_Frame ( {1,1,1,1}, {1,1,1,1}, 3 );
		titolo_a->Add_Frame ( {1,1,1,1}, {1,1,1,0}, 1 );
		sottot_a->Add_Frame ( {1,1,1,1}, {1,1,1,0}, 1 );
		DEBUG ( 3, "animations: ", titolo_a, " ", sottot_a );
		Manager::Animation_Load ( "titolo", titolo_a );
		Manager::Animation_Load ( "sottot", sottot_a );
		obj->Add_Component < Animator > ( )
		->New_Node ( "splash" )
		->Change_Animation ( "splash" )
		->Add_Animation ( "splash", titolo_a )
		->Add_Animation ( "splash", sottot_a );
	}

	void Update ( ) {
		if ( Timer::current_time > 4.5 ) {
			Manager::Set_Active_Scene ( "scene" );
			ReKat::phisiks::Set_Active ( "scene" );
		} 
	}
};

class Display : public Behaviour {
public:
    int _points = 0, _lives = 5, _current_level = 1, _blocks_left;
    Text *points, *lives, *level, *text;

    void Start ( ) {
        auto __points  = Manager::Objekt_Load ( "points", {-400,440,0} );
        auto __lives   = Manager::Objekt_Load ( "lives", {100,440,0} );
        auto __level   = Manager::Objekt_Load ( "level", {400,440,0} );
        auto __text = Manager::Objekt_Load ( "gameover", {0,-200,0} );

        points  = __points->Add_Component < Text > ( )->Set ( "font", "text" );
        lives   = __lives->Add_Component < Text > ( )->Set ( "font", "text" );
        level   = __level->Add_Component < Text > ( )->Set ( "font", "text" );
        text = __text->Add_Component < Text > ( )->Set ( "font", "text" );

        obj->Add_Child ( __points ); 
        obj->Add_Child ( __lives ); 
        obj->Add_Child ( __level );
        obj->Add_Child ( __text );
    }

    void Update ( ) {
        if ( current_state == START ) { text->Set ( "Press Space to Start...\n(A & D to move)" )->Set_Active ( true ); }
        if ( current_state != START ) { text->Set_Active ( false ); }
        points->Set ( std::to_string ( _points ), Text::LEFT );
        lives->Set ( std::to_string ( _lives ), Text::CENTER );
        level->Set ( std::to_string ( _current_level ), Text::CENTER );
        if ( current_state == LIFELOST ) 
        { text->Set ( "Continue? Press Space" )->Set_Active ( true ); }
        if ( _lives == 0 ) { text->Set ( "GAME OVER\nRematch?" )->Set_Active ( true ); current_state = LOSE; }
        if ( _blocks_left == 0 ) { text->Set ( "WIN!!!" )->Set_Active ( true ); current_state = WIN; }
        if ( _current_level == 6 ) 
        { text->Set ( "COMPLETE!!!" )->Set_Active ( true ); _blocks_left = -1; current_state = COMPLETED; }
    }
};

float speed = 104 * 7;
float paddle_speed =  speed * 1.5;

class Paddle : public Behaviour {
    void Start ( ) {
        obj->Set_Size ( { 100,10,10 } );
        obj->Set_Pos ( {0,-400,0} );

        obj->Add_Component < Sprite > ( )->Set ( "sprite", "sprite" );
        obj->Add_Component < Box_Collider > ( )->Set_Size ( { 100,10,10 } );
    }

    void Update ( ) {
        // cntrolls
        if ( Key_Pressed ( "A" ) ) { obj->Inc_Pos ( {-paddle_speed * Timer::delta_time,0,0} ); }
		if ( Key_Pressed ( "D" ) ) { obj->Inc_Pos ( { paddle_speed * Timer::delta_time,0,0} ); }

        // limit
        auto offset = 50;

		if ( obj->Get_Pos ( ).x > half_lunghezza - offset ) 
		{ obj->Set_Pos ( {half_lunghezza - offset, obj->Get_Pos ( ).y, 0} ); }
		
		if ( obj->Get_Pos ( ).x < - half_lunghezza + offset ) 
		{ obj->Set_Pos ( {-half_lunghezza + offset, obj->Get_Pos ( ).y, 0} ); }
    }
};

class Ball_Controller : public Behaviour {
    Paddle * _player;
    Display * _display;
    void Begin ( ) {
        obj->Set_Pos ( vec3 (0,20,0) );
        obj->Get_Component < Rigidbody > ( )->velocity = { ( rand( ) % 3 - 1 ) * speed*0.4,-speed*0.7,0};
    }

    void Start ( ) {
        srand ( time (0) );

        obj->Set_Size ( {15,15,15} );
        obj->Set_Pos ( {0,-300,0} );

        obj->Add_Component < Sprite > ( )->Set ( "sprite", "sprite" );
        obj->Add_Component < Box_Collider > ( )->Set_Size ( 15 );
        obj->Add_Component < Rigidbody > ( );
    }

    void Update ( ) {
        if ( Key_Down ( " " ) && ( current_state == START || current_state == WIN || current_state == LIFELOST ) ) 
        { Begin ( ); current_state = PLAYING; }
        if ( current_state == LOSE && Key_Down ( " " ) ) 
        { _display->_lives = 5; _display->_points = 0; current_state = REBOOT; }

        if ( Key_Down ( "P" ) ) { _display->_blocks_left = 0; }

        auto rigi = obj->Get_Component < Rigidbody > ( );

        // limit
        auto offset = 8;

		if ( obj->Get_Pos ( ).x > half_lunghezza - offset ) {
            obj->Set_Pos ( {half_lunghezza - offset, obj->Get_Pos ( ).y, 0} );
            rigi->velocity = { -rigi->velocity.x, rigi->velocity.y, 0 };
        }
		
		if ( obj->Get_Pos ( ).x < - half_lunghezza + offset ) {
            obj->Set_Pos ( {-half_lunghezza + offset, obj->Get_Pos ( ).y, 0} );
            rigi->velocity = { -rigi->velocity.x, rigi->velocity.y, 0 };
        }

        if ( obj->Get_Pos ( ).y > heigth ) {
            obj->Set_Pos ( {obj->Get_Pos ( ).x, heigth, 0} );
            rigi->velocity = {rigi->velocity.x, -rigi->velocity.y, 0 };
        }

        if ( current_state == COMPLETED ) {
            if ( obj->Get_Pos ( ).y < -480 ) {
                obj->Set_Pos ( {obj->Get_Pos ( ).x, -480, 0} );
                rigi->velocity = {rigi->velocity.x, -rigi->velocity.y, 0 };
            }
        }

        if ( obj->Get_Pos ( ).y < -490 ) {
            rigi->velocity = {0,0,0};
            obj->Set_Pos ( {0,-300,0} );
            _display->_lives--;
            current_state = LIFELOST;
        }

        if ( _display->_lives == 0 || _display->_blocks_left == 0 ) {
            rigi->velocity = {0,0,0};
            obj->Set_Pos ( {0,-300,0} );
        }
    }

    void Collision ( Objekt* _obj ) {
        auto rigi = obj->Get_Component < Rigidbody > ( );

        if ( _obj->Get_Name ( ) != "player" ) {
            rigi->velocity = ( normalize ( rigi->velocity ) * speed );
            return;
        }
		auto an = obj->Get_Pos( ) - _obj->Get_Pos( );
		vec3 norm = normalize ( vec3{an.x,an.y,an.x*0.7} );
		// limit angle at 45 degree
		if ( abs(norm.x) > 0.7071067811865475 ) {
			norm.x = ( norm.x > 0 ? 0.7071067811865475 : -0.7071067811865475 );
			norm.y = ( norm.y > 0 ? 0.7071067811865475 : -0.7071067811865475 );
		}
		rigi->velocity = { norm * speed };
	}
public:
    void Score ( int points ) { _display->_points += points; }

    Ball_Controller * Set ( Paddle * player, Display * display ) 
    { _player = player; _display = display; return this; }
};

int block_width = 0;
int block_heigth = 0;

class Block : public Behaviour {
public:
    Display * _display;
    int _points = 0;
    int _level = 0;
    int _type = 0;
    void Start ( ) {
        obj->Set_Size ( {block_width-2,block_heigth-2,10} );
        obj->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "", {1,1}, 0, 
            ( _type == 2 ? unbrekable_color :  colors[_level] ) );
        obj->Add_Component < Box_Collider > ( )->Set_Size ( obj->Get_Size ( ) );
    }
    void Collision ( Objekt* _obj ) {
        if ( _type == 2 ) { return; }
        obj->Set_Active ( false );
        _obj->Get_Component < Ball_Controller > ( )->Score ( _points );
        _display->_blocks_left --;
    }
    Block * Set ( int level, int points, int type, Display * display ) 
    { _level = level; _points = points; _type = type; _display = display; return this; }
};

class Scene_Display : public Behaviour {
    Display * _display;
    int padding = 50;
    int thickness = 2;
    int blocks = 0;

    void Start_Level ( int _level ) {
        if ( _level != 1 ) 
        { obj->Get_Children ( "active_level" + std::to_string ( _level - 1 ) )->Set_Active ( false ); }
        auto __level = Manager::Objekt_Load ( "active_level" + std::to_string ( _level ) );
        auto level = Manager::Get < Level > ( "level" + std::to_string ( _level ) );
        block_width = largezza_campo / level->width;
        block_heigth = ( heigth - padding ) / level->heigth;
        int y_compensation = ( ( level->heigth % 2 == 1 ) ? block_heigth / 2 : 0 ) + padding;
        int x_compensation = ( level->width % 2 == 0 ) ? block_width / 2: 0;
        DEBUG ( 5, *level );
        for (int y = 0; y < level->heigth; y++) {
            for (int x = 0; x < level->width; x++) {
                if ( level->cells[ y*level->width + x ] != 0 ) {
                    auto block = Manager::Objekt_Load ( std::to_string ( y*level->width + x ), 
                        { ( x - level->width/2 ) * block_width + x_compensation, y * block_heigth + y_compensation,0  } );
                    block->Add_Component < Block > ( )->Set ( y % 6, y + 1, level->cells[ y*level->width + x ], _display );

                    __level->Add_Child ( block );
                    blocks++;
                }
            }
        }
        obj->Add_Child ( __level );
        _display->_blocks_left = blocks;
    }

    void Start ( ) {
        auto Top_Bar    = Manager::Objekt_Load ( "Top_Bar", { 0,heigth,0 }, {largezza_campo,thickness,10} );
        auto Left_Bar   = Manager::Objekt_Load ( "Left_Bar", { -largezza_campo>>1,(heigth-500)/2,0 }, {thickness,500+heigth,10} );
        auto Rigth_Bar  = Manager::Objekt_Load ( "Rigth_Bar", { largezza_campo>>1,(heigth-500)/2,0 }, {thickness,500+heigth,10} );

        Top_Bar->Add_Component < Sprite > ( )->Set ( "sprite", "sprite" );
        Left_Bar->Add_Component < Sprite > ( )->Set ( "sprite", "sprite" );
        Rigth_Bar->Add_Component < Sprite > ( )->Set ( "sprite", "sprite" );

        obj->Add_Child ( Top_Bar );
        obj->Add_Child ( Left_Bar );
        obj->Add_Child ( Rigth_Bar );

        Start_Level ( _display->_current_level );
    }

    void Update ( ) {
        if ( _display->_blocks_left == 0 && Key_Down ( " " ) ) {
            _display->_current_level ++;
            _display->text->Set_Active ( false );
            Start_Level ( _display->_current_level );
        }

        if ( current_state == REBOOT ) {
            obj->Get_Children ( "active_level" + std::to_string ( _display->_current_level ) )->Set_Active ( false );

            _display->_current_level = 1;
            Start_Level ( 1 );
            current_state = START;
        }
    }

public:
    Scene_Display * Set ( Display * display ) 
    { _display = display; }
};

int main ( ) {
	ReKat::phisiks::Start ( 120 );
	ReKat::grapik::Start ( "Pong", 800, 600,false,false,true );
	ReKat::synth::Start ( );

    Manager::Texture_Load ( "sprite", "Data/empty.png" );
    Manager::Font_Load ( "font", "Data/Font.ttf", 90, 13 );
    Manager::Shader_Load ( "sprite", "Data/sprite.vs", "Data/sprite.fs" );
    Manager::Shader_Load ( "text", "Data/text.vs", "Data/text.fs" );

    Manager::Level_Load ( "level1", "Data/Levels/Level1.kat" );
    Manager::Level_Load ( "level2", "Data/Levels/Level2.kat" );
    Manager::Level_Load ( "level3", "Data/Levels/Level3.kat" );
    Manager::Level_Load ( "level4", "Data/Levels/Level4.kat" );
    Manager::Level_Load ( "level5", "Data/Levels/Level5.kat" );
    Manager::Level_Load ( "level6", "Data/Levels/Level6.kat" );

    auto Splash = Manager::Objekt_Load ( "splash" );
    auto Scene  = Manager::Objekt_Load ( "scene" );
    auto Player = Manager::Objekt_Load ( "player" );
    auto Ball   = Manager::Objekt_Load ( "Ball" );

    Scene->Add_Child ( Player );
    Scene->Add_Child ( Ball );

    Splash->Add_Component < Splash_Screen > ( );

    auto display = Scene->Add_Component < Display > ( );

    Scene->Add_Component < Scene_Display > ( )->Set ( display );
    Ball->Add_Component < Ball_Controller > ( )->Set (
        Player->Add_Component < Paddle > ( ),
        display
    );

    Manager::Set_Active_Scene ( "scene" );
	ReKat::phisiks::Set_Active ( "scene" );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Manager::Update ( );
		ReKat::grapik::Update ( );
		ReKat::phisiks::Update ( );
	}

	Manager::Free ( );
	ReKat::grapik::End ( );
	ReKat::synth::End ( );
	return 0;
}
