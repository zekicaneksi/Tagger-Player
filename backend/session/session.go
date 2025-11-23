package session

import (
	"fmt"
	"sync"
	"errors"
	"database/sql"

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


func SetSession(path string) (string, error) {
	mu_activeSessions.Lock()
	defer mu_activeSessions.Unlock()

	if len(activeSessions) >= 100 {
		return "", errors.New("Not setting session beucase of memory clogging concern")
	}

	// Create uuid
	var id string

	for i := 0; i < 10; i++ {
		id = uuid.New().String()

		// Check if uuid exists (even though very low chance)
		if _, exists := activeSessions[id]; exists == false {
			break
		}

		if i == 9 {
			return "", errors.New("Something is wrong with uuid creation")
		}
	}

	// Check if the path already exists, meaning someone already has opened the file
	// and if so, (effectively) replace the uuid and return
	for k, v := range activeSessions {
		if v.Path == path {
			activeSessions[id] = Session{Path: path, DbHandle: v.DbHandle}
			delete(activeSessions, k);
			return id, nil
		}
	}

	// Note: The "unable to open database file: out of memory (14)" error usually happens when there is something wrong with the path,
	// I don't know why it gives such a weird error description.
	dbHandle, err := sql.Open("sqlite", path)
	if err != nil {
		return "", fmt.Errorf("Error when creating/opening the sqlite file: %w", err)
	}

	activeSessions[id] = Session{Path: path, DbHandle: dbHandle}	

	return id, nil
}

func GetSessionDbHandle(uuid string) (*sql.DB, error) {
	mu_activeSessions.Lock()
	defer mu_activeSessions.Unlock()

	if _, exists := activeSessions[uuid]; exists == false {
		return nil, errors.New("uuid doesn't exist")
	}

	return activeSessions[uuid].DbHandle, nil
}

func EndAllSessions() {
	mu_activeSessions.Lock()
	defer mu_activeSessions.Unlock()

	for _, v := range activeSessions {
		v.DbHandle.Close();
	}

	clear(activeSessions)	
}
