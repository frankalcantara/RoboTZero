# Line Follower Simulator Architecture

## Overview

A browser-based simulation and optimization system for line-follower robots that runs entirely in the web browser. The project is fully static, hosted on GitHub Pages, allowing users to create tracks, configure robots, execute realistic physics simulations, and find optimal configurations through optimization algorithms.

## Project Goals

- Enable visual creation and editing of complex tracks with curves, straights, and artifacts
- Simulate realistic robot physics considering mass, friction, temperature, pressure, and other environmental variables
- Find optimal control configurations for maximum efficiency in each track section
- Visualize competition in 3D with playback controls
- Save and share projects, tracks, and robot configurations
- Function completely offline after initial load

## Three-Layer Architecture

### Layer 1: User Interface

**Technology:** JavaScript/Svelte

**Rationale:** Svelte compiles components to optimized vanilla JavaScript without runtime overhead, offering efficient reactivity through simple assignments, ideal for updating UI with continuous simulation data.

**Responsibilities:**
- Visual track editor with curve and segment manipulation
- Configuration panels for robot and simulation parameters
- Animation playback controls (play, pause, timeline, speed)
- Saved project management and export/import interface
- Visualization of optimization results and convergence graphs

**State Management:**
- Svelte Stores for global state shared between components
- Reactive local state in each component
- Automatic synchronization with IndexedDB for persistence

### Layer 2: Simulation and Optimization

**Technology:** C++ compiled to WebAssembly via Emscripten

**Rationale:** Near-native performance (80-95%) for intensive physics and optimization calculations, with full control over memory allocation and predictable execution.

#### Physics Module

**Library:** Box2D (official WebAssembly port)

**Rationale:** Complete 2D rigid body physics engine with collision detection and stable numerical integration, eliminating the need to implement subtle stability details.

**Implementation:**
- Dynamic robot model with two independent motors (differential drive)
- Kinematic calculations considering mass, inertia, and geometry
- Friction simulation between wheels and surface
- Collision detection with track boundaries
- Infrared sensor system simulating analog line reading

**Environmental Factors:**
- Temperature: affects friction and motor response
- Atmospheric pressure: influences air density and drag
- Robot weight: determines inertia and grip
- Friction coefficient: varies by surface type

#### Mathematics Module

**Library:** Eigen

**Rationale:** Header-only linear algebra library that works perfectly with Emscripten, offering optimized matrix operations and solvers essential for optimization algorithms.

**Usage:**
- Vector and matrix operations for kinematic calculations
- Linear system solvers
- Geometric transformation calculations

#### Optimization Module

**Strategy:** Artifact-Based Hierarchical Decomposition

**Rationale:** Instead of optimizing the entire track as a complex global problem, the system decomposes the track into known geometric primitives (artifacts) and applies specific analytical or semi-analytical solutions for each type. This approach drastically reduces the search space, provides provably optimal solutions for known cases, and allows interpretability of applied strategies.

##### Phase 1: Pattern Recognition

**Pattern Recognizer:** Analyzes track geometry and identifies known artifacts.

**Supported Artifacts in Library:**
- Simple straight: linear segment without curvature
- Circular curve: arc of circle with constant radius
- Straight-curve-straight transition: common sequence in racing tracks
- S-Curve: two consecutive curves in opposite directions
- Chicane: rapid sequence of alternating curves
- Hairpin: tight 180-degree turn
- Spiral: curve with continuously varying radius

**Recognition Process:**
1. Scan track segments sequentially
2. Attempt match with patterns from most specific to most generic
3. When match found, mark segments as part of artifact
4. Unrecognized segments marked as "complex artifacts" for numerical optimization

##### Phase 2: Analytical Strategy Library

Each artifact in the library has a specific optimization strategy based on established physical principles and control theory.

**Straight:**
- Strategy: accelerate to maximum robot speed and maintain
- Instantaneous calculation, trivially optimal solution

**Circular Curve:**
- Maximum speed: `v_max = sqrt(μ * g * R)` where μ is friction coefficient, g gravity, R radius
- Racing line: enter wide, clip apex (innermost point), exit wide to maximize effective radius
- Analytical solution based on classical vehicle dynamics

