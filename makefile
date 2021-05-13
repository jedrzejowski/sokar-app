
GDCM_VERSION=3.0.8

gdcm:

gdcm-download:
	mkdir -p lib/
	wget -O lib/gdcm-${GDCM_VERSION}.tar.gz \
		https://sourceforge.net/projects/gdcm/files/gdcm%203.x/GDCM%20${GDCM_VERSION}/gdcm-${GDCM_VERSION}.tar.gz/download

gdcm-extract:
	rm -rf lib/gdcm-${GDCM_VERSION}
	mkdir -p lib/gdcm-${GDCM_VERSION}
	tar -xf lib/gdcm-${GDCM_VERSION}.tar.gz -C lib/

gdcm-cmake:
	rm -rf lib/gdcm-bin
	mkdir -p lib/gdcm-bin

	cmake -S lib/gdcm-${GDCM_VERSION} -B lib/gdcm-bin

gdcm-make:
	cd lib/gdcm-bin && make
