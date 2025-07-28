#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int set;
    //name + email
    char info[];
};

struct Database {
    int unsigned max_data;//default 512
    int unsigned max_rows;//default 100
    void *infos;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

#define db_row_size(db)  (db->max_data * 2 + sizeof(int)*2)
#define db_size(db)  (db_row_size(db)* db->max_rows + sizeof(int unsigned)*2)

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close(conn);

    exit(1);
}

void Address_print(struct Address *addr, int unsigned max_data)
{
    printf("%d %s %s\n",
            addr->id, addr->info, (addr->info + max_data));
}

void Database_load(struct Connection *conn)
{
    int rc = fread(conn->db, db_size(conn->db), 1, conn->file);
    if(rc != 1) die("Failed to load database.", conn);
}

struct Connection *Database_open(const char *filename, char mode, int unsigned data_len, int unsigned row_cnt)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error", conn);

    if(mode == 'c') {
        conn->db->max_data = data_len;
        conn->db->max_rows = row_cnt;
        conn->db->infos = malloc(db_size(conn->db));
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");
    }
    if(conn->file) {
        Database_load(conn);
    }

    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) free(conn->db);
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, db_size(conn->db), 1, conn->file);
    if(rc != 1) die("Failed to write database.", conn);

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < conn->db->max_rows; i++) {
        // make a prototype to initialize it
        struct Address *addr = (conn->db->infos + (i*(conn->db->max_data)));

        memset(addr, 0, db_row_size(conn->db));
        addr->id = i;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = (conn->db->infos + id * db_row_size(conn->db));
    int unsigned max_data_size = conn->db->max_data;

    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;
    // WARNING: bug, read the "How To Break It" and fix this
    char *res = strncpy(addr->info, name, (max_data_size-1));
    // demonstrate the strncpy bug
    if(!res) die("Name copy failed", conn);

    res = strncpy(addr->info + max_data_size, email, (max_data_size-1));
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = (conn->db->infos + db_row_size(conn->db));

    if(addr->set) {
        Address_print(addr, conn->db->max_data);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address *addr = (conn->db->infos + db_row_size(conn->db));

    memset(addr, 0, db_row_size(conn->db));
    addr->id  = id;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;
    int unsigned max_rows = conn->db->max_rows;

    for(i = 0; i < max_rows; i++) {
        struct Address *cur = (db->infos + db_row_size(db));

        if(cur->set) {
            Address_print(cur, db->max_data);
        }
    }
}

int main(int argc, char *argv[])
{
    struct Connection *conn;

    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", 0);

    char *filename = argv[1];
    char action = argv[2][0];
    if (action == 'c') {
        if (argc < 5) {
            die("USAGE: ex17 <dbfile> c max_data max_row", 0);
        }
        int unsigned max_data = atoi(argv[3]);
        int unsigned max_row = atoi(argv[4]);

        conn = Database_open(filename, action, max_data, max_row);
    } else {
        conn = Database_open(filename, action, 0, 0);
    }
    int id = 0;

    if(argc > 3) id = atoi(argv[3]);
    if(id >= conn->db->max_rows) die("There's not that many records.", conn);

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete",conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}