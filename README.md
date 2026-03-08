# Monalisa - Simple Image Viewer for Hyprland

A minimal, fast image viewer built with Hyprtoolkit for Wayland/Hyprland environments.

## Features

- **Simple & Fast** - Just an image, nothing else. SVG does not work currently.
- **Keyboard Navigation** - Next/previous, zoom, fit mode
- **Zoom Controls** - Zoom in/out, reset to actual size. Currently not working.
- **Fit Mode** - Toggle between fit-to-window and actual size
- **Directory Navigation** - Automatically browse all images in the same folder
- **Info Display** - Show filename and file size
- **System Colors** - Respects your Hyprland color palette
- **Easy Exit** - Press `q` or use `Meta+C` (killactive)

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `q` | Quit |
| `+` / `=` | Zoom in |
| `-` | Zoom out |
| `0` | Reset zoom / Fit to window |
| `Space` | Toggle fit mode |
| `→` or `n` | Next image |
| `←` or `p` | Previous image |
| `i` | Show image info |

## Installation

### Dependencies

- Hyprtoolkit

### Build from Source

```bash
git clone https://github.com/yourusername/monalisa.git
cd monalisa
./make.sh

```

Must move the built executable to system bin e.g. /usr/bin.

Move monalisa.desktop to user local **~/.local/share/applications** or system share/applications. 
This will allow you to use monalisa globally and also offer itself as open-with in context menu.

### Usage

```bash
monalisa /path/to/your/image.jpg
```


## Integration with File Managers

To set Monalisa as your default image viewer, create a desktop file:

```bash
cat > ~/.local/share/applications/monalisa.desktop << EOF
[Desktop Entry]
Name=Monalisa
Comment=Simple Image Viewer
Exec=/usr/local/bin/monalisa %f
Icon=image-x-generic
Terminal=false
Type=Application
Categories=Graphics;Viewer;
MimeType=image/png;image/jpeg;image/jpg;image/gif;image/bmp;image/webp;image/svg+xml;
EOF

update-desktop-database ~/.local/share/applications
```

## Building from Source

```bash
mkdir build && cd build
cmake ..
make
```

The binary will be created at `build/monalisa`.

## License

MIT License - feel free to use and modify!

## Credits

Hyprtoolkit
