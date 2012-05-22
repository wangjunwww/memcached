#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <libmemcached/memcached.h>

#include <lib/timer.h>

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("Usage: %s [iterations] [key size] [value size] [output file]\n\n", argv[0]);
		printf("Runs benchmark with [iterations] number of keys, using random length \nof size [key size] set to a single value of random bytes of size [value size]\nand outputs result to [output file]\n\n");
		exit(1);
	}
	FILE *fp;
	if ((fp = fopen(argv[4], "w")) == NULL ) {
		printf("Failed to open file!\n");
		exit(1);
	}
	char *val;
	unsigned int setter = 1;
	int num_keys = atoi(argv[1]);
	int key_size = atoi(argv[2]);
	int value_size = 6 * 1024;
	if (argc >= 4)
		value_size = atoi(argv[3]);
	unsigned long long getter;
	char **keys;
	char * randomstuff = (char *)malloc(value_size);
	int i, j;
	printf("Num Keys: %d   Key size: %d  value size: %d\n", num_keys, key_size, value_size);
	fprintf(fp, "Num Keys: %d   Key size: %d  value size: %d\n", num_keys, key_size, value_size);
	
	memset(randomstuff, '\0', value_size);
	size_t val_len;
	memcached_st *memc;
	memcached_return rc;
	memcached_server_st *servers;
	size_t *lengths = (size_t *)malloc(num_keys * sizeof(size_t));
	srand(time(NULL));
	for (i = 0; i < value_size - 1; i++)
		randomstuff[i] = (char) (rand() % 26) + 97;
	//printf("random value: '%s'\n", randomstuff);
	//printf("random key time: ");
	//start_timer();
	// generate random keys
	keys = (char **)malloc(num_keys * sizeof(char **));
	for (i = 0; i < num_keys; i++) {
		keys[i] = (char *)malloc( key_size + 1);
		for(j = 0; j < key_size; j++)
			keys[i][j] = (char) (rand() % 26) + 97;
		keys[i][j] = 0;
		lengths[i] = strlen(keys[i]);
		//printf("key: '%s'\n", keys[i]);
	}
	//stop_timer(num_keys);
	val_len = strlen(randomstuff);
	//printf("str: '%s'\n", randomstuff);
	val = NULL;
	memc = memcached_create(NULL);
	
	char servername[]= "localhost";
	servers = memcached_server_list_append(NULL, servername, 0, &rc);
	rc = memcached_server_push(memc, servers);
	//memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_NO_BLOCK, &setter);
	//memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_TCP_NODELAY, &setter);
	//memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_MD5_HASHING, &setter);
	//memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
	//memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
	
	//setter = 20 * 1024576;
	//memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_SOCKET_SEND_SIZE, setter);
	//getter = memcached_behavior_get(memc, MEMCACHED_BEHAVIOR_SOCKET_SEND_SIZE);
	//printf("Socket send size: %ld\n", getter);
	//getter = memcached_behavior_get(memc, MEMCACHED_BEHAVIOR_SOCKET_RECV_SIZE);
	//printf("Socket recv size: %ld\n", getter);
	
	char *getval;
	uint32_t flags;
	// add key
	printf("SET %d keys... ", num_keys);
	fprintf(fp, "SET %d keys... ", num_keys);
	start_timer();
	
	for (i = 0; i < num_keys; i++) {
		#if 1
		memcached_set(memc, keys[i], strlen(keys[i]), randomstuff, strlen(randomstuff), 3600, 0);
		//printf("."); fflush(stdout);
		#endif
		#if 0
		memcached_set(memc, keys[i], strlen(keys[i]), "bar", strlen("bar"), 3600, 0);
		#endif
	}
	stop_timer(num_keys, fp);
	
	#if 1 
	printf("GET %d keys... ", num_keys);
	fprintf(fp, "GET %d keys... ", num_keys);
	start_timer();
	for (i = 0; i < num_keys; i++) {
		getval = memcached_get(memc,  keys[i], strlen(keys[i]), &val_len, &flags, &rc);
		//printf("'%s'­>'%s'\n\n", keys[i], getval);
		free(getval);
	}
	stop_timer(num_keys, fp);
	#endif
	
	#if 0
	printf("MGET %d keys... ", num_keys);
	char return_key[MEMCACHED_MAX_KEY];
	size_t return_keylen;
	char *return_value;
	size_t return_valuelen;
	start_timer();
	rc = memcached_mget(memc, keys, lengths, num_keys);
	for(i = 0; rc == MEMCACHED_SUCCESS; i++) {
		return_value = memcached_fetch(memc, return_key, &return_keylen, &return_valuelen, &flags, &rc);
		return_key[return_keylen] = 0;
		//printf("key: '%s'\n", return_key);
	}
	stop_timer(i);
	#endif
	
	fclose(fp);
	memcached_server_list_free(servers);
	memcached_free(memc);
	return 0;
}

