# Line Follower Robot Simulator

A comprehensive web-based simulator for line-follower robots with realistic physics, intelligent optimization, and 3D visualization. The entire system runs in the browser using WebAssembly for performance-critical computations.

## Features

### Phase 1 (Current)
- **Visual Track Editor**: Create complex tracks with straight lines and curves
- **Robot Configuration**: Adjust physical parameters (mass, wheelbase, sensors, PID gains)
- **Physics Simulation**: Realistic dynamics using Box2D physics engine
- **3D Visualization**: Interactive Three.js-based 3D view with playback controls
- **Project Management**: Save/load projects locally using IndexedDB
- **Import/Export**: Share configurations via .lfsim files

### Phase 2 (Planned)
- **Artifact-Based Optimization**: Intelligent decomposition of tracks into geometric primitives
- **Analytical Solutions**: Provably optimal strategies for known track patterns
- **Hybrid Optimization**: Combines analytical methods with numerical optimization
- **Advanced Algorithms**: L-BFGS, MPC, Direct Collocation for complex sections
- **Performance Analysis**: Detailed breakdown of strategies applied per track section

### Phase 3 (Future)
- **Arduino Emulation**: Run actual Arduino code in the simulation

## Technology Stack

### Frontend
- **Svelte**: Reactive UI framework with minimal runtime overhead
- **Three.js**: 3D rendering and visualization
- **Dexie.js**: IndexedDB wrapper for local persistence
- **Vite**: Modern build tool with native WASM support

### Core Engine
- **C++17**: High-performance simulation and optimization
- **WebAssembly**: Near-native execution speed in the browser
- **Box2D**: 2D physics engine
- **Eigen**: Linear algebra library
- **Emscripten**: C++ to WebAssembly compiler

## Getting Started

### Prerequisites

- Node.js 18+ and npm
- Emscripten SDK (for compiling C++ to WASM)
- Modern web browser with WebAssembly support

### Installation

1. Clone the repository:
```bash
git clone https://github.com/frankalcantara/LineFollower.git
cd LineFollower/simulator
```

2. Install JavaScript dependencies:
```bash
npm install
```

3. Install Emscripten SDK:
```bash
# Follow instructions at https://emscripten.org/docs/getting_started/downloads.html
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

4. Download external C++ libraries:
```bash
cd cpp/external
# Box2D
git clone https://github.com/erincatto/box2d.git
# Eigen (header-only)
git clone https://gitlab.com/libeigen/eigen.git
```

### Development

Start the development server:
```bash
npm run dev
```

Build WebAssembly module:
```bash
npm run build:wasm
```

Build for production:
```bash
npm run build
```

### Project Structure

```
simulator/
├── src/              # Frontend Svelte application
│   ├── lib/
│   │   ├── components/   # UI components
│   │   ├── stores/       # State management
│   │   ├── storage/      # Persistence layer
│   │   └── three/        # 3D visualization
│   └── workers/      # Web Workers for background tasks
│
├── cpp/              # C++ WebAssembly engine
│   ├── include/      # Header files
│   ├── src/          # Implementation files
│   └── external/     # Third-party libraries
│
├── public/           # Static assets
└── dist/             # Build output
```

## Usage

### Creating a Track

1. Open the Track Editor
2. Click to add control points defining the track path
3. Adjust curve parameters for each segment
4. Set track width and line properties

### Configuring the Robot

1. Open Robot Configuration panel
2. Set physical parameters:
   - Mass and wheelbase
   - Sensor positions and sensitivity
   - Motor characteristics
3. Tune PID controller gains
4. Configure environmental factors (temperature, friction)

### Running Simulations

1. Click "Simulate" to run physics simulation
2. Use playback controls (play/pause/scrub)
3. Adjust camera view in 3D visualization
4. Monitor sensor readings and robot state in real-time

### Saving and Sharing

- **Save Project**: Stores in browser's IndexedDB
- **Export**: Download .lfsim file
- **Import**: Load .lfsim file
- **Share URL**: Generate compressed link (for small projects)

## Architecture

See [ARCHITECTURE.md](./ARCHITECTURE.md) for detailed architectural documentation, including:
- Three-layer architecture (UI, Simulation, Visualization)
- Artifact-based optimization strategy
- Communication between JavaScript and WebAssembly
- Performance considerations
- Development phases and roadmap

## Performance

### Targets
- **Simulation**: 1000+ simulations/second (batch mode), 60 FPS (step mode)
- **Rendering**: 60 FPS on modern hardware, 30 FPS minimum
- **Load Time**: <2 seconds initial, <500ms project loading

### Optimizations
- WebAssembly with -O3 optimization
- SIMD vectorization where available
- Memory pooling to avoid allocations
- Three.js instancing and LOD
- Code splitting and lazy loading

## Browser Support

- Chrome/Edge 90+
- Firefox 88+
- Safari 14+
- Mobile browsers (limited functionality)

## Contributing

Contributions are welcome! Areas of interest:
- New track artifacts and analytical solutions
- Additional optimization algorithms
- UI/UX improvements
- Documentation and tutorials
- Bug fixes and performance improvements

## License

MIT License - see [LICENSE](../LICENSE) file for details

## Acknowledgments

- **Box2D**: Erin Catto and contributors
- **Eigen**: Benoît Jacob, Gaël Guennebaud, and contributors
- **Emscripten**: Alon Zakai and contributors
- **Three.js**: Ricardo Cabello (mrdoob) and contributors
- **Svelte**: Rich Harris and contributors

## References

- [Racing Line Theory](https://en.wikipedia.org/wiki/Racing_line)
- [PID Controller Tuning](https://en.wikipedia.org/wiki/PID_controller)
- [Mobile Robot Dynamics](https://www.roboticsbook.org/)
- [WebAssembly Documentation](https://webassembly.org)

## Support

For issues, questions, or suggestions:
- Open an issue on [GitHub](https://github.com/frankalcantara/LineFollower/issues)
- Check existing documentation in `/docs`

---

**Project Status**: Phase 1 - Initial Development
**Last Updated**: 2025-11-05
