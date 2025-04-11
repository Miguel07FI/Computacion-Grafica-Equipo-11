#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Primitivas 2D Hernandez Ramirez Miguel Angel", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		// Posición (x, y, z)    // Color (R, G, B)
		-1.0f, -1.0f,  0.0f,   1.0f, 1.0f, 0.0f,  // 0 
		 1.0f, -1.0f,  0.0f,   1.0f, 1.0f, 0.0f,  // 1 
		-0.66f,  0.42f,  0.0f,  1.0f, 0.0f, 0.0f,  // 2 
	-0.44f,  0.14f,  0.0f,  1.0f, 0.0f, 0.0f,  // 3
	-0.73f,  0.14f,  0.0f,  1.0f, 0.0f, 0.0f,  // 4 

	// Triángulo 2 (Verde)
	-0.44f,  0.14f,  0.0f,  0.0f, 1.0f, 0.0f,  // 5 
	-0.6f,  0.14f,  0.0f,   0.0f, 1.0f, 0.0f,  // 6
	-0.6f,  -0.4f,  0.0f,  0.0f, 1.0f, 0.0f,  // 7 

	// Triángulo 3 (Azul)
	-0.44f,  0.14f,  0.0f,  0.0f, 0.0f, 1.0f,  // 8 
	-0.6f,  -0.4f,  0.0f,  0.0f, 0.0f, 1.0f,  // 9 
	-0.32f,  -0.18f,  0.0f,  0.0f, 0.0f, 1.0f,  // 10 

	// Triángulo 4 (Cyan)
	-0.44f,  0.14f,  0.0f,  0.0f, 1.0f, 1.0f,  // 11 
	-0.32f,  -0.18f,  0.0f,  0.0f, 1.0f, 1.0f,  // 12 
	0.15f,  0.14f,  0.0f,  0.0f, 1.0f, 1.0f,  // 13 

	// Triángulo 5 (Magenta)
	-0.32f,  -0.18f,  0.0f,  1.0f, 0.0f, 1.0f,  // 14 
	0.15f,  0.14f,  0.0f,  1.0f, 0.0f, 1.0f,  // 15 
	0.15f,  -0.18f,  0.0f,  1.0f, 0.0f, 1.0f,  // 16 

	// Triángulo 6 (Naranja)
	0.15f,  0.14f,  0.0f,  1.0f, 0.5f, 0.0f,  // 17
	0.15f,  -0.18f,  0.0f,  1.0f, 0.5f, 0.0f,  // 18 
	0.76f,  0.14f,  0.0f,  1.0f, 0.5f, 0.0f,  // 19

	// Triángulo 7 (Rosa)
	0.15f,  -0.18f,  0.0f,  1.0f, 0.0f, 0.5f,  // 20
	0.76f,  0.14f,  0.0f,  1.0f, 0.0f, 0.5f,  // 21
	0.59f,  -0.18f,  0.0f,  1.0f, 0.0f, 0.5f,  // 22

	// Triángulo 8 (Blanco)
	0.59f,  -0.18f,  0.0f,  1.0f, 1.0f, 1.0f,  // 23
	0.76f,  0.14f,  0.0f,  1.0f, 1.0f, 1.0f,  // 24
	0.88f,  -0.43f,  0.0f,  1.0f, 1.0f, 1.0f,  // 25

	// Triángulo 9 (Amarillo)
	0.76f,  0.14f,  0.0f,  1.0f, 1.0f, 0.0f,  // 26
	0.88f,  -0.43f,  0.0f,  1.0f, 1.0f, 0.0f,  // 27
	0.76f,  0.11f,  0.0f,  1.0f, 1.0f, 0.0f,  // 28
		-0.66f,  0.42f,  0.0f, 1.0f, 1.0f, 0.0f,  // 29
		-0.9f,  0.25f,  0.0f, 1.0f, 1.0f, 0.0f,  // 30
		-0.73f,  0.14f,  0.0f, 1.0f, 1.0f, 0.0f,  // 31 
		-0.9f,  0.25f,  0.0f, 1.0f, 1.0f, 0.0f,  // 32
		-0.73f,  0.14f,  0.0f, 1.0f, 1.0f, 0.0f,  // 33 
		-0.85f,  0.14f,  0.0f, 1.0f, 1.0f, 0.0f,  // 34 
		-0.6f,  0.14f,  0.0f, 1.0f, 1.0f, 0.0f,  // 35
		-0.7f,  0.14f,  0.0f, 1.0f, 1.0f, 0.0f,  // 36 
		-0.5f,  -0.08f,  0.0f, 1.0f, 1.0f, 0.0f,  // 37 
		-0.6f,  -0.08f,  0.0f, 1.0f, 1.0f, 0.0f,  // 38
		-0.65f,  -0.4f,  0.0f, 1.0f, 1.0f, 0.0f,  // 39 
		-0.6f,  -0.35f,  0.0f, 1.0f, 1.0f, 0.0f,  // 40 
		0.86f,  -0.40f,  0.0f, 1.0f, 1.0f, 0.0f,  // 41
		0.9f,  -0.42f,  0.0f, 1.0f, 1.0f, 0.0f,  // 42 
		0.79f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,  // 43

		
	};
	unsigned int indices[] = {
	2, 3, 4,  // Primer triángulo
	5, 6, 7,
	8, 9, 10,
	11, 12, 13,
	14, 15, 16,
	17, 18, 19,
	20, 21, 22,
	23, 24, 25,
	26, 27, 28,
	29, 30, 31,
	32, 33, 34,
	35, 36, 37,
	28, 39, 40,
	41, 42, 43// Segundo triángulo
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);
	
	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


      //glPointSize(10);
      //glDrawArrays(GL_POINTS,0,1);
        
      //glDrawArrays(GL_LINES,0,4);
       // glDrawArrays(GL_LINE_LOOP,0,4);
        
		glDrawArrays(GL_TRIANGLES, 2, 3); // Dibuja el primer triángulo
		glDrawArrays(GL_TRIANGLES, 5, 3);
		glDrawArrays(GL_TRIANGLES, 8, 3);
		glDrawArrays(GL_TRIANGLES, 11, 3);
		glDrawArrays(GL_TRIANGLES, 14, 3);
		glDrawArrays(GL_TRIANGLES, 17, 3);
		glDrawArrays(GL_TRIANGLES, 20, 3);
		glDrawArrays(GL_TRIANGLES, 23, 3);
		glDrawArrays(GL_TRIANGLES, 26, 3);
		glDrawArrays(GL_TRIANGLES, 29, 3);
		glDrawArrays(GL_TRIANGLES, 32, 3);
		glDrawArrays(GL_TRIANGLES, 35, 3);
		glDrawArrays(GL_TRIANGLES, 38, 3);
		glDrawArrays(GL_TRIANGLES, 41, 3);

        //glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}