#include "Mesa.h"
#include "stb_image.h"
#include <iostream>

Mesa::Mesa(glm::vec3 posInicial) {
    posicion = posInicial;
	rotacion = glm::vec3(0.0f, 0.0f, 0.0f);
    escala = glm::vec3(1.0f, 1.0f, 1.0f);
    anguloApertura = 90.0f; // Inicia cerrada según instrucciones
}

void Mesa::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    int width, height, nrChannels;
    unsigned char* data;

    // --- 1. Textura del Tablón 
    glGenTextures(1, &texturaTablon);
    glBindTexture(GL_TEXTURE_2D, texturaTablon);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("images/tablon.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    stbi_image_free(data);

    // --- 2. Textura de las Patas (Metal) ---
    glGenTextures(1, &texturaPatas);
    glBindTexture(GL_TEXTURE_2D, texturaPatas);
    data = stbi_load("images/plataforma.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesa::Draw(Shader& shader, GLuint VAO, glm::mat4 matrizPadre) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");
    GLint matSpecLoc = glGetUniformLocation(shader.Program, "material.specular");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");
    if (shininessLoc != -1) glUniform1f(shininessLoc, 32.0f);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(matDiffLoc, 0);

    glm::mat4 modelMesa = glm::translate(matrizPadre, posicion);
	modelMesa = glm::rotate(modelMesa, glm::radians(rotacion.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMesa = glm::rotate(modelMesa, glm::radians(rotacion.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMesa = glm::rotate(modelMesa, glm::radians(rotacion.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMesa = glm::scale(modelMesa, escala);
    // rotTablero: 90 (cerrada vertical) -> 0 (abierta horizontal)
    float rotTablero = 180.0f - anguloApertura;

    // rotPata: 
    // Cuando la mesa está abierta (180), la resta da 0°, apuntando directo al piso.
    // Cuando la mesa está cerrada (90), la pata rota 90° hacia adentro para apuntar a la bisagra.
    float rotPataIzq = 180.0f - anguloApertura;
    float rotPataDer = anguloApertura - 180.0f;

    //MITAD IZQUIERDA
    glm::mat4 mBisagraIzq = glm::rotate(modelMesa, glm::radians(rotTablero), glm::vec3(0.0f, 0.0f, 1.0f));

    // DIBUJO TABLERO IZQUIERDO
    glBindTexture(GL_TEXTURE_2D, texturaTablon);
    glm::mat4 tableroIzq = glm::translate(mBisagraIzq, glm::vec3(-0.5f, 0.025f, 0.0f));
    tableroIzq = glm::scale(tableroIzq, glm::vec3(1.0f, 0.05f, 0.8f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tableroIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // DIBUJO PATAS IZQUIERDAS
    glBindTexture(GL_TEXTURE_2D, texturaPatas);
    glm::mat4 mPataIzqBase = glm::translate(mBisagraIzq, glm::vec3(-0.95f, -0.01f, 0.0f));
    mPataIzqBase = glm::rotate(mPataIzqBase, glm::radians(rotPataIzq), glm::vec3(0.0f, 0.0f, 1.0f));

    float posZ[] = { 0.35f, -0.35f };
    for (int i = 0; i < 2; i++) {
        glm::mat4 p = glm::translate(mPataIzqBase, glm::vec3(0.0f, -0.35f, posZ[i]));
        p = glm::scale(p, glm::vec3(0.03f, 0.7f, 0.03f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(p));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // MITAD DERECHA
    glm::mat4 mBisagraDer = glm::rotate(modelMesa, glm::radians(-rotTablero), glm::vec3(0.0f, 0.0f, 1.0f));

    // DIBUJO TABLERO DERECHO
    glBindTexture(GL_TEXTURE_2D, texturaTablon);
    glm::mat4 tableroDer = glm::translate(mBisagraDer, glm::vec3(0.5f, 0.025f, 0.0f));
    tableroDer = glm::scale(tableroDer, glm::vec3(1.0f, 0.05f, 0.8f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tableroDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // DIBUJO PATAS DERECHAS
    glBindTexture(GL_TEXTURE_2D, texturaPatas);
    glm::mat4 mPataDerBase = glm::translate(mBisagraDer, glm::vec3(0.95f, -0.01f, 0.0f));
    mPataDerBase = glm::rotate(mPataDerBase, glm::radians(rotPataDer), glm::vec3(0.0f, 0.0f, 1.0f));

    for (int i = 0; i < 2; i++) {
        glm::mat4 p = glm::translate(mPataDerBase, glm::vec3(0.0f, -0.35f, posZ[i]));
        p = glm::scale(p, glm::vec3(0.03f, 0.7f, 0.03f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(p));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }


    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}