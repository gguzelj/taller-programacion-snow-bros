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
#define LADOS				"lados"
#define COLOR				"color"
#define ROT					"rot"
#define INCLINACION			"inclinacion"
#define MASA				"masa"
#define ESTATICO			"estatico"

#define RECTANGULO			"rectangulo"
#define CIRCULO				"circulo"
#define PARALELOGRAMO		"paralelogramo"
#define POLIGONO			"poligono"
#define TRAPECIO			"trapecio"

//Valores Default para el juego
#define ALTO_PX_DEF			768
#define ANCHO_PX_DEF		1024
#define ALTO_UN_DEF			19.2
#define ANCHO_UN_DEF		25.6
#define IMAGEN_FONDO_DEF	"Everest.jpg"
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
#define ANCHO_UN_MIN		10
#define ALTO_UN_MAX			2048
#define ALTO_UN_MIN			10
#define	ESCALA_MAX			10
#define	ESCALA_MIN			0.1
#define	ANCHO_MAX			10
#define	ANCHO_MIN			0.1
#define	ALTO_MAX			10
#define	ALTO_MIN			0.1
#define	ROT_MIN				0
#define	ROT_MAX				360
#define INCLINACION_MIN		30
#define INCLINACION_MAX		150
#define	MASA_MIN			1
#define	MASA_MAX			36000
#define LADOS_MIN			3
#define LADOS_MAX			6

//Conversion de unidades
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

//Errores del parser
#define PARSER_ERROR_OPEN_FILE				"Error Parser: Al tratar de abrir el archivo "
#define PARSER_ERROR_INVALID_JSON			"Error Parser: Al leer el archivo JSON "
#define PARSER_ERROR_NO_ESCENARIO			"Error Parser: El archivo JSON no contiene un escenario definido"
#define PARSER_ERROR_NO_PERSONAJE			"Error Parser: El archivo JSON no contiene un personaje definido"
#define PARSER_ERROR_NO_OBJETOS				"Error Parser: El archivo JSON no contiene objetos definidos"
#define PARSER_ERROR_TIPO_ESCENARIO			"Error Parser: Escenario - El atributo debe ser definido como un objeto"
#define PARSER_ERROR_TIPO_PERSONAJE			"Error Parser: Personaje - El atributo debe ser definido como un objeto"
#define PARSER_ERROR_TIPO_OBJETOS			"Error Parser: Objetos - El atributo debe ser definido como un array"
#define PARSER_ERROR_SIN_OBJETOS			"Error Parser: No se pudo encontrar ningun objeto correctamente definido"
#define PARSER_ERROR_PER_COOR_X				"Error Parser: Personaje - Falta definir la coordenada X"
#define PARSER_ERROR_PER_COOR_X_NO_NUMBER	"Error Parser: Personaje - La coordenada X no es un numero"
#define PARSER_ERROR_PER_COOR_X_FUERA_RANGO	"Error Parser: Personaje - La coordenada X esta fuera del rango del escenario"
#define PARSER_ERROR_PER_COOR_Y				"Error Parser: Personaje - Falta definir la coordenada Y"
#define PARSER_ERROR_PER_COOR_Y_NO_NUMBER	"Error Parser: Personaje - La coordenada Y no es un numero"
#define PARSER_ERROR_PER_COOR_Y_FUERA_RANGO	"Error Parser: Personaje - La coordenada Y esta fuera del rango del escenario"
#define PARSER_ERROR_OBJ_TIPO				"Error Parser: Objeto - Falta definir el Tipo de Objeto"
#define PARSER_ERROR_OBJ_TIPO_NO_STRING		"Error Parser: Objeto - El Tipo de Objeto debe estar definido como un string"
#define PARSER_ERROR_OBJ_TIPO_DESCONOCIDO	"Error Parser: Objeto - El Tipo de Objeto es desconocido"
#define PARSER_ERROR_OBJ_COOR_X				"Error Parser: Objeto - Falta definir la coordenada X"
#define PARSER_ERROR_OBJ_COOR_X_NO_NUMBER	"Error Parser: Objeto - La coordenada X no es un numero"
#define PARSER_ERROR_OBJ_COOR_X_FUERA_RANGO "Error Parser: Objeto - La coordenada X esta fuera del rango del escenario"
#define PARSER_ERROR_OBJ_COOR_Y				"Error Parser: Objeto - Falta definir la coordenada Y"
#define PARSER_ERROR_OBJ_COOR_Y_NO_NUMBER	"Error Parser: Objeto - La coordenada Y no es un numero"
#define PARSER_ERROR_OBJ_COOR_Y_FUERA_RANGO "Error Parser: Objeto - La coordenada Y esta fuera del rango del escenario"
#define PARSER_ERROR_OBJ_ESCALA				"Error Parser: Objeto - Falta definir la Escala"
#define PARSER_ERROR_OBJ_ESCALA_NO_NUMBER	"Error Parser: Objeto - La Escala no es un numero"
#define PARSER_ERROR_OBJ_ESCALA_FUERA_RANGO "Error Parser: Objeto - La Escala esta fuera del rango permitido"
#define PARSER_ERROR_OBJ_ANCHO				"Error Parser: Objeto - Falta definir el Ancho"
#define PARSER_ERROR_OBJ_ANCHO_NO_NUMBER	"Error Parser: Objeto - El Ancho no es un numero"
#define PARSER_ERROR_OBJ_ANCHO_FUERA_RANGO 	"Error Parser: Objeto - El Ancho esta fuera del rango permitido"
#define PARSER_ERROR_OBJ_ALTO				"Error Parser: Objeto - Falta definir el Alto"
#define PARSER_ERROR_OBJ_ALTO_NO_NUMBER		"Error Parser: Objeto - El Alto no es un numero"
#define PARSER_ERROR_OBJ_ALTO_FUERA_RANGO 	"Error Parser: Objeto - El Alto esta fuera del rango permitido"
#define PARSER_ERROR_OBJ_ESTATICO			"Error Parser: Objeto - Falta definir si el objeto es Estatico"
#define PARSER_ERROR_OBJ_ESTATICO_NO_BOOL	"Error Parser: Objeto - El atributo estatico no es tipo Bool"
#define PARSER_ERROR_OBJ_LADOS				"Error Parser: Objeto - Falta definir cantidad de Lados"
#define PARSER_ERROR_OBJ_LADOS_NO_NUMBER	"Error Parser: Objeto - Cantidad de Lados no es un numero"
#define PARSER_ERROR_OBJ_LADOS_FUERA_RANGO	"Error Parser: Objeto - Cantidad de Lados fuera del rango permitido"

