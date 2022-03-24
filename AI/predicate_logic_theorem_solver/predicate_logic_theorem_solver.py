from copy import deepcopy

class tree:
	def __init__(self , token , type , parent , children ,negated):
		self.token = token
		self.type = type
		self.parent = parent
		self.children = children
		self.negated = negated

	def equal(self , other , check_neg):
		if self.token != other.token or (self.negated != other.negated and check_neg):
			return False
		
		res = True

		for ind,expr in enumerate(self.children):
			res = res and expr.equal(other.children[ind] , check_neg)

		return res

	def occurs(self , query_token):
		if self.token == query_token:
			return True
		else:
			res = False

			for expr in self.children:
				res = res or expr.occurs(query_token)

				if(res):
					return True
			
			return res

	def apply_sub(self , sub):
		if self.token == sub[1].token:
			self.token = sub[0].token
			self.type = sub[0].type
			self.negated = sub[0].negated
			self.children = deepcopy(sub[0].children)

			return
		
		for expr in self.children:
			expr.apply_sub(sub)

	def make_string(self , first = True):
		if first and self.negated:
			res = "!"
		else:
			res = ""
			first = False
		if self.type == "func":
			res += self.token + "("

			for ind,expr in enumerate(self.children):
				if(ind == len(self.children) - 1):
					res += expr.make_string(False)
				else:
					res += expr.make_string(False) + ","
			res += ")"
		else:
			res = self.token

		return res

	def print_tree(self , depth = 0):
		print("  " * depth , end = "")
		print(self.type , end = " ")
		print(self.token)

		for i in self.children:
			i.print_tree(depth + 1)

class parser:
	def __init__(self , s = ""):
		if s[0] == "!":
			self.negated = True
			s = s[1:]
		else:
			self.negated = False
		
		self.root = tree("dummy" , "dummy" , None , [] , self.negated)

		i = 0
		while i < len(s):
			if "A" <= s[i] <= "Z":
				token = ""
				
				while i < len(s) and ("A" <= s[i] <= "Z" or "a" <= s[i] <= "z"):
					token += s[i]
					i += 1

				self.root.children.append(tree(token , "const" , self.root , [] , self.negated))

			elif "a" <= s[i] <= "z":
				token = ""
				
				while i < len(s) and ("A" <= s[i] <= "Z" or "a" <= s[i] <= "z"):
					token += s[i]
					i += 1

				self.root.children.append(tree(token , "var" , self.root , [] , self.negated))

			elif s[i] == "(":
				self.root = self.root.children[-1]
				self.root.type = "func"
				i += 1

			elif s[i] == ")":
				self.root = self.root.parent
				i += 1

			elif s[i] == ",":
				i += 1
		
		self.root = self.root.children[0]

	def print_expr(self):
		self.root.print_tree()

def unify(expr1 , expr2 , new_clause = [] , ignore_clause = False):
	#1
	if expr1.type != "func" or expr2.type != "func":
		if expr1.token == expr2.token:
			return [] #NIL
		
		elif expr1.type == "var":
			if expr2.occurs(expr1.token):
				return "failure"
			else:
				return [(deepcopy(expr2) , deepcopy(expr1))]
		
		elif expr2.type == "var":
			if expr1.occurs(expr2.token):
				return "failure"
			else:
				return [(deepcopy(expr1) , deepcopy(expr2))]
		else:
			return "failure"
	#2
	if expr1.token != expr2.token:
		return "failure"

	#3
	if len(expr1.children) != len(expr2.children):
		return "failure"

	#4
	subs = [] #NIL

	#5
	for i in range(len(expr1.children)):
		S = unify(expr1.children[i] , expr2.children[i] , new_clause)

		if S == "failure":
			return "failure"

		if S != []: #NIL
			for sub in S:
				expr1.apply_sub(sub)
				expr2.apply_sub(sub)
				
				if ignore_clause:
					continue

				j = 0
				while  j < len(new_clause):
					new_clause[j].apply_sub(sub)
					j += 1

			subs += S

	return subs

