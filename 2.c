#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
//implementation of max heap
#include "heap.h"
//structure to store process
struct Process{
    int arrival_time;
    int burst_time;
    int pr;
    int ind;
};

//returns the weight of the process
float weight(float w1, float w2, int t, int pr){
    float ans = 0.0;
    ans = (w1*t + w2*pr);
    return ans;
}
//function for scheduling the processes according to PLRTF algo
void plrtf(int n, struct Process pro[n], float w1, float w2, int exec[], int wait[], int end_time[]){

    struct Wt a[n];
    //storing burst time and turn around time
    int bt[n], tat[n];
    int time, n1 = 0, comp = 0;
    struct Wt *top;
    //copy the burst time of the processes into bt[]
    for (int i = 0; i < n; i++) 
        bt[i] = pro[i].burst_time; 
    
    for(time = 0; comp != n; time++){
        //flag to check if any new process has entered the ready queue
        int check = 0;
        for(int i = 0; i < n; i++){
            //add any incoming processes to the heap
            if(time == pro[i].arrival_time){
                struct Wt temp;
                temp.ind = pro[i].ind;
                temp.weight = weight(w1,w2,bt[i], pro[i].pr);
                temp.at = pro[i].arrival_time;
                insert(a,temp,&n1);
                check = 1;
            }
        }
        int top_ind;
        //no new process
        //continue executing previously executing process
        if(check == 0){
            //ready queue is empty, do nothing
            if(is_empty(a,&n1))
                continue;           
        }else{
            //new process enters
            //check for maxium weight process from the heap and execute
            // print_heap(a,n1);
            top = get_max(a);      
        }

        top_ind = (*top).ind;
        //reduce burst time
        bt[top_ind] --;
        exec[time] = top_ind;
        end_time[top_ind] = time + 1;
        //if process is completed update wait time
        if(bt[top_ind] == 0){
            comp++;
            struct Wt temp = extract_max(a,&n1);
            tat[top_ind] = end_time[top_ind] - pro[top_ind].arrival_time;
            wait[top_ind] = tat[top_ind] - pro[top_ind].burst_time;                    
        }
        if(check == 0){
            //update weight
            (*top).weight = weight(w1,w2,bt[top_ind],pro[top_ind].pr);
        }    
    }
}

int main(){

    int n;
    float w1, w2;
    printf("Enter the number of processes: ");
    scanf("%d",&n);

    struct Process pro[n];
    int wait[n], end_time[n];
    for(int i = 0; i < n; i++){
        pro[i].arrival_time = 0;
        pro[i].burst_time = 0;
        pro[i].pr = 0;
        wait[i] = 0;
        end_time[i] = 0;
    }
    int tot_time = 0, tot_wait = 0, first_proc_time = INT_MAX;
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
        if(t < first_proc_time){
            first_proc_time =t;
        }
        tot_time += b;
    }
    tot_time += first_proc_time;
    printf("Enter the value of w1: ");
    scanf("%f",&w1);
    printf("Enter the value of w2: ");
    scanf("%f",&w2);
    int exec[tot_time];
    for(int i = 0; i < tot_time; i++){
        exec[i] = -1;
    }
    //main function 
    plrtf(n, pro, w1, w2,exec,wait,end_time);
    
    printf("\n\n");
    for(int i = 0; i < n; i++){
        tot_wait += wait[i];
        printf("Waiting time of P%d: %d\n",i+1,wait[i]);
    }
    printf("\n\n");
    printf("Average waiting time: %f\n\n",(float)tot_wait/(float)n);

    printf("\nExecution order : P%d",exec[first_proc_time]+1);
    for(int i = first_proc_time+1; i < tot_time; i++){
        if(exec[i] == -1)continue;
        if(exec[i] == exec[i-1]){
            continue;
        }else{
            printf("->P%d",exec[i]+1);
        }
    }
    printf("\n\n");
    for(int i = 0; i < n; i++){
        printf("P%d finished at: %d\n",i+1,end_time[i]);
    }
    printf("\n\n");
    
    return 0;
}