#define OPENGL
#define OPENAL
#define ONLINE_PEER
#define ALL_MODULES
#include <ReKat.hpp>
#include "gravitate.h"
#include "camera_controll.h"

static void __FreamBufferResize ( GLFWwindow* window, int width, int height ) {
    glViewport ( 0, 0, ReKat::grapik::Internal::SCR_WIDTH = width, ReKat::grapik::Internal::SCR_HEIGTH = height );
}

void Main_Loop_FrameBuf ( ) {
	// framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

	
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	while ( ReKat::grapik::IsEnd ( ) ) {
		// glClearColor(0.6, 0.4, 0.5, 1.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		Scene_Manager::Update();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);
		Manager::Shader_Get ( "frame" )->Use ( );
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
		ReKat::grapik::Update();
	}
}

int Load ( ) {
	ReKat::grapik::Input::_FreamBufferResize = __FreamBufferResize;
	int result = 0;

	result += Manager::Shader_Load  ( "sprite", "sprite.vs", "sprite.fs");
	result += Manager::Shader_Load  ( "frame", "framebuffers.vs", "framebuffers.fs");

	result += Manager::Texture_Load ( "sprite", "sprite.png" );
	if ( result != 0 ) { return 1; }

	Objekt main ( "main" );
	Objekt pier ( "pier",{-500,0,0}, {100,100,10} );
	Objekt gio ( "gio",{500,0,0}, {200,200,100} );
	Objekt camera ( "camera" );
	
	main.Add_Child ( &camera );
	main.Add_Child ( &pier );
	main.Add_Child ( &gio );

	main.Add_component < Fps > ( )->MAX_FPS = -1;
	main.Add_component < Phisiks > ( );
	camera.Add_component < Camera > ( );
	camera.Add_component < Camera_controll > ( );
	pier.Add_component < Sprite > ( );
	pier.Add_component < Rigidbody > ( )->velocity = {5,2,0};
	pier.Add_component < Sfere_Collider > ( )->Set_Size ( 100 );
	gio.Add_component < Sprite > ( );
	gio.Add_component < Rigidbody > ( )->mass = 100000000000000000;
	gio.Add_component < Sfere_Collider > ( )->Set_Size ( 200 );

	pier.Add_component < Gravitate > ( )->target = &gio;
	pier.Get_component < Gravitate > ( )->mass = LLONG_MAX;
	gio.Add_component < Gravitate > ( )->target = &pier;
	pier.Get_component < Gravitate > ( )->mass = 10;

	pier.Get_component < Sprite > ( ) ->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,1}, {0,0,1,1} )->frame = 0;
	
	gio.Get_component < Sprite > ( ) ->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,1} )->frame = 0;

	Scene_Manager::Set_Active_Scene ( &main );
	gio.Get_component < Sfere_Collider > ( )->Set_Static ( true );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::cout << "-------------- Start Main Loop -----------------\n";
	Main_Loop_FrameBuf ( );
	/*while (ReKat::grapik::IsEnd()) {
		glEnable(GL_DEPTH_TEST);
		Scene_Manager::Update();
		ReKat::grapik::Update();
	}*/
	std::cout << "\n-------------- Ended Main Loop -----------------\n";

	return result;
}
