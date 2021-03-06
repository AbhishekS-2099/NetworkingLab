#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include<cstdlib>
#include<time.h>
using namespace std;
sem_t resource,rmutex,wmutex,readTry;
int readcount = 0;
int writecount = 0;
int shared_resource = 0;
void *ReaderCallBack(void * thread_no){
    sem_wait(&readTry);
    sem_wait(&rmutex);
    readcount++;
    if(readcount == 1)
        sem_wait(&resource);
    sem_post(&rmutex);    
    sem_post(&readTry);
    printf("Thread Number :%ld ,Shared Resource : %ld \n",(long)thread_no,shared_resource);
    sem_wait(&rmutex);
    readcount--;
    if(readcount == 0)
        sem_post(&resource);    
    sem_post(&rmutex);
    pthread_exit(NULL);
}
void *WriterCallBack(void * thread_no){
    sem_wait(&wmutex);
    writecount++;
    if(writecount == 1)
        sem_wait(&readTry);
    sem_post(&wmutex);    
    sem_wait(&resource);
    shared_resource++;
    printf("Shared resource was modified by thread number : %ld \n",(long)thread_no);
    sem_post(&resource);
    sem_wait(&wmutex);
    writecount--;
    if(writecount == 0)
        sem_post(&readTry);
    sem_post(&wmutex);    

    pthread_exit(NULL);
}
int main(){
    sem_init(&resource,0,1);
    sem_init(&rmutex,0,1);
    sem_init(&wmutex,0,1);
    sem_init(&readTry,0,1);
    srand (time(NULL));

    for(int i = 0;i<14;i++){
        int randomNo = rand()%100+1;
        if(randomNo <50){
            pthread_t reader;
            int iret;
            iret = pthread_create(&reader,NULL,ReaderCallBack,(void *)i);
            if(iret){
                return 1;
            }
            }
        else{
            pthread_t writer;
            int iret;
            iret = pthread_create(&writer,NULL,WriterCallBack,(void *)i);
            if(iret)
                return 1;

            }
  
    }

    return 0;
}
