#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char ** argv){
	int i = 1;
	FILE	*moduleFile = NULL,
		*netFile = NULL,
		*pioFile = NULL;
	/*
	 * Read parameters from terminal
	 */
	while( i < argc){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
			int iter  ;
			case 'm'://must be defined before -w

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
	
}

/*
 * Read the module file
 */
int read_module_file(FILE * moduleFile){
	int state =  0 ;
	int number ;
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
		
		default:
		if( c >= '0' && c <= '9'){
			ungetc(c, moduleFile);
			fscanf(moduleFile, "%d", &number); 
		}
		switch(state){
			case module_index:
			break;

			case x_coordinate:
			break;

			case y_coordinate:
			break;

			case x_width:
			break;

			case y_width:
			break;
			default:
			break;
		}
		break;
		}
	}
}