def make_step_string(clause1 , clause2 , clause3):
	str1 = "+".join([expr.make_string() for expr in clause1])
	str2 = "+".join([expr.make_string() for expr in clause2])

	if clause3 == []:
		return "$".join([str1 , str2 , "empty"])
	else:
		str3 = "+".join([expr.make_string() for expr in clause3])
		return "$".join([str1 , str2 , str3])

def subsumes(clause1 , clause2):
	if len(clause2) == 0:
		return False
	for expri in clause1:
		found = False
		for exprj in clause2:
			if expri.negated ^ exprj.negated:
				continue

			expr1 = deepcopy(expri)
			expr2 = deepcopy(exprj)

			res = unify(expr1 , expr2 , [] , True)

			if res != "failure":
				found = True
				break
		if not found:
			return False
	return True

def tautology(clause):
	if len(clause) <= 1:
		return False

	i = 0
	while i < len(clause):
		j = i + 1
		while j < len(clause):
			if not (clause[i].negated ^ clause[j].negated):
				if(clause[i].equal(clause[j] , True)):
					del clause[j]
				else:
					j += 1
				continue
			
			expr1 = deepcopy(clause[i])
			expr2 = deepcopy(clause[j])

			res = unify(expr1 , expr2 , [] , True)

			if res != "failure":
				return True
			j += 1
		i += 1
	return False

def theorem_prover(kb , refuted):
	kb += refuted
	clauses = []
	steps = []
	repeat = set()

	for clause in kb:
		expressions = clause.split("+")

		parsed_clause = []

		for expression in expressions:
			parsed_clause.append(parser(expression).root)

		clauses.append(parsed_clause)

	init_len = len(kb)
	i = 0

	while i < init_len:
		j = i + 1
		while j < init_len:
			k = 0
			while k < len(clauses[i]):
				l = 0
				while l < len(clauses[j]):
					if not (clauses[i][k].negated ^ clauses[j][l].negated):
						l += 1
						continue

					expr1 = deepcopy(clauses[i][k])
					expr2 = deepcopy(clauses[j][l])

					new_clause = deepcopy(clauses[i][:k] + clauses[i][k+1:] + clauses[j][:l] + clauses[j][l+1:])

					subs = unify(expr1 , expr2 , new_clause)

					if subs != "failure":
						proc_queue = [((i , j) , clauses[i] , clauses[j] , new_clause)]


						while len(proc_queue):
							res = proc_queue.pop()

							check = make_step_string(res[1] , res[2] , res[3])
							
							if check in repeat:
								continue
							else:
								repeat.add(check)

							if(tautology(res[3]) or subsumes(res[1] , res[3]) or subsumes(res[2] , res[3])):
								continue

							if len(res[3]) == 0:
								steps.append(make_step_string(res[1] , res[2] , []))

								return ("yes" , steps)
							else:
								steps.append(make_step_string(res[1] , res[2] , res[3]))

							for ind,clause in enumerate(clauses):
								if ind == res[0][0] or ind == res[0][1]:
									continue
								
								for ind1,express1 in enumerate(clause):
									for ind2,express2 in enumerate(res[3]):
										
										if not(express1.negated ^ express2.negated):
											continue
										
										expr_1 = deepcopy(express1)
										expr_2 = deepcopy(express2)
										new_clause_ = deepcopy(clauses[ind][:ind1] + clauses[ind][ind1+1:] + res[3][:ind2] + res[3][ind2 + 1 :])

										subs_ = unify(expr_1 , expr_2 , new_clause_)

										if subs_ != "failure":
											#proc_queue.append(((ind , -1) , clause , res[3] , new_clause_))
											proc_queue.append(((ind , -1) , res[3] , clause , new_clause_))
					l += 1

				k += 1

			j += 1

		i += 1
	
	return ("no" , steps)