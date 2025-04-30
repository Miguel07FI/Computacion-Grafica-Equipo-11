/*
Hernandez Ramirez Miguel Angel
Osorio Angeles Rodrigo Jafet
Del Razo Sanchez Diego Adrian
Proyecto 1 Equipo 11
11 de abril del 2025
319044618
318008893
319114939
*/
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
#include "Texture.h"

#include <vector>


//ANIMAICION


float globalTranslationYInicial = 7.0f;
float globalTranslationYFinal = 0.5f;
bool animacionXTerminada = false;

glm::vec3 globalTranslationX = glm::vec3(0.0f, globalTranslationYInicial, -25.8f);
glm::vec3 globalScaleX = glm::vec3(4.0f, 4.0f, 4.0f);
float globalRotationX = 90.0f;

bool bajandoY = false;
float velocidadBajadaY = 4.0f; // puedes ajustarlo si quieres más lento


bool reduciendoEscala = false;
float escalaTemporal = 4.0f;




// Animación silla 'si' y nueva silla 'sn'
bool animSiActive = true;         // Aparece al arranque
bool animSnActive = false;

bool siMovingOut = false;
bool siShrinking = false;
bool snAppearing = false;
bool snMovingIn = false;

glm::vec3 siPos = glm::vec3(5.5f, -1.5f, -25.0f);
glm::vec3 snPos = glm::vec3(9.0f, -1.0f, -15.0f);  // Aparece aquí

float siScale = 4.0f;
float snScale = 0.0f;



// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
bool explosionActive = false;



float posBallY = 0.0f;  // Posición inicial de la pelota en Y
float velocidad = 2.0f;  // Velocidad de la animación (puedes ajustarla)
bool moviendoArriba = true;  // Variable para controlar la dirección de la pelota
// Control de la animación
bool animacionActivada = false;  // Animación inicialmente desactivada


float limiteInferior = 0.0f;
float limiteSuperior = 1.7;
float explosionFactor = 0.0f;

float implosionFactor = 0.0f;
bool implosionActive = false;

// Control de visibilidad y animación de modelos
bool model1Visible = true;
bool model2Visible = false;
bool model2Appearing = false;
float model2ScaleFactor = 0.0f;
float model2Rotation = 0.0f;
float model2TargetRotation = 1080.0f; // 3 vueltas completas

const float escalaX = 2.0f;
const float escalaY = 3.2f;
const float escalaZ = 4.0f;

// Para modelo 'ga'
bool gaExplosionActive = false;
bool gaVisible = true;
float gaExplosionFactor = 1.0f;
bool gaInflating = false;
bool gaContracting = false;


bool ganAppearing = false;
float ganRotation = 0.0f;
float ganTargetRotation = 270.0f;

// Agregar variables globales para animación del modelo g
float gOffsetX = -0.5f;
bool gMovingOut = false;
bool gReturning = false;
bool gAnimActive = false;

// Para modelo 'g'
bool gVisible = false;
bool gAppearing = false;
float gScaleFactor = 0.0f;
float gRotation = 0.0f;
float gTargetRotation = 180.0f;

// Variables para modelo 'gr'
bool grVisible = false;
bool grAppearing = false;
float grScaleFactor = 0.0f;
float grRotation = 0.0f;
float grTargetRotation = 360.0f;

float grOffsetX = -0.5f;
bool grMovingOut = false;
bool grReturning = false;
bool grAnimActive = false;

bool ramVisible = false;
bool ramAppearing = false;
float ramScaleFactor = 0.0f;

bool grAppearingScale = false;  // Ya existe `grAppearing` pero lo usamos para rotación, así que usamos otro para la escala

bool ramAnimActive = false;
bool ramMovingOut = false;
bool ramReturning = false;
float ramOffsetX = -0.5f;

static bool pasoExtraGr = false;
static bool pasoExtraRam = false;

bool animacionXCompleta = false;


// Estados por modelo para X
bool gInPhase1 = false, gInPhase2 = false;
bool grInPhase1 = false, grInPhase2 = false;
bool ramInPhase1 = false, ramInPhase2 = false;

float ramRotation = 0.0f;
float ramTargetRotation = 360.0f;


bool prVisible = false;
bool prAnimActive = false;
bool prInPhase1 = false, prInPhase2 = false;
float prScaleFactor = 0.0f;
float prRotation = 0.0f;
float prOffsetX = -0.5f;

// Variables para modelo 'fu'
bool fuVisible = false;
bool fuAnimActive = false;
bool fuInPhase1 = false, fuInPhase2 = false;
float fuScaleFactor = 0.0f;
float fuRotation = 0.0f;
float fuOffsetX = -0.5f;
float fuTargetRotation = 180.0f;

bool ganFinished = false;

bool gaAntesDeX = false;

bool ganVisible = false;     // Antes estaba en true
float ganScaleFactor = 1.0f;
bool ganDisappearing = false;



struct InstanciaAnimacion {
	glm::vec3 posicion;
	glm::vec3 escala;
	float rotacionBase;
	float escalaTemporal;
	float translationY;
	bool visible;

	// Estados de aparición
	bool gaVisible, gaExplosionActive, gaInflating, gaContracting;
	float gaExplosionFactor;
	bool animacionEstatica;  // true para los 3 nuevos modelos sin escala ni traslación

	bool ganVisible, ganAppearing;
	float ganScaleFactor, ganRotation;

	bool ganFinished;

	bool gVisible, gAppearing, gAnimActive;
	float gScaleFactor, gRotation, gOffsetX;
	float gTargetRotation;
	bool gInPhase1, gInPhase2;

	bool grVisible, grAnimActive;
	float grScaleFactor, grRotation, grOffsetX;
	float grTargetRotation;
	bool grInPhase1, grInPhase2;

	bool ramVisible, ramAnimActive;
	float ramScaleFactor, ramRotation, ramOffsetX;
	float ramTargetRotation;
	bool ramInPhase1, ramInPhase2;

	bool prVisible, prAnimActive;
	float prScaleFactor, prRotation, prOffsetX;
	bool prInPhase1, prInPhase2;

	bool fuVisible, fuAnimActive;
	float fuScaleFactor, fuRotation, fuOffsetX;
	float fuTargetRotation;
	bool fuInPhase1, fuInPhase2;

	bool reduciendoEscala;
	bool bajandoY;
	bool escala5Set;



	InstanciaAnimacion(glm::vec3 pos, bool estatica = false)
	{
		animacionEstatica = estatica;
		posicion = pos;
		escala = glm::vec3(2.0f);
		rotacionBase = 90.0f;
		escalaTemporal = 4.0f;
		translationY = estatica ? pos.y : globalTranslationYInicial;

		visible = true;

		gaVisible = true; gaExplosionActive = true; gaInflating = true; gaContracting = false;
		gaExplosionFactor = 1.0f;

		ganVisible = false; ganAppearing = false;
		ganScaleFactor = 1.0f; ganRotation = 0.0f;

		ganFinished = false;

		gVisible = false; gAppearing = false; gAnimActive = false;
		gScaleFactor = 0.0f; gRotation = 0.0f; gOffsetX = -0.5f;
		gTargetRotation = 180.0f;
		gInPhase1 = false; gInPhase2 = false;

		grVisible = false; grAnimActive = false;
		grScaleFactor = 0.0f; grRotation = 0.0f; grOffsetX = -0.5f;
		grTargetRotation = 360.0f;
		grInPhase1 = false; grInPhase2 = false;

		ramVisible = false; ramAnimActive = false;
		ramScaleFactor = 0.0f; ramRotation = 0.0f; ramOffsetX = -0.5f;
		ramTargetRotation = 360.0f;
		ramInPhase1 = false; ramInPhase2 = false;

		prVisible = false; prAnimActive = false;
		prScaleFactor = 0.0f; prRotation = 0.0f; prOffsetX = -0.5f;
		prInPhase1 = false; prInPhase2 = false;

		fuVisible = false; fuAnimActive = false;
		fuScaleFactor = 0.0f; fuRotation = 0.0f; fuOffsetX = -0.5f;
		fuTargetRotation = 180.0f;
		fuInPhase1 = false; fuInPhase2 = false;

		reduciendoEscala = false;
		bajandoY = false;
		escala5Set = false;

	}
};



struct AnimacionXGroup {
	glm::vec3 globalTranslationX;
	glm::vec3 globalScaleX;
	float globalRotationX;


	bool ganVisible = true;
	bool ganAppearing = false;
	float ganRotation = 0.0f;
	float ganTargetRotation = 270.0f;
	float ganScaleFactor = 0.0f;
	bool ganFinished = false;

	bool gVisible = false;
	bool gAppearing = false;
	bool gAnimActive = false;
	bool gInPhase1 = false;
	bool gInPhase2 = false;
	float gOffsetX = -0.5f;
	float gScaleFactor = 0.0f;
	float gRotation = 0.0f;
	float gTargetRotation = 180.0f;

	bool grVisible = false;
	bool grAppearing = false;
	bool grAnimActive = false;
	bool grInPhase1 = false;
	bool grInPhase2 = false;
	float grOffsetX = -0.5f;
	float grScaleFactor = 0.0f;
	float grRotation = 0.0f;
	float grTargetRotation = 360.0f;

	bool ramVisible = false;
	bool ramAnimActive = false;
	bool ramInPhase1 = false;
	bool ramInPhase2 = false;
	float ramOffsetX = -0.5f;
	float ramScaleFactor = 0.0f;
	float ramRotation = 0.0f;
	float ramTargetRotation = 360.0f;

	bool prVisible = false;
	bool prAnimActive = false;
	bool prInPhase1 = false;
	bool prInPhase2 = false;
	float prOffsetX = -0.5f;
	float prScaleFactor = 0.0f;
	float prRotation = 0.0f;

	bool fuVisible = false;
	bool fuAnimActive = false;
	bool fuInPhase1 = false;
	bool fuInPhase2 = false;
	float fuOffsetX = -0.5f;
	float fuScaleFactor = 0.0f;
	float fuRotation = 0.0f;
	float fuTargetRotation = 180.0f;

