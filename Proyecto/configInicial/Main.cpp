#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//objetos creados
#include "Carro.h"
#include "Tren.h"
#include "Silla.h"
#include "Mesa.h"
#include "pikachu.h"
#include "stand.h"
#include "persona.h"
#include "Lampara.h"
#include "Reja.h"
// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint LoadTextureRGBA(const char* path)
{
	GLuint id; glGenTextures(1, &id);
	int w, h, ch;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &w, &h, &ch, 4);
	if (data) {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		unsigned char pink[4] = { 255,0,128,255 };
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pink);
	}
	stbi_image_free(data);
	return id;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void DrawConfetti(GLuint VAO, Shader& s, GLint mL, GLint cL, GLint uTL, float timer);


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(15.0f, 1.0f, 15.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	//fila 1 horizontal
	glm::vec3(7.0f,  2.7f, -5.7f),  // lampara1
	glm::vec3(7.0f,  2.7f, -10.7f), // lampara2
	glm::vec3(7.0f, 2.7f, -18.4f),  // lampara3
	//fila2 horizontal
	glm::vec3(12.0f, 2.7f, -5.7f), // lampara4
	glm::vec3(12.0f, 2.7f, -10.7f),  // lampara5
	glm::vec3(12.0f, 2.7f, -18.4f),  // lampara6
	//fila3 muestra horizontal
	glm::vec3(17.0f, 2.7f, -5.7f),// lampara 7
	glm::vec3(17.0f, 2.7f, -14.7f)//lampara 9
};



float vertices[] = {
	// Posiciones          // Normales           // UVs
	// Frente
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
	// Atrás
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
	// Izquierda
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
	// Derecha
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
	 // Abajo
	 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
	 // Arriba
	 -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
};



glm::vec3 Light1 = glm::vec3(0);

//declaracion de objetos
Carro carro(glm::vec3(37.0f, 0.050f, 11.0f));
bool animCarro = false;
int estadoCarro = 0; // 0: Recto, 1: Curva, 2: Posicionamiento final, 3: Detenido
float velocidadCarro = 4.0f;
float rLlantaVel = 300.0f; // Velocidad de rotación de las llantas
bool alineadoX = false;
bool alineadoZ = false;

Tren TrenRojo(glm::vec3(-1.0f, 0.0f, -26.0f));
bool AnimTren = false;

Silla SillaPlegable(glm::vec3(6.8f, 0.050f, -0.53));
bool AnimSilla = false;      // Estado de animación de la silla
bool SillaAbierta = false;   // Estado actual de la silla (abierta/cerrada)
bool AnimacionEnProgreso = false;

Mesa MesaPlegable(glm::vec3(6.8f, 0.74f, -1.8f));
bool AnimMesa = false;           // Estado de animación de la mesa
bool MesaAbierta = false;        // Estado actual de la mesa (abierta/cerrada)
bool AnimacionMesaEnProgreso = false;  // Para evitar múltiples activaciones

Stand stand1(glm::vec3(6.8f, 0.03f, -1.5f));

Pikachu botarga(glm::vec3(19.0f, 0.05f, 2.7f));
bool animPikachuDance = false;

// Estructura para guardar las articulaciones de cada pose
typedef struct _frame {
	float posY;   // Rebote vertical del cuerpo
	float pDerX;  // Pierna Derecha
	float pIzqX;  // Pierna Izquierda
	float cZ;    // Movimiento rítmico de cabeza
	// Brazo Izquierdo
	float hIzqX;  // Giro del hombro izquierdo sobre sí mismo
	float hIzqY;  // Mueve el brazo izquierdo hacia el frente
	float hIzqZ;  // Levanta o baja el brazo izquierdo
	float cIzq;   // Dobla el codo izquierdo hacia la cara
	// Brazo Derecho
	float hDerX;  // Giro del hombro derecho sobre sí mismo
	float hDerY;  // Mueve el brazo derecho hacia el frente
	float hDerZ;  // Levanta o baja el brazo derecho
	float cDer;   // Dobla el codo derecho
} FRAME;

#define MAX_FRAMES 4
FRAME KeyFrame[MAX_FRAMES];

int playIndex = 0;       // Indica en qué frame vamos
int i_max_steps = 60;    // Velocidad de interpolación (menor = más rápido el baile)
int i_curr_steps = 0;    // Contador de pasos actual

// Variables de incremento para suavizar la animación
float inc_posY, inc_pDerX, inc_pIzqX, inc_cZ;
float inc_hIzqX, inc_hIzqY, inc_hIzqZ, inc_cIzq;
float inc_hDerX, inc_hDerY, inc_hDerZ, inc_cDer;

Pikachu botarga2(glm::vec3(-17.0f, 0.05f, 2.7f));
bool animScubaCat = false;

typedef struct _frameScuba {
	float posX;   // Desplazamiento lateral del cuerpo
	float posY;   // Rebote vertical del cuerpo
	float pDerX;  // Pierna Derecha
	float pIzqX;  // Pierna Izquierda
	float cZ;    // Inclinación lateral de la cabeza
	// Brazo Izquierdo (Mano en la boca)
	float hIzqX;  // Giro del hombro izquierdo sobre sí mismo
	float hIzqY;  // Mueve el brazo izquierdo hacia el frente (pecho/boca)
	float hIzqZ;  // Levanta o baja el brazo izquierdo
	float cIzq;   // Dobla el codo izquierdo hacia la cara
	// Brazo Derecho (Recto)
	float hDerX;  // Giro del hombro derecho sobre sí mismo
	float hDerY;  // Mueve el brazo derecho hacia el frente
	float hDerZ;  // Levanta o baja el brazo derecho
	float cDer;   // Dobla el codo derecho
} FRAME_SCUBA;

#define MAX_FRAMES_SCUBA 4
FRAME_SCUBA KeyFrameScuba[MAX_FRAMES_SCUBA];

int playIndexScuba = 0;
int i_max_stepsScuba = 50;
int i_curr_stepsScuba = 0;

// Incrementos para la interpolación de botarga2
float incS_posX, incS_posY, incS_pDerX, incS_pIzqX;
float incS_hIzqX, incS_hIzqY, incS_hIzqZ, incS_cIzq;
float incS_hDerX, incS_hDerY, incS_hDerZ, incS_cDer;
float incS_cZ;

Persona persona1(glm::vec3(6.8f, 0.13f, -1.1f));

Persona personaCaminando(glm::vec3(12.f, 0.13f, 1.0f));
bool animPersonaCaminando = false;
int estadoCaminando = 0; // 0: Reposo, 1: Pierna Der. Adelante, 2: Pierna Izq. Adelante
float velocidadCaminata = 80.0f; // Velocidad de rotación de las extremidades
float limitePaso = 35.0f; // Ángulo máximo que pueden alcanzar las piernas

Persona persona2(glm::vec3(28.0f, 0.13f, -18.1f));
bool animPersona2Caminando = false;
int estadoCaminando2 = 0; // 0: Reposo, 1: Pierna Der. Adelante, 2: Pierna Izq. Adelante
float velocidadCaminata2 = 80.0f; // Velocidad de rotación de las extremidades
float limitePaso2 = 35.0f; // Ángulo máximo que pueden alcanzar las piernas
int fasePersona2 = 0;

Lampara lampara1(glm::vec3(17.0f, 3.85f, -5.67f));

Silla silla2(glm::vec3(19.0f, 0.050f, -2.0f));
Mesa mesa2(glm::vec3(19.0f, 0.74f, -1.4f));
Stand stand2(glm::vec3(19.0f, 0.03f, -1.5f));

Silla silla3(glm::vec3(11.5f, 0.050f, -5.0f));
Mesa mesa3(glm::vec3(11.5f, 0.74f, -4.0f));
Stand stand3(glm::vec3(11.5f, 0.03f, -4.2f));

