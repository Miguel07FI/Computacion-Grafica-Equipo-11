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
float gOffsetX = 0.0f;
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

float grOffsetX = 0.0f;
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
float ramOffsetX = 0.0f;

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
float prOffsetX = 0.0f;

// Variables para modelo 'fu'
bool fuVisible = false;
bool fuAnimActive = false;
bool fuInPhase1 = false, fuInPhase2 = false;
float fuScaleFactor = 0.0f;
float fuRotation = 0.0f;
float fuOffsetX = 0.0f;
float fuTargetRotation = 180.0f;

bool ganFinished = false;

bool gaAntesDeX = false;

bool ganVisible = false;     // Antes estaba en true
float ganScaleFactor = 1.0f;
bool ganDisappearing = false;



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







//

//
//		
//		
//		
		// PANTALLAS 1
		glm::mat4 groupTransform = glm::mat4(1.0f);
		groupTransform = glm::translate(groupTransform, glm::vec3(3.2f, 0.5f, -23.5f)); // posición base
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
		groupTransform1 = glm::translate(groupTransform1, glm::vec3(3.2f, 0.5f, -28.0f)); // posición base
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
		groupTransform2 = glm::translate(groupTransform2, glm::vec3(3.2f, 0.5f, -32.5f)); // posición base
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

//
//		// Dibujo tercer monitor con su cambio
//		if (model1Visible && explosionFactor < 1.7f)
//		{
//			glm::mat4 explodedModel = glm::mat4(1.0f);
//
//			// 🔁 Explosión desde la base hacia arriba, sin colapsar hacia el centro
//			float explosionScale = 1.0f - explosionFactor / 1.7f;
//			float baseOffset = 0.8f; // base original del modelo
//			float verticalCompensate = baseOffset * (1.0f - explosionScale); // lo que sube
//
//			// Coloca el modelo más arriba al escalar, para que no traspase el piso
//			explodedModel = glm::translate(explodedModel, glm::vec3(1.9f, verticalCompensate, 0.0f));
//			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale, explosionScale, explosionScale));
//			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//			// Enviar al shader y dibujar
//			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedModel));
//			Com.Draw(lightingShader);
//		}
//
//
//		if (model2Visible)
//		{
//			glm::mat4 model2 = glm::mat4(1.0f);
//			model2 = glm::translate(model2, glm::vec3(1.9f, 0.0f, 0.0f));
//
//			// Aplica rotación mientras aparece
//			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
//			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));
//
//			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
//			Com2.Draw(lightingShader);
//		}
//
//		// Dibujo cuarto monitor con su cambio
//		if (model1Visible && explosionFactor < 1.7f)
//		{
//			glm::mat4 explodedModel = glm::mat4(1.0f);
//
//			// 🔁 Explosión desde la base hacia arriba, sin colapsar hacia el centro
//			float explosionScale = 1.0f - explosionFactor / 1.7f;
//			float baseOffset = 0.8f; // base original del modelo
//			float verticalCompensate = baseOffset * (1.0f - explosionScale); // lo que sube
//
//			// Coloca el modelo más arriba al escalar, para que no traspase el piso
//			explodedModel = glm::translate(explodedModel, glm::vec3(3.9f, verticalCompensate, 0.0f));
//			explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale, explosionScale, explosionScale));
//			explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//			// Enviar al shader y dibujar
//			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedModel));
//			Com.Draw(lightingShader);
//		}
//
//
//		if (model2Visible)
//		{
//			glm::mat4 model2 = glm::mat4(1.0f);
//			model2 = glm::translate(model2, glm::vec3(3.9f, 0.0f, 0.0f));
//
//			// Aplica rotación mientras aparece
//			model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
//			model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));
//
//			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
//			Com2.Draw(lightingShader);
//		}
//		
//		
//		
//		
//		
//		
//		
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
//		
//		
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
		//FILA 2 VENTANA
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
		//FILA 2.2 VENTANA
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


		


