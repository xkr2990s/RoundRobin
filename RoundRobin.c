#include <stdio.h>

#define MAX 100
//대기큐, 스케줄링 결과 배열, 대기큐 끝, 결과 배열 끝, 실행시간, 시간할당량
int queue[MAX], result[MAX], q_rear=-1, r_rear=-1, runngTime=0, timeQuantum=20;
int static totalExe=0;
//프로세스 구조체
struct process{
    int p_id;
    int p_pry;
    int c_time;
    int p_time;
    int p_start;
    int p_end;
}p[MAX];
//대기큐 삽입
void enQueue(int id) {
    queue[++q_rear] = id;
}
//대기큐 삭제
void deQueue(int lostTime) {
    //실행시간 남을 시 대기큐 끝에 삽입
    if(lostTime)
        enQueue(queue[0]);

    for(int i=1;i<=q_rear;i++)
        queue[i-1] = queue[i];
    queue[q_rear] = 0;
    q_rear--;
}
//라운드로빈 알고리즘
void roundRobin(int lostTime) {
    totalExe++;
    //프로세스 입력 시점 입력
    if(p[queue[0]-1].p_start<0) {
        if(lostTime == timeQuantum)
            p[queue[0]-1].p_start = runngTime;
        else {//다른 프로세스 종료 후 다음 프로세스 실행 시
            p[queue[0]-1].p_start = runngTime+lostTime;
            runngTime+=lostTime;
        }
    }
    //프로세스의 필요한 실행시간이 남은 주기보다 클 때
    if(p[queue[0]-1].p_time>lostTime) {
        p[queue[0]-1].p_time -= lostTime;
        deQueue(p[queue[0]-1].p_time);
        runngTime += timeQuantum;
    }
    else { //프로세스의 필요한 실행시간이 남은 주기보다 작거나 같을 떄
        p[queue[0]-1].p_end = runngTime+p[queue[0]-1].p_time;
        lostTime -= p[queue[0]-1].p_time;
        p[queue[0]-1].p_time = 0;
        result[++r_rear] = p[queue[0]-1].p_id;
        //대기큐가 비어있을 때
        if(q_rear<0)
            return;
        //대기큐 삭제
        deQueue(0);
        
        if(!lostTime)
            return;
        //재귀를 통한 남은 실행 시간 스케줄링
        roundRobin(lostTime);
    }
}

int main() {
    int state=1, type, i=0, schedule[3], totalTime=0;
    printf("시간 할당량 입력: ");
    scanf("%d", &timeQuantum);

    while(state){
        scanf("%d", &type);
        //-1, 0, 1
        switch(type) {
            case 0: { //프로세스 생성 & 대기큐 삽입
                scanf("%d %d %d", &(p[i].p_id), &(p[i].p_pry), &(p[i].c_time));
                p[i].p_time = p[i].c_time;
                p[i].p_start = -1;
                enQueue(p[i++].p_id);
                break;
            }
            case 1: { //스케줄링
                scanf("%d %d %d", schedule, schedule+1, schedule+2);
                roundRobin(timeQuantum);
                break;
            }
            case -1: { //프로세스 생성 종료
                state = 0;
                break;
            }
        }
    } //대기큐에 남은 프로세스 스케줄링
    while(q_rear>=0)
        roundRobin(timeQuantum);
    //출력
    printf("\nProcess_id\tcomputing_time\tturn_around time\n");
    for(i=0;i<r_rear;i++) {
        printf("%d\t\t%d\t\t%d\n", p[result[i]-1].p_id, p[result[i]-1].c_time,
                p[result[i]-1].p_end-p[result[i]-1].p_start);
        totalTime += p[result[i]-1].p_end-p[result[i]-1].p_start;
    }
    printf("\n스케줄링 횟수: %d\n총 시간: %d\nATT: %.1f", totalExe, totalTime, (float)totalTime/r_rear);
}