	bool animacionXCompleta = false;
};

struct Keyframe {
	float time;  // Tiempo relativo desde el inicio
	glm::vec3 position;
	float scale;
};

struct SillaKeyframeAnimation {
	std::vector<Keyframe> keyframes;
	float currentTime = 0.0f;
	bool active = false;
	int currentIndex = 0;

	glm::vec3 interpolatedPosition;
	float interpolatedScale;

	void start() {
		currentTime = 0.0f;
		currentIndex = 0;
		active = true;
	}

	void update(float deltaTime) {
		if (!active || keyframes.size() < 2) return;

		currentTime += deltaTime;

		while (currentIndex < keyframes.size() - 1 &&
			currentTime > keyframes[currentIndex + 1].time) {
			currentIndex++;
		}

		if (currentIndex >= keyframes.size() - 1) {
			active = false;
			return;
		}

		Keyframe& kf1 = keyframes[currentIndex];
		Keyframe& kf2 = keyframes[currentIndex + 1];

		float localTime = currentTime - kf1.time;
		float duration = kf2.time - kf1.time;
		float t = localTime / duration;

		interpolatedPosition = glm::mix(kf1.position, kf2.position, t);
		interpolatedScale = glm::mix(kf1.scale, kf2.scale, t);
	}
};

struct SillaAnimada {
	glm::vec3 siPos;
	glm::vec3 snPos;
	glm::vec3 targetPos;
	glm::vec3 siPosOriginal; // NUEVA
	bool siExplotaComoModel1 = false;
	float siExplosionFactor = 0.0f;

	bool siVisible;
	bool siMoving;
	bool siShrinking;
	bool snVisible;
	bool snAppearing;
	bool snReturning;
	float siScale;
	float snScale;
	bool snRegresoTerminado;
	SillaKeyframeAnimation animacion;


	enum class FaseAnimSilla {
		EscalandoAntesDeMover,
		SubirAntesDeMover,
		MoverASalida,
		DesaparecerSi,
		AparecerSn,
		SnMoverAOriginal,
		SnEscalarOriginal,
		SnBajarFinal,
		Completa

		

		
	};

	FaseAnimSilla fase = FaseAnimSilla::Completa;



	struct Keyframe {
		float time;  // Tiempo relativo desde el inicio
		glm::vec3 position;
		float scale;
	};

	struct SillaKeyframeAnimation {
		std::vector<Keyframe> keyframes;
		float currentTime = 0.0f;
		bool active = false;
		int currentIndex = 0;

		glm::vec3 interpolatedPosition;
		float interpolatedScale;

		void start() {
			currentTime = 0.0f;
			currentIndex = 0;
			active = true;
		}

		void update(float deltaTime) {
			if (!active || keyframes.size() < 2) return;

			currentTime += deltaTime;

			while (currentIndex < keyframes.size() - 1 &&
				currentTime > keyframes[currentIndex + 1].time) {
				currentIndex++;
			}

			if (currentIndex >= keyframes.size() - 1) {
				active = false;
				return;
			}

			Keyframe& kf1 = keyframes[currentIndex];
			Keyframe& kf2 = keyframes[currentIndex + 1];

			float localTime = currentTime - kf1.time;
			float duration = kf2.time - kf1.time;
			float t = localTime / duration;

			interpolatedPosition = glm::mix(kf1.position, kf2.position, t);
			interpolatedScale = glm::mix(kf1.scale, kf2.scale, t);
		}
	};
	SillaAnimada(glm::vec3 start)
	{

		siVisible = true;
		siMoving = false;
		siShrinking = false;
		siScale = 4.0f;

		snPos = glm::vec3(11.0f, -1.0f, -10.0f);
		snScale = 0.0f;
		snAppearing = false;
		snReturning = false;
		snVisible = false;
		siPos = start;
		siPosOriginal = start; // GUARDAMOS

		targetPos = glm::vec3(11.0f, siPosOriginal.y + 4.0f, -20.0f);

		snRegresoTerminado = false;
		


	}
};

//////////////// VARIABLES GLOBALES PARA ACTUALIZACION GLOBAL/////////////////////////

bool piVisible = true;
bool nVisible = false;
bool piMinimizando = false;
float piScaleFactor = 1.0f;
bool nApareciendo = false;
float nScaleFactor = 0.0f;

bool sillaEspecial1Minimizando = false;
bool sillaEspecial2Minimizando = false;
float escalaSillaEspecial1 = 4.0f;
float escalaSillaEspecial2 = 4.0f;
bool sillaEspecial1Visible = true;
bool sillaEspecial2Visible = true;


/////////////////// MESAS //////////////////////

std::vector<glm::vec3> posicionesMe = {
	// Ventanas
	{4.0f, -2.0f, -56.0f},
	{4.0f, -2.0f, -49.0f},
	{-3.0f, -2.0f, -56.0f},
	{-3.0f, -2.0f, -49.0f},
	{-10.0f, -2.0f, -56.0f},
	{-10.0f, -2.0f, -49.0f},
	// Puertas
	{4.0f, -2.0f, -25.0f},
	{4.0f, -2.0f, -32.0f},
	{-3.0f, -2.0f, -25.0f},
	{-3.0f, -2.0f, -32.0f},
	{-10.0f, -2.0f, -25.0f},
	{-10.0f, -2.0f, -32.0f}
};



/////--------------TECLADOS----------/////////////
std::vector<glm::vec3> posicionesTe = {
	{4.8f, -0.5f, -23.5f}, {4.8f, -0.5f, -28.0f}, {4.8f, -0.5f, -32.5f},
	{-2.2f, -0.5f, -23.5f}, {-2.2f, -0.5f, -28.0f}, {-2.2f, -0.5f, -32.5f},
	{-9.2f, -0.5f, -23.5f}, {-9.2f, -0.5f, -28.0f}, {-9.2f, -0.5f, -32.5f},
	{5.0f, -0.5f, -58.0f}, {4.8f, -0.5f, -54.5f}, {4.8f, -0.5f, -50.5f},
	{5.0f, -0.5f, -47.0f}, {-2.0f, -0.5f, -58.0f}, {-2.2f, -0.5f, -54.5f},
	{-2.2f, -0.5f, -50.5f}, {-2.0f, -0.5f, -47.0f}, {-9.0f, -0.5f, -58.0f},
	{-9.2f, -0.5f, -54.5f}, {-9.2f, -0.5f, -50.5f}, {-9.0f, -0.5f, -47.0f}
};

///----------PANTALLAS----------/////////

std::vector<glm::vec3> posicionesPantallas = {
	{  3.5f, 0.45f, -23.5f },
	{  3.5f, 0.45f, -28.0f },
	{  3.5f, 0.45f, -32.5f },
	{ -3.5f, 0.45f, -32.5f },
	{ -3.5f, 0.45f, -28.0f },
	{ -3.5f, 0.45f, -23.5f },
	{ -10.5f, 0.45f, -23.5f },
	{ -10.5f, 0.45f, -28.0f },
	{ -10.5f, 0.45f, -32.5f },
	{  3.5f, 0.45f, -58.5f },
	{  3.5f, 0.45f, -54.5f },
	{  3.5f, 0.45f, -50.5f },
	{  3.5f, 0.45f, -46.5f },
	{ -3.5f, 0.45f, -58.5f },
	{ -3.5f, 0.45f, -54.5f },
	{ -3.5f, 0.45f, -50.5f },
	{ -3.5f, 0.45f, -46.5f },
	{ -10.5f, 0.45f, -58.5f },
	{ -10.5f, 0.45f, -54.5f },
	{ -10.5f, 0.45f, -50.5f },
	{ -10.5f, 0.45f, -46.5f }
};


//////////////////MESAS NUVEAS POR VIEWJAS/////////////

bool mesasViejasVisibles = true;
bool mesasViejasReduciendo = false; // << nueva variable para controlar la desaparición
bool mesasNuevasVisibles = false;
bool mesasAnimandoAparecer = false;
float mesasScaleFactorViejas = 1.0f; // << la escala de las viejas
float mesasScaleFactorNuevas = 0.0f; // << la escala de las nuevas
float mesasScaleFactor = 0.0f;

////////////////----------TECLADOS------------//////////////////////

bool teVisible = true;
bool tnVisible = false;
bool teMinimizando = false;
bool tnApareciendo = false;
float teScaleFactor = 1.0f;
float tnScaleFactor = 0.0f;


//////-------------DESAAPRECER EXTRAS------------////////////

bool modeloPvVisible = true;
bool modeloMessVisible = true;
float escalaPv = 1.0f;
float escalaMess = 1.0f;
bool minimizandoConT = false;
bool modeloMMVisible = true;
float escalaMM = 1.0f;
bool minimizandoMM = false;
bool modeloSalonVisible = true;
bool modeloSalonNuevoVisible = false;

///////////--------------BANDERA PARA LUZ PUNTUAL---------------/////////////

bool pointLightActive = true;
bool lucesXneg10 = true;  // U
bool lucesX0 = true;      // I
bool lucesX10 = true;     // O

/////------CORTINAS-------////////////
bool cortinasViejasVisibles = true;
bool cortinasNuevasVisibles = false;
bool rackVisible = true;
bool modemVisible = false;
bool aireViejoVisible = true;
bool aireNuevoVisible = false;
bool proyectorVisible = true;





std::vector<SillaAnimada> sillas;

bool hayColision(glm::vec3 nuevaPos, const SillaAnimada& actual)
{
	for (const auto& silla : sillas)
	{
		if (&silla == &actual) continue; // Ignora la propia silla

		if (silla.siVisible && glm::distance(glm::vec2(silla.siPos.x, silla.siPos.z), glm::vec2(nuevaPos.x, nuevaPos.z)) < 2.0f)
			return true;

		if (silla.snVisible && glm::distance(glm::vec2(silla.snPos.x, silla.snPos.z), glm::vec2(nuevaPos.x, nuevaPos.z)) < 2.0f)
			return true;
	}
	return false;
}

std::vector<InstanciaAnimacion> animaciones;

