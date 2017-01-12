all-w: clean-all build-all-w

all-u: clean-all build-all-u

launch:
	cd bin && ./demo && cd ..

build-app:
	cd build && make -s && cd ..
	
clean:
	cd build && make clean && cd ..
	
clean-all:
	rm -rf build/*
	
build-all-w:
	cd build && cmake .. -G "MSYS Makefiles" && make -s && cd ..

build-all-u:
	cd build && cmake .. -G "Unix Makefiles" && make -s && cd ..