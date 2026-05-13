#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Carro {
public:
    glm::vec3 posicion;
    glm::vec3 escala;
    glm::vec3 rotacion;
    float rotacionLlantas;
    float giroVolante;
    float avance;

    GLuint texturaChasis;
    GLuint texturaCabina;
    GLuint texturaLlanta;
    GLuint texturaLlantaLateral;
    GLuint texturaFaroDelantero;
    GLuint texturaFaroTrasero;
    GLuint specMetal;
    GLuint specGoma;

    Carro(glm::vec3 posInicial);

    void Inicializar();

    
    void Draw(Shader& shader, GLuint VAO);
};