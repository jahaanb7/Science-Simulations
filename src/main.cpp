#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/constants.hpp>
#include "../include/particle.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const float speed = 20.0f;

void drawParticle(const Particle& p, int numSegments) {

    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < numSegments; i++){

        float theta = 2.0f * glm::pi<float>() * float(i) / float(numSegments);

        float x = p.radius * cos(theta);
        float y = p.radius * sin(theta);

        glVertex2f(x + p.position.x, y + p.position.y);
    }
    glEnd();
}

int main(void)
{
    if (!glfwInit()){
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Space Simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glm::vec2 ParticlePosition(400.0f, 300.0f);
    glm::vec2 ParticleVelocity = glm::vec2(30.0f, 10.0f);

    Particle particle(ParticlePosition, ParticleVelocity, 1.0f, 30.0f);

    float deltaTime = 0.0167f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        double xpos; 
        double ypos;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwGetCursorPos(window, &xpos, &ypos);

        float xPosMouse = static_cast<float>(xpos);
        float yPosMouse = HEIGHT - static_cast<float>(ypos);

        glm::vec2 mousePos(xPosMouse, yPosMouse);
        glm::vec2 direction = mousePos - particle.position;

        particle.velocity = direction * speed;

        drawParticle(particle, 50);

        particle.update(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

