#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
DEPS_DIR="${WASM_DEPS_DIR:-${PROJECT_DIR}/wasm-deps}"
INSTALL_PREFIX="${WASM_INSTALL_PREFIX:-${DEPS_DIR}/install}"
BUILD_DIR="${WASM_BUILD_DIR:-${PROJECT_DIR}/build-wasm}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
RUN_TESTS="${RUN_TESTS:-1}"

if [[ -z "${EMSCRIPTEN:-}" ]]; then
  if [[ -f "${EMSDK:-/opt/emsdk}/emsdk_env.sh" ]]; then
    # shellcheck source=/dev/null
    source "${EMSDK:-/opt/emsdk}/emsdk_env.sh"
  else
    echo "Emscripten not found. Install the emsdk and source emsdk_env.sh." >&2
    exit 1
  fi
fi

if [[ ! -f "${INSTALL_PREFIX}/lib/libmpfr.a" ]]; then
  echo "MPFR for WebAssembly not found. Building dependencies..."
  "${SCRIPT_DIR}/build-wasm-deps.sh"
fi

emcmake cmake \
  -B "${BUILD_DIR}" \
  -S "${PROJECT_DIR}" \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
  -DMPFR_ROOT="${INSTALL_PREFIX}"

emmake cmake --build "${BUILD_DIR}" --parallel "$(nproc)"
emmake cmake --install "${BUILD_DIR}"

if [[ "${RUN_TESTS}" == "1" ]]; then
  ctest --test-dir "${BUILD_DIR}" --output-on-failure --verbose
fi

echo "WebAssembly build installed to ${INSTALL_PREFIX}"
