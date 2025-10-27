package db_operations

import (
	"database/sql"

	_ "modernc.org/sqlite"
)

func CreateInitialTables(dbHandle *sql.DB) error {
	// Create a random table for now, replace it later with the actual table schema
	// Note: The "unable to open database file: out of memory (14)" error usually happens when there is something wrong with the path,
	// I don't know why it gives such a weird error description.

	if _, err := dbHandle.Exec(`
	drop table if exists test_values;
	create table test_values(i);
	`); err != nil {
		return err;
	}

	return nil;
}
