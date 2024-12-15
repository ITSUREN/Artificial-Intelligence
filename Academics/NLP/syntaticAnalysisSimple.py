import nltk
#nltk.download('punkt')
from nltk import CFG

# Updated grammar to handle adjectives and copula verbs
grammar = CFG.fromstring("""
    S -> NP VP
    NP -> Det N | Det Adj
    VP -> V Adj | V NP
    Det -> 'the' | 'a'
    N -> 'dog' | 'cat'
    V -> 'is'
    Adj -> 'friendly'
""")

# Prompt the user for a sentence
print("Enter a sentence (e.g., 'the dog is friendly'):")
sentence = input("> ").lower()

# Tokenize the sentence using simple splitting
tokens = sentence.split()

# Create a parser
parser = nltk.ChartParser(grammar)

# Parse the sentence and draw the tree
try:
    for tree in parser.parse(tokens):
        print(tree)  # Print the tree structure
        tree.draw()  # Display the tree graphically
except ValueError:
    print("Error: Unable to parse the sentence. Please try a different one.")