**Straight-Curve-Straight Transition:**
- Braking distance: `d = (v_initial² - v_curve²) / (2 * a_max)`
- Braking start point calculated analytically
- Strategy: brake as late as possible before curve, clip apex, accelerate as early as possible on exit
- Based on optimal racing line theory

**S-Curve and Chicane:**
- Smoothed trajectory to minimize abrupt direction changes
- Modulated speed to maintain grip in transitions
- Uses Bézier splines or clothoids for smooth transitions

**Hairpin:**
- Very reduced speed at apex
- Wide entry, late apex, accelerate on exit
- Calculation based on robot's minimum turning radius

**Spiral:**
- Speed inversely proportional to local curvature
- Racing line follows varying radius
- Calculation by integration along curve

##### Phase 3: Transition Optimization

**Reduced Problem:** For N artifacts, optimize only N-1 transition velocities between consecutive artifacts.

**Approach:**
- Intelligent initialization using physical estimates
- Optimization by numerical (finite differences) or analytical gradient if available
- Problem typically with 5-15 variables (much smaller than global optimization)
- Fast convergence due to reduced search space

**Algorithm:**
1. Calculate optimal local strategy for each artifact assuming initial transition velocities
2. Optimize transition velocities to minimize total time
3. Recalculate local strategies with optimized velocities
4. Iterate until convergence (typically 3-5 iterations)

##### Phase 4: Complex Artifact Optimization

For geometries that don't fit known patterns, apply advanced numerical optimization techniques only locally to that artifact.

**Available Techniques:**

**Gradient-Based Optimization:**
- L-BFGS: quasi-Newton method without Hessian requirement, fast convergence
- Gradient Descent with momentum: for cases with noisy objective function
- Automatic Differentiation via CppAD: calculates exact gradients without approximations
- Finite Differences: numerical approximation when automatic differentiation not applicable

**Model Predictive Control (MPC):**
- Repeatedly solves finite-horizon optimization problem
- Suitable for artifacts with complex constraints or variable geometry
- Allows incorporation of future state prediction
- Used in modern automotive control and mobile robotics

**Sequential Quadratic Programming (SQP):**
- For nonlinear problems with equality and inequality constraints
- Approximates problem locally by quadratic program
- Superlinear convergence when near solution

**Direct Collocation:**
- Discretizes trajectory into control points
- Transforms optimal control problem into nonlinear parameter optimization
- Effective for complex trajectories with multiple constraints

**Shooting Methods:**
- Integrates dynamics equations forward with candidate controls
- Adjusts controls to satisfy boundary conditions
- Single shooting (simple) or Multiple shooting (more robust)

**Fallback - Stochastic Algorithms:**
Only when deterministic methods fail due to multiple local minima:
- Particle Swarm Optimization (PSO): generally faster than GA
- Genetic Algorithm: robust but computationally expensive
- Simulated Annealing: for highly non-convex search spaces

##### Optimizable Parameters

**Artifact Level:**
- Entry and exit velocity
- Braking/acceleration start points
- Trajectory (racing line) for curves
- Maneuver aggressiveness

**Global Level:**
- PID controller gains (Kp, Ki, Kd) via established tuning methods (Ziegler-Nichols, Cohen-Coon)
- Maximum robot speed in different environmental conditions
- Safety margins (minimum distance from edges)

##### Objective Function

**Primary:** Total time to complete track

**Secondary (weighted):**
- Trajectory smoothness: penalizes abrupt accelerations and oscillations
- Energy consumption: power integral along path
- Safety margin: penalizes excessive proximity to edges
- Mechanical wear: penalizes maneuvers that stress components

##### Advantages of Artifact-Based Approach

**Computational Efficiency:**
- 80-95% reduction in number of variables to optimize
- Analytical solutions are instantaneous (microseconds)
- Numerical optimization applied only where necessary

**Interpretability:**
- User sees specific strategy applied to each artifact
- Detailed feedback: "In this 8m radius curve, limited by grip to 1.2 m/s"
- Educational value about vehicle dynamics and racing strategies

**Robustness:**
- Analytical solutions are provably optimal within physical constraints
- No risk of convergence to poor local minima in known artifacts
- Deterministic and reproducible results

**Scalability:**
- Artifact library can grow incrementally
- Advanced users can contribute new artifacts
- Each artifact is independently testable module

