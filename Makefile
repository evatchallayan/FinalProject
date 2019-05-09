all: resize_comp	lighten_comp canny_comp dilation_comp
execute: resize lighten canny dilation

resize_comp:
	g++ resize.cpp  `pkg-config --libs --cflags opencv4` -o resize.o
lighten_comp:
	g++ lighten_darken.cpp  `pkg-config --libs --cflags opencv4` -o lighten_darken.o
canny_comp:
	g++ canny_edge.cpp `pkg-config --libs --cflags opencv4` -o canny_edge.o
dilation_comp:
	g++ dilation_erosion.cpp `pkg-config --libs --cflags opencv4` -o dilation_erosion.o



resize:
	./resize.o
lighten:
	./lighten_darken.o
canny:
	./canny_edge.o
dilation:
	./dilation_erosion.o
