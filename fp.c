#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define __FP_H__

#define MODULE_LIMIT 500
#define OPERATOR_LIMIT 1000
#define NET_LIMIT 500
#define TOTAL_WIDTH 100
#define TOTAL_HEIGHT 100

#define AREA 1
#define RATIO 10
#define SEMI 3
#define PIO 6

int TOTAL_MODULES  = 0;
int TOTAL_NETS = 0;

enum read_module_file_state{
	module_index = 0,
	x_coordinate,
	y_coordinate,
	x_width,
	y_width
};
enum transition_type{
	SWAP_OPERATOR = 0,
	FLIP  ,
	SWAP_OPERANT
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
};
typedef struct fptreenode FPTREE ;

FPTREE  * solution ;
FPTREE * operators[OPERATOR_LIMIT];
FPTREE * operants[MODULE_LIMIT];
FPTREE  * nodes[3*OPERATOR_LIMIT];

MODULE module_arr[MODULE_LIMIT];
NET net_arr[NET_LIMIT];

FPTREE * iter_construct_tree_H(int i,FPTREE * left, FPTREE * right, MODULE * module_arr);
long solution_cost(FPTREE * solution,  NET * net_arr,  MODULE * module_arr);
extern anneal(NET * net_arr, MODULE * module_arr) ;


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
		if(i == TOTAL_MODULES ){
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
		}else {if(i > TOTAL_MODULES){
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
		if(i > TOTAL_MODULES){
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
int iter_update_module(int x, int y, FPTREE * node, MODULE * module_arr){
	switch(node->operator ){
		case 'V':
		iter_update_module(x, y, node->left, module_arr);
		iter_update_module(x + node->left->width, y, node->right, module_arr);
		break;
		case 'H':
		iter_update_module(x, y, node->left, module_arr);
		iter_update_module(x, y + node->left->height, node->right, module_arr);
		break;

		default:
		module_arr[node->node_number].x_coordinate = x ;
		module_arr[node->node_number].y_coordinate = y ;
		break;
	}
}
void iter_update_tree(FPTREE * node){
	if(node->operator == 0){
		return ;
	}else{
		iter_update_tree(node->left);
		iter_update_tree(node->right);
		if(node->operator == 'V'){
			node->width = node->left->width + node->right->width ;
			node->height = node->left->height > node->right->height ? node->left->height :node->right->height ;
		}else{ if(node->operator == 'H'){
			node->height = node->left->height + node->right->height ;
			node->width = node->left->width > node->right->width ? node->left->width :node->right->width ;
		}}
	}
}

long solution_cost(FPTREE * solution,  NET * net_arr,  MODULE * module_arr){
	int area = solution->width * solution ->height ;
	float ratio = (0.0 + abs(solution->width - solution->height)) / (solution->width > solution->height ? solution->width : solution->height);
	int i , semip = 0, pioleng = 0;
	for(i = 1; i <= TOTAL_NETS; i ++){
		int j , mxx = 0, mxy = 0, minx = TOTAL_WIDTH, miny = TOTAL_HEIGHT;
		for ( j = 0; j < net_arr[i].module_count; j++){
			if(module_arr[net_arr[i].module_list[j]].x_coordinate < minx){
				minx = module_arr[net_arr[i].module_list[j]].x_coordinate ;
			}
			if(module_arr[net_arr[i].module_list[j]].x_coordinate > mxx){
				mxx = module_arr[net_arr[i].module_list[j]].x_coordinate ;
			}
			if(module_arr[net_arr[i].module_list[j]].y_coordinate < miny){
				miny = module_arr[net_arr[i].module_list[j]].y_coordinate ;
			}
			if(module_arr[net_arr[i].module_list[j]].y_coordinate > mxy){
				mxy = module_arr[net_arr[i].module_list[j]].y_coordinate ;
			}
		}
		semip += mxx - minx + mxy - miny ;
		printf("net %d: minx: %d miny: %d maxx: %d maxy: %d\n", i, minx, miny, mxx, mxy );
	}
	for(i = 1; i <= TOTAL_MODULES ; i ++){
		if(module_arr[i].pio){
			pioleng += module_arr[i].x_coordinate + module_arr[i].y_coordinate ;
		}
	}
//	printf("semi: %d\npioleng: %d\nwidth: %d\nheight: %d\n", semip , pioleng, solution->width, solution->height);
//	printf("COST: %d\n", AREA * area + RATIO * ratio + SEMI * semip + PIO * pioleng) ;
	return AREA * area + (int)(RATIO * ratio) + SEMI * semip + PIO * pioleng ;
}
void  traverse_operator_list(FPTREE * node){
	static int operator_index = 0;
	static int operant_index = 0;
	if(node->operator == 'V' || node->operator == 'H'){
		operators[operator_index] = node ;
		operator_index += 1; 
	}else{
		operants[operant_index] = node ;
		operant_index += 1;
		return ;
	}
	traverse_operator_list(node->left);
	traverse_operator_list(node->right);
}
void traverse_list(FPTREE * node){
	static int index = 0;
	if(node->operator == 0){
		nodes[index] = node ;
		index += 1;
		return ;
	}
	traverse_list(node->left);
	traverse_list(node->right);
	nodes[index] = node ;
	index += 1;
	return ;
}
FPTREE * list2tree(FPTREE * nodes[]){
	FPTREE * stack [3* MODULE_LIMIT] ;
	int i = 0;
	int stack_index = 0;
	for(i = 0; i < 3 * MODULE_LIMIT; i++){
		if(nodes[i] != NULL){
			if(nodes[i]->operator == 0){
				stack[stack_index] = nodes[i] ;
				stack_index += 1 ;
			}else{
				nodes[i]->right = stack[stack_index - 1] ;
				nodes[i]->left = stack[stack_index - 2];
				stack[stack_index - 2] = nodes[i];
				stack_index -= 1;
			}
		}else{
			return stack[0] ;
		}
	}
}
void print_list(FPTREE ** list){
	int i;
	for(i = 0; i <OPERATOR_LIMIT; i++){
		if(list[i] == NULL){
			break;
		}else{
			if(list[i]->operator != 0){
				printf("%c ", list[i]->operator);
			}else{
				printf("%d ", list[i]->node_number);
			}
		}
	}
	putchar('\n');
}
int transition(NET * net_arr, MODULE * module_arr, int i1){
	int initcost = solution_cost(solution, net_arr, module_arr);
	int type ;
	FPTREE * temp ;
	if(nodes[i1]->operator == 'V' || nodes[i1]->operator == 'H'){
		int i ;
		for(i = i1; ; i--){
			if(nodes[i] == NULL || nodes[i]->operator == 0){
				break;
			}else{
				if(nodes[i]->operator == 'H'){
					nodes[i]->operator = 'V';
				}else{if(nodes[i]->operator == 'V'){
					nodes[i]->operator = 'H';
				}}
			}
		}
		for(i = i1; ; i++){
			if(nodes[i] == NULL || nodes[i]->operator == 0){
				break;
			}else{
				if(nodes[i]->operator == 'H'){
					nodes[i]->operator = 'V';
				}else{if(nodes[i]->operator == 'V'){
					nodes[i]->operator = 'H';
				}}
			}
		}
		type = 0 ;
	}else{if(nodes[i1]->operator == 0){
		int i , operant_count = 0, operator_count = 0;
		temp = nodes[i1] ;
		nodes[i1] = nodes[i1 + 1] ;
		nodes[i1 + 1] = temp ;
		for(i = i1; i >= 0; i--){
			if(nodes[i] == NULL || nodes[i]->operator == 0){
				operant_count += 1;
			}else{
				operator_count += 1;
			}
		}
		if( operator_count < operant_count && (i1 == 0 || nodes[i1 - 1]->operator != nodes[i1]->operator )){//legal normalized polish expression
		}else{
			nodes[i1 + 1] = nodes[i1];
			nodes[i1] = temp ;
			return 0;
		}
		type = 1;
	}}
	solution = list2tree(nodes);
	iter_update_tree(solution);
	iter_update_module(0, 0, solution, module_arr);
	if(solution->width <= 100 && solution->height <= 100){
		return solution_cost(solution, net_arr, module_arr) - initcost ;
	}else{
		if(type == 0){
			int i ;
			for(i = i1; ; i--){
				if(nodes[i] == NULL || nodes[i]->operator == 0){
					break;
				}else{
					if(nodes[i]->operator == 'H'){
						nodes[i]->operator = 'V';
					}else{if(nodes[i]->operator == 'V'){
						nodes[i]->operator = 'H';
					}}
				}
			}
			for(i = i1; ; i++){
				if(nodes[i] == NULL || nodes[i]->operator == 0){
					break;
				}else{
					if(nodes[i]->operator == 'H'){
						nodes[i]->operator = 'V';
					}else{if(nodes[i]->operator == 'V'){
						nodes[i]->operator = 'H';
					}}
				}
			}
		}else{if(type == 1){
			FPTREE * temp ;
			temp = nodes[i1] ;
			nodes[i1 ] = nodes[i1 + 1] ;
			nodes[i1 + 1] = temp ;
		}}
		solution = list2tree(nodes);
		iter_update_tree(solution);
		iter_update_module(0,0,solution, module_arr);
		return 0;
	}
}
int main(int argc, char ** argv){
	int i = 1;
	FILE	*moduleFile = NULL,
		*netFile = NULL,
		*pioFile = NULL;
	memset( module_arr, 0, sizeof(MODULE) * MODULE_LIMIT); 
	memset( net_arr, 0, sizeof(NET) * NET_LIMIT); 
	memset( nodes, 0, sizeof(FPTREE *) * 3 * OPERATOR_LIMIT);
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
	TOTAL_MODULES = total_module(module_arr);
	TOTAL_NETS = total_net(net_arr) ;
	solution = initialize_solution( module_arr);
	printf("area: %d x %d = %d\n", solution->width, solution->height, solution->width * solution->height) ;
	iter_update_module(0, 0, solution, module_arr);
	solution_cost(solution, net_arr, module_arr);
	traverse_operator_list(solution);
	traverse_list(solution);
	//print_list(operators);
	//print_list(operants);
	//print_list(nodes);
	//traverse_list(list2tree(nodes));
	//print_list(nodes);
	print_module(module_arr);
	print_net(net_arr);
	anneal( net_arr, module_arr);
	return 0;
}
/*
 * Print the module array
 */
int print_module(MODULE * module_arr){
	int i;
	printf("total_module: %d\n", TOTAL_MODULES);
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
	printf("total_net: %d\n", total_net(net_arr));
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
