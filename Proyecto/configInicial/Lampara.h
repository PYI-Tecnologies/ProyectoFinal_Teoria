#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Lampara {
public:

    glm::vec3 posicion;
    glm::vec3 escala;
    glm::vec3 rotacion;

    // Texturas para colores sólidos
    GLuint texturaNegroMetal;
    GLuint texturaBlancoLuz;

    Lampara(glm::vec3 posInicial);
    void Inicializar();
    void Draw(Shader& shader, GLuint VAO, glm::mat4 matrizPadre = glm::mat4(1.0f));
};
