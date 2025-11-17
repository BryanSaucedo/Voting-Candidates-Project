Round 1 Voting – Ranked-Choice Election Simulator

Hi, I’m Bryan Saucedo-Mondragon, a Berkeley student and aspiring electrical engineer and software developer. This project, Round 1 Voting, is a C++ program designed to simulate ranked-choice elections using custom data structures, object-oriented programming principles, and XML input parsing. The purpose of this project was to build a practical, structured program that can process ballots and candidates, calculate votes across multiple runoff rounds, and determine the winner based on majority thresholds. Through this project, I aimed to deepen my understanding of advanced C++ concepts, modular software design, and the use of both built-in and custom data structures for efficient data processing.

The program uses custom HashTable and LinkedList implementations to manage candidates and ballots. The hash table allows for fast insertion, lookup, and deletion of candidates during each round of voting, while the linked list stores ballots in a sequential structure that can be easily iterated through for vote tallying. By implementing these data structures myself, I gained hands-on experience with memory management, iterators, and templated classes, which strengthened my understanding of C++ at a low-level systems programming level.

Input data for the simulation is provided via XML files. Candidates and ballots are parsed using the lightweight pugixml library, which allows the program to read structured XML input efficiently. Each candidate has a unique ID and name, and each ballot contains rankings for up to three candidates. The program processes ballots in rounds, where the highest-ranked candidate on each ballot receives a vote. If no candidate reaches a majority, the candidate with the fewest votes is eliminated, and the next highest-ranked candidate on affected ballots is counted in the subsequent round. This iterative runoff continues until a candidate reaches the majority threshold or a maximum number of rounds is reached.

The project demonstrates modular object-oriented design. Candidate and Ballot classes encapsulate properties and behaviors, separating data representation from vote tallying and I/O parsing. Core functions are clearly divided: XML parsing, vote tallying, candidate elimination, and result display are handled independently to improve maintainability and readability. Additionally, robust error handling is implemented for missing or malformed files, empty ballots, and invalid data, ensuring the program can execute reliably under a variety of input conditions.

Through building Round 1 Voting, I strengthened my skills in C++ programming, data structures, algorithmic design, modular software architecture, and XML parsing. It is a practical example of designing and implementing a complex system from scratch, integrating both standard libraries and custom implementations to solve a real-world problem.

Usage:

./Round1Voting <Candidates XML filename> <Ballots XML filename>


The program reads the XML files, simulates the election rounds, and prints the results of each round to the console, showing vote totals and candidate eliminations.
