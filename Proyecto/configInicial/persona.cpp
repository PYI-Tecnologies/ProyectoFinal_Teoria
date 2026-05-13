#include "Persona.h"
#include <iostream>

Persona::Persona(glm::vec3 posInicial) {
    posicion = posInicial;
	rotacion = glm::vec3(0.0f, 0.0f, 0.0f);
    escala = glm::vec3(1.0f, 1.0f, 1.0f);

    // Inicialización de pose
    rotCuelloX = 0.0f; rotCuelloY = 0.0f;
    rotHombroIzqX = 0.0f; rotHombroIzqZ = -5.0f;// Brazo en descanso
    rotCodoIzq = 0.0f;
    rotHombroDerX = 0.0f; rotHombroDerZ = 5.0f; // Brazo en descanso
    rotCodoDer = 0.0f;
    rotPiernaIzqX = 0.0f; rotRodillaIzq = 0.0f;
    rotPiernaDerX = 0.0f; rotRodillaDer = 0.0f;
}

void Persona::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Función auxiliar clásica vista en el curso para colores sólidos
    auto crearColorSolido = [](GLuint& id, unsigned char r, unsigned char g, unsigned char b) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        unsigned char color[] = { r, g, b };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        };

    crearColorSolido(texturaPiel, 235, 195, 160);       // Tono de piel
    crearColorSolido(texturaPlayera, 240, 240, 240);    // Playera blanca
    crearColorSolido(texturaPantalon, 180, 160, 130);   // Pantalón corto (Khaki)
    crearColorSolido(texturaCabello, 40, 25, 20);       // Cabello castaño oscuro/negro
    crearColorSolido(texturaEsclerotica, 255, 255, 255);// Blanco del ojo
    crearColorSolido(texturaIris, 30, 15, 10);          // Ojo oscuro
    crearColorSolido(texturaSandalias, 25, 25, 25);     // Sandalias negras

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Persona::Draw(Shader& shader, GLuint VAO) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");
    // Le asignamos un brillo muy bajo (4.0f) para que la luz sea difusa
    if (shininessLoc != -1) glUniform1f(shininessLoc, 4.0f);
    glBindVertexArray(VAO);
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(matDiffLoc, 0);

    // Matriz Raíz
    glm::mat4 mRaiz = glm::translate(glm::mat4(1.0f), posicion);
	mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.z), glm::vec3(0.0f, 0.0f, 1.0f));
    mRaiz = glm::scale(mRaiz, escala);

    // TORSO (Playera) 
    glBindTexture(GL_TEXTURE_2D, texturaPlayera);
    // Posicionamos el torso a una altura de 1.8 para dejar espacio a las piernas
    glm::mat4 mTorsoBase = glm::translate(mRaiz, glm::vec3(0.0f, 1.8f, 0.0f));
    glm::mat4 mTorsoDraw = glm::scale(mTorsoBase, glm::vec3(0.7f, 1.0f, 0.4f)); // Rectángulo vertical
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mTorsoDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // CABEZA Y CUELLO
    // Cuello (Pivote)
    glBindTexture(GL_TEXTURE_2D, texturaPiel);
    glm::mat4 mCuelloPivote = glm::translate(mTorsoBase, glm::vec3(0.0f, 0.5f, 0.0f));
    mCuelloPivote = glm::rotate(mCuelloPivote, glm::radians(rotCuelloY), glm::vec3(0.0f, 1.0f, 0.0f));
    mCuelloPivote = glm::rotate(mCuelloPivote, glm::radians(rotCuelloX), glm::vec3(1.0f, 0.0f, 0.0f));
    
    // Cabeza (Piel)
    glBindTexture(GL_TEXTURE_2D, texturaPiel);
    glm::mat4 mCabezaDraw = glm::translate(mCuelloPivote, glm::vec3(0.0f, 0.3f, 0.0f));
    mCabezaDraw = glm::scale(mCabezaDraw, glm::vec3(0.45f, 0.5f, 0.45f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mCabezaDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Nariz
    glm::mat4 mNariz = glm::translate(mCuelloPivote, glm::vec3(0.0f, 0.3f, 0.23f));
    mNariz = glm::scale(mNariz, glm::vec3(0.08f, 0.15f, 0.08f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mNariz));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Orejas
    glm::mat4 mOrejaIzq = glm::translate(mCuelloPivote, glm::vec3(-0.25f, 0.3f, 0.0f));
    mOrejaIzq = glm::scale(mOrejaIzq, glm::vec3(0.05f, 0.15f, 0.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOrejaIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mOrejaDer = glm::translate(mCuelloPivote, glm::vec3(0.25f, 0.3f, 0.0f));
    mOrejaDer = glm::scale(mOrejaDer, glm::vec3(0.05f, 0.15f, 0.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOrejaDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Esclerótica (Ojos blancos)
    glBindTexture(GL_TEXTURE_2D, texturaEsclerotica);
    glm::mat4 mOjoIzq = glm::translate(mCuelloPivote, glm::vec3(-0.1f, 0.38f, 0.23f));
    mOjoIzq = glm::scale(mOjoIzq, glm::vec3(0.12f, 0.08f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOjoIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mOjoDer = glm::translate(mCuelloPivote, glm::vec3(0.1f, 0.38f, 0.23f));
    mOjoDer = glm::scale(mOjoDer, glm::vec3(0.12f, 0.08f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOjoDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Iris (Ojos oscuros)
    glBindTexture(GL_TEXTURE_2D, texturaIris);
    glm::mat4 mIrisIzq = glm::translate(mCuelloPivote, glm::vec3(-0.1f, 0.38f, 0.24f));
    mIrisIzq = glm::scale(mIrisIzq, glm::vec3(0.05f, 0.06f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mIrisIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mIrisDer = glm::translate(mCuelloPivote, glm::vec3(0.1f, 0.38f, 0.24f));
    mIrisDer = glm::scale(mIrisDer, glm::vec3(0.05f, 0.06f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mIrisDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cabello
    glBindTexture(GL_TEXTURE_2D, texturaCabello);
    glm::mat4 mCabello = glm::translate(mCuelloPivote, glm::vec3(0.0f, 0.56f, -0.02f));
    mCabello = glm::scale(mCabello, glm::vec3(0.5f, 0.15f, 0.5f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mCabello));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // BRAZOS
    //BRAZO IZQUIERDO
    glBindTexture(GL_TEXTURE_2D, texturaPlayera); // Manga corta
    glm::mat4 mHombroIzq = glm::translate(mTorsoBase, glm::vec3(-0.45f, 0.4f, 0.0f));
    mHombroIzq = glm::rotate(mHombroIzq, glm::radians(rotHombroIzqX), glm::vec3(1.0f, 0.0f, 0.0f));
    mHombroIzq = glm::rotate(mHombroIzq, glm::radians(rotHombroIzqZ), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 mMangaIzqDraw = glm::translate(mHombroIzq, glm::vec3(0.0f, -0.2f, 0.0f));
    mMangaIzqDraw = glm::scale(mMangaIzqDraw, glm::vec3(0.2f, 0.4f, 0.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMangaIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Antebrazo Izquierdo (Piel)
    glBindTexture(GL_TEXTURE_2D, texturaPiel);
    glm::mat4 mCodoIzq = glm::translate(mHombroIzq, glm::vec3(0.0f, -0.4f, 0.0f));
    mCodoIzq = glm::rotate(mCodoIzq, glm::radians(rotCodoIzq), glm::vec3(1.0f, 0.0f, 0.0f)); // Dobla hacia adelante

    glm::mat4 mAntebrazoIzqDraw = glm::translate(mCodoIzq, glm::vec3(0.0f, -0.3f, 0.0f));
    mAntebrazoIzqDraw = glm::scale(mAntebrazoIzqDraw, glm::vec3(0.18f, 0.6f, 0.18f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mAntebrazoIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Mano Izquierda
    glm::mat4 mManoIzq = glm::translate(mCodoIzq, glm::vec3(0.0f, -0.65f, 0.0f));
    mManoIzq = glm::scale(mManoIzq, glm::vec3(0.15f, 0.2f, 0.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mManoIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //BRAZO DERECHO
    glBindTexture(GL_TEXTURE_2D, texturaPlayera); // Manga corta
    glm::mat4 mHombroDer = glm::translate(mTorsoBase, glm::vec3(0.45f, 0.4f, 0.0f));
    mHombroDer = glm::rotate(mHombroDer, glm::radians(rotHombroDerX), glm::vec3(1.0f, 0.0f, 0.0f));
    mHombroDer = glm::rotate(mHombroDer, glm::radians(rotHombroDerZ), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 mMangaDerDraw = glm::translate(mHombroDer, glm::vec3(0.0f, -0.2f, 0.0f));
    mMangaDerDraw = glm::scale(mMangaDerDraw, glm::vec3(0.2f, 0.4f, 0.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMangaDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Antebrazo Derecho (Piel)
    glBindTexture(GL_TEXTURE_2D, texturaPiel);
    glm::mat4 mCodoDer = glm::translate(mHombroDer, glm::vec3(0.0f, -0.4f, 0.0f));
    mCodoDer = glm::rotate(mCodoDer, glm::radians(rotCodoDer), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mAntebrazoDerDraw = glm::translate(mCodoDer, glm::vec3(0.0f, -0.3f, 0.0f));
    mAntebrazoDerDraw = glm::scale(mAntebrazoDerDraw, glm::vec3(0.18f, 0.6f, 0.18f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mAntebrazoDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Mano Derecha
    glm::mat4 mManoDer = glm::translate(mCodoDer, glm::vec3(0.0f, -0.65f, 0.0f));
    mManoDer = glm::scale(mManoDer, glm::vec3(0.15f, 0.2f, 0.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mManoDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //PIERNAS
    //PIERNA IZQUIERDA
    glBindTexture(GL_TEXTURE_2D, texturaPantalon); // Shorts
    glm::mat4 mCaderaIzq = glm::translate(mTorsoBase, glm::vec3(-0.2f, -0.5f, 0.0f));
    mCaderaIzq = glm::rotate(mCaderaIzq, glm::radians(rotPiernaIzqX), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mMusloIzqDraw = glm::translate(mCaderaIzq, glm::vec3(0.0f, -0.3f, 0.0f));
    mMusloIzqDraw = glm::scale(mMusloIzqDraw, glm::vec3(0.28f, 0.6f, 0.3f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMusloIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pantorrilla Izquierda (Piel)
    glBindTexture(GL_TEXTURE_2D, texturaPiel);
    glm::mat4 mRodillaIzq = glm::translate(mCaderaIzq, glm::vec3(0.0f, -0.6f, 0.0f));
    mRodillaIzq = glm::rotate(mRodillaIzq, glm::radians(rotRodillaIzq), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mPantorrillaIzqDraw = glm::translate(mRodillaIzq, glm::vec3(0.0f, -0.4f, 0.0f));
    mPantorrillaIzqDraw = glm::scale(mPantorrillaIzqDraw, glm::vec3(0.2f, 0.8f, 0.22f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPantorrillaIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pie Izquierda
    glBindTexture(GL_TEXTURE_2D, texturaSandalias);
    glm::mat4 mPieIzq = glm::translate(mRodillaIzq, glm::vec3(0.0f, -0.85f, 0.08f));
    mPieIzq = glm::scale(mPieIzq, glm::vec3(0.22f, 0.1f, 0.35f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPieIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //PIERNA DERECHA
    glBindTexture(GL_TEXTURE_2D, texturaPantalon); // Shorts
    glm::mat4 mCaderaDer = glm::translate(mTorsoBase, glm::vec3(0.2f, -0.5f, 0.0f));
    mCaderaDer = glm::rotate(mCaderaDer, glm::radians(rotPiernaDerX), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mMusloDerDraw = glm::translate(mCaderaDer, glm::vec3(0.0f, -0.3f, 0.0f));
    mMusloDerDraw = glm::scale(mMusloDerDraw, glm::vec3(0.28f, 0.6f, 0.3f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMusloDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pantorrilla Derecha (Piel)
    glBindTexture(GL_TEXTURE_2D, texturaPiel);
    glm::mat4 mRodillaDer = glm::translate(mCaderaDer, glm::vec3(0.0f, -0.6f, 0.0f));
    mRodillaDer = glm::rotate(mRodillaDer, glm::radians(rotRodillaDer), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mPantorrillaDerDraw = glm::translate(mRodillaDer, glm::vec3(0.0f, -0.4f, 0.0f));
    mPantorrillaDerDraw = glm::scale(mPantorrillaDerDraw, glm::vec3(0.2f, 0.8f, 0.22f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPantorrillaDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pie Derecha
    glBindTexture(GL_TEXTURE_2D, texturaSandalias);
    glm::mat4 mPieDer = glm::translate(mRodillaDer, glm::vec3(0.0f, -0.85f, 0.08f));
    mPieDer = glm::scale(mPieDer, glm::vec3(0.22f, 0.1f, 0.35f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPieDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}