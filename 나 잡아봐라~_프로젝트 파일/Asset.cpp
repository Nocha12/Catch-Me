#include "DXUT.h"
#include "Asset.h"


Asset::Asset(void)
	:filesLoaded(0), filesToLoad(0)
{
}


Asset::~Asset(void)
{
}


void Asset::init()
{
	_s.Initialize(DXUTGetHWND(), DSSCL_PRIORITY);
}


void Asset::dispose()
{
	for(auto iter : textures)
		SAFE_DELETE(iter.second);
	for(auto iter : sounds)
		SAFE_DELETE(iter.second);
	for(auto iter : shaders)
		SAFE_RELEASE(iter);
}


void Asset::ready(string rootFolderPath)
{
	for(recursive_directory_iterator iter(rootFolderPath); iter != recursive_directory_iterator(); iter++)
	{
		string extension = iter->path().extension();
		if(extension == ".png" || extension == ".dds")
			paths.push_back(iter->path());
	}

	filesToLoad = paths.size();
	pathIter = paths.begin();
}

void Asset::loadNext()
{
	getTexture(*pathIter);
	pathIter++;
}

Texture* Asset::getTexture(path p)
{
	if(!exists(p))
		cout << "파일 없음 : " << p << endl;

	if(!textures[p])
	{
		textures[p] = new Texture();
		D3DXCreateTextureFromFileExA(
			DXUTGetD3D9Device(),
			p.string().data(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			1,
			NULL,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			NULL,
			&textures[p]->info,
			NULL,
			&textures[p]->d3dTexture
		);

		++filesLoaded;
	}

	return textures[p];
}

CSound* Asset::getSound(wpath p)
{
	if(!sounds[p])
		_s.Create(&sounds[p], const_cast<LPWSTR>(p.string().data()));

	return sounds[p];
}

LPD3DXEFFECT Asset::getShader(string path)
{
	LPD3DXEFFECT p;

	D3DXCreateEffectFromFileA(
		DXUTGetD3D9Device(),
		path.data(),
		NULL,
		NULL,
		D3DXSHADER_USE_LEGACY_D3DX9_31_DLL,
		NULL,
		&p,
		NULL );

	shaders.push_back( p );

	return p;
}

Asset asset;