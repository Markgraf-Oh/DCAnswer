// DCAnswer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>
#include <time.h>
#include "Network.h"


int main()
{
    int A = 200;
    int B = 400;
    //5, 1
    //10, 5
    //100, 100
    //200, 400
    srand(time(NULL));
    Network Net;
    Net.InitializeLatticeNetwork(A);
    while (Net.GetDeActivateRatio() < 0.6f)
    {
        Net.AttackRandomNode(B);
        Net.KillSmallClusters();
        Net.PrintCurrentState();
        Net.EndPhase();
    }
    Net.DistroyNodes();
}