////Dibujamos la PC vieja
//if (gaVisible)
//{
//	glm::mat4 explodedGA = glm::mat4(1.0f);
//	float baseOffset = 0.8f;
//
//	// Aparece con escala normal si aún no se ha inflado
//	float scaleXZ = (!gaExplosionActive && gaExplosionFactor <= 1.0f) ? 1.0f : gaExplosionFactor;
//	float scaleY = 1.0f;
//
//	float verticalCompensate = baseOffset * (1.0f - scaleY);
//	explodedGA = glm::translate(explodedGA, glm::vec3(-1.0f, 0.09f + verticalCompensate, 0.0f));
//
//	explodedGA = glm::scale(explodedGA, glm::vec3(scaleXZ, scaleY, scaleXZ));
//	explodedGA = glm::rotate(explodedGA, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedGA));
//	ga.Draw(lightingShader);
//}
//
//
////Dibujamos la PC vieja
//if (gaVisible)
//{
//	glm::mat4 explodedGA = glm::mat4(1.0f);
//	float baseOffset = 0.8f;
//
//	// Aparece con escala normal si aún no se ha inflado
//	float scaleXZ = (!gaExplosionActive && gaExplosionFactor <= 1.0f) ? 1.0f : gaExplosionFactor;
//	float scaleY = 1.0f;
//
//	float verticalCompensate = baseOffset * (1.0f - scaleY);
//	explodedGA = glm::translate(explodedGA, glm::vec3(0.98f, 0.09f + verticalCompensate, 0.0f));
//
//	explodedGA = glm::scale(explodedGA, glm::vec3(scaleXZ, scaleY, scaleXZ));
//	explodedGA = glm::rotate(explodedGA, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedGA));
//	ga.Draw(lightingShader);
//}
//
//// Dibujamos la PC vieja
//if (gaVisible)
//{
//	glm::mat4 explodedGA = glm::mat4(1.0f);
//	float baseOffset = 0.8f;
//
//	// Aparece con escala normal si aún no se ha inflado
//	float scaleXZ = (!gaExplosionActive && gaExplosionFactor <= 1.0f) ? 1.0f : gaExplosionFactor;
//	float scaleY = 1.0f;
//
//	float verticalCompensate = baseOffset * (1.0f - scaleY);
//	explodedGA = glm::translate(explodedGA, glm::vec3(2.98f, 0.09f + verticalCompensate, 0.0f));
//
//	explodedGA = glm::scale(explodedGA, glm::vec3(scaleXZ, scaleY, scaleXZ));
//	explodedGA = glm::rotate(explodedGA, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedGA));
//	ga.Draw(lightingShader);
//}
//
// //Dibujamos la PC vieja
//if (gaVisible)
//{
//	glm::mat4 explodedGA = glm::mat4(1.0f);
//	float baseOffset = 0.8f;
//
//	// Aparece con escala normal si aún no se ha inflado
//	float scaleXZ = (!gaExplosionActive && gaExplosionFactor <= 1.0f) ? 1.0f : gaExplosionFactor;
//	float scaleY = 1.0f;
//
//	float verticalCompensate = baseOffset * (1.0f - scaleY);
//	explodedGA = glm::translate(explodedGA, glm::vec3(4.4f, -1.15f + verticalCompensate, 0.2f));
//
//	explodedGA = glm::scale(explodedGA, glm::vec3(scaleXZ, scaleY, scaleXZ));
//	explodedGA = glm::rotate(explodedGA, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedGA));
//	ga.Draw(lightingShader);
//}