////////////POSICIONES DE LUCES
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 8.3f, -55.0f),
	glm::vec3(-10.0f, 8.3f, -55.0f),
	glm::vec3(10.0f,8.3f,  -55.0f),
	glm::vec3(-10.0f,8.3f, -30.0f),
	glm::vec3(0.0f,8.3f, -30.0f),
	glm::vec3(10.0f,8.3f, -30.0f)
};

glm::vec3 ganTranslation(0.0f, 0.0f, 0.0f);  // Traslación general de GAN


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0;
bool AnimBall = false;






// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Laboratorio Computacion Grafica Equipo 11", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

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

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

	
	//models
	Model Com((char*)"Models/Com/1.obj");
	Model Com2((char*)"Models/Comp2/2.obj");
	Model me((char*)"Models/Mes/me.obj");
	Model ga((char*)"Models/Ga/ga.obj");
	Model gan((char*)"Models/GabN/gan.obj");
	Model g((char*)"Models/Graf/g.obj");
	Model gr((char*)"Models/gr/gr.obj");
	Model ram((char*)"Models/ram/ram.obj");
	Model pr((char*)"Models/pr/pr.obj");
	Model fu((char*)"Models/fu/fu.obj");
	Model si((char*)"Models/silla/si.obj");
	Model sn((char*)"Models/sn/sn.obj");
	Model sal((char*)"Models/salon/salon2.obj");
	Model PC((char*)"Models/PC/PC.obj");
	Model pi((char*)"Models/pi/pi.obj");
	Model ai((char*)"Models/ai/ai.obj");
	Model bt((char*)"Models/bt/bt.obj");
	Model l1((char*)"Models/l1/l1.obj");
	Model l2((char*)"Models/l2/l2.obj");
	Model l3((char*)"Models/l3/l3.obj");
	Model l4((char*)"Models/l4/l4.obj");
	Model l5((char*)"Models/l5/l5.obj");
	Model l6((char*)"Models/l6/l6.obj");
	Model te((char*)"Models/te/te.obj");
	Model rack((char*)"Models/rack/rack.obj");
	Model l7((char*)"Models/l7/l7.obj");
	Model pv((char*)"Models/pv/pv.obj");
	Model mess((char*)"Models/mess/mess.obj");
	Model co((char*)"Models/co/co.obj");
	Model ex((char*)"Models/ex/ex.obj");
	Model r((char*)"Models/r/R.obj");
	Model N((char*)"Models/piN/N.obj");
	Model mN((char*)"Models/mN/mN.obj");
	Model tN((char*)"Models/tN/tN.obj");
	Model MM((char*)"Models/MM/MM.obj");
	Model SAN((char*)"Models/SAN/SAN.obj");
	Model LAM((char*)"Models/LAM/LAM.obj");
	Model co2((char*)"Models/co2/co2.obj");
	Model MO((char*)"Models/MO/MO.obj");
	Model I((char*)"Models/I/I.obj");



	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};









	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Skybox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


	//Load texture
	vector < const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");


	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);




	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	
	//////////////----------------DIBUJOS DE SILLAS Y PC'S---------------------////////////////
	
	// Agregar instancia inicial con GA visible
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -25.8f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -30.3f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -34.8f));

	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -25.8f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -30.3f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -34.8f));

	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -25.8f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -30.3f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -34.8f));


	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -48.5f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -56.5f));
	animaciones.emplace_back(glm::vec3(3.5f, 0.5f, -52.5f));

	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -48.5f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -56.5f));
	animaciones.emplace_back(glm::vec3(-3.5f, 0.5f, -52.5f));

	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -48.5f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -56.5f));
	animaciones.emplace_back(glm::vec3(-10.5f, 0.5f, -52.5f));

	// Nuevas instancias sin cambio de posición o escala (solo animación en el lugar)
	animaciones.emplace_back(glm::vec3(4.0f, -2.3f, -59.0f), true);
	animaciones.emplace_back(glm::vec3(-3.0f, -2.3f, -59.0f), true);
	animaciones.emplace_back(glm::vec3(-10.0f, -2.3f, -59.0f), true);

	sillas.emplace_back(glm::vec3(7.0f, -1.5f, -28.0f));
	sillas.emplace_back(glm::vec3(7.0f, -1.5f, -23.0f));
	sillas.emplace_back(glm::vec3(7.0f, -1.5f, -33.0f));

	sillas.emplace_back(glm::vec3(0.0f, -1.5f, -28.0f));
	sillas.emplace_back(glm::vec3(0.0f, -1.5f, -23.0f));
	sillas.emplace_back(glm::vec3(0.0f, -1.5f, -33.0f));

	sillas.emplace_back(glm::vec3(-6.2f, -1.5f, -28.0f));
	sillas.emplace_back(glm::vec3(-7.5f, -1.5f, -23.0f));
	sillas.emplace_back(glm::vec3(-7.5f, -1.5f, -33.0f));

	sillas.emplace_back(glm::vec3(7.0f, -1.5f, -58.5f));
	sillas.emplace_back(glm::vec3(7.0f, -1.5f, -54.5f));
	sillas.emplace_back(glm::vec3(7.0f, -1.5f, -50.5f));
	sillas.emplace_back(glm::vec3(7.0f, -1.5f, -46.5f));

	sillas.emplace_back(glm::vec3(0.0f, -1.5f, -58.5f));
	sillas.emplace_back(glm::vec3(0.0f, -1.5f, -54.5f));
	sillas.emplace_back(glm::vec3(0.0f, -1.5f, -50.5f));
	sillas.emplace_back(glm::vec3(0.0f, -1.5f, -46.5f));

	sillas.emplace_back(glm::vec3(-7.5f, -1.5f, -58.5f));
	sillas.emplace_back(glm::vec3(-6.5f, -1.5f, -54.5f));
	sillas.emplace_back(glm::vec3(-7.5f, -1.5f, -50.5f));
	sillas.emplace_back(glm::vec3(-6.5f, -1.5f, -46.5f));



	

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
	// Luz direccional atenuada
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.1f);



		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		
		if (pointLightActive) {
			for (int i = 0; i < 6; ++i) {
				glm::vec3 pos = pointLightPositions[i];
				bool activo = false;

				if (pos.x == -10.0f) activo = lucesXneg10;
				else if (pos.x == 0.0f) activo = lucesX0;
				else if (pos.x == 10.0f) activo = lucesX10;

				std::string prefix = "pointLights[" + std::to_string(i) + "].";

				glUniform3f(glGetUniformLocation(lightingShader.Program, (prefix + "position").c_str()), pos.x, pos.y, pos.z);

				if (activo) {
					glUniform3f(glGetUniformLocation(lightingShader.Program, (prefix + "ambient").c_str()), 0.13f, 0.13f, 0.13f);
					glUniform3f(glGetUniformLocation(lightingShader.Program, (prefix + "diffuse").c_str()), 0.26f, 0.26f, 0.26f);
					glUniform3f(glGetUniformLocation(lightingShader.Program, (prefix + "specular").c_str()), 0.39f, 0.39f, 0.39f);

				}
				else {
					glUniform3f(glGetUniformLocation(lightingShader.Program, (prefix + "ambient").c_str()), 0.0f, 0.0f, 0.0f);
					glUniform3f(glGetUniformLocation(lightingShader.Program, (prefix + "diffuse").c_str()), 0.0f, 0.0f, 0.0f);
					glUniform3f(glGetUniformLocation(lightingShader.Program, (prefix + "specular").c_str()), 0.0f, 0.0f, 0.0f);
				}

				glUniform1f(glGetUniformLocation(lightingShader.Program, (prefix + "constant").c_str()), 1.0f);
				glUniform1f(glGetUniformLocation(lightingShader.Program, (prefix + "linear").c_str()), 0.014f);
				glUniform1f(glGetUniformLocation(lightingShader.Program, (prefix + "quadratic").c_str()), 0.0007f);
			}

		

		}
		




		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Ahora sí la mandas correctamente
		glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), explosionFactor);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


		/////////--------------DIBUJO DE MODELOS--------------//////////////////


		/////////--------------------PANTALLAS----------//////////////

		for (const auto& pos : posicionesPantallas)
		{
			glm::mat4 groupTransform = glm::mat4(1.0f);
			groupTransform = glm::translate(groupTransform, pos);
			groupTransform = glm::scale(groupTransform, glm::vec3(2.5f));
			groupTransform = glm::rotate(groupTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			// MODEL 1 - Explosion Animation
			if (model1Visible && explosionFactor < 1.7f)
			{
				glm::mat4 explodedModel = glm::mat4(1.0f);

				float explosionScale = 1.0f - explosionFactor / 1.7f;
				float baseOffset = 0.8f;
				float verticalCompensate = baseOffset * (1.0f - explosionScale);

				explodedModel = glm::translate(explodedModel, glm::vec3(0.0f, verticalCompensate, 0.0f));
				explodedModel = glm::scale(explodedModel, glm::vec3(explosionScale));
				explodedModel = glm::rotate(explodedModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				glm::mat4 finalModel1 = groupTransform * explodedModel;
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel1));
				Com.Draw(lightingShader);
			}

			// MODEL 2 - Aparición animada
			if (model2Visible)
			{
				glm::mat4 model2 = glm::mat4(1.0f);
				model2 = glm::rotate(model2, glm::radians(model2Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
				model2 = glm::scale(model2, glm::vec3(model2ScaleFactor));

				glm::mat4 finalModel2 = groupTransform * model2;
				glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
				glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel2));
				Com2.Draw(lightingShader);
			}
		}

		// ------------------ PIZARRON ------------------//////////
		if (piVisible)
		{
			glm::mat4 modelPi = glm::mat4(1.0f);
			modelPi = glm::translate(modelPi, glm::vec3(-18.0f, 3.5f, -40.0f));
			modelPi = glm::rotate(modelPi, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelPi = glm::scale(modelPi, glm::vec3(10.5f, 8.0f, 10.5f) * piScaleFactor);

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPi));

			pi.Draw(lightingShader);
		}


		// ------------------ PIZARRON NUEVA ------------------/////
		if (nVisible)
		{
			glm::mat4 modelN = glm::mat4(1.0f);
			modelN = glm::translate(modelN, glm::vec3(-18.0f, 3.5f, -40.0f));
			modelN = glm::rotate(modelN, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelN = glm::scale(modelN, glm::vec3(5.5f, 15.0f, 17.5f) * nScaleFactor); // escala animada

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelN));

			N.Draw(lightingShader);
		}




		// ------------------ AIRE VIEJO ------------------
		if (aireViejoVisible)
		{
			glm::mat4 modelaI = glm::mat4(1.0f);
			modelaI = glm::translate(modelaI, glm::vec3(-3.5f, 6.5f, -60.0f));
			modelaI = glm::rotate(modelaI, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelaI = glm::scale(modelaI, glm::vec3(5.0f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelaI));

			ai.Draw(lightingShader);
		}

		// ------------------ AIRE NUEVO ------------------
		if (aireNuevoVisible)
		{
			glm::mat4 modelaIN = glm::mat4(1.0f);
			modelaIN = glm::translate(modelaIN, glm::vec3(-3.5f, 6.5f, -60.0f));
			modelaIN = glm::rotate(modelaIN, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelaIN = glm::scale(modelaIN, glm::vec3(5.0f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelaIN));

			I.Draw(lightingShader);
		}


		// ------------------ BOTIQUIN ------------------
		{
			glm::mat4 modelbt = glm::mat4(1.0f);  // Matriz identidad
			modelbt = glm::translate(modelbt, glm::vec3(-3.0f, 3.5f, -60.0f)); // Posición del modelo
			modelbt = glm::rotate(modelbt, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modelbt = glm::scale(modelbt, glm::vec3(2.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelbt));

			bt.Draw(lightingShader);  // Dibuja el modelo
		}

		// ------------------ l1 ------------------
		{
			glm::mat4 modell1 = glm::mat4(1.0f);  // Matriz identidad
			modell1 = glm::translate(modell1, glm::vec3(-2.0f, 5.0f, -60.5f)); // Posición del modelo
			modell1 = glm::rotate(modell1, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modell1 = glm::scale(modell1, glm::vec3(1.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modell1));

			l1.Draw(lightingShader);  // Dibuja el modelo
		}

		// ------------------ 12 ------------------
		{
			glm::mat4 modell2 = glm::mat4(1.0f);  // Matriz identidad
			modell2 = glm::translate(modell2, glm::vec3(-3.5f, 5.0f, -60.5f)); // Posición del modelo
			modell2 = glm::rotate(modell2, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modell2 = glm::scale(modell2, glm::vec3(1.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modell2));

			l2.Draw(lightingShader);  // Dibuja el modelo
		}
		// ------------------ 13 ------------------
		{
			glm::mat4 modell3 = glm::mat4(1.0f);  // Matriz identidad
			modell3 = glm::translate(modell3, glm::vec3(-18.0f, 7.0f, -37.0)); // Posición del modelo
			modell3 = glm::rotate(modell3, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modell3 = glm::scale(modell3, glm::vec3(1.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modell3));

			l3.Draw(lightingShader);  // Dibuja el modelo
		}

		// ------------------ 14 ------------------
		{
			glm::mat4 modell4 = glm::mat4(1.0f);  // Matriz identidad
			modell4 = glm::translate(modell4, glm::vec3(-18.0f, 7.0f, -38.0)); // Posición del modelo
			modell4 = glm::rotate(modell4, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modell4 = glm::scale(modell4, glm::vec3(1.5f, 1.0f, 1.5f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modell4));

			l4.Draw(lightingShader);  // Dibuja el modelo
		}

		// ------------------ 15 ------------------
		{
			glm::mat4 modell5 = glm::mat4(1.0f);  // Matriz identidad
			modell5 = glm::translate(modell5, glm::vec3(-18.0f, 7.0f, -43.0)); // Posición del modelo
			modell5 = glm::rotate(modell5, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modell5 = glm::scale(modell5, glm::vec3(2.0f, 1.5f, 2.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modell5));

			l5.Draw(lightingShader);  // Dibuja el modelo
		}

		// ------------------ 16 ------------------
		{
			glm::mat4 modell6 = glm::mat4(1.0f);  // Matriz identidad
			modell6 = glm::translate(modell6, glm::vec3(-18.3f, 3.5f, -29.5)); // Posición del modelo
			modell6 = glm::rotate(modell6, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modell6 = glm::scale(modell6, glm::vec3(3.0f, 2.5f, 3.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modell6));

			l6.Draw(lightingShader);  // Dibuja el modelo
		}

		/////////////-------------------TECLADOS--------------////////////////////////

		for (const auto& pos : posicionesTe)
		{
			if (teVisible)
			{
				glm::mat4 modelTe = glm::mat4(1.0f);
				modelTe = glm::translate(modelTe, pos);
				modelTe = glm::rotate(modelTe, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				modelTe = glm::scale(modelTe, glm::vec3(0.7f, 3.0f, 3.0f) * teScaleFactor);

				glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
				glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTe));

				te.Draw(lightingShader);
			}

			if (tnVisible)
			{
				glm::mat4 modelTN = glm::mat4(1.0f);
				modelTN = glm::translate(modelTN, pos);
				modelTN = glm::rotate(modelTN, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				modelTN = glm::scale(modelTN, glm::vec3(2.0f, 1.0f, 3.0f) * tnScaleFactor);

				glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
				glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTN));

				tN.Draw(lightingShader);
			}
		}


		/////-----------SILLAS-------------//////////

		for (const auto& silla : sillas) {
			if (silla.siVisible) {
				glm::mat4 modelSi = glm::mat4(1.0f);
				modelSi = glm::translate(modelSi, silla.siPos);
				modelSi = glm::rotate(modelSi, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelSi = glm::scale(modelSi, glm::vec3(silla.siScale));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSi));
				glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), silla.siExplosionFactor);
				si.Draw(lightingShader);
			}


			if (silla.snVisible) {
				glm::mat4 modelSn = glm::mat4(1.0f);
				modelSn = glm::translate(modelSn, silla.snPos); // <--- esta línea DEBE usar snPos
				modelSn = glm::rotate(modelSn, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelSn = glm::scale(modelSn, glm::vec3(silla.snScale));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSn));
				sn.Draw(lightingShader);
			}
		}


		//////----------PC'S-------------////////////////


		for (const auto& instancia : animaciones)
		{
			glm::vec3 posicionY = instancia.animacionEstatica ?
				instancia.posicion :  // usa directamente la Y del vector `posicion`
				glm::vec3(instancia.posicion.x, instancia.translationY, instancia.posicion.z);



			// ------------------ GA ------------------
			if (instancia.gaVisible)
			{
				glm::mat4 explodedGA = glm::mat4(1.0f);

				glm::vec3 gaPos = instancia.animacionEstatica
					? instancia.posicion
					: glm::vec3(instancia.posicion.x, 0.5f, instancia.posicion.z);

				explodedGA = glm::translate(explodedGA, gaPos);
				explodedGA = glm::scale(explodedGA, glm::vec3(
					2.0f * instancia.gaExplosionFactor,
					2.0f * instancia.gaExplosionFactor,
					2.0f * instancia.gaExplosionFactor
				));
				explodedGA = glm::rotate(explodedGA, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
				glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explodedGA));
				ga.Draw(lightingShader);
			}


			// ------------------ GAN ------------------
			if (instancia.ganVisible)
			{
				glm::mat4 modelGan = glm::mat4(1.0f);
				modelGan = glm::translate(modelGan, posicionY);
				modelGan = glm::rotate(modelGan, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
				modelGan = glm::scale(modelGan, instancia.escala);
				modelGan = glm::rotate(modelGan, glm::radians(instancia.ganRotation), glm::vec3(0.0f, 1.0f, 0.0f));
				modelGan = glm::scale(modelGan, glm::vec3(instancia.ganScaleFactor));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGan));
				gan.Draw(lightingShader);
			}

			// ------------------ G ------------------
			if (instancia.gVisible)
			{
				glm::mat4 modelG = glm::mat4(1.0f);
				modelG = glm::translate(modelG, posicionY);
				modelG = glm::rotate(modelG, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
				modelG = glm::scale(modelG, instancia.escala);
				modelG = glm::translate(modelG, ganTranslation + glm::vec3(instancia.gOffsetX, 0.0f, 0.0f));
				modelG = glm::rotate(modelG, glm::radians(instancia.gRotation), glm::vec3(1.0f, 0.0f, 0.0f));
				modelG = glm::rotate(modelG, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				modelG = glm::scale(modelG, glm::vec3(instancia.gScaleFactor * 0.7f));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelG));
				g.Draw(lightingShader);
			}

			// ------------------ GR ------------------
			if (instancia.grVisible)
			{
				glm::mat4 modelGR = glm::mat4(1.0f);
				modelGR = glm::translate(modelGR, posicionY);
				modelGR = glm::rotate(modelGR, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
				modelGR = glm::scale(modelGR, instancia.escala);
				modelGR = glm::translate(modelGR, ganTranslation + glm::vec3(instancia.grOffsetX - 0.05f, 0.0f, -0.15f));
				modelGR = glm::rotate(modelGR, glm::radians(instancia.grRotation), glm::vec3(0.0f, 1.0f, 0.0f));
				modelGR = glm::rotate(modelGR, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelGR = glm::rotate(modelGR, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				modelGR = glm::rotate(modelGR, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelGR = glm::scale(modelGR, glm::vec3(0.5f * instancia.grScaleFactor));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGR));
				gr.Draw(lightingShader);
			}

			// ------------------ RAM ------------------
			if (instancia.ramVisible)
			{
				glm::mat4 modelRAM = glm::mat4(1.0f);
				modelRAM = glm::translate(modelRAM, posicionY);
				modelRAM = glm::rotate(modelRAM, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
				modelRAM = glm::scale(modelRAM, instancia.escala);
				modelRAM = glm::translate(modelRAM, ganTranslation + glm::vec3(instancia.ramOffsetX - 0.02f, -0.25f, -0.06f));
				modelRAM = glm::rotate(modelRAM, glm::radians(instancia.ramRotation), glm::vec3(0.0f, 1.0f, 0.0f));
				modelRAM = glm::rotate(modelRAM, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelRAM = glm::rotate(modelRAM, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelRAM = glm::scale(modelRAM, glm::vec3(0.3f * instancia.ramScaleFactor));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRAM));
				ram.Draw(lightingShader);
			}

			// ------------------ PR ------------------
			if (instancia.prVisible)
			{
				glm::mat4 modelPR = glm::mat4(1.0f);
				modelPR = glm::translate(modelPR, posicionY);
				modelPR = glm::rotate(modelPR, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
				modelPR = glm::scale(modelPR, instancia.escala);
				modelPR = glm::translate(modelPR, ganTranslation + glm::vec3(instancia.prOffsetX + 0.012f, -0.19f, -0.18f));
				modelPR = glm::rotate(modelPR, glm::radians(instancia.prRotation), glm::vec3(0.0f, 1.0f, 0.0f));
				modelPR = glm::rotate(modelPR, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelPR = glm::rotate(modelPR, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelPR = glm::scale(modelPR, glm::vec3(0.07f * instancia.prScaleFactor));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPR));
				pr.Draw(lightingShader);
			}

			// ------------------ FU ------------------
			if (instancia.fuVisible)
			{
				glm::mat4 modelFU = glm::mat4(1.0f);
				modelFU = glm::translate(modelFU, posicionY);
				modelFU = glm::rotate(modelFU, glm::radians(globalRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
				modelFU = glm::scale(modelFU, instancia.escala);
				modelFU = glm::translate(modelFU, ganTranslation + glm::vec3(instancia.fuOffsetX - 0.2f, -0.26f, -0.3f));
				modelFU = glm::rotate(modelFU, glm::radians(instancia.fuRotation), glm::vec3(0.0f, 1.0f, 0.0f));
				modelFU = glm::rotate(modelFU, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelFU = glm::rotate(modelFU, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelFU = glm::rotate(modelFU, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				modelFU = glm::scale(modelFU, glm::vec3(0.35f * instancia.fuScaleFactor));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFU));
				fu.Draw(lightingShader);
			}
		}


		// ------------------ rack ------------------
		if (rackVisible)
		{
			glm::mat4 modelr = glm::mat4(1.0f);  // Matriz identidad
			modelr = glm::translate(modelr, glm::vec3(12.0f, 0.5f, -40.0f)); // Posición del modelo
			modelr = glm::rotate(modelr, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre Y
			modelr = glm::scale(modelr, glm::vec3(9.0f));  // Escala

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelr));

			rack.Draw(lightingShader);
		}


		// ------------------ MODEM ------------------
		if (modemVisible)
		{
			glm::mat4 modelMO = glm::mat4(1.0f);
			modelMO = glm::translate(modelMO, glm::vec3(12.7f, 7.4f, -40.0));
			modelMO = glm::rotate(modelMO, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMO = glm::rotate(modelMO, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			modelMO = glm::scale(modelMO, glm::vec3(2.0f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMO));

			MO.Draw(lightingShader);
		}



		// ------------------ 17 ------------------
		{
			glm::mat4 modell7 = glm::mat4(1.0f);  // Matriz identidad
			modell7 = glm::translate(modell7, glm::vec3(13.3f, 3.5f, -49.5)); // Posición del modelo
			modell7 = glm::rotate(modell7, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modell7 = glm::scale(modell7, glm::vec3(0.5f, 4.5f, 5.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modell7));

			l7.Draw(lightingShader);  // Dibuja el modelo
		}


		// ------------------ ESCRITORIO DE METAL ------------------
		if (modeloPvVisible)
		{
			glm::mat4 modellv = glm::mat4(1.0f);
			modellv = glm::translate(modellv, glm::vec3(-16.4f, -1.9f, -24.5f));
			modellv = glm::rotate(modellv, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modellv = glm::scale(modellv, glm::vec3(7.5f * escalaPv));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modellv));

			pv.Draw(lightingShader);
		}


		// ------------------ MESA ESQUINADA ------------------
		if (modeloMessVisible)
		{
			glm::mat4 modellt = glm::mat4(1.0f);
			modellt = glm::translate(modellt, glm::vec3(-16.3f, -2.2f, -54.5f));
			modellt = glm::rotate(modellt, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modellt = glm::scale(modellt, glm::vec3(8.0f * escalaMess, 6.0f * escalaMess, 8.0f * escalaMess));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modellt));

			mess.Draw(lightingShader);
		}





		// ------------------ EXTINTOR ------------------
		{
			glm::mat4 modelco4 = glm::mat4(1.0f);  // Matriz identidad
			modelco4 = glm::translate(modelco4, glm::vec3(12.0f, 1.5f, -59.0)); // Posición del modelo
			modelco4 = glm::rotate(modelco4, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modelco4 = glm::scale(modelco4, glm::vec3(2.0f, 2.0f, 2.0f));  // Escala fija

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelco4));

			ex.Draw(lightingShader);  // Dibuja el modelo
		}

		///////-------------MESAS-------------//////////

		for (const auto& pos : posicionesMe)
		{
			glm::mat4 modelMe = glm::mat4(1.0f);
			modelMe = glm::translate(modelMe, pos);
			modelMe = glm::rotate(modelMe, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			if (mesasViejasVisibles)
				modelMe = glm::scale(modelMe, glm::vec3(7.5f));
			else
				modelMe = glm::scale(modelMe, glm::vec3(0.0f)); // no dibujes nada si no son visibles

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMe));

			if (mesasViejasVisibles)
				me.Draw(lightingShader);
		}

		for (const auto& pos : posicionesMe)
		{
			if (mesasNuevasVisibles)
			{
				glm::mat4 modelMN = glm::mat4(1.0f);
				modelMN = glm::translate(modelMN, pos);
				modelMN = glm::rotate(modelMN, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelMN = glm::scale(modelMN, glm::vec3(7.5f, 5.6f, 7.5f) * mesasScaleFactorNuevas);


				glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
				glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMN));

				mN.Draw(lightingShader);
			}
		}



		// ------------------ MESA DE MADERA ------------------
		if (modeloMMVisible)
		{
			glm::mat4 modelM = glm::mat4(1.0f);  // Matriz identidad
			modelM = glm::translate(modelM, glm::vec3(-13.0f, -2.3f, -47.0)); // Posición del modelo
			modelM = glm::rotate(modelM, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
			modelM = glm::rotate(modelM, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelM = glm::scale(modelM, glm::vec3(3.5f, 2.5f, 3.5f) * escalaMM);  // <<< AQUI ahora se escala dinámicamente

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelM));

			MM.Draw(lightingShader);  // Dibuja el modelo
		}


		// ------------------ PROYECTOR ------------------
		if (proyectorVisible)
		{
			glm::mat4 modelcor = glm::mat4(1.0f);  // Matriz identidad
			modelcor = glm::translate(modelcor, glm::vec3(-3.0f, 7.5f, -40.0));
			modelcor = glm::rotate(modelcor, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelcor = glm::rotate(modelcor, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelcor = glm::scale(modelcor, glm::vec3(2.0f, 1.0f, 2.0f));

			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelcor));

			r.Draw(lightingShader);
		}


		// ------------------ CORTINAS VIEJAS (modelos co) ------------------
		if (cortinasViejasVisibles) {
			glm::vec3 scale = glm::vec3(20.0f, 8.0f, 16.0f);



			glm::mat4 modelco = glm::mat4(1.0f);
			modelco = glm::translate(modelco, glm::vec3(5.0f, 4.4f, -60.0));
			modelco = glm::rotate(modelco, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelco = glm::scale(modelco, scale);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelco));
			co.Draw(lightingShader);

			glm::mat4 modelco1 = glm::mat4(1.0f);
			modelco1 = glm::translate(modelco1, glm::vec3(-11.0f, 4.4f, -60.0));
			modelco1 = glm::rotate(modelco1, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelco1 = glm::scale(modelco1, scale);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelco1));
			co.Draw(lightingShader);
		}


		// ------------------ CORTINAS NUEVAS (modelos co2) ------------------
		if (cortinasNuevasVisibles) {
			glm::vec3 scaleNew = glm::vec3(20.0f, 8.0f, 16.0f);


			glm::mat4 modelco2 = glm::mat4(1.0f);
			modelco2 = glm::translate(modelco2, glm::vec3(5.0f, 4.4f, -60.0));
			modelco2 = glm::rotate(modelco2, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelco2 = glm::scale(modelco2, scaleNew);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelco2));
			co2.Draw(lightingShader);

			glm::mat4 modelco3 = glm::mat4(1.0f);
			modelco3 = glm::translate(modelco3, glm::vec3(-11.0f, 4.4f, -60.0));
			modelco3 = glm::rotate(modelco3, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelco3 = glm::scale(modelco3, scaleNew);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelco3));
			co2.Draw(lightingShader);

			glm::mat4 modelco4 = glm::mat4(1.0f);
			modelco4 = glm::translate(modelco4, glm::vec3(1.0f, 4.4f, -20.0));
			modelco4 = glm::rotate(modelco4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelco4 = glm::scale(modelco4, scaleNew);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelco4));
			co2.Draw(lightingShader);

			glm::mat4 modelco5 = glm::mat4(1.0f);
			modelco5 = glm::translate(modelco5, glm::vec3(-11.0f, 4.4f, -20.0));
			modelco5 = glm::rotate(modelco5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelco5 = glm::scale(modelco5, scaleNew);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelco5));
			co2.Draw(lightingShader);
		}













	



	
	




		
		
		
		
		
		
		
		//		//DIBUJO DE SALON

		if (modeloSalonVisible)
		{
			glm::mat4 modelSalon = glm::mat4(1.0f);
			modelSalon = glm::translate(modelSalon, glm::vec3(0.0f, -4.0f, -40.0f));
			modelSalon = glm::rotate(modelSalon, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSalon = glm::scale(modelSalon, glm::vec3(1.0f));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSalon));

			sal.Draw(lightingShader);
		}
		else if (modeloSalonNuevoVisible)
		{
			glm::mat4 modelSalon = glm::mat4(1.0f);
			modelSalon = glm::translate(modelSalon, glm::vec3(0.0f, -4.0f, -40.0f));
			modelSalon = glm::rotate(modelSalon, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSalon = glm::scale(modelSalon, glm::vec3(1.0f));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSalon));

			SAN.Draw(lightingShader);
		}











		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

