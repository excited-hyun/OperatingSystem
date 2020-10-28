#include "main.h"

struct list List[10];
struct bitmap *Bitmap[10];
struct hash Hash[10];


int main(void){
	char get_command[100];
	char **command;
	char temp[100];
	char *token;
	int cnt = 0;
	int token_cnt = 0;

	while(1){
		fgets(get_command, sizeof(get_command), stdin);

		get_command[strlen(get_command)-1] = '\0';
		strcpy(temp, get_command);

		token_cnt = 0;
		token = strtok(temp, " ");

		while(token){
			token = strtok(NULL, " \n\t\r");
			token_cnt++;
		}//token수 계산

		strcpy(temp, get_command);

		command = (char**)malloc(sizeof(char*)*token_cnt);
		for(int i=0; i<token_cnt; i++){
			command[i] = (char*)malloc(sizeof(char)*50);
		}

		token = strtok(temp, " ");
		strcpy(command[0], token);

		for(int i=1; i<token_cnt; i++){
			token = strtok(NULL, " \n\t\r");
			strcpy(command[i], token);
		}

		//quit
		if(strcmp(command[0], "quit")==0)
			break;

		//create
		else if(strcmp(command[0], "create")==0){
			int index;
			//create list
			if(strcmp(command[1], "list")==0){
				index = command[2][4]-'0';
				if(index < 10){
					list_init(&List[index]);
				}
			}

			//create hashtable
			else if(strcmp(command[1], "hashtable")==0){
				index = command[2][4]-'0';
				if(index < 10){
					hash_init(&Hash[index], hash_hash, hash_less, NULL);
				}
			}

			//create bitmap
			else if(strcmp(command[1], "bitmap") == 0){
				index = command[2][2]-'0';
				if(index < 10){
					unsigned len;
					sscanf(command[3], "%u", &len);
					Bitmap[index] = bitmap_create((size_t)len);
				}
			}
		}
	

		//dumpdata
		else if (strcmp(command[0], "dumpdata") == 0){
			int index;
			strncpy(temp, command[1], strlen(command[1])-1);
			temp[strlen(command[1])-1] = '\0';
			//dumpdata list
			if(strcmp(temp, "list")==0){
				struct list_elem *e;
				index = command[1][4]-'0';
				//printf("list %d\n", index);

				if(list_empty(&List[index]) != true){
					for(e = list_begin(&List[index]); e!=list_end(&List[index]); e = list_next(e)){
						struct list_item *use = list_entry(e, struct list_item, elem);
						printf("%d ", use->data);
					}
					printf("\n");
				}
					
			}

			//dumpdata hash
			else if(strcmp(temp, "hash") == 0){
				struct hash_item *ha;
				struct hash_iterator it;
				int cnt=0;
				index = command[1][4] - '0';
				
				if(hash_empty(&Hash[index]) != true){
					//printf("a\n");
					hash_first(&it, &Hash[index]);
					while(hash_next(&it)){
						//printf("b\n");
						ha = hash_entry(hash_cur(&it), struct hash_item, elem);
						printf("%d ", ha->data);
						cnt++;
					}
					if(cnt != 0)
						printf("\n");
				}

			}

			//dumpdata bm
			else if(strcmp(temp, "bm") == 0){
				unsigned size;
				index = command[1][2] - '0';
				if(Bitmap[index] != NULL){
					size = bitmap_size(Bitmap[index]);

					for(unsigned i=0; i<size; i++){
						if(bitmap_test(Bitmap[index], i)){
							printf("1");
						}
						else{
							printf("0");
						}
					}
					printf("\n");

				}
			}
		}

		//delete
		else if(strcmp(command[0], "delete")==0){
			int index;
			strncpy(temp, command[1], strlen(command[1])-1);
			temp[strlen(command[1])-1] = '\0';

			//delete list
			if(strcmp(temp,"list")==0) {
				index = command[1][4] - '0';
				//printf("here\n");
				
				while(1) {
					//printf("hi\n");
					if(list_empty(&List[index]) == true){
						//printf("!!!\n");
						break;
					}
					struct list_elem *e = list_pop_back(&List[index]);
					//free(e);
				}
			}

			//delete hash
			else if(strcmp(temp, "hash") == 0){

				index = command[1][4] - '0';
				hash_destroy(&Hash[index], destructor);

			}

			//delete bm
			else if(strcmp(temp, "bm") == 0){

				index = command[1][2] - '0';
				bitmap_destroy(Bitmap[index]);
			}
		}

		//--------------------------hash-----------------------------------
		else if(strcmp(command[0], "hash_apply")==0){
			int index = command[1][4] - '0';

			//hash_apply triple
			if(strcmp(command[2], "triple")==0) 
				hash_apply(&Hash[index], triple);
			//hash_apply square
			else if(strcmp(command[2], "square")==0) 
				hash_apply(&Hash[index], square);
		}

		//hash_insert
		else if(strcmp(command[0], "hash_insert")==0){
			int index = command[1][4] -'0';
			int new_data;
			struct hash_item* item = (struct hash_item*)malloc(sizeof(struct hash_item));

			new_data = atoi(command[2]);
			item->data = new_data;
			//printf("%d\n", item->data);
			hash_insert(&Hash[index], &item->elem);
		}
		//hash_delete
		else if(strcmp(command[0], "hash_delete")==0){
			int index = command[1][4] - '0';
			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *item = hash_entry(e, struct hash_item, elem);
			item->data = atoi(command[2]);

			struct hash_elem *node = hash_delete(&Hash[index], e);
		}

		//hash_empty
		else if(strcmp(command[0], "hash_empty")==0){
			int index = command[1][4] - '0';

			if(hash_empty(&Hash[index]) == true)	//empty
				printf("true\n");
			else		//not empty
				printf("false\n");

		}

		//hash_size
		else if(strcmp(command[0], "hash_size")==0){
			int index = command[1][4] - '0';
			printf("%u\n", (unsigned)hash_size(&Hash[index]));
		}

		//hash_clear
		else if(strcmp(command[0], "hash_clear")==0){
			int index = command[1][4] - '0';
			hash_clear(&Hash[index], destructor);
		}

		//hash_find
		else if(strcmp(command[0], "hash_find")==0){
			int index = command[1][4] - '0';
			int find;
			find = atoi(command[2]);

			struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item* item = hash_entry(e, struct hash_item, elem);
			item->data = find;

			struct hash_elem* result = hash_find(&Hash[index], e);

			if(result != NULL)
				printf("%d\n", find);
		}
		//hash_replace
		else if(strcmp(command[0], "hash_replace")==0){
			int index = command[1][4] -'0';
			int new_data;
			new_data = atoi(command[2]);
			struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item* item = hash_entry(e, struct hash_item, elem);
			item->data = new_data;

			hash_replace(&Hash[index], e);
		}


		//---------------------------bitmap-------------------------------------

		//bitmap_mark
		else if(strcmp(command[0], "bitmap_mark")==0){
			int index, bit;
			index = command[1][2]-'0';
			bit = atoi(command[2]);
			bitmap_mark(Bitmap[index], bit);

		}

		//bitmap_all
		else if(strcmp(command[0], "bitmap_all")==0){
			int index, start, cnt;
			
			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);

			if(bitmap_all(Bitmap[index], start, cnt)) 
				printf("true\n");
			else 
				printf("false\n");
		}
		//bitmap_any
		else if(strcmp(command[0], "bitmap_any")==0){
			int index, start, cnt;
			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);

			if(bitmap_any(Bitmap[index], start, cnt)) 
				printf("true\n");
			else 
				printf("false\n");
		}
		//bitmap_contains
		else if(strcmp(command[0], "bitmap_contains")==0){
			int index, start, cnt;
			bool value;
			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);
			value = true;

			if (strcmp(command[4], "false") == 0) 
				value = false;

			if((bitmap_contains(Bitmap[index], start, cnt, value))==true) 
				printf("true\n");
			else 
				printf("false\n");
		}
		//bitmap_count
		else if(strcmp(command[0], "bitmap_count")==0){
			int index, start, cnt;
			bool value = true;
			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);

			if(strcmp(command[4],"false")== 0) 
				value = false;
			
			printf("%d\n", (int)bitmap_count(Bitmap[index], start, cnt, value));
		}
		//bitmap_dump
		else if(strcmp(command[0], "bitmap_dump")==0){
			int index;
			index = command[1][2]-'0';
			bitmap_dump(Bitmap[index]);
		}
		//bitmap_expand
		else if(strcmp(command[0], "bitmap_expand")==0){
			int index;
			index = command[1][2]-'0';
			Bitmap[index] = bitmap_expand(Bitmap[index], atoi(command[2]));
		}
		//bitmap_set_all
		else if(strcmp(command[0], "bitmap_set_all")==0){
			int index;
			bool value = true;
			index = command[1][2]-'0';
			if(strcmp(command[2],"false")==0) 
				value = false;
			bitmap_set_all(Bitmap[index], value);
		}
		//bitmap_flip
		else if(strcmp(command[0], "bitmap_flip")==0){
			int index, cnt;
			
			index = command[1][2]-'0';
			cnt = atoi(command[2]);
			bitmap_flip(Bitmap[index], cnt);

		}
		//bitmap_none
		else if(strcmp(command[0], "bitmap_none")==0){
			int index, start, cnt;
			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);

			if(bitmap_none(Bitmap[index], start, cnt) == true) 
				printf("true\n");
			else 
				printf("false\n");
		}
		//bitmap_reset
		else if(strcmp(command[0], "bitmap_reset")==0){
			int index = command[1][2]-'0';
			bitmap_reset(Bitmap[index], (unsigned)atoi(command[2]));
		}
		//bitmap_scan_and_flip
		else if(strcmp(command[0], "bitmap_scan_and_flip")==0){
			int index, start, cnt;
			bool value = true;

			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);

			if(strcmp(command[4],"false")==0) 
				value = false;

			printf("%zu\n",bitmap_scan_and_flip(Bitmap[index], start, cnt, value));
		}
		//bitmap_scan
		else if(strcmp(command[0], "bitmap_scan")==0){
			int index, start, cnt;
			bool value = true;
			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);

			if(strcmp(command[4],"false")==0) 
				value = false;
			printf("%zu\n",bitmap_scan(Bitmap[index], start, cnt, value));

		}
		//bitmap_set
		else if(strcmp(command[0], "bitmap_set")==0){
			int index, idx;
			bool value = true;
			index = command[1][2]-'0';
			idx = atoi(command[2]);

			if(strcmp(command[3],"false")==0) 
				value = false;

			bitmap_set(Bitmap[index], idx, value);
		}
		//bitmap_set_multiple
		else if(strcmp(command[0], "bitmap_set_multiple")==0){
			int index, start, cnt;
			bool value = true;

			index = command[1][2]-'0';
			start = atoi(command[2]);
			cnt = atoi(command[3]);
			value = true;

			if(strcmp(command[4],"false")==0) 
				value = false;

			bitmap_set_multiple(Bitmap[index], start, cnt, value);
		}
		//bitmap_size
		else if(strcmp(command[0], "bitmap_size")==0){
			int index = command[1][2]-'0';
			printf("%u\n", (unsigned)bitmap_size(Bitmap[index]));
		}
		//bitmap_test
		else if(strcmp(command[0], "bitmap_test")==0){
			int index = command[1][2]-'0';

			if(bitmap_test(Bitmap[index], atoi(command[2])) == true) printf("true\n");
			else printf("false\n");
		}


		//---------------------------list----------------------
		//list_front
		else if(strcmp(command[0], "list_front")==0){
			int index = command[1][4]-'0';
			struct list_elem *e = list_front(&List[index]);
			struct list_item *item = list_entry(e, struct list_item, elem);

			if(item != NULL) 
				printf("%d\n", item->data);
		}
		//list_back
		else if(strcmp(command[0], "list_back")==0){

			int index = command[1][4]-'0';
			struct list_elem *e = list_back(&List[index]);
			struct list_item *item = list_entry(e, struct list_item, elem);

			if(item != NULL) 
				printf("%d\n", item->data);
		}
		//list_pop_front
		else if(strcmp(command[0], "list_pop_front")==0){
			int index = command[1][4]-'0';
			list_pop_front(&List[index]);
		}
		//list_pop_back
		else if(strcmp(command[0], "list_pop_back")==0){
			int index = command[1][4]-'0';
			list_pop_back(&List[index]);
		}
		//list_insert
		else if(strcmp(command[0], "list_insert")==0){
			int index, list_num, new_data;

			index = command[1][4]-'0';
			list_num = atoi(command[2]);	//insert 위치
			new_data = atoi(command[3]);	//insert할 data

			struct list_elem *e = (struct list_elem *)malloc(sizeof(struct list_elem));
			struct list_item *item = list_entry(e, struct list_item, elem);

			item->data = new_data;

			if(list_num == 0){
				list_push_front(&List[index], e);
				continue;
			}//맨앞

			else if( list_num > list_size(&List[index])-1){
				list_push_back(&List[index],e);
				continue;
			}//맨뒤

			struct list_elem *prev;
			prev = list_begin(&List[index]);

			for(int i=0; i<list_num; i++){
				prev = list_next(prev);
			}//inset할 위치까지
			list_insert(prev, e);
			
		}
		//list_inser_ordered
		else if(strcmp(command[0], "list_insert_ordered")==0){
			int index, new_data;
			index = command[1][4] - '0';
			new_data = atoi(command[2]);

			struct list_elem *e = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *item = list_entry(e, struct list_item, elem);
			item->data = new_data;

			list_insert_ordered(&List[index], e,list_less, NULL);
		}
		//list_push_back
		else if(strcmp(command[0], "list_push_back")==0){
			int index, new_data;

			index = command[1][4]-'0';
			new_data = atoi(command[2]);

			struct list_elem *e = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *item = list_entry(e, struct list_item, elem);
			item->data = new_data;

			list_push_back(&List[index], e);
		}
		//list_push_front
		else if(strcmp(command[0], "list_push_front")==0){
			int index, new_data;

			index = command[1][4]-'0';
			new_data = atoi(command[2]);

			struct list_elem *e = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *item = list_entry(e, struct list_item, elem);
			item->data = new_data;
			list_push_front(&List[index], e);
		}
		//list_empty
		else if(strcmp(command[0], "list_empty")==0){
			int index = command[1][4]-'0';
			if (list_empty(&List[index]) == true) 
				printf("true\n");
			else 
				printf("false\n");
		}
		//list_size
		else if(strcmp(command[0], "list_size")==0){
			int index, size;
			index = command[1][4]-'0';
			size = list_size(&List[index]);
			printf("%d\n", size);
		}

		//list_max
		else if(strcmp(command[0], "list_max")==0){
			int index;

			index = command[1][4]-'0';
			struct list_elem *e = list_max(&List[index], list_less, NULL);
			struct list_item *item = list_entry(e,struct list_item, elem);

			printf("%d\n",item->data);
		}
		//list_min
		else if(strcmp(command[0], "list_min")==0){
			int index;

			index = command[1][4]-'0';
			struct list_elem *e = list_min(&List[index], list_less, NULL); 
			struct list_item *item = list_entry(e,struct list_item, elem);
			printf("%d\n",item->data);
		}
		//list_remove
		else if(strcmp(command[0], "list_remove")==0){
			int index, num;
			index = command[1][4] - '0';
			num = atoi(command[2]);

			struct list_elem *e = list_begin(&List[index]);

			for(int i=0; i<num; i++) {
				e = list_next(e);
			}

			list_remove(e);

		}

		//list_reverse
		else if(strcmp(command[0], "list_reverse")==0){
			int index;
			index = command[1][4] - '0';
			list_reverse(&List[index]);
		}
		//list_shuffle
		else if(strcmp(command[0], "list_shuffle")==0){

			int index;
			index = command[1][4] - '0';
			list_shuffle(&List[index]);
			
		}
		//list_sort
		else if(strcmp(command[0], "list_sort")==0){

			int index;
			index = command[1][4] - '0';
			list_sort(&List[index], list_less, NULL);
		}
		//list_splice
		else if(strcmp(command[0], "list_splice")==0){
			int index1, index2, before, first, last;

			index1 = command[1][4] - '0';
			before = atoi(command[2]);

			index2 = command[3][4] - '0';
			first = atoi(command[4]);
			last = atoi(command[5]);

			struct list_elem *e_before = list_begin(&List[index1]);
			for( int i=0; i<before; i++){
				e_before = list_next(e_before);
			}

			struct list_elem *e_first = list_begin(&List[index2]);
			for(int i=0; i<first; i++){
				e_first = list_next(e_first);
			}

			struct list_elem *e_last = list_begin(&List[index2]);
			for(int i=0; i<last; i++){
				e_last = list_next(e_last);
			}

			list_splice(e_before, e_first, e_last);

		}
		//list_swap
		else if(strcmp(command[0], "list_swap")==0){

			int index, swapa, swapb;
			index = command[1][4] - '0';
			swapa = atoi(command[2]);
			swapb = atoi(command[3]);

			struct list_elem *a = list_begin(&List[index]);
			for (int i=0; i<swapa; i++)
			{
				a = list_next(a);
			}

			struct list_elem *b = list_begin(&List[index]);
			for (int i=0; i<swapb; i++)
			{
				b = list_next(b);
			}


			list_swap(a, b);
		}
		//list_unique
		else if(strcmp(command[0], "list_unique")==0){
			int index1, index2;
			index1 = command[1][4] - '0';

			if(token_cnt == 2){
				list_unique(&List[index1], NULL, list_less, NULL);
			}
			else if(token_cnt == 3){
				index2 = command[2][4] - '0';
				list_unique(&List[index1], &List[index2], list_less, NULL);
			}
		}

	}

	return 0;
}
