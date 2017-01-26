#include "Game.h"

static bool flip;

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Vertex and Fragment Shaders")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[4];
} Vertex;

Vertex vertex[8];
GLubyte triangles[38];

/* Variable to hold the VBO identifier and shader data */
GLuint	index, //Index to draw
		vsid, //Vertex Shader ID
		fsid, //Fragment Shader ID
		progID, //Program ID
		vao = 0, //Vertex Array ID
		vbo[1], // Vertex Buffer ID
		positionID, //Position ID
		colorID; // Color ID


void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	/* Vertices counter-clockwise winding */
	//Lb
	vertex[0].coordinate[0] = -0.5f;
	vertex[0].coordinate[1] = -0.5f;
	vertex[0].coordinate[2] = 0.0f;

	//LT
	vertex[1].coordinate[0] = -0.5f;
	vertex[1].coordinate[1] = 0.5f;
	vertex[1].coordinate[2] = 0.0f;

	//RT
	vertex[2].coordinate[0] = 0.5f;
	vertex[2].coordinate[1] = 0.5f;
	vertex[2].coordinate[2] = 0.0f;

	//RB
	vertex[3].coordinate[0] = 0.5f;
	vertex[3].coordinate[1] = -0.5f;
	vertex[3].coordinate[2] = 0.0f;

	//LBB
	vertex[4].coordinate[0] = -0.5f;
	vertex[4].coordinate[1] = -0.5f;
	vertex[4].coordinate[2] = -1.0f;

	//LBT
	vertex[5].coordinate[0] = -0.5f;
	vertex[5].coordinate[1] = 0.5f;
	vertex[5].coordinate[2] = -1.0f;

	//RBT
	vertex[6].coordinate[0] = 0.5f;
	vertex[6].coordinate[1] = 0.5f;
	vertex[6].coordinate[2] = -1.0f;

	//RBB
	vertex[7].coordinate[0] = 0.5f;
	vertex[7].coordinate[1] = -0.5f;
	vertex[7].coordinate[2] = -1.0f;

	/*Assigning Colours to Vertices */
	//vertex[0].color[0] = 0.0f;
	//vertex[0].color[1] = 0.0f;
	//vertex[0].color[2] = 0.0f;
	//vertex[0].color[3] = 1.0f;

	//vertex[1].color[0] = 0.0f;
	//vertex[1].color[1] = 0.0f;
	//vertex[1].color[2] = 1.0f;
	//vertex[1].color[3] = 0.0f;

	//vertex[2].color[0] = 0.0f;
	//vertex[2].color[1] = 1.0f;
	//vertex[2].color[2] = 0.0f;
	//vertex[2].color[3] = 0.0f;

	//vertex[3].color[0] = 1.0f;
	//vertex[3].color[1] = 0.0f;
	//vertex[3].color[2] = 0.0f;
	//vertex[3].color[3] = 0.0f;

	//vertex[4].color[0] = 0.0f;
	//vertex[4].color[1] = 0.0f;
	//vertex[4].color[2] = 1.0f;
	//vertex[4].color[3] = 1.0f;

	//vertex[5].color[0] = 0.0f;
	//vertex[5].color[1] = 1.0f;
	//vertex[5].color[2] = 0.0f;
	//vertex[5].color[3] = 1.0f;

	//vertex[6].color[0] = 1.0f;
	//vertex[6].color[1] = 0.0f;
	//vertex[6].color[2] = 0.0f;
	//vertex[6].color[3] = 1.0f;

	//vertex[7].color[0] = 0.0f;
	//vertex[7].color[1] = 1.0f;
	//vertex[7].color[2] = 1.0f;
	//vertex[7].color[3] = 0.0f;
	
	/*Index of Poly / Triangle to Draw */
	//Front
	triangles[0] = 2;   triangles[1] = 1;   triangles[2] = 0;
	triangles[3] = 2;   triangles[4] = 0;   triangles[5] = 3;

	//Back
	triangles[6] = 5;  triangles[7] = 6;   triangles[8] = 7;
	triangles[9] = 5;  triangles[10] = 7;  triangles[11] = 4;

	//Left
	triangles[12] = 1;  triangles[13] = 5;  triangles[14] = 4;
	triangles[15] = 1;  triangles[16] = 4;  triangles[17] = 0;

	//Right
	triangles[18] = 6;   triangles[19] = 2;   triangles[20] = 3;
	triangles[21] = 6;   triangles[22] = 3;   triangles[23] = 7;

	//Top
	triangles[24] = 6;  triangles[25] = 5;  triangles[26] = 1;
	triangles[27] = 6;  triangles[28] = 1;  triangles[29] = 2;

	//Bottom
	triangles[30] = 3;  triangles[31] = 0;  triangles[32] = 4;
	triangles[33] = 3;  triangles[34] = 4;  triangles[35] = 7;

	glEnable(GL_CULL_FACE);
	

	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	const char* vs_src ="#version 400\n\r"
						"in vec4 sv_position;"
						"in vec4 sv_color;"
						"out vec4 color;"
						"void main() {"
						"	color = sv_color;"
						"	gl_Position = sv_position;"
						"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid); //Check that the shader compiles

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */
	const char* fs_src ="#version 400\n\r"
						"in vec4 color;"
						"out vec4 fColor;"
						"void main() {"
						"	fColor = color + vec4(1.0f, 0.0f, 1.0f, 1.0f);"
						"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	// Find variables in the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f)
	{
		clock.restart();

		flip = true;

		if (!flip)
		{
			flip = true;
		}
		else
			flip = false;
	}

	if (flip)
	{
		rotationAngle += 0.005f;

		if (rotationAngle > 360.0f)
		{
			rotationAngle -= 360.0f;
		}
	}

	//Change vertex data
	vertex[0].coordinate[0] += -0.0001f;
	vertex[0].coordinate[1] += -0.0001f;
	vertex[0].coordinate[2] += -0.0001f;

	vertex[1].coordinate[0] += -0.0001f;
	vertex[1].coordinate[1] += -0.0001f;
	vertex[1].coordinate[2] += -0.0001f;

	vertex[2].coordinate[0] += -0.0001f;
	vertex[2].coordinate[1] += -0.0001f;
	vertex[2].coordinate[2] += -0.0001f;
	
	vertex[3].coordinate[0] += -0.0001f;
	vertex[3].coordinate[1] += -0.0001f;
	vertex[3].coordinate[2] += -0.0001f;

	//vertex[4].coordinate[0] += -0.0001f;
	//vertex[4].coordinate[1] += -0.0001f;
	//vertex[4].coordinate[2] += -0.0001f;

	//vertex[5].coordinate[0] += -0.0001f;
	//vertex[5].coordinate[1] += -0.0001f;
	//vertex[5].coordinate[2] += -0.0001f;
	//
	//vertex[6].coordinate[0] += -0.0001f;
	//vertex[6].coordinate[1] += -0.0001f;
	//vertex[6].coordinate[2] += -0.0001f;

	//vertex[7].coordinate[0] += -0.0001f;
	//vertex[7].coordinate[1] += -0.0001f;
	//vertex[7].coordinate[2] += -0.0001f;

#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	window.display();

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, vbo);
}

