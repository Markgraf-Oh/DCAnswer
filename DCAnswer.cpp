// DCAnswer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>
#include <time.h>
#include "Network.h"


int main()
{
    int A = 5;
    int B = 1;
    //5, 1
    //10, 5
    //100, 100
    //200, 400
    srand(time(NULL));
    int Runtime = 100;
    float Endtime = 0.0f;
    for (int i = 0; i < Runtime; i++)
    {
        Network Net;
        Net.InitializeLatticeNetwork(A);
        int day = 0;
        while (Net.GetDeActivateRatio() < 0.6f)
        {
            day++;
            Net.EndPhase();
            Net.AttackRandomNode(B);
            Net.KillSmallClusters();
            //Net.PrintCurrentState();
        }
        Net.PrintCurrentState();
        std::cout << "EndTime = " << day << std::endl;
        Endtime += static_cast<float>(day) / static_cast<float>(Runtime);
        Net.DistroyNodes();
    }
    std::cout << "Average : " << Endtime << std::endl;
    
    //TTS 켜져 있으니까 질문하면 바로 답할게요
    //생각해보니 이거 내가 디버깅해도 결과가 안보이는 구나 ㄷㄷ
}
