#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 8.0f, 10.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;

// Variables de animación
float rot = 0.0f;
float posicion = 0.0f;
float rotpuertaCasa = 0.0f;
bool animpuertaCasa = false;
bool animLamp = false;
bool activLamp = false;
float rotLamp = 0.0f;
bool animSofa = false;
float rotSofa = 0.0f;
float rotPuertaIz = 0.0f;
float rotPuertaDe = 0.0f;
float rotPuertaRefri = 0.0f;
bool animRefri = false;
bool animAlacena = false;
float rotSilla = 0.0f;
float rotSilla3 = 0.0f;
bool animSilla = false;
bool animSilla3 = false;
float rotMesa = 0.0f;
bool animMesa = false;


bool animCajon = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
float movx = 0.0f;
float movz = 0.0f;
float movy = 0.0f;
float rotCajon = 0.0;

// Light attributes
glm::vec3 lightPos(-5.0f, 11.0f, 0.0f);
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

//// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z;
//
//#define MAX_FRAMES 9
//int i_max_steps = 190;
//int i_curr_steps = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-5.0f,11.0f, 0.0f),
	glm::vec3(0.0f,0.0f,0.0f),
	glm::vec3(0.0f,0.0f,0.0f),
	glm::vec3(0.0f,0.0f,0.0f)
};

// Position of the spotlight
glm::vec3 SpotLightPosition[] = {
	glm::vec3(0.0f,0.0f,0.0f)
};

// Direction of the spotlight
glm::vec3 dirSpotLightPosition[] = {
	glm::vec3(-0.2f, -1.0f, -0.3f)
};

