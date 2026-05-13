#include "Pikachu.h"
#include "stb_image.h"
#include <iostream>

Pikachu::Pikachu(glm::vec3 posInicial) {
    posicion = posInicial;
    rotacion = glm::vec3(0.0f, 0.0f, 0.0f);
    escala = glm::vec3(1.0f, 1.0f, 1.0f);
	

    // Inicialización de articulaciones en posición de reposo (0 grados)
    rotCabezaX = 0.0f; rotCabezaY = 0.0f; rotCabezaZ = 0.0f;
    rotHombroIzqX = 0.0f; rotHombroIzqZ = 60.0f; rotHombroIzqY = 0.0f; // Ligeramente despegados del cuerpo
    rotCodoIzq = 0.0f;
    rotHombroDerX = 0.0f; rotHombroDerZ = -60.0f; rotHombroDerY = 0.0f;
    rotCodoDer = 0.0f;
    rotPiernaIzqX = 0.0f; rotRodillaIzq = 0.0f;
    rotPiernaDerX = 0.0f; rotRodillaDer = 0.0f;
}

void Pikachu::Inicializar() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Color Cuerpo
    glGenTextures(1, &texturaCuerpo);
    glBindTexture(GL_TEXTURE_2D, texturaCuerpo);
    unsigned char colorCuerpo[] = { 217, 179, 83 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorCuerpo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Color Orejas
    glGenTextures(1, &texturaOrejas);
    glBindTexture(GL_TEXTURE_2D, texturaOrejas);
    unsigned char colorOrejas[] = { 98, 52, 40 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorOrejas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Color Pecas
    glGenTextures(1, &texturaPecas);
    glBindTexture(GL_TEXTURE_2D, texturaPecas);
    unsigned char colorPecas[] = { 212, 60, 60 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorPecas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Color Boca
    glGenTextures(1, &texturaBoca);
    glBindTexture(GL_TEXTURE_2D, texturaBoca);
    // El cuarto valor (100) es el canal Alfa que dicta el nivel de transparencia (0 a 255)
    unsigned char colorBoca[] = { 20, 20, 20, 100 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBoca);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Color Ojos
    glGenTextures(1, &texturaOjos);
    glBindTexture(GL_TEXTURE_2D, texturaOjos);
    unsigned char colorOjos[] = { 0, 0, 0 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorOjos);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
    //Color Iris
    glGenTextures(1, &texturaIris);
    glBindTexture(GL_TEXTURE_2D, texturaIris);
    unsigned char colorIris[] = { 255, 255, 255 }; 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorIris);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
	//Color Guantes
    glGenTextures(1, &texturaGuantes);
    glBindTexture(GL_TEXTURE_2D, texturaGuantes);
    unsigned char colorNegro[] = { 0, 0, 0 }; 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, colorNegro);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Pikachu::Draw(Shader& shader, GLuint VAO) {
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint matDiffLoc = glGetUniformLocation(shader.Program, "material.diffuse");
    GLuint shininessLoc = glGetUniformLocation(shader.Program, "material.shininess");
    GLint matSpecLoc = glGetUniformLocation(shader.Program, "material.specular");

    // Le asignamos un brillo alto (32.0f) para que parezca plástico o nylon
    if (shininessLoc != -1) glUniform1f(shininessLoc, 32.0f);
    glBindVertexArray(VAO);
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(matDiffLoc, 0);


    glm::mat4 mRaiz = glm::translate(glm::mat4(1.0f), posicion);
    mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mRaiz = glm::rotate(mRaiz, glm::radians(rotacion.z), glm::vec3(0.0f, 0.0f, 1.0f));
    mRaiz = glm::scale(mRaiz, escala);

    //TORSO 
    glBindTexture(GL_TEXTURE_2D, texturaCuerpo); // Textura general
    glm::mat4 mTorsoBase = glm::translate(mRaiz, glm::vec3(0.0f, 1.5f, 0.0f));

    glm::mat4 mTorsoDraw = glm::scale(mTorsoBase, glm::vec3(1.3f, 1.5f, 1.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mTorsoDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //CABEZA Y DETALLES
    glm::mat4 mCuelloPivote = glm::translate(mTorsoBase, glm::vec3(0.0f, 0.75f, 0.0f));
    mCuelloPivote = glm::rotate(mCuelloPivote, glm::radians(rotCabezaY), glm::vec3(0.0f, 1.0f, 0.0f));
    mCuelloPivote = glm::rotate(mCuelloPivote, glm::radians(rotCabezaX), glm::vec3(1.0f, 0.0f, 0.0f));
    mCuelloPivote = glm::rotate(mCuelloPivote, glm::radians(rotCabezaZ), glm::vec3(0.0f, 0.0f, 1.0f));

    // Cabeza
    glm::mat4 mCabezaDraw = glm::translate(mCuelloPivote, glm::vec3(0.0f, 0.5f, 0.0f));
    mCabezaDraw = glm::scale(mCabezaDraw, glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mCabezaDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Ojos (Hijos de la cabeza, ubicados en la cara frontal Z=0.51 para evitar solapamiento)
    glBindTexture(GL_TEXTURE_2D, texturaOjos);
    glm::mat4 mOjoIzq = glm::translate(mCuelloPivote, glm::vec3(-0.2f, 0.6f, 0.51f));
    mOjoIzq = glm::scale(mOjoIzq, glm::vec3(0.15f, 0.15f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOjoIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mOjoDer = glm::translate(mCuelloPivote, glm::vec3(0.2f, 0.6f, 0.51f));
    mOjoDer = glm::scale(mOjoDer, glm::vec3(0.15f, 0.15f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOjoDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Iris (Blancos, ligeramente desplazados al frente)
    glBindTexture(GL_TEXTURE_2D, texturaIris);
    glm::mat4 mIrisIzq = glm::translate(mCuelloPivote, glm::vec3(-0.2f, 0.62f, 0.54f));
    mIrisIzq = glm::scale(mIrisIzq, glm::vec3(0.06f, 0.06f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mIrisIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mIrisDer = glm::translate(mCuelloPivote, glm::vec3(0.2f, 0.62f, 0.54f));
    mIrisDer = glm::scale(mIrisDer, glm::vec3(0.06f, 0.06f, 0.02f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mIrisDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pecas (Hijas de la cabeza, debajo de los ojos)
    glBindTexture(GL_TEXTURE_2D, texturaPecas);
    glm::mat4 mPecaIzq = glm::translate(mCuelloPivote, glm::vec3(-0.35f, 0.4f, 0.51f));
    mPecaIzq = glm::scale(mPecaIzq, glm::vec3(0.15f, 0.15f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPecaIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mPecaDer = glm::translate(mCuelloPivote, glm::vec3(0.35f, 0.4f, 0.51f));
    mPecaDer = glm::scale(mPecaDer, glm::vec3(0.15f, 0.15f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPecaDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

	//Boca (vision de persona dentro de botarga)

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texturaBoca);
    // La posicionamos centrada (X=0.0f) y debajo de los ojos/pecas (Y=0.25f)
    glm::mat4 mBoca = glm::translate(mCuelloPivote, glm::vec3(0.0f, 0.25f, 0.51f));
    mBoca = glm::scale(mBoca, glm::vec3(0.3f, 0.15f, 0.05f)); // Rectángulo ancho
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBoca));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Apagamos la transparencia para que el resto del cuerpo y la escena se dibujen opacos
    glDisable(GL_BLEND);

    // Orejas
    glBindTexture(GL_TEXTURE_2D, texturaOrejas);
    glm::mat4 mOrejaIzq = glm::translate(mCuelloPivote, glm::vec3(-0.4f, 1.2f, 0.0f));
    mOrejaIzq = glm::rotate(mOrejaIzq, glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mOrejaIzq = glm::scale(mOrejaIzq, glm::vec3(0.2f, 0.7f, 0.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOrejaIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mOrejaDer = glm::translate(mCuelloPivote, glm::vec3(0.4f, 1.2f, 0.0f));
    mOrejaDer = glm::rotate(mOrejaDer, glm::radians(-25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mOrejaDer = glm::scale(mOrejaDer, glm::vec3(0.2f, 0.7f, 0.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mOrejaDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //RESTO DEL CUERPO
    // Regresamos al color principal para las extremidades
    glBindTexture(GL_TEXTURE_2D, texturaCuerpo);

    // BRAZO IZQUIERDO
    glm::mat4 mHombroIzq = glm::translate(mTorsoBase, glm::vec3(-0.65f, 0.5f, 0.0f));
    mHombroIzq = glm::rotate(mHombroIzq, glm::radians(rotHombroIzqX), glm::vec3(1.0f, 0.0f, 0.0f));
    mHombroIzq = glm::rotate(mHombroIzq, glm::radians(rotHombroIzqZ), glm::vec3(0.0f, 0.0f, 1.0f));
	mHombroIzq = glm::rotate(mHombroIzq, glm::radians(rotHombroIzqY), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 mBrazoIzqDraw = glm::translate(mHombroIzq, glm::vec3(-0.25f, 0.0f, 0.0f));
    mBrazoIzqDraw = glm::scale(mBrazoIzqDraw, glm::vec3(0.5f, 0.35f, 0.35f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBrazoIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mCodoIzq = glm::translate(mHombroIzq, glm::vec3(-0.5f, 0.0f, 0.0f));
    mCodoIzq = glm::rotate(mCodoIzq, glm::radians(rotCodoIzq), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 mAntebrazoIzqDraw = glm::translate(mCodoIzq, glm::vec3(-0.2f, 0.0f, 0.0f));
    mAntebrazoIzqDraw = glm::scale(mAntebrazoIzqDraw, glm::vec3(0.4f, 0.3f, 0.3f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mAntebrazoIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);
	
    //MANO IZQUIERDA
    glBindTexture(GL_TEXTURE_2D, texturaGuantes);
    glm::mat4 mManoIzq = glm::translate(mCodoIzq, glm::vec3(-0.45f, 0.0f, 0.0f)); // Al final del antebrazo
    mManoIzq = glm::scale(mManoIzq, glm::vec3(0.15f, 0.25f, 0.25f)); // Cubo pequeño
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mManoIzq));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // BRAZO DERECHO 
    glBindTexture(GL_TEXTURE_2D, texturaCuerpo);
    glm::mat4 mHombroDer = glm::translate(mTorsoBase, glm::vec3(0.65f, 0.5f, 0.0f));
    mHombroDer = glm::rotate(mHombroDer, glm::radians(rotHombroDerX), glm::vec3(1.0f, 0.0f, 0.0f));
    mHombroDer = glm::rotate(mHombroDer, glm::radians(rotHombroDerZ), glm::vec3(0.0f, 0.0f, 1.0f));
	mHombroDer = glm::rotate(mHombroDer, glm::radians(rotHombroDerY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mBrazoDerDraw = glm::translate(mHombroDer, glm::vec3(0.25f, 0.0f, 0.0f));
    mBrazoDerDraw = glm::scale(mBrazoDerDraw, glm::vec3(0.5f, 0.35f, 0.35f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBrazoDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mCodoDer = glm::translate(mHombroDer, glm::vec3(0.5f, 0.0f, 0.0f));
    mCodoDer = glm::rotate(mCodoDer, glm::radians(rotCodoDer), glm::vec3(0.0f, -1.0f, 0.0f));

    glm::mat4 mAntebrazoDerDraw = glm::translate(mCodoDer, glm::vec3(0.2f, 0.0f, 0.0f));
    mAntebrazoDerDraw = glm::scale(mAntebrazoDerDraw, glm::vec3(0.4f, 0.3f, 0.3f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mAntebrazoDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);
   
    //MANO DERECHA
    glBindTexture(GL_TEXTURE_2D, texturaGuantes);
    glm::mat4 mManoDer = glm::translate(mCodoDer, glm::vec3(0.45f, 0.0f, 0.0f)); // Espejo en X
    mManoDer = glm::scale(mManoDer, glm::vec3(0.15f, 0.25f, 0.25f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mManoDer));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // PIERNA IZQUIERDA
    glBindTexture(GL_TEXTURE_2D, texturaCuerpo);
    glm::mat4 mCaderaIzq = glm::translate(mTorsoBase, glm::vec3(-0.35f, -0.75f, 0.0f));
    mCaderaIzq = glm::rotate(mCaderaIzq, glm::radians(rotPiernaIzqX), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mMusloIzqDraw = glm::translate(mCaderaIzq, glm::vec3(0.0f, -0.2f, 0.0f));
    mMusloIzqDraw = glm::scale(mMusloIzqDraw, glm::vec3(0.45f, 0.4f, 0.45f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMusloIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mRodillaIzq = glm::translate(mCaderaIzq, glm::vec3(0.0f, -0.4f, 0.0f));
    mRodillaIzq = glm::rotate(mRodillaIzq, glm::radians(rotRodillaIzq), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mPantorrillaIzqDraw = glm::translate(mRodillaIzq, glm::vec3(0.0f, -0.2f, 0.1f));
    mPantorrillaIzqDraw = glm::scale(mPantorrillaIzqDraw, glm::vec3(0.4f, 0.4f, 0.5f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPantorrillaIzqDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // PIERNA DERECHA
    glm::mat4 mCaderaDer = glm::translate(mTorsoBase, glm::vec3(0.35f, -0.75f, 0.0f));
    mCaderaDer = glm::rotate(mCaderaDer, glm::radians(rotPiernaDerX), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mMusloDerDraw = glm::translate(mCaderaDer, glm::vec3(0.0f, -0.2f, 0.0f));
    mMusloDerDraw = glm::scale(mMusloDerDraw, glm::vec3(0.45f, 0.4f, 0.45f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMusloDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 mRodillaDer = glm::translate(mCaderaDer, glm::vec3(0.0f, -0.4f, 0.0f));
    mRodillaDer = glm::rotate(mRodillaDer, glm::radians(rotRodillaDer), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mPantorrillaDerDraw = glm::translate(mRodillaDer, glm::vec3(0.0f, -0.2f, 0.1f));
    mPantorrillaDerDraw = glm::scale(mPantorrillaDerDraw, glm::vec3(0.4f, 0.4f, 0.5f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPantorrillaDerDraw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // COLA
    glBindTexture(GL_TEXTURE_2D, texturaCuerpo);
    glm::mat4 mColaBase = glm::translate(mTorsoBase, glm::vec3(0.0f, -0.5f, -0.55f));

    // Segmento 1
    glm::mat4 mColaS1 = glm::rotate(mColaBase, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mColaS1 = glm::translate(mColaS1, glm::vec3(0.0f, 0.3f, 0.0f));
    mColaS1 = glm::scale(mColaS1, glm::vec3(0.2f, 0.6f, 0.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mColaS1));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Segmento 2 (Punta)
    glBindTexture(GL_TEXTURE_2D, texturaOrejas);
    glm::mat4 mColaS2 = glm::translate(mColaBase, glm::vec3(0.0f, 0.70f, -0.4f));
    mColaS2 = glm::scale(mColaS2, glm::vec3(0.4f, 0.6f, 0.1f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mColaS2));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}