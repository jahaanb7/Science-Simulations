$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$buildDir = Join-Path $root 'build'

cmake -S (Join-Path $root 'ParticleSimulation') -B $buildDir -DCMAKE_BUILD_TYPE=Release
cmake --build $buildDir --config Release

$releaseExe = Join-Path $buildDir 'Release/ParticleSimulation.exe'
$directExe = Join-Path $buildDir 'ParticleSimulation.exe'

if (Test-Path $releaseExe) {
    & $releaseExe
    exit $LASTEXITCODE
}

if (Test-Path $directExe) {
    & $directExe
    exit $LASTEXITCODE
}

throw 'Build succeeded, but executable not found in expected paths.'
