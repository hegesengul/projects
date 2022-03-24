from copy import deepcopy
#from line_profiler import LineProfiler

search_algorithm = input()

threshold    = int(input())
num_of_disks = int(input())
goal_rod     = input()

rod_a = input().split(",")
rod_b = input().split(",")
rod_c = input().split(",")

if(rod_a[0] != ""):
    rod_a = [int(i) for i in rod_a]
else:
	rod_a = []

if(rod_b[0] != ""):
    rod_b = [int(i) for i in rod_b]
else:
	rod_b = []

if(rod_c[0] != ""):
    rod_c = [int(i) for i in rod_c]
else:
	rod_c = []

action_dict = {}
path_dict = {}
pos_action_dict = {}

class Node:
	"""docstring for Node"""
	rods = {}
	accum_cost = 0
	prev_action = 0
	heuristic = 0
	prev_state = 0

	def __init__(self, rods = {} , accum_cost = 0 , prev_state = None , prev_action = -1 , heuristic = 0):
		self.rods = deepcopy(rods)

		self.accum_cost = accum_cost

		self.prev_state = prev_state

		self.heuristic = heuristic

		self.prev_action = prev_action

	def __eq__(self , other):
		if other == None :
			return False
		return self.rods == other.rods
	def __lt__(self,other): 
		left = self.accum_cost + self.heuristic
		right = other.accum_cost + other.heuristic

		if left < right or (left == right and self.prev_action < other.prev_action):
			return True
		else:
			return False
	
	def maxOfRod(self , rod):
		if len(self.rods[rod]) == 0:
			return 0
		else:
			return self.rods[rod][0]

	def setHeuristic(self):
		x = 0
		y = 0		

		max_other = 0

		for i in self.rods:
			if i != goal_rod :
				x += len(self.rods[i])
				max_other = max(max_other , self.maxOfRod(i))

		for i in reversed(range(len(self.rods[goal_rod]))):
			if self.rods[goal_rod][i] < max_other:
				y = i + 1
				break

		self.heuristic = x + 2 * y

	def x_to_y(self , x , y):
		return len(self.rods[x]) and (len(self.rods[y]) == 0 or self.rods[x][-1] < self.rods[y][-1])

	def getActions(self):
		k = (tuple(self.rods["A"]) , tuple(self.rods["B"]) , tuple(self.rods["C"]))
		if k in pos_action_dict:
			return list(pos_action_dict[k])
		res = []

		if(self.x_to_y("A" , "B")):
			res.append(1)
		if(self.x_to_y("A" , "C")):
			res.append(2)
		if(self.x_to_y("B" , "A")):
			res.append(3)
		if(self.x_to_y("B" , "C")):
			res.append(4)
		if(self.x_to_y("C" , "A")):
			res.append(5)
		if(self.x_to_y("C" , "B")):
			res.append(6)

		pos_action_dict[k] = tuple(res)
		
		return res

	def do_action(self , action_num):
		k = (tuple(self.rods["A"]) , tuple(self.rods["B"]) , tuple(self.rods["C"]) , action_num)
		if k in action_dict:
			return action_dict[k]
		res = Node(self.rods , self.accum_cost + 1 , self , action_num)

		if action_num == 1 :
			res.rods["B"].append(res.rods["A"][-1])
			res.rods["A"] = res.rods["A"][:-1]
		elif action_num == 2:
			res.rods["C"].append(res.rods["A"][-1])
			res.rods["A"] = res.rods["A"][:-1]
		elif action_num == 3:
			res.rods["A"].append(res.rods["B"][-1])
			res.rods["B"] = res.rods["B"][:-1]
		elif action_num == 4:
			res.rods["C"].append(res.rods["B"][-1])
			res.rods["B"] = res.rods["B"][:-1]
		elif action_num == 5:
			res.rods["A"].append(res.rods["C"][-1])
			res.rods["C"] = res.rods["C"][:-1]
		elif action_num == 6:
			res.rods["B"].append(res.rods["C"][-1])
			res.rods["C"] = res.rods["C"][:-1]


		res.setHeuristic()
		action_dict[k] = res

		return res

class MinHeap:
	"""docstring for MinHeap"""
	arr = []

	def __init__(self, arr = []):
		self.arr = arr

	def empty(self):
		return len(self.arr) == 0

	def getParent(self , i):
		if i == 0:
			assert "The root node has no parent!!!"
		else:
			return (i - 1) // 2
	
	def getLeftChild(self , i):
		left = 2 * i + 1

		if left >= len(self.arr):
			return -1
		else:
			return left

	def getRightChild(self , i):
		right = 2 * i + 2

		if right >= len(self.arr):
			return -1
		else:
			return right

	def push(self , newNode):
