/*
 * Simple blog API
 */

#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include "json_utils.h"
#include "common_utils.h"

int init(int);
int page(struct http_request *);
int posts(struct http_request *);
int authors(struct http_request *);

int init(int state)
{
    /* Example database config. */
    kore_pgsql_register(
        "kore-blog",
        "host=localhost port=5432 dbname=blog user=blog password=blog");

    return (KORE_RESULT_OK);
}

int page(struct http_request *req)
{
    char *json_response = generate_index_json();
    http_response_header(req, "post_content-type", "application/json");

    return_http_response(req, json_response);
    return (KORE_RESULT_OK);
}

int posts(struct http_request *req)
{
    int rows;
    char *json_response = NULL;
    struct kore_pgsql sql;

    http_response_header(req, "post_content-type", "application/json");
    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_pgsql_init(&sql);
    rows = get_sql_result(&sql,
                          "SELECT id, author, content, created_at FROM posts");

    json_response = generate_posts_json(rows, &sql);
    return_http_response(req, json_response);

    kore_pgsql_cleanup(&sql);
    return (KORE_RESULT_OK);
}

int authors(struct http_request *req)
{
    int rows;
    char *json_response = NULL;
    struct kore_pgsql sql;

    http_response_header(req, "post_content-type", "application/json");

    kore_pgsql_init(&sql);
    rows = get_sql_result(&sql, "SELECT id, name FROM authors");
    json_response = generate_authors_json(rows, &sql);
    return_http_response(req, json_response);

    kore_pgsql_cleanup(&sql);
    return (KORE_RESULT_OK);
}
