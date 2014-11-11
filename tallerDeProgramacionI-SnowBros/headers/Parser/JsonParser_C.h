
#ifndef JSONPARSER_C_H_
#define JSONPARSER_C_H_

//Constantes para los atributos del json
#define ESCENARIO              "escenario"
#define MAXIMO_JUGADORES	   "maximo-jugadores"
#define GRAVEDAD 			   "gravedad"
#define ALTO_PX                "alto-px"
#define ANCHO_PX               "ancho-px"
#define IMAGEN_FONDO           "imagen_fondo"
#define PERSONAJE              "personaje"
#define X_COOR                 "x"
#define Y_COOR                 "y"
#define ESCALA                 "escala"
#define OBJETOS                "objetos"
#define TIPO                   "tipo"
#define ANCHO                  "ancho"
#define ALTO                   "alto"
#define LADOS                  "lados"
#define ROT                    "rot"
#define BASE           		   "base"
#define MASA                   "masa"
#define ESTATICO               "estatico"

#define RECTANGULO             "rectangulo"
#define CIRCULO                "circulo"
#define PARALELOGRAMO          "paralelogramo"
#define POLIGONO               "poligono"
#define TRAPECIO               "trapecio"

//Valores Default para el juego
#define MAXIMO_JUGADORES_DEF	4
#define GRAVEDAD_DEF			-10.0f
#define COLOR_DEF               "#00FF00"
#define ROT_DEF                 0
#define MASA_DEF                1
#define PATH_DEF				"resources/config.json"
#define PERSONAJE_X_DEF			 0
#define PERSONAJE_Y_DEF			 0

//Rangos de valores validos para el juego
#define MAXIMO_JUGADORES_MAX	4
#define MAXIMO_JUGADORES_MIN	2
#define ESCALA_MAX              50
#define ESCALA_MIN              0.1
#define ANCHO_MAX               90
#define ANCHO_MIN               0.1
#define ALTO_MAX                75
#define ALTO_MIN                0.1
#define ROT_MIN                 -359
#define ROT_MAX                 359
#define BASE_MIN       		    0.2
#define BASE_MAX       		    75
#define MASA_MIN                1
#define MASA_MAX                100
#define LADOS_MIN               3
#define LADOS_MAX               6

//Conversion de unidades
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