// Usa el shader especial para los cubos de luz
		lampShader.Use();

		// Configuración del shader de lámparas
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);  // Los cubitos usan su propio VAO

		for (int i = 0; i < 6; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Tamaño del cubito
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36); // Dibuja el cubito
		}

		// Ahora cambiamos de shader para dibujar LAM
		lightingShader.Use();

		glUniform1f(glGetUniformLocation(lightingShader.Program, "explosionFactor"), 0.0f);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		modelLoc = glGetUniformLocation(lightingShader.Program, "model");

		for (int i = 0; i < 6; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);

			// ⬇️ Aquí aplicas la rotación (ángulo en radianes)
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		

			model = glm::scale(model, glm::vec3(10.5f, 10.0f, 5.5f)); // Escalado final

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			LAM.Draw(lightingShader);
		}


		glBindVertexArray(0); // Libera el VAO





		//Draw SkyBox
		glDepthFunc(GL_LEQUAL);
		skyboxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}



































// Moves/alters the camera positions based on user input
void DoMovement()
{

	float speedMultiplier = 3.0f; // Ajusta este número como quieras

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime * speedMultiplier);

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime * speedMultiplier);

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime * speedMultiplier);

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime * speedMultiplier);

	

}





void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{


	if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		explosionActive = true;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		explosionFactor = 0.0f;
		implosionFactor = 0.0f;
		explosionActive = false;
		implosionActive = false;
		model1Visible = true;
		model2Visible = false;
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		gaExplosionActive = true;
		gaInflating = true;
		gaContracting = false;
		gaExplosionFactor = 1.0f;
		gaVisible = true;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		for (auto& instancia : animaciones)
		{
			instancia.gaContracting = true;
			instancia.gaExplosionActive = true;
		}
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		for (auto& silla : sillas)
		{
			if (!silla.animacion.active && silla.siVisible && silla.fase == SillaAnimada::FaseAnimSilla::Completa) 
				silla.fase = SillaAnimada::FaseAnimSilla::EscalandoAntesDeMover;
			{
				silla.animacion.keyframes = {
	{0.0f, silla.snPos, 1.0f},
	{1.0f, glm::vec3(silla.targetPos.x, 4.0f, silla.targetPos.z), 1.0f}
				};

				
				silla.animacion.start();

				// No cambiar visibilidad aquí. Espera a que termine la animación si.
				silla.siMoving = true;
			}
		}
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		for (auto& silla : sillas)
		{
			// Reinicia posiciones y visibilidad
			silla.siPos = silla.siPosOriginal;
			silla.siScale = 4.0f;
			silla.siVisible = true;

			silla.snPos = silla.targetPos + glm::vec3(0.0f, 4.0f, 0.0f);  // Empieza más arriba
			silla.snScale = 0.0f;
			silla.snVisible = false;

			// Reinicia estados de control
			silla.siMoving = false;
			silla.snAppearing = false;
			silla.snReturning = false;
			silla.snRegresoTerminado = false;

			// Reinicia animación
			silla.animacion.keyframes.clear();
			silla.animacion.currentTime = 0.0f;
			silla.animacion.currentIndex = 0;
			silla.animacion.active = false;

			// ← Importante: reinicia la fase
			silla.fase = SillaAnimada::FaseAnimSilla::EscalandoAntesDeMover;
		}
	}






	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		for (auto& instancia : animaciones)
		{
			// GA visible desde el inicio
			instancia.gaVisible = true;
			instancia.gaExplosionFactor = 1.0f;
			instancia.gaExplosionActive = false;
			instancia.gaInflating = false;
			instancia.gaContracting = false;

			// GAN reinicia oculto
			instancia.ganVisible = false;
			instancia.ganAppearing = false;
			instancia.ganScaleFactor = 1.0f;
			instancia.ganRotation = 0.0f;
			instancia.ganFinished = false;

			// G
			instancia.gVisible = false;
			instancia.gAppearing = false;
			instancia.gAnimActive = false;
			instancia.gScaleFactor = 0.0f;
			instancia.gRotation = 0.0f;
			instancia.gOffsetX = 0.0f;
			instancia.gInPhase1 = false;
			instancia.gInPhase2 = false;

			// GR
			instancia.grVisible = false;
			instancia.grAnimActive = false;
			instancia.grScaleFactor = 0.0f;
			instancia.grRotation = 0.0f;
			instancia.grOffsetX = 0.0f;
			instancia.grInPhase1 = false;
			instancia.grInPhase2 = false;

			// RAM
			instancia.ramVisible = false;
			instancia.ramAnimActive = false;
			instancia.ramScaleFactor = 0.0f;
			instancia.ramRotation = 0.0f;
			instancia.ramOffsetX = 0.0f;
			instancia.ramInPhase1 = false;
			instancia.ramInPhase2 = false;

			// PR
			instancia.prVisible = false;
			instancia.prAnimActive = false;
			instancia.prScaleFactor = 0.0f;
			instancia.prRotation = 0.0f;
			instancia.prOffsetX = 0.0f;
			instancia.prInPhase1 = false;
			instancia.prInPhase2 = false;

			// FU
			instancia.fuVisible = false;
			instancia.fuAnimActive = false;
			instancia.fuScaleFactor = 0.0f;
			instancia.fuRotation = 0.0f;
			instancia.fuOffsetX = 0.0f;
			instancia.fuInPhase1 = false;
			instancia.fuInPhase2 = false;

			// Reinicio final
			instancia.translationY = globalTranslationYInicial;
			instancia.escala = glm::vec3(3.0f);
			instancia.reduciendoEscala = false;
			instancia.bajandoY = false;
			instancia.escala5Set = false;
		}

		// Variables globales por si acaso
		gaAntesDeX = false;
		animacionXCompleta = false;
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		piMinimizando = true;
		mesasViejasReduciendo = true;
		teMinimizando = true;
		minimizandoConT = true;

		sillaEspecial1Minimizando = true;
		sillaEspecial2Minimizando = true;
		minimizandoMM = true;
		cortinasViejasVisibles = false;
		cortinasNuevasVisibles = true;
		rackVisible = false;
		modemVisible = true;
		aireViejoVisible = false;
		aireNuevoVisible = true;
		proyectorVisible = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		modeloSalonVisible = false;
		modeloSalonNuevoVisible = true;
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		lucesXneg10 = !lucesXneg10;
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		lucesX0 = !lucesX0;
	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		lucesX10 = !lucesX10;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		// Restaurar visibilidad y escala de modelos principales
		piVisible = true;
		nVisible = false;
		piScaleFactor = 1.0f;
		nScaleFactor = 0.0f;
		piMinimizando = false;
		nApareciendo = false;

		// Mesas
		mesasViejasVisibles = true;
		mesasNuevasVisibles = false;
		mesasScaleFactorViejas = 1.0f;
		mesasScaleFactorNuevas = 0.0f;
		mesasViejasReduciendo = false;
		mesasAnimandoAparecer = false;

		// Teclados
		teVisible = true;
		tnVisible = false;
		teScaleFactor = 1.0f;
		tnScaleFactor = 0.0f;
		teMinimizando = false;
		tnApareciendo = false;

		// Sillas especiales
		sillaEspecial1Visible = true;
		sillaEspecial2Visible = true;
		escalaSillaEspecial1 = 4.0f;
		escalaSillaEspecial2 = 4.0f;
		sillaEspecial1Minimizando = false;
		sillaEspecial2Minimizando = false;

		// Modelos Pv y Mess
		modeloPvVisible = true;
		modeloMessVisible = true;
		escalaPv = 1.0f;
		escalaMess = 1.0f;
		minimizandoConT = false;

		// MM
		modeloMMVisible = true;
		escalaMM = 1.0f;
		minimizandoMM = false;

		// Cortinas
		cortinasViejasVisibles = true;
		cortinasNuevasVisibles = false;

		// Rack / Modem
		rackVisible = true;
		modemVisible = false;

		// Aire acondicionado
		aireViejoVisible = true;
		aireNuevoVisible = false;

		// Proyector
		proyectorVisible = true;

		// Salon
		modeloSalonVisible = true;
		modeloSalonNuevoVisible = false;

		for (auto& instancia : animaciones)
		{
			instancia.gaVisible = true;
			instancia.gaExplosionFactor = 1.0f;
			instancia.gaExplosionActive = false;
			instancia.gaInflating = false;
			instancia.gaContracting = false;

			instancia.ganVisible = false;
			instancia.ganAppearing = false;
			instancia.ganScaleFactor = 1.0f;
			instancia.ganRotation = 0.0f;
			instancia.ganFinished = false;

			instancia.gVisible = false;
			instancia.gAppearing = false;
			instancia.gAnimActive = false;
			instancia.gScaleFactor = 0.0f;
			instancia.gRotation = 0.0f;
			instancia.gOffsetX = 0.0f;
			instancia.gInPhase1 = false;
			instancia.gInPhase2 = false;

			instancia.grVisible = false;
			instancia.grAnimActive = false;
			instancia.grScaleFactor = 0.0f;
			instancia.grRotation = 0.0f;
			instancia.grOffsetX = 0.0f;
			instancia.grInPhase1 = false;
			instancia.grInPhase2 = false;

			instancia.ramVisible = false;
			instancia.ramAnimActive = false;
			instancia.ramScaleFactor = 0.0f;
			instancia.ramRotation = 0.0f;
			instancia.ramOffsetX = 0.0f;
			instancia.ramInPhase1 = false;
			instancia.ramInPhase2 = false;

			instancia.prVisible = false;
			instancia.prAnimActive = false;
			instancia.prScaleFactor = 0.0f;
			instancia.prRotation = 0.0f;
			instancia.prOffsetX = 0.0f;
			instancia.prInPhase1 = false;
			instancia.prInPhase2 = false;

			instancia.fuVisible = false;
			instancia.fuAnimActive = false;
			instancia.fuScaleFactor = 0.0f;
			instancia.fuRotation = 0.0f;
			instancia.fuOffsetX = 0.0f;
			instancia.fuInPhase1 = false;
			instancia.fuInPhase2 = false;

			instancia.translationY = globalTranslationYInicial;
			instancia.escala = glm::vec3(4.0f);
			instancia.reduciendoEscala = false;
			instancia.bajandoY = false;
			instancia.escala5Set = false;

			// Reinicia estados de los modelos de monitor
			model1Visible = true;
			model2Visible = false;
			model2Appearing = false;
			model2ScaleFactor = 0.0f;
			model2Rotation = 0.0f;
			explosionActive = false;
			implosionActive = false;
			explosionFactor = 0.0f;
			implosionFactor = 0.0f;

		}
		for (auto& silla : sillas)
		{
			silla.siPos = silla.siPosOriginal;
			silla.siScale = 4.0f;
			silla.siVisible = true;
			silla.siMoving = false;
			silla.siShrinking = false;

			silla.snPos = silla.targetPos + glm::vec3(0.0f, 4.0f, 0.0f);
			silla.snScale = 0.0f;
			silla.snAppearing = false;
			silla.snReturning = false;
			silla.snVisible = false;
			silla.snRegresoTerminado = false;

			silla.fase = SillaAnimada::FaseAnimSilla::Completa;

			silla.animacion.keyframes.clear();
			silla.animacion.currentTime = 0.0f;
			silla.animacion.currentIndex = 0;
			silla.animacion.active = false;
		}

	}




}



























