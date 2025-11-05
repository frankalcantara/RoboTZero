# External Libraries

This directory contains third-party libraries used by the Line Follower Simulator.

## Required Libraries

### Box2D

2D physics engine for simulating robot dynamics.

**Installation:**

```bash
cd external
git clone https://github.com/erincatto/box2d.git
cd box2d
mkdir build
cd build

# For WebAssembly
emcmake cmake -DCMAKE_BUILD_TYPE=Release -DBOX2D_BUILD_UNIT_TESTS=OFF -DBOX2D_BUILD_TESTBED=OFF ..
emmake make

# For native (testing)
# cmake -DCMAKE_BUILD_TYPE=Release -DBOX2D_BUILD_UNIT_TESTS=OFF -DBOX2D_BUILD_TESTBED=OFF ..
# make
```

**License:** MIT License

**Repository:** https://github.com/erincatto/box2d

### Eigen

Header-only linear algebra library.

**Installation:**

```bash
cd external
git clone https://gitlab.com/libeigen/eigen.git
```

No build required - Eigen is header-only.

**License:** MPL2 License

**Repository:** https://gitlab.com/libeigen/eigen

### CppAD (Optional - Phase 2)

Automatic differentiation for gradient-based optimization.

**Installation:**

```bash
cd external
git clone https://github.com/coin-or/CppAD.git
```

**License:** EPL 2.0 or GPL 3.0

**Repository:** https://github.com/coin-or/CppAD

## Notes

- All libraries are git-ignored to avoid committing large dependencies
- Users must download and build libraries locally
- See main README.md for complete setup instructions
- WebAssembly builds require Emscripten SDK to be installed and activated