#define PARSER_WARNING_ESC_ALTO_PX					"Adv. Parser: Escenario - Atributo alto-px sin definir. Se carga valor por default."
#define PARSER_WARNING_ESC_ALTO_PX_NO_NUMBER		"Adv. Parser: Escenario - Atributo alto-px es un numero. Se carga valor por default."
#define PARSER_WARNING_ESC_ALTO_PX_FUERA_RANGO		"Adv. Parser: Escenario - Atributo alto-px fuera de rango. Se carga valor por default."
#define PARSER_WARNING_ESC_ANCHO_PX					"Adv. Parser: Escenario - Atributo ancho-px sin definir. Se carga valor por default."
#define PARSER_WARNING_ESC_ANCHO_PX_NO_NUMBER		"Adv. Parser: Escenario - Atributo ancho-px es un numero. Se carga valor por default."
#define PARSER_WARNING_ESC_ANCHO_PX_FUERA_RANGO		"Adv. Parser: Escenario - Atributo ancho-px fuera de rango. Se carga valor por default."
#define PARSER_WARNING_ESC_ALTO_UN					"Adv. Parser: Escenario - Atributo alto-un sin definir. Se carga valor por default."
#define PARSER_WARNING_ESC_ALTO_UN_NO_NUMBER		"Adv. Parser: Escenario - Atributo alto-un es un numero. Se carga valor por default."
#define PARSER_WARNING_ESC_ALTO_UN_FUERA_RANGO		"Adv. Parser: Escenario - Atributo alto-un fuera de rango. Se carga valor por default."
#define PARSER_WARNING_ESC_ANCHO_UN					"Adv. Parser: Escenario - Atributo ancho-un sin definir. Se carga valor por default."
#define PARSER_WARNING_ESC_ANCHO_UN_NO_NUMBER		"Adv. Parser: Escenario - Atributo ancho-un es un numero. Se carga valor por default."
#define PARSER_WARNING_ESC_ANCHO_UN_FUERA_RANGO		"Adv. Parser: Escenario - Atributo ancho-un fuera de rango. Se carga valor por default."
#define PARSER_WARNING_ESC_IMAGEN_FONDO				"Adv. Parser: Escenario - Atributo imagen_fondo sin definir. Se carga valor por default."
#define PARSER_WARNING_ESC_IMAGEN_FONDO_NO_STRING 	"Adv. Parser: Escenario - Atributo imagen_fondo es de un tipo distinto a string. Se carga valor por default."
#define PARSER_WARNING_ESC_IMAGEN_FONDO_NO_EXISTE	"Adv. Parser: Escenario - Atributo imagen_fondo no existe. Se carga valor por default."
#define PARSER_WARNING_OBJ_COLOR					"Adv. Parser: Objeto - El Color no fue definido. Se carga valor por default."
#define PARSER_WARNING_OBJ_COLOR_NO_STRING			"Adv. Parser: Objeto - El Color no es del tipo string. Se carga valor por default."
#define PARSER_WARNING_OBJ_COLOR_DESCONOCIDO		"Adv. Parser: Objeto - Color desconocido. Se carga valor por default."
#define PARSER_WARNING_OBJ_ROT						"Adv. Parser: Objeto - La Rotacion no fue definida. Se carga valor por default."
#define PARSER_WARNING_OBJ_ROT_NO_NUMBER			"Adv. Parser: Objeto - La Rotacion no es un numero. Se carga valor por default."
#define PARSER_WARNING_OBJ_ROT_FUERA_RANGO			"Adv. Parser: Objeto - La Rotacion esta fuera del rango permitido. Se carga valor por default."
#define PARSER_WARNING_OBJ_MASA						"Adv. Parser: Objeto - La Masa no fue definida. Se carga valor por default."
#define PARSER_WARNING_OBJ_MASA_NO_NUMBER			"Adv. Parser: Objeto - La Masa no es un numero. Se carga valor por default."
#define PARSER_WARNING_OBJ_MASA_FUERA_RANGO			"Adv. Parser: Objeto - La Masa esta fuera del rango permitido. Se carga valor por default."

#define PARSER_INFO_DEFAULT_GAME			"Se intenta cargar un escenario default"


#endif /* JSONPARSER_C_H_ */