void Animation() {


	////--------------------ANIMACION DE SILLAS-------------/////////

	for (auto& silla : sillas) {
		switch (silla.fase) {

		case SillaAnimada::FaseAnimSilla::EscalandoAntesDeMover:
			silla.siScale = glm::mix(silla.siScale, 1.0f, deltaTime * 1.0f);
			if (fabs(silla.siScale - 1.0f) < 0.05f) {
				silla.siScale = 1.0f;
				silla.fase = SillaAnimada::FaseAnimSilla::SubirAntesDeMover;
			}
			break;

		case SillaAnimada::FaseAnimSilla::SubirAntesDeMover:
			silla.siPos.y = glm::mix(silla.siPos.y, silla.siPosOriginal.y + 4.0f, deltaTime * 1.0f);
			if (fabs(silla.siPos.y - (silla.siPosOriginal.y + 4.0f)) < 0.05f) {
				silla.siPos.y = silla.siPosOriginal.y + 4.0f;

				silla.snScale = 0.0f;
				silla.snPos = silla.targetPos; // ya en la posición
				silla.animacion.keyframes = {
				{0.0f, silla.siPos, 1.0f},
				{	2.0f, silla.targetPos, 1.0f}
				};
				silla.animacion.start();
				silla.fase = SillaAnimada::FaseAnimSilla::MoverASalida;


			}
			break;



		case SillaAnimada::FaseAnimSilla::MoverASalida:
			if (silla.animacion.active) {
				silla.animacion.update(deltaTime * 1.0f);
				silla.siPos = silla.animacion.interpolatedPosition;
			}
			else {
				silla.siExplotaComoModel1 = true;
				silla.siExplosionFactor = 0.0f;
				silla.fase = SillaAnimada::FaseAnimSilla::DesaparecerSi; // ← solo esto

				// El resto va en DesaparecerSi
			}

			break;

		case SillaAnimada::FaseAnimSilla::AparecerSn:
			silla.snScale = glm::mix(silla.snScale, 1.0f, deltaTime * 2.0f);
			if (fabs(silla.snScale - 1.0f) < 0.05f) {
				silla.snScale = 1.0f;

				silla.animacion.keyframes = {
					{0.0f, silla.snPos, 1.0f},
					{3.0f, silla.siPosOriginal + glm::vec3(0.0f, 4.0f, 0.0f), 1.0f}
				};
				silla.animacion.start();
				silla.fase = SillaAnimada::FaseAnimSilla::SnMoverAOriginal;
			}
			break;
		case SillaAnimada::FaseAnimSilla::SnMoverAOriginal:
			if (silla.animacion.active) {
				silla.animacion.update(deltaTime * 1.0f);
				silla.snPos = silla.animacion.interpolatedPosition;
			}
			else {
				silla.fase = SillaAnimada::FaseAnimSilla::SnBajarFinal; // ← PRIMERO baja
			}
			break;




		case SillaAnimada::FaseAnimSilla::SnBajarFinal: {
			const float velocidadBajada = 1.5f;
			silla.snPos.y = glm::mix(silla.snPos.y, silla.siPosOriginal.y, deltaTime * velocidadBajada);

			if (fabs(silla.snPos.y - silla.siPosOriginal.y) < 0.01f) {
				silla.snPos.y = silla.siPosOriginal.y;
				silla.fase = SillaAnimada::FaseAnimSilla::SnEscalarOriginal;  // <--- CAMBIAR AQUI
			}
			break;
		}

		case SillaAnimada::FaseAnimSilla::SnEscalarOriginal:
			silla.snScale = glm::mix(silla.snScale, 5.0f, deltaTime * 1.0f);

			if (fabs(silla.snScale - 4.0f) < 0.05f) {
				silla.snScale = 4.0f;
				silla.fase = SillaAnimada::FaseAnimSilla::Completa;  // <--- Ahora aquí termina
			}
			break;

		case SillaAnimada::FaseAnimSilla::DesaparecerSi:
			if (silla.siExplosionFactor < 1.7f) {
				silla.siExplosionFactor += deltaTime * 0.3f;
			}

			// Escala a 0 mientras explota
			silla.siScale = glm::mix(silla.siScale, 0.0f, deltaTime * 1.0f);

			if (silla.siExplosionFactor >= 1.7f && silla.siScale <= 0.05f) {
				silla.siVisible = false;
				silla.siExplotaComoModel1 = false;
				silla.siExplosionFactor = 0.0f;
				silla.siScale = 0.0f;

				// Activa el modelo sn
				silla.snVisible = true;
				silla.snScale = 0.0f;
				silla.snPos = silla.targetPos;

				silla.animacion.keyframes = {
					{0.0f, silla.snPos, 1.0f},
					{3.0f, silla.siPosOriginal + glm::vec3(0.0f, 4.0f, 0.0f), 1.0f}
				};
				silla.animacion.start();
				silla.fase = SillaAnimada::FaseAnimSilla::AparecerSn;
			}
			break;



		case SillaAnimada::FaseAnimSilla::Completa:
		default:
			break;
		}
	}





///////--------ANIMACION DE PANTALLAS--------------///////////


	// ------------------- EXPLOSIÓN (tecla N) -------------------
	if (explosionActive && explosionFactor < 1.7f) {
		explosionFactor += deltaTime * 0.2f;  // velocidad de expansión
		if (explosionFactor >= 1.7f) {
			explosionFactor = 1.7f;
			explosionActive = false;
			model1Visible = false;
			model2Visible = true;
			model2Appearing = true;
			model2ScaleFactor = 0.0f;
			model2Rotation = 0.0f;
		}
	}

	// ------------------- APARICIÓN MODEL2 -------------------
	if (model2Appearing) {
		model2ScaleFactor += deltaTime * 1.0f;
		if (model2ScaleFactor > 1.0f) model2ScaleFactor = 1.0f;

		model2Rotation += deltaTime * 360.0f;
		if (model2Rotation >= model2TargetRotation) {
			model2Rotation = model2TargetRotation;
			model2Appearing = false;
		}
	}

	// ------------------- IMPLOSIÓN (tecla M) -------------------
	if (implosionActive) {
		implosionFactor += deltaTime * 2.0f;  // velocidad de contracción
		if (implosionFactor >= 1.7f) {
			implosionFactor = 0.0f;
			explosionFactor = 0.0f;
			implosionActive = false;

			model1Visible = true;
			model2Visible = false;
			model2Appearing = false;
			model2ScaleFactor = 0.0f;
			model2Rotation = 0.0f;
		}

	}



/////////---------------------ANIMACION DE PC'S-----------------------////////////////

	for (InstanciaAnimacion& instancia : animaciones) {
		if (instancia.animacionEstatica && !instancia.gaContracting && !instancia.ganAppearing && !instancia.gVisible) {
			continue;
		}

		if (instancia.gaVisible && instancia.gaContracting)
		{
			instancia.gaExplosionFactor -= deltaTime * 1.5f;

			if (instancia.gaExplosionFactor <= 0.0f)
			{
				instancia.gaExplosionFactor = 0.0f;
				instancia.gaVisible = false;
				instancia.ganVisible = true;
				instancia.ganAppearing = true;
				instancia.ganScaleFactor = 0.0f;
				instancia.ganRotation = 0.0f;

				// Solo si estaba arriba, sube a Y=7.0 para animar
				if (!instancia.animacionEstatica)
					instancia.translationY = globalTranslationYInicial;
			}
		}

		if (instancia.posicion.x == 7.0f) {
			// Desactiva el cambio de escala y bajada
			instancia.reduciendoEscala = false;
			instancia.bajandoY = false;
		}


		// Aparece GAN
		if (instancia.ganAppearing) {
			if (instancia.ganScaleFactor < 1.0f) {
				instancia.ganScaleFactor += deltaTime * 0.5f;
				if (instancia.ganScaleFactor > 1.0f) instancia.ganScaleFactor = 1.0f;
			}
			if (instancia.ganRotation < ganTargetRotation) {
				instancia.ganRotation += deltaTime * 180.0f;
				if (instancia.ganRotation > ganTargetRotation)
					instancia.ganRotation = ganTargetRotation;
			}

			if (instancia.ganScaleFactor >= 1.0f && instancia.ganRotation >= ganTargetRotation) {
				instancia.ganAppearing = false;
				instancia.ganFinished = true;
			}
		}

		// Activar G después de GAN
		if (instancia.ganFinished && !instancia.gAnimActive) {
			instancia.gVisible = true;
			instancia.gScaleFactor = 0.0f;
			instancia.gRotation = 0.0f;
			instancia.gOffsetX = -0.5f;
			instancia.gAnimActive = true;
			instancia.gInPhase1 = true;
			instancia.ganFinished = false;
		}

		// Animación de G
		if (instancia.gAnimActive) {
			if (instancia.gInPhase1) {
				instancia.gScaleFactor += deltaTime * 1.5f;
				if (instancia.gScaleFactor > 1.0f) instancia.gScaleFactor = 1.0f;

				instancia.gRotation += deltaTime * 180.0f;
				if (instancia.gRotation >= instancia.gTargetRotation) {
					instancia.gRotation = instancia.gTargetRotation;
					instancia.gInPhase1 = false;
					instancia.gInPhase2 = true;
				}
			}
			else if (instancia.gInPhase2) {
				instancia.gOffsetX += deltaTime * 1.0f;
				if (instancia.gOffsetX >= 0.0f) {
					instancia.gOffsetX = 0.0f;
					instancia.gAnimActive = false;
					instancia.gInPhase2 = false;

					// Activar GR
					instancia.grVisible = true;
					instancia.grScaleFactor = 0.0f;
					instancia.grRotation = 0.0f;
					instancia.grOffsetX = -0.5f;
					instancia.grAnimActive = true;
					instancia.grInPhase1 = true;
				}
			}
		}

		// Animación de GR
		if (instancia.grAnimActive) {
			if (instancia.grInPhase1) {
				instancia.grScaleFactor += deltaTime * 1.5f;
				if (instancia.grScaleFactor > 1.0f) instancia.grScaleFactor = 1.0f;

				instancia.grRotation += deltaTime * 180.0f;
				if (instancia.grRotation >= instancia.grTargetRotation) {
					instancia.grRotation = instancia.grTargetRotation;
					instancia.grInPhase1 = false;
					instancia.grInPhase2 = true;
				}
			}
			else if (instancia.grInPhase2) {
				instancia.grOffsetX += deltaTime * 1.0f;
				if (instancia.grOffsetX >= 0.0f) {
					instancia.grOffsetX = 0.0f;
					instancia.grAnimActive = false;
					instancia.grInPhase2 = false;

					// Activar RAM
					instancia.ramVisible = true;
					instancia.ramScaleFactor = 0.0f;
					instancia.ramRotation = 0.0f;
					instancia.ramOffsetX = -0.5f;
					instancia.ramAnimActive = true;
					instancia.ramInPhase1 = true;
				}
			}
		}

		// RAM
		if (instancia.ramAnimActive) {
			if (instancia.ramInPhase1) {
				instancia.ramScaleFactor += deltaTime * 1.5f;
				if (instancia.ramScaleFactor > 1.0f) instancia.ramScaleFactor = 1.0f;

				instancia.ramRotation += deltaTime * 180.0f;
				if (instancia.ramRotation >= instancia.ramTargetRotation) {
					instancia.ramRotation = instancia.ramTargetRotation;
					instancia.ramInPhase1 = false;
					instancia.ramInPhase2 = true;
				}
			}
			else if (instancia.ramInPhase2) {
				instancia.ramOffsetX += deltaTime * 1.0f;
				if (instancia.ramOffsetX >= 0.0f) {
					instancia.ramOffsetX = 0.0f;
					instancia.ramAnimActive = false;
					instancia.ramInPhase2 = false;

					instancia.prVisible = true;
					instancia.prScaleFactor = 0.0f;
					instancia.prRotation = 0.0f;
					instancia.prOffsetX = -0.5f;
					instancia.prAnimActive = true;
					instancia.prInPhase1 = true;
				}
			}
		}

		// PR
		if (instancia.prAnimActive) {
			if (instancia.prInPhase1) {
				instancia.prScaleFactor += deltaTime * 1.5f;
				if (instancia.prScaleFactor > 1.0f) instancia.prScaleFactor = 1.0f;

				instancia.prRotation += deltaTime * 180.0f;
				if (instancia.prRotation >= 360.0f) {
					instancia.prRotation = 360.0f;
					instancia.prInPhase1 = false;
					instancia.prInPhase2 = true;
				}
			}
			else if (instancia.prInPhase2) {
				instancia.prOffsetX += deltaTime * 1.0f;
				if (instancia.prOffsetX >= 0.0f) {
					instancia.prOffsetX = 0.0f;
					instancia.prAnimActive = false;
					instancia.prInPhase2 = false;

					instancia.fuVisible = true;
					instancia.fuScaleFactor = 0.0f;
					instancia.fuRotation = 0.0f;
					instancia.fuOffsetX = -0.5f;
					instancia.fuAnimActive = true;
					instancia.fuInPhase1 = true;
				}
			}
		}

		// FU
		if (instancia.fuAnimActive) {
			if (instancia.fuInPhase1) {
				instancia.fuScaleFactor += deltaTime * 1.5f;
				if (instancia.fuScaleFactor > 1.0f) instancia.fuScaleFactor = 1.0f;

				instancia.fuRotation += deltaTime * 180.0f;
				if (instancia.fuRotation >= instancia.fuTargetRotation) {
					instancia.fuRotation = instancia.fuTargetRotation;
					instancia.fuInPhase1 = false;
					instancia.fuInPhase2 = true;
				}
			}
			// FU - final
			else if (instancia.fuInPhase2) {
				instancia.fuOffsetX += deltaTime * 1.0f;
				if (instancia.fuOffsetX >= 0.0f) {
					instancia.fuOffsetX = 0.0f;
					instancia.fuAnimActive = false;
					instancia.fuInPhase2 = false;

					instancia.reduciendoEscala = true;
				}
			}

		}


		else if (instancia.reduciendoEscala && !instancia.animacionEstatica) {
			instancia.escala = glm::mix(instancia.escala, glm::vec3(2.0f), deltaTime * 1.5f);
			if (glm::length(instancia.escala - glm::vec3(2.0f)) < 0.01f) {
				instancia.escala = glm::vec3(2.0f);
				instancia.reduciendoEscala = false;
				instancia.bajandoY = true;
			}
		}

		if (instancia.bajandoY && !instancia.animacionEstatica) {
			instancia.translationY -= deltaTime * 2.0f;
			if (instancia.translationY <= 0.5f) {
				instancia.translationY = 0.5f;
				instancia.bajandoY = false;
			}
		}

	}



	/////////////////--------------------------------ANIMACION DE APARICION Y DESAPARICION DE OBJETOS ----------------////////////////////
	//////////MINIMZAR///////////////////////
	if (piMinimizando)
	{
		piScaleFactor -= deltaTime * 2.0f; // velocidad de minimizado
		if (piScaleFactor <= 0.0f)
		{
			piScaleFactor = 0.0f;
			piMinimizando = false;
			piVisible = false;
			nVisible = true;
			nApareciendo = true;
			nScaleFactor = 0.0f;


		}
	}

	//////////////////APARICION////////////////////////ANIMACIONES EXTRAS
	if (nApareciendo)
	{
		nScaleFactor += deltaTime * 2.0f; // velocidad de aparición
		if (nScaleFactor >= 1.0f)
		{
			nScaleFactor = 1.0f;
			nApareciendo = false;
		}
	}

	if (mesasViejasReduciendo)
	{
		mesasScaleFactorViejas -= deltaTime * 2.0f; // velocidad de reducción
		if (mesasScaleFactorViejas <= 0.0f)
		{
			mesasScaleFactorViejas = 0.0f;
			mesasViejasReduciendo = false;
			mesasViejasVisibles = false;
			mesasNuevasVisibles = true;
			mesasAnimandoAparecer = true;
			mesasScaleFactorNuevas = 0.0f;
		}
	}

	if (mesasAnimandoAparecer)
	{
		mesasScaleFactorNuevas += deltaTime * 2.0f; // velocidad de aparición
		if (mesasScaleFactorNuevas >= 1.0f)
		{
			mesasScaleFactorNuevas = 1.0f;
			mesasAnimandoAparecer = false;
		}
	}

	// Minimizar TE
	if (teMinimizando)
	{
		teScaleFactor -= deltaTime * 2.0f;
		if (teScaleFactor <= 0.0f)
		{
			teScaleFactor = 0.0f;
			teMinimizando = false;
			teVisible = false;
			tnVisible = true;
			tnApareciendo = true;
			tnScaleFactor = 0.0f;
		}
	}

	// Aparecer TN
	if (tnApareciendo)
	{
		tnScaleFactor += deltaTime * 2.0f;
		if (tnScaleFactor >= 1.0f)
		{
			tnScaleFactor = 1.0f;
			tnApareciendo = false;
		}
	}

	if (minimizandoConT)
	{
		escalaPv -= 0.02f;  // Velocidad de desaparición
		escalaMess -= 0.02f;
		

		if (escalaPv <= 0.0f)
		{
			escalaPv = 0.0f;
			modeloPvVisible = false;
		}
		if (escalaMess <= 0.0f)
		{
			escalaMess = 0.0f;
			modeloMessVisible = false;
		}
	}

	if (sillaEspecial1Minimizando)
	{
		escalaSillaEspecial1 -= deltaTime * 2.0f; // velocidad de minimizado
		if (escalaSillaEspecial1 <= 0.0f)
		{
			escalaSillaEspecial1 = 0.0f;
			sillaEspecial1Minimizando = false;
			sillaEspecial1Visible = false;
		}
	}

	if (sillaEspecial2Minimizando)
	{
		escalaSillaEspecial2 -= deltaTime * 2.0f;
		if (escalaSillaEspecial2 <= 0.0f)
		{
			escalaSillaEspecial2 = 0.0f;
			sillaEspecial2Minimizando = false;
			sillaEspecial2Visible = false;
		}
	}

	if (minimizandoMM)
	{
		escalaMM -= deltaTime * 2.0f; // velocidad de minimizado
		if (escalaMM <= 0.0f)
		{
			escalaMM = 0.0f;
			minimizandoMM = false;
			modeloMMVisible = false;
		}
	}
	


}










void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
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