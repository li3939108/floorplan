#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODULE_LIMIT 500
#define NET_LIMIT 500


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
};
typedef struct module MODULE ;
struct net{
	int net_index ;
	int *module_list ;
	int module_count ;
};
typedef struct net NET;

MODULE module_arr[MODULE_LIMIT];
NET net_arr[NET_LIMIT];

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
	print_module(module_arr);
	read_net_file(netFile);
	print_net(net_arr);
	return 0;
}
/*
 * Print the module array
 */
int print_module(MODULE * module_arr){
	int i;
	for(i = 0; i < MODULE_LIMIT; i++){
		if(module_arr[i].x_width == 0){continue;}
		printf("[%d] x:%d y:%d xw:%d yw:%d\n", i, module_arr[i].x_coordinate, module_arr[i].y_coordinate, module_arr[i].x_width, module_arr[i].y_width);
	}
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
		}
		break;
		}
	}	
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
}
