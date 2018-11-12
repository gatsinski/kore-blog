#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>

int get_sql_result(struct kore_pgsql *sql, char *select_statement);
void return_http_response(struct http_request *req, char *json_response);
