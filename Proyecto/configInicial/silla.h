#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Silla {
public:
    glm::vec3 posicion;
    glm::vec3 escala;
	glm::vec3 rotacion;
    // Variable para controlar la animación (0.0f = Abierta, 90.0f = Plegada)
    float anguloPlegado;

    GLuint texturaEstructura;
    GLuint texturaAsiento;

    Silla(glm::vec3 posInicial);
    void Inicializar();
    void Draw(Shader& shader, GLuint VAO, glm::mat4 matrizPadre = glm::mat4(1.0f));
};
