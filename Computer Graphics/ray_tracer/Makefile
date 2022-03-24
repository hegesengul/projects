final:
	g++ -O3 *.cpp -c -std=c++11 -march=native -ljpeg -lm
	g++ *.o -o raytracer -lpthread -ljpeg -lm

nothread:
	g++ -O3 *.cpp -c -std=c++11 -march=native -ljpeg -lm
	g++ *.o -o raytracer -ljpeg -lm
debug:
	g++ -g *.cpp -c -std=c++11 -march=native -ljpeg -lm
	g++ *.o -o raytracer -lpthread -ljpeg -lm

clean:
	rm -f *.o
	rm -f raytracer
	rm -f *.ppm