//#include "parseRDF.h"
#include <neo4j-client.h>
#include <errno.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
	neo4j_client_init();

    neo4j_connection_t *connection =
            neo4j_connect("neo4j://neo4j/neo4j:pass@localhost:7474", NULL, NEO4J_INSECURE);
    /*if (connection == NULL) {
        neo4j_perror(stderr, errno, "Connection failed");
        return 1;
    }

    neo4j_result_stream_t *results =
            neo4j_run(connection, "RETURN 'hello world'", neo4j_null);
    if (results == NULL) {
        neo4j_perror(stderr, errno, "Failed to run statement");
        return 1;
    }

    neo4j_result_t *result = neo4j_fetch_next(results);
    if (result == NULL)
    {
        neo4j_perror(stderr, errno, "Failed to fetch result");
        return 1;
    }

    neo4j_value_t value = neo4j_result_field(result, 0);
    char buf[128];
    printf("%s\n", neo4j_tostring(value, buf, sizeof(buf)));

    neo4j_close_results(results);*/
    neo4j_close(connection);
    neo4j_client_cleanup();
    return 0;
    
    
	/*char *filename = argv[1];
	parseRDF(filename);
	
	return 0;*/
}
