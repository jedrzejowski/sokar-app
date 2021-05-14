
GDCM_VERSION=3.0.8

.DEFAULT_GOAL:=build

SRC_DIR=lib/gdcm-${GDCM_VERSION}
TARBALL=lib/gdcm-${GDCM_VERSION}.tar.gz
BIN_DIR=lib/gdcm-${GDCM_VERSION}-bin

lib/gdcm-${GDCM_VERSION}.tar.gz:
	mkdir -p lib/
	wget -O ${TARBALL} \
		https://sourceforge.net/projects/gdcm/files/gdcm%203.x/GDCM%20${GDCM_VERSION}/gdcm-${GDCM_VERSION}.tar.gz/download

${SRC_DIR}:
	rm -rf "${SRC_DIR}"
	tar -xf "${TARBALL}" -C lib/

${BIN_DIR}/Makefile:
	rm -rf "${BIN_DIR}"
	mkdir -p "${BIN_DIR}"

	cmake -S "${SRC_DIR}" -B "${BIN_DIR}"

${BIN_DIR}/.done:
	cd "${BIN_DIR}" && make
	touch "${BIN_DIR}"/.done

lib/gdcm-bin:
	ln -s gdcm-${GDCM_VERSION}-bin lib/gdcm-bin

download: ${TARBALL}
extract: download ${SRC_DIR}
cmake: extract ${BIN_DIR}/Makefile
build: cmake ${BIN_DIR}/.done lib/gdcm-bin

clear:
	rm -f lib/gdcm-bin "${BIN_DIR}"/.done