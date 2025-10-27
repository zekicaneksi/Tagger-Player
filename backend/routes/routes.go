package routes

import (
	"fmt"
	"net/http"

	"tagger_player/session"

	"github.com/gin-gonic/gin"
)

func SetUpRoutes(r *gin.Engine) error {

	r.POST("/pick-path", func(c *gin.Context) {

		var pickPathInput struct {
			Path  string `json:"path" binding:"required"`
		}

		if err := c.ShouldBindJSON(&pickPathInput); err != nil {
			c.String(http.StatusBadRequest, "bad request")
			return
		}

		if (len(pickPathInput.Path) >= 1000) {
			c.String(http.StatusBadRequest, "too long of a path, refused because of memory clogging concern")
			return
		}

		err, uuid := session.SetSession(pickPathInput.Path)
		if err != nil {
			c.String(http.StatusInternalServerError, "Something went wrong, please make contact.")
			fmt.Println(err)
			return
		}

		c.String(http.StatusOK, uuid)
	})

	return nil
}
