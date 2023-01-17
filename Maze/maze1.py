import sys 

class Node(): 
    def __init__(self, state, parent, action):
        self.state=state
        self.parent=parent
        self.action=action

class StackFrontier():
    def __init__(self):
        self.frontier=[]

    def add(self,node):
        self.frontier.append(node)

    def contains_state(self,state):
        return any(node.state == state for node in self.frontier)
    
    def empty(self): 
        return len(self.frontier)==0
    
    def remove(self):
        #empty function defined above
        if self.empty():
            raise Exception("empty frontier")
        else:
            node = self.frontier[-1]
            #removes the last one
            self.frontier = self.frontier[:-1]
            return node

#inhereting from StackFrontier      
class QueueFrontier(StackFrontier):
    def remove(self):
        if self.empty():
            raise Exception("empty frontier")
        else:
            node = self.frontier[0]
            #remoes the first one
            self.frontier = self.frontier[1:]
            return node
        
class Maze():
    def __init__(self, filename):

        #red the file name and set height and wdth of the maze
        with open (filename) as f:
            contents = f.read()

        #validate start and goal, makes sure there are only one start and goal
        if contents.count("A") !=1:
            raise Exception("Maze must have exactly one starting point but multiple were given")
        if contents.count("B") !=1:
            raise Exception("Maze must have exactly one goal but multiple were given")

        #determine the height and width of maze
        #split lines splits the whole text into a elemnts of array with each line 
        contents =contents.splitlines()
        self.height = len(contents)
        self.width = max(len(line) for line in contents)

        #keep track of walls, translates maze into true and false
        self.walls=[]
        for i in range(self.height):
            row = []
            for j in range(self.width):
                try:
                    if contents[i][j] == "A":
                        self.start = (i,j)
                        row.append(False)
                    elif contents[i][j] == "B": 
                        self.goal = (i,j)
                        row.append(False)
                    elif contents[i][j] == " ":
                        row.append(False)
                    else:
                        row.append(True)
                except IndexError:
                    row.append(False)
            self.walls.append(row)

        self.solution = None
    
    def print(self):
        solution = self.solution[1] if self.solution is not None else None
        print()
        for i, row in enumerate(self.walls):
            for j, col in enumerate(row):
                if col: 
                    print("∎",end="")
                elif (i,j) == self.start:
                    print("A",end="")
                elif (i,j) == self.goal:
                    print("B",end="")
                elif solution is not None and (i,j)in solution:
                    print("*",end="")
                else:
                    print(" ",end="")
            print()
        print()
    
    def neighbors(self, state):
        row, col =state

        #All possible actions
        candidates= [
            ("up", (row - 1, col)),
            ("down", (row + 1, col)),
            ("left", (row, col - 1)),
            ("right", (row, col + 1))
        ]

        #ensure actions are valid 
        result = []
        for action, (r,c) in candidates:
            try:
                if not self.walls[r][c]:
                    result.append((action, (r,c)))
            except IndexError:
                continue 
        return result 

    def solve(self):
        """finds a solution to maze, if one exists"""

        #keep track of nuer of state explored
        self.num_explored = 0
        
        #initialize frontier to just the starting position
        start = Node(state=self.start, parent=None, action=None)
        frontier = QueueFrontier()
        frontier.add(start)

        #initaialize an empty expored set
        self.explored = set()

        #keep looping until solution found 
        while True:
            #if nothing left in frontier then no path
            if frontier.empty():
                raise Exception("no solution")
            
            #choose a node fronm the frontier
            node = frontier.remove()
            self.num_explored +=1

            #if node is the gold, then we have  asolution
            if node.state == self.goal: 
                actions = []
                cells = []

                #follow parent nodes to find solution
                while node.parent is not None:
                    actions.append(node.action)
                    cells.append(node.state)
                    node = node.parent
                actions.reverse()
                cells.reverse()
                self.solution = (actions, cells)
                return
            
            #mark node as explored
            self.explored.add(node.state)

            #add neighbors to frontier
            for action, state in self.neighbors(node.state):
                if not frontier.contains_state(state) and state not in self.explored:
                    child = Node(state=state, parent=node, action=action)
                    frontier.add(child)

        
m = Maze(sys.argv[1])      
print("Maze: ")
m.print()
print("solving....")
m.solve()
print("States Explored:",m.num_explored)
print("Solution:")
m.print()

            
            