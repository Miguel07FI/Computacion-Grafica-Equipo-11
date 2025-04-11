/*
Hernandez Ramirez Miguel Angel
Fuentes de luz PRACTICA 9
04 de abril del 2025
319044618
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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
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
bool directionalLightOn = true;  // Variable para alternar el estado de la luz direccional
bool pointLightActive = true;  // Controlar si la luz puntual está encendida o apagada
bool reflectorLightActive = true;  // Estado de la luz reflectora (encendida o apagada)



// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.0f, 3.0f, -5.0f),  // Nueva posición de la primera luz puntual
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f)
};

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
glm::vec3 reflectorLightPos(0.0f, 0.3f, 3.5f); // Posición inicial de la luz reflectora
glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // Luz blanca

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Cambiar la posición de la luz puntual a una nueva ubicación
	pointLightPositions[0] = glm::vec3(3.5f, 0.2f, 3.25f);

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz Hernandez Ramirez Miguel Angel", nullptr, nullptr);

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

	Model red_dog((char*)"Models/RedDog.obj");
	Model cir((char*)"Models/circo.obj");
	Model silla((char*)"Models/M/tripo_convert_6b109b73-3709-444c-8eb8-9b6703a81ae1.obj");
	Model per_neg((char*)"Models/P/1.obj");
	Model Pal((char*)"Models/3/pal.obj");
	Model pis((char*)"Models/4/piso.obj");
	Model pay((char*)"Models/5/pay.obj");
	Model L((char*)"Models/LAM/LA.obj");
	Model F((char*)"Models/FOC/F.obj");
	
	GLuint diffuseTextureID;  // Variable para almacenar el ID de la textura difusa

	// Cargar la textura utilizando SOIL, stb_image, o cualquier otra librería
	diffuseTextureID = SOIL_load_OGL_texture("C:/path/to/your/textures/texture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);



	

	GLuint lightTextureF, lightTextureL;

// Cargar las texturas para "F" y "L"
	lightTextureF = SOIL_load_OGL_texture("Models/FOC/Diffuse_Bake.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	lightTextureL = SOIL_load_OGL_texture("Models/FOC/Normal_Bake.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);








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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTextureID); // Asegúrate de que la textura se haya cargado correctamente
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);


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

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Dirección de la luz direccional
		if (directionalLightOn)
		{
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);
		}
		else
		{
			// Desactivar la luz direccional (se puede poner valores a 0 o no enviar estos parámetros)
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);
		}




		
		if (pointLightActive)
		{
			// Point light 1
			glm::vec3 lightColor;
			lightColor.x = abs(sin(glfwGetTime() * Light1.x));
			lightColor.y = abs(sin(glfwGetTime() * Light1.y));
			lightColor.z = sin(glfwGetTime() * Light1.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
			// Point light 1
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.1f, 0.1f, 0.1f);  // Aumenta el brillo ambiente
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.2f, 0.2f, 0.2f);  // Aumenta la intensidad difusa
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.2f, 0.2f, 0.2f);  // Aumenta la intensidad especular


			// Parámetros de atenuación de la luz puntual
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);




			// Point light 2
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

			// Point light 3
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

			// Point light 4
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);
		}
		else {
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
			// Point light 1
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);  // Aumenta el brillo ambiente
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);  // Aumenta la intensidad difusa
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);  // Aumenta la intensidad especular


			// Parámetros de atenuación de la luz puntual
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0f);




			// Point light 2
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

			// Point light 3
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

			// Point light 4
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);
		}

		// Controlar la luz reflectora
		if (reflectorLightActive)
		{
			// La luz reflectora está activada, entonces puedes configurar sus parámetros
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
				reflectorLightPos.x, reflectorLightPos.y, reflectorLightPos.z);  // No se mueve, solo se enciende
		}
		else
		{
			// La luz reflectora está apagada, no afectamos nada o configuramos valores nulos
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
				0.0f, 0.0f, 0.0f);  // Apagar la luz
		}




		// SpotLight
		
		// SpotLight (Luz reflectora)
		if (reflectorLightActive)
		{
			// La luz reflectora está activada, entonces puedes configurar sus parámetros
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
				reflectorLightPos.x, reflectorLightPos.y, reflectorLightPos.z);  // No se mueve, solo se enciende

			// Incrementar la intensidad de la luz reflectora
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.8f, 0.8f, 0.8f);  // Aumentar la luz ambiental
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);   // Aumentar la intensidad difusa
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);  // Aumentar la intensidad especular

			// Ajustar la dirección de la luz (la luz sigue la dirección de la cámara)
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
				0.0f, 0.0f, -1.0f);  // Dirección fija hacia abajo (en el eje Y negativo)


			// Otros parámetros de la luz spot
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);    // Atenuación más fuerte
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);  // Atenuación aún más fuerte

			// Aumentar los ángulos de corte para que la luz sea más intensa y focalizada
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(15.0f)));  // Ángulo más amplio
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(30.0f)));  // Ángulo exterior más amplio
		}
		else
		{
			// Si la luz reflectora está apagada, no se modifica nada
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
				0.0f, 0.0f, 0.0f);  // Apagar la luz reflectora
		}


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

		// Dibujar el modelo del perro
		glm::mat4 modelDog = glm::mat4(1.0f);
		modelDog = glm::translate(modelDog, glm::vec3(0.0f, -0.2f, 0.0f)); // Ajusta la posici�n de la casa
		modelDog = glm::rotate(modelDog, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Corregir el modelo rotado
		lightingShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
		red_dog.Draw(lightingShader);

		// Dibujar la circo en una posici�n diferente
		glm::mat4 modelCottage = glm::mat4(1.0f);
		modelCottage = glm::translate(modelCottage, glm::vec3(0.0f, 0.35f, -0.19f)); // Ajusta la posici�n de la casa
		modelCottage = glm::scale(modelCottage, glm::vec3(7.0f, 7.0f, 7.0f)); // Ajusa el tama�o si es necesario
		modelCottage = glm::rotate(modelCottage, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Mantener la rotaci�n
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCottage));
		cir.Draw(lightingShader);

		// Dibujar la primera silla
		glm::mat4 modelSilla1 = glm::mat4(1.0f);
		modelSilla1 = glm::translate(modelSilla1, glm::vec3(0.0f, -1.0f, 2.0f));  // Ajusta la posici�n aqu�
		modelSilla1 = glm::scale(modelSilla1, glm::vec3(1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla1 = glm::rotate(modelSilla1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla1));
		silla.Draw(lightingShader);


		// Dibujar la segunda silla
		glm::mat4 modelSilla2 = glm::mat4(1.0f);
		modelSilla2 = glm::translate(modelSilla2, glm::vec3(1.0f, -1.0f, 2.0f));  // Ajusta la posici�n aqu�
		modelSilla2 = glm::scale(modelSilla2, glm::vec3(1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla2 = glm::rotate(modelSilla2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla2));
		silla.Draw(lightingShader);


		// Dibujar la segunda silla
		glm::mat4 modelSilla3 = glm::mat4(1.0f);
		modelSilla3 = glm::translate(modelSilla3, glm::vec3(-1.0f, -1.0f, 2.0f));  // Ajusta la posici�n aqu�
		modelSilla3 = glm::scale(modelSilla3, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla3 = glm::rotate(modelSilla3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla3));
		silla.Draw(lightingShader);


		// Dibujar la segunda silla
		glm::mat4 modelSilla4 = glm::mat4(1.0f);
		modelSilla4 = glm::translate(modelSilla4, glm::vec3(-2.0f, -1.0f, 2.0f));  // Ajusta la posici�n aqu�
		modelSilla4 = glm::scale(modelSilla4, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla4 = glm::rotate(modelSilla4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla4));
		silla.Draw(lightingShader);


		// Dibujar la segunda silla
		glm::mat4 modelSilla5 = glm::mat4(1.0f);
		modelSilla5 = glm::translate(modelSilla5, glm::vec3(2.0f, -1.0f, 2.0f));  // Ajusta la posici�n aqu�
		modelSilla5 = glm::scale(modelSilla5, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla5 = glm::rotate(modelSilla5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla5));
		silla.Draw(lightingShader);


		// Dibujar la segunda silla
		glm::mat4 modelSilla6 = glm::mat4(1.0f);
		modelSilla6 = glm::translate(modelSilla6, glm::vec3(1.0f, -1.0f, 3.0f));  // Ajusta la posici�n aqu�
		modelSilla6 = glm::scale(modelSilla6, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla6 = glm::rotate(modelSilla6, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla6));
		silla.Draw(lightingShader);


		// Dibujar la segunda silla
		glm::mat4 modelSilla7 = glm::mat4(1.0f);
		modelSilla7 = glm::translate(modelSilla7, glm::vec3(0.0f, -1.0f, 3.0f));  // Ajusta la posici�n aqu�
		modelSilla7 = glm::scale(modelSilla7, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla7 = glm::rotate(modelSilla7, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla7));
		silla.Draw(lightingShader);


		// Dibujar la segunda silla
		glm::mat4 modelSilla8 = glm::mat4(1.0f);
		modelSilla8 = glm::translate(modelSilla8, glm::vec3(-1.0f, -1.0f, 3.0f));  // Ajusta la posici�n aqu�
		modelSilla8 = glm::scale(modelSilla8, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelSilla8 = glm::rotate(modelSilla8, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota seg�n lo que necesites
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla8));
		silla.Draw(lightingShader);


		// Dibujar el modelo .obj
		glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, glm::vec3(0.0f, -0.75f, 1.8f));  // Ajusta la posici�n del modelo
		modelTransform = glm::scale(modelTransform, glm::vec3(1.0f, 1.0f, 1.0f));      // Escala seg�n sea necesario
		modelTransform = glm::rotate(modelTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform));
		per_neg.Draw(lightingShader);


		// Dibujar el modelo .obj
		glm::mat4 modelTransform1 = glm::mat4(1.0f);
		modelTransform1 = glm::translate(modelTransform1, glm::vec3(3.5f, -0.5f, 2.5f));  // Ajusta la posici�n del modelo
		modelTransform1 = glm::scale(modelTransform1, glm::vec3(2.0f, 2.0f, 2.0f));      // Escala seg�n sea necesario
		modelTransform1 = glm::rotate(modelTransform1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform1));
		Pal.Draw(lightingShader);


		glm::mat4 modelTransform2 = glm::mat4(1.0f);
		modelTransform2 = glm::translate(modelTransform2, glm::vec3(0.0f, -1.6f, 0.0f));  // Ajusta la posici�n del modelo
		modelTransform2 = glm::scale(modelTransform2, glm::vec3(9.0f, 9.0f, 9.0f));      // Escala seg�n sea necesario
		modelTransform2 = glm::rotate(modelTransform2, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform2));
		pis.Draw(lightingShader);


		glm::mat4 modelTransform5 = glm::mat4(1.0f);
		modelTransform5 = glm::translate(modelTransform5, glm::vec3(1.5f, -0.5f, 0.5f));  // Ajusta la posici�n del modelo
		modelTransform5 = glm::scale(modelTransform5, glm::vec3(2.0f, 2.0f, 2.0f));      // Escala seg�n sea necesario
		modelTransform5 = glm::rotate(modelTransform5, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform5));
		pay.Draw(lightingShader);

		glm::mat4 modelTransform6 = glm::mat4(1.0f);
		modelTransform6 = glm::translate(modelTransform6, glm::vec3(0.0f, 0.5f, 3.5f));  // Ajusta la posici�n del modelo
		modelTransform6 = glm::scale(modelTransform6, glm::vec3(0.6f, 0.6f, 0.6f));      // Escala seg�n sea necesario
		modelTransform6 = glm::rotate(modelTransform6, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform6));
		L.Draw(lightingShader);

		glm::mat4 modelTransform9 = glm::mat4(1.0f);
		modelTransform9 = glm::translate(modelTransform9, glm::vec3(3.5f, 0.2f, 3.0f));  // Ajusta la posici�n del modelo
		modelTransform9 = glm::scale(modelTransform9, glm::vec3(0.5f, 0.5f, 0.5f));      // Escala seg�n sea necesario
		modelTransform9 = glm::rotate(modelTransform9, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform9));
		F.Draw(lightingShader);
		
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
		model = glm::scale(model, glm::vec3(0.05f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
		// For Point Light 0
		glm::mat4 modelLight0 = glm::mat4(1.0f);
		modelLight0 = glm::translate(modelLight0, pointLightPositions[0]);  // Position of the first point light
		modelLight0 = glm::scale(modelLight0, glm::vec3(0.005f));  // Small cube to represent the light
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLight0));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Draw the light cube

		// For Reflector Light (Spotlight)
		glm::mat4 modelReflectorLight = glm::mat4(1.0f);
		modelReflectorLight = glm::translate(modelReflectorLight, reflectorLightPos); // Use the reflector light position
		modelReflectorLight = glm::scale(modelReflectorLight, glm::vec3(0.05f));  // Adjust size if necessary
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelReflectorLight));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Draw the reflector light cube

			


		glBindVertexArray(0);



		









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



}

// Is called whenever a key is pressed/released via GLFW


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Salir del programa con ESC
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Cambiar el estado de las teclas
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

	// Alternar luz direccional con la tecla 'N'
	if (keys[GLFW_KEY_N] && action == GLFW_PRESS)
	{
		directionalLightOn = !directionalLightOn;  // Cambiar el estado
	}

	// Alternar el estado de la luz reflectora con la tecla 'L'
	if (keys[GLFW_KEY_L] && action == GLFW_PRESS)
	{
		reflectorLightActive = !reflectorLightActive;  // Cambiar el estado de la luz reflectora

		if (reflectorLightActive)
		{
			// Si la luz está activada, ponla en su posición original
			reflectorLightPos = glm::vec3(0.0f, 0.3f, 3.5f);  // Posición inicial de la luz reflectora
		}
		else
		{
			// Si la luz está apagada, mueve la luz a una posición fuera de la vista
			reflectorLightPos = glm::vec3(1000.0f, 1000.0f, 1000.0f); // Fuera del alcance de la cámara
		}
	}

	// Alternar estado de la luz puntual con la tecla 'V'
	// Alternar estado de la luz puntual 0 con la tecla 'V'
	if (keys[GLFW_KEY_V] && action == GLFW_PRESS)
	{
		pointLightActive = !pointLightActive;  // Cambiar solo el estado de la luz puntual 0
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