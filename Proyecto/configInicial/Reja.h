#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Reja{
public:
    glm::vec3 posicion;
    glm::vec3 escala;

    // Variable para la animación de deslizamiento
    float desplazamientoX;

    // Textura de la imagen
    GLuint texturaBarrotes;

    Reja(glm::vec3 posInicial);
    void Inicializar();
    void Draw(Shader& shader, GLuint VAO);
}; 
