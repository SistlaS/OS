#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
struct Process{
    int arrival_time;
    int burst_time;
    int pr;
    int ind;
};

int comp(const void* p, const void* q){
    return ((struct Process*)p)->arrival_time > ((struct Process*)q)->arrival_time;
}

float weight(float w1, float w2, int t, int pr){
    float ans = 0.0;
    ans = (w1*t + w2*pr);
    return ans;
}

void avg(int n, struct Process pro[n], float w1, float w2, int exec[], int wait[], int end[]){

    struct Wt a[n];
    int rt[n], tat[n];
    int time, n1 = 0, comp = 0;
    struct Wt *top;

    for (int i = 0; i < n; i++) 
        rt[i] = pro[i].burst_time; 
    
    for(time = 0; comp != n; time++){
        int check = 0;
        for(int i = 0; i < n; i++){
            //add any incoming processes to the heap
            if(time == pro[i].arrival_time){
                struct Wt temp;
                temp.ind = pro[i].ind;
                temp.weight = weight(w1,w2,rt[i], pro[i].pr);
                insert(a,temp,&n1);
                check = 1;
            }
        }
        
        //no new process
        if(check == 0){
            //ready queue is empty
            if(is_empty(a,&n1)){
                continue;
            }
            //update prev max weight           
            int top_ind = (*top).ind;
            // printf("time = %d  top ind = %d  we = %f",time,top_ind,(*top).weight);
            rt[top_ind] --;
            exec[time] = top_ind;
            end[top_ind] = time + 1;
            if(rt[top_ind] == 0){
                comp++;
                struct Wt temp = extract_max(a,&n1);
                tat[top_ind] = end[top_ind] - pro[top_ind].arrival_time;
                wait[top_ind] = tat[top_ind] - pro[top_ind].burst_time;                    
            }
            (*top).weight = weight(w1,w2,rt[top_ind],pro[top_ind].pr);
        }else{
            //new process enters
            //get the max from heap
            top = get_max(a);
            int top_ind = (*top).ind;
            // printf("time = %d  top ind = %d we = %f",time,top_ind,(*top).weight);
            rt[top_ind] --;
            exec[time] = top_ind;
            end[top_ind] = time + 1;
            //wait[top_ind] = end[top_ind] - pro[top_ind].arrival_time;
            if(rt[top_ind] == 0){
                comp++;
                struct Wt temp = extract_max(a,&n1);
                tat[top_ind] = end[top_ind] - pro[top_ind].arrival_time;
                wait[top_ind] = tat[top_ind] - pro[top_ind].burst_time;   
            }
        }
    }
}

int main(){

    int n;
    float w1, w2;
    printf("Enter the number of processes: ");
    scanf("%d",&n);

    struct Process pro[n];
    int wait[n], end[n];
    for(int i = 0; i < n; i++){
        pro[i].arrival_time = 0;
        pro[i].burst_time = 0;
        pro[i].pr = 0;
        wait[i] = 0;
        end[i] = 0;
    }
    int tot_time = 0;
    int tot_wait = 0;
    for(int i = 0; i < n; i++){
        int t, b, p;
        printf("Enter the arrival time of P%d: ",i+1);
        scanf("%d",&t);
        printf("Enter CPU burst cycle of P%d: ",i+1);
        scanf("%d",&b);
        printf("Enter priority of P%d: ",i+1);
        scanf("%d",&p);

        pro[i].arrival_time = t;
        pro[i].burst_time = b;
        pro[i].pr = p;
        pro[i].ind = i;
        tot_time += b;
    }

    printf("Enter the value of w1: ");
    scanf("%f",&w1);
    printf("Enter the value of w2: ");
    scanf("%f",&w2);
    int exec[tot_time];
    avg(n, pro, w1, w2,exec,wait,end);
    
    printf("\n\n");
    for(int i = 0; i < n; i++){
        tot_wait += wait[i];
        printf("Waiting time of P%d: %d\n",i+1,wait[i]);
    }
    printf("\n\n");
    printf("Average waiting time: %f\n\n",(float)tot_wait/(float)n);
    printf("\nExecution order : P%d",exec[0]+1);
    for(int i = 1; i < tot_time; i++){
        if(exec[i] == exec[i-1]){
            continue;
        }else{
            printf("->P%d",exec[i]+1);
        }
    }
    printf("\n\n");
    for(int i = 0; i < n; i++){
        printf("P%d finished at: %d\n",i+1,end[i]);
    }
    printf("\n\n");
    
    return 0;
}