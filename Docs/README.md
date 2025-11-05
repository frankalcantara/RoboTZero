# RoboTZero Documentation

This directory contains comprehensive documentation for the RoboTZero project, including setup guides, architecture documentation, and educational materials.

## Documentation Files

### Installation Guide

**File:** `installation-guide.qmd`

Complete installation and setup guide for all development environments required for the RoboTZero project.

**Contents:**
- WSL2 setup and configuration on Windows 11
- Git and GitHub configuration
- Visual Studio Code setup with extensions
- C++ development environment (GCC, Clang, CMake)
- Node.js and npm installation
- Svelte development setup
- Emscripten WebAssembly toolchain
- PlatformIO for Arduino/ESP32 development
- USB device access in WSL2
- Project setup and verification
- Troubleshooting guide

**Target Environment:** Ubuntu 24.04 LTS on WSL2

**Render to HTML:**
```bash
quarto render installation-guide.qmd
```

**Render to PDF:**
```bash
quarto render installation-guide.qmd --to pdf
```

## Quarto Documentation System

This project uses [Quarto](https://quarto.org/) for documentation. Quarto is an open-source scientific and technical publishing system.

### Installing Quarto

See the Installation Guide (`installation-guide.qmd`) for detailed instructions.

Quick install:
```bash
# Download and install
wget https://github.com/quarto-dev/quarto-cli/releases/download/v1.4.549/quarto-1.4.549-linux-amd64.deb
sudo dpkg -i quarto-1.4.549-linux-amd64.deb
```

### Rendering Documents

Render all documents:
```bash
quarto render
```

Render specific document:
```bash
quarto render installation-guide.qmd
```

Preview in browser:
```bash
quarto preview installation-guide.qmd
```

### Output Formats

All `.qmd` files support multiple output formats:
- HTML (default)
- PDF (requires LaTeX)
- DOCX (Microsoft Word)

## Contributing to Documentation

When adding new documentation:

1. Use `.qmd` (Quarto Markdown) format
2. Include proper YAML frontmatter
3. Use semantic sectioning (# ## ###)
4. Add code blocks with syntax highlighting
5. Include callout boxes for important notes
6. Test rendering before committing

### Document Template

```markdown
---
title: "Document Title"
author: "Author Name"
date: today
format:
  html:
    toc: true
    number-sections: true
  pdf:
    toc: true
---

# Introduction

Content here...
```

## Documentation Structure

```
Docs/
├── README.md                  # This file
├── installation-guide.qmd     # Complete setup guide
└── (future documents)
```

## Future Documentation

Planned documentation includes:

- **Architecture Guide**: Detailed system architecture and design decisions
- **API Reference**: Complete API documentation for simulator and libraries
- **Hardware Guide**: Wiring diagrams and hardware setup
- **Algorithm Guide**: Line following algorithms and optimization strategies
- **Tutorial Series**: Step-by-step project tutorials
- **Lab Exercises**: Hands-on exercises for students

## Useful Resources

- [Quarto Documentation](https://quarto.org/docs/)
- [Markdown Guide](https://www.markdownguide.org/)
- [GitHub Flavored Markdown](https://github.github.com/gfm/)

## Support

For questions or issues with documentation:
- Open an issue on GitHub
- Contact the course instructor
- Check the troubleshooting section in installation-guide.qmd

---

**Last Updated:** 2025-11-05
