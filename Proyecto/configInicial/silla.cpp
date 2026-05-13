#include "Silla.h"
#include "stb_image.h"
#include <iostream>

Silla::Silla(glm::vec3 posInicial) {
    posicion = posInicial;
    escala = glm::vec3(1.0f, 1.0f, 1.0f);
    rotacion = glm::vec3(0.0f, 0.0f, 0.0f);
    anguloPlegado = 80.0f;
}

void Silla::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    int width, height, nrChannels;
    unsigned char* data;

    //Textura Estructura
    glGenTextures(1, &texturaEstructura);
    glBindTexture(GL_TEXTURE_2D, texturaEstructura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("images/metal.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    stbi_image_free(data);

    //Textura Asiento y Respaldo
    glGenTextures(1, &texturaAsiento);
    glBindTexture(GL_TEXTURE_2D, texturaAsiento);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("images/plastico.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Silla::Draw(Shader& shader, GLuint VAO, glm::mat4 matrizPadre) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");
    if (shininessLoc != -1) glUniform1f(shininessLoc, 32.0f);

    glBindVertexArray(VAO);
    glDisable(GL_BLEND);
    glm::mat4 modelSilla = glm::translate(matrizPadre, posicion);
    
	modelSilla = glm::rotate(modelSilla, glm::radians(rotacion.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelSilla = glm::rotate(modelSilla, glm::radians(rotacion.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelSilla = glm::rotate(modelSilla, glm::radians(rotacion.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelSilla = glm::scale(modelSilla, escala);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(matDiffLoc, 0);

    // Ángulos fijos de la estructura
    float anguloPatasDelanteras = -10.0f;

    glBindTexture(GL_TEXTURE_2D, texturaEstructura);

    // Pata Delantera Izquierda
    glm::mat4 pataDI = glm::translate(modelSilla, glm::vec3(-0.2f, 0.5f, 0.1f));
   
    pataDI = glm::scale(pataDI, glm::vec3(0.03f, 1.0f, 0.03f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pataDI));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pata Delantera Derecha
    glm::mat4 pataDD = glm::translate(modelSilla, glm::vec3(0.2f, 0.5f, 0.1f));
   
    pataDD = glm::scale(pataDD, glm::vec3(0.03f, 1.0f, 0.03f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pataDD));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_2D, texturaAsiento);
   
    // Respaldo (Fijo a las patas delanteras)
    glm::mat4 respaldo = glm::translate(modelSilla, glm::vec3(0.0f, 0.85f, 0.1f));
    
    respaldo = glm::scale(respaldo, glm::vec3(0.38f, 0.15f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(respaldo));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Matriz Pivote Central (Esta matriz controla la rotación base del asiento)
    glm::mat4 pivoteAsiento = glm::translate(modelSilla, glm::vec3(0.0f, 0.45f, 0.12f));
    pivoteAsiento = glm::rotate(pivoteAsiento, glm::radians(anguloPatasDelanteras - anguloPlegado), glm::vec3(1.0f, 0.0f, 0.0f));

    // Dibujo del Asiento (Hijo del Pivote Central)
    glm::mat4 asiento = glm::translate(pivoteAsiento, glm::vec3(0.0f, 0.0f, 0.18f));
    asiento = glm::scale(asiento, glm::vec3(0.38f, 0.02f, 0.35f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(asiento));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //  PATAS ARTICULADAS 
    glBindTexture(GL_TEXTURE_2D, texturaEstructura);

    // Creamos un punto de anclaje para las patas 
    glm::mat4 anclajePatas = glm::translate(pivoteAsiento, glm::vec3(0.0f, 0.0f, 0.32f));

    // Matemáticas del pliegue: 
    // 15.0f nos da una inclinación inicial más recta pero estable.
    // Multiplicar el anguloPlegado por 0.85 compensa el giro del asiento y cierra las patas hacia el frente.
    float inclinacionRelativa = 8.0f + (anguloPlegado * 0.97f);

    // Pata Trasera Izquierda
    glm::mat4 pataTI = glm::translate(anclajePatas, glm::vec3(-0.16f, 0.0f, 0.0f));
    pataTI = glm::rotate(pataTI, glm::radians(inclinacionRelativa), glm::vec3(1.0f, 0.0f, 0.0f));
    pataTI = glm::translate(pataTI, glm::vec3(0.0f, -0.27f, 0.0f)); // Traslación de la escala hacia abajo
    pataTI = glm::scale(pataTI, glm::vec3(0.03f, 0.55f, 0.03f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pataTI));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pata Trasera Derecha
    glm::mat4 pataTD = glm::translate(anclajePatas, glm::vec3(0.16f, 0.0f, 0.0f));
    pataTD = glm::rotate(pataTD, glm::radians(inclinacionRelativa), glm::vec3(1.0f, 0.0f, 0.0f));
    pataTD = glm::translate(pataTD, glm::vec3(0.0f, -0.27f, 0.0f));
    pataTD = glm::scale(pataTD, glm::vec3(0.03f, 0.55f, 0.03f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pataTD));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}