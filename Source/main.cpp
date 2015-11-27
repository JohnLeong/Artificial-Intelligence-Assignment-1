#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFw/glfw3.h>
#include <ft2build.h>
#include "Vector2.h"
#include "Character.h"
#include "Character_Shopper.h"
#include "Character_Waiter.h"

#include FT_FREETYPE_H
using namespace std;

#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

// SHADERS //////////////////////////////////////////////////////////////////
const char *VERTEX_SHADER = ""
"#version 410 core\n"
"in vec4 in_Position;\n"
"out vec2 texCoords;\n"
"void main(void) {\n"
"    gl_Position = vec4(in_Position.xy, 0, 1);\n"
"    texCoords = in_Position.zw;\n"
"}\n";

const char *FRAGMENT_SHADER = ""
"#version 410 core\n"
"precision highp float;\n"
"uniform sampler2D tex;\n"
"uniform vec4 color;\n"
"in vec2 texCoords;\n"
"out vec4 fragColor;\n"
"void main(void) {\n"
"    fragColor = vec4(1, 1, 1, texture(tex, texCoords).r) * color;\n"
"}\n";

GLuint texUniform, colorUniform;
GLuint texture{ 0 }, sampler{ 0 };
GLuint vbo{ 0 }, vao{ 0 };
GLuint vs{ 0 }, fs{ 0 }, program{ 0 };

///////////////////////////////////////////////////////////////////

//Global varibles
const char *application_name = "AI Assignment 1";
float width, height;
GLFWwindow* window;

void Render( GLFWwindow* window );
void Init(void);
void Update(void);
void RunFSM(void);
void RenderObjects(void);
void RenderUI(void);

int RandomInteger( int lowerLimit, int upperLimit )
{
	return rand() % ( upperLimit - lowerLimit + 1 ) + lowerLimit;
}

// long integer to string
string itos( const long value )
{ 
	ostringstream buffer; 
	buffer << value; 
	return buffer.str();
}

/******************************************************************************/
/*!
\brief
Renders a circle outline
*/
/******************************************************************************/
void RenderCircle( GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b )
{
	int n = 360;
	glColor3f( r, g, b );
	glBegin( GL_POINTS );
	for ( int i = 0; i <= n; i++ )
	{
		float angle = (float) ( i * ( 2.0 * 3.14159 / n ) );
		glVertex2f( x + radius * cos( angle ), y + radius * sin( angle ) );
	}
	glEnd();
}
/******************************************************************************/
/*!
\brief
Renders full circle
*/
/******************************************************************************/
void RenderFillCircle( GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b )
{
	int n = 360;
	glColor3f ( r, g, b );
	glBegin(GL_TRIANGLE_FAN );
	glVertex2f( x, y );
	for (int i = 0; i <= n; i++)
	{
		float angle = (float) ( i * ( 2.0 * 3.14159 / n ) );
		glVertex2f( x + radius * cos( angle ), y + radius * sin( angle ) );
	}
	glEnd();
}
void RenderFillSquare(GLfloat x, GLfloat y, GLfloat size, GLfloat r, GLfloat g, GLfloat b)
{
	int n = 360;
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);

	glVertex2f(x + (0.5f * size), y + (-0.5f * size));
	glVertex2f(x + (-0.5f * size), y + (-0.5f * size));
	glVertex2f(x + (0.5f * size), y + (0.5f * size));
	glVertex2f(x + (-0.5f * size), y + (0.5f * size));
	glVertex2f(x + (0.5f * size), y + (0.5f * size));
	glVertex2f(x + (-0.5f * size), y + (-0.5f * size));
	glEnd();
}
void RenderBar(GLfloat x, GLfloat y, GLfloat Xsize, GLfloat Ysize, GLfloat r, GLfloat g, GLfloat b)
{
	int n = 360;
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);

	glVertex2f(x + (1.f * Xsize), y + (-0.5f * Ysize));
	glVertex2f(x + (0.f * Xsize), y + (-0.5f * Ysize));
	glVertex2f(x + (1.f * Xsize), y + (0.5f * Ysize));
	glVertex2f(x + (0.f * Xsize), y + (0.5f * Ysize));
	glVertex2f(x + (1.f * Xsize), y + (0.5f * Ysize));
	glVertex2f(x + (0.f * Xsize), y + (-0.5f * Ysize));
	glEnd();
}
/******************************************************************************/
/*!
\brief
Renders text on screen
*/
/******************************************************************************/
void RenderText(const string str, FT_Face face, float x, float y, float _sx, float _sy)
{
	float sx = _sx / width;
	float sy = _sy / height;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	const FT_GlyphSlot glyph = face->glyph;
	for (auto c : str) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
			continue;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
			glyph->bitmap.width, glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

		const float vx = x + glyph->bitmap_left * sx;
		const float vy = y + glyph->bitmap_top * sy;
		const float w = glyph->bitmap.width * sx;
		const float h = glyph->bitmap.rows * sy;

		struct {
			float x, y, s, t;
		} data[6] = {
			{ vx, vy, 0, 0 },
			{ vx, vy - h, 0, 1 },
			{ vx + w, vy, 1, 0 },
			{ vx + w, vy, 1, 0 },
			{ vx, vy - h, 0, 1 },
			{ vx + w, vy - h, 1, 1 }
		};
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (glyph->advance.x >> 6) * sx;
		y += (glyph->advance.y >> 6) * sy;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

static void ErrorCallBack ( int error, const char*description )
{
	fputs( description, stderr );
}

static void ResizeCallBack(GLFWwindow *window, int w, int h)
{
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60, (float) w / (float) h, 0, 100 );
	glMatrixMode( GL_MODELVIEW );
}

