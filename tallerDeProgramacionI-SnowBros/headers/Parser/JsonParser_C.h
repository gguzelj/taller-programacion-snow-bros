#ifndef JSONPARSER_C_H_
#define JSONPARSER_C_H_

//Constantes para los atributos del json
#define ESCENARIO			"escenario"
#define ALTO_PX				"alto-px"
#define ANCHO_PX			"ancho-px"
#define ALTO_UN				"alto-un"
#define ANCHO_UN			"ancho-un"
#define IMAGEN_FONDO		"imagen_fondo"
#define PERSONAJE			"personaje"
#define X_COOR				"x"
#define Y_COOR				"y"
#define ESCALA				"escala"
#define OBJETOS				"objetos"
#define TIPO				"tipo"
#define ANCHO				"ancho"
#define ALTO				"alto"
#define COLOR				"color"
#define ROT					"rot"
#define MASA				"masa"
#define ESTATICO			"estatico"

#define RECTANGULO			"rectangulo"
#define CIRCULO				"circulo"
#define PARALELOGRAMO		"paralelogramo"
#define POLIGONO			"poligono"

//Valores Default para el juego
#define ALTO_PX_DEF			768
#define ANCHO_PX_DEF		1024
#define ALTO_UN_DEF			100
#define ANCHO_UN_DEF		50
#define IMAGEN_FONDO_DEF	"Everest.png"
#define X_COOR_DEF			100.0
#define Y_COOR_DEF			100.0
#define COLOR_DEF			"#00FF00"
#define	ROT_DEF				0
#define	MASA_DEF			1
#define PATH_DEF			"resources/example.json"

//Rangos de valores validos para el juego
#define ALTO_PX_MAX			2048
#define ALTO_PX_MIN			200
#define ANCHO_PX_MAX		2048
#define ANCHO_PX_MIN		200
#define ANCHO_UN_MAX		2048
#define ANCHO_UN_MIN		200
#define ALTO_UN_MAX			2048
#define ALTO_UN_MIN			200
#define	ESCALA_MAX			10
#define	ESCALA_MIN			1
#define	ANCHO_MAX			100
#define	ANCHO_MIN			1
#define	ALTO_MAX			100
#define	ALTO_MIN			1
#define	ROT_MIN				0
#define	ROT_MAX				360
#define	MASA_MIN			1
#define	MASA_MAX			36000

//Errores del parser
#define PARSER_ERROR_OPEN_FILE		"Error al tratar de abrir el archivo "
#define PARSER_ERROR_INVALID_JSON	"Error al parsear el archivo JSON: "
#define PARSER_ERROR_NO_ESCENARIO	"Error: El siguiente archivo JSON no contiene un escenario definido: "

#define PARSER_INFO_DEFAULT_GAME	"Se intenta cargar un escenario default"


#endif /* JSONPARSER_C_H_ */
