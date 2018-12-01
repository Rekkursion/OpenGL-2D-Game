#include "Sprite2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

TextureData Load_png(const char* path)
{
	TextureData texture;
	int n;
	stbi_uc *data = stbi_load(path, &texture.width, &texture.height, &n, 4);
	if (data != NULL)
	{
		texture.data = new unsigned char[texture.width * texture.height * 4 * sizeof(unsigned char)];
		memcpy(texture.data, data, texture.width * texture.height * 4 * sizeof(unsigned char));
		// vertical-mirror image data
		for (size_t i = 0; i < texture.width; i++)
		{
			for (size_t j = 0; j < texture.height / 2; j++)
			{
				for (size_t k = 0; k < 4; k++) {
					std::swap(texture.data[(j * texture.width + i) * 4 + k], texture.data[((texture.height - j - 1) * texture.width + i) * 4 + k]);
				}
			}
		}
		stbi_image_free(data);
	}
	return texture;
}

Sprite2D::Sprite2D()
{
	
}


Sprite2D::~Sprite2D()
{
}

bool Sprite2D::Init(std::string fileName, int FPS)
{
	TextureData tdata = Load_png(fileName.c_str());

	if (tdata.data != nullptr) {
		spriteFPS = FPS;
		spriteAspect = (float)subWidth / (float)subHeight;
		scaleMat = glm::scale(glm::vec3(spriteAspect, 1, 1));

		int w = tdata.width, h = tdata.height;
		bool alpha = true;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (alpha) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)tdata.data);
		}
		else {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, (GLvoid*)tdata.data);
		}

		delete[] tdata.data;
		return true;
	}
	else {
		puts("Can't find png picture");
	}

	return false;
}

glm::mat4 Sprite2D::GetModelMat()
{
	return scaleMat;
}

float Sprite2D::GetAspect()
{
	return spriteAspect;
}

float Sprite2D::GetFPS()
{
	return spriteFPS;
}

int Sprite2D::GetCount()
{
	return spriteCount;
}

void Sprite2D::Enable()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Sprite2D::Disable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
