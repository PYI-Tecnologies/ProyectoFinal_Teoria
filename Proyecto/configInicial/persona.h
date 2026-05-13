#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Persona {
public:
    glm::vec3 posicion;
    glm::vec3 escala;
    glm::vec3 rotacion;
    // --- Ángulos de Articulación ---
    float rotCuelloX, rotCuelloY;

    // Brazo Izquierdo
    float rotHombroIzqX, rotHombroIzqZ;
    float rotCodoIzq;

    // Brazo Derecho
    float rotHombroDerX, rotHombroDerZ;
    float rotCodoDer;

    // Piernas
    float rotPiernaIzqX, rotRodillaIzq;
    float rotPiernaDerX, rotRodillaDer;

    // --- Texturas de Colores Sólidos ---
    GLuint texturaPiel;
    GLuint texturaPlayera;
    GLuint texturaPantalon;
    GLuint texturaCabello;
    GLuint texturaEsclerotica;
    GLuint texturaIris;
    GLuint texturaSandalias;

    Persona(glm::vec3 posInicial);
    void Inicializar();
    void Draw(Shader& shader, GLuint VAO);
};