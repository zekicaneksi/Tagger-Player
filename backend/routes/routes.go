package routes

import (
	"fmt"
	"net/http"
	"errors"

	"tagger_player/session"
	"tagger_player/db_operations"

	"github.com/gin-gonic/gin"
)

var (
	InternalError = errors.New("Something went wrong, please make contact.")
)

// --- Middlewares

func UUIDMW() gin.HandlerFunc {
	return func(c *gin.Context) {

		uuid := c.Request.Header.Get("X-uuid")

		if uuid == "" {
			c.AbortWithStatusJSON(401, gin.H{
				"error": "Unauthorized: missing uuid",
			})
			return
		}

		// Store the ID and continue to next handler
		c.Set("uuid", uuid)
		c.Next()
	}
}

// --- Routes

func SetUpRoutes(r *gin.Engine) error {

	r.POST("/pick-path", func(c *gin.Context) {

		var pickPathInput struct {
			Path  string `json:"path" binding:"required"`
		}

		if err := c.ShouldBindJSON(&pickPathInput); err != nil {
			c.JSON(http.StatusBadRequest, gin.H{"error": "path is missing"})
			return
		}

		if (len(pickPathInput.Path) >= 1000) {
			c.JSON(http.StatusBadRequest, gin.H{"error": "too long of a path, refused because of memory clogging concern"})
			return
		}

		uuid, err := session.SetSession(pickPathInput.Path + "/.tagger_player")
		if err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": InternalError.Error()})
			fmt.Println(err)
			return
		}

		dbHandle, err := session.GetSessionDbHandle(uuid)
		if err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": InternalError.Error()})
			fmt.Println(err)
			return
		}
		if err := db_operations.CreateInitialTables(dbHandle); err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": InternalError.Error()})
			fmt.Println("Error when creating the initial tables --- ", err)
			return
		}

		c.JSON(http.StatusOK, gin.H{"uuid": uuid})
	})

	r.POST("/add-tag", UUIDMW(), func(c *gin.Context) {

		var addTagInput struct{
			Tag string `json:"tag" binding:"required"`
		}

		if err := c.ShouldBindJSON(&addTagInput); err != nil {
			c.JSON(http.StatusBadRequest, gin.H{"error": "bad request"})
			return
		}

		if (len(addTagInput.Tag) >= 200) {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Too long of a tag, refused because of memory clogging concern"})
			return
		}

		if (len(addTagInput.Tag) == 0) {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Tag cannot be empty"})
			return
		}

		err := db_operations.AddTag(c.MustGet("uuid").(string), addTagInput.Tag)
		if err != nil {
			if errors.Is(err, db_operations.ErrUUID) {
				c.JSON(http.StatusUnauthorized, gin.H{"error": err.Error()})
				return
			} else {
				c.JSON(http.StatusInternalServerError, gin.H{"error": InternalError.Error()})
				fmt.Println(err)
				return
			}
		}

		c.JSON(http.StatusOK, gin.H{})

	})

	r.GET("/get-tags", UUIDMW(), func(c *gin.Context) {

		tags, err := db_operations.GetTags(c.MustGet("uuid").(string));
		if err != nil {
			if errors.Is(err, db_operations.ErrUUID) {
				c.JSON(http.StatusUnauthorized, gin.H{"error": err.Error()})
				return
			} else {
				c.JSON(http.StatusInternalServerError, gin.H{"error": InternalError.Error()})
				fmt.Println(err)
				return
			}
		}

		c.JSON(http.StatusOK, gin.H{"tags": tags})

	})

	return nil
}