//if (ganVisible)
//{
//	glm::mat4 modelGan = glm::mat4(1.0f);
//	modelGan = glm::translate(modelGan, glm::vec3(0.0f, 0.0f, 0.0f)); // Ajusta posición
//	modelGan = glm::rotate(modelGan, glm::radians(ganRotation), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelGan = glm::scale(modelGan, glm::vec3(ganScaleFactor));
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGan));
//	gan.Draw(lightingShader);
//}
//
//if (gVisible)
//{
//	glm::mat4 modelG = glm::mat4(1.0f);
//	modelG = glm::translate(modelG, ganTranslation + glm::vec3(gOffsetX, 0.0f, 0.0f));
//	modelG = glm::rotate(modelG, glm::radians(gRotation), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelG = glm::rotate(modelG, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelG = glm::scale(modelG, glm::vec3(gScaleFactor * 0.7f)); // Escala progresiva
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelG));
//
//	g.Draw(lightingShader);
//}
//
//
//if (grVisible)
//{
//	glm::mat4 modelGR = glm::mat4(1.0f);
//	modelGR = glm::translate(modelGR, ganTranslation + glm::vec3(grOffsetX - 0.05f, 0.0f, -0.15f));
//	modelGR = glm::rotate(modelGR, glm::radians(grRotation), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelGR = glm::rotate(modelGR, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelGR = glm::rotate(modelGR, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelGR = glm::rotate(modelGR, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelGR = glm::scale(modelGR, glm::vec3(0.5f * grScaleFactor)); // Animación de escala
//
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGR));
//
//	gr.Draw(lightingShader);
//}
//
//if (ramVisible)
//{
//	glm::mat4 modelRAM = glm::mat4(1.0f);
//	modelRAM = glm::translate(modelRAM, ganTranslation + glm::vec3(ramOffsetX - 0.02f, -0.25f, -0.06f));
//	modelRAM = glm::rotate(modelRAM, glm::radians(ramRotation), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelRAM = glm::rotate(modelRAM, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelRAM = glm::rotate(modelRAM, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelRAM = glm::scale(modelRAM, glm::vec3(0.3f * ramScaleFactor)); // Animación de escala
//
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelRAM));
//
//	ram.Draw(lightingShader);
//}
//
//if (prVisible)
//{
//	glm::mat4 modelPR = glm::mat4(1.0f);
//	modelPR = glm::translate(modelPR, ganTranslation + glm::vec3(prOffsetX + 0.012f, -0.19f, -0.18f));
//	modelPR = glm::rotate(modelPR, glm::radians(prRotation), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelPR = glm::rotate(modelPR, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelPR = glm::rotate(modelPR, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelPR = glm::scale(modelPR, glm::vec3(0.07f * prScaleFactor));
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPR));
//
//	pr.Draw(lightingShader);
//}
//
//if (fuVisible)
//{
//	glm::mat4 modelFU = glm::mat4(1.0f);
//	modelFU = glm::translate(modelFU, ganTranslation + glm::vec3(fuOffsetX - 0.2f, -0.26f, -0.3f));
//	modelFU = glm::rotate(modelFU, glm::radians(fuRotation), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelFU = glm::rotate(modelFU, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelFU = glm::rotate(modelFU, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelFU = glm::rotate(modelFU, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelFU = glm::scale(modelFU, glm::vec3(0.35f * fuScaleFactor));
//
//	glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
//	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
//	glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFU));
//
//	fu.Draw(lightingShader);
//}








		


		
	

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
		if (gaVisible && !gaExplosionActive)
		{
			gaExplosionActive = true;
			gaContracting = true;
			gaInflating = false;

			gaAntesDeX = true; // 🔁 Marca que debe iniciar la animación de X cuando ga desaparezca
		}
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		// Reset total
		gaVisible = true;
		gaExplosionFactor = 1.0f;
		gaExplosionActive = false;
		gaInflating = false;
		gaContracting = false;

		ganVisible = false;
		ganAppearing = false;
		ganRotation = 0.0f;
		ganScaleFactor = 1.0f;
		ganFinished = false;

		gVisible = false;
		gAppearing = false;
		gAnimActive = false;
		gOffsetX = 0.0f;
		gRotation = 0.0f;
		gScaleFactor = 0.0f;
		gInPhase1 = false;
		gInPhase2 = false;

		grVisible = false;
		grAppearing = false;
		grAnimActive = false;
		grOffsetX = -1.0f;
		grRotation = 0.0f;
		grScaleFactor = 0.0f;
		grInPhase1 = false;
		grInPhase2 = false;

		ramVisible = false;
		ramAppearing = false;
		ramAnimActive = false;
		ramOffsetX = -1.0f;
		ramRotation = 0.0f;
		ramScaleFactor = 0.0f;
		ramInPhase1 = false;
		ramInPhase2 = false;

		prVisible = false;
		prAnimActive = false;
		prOffsetX = -1.0f;
		prRotation = 0.0f;
		prScaleFactor = 0.0f;
		prInPhase1 = false;
		prInPhase2 = false;

		fuVisible = false;
		fuAnimActive = false;
		fuOffsetX = -1.0f;
		fuRotation = 0.0f;
		fuScaleFactor = 0.0f;
		fuInPhase1 = false;
		fuInPhase2 = false;

		explosionActive = false;
		explosionFactor = 0.0f;
		model1Visible = false;
		model2Visible = false;
		model2Appearing = false;

		gaAntesDeX = false;
		animacionXCompleta = false;
	}








}