Reja rejaPuertaIzq(glm::vec3(6.5f, 1.0f, -0.02f));
Reja rejaPuertaDer(glm::vec3(22.5f, 1.0f, -0.02f));
bool AnimRejas = false;           // Estado de animación de la reja
bool RejasAbiertas = true;       // Estado actual (abierta/cerrada)
bool AnimacionRejasEnProgreso = false; // Para evitar el rebote del teclado
float velocidadReja = 2.0f;       // Qué tan rápido se deslizan
bool p1_llego = false;
bool p2_llego = false;

//  CONFETI — extraido de ProyectoFinal (Lobby JBS)
struct ConfettiParticle { float x, z, speed, rotSpeed, colorR, colorG, colorB, phase; };
const int NUM_CONFETTI = 200;
ConfettiParticle confParticles[NUM_CONFETTI];
bool confettiActive = false;
float confettiTimer = 0.0f;

void InitConfetti()
{
	glm::vec3 pal[3] = { {0.0f,0.27f,0.55f},{0.85f,0.65f,0.13f},{1.0f,1.0f,1.0f} };
	for (int i = 0; i < NUM_CONFETTI; i++) {
		confParticles[i].x = ((rand() % 2800) / 100.0f) - 14.0f;
		confParticles[i].z = ((rand() % 1900) / 100.0f) - 9.5f;
		confParticles[i].speed = 0.8f + (rand() % 50) / 100.0f;
		confParticles[i].rotSpeed = 30.0f + (rand() % 120);
		confParticles[i].phase = (rand() % 300) / 100.0f;
		int c = rand() % 3;
		confParticles[i].colorR = pal[c].r;
		confParticles[i].colorG = pal[c].g;
		confParticles[i].colorB = pal[c].b;
	}
}

// ── NUEVO: ANIMACION 5: Señalética pulsante ───────────────────────────────────
GLuint texSenal;
GLuint senalVAO, senalVBO;
float quadVertsSenal[] = {
	-0.5f,-0.5f,0,  0,0,1,  0,0,
	 0.5f,-0.5f,0,  0,0,1,  1,0,
	 0.5f, 0.5f,0,  0,0,1,  1,1,
	 0.5f, 0.5f,0,  0,0,1,  1,1,
	-0.5f, 0.5f,0,  0,0,1,  0,1,
	-0.5f,-0.5f,0,  0,0,1,  0,0,
};
glm::vec3 posicionesSenal[] = {
	glm::vec3(7.0f,  2.2f, -5.5f),
	glm::vec3(12.0f, 2.2f, -5.5f),
	glm::vec3(17.0f, 2.2f, -5.5f),
	glm::vec3(7.0f,  2.2f, -18.0f),
};

// ── NUEVO: ANIMACION 6: Tooltip de stand ─────────────────────────────────────
GLuint texTooltip;
float tooltipScale = 0.0f;
float tooltipTarget = 0.0f;
int   tooltipStandIdx = -1;
glm::vec3 posicionesStandTooltip[] = {
	glm::vec3(6.8f,  1.0f, -1.5f),
	glm::vec3(19.0f, 1.0f, -1.5f),
	glm::vec3(11.5f, 1.0f, -4.2f),
};
const float TOOLTIP_RADIO = 3.2f;

// ── NUEVO: ANIMACION 7: Transicion de camara ─────────────────────────────────
bool  animCamTransicion = false;
bool  camEnInterior = false;
float tCam = 0.0f;
float velCam = 0.28f;
bool  camTransActiva = false;
glm::vec3 camTransPos;
glm::vec3 bezP0 = glm::vec3(15.0f, 1.5f, 12.0f);
glm::vec3 bezP1 = glm::vec3(14.0f, 1.5f, 2.0f);
glm::vec3 bezP2 = glm::vec3(13.5f, 1.5f, -6.0f);
glm::vec3 bezP3 = glm::vec3(13.0f, 1.5f, -13.5f);

// ── NUEVO: ANIMACION 8: Bandera UNAM ondeando ────────────────────────────────
GLuint texBandera;
const int FLAG_COLS = 16;
const int FLAG_ROWS = 10;
float     flagVerts[FLAG_ROWS * FLAG_COLS * 8];
unsigned int flagIdx[(FLAG_ROWS - 1) * (FLAG_COLS - 1) * 6];
GLuint flagVAO, flagVBO, flagEBO;

// ── NUEVO: ANIMACION 10: Ciclo solar controlado con teclas J/K ───────────────
float tSolar = 0.25f;  // 0=amanecer, 0.5=mediodia, 1=atardecer

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// ── NUEVO: Funciones auxiliares para las animaciones nuevas ──────────────────
glm::vec3 EvalBezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t)
{
	float u = 1.0f - t;
	return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
}

glm::vec3 GetSunDirection(float t)
{
	float angulo = t * glm::pi<float>();
	return glm::normalize(glm::vec3(-cosf(angulo), -sinf(angulo) - 0.1f, -0.3f));
}

glm::vec3 GetSunColor(float t)
{
	if (t < 0.15f || t > 0.85f) return glm::vec3(1.0f, 0.55f, 0.2f);
	if (t < 0.25f || t > 0.75f) return glm::vec3(1.0f, 0.80f, 0.5f);
	return glm::vec3(1.0f, 0.95f, 0.75f);
}

