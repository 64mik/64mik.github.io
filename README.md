# 64mik.github.io

A custom C++ Static Site Generator (SSG) for building Markdown-based websites and blogs. Converts Markdown files to HTML pages with custom templating and incremental caching.

## Features

- Native Performance: Written in C++ for fast Markdown parsing and HTML generation.
- Custom Templating: Supports HTML templates with placeholders for metadata and content.
- Incremental Builds: Caching via file hashing to skip unchanged files.
- Frontmatter Parsing: Parses page metadata (title, date, tags, etc.) from Markdown frontmatter.
- Post Management: Separate handling for blog posts with category support.
- Zero Jekyll Dependency: Independent build process without relying on GitHub's Jekyll.

## How to use

## Building

This project uses C++17 and requires a C++ compiler

### Prerequisites

- C++17 compatible compiler
- Windows: MinGW-w64 or MSVC
- Cmake 3.15+

## License

MIT License