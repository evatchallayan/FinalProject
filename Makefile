all: resize_comp	lighten_comp canny_comp dilation_comp panorama_comp
execute: resize lighten canny dilation
opencv3: resize_comp_opencv3	lighten_comp_opencv3 canny_comp_opencv3 dilation_comp_opencv3 panorama_comp_opencv3

resize_comp:
	g++ resize.cpp  `pkg-config --libs --cflags opencv4` -o resize.o
lighten_comp:
	g++ lighten_darken.cpp  `pkg-config --libs --cflags opencv4` -o lighten_darken.o
canny_comp:
	g++ canny_edge.cpp `pkg-config --libs --cflags opencv4` -o canny_edge.o
dilation_comp:
	g++ dilation_erosion.cpp `pkg-config --libs --cflags opencv4` -o dilation_erosion.o
panorama_comp:
	g++ panorama_stitching.cpp `pkg-config --libs --cflags opencv4` -o panorama_stitching.o


resize:
	./resize.o
lighten:
	./lighten_darken.o
canny:
	./canny_edge.o
dilation:
	./dilation_erosion.o
panorama:
	./panorama_stitching.o /Images/eiffel1.jpeg Images/eiffel2.jpeg

resize_comp_opencv3:
	g++ resize.cpp  `pkg-config --libs --cflags opencv` -o resize.o
lighten_comp_opencv3:
	g++ lighten_darken.cpp  `pkg-config --libs --cflags opencv` -o lighten_darken.o
canny_comp_opencv3:
	g++ canny_edge.cpp `pkg-config --libs --cflags opencv` -o canny_edge.o
dilation_comp_opencv3:
	g++ dilation_erosion.cpp `pkg-config --libs --cflags opencv` -o dilation_erosion.o
panorama_comp_opencv3:
	g++ panorama_stitching.cpp `pkg-config --libs --cflags opencv` -o panorama_stitching.o
