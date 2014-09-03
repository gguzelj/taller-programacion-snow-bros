#include "../headers/Drawer.h"

Drawer::Drawer(){}

Drawer::~Drawer(){}

//Dibuja un cuerpo estatico
void Drawer::drawStaticBody(SDL_Renderer* renderer, b2Body* body){
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	int ox = 320;
	int oy = 400;
	float sc = 40.0;
	//http://box2d.org/forum/viewtopic.php?f=3&t=1933
	for( b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() ){
		if( fixture->GetType() == b2Shape::e_polygon ){
			b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();

			const int count = poly->GetVertexCount();

			for( int i = 0; i < count; i++ ){
				int ind0 = (i + 1) % count ;
				b2Vec2 p0 = body->GetWorldPoint(  poly->GetVertex( ind0 ) );
				b2Vec2 p1 = body->GetWorldPoint(  poly->GetVertex(i) );

				SDL_RenderDrawLine(renderer, sc * p0.x + ox, -sc * p0.y + oy , sc * p1.x + ox, -sc * p1.y + oy);
			}
		}
	}
}

void Drawer::drawScenary(b2World* world, VentanaGrafica* window){
	for(b2Body* bodi = world->GetBodyList(); bodi; bodi = bodi->GetNext()){
		drawStaticBody(window->renderer, bodi);
	}
}
