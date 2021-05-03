#pragma once
#include <vector>

enum class NodeState
{
    Active,
    Attacked,
    Deactivated
};

class Node
{
public:
    std::vector<Node*> AdjacencyList;
    bool bVisited = false;
    NodeState State = NodeState::Active;
    int ID = -1;
    friend class Network;
};


class Network
{
public:
    std::vector<std::vector<Node*>> NodeList;

    void InitializeLatticeNetwork(int XYsize);
    bool IsValidIndex(int X, int Y);
    std::vector<Node*> AttackRandomNode(int TargetN);
    std::vector<std::vector<Node*>> GetClusters();
    void ClearVisited();
    void KillSmallClusters();
    void PrintCurrentState();
    void DistroyNodes();
    float GetDeActivateRatio();
    void EndPhase();

    virtual ~Network()
    {
        DistroyNodes();
    }
};

