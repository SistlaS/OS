#include<stdio.h>
#include<limits.h>
//structure to store process
struct Process{
    int arrival_time;
    int burst_time;
    int pr;
};

//returns the weight of the process
float weight(float w1, float w2, int t, int pr){
    float ans = 0.0;
    ans = (w1*t + w2*pr);
    return ans;
}
//function for scheduling the processes according to PLRTF algo
void plrtf(int n, struct Process pro[n], float w1, float w2, int exec[], int end_time[]){
    //index of currently executing process
    int curr_proc = -1, time;
    double wt = 0;
    //number of completed processes
    int comp = 0;
    for(time = 0; comp != n; time++){
        if(curr_proc != -1){
            //update wt
            wt = weight(w1,w2,pro[curr_proc].burst_time,pro[curr_proc].pr);
        }
        //if a new process enters the queue, execute the process with the highest weight
        for(int i = 0; i < n; i++){
            if(pro[i].arrival_time == time){
                double p = weight(w1,w2,pro[i].burst_time,pro[i].pr);
                if(p > wt){
                    curr_proc = i;
                    wt = p;
                }
            }
        }
        //reduce the burst time of the executing process
        if(curr_proc != -1 && pro[curr_proc].burst_time > 0){
            pro[curr_proc].burst_time--;
        }

        exec[time] = curr_proc;
        //if a process is executed completely 
        //update the end time and find the next process with highest weight
        if(curr_proc != -1 && pro[curr_proc].burst_time == 0){
            comp++;
            end_time[curr_proc] = time + 1;
            curr_proc = -1;
            wt = 0;

            for(int j=0;j<n;j++){
                if(pro[j].burst_time > 0 && pro[j].arrival_time <= time){
                    double p = weight(w1,w2,pro[j].burst_time,pro[j].pr);
                    if(p > wt){
                        wt = p;
                        curr_proc = j;
                    }
                }
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
    //stores the index of the executing process at each time
    int exec[tot_time];
    for(int i = 0; i < tot_time; i++){
        exec[i] = -1;
    }
    int bt[n];
    for (int i = 0; i < n; i++) 
        bt[i] = pro[i].burst_time; 
    
    //main function 
    plrtf(n, pro, w1, w2, exec, end_time);
    
    printf("\n");
    //compute the wait time
    for(int i=0;i<n;i++){
        wait[i] = end_time[i] - bt[i] - pro[i].arrival_time;
        tot_wait += wait[i];
        printf("Waiting time for P%d: %d\n", i+1, wait[i]);
    }
    printf("\nAverage waiting time: %0.2lf\n", tot_wait/(double)n);

    printf("\nProcess Execution order: P%d", exec[first_proc_time]+1);
    for(int j = first_proc_time; j < tot_time - 1;j++){
        if(exec[j]!=exec[j+1] && exec[j+1]!=-1){
            printf(" -> ");
            printf("P%d", exec[j+1]+1);
        }
    }
    printf("\n");

    for(int i=0;i<n;i++)
        printf("\nProcess P%d finished at: %d", i+1, end_time[i]);

    printf("\n\n");

    return 0;
}