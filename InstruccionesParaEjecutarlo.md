<h2> Librerías </h2>
<h3>Incluir en el server las siguientes librerías:</h3>
<ul>
<li> Box2D </li>
<li> jsoncpp </li>
<li> SDL2 </li>
</ul>

<h3>Incluir en el cliente las siguientes librerías:</h3>
<ul>
<li> SDL2 </li>
<li> SDL2_gfx </li>
<li> SDL2_image </li>
<li> SDL2_ttf </li>
<li> SDL2_mixer </li>
</ul>

<h4>Compilar con c++11 y pthread</h4>
<h4>Tener instalado el siguiente plugin para MIDI: vlc-plugin-fluidsynth</h4>

<h2> Parametros del ejecutable </h2>
<h3> Server </h3>
ejecutable path/to/json puerto

Ej:
./SnowBrosServer resources/config.json 9090

<h3> Cliente </h3>
ejecutable ipDelServer puerto userId

Ej:
./SnowBrosClient localhost 9090 lea