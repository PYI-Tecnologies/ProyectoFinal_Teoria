#include "Stand.h"
#include <iostream>
#include "stb_image.h"

Stand::Stand(glm::vec3 posInicial) {
    posicion = posInicial;
    rotacion = glm::vec3(0.0f, 0.0f, 0.0f);
    escala = glm::vec3(1.0f, 1.0f, 1.0f);
    
}

void Stand::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    int width, height, nrChannels;
    unsigned char* data;
    //Textura Metal
    glGenTextures(1, &texturaMetal);
    glBindTexture(GL_TEXTURE_2D, texturaMetal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("images/Alumnio.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    // Color blanco mate
    //Textura Metal
    glGenTextures(1, &texturaPanel);
    glBindTexture(GL_TEXTURE_2D, texturaPanel);
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

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Stand::Draw(Shader& shader, GLuint VAO, glm::mat4 matrizPadre) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");
    if (shininessLoc != -1) glUniform1f(shininessLoc, 32.0f);
    glBindVertexArray(VAO);
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaMetal);
    glUniform1i(matDiffLoc, 0);

    glm::mat4 mRaiz = glm::translate(matrizPadre, posicion);
	mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.z), glm::vec3(0.0f, 0.0f, 1.0f));
    mRaiz = glm::scale(mRaiz, escala);
    
    // Definimos un grosor general para todos los tubos
    float grosor = 0.05f;

    // PILARES VERTICALES
    // Altura de 3.0 unidades, por lo tanto su centro debe estar en Y = 1.5 para tocar el piso

    // Pilar Frontal Izquierdo
    glm::mat4 mPilarFI = glm::translate(mRaiz, glm::vec3(-1.5f, 1.5f, 1.0f));
    mPilarFI = glm::scale(mPilarFI, glm::vec3(grosor, 3.0f, grosor));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPilarFI));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pilar Trasero Izquierdo
    glm::mat4 mPilarTI = glm::translate(mRaiz, glm::vec3(-1.5f, 1.5f, -1.0f));
    mPilarTI = glm::scale(mPilarTI, glm::vec3(grosor, 3.0f, grosor));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPilarTI));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pilar Trasero Derecho
    glm::mat4 mPilarTD = glm::translate(mRaiz, glm::vec3(1.5f, 1.5f, -1.0f));
    mPilarTD = glm::scale(mPilarTD, glm::vec3(grosor, 3.0f, grosor));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPilarTD));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pilar Frontal Derecho
    glm::mat4 mPilarFD = glm::translate(mRaiz, glm::vec3(1.5f, 1.5f, 1.0f));
    mPilarFD = glm::scale(mPilarFD, glm::vec3(grosor, 3.0f, grosor));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPilarFD));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //BASES (Tubos horizontales a ras de suelo)
    // Altura Y = 0.05 para que no raspen con el piso

    // Base Izquierda (Conecta Pilar Frontal Izq con Trasero Izq)
    glm::mat4 mBaseIzq = glm::translate(mRaiz, glm::vec3(-1.5f, 0.05f, 0.0f));
    mBaseIzq = glm::scale(mBaseIzq, glm::vec3(grosor, grosor, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBaseIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Base Trasera (Conecta Pilar Trasero Izq con Trasero Der)
    glm::mat4 mBaseTras = glm::translate(mRaiz, glm::vec3(0.0f, 0.05f, -1.0f));
    mBaseTras = glm::scale(mBaseTras, glm::vec3(3.0f, grosor, grosor));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBaseTras));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Base Derecha (Conecta Pilar Frontal Der con Trasero Der)
    glm::mat4 mBaseDer = glm::translate(mRaiz, glm::vec3(1.5f, 0.05f, 0.0f));
    mBaseDer = glm::scale(mBaseDer, glm::vec3(grosor, grosor, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBaseDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // TRAVESAÑOS MEDIOS (Soporte horizontal intermedio)
    // Los colocamos a la mitad de la altura, en Y = 1.5

    // Travesaño Izquierdo
    glm::mat4 mTravIzq = glm::translate(mRaiz, glm::vec3(-1.5f, 1.5f, 0.0f));
    mTravIzq = glm::scale(mTravIzq, glm::vec3(grosor, grosor, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mTravIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Travesaño Trasero
    glm::mat4 mTravTras = glm::translate(mRaiz, glm::vec3(0.0f, 1.5f, -1.0f));
    mTravTras = glm::scale(mTravTras, glm::vec3(3.0f, grosor, grosor));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mTravTras));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Travesaño Derecho
    glm::mat4 mTravDer = glm::translate(mRaiz, glm::vec3(1.5f, 1.5f, 0.0f));
    mTravDer = glm::scale(mTravDer, glm::vec3(grosor, grosor, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mTravDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //PANELES INDEPENDIENTES(Blancos) 
    glBindTexture(GL_TEXTURE_2D, texturaPanel);

    // PANELES INFERIORES (Altura central Y = 0.775, Altura total = 1.45)

    // Panel Inferior Izquierdo
    glm::mat4 mPanelInfIzq = glm::translate(mRaiz, glm::vec3(-1.5f, 0.775f, 0.0f));
    mPanelInfIzq = glm::scale(mPanelInfIzq, glm::vec3(0.02f, 1.45f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPanelInfIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Panel Inferior Derecho
    glm::mat4 mPanelInfDer = glm::translate(mRaiz, glm::vec3(1.5f, 0.775f, 0.0f));
    mPanelInfDer = glm::scale(mPanelInfDer, glm::vec3(0.02f, 1.45f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPanelInfDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Panel Inferior Trasero
    glm::mat4 mPanelInfTras = glm::translate(mRaiz, glm::vec3(0.0f, 0.775f, -1.0f));
    mPanelInfTras = glm::scale(mPanelInfTras, glm::vec3(3.0f, 1.45f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPanelInfTras));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // PANELES SUPERIORES (Altura central Y = 2.25, Altura total = 1.5)

    // Panel Superior Izquierdo
    glm::mat4 mPanelSupIzq = glm::translate(mRaiz, glm::vec3(-1.5f, 2.25f, 0.0f));
    mPanelSupIzq = glm::scale(mPanelSupIzq, glm::vec3(0.02f, 1.5f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPanelSupIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Panel Superior Derecho
    glm::mat4 mPanelSupDer = glm::translate(mRaiz, glm::vec3(1.5f, 2.25f, 0.0f));
    mPanelSupDer = glm::scale(mPanelSupDer, glm::vec3(0.02f, 1.5f, 2.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPanelSupDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Panel Superior Trasero
    glm::mat4 mPanelSupTras = glm::translate(mRaiz, glm::vec3(0.0f, 2.25f, -1.0f));
    mPanelSupTras = glm::scale(mPanelSupTras, glm::vec3(3.0f, 1.5f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPanelSupTras));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}