// Free Type //////////////////////////////////////////////////////
FT_Library ft_lib{ nullptr };
FT_Face face{ nullptr };

///////////////////////////////////////////////////////////////////

void Cleanup()
{
	FT_Done_Face(face);
	FT_Done_FreeType(ft_lib);
	glDeleteTextures(1, &texture);
	glDeleteSamplers(1, &sampler);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
}

void DoExit()
{
	Cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static void KeyCallBack( GLFWwindow *window, int key, int scancode, int action, int mods )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );
}

void ApplicationInit()
{
	// INIT ///////////////////////////////////////////////////////////////
	width = 1280;
	height = 600;

	glfwSetErrorCallback(ErrorCallBack);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow((int)width, (int)height, application_name, NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to create GLFW windows.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallBack);
	glfwSetWindowSizeCallback(window, ResizeCallBack);

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fprintf(stderr, "Error : %s\n", glewGetErrorString(error));
		exit(EXIT_FAILURE);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, width / height, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	///////////////////////////////////////////////////////////////////////////

	// Initialize and load our freetype face
	if (FT_Init_FreeType(&ft_lib) != 0)
	{
		fprintf(stderr, "Couldn't initialize FreeType library\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	if (FT_New_Face(ft_lib, "arial.ttf", 0, &face) != 0)
	{
		fprintf(stderr, "Unable to load arial.ttf\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}
	// Initialize our texture and VBOs
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenTextures(1, &texture);
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Initialize shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &VERTEX_SHADER, 0);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &FRAGMENT_SHADER, 0);
	glCompileShader(fs);

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// Initialize GL state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Get shader uniforms
	glUseProgram(program);
	glBindAttribLocation(program, 0, "in_Position");
	texUniform = glGetUniformLocation(program, "tex");
	colorUniform = glGetUniformLocation(program, "color");
}

//All simulation related variables here
std::vector<CCharacter *> character_list;	//vector list of all characters
std::vector<Vector2> seat_list;				//vector list of position of seats in restaraunt


int main()
{
	ApplicationInit();
	Init();
	while (!glfwWindowShouldClose(window))
	{
		Update();
		Render(window);
	}
	
	DoExit();
}

/******************************************************************************/
/*!
\brief
Inits the simulation
*/
/******************************************************************************/
void Init()
{
	srand((unsigned)time(NULL));
	int randomIndex = RandomInteger(1, 3);

	//Initialise characters here
	CCharacter *test = new CCharacter_Shopper();
	character_list.push_back(test);
	test->AddWaypoint(Vector2(5, 5));
	test->SetState(CCharacter::STATE_PATROL);
}
/******************************************************************************/
/*!
\brief
Updates the simulation
*/
/******************************************************************************/
void Update()
{
	for (unsigned i = 0; i < character_list.size(); ++i)
	{
		character_list[i]->Update();
	}
}
/******************************************************************************/
/*!
\brief
Renders the simulation
*/
/******************************************************************************/
void Render( GLFWwindow* window )
{
	glUseProgram(0);
	glClear(GL_COLOR_BUFFER_BIT);

	string stateString = "";
	RenderObjects();

	// Bind stuff
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindSampler(0, sampler);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glUseProgram(program);
	glUniform4f(colorUniform, 1, 1, 1, 1);
	glUniform1i(texUniform, 0);

	FT_Set_Pixel_Sizes(face, 0, 50);

	RenderText("State : ", face, -0.95f, 0.925f, 0.55f, 0.55f);
	RenderText(stateString, face, -0.8f, 0.925f, 0.55f, 0.55f);
	RenderText("Player - Blue Enemy - Green Red - Patrol Point", face, -0.6f, 0.925f, 0.55f, 0.55f);

	RenderUI();

	glfwSwapBuffers(window);
	glfwPollEvents();

}

void RenderObjects(void)
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -10.0f);//Render behind text?
	static float nono = 1.f;
	nono += 0.005f;
	//Render characters
	for (unsigned i = 0; i < character_list.size(); ++i)
	{
		RenderFillCircle(character_list[i]->GetXPos(), character_list[i]->GetYPos(), 1.f, 1.0, 0.f, 0.f);
		RenderBar(character_list[i]->GetXPos() - 1.f, character_list[i]->GetYPos() + 1.f, 2.f, 1.f, 0.0, 1.f, 0.f);
	}

	glPopMatrix();
}

void RenderUI(void)
{
	//Render character UI state
	for (unsigned i = 0; i < character_list.size(); ++i)
	{
		RenderText("Test", face, character_list[i]->GetXPos() / 10, character_list[i]->GetYPos() / 10, 0.55f, 0.55f);
	}
}