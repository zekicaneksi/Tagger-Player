package session

import (
	"fmt"
	"sync"
	"errors"
	"database/sql"

	"tagger_player/db_operations"

	"github.com/google/uuid"
	_ "modernc.org/sqlite"
)

type Session struct {
	Path string
	DbHandle *sql.DB
}

var (
	activeSessions = make(map[string]Session)
	mu_activeSessions sync.Mutex
)


func SetSession(path string) (error, string) {
	mu_activeSessions.Lock()
	defer mu_activeSessions.Unlock()

	if len(activeSessions) >= 100 {
		return errors.New("Not setting session beucase of memory clogging concern"), ""
	}

	// Check if the path already exists, and if so delete the key-value (we don't want multiple handles to the same sql file)
	for k, v := range activeSessions {
		if v.Path == path {
			activeSessions[k].DbHandle.Close();
			delete(activeSessions, k);
			break
		}
	}

	var id string

	for i := 0; i < 10; i++ {
		id = uuid.New().String()

		// Check if uuid exists (even though very low chance)
		if _, exists := activeSessions[id]; exists == false {
			break
		}

		if i == 9 {
			return errors.New("Something is wrong with uuid creation"), ""
		}
	}

	// Note: The "unable to open database file: out of memory (14)" error usually happens when there is something wrong with the path,
	// I don't know why it gives such a weird error description.
	dbHandle, err := sql.Open("sqlite", path)
	if err != nil {
		return fmt.Errorf("Error when creating/opening the sqlite file: %w", err), ""
	}

	if err := db_operations.CreateInitialTables(dbHandle); err != nil {
		return fmt.Errorf("Error when creating the initial tables: %w", err), ""
	}

	activeSessions[id] = Session{Path: path, DbHandle: dbHandle}	

	return nil, id
}

func GetSessionDbHandle(uuid string) (error, *sql.DB) {
	mu_activeSessions.Lock()
	defer mu_activeSessions.Unlock()

	if _, exists := activeSessions[uuid]; exists == false {
		return errors.New("uuid doesn't exist"), nil
	}

	return nil, activeSessions[uuid].DbHandle
}

func EndAllSessions() {
	mu_activeSessions.Lock()
	defer mu_activeSessions.Unlock()

	for _, v := range activeSessions {
		v.DbHandle.Close();
	}

	clear(activeSessions)	
}
