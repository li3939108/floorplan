#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>

#define MODULE_LIMIT 500
#define NET_LIMIT 500
#define TOTAL_WIDTH 100
#define TOTAL_HEIGHT 100

int TOTAL_MODULE  = 0;
int TOTAL_NET = 0;

enum read_module_file_state{
	module_index = 0,
	x_coordinate,
	y_coordinate,
	x_width,
	y_width
};

struct module{
	int module_index;
	int x_coordinate ;
	int y_coordinate;
	int x_width;
	int y_width;
	int pio ;
};
typedef struct module MODULE ;
struct net{
	int net_index ;
	int *module_list ;
	int module_count ;
};
typedef struct net NET;

struct fptreenode{
	char operator ;
	int node_number ;
	int width;
	int height ;
	struct fptreenode * left ; 
	struct fptreenode * right ;
//	struct fptreenode * parent ;
};
typedef struct fptreenode FPTREE ;

FPTREE  * solution ;

MODULE module_arr[MODULE_LIMIT];
NET net_arr[NET_LIMIT];

FPTREE * iter_construct_tree_H(int i,FPTREE * left, FPTREE * right, MODULE * module_arr);


FPTREE * initialize_solution( MODULE * module_arr){
	return iter_construct_tree_H(1, NULL, NULL, module_arr) ;
}
FPTREE * construct_tree (FPTREE * left, FPTREE * right, char operator){
	FPTREE * parent = malloc ( sizeof(FPTREE));
	parent->left = left ;
	parent->right = right ;
	parent->operator = operator ;
	if(operator == 'V'){
		parent->width = left->width + right->width ;
		parent->height = left->height > right->height ? left->height : right->height;
	}else{if(operator == 'H'){
		parent->width = left->width > right->width? left->width : right->width ;
		parent->height = left->height + right->height ;
	}else{
		perror("logical error, not an operator");
		exit(EXIT_FAILURE);
	}}
	return parent ;
}
FPTREE * iter_construct_tree_V(int i , FPTREE * left, FPTREE * right, MODULE *  module_arr ){
	int k , l ;
	if(left == NULL && right == NULL){
		if(i == TOTAL_MODULE ){
			FPTREE * temp = malloc(sizeof(FPTREE));
			temp->left= temp->right = NULL ;
			temp->width = module_arr[i].x_width ;
			temp->height = module_arr[i].y_width ;
			if(temp->width <= TOTAL_WIDTH){
				module_arr[i].x_coordinate = 0;
				return  temp;
			}else{
				return NULL ;
			}
		}else {if(i > TOTAL_MODULE){
			return NULL ;
		}else{
			k = i; l = k + 1;
		}}
		left = malloc(sizeof(FPTREE)) ;
		right = malloc(sizeof(FPTREE)) ;
		left->operator = right->operator =  'A' - 'A' ;
		left->node_number = k ;right->node_number = l ;
		left->width = module_arr[k].x_width ; right->width = module_arr[l].x_width ;		
		left->height = module_arr[k].y_width; right->height = module_arr[l].y_width ;
		left->left = right->left = left->right = right->right = NULL ;
		if(left->width + right->width <= TOTAL_WIDTH){
			module_arr[k].x_coordinate = 0;
			module_arr[l].x_coordinate = left->width ;
			return iter_construct_tree_V(l + 1, construct_tree(left, right, 'V'), NULL, module_arr);
		}else{if(left->width <= TOTAL_WIDTH){
			module_arr[i].x_coordinate = 0;
			return left ;	
		}else{
			return NULL ;
		}}
	}else{if(left != NULL && right == NULL){
		if(i > TOTAL_MODULE){
			return left;
		}else{
			FPTREE * right = malloc(sizeof(FPTREE));
			right->left= right->right = NULL ;
			right->width = module_arr[i].x_width ;
			right->height = module_arr[i].y_width ;
			right->node_number = i ;
			if(left->width + right->width <= TOTAL_WIDTH){
				module_arr[i].x_coordinate = left->width ;
				return iter_construct_tree_V(i + 1, construct_tree(left, right , 'V'), NULL, module_arr);
			}else{
				return left ;
			}
		}
	}}
}
FPTREE * iter_construct_tree_H(int i, FPTREE * left, FPTREE * right, MODULE * module_arr){
	if(left == NULL && right == NULL){
		left = iter_construct_tree_V(i, NULL, NULL, module_arr);
		FPTREE * temp = left;
		while(temp->operator != 0){temp = temp -> right ;}
		right = iter_construct_tree_V(temp->node_number + 1, NULL, NULL, module_arr);
		if(left == NULL){
			return NULL ;
		}
		if(right == NULL){
			return left ;
		}else{
			return iter_construct_tree_H(right->right == NULL ? right->node_number : right->right->node_number ,construct_tree(left, right, 'H'), NULL, module_arr);
		}
	}else{if(left != NULL && right == NULL){
		FPTREE * temp = left;
		while(temp->operator != 0){temp = temp -> right ;}
		right = iter_construct_tree_V(temp->node_number + 1, NULL, NULL, module_arr);
		if(right == NULL){
			return left ;
		}else{
			return iter_construct_tree_H(right->right == NULL ? right->node_number : right->right->node_number ,construct_tree(left, right, 'H'), NULL, module_arr);
		}
	}}
}
int iter_update_module(int x, int y, FPTREE * solution, MODULE * module_arr){
	
}
int solution_cost(FPTREE * solution, MODULE * module_arr){
}
int main(int argc, char ** argv){
	int i = 1;
	FILE	*moduleFile = NULL,
		*netFile = NULL,
		*pioFile = NULL;
	memset( module_arr, 0, sizeof(MODULE) * MODULE_LIMIT); 
	memset( net_arr, 0, sizeof(NET) * NET_LIMIT); 
	/*
	 * Read parameters from terminal
	 */
	while( i < argc){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
			int iter  ;
			case 'm':

			if( i + 1 < argc){moduleFile = fopen(argv[i + 1], "r") ;}
			else{
				perror("Unspecified parameter for module file");
				exit(EXIT_FAILURE) ;
			}
			i = i + 2 ;
			continue ;
			break;
			
			case 'n':
			if( i + 1 < argc){netFile = fopen(argv[i + 1], "r") ;}
			else{
				perror("Unspecified parameter for net file");
				exit(EXIT_FAILURE) ;
			}
			i = i + 2;
			continue;
			break ;
			
			case 'p':
			if( i + 1 < argc){pioFile = fopen(argv[i + 1], "r") ;}
			else{
				perror("Unspecified parameter for pio file");
				exit(EXIT_FAILURE) ;
			}
			i = i + 2;
			continue;
			break ;

			default:
			perror("parameter error");
			exit(EXIT_FAILURE);
			break;
			}
		}else{
			perror("parameter error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	if(pioFile == NULL || netFile == NULL || moduleFile == NULL){
		perror("too few input file");
		exit(EXIT_FAILURE);
	}
	read_module_file(moduleFile);
	read_pio_file(pioFile);
	read_net_file(netFile);
	TOTAL_MODULE = total_module(module_arr);
	TOTAL_NET = total_net(net_arr) ;
	solution = initialize_solution( module_arr);
	print_module(module_arr);
	print_net(net_arr);
	return 0;
}
/*
 * Print the module array
 */
