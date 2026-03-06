# Particle Simulation

Real-time 3D particle simulation built with C++, OpenGL, GLFW, GLAD, and GLM.

## Features

- 3D particles with mass and radius
- Velocity Verlet integration
- Inverse-square gravity
- Elastic particle collisions
- Boundary sphere containment
- Free-look camera

## Cross-Platform Demo

The project now supports macOS, Linux, and Windows with a one-command demo flow.

### Usage

From the repository root:

- macOS/Linux:
  - `./scripts/demo.sh`
- Windows (PowerShell):
  - `./scripts/demo.ps1`

## GitHub Actions Artifacts

The CI workflow builds binaries for all three operating systems and publishes artifacts:

- `ParticleSimulation-macOS`
- `ParticleSimulation-Linux`
- `ParticleSimulation-Windows`

Anyone can download the latest successful workflow artifacts from the Actions tab and run the executable for their OS.
