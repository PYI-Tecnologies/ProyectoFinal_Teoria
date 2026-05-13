#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Pikachu {
public:
    glm::vec3 posicion;
    glm::vec3 escala;
    glm::vec3 rotacion;
    // --- Ángulos de Articulación ---
    float rotCabezaX, rotCabezaY, rotCabezaZ;

    // Brazo Izquierdo
    float rotHombroIzqX, rotHombroIzqZ, rotHombroIzqY;
    float rotCodoIzq;

    // Brazo Derecho
    float rotHombroDerX, rotHombroDerZ, rotHombroDerY;
    float rotCodoDer;

    // Pierna Izquierda
    float rotPiernaIzqX;
    float rotRodillaIzq;

    // Pierna Derecha
    float rotPiernaDerX;
    float rotRodillaDer;

    // Textura principal
    GLuint texturaCuerpo;
    GLuint texturaOrejas;
    GLuint texturaPecas;
    GLuint texturaOjos;
    GLuint texturaIris;
    GLuint texturaGuantes;
    GLuint texturaBoca;
   

    Pikachu(glm::vec3 posInicial);
    void Inicializar();
    void Draw(Shader& shader, GLuint VAO);
};