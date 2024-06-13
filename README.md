# Hello

## How it works

Open the app and choose a directory. The app will create a file named `.tagger.db` which is an Sqlite file to store information in the chosen directory. After that, you can tag your files within the directory using the app and all the information will be stored in the created file.

Nix

```sh
nix-shell
```

Build

```sh
make
```

Run

```sh
build/main
```

## Database Schema

```sql
CREATE TABLE file (
    id INT PRIMARY KEY,
    name varchar(255) UNIQUE NOT NULL
);
```

```sql
CREATE TABLE tag (
    id INT PRIMARY KEY,
    name varchar(255) UNIQUE NOT NULL
);
```

```sql
CREATE TABLE file_tag (
    file_id INT,
    tag_id INT,
    FOREIGN KEY(file_id) REFERENCES file(id),
    FOREIGN KEY(tag_id) REFERENCES tag(id),
    UNIQUE(file_id, tag_id)
);
```
