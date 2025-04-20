/*
Hernandez Ramirez Miguel Angel
Osorio Angeles Rodrigo Jafet
Del Razo Sanchez Diego Adrian
Proyecto 1 Equipo 11
11 de abril del 2025
319044618
318008893
319114939
*/
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

#include <vector>


//ANIMAICION


float globalTranslationYInicial = 7.0f;
float globalTranslationYFinal = 0.5f;
bool animacionXTerminada = false;

glm::vec3 globalTranslationX = glm::vec3(0.0f, globalTranslationYInicial, -25.8f);
glm::vec3 globalScaleX = glm::vec3(4.0f, 4.0f, 4.0f);
float globalRotationX = 90.0f;

bool bajandoY = false;
float velocidadBajadaY = 4.0f; // puedes ajustarlo si quieres más lento


bool reduciendoEscala = false;
float escalaTemporal = 4.0f;





// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
bool explosionActive = false;



float posBallY = 0.0f;  // Posición inicial de la pelota en Y
float velocidad = 2.0f;  // Velocidad de la animación (puedes ajustarla)
bool moviendoArriba = true;  // Variable para controlar la dirección de la pelota
// Control de la animación
bool animacionActivada = false;  // Animación inicialmente desactivada


float limiteInferior = 0.0f;  
float limiteSuperior = 1.7;   
float explosionFactor = 0.0f;

float implosionFactor = 0.0f;
bool implosionActive = false;

// Control de visibilidad y animación de modelos
bool model1Visible = true;
bool model2Visible = false;
bool model2Appearing = false;
float model2ScaleFactor = 0.0f;
float model2Rotation = 0.0f;
float model2TargetRotation = 1080.0f; // 3 vueltas completas

const float escalaX = 2.0f;
const float escalaY = 3.2f;
const float escalaZ = 4.0f;

// Para modelo 'ga'
bool gaExplosionActive = false;
bool gaVisible = true;
float gaExplosionFactor = 1.0f;
bool gaInflating = false;
bool gaContracting = false;


bool ganAppearing = false;
float ganRotation = 0.0f;
float ganTargetRotation = 270.0f;

// Agregar variables globales para animación del modelo g
float gOffsetX = -0.5f;
bool gMovingOut = false;
bool gReturning = false;
bool gAnimActive = false;

// Para modelo 'g'
bool gVisible = false;
bool gAppearing = false;
float gScaleFactor = 0.0f;
float gRotation = 0.0f;
float gTargetRotation = 180.0f;

// Variables para modelo 'gr'
bool grVisible = false;
bool grAppearing = false;
float grScaleFactor = 0.0f;
float grRotation = 0.0f;
float grTargetRotation = 360.0f;

float grOffsetX = -0.5f;
bool grMovingOut = false;
bool grReturning = false;
bool grAnimActive = false;

bool ramVisible = false;
bool ramAppearing = false;
float ramScaleFactor = 0.0f;

bool grAppearingScale = false;  // Ya existe `grAppearing` pero lo usamos para rotación, así que usamos otro para la escala

bool ramAnimActive = false;
bool ramMovingOut = false;
bool ramReturning = false;
float ramOffsetX = -0.5f;

static bool pasoExtraGr = false;
static bool pasoExtraRam = false;

bool animacionXCompleta = false;


// Estados por modelo para X
bool gInPhase1 = false, gInPhase2 = false;
bool grInPhase1 = false, grInPhase2 = false;
bool ramInPhase1 = false, ramInPhase2 = false;

float ramRotation = 0.0f;
float ramTargetRotation = 360.0f;


bool prVisible = false;
bool prAnimActive = false;
bool prInPhase1 = false, prInPhase2 = false;
float prScaleFactor = 0.0f;
float prRotation = 0.0f;
float prOffsetX = -0.5f;

// Variables para modelo 'fu'
bool fuVisible = false;
bool fuAnimActive = false;
bool fuInPhase1 = false, fuInPhase2 = false;
float fuScaleFactor = 0.0f;
float fuRotation = 0.0f;
float fuOffsetX = -0.5f;
float fuTargetRotation = 180.0f;

bool ganFinished = false;

bool gaAntesDeX = false;

bool ganVisible = false;     // Antes estaba en true
float ganScaleFactor = 1.0f;
bool ganDisappearing = false;



struct InstanciaAnimacion {
	glm::vec3 posicion;
	glm::vec3 escala;
	float rotacionBase;
	float escalaTemporal;
	float translationY;
	bool visible;

	// Estados de aparición
	bool gaVisible, gaExplosionActive, gaInflating, gaContracting;
	float gaExplosionFactor;
	bool animacionEstatica;  // true para los 3 nuevos modelos sin escala ni traslación

	bool ganVisible, ganAppearing;
	float ganScaleFactor, ganRotation;

	bool ganFinished;

	bool gVisible, gAppearing, gAnimActive;
	float gScaleFactor, gRotation, gOffsetX;
	float gTargetRotation;
	bool gInPhase1, gInPhase2;

	bool grVisible, grAnimActive;
	float grScaleFactor, grRotation, grOffsetX;
	float grTargetRotation;
	bool grInPhase1, grInPhase2;

