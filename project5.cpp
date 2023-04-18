// Project 5
// CS 2413 Data Structures
// Spring 2023

// Code generated for input 1 would be:
// if(expression1)
// {
// 	statement1;
// }
// else
// {
// 	statement2;
// }

#include <iostream>
#include <string> // for the fields in the class
#include <vector> // for the flowchart structures
#include <stack>  // for conversion

using namespace std;

// class to store node information
class Node
{
private:
	int nodeNumber;    // to store the node number
	string type;       // can either be START, END, IF, BLOCK
	string textWithin; // can be expression or statement - for START and END this will be empty string
public:
	// TODO: construtors, getters, setters and other methods as needed
	Node()//default constructor
	{
		nodeNumber = 0;
		type = "";
		textWithin = "";
	}
	Node(int nodeNumber, string type)//non-default constructor: if textWithin is absent
	{
		this->nodeNumber = nodeNumber;
		this->type = type;
	}
	Node(int nodeNumber, string type, string textWithin)//non-default constructor
	{
		this->nodeNumber = nodeNumber;
		this->type = type;
		this->textWithin = textWithin;
	}

	int getNodeNumber()
	{
		return nodeNumber;
	}
	string getType()
	{
		return type;
	}
	string getTextWithin()
	{
		return textWithin;
	}
	string getTypeByNumber(int num) //return type of corresponding node number
	{
		if(this->nodeNumber == num)return type;
		return ""; //return empty string if no matches
	}

