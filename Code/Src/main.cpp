#include "Engine/load.h"
#include "Engine/controlls.h"
#include "Online/load.h"

float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

int main(int argc, char const *argv[]) {
    ReKat::grapik::Start ( "Kat Kave", 800, 600, false, false, true );
    std::cout << "icon set: " << ReKat::grapik::SetIcon ( "favicon.png" ) << '\n';;
	ReKat::synth::Start ( );

    ReKat::grapik::Input::_Keyboard = &__Keyboard;
    ReKat::grapik::Input::_Mouse_pos = &__Mouse_pos;
    ReKat::grapik::Input::_Mouse_key = &__Mouse_key;
    ReKat::grapik::Input::_ScrollWell = &__ScrollWell;
    ReKat::grapik::Input::_FreamBufferResize = &__FreamBufferResize;
    ReKat::grapik::Input::_Caracters = &__Caracters;

	int result = 0;
    result += Load_resources ( );
	result += Load_Audio ( );
	if ( result != 0 ) { return 0; }
    Create_Objs ( );
    Create_Scenes ( );

    Start_Online ( argc, argv );
    
    // connect to server
    if ( argc != 3 ) {
        //Manager::Set_Active_Scene ( "loading" );
        //Manager::Start ( );
        //Manager::Update ( );
        //online::Connect ( "server", "localhost" );
    }

    Manager::Set_Active_Scene ( "main" );
    Manager::Start ( );

	// Prepare framebuffer rectangle VBO and VAO
	/*unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Create Frame Buffer Object
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);*/

    int Fps = 0;
	int t = time (0);

    while ( ReKat::grapik::IsEnd ( ) ) {
		Fps ++;
		if ( t + 1 == time(0) ) { 
			t = time (0); 
			std::cout << "Fps: " << Fps << '\n'; 
			Fps = 0; 
		}
        Send_positon ( );
		
		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        Manager::Update ( );
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		Manager::Shader_Get( "Frame" )->Use();
		//glBindVertexArray(rectVAO);
		//glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded

		//glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		

        ReKat::grapik::Pool ( );	
    }
    std::cout << "End\n";
    End_Online ( );
	ReKat::synth::End();
    
    return 0;
}