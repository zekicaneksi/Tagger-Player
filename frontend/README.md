# Frontend

## Setting up the Frontend

### Requirements

- Node.js
- NPM

### Install Dependencies

```sh
npm install
```

### Development

```sh
npm run start
```

### Build

Running this command will create the executables in the `out/make/` directory.

```sh
npm run make
```

Building the RPM package might fail in some Linux systems. In that case, see;

https://github.com/electron/forge/issues/3701#issuecomment-2552233499

#### Arch Linux

To install the app after build via Pacman, a `PKGBUILD` file is provided. To install, simply copy the `PKGBUILD` file into the `out/make/deb/x64` directory and run

```sh
makepkg --clean OPTIONS=-debug
# As root
pacman -U tagger-player-1.0.0-1-x86_64.pkg.tar.zst
# As normal user
tagger-player
```

To uninstall the app

```sh
# As root
pacman -R tagger-player
```

