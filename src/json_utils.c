#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include "cJSON.h"
#include "json_utils.h"

char *generate_index_json()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "version", "1.0");
    cJSON_AddStringToObject(json, "posts", "/posts");
    cJSON_AddStringToObject(json, "authors", "/authors");
    return cJSON_Print(json);
}

char *generate_posts_json(int rows, struct kore_pgsql *sql)
{
    char *post_id;
    char *post_author;
    char *post_content;
    char *json_response = NULL;
    cJSON *posts = NULL;
    cJSON *json = cJSON_CreateObject();

    if (cJSON_AddStringToObject(json, "version", "1.0") == NULL)
        goto out;

    posts = cJSON_AddArrayToObject(json, "posts");
    if (posts == NULL)
        goto out;

    for (int i = 0; i < rows; i++)
    {
        cJSON *post = cJSON_CreateObject();

        post_id = kore_pgsql_getvalue(sql, i, 0);
        post_author = kore_pgsql_getvalue(sql, i, 1);
        post_content = kore_pgsql_getvalue(sql, i, 2);

        if (cJSON_AddStringToObject(post, "id", post_id) == NULL)
            goto out;

        if (cJSON_AddStringToObject(post, "author", post_author) == NULL)
            goto out;

        if (cJSON_AddStringToObject(post, "text", post_content) == NULL)
            goto out;


        cJSON_AddItemToArray(posts, post);
    }

    json_response = cJSON_Print(json);
    if (json_response == NULL)
        kore_log(LOG_ERR, "Failed to print json.");

out:
    cJSON_Delete(json);
    return json_response;
}

char *generate_authors_json(int rows, struct kore_pgsql *sql)
{
    char *author_id;
    char *author_name;
    char *json_response = NULL;
    cJSON *authors = NULL;
    cJSON *json = cJSON_CreateObject();

    if (cJSON_AddStringToObject(json, "version", "1.0") == NULL)
        goto out;

    authors = cJSON_AddArrayToObject(json, "authors");
    if (authors == NULL)
        goto out;

    for (int i = 0; i < rows; i++)
    {
        cJSON *author = cJSON_CreateObject();

        author_id = kore_pgsql_getvalue(sql, i, 0);
        author_name = kore_pgsql_getvalue(sql, i, 1);

        if (cJSON_AddStringToObject(author, "id", author_id) == NULL)
            goto out;

        if (cJSON_AddStringToObject(author, "name", author_name) == NULL)
            goto out;

        cJSON_AddItemToArray(authors, author);
    }

    json_response = cJSON_Print(json);
    if (json_response == NULL)
        kore_log(LOG_ERR, "Failed to print json.");

out:
    cJSON_Delete(json);
    return json_response;
}
