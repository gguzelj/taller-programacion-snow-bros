#ifndef VENTANAGRAFICA_H_
#define VENTANAGRAFICA_H_

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../headers/constants.h"
#include "../headers/Drawer.h"
using namespace std;

class VentanaGrafica : public Drawer{
        private:
                int alto_px;
                int ancho_px;
                string imagePath;
                SDL_Window *window;
                SDL_Texture *image;
                SDL_Renderer *renderer;
                void inicializarSDL();

                /**
                * Loads an image into a texture on the rendering device
                * @param file The image file to load
                * @param ren The renderer to load the texture onto
                * @return the loaded texture, or nullptr if something went wrong.
                */
                SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

                /**
                * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
                * width and height
                * @param tex The source texture we want to draw
                * @param ren The renderer we want to draw to
                * @param x The x coordinate to draw to
                * @param y The y coordinate to draw to
                * @param w The width of the texture to draw
                * @param h The height of the texture to draw
                */
                void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

                /**
                * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
                * the texture's width and height
                * @param tex The source texture we want to draw
                * @param ren The renderer we want to draw to
                * @param x The x coordinate to draw to
                * @param y The y coordinate to draw to
                */
                void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

                /**
                * Log an SDL error with some error message to the output stream of our choice
                * @param os The output stream to write the message to
                * @param msg The error message to write, format will be msg error: SDL_GetError()
                */

                void logSDLError(std::ostream &os, const std::string &msg);

        public:
                /*
                 * El constructor setea los atributos, chequea que SDL2 inicialice correctamente, crea una ventana
                 * luego crea un renderer, que es donde vamos a dibujar y luego carga la imagen en memoria
                 */
                VentanaGrafica();
                /*
                 * Constructor de la clase. Los parametros deben ser solicitados al parser, quien verificara
                 * si el parametro pasado por el usuario es correcto o no y le devolvera a VentanaGrafica un
                 * parametro con estado correcto
                 */
                VentanaGrafica(int alto_px ,int ancho_px ,string imagePath);

                ~VentanaGrafica();

                /*
                 * Implementa los métodos de la interfaz "Drawer"
                 */
                void clearScenary();

                void drawBackground();

                void drawStaticBody(b2Body* body);

                void drawScenary(b2World* world);

                void presentScenary();
};

#endif /* VENTANAGRAFICA_H_ */
