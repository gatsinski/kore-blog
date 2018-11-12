#include <kore/pgsql.h>

char *generate_index_json(void);
char *generate_posts_json(int, struct kore_pgsql *);
char *generate_authors_json(int, struct kore_pgsql *);