	bool ramVisible, ramAnimActive;
	float ramScaleFactor, ramRotation, ramOffsetX;
	float ramTargetRotation;
	bool ramInPhase1, ramInPhase2;

	bool prVisible, prAnimActive;
	float prScaleFactor, prRotation, prOffsetX;
	bool prInPhase1, prInPhase2;

	bool fuVisible, fuAnimActive;
	float fuScaleFactor, fuRotation, fuOffsetX;
	float fuTargetRotation;
	bool fuInPhase1, fuInPhase2;

	bool reduciendoEscala;
	bool bajandoY;
	bool escala5Set;


	InstanciaAnimacion(glm::vec3 pos, bool estatica = false)
	{
		animacionEstatica = estatica;
		posicion = pos;
		escala = glm::vec3(2.0f);
		rotacionBase = 90.0f;
		escalaTemporal = 4.0f;
		translationY = estatica ? pos.y : globalTranslationYInicial;

		visible = true;

		gaVisible = true; gaExplosionActive = true; gaInflating = true; gaContracting = false;
		gaExplosionFactor = 1.0f;

		ganVisible = false; ganAppearing = false;
		ganScaleFactor = 1.0f; ganRotation = 0.0f;

		ganFinished = false;

		gVisible = false; gAppearing = false; gAnimActive = false;
		gScaleFactor = 0.0f; gRotation = 0.0f; gOffsetX = -0.5f;
		gTargetRotation = 180.0f;
		gInPhase1 = false; gInPhase2 = false;

		grVisible = false; grAnimActive = false;
		grScaleFactor = 0.0f; grRotation = 0.0f; grOffsetX = -0.5f;
		grTargetRotation = 360.0f;
		grInPhase1 = false; grInPhase2 = false;

		ramVisible = false; ramAnimActive = false;
		ramScaleFactor = 0.0f; ramRotation = 0.0f; ramOffsetX = -0.5f;
		ramTargetRotation = 360.0f;
		ramInPhase1 = false; ramInPhase2 = false;

		prVisible = false; prAnimActive = false;
		prScaleFactor = 0.0f; prRotation = 0.0f; prOffsetX = -0.5f;
		prInPhase1 = false; prInPhase2 = false;

		fuVisible = false; fuAnimActive = false;
		fuScaleFactor = 0.0f; fuRotation = 0.0f; fuOffsetX = -0.5f;
		fuTargetRotation = 180.0f;
		fuInPhase1 = false; fuInPhase2 = false;

		reduciendoEscala = false;
		bajandoY = false;
		escala5Set = false;

	}
};


	
struct AnimacionXGroup {
	glm::vec3 globalTranslationX;
	glm::vec3 globalScaleX;
	float globalRotationX;


	bool ganVisible = true;
	bool ganAppearing = false;
	float ganRotation = 0.0f;
	float ganTargetRotation = 270.0f;
	float ganScaleFactor = 0.0f;
	bool ganFinished = false;

	bool gVisible = false;
	bool gAppearing = false;
	bool gAnimActive = false;
	bool gInPhase1 = false;
	bool gInPhase2 = false;
	float gOffsetX = -0.5f;
	float gScaleFactor = 0.0f;
	float gRotation = 0.0f;
	float gTargetRotation = 180.0f;

	bool grVisible = false;
	bool grAppearing = false;
	bool grAnimActive = false;
	bool grInPhase1 = false;
	bool grInPhase2 = false;
	float grOffsetX = -0.5f;
	float grScaleFactor = 0.0f;
	float grRotation = 0.0f;
	float grTargetRotation = 360.0f;

	bool ramVisible = false;
	bool ramAnimActive = false;
	bool ramInPhase1 = false;
	bool ramInPhase2 = false;
	float ramOffsetX = -0.5f;
	float ramScaleFactor = 0.0f;
	float ramRotation = 0.0f;
	float ramTargetRotation = 360.0f;

	bool prVisible = false;
	bool prAnimActive = false;
	bool prInPhase1 = false;
	bool prInPhase2 = false;
	float prOffsetX = -0.5f;
	float prScaleFactor = 0.0f;
	float prRotation = 0.0f;

	bool fuVisible = false;
	bool fuAnimActive = false;
	bool fuInPhase1 = false;
	bool fuInPhase2 = false;
	float fuOffsetX = -0.5f;
	float fuScaleFactor = 0.0f;
	float fuRotation = 0.0f;
	float fuTargetRotation = 180.0f;

	bool animacionXCompleta = false;
};

std::vector<InstanciaAnimacion> animaciones;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

glm::vec3 ganTranslation(0.0f, 0.0f, 0.0f);  // Traslación general de GAN


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0;
bool AnimBall = false;






// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Laboratorio Computacion Grafica Equipo 11", nullptr, nullptr);

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

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	//models
	Model Com((char*)"Models/Com/1.obj");
	Model Com2((char*)"Models/Comp2/2.obj");
	Model me((char*)"Models/Mes/me.obj");
	Model ga((char*)"Models/Ga/ga.obj");
	Model gan((char*)"Models/GabN/gan.obj");
	Model g((char*)"Models/Graf/g.obj");
	Model gr((char*)"Models/gr/gr.obj");
	Model ram((char*)"Models/ram/ram.obj");
	Model pr((char*)"Models/pr/pr.obj");
	Model fu((char*)"Models/fu/fu.obj");
	Model si((char*)"Models/sn/sn.obj");
	Model sal((char*)"Models/salon/salon2.obj");
	Model PC((char*)"Models/PC/PC.obj");






	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Agregar instancia inicial con GA visible
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -25.8f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -30.3f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -34.8f));

	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -25.8f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -30.3f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -34.8f));

	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -25.8f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -30.3f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -34.8f));


	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -48.5f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -56.5f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -52.5f));

	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -48.5f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -56.5f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -52.5f));

	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -48.5f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -56.5f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -52.5f));

	// Nuevas instancias sin cambio de posición o escala (solo animación en el lugar)
	animaciones.emplace_back(glm::vec3(4.0f, -2.5f, -59.0f),true);
	animaciones.emplace_back(glm::vec3(-3.0f, -2.5f, -59.0f),true);
	animaciones.emplace_back(glm::vec3(-10.0f, -2.5f, -59.0f),true);


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
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

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


		glm::mat4 model(1);

	
	

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Ahora sí la mandas correctamente
		glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), explosionFactor);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);





		

		// PANTALLAS 1
		glm::mat4 groupTransform = glm::mat4(1.0f);
		groupTransform = glm::translate(groupTransform, glm::vec3(3.5f, 0.45f, -23.5f)); // posición base
		groupTransform = glm::scale(groupTransform, glm::vec3(2.5f)); // escala del conjunto
		groupTransform = glm::rotate(groupTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 2
		glm::mat4 groupTransform1 = glm::mat4(1.0f);
		groupTransform1 = glm::translate(groupTransform1, glm::vec3(3.5f, 0.45f, -28.0f)); // posición base
		groupTransform1 = glm::scale(groupTransform1, glm::vec3(2.5f)); // escala del conjunto
		groupTransform1 = glm::rotate(groupTransform1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform1 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform1 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 3
		glm::mat4 groupTransform2 = glm::mat4(1.0f);
		groupTransform2 = glm::translate(groupTransform2, glm::vec3(3.5f, 0.45f, -32.5f)); // posición base
		groupTransform2 = glm::scale(groupTransform2, glm::vec3(2.5f)); // escala del conjunto
		groupTransform2 = glm::rotate(groupTransform2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform2 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform2 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 4
		glm::mat4 groupTransform3 = glm::mat4(1.0f);
		groupTransform3 = glm::translate(groupTransform3, glm::vec3(-3.5f, 0.45f, -32.5f)); // posición base
		groupTransform3 = glm::scale(groupTransform3, glm::vec3(2.5f)); // escala del conjunto
		groupTransform3 = glm::rotate(groupTransform3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform3 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform3 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 5
		glm::mat4 groupTransform4 = glm::mat4(1.0f);
		groupTransform4 = glm::translate(groupTransform4, glm::vec3(-3.5f, 0.45f, -28.0f)); // posición base
		groupTransform4 = glm::scale(groupTransform4, glm::vec3(2.5f)); // escala del conjunto
		groupTransform4 = glm::rotate(groupTransform4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform4 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform4 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 6
		glm::mat4 groupTransform5 = glm::mat4(1.0f);
		groupTransform5 = glm::translate(groupTransform5, glm::vec3(-3.5f, 0.45f, -23.5f)); // posición base
		groupTransform5 = glm::scale(groupTransform5, glm::vec3(2.5f)); // escala del conjunto
		groupTransform5 = glm::rotate(groupTransform5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform5 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform5 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 7
		glm::mat4 groupTransform6 = glm::mat4(1.0f);
		groupTransform6 = glm::translate(groupTransform6, glm::vec3(-10.5f, 0.45f, -23.5f)); // posición base
		groupTransform6 = glm::scale(groupTransform6, glm::vec3(2.5f)); // escala del conjunto
		groupTransform6 = glm::rotate(groupTransform6, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform6 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform6 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}


		// PANTALLAS 8
		glm::mat4 groupTransform7 = glm::mat4(1.0f);
		groupTransform7 = glm::translate(groupTransform7, glm::vec3(-10.5f, 0.45f, -28.0f)); // posición base
		groupTransform7 = glm::scale(groupTransform7, glm::vec3(2.5f)); // escala del conjunto
		groupTransform7 = glm::rotate(groupTransform7, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform7 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform7 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 9
		glm::mat4 groupTransform8 = glm::mat4(1.0f);
		groupTransform8 = glm::translate(groupTransform8, glm::vec3(-10.5f, 0.45f, -32.5f)); // posición base
		groupTransform8 = glm::scale(groupTransform8, glm::vec3(2.5f)); // escala del conjunto
		groupTransform8 = glm::rotate(groupTransform8, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform8 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform8 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}
		

		// PANTALLAS 10
		glm::mat4 groupTransform9 = glm::mat4(1.0f);
		groupTransform9 = glm::translate(groupTransform9, glm::vec3(3.5f, 0.45f, -58.5f)); // posición base
		groupTransform9 = glm::scale(groupTransform9, glm::vec3(2.5f)); // escala del conjunto
		groupTransform9 = glm::rotate(groupTransform9, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform9 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform9 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 11
		glm::mat4 groupTransform10 = glm::mat4(1.0f);
		groupTransform10 = glm::translate(groupTransform10, glm::vec3(3.5f, 0.45f, -54.5f)); // posición base
		groupTransform10 = glm::scale(groupTransform10, glm::vec3(2.5f)); // escala del conjunto
		groupTransform10 = glm::rotate(groupTransform10, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform10 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform10 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 12
		glm::mat4 groupTransform11 = glm::mat4(1.0f);
		groupTransform11 = glm::translate(groupTransform11, glm::vec3(3.5f, 0.45f, -50.5f)); // posición base
		groupTransform11 = glm::scale(groupTransform11, glm::vec3(2.5f)); // escala del conjunto
		groupTransform11 = glm::rotate(groupTransform11, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform11 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform11 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 13
		glm::mat4 groupTransform12 = glm::mat4(1.0f);
		groupTransform12 = glm::translate(groupTransform12, glm::vec3(3.5f, 0.45f, -46.5f)); // posición base
		groupTransform12 = glm::scale(groupTransform12, glm::vec3(2.5f)); // escala del conjunto
		groupTransform12 = glm::rotate(groupTransform12, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform12 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform12 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 14
		glm::mat4 groupTransform13 = glm::mat4(1.0f);
		groupTransform13 = glm::translate(groupTransform13, glm::vec3(-3.5f, 0.45f, -58.5f)); // posición base
		groupTransform13 = glm::scale(groupTransform13, glm::vec3(2.5f)); // escala del conjunto
		groupTransform13 = glm::rotate(groupTransform13, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform13 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform13 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 15
		glm::mat4 groupTransform14 = glm::mat4(1.0f);
		groupTransform14 = glm::translate(groupTransform14, glm::vec3(-3.5f, 0.45f, -54.5f)); // posición base
		groupTransform14 = glm::scale(groupTransform14, glm::vec3(2.5f)); // escala del conjunto
		groupTransform14 = glm::rotate(groupTransform14, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform14 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform14 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 16
		glm::mat4 groupTransform15 = glm::mat4(1.0f);
		groupTransform15 = glm::translate(groupTransform15, glm::vec3(-3.5f, 0.45f, -50.5f)); // posición base
		groupTransform15 = glm::scale(groupTransform15, glm::vec3(2.5f)); // escala del conjunto
		groupTransform15 = glm::rotate(groupTransform15, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform15 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform15 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 17
		glm::mat4 groupTransform16 = glm::mat4(1.0f);
		groupTransform16 = glm::translate(groupTransform16, glm::vec3(-3.5f, 0.45f, -46.5f)); // posición base
		groupTransform16 = glm::scale(groupTransform16, glm::vec3(2.5f)); // escala del conjunto
		groupTransform16 = glm::rotate(groupTransform16, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform16 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform16 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 18
		glm::mat4 groupTransform17 = glm::mat4(1.0f);
		groupTransform17 = glm::translate(groupTransform17, glm::vec3(-10.5f, 0.45f, -58.5f)); // posición base
		groupTransform17 = glm::scale(groupTransform17, glm::vec3(2.5f)); // escala del conjunto
		groupTransform17 = glm::rotate(groupTransform17, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform17 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform17 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 19
		glm::mat4 groupTransform18 = glm::mat4(1.0f);
		groupTransform18 = glm::translate(groupTransform18, glm::vec3(-10.5f, 0.45f, -54.5f)); // posición base
		groupTransform18 = glm::scale(groupTransform18, glm::vec3(2.5f)); // escala del conjunto
		groupTransform18 = glm::rotate(groupTransform18, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform18 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform18 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 20
		glm::mat4 groupTransform19 = glm::mat4(1.0f);
		groupTransform19 = glm::translate(groupTransform19, glm::vec3(-10.5f, 0.45f, -50.5f)); // posición base
		groupTransform19 = glm::scale(groupTransform19, glm::vec3(2.5f)); // escala del conjunto
		groupTransform19 = glm::rotate(groupTransform19, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform19 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform19 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		// PANTALLAS 21
		glm::mat4 groupTransform20 = glm::mat4(1.0f);
		groupTransform20 = glm::translate(groupTransform20, glm::vec3(-10.5f, 0.45f, -46.5f)); // posición base
		groupTransform20 = glm::scale(groupTransform20, glm::vec3(2.5f)); // escala del conjunto
		groupTransform20 = glm::rotate(groupTransform20, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación del conjunto

		// MODEL 1 - Explosion Animation
		if (model1Visible && explosionFactor < 1.7f)
		{
			glm::mat4 explodedModel = glm::mat4(1.0f);

			float explosionScale = 1.0f - explosionFactor / 1.7f;
			float baseOffset = 0.8f;
			float verticalCompensate = baseOffset * (1.0f - explosionScale);

			explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f)); // relativa a la base
			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalModel1 = groupTransform20 * explodedModel;  // combinación padre + local
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
			Com.Draw(lightingShader);
		}

		// MODEL 2 - Aparición animada
		if (model2Visible)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

			glm::mat4 finalModel2 = groupTransform20 * model2;  // también relativo al grupo
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
			Com2.Draw(lightingShader);
		}

		//		//DIBUJO DE SALON

		if (true)  // Puedes agregar condiciones si lo quieres mostrar opcionalmente
		{
			glm::mat4 modelSalon = glm::mat4(1.0f);  // Matriz identidad

			// No hay traslación, rotación ni escala (centrado y a tamaño original)
			// Si quieres escalar un poco:
			// modelSalon = glm::scale(modelSalon, glm::vec3(0.5f));
			modelSalon = glm::translate(modelSalon, glm::vec3(0.0f, -4.0f, -40.0f)); // Baja un poco el modelo
			modelSalon = glm::rotate(modelSalon, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Gira
			modelSalon = glm::scale(modelSalon, glm::vec3(1.0f)); // Reduce a la mitad
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSalon));

			sal.Draw(lightingShader);
		}


		//FILA 1 VENTANA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(4.0f, -2.0f, -56.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 1.2 VENTANA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(4.0f, -2.0f, -49.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 2 VENTANA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-3.0f, -2.0f, -56.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 2.2 VENTANA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-3.0f, -2.0f, -49.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 3 VENTANA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-10.0f, -2.0f, -56.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 3.2 VENTANA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-10.0f, -2.0f, -49.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}

		//FILA 1 PUERTA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(4.0f, -2.0f, -25.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 1.2 PUERTA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(4.0f, -2.0f, -32.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 2 PUERTA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-3.0f, -2.0f, -25.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 2.2 PUERTA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-3.0f, -2.0f, -32.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}

		//FILA 3 PUERTA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-10.0f, -2.0f, -25.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}
		//FILA 3.2 PUERTA
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, glm::vec3(-10.0f, -2.0f, -32.0f)); // puedes ajustar la posición
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // sin rotación
			modelMe = glm::scale(modelMe, glm::vec3(7.5f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f); // sin explosión
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);       // sin transparencia
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			me.Draw(lightingShader);
		}






 


for (const auto& instancia : animaciones)
{
	glm::vec3 posicionY = instancia.animacionEstatica ?
		instancia.posicion :  // usa directamente la Y del vector `posicion`
		glm::vec3(instancia.posicion.x, instancia.translationY, instancia.posicion.z);


	
	// ------------------ GA ------------------
	if (instancia.gaVisible)
	{
		glm::mat4 explodedGA = glm::mat4(1.0f);

		glm::vec3 gaPos = instancia.animacionEstatica
			? instancia.posicion
			: glm::vec3(instancia.posicion.x, 0.5f, instancia.posicion.z);

		explodedGA = glm::translate(explodedGA, gaPos);
		explodedGA = glm::scale(explodedGA, glm::vec3(
			2.0f * instancia.gaExplosionFactor,
			2.0f * instancia.gaExplosionFactor,
			2.0f * instancia.gaExplosionFactor
		));
		explodedGA = glm::rotate(explodedGA, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedGA));
		ga.Draw(lightingShader);
	}


	// ------------------ GAN ------------------
	if (instancia.ganVisible)
	{
		glm::mat4 modelGan = glm::mat4(1.0f);
		modelGan = glm::translate(modelGan, posicionY);
		modelGan = glm::rotate(modelGan, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGan = glm::scale(modelGan, instancia.escala);
		modelGan = glm::rotate(modelGan, glm::radians(instancia.ganRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGan = glm::scale(modelGan, glm::vec3(instancia.ganScaleFactor));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGan));
		gan.Draw(lightingShader);
	}

	// ------------------ G ------------------
	if (instancia.gVisible)
	{
		glm::mat4 modelG = glm::mat4(1.0f);
		modelG = glm::translate(modelG, posicionY);
		modelG = glm::rotate(modelG, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		modelG = glm::scale(modelG, instancia.escala);
		modelG = glm::translate(modelG, ganTranslation + glm::vec3(instancia.gOffsetX, 0.0f, 0.0f));
		modelG = glm::rotate(modelG, glm::radians(instancia.gRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		modelG = glm::rotate(modelG, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelG = glm::scale(modelG, glm::vec3(instancia.gScaleFactor * 0.7f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelG));
		g.Draw(lightingShader);
	}

	// ------------------ GR ------------------
	if (instancia.grVisible)
	{
		glm::mat4 modelGR = glm::mat4(1.0f);
		modelGR = glm::translate(modelGR, posicionY);
		modelGR = glm::rotate(modelGR, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGR = glm::scale(modelGR, instancia.escala);
		modelGR = glm::translate(modelGR, ganTranslation + glm::vec3(instancia.grOffsetX - 0.05f, 0.0f, -0.15f));
		modelGR = glm::rotate(modelGR, glm::radians(instancia.grRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGR = glm::rotate(modelGR, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGR = glm::rotate(modelGR, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelGR = glm::rotate(modelGR, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelGR = glm::scale(modelGR, glm::vec3(0.5f * instancia.grScaleFactor));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGR));
		gr.Draw(lightingShader);
	}

	// ------------------ RAM ------------------
	if (instancia.ramVisible)
	{
		glm::mat4 modelRAM = glm::mat4(1.0f);
		modelRAM = glm::translate(modelRAM, posicionY);
		modelRAM = glm::rotate(modelRAM, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRAM = glm::scale(modelRAM, instancia.escala);
		modelRAM = glm::translate(modelRAM, ganTranslation + glm::vec3(instancia.ramOffsetX - 0.02f, -0.25f, -0.06f));
		modelRAM = glm::rotate(modelRAM, glm::radians(instancia.ramRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRAM = glm::rotate(modelRAM, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRAM = glm::rotate(modelRAM, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelRAM = glm::scale(modelRAM, glm::vec3(0.3f * instancia.ramScaleFactor));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRAM));
		ram.Draw(lightingShader);
	}

	// ------------------ PR ------------------
	if (instancia.prVisible)
	{
		glm::mat4 modelPR = glm::mat4(1.0f);
		modelPR = glm::translate(modelPR, posicionY);
		modelPR = glm::rotate(modelPR, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPR = glm::scale(modelPR, instancia.escala);
		modelPR = glm::translate(modelPR, ganTranslation + glm::vec3(instancia.prOffsetX + 0.012f, -0.19f, -0.18f));
		modelPR = glm::rotate(modelPR, glm::radians(instancia.prRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPR = glm::rotate(modelPR, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPR = glm::rotate(modelPR, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPR = glm::scale(modelPR, glm::vec3(0.07f * instancia.prScaleFactor));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPR));
		pr.Draw(lightingShader);
	}

	// ------------------ FU ------------------
	if (instancia.fuVisible)
	{
		glm::mat4 modelFU = glm::mat4(1.0f);
		modelFU = glm::translate(modelFU, posicionY);
		modelFU = glm::rotate(modelFU, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		modelFU = glm::scale(modelFU, instancia.escala);
		modelFU = glm::translate(modelFU, ganTranslation + glm::vec3(instancia.fuOffsetX - 0.2f, -0.26f, -0.3f));
		modelFU = glm::rotate(modelFU, glm::radians(instancia.fuRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		modelFU = glm::rotate(modelFU, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelFU = glm::rotate(modelFU, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelFU = glm::rotate(modelFU, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelFU = glm::scale(modelFU, glm::vec3(0.35f * instancia.fuScaleFactor));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFU));
		fu.Draw(lightingShader);
	}
}








		


		
	

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
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[0]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	float speedMultiplier = 3.0f; // Ajusta este número como quieras

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime * speedMultiplier);

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime * speedMultiplier);

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime * speedMultiplier);

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime * speedMultiplier);

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		explosionActive = true;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		explosionFactor = 0.0f;
		implosionFactor = 0.0f;
		explosionActive = false;
		implosionActive = false;
		model1Visible = true;
		model2Visible = false;
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		gaExplosionActive = true;
		gaInflating = true;
		gaContracting = false;
		gaExplosionFactor = 1.0f;
		gaVisible = true;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		for (auto& instancia : animaciones)
		{
			instancia.gaContracting = true;
			instancia.gaExplosionActive = true;
		}
	}



	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		for (auto& instancia : animaciones)
		{
			// GA visible desde el inicio
			instancia.gaVisible = true;
			instancia.gaExplosionFactor = 1.0f;
			instancia.gaExplosionActive = false;
			instancia.gaInflating = false;
			instancia.gaContracting = false;

			// GAN reinicia oculto
			instancia.ganVisible = false;
			instancia.ganAppearing = false;
			instancia.ganScaleFactor = 1.0f;
			instancia.ganRotation = 0.0f;
			instancia.ganFinished = false;

			// G
			instancia.gVisible = false;
			instancia.gAppearing = false;
			instancia.gAnimActive = false;
			instancia.gScaleFactor = 0.0f;
			instancia.gRotation = 0.0f;
			instancia.gOffsetX = 0.0f;
			instancia.gInPhase1 = false;
			instancia.gInPhase2 = false;

			// GR
			instancia.grVisible = false;
			instancia.grAnimActive = false;
			instancia.grScaleFactor = 0.0f;
			instancia.grRotation = 0.0f;
			instancia.grOffsetX = 0.0f;
			instancia.grInPhase1 = false;
			instancia.grInPhase2 = false;

			// RAM
			instancia.ramVisible = false;
			instancia.ramAnimActive = false;
			instancia.ramScaleFactor = 0.0f;
			instancia.ramRotation = 0.0f;
			instancia.ramOffsetX = 0.0f;
			instancia.ramInPhase1 = false;
			instancia.ramInPhase2 = false;

			// PR
			instancia.prVisible = false;
			instancia.prAnimActive = false;
			instancia.prScaleFactor = 0.0f;
			instancia.prRotation = 0.0f;
			instancia.prOffsetX = 0.0f;
			instancia.prInPhase1 = false;
			instancia.prInPhase2 = false;

			// FU
			instancia.fuVisible = false;
			instancia.fuAnimActive = false;
			instancia.fuScaleFactor = 0.0f;
			instancia.fuRotation = 0.0f;
			instancia.fuOffsetX = 0.0f;
			instancia.fuInPhase1 = false;
			instancia.fuInPhase2 = false;

			// Reinicio final
			instancia.translationY = globalTranslationYInicial;
			instancia.escala = glm::vec3(3.0f);
			instancia.reduciendoEscala = false;
			instancia.bajandoY = false;
			instancia.escala5Set = false;
		}

		// Variables globales por si acaso
		gaAntesDeX = false;
		animacionXCompleta = false;
	}


}




void Animation() {

	// ------------------- EXPLOSIÓN (tecla N) -------------------
	if (explosionActive && explosionFactor < 1.7f) {
		explosionFactor += deltaTime * 2.0f;  // velocidad de expansión
		if (explosionFactor >= 1.7f) {
			explosionFactor = 1.7f;
			explosionActive = false;
			model1Visible = false;
			model2Visible = true;
			model2Appearing = true;
			model2ScaleFactor = 0.0f;
			model2Rotation = 0.0f;
		}
	}

	// ------------------- APARICIÓN MODEL2 -------------------
	if (model2Appearing) {
		model2ScaleFactor += deltaTime * 1.5f;
		if (model2ScaleFactor > 1.0f) model2ScaleFactor = 1.0f;

		model2Rotation += deltaTime * 360.0f;
		if (model2Rotation >= model2TargetRotation) {
			model2Rotation = model2TargetRotation;
			model2Appearing = false;
		}
	}

	// ------------------- IMPLOSIÓN (tecla M) -------------------
	if (implosionActive) {
		implosionFactor += deltaTime * 2.0f;  // velocidad de contracción
		if (implosionFactor >= 1.7f) {
			implosionFactor = 0.0f;
			explosionFactor = 0.0f;
			implosionActive = false;

			model1Visible = true;
			model2Visible = false;
			model2Appearing = false;
			model2ScaleFactor = 0.0f;
			model2Rotation = 0.0f;
		}
	}

	

	// Animación por instancia (usando vector `animaciones`)
	for (InstanciaAnimacion& instancia : animaciones) {
		if (instancia.animacionEstatica && !instancia.gaContracting && !instancia.ganAppearing && !instancia.gVisible){
			continue;
		}

		if (instancia.gaVisible && instancia.gaContracting)
		{
			instancia.gaExplosionFactor -= deltaTime * 1.5f;

			if (instancia.gaExplosionFactor <= 0.0f)
			{
				instancia.gaExplosionFactor = 0.0f;
				instancia.gaVisible = false;
				instancia.ganVisible = true;
				instancia.ganAppearing = true;
				instancia.ganScaleFactor = 0.0f;
				instancia.ganRotation = 0.0f;

				// Solo si estaba arriba, sube a Y=7.0 para animar
				if (!instancia.animacionEstatica)
					instancia.translationY = globalTranslationYInicial;
			}
		}

		if (instancia.posicion.x == 7.0f) {
			// Desactiva el cambio de escala y bajada
			instancia.reduciendoEscala = false;
			instancia.bajandoY = false;
		}


		// Aparece GAN
		if (instancia.ganAppearing) {
			if (instancia.ganScaleFactor < 1.0f) {
				instancia.ganScaleFactor += deltaTime * 0.5f;
				if (instancia.ganScaleFactor > 1.0f) instancia.ganScaleFactor = 1.0f;
			}
			if (instancia.ganRotation < ganTargetRotation) {
				instancia.ganRotation += deltaTime * 180.0f;
				if (instancia.ganRotation > ganTargetRotation)
					instancia.ganRotation = ganTargetRotation;
			}

			if (instancia.ganScaleFactor >= 1.0f && instancia.ganRotation >= ganTargetRotation) {
				instancia.ganAppearing = false;
				instancia.ganFinished = true;
			}
		}

		// Activar G después de GAN
		if (instancia.ganFinished && !instancia.gAnimActive) {
			instancia.gVisible = true;
			instancia.gScaleFactor = 0.0f;
			instancia.gRotation = 0.0f;
			instancia.gOffsetX = -0.5f;
			instancia.gAnimActive = true;
			instancia.gInPhase1 = true;
			instancia.ganFinished = false;
		}

		// Animación de G
		if (instancia.gAnimActive) {
			if (instancia.gInPhase1) {
				instancia.gScaleFactor += deltaTime * 1.5f;
				if (instancia.gScaleFactor > 1.0f) instancia.gScaleFactor = 1.0f;

				instancia.gRotation += deltaTime * 180.0f;
				if (instancia.gRotation >= instancia.gTargetRotation) {
					instancia.gRotation = instancia.gTargetRotation;
					instancia.gInPhase1 = false;
					instancia.gInPhase2 = true;
				}
			}
			else if (instancia.gInPhase2) {
				instancia.gOffsetX += deltaTime * 1.0f;
				if (instancia.gOffsetX >= 0.0f) {
					instancia.gOffsetX = 0.0f;
					instancia.gAnimActive = false;
					instancia.gInPhase2 = false;

					// Activar GR
					instancia.grVisible = true;
					instancia.grScaleFactor = 0.0f;
					instancia.grRotation = 0.0f;
					instancia.grOffsetX = -0.5f;
					instancia.grAnimActive = true;
					instancia.grInPhase1 = true;
				}
			}
		}

		// Animación de GR
		if (instancia.grAnimActive) {
			if (instancia.grInPhase1) {
				instancia.grScaleFactor += deltaTime * 1.5f;
				if (instancia.grScaleFactor > 1.0f) instancia.grScaleFactor = 1.0f;

				instancia.grRotation += deltaTime * 180.0f;
				if (instancia.grRotation >= instancia.grTargetRotation) {
					instancia.grRotation = instancia.grTargetRotation;
					instancia.grInPhase1 = false;
					instancia.grInPhase2 = true;
				}
			}
			else if (instancia.grInPhase2) {
				instancia.grOffsetX += deltaTime * 1.0f;
				if (instancia.grOffsetX >= 0.0f) {
					instancia.grOffsetX = 0.0f;
					instancia.grAnimActive = false;
					instancia.grInPhase2 = false;

					// Activar RAM
					instancia.ramVisible = true;
					instancia.ramScaleFactor = 0.0f;
					instancia.ramRotation = 0.0f;
					instancia.ramOffsetX = -0.5f;
					instancia.ramAnimActive = true;
					instancia.ramInPhase1 = true;
				}
			}
		}

		// RAM
		if (instancia.ramAnimActive) {
			if (instancia.ramInPhase1) {
				instancia.ramScaleFactor += deltaTime * 1.5f;
				if (instancia.ramScaleFactor > 1.0f) instancia.ramScaleFactor = 1.0f;

				instancia.ramRotation += deltaTime * 180.0f;
				if (instancia.ramRotation >= instancia.ramTargetRotation) {
					instancia.ramRotation = instancia.ramTargetRotation;
					instancia.ramInPhase1 = false;
					instancia.ramInPhase2 = true;
				}
			}
			else if (instancia.ramInPhase2) {
				instancia.ramOffsetX += deltaTime * 1.0f;
				if (instancia.ramOffsetX >= 0.0f) {
					instancia.ramOffsetX = 0.0f;
					instancia.ramAnimActive = false;
					instancia.ramInPhase2 = false;

					instancia.prVisible = true;
					instancia.prScaleFactor = 0.0f;
					instancia.prRotation = 0.0f;
					instancia.prOffsetX = -0.5f;
					instancia.prAnimActive = true;
					instancia.prInPhase1 = true;
				}
			}
		}

		// PR
		if (instancia.prAnimActive) {
			if (instancia.prInPhase1) {
				instancia.prScaleFactor += deltaTime * 1.5f;
				if (instancia.prScaleFactor > 1.0f) instancia.prScaleFactor = 1.0f;

				instancia.prRotation += deltaTime * 180.0f;
				if (instancia.prRotation >= 360.0f) {
					instancia.prRotation = 360.0f;
					instancia.prInPhase1 = false;
					instancia.prInPhase2 = true;
				}
			}
			else if (instancia.prInPhase2) {
				instancia.prOffsetX += deltaTime * 1.0f;
				if (instancia.prOffsetX >= 0.0f) {
					instancia.prOffsetX = 0.0f;
					instancia.prAnimActive = false;
					instancia.prInPhase2 = false;

					instancia.fuVisible = true;
					instancia.fuScaleFactor = 0.0f;
					instancia.fuRotation = 0.0f;
					instancia.fuOffsetX = -0.5f;
					instancia.fuAnimActive = true;
					instancia.fuInPhase1 = true;
				}
			}
		}

		// FU
		if (instancia.fuAnimActive) {
			if (instancia.fuInPhase1) {
				instancia.fuScaleFactor += deltaTime * 1.5f;
				if (instancia.fuScaleFactor > 1.0f) instancia.fuScaleFactor = 1.0f;

				instancia.fuRotation += deltaTime * 180.0f;
				if (instancia.fuRotation >= instancia.fuTargetRotation) {
					instancia.fuRotation = instancia.fuTargetRotation;
					instancia.fuInPhase1 = false;
					instancia.fuInPhase2 = true;
				}
			}
			// FU - final
			else if (instancia.fuInPhase2) {
				instancia.fuOffsetX += deltaTime * 1.0f;
				if (instancia.fuOffsetX >= 0.0f) {
					instancia.fuOffsetX = 0.0f;
					instancia.fuAnimActive = false;
					instancia.fuInPhase2 = false;

					instancia.reduciendoEscala = true;
				}
			}

		}

		
		else if (instancia.reduciendoEscala && !instancia.animacionEstatica) {
			instancia.escala = glm::mix(instancia.escala, glm::vec3(2.0f), deltaTime * 1.5f);
			if (glm::length(instancia.escala - glm::vec3(2.0f)) < 0.01f) {
				instancia.escala = glm::vec3(2.0f);
				instancia.reduciendoEscala = false;
				instancia.bajandoY = true;
			}
		}

		if (instancia.bajandoY && !instancia.animacionEstatica) {
			instancia.translationY -= deltaTime * 2.0f;
			if (instancia.translationY <= 0.5f) {
				instancia.translationY = 0.5f;
				instancia.bajandoY = false;
			}
		}

	}
}









void MouseCallback(GLFWwindow *window, double xPos, double yPos)
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