//MENSAJES
//ERROR
#define PARSER_MSG_INVALID_JSON               "Parser: Al leer el archivo JSON: "
#define PARSER_MSG_INVALID_DEF_JSON			  "Parser: El archivo JSON default no es consistente. Se detiene el juego."
#define PARSER_MSG_COMIENZO_ESCENARIO		  "Parser: Analizando escenario"
#define PARSER_MSG_COMIENZO_PERSONAJE		  "Parser: Analizando personaje"
#define PARSER_MSG_COMIENZO_OBJETOS			  "Parser: Analizando objetos"
#define PARSER_MSG_NO_ESCENARIO               "Parser: El archivo JSON no contiene un escenario definido"
#define PARSER_MSG_NO_PERSONAJE               "Parser: El archivo JSON no contiene un personaje definido"
#define PARSER_MSG_NO_OBJETOS                 "Parser: El archivo JSON no contiene objetos definidos"
#define PARSER_MSG_TIPO_ESCENARIO             "Parser: Escenario - El atributo debe ser definido como un objeto"
#define PARSER_MSG_TIPO_PERSONAJE             "Parser: Personaje - El atributo debe ser definido como un objeto"
#define PARSER_MSG_TIPO_OBJETOS               "Parser: Objetos - El atributo debe ser definido como un array"
#define PARSER_MSG_SIN_OBJETOS                "Parser: No se pudo encontrar ningun objeto correctamente definido"
#define PARSER_MSG_PER_COOR_X                 "Parser: Personaje - Falta definir la coordenada X"
#define PARSER_MSG_PER_COOR_X_NO_NUMBER       "Parser: Personaje - La coordenada X no es un numero"
#define PARSER_MSG_PER_COOR_X_FUERA_RANGO     "Parser: Personaje - La coordenada X esta fuera del rango del escenario"
#define PARSER_MSG_PER_COOR_Y                 "Parser: Personaje - Falta definir la coordenada Y"
#define PARSER_MSG_PER_COOR_Y_NO_NUMBER       "Parser: Personaje - La coordenada Y no es un numero"
#define PARSER_MSG_PER_COOR_Y_FUERA_RANGO     "Parser: Personaje - La coordenada Y esta fuera del rango del escenario"
#define PARSER_MSG_OBJ_TIPO                   "Parser: Objeto - Falta definir el Tipo de Objeto \n"
#define PARSER_MSG_OBJ_TIPO_NO_STRING         "Parser: Objeto - El Tipo de Objeto debe estar definido como un string \n"
#define PARSER_MSG_OBJ_TIPO_DESCONOCIDO       "Parser: Objeto - El Tipo de Objeto es desconocido \n"
#define PARSER_MSG_OBJ_COOR_X                 "Parser: Objeto - Falta definir la coordenada X \n"
#define PARSER_MSG_OBJ_COOR_X_NO_NUMBER       "Parser: Objeto - La coordenada X no es un numero \n"
#define PARSER_MSG_OBJ_COOR_X_FUERA_RANGO	  "Parser: Objeto - La coordenada X esta fuera del rango del escenario \n"
#define PARSER_MSG_OBJ_COOR_Y                 "Parser: Objeto - Falta definir la coordenada Y \n"
#define PARSER_MSG_OBJ_COOR_Y_NO_NUMBER       "Parser: Objeto - La coordenada Y no es un numero \n"
#define PARSER_MSG_OBJ_COOR_Y_FUERA_RANGO 	  "Parser: Objeto - La coordenada Y esta fuera del rango del escenario \n"
#define PARSER_MSG_OBJ_ESCALA                 "Parser: Objeto - Falta definir la Escala \n"
#define PARSER_MSG_OBJ_ESCALA_NO_NUMBER       "Parser: Objeto - La Escala no es un numero \n"
#define PARSER_MSG_OBJ_ESCALA_FUERA_RANGO     "Parser: Objeto - La Escala esta fuera del rango permitido \n"
#define PARSER_MSG_OBJ_ANCHO                  "Parser: Objeto - Falta definir el Ancho \n"
#define PARSER_MSG_OBJ_ANCHO_NO_NUMBER        "Parser: Objeto - El Ancho no es un numero \n"
#define PARSER_MSG_OBJ_ANCHO_FUERA_RANGO      "Parser: Objeto - El Ancho esta fuera del rango permitido \n"
#define PARSER_MSG_OBJ_ALTO                   "Parser: Objeto - Falta definir el Alto \n"
#define PARSER_MSG_OBJ_ALTO_NO_NUMBER         "Parser: Objeto - El Alto no es un numero \n"
#define PARSER_MSG_OBJ_ALTO_FUERA_RANGO       "Parser: Objeto - El Alto esta fuera del rango permitido \n"
#define PARSER_MSG_OBJ_ESTATICO               "Parser: Objeto - Falta definir si el objeto es Estatico \n"
#define PARSER_MSG_OBJ_ESTATICO_NO_BOOL       "Parser: Objeto - El atributo estatico no es tipo Bool \n"
#define PARSER_MSG_OBJ_LADOS                  "Parser: Objeto - Falta definir cantidad de Lados \n"
#define PARSER_MSG_OBJ_LADOS_NO_NUMBER        "Parser: Objeto - Cantidad de Lados no es un numero \n"
#define PARSER_MSG_OBJ_LADOS_FUERA_RANGO      "Parser: Objeto - Cantidad de Lados fuera del rango permitido \n"
#define PARSER_MSG_OBJ_INCL                   "Parser: Objeto - Falta definir la inclinacion \n"
#define PARSER_MSG_OBJ_INCL_NO_NUMBER         "Parser: Objeto - La inclinacion no es un numero \n"
#define PARSER_MSG_OBJ_INCL_FUERA_RANGO       "Parser: Objeto - La inclinacion esta fuera de rango \n"
#define PARSER_MSG_OBJ_BASE		              "Parser: Objeto - Falta definir la base \n"
#define PARSER_MSG_OBJ_BASE_NO_NUMBER		  "Parser: Objeto - La base no es un numero \n"
#define PARSER_MSG_OBJ_BASE_FUERA_RANGO 	  "Parser: Objeto - La base esta fuera de rango \n"
#define PARSER_MSG_CARGA_PERSONAJE_DEF 		  "Parser: Se procedera a cargar un personaje por default."
#define PARSER_MSG_CARGA_COORD_X_DEF 		  "Parser: Se asignara una coordenada X por default"
#define PARSER_MSG_CARGA_COORD_Y_DEF 		  "Parser: Se asignara una coordenada Y por default"


