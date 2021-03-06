// Voting System.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <stdlib.h> 
#include <string>
#include <list>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
using namespace std;


list<string> readList(string filepath) {
	string line;
	list<string> LineList = {};
	ifstream File("Votes.txt");
	if (File.is_open())
	{
		while (getline(File, line))
		{
			LineList.push_back(line);
		}
		File.close();
	}
	return LineList;
}

class Candidate
{
public:
	string Candidatename;
	string Candidateparty;

	Candidate()
	{

	}
	~Candidate() {}


	void setCandidatename(string name)
	{
		// Read in input until there is a space all names at top
		Candidatename = name;
	}

	string getCandidatename()
	{
		return Candidatename;
	}

	void setCandidateparty(string party)
	{
		Candidateparty = party;
	}

	void getCandidateparty()
	{

	}


};

class BallotPaper
{
public:
	BallotPaper()
	{

	}
	~BallotPaper() {}

	map<int, Candidate> Canmap;

	void setPreference(int pref, Candidate c)
	{
		Canmap[pref] = c;

	}
	Candidate getPreference(int pref)
	{
		return Canmap[pref];
	}
};
struct RunningCan
{
	Candidate can;
	vector<BallotPaper> Votesfor;

};

int main()
{
	// Reads from file Creates a list called user inputs 
	list<string> Userinputs = readList("Votes.txt");
	// A vector (Arraylist) of type candidate 
	vector<Candidate> Canadidates;
	vector<BallotPaper> Ballotpapers;
	int Cannum = 1;
	// Creates a pointer of type candidate this holds a candidate object as we build it by seting name and party 
	Candidate *Cantemp = nullptr;
	BallotPaper *Baltemp = new BallotPaper();

	// Is it in the candidate or the voter section of the file 
	bool isCandid = true;
	// For each line in user inputs (Auto defines the type for u as it is obvious this is a string)
	for (auto line : Userinputs)
	{
		if (isCandid)
		{
			// If this skip to voter section or candidate section is done
			if (line == "")
			{
				isCandid = false;
			}
			// If the candidate container is empty/ null create a cadidate this will read line 1 3 
			else if (Cantemp == nullptr)
			{
				// Create a new Candidate and put it in Cantemp
				Cantemp = new Candidate();
				// Set Candidate name to whatever is in line which is read from the text file
				Cantemp->setCandidatename(line);
			}
			else
			{
				Cantemp->setCandidateparty(line);
				Canadidates.push_back(*Cantemp);
				Cantemp = nullptr;
			}
		}
		else
		{
			if (line == (","))
			{
				Ballotpapers.push_back(*Baltemp);
				Baltemp = new BallotPaper();
				Cannum = 1;
			}
			// If the candidate container is empty/ null create a cadidate this will read line 1 3 
			else
			{
				// Create a new Balotpaper and put it in Baltemp
				// This converts the string read in to an int
				stringstream coversion(line);
				int x = 0;
				coversion >> x;
				// Set Baltemp to whatever is in line which is read from the text file which is a vote and also takes in the candidate relevant to it
				Baltemp->setPreference(Cannum, Canadidates[x - 1]);
				Cannum++;
			}
		}
	}

	int x = 0;
	cout << "Ballot Paper Results" << endl;
	vector<RunningCan> runningCandidates;
	for (auto i = 0; i < Ballotpapers.size(); i++)
	{
		cout << "voter : " << i + 1 << "->";
		for (int j = 0; j < Canadidates.size(); j++)
		{
			cout << Ballotpapers[i].getPreference(j + 1).getCandidatename() << ",";
		}
		cout << endl;
	}
	for (auto c : Canadidates)
	{
		RunningCan *tmp = new RunningCan;
		tmp->can = c;
		for (auto bp : Ballotpapers)
		{
			if (bp.getPreference(1).getCandidatename() == c.getCandidatename())
			{
				tmp->Votesfor.push_back(bp);
			}
		}
		runningCandidates.push_back(*tmp);
	}
	
	
	//THis is the alogrotim
	//
	while (runningCandidates.size() > 1) 
	{
		//staRT OF ROUND
		for (int j = 0; j < runningCandidates.size(); j++)
		{
			cout << runningCandidates[j].can.getCandidatename() << " Votes =" << runningCandidates.at(j).Votesfor.size() <<endl;
		}

		int lowestVote = Ballotpapers.size() + 1; //cannot be bigger then the amount of total ballotpapers
		// This determins the lowest vote
	
		for (int y = 0; y < runningCandidates.size(); y++)
		{
			if (runningCandidates[y].Votesfor.size() < lowestVote)
			{
				lowestVote = runningCandidates[y].Votesfor.size();
			}
		}
		
		//adds losers to the loser vector
		vector<RunningCan> losers;
		for (int y = 0; y < runningCandidates.size(); y++)
		{
			if (runningCandidates[y].Votesfor.size() == lowestVote)
			{
				losers.push_back(runningCandidates[y]);
			}
		}

		RunningCan lost;

		if (losers.size() == 1)
		{
			lost = losers[0];
		}
		else // more then one loser
		{
			//loop through the losers and print out the names and say that they tied and one is being randomly selected to lose
			int lostIndex = rand() % losers.size(); // gives a random number between 0 and the size of the running candidates -1
			lost = losers[lostIndex];
		}
		cout << endl;
		cout << lost.can.getCandidatename() << " Has lost the race with : " << lost.Votesfor.size() << " votes" << endl;
		for (int y = 0; y < lost.Votesfor.size(); y++)
		{
			int pref = 2; //We already started at one
			bool movedBallot = false;
			while (movedBallot == false)
			{
				for (int x = 0; x < runningCandidates.size(); x++)
				{
					if (lost.Votesfor[y].getPreference(pref).getCandidatename() == runningCandidates[x].can.getCandidatename() &&
						lost.Votesfor[y].getPreference(pref).getCandidatename() != lost.can.getCandidatename())
					{
						runningCandidates[x].Votesfor.push_back(lost.Votesfor[y]);
						movedBallot = true;
					}

				}
				pref++;
			}
		}
		vector<RunningCan> tmp;
		for (int i = 0; i < runningCandidates.size(); i++)
		{
			if (runningCandidates[i].can.getCandidatename() != lost.can.getCandidatename())
				tmp.push_back(runningCandidates[i]);
		}
		runningCandidates = tmp;
	}
	cout << endl;
	cout << runningCandidates[0].can.getCandidatename() << " Has won the election with : " << runningCandidates[0].Votesfor.size() << " votes" << endl;

	system("pause");
	return 0;
}




