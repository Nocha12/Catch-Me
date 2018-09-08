#include "DXUT.h"
#include "Texture.h"


Texture::Texture(void)
{
}


Texture::~Texture(void)
{
	SAFE_RELEASE(d3dTexture);
}
