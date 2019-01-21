import queue

graph = {}
graph["A"] = ("Z", "S", "T")
graph["Z"] = ("A", "O")
graph["T"] = ("A", "L")
graph["O"] = ("Z", "S")
graph["S"] = ("A", "O", "R", "F")
graph["L"] = ("T", "M")
graph["M"] = ("L", "D")
graph["D"] = ("M", "C")
graph["C"] = ("D", "R", "P")
graph["P"] = ("C", "R", "B")
graph["R"] = ("S", "P", "C")
graph["F"] = ("S", "B")
graph["B"] = ("P", "F", "G", "U")
graph["G"] = ("B")
graph["U"] = ("B", "H", "V")
graph["H"] = ("U", "E")
graph["E"] = ("H")
graph["V"] = ("U", "I")
graph["I"] = ("V", "N")
graph["N"] = ("I")

def bfs(graph, start, dest):
  frontier = queue.Queue()
  print ("Queue status: + is enqueue and - is dequeue")
  frontier.put(start + " BFS-Path")
  print (" + " + start + " [" + str(frontier.qsize()) + " elements in queue]")
  explored = set()
  while not frontier.empty():
    node = frontier.get()
    nodestate, path = node.split()
    print ("- " + nodestate + " [" + str(frontier.qsize()) + " elements in queue]")
    explored.add(nodestate)
    for neighbor in graph[nodestate]:
      if neighbor not in explored:
        if neighbor == dest:
          return path + "->" + nodestate + "->" + neighbor
        frontier.put(neighbor + " " + path + "->" + nodestate)
        print (" + " + str(neighbor) + " [" + str(frontier.qsize()) + "elements in queue]")
  return False

print ("")
print ("BFS path from Arad to Bucharest:")
print ( bfs(graph, "A", "B"))

print ("")
print ("BFS Path from Faragas to Drobeta:")
print ( bfs(graph, "F", "D"))

#to change above code to UniformCostSearch... implement a priority queue.
#to change code to DepthFirstSearch... use FIFO queue
#to change code to A*Search... implement heuristic function H(n) instead of G(n)