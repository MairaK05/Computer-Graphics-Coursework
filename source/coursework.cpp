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
    
    Model statue("../assets/sphere.obj"); //load the statue model
    statue.addTexture("../assets/statueTexture.png", "diffuse");

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Use back face culling
    glEnable(GL_CULL_FACE);

    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


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
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    statueShaderID = LoadShaders("statueVertexShader.glsl", "statueFragmentShader.glsl");

    glUseProgram(shaderID);

    // Load the textures
    unsigned int texture;
    texture = loadTexture("../assets/tileTexture.png");

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

    camera.eye = glm::vec3(5.0f, 0.0f, 10.0f); // set the camera position
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Get inputs
        keyboardInput(window);

        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;
        
        // Clear the window
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.target = camera.eye + camera.front;
        camera.calculateMatrices();

        // Send the VBO to the GPU
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Send the UV buffer to the GPU
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        //Draw cubes -------------------------------------------------------

        glUseProgram(shaderID);
        glBindVertexArray(VAO);

        for (int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            // Calculate the model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);
            glm::mat4 model = translate * rotate * scale;

            // Calculate the MVP matrix
            glm::mat4 MVP = camera.projection * camera.view * model;

            // Send MVP matrix to the vertex shader
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

            // Draw the triangles
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
            
        }


        glBindVertexArray(0);
        //// Draw the triangles
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        //////Activate statue shader ---------------------------------------
        glUseProgram(statueShaderID);

        //////Model matrix calculation
        glm::mat4 translate; // = Maths::translate(statueObj.position);
        glm::mat4 scale; // = Maths::scale(statueObj.scale);
        glm::mat4 rotate; // = Maths::rotate(statueObj.angle, statueObj.rotation);
        glm::mat4 model = translate * rotate * scale;

        glm::mat4 MV = camera.view * model;
        glm::mat4 MVP = camera.projection * MV;
        glUniformMatrix4fv(glGetUniformLocation(statueShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

        statue.draw(statueShaderID);

        //// ---------------------------------------------------------------

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

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.right;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.right;
}
