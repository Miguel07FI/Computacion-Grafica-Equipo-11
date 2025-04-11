/*
Hernandez Ramirez Miguel Angel
Modelado Jerarquico PRACTICA 5
09 de febrero del 2025
319044618
*/

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);


const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float	hombro = 0.0f,
codo = 0.0f,
muneca = 0.0f,
dedo1 = 0.0f,
dedo2 = 0.0f,
dedo1_2 = 0.0f,
dedo2_2 = 0.0f,
dedo3 = 0.0f,
dedo3_2 = 0.0f,
dedo4 = 0.0f,
dedo4_2 = 0.0f,
dedo5 = 0.0f,
dedo1_3 = 0.0f,
dedo2_3 = 0.0f,
dedo3_3 = 0.0f,
dedo4_3 = 0.0f,
dedo5_2=0.0f;




int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado jerarquico Hernandez Ramirez Miguel Angel", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
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


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection




	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, -0.5f,-0.5f,
		 0.5f, -0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		-0.5f,  0.5f,-0.5f,
		-0.5f, -0.5f,-0.5f,

		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);



	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	glm::mat4 projection = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp3 = glm::mat4(1.0f); //Temp



		//View set up 
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		glBindVertexArray(VAO);

		// Límite del movimiento del hombro
		hombro = glm::clamp(hombro, -90.0f, 90.0f);

		// Model Bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); //A

		//Model Antebrazo

		codo = glm::clamp(codo, 0.0f, 120.0f);
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));  // nos movemos al extremo de nuestro modelo 1
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 0.0f, 1.0f)); //colocamos nuestra matriz de codo
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));  // Generamos la jerarquia
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); // Dibujamos nuestro modelo, de escala 2 para que quede en la posicion ideal conenctado a nuestro modelo 1
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//B

		//Modelo Palma
		muneca = glm::clamp(muneca, -45.0f, 45.0f);
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));  // regresamos al valor anterior para no tener que empezar desde 0
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp2 = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//C

		// Modelo Dedo1
		dedo1 = glm::clamp(dedo1, -10.0f, 30.0f);
		model = glm::translate(modelTemp2, glm::vec3(0.1f, 0.35f, 0.375f)); // Posición inicial del dedo 1
		modelTemp = model;
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		modelTemp2 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo1_2
		dedo1_2 = glm::clamp(dedo1_2, -35.0f, 0.0f);
		model = modelTemp2;
		model = glm::rotate(model, glm::radians(dedo1_2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		modelTemp3 = model;  // Guardamos la posición para la parte 3 del dedo 1
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo1_3
		dedo1_3 = glm::clamp(dedo1_3, -60.0f, 0.0f);
		model = modelTemp3; // Ahora sí tomamos la posición correcta
		model = glm::rotate(model, glm::radians(dedo1_3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// Modelo Dedo2
		dedo2 = glm::clamp(dedo2, -10.0f, 30.0f);
		model = glm::translate(modelTemp, glm::vec3(0.1f, 0.0f, -0.225f));
		modelTemp = model;
		model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		modelTemp2 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		dedo2_2 = glm::clamp(dedo2_2, -35.0f, 0.0f);
		model = modelTemp2;
		model = glm::rotate(model, glm::radians(dedo2_2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		modelTemp3 = model;  // Guardamos la posición para la parte 3 del dedo 2
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo2_3
		dedo2_3 = glm::clamp(dedo2_3, -60.0f, 0.0f);
		model = modelTemp3;
		model = glm::rotate(model, glm::radians(dedo2_3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo3 (Medio)
		dedo3 = glm::clamp(dedo3, -10.0f, 30.0f);
		model = glm::translate(modelTemp, glm::vec3(0.1f, 0.0f, -0.225f));
		modelTemp = model;
		model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
		modelTemp2 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.2f, 0.2f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo3_2
		dedo3_2 = glm::clamp(dedo3_2, -35.0f, 0.0f);
		model = modelTemp2;
		model = glm::rotate(model, glm::radians(dedo3_2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		modelTemp3 = model;  // Guardamos la posición para la parte 3 del dedo 3
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo3_3
		dedo3_3 = glm::clamp(dedo1_3, -60.0f, 0.0f);
		model = modelTemp3;
		model = glm::rotate(model, glm::radians(dedo3_3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo4 (Anular)
		dedo4 = glm::clamp(dedo4, -10.0f, 30.0f);
		model = glm::translate(modelTemp, glm::vec3(0.1f, 0.0f, -0.225f));
		modelTemp = model;
		model = glm::rotate(model, glm::radians(dedo4), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, 0.0f));
		modelTemp2 = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.2f));
		color = glm::vec3(1.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo4_2
		dedo4_2 = glm::clamp(dedo4_2, -35.0f, 0.0f);
		model = modelTemp2;
		model = glm::rotate(model, glm::radians(dedo4_2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		modelTemp3 = model;  // Guardamos la posición para la parte 3 del dedo 4
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo4_3
		dedo4_3 = glm::clamp(dedo4_3, -60.0f, 0.0f);
		model = modelTemp3;
		model = glm::rotate(model, glm::radians(dedo4_3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Modelo Dedo5 (Meñique)
		dedo5 = glm::clamp(dedo5, -40.0f, 25.0f);
		model = glm::translate(modelTemp, glm::vec3(-0.15f, -0.7f, -0.1f));
		modelTemp = model;
		model = glm::rotate(model, glm::radians(dedo5), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, 0.0f));
		modelTemp2 = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.2f));
		color = glm::vec3(0.5f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Modelo Dedo5_2
		dedo5_2 = glm::clamp(dedo5_2, -30.0f, 20.0f);
		model = modelTemp2;
		model = glm::rotate(model, glm::radians(dedo5_2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		modelTemp3 = model; // Guardamos la posición para una posible tercera parte
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.2f));
		color = glm::vec3(1.0f, 0.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		




		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Movimiento de cámara
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.18f;

	// Articulaciones principales
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		hombro += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		hombro -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		codo += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		codo -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		muneca += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		muneca -= 0.18f;

	// Movimiento de los dedos
	// Dedo 1 (Índice)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo1 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo1 -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo1_2 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo1_2 -= 0.18f;

	// Dedo 2 (Pulgar)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo2 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo2 -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo2_2 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // ';'
		dedo2_2 -= 0.18f;

	// Dedo 3 (Medio)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo3 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo3 -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo3_2 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo3_2 -= 0.18f;

	// Dedo 4 (Anular)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo4 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo4 -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo4_2 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo4_2 -= 0.18f;

	// Dedo 5 (Meñique)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // ','
		dedo5 -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // '.'
		dedo5 += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    dedo1_3 += 0.18f;
if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    dedo1_3 -= 0.18f;

if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    dedo2_3 += 0.18f;
if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    dedo2_3 -= 0.18f;

if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    dedo3_3 += 0.18f;
if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    dedo3_3 -= 0.18f;

if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    dedo4_3 += 0.18f;
if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    dedo4_3 -= 0.18f;

if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
dedo5_2 -= 0.18f;
if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
dedo5_2 += 0.18f;
	
}


