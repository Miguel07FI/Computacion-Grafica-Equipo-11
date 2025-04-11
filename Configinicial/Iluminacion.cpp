/*
Miguel Angel Hernandez Ramirez
319044618
Actividad Previa Materiales e Iluminaciones PRACTICA 8
22/03/2025
*/

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

float moveLightPosY = 0.0f; // Movimiento en Y
float moveLightPosZ = 0.0f; // Movimiento en Z
float lightAngle = 0.0f; // Ángulo de rotación
float lightRadius = 4.0f; // Radio del giro

bool isDay = true;  // Verdadero si es de día, falso si es de noche
glm::vec3 lightColor(1.0f, 0.9f, 0.6f);  // Color de luz para el día (amarillo cálido)





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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materiales e Iluminacion Practica 8 Hernandez Ramirez Miguel Angel", nullptr, nullptr);

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
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    glm::vec3 lightPos(0.8f, 0.7f, 0.3f);  // Fuente de luz principal
    glm::vec3 secondLightPos(-1.0f, 1.0f, 1.0f);  // Segunda fuente de luz
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

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
    Model cir((char*)"Models/circo.obj");
    Model silla((char*)"Models/M/tripo_convert_6b109b73-3709-444c-8eb8-9b6703a81ae1.obj");
    Model per_neg((char*)"Models/P/1.obj");
    Model Pal((char*)"Models/3/pal.obj");
    Model pis((char*)"Models/4/piso.obj");
    Model pay((char*)"Models/5/pay.obj");
    Model sol((char*)"Models/6/sol.obj");
    Model luna((char*)"Models/7/luna.obj");



    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 modelLight(1.0f);
    modelLight = glm::translate(modelLight, lightPos);  // Posición de la luz
    modelLight = glm::scale(modelLight, glm::vec3(0.3f));  // Escala del modelo de luz (ajusta el tamaño)


   

    GLuint lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);



    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    while (!glfwWindowShouldClose(window))
    {
        lightPos.x = lightRadius * cos(lightAngle);
        lightPos.z = lightRadius * sin(lightAngle);
        lightPos.y = 1.2f;  // Mantén la altura de la luz fija si es necesario



        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer and depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Aquí aplicamos la luz según si es día o noche
        if (isDay)
        {
            // Luz tipo sol (amarillo cálido)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.1f);  // Luz ambiental más intensa
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 1.0f, 0.8f); // Luz difusa más intensa (amarilla)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 0.8f);  // Luz especular más intensa (brillante)

            // Dibujar solo el modelo del sol
            glm::mat4 modelLight(1.0f);
            modelLight = glm::translate(modelLight, lightPos);
            modelLight = glm::scale(modelLight, glm::vec3(0.3f));
            glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLight));
            sol.Draw(lightingShader);  // Modelo de sol

        }
        else
        {
            // Luz tipo luna (azul)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);  // Luz ambiental más tenue
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.1f, 0.1f, 0.5f);    // Luz azul difusa
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.1f, 0.1f, 0.3f);   // Luz azul especular

            // Dibujar solo el modelo de la luna
            glm::mat4 modelLight(1.0f);
            modelLight = glm::translate(modelLight, lightPos);
            modelLight = glm::scale(modelLight, glm::vec3(0.3f));
            glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLight));
            luna.Draw(lightingShader);  // Modelo de luna
        }


        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.1f);   // Material más brillante
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 0.6f);   // Material con un buen color difuso
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 0.8f);  // Alta reflectividad
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f); // Brillo más alto



        // Pasar otros uniformes que ya usas (como la posición de la cámara)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        

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


        // Dibujar el primer cubo de luz (lightPos)
       // Dibujar el primer cubo de luz (lightPos)
        lampshader.Use();

 

        // Volver a habilitar la prueba de profundidad después de renderizar el cubo de luz
        glEnable(GL_DEPTH_TEST);


       // Volver a habilitar la prueba de profundidad después de renderizar el cubo de luz
        glEnable(GL_DEPTH_TEST);



        // Intercambiar los buffers para mostrar la escena renderizada
        glfwSwapBuffers(window);
    }



    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


void DoMovement()
{
    // Movimiento de la cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // Movimiento circular de la luz con teclas O y L
    if (keys[GLFW_KEY_O])
        lightAngle += 1.5f * deltaTime;  // velocidad ajustada
    if (keys[GLFW_KEY_L])
        lightAngle -= 1.5f * deltaTime;

    // Animación adicional (si la tienes activada con alguna lógica tuya)
    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
}



void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_N && action == GLFW_PRESS)  // Cambiar entre sol y luna
    {
        isDay = !isDay;  // Cambiar entre día y noche
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}




// Función para manejar el movimiento del ratón
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reverso ya que las coordenadas Y aumentan hacia abajo

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
