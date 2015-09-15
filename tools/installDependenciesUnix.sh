#!/bin/bash
	
	rm -r ../dependencies
        mkdir ../bin
	mkdir ../dependencies
	mkdir ../dependencies/GL
	mkdir ../dependencies/src
	mkdir ../dependencies/libs
	rm -r temp
	mkdir temp
	cd temp

	# GLFW
	git clone https://github.com/glfw/glfw.git
	cd glfw
	cmake .
	make -j5
	cp -r include/GLFW/* "../../../dependencies/GL"
	cp "src/libglfw3.a" "../../../dependencies/libs/libglfw3.a"
	cd ../

	# Bullet
	curl -o bullet.tgz https://bullet.googlecode.com/files/bullet-2.82-r2704.tgz
	tar -zxvf bullet.tgz
	cd bullet-2.82-r2704
	cmake . -DBUILD_EXTRAS=off -DBUILD_DEMOS=off
	make -j5
	cp -r "src/BulletCollision" "../../../dependencies/"
	cp -r "src/BulletDynamics" "../../../dependencies/"
	cp -r "src/LinearMath" "../../../dependencies/"
	cp "src/BulletCollision/libBulletCollision.a" "../../../dependencies/libs/libBulletCollision.a"
	cp "src/BulletDynamics/libBulletDynamics.a" "../../../dependencies/libs/libBulletDynamics.a"
	cp "src/LinearMath/libLinearMath.a" "../../../dependencies/libs/libLinearMath.a"
	cp "src/btBulletCollisionCommon.h" "../../../dependencies/btBulletCollisionCommon.h"
	cp "src/btBulletDynamicsCommon.h" "../../../dependencies/btBulletDynamicsCommon.h"
	cd ../
	
	# duktape (javascript engine)
	curl -o "duktape.tar.xz" http://www.duktape.org/duktape-0.9.0.tar.xz
	tar -xJf duktape.tar.xz
	cd duktape-0.9.0/src
	gcc -c -Os -std=c99 duktape.c -lm
	cp "duktape.h" "../../../../dependencies/duktape.h"
	cp "duktape.o" "../../../../dependencies/duktape.o"
	cd ../../

	# GLEW
	git clone https://github.com/nigels-com/glew.git glew
	cd glew
	make extensions
	make -j5
	cp -r include/GL/* "../../../dependencies/GL/"
	cp "lib/libGLEW.a" "../../../dependencies/libs/libGLEW.a"
	cd ../
	
	# drawlib and lodepng
	curl -o "drawlib.h" http://cameron.tylerkline.com/drawlib.h
	curl -o "drawlib.cpp" http://cameron.tylerkline.com/drawlib.cpp
	curl -o "lodepng.h" http://lpi.googlecode.com/svn/trunk/lodepng.h
	curl -o "lodepng.cpp" http://lpi.googlecode.com/svn/trunk/lodepng.cpp
	cp "drawlib.h" "../../dependencies/drawlib.h"
	cp "drawlib.cpp" "../../dependencies/src/drawlib.cpp"
	cp "lodepng.h" "../../dependencies/lodepng.h"
	cp "lodepng.cpp" "../../dependencies/src/lodepng.cpp"
	
	# GLM
	git clone https://github.com/g-truc/glm.git
	cd glm
	cp -r "glm" "../../../dependencies/"
	cd ../../
	
	rm -Rf temp
	
	
