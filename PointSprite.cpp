#include "PointSprite.h"
#include "ShaderFunction.h"
#include <STB/stb_image.h>

typedef struct _TextureData {
	_TextureData() : width(0), height(0), data(0) {}
	int width;
	int height;
	unsigned char* data;
} TextureData;

static TextureData Load_png(const char* path) {
	TextureData texture;
	int n;
	stbi_uc *data = stbi_load(path, &texture.width, &texture.height, &n, 4);
	if (data != NULL) {
		texture.data = new unsigned char[texture.width * texture.height * 4 * sizeof(unsigned char)];
		memcpy(texture.data, data, texture.width * texture.height * 4 * sizeof(unsigned char));
		// vertical-mirror image data
		for (size_t i = 0; i < texture.width; i++) {
			for (size_t j = 0; j < texture.height / 2; j++) {
				for (size_t k = 0; k < 4; k++) {
					std::swap(texture.data[(j * texture.width + i) * 4 + k], texture.data[((texture.height - j - 1) * texture.width + i) * 4 + k]);
				}
			}
		}
		stbi_image_free(data);
	}
	return texture;
}

static unsigned int seed = 0x13371337;
static inline float random_float() {
	// 0.0 - 1.0
	float res;
	unsigned int tmp;

	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^ (seed << 15);
	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

PointSprite::PointSprite() {
	counter = 0;

	starsNum = INIT_STARS_NUM;
	starsSpeed = 1;
	starsAngle = 0.0f;
}

PointSprite::~PointSprite() {}

void PointSprite::pointSpriteInit(const char* vs, const char* fs, const char* textureFilePath, float offsetY) {
	char** vsSource = loadShaderSource(vs);
	char** fsSource = loadShaderSource(fs);

	if (vsSource == NULL || fsSource == NULL)
		return;

	// delete old objects
	glDeleteProgram(pointSpriteProgram);
	glDeleteShader(pointSpriteVS);
	glDeleteShader(pointSpriteFS);
	glDeleteVertexArrays(1, &pointSpriteVAO);
	glDeleteBuffers(1, &pointSpriteVBO);
	glDeleteTextures(1, &pointSpriteTexture);


	pointSpriteProgram = glCreateProgram();

	pointSpriteVS = glCreateShader(GL_VERTEX_SHADER);
	pointSpriteFS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pointSpriteVS, 1, vsSource, NULL);
	glShaderSource(pointSpriteFS, 1, fsSource, NULL);
	freeShaderSource(vsSource);
	freeShaderSource(fsSource);
	glCompileShader(pointSpriteVS);
	glCompileShader(pointSpriteFS);
	shaderLog(pointSpriteVS);
	shaderLog(pointSpriteFS);

	glAttachShader(pointSpriteProgram, pointSpriteVS);
	glAttachShader(pointSpriteProgram, pointSpriteFS);
	glLinkProgram(pointSpriteProgram);

	glGenVertexArrays(1, &pointSpriteVAO);
	glBindVertexArray(pointSpriteVAO);

	pointSpritePorjectionLocation = glGetUniformLocation(pointSpriteProgram, "proj_matrix");
	pointSpriteTimeLocation = glGetUniformLocation(pointSpriteProgram, "time");
	pointSpriteAngleLocation = glGetUniformLocation(pointSpriteProgram, "angle");

	glGenVertexArrays(1, &pointSpriteVAO);
	glBindVertexArray(pointSpriteVAO);

	struct star_t {
		glm::vec3 position;
		glm::vec3 color;
	};

	glGenBuffers(1, &pointSpriteVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointSpriteVBO);
	glBufferData(GL_ARRAY_BUFFER, starsNum * sizeof(star_t), NULL, GL_STATIC_DRAW);

	star_t * star = (star_t *)glMapBufferRange(GL_ARRAY_BUFFER, 0, starsNum * sizeof(star_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < starsNum; i++) {
		star[i].position[0] = (random_float() * 2.0f - 1.0f) * 1550.0f;
		star[i].position[1] = (random_float() * 2.0f - 1.0f - 2.0f - offsetY) * 550.0f;
		star[i].position[2] = random_float();
		star[i].color[0] = 0.8f + random_float() * 0.2f;
		star[i].color[1] = 0.8f + random_float() * 0.2f;
		star[i].color[2] = 0.8f + random_float() * 0.2f;
	}

	this->offsetY = offsetY * 20;
	counter = -(this->offsetY);

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), (void *)sizeof(glm::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	TextureData tdata = Load_png(textureFilePath);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(1, &pointSpriteTexture);
	glBindTexture(GL_TEXTURE_2D, pointSpriteTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	return;
}

void PointSprite::render(glm::mat4 proj_matrix) {
	glBindVertexArray(pointSpriteVAO);
	glUseProgram(pointSpriteProgram);

	//float f_timer_cnt = glutGet(GLUT_ELAPSED_TIME);
	//float currentTime = f_timer_cnt * 0.1f;
	counter++;
	if (counter >= COUNTER_UP_BOUND + 10 * offsetY)
		counter = -offsetY;

	float currentTime = (float)counter * (float)starsSpeed;

	//currentTime *= 0.1f;
	//currentTime -= floor(currentTime);
	//currentTime *= 20.0;

	//std::cout << "currentTime: " << currentTime << std::endl;

	glUniform1f(pointSpriteTimeLocation, currentTime);
	glUniformMatrix4fv(pointSpritePorjectionLocation, 1, GL_FALSE, &proj_matrix[0][0]);
	glUniform1f(pointSpriteAngleLocation, starsAngle * (float)counter);

	glEnable(GL_POINT_SPRITE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pointSpriteTexture);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, starsNum);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void PointSprite::increaseStarsNum() {
	starsNum += STARS_NUM_INCREMENT;

	if (starsNum > MAX_STARS_NUM)
		starsNum = MAX_STARS_NUM;

	return;
}

void PointSprite::decreaseStarsNum() {
	starsNum -= STARS_NUM_INCREMENT;

	if (starsNum < 0)
		starsNum = 0;

	return;
}

void PointSprite::increaseStarsSpeed() {
	starsSpeed++;
}

void PointSprite::decreaseStarsSpeed() {
	starsSpeed--;
	if (starsSpeed < 1)
		starsSpeed = 1;
}

void PointSprite::increaseStarsAngle() {
	starsAngle += STARS_ANGLE_INCREMENT;
}

void PointSprite::decreaseStarsAngle() {
	starsAngle -= STARS_ANGLE_INCREMENT;
}