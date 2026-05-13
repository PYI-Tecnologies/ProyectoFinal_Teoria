#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Mesa {
public:
    glm::vec3 posicion;
    glm::vec3 escala;
    glm::vec3 rotacion;
    // 90.0f = Cerrada, 180.0f = Abierta
    float anguloApertura;

    GLuint texturaTablon;
    GLuint texturaPatas;

    Mesa(glm::vec3 posInicial);
    void Inicializar();
    void Draw(Shader& shader, GLuint VAO, glm::mat4 matrizPadre = glm::mat4(1.0f));
};