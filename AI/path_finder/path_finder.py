import sys
import random

def one_el(lst):
    if len(lst) == 1:
        return lst[0]
    else:
        return lst

def converged(dict1 , dict2 , epsilon):
    for i in dict1:
        if abs(dict1[i] - dict2[i]) >= epsilon:
            return False

    return True

file = open(sys.argv[1] , "r")

typ = file.readline().strip()

lst = list(map(lambda  x : one_el(x.strip().split()) , file.readlines()))

file.close()

if typ == "Q":
    episode_count = int(lst[0])

    learning_rate = float(lst[1])

    discount_factor = float(lst[2])

    epsilon = float(lst[3])

    y , x = tuple(map(int , lst[4]))

    i = 6 + int(lst[5])

    obstacles = set(map(lambda x : tuple(map(int , x)) , lst[6 : i]))

    j = i + 1 + int(lst[i])

    pitfalls = set(map(lambda x : tuple(map(int , x)) , lst[i + 1 : j]))

    goal = tuple(map(int , lst[j]))

    step_r , hit_r , pitfall_r , goal_r = tuple(map(float , lst[j + 1]))

    q_table = {}
    states = set()
    starting_states = []


    for i in range(1 , x + 1):
        for j in range(1 , y + 1):
            is_starting = True
            is_bendable = False

            if (-i , -j) in pitfalls:
                q_table[(-i , -j)] = {"N":0 , "S":0 , "E":0 , "W":0 , "B":0}

                states.add((-i , -j))

                is_bendable = True

            if (-i , -j) == goal:
                q_table[(-i , -j)] = {"N":0 , "S":0 , "E":0 , "W":0 , "B":0}

                states.add((-i , -j))

                is_starting = False

                is_bendable = True

            if (i , j) not in obstacles:
                if is_bendable:
                    q_table[(i , j)] = {"N":0 , "S":0 , "E":0 , "W":0 , "B":0}
                else:
                    q_table[(i , j)] = {"N":0 , "S":0 , "E":0 , "W":0}

                states.add((i , j))

                if is_starting:
                    starting_states.append((i , j))


    for i in range(episode_count):
        s = random.choice(starting_states)

        while s != goal:
            if random.uniform(0.0 , 1.0) < epsilon:
                action = random.choice(list(q_table[s].keys()))
            else:
                action = max(q_table[s] , key = lambda key : q_table[s][key])

            if action == "B":
                S_ = (-1 * abs(s[0]) , -1 * abs(s[1]))

                if S_ == goal:
                    R = goal_r
                else:
                    R = pitfall_r
            else:
                if action == "N":

                    S_ = (abs(s[0]) , abs(s[1]) + 1)

                elif action == "S":

                    S_ = (abs(s[0]) , abs(s[1]) - 1)

                elif action == "E":

                    S_ = (abs(s[0]) + 1 , abs(s[1]))

                else: # action == "W"

                    S_ = (abs(s[0]) - 1 , abs(s[1]))

                if 1 <= S_[0] <= x and 1 <= S_[1] <= y and S_ not in obstacles:
                    R = step_r
                else:
                    R = hit_r
                    S_ = (abs(s[0]) , abs(s[1]))

            q_table[s][action] = q_table[s][action] + learning_rate * (R + discount_factor * max(list(q_table[S_].values())) - q_table[s][action])

            s = S_

    action_code = {"N":0 , "E":1 , "S":2 , "W":3 , "B":4}

    f = open(sys.argv[2], 'w')

    for i in range(1 , x + 1):
        for j in range(1 , y + 1):
            
            if (i , j) in obstacles:
                    continue

            print(str(i) + " " + str(j) , end = " " , file = f)
            print(action_code[max(q_table[(i , j)] , key = lambda key : q_table[(i , j)][key])] , file = f)

            if (-i , -j) in pitfalls:
                print(str(-i) + " " + str(-j) , end = " " , file = f)
                print(action_code[max(q_table[(-i , -j)] , key = lambda key : q_table[(-i , -j)][key])] , file = f)

    f.close()

elif typ == "V":
    termination_factor = float(lst[0])

    discount_factor = float(lst[1])

    y , x = tuple(map(int , lst[2]))

    i = 4 + int(lst[3])

    obstacles = set(map(lambda x : tuple(map(int , x)) , lst[4 : i]))

    j = i + 1 + int(lst[i])

    pitfalls = set(map(lambda x : tuple(map(int , x)) , lst[i + 1 : j]))

    goal = tuple(map(int , lst[j]))

    step_r , hit_r , pitfall_r , goal_r = tuple(map(float , lst[j + 1]))

    states_actions = {}
    states_values = {}
    states_values_prev = {}

    for i in range(1 , x + 1):
        for j in range(1 , y + 1):

            if (-i , -j) in pitfalls:
                states_values[(-i , -j)] = 0
                states_values_prev[(-i , -j)] = float("inf")

            if (-i , -j) == goal:
                states_values[(-i , -j)] = 0
                states_values_prev[(-i , -j)] = float("inf")
                #pass

            if (i , j) not in obstacles:
                states_values[(i , j)] = 0
                states_values_prev[(i , j)] = float("inf")

    actions = {"N" , "S" , "E" , "W" , "B"}

    while not converged(states_values , states_values_prev , termination_factor):
        for s in states_values:
            states_values_prev[s] = states_values[s]

        for s in states_values:
            
            negative = (-1 * abs(s[0]) , -1 * abs(s[1]))

            if negative not in pitfalls and negative != goal:
                q = {"N":0 , "S":0 , "E":0 , "W":0}

            else:
                q = {"N":0 , "S":0 , "E":0 , "W":0 , "B":0}

            for action in q:

                if action == "B":

                    S_ = (-1 * abs(s[0]) , -1 * abs(s[1]))

                    if S_ == goal:
                        R = goal_r
                    else:
                        R = pitfall_r
                else:
                    if action == "N":

                        S_ = (abs(s[0]) , abs(s[1]) + 1)

                    elif action == "S":

                        S_ = (abs(s[0]) , abs(s[1]) - 1)

                    elif action == "E":

                        S_ = (abs(s[0]) + 1 , abs(s[1]))

                    else: # action == "W"

                        S_ = (abs(s[0]) - 1 , abs(s[1]))

                    if 1 <= S_[0] <= x and 1 <= S_[1] <= y and S_ not in obstacles:
                        R = step_r
                    else:
                        R = hit_r
                        S_ = (abs(s[0]) , abs(s[1]))
                
                q[action] = R + discount_factor * states_values_prev[S_]

            best_act = max(q , key = q.get)

            states_actions[s] = best_act
            states_values[s] = q[best_act]

    action_code = {"N":0 , "E":1 , "S":2 , "W":3 , "B":4}

    f = open(sys.argv[2], 'w')

    for i in range(1 , x + 1):
        for j in range(1 , y + 1):
            
            if (i , j) in obstacles:
                    continue

            if (-i , -j) in pitfalls:
                print(str(-i) + " " + str(-j) + " " + str(action_code[states_actions[(-i , -j)]]), file = f)

            print(str(i) + " " + str(j) + " " + str(action_code[states_actions[(i , j)]]), file = f)

    f.close()
