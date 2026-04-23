SRC = neighbor.C spheres.C box.C sphere.C event.C heap.C read_input.C

spheres : $(SRC)
	g++ -O3 $(SRC) -o spheres

clean :
	rm -f spheres
