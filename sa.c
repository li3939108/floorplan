#include <math.h>
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



void anneal(FPTREE * solution, NET * net_arr, MODULE * module_arr ){
	int i1, i2;				/* pair of items to swap */
	int i,j;				/* counters */
	double temperature;			/* the current system temp */
	long current_value;			/* value of current state */
	long start_value;			/* value at start of loop */
	int delta;				/* value after swap */
	double merit, flip;			/* hold swap accept conditions*/
	double exponent;			/* exponent for energy funct*/
	double random_float();
	//double solution_cost();
	//double transition(); 

	temperature = INITIAL_TEMPERATURE;

//	initialize_solution(t->n,s);
	current_value = solution_cost(solution, net_arr, module_arr);

	for (i=1; i<=COOLING_STEPS; i++) {
		temperature *= COOLING_FRACTION;

		start_value = current_value;
		
		for (j=1; j<=STEPS_PER_TEMP; j++) {
			current_value = solution_cost(solution, net_arr, module_arr);
			int randint1 = random_int(1, 98);
//			int randint2 = random_int(1,99);
//			int tst = random_int(1,2);
			delta = transition(solution,  net_arr, module_arr, randint1 );
			if(solution->width > 100 || solution->height > 100){
				transition(solution,  net_arr, module_arr, randint1);
				continue ;
			}

			flip = random_float(0,1);
			exponent = (-delta/(current_value + 0.0))/(K * temperature);
			merit = pow(E,exponent);
			/*printf("merit = %f  flip=%f  exponent=%f\n",merit,flip,exponent); */
			/*if (merit >= 1.0)
			merit = 0.0;*/ /* don't do unchanging swaps*/

			if(delta < 0) {			/*ACCEPT-WIN choose a better solution*/
				current_value = current_value + delta;
				if (TRACE_OUTPUT) {
					fprintf(stderr, "swap WIN %d value %ld  temp=%f \n",
					randint1, current_value,temperature);
					solution_cost(solution,net_arr, module_arr);
					printf("\n\n");
				}
			}else{if(merit > flip){ 		/*ACCEPT-LOSS choose a worse solution*/
				current_value = current_value+delta;
				if (TRACE_OUTPUT) {
					fprintf(stderr, "swap LOSS %d value %ld merit=%f flip=%f\n",
					randint1, current_value, merit, flip) ;
					solution_cost(solution,net_arr, module_arr) ;
					printf("\n\n") ;
				}
			}else{ 				/* REJECT */
			      transition(solution,  net_arr, module_arr, randint1 ) ;
			}}
			solution_count_update(solution, net_arr, module_arr) ;
		}
		if((current_value-start_value) < 0.0){ /* rerun at this temp */
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
