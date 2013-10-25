#ifndef __FP_H__
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


extern int TOTAL_MODULES  ;
extern int TOTAL_NETS ;

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

extern FPTREE  * solution ;
extern FPTREE * operators[OPERATOR_LIMIT];
extern FPTREE * operants[MODULE_LIMIT];

extern MODULE module_arr[MODULE_LIMIT];
extern NET net_arr[NET_LIMIT];

extern FPTREE * iter_construct_tree_H(int i,FPTREE * left, FPTREE * right, MODULE * module_arr);
extern long solution_cost(FPTREE * solution,  NET * net_arr,  MODULE * module_arr);
extern int transition(FPTREE * solution, NET* net_arr, MODULE * module_arr, int i1);

#endif
