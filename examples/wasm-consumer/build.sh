#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"
INSTALL_PREFIX="${PEELO_NUMBER_PREFIX:-${PROJECT_DIR}/wasm-deps/install}"
BUILD_DIR="${BUILD_DIR:-${SCRIPT_DIR}/build}"

if [[ -z "${EMSCRIPTEN:-}" ]]; then
  if [[ -f "${EMSDK:-/opt/emsdk}/emsdk_env.sh" ]]; then
    # shellcheck source=/dev/null
    source "${EMSDK:-/opt/emsdk}/emsdk_env.sh"
  else
    echo "Emscripten not found. Install the emsdk and source emsdk_env.sh." >&2
    exit 1
  fi
fi

if [[ ! -f "${INSTALL_PREFIX}/lib/libPeeloNumber.a" ]]; then
  echo "PeeloNumber for WebAssembly not found at ${INSTALL_PREFIX}." >&2
  echo "Build it first from the repository root: ./scripts/build-wasm.sh" >&2
  exit 1
fi

emcmake cmake \
  -B "${BUILD_DIR}" \
  -S "${SCRIPT_DIR}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="${INSTALL_PREFIX}" \
  -DPeeloNumber_DIR="${INSTALL_PREFIX}/lib/cmake/PeeloNumber"

emmake cmake --build "${BUILD_DIR}"

echo
node "${BUILD_DIR}/example.js"
