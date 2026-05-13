#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Tren {
public:
    glm::vec3 posicion;
    glm::vec3 escala;
    float avance;
    float rotacionLlantas;

    GLuint texturaFrontal;
    GLuint texturaLaterales;
    GLuint texturaTecho;
    GLuint texturaCupulaLat;
    GLuint texturaRuedaLoc;
    GLuint texturaPlataforma;
    

    Tren(glm::vec3 posInicial);
    void Inicializar();
    void Draw(Shader& shader, GLuint VAO);
};