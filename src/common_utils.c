#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include "common_utils.h"

int get_sql_result(struct kore_pgsql *sql, char *select_statement)
{
    if (!kore_pgsql_setup(sql, "kore-blog", KORE_PGSQL_SYNC))
    {
        kore_pgsql_logerror(sql);
        return 0;
    }

    if (!kore_pgsql_query(sql, select_statement))
    {
        kore_pgsql_logerror(sql);
        return 0;
    }

    return kore_pgsql_ntuples(sql);
}


void return_http_response(struct http_request *req, char *json_response)
{
    if (json_response == NULL)
    {
        kore_log(LOG_ERR, "%s", "Failed to generate JSON response");
        http_response(req, HTTP_STATUS_INTERNAL_ERROR, NULL, 0);
    }
    else
    {
        kore_log(LOG_NOTICE, "Response: '%s'", json_response);
        http_response(req, HTTP_STATUS_OK, json_response, strlen(json_response));
    }
}
