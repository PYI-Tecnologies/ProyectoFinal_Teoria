#include "Reja.h"
#include "stb_image.h"
#include <iostream>

Reja::Reja(glm::vec3 posInicial) {
    posicion = posInicial;
    escala = glm::vec3(6.0f, 4.0f, 0.05f);
    
    desplazamientoX = 0.0f; // Inicia completamente cerrada
}

void Reja::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    int width, height, nrChannels;

    glGenTextures(1, &texturaBarrotes);
    glBindTexture(GL_TEXTURE_2D, texturaBarrotes);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cargar la imagen con transparencia
    unsigned char* data = stbi_load("images/rejapuerta.png", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Error al cargar la textura de la reja" << std::endl;
    }
    stbi_image_free(data);
}

void Reja::Draw(Shader& shader, GLuint VAO) {
    // Iniciamos la matriz limpia
    glm::mat4 model = glm::mat4(1.0f);

    // Nos posicionamos en el mundo y sumamos el deslizamiento
    model = glm::translate(model, posicion + glm::vec3(desplazamientoX, 0.0f, 0.0f));
   
    model = glm::scale(model, escala);

    GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    //MATERIAL: METAL PINTADO (Rugoso)
    GLuint specularLoc = glGetUniformLocation(shader.Program, "material.specular");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");

    // Especularidad moderada (0.4) para que la luz rebote sin ser un espejo cromado
    if (specularLoc != -1) glUniform3f(specularLoc, 0.4f, 0.4f, 0.4f);

    // Brillo bajo para que el reflejo se disperse, simulando pintura porosa
    if (shininessLoc != -1) glUniform1f(shininessLoc, 16.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaBarrotes);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}