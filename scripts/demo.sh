#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"

cmake -S "${ROOT_DIR}/ParticleSimulation" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" --config Release

if [[ -x "${BUILD_DIR}/ParticleSimulation" ]]; then
  exec "${BUILD_DIR}/ParticleSimulation"
fi

if [[ -x "${BUILD_DIR}/Release/ParticleSimulation" ]]; then
  exec "${BUILD_DIR}/Release/ParticleSimulation"
fi

echo "Build succeeded, but executable not found in expected paths." >&2
exit 1
