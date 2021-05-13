
GDCM_VERSION=3.0.8

.DEFAULT_GOAL:=build

lib/gdcm-${GDCM_VERSION}.tar.gz:
	mkdir -p lib/
	wget -O lib/gdcm-${GDCM_VERSION}.tar.gz \
		https://sourceforge.net/projects/gdcm/files/gdcm%203.x/GDCM%20${GDCM_VERSION}/gdcm-${GDCM_VERSION}.tar.gz/download

lib/gdcm-${GDCM_VERSION}:
	rm -rf lib/gdcm-${GDCM_VERSION}
	mkdir -p lib/gdcm-${GDCM_VERSION}
	tar -xf lib/gdcm-${GDCM_VERSION}.tar.gz -C lib/

lib/gdcm-${GDCM_VERSION}/Makefile:
	rm -rf lib/gdcm-bin
	mkdir -p lib/gdcm-bin

	cmake -S lib/gdcm-${GDCM_VERSION} -B lib/gdcm-bin

lib/gdcm-bin/*.a:
	cd lib/gdcm-bin && make

download: lib/gdcm-${GDCM_VERSION}.tar.gz
extract: download lib/gdcm-${GDCM_VERSION}
cmake: extract lib/gdcm-${GDCM_VERSION}/Makefile
build: cmake lib/gdcm-bin/*.a