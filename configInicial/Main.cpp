#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bote de Basura - Lobby FI", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Vertices del cubo base
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,  0.4f, 0.4f, 0.4f,
         0.5f, -0.5f, -0.5f,  0.4f, 0.4f, 0.4f,
         0.5f,  0.5f, -0.5f,  0.4f, 0.4f, 0.4f,
         0.5f,  0.5f, -0.5f,  0.4f, 0.4f, 0.4f,
        -0.5f,  0.5f, -0.5f,  0.4f, 0.4f, 0.4f,
        -0.5f, -0.5f, -0.5f,  0.4f, 0.4f, 0.4f,

         0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.3f,
         0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.3f,
         0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.3f,
         0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.3f,
         0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.3f,
         0.5f, -0.5f,  0.5f,  0.3f, 0.3f, 0.3f,

        -0.5f,  0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
        -0.5f,  0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
        -0.5f, -0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
        -0.5f, -0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
        -0.5f, -0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
        -0.5f,  0.5f,  0.5f,  0.6f, 0.6f, 0.6f,

        -0.5f, -0.5f, -0.5f,  0.2f, 0.2f, 0.2f,
         0.5f, -0.5f, -0.5f,  0.2f, 0.2f, 0.2f,
         0.5f, -0.5f,  0.5f,  0.2f, 0.2f, 0.2f,
         0.5f, -0.5f,  0.5f,  0.2f, 0.2f, 0.2f,
        -0.5f, -0.5f,  0.5f,  0.2f, 0.2f, 0.2f,
        -0.5f, -0.5f, -0.5f,  0.2f, 0.2f, 0.2f,

        -0.5f,  0.5f, -0.5f,  0.7f, 0.7f, 0.7f,
         0.5f,  0.5f, -0.5f,  0.7f, 0.7f, 0.7f,
         0.5f,  0.5f,  0.5f,  0.7f, 0.7f, 0.7f,
         0.5f,  0.5f,  0.5f,  0.7f, 0.7f, 0.7f,
        -0.5f,  0.5f,  0.5f,  0.7f, 0.7f, 0.7f,
        -0.5f,  0.5f, -0.5f,  0.7f, 0.7f, 0.7f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        // Cuerpo del bote
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.6f, 1.0f, 0.6f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Aro superior del bote
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.55f, 0.0f));
        model = glm::scale(model, glm::vec3(0.65f, 0.05f, 0.65f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Tapa del bote
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.62f, 0.0f));
        model = glm::scale(model, glm::vec3(0.55f, 0.08f, 0.55f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Base del bote
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.52f, 0.0f));
        model = glm::scale(model, glm::vec3(0.55f, 0.05f, 0.55f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])   camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
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
    GLfloat yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}