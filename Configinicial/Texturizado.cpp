
/*
Miguel Angel Hernandez Ramirez
319044618
Practica 7 Texturizado
19/03/2025
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

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
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
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

							// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Texturizado Hernandez Ramirez Miguel Angel", nullptr, nullptr);

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

	

	// Build and compile our shader program
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	GLfloat vertices[] = {
		// Frente (Coordenadas Y invertidas)
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.74f, 0.64f, 
		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.98f, 0.64f, 
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.98f, 0.45f, 
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.74f, 0.45F, 

		-0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 1.0f,   0.28f, 0.64f, // D
		 0.5f, -0.5f, 0.5f,    1.0f, 1.0f, 1.0f,   0.5f, 0.64f,  // E
		 0.5f,  0.5f, 0.5f,    1.0f, 1.0f, 1.0f,   0.5f, 0.34f, // G
		-0.5f,  0.5f, 0.5f,    1.0f, 1.0f, 1.0f,   0.28f, 0.34f, // F

		// Izquierda (Coordenadas Y invertidas)
		 -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,   0.27f, 0.5f, // F
		 -0.5f, -0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,   0.04f, 0.5f, // K
		 -0.5f,  0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,   0.04f, 0.4f, // L
		 -0.5f,  0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,   0.27f, 0.4f,  // M


		// Derecha (Coordenadas y invertidas)
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.53f, 0.6f, // Derecha
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.74f, 0.6f, // Derecha
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.74f, 0.36f, // Derecha
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.53f, 0.36f, // Derecha


		 // Abajo (Coordenadas Y invertidas)
		 -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.29f, 0.94f, 
		  0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.5f, 0.94f, 
		  0.5f, -0.5f, 0.5f,     0.0f, -1.0f, 0.0f,    0.5f, 0.67f, 
		 -0.5f, -0.5f, 0.5f,     0.0f, -1.0f, 0.0f,    0.29f, 0.67f, 

		 // Arriba (Coordenadas Y invertidas)
		 -0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.28f, 0.33f, 
		  0.5f, 0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.5f, 0.33f, 
		  0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f,   0.5f, 0.13f, 
		 -0.5f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f,   0.28f, 0.13f  

	};

		GLuint indices[] = {
		0, 1, 3, 1, 2, 3,   // Frente
		4, 5, 7, 5, 6, 7,   // Atrás
		8, 9, 11, 9, 10, 11, // Izquierda
		12, 13, 15, 13, 14, 15, // Derecha
		16, 17, 19, 17, 18, 19, // Arriba
		20, 21, 23, 21, 22, 23  // Abajo
		};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO,EBO;
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
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	// Cargar textura (supongamos que 'texture1' es el identificador de la textura)
	GLuint texture1;  // Usando 'texture1'
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	int textureWidth, textureHeight, nrChannels;

	unsigned char* image = stbi_load("images/cal.png", &textureWidth, &textureHeight, &nrChannels, 0);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded successfully!" << std::endl;
	}
	else {
		std::cout << "Error loading texture" << std::endl;
	}
	stbi_image_free(image);


	// Enlazar la textura al shader
	lampShader.Use();
	glUniform1i(glGetUniformLocation(lampShader.Program, "texture1"), 0); // El 0 corresponde a GL_TEXTURE0





	

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

		lampShader.Use();
		// Configurar la transformación del modelo para mover el cubo
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));  // Mueve el cubo hacia la cámara
		glm::mat4 view = camera.GetViewMatrix(); // Esto depende de cómo tengas configurada la cámara


		// Obtener las ubicaciones de las matrices en el shader
		GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Setear las matrices en el shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Dibuja todas las caras del cubo
		// Enlazar la textura
		glActiveTexture(GL_TEXTURE0);  // Usamos el canal de textura 0
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Enlazar el shader y las matrices
		lampShader.Use();
		glUniform1i(glGetUniformLocation(lampShader.Program, "texture1"), 0);  // Asegúrate de que el 0 corresponda a GL_TEXTURE0

		// Dibujar el cubo
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  // Dibuja todas las caras del cubo
		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
