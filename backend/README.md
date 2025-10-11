# Backend

## Setting up the Backend

### Install Dependencies

```go
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

```go
go run .
```

### Build

To produce the executable

```go
go build .
```
