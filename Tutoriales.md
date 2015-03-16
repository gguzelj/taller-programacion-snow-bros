<h1>Resumen</h1>
<p>Este espacio es creado para volcar anotaciones o links a tutoriales de librerias o configuraciones utilizadas en el proyecto </p>

<p> Tener en cuenta que es necesario instalar otras dependencias para que no falle:</p>

<li> sudo apt-get install libxi-dev </li>
<li> sudo apt-get install libgl1-mesa-dev </li>
<li> sudo apt-get install libglu1-mesa-dev </li>
<li> sudo apt-get install cmake </li>
<li> sudo apt-get install build-essentials </li>



<h2>Instalacion</h2>
<h3>Box2D</h3>
<ol>
<li> Descargar la ultima version (<a href='https://code.google.com/p/box2d/downloads/list'>https://code.google.com/p/box2d/downloads/list</a>) y luego realizar lo siguiente:</li>
<li>Ubicarse en la carpeta build dentro del directorio que descomprime el rar.</li>
<li>Tipear los siguientes comandos:</li>
<li>cmake ../</li>
<li>make</li>
<li>sudo make install</li>
<li>sudo ldconfig</li>
</ol>

<h3>SDL2</h3>
<p> <a href='http://nothingtocode.blogspot.com.ar/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html'>http://nothingtocode.blogspot.com.ar/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html</a></p>
<p> <a href='http://www.squarebitstudios.tk/forum/showthread.php?tid=7'>http://www.squarebitstudios.tk/forum/showthread.php?tid=7</a></p>

<h3>SDL2_image</h3>
<p> <a href='http://nothingtocode.blogspot.com.ar/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html'>http://nothingtocode.blogspot.com.ar/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html</a></p>

<h3>SDL2_ttl</h3>
<p> <a href='http://nothingtocode.blogspot.com.ar/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html'>http://nothingtocode.blogspot.com.ar/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html</a></p>

<h3>SDL2_gfx</h3>
<p> <a href='http://cms.ferzkopp.net/index.php/software/13-sdl-gfx'>http://cms.ferzkopp.net/index.php/software/13-sdl-gfx</a> </p>

<h3>JSON</h3>
<ol>
<li>git clone <a href='https://github.com/open-source-parsers/jsoncpp.git'>https://github.com/open-source-parsers/jsoncpp.git</a></li>
<li>cd jsoncpp</li>
<li>mkdir -p build/debug</li>
<li>cd build/debug</li>
<li>cmake -DCMAKE_BUILD_TYPE=debug -DJSONCPP_LIB_BUILD_SHARED=OFF -G "Unix Makefiles" ../../</li>
<li>make</li>
<li>sudo make install</li>
</ol>

<h3>Gtest</h3>
<p>Framework de pruebas unitarias para C++</p>
<p><a href='https://docs.google.com/document/d/1RbMsmjfXpFrTQOJMRsacNCWCR_Jej9G8uLQwpcRNa4M/edit?usp=sharing'>https://docs.google.com/document/d/1RbMsmjfXpFrTQOJMRsacNCWCR_Jej9G8uLQwpcRNa4M/edit?usp=sharing</a></p>

<h2>Tutoriales de uso</h2>
<h3>SDL</h3>
<p> <a href='http://www.willusher.io/pages/sdl2/'>http://www.willusher.io/pages/sdl2/</a> </p>

<h3>Box2D</h3>
<p> <a href='http://www.iforce2d.net/b2dtut/'>http://www.iforce2d.net/b2dtut/</a> </p>

<h3>Integracion de SDL con Box2D</h3>
<p><a href='http://binarymillenium.googlecode.com/svn-history/r852/trunk/box2d/box2dsdl/HelloWorld.cpp'>http://binarymillenium.googlecode.com/svn-history/r852/trunk/box2d/box2dsdl/HelloWorld.cpp</a></p>


<h3>Dibujar y pintar circulos</h3>
<p><a href='http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles'>http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles</a></p>

<h3>Patrones de diseño</h3>
<p><a href='http://gameprogrammingpatterns.com/'>http://gameprogrammingpatterns.com/</a></p>