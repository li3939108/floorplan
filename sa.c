#include <math.h>
#include <string.h>
#include "annealing.h"			/* type declarations for SA */
#include "bool.h"

#include "fp.h"




void solution_count_update(FPTREE * solution, NET* net_arr, MODULE * module_arr)
{
	static solution_count ;
        solution_count = solution_count+1;
        if ((solution_count % PRINT_FREQUENCY) == 0){
                printf("%d %ld\n",solution_count,solution_cost(solution, net_arr, module_arr));
	}
}



void anneal(NET * net_arr, MODULE * module_arr ){
	int i1, i2;				/* pair of items to swap */
	int i,j;				/* counters */
	double temperature;			/* the current system temp */
	long current_value;			/* value of current state */
	long start_value;			/* value at start of loop */
	int delta;				/* value after swap */
	double merit, flip;			/* hold swap accept conditions*/
	double exponent;			/* exponent for energy funct*/

	FPTREE * tempnodes[3 * OPERATOR_LIMIT];
//	double random_float();
	//double solution_cost();
	//double transition(); 

	temperature = INITIAL_TEMPERATURE;
	current_value = solution_cost(solution, net_arr, module_arr);

	for (i=1; i<=COOLING_STEPS; i++) {
		temperature *= COOLING_FRACTION;
		start_value = current_value;
		for (j=1; j<=STEPS_PER_TEMP; j++) {
			current_value = solution_cost(solution, net_arr, module_arr);
			fprintf(stderr, "current value: %ld\n", current_value);
			int randint1 = random_int(0, 196);
			int type = (nodes[randint1]->operator == 0 ? 1 : 0);
			delta = transition( net_arr, module_arr, randint1 );
//			fprintf(stderr, "delta: %d\n", delta);
			flip = rand()/(RAND_MAX - 1.0);//random_float(0.0,1.0);
			exponent = (-delta)/(K * temperature);
			merit = pow(E,exponent);
			/*printf("merit = %f  flip=%f  exponent=%f\n",merit,flip,exponent); */
			/*if (merit >= 1.0)
			merit = 0.0;*/ /* don't do unchanging swaps*/

			if(delta < 0) {	/*ACCEPT-WIN choose a better solution*/
				//current_value = current_value + delta;
				if (TRACE_OUTPUT) {
					fprintf(stderr, "swap WIN %d value %ld  temp=%f \n",
					delta, solution_cost(solution,net_arr, module_arr),temperature);
					fprintf(stderr, "\n\n");
				}
			}else{if(merit >= flip){ 		/*ACCEPT-LOSS choose a worse solution*/
				//current_value = current_value+delta;
				if (TRACE_OUTPUT) {
					fprintf(stderr, "swap LOSS %d value %ld merit=%f flip=%f\n",
					delta,solution_cost(solution,net_arr, module_arr) , merit, flip) ;
					fprintf(stderr, "\n\n") ;
				}
			}else{ 				/* REJECT */
				if(type == 0){
					int i ;
					for(i = randint1; ; i--){
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
					for(i = randint1; ; i++){
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
					temp = nodes[randint1] ;
					nodes[randint1] = nodes[randint1 + 1];
					nodes[randint1 + 1] = temp ;
				}}
				solution = list2tree(nodes);
				iter_update_tree(solution);
				iter_update_module(0, 0, solution, module_arr);
			}}
			solution_count_update(solution, net_arr, module_arr) ;
		}
		if((current_value-start_value) > 0.0){ // rerun at this temp 
			temperature /= COOLING_FRACTION;
			if (TRACE_OUTPUT) {printf("rerun at temperature %f\n",temperature);}
		}
	}
}

/*

void repeated_annealing(tsp_instance *t, int nsamples, tsp_solution *bestsol)
{
        tsp_solution s;            
        double best_cost;          
        double cost_now;           
        int i;                     

        initialize_solution(t->n,&s);
        best_cost = solution_cost(&s,t);
        copy_solution(&s,bestsol);

        for (i=1; i<=nsamples; i++) {
                anneal(t,&s);
                cost_now = solution_cost(&s,t);
		printf("cost_now=%f\n-----------\n", cost_now);
                if (cost_now < best_cost) {
                        best_cost = cost_now;
                        copy_solution(&s,bestsol);
                }
        }
}*/
