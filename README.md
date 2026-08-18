# C++ DJ Session Manager

A C++ application developed as an academic project for a Systems Programming course.

The project simulates a DJ session management system while focusing on **object-oriented programming, manual memory management, copy/move semantics, polymorphism, and resource ownership**.

## Technologies

* C++11
* Object-Oriented Programming
* Manual Memory Management
* Move Semantics
* Polymorphism
* Make
* Valgrind
* Linux

## Key Features

* DJ session and playlist management
* MP3 and WAV track support
* Polymorphic audio track hierarchy
* LRU-based track caching
* Track loading and mixing simulation
* Configuration-based session setup
* Custom resource ownership wrapper
* Dynamic memory management
* Copy and move operations
* Memory leak testing with Valgrind

## Memory Management

The project focuses heavily on correct resource management in C++.

`AudioTrack` manages dynamically allocated waveform data and implements copy and move operations to safely transfer or duplicate resources.

The implementation includes:

* Destructor
* Copy constructor
* Copy assignment operator
* Move constructor
* Move assignment operator

## Custom Pointer Wrapper

The project includes a move-only `PointerWrapper<T>` that manages ownership of dynamically allocated objects.

It supports:

* Move construction and assignment
* Pointer dereferencing
* Member access
* `release()`
* `reset()`
* Boolean conversion
* Resource cleanup

## LRU Cache

`LRUCache` manages a limited number of audio tracks using a **Least Recently Used** replacement policy.

The cache tracks access order and automatically evicts the least recently used track when its capacity is reached.

## Object-Oriented Design

`AudioTrack` serves as the base class for different audio formats.

Derived classes include:

* `MP3Track`
* `WAVTrack`

Virtual functions and polymorphism are used for format-specific behavior, cloning, track loading, and audio analysis.

## Main Components

* `AudioTrack` – Base class for audio tracks
* `MP3Track` / `WAVTrack` – Format-specific track implementations
* `Playlist` – Track collection management
* `LRUCache` – Least Recently Used caching
* `PointerWrapper` – Custom resource ownership wrapper
* `DJSession` – DJ session orchestration
* `DJControllerService` – Track cache management
* `DJLibraryService` – Music library management
* `MixingEngineService` – Track loading and mixing operations
* `ConfigurationManager` – Application configuration
* `SessionFileParser` – Session configuration parsing

## Project Structure

```text
cpp-dj-session-manager/
├── bin/
│   └── dj_config.txt
├── include/
├── src/
├── Makefile
└── .gitignore
```

## Build and Run

Build the project:

```bash
make
```

Run the built-in memory management and polymorphism tests:

```bash
./bin/dj_manager
```

Run the interactive DJ session:

```bash
./bin/dj_manager -I
```

Run the interactive session with automatic playlist processing:

```bash
./bin/dj_manager -I -A
```

## Memory Leak Testing

Build and run the project using Valgrind:

```bash
make test-leaks
```

Clean generated build files:

```bash
make clean
```

## Configuration

Session settings can be configured in:

```text
bin/dj_config.txt
```

## Academic Project

Developed as an academic project as part of a Systems Programming course at Ben-Gurion University.