int print_module(MODULE * module_arr){
	int i;
	printf("total_module: %d\n", TOTAL_MODULE);
	for(i = 0; i < MODULE_LIMIT; i++){
		if(module_arr[i].x_width == 0){continue;}
		printf("[%d] x:%d y:%d xw:%d yw:%d pio:%s\n", i, module_arr[i].x_coordinate, module_arr[i].y_coordinate, module_arr[i].x_width, module_arr[i].y_width, module_arr[i].pio ? "yes" : "no");
	}
}
/*
 * Get the total module number 
 */
int total_module(MODULE * module_arr){
	int i, j = 0;
	for(i = 0; i < MODULE_LIMIT; i++){
		if(module_arr[i].x_width == 0){continue;}
		j += 1 ;
	}
	return j ;
}
/*
 * Read the module file
 */
int read_module_file(FILE * moduleFile){
	int state = 0 ;
	int number ;
	int mindex ;
	for(;;){
		char c = fgetc(moduleFile) ;
		switch(c){
		case ' ':
		state += 1;
		continue;
		break;
		
		case '\n':
		state = 0;
		break;
		
		case EOF:
		return 0;
		
		default:
		if( c >= '0' && c <= '9'){
			ungetc(c, moduleFile);
			fscanf(moduleFile, "%d", &number); 
		}
		switch(state){
			case module_index:
			mindex = number ;
			break;

			case x_coordinate:
			module_arr[mindex].x_coordinate = number ;
			break;

			case y_coordinate:
			module_arr[mindex].y_coordinate = number ;
			break;

			case x_width:
			module_arr[mindex].x_width = number ;
			break;

			case y_width:
			module_arr[mindex].y_width = number ;
			break;

			default:
			perror("logical error");
			exit(EXIT_FAILURE);
			break;
		}
		break;
		}
	}
	return 0;
}
int read_net_file(FILE * netFile){
	int state = 0;
	int number ;
	int nindex;
	for(;;){
		char c = fgetc(netFile);
		switch(c){
		case '\n':
		state = 0;
		break ;

		case EOF:
		return 0;

		default:
		if(c >= '0' && c <= '9'){
			ungetc(c, netFile);
			fscanf(netFile, "%d", &number);
		}else{break;}
		switch(state){
			case 0:
			nindex = number ;		
			state = 1;
			break ;

			case 1:
			net_arr[nindex].module_count += 1;
			net_arr[nindex].module_list = realloc(net_arr[nindex].module_list, net_arr[nindex].module_count * sizeof(int) );
			net_arr[nindex].module_list[net_arr[nindex].module_count - 1] = number ;
			break ;
			
			default:
			perror("logical error");
			exit(EXIT_FAILURE);
			break;
		}
		break;
		}
	}	
}
/*
 * Get the total net number
 */
int total_net(NET * net_arr){
	int i , j = 0;
	for(i = 0; i < NET_LIMIT; i++){
		if(net_arr[i].module_count == 0){continue;}
		j += 1 ;
	}
	return j;

}
/*
 * Print the net array
 */
int print_net(NET * net_arr){
	int i;
	for(i = 0; i < NET_LIMIT; i++){
		if(net_arr[i].module_count == 0){continue;}
		printf("[%d](%d) \n", i, net_arr[i].module_count);
	}
	return 0;
}
/*
 * Reat the pio file
 */
int read_pio_file(FILE * pioFile){
	int state = 0, number ;
	for(;;){
		char c = fgetc(pioFile); 
		switch(c){
			case '\n':
			state = 0;
			break ;
			
			case EOF:
			return 0;

			default:
			if(c <= '9' && c >= '0'){
				ungetc(c, pioFile);
				fscanf(pioFile, "%d", &number);
			}else{continue;}
			switch(state){
				case 0:
				module_arr[number].pio = 1;
				state = 1;
				break ;
				
				default:
				perror("logical error");
				exit(EXIT_FAILURE);
				break;
			}
			break;
		}
	}
}
