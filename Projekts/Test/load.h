#define OPENGL
#define OPENAL
#define ALL_MODULES
#include <ReKat.hpp>

class Hello : public Behaviour {
	public:
	void Start ( ) { std::cout << "Obj: " << obj->Get_Name() << " says: hello world\n"; }
	void Update ( ) { }
};



int Load ( ) {
	int result = 0;
	result += Manager::Shader_Load  ( "sprite", "sprite.vs", "sprite.fs");
	result += Manager::Texture_Load ( "sprite", "sprite.png" );

	Objekt main ( "main" );
	main.Add_component < Hello > ( );
	auto cam = main.Add_component < Camera > ( );
	Objekt gio  ( "gio" );
	std::cout << "gio: " << &gio << '\n';
	gio.Add_component < Hello > ( );
	auto sprite = gio.Add_component < Sprite > ( );
	sprite->shader = Manager::Shader_Get ( "sprite" );
	sprite->texture = Manager::Texture_Get ( "sprite" );
	sprite->camera = cam;
	sprite->frames = {2,2};
	main.Add_Child ( &gio );

	main.Start ( );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.6, 0.4, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		main.Update ( );
		ReKat::grapik::Pool ( );
    }

	std::cout << "\nresources loaded\n";
	return result;
}