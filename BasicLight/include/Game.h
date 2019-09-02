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
private:
	bool isWireframe;
	float specularAmount;
	float gammaCorrection;

	unsigned int defaultFBO;
	unsigned int defaultColourTex;
	unsigned int rbo;

	Shader* simpleShader;
	Shader* lightShader;
	Shape* shape;
	Cube* light;
	Camera* cam;
	Texture* newTexture;
	glm::vec3 lightColour;

	// Cache
	unsigned int shaderProgram, lightProgram;
};

#endif