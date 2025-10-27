package main

import (
	"fmt"
	"errors"
	"os"

	"tagger_player/session"
	"tagger_player/routes"

	"github.com/gin-gonic/gin"
)

func main() {

	// Close all db handles before quitting the app (should happen automatically actually but making sure doesn't hurt)
	defer session.EndAllSessions()

	if error := SetUpWeb(); error!= nil {
		fmt.Println(error)
	}
}

func SetUpWeb() error {
	port := os.Getenv("TAGGER_PLAYER_GO_PORT")
	if port == "" {
		return errors.New("TAGGER_PLAYER_GO_PORT isn't set")
	}

	// Create a Gin router with default middleware (logger and recovery)
	r := gin.Default()

	if err := routes.SetUpRoutes(r); err != nil {
		return err
	}

	fmt.Println("Listening on port: " + port);
	r.Run(":" + port)

	return nil
}