void BuildSenalVAO()
{
	glGenVertexArrays(1, &senalVAO);
	glGenBuffers(1, &senalVBO);
	glBindVertexArray(senalVAO);
	glBindBuffer(GL_ARRAY_BUFFER, senalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertsSenal), quadVertsSenal, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void BuildFlagMesh()
{
	for (int r = 0; r < FLAG_ROWS; r++) {
		for (int c = 0; c < FLAG_COLS; c++) {
			int idx = (r * FLAG_COLS + c) * 8;
			float u = (float)c / (FLAG_COLS - 1);
			float v = (float)r / (FLAG_ROWS - 1);
			flagVerts[idx + 0] = u * 2.0f - 1.0f;
			flagVerts[idx + 1] = v * 1.5f;
			flagVerts[idx + 2] = 0.0f;
			flagVerts[idx + 3] = 0.0f; flagVerts[idx + 4] = 0.0f; flagVerts[idx + 5] = 1.0f;
			flagVerts[idx + 6] = u;    flagVerts[idx + 7] = 1.0f - v;
		}
	}
	int ei = 0;
	for (int r = 0; r < FLAG_ROWS - 1; r++) {
		for (int c = 0; c < FLAG_COLS - 1; c++) {
			int tl = r * FLAG_COLS + c, tr = tl + 1, bl = tl + FLAG_COLS, br = bl + 1;
			flagIdx[ei++] = tl; flagIdx[ei++] = bl; flagIdx[ei++] = tr;
			flagIdx[ei++] = tr; flagIdx[ei++] = bl; flagIdx[ei++] = br;
		}
	}
	glGenVertexArrays(1, &flagVAO); glGenBuffers(1, &flagVBO); glGenBuffers(1, &flagEBO);
	glBindVertexArray(flagVAO);
	glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flagVerts), flagVerts, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, flagEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(flagIdx), flagIdx, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void UpdateFlagWave(float time)
{
	for (int r = 0; r < FLAG_ROWS; r++) {
		for (int c = 0; c < FLAG_COLS; c++) {
			int idx = (r * FLAG_COLS + c) * 8;
			float u = (float)c / (FLAG_COLS - 1);
			float amplitud = u * 0.18f;
			float dz = sinf(u * 3.5f - time * 2.8f) * amplitud + sinf(u * 6.0f - time * 4.2f + 1.1f) * amplitud * 0.35f;
			flagVerts[idx + 2] = dz;
			float dzdu = cosf(u * 3.5f - time * 2.8f) * 0.18f * 3.5f + cosf(u * 6.0f - time * 4.2f + 1.1f) * 0.18f * 0.35f * 6.0f;
			glm::vec3 n = glm::normalize(glm::vec3(-dzdu, 0.0f, 1.0f));
			flagVerts[idx + 3] = n.x; flagVerts[idx + 4] = n.y; flagVerts[idx + 5] = n.z;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(flagVerts), flagVerts);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawSignage(Shader& shader, GLint modelLoc, float time, glm::mat4 view)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texSenal);
	glBindVertexArray(senalVAO);
	float pulso = 0.925f + 0.075f * sinf(time * 2.5f);
	for (int i = 0; i < 4; i++) {
		glm::mat4 m(1.0f);
		m[0][0] = view[0][0]; m[0][1] = view[1][0]; m[0][2] = view[2][0];
		m[1][0] = view[0][1]; m[1][1] = view[1][1]; m[1][2] = view[2][1];
		m[2][0] = view[0][2]; m[2][1] = view[1][2]; m[2][2] = view[2][2];
		m[3] = glm::vec4(posicionesSenal[i], 1.0f);
		m = glm::scale(m, glm::vec3(pulso * 0.45f, pulso * 0.45f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(0);
}

void UpdateTooltip(glm::vec3 camPos, float dt)
{
	tooltipTarget = 0.0f; tooltipStandIdx = -1;
	for (int i = 0; i < 3; i++) {
		if (glm::length(camPos - posicionesStandTooltip[i]) < TOOLTIP_RADIO) {
			tooltipTarget = 1.0f; tooltipStandIdx = i; break;
		}
	}
	float vel = dt / 0.3f;
	if (tooltipTarget > tooltipScale) tooltipScale = fminf(tooltipScale + vel, 1.0f);
	else                              tooltipScale = fmaxf(tooltipScale - vel, 0.0f);
}

void DrawTooltip(Shader& shader, GLint modelLoc, glm::mat4 view)
{
	if (tooltipScale < 0.01f || tooltipStandIdx < 0) return;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texTooltip);
	glBindVertexArray(senalVAO);
	glm::vec3 pos = posicionesStandTooltip[tooltipStandIdx] + glm::vec3(0, 0.8f, 0);
	glm::mat4 m(1.0f);
	m[0][0] = view[0][0]; m[0][1] = view[1][0]; m[0][2] = view[2][0];
	m[1][0] = view[0][1]; m[1][1] = view[1][1]; m[1][2] = view[2][1];
	m[2][0] = view[0][2]; m[2][1] = view[1][2]; m[2][2] = view[2][2];
	m[3] = glm::vec4(pos, 1.0f);
	float s = tooltipScale * 1.4f;
	m = glm::scale(m, glm::vec3(s, s * 0.55f, 0.01f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

int main()
{
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Sotano edificio A", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	srand(42);
	InitConfetti();

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	//inicializamos objetos
	carro.Inicializar();
	carro.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
	carro.escala = glm::vec3(1.1f, 1.1f, 1.1f);

	TrenRojo.Inicializar();
	TrenRojo.escala = glm::vec3(3.0f, 2.2f, 2.5f);

	SillaPlegable.Inicializar();
	SillaPlegable.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);

	MesaPlegable.Inicializar();

	stand1.Inicializar();
	stand1.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
	stand1.escala = glm::vec3(1.0f, 0.7f, 1.0f);

	persona1.Inicializar();
	persona1.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
	persona1.escala = glm::vec3(0.42f, 0.42f, 0.42f);

	personaCaminando.Inicializar();
	personaCaminando.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
	personaCaminando.escala = glm::vec3(0.42f, 0.42f, 0.42f);

	persona2.Inicializar();
	persona2.rotacion = glm::vec3(0.0f, -180.0f, 0.0f);
	persona2.escala = glm::vec3(0.42f, 0.42f, 0.42f);

	silla2.Inicializar();
	silla2.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
	silla2.anguloPlegado = 165.0f;
	mesa2.Inicializar();
	mesa2.anguloApertura = 180.0f;
	stand2.Inicializar();

	silla3.Inicializar();
	silla3.rotacion = glm::vec3(0.0f, 180.0f, 0.0f);
	silla3.anguloPlegado = 165.5f;
	mesa3.Inicializar();
	mesa3.anguloApertura = 180.0f;
	stand3.Inicializar();

	lampara1.Inicializar();
	rejaPuertaIzq.Inicializar();
	rejaPuertaDer.Inicializar();

	botarga.Inicializar();
	botarga.escala = glm::vec3(0.47f, 0.47f, 0.47f);
	// GUARDADO DE KEYFRAMES rat dance
	//Frame 0:
	KeyFrame[0].posY = -0.2f;  // Rebote arriba
	KeyFrame[0].pDerX = 0.0f; KeyFrame[0].pIzqX = 0.0f; // Cruce de pies
	KeyFrame[0].cZ = 0.0f; // Cabeza acompaña el ritmo
	//Brazo izquierdo
	KeyFrame[0].hIzqX = 15.0f; KeyFrame[0].hIzqY = -50.0f; KeyFrame[0].hIzqZ = 40.0f; KeyFrame[0].cIzq = -90.0f;
	KeyFrame[0].hDerX = -30.0f; KeyFrame[0].hDerY = 30.0f; KeyFrame[0].hDerZ = 10.0f; KeyFrame[0].cDer = 0.0f;

	// Frame 1
	KeyFrame[1].posY = 0.0f;  // Aterrizaje
	KeyFrame[1].pDerX = 20.0f; KeyFrame[1].pIzqX = -10.0f; KeyFrame[1].cZ = 10.0f;
	// Brazos a la mitad del recorrido para dar fluidez
	KeyFrame[1].hIzqX = 1.0f; KeyFrame[1].hIzqY = 30.0f; KeyFrame[1].hIzqZ = 10.0f; KeyFrame[1].cIzq = 0.0f;
	KeyFrame[1].hDerX = 10.0f; KeyFrame[1].hDerY = 50.0f; KeyFrame[1].hDerZ = 40.0f; KeyFrame[1].cDer = 90.0f; //Brazo derecho

	// Frame 2: Cruza Pie Izquierdo, Sube Brazo Derecho (Opuesto)
	KeyFrame[2].posY = 0.2f;
	KeyFrame[2].pDerX = -10.0f; KeyFrame[2].pIzqX = 20.0f; KeyFrame[2].cZ = 0.0f;
	// Brazo Izq (Abajo/Atrás)
	KeyFrame[2].hIzqX = 15.0f; KeyFrame[2].hIzqY = -50.0f; KeyFrame[2].hIzqZ = 40.0f; KeyFrame[2].cIzq = -90.0f; //Brazo izquierdo
	KeyFrame[2].hDerX = -30.0f; KeyFrame[2].hDerY = -30.0f; KeyFrame[2].hDerZ = 10.0f; KeyFrame[2].cDer = 0.0f;

	// Frame 3: Descruce (Regreso a postura neutral)
	KeyFrame[3].posY = 0.0f;
	KeyFrame[3].pDerX = 10.0f; KeyFrame[3].pIzqX = -20.0f; KeyFrame[3].cZ = -10.0f;
	KeyFrame[3].hIzqX = 1.0f; KeyFrame[3].hIzqY = 30.0f; KeyFrame[3].hIzqZ = 10.0f; KeyFrame[3].cIzq = 0.0f;
	KeyFrame[3].hDerX = 10.0f; KeyFrame[3].hDerY = 50.0f; KeyFrame[3].hDerZ = 40.0f; KeyFrame[3].cDer = 90.0f; //Brazo derecho


	botarga2.Inicializar();
	botarga2.escala = glm::vec3(0.47f, 0.47f, 0.47f);
	//GUARDADO DE KEYFRAMES DEL SCUBA CAT
	//Frame 0: Paso a la Izquierda, Pata Izquierda arriba, Derecha baja, ligero agache
	// Frame 0
	KeyFrameScuba[0].posX = -0.3f; KeyFrameScuba[0].posY = -0.1f; KeyFrameScuba[0].cZ = 0.0f;
	// Brazo Izquierdo (Mano en la boca): Lo llevamos al frente (Y) y lo cerramos (Z)
	KeyFrameScuba[0].hIzqX = 0.0f; KeyFrameScuba[0].hIzqZ = 0.0f; KeyFrameScuba[0].hIzqY = 0.0f; KeyFrameScuba[0].cIzq = 0.0f;
	// Brazo Derecho (Recto): Lo mantenemos hacia abajo pero lo levantamos un poco para dar sensación de movimiento
	KeyFrameScuba[0].hDerX = 0.0f;  KeyFrameScuba[0].hDerZ = 15.0f;  KeyFrameScuba[0].cDer = -15.0f;  // Brazo recto
	KeyFrameScuba[0].pDerX = 15.0f;  KeyFrameScuba[0].pIzqX = -10.0f;

	// Frame 1
	KeyFrameScuba[1].posX = 0.0f;  KeyFrameScuba[1].posY = 0.0f; KeyFrameScuba[1].cZ = -20.0f;
	KeyFrameScuba[1].hIzqX = -135.0f; KeyFrameScuba[1].hIzqZ = 70.0f; KeyFrameScuba[1].hIzqY = -35.0f; KeyFrameScuba[1].cIzq = 55.0f;
	KeyFrameScuba[1].hDerX = -90.0f;  KeyFrameScuba[1].hDerZ = -20.0f; KeyFrameScuba[1].cDer = 25.0f;
	KeyFrameScuba[1].pDerX = 0.0f;   KeyFrameScuba[1].pIzqX = 0.0f;

	// Frame 2
	KeyFrameScuba[2].posX = 0.3f;  KeyFrameScuba[2].posY = -0.1f; KeyFrameScuba[2].cZ = 0.0f;
	KeyFrameScuba[2].hIzqX = -135.0f; KeyFrameScuba[2].hIzqZ = 70.0f; KeyFrameScuba[2].hIzqY = -35.0f; KeyFrameScuba[2].cIzq = 55.0f;
	KeyFrameScuba[2].hDerX = -90.0f;  KeyFrameScuba[2].hDerZ = 15.0f; KeyFrameScuba[2].cDer = -15.0f;
	KeyFrameScuba[2].pDerX = -10.0f; KeyFrameScuba[2].pIzqX = 15.0f;

	// Frame 3
	KeyFrameScuba[3].posX = 0.0f;  KeyFrameScuba[3].posY = 0.0f; KeyFrameScuba[3].cZ = 20.0f;
	KeyFrameScuba[3].hIzqX = -135.0f; KeyFrameScuba[3].hIzqZ = 70.0f; KeyFrameScuba[3].hIzqY = -35.0f; KeyFrameScuba[3].cIzq = 55.0f;
	KeyFrameScuba[3].hDerX = -90.0f;  KeyFrameScuba[3].hDerZ = -20.0f; KeyFrameScuba[3].cDer = 25.0f;
	KeyFrameScuba[3].pDerX = 0.0f;   KeyFrameScuba[3].pIzqX = 0.0f;


	//Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models
	Model Sotano((char*)"Models/SotanoA.obj");
	GLuint tMural = LoadTextureRGBA("images/mural.jpg");

	// NUEVO: Texturas adicionales
	GLuint tAulaMagna = LoadTextureRGBA("images/AulaMagnaFrent.jpeg");
	texBandera = LoadTextureRGBA("images/banderaUnam.jpg");
	texSenal = LoadTextureRGBA("images/senal_evacuacion.png");
	texTooltip = LoadTextureRGBA("images/tooltip_stand.png");
	BuildSenalVAO();
	BuildFlagMesh();

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 3. Atributo de Coordenadas de Textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Confeti timer
		if (confettiActive) {
			confettiTimer += deltaTime;
			if (confettiTimer > 10.0f) { confettiActive = false; confettiTimer = 0.0f; }
		}

		// NUEVO: Animacion 7 — avanzar parametro de transicion de camara
		if (animCamTransicion) {
			tCam += velCam * deltaTime;
			if (tCam >= 1.0f) {
				tCam = 1.0f;
				animCamTransicion = false;
				camTransActiva = false;
				camEnInterior = !camEnInterior;
			}
			camTransPos = EvalBezier(
				camEnInterior ? bezP3 : bezP0,
				camEnInterior ? bezP2 : bezP1,
				camEnInterior ? bezP1 : bezP2,
				camEnInterior ? bezP0 : bezP3, tCam);
			camTransActiva = true;
		}

		// NUEVO: Animacion 6 — detectar proximidad a stands para tooltip
		UpdateTooltip(camera.GetPosition(), deltaTime);

		// Para saber la posición de la cámara 
		/*std::cout << "X: " << camera.GetPosition().x
			<< " Y: " << camera.GetPosition().y
			<< " Z: " << camera.GetPosition().z << std::endl;*/

			// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		// NUEVO: Animacion 10 — luz solar dinamica controlada con J/K
		glm::vec3 sunDir = GetSunDirection(tSolar);
		glm::vec3 sunColor = GetSunColor(tSolar);
		float intensidad = 0.3f + 0.7f * sinf(tSolar * glm::pi<float>());
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), sunDir.x, sunDir.y, sunDir.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.25f * intensidad, 0.25f * intensidad, 0.20f * intensidad);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), sunColor.r * intensidad, sunColor.g * intensidad, sunColor.b * intensidad);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.4f * intensidad, 0.4f * intensidad, 0.35f * intensidad);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		for (int i = 0; i < 8; i++) {
			std::string number = std::to_string(i);

			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);

			// Mantenemos los mismos colores e intensidades 
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].ambient").c_str()), 0.016f, 0.012f, 0.016f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].diffuse").c_str()), 0.35f, 0.35f, 0.35f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].specular").c_str()), 0.5f, 0.5f, 0.5f);

			// Atenuación estándar
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].linear").c_str()), 0.013f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].quadratic").c_str()), 0.092f);
		}


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f); // 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f); // 0
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f); // 0
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		// NUEVO: Animacion 7 — usar camara de transicion o la libre segun estado
		glm::mat4 view;
		if (camTransActiva) {
			glm::vec3 destino = camEnInterior ? bezP0 : bezP3;
			view = glm::lookAt(camTransPos, destino, glm::vec3(0, 1, 0));
		}
		else {
			view = camera.GetViewMatrix();
		}

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Carga de modelo 

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::mat4 model(1);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sotano.Draw(lightingShader);
		glDisable(GL_BLEND);

		// Confeti
		if (confettiActive) {
			GLint colorLoc = glGetUniformLocation(lightingShader.Program, "color");
			if (colorLoc == -1) colorLoc = glGetUniformLocation(lightingShader.Program, "diffuseColor");
			DrawConfetti(VAO, lightingShader, modelLoc, colorLoc, -1, confettiTimer);
		}

		// Mural
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tMural);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		glm::mat4 modelMural(1);
		modelMural = glm::translate(modelMural, glm::vec3(0.05f, 3.8f, -13.0f));
		modelMural = glm::rotate(modelMural, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMural = glm::scale(modelMural, glm::vec3(18.0f, 3.0f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMural));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// NUEVO: Pared Aula Magna (lado derecho del sotano, detras de los stands)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tAulaMagna);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		glm::mat4 modelAula(1);
		modelAula = glm::translate(modelAula, glm::vec3(24.8f, 2.0f, -8.4f));
		modelAula = glm::rotate(modelAula, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelAula = glm::scale(modelAula, glm::vec3(18.0f, 4.0f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAula));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// NUEVO: Animacion 8 — Poste de la bandera
		{
			glm::mat4 mp(1);
			mp = glm::translate(mp, glm::vec3(14.5f, 1.5f, 3.5f));
			mp = glm::scale(mp, glm::vec3(0.06f, 3.5f, 0.06f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mp));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		// NUEVO: Animacion 8 — Bandera UNAM ondeando
		UpdateFlagWave(currentFrame);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texBandera);
		{
			glm::mat4 mf(1);
			mf = glm::translate(mf, glm::vec3(14.5f, 1.8f, 3.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mf));
			glBindVertexArray(flagVAO);
			glDrawElements(GL_TRIANGLES, (FLAG_ROWS - 1) * (FLAG_COLS - 1) * 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		// NUEVO: Animacion 5 — Señaletica pulsante + Animacion 6 — Tooltip
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawSignage(lightingShader, modelLoc, currentFrame, view);
		DrawTooltip(lightingShader, modelLoc, view);
		glDisable(GL_BLEND);

		//Dibujar modelos 


		carro.Draw(lightingShader, VAO);

		TrenRojo.Draw(lightingShader, VAO);

		SillaPlegable.Draw(lightingShader, VAO);

		MesaPlegable.Draw(lightingShader, VAO);

		botarga.Draw(lightingShader, VAO);

		stand1.Draw(lightingShader, VAO);

		persona1.Draw(lightingShader, VAO);

		botarga2.Draw(lightingShader, VAO);

		rejaPuertaIzq.Draw(lightingShader, VAO);

		rejaPuertaDer.Draw(lightingShader, VAO);

		personaCaminando.Draw(lightingShader, VAO);

		persona2.Draw(lightingShader, VAO);

		// DIBUJANDO STANDS DE MUESTRA
		float DesplazamientoX = 5.5f; // Espacio entre cada set
		float DesplazamientoZ = -5.0f;
		for (int i = 0; i < 5; i++) {
			glm::mat4 matrizSet = glm::mat4(1.0f);

			// Posicionamos cada set en el eje X
			matrizSet = glm::translate(matrizSet, glm::vec3(i * DesplazamientoX, 0.0f, -19.0f));
			matrizSet = glm::translate(matrizSet, glm::vec3(-13.2f, 0.05f, 0.05f));
			// Dibujamos usando la matriz padre
			stand2.Draw(lightingShader, VAO, matrizSet);
			mesa2.Draw(lightingShader, VAO, matrizSet);
			silla2.Draw(lightingShader, VAO, matrizSet);
		}
		for (int i = 0; i < 3; i++) {
			glm::mat4 matrizSet = glm::mat4(1.0f);
			// Posicionamos cada set en el eje z
			matrizSet = glm::translate(matrizSet, glm::vec3(18.0f, 0.05f, i * DesplazamientoZ));
			matrizSet = glm::translate(matrizSet, glm::vec3(0.05f, 0.2f, -12.0f));
			matrizSet = glm::rotate(matrizSet, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			matrizSet = glm::scale(matrizSet, glm::vec3(0.8f, 0.7f, 0.8f));
			stand3.Draw(lightingShader, VAO, matrizSet);
			mesa3.Draw(lightingShader, VAO, matrizSet);
			silla3.Draw(lightingShader, VAO, matrizSet);
		}


		float desplazamientoX = -5.0f; // Espacio entre cada lampara
		float desplazamientoZ = -9.0f;
		float desplazamientoZ2 = -12.75f;
		float desplazamientoZ1 = -5.0f;
		for (int i = 0; i < 3; i++) {
			glm::mat4 matrizSet = glm::mat4(1.0f);

			// Posicionamos cada lampara en el eje X
			matrizSet = glm::translate(matrizSet, glm::vec3(i * desplazamientoX, 0.0f, 0.0f));
			lampara1.Draw(lightingShader, VAO, matrizSet);
		}

		for (int i = 0; i < 3; i++) {
			glm::mat4 matrizSet = glm::mat4(1.0f);
			// Posicionamos cada lampara en el eje z
			matrizSet = glm::translate(matrizSet, glm::vec3(i * -5.0f, 0.0f, desplazamientoZ));
			lampara1.Draw(lightingShader, VAO, matrizSet);
		}
		for (int i = 0; i < 3; i++) {
			glm::mat4 matrizSet = glm::mat4(1.0f);
			// Posicionamos cada lampara en el eje z
			matrizSet = glm::translate(matrizSet, glm::vec3(i * -5.0f, 0.0f, desplazamientoZ1));
			lampara1.Draw(lightingShader, VAO, matrizSet);
		}
		for (int i = 0; i < 3; i++) {
			glm::mat4 matrizSet = glm::mat4(1.0f);
			// Posicionamos cada lampara en el eje z
			matrizSet = glm::translate(matrizSet, glm::vec3(i * -5.0f, 0.0f, desplazamientoZ2));
			lampara1.Draw(lightingShader, VAO, matrizSet);
		}

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();


		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// NUEVO: Bloquear camara libre durante transicion (Animacion 7)
	if (camTransActiva) return;

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		animCarro = !animCarro;
		if (animCarro) {
			estadoCarro = 0;
			// Reiniciamos las coordenadas base para que repita la animación correctamente
			carro.posicion = glm::vec3(37.0f, 0.050f, 11.0f);
			carro.rotacion.y = 180.0f;
		}
	}

	if (keys[GLFW_KEY_L] && !AnimacionEnProgreso)
	{
		AnimSilla = !AnimSilla;// Alterna entre abrir y cerrar
		SillaAbierta = !SillaAbierta;
		AnimacionEnProgreso = true;
	}
	else if (!keys[GLFW_KEY_L])
	{
		AnimacionEnProgreso = false;
	}

	if (keys[GLFW_KEY_O] && !AnimacionMesaEnProgreso)
	{
		AnimMesa = !AnimMesa;        // Cambia el estado de animación
		MesaAbierta = !MesaAbierta;   // Cambia el estado de la mesa
		AnimacionMesaEnProgreso = true;
	}
	else if (!keys[GLFW_KEY_O])
	{
		AnimacionMesaEnProgreso = false;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		animPersonaCaminando = !animPersonaCaminando;
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		animPikachuDance = !animPikachuDance;
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		animScubaCat = !animScubaCat;
	}
	if (keys[GLFW_KEY_P] && !AnimacionRejasEnProgreso)
	{
		AnimRejas = !AnimRejas;             // Alterna el estado de animación
		RejasAbiertas = !RejasAbiertas;     // Alterna el estado físico
		AnimacionRejasEnProgreso = true;    // Bloquea múltiples lecturas
	}
	else if (!keys[GLFW_KEY_P])
	{
		AnimacionRejasEnProgreso = false;   // Libera el botón al soltarlo
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) { //Botón para el confeti
		confettiActive = true;
		confettiTimer = 0.0f;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		animPersona2Caminando = !animPersona2Caminando;
		if (animPersona2Caminando) {
			estadoCaminando2 = 0; // Reiniciar desde el primer estado
			persona2.posicion = glm::vec3(28.0f, 0.13f, -18.1f); // Reset de posición
		}
	}

	// NUEVO: T → Transicion camara exterior↔interior (Animacion 7)
	if (key == GLFW_KEY_T && action == GLFW_PRESS && !animCamTransicion) {
		animCamTransicion = true;
		camTransActiva = true;
		tCam = 0.0f;
	}

	// NUEVO: J → Sol avanza (mas tarde del dia) (Animacion 10)
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		tSolar += 0.05f;
		if (tSolar > 1.0f) tSolar = 0.0f;
	}

	// NUEVO: K → Sol retrocede (mas temprano) (Animacion 10)
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		tSolar -= 0.05f;
		if (tSolar < 0.0f) tSolar = 1.0f;
	}

}
void Animation() {

	if (animCarro) {
		//Lógica de rotación de llantas diferenciada por estado
		if (estadoCarro == 0) {
			carro.rotacionLlantas -= rLlantaVel * deltaTime; // Adelante
		}
		else if (estadoCarro == 1 || estadoCarro == 2) {
			carro.rotacionLlantas += rLlantaVel * deltaTime; // Reversa normal
		}
		else if (estadoCarro == 3) {
			carro.rotacionLlantas += (rLlantaVel * 0.2f) * deltaTime; // Reversa lenta de acomodo
		}

		//Máquina de Estados del Trayecto
		switch (estadoCarro) {
		case 0: // AVANCE RECTO 
			carro.posicion.x -= velocidadCarro * deltaTime;
			carro.giroVolante = 0.0f;

			// Avanzamos hasta 17.3 para que el trayecto diagonal coincida exactamente con Pikachu
			if (carro.posicion.x <= 18.3f) {
				estadoCarro = 1;
			}
			break;

		case 1: // REVERSA QUEBRANDO (Trompa a +Z, Cola a -Z)
			carro.posicion.x += (velocidadCarro * 0.4f) * deltaTime;
			carro.posicion.z -= (velocidadCarro * 1.05f) * deltaTime;

			carro.giroVolante = 35.0f; // Volante a la izquierda
			carro.rotacion.y += 45.0f * deltaTime;

			// Llegamos a los 225 grados (45° de inclinación)
			if (carro.rotacion.y >= 225.0f) {
				carro.rotacion.y = 225.0f;
				estadoCarro = 2;
			}
			break;

		case 2: // REVERSA EN DIAGONAL (Mantiene el ángulo de 225°)
			// Nos movemos en línea recta hacia atrás, igualando la proporción en X y Z
			carro.posicion.x += (velocidadCarro * 0.5f) * deltaTime;
			carro.posicion.z -= (velocidadCarro * 0.5f) * deltaTime;

			carro.giroVolante = 0.0f; // Llantas rectas
			// Nos detenemos cerca del cajón
			if (carro.posicion.z <= 4.9f) {
				estadoCarro = 3;
			}
			break;

		case 3: // ACOMODO A (22, 0.05, 4.7)
			carro.giroVolante = 0.0f;

			if (carro.posicion.x < 21.9f) {
				carro.posicion.x += (velocidadCarro * 0.2f) * deltaTime;
			}
			else if (carro.posicion.x > 22.1f) {
				carro.posicion.x -= (velocidadCarro * 0.2f) * deltaTime;
			}
			else {
				carro.posicion.x = 22.0f;
				alineadoX = true;
			}

			//Objetivo 4.7f
			if (carro.posicion.z > 4.8f) { // Tolerancia superior
				carro.posicion.z -= (velocidadCarro * 0.2f) * deltaTime;
			}
			else if (carro.posicion.z < 4.6f) { // Tolerancia inferior
				carro.posicion.z += (velocidadCarro * 0.2f) * deltaTime;
			}
			else {
				carro.posicion.z = 4.7f; // Valor final exacto
				alineadoZ = true;
			}

			if (alineadoX && alineadoZ) {
				estadoCarro = 4;
			}
			break;

		case 4: // DETENIDO
			carro.giroVolante = 0.0f;
			animCarro = false;
			break;
		}
	}
	if (AnimMesa) {
		// Abrir mesa (0° a 180°)
		if (MesaAbierta && MesaPlegable.anguloApertura < 180.0f) {
			MesaPlegable.anguloApertura += 60.0f * deltaTime;
		}
		// Cerrar mesa (180° a 90°)
		else if (!MesaAbierta && MesaPlegable.anguloApertura > 90.0f) {
			MesaPlegable.anguloApertura -= 60.0f * deltaTime;
		}
		else {
			AnimMesa = false;  // Termina animación
		}
	}
	if (AnimSilla) {
		// Si la silla está abierta, ciérrala
		if (SillaAbierta && SillaPlegable.anguloPlegado < 80.0f) {
			SillaPlegable.anguloPlegado += 50.0f * deltaTime;
		}
		// Si la silla está cerrada, ábrela
		else if (!SillaAbierta && SillaPlegable.anguloPlegado > 0.0f) {
			SillaPlegable.anguloPlegado -= 50.0f * deltaTime;
		}
		// Cuando termina la animación, desactiva AnimSilla
		else if ((SillaAbierta && SillaPlegable.anguloPlegado >= 80.0f) ||
			(!SillaAbierta && SillaPlegable.anguloPlegado <= 0.0f)) {
			AnimSilla = false;
		}
	}

	//CAMINATA
	if (animPersonaCaminando) {
		// Si acaba de iniciar, pasamos del estado de reposo (0) al primer movimiento (1)
		if (estadoCaminando == 0) {
			estadoCaminando = 1;
		}

		// Pierna derecha adelante, pierna izquierda atrás
		if (estadoCaminando == 1) {
			personaCaminando.rotPiernaDerX += velocidadCaminata * deltaTime;
			personaCaminando.rotPiernaIzqX -= velocidadCaminata * deltaTime;

			// Los brazos hacen el movimiento contrario a las piernas
			personaCaminando.rotHombroDerX -= velocidadCaminata * deltaTime;
			personaCaminando.rotHombroIzqX += velocidadCaminata * deltaTime;

			// Condición para cambiar de estado (Llegó al límite del paso)
			if (personaCaminando.rotPiernaDerX >= limitePaso) {
				estadoCaminando = 2; // Cambiamos al movimiento contrario
			}
		}
		// Pierna izquierda adelante, pierna derecha atrás
		else if (estadoCaminando == 2) {
			personaCaminando.rotPiernaDerX -= velocidadCaminata * deltaTime;
			personaCaminando.rotPiernaIzqX += velocidadCaminata * deltaTime;

			// Los brazos hacen el movimiento contrario
			personaCaminando.rotHombroDerX += velocidadCaminata * deltaTime;
			personaCaminando.rotHombroIzqX -= velocidadCaminata * deltaTime;

			// Condición para regresar al primer estado
			if (personaCaminando.rotPiernaDerX <= -limitePaso) {
				estadoCaminando = 1;
			}
		}

		//DESPLAZAMIENTO
		personaCaminando.posicion.z -= 1.5f * deltaTime;

		if (personaCaminando.posicion.z <= -18.0f) { //limite final
			animPersonaCaminando = false;

		}
		// Flexionar ligeramente las rodillas/codos mientras camina 
		personaCaminando.rotRodillaDer = 15.0f;
		personaCaminando.rotRodillaIzq = 15.0f;
		personaCaminando.rotCodoDer = -15.0f;
		personaCaminando.rotCodoIzq = -15.0f;
	}
	else {
		// Cuando se desactiva la animación, mandamos un cero en cada una de sus componentes (reposo)
		estadoCaminando = 0;
		personaCaminando.rotPiernaDerX = 0.0f;
		personaCaminando.rotPiernaIzqX = 0.0f;
		personaCaminando.rotHombroDerX = 0.0f;
		personaCaminando.rotHombroIzqX = 0.0f;

		personaCaminando.rotRodillaDer = 0.0f;
		personaCaminando.rotRodillaIzq = 0.0f;
		personaCaminando.rotCodoDer = 0.0f;
		personaCaminando.rotCodoIzq = 0.0f;
	}
	//RAT DANCE
	if (animPikachuDance) {
		if (i_curr_steps >= i_max_steps) {
			playIndex = (playIndex + 1) % MAX_FRAMES;
			i_curr_steps = 0;
		}

		if (i_curr_steps == 0) {
			int nextFrame = (playIndex + 1) % MAX_FRAMES;

			inc_posY = (KeyFrame[nextFrame].posY - KeyFrame[playIndex].posY) / i_max_steps;
			inc_pDerX = (KeyFrame[nextFrame].pDerX - KeyFrame[playIndex].pDerX) / i_max_steps;
			inc_pIzqX = (KeyFrame[nextFrame].pIzqX - KeyFrame[playIndex].pIzqX) / i_max_steps;
			inc_cZ = (KeyFrame[nextFrame].cZ - KeyFrame[playIndex].cZ) / i_max_steps;

			// Brazo Izquierdo
			inc_hIzqX = (KeyFrame[nextFrame].hIzqX - KeyFrame[playIndex].hIzqX) / i_max_steps;
			inc_hIzqY = (KeyFrame[nextFrame].hIzqY - KeyFrame[playIndex].hIzqY) / i_max_steps;
			inc_hIzqZ = (KeyFrame[nextFrame].hIzqZ - KeyFrame[playIndex].hIzqZ) / i_max_steps;
			inc_cIzq = (KeyFrame[nextFrame].cIzq - KeyFrame[playIndex].cIzq) / i_max_steps;

			// Brazo Derecho
			inc_hDerX = (KeyFrame[nextFrame].hDerX - KeyFrame[playIndex].hDerX) / i_max_steps;
			inc_hDerY = (KeyFrame[nextFrame].hDerY - KeyFrame[playIndex].hDerY) / i_max_steps;
			inc_hDerZ = (KeyFrame[nextFrame].hDerZ - KeyFrame[playIndex].hDerZ) / i_max_steps;
			inc_cDer = (KeyFrame[nextFrame].cDer - KeyFrame[playIndex].cDer) / i_max_steps;
		}

		botarga.posicion.y += inc_posY;
		botarga.rotPiernaDerX += inc_pDerX;
		botarga.rotPiernaIzqX += inc_pIzqX;
		botarga.rotCabezaZ += inc_cZ;

		botarga.rotHombroIzqX += inc_hIzqX;
		botarga.rotHombroIzqY += inc_hIzqY;
		botarga.rotHombroIzqZ += inc_hIzqZ;
		botarga.rotCodoIzq += inc_cIzq;

		botarga.rotHombroDerX += inc_hDerX;
		botarga.rotHombroDerY += inc_hDerY;
		botarga.rotHombroDerZ += inc_hDerZ;
		botarga.rotCodoDer += inc_cDer;

		i_curr_steps++;
	}
	else {
		// Al apagar el baile, reiniciar a la pose oficial de tu constructor en pikachu.cpp
		playIndex = 0;
		i_curr_steps = 0;
		botarga.posicion.y = 0.0f;
		botarga.rotPiernaDerX = 0.0f; botarga.rotPiernaIzqX = 0.0f;
		botarga.rotCabezaZ = 0.0f;
		// Pose de reposo: Brazos a los costados
		botarga.rotHombroIzqX = 0.0f; botarga.rotHombroIzqY = 0.0f; botarga.rotHombroIzqZ = 60.0f;
		botarga.rotCodoIzq = 0.0f;
		botarga.rotHombroDerX = 0.0f; botarga.rotHombroDerY = 0.0f; botarga.rotHombroDerZ = -60.0f;
		botarga.rotCodoDer = 0.0f;
	}

	//SCUBA CAT
	if (animScubaCat) {
		if (i_curr_stepsScuba >= i_max_stepsScuba) {
			playIndexScuba = (playIndexScuba + 1) % MAX_FRAMES_SCUBA;
			i_curr_stepsScuba = 0;
		}

		if (i_curr_stepsScuba == 0) {
			int nextFrame = (playIndexScuba + 1) % MAX_FRAMES_SCUBA;

			incS_cZ = (KeyFrameScuba[nextFrame].cZ - KeyFrameScuba[playIndexScuba].cZ) / i_max_stepsScuba;
			incS_posX = (KeyFrameScuba[nextFrame].posX - KeyFrameScuba[playIndexScuba].posX) / i_max_stepsScuba;
			incS_posY = (KeyFrameScuba[nextFrame].posY - KeyFrameScuba[playIndexScuba].posY) / i_max_stepsScuba;
			incS_pDerX = (KeyFrameScuba[nextFrame].pDerX - KeyFrameScuba[playIndexScuba].pDerX) / i_max_stepsScuba;
			incS_pIzqX = (KeyFrameScuba[nextFrame].pIzqX - KeyFrameScuba[playIndexScuba].pIzqX) / i_max_stepsScuba;

			// Incrementos Brazo Izquierdo
			incS_hIzqX = (KeyFrameScuba[nextFrame].hIzqX - KeyFrameScuba[playIndexScuba].hIzqX) / i_max_stepsScuba;
			incS_hIzqY = (KeyFrameScuba[nextFrame].hIzqY - KeyFrameScuba[playIndexScuba].hIzqY) / i_max_stepsScuba;
			incS_hIzqZ = (KeyFrameScuba[nextFrame].hIzqZ - KeyFrameScuba[playIndexScuba].hIzqZ) / i_max_stepsScuba;
			incS_cIzq = (KeyFrameScuba[nextFrame].cIzq - KeyFrameScuba[playIndexScuba].cIzq) / i_max_stepsScuba;

			// Incrementos Brazo Derecho
			incS_hDerX = (KeyFrameScuba[nextFrame].hDerX - KeyFrameScuba[playIndexScuba].hDerX) / i_max_stepsScuba;
			incS_hDerY = (KeyFrameScuba[nextFrame].hDerY - KeyFrameScuba[playIndexScuba].hDerY) / i_max_stepsScuba;
			incS_hDerZ = (KeyFrameScuba[nextFrame].hDerZ - KeyFrameScuba[playIndexScuba].hDerZ) / i_max_stepsScuba;
			incS_cDer = (KeyFrameScuba[nextFrame].cDer - KeyFrameScuba[playIndexScuba].cDer) / i_max_stepsScuba;
		}

		botarga2.rotCabezaZ += incS_cZ;

		botarga2.posicion.x += incS_posX;
		botarga2.posicion.y += incS_posY;
		botarga2.rotPiernaDerX += incS_pDerX;
		botarga2.rotPiernaIzqX += incS_pIzqX;

		botarga2.rotHombroIzqX += incS_hIzqX;
		botarga2.rotHombroIzqY += incS_hIzqY;
		botarga2.rotHombroIzqZ += incS_hIzqZ;
		botarga2.rotCodoIzq += incS_cIzq;

		botarga2.rotHombroDerX += incS_hDerX;
		botarga2.rotHombroDerY += incS_hDerY;
		botarga2.rotHombroDerZ += incS_hDerZ;
		botarga2.rotCodoDer += incS_cDer;

		i_curr_stepsScuba++;
	}
	else {

		playIndexScuba = 0;
		i_curr_stepsScuba = 0;
		botarga2.posicion.x = 5.0f;
		botarga2.posicion.y = 0.0f;
		botarga2.rotPiernaDerX = 0.0f; botarga2.rotPiernaIzqX = 0.0f;
		botarga2.rotCabezaZ = 0.0f;

		// Reposo definido en tu pikachu.cpp
		botarga2.rotHombroIzqX = 0.0f; botarga2.rotHombroIzqY = 0.0f; botarga2.rotHombroIzqZ = 60.0f;
		botarga2.rotCodoIzq = 0.0f;
		botarga2.rotHombroDerX = 0.0f; botarga2.rotHombroDerY = 0.0f; botarga2.rotHombroDerZ = -60.0f;
		botarga2.rotCodoDer = 0.0f;
	}
	//ANIMACIÓN REJAS
	if (AnimRejas) {
		if (!RejasAbiertas) {
			if (rejaPuertaIzq.desplazamientoX < 5.0f) {
				rejaPuertaIzq.desplazamientoX += velocidadReja * deltaTime;
			}
			else {
				rejaPuertaIzq.desplazamientoX = 5.0f; // Tope exacto
				p1_llego = true;
			}

			// Reja Derecha: resta 5 unidades en -X
			if (rejaPuertaDer.desplazamientoX > -5.0f) {
				rejaPuertaDer.desplazamientoX -= velocidadReja * deltaTime;
			}
			else {
				rejaPuertaDer.desplazamientoX = -5.0f; // Tope exacto
				p2_llego = true;
			}
		}

		else {

			// Reja Izquierda: resta en X para volver a 0
			if (rejaPuertaIzq.desplazamientoX > 0.0f) {
				rejaPuertaIzq.desplazamientoX -= velocidadReja * deltaTime;
			}
			else {
				rejaPuertaIzq.desplazamientoX = 0.0f;
				p1_llego = true;
			}

			// Reja Derecha: suma en X para volver a 0
			if (rejaPuertaDer.desplazamientoX < 0.0f) {
				rejaPuertaDer.desplazamientoX += velocidadReja * deltaTime;
			}
			else {
				rejaPuertaDer.desplazamientoX = 0.0f;
				p2_llego = true;
			}

		}

		// Condición Maestra de Paro (Aplica tanto para abrir como para cerrar)
		if (p1_llego && p2_llego) {
			AnimRejas = false; // Apagamos el motor de animación

			// Reiniciamos los flags para que estén limpios la próxima vez que presiones 'P'
			p1_llego = false;
			p2_llego = false;

		}
	}
	//CAMINATA CON GIROS Y SALUDO
	if (animPersona2Caminando) {
		static bool pasoDerecho = true;
		static bool codoSube = true;; // Movimiento rítmico de caminata
		static float tiempoSaludo = 0.0f;
		// Aplicamos la flexión solo durante la caminata y los giros(Estados 0 al 3)
		if (estadoCaminando2 < 4) {
			persona2.rotRodillaDer = 15.0f;
			persona2.rotRodillaIzq = 15.0f;
			persona2.rotCodoDer = -15.0f;
			persona2.rotCodoIzq = -15.0f;

		}
		switch (estadoCaminando2) {
		case 0: // GIRAR PARA MIRAR AL EJE X POSITIVO
			persona2.rotacion.y += 100.0f * deltaTime;
			if (persona2.rotacion.y >= -90.0f) { // Apunta a +X
				persona2.rotacion.y = -90.0f;
				estadoCaminando2 = 1;
			}
			break;

		case 1: // CAMINAR HACIA X = 14
			persona2.posicion.x -= 2.5f * deltaTime; // Restamos para avanzar a 14

			// Animación de extremidades
			if (pasoDerecho) {
				persona2.rotPiernaDerX += velocidadCaminata2 * deltaTime;
				persona2.rotPiernaIzqX -= velocidadCaminata2 * deltaTime;
				persona2.rotHombroDerX -= velocidadCaminata2 * deltaTime;
				persona2.rotHombroIzqX += velocidadCaminata2 * deltaTime;
				if (persona2.rotPiernaDerX >= limitePaso2) pasoDerecho = false; // Invertir
			}
			else {
				persona2.rotPiernaDerX -= velocidadCaminata2 * deltaTime;
				persona2.rotPiernaIzqX += velocidadCaminata2 * deltaTime;
				persona2.rotHombroDerX += velocidadCaminata2 * deltaTime;
				persona2.rotHombroIzqX -= velocidadCaminata2 * deltaTime;
				if (persona2.rotPiernaDerX <= -limitePaso2) pasoDerecho = true; // Invertir
			}
			if (persona2.posicion.x <= 14.0f) {
				persona2.posicion.x = 14.0f;
				estadoCaminando2 = 2;
			}
			break;

		case 2: // ROTAR 90 GRADOS HACIA EJE Z POSITIVO
			persona2.rotacion.y += 100.0f * deltaTime;
			if (persona2.rotacion.y >= 0.0f) { // Mira hacia +Z
				persona2.rotacion.y = 0.0f;
				estadoCaminando2 = 3;
			}
			break;

		case 3: // CAMINAR HACIA Z = 1
			persona2.posicion.z += 2.5f * deltaTime; // Sumamos en Z

			if (pasoDerecho) {
				persona2.rotPiernaDerX += velocidadCaminata2 * deltaTime;
				persona2.rotPiernaIzqX -= velocidadCaminata2 * deltaTime;
				persona2.rotHombroDerX -= velocidadCaminata2 * deltaTime;
				persona2.rotHombroIzqX += velocidadCaminata2 * deltaTime;
				if (persona2.rotPiernaDerX >= limitePaso2) pasoDerecho = false;
			}
			else {
				persona2.rotPiernaDerX -= velocidadCaminata2 * deltaTime;
				persona2.rotPiernaIzqX += velocidadCaminata2 * deltaTime;
				persona2.rotHombroDerX += velocidadCaminata2 * deltaTime;
				persona2.rotHombroIzqX -= velocidadCaminata2 * deltaTime;
				if (persona2.rotPiernaDerX <= -limitePaso2) pasoDerecho = true;
			}

			if (persona2.posicion.z >= 1.0f) {
				persona2.posicion.z = 1.0f;
				estadoCaminando2 = 4;
				// Reset de extremidades a posición neutral para el saludo
				persona2.rotPiernaDerX = 0; persona2.rotPiernaIzqX = 0;
				persona2.rotHombroDerX = 0; persona2.rotHombroIzqX = 0;
				tiempoSaludo = 0.0f;
			}
			break;

		case 4: // SALUDO CON BRAZO IZQUIERDO
			persona2.rotHombroIzqZ = 240.0f; // Levanta el brazo
			// Movimiento de saludo
			if (codoSube) {
				persona2.rotCodoIzq += 120.0f * deltaTime; // 120 es la velocidad del saludo
				if (persona2.rotCodoIzq >= 30.0f) codoSube = false; // Tope superior
			}
			else {
				persona2.rotCodoIzq -= 120.0f * deltaTime;
				if (persona2.rotCodoIzq <= -30.0f) codoSube = true; // Tope inferior
			}
			tiempoSaludo += deltaTime;
			// Terminar saludo después de unos segundos 
			if (tiempoSaludo > 3.0f) { // Ajustar según tiempo deseado
				estadoCaminando2 = 5;
			}
			break;

		case 5: // FINALIZAR Y REPOSO
			persona2.rotHombroIzqZ = 0.0f;
			persona2.rotCodoIzq = 0.0f;
			animPersona2Caminando = false;
			break;
		}
	}
	else {
		// Cuando se desactiva la animación, mandamos un cero en todas sus componentes (reposo)
		estadoCaminando2 = 0;
		persona2.rotPiernaDerX = 0.0f;
		persona2.rotPiernaIzqX = 0.0f;
		persona2.rotHombroDerX = 0.0f;
		persona2.rotHombroIzqX = 0.0f;
		persona2.rotHombroIzqZ = 0.0f; // Reseteamos también el levantamiento del brazo del saludo

		persona2.rotRodillaDer = 0.0f;
		persona2.rotRodillaIzq = 0.0f;
		persona2.rotCodoDer = 0.0f;
		persona2.rotCodoIzq = 0.0f;
	}
}

void DrawConfetti(GLuint VAO, Shader& s, GLint mL, GLint cL, GLint uTL, float timer)
{
	float ceil = 3.0f, life = 4.0f;
	for (int i = 0; i < NUM_CONFETTI; i++) {
		const ConfettiParticle& p = confParticles[i];
		float rel = fmodf(timer + p.phase, life);
		float y = ceil - p.speed * rel * 1.8f; if (y < 0) continue;
		float sc = 0.12f, lr = rel / life;
		if (lr > 0.80f) sc *= (1 - (lr - 0.80f) / 0.20f);
		if (sc < 0.005f) continue;

		// Crear textura de 1x1 pixel con el color de la particula
		GLuint texTemp;
		glGenTextures(1, &texTemp);
		glBindTexture(GL_TEXTURE_2D, texTemp);
		unsigned char col[3] = {
			(unsigned char)(p.colorR * 255),
			(unsigned char)(p.colorG * 255),
			(unsigned char)(p.colorB * 255)
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, col);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texTemp);

		glm::mat4 m(1);
		m = glm::translate(m, glm::vec3(p.x, y, p.z));
		m = glm::rotate(m, glm::radians(p.rotSpeed * rel), glm::vec3(0, 0, 1));
		m = glm::scale(m, glm::vec3(sc, sc * .5f, sc * .1f));
		glUniformMatrix4fv(mL, 1, GL_FALSE, glm::value_ptr(m));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDeleteTextures(1, &texTemp);
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	// NUEVO: Bloquear mouse durante transicion de camara (Animacion 7)
	if (camTransActiva) return;

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}