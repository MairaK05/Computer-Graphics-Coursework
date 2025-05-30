#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>

// Function prototypes
void keyboardInput(GLFWwindow *window);
void mouseInput(GLFWwindow* window);

// Frame timers
float previousTime = 0.0f;  // time of previous iteration of the loop
float deltaTime = 0.0f;  // time elapsed since the previous frame

Camera camera(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, -2.0f)); // Camera

struct Object //object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};

//light source properties
glm::vec3 lightPosition = glm::vec3(15.0f, 8.0f, 10.0f);
glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 0.2f);

//statue properties
glm::vec3 statuePosition = glm::vec3(15.0f, 1.0f, 10.0f);

int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Use back face culling
    glEnable(GL_CULL_FACE);

    Model statue("../assets/teapot.obj"); //load the statue model
    statue.addTexture("../assets/diamond_normal.png", "normal");

    Model sphere("../assets/sphere.obj");

    Model terrain("../assets/plane.obj");
    terrain.addTexture("../assets/grassSpecularMap.png", "specular");

    // capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Capture mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    // Define vertices
    const float vertices[] = {
        // front
        -1.0f, -1.0f,  1.0f,    //              + ------ +
         1.0f, -1.0f,  1.0f,    //             /|       /|
         1.0f,  1.0f,  1.0f,    //   y        / |      / |
        -1.0f, -1.0f,  1.0f,    //   |       + ------ +  |
         1.0f,  1.0f,  1.0f,    //   + - x   |  + ----|- +
        -1.0f,  1.0f,  1.0f,    //  /        | /      | /
        // right                // z         |/       |/
         1.0f, -1.0f,  1.0f,    //           + ------ +
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         // back
          1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          // left
          -1.0f, -1.0f, -1.0f,
          -1.0f, -1.0f,  1.0f,
          -1.0f,  1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
          -1.0f,  1.0f,  1.0f,
          -1.0f,  1.0f, -1.0f,
          // bottom
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f,  1.0f,
          // top
          -1.0f,  1.0f,  1.0f,
           1.0f,  1.0f,  1.0f,
           1.0f,  1.0f, -1.0f,
          -1.0f,  1.0f,  1.0f,
           1.0f,  1.0f, -1.0f,
          -1.0f,  1.0f, -1.0f,
    };

    // Define texture coordinates
    const float uv[] = {
        // front
        0.0f, 0.0f,     // vertex co-ordinates are the same for each side
        1.0f, 0.0f,     // of the cube so repeat every six vertices
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // right
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // back
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // left
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // top
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    // Define indices
    unsigned int indices[] = {
        0,   1,  2,  3,  4,  5,     // front
        6,   7,  8,  9, 10, 11,     // right
        12, 13, 14, 15, 16, 17,     // back
        18, 19, 20, 21, 22, 23,     // left
        24, 25, 26, 27, 28, 29,     // bottom
        30, 31, 32, 33, 34, 35      // top
    };


    // Create the Vertex Array Object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create texture buffer
    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

    // Create Element Buffer Object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Compile shader program
    unsigned int shaderID;
    unsigned int statueShaderID;
    unsigned int sunShaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    statueShaderID = LoadShaders("statueVertexShader.glsl", "statueFragmentShader.glsl");
    sunShaderID = LoadShaders("sunVertexShader.glsl", "sunFragmentShader.glsl");

    glUseProgram(shaderID);

    // Load the textures
    unsigned int texture;
    texture = loadTexture("../assets/grassSpecularMap.png");

    // Send the texture uniforms to the fragment shader
    unsigned int textureID;
    textureID = glGetUniformLocation(shaderID, "texture");
    glUniform1i(textureID, 0);

    // Cube positions
    glm::vec3 positions[] = {
        glm::vec3(0.0f, -2.0f,  0.0f)
    };


    // Add cubes to objects vector
    std::vector<Object> objects;
    Object object;
    object.name = "cube";

    for (unsigned int x = 0; x < 30; x++) {

        for (unsigned int z = 0; z < 35; z++)
        {
            object.position = positions[0];
            object.position.z = positions[0].z + z;
            object.position.x = positions[0].x + x;
            object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
            object.scale = glm::vec3(0.5f, 0.5f, 0.5f);
            object.angle = Maths::radians(0.0f);
            objects.push_back(object);
        }
        
    }

    //Lighting properties for statue
    statue.ka = 0.7f;
    statue.kd = 0.5f;
    statue.ks = 1.0f;
    statue.Ns = 20.0f;

    terrain.ka = 0.7f;
    terrain.kd = 0.5f;
    terrain.ks = 1.0f;
    terrain.Ns = 20.0f;

    Light lightSources;
    lightSources.addPointLight(glm::vec3(15.0f, 20.0f, 20.0f),         // position
        glm::vec3(1.0f, 1.0f, 0.0f),         // colour
        1.0f, 0.1f, 0.02f);                  // attenuation

    lightSources.addPointLight(glm::vec3(1.0f, 15.0f, -8.0f),        // position
        glm::vec3(1.0f, 0.0f, 1.0f),         // colour
        1.0f, 0.1f, 0.02f);                  // attenuation

    lightSources.addSpotLight(glm::vec3(0.0f, 10.0f, 0.0f),          // position
        statuePosition,         // direction
        glm::vec3(0.0f, 1.0f, 1.0f),          // colour
        1.0f, 0.1f, 0.02f,                    // attenuation
        std::cos(Maths::radians(45.0f)));     // cos(phi)                 

    lightSources.addDirectionalLight(glm::vec3(1.0f, -1.0f, 0.0f),  // direction
        glm::vec3(1.0f, 1.0f, 0.0f));  // colour
    
    float constant = 1.0f;
    float linear = 0.1f;
    float quadratic = 0.02f;

    camera.eye = glm::vec3(15.0f, 2.0f, 20.0f); // set the camera position
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Get inputs
        keyboardInput(window);
        mouseInput(window);

        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;
        
        // Clear the window
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //calculate view and projection matrices
        camera.target = camera.eye + camera.front;
        camera.calculateMatrices();

        //////Activate statue shader ---------------------------------------------------------------------------------
        glUseProgram(statueShaderID);

        // Send light source properties to the shader
        lightSources.toShader(statueShaderID, camera.view);

        //////Model matrix calculation
        glm::mat4 translate = Maths::translate(statuePosition);
        glm::mat4 scale; // = Maths::scale(statueObj.scale);
        glm::mat4 rotate; // = Maths::rotate(statueObj.angle, statueObj.rotation);
        glm::mat4 model = translate * rotate * scale;

        glm::mat4 MV = camera.view * model;
        glm::mat4 MVP = camera.projection * MV;
        glUniformMatrix4fv(glGetUniformLocation(statueShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]); //send mvp to shader
        // Send MV matrix to the vertex shader
        glUniformMatrix4fv(glGetUniformLocation(statueShaderID, "MV"), 1, GL_FALSE, &MV[0][0]);

        statue.draw(statueShaderID);

        //// ---------------------------------------------------------------------------------------------------------------
        //Draw the sun
        //glUseProgram(sunShaderID);

        //translate = Maths::translate(lightPosition);
        //scale = Maths::scale(glm::vec3(0.9f));
        //model = translate * scale;

        //MVP = camera.projection * camera.view * model;
        //glUniformMatrix4fv(glGetUniformLocation(sunShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        //glUniform3fv(glGetUniformLocation(sunShaderID, "lightColour"), 1, &lightColour[0]);

        //sphere.draw(sunShaderID);

        //Draw the plane -----------------------------------------------
        
        glUseProgram(statueShaderID);

        // Send light source properties to the shader
        lightSources.toShader(statueShaderID, camera.view);

        //////Model matrix calculation
        translate = Maths::translate(glm::vec3(0.0f, 0.0f, 0.0f));
        scale = Maths::scale(glm::vec3(5.0f, 0.2f, 5.0f));
        rotate; // = Maths::rotate(statueObj.angle, statueObj.rotation);
        model = translate * rotate * scale;

        MV = camera.view * model;
        MVP = camera.projection * MV;
        glUniformMatrix4fv(glGetUniformLocation(statueShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]); //send mvp to shader
        // Send MV matrix to the vertex shader
        glUniformMatrix4fv(glGetUniformLocation(statueShaderID, "MV"), 1, GL_FALSE, &MV[0][0]);

        terrain.draw(statueShaderID);


        //--------------------------------------------------------------


        lightSources.draw(sunShaderID, camera.view, camera.projection, sphere);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteProgram(shaderID);

    glDeleteProgram(statueShaderID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera using WSAD keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.front;
        camera.eye.y = 4.0f;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.front;
        camera.eye.y = 4.0f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.right;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.right;

    if (glm::distance(camera.eye, statuePosition) <= 8.0f)
    {
        //Move the teapot using arrow keys
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            statuePosition += 2.0f * deltaTime * camera.right;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            statuePosition -= 2.0f * deltaTime * camera.right;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            statuePosition += 2.0f * deltaTime * camera.front;
            statuePosition.y = 1.0f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            statuePosition -= 2.0f * deltaTime * camera.front;
            statuePosition.y = 1.0f;
    }



}
void mouseInput(GLFWwindow* window)
{
    // Get mouse cursor position and reset to centre
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Update yaw and pitch angles
    camera.yaw += 0.005f * float(xPos - 1024 / 2);
    camera.pitch += 0.005f * float(768 / 2 - yPos);

    // Calculate camera vectors from the yaw and pitch angles
    camera.calculateCameraVectors();
}
