#include "../../headers/Vista/LTexture.h"

LTexture::LTexture()
{
	//Initialize
	mTexture = nullptr;
}

LTexture::~LTexture(){
	SDL_DestroyTexture(mTexture);
}

bool LTexture::loadFromFile( std::string path, SDL_Renderer* gRenderer )
{
	//The final texture
	SDL_Texture* newTexture = IMG_LoadTexture(gRenderer, path.c_str());

	//Return success
	mTexture = newTexture;
	return mTexture != nullptr;
}

void LTexture::render( SDL_Renderer* gRenderer, int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, w, h };

	//Set clip rendering dimensions
	if( clip != nullptr )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