#### Operation Modes

**Batch Mode (Optimization):**
- Executes thousands of complete simulations without visualization
- Returns only final results (fitness, time, parameters)
- Optimized for maximum execution speed
- Runs in Web Worker to avoid blocking interface

**Step Mode (Visualization):**
- Advances simulation incrementally synchronized with animation
- Returns complete state at each step (position, velocity, sensors)
- Generates complete state log for later playback
- Allows fine control of simulation rate

### Layer 3: 3D Visualization

**Technology:** Three.js

**Rationale:** Mature and performant library for 3D rendering in browser with extensive ecosystem and documentation.

**Scene Components:**
- Track surface (plane or textured mesh)
- Black line as extruded geometry or texture
- Robot 3D model (simple geometry with orientation indication)
- Sensor visualization (rays or cones showing detection)
- Configurable camera (free perspective or following robot)
- Lighting and shadows for visual depth

**Playback System:**
- Simulation runs independently generating complete state log
- Animation plays back log synchronized with browser clock
- Controls: play, pause, stop, timeline scrubbing
- Adjustable speed (slow motion, real time, accelerated)
- Optimized storage via keyframes with interpolation

**Performance Optimization:**
- Instanced rendering for repetitive track elements
- LOD (Level of Detail) for distant objects
- Automatic frustum culling from Three.js
- Stats monitor for performance debugging

## Inter-Component Communication

### JavaScript ↔ WebAssembly Binding

**Technology:** Embind (provided by Emscripten)

**Structure:**
```
Svelte Interface (JS)
       ↕
   Embind API
       ↕
Core Engine (C++ WASM)
```

**Exposed Types:**
- RobotConfig: complete robot configuration
- TrackSegment: track segment definition
- SimulationState: instantaneous simulation state
- OptimizationResult: optimization process results

**Transfer Optimization:**
- Shared memory via TypedArrays for frequent data
- JSON serialization for sporadic configurations
- Reusable buffers to avoid repeated allocations

### Asynchronous Execution

**Web Workers:**
- Long optimization operations run in dedicated workers
- Interface remains responsive during intensive processing
- Communication via postMessage with serializable data
- Progress callbacks for progress bar updates

## Persistence and Sharing

### Local Storage

**Technology:** IndexedDB via Dexie.js

**Rationale:** Dexie.js provides simplified wrapper for IndexedDB with Promise-based API and advanced query capabilities.

**Schemas:**
```javascript
projects: "++id, name, modified"
tracks: "++id, name, created"
robots: "++id, name, created"
simulations: "++id, projectId, timestamp"
```

**Capacity:**
- 50MB to several GB depending on browser
- Sufficient for hundreds of complex projects

**Compaction Strategy:**
- Simulation logs stored as keyframes
- Gzip compression for large data
- Automatic cleanup of old data (optional)

### File Format

**Extension:** .lfsim (structured JSON)

**Structure:**
```json
{
  "version": "1.0",
  "metadata": {
    "name": "string",
    "author": "string",
    "created": "timestamp",
    "modified": "timestamp"
  },
  "track": {
    "width": "number",
    "height": "number",
    "segments": [...]
  },
  "robot": {
    "mass": "number",
    "wheelbase": "number",
    "sensors": {...},
    "pid": {...}
  },
  "simulation": {
    "parameters": {...},
    "results": {...}
  }
}
```

### URL Sharing

**Technology:** Pako (gzip compression)

**Process:**
1. Serialize project to JSON
2. Compress with gzip (Pako)
3. Encode in Base64
4. Include in URL query parameter

**Practical Limit:**
- URLs up to ~2KB work in all browsers
- URLs up to ~8KB work in most cases
- Large projects require file export

## Complete Technology Stack

### Frontend

**Runtime:**
- Svelte: Reactive UI framework
- JavaScript/TypeScript: Primary language
- Three.js: 3D rendering
- Dexie.js: IndexedDB wrapper
- Pako: Gzip compression

**Build Tools:**
- Vite: Modern bundler with native WASM support
- TypeScript (optional): Static typing
- ESLint: JavaScript code linting
- Prettier: Code formatting

### Core Engine

**Language:** C++17

