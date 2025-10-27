package main

import (
	"fmt"
	"sync"
	"errors"
	"net/http"
	"os"
	"database/sql"

	"github.com/gin-gonic/gin"
	"github.com/google/uuid"
	_ "modernc.org/sqlite"
)

type Session struct {
	Path string
	DbHandle *sql.DB
}

var (
	ActiveSessions = make(map[string]Session)
	mu_ActiveSessions sync.Mutex
)


func setSession(path string) (error, string) {
	mu_ActiveSessions.Lock()
	defer mu_ActiveSessions.Unlock()

	if len(ActiveSessions) >= 100 {
		return errors.New("Not setting session beucase of memory clogging concern"), ""
	}

	// Check if the path already exists, and if so delete the key-value (we don't want multiple handles to the same sql file)
	for k, v := range ActiveSessions {
		if v.Path == path {
			ActiveSessions[k].DbHandle.Close();
			delete(ActiveSessions, k);
			break
		}
	}

	var id string

	for i := 0; i < 10; i++ {
		id = uuid.New().String()

		// Check if uuid exists (even though very low chance)
		if _, exists := ActiveSessions[id]; exists == false {
			break
		}

		if i == 9 {
			return errors.New("Something is wrong with uuid creation"), ""
		}
	}	

	dbHandle, err := sql.Open("sqlite", path)
	if err != nil {
		return fmt.Errorf("Error when creating/opening the sqlite file: %w", err), ""
	}

	// Create a random table for now, replace it later with the actual table schema
	// Note: The "unable to open database file: out of memory (14)" error usually happens when there is something wrong with the path,
	// I don't know why it gives such a weird error description.

	if _, err = dbHandle.Exec(`
	drop table if exists test_values;
	create table test_values(i);
	`); err != nil {
		return fmt.Errorf("Error when creating the initial tables: %w", err), ""
	}

	ActiveSessions[id] = Session{Path: path, DbHandle: dbHandle}	

	return nil, id
}

func getSessionDbHandle(uuid string) (error, *sql.DB) {
	mu_ActiveSessions.Lock()
	defer mu_ActiveSessions.Unlock()

	if _, exists := ActiveSessions[uuid]; exists == false {
		return errors.New("uuid doesn't exist"), nil
	}

	return nil, ActiveSessions[uuid].DbHandle
}

func endAllSessions() {
	mu_ActiveSessions.Lock()
	defer mu_ActiveSessions.Unlock()

	for _, v := range ActiveSessions {
		v.DbHandle.Close();
	}

	clear(ActiveSessions)	
}

func main() {

	// Close all db handles before quitting the app (should happen automatically actually but making sure doesn't hurt)
	defer endAllSessions()

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

	if err := SetUpRoutes(r); err != nil {
		return err
	}

	fmt.Println("Listening on port: " + port);
	r.Run(":" + port)

	return nil
}

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

		err, uuid := setSession(pickPathInput.Path)
		if err != nil {
			c.String(http.StatusInternalServerError, "Something went wrong, please make contact.")
			fmt.Println(err)
			return
		}

		c.String(http.StatusOK, uuid)
	})

	return nil
}