//WARNING
#define PARSER_MSG_OPEN_FILE                        "Parser: No se pudo abrir el archivo "
#define PARSER_MSG_ESC_MXJG							"Parser: Escenario - Atributo maximo-jugadores sin definir. Se carga valor por default."
#define PARSER_MSG_ESC_MXJG_NO_NUMBER				"Parser: Escenario - Atributo maximo-jugadores es un numero. Se carga valor por default."
#define PARSER_MSG_ESC_MXJG_FUERA_RANGO				"Parser: Escenario - Atributo maximo-jugadores fuera de rango. Se carga valor por default."
#define PARSER_MSG_ESC_GRAVEDAD                     "Parser: Escenario - Atributo gravedad sin definir. Se carga valor por default."
#define PARSER_MSG_ESC_GRAVEDAD_NO_NUMBER			"Parser: Escenario - Atributo gravedad es un numero. Se carga valor por default."
#define PARSER_MSG_ESC_GRAVEDAD_POSITIVA			"Parser: Escenario - Atributo gravedad debe ser un numero negativo. Se carga valor por default."
#define PARSER_MSG_ESC_ALTO_PX                      "Parser: Escenario - Atributo alto-px sin definir. Se carga valor por default."
#define PARSER_MSG_ESC_ALTO_PX_NO_NUMBER            "Parser: Escenario - Atributo alto-px es un numero. Se carga valor por default."
#define PARSER_MSG_ESC_ALTO_PX_FUERA_RANGO          "Parser: Escenario - Atributo alto-px fuera de rango. Se carga valor por default."
#define PARSER_MSG_ESC_ANCHO_PX                     "Parser: Escenario - Atributo ancho-px sin definir. Se carga valor por default."
#define PARSER_MSG_ESC_ANCHO_PX_NO_NUMBER           "Parser: Escenario - Atributo ancho-px es un numero. Se carga valor por default."
#define PARSER_MSG_ESC_ANCHO_PX_FUERA_RANGO         "Parser: Escenario - Atributo ancho-px fuera de rango. Se carga valor por default."
#define PARSER_MSG_ESC_ALTO_UN                      "Parser: Escenario - Atributo alto-un sin definir. Se carga valor por default."
#define PARSER_MSG_ESC_ALTO_UN_NO_NUMBER            "Parser: Escenario - Atributo alto-un es un numero. Se carga valor por default."
#define PARSER_MSG_ESC_ALTO_UN_FUERA_RANGO          "Parser: Escenario - Atributo alto-un fuera de rango. Se carga valor por default."
#define PARSER_MSG_ESC_ANCHO_UN                     "Parser: Escenario - Atributo ancho-un sin definir. Se carga valor por default."
#define PARSER_MSG_ESC_ANCHO_UN_NO_NUMBER           "Parser: Escenario - Atributo ancho-un es un numero. Se carga valor por default."
#define PARSER_MSG_ESC_ANCHO_UN_FUERA_RANGO         "Parser: Escenario - Atributo ancho-un fuera de rango. Se carga valor por default."
#define PARSER_MSG_ESC_IMAGEN_FONDO                 "Parser: Escenario - Atributo imagen_fondo sin definir. Se carga valor por default."
#define PARSER_MSG_ESC_IMAGEN_FONDO_NO_STRING       "Parser: Escenario - Atributo imagen_fondo es de un tipo distinto a string. Se carga valor por default."
#define PARSER_MSG_ESC_IMAGEN_FONDO_NO_EXISTE       "Parser: Escenario - Atributo imagen_fondo no existe. Se carga valor por default."
#define PARSER_MSG_OBJ_ROT                          "Parser: Objeto - La Rotacion no fue definida. Se carga valor por default. \n"
#define PARSER_MSG_OBJ_ROT_NO_NUMBER                "Parser: Objeto - La Rotacion no es un numero. Se carga valor por default. \n"
#define PARSER_MSG_OBJ_ROT_FUERA_RANGO              "Parser: Objeto - La Rotacion esta fuera del rango permitido. Se carga valor por default. \n"
#define PARSER_MSG_OBJ_MASA                         "Parser: Objeto - La Masa no fue definida. Se carga valor por default. \n"
#define PARSER_MSG_OBJ_MASA_NO_NUMBER               "Parser: Objeto - La Masa no es un numero. Se carga valor por default. \n"
#define PARSER_MSG_OBJ_MASA_FUERA_RANGO             "Parser: Objeto - La Masa esta fuera del rango permitido. Se carga valor por default. \n"
#define PARSER_MSG_OBJ_BASE_MAYOR_MENOR_MENOR       "Parser: Objeto - La base mayor del objeto es menor a la base menor. Se intercambian. \n"

//INFO
#define PARSER_MSG_START_PARSING					"Parser: Comenzamos a parsear el archivo "
#define PARSER_MSG_DEFAULT_GAME						"Parser: Se intenta cargar un escenario default"
#define PARSER_MSG_JSON_OK							"Parser: Archivo JSON OK!"


#endif /* JSONPARSER_C_H_ */
