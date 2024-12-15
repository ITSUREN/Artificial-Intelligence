import nltk
import os
from nltk import CFG

# Load the dictionary file and construct grammar rules dynamically
def load_dictionary(file_name):
    word_dict = {}
    # Get the current directory and create the full path to the dictionary file
    current_directory = os.getcwd()
    file_path = os.path.join(current_directory, file_name)
    print(file_path);

    try:
        with open(file_path, 'r') as file:
            for line in file:
                parts = line.strip().split()
                pos = parts[0]  # The first part is the POS tag
                words = parts[1:]  # The rest are words for that POS tag
                word_dict[pos] = words
    except FileNotFoundError:
        print(f"Error: The file {file_name} was not found in the current directory.")
        exit()

    return word_dict

def construct_grammar(word_dict):
    grammar_rules = [
        "S -> NP VP",
        "NP -> N | Det N | Det N PP | Det Adj N | Det Adj N | Det Adj",
        "VP -> V NP | V PP NP | V Adj",
        "PP -> P| P NP",
    ]

    # Add grammar rules for each POS category
    for pos, words in word_dict.items():
        rule = f"{pos} -> " + " | ".join(f"'{word}'" for word in words)
        grammar_rules.append(rule)

    return "\n".join(grammar_rules)

# Load the dictionary file (assumes dictionary.txt is in the current directory)
dictionary_file = "dictionary.txt"
word_dict = load_dictionary(dictionary_file)

# Dynamically construct the grammar
grammar_text = construct_grammar(word_dict)
grammar = CFG.fromstring(grammar_text)

# Prompt the user for a sentence
print("Enter a sentence (e.g., 'the cat saw the dog with a telescope'):")
sentence = input("> ").lower()

# Tokenize the sentence
tokens = nltk.word_tokenize(sentence)

# Create a parser
parser = nltk.ChartParser(grammar)

# Parse the sentence and draw the tree
try:
    for tree in parser.parse(tokens):
        print(tree)  # Print the tree structure
        tree.draw()  # Display the tree graphically
        break  # Only display the first valid tree
except ValueError:
    print("Error: Unable to parse the sentence. Please try a different one.")