**Libraries:**
- Box2D: 2D physics engine
- Eigen: Linear algebra (header-only)
- Emscripten: WASM toolchain
- CppAD (optional): Automatic differentiation for gradient-based optimization
- NLopt (optional): Nonlinear optimization algorithm library

**Build Tools:**
- CMake: Build system
- Emscripten SDK (emsdk): C++ → WASM compiler

**Compilation Flags:**
```
-O3                          # Maximum optimization
-s WASM=1                    # Generate WebAssembly
-s MODULARIZE=1              # ES6 module
-s ALLOW_MEMORY_GROWTH=1     # Dynamic memory
-s INITIAL_MEMORY=16MB       # Initial memory
-s MAXIMUM_MEMORY=512MB      # Memory limit
--bind                       # Enable Embind
```

### Deployment

**Hosting:** GitHub Pages

**CI/CD:** GitHub Actions

**Pipeline:**
1. Compile C++ to WASM via Emscripten
2. Bundle JavaScript/Svelte via Vite
3. Optimize assets (images, fonts)
4. Deploy to gh-pages branch

**Requirements:**
- Public repository or GitHub Pro
- GitHub Actions enabled
- GitHub Pages configuration pointing to gh-pages branch

## Development Phases

### Phase 1: Foundation and Basic Simulation

**Objective:** Create functional system with abstract control model

**Components:**
1. Basic Svelte interface
2. Physical simulator in C++/WASM
3. 3D visualization with Three.js
4. Basic persistence

**Deliverable:** Functional simulator where users can create tracks, configure robots, and visualize simulations.

**Estimated Duration:** 2-3 months

### Phase 2: Artifact-Based Optimization

**Objective:** Implement hierarchical optimization system with artifact decomposition

**Components:**
1. Pattern Recognition System
2. Artifact Library
3. Transition Optimizer
4. Advanced Optimizers for Complex Artifacts
5. Optimization Interface
6. Simulation Improvements
7. Sharing

**Deliverable:** Complete optimization system that decomposes tracks into artifacts, applies analytical solutions where possible, and optimizes numerically only when necessary.

**Estimated Duration:** 3-4 months

### Phase 3: Arduino Emulation (Future)

**Objective:** Allow execution of real Arduino code in simulation

**Deliverable:** System that allows development and testing of real Arduino code in simulator.

**Estimated Duration:** 3-6 months (depending on approach)

## Performance Considerations

### Performance Targets

**Simulation:**
- Batch mode: 1000+ simulations/second (optimization)
- Step mode: 60 FPS with stable physics

**Rendering:**
- 60 FPS constant on modern hardware
- 30 FPS minimum on older hardware
- Graceful quality degradation if necessary

**Loading:**
- Initial download: <5 MB
- Initialization time: <2 seconds
- Project loading: <500 ms

### Planned Optimizations

**C++ WASM:**
- Compilation with -O3 and link-time optimization
- SIMD for vector operations when available
- Memory pools to avoid repeated allocations
- Profile-guided optimization in hot paths

**Three.js:**
- Geometry instancing for repeated elements
- Texture atlases to reduce draw calls
- Object pooling to avoid creation/destruction
- Frustum culling and occlusion culling

**JavaScript:**
- Code splitting for on-demand loading
- Tree shaking to eliminate unused code
- Gzip/brotli compression of assets
- Service Worker for offline cache

## System Requirements

### Supported Browsers

- Chrome/Edge 90+
- Firefox 88+
- Safari 14+
- Modern mobile browsers (limited functionality)

### Hardware Requirements

**Minimum:**
- CPU: Dual-core 2.0 GHz
- RAM: 4 GB
- GPU: WebGL 2.0 support

**Recommended:**
- CPU: Quad-core 3.0 GHz+
- RAM: 8 GB+
- GPU: Dedicated with WebGL 2.0 support

## Licensing

**Own Code:** To be defined (MIT or GPL suggested)

**Dependencies:**
- Svelte: MIT License
- Three.js: MIT License
- Box2D: MIT License
- Eigen: MPL2 License
- Emscripten: MIT/LLVM License
- Dexie.js: Apache License 2.0
- Pako: MIT License

---

**Document Version:** 1.0
**Last Updated:** 2025-11-05
**Status:** Architecture defined, awaiting implementation start
