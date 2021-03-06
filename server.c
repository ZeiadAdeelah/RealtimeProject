#include "conf.h"
#define KEY 500
void (* old_handler)(int);
void handler(int signum);
int fd = 0;

int main() {
    int numWorkers,numConnections,portNumber;

    struct sockaddr_in srv;
//    struct sockaddr_in cli;
    /*read from file*/
    int fileData[3];
    int i=0;
    FILE* fp;
    fp=fopen("conf.txt","r");
    char line[10];
    while(fgets(line, sizeof(line), fp) != NULL){
        fileData[i++] = atoi(line);
    }

    for(i=0;i<3;i++)
        printf("%d\n",fileData[i]);

    numWorkers = fileData[0];
    numConnections = fileData[1];
    portNumber = fileData[2];
    int *workersPIDs = (int *)malloc(numWorkers*sizeof(int));

    /*socket creation*/

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    srv.sin_family = AF_INET; /* use the Internet addr family */
    srv.sin_port = htons(portNumber); /* bind socket ‘fd’ to port 80*/
    /* bind: a client may connect to any of my addresses */

    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("bind error");
        exit(1);
    }

    if(listen(fd, 1000) < 0) {
        perror("listen");
        exit(1);
    }
    printf("after binding the socket with fd %d\n",fd);

    /*workers creation*/
    for(int j=0;j<numWorkers;j++){
        int pid = fork();




        if(!pid){//child
            old_handler = signal (SIGINT, handler);
//            printf("request Accepted by child %d\n",getpid());
           while(1);



//            printf("child %d\n",getpid());
//            int qid = open_MQ(KEY);
////            printf("qid child %d\n",qid);
//            struct msgbuf msg;
//            strcpy(msg.a,"je");
//            printf("fd %d\n",fd);
//            while(1){
//                receive_msg_MQ(qid,&msg,getpid());
//                msgrcv(qid,&msg,sizeof(msg)-sizeof(long),getpid(),0);
//                int newfd; /* returned by accept() */
//                int cli_len = sizeof(cli);
//                //newfd= (int*)malloc(sizeof(int));
////                printf("fd %d\n",fd);
//                newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);
//                if(newfd < 0) {
//                    perror("accept"); exit(1);
//                }
//                printf("the message is %s in child %d\n",msg.a, getpid());
//            }



            return 0;

        }
        else{ //parent
//            printf("parent %d with child %d\n",getpid(),pid);
            workersPIDs[j] = pid;
        }

//        execlp();
    }
//    for(int k=0;k<numWorkers;k++){
//        printf("worker #%d is %d\n",k,workersPIDs[k]);
//    }
    printf("after forking \n");

    /*MQ*/


//    int qid = open_MQ(KEY);
//    printf("qid parent %d\n",qid);
//    struct msgbuf p;
//    strcpy(p.a,"ahln");
//    int workerIndex = rand() % 10;
//    long workerPID = workersPIDs[workerIndex];
//    printf("we send message to child %ld\n",workerPID);
//    p.type = workerPID;
//    p.pid = getpid();

//    send_msg_MQ(qid,&p);
//    if(msgsnd(qid,&p,sizeof(p)-sizeof(long),0)<0)
//    {
//        perror("error in sending to MQ");
//        exit(1);
//    }
//    printf("message is sent to worker\n");


    /* socket descriptor */



    /*check if there's data to read from the general socket*/
    while(1){

        fd_set mainReadfd;
        FD_ZERO(&mainReadfd);
        FD_SET(fd,&mainReadfd);
        if(select(fd+1,&mainReadfd,0,0,0)<0){
            perror("select");
            exit(1);
        }
        if(FD_ISSET(fd,&mainReadfd)){ // send msg to one of the workers to accept the request
//            printf("there's data on the main socket\n");
            int workerIndex = rand() % numWorkers;
            int workerPID = workersPIDs[workerIndex];
            kill(workerPID,SIGINT);
           //sleep(1000);

        }
       //break;
        
    }













//    }

}


int open_MQ(key_t key){

    return msgget(key,0666|IPC_CREAT);
}
int send_msg_MQ(int qid, struct msgbuf *msgp){
    int sz;
    sz= sizeof(msgp)-sizeof(long);
    return msgsnd(qid,msgp,sz,0);
}

int receive_msg_MQ(int qid, struct msgbuf *msgp,int type){
    int sz;
    sz= sizeof(msgp)-sizeof(long);

    return msgrcv(qid,msgp,sz,type,0);
}

void remove_MQ(int qid){
    msgctl(qid,IPC_RMID,NULL);
}


void handler(int signum) {
    signal (SIGINT, handler);
    printf("Signal handled\n");
    struct sockaddr_in cli;
    int cli_len = sizeof(cli);
    int newfd; /* returned by accept() */
    int nbytes;
    char buffer[512];
    int next = 0;
    int newfds[100], sd;
    int max_sd;
    fd_set readfds;

    //strcpy(buffer,"fdset msg from server to client");

     for (int i = 0; i < 100; i++)
        {
            newfds[i] = 0;
        }
    while(1){
        FD_ZERO(&readfds);

        FD_SET(fd,&readfds);
        max_sd = fd; 
        printf("before for\n");
/* Now use FD_SET to initialize other newfd’s that have already been returned by accept() */
        for(int i=0;i<100;i++){
            sd =  newfds[i] ;
            FD_SET(sd,&readfds);
           // if(sd > max_sd)
             //  max_sd = sd;
            //printf("%d\n",i);
        }
          max_sd = 100;
        //printf("after for\n");
       // printf("max sd %d\n", max_sd);
        select(max_sd+1, &readfds, NULL, NULL, NULL);
         //printf("after select\n");

       if(FD_ISSET(fd, &readfds)) {
           if ((newfds[next++] = accept(fd, (struct sockaddr *) &cli, &cli_len)) < 0) {
               perror("accept");
               exit(1);
           } else {


               nbytes = read(newfds[next - 1], &buffer, sizeof(buffer));
               printf("%s buffer value from client\n", buffer);
               printf("%d next value\n", next);
               if (write(newfds[next-1], &buffer, strlen(buffer) < 0)) {
                   perror("write");
                   exit(1);
               }
           }

       }


           for (int i = 0; i < 100; i++) {
               if (FD_ISSET(newfds[i], &readfds)) {
                   if ((nbytes = read(newfds[i], &buffer, sizeof(buffer))) < 0) {
                       perror("read error");
                       exit(1);
                   } else {
                       write(newfds[i], &buffer, strlen(buffer) < 0); 
                       printf("%s buffer value\n", buffer);
                       newfds[i] = 0;
                       close(newfds[i]);
                   }
               }
           }

           if (next == 99) {
               next = 0;
           }

    }
}




