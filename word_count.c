#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "word_count.h"
#include "hashmap.h"
#include "bounded_buffer.h"
#define SIZE 10


struct arg_mapper {
    char *s; // single word
    int r;   // number of reducers
};

struct arg_reducer {
   int no_m;// locate reducer's own buffer
   int m; // num of mapper
};

struct buffered_queue *buffer;

int print_result(char*, int);
void *mapper(void *ptr);
void *reducer(void *ptr);
unsigned long crc32(const unsigned char *s, unsigned int len);

void word_count(int m, int r, char **text){
    // Malloc space for r buffers
    buffer = malloc(sizeof(struct buffered_queue) * r);
    // initial buffers
    for(int k =0;k<r;k++)
        buffered_queue_init((buffer+k),SIZE);

    
    const char delimiter[2] = " ";
    char *token;
    char *tmp;
    char *pp1;

    pthread_t p[m];
    pthread_t c[r];
    int i;
    struct arg_mapper *map1;

    // create mapper threads
    map1 = malloc(m * sizeof(struct arg_mapper));
    for(i =0;i<m;i++){
	struct arg_mapper *map2;
        map2 = map1+i;
	map2->s = text[i];
	//map1->r = malloc(sizeof(int));  
        map2->r = r;       
	pthread_create(&p[i], NULL, mapper, (void *)map2);
    }



    // create reducer threads
    struct arg_reducer *redu2;
    redu2 = malloc(r * sizeof(struct arg_reducer));
    for(int j =0;j<r;j++){
        //printf("iii %d\n",j);
	struct arg_reducer *redu1;
        redu1 = redu2 +j;
        redu1->m = m;
        redu1->no_m = j;
        //printf("no_m %d\n",redu1->no_m);	
    	pthread_create(&c[j], NULL, reducer, (void *)redu1);
    }

    printf("Result:\n");
    for(i=0;i<m;i++){
    pthread_join(p[i], NULL);
}
/**/
    for(int z=0;z<r;z++){
    pthread_join(c[z], NULL);
}
// free

    free(map1);
    free(redu2);
    for(int k =0;k<r;k++){
        buffered_queue_destroy((buffer+k));
    }
   free(buffer);
 
}
  
void *mapper(void *ptr){
      //printf("texted: %s\n",((struct arg_mapper *)ptr)->s);
      //printf("mapper create\n");
      char *tmp;
      //tmp = ((struct arg_mapper *)ptr)->r;
      //tmp=(char *)malloc(strlen(token));
      const char delimiter[2] = " ";
      char *pp = NULL;
      //pp= malloc(strlen(((struct arg_mapper *)ptr)->s));
//printf("pp size: %d\n",strlen(((struct arg_mapper *)ptr)->s));
      unsigned long i;
      //int num = ((struct arg_mapper *)ptr)->r;
	int num = ((struct arg_mapper *)ptr)->r;

	tmp = strtok_r((char *)(((struct arg_mapper *)ptr)->s), delimiter,&pp);
        //printf("temp: %s\n",tmp);

	while( tmp != NULL ) {       
    	    //printf("mapper: %s\n", tmp);
            i= crc32((unsigned char*)tmp,strlen(tmp));
           // printf("i: %d\n",i%num);
            buffered_queue_push((buffer+(i%num)),tmp);
      	    tmp =(char *)strtok_r(NULL,delimiter,&pp);
   	}
        // Mapper end
	//printf("ENd\n");
        /**/for(int j=0;j<num;j++){
                 //printf("special\n");
		char *c;
                //c = malloc( 2 * sizeof(char));
                c = "<";
        	buffered_queue_push((buffer+j),c);
}
    //printf("add1:%d",(int *)ptr);
    //free(ptr);
    return NULL;
}
/**/
void *reducer(void *ptr){
     //printf("reducer create\n");
    int num = ((struct arg_reducer *)ptr)->m;

    //printf("reudcer num %d\n", num);
/**/
    int finish=0;
    void *pop;
    int error;
    int i = ((struct arg_reducer *)ptr)->no_m;

//printf("reudcer num12 %d\n", (*(struct arg_reducer *)ptr).no_m);
    //printf("reudcer num1 %d\n", i);
    map_t mymap;
    mymap = hashmap_new();

    while(1){
	int count;
	/*If there exists a count for this word,
	       add the count by 1. Otherwise, set count
	       to 1*/
        if(finish == num){
		break;
	}


	pop = buffered_queue_pop((buffer+i));

	

        if(strcmp((char *)pop,"<") == 0){
             //printf("fin\n");
	     finish++;
             
        }else{
        	error = hashmap_get(mymap, (char *)pop, &count);
		if(error==MAP_OK)
           		hashmap_put(mymap, (char *)pop, count+1);
		else
	   		hashmap_put(mymap, (char *)pop, 1);
	}


    }
    hashmap_iterate(mymap, print_result);
    hashmap_free(mymap);

    return NULL;
}

int print_result(char *key, int value){
    printf("count of %s = %d\n", key, value);
    return 0;
}


