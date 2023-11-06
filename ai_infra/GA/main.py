import time
import random  
import get_node_by_command as get_node

time_start = time.time()

input_file = '../benchmark/b05_comb/b05_comb.aig'
strings = ["balance", "rewrite", "rewrite -z", "rewrite -v", "refactor", "refactor -z", "refactor -v", "rewrite -l","refactor -l"]
algo_num = 10  #the numbers of the operator sequence
no_opt_delay,no_opt_area = get_node.get_initial_output(input_file)

initial_population = [] 
initial_area = []
initial_delay = []
for i in range(10):
    initial_node = get_node.get_random_sequence(strings,algo_num)
    initial_population.append(initial_node)
    delay,area=get_node.run_optimization(input_file,initial_node)
    initial_area.append(area)
    initial_delay.append(delay)

def reward_func(current_area , current_delay , no_opt_area , no_opt_delay):
    qor = 0.4*(current_area / no_opt_area) + 0.6*(current_delay / no_opt_delay)
    return qor

def fitness_func(current_area , current_delay , no_opt_area , no_opt_delay):
    fitness_value = 1/(reward_func(current_area , current_delay , no_opt_area , no_opt_delay))
    return fitness_value
qors = []
initial_fitness = []
for i in range(10):
    qor = reward_func(initial_area[i], initial_delay[i], no_opt_area, no_opt_delay)
    qors.append(qor)
    fitness = fitness_func(initial_area[i], initial_delay[i], no_opt_area, no_opt_delay)
    initial_fitness.append(fitness)

def find_top_half_population(fitness):
    index_fit = list(enumerate(fitness))
    sorted_fit = sorted(index_fit, key = lambda x:x[1], reverse=True)  
    top_half = sorted_fit[:(2*len(fitness)//3)]
    top_half_fit = [elem for _, elem in top_half]
    top_half_index = [idx for idx, _ in top_half]
    return top_half_fit, top_half_index

def compute_probability(fitness_value):  #compute fitness probability
    fit_p = []
    total = sum(fitness_value)
    for i in range(len(fitness_value)):
        p = fitness_value[i] / total
        fit_p.append(p)
    return fit_p

def select_best(population, fitness_value, initial_area, initial_delay):
    """
    select the best individual from pop
    """
    print("fitness:",fitness_value)
    max_fitness = max(fitness_value)
    max_index = fitness_value.index(max_fitness)
    best_selection = population[max_index]
    area = initial_area[max_index]
    delay = initial_delay[max_index]
    return best_selection, area, delay

def ga_selection(population, fitness_value):
    #轮盘赌选择
    fit_p = compute_probability(fitness_value)
    random_num = random.uniform(0.0,1.0)
    cum_p = 0
    for j in range(len(fit_p)):
        cum_p = cum_p + fit_p[j]
        if random_num <= cum_p:
            select_population = population[j]
            select_area = initial_area[j]
            select_delay = initial_delay[j]
            select_fit = initial_fitness[j]
            break
    return select_population,select_area,select_delay,select_fit

def crossoperate(cross_pop):
    cross_x = cross_pop[0][:-1]  #delete the last operator "map_fpga"
    cross_y = cross_pop[1][:-1]
    dim = len(cross_x)
    if dim == 0: 
        return
    elif dim == 1:
        pos1 = 1
        pos2 = 1
    else:
        pos1 = random.randrange(1, dim)  #select a position in the range 0 to dim-1
        pos2 = random.randrange(1, dim)  #two crossing point

    after_cross_x = []
    after_cross_y = []
    for i in range(dim):
        if min(pos1,pos2) <= i < max(pos1,pos2):
            after_cross_x.append(cross_y[i])
            after_cross_y.append(cross_x[i])
        else:
            after_cross_x.append(cross_x[i])
            after_cross_y.append(cross_y[i])

    after_cross_x.append("map_fpga")
    after_cross_y.append("map_fpga")

    return after_cross_x, after_cross_y

def mutation(sequence):
    dim = len(sequence) - 1
    if dim == 0:return
    elif dim == 1: pos = 0
    else:
        pos = random.randrange(0,dim)  #chose a position in sequence to perform mutation

    operator = ["balance", "rewrite", "rewrite -z", "rewrite -v", "refactor", "refactor -z", "refactor -v", "rewrite -l","refactor -l"]
    sequence[pos] = operator[random.randrange(len(operator))]  #change the operator on the position
    return sequence

cross_probability = 0.6
mutation_probability = 0.3
count = 0
while(1):

    next_population = []
    next_fitness = []
    next_area = []
    next_delay = []
    parents = []
    better_fit_of_pop, index_of_better_pop = find_top_half_population(initial_fitness)
    
    for i in range(len(index_of_better_pop)):
        next_population.append(initial_population[index_of_better_pop[i]])
        next_fitness.append(initial_fitness[index_of_better_pop[i]])
        next_area.append(initial_area[index_of_better_pop[i]])
        next_delay.append(initial_delay[index_of_better_pop[i]])

    best_sequence, area, delay = select_best(initial_population,initial_fitness,initial_area,initial_delay)
    print("best_sequence:",best_sequence)
    print("area:",area)
    print("delay:",delay)
    
    for j in range((len(initial_population)-len(next_population))//2):

        father,father_area,father_delay,father_fitness = ga_selection(next_population,next_fitness)
        father_list = [father_fitness,father_area,father_delay,father]

        mother,mother_area,mother_delay,mother_fitness = ga_selection(next_population,next_fitness)
        mother_list = [mother_fitness,mother_area,mother_delay,mother]

        parents.append(father)  
        parents.append(mother)

        if random.random() <= cross_probability:   
            child1, child2 = crossoperate(parents)
        else:
            child1 = father
            child2 = mother
        
        if random.random() <= mutation_probability:
            child1 = mutation(child1)
            child2 = mutation(child2)
        else:   
            child1 = father
            child2 = mother

        child1_delay,child1_area=get_node.run_optimization(input_file,child1)
        child1_fitness = fitness_func(child1_area, child1_delay, no_opt_area, no_opt_delay)
        child1_list = [child1_fitness,child1_area,child1_delay,child1]

        child2_delay,child2_area=get_node.run_optimization(input_file,child2)
        child2_fitness = fitness_func(child2_area, child2_delay, no_opt_area, no_opt_delay)
        child2_list = [child1_fitness,child2_area,child2_delay,child2]

        total_list = [father_list,mother_list,child1_list,child2_list]
        fitness_list = sorted(total_list, key=lambda x: x[0])

        next_area.append(fitness_list[0][1])
        next_area.append(fitness_list[1][1])
        next_delay.append(fitness_list[0][2])
        next_delay.append(fitness_list[1][2])
        next_fitness.append(fitness_list[0][0])
        next_fitness.append(fitness_list[1][0])
        next_population.append(fitness_list[0][3])
        next_population.append(fitness_list[1][3])

    initial_population = next_population
    initial_fitness = next_fitness
    initial_area = next_area
    initial_delay = next_delay
    count += 1
    print(count)
    time_end = time.time()
    time_interval = time_end - time_start
    print("time_interval:",time_interval)
    # if time_interval >= 60.0:
    #     break