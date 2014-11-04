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

bool LTexture::loadFromRenderedText( SDL_Renderer* gRenderer, TTF_Font* gFont, std::string textureText, SDL_Color textColor, int* ancho, int* alto )
{
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL ){
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		//No tiene un tamaño fijo, por eso lo tengo que modificar.
		*ancho = textSurface->w;
		*alto = textSurface->h;
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	//Return success
	return mTexture != NULL;
}
