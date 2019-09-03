#ifndef __GAME_H__
#define __GAME_H__

#include "Application.h"

#include "Cube.h"
#include "Pyramid.h"
#include "Shader.h"
#include "Camera.h"
#include "Utilities.h"
#include "Texture.h"

#include "glm/glm.hpp"

class Game : public Application
{
public:
	Game();
	~Game();

	bool OnCreate() override;
	void Update(float a_deltaTime) override;
	void Draw() override;
	void Destroy() override;

private:
	void ImGuiDraw();
	void ResizeFBO(float a_width, float a_height);
	void Screenshot();
private:
	bool isWireframe;
	float specularAmount;
	float gammaCorrection;

	unsigned int defaultFBO;
	unsigned int defaultColourTex;
	unsigned int depthRender;
	float fboWidth, fboHeight;

	Shader* simpleShader;
	Shader* lightShader;
	Shape* shape;
	Cube* light;
	Camera* cam;
	Texture* newTexture;
	glm::vec3 lightColour;

	// Cache
	unsigned int shaderProgram, lightProgram;
	bool screenshot;
	glm::vec2 viewPortSize;

	// Globals
	char ssPath[128];
};

#endif