all: resize_comp	lighten_comp canny_comp dilation_comp panorama_comp main_comp face_comp
execute: run
opencv3: resize_comp_opencv3 lighten_comp_opencv3 canny_comp_opencv3 dilation_comp_opencv3 panorama_comp_opencv3 main_comp_opencv3 face_comp_opencv3
opencv3: resize_comp_opencv3 lighten_comp_opencv3 canny_comp_opencv3 dilation_comp_opencv3 panorama_comp_opencv3 main_comp_opencv3

resize_comp:
	g++ resize.cpp  `pkg-config --libs --cflags opencv4` -o resize
lighten_comp:
	g++ lighten_darken.cpp  `pkg-config --libs --cflags opencv4` -o lighten_darken
canny_comp:
	g++ canny_edge.cpp `pkg-config --libs --cflags opencv4` -o canny_edge
dilation_comp:
	g++ dilation_erosion.cpp `pkg-config --libs --cflags opencv4` -o dilation_erosion
panorama_comp:
	g++ panorama_stitching.cpp `pkg-config --libs --cflags opencv4` -o panorama_stitching
main_comp:
	g++ main.cpp `pkg-config --libs --cflags opencv4` -o main
face_comp:
	g++ facedetection_photo.cpp `pkg-config --libs --cflags opencv4` -o face_detection

resize:
	./resize
lighten:
	./lighten_darken
canny:
	./canny_edge
dilation:
	./dilation_erosion
panorama:
	./panorama_stitching Images/eiffel1.jpeg Images/eiffel2.jpeg
face:
	./face_detection
run:
	./main

resize_comp_opencv3:
	g++ resize.cpp  `pkg-config --libs --cflags opencv` -o resize
lighten_comp_opencv3:
	g++ lighten_darken.cpp  `pkg-config --libs --cflags opencv` -o lighten_darken
canny_comp_opencv3:
	g++ canny_edge.cpp `pkg-config --libs --cflags opencv` -o canny_edge
dilation_comp_opencv3:
	g++ dilation_erosion.cpp `pkg-config --libs --cflags opencv` -o dilation_erosion
panorama_comp_opencv3:
	g++ panorama_stitching.cpp `pkg-config --libs --cflags opencv` -o panorama_stitching
main_comp_opencv3:
	g++ main.cpp `pkg-config --libs --cflags opencv` -o main
face_comp_opencv3:
	g++ facedetection_photo.cpp `pkg-config --libs --cflags opencv` -o face_detection