glm::vec3 Light1 = glm::vec3(0);

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	Model casaCuarto2((char*)"Models/Casa/casaCuarto2.obj");
	Model puertaCasa((char*)"Models/Casa/puerta.obj");
	Model Lampara((char*)"Models/Lampara/lampara.obj");
    Model base((char*)"Models/Lampara/base.obj");
	Model Sofa((char*)"Models/Sala/Sofa.obj");
	Model Alacena((char*)"Models/Alacena/alacena.obj");
	Model puertaDerecha((char*)"Models/Alacena/puertaDerecha.obj");
	Model puertaIzquierda((char*)"Models/Alacena/puertaIzquierda.obj");
	Model Refri((char*)"Models/Refri/refri.obj");
	Model puerta1((char*)"Models/Refri/puerta1.obj");
	Model puerta2((char*)"Models/Refri/puerta2.obj");
	Model mesa((char*)"Models/Silla/mesa.obj");
	Model silla((char*)"Models/Silla/silla.obj");
	Model Buro((char*)"Models/Buro/buro.obj");
	Model Cajon((char*)"Models/Buro/cajon.obj");
	Model Jarron((char*)"Models/Jarron/Jarron.obj");
	Model Caballete((char*)"Models/Caballete/Caballete.obj");
	Model Cuadro((char*)"Models/Cuadro/Cuadro.obj");
	Model Banco((char*)"Models/Banco/Banco.obj");
	Model lamp((char*)"Models/Lampara/lamp.obj");
	Model Cuarto2((char*)"Models/Habitación/Cuarto2.obj");


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin((1.0f, 1.0f, 1.0f) * Light1.x));
		lightColor.y = abs(sin((1.0f, 1.0f, 1.0f) * Light1.y));
		lightColor.z = sin(sin((1.0f, 1.0f, 1.0f) * Light1.z));

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.017f);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.017f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.017f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.017f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0.0f,0.0f,0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, 5.0f, 5.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(20.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(25.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

		//Carga de modelo 
		//Casa Cuarto
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, 18.9f, 0.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		casaCuarto2.Draw(lightingShader);

		//Puerta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.0f, 10.0f, 17.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotpuertaCasa), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaCasa.Draw(lightingShader);

		//Lampara
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.5, 17.7, 0));
		model = glm::translate(model,glm::vec3(posX,posY,posZ));
		model = glm::rotate(model, glm::radians(rotLamp), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(lightingShader);

		//base
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(-4.5, 17.7, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		base.Draw(lightingShader);

		//Sofa
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(9, 0.9, 6));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotSofa), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sofa.Draw(lightingShader);

		//Alacena
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(6, 8, -10.5));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena.Draw(lightingShader);

		//Puerta1
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(3.9, 8, -11.8));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPuertaIz), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaIzquierda.Draw(lightingShader);

		//Puerta2
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(5.7f, 8.0f, -10.8f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPuertaIz), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaIzquierda.Draw(lightingShader);

		//Puerta4
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(8.9f, 8.0f, -8.9f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPuertaIz), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaIzquierda.Draw(lightingShader);

		//Puerta3
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(6.1, 8, -10.5));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPuertaDe), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaDerecha.Draw(lightingShader);

		//Refri
	    model = glm::mat4(1);
	    model = glm::translate(tmp, glm::vec3(0, 6.7, -10));
	    model = glm::translate(model, glm::vec3(posX, posY, posZ));
	    model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	    model = glm::scale(model, glm::vec3(3.5f, 2.3, 3.0));
	    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	    Refri.Draw(lightingShader);

	   //Puerta1
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(0, 6.7, -10));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::rotate(model, glm::radians(rotPuertaRefri), glm::vec3(0.0f, -1.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(3.5f, 2.3, 3.0));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   puerta1.Draw(lightingShader);

	   //Puerta2
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(0, 3.2, -10));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::rotate(model, glm::radians(rotPuertaRefri), glm::vec3(0.0f, -1.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(3.5f, 2.3, 3.0));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   puerta2.Draw(lightingShader);

	   //Mesa
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-8, 1.1, 0));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(rotMesa), glm::vec3(-1.0f, 0.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(0.9f, 0.7, 0.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   mesa.Draw(lightingShader);

	   //Silla 1
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-3, 1.2, 0));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::rotate(model, glm::radians(rotSilla), glm::vec3(-1.0f, 0.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(0.9f, 0.7, 0.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   silla.Draw(lightingShader);


	   //Silla 2
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-6.5, 1.2, -4));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::rotate(model, glm::radians(rotSilla), glm::vec3(-1.0f, 0.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(0.9f, 0.7, 0.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   silla.Draw(lightingShader);


	   //Silla 3
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-12, 1.2, -1.2));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::rotate(model, glm::radians(rotSilla3), glm::vec3(-1.0f, 0.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(0.9f, 0.7, 0.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   silla.Draw(lightingShader);

	   //Silla 4
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-8.5, 1.2, 4));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(170.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::rotate(model, glm::radians(rotSilla), glm::vec3(-1.0f, 0.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(0.9f, 0.7, 0.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   silla.Draw(lightingShader);


	   //Buro
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(10, 2.8, -8));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(2.0f, 2.5, 2.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Buro.Draw(lightingShader);

	   //Cajon 1
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(10.0f+movx, 2.8+movy, -8.0f+movz));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::rotate(model, glm::radians(rotCajon), glm::vec3(1.0f, 0.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(2.0f, 2.5, 2.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Cajon.Draw(lightingShader);

	   //Cajon 2
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(8, 2.8, -10.2));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(2.0f, 2.5, 2.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Cajon.Draw(lightingShader);

	   //Cajon 3
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(8, 1.5, -10.2));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(2.0f, 2.5, 2.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Cajon.Draw(lightingShader);

	   //Cajon 4
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(10, 1.5, -8));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	   model = glm::scale(model, glm::vec3(2.0f, 2.5, 2.9));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Cajon.Draw(lightingShader);

	   //Jarron
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-1, 19.2, -3));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   /*model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
	   model = glm::scale(model, glm::vec3(2.8f, 2.3, 2.8));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Jarron.Draw(lightingShader);

	   //Caballete
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-4.0, 19.1, -1.0));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   /*model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
	   model = glm::scale(model, glm::vec3(1.5f, 1.3, 1.5));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Caballete.Draw(lightingShader);

	   //Cuadro
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(3.0, 21.5, -7.8));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   model = glm::scale(model, glm::vec3(0.7f, 0.7, 0.7));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Cuadro.Draw(lightingShader);

	   //Banco
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(5.0, 19.0, -1.0));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   /*model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
	   model = glm::scale(model, glm::vec3(1.5f, 1.5, 1.5));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Banco.Draw(lightingShader);

	   //Lampara Cuarto 2
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(-2.0, 24.0, 0.0));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   /*model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
	   model = glm::scale(model, glm::vec3(2.0f, 2.5, 2.0));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   lamp.Draw(lightingShader);

	   //Cuarto 2
	   model = glm::mat4(1);
	   model = glm::translate(tmp, glm::vec3(0.0, 18.9, 0.0));
	   model = glm::translate(model, glm::vec3(posX, posY, posZ));
	   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	   Cuarto2.Draw(lightingShader);

		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		glBindVertexArray(0);

		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();


	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_2])
	{
		active = !active;
		if (active)
			Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
		else
			Light1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	if (keys[GLFW_KEY_1])
	{
		animSofa = true;
	}

	if (keys[GLFW_KEY_2])
	{
		activLamp = true;
		animLamp = !animLamp;
	}

	if (keys[GLFW_KEY_3])
	{
		animpuertaCasa = true;
	}

	if (keys[GLFW_KEY_4])
	{
		animAlacena = true;
	}
	
	if (keys[GLFW_KEY_5])
	{
		animSilla = true;
		animMesa = true;
		animSilla3 = true;
	}

	if (keys[GLFW_KEY_6])
	{
		animRefri = true;
	}

	if (keys[GLFW_KEY_7])
	{
		animCajon = true;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	//Animación Sofa
	if (animSofa)
	{
		if (rotSofa > -87.0f)
			rotSofa -= 3.5f;
		else {
			animSofa = false;
		}
	}

	//Animación Lampara
	if (animLamp) {
		if (activLamp)
			rotLamp -= 3.0f;
	}

	//Animación PuertaCasa
	if (animpuertaCasa)
	{
		if (rotpuertaCasa > -105.0f)
			rotpuertaCasa -= 2.5f;
		else {
			animpuertaCasa = false;
		}
	}

	//Animación Alacena
	if (animAlacena) {

		if (rotPuertaDe > -90.0f)
			rotPuertaDe -= 1.5f;

		if (rotPuertaIz < 90.0f)
			rotPuertaIz += 1.5f;
	}

	//Animación Sillas 1, 2 y 4
	if (animSilla) {

		if (rotSilla < 87.0f)
			rotSilla += 3.5f;
	}

	//Animación Silla 3
	if (animSilla3) {

		if (rotSilla3 < 31.5f)
			rotSilla3 += 3.5f;
	}

	//Animación Mesa
	if (animMesa) {

		if (rotMesa < 25.0f)
			rotMesa += 3.5f;
	}

	//Animación Refri
	if (animRefri) {

		if (rotPuertaRefri > -90.0f)
			rotPuertaRefri -= 3.0f;
	}

	//Animación Cajón
	if (animCajon)
	{
		if (recorrido1) {

			if (movz > 3.0f && movx < -3.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
			else
				rotCajon += 2.0f;
				movz += 0.2f;
				movx -= 0.2f;
		}

		if (recorrido2) {

			if (movy < -0.059f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
			else 
				if (rotCajon < 80.0f) {
					rotCajon += 9.0f;
					movy -= 0.01f;
					movz += 0.2f;
					movx -= 0.2f;
				}
		}

		if (recorrido3) {
			if (movy < -4.5f)
			{
				recorrido3 = false;
			}
			else
				if (rotCajon < 180.0f) {
					rotCajon += 10.0f;
					if (rotCajon > 160.0f) {
						movy -= 0.25f;
					}
				}
		}
	}
}