#		if newNode.accum_cost > threshold:
#			return
		self.arr.append(newNode)

		newNodeIndex = len(self.arr) - 1

		while newNodeIndex :
			newNodeParentIndex = self.getParent(newNodeIndex)

			down_cost = self.arr[newNodeIndex].accum_cost + self.arr[newNodeIndex].heuristic
			up_cost = self.arr[newNodeParentIndex].accum_cost + self.arr[newNodeParentIndex].heuristic
			
			if self.arr[newNodeIndex] < self.arr[newNodeParentIndex] :
				temp = self.arr[newNodeParentIndex]
				self.arr[newNodeParentIndex] = deepcopy(self.arr[newNodeIndex])
				self.arr[newNodeIndex] = deepcopy(temp)

				newNodeIndex = newNodeParentIndex
			else:
				break

	def pop_top(self):
		if len(self.arr) == 0 :
			return Node()
		elif len(self.arr) == 1 :
			res = self.arr[0]
			self.arr = []
			return res
		else:
			res = self.arr[0]
			self.arr[0] = self.arr[-1]
			self.arr = self.arr[:-1]

			flush_down = 0
			left = self.getLeftChild(flush_down)
			right = self.getRightChild(flush_down)

			while left != -1 :
				if right != -1 :
					if self.arr[left] < self.arr[right] :
						if self.arr[left] < self.arr[flush_down] :
							#swap with left
							temp = self.arr[left]
							self.arr[left] = deepcopy(self.arr[flush_down])
							self.arr[flush_down] = deepcopy(temp)

							flush_down = left
							left = self.getLeftChild(flush_down)
							right = self.getRightChild(flush_down)
						else:
							break
					else :
						if self.arr[right] < self.arr[flush_down] :
							#swap with right
							temp = self.arr[right]
							self.arr[right] = deepcopy(self.arr[flush_down])
							self.arr[flush_down] = deepcopy(temp)

							flush_down = right
							left = self.getLeftChild(flush_down)
							right = self.getRightChild(flush_down)
						else:
							break
				else :
					if self.arr[left] < self.arr[flush_down] :
						#swap with left
						temp = self.arr[left]
						self.arr[left] = deepcopy(self.arr[flush_down])
						self.arr[flush_down] = deepcopy(temp)

						flush_down = left
						left = self.getLeftChild(flush_down)
						right = self.getRightChild(flush_down)
					else:
						break
			return res

def ida_star(node):
	global path_dict
	bound = node.heuristic
	path = [deepcopy(node)]


	while True :
		path_dict = {(tuple(rod_a) , tuple(rod_b) , tuple(rod_c)) :1}

		t = search(path , 0 , bound)
		
		if t == "FOUND" :
			if path[-1].accum_cost > threshold :
				print("FAILURE")
				return False
			else :
				print("SUCCESS")
				for i in path:
					print("\nA->{}\tB->{}\tC->{}\n".format(i.rods["A"] , i.rods["B"] , i.rods["C"]))
				return True
		elif t == float('inf') :
			print("FAILURE")
			return False

		bound = t


def search(path , g , bound):
	node = path[-1]

	f = g + node.heuristic

	if f > bound:
		return f

	if len(node.rods[goal_rod]) == num_of_disks:
		return "FOUND"

	_min = float('inf')

	successors = node.getActions()
		
	for i in range(len(successors)):	
		successors[i] = deepcopy(node.do_action(successors[i]))
		successors[i].accum_cost = node.accum_cost + 1
	
	successors.sort()
	
	for succ in successors:
		k = (tuple(succ.rods["A"]) , tuple(succ.rods["B"]) , tuple(succ.rods["C"]))
		if k not in path_dict:
			path.append(succ)
			path_dict[k] = 1
			t = search(path , g + 1 , bound)

			if t == "FOUND" :
				return "FOUND"

			elif t < _min:
				_min = t
			
			path.pop()
			del path_dict[k]

	return _min

if search_algorithm == "A*" :
	PQ = MinHeap()
	PQ.push(Node({"A":rod_a , "B" : rod_b , "C" : rod_c}))
	PQ.arr[0].setHeuristic()

	done_states = {(tuple(PQ.arr[0].rods["A"]) , tuple(PQ.arr[0].rods["B"]) , tuple(PQ.arr[0].rods["C"]) )  : 1}

	is_solved = False

	while not PQ.empty():
		state = PQ.pop_top()
		if state.accum_cost > threshold:
			break
		if len(state.rods[goal_rod]) == num_of_disks :
			#print the solution
			is_solved = True
			print("SUCCESS")
			sol = []
			while state != None:
				sol.append("\nA->{}\tB->{}\tC->{}\n".format(state.rods["A"] , state.rods["B"] , state.rods["C"]))
				state = state.prev_state

			for i in reversed(sol) :
				print(i)
			break
		else:
			pos_actions = state.getActions()

			for i in pos_actions:
				pos_state = state.do_action(i)

				hist_check = (tuple(pos_state.rods["A"]) , tuple(pos_state.rods["B"]) , tuple(pos_state.rods["C"]) )

				if hist_check not in done_states:
					PQ.push(pos_state)
					done_states[hist_check] = 1

	if not is_solved:
		print("FAILURE")

elif search_algorithm == "IDA*":
	
	start = Node({"A":rod_a , "B" : rod_b , "C" : rod_c})
	start.setHeuristic()
	
	ida_star(start)