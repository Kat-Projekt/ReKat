#ifndef TEXT_H
#define TEXT_H

#include "shader.h"
#include "texture.h"

#include <map>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

class Text {
public:
    enum Status {
        SUCCESS,
        FAILED_INIT_FREETYPE,
        FAILED_LOAD_FONT,
        SHADER_NOT_SET
    };

    enum Text_guistifiaction {
        RIGHT,
        CENTER,
        LEFT,
        GIUSTIFY
    };

    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    FT_Library ft;
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    Shader *_shader = nullptr;

    Text ( ) { }
    Text ( std::string font, Shader *shader ) { Make ( font, shader ); }

    int Make ( std::string font, Shader *shader ) {
        _shader = shader;

        if ( FT_Init_FreeType ( &ft ) ) 
        { return FAILED_INIT_FREETYPE; }

        // find path to font
        if ( font.empty ( ) ) { return FAILED_LOAD_FONT; }
        
        // load font as face
        FT_Face face;
        if ( FT_New_Face ( ft, font.c_str(), 0, &face ) ) { return FAILED_LOAD_FONT; }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++) {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER)) { continue; }
                // generate texture
                unsigned int texture = Texture::Make ( face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1 );
                // now store character for later use
                Character character = { texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert( {c, character} );
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        
        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return SUCCESS;
    }

    int Get_Max_Rows ( float heigth, int *wrap_h_out = nullptr, int wrap_h = -1 ) {
        float max_size = 0;
        float max_bearing = 0;
        if ( wrap_h == -1 ) {
            for ( auto c : Characters ) {
                if ( max_size < c.second.Size.y ) { max_size = c.second.Size.y; }
                if ( max_bearing < c.second.Bearing.y ) { max_bearing = c.second.Bearing.y; }
            }
            
            wrap_h = (int)(max_bearing); }
        if ( wrap_h_out != nullptr ) { *wrap_h_out = wrap_h; }
        return (int)(heigth / wrap_h);
    }

    // creates a box for the text (the bottom left corner is {0,0}) and start to render from the top of the box 
    // if wrap_h is negative the heigth of the A * 1.5 will be used
    // @return rendered lines
    // start_line > 0
    int RenderText ( std::string text, glm::vec2 pos, glm::vec2 dim, float scale, glm::vec4 color = { 0, 0 ,0, 1 }, 
    int start_rows = 0, int wrap_h = -1, Text_guistifiaction text_guistifiaction = LEFT ) {
        if ( _shader == nullptr ) { return SHADER_NOT_SET; }
        // calcualte max aviable rows
        int max_rows = Get_Max_Rows ( dim.y, &wrap_h );

        _shader->Use();
        _shader->setVec4 ( "textColor", color );
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // count rows
        std::vector < std::string > lines = {""};
        int lines_to_render = 0;
        float x = pos.x;
        for ( char c : text ) {
            Character ch = Characters[c];
            // line counter
            // -- text wrapping
            if ( x + ch.Size.x >= ( dim.x + pos.x ) || c == '\n' ) 
            { lines_to_render++; lines.push_back(""); x = pos.x; }
            // -- newline
            if ( c == '\n' ) { continue; }
            lines[lines_to_render] += c;
            x += (ch.Advance >> 6) * scale;
        }

        // find render limits
        int first_line = 0;           // min
        int last_line = lines.size(); // max

        // calculate first line
        first_line = start_rows;
        first_line > lines.size() ? first_line = lines.size() : 0;

        // calculate last line
        last_line = first_line;
        // calcolo il rimanente
        last_line += ( lines.size() - first_line );
        last_line > ( max_rows + first_line ) ? last_line = max_rows + first_line : 0;

        // render
        float y = pos.y - wrap_h * scale;
        int rendered_lines = 0;
        for ( size_t line_index = (first_line < 0 ? 0 : first_line); line_index < last_line; line_index++ ) {
            rendered_lines ++;
            std::string line = lines[line_index];
            x = pos.x;
            for ( char c : line ) {
                Character ch = Characters[c];
                // rendering from top to bottom
                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;
                // update VBO for each character
                float vertices[] = {
                    xpos,     ypos + h,   0.0f, 0.0f,
                    xpos,     ypos,       0.0f, 1.0f,
                    xpos + w, ypos,       1.0f, 1.0f,

                    xpos,     ypos + h,   0.0f, 0.0f,
                    xpos + w, ypos,       1.0f, 1.0f,
                    xpos + w, ypos + h,   1.0f, 0.0f
                };
                /*float vertices[] = { 
                    // pos                // tex
                    xpos,     ypos + h,   0.0f, 1.0f,
                    xpos + w, ypos,       1.0f, 0.0f,
                    xpos,     ypos,       0.0f, 0.0f, 

                    xpos,     ypos + h,   0.0f, 1.0f,
                    xpos + w, ypos + h,   1.0f, 1.0f,
                    xpos + w, ypos,       1.0f, 0.0f
                };*/

                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }
            y -= wrap_h * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return lines.size() - 1;
        // return RenderText ( *_shader, text, pos.x, pos.y, scale, color, dimensions.x, wrap_h, max_rows, start_rows );
    }
    int RenderText ( Shader &shader, std::string text, float init_x, float init_y, float scale, glm::vec3 color, float to_wrap, float wrap_h, int Max_Rows, int Start_Row ) {
        // activate corresponding render state	
        shader.Use();
        glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        init_y = init_y;
        int x = init_x, y = init_y;

        y -= (scale * wrap_h ) * Start_Row;
        int last_y = init_y - (scale * wrap_h ) * Max_Rows;
        int Rows = 0;
    
        for (c = text.begin(); c != text.end(); c++) {
            // new line check
            if ( *c == '\n' ) { y -= (scale * wrap_h ); Rows++; x = init_x; continue; }
            // last line chiek
            Character ch = Characters[*c];
            // text wrapping
            if ( x + ch.Size.x >= ( to_wrap + init_x ) ) { y -= (scale * wrap_h ); Rows++; x = init_x; }
            if ( ! ( y <= last_y ) ) { 
                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;
                // update VBO for each character
                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },            
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }           
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }

        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return Rows;
    }
};

#endif