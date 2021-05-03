#include "Network.h"
#include <iostream>
#include <queue>

void Network::InitializeLatticeNetwork(int XYsize)
{
	NodeList.clear();
	NodeList.resize(XYsize);
	int NodeCount = 0;
	for (int i = 0; i < XYsize; i++)
	{
		for (int j = 0; j < XYsize; j++)
		{
			Node* AddingNode = new Node();
			AddingNode->ID = NodeCount++;
			NodeList[i].push_back(AddingNode);
		}
	}

	std::vector<std::pair<int, int>> Nearby = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

	for (int x = 0; x < XYsize; x++)
	{
		for (int y = 0; y < XYsize; y++)
		{
			for (auto& Next : Nearby)
			{
				if (IsValidIndex(x + Next.first, y + Next.second))
				{
					NodeList[x][y]->AdjacencyList.push_back(NodeList[x + Next.first][y + Next.second]);
				}
			}
		}
	}
}


bool Network::IsValidIndex(int X, int Y)
{
	if (X < 0 || X >= NodeList.size())
	{
		return false;
	}
	if (Y < 0 || Y >= NodeList[0].size())
	{
		return false;
	}
	
	return true;
}

std::vector<Node*> Network::AttackRandomNode(int TargetN)
{
	std::vector<Node*> AliveNodeList;
	for (auto& NodeLine : NodeList)
	{
		for (Node* CurrentNode : NodeLine)
		{
			if (CurrentNode->State == NodeState::Active)
			{
				AliveNodeList.push_back(CurrentNode);
			}
		}
	}
	int ListSize = AliveNodeList.size();
	for (int i = 0; i < ListSize; i++)
	{
		int TargetLoc = rand() % ListSize;
		std::swap(AliveNodeList[i], AliveNodeList[TargetLoc]);
	}

	std::vector<Node*> OutVector;
	for (int i = 0; i < TargetN; i++)
	{
		OutVector.push_back(AliveNodeList[i]);
		AliveNodeList[i]->State = NodeState::Attacked;
	}
	return OutVector;
}

std::vector<std::vector<Node*>> Network::GetClusters()
{
	std::vector<std::vector<Node*>> ClusterList;
	ClearVisited();
	std::vector<Node*> AliveNodeList;
	for (auto& NodeLine : NodeList)
	{
		for (Node* CurrentNode : NodeLine)
		{
			if (CurrentNode->State == NodeState::Active)
			{
				AliveNodeList.push_back(CurrentNode);
			}
		}
	}
	for (Node* StartNode : AliveNodeList)
	{
		if (StartNode->bVisited == true || StartNode->State != NodeState::Active)
		{
			continue;
		}		
		ClusterList.emplace_back();
		int ClusterIndex = ClusterList.size() - 1;		
		std::queue<Node*> BFSQ;
		BFSQ.push(StartNode);
		StartNode->bVisited = true;
		while (!BFSQ.empty())
		{
			Node* Current = BFSQ.front();
			BFSQ.pop();
			for (Node* Nearby : Current->AdjacencyList)
			{
				if (Nearby->bVisited == true || Nearby->State != NodeState::Active)
				{
					continue;
				}
				BFSQ.push(Nearby);
				Nearby->bVisited = true;
			}
			ClusterList[ClusterIndex].push_back(Current);
		}
	}
	return ClusterList;
}

void Network::ClearVisited()
{
	for (auto& NodeLine : NodeList)
	{
		for (Node* CurrentNode : NodeLine)
		{
			CurrentNode->bVisited = false;
		}
	}
}

void Network::KillSmallClusters()
{
	std::vector<std::vector<Node*>> ClusterList = GetClusters();
	int LargestClusterSize = 0;
	for (const auto& Cluster : ClusterList)
	{
		LargestClusterSize = std::max<int>(LargestClusterSize, Cluster.size());
	}
	std::vector<int> BigClusterIndexs;
	for (int i=0; i< ClusterList.size(); i++)
	{
		if (ClusterList[i].size() == LargestClusterSize)
		{
			BigClusterIndexs.push_back(i);
		}
	}
	int LargestClusterIndex = BigClusterIndexs[rand() % BigClusterIndexs.size()];

	for (int i = 0; i < ClusterList.size(); i++)
	{
		if (i == LargestClusterIndex)
		{
			continue;
		}
		for (Node* DisconnectedNode : ClusterList[i])
		{
			DisconnectedNode->State = NodeState::Attacked;
		}
	}
}

void Network::PrintCurrentState()
{
	std::cout << std::endl << "Print Network" << std::endl;
	for (std::vector<Node*>& NodeLine : NodeList)
	{		
		for (Node* Target : NodeLine)
		{
			const char* Out = "E";
			switch (Target->State)
			{
			case NodeState::Active :
				Out = "-";
				break;
			case NodeState::Attacked:
				Out = "O";
				break;
			case NodeState::Deactivated:
				Out = "@";
				break;
			default:
				break;
			}
			std::cout << Out;
		}
		std::cout << std::endl;
	}
	std::cout << "DeactivateRatio = " << GetDeActivateRatio() << std::endl;
}

void Network::DistroyNodes()
{
	for (std::vector<Node*>& NodeLine : NodeList)
	{
		for (Node*& Target : NodeLine)
		{
			delete Target;
		}
		NodeLine.clear();
	}
	NodeList.clear();
}

float Network::GetDeActivateRatio()
{
	int DeActivateCount = 0;
	for (std::vector<Node*>& NodeLine : NodeList)
	{
		for (Node* Target : NodeLine)
		{
			if (Target->State != NodeState::Active)
			{
				DeActivateCount++;
			}
		}
	}
	int TotalNodeN = NodeList.size() * NodeList[0].size();
	return static_cast<float>(DeActivateCount) /static_cast<float>(TotalNodeN);
}

void Network::EndPhase()
{
	for (std::vector<Node*>& NodeLine : NodeList)
	{
		for (Node* Target : NodeLine)
		{
			if (Target->State == NodeState::Attacked)
			{
				Target->State = NodeState::Deactivated;
			}
		}
	}
}
