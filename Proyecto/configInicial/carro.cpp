#include "Carro.h"
#include "stb_image.h" // Incluimos la librería para leer la imagen
#include <iostream>

Carro::Carro(glm::vec3 posInicial) {
    posicion = posInicial;
    escala = glm::vec3(1.0f, 1.0f, 1.0f);
    avance = 0.0f;
    rotacionLlantas = 0.0f;
    giroVolante = 0.0f;
}

void Carro::Inicializar() {
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width, height, nrChannels;
    //Textura del Chasis
    glGenTextures(1, &texturaChasis);
    glBindTexture(GL_TEXTURE_2D, texturaChasis);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* dataChasis = stbi_load("images/cd7.jpg", &width, &height, &nrChannels, 0);
    if (dataChasis) {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, dataChasis);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: Fallo al cargar textura del chasis." << std::endl;
    }
    stbi_image_free(dataChasis);
   

    //Textura de la Cabina 
    glGenTextures(1, &texturaCabina);
    glBindTexture(GL_TEXTURE_2D, texturaCabina);

    // Arreglo RGBA: Azul claro con canal Alfa en 100 (parcialmente transparente)
    unsigned char colorCabina[] = { 150, 200, 255, 100 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorCabina);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Textura Faros delanteros (Ambar)

    glGenTextures(1, &texturaFaroDelantero);
    glBindTexture(GL_TEXTURE_2D, texturaFaroDelantero);
    unsigned char colorFaroDel[] = { 255, 191, 0, 100 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorFaroDel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Textura Faros Traseros (rojos)
    glGenTextures(1, &texturaFaroTrasero);
    glBindTexture(GL_TEXTURE_2D, texturaFaroTrasero);
    unsigned char colorFaroTras[] = { 200, 20, 20, 100 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorFaroTras);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // --- Specular Map: Metal (Blanco = Alto brillo para chasis y rines) ---
    glGenTextures(1, &specMetal);
    glBindTexture(GL_TEXTURE_2D, specMetal);
    unsigned char colorSpecMetal[] = { 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorSpecMetal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // --- Specular Map: Goma/Caucho (Casi Negro = Sin brillo para llantas) ---
    glGenTextures(1, &specGoma);
    glBindTexture(GL_TEXTURE_2D, specGoma);
    unsigned char colorSpecGoma[] = { 10, 10, 10 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorSpecGoma);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    //Textura de las Llantas 
    glGenTextures(1, &texturaLlanta);
    glBindTexture(GL_TEXTURE_2D, texturaLlanta);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* dataLlanta = stbi_load("images/llanta1.jpg", &width, &height, &nrChannels, 0);
    if (dataLlanta)
    {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, dataLlanta);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: Fallo al cargar la textura de la llanta desde la clase." << std::endl;
    }
    stbi_image_free(dataLlanta);
    
	//llanta lateral (rin)
    glGenTextures(1, &texturaLlantaLateral);
    glBindTexture(GL_TEXTURE_2D, texturaLlantaLateral);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* dataRin = stbi_load("images/rin2.png", &width, &height, &nrChannels, 0);
    if (dataRin)
    {
        GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, dataRin);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: Fallo al cargar la textura de la llanta desde la clase." << std::endl;
    }
    stbi_image_free(dataRin);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Carro::Draw(Shader& shader, GLuint VAO) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");
    GLint matSpecLoc = glGetUniformLocation(shader.Program, "material.specular");
    GLint matShineLoc = glGetUniformLocation(shader.Program, "material.shininess");

    glBindVertexArray(VAO);

    glm::mat4 modelCarro = glm::mat4(1.0f);
    modelCarro = glm::translate(modelCarro, posicion);
    modelCarro = glm::translate(modelCarro, glm::vec3(avance, 0.0f, 0.0f));
    modelCarro = glm::rotate(modelCarro, glm::radians(rotacion.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelCarro = glm::rotate(modelCarro, glm::radians(rotacion.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelCarro = glm::rotate(modelCarro, glm::radians(rotacion.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelCarro = glm::scale(modelCarro, escala);
    // DIBUJAR CHASIS
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Textura 0: Color del chasis
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaChasis);
    glUniform1i(matDiffLoc, 0);

    // Textura 1: Brillo Metálico
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specMetal);
    glUniform1i(matSpecLoc, 1);
    glUniform1f(matShineLoc, 32.0f); // Concentra el brillo como un metal pulido

    glm::mat4 modelChasis = glm::translate(modelCarro, glm::vec3(0.0f, 0.5f, 0.0f));
    modelChasis = glm::scale(modelChasis, glm::vec3(2.5f, 0.5f, 1.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelChasis));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisable(GL_BLEND);
	

    //DIBUJAR FAROS DELANTEROS
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaFaroDelantero);
    glActiveTexture(GL_TEXTURE1); // Le quitamos el brillo metálico al faro
    glBindTexture(GL_TEXTURE_2D, specGoma);

    // Faro Delantero Izquierdo
    glm::mat4 mFaroDI = glm::translate(modelCarro, glm::vec3(1.25f, 0.5f, 0.4f));
    mFaroDI = glm::scale(mFaroDI, glm::vec3(0.05f, 0.15f, 0.25f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mFaroDI));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Faro Delantero Derecho
    glm::mat4 mFaroDD = glm::translate(modelCarro, glm::vec3(1.25f, 0.5f, -0.4f));
    mFaroDD = glm::scale(mFaroDD, glm::vec3(0.05f, 0.15f, 0.25f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mFaroDD));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //DIBUJAR FAROS TRASEROS
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaFaroTrasero);
    glActiveTexture(GL_TEXTURE1); // Le quitamos el brillo metálico al faro
    glBindTexture(GL_TEXTURE_2D, specGoma);
    // Faro Trasero Izquierdo
    glm::mat4 mFaroTI = glm::translate(modelCarro, glm::vec3(-1.25f, 0.5f, 0.4f));
    mFaroTI = glm::scale(mFaroTI, glm::vec3(0.05f, 0.15f, 0.25f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mFaroTI));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Faro Trasero Derecho
    glm::mat4 mFaroTD = glm::translate(modelCarro, glm::vec3(-1.25f, 0.5f, -0.4f));
    mFaroTD = glm::scale(mFaroTD, glm::vec3(0.05f, 0.15f, 0.25f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mFaroTD));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisable(GL_BLEND);

    // DIBUJAR LLANTAS
    glm::vec3 posLlantas[] = {
        glm::vec3(1.0f, 0.2f, 0.7f),   // DI
        glm::vec3(1.0f, 0.2f, -0.7f),  // DD
        glm::vec3(-1.0f, 0.2f, 0.7f),  // TI
        glm::vec3(-1.0f, 0.2f, -0.7f)  // TD
    };

    for (int i = 0; i < 4; i++) {
        glm::mat4 m = glm::translate(modelCarro, posLlantas[i]);
        if (i < 2) m = glm::rotate(m, glm::radians(giroVolante), glm::vec3(0.0f, 1.0f, 0.0f));

        m = glm::rotate(m, glm::radians(rotacionLlantas), glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::scale(m, glm::vec3(0.4f, 0.4f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
        
        // DIBUJAMOS LOS RINES (Metálicos)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturaLlantaLateral);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specMetal);
        glUniform1f(matShineLoc, 32.0f); // Brillo de metal

        glDrawArrays(GL_TRIANGLES, 0, 12);

        // DIBUJAMOS LA BANDA DE RODAMIENTO (Caucho opaco)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturaLlanta);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specGoma);
        glUniform1f(matShineLoc, 4.0f); // Brillo muy disperso y apagado

        glDrawArrays(GL_TRIANGLES, 12, 24);
    }
    
  
    // DIBUJAR CABINA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaCabina);
    glActiveTexture(GL_TEXTURE1); // Le quitamos el brillo metálico al faro
    glBindTexture(GL_TEXTURE_2D, specGoma);
    glm::mat4 modelCabina = glm::translate(modelCarro, glm::vec3(-0.2f, 1.0f, 0.0f));
    modelCabina = glm::scale(modelCabina, glm::vec3(1.2f, 0.5f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCabina));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Limpieza
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}