// ... (todo el contenido anterior se mantiene igual hasta la función Animation)

// Animación modificada
void Animation() {
	if (animacionActivada)
	{
		if (posBallY >= limiteSuperior) moviendoArriba = false;
		if (posBallY <= limiteInferior) moviendoArriba = true;
		if (moviendoArriba) posBallY += velocidad * deltaTime;
		else posBallY -= velocidad * deltaTime;
	}

	if (explosionActive)
	{
		explosionFactor += 0.008f;
		if (explosionFactor >= 1.7f)
		{
			explosionFactor = 1.7f;
			explosionActive = false;
			model1Visible = false;
			model2Appearing = true;
			model2ScaleFactor = 0.0f;
			model2Rotation = 0.0f;
			model2TargetRotation = 1080.0f;
			model2Visible = true;
		}
	}

	if (model2Appearing)
	{
		if (model2ScaleFactor < 1.2f)
		{
			model2ScaleFactor += deltaTime * 1.5f;
			if (model2ScaleFactor > 1.2f)
				model2ScaleFactor = 1.2f;
		}
		if (model2Rotation < model2TargetRotation)
		{
			model2Rotation += deltaTime * 360.0f;
			if (model2Rotation >= model2TargetRotation)
			{
				model2Rotation = 0.0f;
				model2Appearing = false;
			}
		}
	}

	if (gaExplosionActive)
	{
		if (gaInflating)
		{
			gaExplosionFactor += deltaTime * 1.5f;
			if (gaExplosionFactor >= 2.0f)
			{
				gaExplosionFactor = 2.0f;
				gaInflating = false;
				gaContracting = true;
			}
		}
		else if (gaContracting)
		{
			gaExplosionFactor -= deltaTime * 1.5f;
			if (gaExplosionFactor <= 0.0f)
			{
				gaExplosionFactor = 0.0f;
				gaExplosionActive = false;
				gaContracting = false;
				gaVisible = false;
			}
		}
	}

	
	


	if (gAppearing)
	{
		if (gScaleFactor < 1.0f)
		{
			gScaleFactor += deltaTime * 1.2f;
			if (gScaleFactor > 1.0f) gScaleFactor = 1.0f;
		}
		if (gRotation < gTargetRotation)
		{
			gRotation += deltaTime * 180.0f;
			if (gRotation >= gTargetRotation)
			{
				gRotation = gTargetRotation;
				gAppearing = false;
			}
		}
	}

	if (gAnimActive)
	{
		if (gInPhase1)
		{
			gScaleFactor += deltaTime * 1.5f;
			if (gScaleFactor > 1.0f) gScaleFactor = 1.0f;

			gRotation += deltaTime * 180.0f;
			if (gRotation >= gTargetRotation)
			{
				gRotation = gTargetRotation;
				gInPhase1 = false;
				gInPhase2 = true;
			}
		}
		else if (gInPhase2)
		{
			gOffsetX += deltaTime * 1.0f;
			if (gOffsetX >= 0.0f)
			{
				gOffsetX = 0.0f;
				gAnimActive = false;
				gInPhase2 = false;

				grVisible = true;
				grScaleFactor = 0.0f;
				grRotation = 0.0f;
				grOffsetX = -1.0f;
				grAnimActive = true;
				grInPhase1 = true;
			}
		}
	}
	if (grAnimActive)
	{
		if (grInPhase1)
		{
			grScaleFactor += deltaTime * 1.5f;
			if (grScaleFactor > 1.0f) grScaleFactor = 1.0f;

			grRotation += deltaTime * 180.0f; // rotación progresiva
			if (grRotation >= grTargetRotation)
			{
				grRotation = grTargetRotation;
				grInPhase1 = false;
				grInPhase2 = true;
			}
		}
		else if (grInPhase2)
		{
			grOffsetX += deltaTime * 1.0f;
			if (grOffsetX >= 0.0f)
			{
				grOffsetX = 0.0f;
				grAnimActive = false;
				grInPhase2 = false;

				ramVisible = true;
				ramScaleFactor = 0.0f;
				ramRotation = 0.0f;
				ramOffsetX = -1.0f;
				ramAnimActive = true;
				ramInPhase1 = true;
			}
		}
	}

	if (ramAnimActive)
	{
		if (ramInPhase1)
		{
			ramScaleFactor += deltaTime * 1.5f;
			if (ramScaleFactor > 1.0f) ramScaleFactor = 1.0f;

			ramRotation += deltaTime * 180.0f; // rotación progresiva
			if (ramRotation >= ramTargetRotation)
			{
				ramRotation = ramTargetRotation;
				ramInPhase1 = false;
				ramInPhase2 = true;
			}
		}
		else if (ramInPhase2)
		{
			ramOffsetX += deltaTime * 1.0f;
			if (ramOffsetX >= 0.0f)
			{
				ramOffsetX = 0.0f;
				ramAnimActive = false;
				ramInPhase2 = false;

				prVisible = true;
				prScaleFactor = 0.0f;
				prRotation = 0.0f;
				prOffsetX = -1.0f;
				prAnimActive = true;
				prInPhase1 = true;
			}
		}
	}
	if (prAnimActive)
	{
		if (prInPhase1)
		{
			prScaleFactor += deltaTime * 1.5f;
			if (prScaleFactor > 1.0f) prScaleFactor = 1.0f;

			prRotation += deltaTime * 180.0f;
			if (prRotation >= 360.0f)
			{
				prRotation = 360.0f;
				prInPhase1 = false;
				prInPhase2 = true;
			}
		}
		else if (prInPhase2)
		{
			prOffsetX += deltaTime * 1.0f;
			if (prOffsetX >= 0.0f)
			{
				prOffsetX = 0.0f;
				prAnimActive = false;
				prInPhase2 = false;

				// Activar modelo FU
				fuVisible = true;
				fuScaleFactor = 0.0f;
				fuRotation = 0.0f;
				fuOffsetX = -1.0f;
				fuAnimActive = true;
				fuInPhase1 = true;
			}
		}
	}
	if (fuAnimActive)
	{
		if (fuInPhase1)
		{
			fuScaleFactor += deltaTime * 1.5f;
			if (fuScaleFactor > 1.0f) fuScaleFactor = 1.0f;

			fuRotation += deltaTime * 180.0f;
			if (fuRotation >= fuTargetRotation)
			{
				fuRotation = fuTargetRotation;
				fuInPhase1 = false;
				fuInPhase2 = true;
			}
		}
		else if (fuInPhase2)
		{
			fuOffsetX += deltaTime * 1.0f;
			if (fuOffsetX >= 0.0f)
			{
				fuOffsetX = 0.0f;
				fuAnimActive = false;
				fuInPhase2 = false;
			}
		}
	}
	// ✅ Si se presionó X mientras ga estaba activo, espera a que termine para iniciar animación
	if (gaAntesDeX && !gaVisible && !gaExplosionActive)
	{
		// Ahora sí activa la animación X
		animacionXCompleta = false;

		ganAppearing = true;
		ganScaleFactor = 0.0f;
		ganRotation = 0.0f;
		ganVisible = true;
		ganFinished = false;

		// Resetear modelos posteriores
		grVisible = false;
		grAnimActive = false;
		grScaleFactor = 0.0f;
		grRotation = 0.0f;
		grOffsetX = -1.0f;
		grInPhase1 = false;

		ramVisible = false;
		ramAnimActive = false;
		ramScaleFactor = 0.0f;
		ramRotation = 0.0f;
		ramOffsetX = -1.0f;
		ramInPhase1 = false;

		prVisible = false;
		prAnimActive = false;
		prScaleFactor = 0.0f;
		prRotation = 0.0f;
		prOffsetX = -1.0f;
		prInPhase1 = false;

		fuVisible = false;
		fuAnimActive = false;
		fuScaleFactor = 0.0f;
		fuRotation = 0.0f;
		fuOffsetX = -1.0f;
		fuInPhase1 = false;

		gaAntesDeX = false; // ✅ Ya se usó
	}
	if (ganAppearing)
	{
		// Escalar progresivamente hasta 1.0
		if (ganScaleFactor < 1.0f)
		{
			ganScaleFactor += deltaTime * 0.5f;
			if (ganScaleFactor > 1.0f) ganScaleFactor = 1.0f;
		}

		// Rotar progresivamente hasta el ángulo objetivo
		if (ganRotation < ganTargetRotation)
		{
			ganRotation += deltaTime * 180.0f;
			if (ganRotation > ganTargetRotation)
				ganRotation = ganTargetRotation;
		}

		// Solo se termina cuando ambas animaciones han concluido
		if (ganScaleFactor >= 1.0f && ganRotation >= ganTargetRotation)
		{
			ganAppearing = false;
			ganFinished = true;
		}
	}



	if (ganFinished && !gAnimActive)
	{
		gVisible = true;
		gScaleFactor = 0.0f;
		gRotation = 0.0f;
		gOffsetX = -1.0f;
		gAnimActive = true;
		gInPhase1 = true;

		// ✅ Esto evita que vuelva a ejecutarse
		ganFinished = false;
	}
	if (gaAntesDeX && !gaVisible && !gaExplosionActive)
	{
		gaAntesDeX = false;

		// Activa la secuencia de modelos posteriores
		gVisible = true;
		gScaleFactor = 0.0f;
		gRotation = 0.0f;
		gOffsetX = -1.0f;
		gAnimActive = true;
		gInPhase1 = true;

		// Reiniciar todos los demás
		grVisible = false;
		grAnimActive = false;
		grScaleFactor = 0.0f;
		grRotation = 0.0f;
		grOffsetX = -1.0f;
		grInPhase1 = false;

		ramVisible = false;
		ramAnimActive = false;
		ramScaleFactor = 0.0f;
		ramRotation = 0.0f;
		ramOffsetX = -1.0f;
		ramInPhase1 = false;

		prVisible = false;
		prAnimActive = false;
		prScaleFactor = 0.0f;
		prRotation = 0.0f;
		prOffsetX = -1.0f;
		prInPhase1 = false;

		fuVisible = false;
		fuAnimActive = false;
		fuScaleFactor = 0.0f;
		fuRotation = 0.0f;
		fuOffsetX = -1.0f;
		fuInPhase1 = false;
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