	void display() //display node info for debugging
	{
		if(textWithin != "")cout << nodeNumber << " " << type << " " << textWithin << '\n';
		else cout << nodeNumber << " " << type << '\n';
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Node getNodeByNumber(int num, vector<Node> allNodes) //helper function to return the node of its respective number
{
	for(int i = 0; i < allNodes.size(); i++)
	{
		if(allNodes[i].getNodeNumber() == num)return allNodes[i];
	}
	return Node();
}
vector<vector<int>> generateOutflows(vector<vector<int>> adjList) //helper function to generate a vector of vectors that store each node's outflow information
{																  //to save line space in the convert flowchart method
	vector<vector<int>> outflows = vector<vector<int>>(adjList); //copy constructor of vector
	for(int i = 0; i < outflows.size(); i++)
	{
		outflows[i].erase(outflows[i].begin());
	}
	return outflows;
}

// function to convert the given flowchart to generate code
void convertFlowChart(vector<Node> allNodes, vector<vector<int>> adjList)
{
	cout << "start\n";
	vector<vector<int>> outflows = generateOutflows(adjList);//create a vector of outflows for ease of use in code
	int depth = 0; //int to keep track of indentation depth for nested if statements
	int numOpenParenthesis = 0; //keep track of open parenthesis
	int hasElse = 0; //keep track of required else statements
	int hasIf = 0; //keep track of open if statements
	bool prevIfClosed = false; //for determining where to print an else statement
	stack<Node> nodeStack; //initialize the stack

	nodeStack.push(getNodeByNumber((outflows.front().front()), allNodes)); //push in starting node

	while(!nodeStack.empty()) //while there are still nodes in the stack
	{
		Node currNode = nodeStack.top(); //set a current node equal to the top node
		nodeStack.pop(); //pop the top node
		if(currNode.getType() == "END") //end the loop if the current node is "END": "END" types will always be the last in the stack
		{
			continue;
		}
		if(currNode.getType() == "IF") //if the current node is an "IF" node
		{
			for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
			cout << "if(" << currNode.getTextWithin() << ")\n"; //output the if statement beginning
			for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
			cout << "{\n"; //output if statement open bracket
			depth++; hasIf++; hasElse++; numOpenParenthesis++; //increase corresponding tracker variables
		}
		if(currNode.getType() == "BLOCK") //if the current node is a "BLOCK" node
		{
			if(numOpenParenthesis == 0 && hasElse == 0) //if there are no open parenthesis
			{
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << currNode.getTextWithin() << "\n"; //output BLOCK text
			}
			if(numOpenParenthesis > 0 && !prevIfClosed) //for closing an open if statement, checks if the previous action was to close an if statement
			{
				if(depth == 0) //if there is no depth (outside of parenthesis), this is to prevent blocks being printed between if and else statements
				{
					for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
					cout << "}\n"; //output closing bracket
					hasIf--; numOpenParenthesis--; prevIfClosed = true; //modify all tracker variables accordingly
					continue; //go to beginning of while loop
				}
				//otherwise execute this code
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << currNode.getTextWithin() << "\n"; //output block text
				if(depth > 0)depth--; //prevent depth from becoming negative
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << "}\n"; //output closing bracket
				hasIf--; numOpenParenthesis--; prevIfClosed = true; //modify all tracker variables accordingly
				continue; //return to beginning of loop
			}
			if(hasElse > hasIf) //for standard else case; more open elses than ifs implies the else must be created before the if is closed
			{
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << "else\n";
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << "{\n";
				numOpenParenthesis++; depth++; //increase tracker variables due to added bracket
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << currNode.getTextWithin() << "\n"; //output block text
				if(depth > 0)depth--; //decrease depth for closing bracket
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << "}\n"; //closing bracket for else statement
				numOpenParenthesis--; if(depth > 0)depth--; //decrease depth
				hasElse--; prevIfClosed = false; //modify all tracker variables accordingly 
			}
			else if(hasIf > 0 && numOpenParenthesis > 0 && hasElse > 0) //for closing a nested if statement
			{
				for(int i = 0; i < depth; i++)cout << " ";//indent based on depth
				cout << "}\n"; //closing bracket
				depth--; hasIf--; numOpenParenthesis--; prevIfClosed = false; //modify all tracker variables accordingly 
			}
		}

		//loop to push the next set of outflows on to the stack
		for(int i = outflows[currNode.getNodeNumber()].size()-1; i > -1; i--) //loop through all the outflows of the current node
		{
			nodeStack.push(allNodes[outflows[currNode.getNodeNumber()][i]]); //push all the outflows of the current node on to the stack
		}
	} //continue the loop
	cout << "end"; //output end once while loop is complete
}

void displayAutograderOutput(vector<Node> allNodes, vector<vector<int>> adjList) //function to output non logic intensive autograder output
{
	for(int i = 0; i < allNodes.size(); i++)//loop through all nodes and print all node data
	{
		if(allNodes[i].getType() == "START" || allNodes[i].getType() == "END")cout << i << ": " << allNodes[i].getType() << " node\n"; //conditional for start and end nodes
		else cout << i << ": " << allNodes[i].getType() << " node - " << allNodes[i].getTextWithin() << '\n';
	}
	vector<vector<int>> outflows = generateOutflows(adjList); //outflow vector for autograder output
	cout << "AdjList:\n";
	for(int i = 0; i < outflows.size(); i++)
	{
		cout << i << ": ";
		for(int j = 0; j < outflows[i].size(); j++)
		{
			cout << outflows[i][j] << " ";
		}
		cout << '\n';
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{
	int numNodesInFlowChart; // number of nodes in the flow chart

	cin >> numNodesInFlowChart;
	cout << "Number of nodes: " << numNodesInFlowChart << endl;

	// Node objects array to store all the information
	vector<Node> allNodes(numNodesInFlowChart);

	// TODO: read in the information about the nodes and store it in allNodes
	string nodeNumber, type, textWithin;
	cin >> nodeNumber >> type;
	allNodes[0] = (Node(stoi(nodeNumber), type)); //for start node
	for(int i = 1; i < numNodesInFlowChart-1; i++) //loop to read in all nodes between start and end
	{
		cin >> nodeNumber >> type >> textWithin; 
		allNodes[i] = (Node(stoi(nodeNumber), type, textWithin)); //add node into vector
	}
	cin >> nodeNumber >> type;
	allNodes[allNodes.size()-1]=(Node(stoi(nodeNumber), type)); //for end node

	// adjacency list to store the flow chart
	vector<vector<int>> adjList(numNodesInFlowChart);

	// TODO: read in the adjacency list
	int from, target1, target2;
	cin >> from >> target1;
	vector<int> start(2); //read in first adjacency vector
	start[0] = from;
	start[1] = target1;
	adjList[0] = start;
	for(int i = 1; i < allNodes.size() - 1; i++) //loop to read in all adjacency vectors between start and end
	{
		cin >> from;
		if(allNodes[i].getTypeByNumber(from) == "IF") //conditional for if adjacency, since if will always have 2 adjacencies
		{
			cin >> target1 >> target2;
			vector<int> a(3);
			a[0] = from;
			a[1] = target1;
			a[2] = target2;
			adjList[i] = a;
		}
		else //for block adjacencies
		{
			cin >> target1;
			vector<int> b(2);
			b[0] = from;
			b[1] = target1;
			adjList[i] = b;
		}
	}
	cin >> from >> target1; //read in last adjacency vector
	vector<int> end(1);
	end[0] = from;
	adjList[allNodes.size()-1] = end;

	displayAutograderOutput(allNodes, adjList); //output first half of autograder output
	convertFlowChart(allNodes, adjList); //output code generated from input file

	return 0;
}
