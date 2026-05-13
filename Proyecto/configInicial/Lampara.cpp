#include "Lampara.h"
#include <iostream>

Lampara::Lampara(glm::vec3 posInicial) {
    posicion = posInicial;
    rotacion = glm::vec3(0.0f, 0.0f, 0.0f);
    escala = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Lampara::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Función auxiliar para crear texturas de color sólido en tiempo de ejecución
    auto crearColorSolido = [](GLuint& id, unsigned char r, unsigned char g, unsigned char b) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        unsigned char color[] = { r, g, b, 255 }; // RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glGenerateMipmap(GL_TEXTURE_2D);
        };

    // Generamos los colores base
    crearColorSolido(texturaNegroMetal, 25, 25, 25);     // Gris muy oscuro/Negro
    crearColorSolido(texturaBlancoLuz, 255, 255, 255);   // Blanco puro
}

void Lampara::Draw(Shader& shader, GLuint VAO, glm::mat4 matrizPadre) {
    // Matriz Raíz del objeto completo
    glm::mat4 mRaiz = glm::translate(matrizPadre, posicion);
    mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.z), glm::vec3(0.0f, 0.0f, 1.0f));
    mRaiz = glm::scale(mRaiz, escala);

    // Localizaciones en el Shader (Asegúrate de que coincidan con los nombres en tu .frag)
    GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLuint specularLoc = glGetUniformLocation(shader.Program, "material.specular");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");

    glBindVertexArray(VAO);

    //MARCO Y CABLES (NEGRO METÁLICO)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaNegroMetal);

    // Configuramos el material para que sea brillante y metálico
    if (specularLoc != -1) glUniform3f(specularLoc, 0.9f, 0.9f, 0.9f); // Reflejo especular alto
    if (shininessLoc != -1) glUniform1f(shininessLoc, 64.0f);          // Brillo muy concentrado

    // Base anclada al techo
    glm::mat4 mBase = glm::translate(mRaiz, glm::vec3(0.0f, 0.0f, 0.0f));
    mBase = glm::scale(mBase, glm::vec3(0.6f, 0.05f, 0.4f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBase));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cable colgante izquierdo
    glm::mat4 mCableIzq = glm::translate(mRaiz, glm::vec3(-0.25f, -0.4f, 0.0f));
    mCableIzq = glm::scale(mCableIzq, glm::vec3(0.02f, 0.8f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mCableIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cable colgante derecho
    glm::mat4 mCableDer = glm::translate(mRaiz, glm::vec3(0.25f, -0.4f, 0.0f));
    mCableDer = glm::scale(mCableDer, glm::vec3(0.02f, 0.8f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mCableDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Marco principal de la lámpara (Cuerpo)
    glm::mat4 mMarco = glm::translate(mRaiz, glm::vec3(0.0f, -0.8f, 0.0f));
    mMarco = glm::scale(mMarco, glm::vec3(1.6f, 0.1f, 0.8f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMarco));
    glDrawArrays(GL_TRIANGLES, 0, 36);


    //CARA INFERIOR (BLANCO DIFUSO)
    glBindTexture(GL_TEXTURE_2D, texturaBlancoLuz);

    // Configuramos el material para que sea opaco (luz difusa, sin reflejo especular)
    if (specularLoc != -1) glUniform3f(specularLoc, 0.0f, 0.0f, 0.0f);
    if (shininessLoc != -1) glUniform1f(shininessLoc, 1.0f);

    // Panel blanco (Se dibuja ligeramente debajo del marco y un poco más pequeño para encajar)
    glm::mat4 mPanel = glm::translate(mRaiz, glm::vec3(0.0f, -0.86f, 0.0f));
    mPanel = glm::scale(mPanel, glm::vec3(1.5f, 0.02f, 0.7f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPanel));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}