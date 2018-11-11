#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>

int init(int);
int page(struct http_request *);

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
    struct kore_pgsql sql;
    char *content;
    int rows, i;

    req->status = HTTP_STATUS_INTERNAL_ERROR;

    kore_pgsql_init(&sql);

    if (!kore_pgsql_setup(&sql, "kore-blog", KORE_PGSQL_SYNC))
    {
        kore_pgsql_logerror(&sql);
        goto out;
    }

    if (!kore_pgsql_query(&sql, "SELECT content FROM posts"))
    {
        kore_pgsql_logerror(&sql);
        goto out;
    }

    rows = kore_pgsql_ntuples(&sql);

    for (i = 0; i < rows; i++)
    {
        content = kore_pgsql_getvalue(&sql, i, 0);
        kore_log(LOG_NOTICE, "name: '%s'", content);
        http_response(req, 200, content, strlen(content));
    }

    req->status = HTTP_STATUS_OK;

out:
    http_response(req, req->status, NULL, 0);

    /* Don't forget to cleanup the kore_pgsql data structure. */
    kore_pgsql_cleanup(&sql);

    return (KORE_RESULT_OK);
}
