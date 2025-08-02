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

struct db_header_s {
    int unsigned max_data;
    int unsigned max_rows;
};

struct Database {
    struct db_header_s header;
    char *body;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

#define db_row_size(db)  (db->header.max_data * 2 + sizeof(int)*2) /* id + set + name + email */
#define db_body_size(db)  (db_row_size(db)* db->header.max_rows)
#define db_size(db)  (db_body_size(db) + sizeof(struct db_header_s))

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

struct Connection *Database_open(const char *filename, char mode, int unsigned data_len, int unsigned row_cnt)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error 1", conn);

    if(mode == 'c') {
        conn->db->header.max_data = data_len;
        conn->db->header.max_rows = row_cnt;
        conn->db->body = malloc(db_body_size(conn->db));
        if(!conn->db->body) die("Memory error 2", conn);
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");
        if(conn->file) {
            int db_data_size = 0;
            int rc = fread(conn->db, 1, sizeof(struct db_header_s), conn->file);
            if(rc != sizeof(struct db_header_s)) die("Failed to load database.1", conn);

            conn->db->body = malloc(db_body_size(conn->db));
            if(!conn->db->body) die("Memory error 2", conn);

            db_data_size = db_body_size(conn->db);

            rc = fread(conn->db->body, 1, db_data_size, conn->file);
            if(rc != db_data_size) die("Failed to load database.2", conn);
        }
    }

    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) {
            if (conn->db->body) {
                free(conn->db->body);
            }

            free(conn->db);
        }
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct db_header_s), 1, conn->file);
    if(rc != 1) die("Failed to write database1.", conn);

    rc = fwrite(conn->db->body, db_body_size(conn->db), 1, conn->file);
    if(rc != 1) die("Failed to write database2.", conn);

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < conn->db->header.max_rows; i++) {
        // make a prototype to initialize it
        struct Address *addr = (struct Address *)(conn->db->body + (i*db_row_size(conn->db)));

        memset(addr, 0, db_row_size(conn->db));
        addr->id = i;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr;
    int unsigned max_data_size = conn->db->header.max_data;

    addr = (struct Address *)(conn->db->body + id * db_row_size(conn->db));

    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;

    int str_len = strlen(name) > (max_data_size - 1) ? (max_data_size - 1) : strlen(name);
    char *res = strncpy(addr->info, name, str_len);
    // demonstrate the strncpy bug
    if(!res) die("Name copy failed", conn);

    str_len = strlen(email) > (max_data_size - 1) ? (max_data_size - 1) : strlen(email);
    res = strncpy(addr->info + max_data_size, email, str_len);
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = (struct Address *)(conn->db->body + id * db_row_size(conn->db));

    if(addr->set) {
        Address_print(addr, conn->db->header.max_data);
    } else {
        die("ID is not set", conn);
    }
}

void Database_find(struct Connection *conn, int id)
{
    if(id >= conn->db->header.max_rows) {
        char log[256];

        sprintf(log, "id[%d] is error, max_rows:%d", id, conn->db->header.max_rows);
        die(log, conn);
        return;
    }

    struct Address *addr = (struct Address *)(conn->db->body + id * db_row_size(conn->db));

    if(addr->set) {
        printf("ID:%d is set\n", id);
    } else {
        printf("ID:%d is not set\n", id);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    if(id >= conn->db->header.max_rows) {
        char log[256];

        sprintf(log, "id[%d] is error, max_rows:%d", id, conn->db->header.max_rows);
        die(log, conn);
        return;
    }

    struct Address *addr = (struct Address *)(conn->db->body + id * db_row_size(conn->db));

    memset(addr, 0, db_row_size(conn->db));
    addr->id  = id;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;
    int unsigned max_rows = conn->db->header.max_rows;

    for(i = 0; i < max_rows; i++) {
        struct Address *cur = (struct Address *)(db->body + i * db_row_size(db));

        if(cur->set) {
            Address_print(cur, db->header.max_data);
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

    if (action != 'c') {
        if(argc > 3) id = atoi(argv[3]);
        if(id >= conn->db->header.max_rows) die("There's not that many records.", conn);
    }

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 'f':
            if(argc != 4) die("Need an id to find", conn);

            Database_find(conn, id);
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