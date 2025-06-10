#!/bin/sh
set -e

if [ -z "${SAGE_HOME}" ]; then
    SAGE_HOME="$(pwd)"
fi
VENDOR_HOME="${SAGE_HOME}/.vendor"
SUNDER_HOME="${SAGE_HOME}/.sunder"

if [ -e "${VENDOR_HOME}" ]; then
    echo "error: \$VENDOR_HOME location ${VENDOR_HOME} already exists!"
    echo "Run \`rm -rf ${VENDOR_HOME}\` to remove the existing directory"
    exit 1
fi
if [ -e "${SUNDER_HOME}" ]; then
    echo "error: \$SUNDER_HOME location ${SUNDER_HOME} already exists!"
    echo "Run \`rm -rf ${SUNDER_HOME}\` to remove the existing directory"
    exit 1
fi

mkdir -p "${VENDOR_HOME}"

git clone --depth 1 --branch main \
    https://github.com/ashn-dot-dev/sunder.git "${VENDOR_HOME}/sunder"
(cd "${VENDOR_HOME}/sunder" && \
    make install SUNDER_HOME="${SUNDER_HOME}")

git clone --depth 1 --branch 5.5 \
    https://github.com/ashn-dot-dev/raylib.git "${VENDOR_HOME}/raylib"
git clone --depth 1 --branch main \
    https://github.com/ashn-dot-dev/raylib-sunder.git \
    "${VENDOR_HOME}/raylib-sunder"
(cd "${VENDOR_HOME}/raylib-sunder" && \
    make install \
        SUNDER_HOME="${SUNDER_HOME}" \
        RAYLIB_DIRECTORY="${VENDOR_HOME}/raylib")

git clone --depth 1 --branch main \
    https://github.com/emscripten-core/emsdk.git "${VENDOR_HOME}/emsdk"
(cd "${VENDOR_HOME}/emsdk" && ./emsdk install latest)
(cd "${VENDOR_HOME}/emsdk" && ./emsdk activate latest)
