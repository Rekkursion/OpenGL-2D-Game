#include "DotLight.h"



DotLight::DotLight(): DotLight(0.0f, 0.0f) {
}

DotLight::DotLight(float marcoScreenPosX, float marcoScreenPosY) {
	radAngle = 0.0f;
	radius = DOT_LIGHT_SCREEN_RADIUS;
	screenPos3D = glm::vec3(marcoScreenPosX + radius, -0.63f, 0.0f);
	pos = screenPos3D.xy;
}

DotLight::~DotLight() {}

void DotLight::initSprite() {
	std::string filename = "../media/DotLight.png";
	dotLightSprite.Init(filename, 0);
}

void DotLight::initialize(const char* vs, const char* fs) {
	Object::initialize(vs, fs);
}

float DotLight::getScreenPosZ() {
	return screenPos3D.z;
}

void DotLight::render(float marcoScreenPosX, float marcoScreenPosY) {
	screenPos3D.xyz = glm::vec3(cosf(radAngle) * radius + marcoScreenPosX, marcoScreenPosY + 0.17f, sinf(radAngle) * radius);
	pos = screenPos3D.xy;

	radAngle += DOT_LIGHT_SCREEN_ANGLE_SPEED;
	if (radAngle >= 3.14f * 2.0f)
		radAngle = 0.0f;

	//std::cout << "screenPos3D.z: " << screenPos3D.z << std::endl;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	dotLightSprite.Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	dotLightSprite.Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}