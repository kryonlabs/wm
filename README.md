# TaijiOS Window Manager (WM)

A 9front/rio-style window manager with right-click menu interface.

## Usage

### Start with empty screen
```bash
wm
```
This starts the window manager with an empty screen. Right-click anywhere to show the menu and create windows.

### Load a .kry file
```bash
wm examples/minimal.kry
```
Or with the --run flag:
```bash
wm --run examples/minimal.kry
```

### Options
- `--marrow ADDR`    Marrow server address (default: tcp!localhost!17010)
- `--dump-screen`    Dump screenshot to /tmp/wm_before.raw
- `--watch`          Enable hot-reload for .kry file
- `--list-examples`  List available example files
- `--blank`          Clear screen to black and wait
- `--help`           Show help message

## Right-Click Menu

Right-click anywhere on the screen to show the 9front-style menu:

- **New** - Create a new shell window (640x480) with rc shell inside
- **Resize** - Resize the topmost window (not yet implemented)
- **Move** - Move the topmost window (not yet implemented)
- **Delete** - Delete the topmost window
- **Hide** - Hide the topmost window
- **Exit** - Exit the window manager

## Keyboard Shortcuts

- **Ctrl-C** - Exit the window manager

## Features

- 9front-style right-click menu with blue selection highlighting
- Window management (create, delete, hide)
- Shell window spawning with rc
- Empty screen mode - start without any .kry file
- Optional .kry file loading

## Examples

```bash
# Start with empty screen
wm

# Load an example file
wm examples/minimal.kry

# Load with custom Marrow address
wm --marrow tcp!192.168.1.100!17010 myapp.kry

# Enable hot-reload during development
wm --watch myapp.kry
```

## Window Management

Windows are managed via the right-click menu:
- New windows appear at offset positions (30px diagonal offset)
- Windows can be deleted or hidden via the menu
- Hidden windows are stored but not rendered
- Window stacking follows last-clicked order

## Development

### Building
```bash
cd /mnt/storage/Projects/TaijiOS
mk install
```

### Source Structure
- `src/main.c` - Main entry point and event loop
- `src/menu.c` - Menu rendering and interaction
- `src/shell.c` - Shell spawning
- `src/wmgr.c` - Window management
- `include/wm.h` - Public API

## Design

The WM follows 9front/rio design principles:
- Right-click menu is the primary interface
- Minimal UI - menus appear on demand
- Text-based menu with simple selection highlighting
- Shell windows are first-class citizens
- Clean separation between WM and shell processes

## Future Enhancements

- [ ] Resize mode (drag to resize windows)
- [ ] Move mode (drag to move windows)
- [ ] Window borders with drag handles
- [ ] Unhide all windows function
- [ ] Multiple shell types (rc, sh, etc.)
- [ ] Window title bar customization
- [ ] Keyboard shortcuts for menu items
