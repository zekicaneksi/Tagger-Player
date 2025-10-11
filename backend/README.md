# Backend

## Setting up the Backend

### Requirements

- Golang

### Install Dependencies

```sh
go mod download
```

### Environment Variables

```env
TAGGER_PLAYER_GO_PORT
// The port to host the app on. Required.
GIN_MODE
// "debug" or "release" depending on the development/production environment. Not required.
```

### Run

```sh
go run .
```

### Build

To produce the executable

```sh
go build .
```
