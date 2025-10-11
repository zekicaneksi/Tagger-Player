package main

import (
	"fmt"
	"errors"
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
)


func main() {
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

	r.GET("/", func(c *gin.Context) {
		c.String(http.StatusOK, "Hello from the backend!")
	})

	fmt.Println("Listening on port: " + port);
	r.Run(":" + port)

	return nil
}
