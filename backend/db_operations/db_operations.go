package db_operations

import (
	"database/sql"
	"errors"

	"tagger_player/session"

	_ "modernc.org/sqlite"
)

var (
	ErrUUID = errors.New("uuid is not valid or missing")
)

func CreateInitialTables(dbHandle *sql.DB) error {
	// Create a random table for now, replace it later with the actual table schema

	if _, err := dbHandle.Exec(`
	create table if not exists test_values(name text not null)
	`); err != nil {
		return err;
	}

	return nil;
}

func AddTag(uuid, tagText string) error {
	dbHandle, err := session.GetSessionDbHandle(uuid)
	if err != nil {
		return ErrUUID;
	}

	if _, err := dbHandle.Exec(`insert into test_values (name) values(?)`, tagText); err != nil {
		return err
	}

	return nil;
}

func GetTags(uuid string) ([]string, error) {
	dbHandle, err := session.GetSessionDbHandle(uuid)
	if err != nil {
		return nil, err;
	}

	rows, err := dbHandle.Query("select name from test_values")
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	tags := []string{}

	for rows.Next() {
		var name string
		if err = rows.Scan(&name); err != nil {
			return nil, err
		}
		tags = append(tags, name)
	}

	if err = rows.Err(); err != nil {
		return nil, err
	}

	return tags, nil
}
