
/*
Hernandez Ramirez Miguel Angel
Carga de Modelos PRACTICA 6
14 de marzo del 2025
N.C 319044618
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
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Carga de modelos y camara sintetica Hernandez Ramirez Miguel Angel", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Cargar modelos
       Model dog((char*)"Models/RedDog.obj");
       Model cir((char*)"Models/circo.obj");  
       Model silla((char*)"Models/M/tripo_convert_6b109b73-3709-444c-8eb8-9b6703a81ae1.obj");  
       Model customModel((char*)"Models/P/1.obj"); 
       Model Pal((char*)"Models/3/pal.obj");  
       Model pis((char*)"Models/4/piso.obj");  
       Model pay((char*)"Models/5/pay.obj");  


        glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

       

        // Dibujar el modelo del perro
        glm::mat4 modelDog = glm::mat4(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(0.0f, -0.2f, 0.0f)); // Ajusta la posición de la casa
        modelDog = glm::rotate(modelDog, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Corregir el modelo rotado
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        dog.Draw(shader);
        
        // Dibujar la circo en una posición diferente
        glm::mat4 modelCottage = glm::mat4(1.0f);
        modelCottage = glm::translate(modelCottage, glm::vec3(0.0f, 0.35f, -0.19f)); // Ajusta la posición de la casa
        modelCottage = glm::scale(modelCottage, glm::vec3(7.0f, 7.0f, 7.0f)); // Ajusa el tamaño si es necesario
        modelCottage = glm::rotate(modelCottage, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Mantener la rotación
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCottage));
        cir.Draw(shader);

        // Dibujar la primera silla
        glm::mat4 modelSilla1 = glm::mat4(1.0f);
        modelSilla1 = glm::translate(modelSilla1, glm::vec3(0.0f, -1.0f, 2.0f));  // Ajusta la posición aquí
        modelSilla1 = glm::scale(modelSilla1, glm::vec3(1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla1 = glm::rotate(modelSilla1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla1));
        silla.Draw(shader);

        // Dibujar la segunda silla
        glm::mat4 modelSilla2 = glm::mat4(1.0f);
        modelSilla2 = glm::translate(modelSilla2, glm::vec3(1.0f, -1.0f, 2.0f));  // Ajusta la posición aquí
        modelSilla2 = glm::scale(modelSilla2, glm::vec3(1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla2 = glm::rotate(modelSilla2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla2));
        silla.Draw(shader);

        // Dibujar la segunda silla
        glm::mat4 modelSilla3 = glm::mat4(1.0f);
        modelSilla3 = glm::translate(modelSilla3, glm::vec3(-1.0f, -1.0f, 2.0f));  // Ajusta la posición aquí
        modelSilla3 = glm::scale(modelSilla3, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla3 = glm::rotate(modelSilla3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla3));
        silla.Draw(shader);

        // Dibujar la segunda silla
        glm::mat4 modelSilla4 = glm::mat4(1.0f);
        modelSilla4 = glm::translate(modelSilla4, glm::vec3(-2.0f, -1.0f, 2.0f));  // Ajusta la posición aquí
        modelSilla4 = glm::scale(modelSilla4, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla4 = glm::rotate(modelSilla4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla4));
        silla.Draw(shader);

        // Dibujar la segunda silla
        glm::mat4 modelSilla5 = glm::mat4(1.0f);
        modelSilla5 = glm::translate(modelSilla5, glm::vec3(2.0f, -1.0f, 2.0f));  // Ajusta la posición aquí
        modelSilla5 = glm::scale(modelSilla5, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla5 = glm::rotate(modelSilla5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla5));
        silla.Draw(shader);

        // Dibujar la segunda silla
        glm::mat4 modelSilla6 = glm::mat4(1.0f);
        modelSilla6 = glm::translate(modelSilla6, glm::vec3(1.0f, -1.0f, 3.0f));  // Ajusta la posición aquí
        modelSilla6 = glm::scale(modelSilla6, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla6 = glm::rotate(modelSilla6, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla6));
        silla.Draw(shader);

        // Dibujar la segunda silla
        glm::mat4 modelSilla7 = glm::mat4(1.0f);
        modelSilla7 = glm::translate(modelSilla7, glm::vec3(0.0f, -1.0f, 3.0f));  // Ajusta la posición aquí
        modelSilla7 = glm::scale(modelSilla7, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla7 = glm::rotate(modelSilla7, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla7));
        silla.Draw(shader);

        // Dibujar la segunda silla
        glm::mat4 modelSilla8 = glm::mat4(1.0f);
        modelSilla8 = glm::translate(modelSilla8, glm::vec3(-1.0f, -1.0f, 3.0f));  // Ajusta la posición aquí
        modelSilla8 = glm::scale(modelSilla8, glm::vec3(-1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelSilla8 = glm::rotate(modelSilla8, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota según lo que necesites
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla8));
        silla.Draw(shader);

        // Dibujar el modelo .obj
        glm::mat4 modelTransform = glm::mat4(1.0f);
        modelTransform = glm::translate(modelTransform, glm::vec3(0.0f, -0.75f, 1.8f));  // Ajusta la posición del modelo
        modelTransform = glm::scale(modelTransform, glm::vec3(1.0f, 1.0f, 1.0f));      // Escala según sea necesario
        modelTransform = glm::rotate(modelTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform));
        customModel.Draw(shader);

        // Dibujar el modelo .obj
        glm::mat4 modelTransform1 = glm::mat4(1.0f);
        modelTransform1 = glm::translate(modelTransform1, glm::vec3(3.5f, -0.5f, 2.5f));  // Ajusta la posición del modelo
        modelTransform1 = glm::scale(modelTransform1, glm::vec3(2.0f, 2.0f, 2.0f));      // Escala según sea necesario
        modelTransform1 = glm::rotate(modelTransform1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform1));
        Pal.Draw(shader);

        glm::mat4 modelTransform2 = glm::mat4(1.0f);
        modelTransform2 = glm::translate(modelTransform2, glm::vec3(0.0f, -1.6f, 0.0f));  // Ajusta la posición del modelo
        modelTransform2 = glm::scale(modelTransform2, glm::vec3(9.0f, 9.0f, 9.0f));      // Escala según sea necesario
        modelTransform2 = glm::rotate(modelTransform2, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform2));
        pis.Draw(shader);

        glm::mat4 modelTransform5 = glm::mat4(1.0f);
        modelTransform5 = glm::translate(modelTransform5, glm::vec3(1.5f, -0.5f, 0.5f));  // Ajusta la posición del modelo
        modelTransform5 = glm::scale(modelTransform5, glm::vec3(2.0f, 2.0f, 2.0f));      // Escala según sea necesario
        modelTransform5 = glm::rotate(modelTransform5, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rota si es necesario
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform5));
        pay.Draw(shader);
        
        //


        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

