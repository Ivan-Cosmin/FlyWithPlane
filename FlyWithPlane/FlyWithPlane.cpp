#include "ShaderManager.h"
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const GLchar* VertexShader =
{
	"#version 400\n"\
	"layout(location=0) in vec4 in_Position;\n"\
	"layout(location=1) in vec4 in_Color;\n"\
	"out vec4 ex_Color;\n"\
	"uniform mat4 WorldMatrix;\n"\
	"void main()\n"\
	"{\n"\
	"  gl_Position = WorldMatrix * in_Position;\n"\
	"  ex_Color = in_Color;\n"\
	"}\n"
};
// Shader-ul de fragment / Fragment shader (este privit ca un sir de caractere)
const GLchar* FragmentShader =
{
	"#version 400\n"\
	"in vec4 ex_Color;\n"\
	"out vec4 out_Color;\n"\
	"void main()\n"\
	"{\n"\
	"  out_Color = ex_Color;\n"\
	"}\n"
};



int main() {
	try {
		Screen app;
		app.run();
		ShaderManager shader(VertexShader,FragmentShader);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}