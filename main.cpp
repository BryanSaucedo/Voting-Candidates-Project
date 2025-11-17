/***************************************
Bryan Saucedo-Mondragon
Author: Round 1 Voting
Date Completed: 4/14/2024
Description: Create a program that makes use of a hash table.
***************************************/
#include <iostream>
#include "HashTable.hpp"
#include "LinkedList.hpp"
#include "pugixml.hpp"
#include <string>
#include <climits> // For using INT_MAX

using namespace std;

// Candidate structure
class Candidate {
private:
    string id;
    string name;
    int votes;

public:
    // Constructor to initialize Candidate
    Candidate(const string& id, const string& name) : id(id), name(name), votes(0) {}

    // Getter for ID
    string getId() const { return id; }

    // Getter for Name
    string getName() const { return name; }

    // Increment votes
    void addVote() { votes++; }

    // Getter for votes
    int getVotes() const { return votes; }
};

// Ballot structure
class Ballot {
private:
    string first;
    string second;
    string third;

public:
    // Constructor to initialize Ballot
    Ballot(const string& first, const string& second, const string& third)
        : first(first), second(second), third(third) {}

    // Member function to get the ID of the highest ranked candidate
    string getHighestRankedCandidate() const {
        if (!first.empty()) {
            return first;
        } else if (!second.empty()) {
            return second;
        } else if (!third.empty()) {
            return third;
        } else {
            return ""; // Return an empty string if all rankings are empty
        }
    }

    string getFirst() const {
        return first;
    }

    string getSecond() const {
        return second;
    }

    string getThird() const {
        return third;
    }
};

// Function to parse Candidates XML and populate hash table
void parseCandidatesXML(const string& filename, HashTable<Candidate>& candidatesTable) {
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str())) {
        cerr << "Error: Could not open " << filename << endl;
        return;
    }

    pugi::xml_node candidatesNode = doc.child("candidates");
    for (pugi::xml_node candidateNode : candidatesNode.children("candidate")) {
        string id = candidateNode.attribute("id").value();
        string name = candidateNode.child_value("name");
        candidatesTable.add(id, Candidate(id, name));
    }
}

void parseBallotsXML(const string& filename, LinkedList<Ballot>& ballotsList) {
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str())) {
        cerr << "Error: Could not open " << filename << endl;
        return;
    }

    pugi::xml_node ballotsNode = doc.child("ballots");
    for (pugi::xml_node ballotNode : ballotsNode.children("ballot")) {
        string first, second, third;

        // Iterate over the child nodes of the current ballot node
        int rank = 1;
        for (pugi::xml_node voteNode : ballotNode.children("vote")) {
            // Assign the value of each vote to the corresponding rank
            switch (rank) {
                case 1:
                    first = voteNode.child_value();
                    break;
                case 2:
                    second = voteNode.child_value();
                    break;
                case 3:
                    third = voteNode.child_value();
                    break;
                default:
                    break;
            }
            rank++;
        }

        // Create a Ballot object with the parsed vote values and add it to the list
        ballotsList.pushBack(Ballot(first, second, third));
    }
}
void tallyVotesAndPrintResults(HashTable<Candidate>& candidatesTable, LinkedList<Ballot>& ballotsList) {
    int totalBallots = ballotsList.size();
    int majorityThreshold = totalBallots / 2;
    int roundNumber = 1;
    const int maxRounds = 5; // Maximum number of rounds allowed

    // Initial hash table to keep track of all candidates
    HashTable<Candidate> currentRoundTable;

    // Populate currentRoundTable with candidates from candidatesTable
    HashTable<Candidate>::Iterator tableIter = candidatesTable.generateIterator();
    while (tableIter.hasNext()) {
        pair<string, Candidate>* entry = tableIter.current();
        currentRoundTable.add(entry->first, entry->second);
        tableIter.next();
    }

    while (roundNumber <= maxRounds) { // Ensure loop stops after reaching max rounds
        cout << "Round " << roundNumber << " Runoff Results:" << endl;

        // Reset votes for all candidates at the start of each round
        HashTable<Candidate>::Iterator resetIter = currentRoundTable.generateIterator();
        while (resetIter.hasNext()) {
            pair<string, Candidate>* entry = resetIter.current();
            entry->second = Candidate(entry->second.getId(), entry->second.getName()); // Reset candidate's votes
            resetIter.next();
        }

        // Tally votes based on current ballots
        for (const Ballot& ballot : ballotsList) {
            string candidateId = ballot.getHighestRankedCandidate();

            if (!candidateId.empty()) {
                Candidate* candidate = currentRoundTable.get(candidateId);
                if (candidate != nullptr) {
                    candidate->addVote(); // Increment the candidate's vote count
                }
            }
        }

        // Display runoff results for this round
        HashTable<Candidate>::Iterator iter = currentRoundTable.generateIterator();
        bool hasWinner = false;
        string winnerId;
        int maxVotes = 0;

        while (iter.hasNext()) {
            pair<string, Candidate>* entry = iter.current();
            const Candidate& candidate = entry->second;

            cout << candidate.getName() << ": " << candidate.getVotes() << " vote(s)" << endl;

            // Determine if a candidate has reached the majority threshold
            if (candidate.getVotes() > maxVotes) {
                maxVotes = candidate.getVotes();
                winnerId = entry->first;
            }

            iter.next(); // Move to the next candidate
        }

        if (maxVotes > majorityThreshold) {
            cout << "\nThe winner is " << currentRoundTable.get(winnerId)->getName() << " with " << maxVotes << " votes!" << endl;
            hasWinner = true;
            break; // Exit loop if a winner is found
        }

        // Determine the candidate to eliminate (candidate with the least votes)
        string candidateToEliminateId;
        int minVotes = INT_MAX;

        iter = currentRoundTable.generateIterator(); // Reset iterator for candidate elimination
        while (iter.hasNext()) {
            pair<string, Candidate>* entry = iter.current();
            const Candidate& candidate = entry->second;

            // Find the candidate with the minimum votes
            if (candidate.getVotes() < minVotes) {
                candidateToEliminateId = entry->first;
                minVotes = candidate.getVotes();
            }

            iter.next(); // Move to the next candidate
        }

        if (!candidateToEliminateId.empty()) {
            cout << "\nEliminating candidate with least votes: " << currentRoundTable.get(candidateToEliminateId)->getName() << endl;
            currentRoundTable.remove(candidateToEliminateId); // Remove the candidate with the least votes from currentRoundTable
        }

        // Increment round number
        roundNumber++;
    }


}




int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <Candidates XML filename> <Ballots XML filename>" << endl;
        return 1;
    }

    // Extract the filenames from command-line arguments
    string candidatesFilename = argv[1];
    string ballotsFilename = argv[2];

    // Create hash table for candidates
    HashTable<Candidate> candidatesTable;

    // Parse Candidates XML file
    parseCandidatesXML(candidatesFilename, candidatesTable);

    // Create linked list for ballots
    LinkedList<Ballot> ballotsList;

    // Parse Ballots XML file
    parseBallotsXML(ballotsFilename, ballotsList);

    // Tally votes and print results
    tallyVotesAndPrintResults(candidatesTable, ballotsList);

    return 0;
}