#include "Tren.h"
#include "stb_image.h"
#include <iostream>

Tren::Tren(glm::vec3 posInicial) {
    posicion = posInicial;
    escala = glm::vec3(1.0f, 1.0f, 1.0f);
    
    avance = 0.0f;
    rotacionLlantas = 0.0f;
}

void Tren::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    int width, height, nrChannels;
    unsigned char* data;

    //Textura Frontal
    glGenTextures(1, &texturaFrontal);
    glBindTexture(GL_TEXTURE_2D, texturaFrontal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("images/frontal.png", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura frontal." << std::endl;
    }
    stbi_image_free(data);



    //Textura Laterales
    glGenTextures(1, &texturaLaterales);
    glBindTexture(GL_TEXTURE_2D, texturaLaterales);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("images/laterales.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura laterales." << std::endl;
    }
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);
    //Textura Techo
    glGenTextures(1, &texturaTecho);
    glBindTexture(GL_TEXTURE_2D, texturaTecho);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("images/techoCuerpo.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura techo." << std::endl;
    }
    stbi_image_free(data);

    //Textura Cúpula Lateral
    glGenTextures(1, &texturaCupulaLat);
    glBindTexture(GL_TEXTURE_2D, texturaCupulaLat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("images/cupulaLateral.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura cupula lateral." << std::endl;
    }
    stbi_image_free(data);

    //Textura Rueda Locomotora
    glGenTextures(1, &texturaRuedaLoc);
    glBindTexture(GL_TEXTURE_2D, texturaRuedaLoc);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("images/rueda_locomotora.png", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura rueda locomotora." << std::endl;
    }
    stbi_image_free(data);

    //Textura Plataforma
    glGenTextures(1, &texturaPlataforma);
    glBindTexture(GL_TEXTURE_2D, texturaPlataforma);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("images/plataforma.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura plataforma." << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tren::Draw(Shader& shader, GLuint VAO) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");
    if (shininessLoc != -1) glUniform1f(shininessLoc, 128.0f);

    glBindVertexArray(VAO);
    glDisable(GL_BLEND);

    glm::mat4 modelTren = glm::mat4(1.0f);
    modelTren = glm::translate(modelTren, posicion);
    modelTren = glm::translate(modelTren, glm::vec3(avance, 0.0f, 0.0f));
    modelTren = glm::scale(modelTren, escala);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(matDiffLoc, 0);

    // --- 1. PLATAFORMA ---
    glBindTexture(GL_TEXTURE_2D, texturaPlataforma);
    glm::mat4 plat = glm::translate(modelTren, glm::vec3(0.0f, 0.5f, 0.0f));
    plat = glm::scale(plat, glm::vec3(3.0f, 0.2f, 1.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(plat));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // --- 2. CUERPO CENTRAL ---
    glm::mat4 cuerpo = glm::translate(modelTren, glm::vec3(0.0f, 1.1f, 0.0f));
    cuerpo = glm::scale(cuerpo, glm::vec3(2.6f, 1.0f, 1.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cuerpo));

    // Laterales
    glBindTexture(GL_TEXTURE_2D, texturaLaterales);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    // Frontales
    glBindTexture(GL_TEXTURE_2D, texturaFrontal);
    glDrawArrays(GL_TRIANGLES, 12, 12);
    // Superior e inferior (Tapas)
    glBindTexture(GL_TEXTURE_2D, texturaTecho);
    glDrawArrays(GL_TRIANGLES, 24, 12);

    // --- 3. TECHO DEL CUERPO ---
    glBindTexture(GL_TEXTURE_2D, texturaTecho);
    glm::mat4 tCuerpo = glm::translate(modelTren, glm::vec3(0.0f, 1.65f, 0.0f));
    tCuerpo = glm::scale(tCuerpo, glm::vec3(2.8f, 0.1f, 1.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tCuerpo));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // --- 4. CÚPULA SUPERIOR ---
    glm::mat4 cup = glm::translate(modelTren, glm::vec3(0.0f, 1.9f, 0.0f));
    cup = glm::scale(cup, glm::vec3(0.8f, 0.4f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cup));

    // Laterales de la cúpula
    glBindTexture(GL_TEXTURE_2D, texturaCupulaLat);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    // Superior e inferior de la cúpula
    glBindTexture(GL_TEXTURE_2D, texturaTecho);
    glDrawArrays(GL_TRIANGLES, 24, 12);

    // --- 5. TECHO DE LA CÚPULA ---
    glBindTexture(GL_TEXTURE_2D, texturaTecho);
    glm::mat4 tCup = glm::translate(modelTren, glm::vec3(0.0f, 2.15f, 0.0f));
    tCup = glm::scale(tCup, glm::vec3(1.0f, 0.1f, 1.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tCup));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // --- 6. LLANTAS ---
    float posXRuedas[] = { 1.2f, 0.6f, -0.6f, -1.2f };
    float posZRuedas[] = { 0.4f, -0.4f };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            glm::mat4 m = glm::translate(modelTren, glm::vec3(posXRuedas[i], 0.2f, posZRuedas[j]));
            m = glm::rotate(m, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 1.0f));
            m = glm::scale(m, glm::vec3(0.4f, 0.4f, 0.15f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));

            // Caras laterales (Rueda frontal y trasera)
            glBindTexture(GL_TEXTURE_2D, texturaRuedaLoc);
            glDrawArrays(GL_TRIANGLES, 0, 12);
            
        }
    }
    // BIELAS DE ACOPLAMIENTO
    glBindTexture(GL_TEXTURE_2D, texturaPlataforma);

    float radioPerno = 0.1f;
    float offsetXBiela = radioPerno * glm::cos(glm::radians(rotacionLlantas));
    float offsetYBiela = radioPerno * glm::sin(glm::radians(rotacionLlantas));

    float centrosXBogey[] = { 0.9f, -0.9f };
    float posZBielas[] = { 0.51f, -0.51f };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            glm::mat4 m = glm::translate(modelTren, glm::vec3(centrosXBogey[i] + offsetXBiela, 0.2f + offsetYBiela, posZBielas[j]));
            m = glm::scale(m, glm::vec3(0.8f, 0.05f, 0.05f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}