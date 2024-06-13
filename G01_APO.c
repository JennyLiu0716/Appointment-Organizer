#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int globalStartTime, globalEndTime;
FILE *requestFile = NULL, *FCFSfile = NULL, *priorityFile = NULL;
char buff[260], buffRsp[260], fileNames[5][30], fileTypes[4][30], commands[9][20], KILLSIG[10], ACCEPTSIG[6], REJECTSIG[6], PRINTACCEPTSIG[12], DONESIG[4], userName[10][20], appointments[1000][255],RESCHEDULE[20],INITSIG[20];
int person_ac_num[10];
int numAppointments;
int numUser;
int cmd[10][2], rsp[10][2];
char fileName[30];
int data[6];
int rejectTable[1000];
int i, j, k;
int numFile = 0;
double reject_num,accept_num;
double timeu[10];
int ac_people[40][25];
char Pappointments[1000][255];
int numONE = 0, numTWO = 0, numTHREE = 0, numFOUR = 0, numFIVE = 0;
char Lappointments[1000][255];
void reschedule(char app[][255],char keyword[]);
void setData(char buffCopy[], int n, int arr[]); 
int all = 0;
int weekend[31]; 
int day[] = {1,26,7,14,21,28};

char *getFilename(char line[])
{
    char *current;
    int count = 0;
    current = strtok(line, " ");
    while (current != NULL) {
        count++;
        if (count == 2) {
            return current;
        }
        current = strtok(NULL, " ");
    }
     
}

void newsort(){
	int data[6];
    int value;
    numONE = 0, numTWO = 0, numTHREE = 0, numFOUR = 0, numFIVE = 0;
    memset(Lappointments,'0',sizeof(char)*1000*255);
    int num1,num2,num3,num4,num5;
    for (i = 0; i < numAppointments; ++i)
    {
        memset(data, 0, sizeof(data));
        setData(appointments[i], strlen(appointments[i]), data);
        value = data[4];
        if (value == 1)
        {
            numONE++;
        }
        else if (value == 2)
        {
            numTWO++;
        }
        else if (value == 3)
        {
            numTHREE++;
        }
        else if (value == 4)
        {
            numFOUR++;
        }
        else if (value == 5)
        {
            numFIVE++;
        }
    }
    int slot5 = 0;
    int slot4 = numFIVE;
    int slot3 = slot4 + numFOUR;
    int slot2 = slot3 + numTHREE;
    int slot1 = slot2 + numTWO;
    
    for (i = 0; i < numAppointments; ++i)
    {
        memset(data, 0, sizeof(data));
        setData(appointments[i], strlen(appointments[i]), data);
        value = data[4];
        if (value==1)
        {
            strcpy(Lappointments[slot1], appointments[i]);
            slot1++;
        }
        else if (value==2)
        {
            strcpy(Lappointments[slot2], appointments[i]);
            slot2++;
        }
        else if (value==3)
        {
            strcpy(Lappointments[slot3], appointments[i]);
            slot3++;
        }
        else if (value==4)
        {
            strcpy(Lappointments[slot4], appointments[i]);
            slot4++;
        }
        else if (value==5)
        {
            strcpy(Lappointments[slot5], appointments[i]);
            slot5++;
        }
    }
}

void prioritySort(){
    
    memset(Pappointments, 0, sizeof(Pappointments));
    int numPrivate = 0, numGroupstudy = 0, numGathering = 0, numProjectMeeting = 0;
    for (i = 0; i < numAppointments; ++i)
    {
        if (appointments[i][3] == 'v')
        {
            numPrivate++;
        }
        else if (appointments[i][3] == 'j')
        {
            numProjectMeeting++;
        }
        else if (appointments[i][3] == 'u')
        {
            numGroupstudy++;
        }
        else if (appointments[i][3] == 'h')
        {
            numGathering++;
        }
    }
    int ptrPrivate = 0;
    int ptrProjectMeeting = numPrivate;
    int ptrGroupStudy = ptrProjectMeeting + numProjectMeeting;
    int ptrGathering = ptrGroupStudy + numGroupstudy;
    for (i = 0; i < numAppointments; ++i)
    {
        if (appointments[i][3] == 'v')
        {
            strcpy(Pappointments[ptrPrivate], appointments[i]);
            ptrPrivate++;
        }
        else if (appointments[i][3] == 'j')
        {
            strcpy(Pappointments[ptrProjectMeeting], appointments[i]);
            ptrProjectMeeting++;
        }
        else if (appointments[i][3] == 'u')
        {
            strcpy(Pappointments[ptrGroupStudy], appointments[i]);
            ptrGroupStudy++;
        }
        else if (appointments[i][3] == 'h')
        {
            strcpy(Pappointments[ptrGathering], appointments[i]);
            ptrGathering++;
        }
    }
}

void init()
{
	
    memset(appointments, 0, sizeof(appointments));
    numAppointments = 0;
    numUser = 0;
    strcpy(commands[1], "endProgram");
    strcpy(commands[2], "printSchd");
    strcpy(commands[3], "priority");
    strcpy(commands[4], "privateTime");
    strcpy(commands[5], "projectMeeting");
    strcpy(commands[6], "groupStudy");
    strcpy(commands[7], "gathering");
    strcpy(commands[8], "file");

    strcpy(fileTypes[0], "FCFS");
    strcpy(fileTypes[1], "Priority");
    strcpy(fileTypes[2], "NEW");
    strcpy(fileTypes[3], "ALL");
    strcpy(fileTypes[4], "NEW0");

    strcpy(fileNames[0], "G01_XX_FCFS.txt");
    strcpy(fileNames[1], "G01_XX_PRIORITY.txt");
    strcpy(fileNames[2], "G01_XX_NEW.txt");
    strcpy(fileNames[3], "G01_XX_ALL.txt");
    strcpy(fileNames[4], "G01_XX_NEW0.txt");

    strcpy(PRINTACCEPTSIG, "printAccept");
    strcpy(INITSIG, "initialization");
    strcpy(REJECTSIG, "reject");
    strcpy(ACCEPTSIG, "accept");
    strcpy(DONESIG, "done");
    strcpy(KILLSIG, "kill");
    memset(weekend,0,sizeof(int)*31);
    for(i=0;i<=5;i++){
    	weekend[day[i]]=1;
	}
    
}

int checkCommand(char buffCopy[], int n)
{
    char tmp1[255];
    memset(tmp1, 0, sizeof(tmp1));
    int i, j;
    for (i = 0; i <= n; ++i)
    {
        if (buffCopy[i] == ' ' || i == n)
        {
            if (i == n)
                strncpy(tmp1, buffCopy, i + 1);
            else
                strncpy(tmp1, buffCopy, i);
            for (j = 1; j < 9; ++j)
            {
                if (strcmp(tmp1, commands[j]) == 0)
                    return j;
            }
            return -1;
        }
    }
}

int getUserID(char name[])
{
    int i;
    if (name[0]<='z'&&name[0]>='a') name[0]-=32;
    for(j=1;j<strlen(name);j++){
    	if (name[j]<='Z'&&name[j]>='A') name[j]+=32;
	}
    for (i = 0; i < numUser; ++i)
    {
        if (strcmp(name, userName[i]) == 0)
            return i;
    }
}

void setData(char buffCopy[], int n, int arr[])
{
    int pre = -1; // the index of the previous space
    int pos = 0;  // the position of the current data in the command line (eg. pos=0 means current information is for 'command type')
    char tmp[255];
    int i;
    for (i = 0; i <= n; ++i)
    {
        if (buffCopy[i] == ' ' || i == n)
        {
            memset(tmp, 0, sizeof(tmp));
            if (i == n)
                strncpy(tmp, buffCopy + pre + 1, i - pre);
            else
                strncpy(tmp, buffCopy + pre + 1, i - pre - 1);
            if (pos == 0)
                arr[0] = checkCommand(buffCopy, n); // appointment type
            else if (pos == 1)
            {
                arr[1] = getUserID(tmp); // caller userID
                arr[5] = 1 << arr[1];
            }
            else if (pos == 2)
                arr[2] = atoi(tmp) % 100; // appointment date
            else if (pos == 3)
                arr[3] = atoi(tmp) / 100; // appointment start time
            else if (pos == 4)
                arr[4] = (int)(atof(tmp)); // appointment duration
            else if (pos >= 5)
            {
                arr[5] = arr[5] + (1 << (getUserID(tmp))); // a binary number to store the callees (eg. 5=101 means callees are user0 and user2)
            }
            pos++;
            pre = i;
        }
    }
}

void rejectCollect(int rejectTable[], int numAppointments, int numReject, char appointments[1000][255], char algorithm)
{
	reject_num=0;
    int i = 0, j = 0;
    int type, date, people, userID, startTime, duration, count = 1;
    FILE *rejectFile = NULL;
    char fileName[30];
    memset(fileName, 0, sizeof(fileName));
//    numFile++;
    if (algorithm == 'F')
    {
        strcpy(fileName, "G01_XX_FCFS_rejected.dat");
//		strcpy(fileName, "G01_rejected.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10);
        printf("-> [Exported file: %s]\n", fileName);
        rejectFile = fopen(fileName, "w+");
        if (rejectFile == NULL)
        {
            printf("Open file failed\n");
            exit(1);
        }
    }
    else if (algorithm == 'P')
    {
        strcpy(fileName, "G01_XX_Priority_rejected.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10);
        printf("-> [Exported file: %s]\n", fileName);
        rejectFile = fopen(fileName, "w+");
        if (rejectFile == NULL)
        {
            printf("Open file failed\n");
            exit(1);
        }
    }
    else if (algorithm == 'A')
    {
        strcpy(fileName, "G01_XX_All_rejected.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10);
        
		if (rejectFile = fopen(fileName, "r")) 
        {
            fclose(rejectFile);  
        }
        else{
			printf("-> [Exported file: %s]\n", fileName);
		}rejectFile = fopen(fileName, "a");
    }
    else if (algorithm == 'N')
    {
        strcpy(fileName, "G01_XX_NEW_rejected.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10);
        printf("-> [Exported file: %s]\n", fileName);
        rejectFile = fopen(fileName, "w+");
        if (rejectFile == NULL)
        {
            printf("Open file failed\n");
            exit(1);
        }
    }

    fprintf(rejectFile, "***Rejected List***\n");
	if (all!=0){
	    if (all==1) fprintf(rejectFile, "Algorithm used: FCFS\n\n");
	    else if (all==2) fprintf(rejectFile, "Algorithm used: Priority\n\n");
	    else if (all==3) fprintf(rejectFile, "Algorithm used: NEW\n\n");
	}
    fprintf(rejectFile, "Altogether there are %d appointments rejected\n", numReject);
    fprintf(rejectFile, "=========================================================================\n");
    for (i = 0; i < numAppointments; i++)
    {
        if (rejectTable[i] != 0)
        {
        	reject_num++;
            fprintf(rejectFile, "%d. %s\n", count++, appointments[i]);
            fflush(rejectFile);
        }
    }
    fprintf(rejectFile, "=========================================================================\n");
    fprintf(rejectFile, "                            - End of Rejected List  -\n");
    fclose(rejectFile);
}



void performance(char algorithm){
	FILE *performancefile = NULL;
	char fileName[30];
    if (algorithm == 'F')
    {
        strcpy(fileName, "G01_XX_FCFS_performance.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10); 
        performancefile = fopen(fileName, "w+");
        
        if (performancefile == NULL)
        {
            printf("Open file failed\n");
            exit(1);
        }printf("-> [Exported file: %s]\n", fileName);
    }
    else if (algorithm == 'P')
    {
        strcpy(fileName, "G01_XX_Priority_performance.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10);
        performancefile = fopen(fileName, "w+");
        
        if (performancefile == NULL)
        {
            printf("Open file failed\n");
            exit(1);
        }printf("-> [Exported file: %s]\n", fileName);
    }
    else if (algorithm == 'A')
    {
        strcpy(fileName, "G01_XX_All_performance.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10);
        if (performancefile = fopen(fileName, "r")) 
        {
            fclose(performancefile);  
        }
        else{
			printf("-> [Exported file: %s]\n", fileName);
		}
        performancefile = fopen(fileName, "a");
    }
    else if (algorithm == 'N')
    {
        strcpy(fileName, "G01_XX_NEW_performance.dat");
        fileName[4] = '0' + numFile / 10;
        fileName[5] = '0' + (numFile % 10);
        performancefile = fopen(fileName, "w+");
        
        if (performancefile == NULL)
        {
            printf("Open file failed\n");
            exit(1);
        }printf("-> [Exported file: %s]\n", fileName);
    }

    fprintf(performancefile, "***  Performance  ***\n\n");
	if (all!=0){
	    if (all==1) fprintf(performancefile, "Algorithm used: FCFS\n\n");
	    else if (all==2) fprintf(performancefile, "Algorithm used: Priority\n\n");
	    else if (all==3) fprintf(performancefile, "Algorithm used: NEW\n\n");
	}
    fprintf(performancefile,"Total Number of Requests Received: %d\n      Number of Requests Accepted: %.0lf (%.2lf%)\n      Number of Requests Rejected: %.0lf (%.2lf\%)\n\n",numAppointments, accept_num, accept_num/numAppointments*100, reject_num, reject_num/numAppointments*100 );
    fprintf(performancefile,"Number of Requests Accepted by Individual:\n");
    for(i =0; i< numUser;i++){
    	fprintf(performancefile,"%s\t\t-%d\n",userName[i],person_ac_num[i]);
	}
	fprintf(performancefile,"Utilization of Time Slot:\n");
    for(i =0; i< numUser;i++){
    	fprintf(performancefile,"%s\t\t-%.2lf\%\n",userName[i],timeu[i]/155*100);
	}
	
    fclose(performancefile);
    
}

void communication(char app[][255], int type, char keyWords[],int num)
{
	memset(ac_people,0,sizeof(int)*40*5);
	memset(timeu,0,sizeof(double)*10);
	memset(person_ac_num,0,sizeof(int)*10);
	accept_num=0;
    for (i = 0; i < numUser; ++i)
    {
        write(cmd[i][1], fileTypes[type], strlen(fileTypes[type]));
    }
    for (i = 0; i < numUser; ++i)
    {
        memset(buff, 0, sizeof(buff));
        int n = read(rsp[i][0], buff, 255);
        if (n <= 0)
        {
            printf("pipe error\n");
            exit(1);
        }
    }
	numAppointments = num;
    memset(rejectTable, 0, sizeof(rejectTable));
    int rejectCount = 0;
    for (i = 0; i < numAppointments; ++i)
    {
        memset(data, 0, sizeof(data));
        setData(app[i], strlen(app[i]), data);
        // check time table
        for (j = 0; j < numUser; ++j)
        {
            if (data[5] & (1 << j))
            {
                write(cmd[j][1], app[i], strlen(app[i]));
            }
        }
        int rejectFlag = 0;
        for (j = 0; j < numUser; ++j)
        {
            if (data[5] & (1 << j))
            {
                memset(buffRsp, 0, sizeof(buffRsp));
                int n = read(rsp[j][0], buffRsp, 255);
                buffRsp[n] = 0;
                if (n <= 0)
                {
                    printf("pipe error\n");    
                    exit(1);
                }
                if (strcmp(buffRsp, REJECTSIG) == 0)
                {
                    rejectFlag = 1;
                }
            }
        }
        if (rejectFlag == 1)
        {
            rejectCount++;
            rejectTable[i] = 1;
            for (j = 0; j < numUser; ++j)
            {
                if (data[5] & (1 << j))
                {
                    write(cmd[j][1], REJECTSIG, strlen(REJECTSIG));
                }
            }
            for (j = 0; j < numUser; ++j)
            {
                if (data[5] & (1 << j))
                {
                    memset(buffRsp, 0, sizeof(buffRsp));
                    int n = read(rsp[j][0], buffRsp, 255);
                    if (n <= 0)
                    {
                        printf("pipe error\n");
                        exit(1);
                    }
                }
            }
            continue;
        }
		accept_num++;
        // record accepted appointment
        for (j = 0; j < numUser; ++j)
        {
            if (data[5] & (1 << j))
            {
                write(cmd[j][1], ACCEPTSIG, strlen(ACCEPTSIG));
                person_ac_num[j]++;
                timeu[j]=timeu[j]+data[4];
            }
        }
        
        for (j = 0; j < numUser; ++j)
        {
            if (data[5] & (1 << j))
            {
                memset(buffRsp, 0, sizeof(buffRsp));
                int n = read(rsp[j][0], buffRsp, 255);
                if (n <= 0)
                {
                    printf("pipe error\n");
                    exit(1);
                }
            }
        }

        int date = data[2];
        int startTime = data[3];
        int duration = data[4];
        for (j = startTime; j < startTime + duration; ++j)
            ac_people[date][j] += data[5];

    }
    if (strcmp(keyWords,"NEW0")!=0){
	    memset(fileName, 0, sizeof(fileName));
	    if (type!=3&&type!=4){
	    	numFile++;
		}
	    
	    strcpy(fileName, fileNames[type]);
	    fileName[4] = '0' + numFile / 10;
	    fileName[5] = '0' + (numFile % 10);
	    
	    if (type!=3){
	    	FCFSfile = fopen(fileName, "w+");
	    	
	    	if (FCFSfile == NULL)
			{
	            printf("Open file failed\n");
	            exit(1);
	        }printf("-> [Exported file: %s]\n", fileName);
		}else {
			if (FCFSfile = fopen(fileName, "r")) 
            {
                fclose(FCFSfile);  
            }
            else{
			    printf("-> [Exported file: %s]\n", fileName);
	    	}
			FCFSfile = fopen(fileName, "a");			
		}
	    

	    fprintf(FCFSfile, "Period: %d-%d%d-%d%d to %d-%d%d-%d%d\n", globalStartTime / 10000, ((globalStartTime % 10000) / 100) / 10, ((globalStartTime % 10000) / 100) % 10, (globalStartTime % 100) / 10, (globalStartTime % 100) % 10, globalEndTime / 10000, ((globalEndTime % 10000) / 100) / 10, ((globalEndTime % 10000) / 100) % 10, (globalEndTime % 100) / 10, (globalEndTime % 100) % 10);
	    if (all!=0){
	    	if (all==1) fprintf(FCFSfile, "Algorithm used: FCFS\n\n");
	    	else if (all==2) fprintf(FCFSfile, "Algorithm used: Priority\n\n");
	    	else if (all==3) fprintf(FCFSfile, "Algorithm used: NEW\n\n");
		}else fprintf(FCFSfile, "Algorithm used: %s\n\n", keyWords);
		
	    fprintf(FCFSfile, "***Appointment Schedule***\n\n");
	    fflush(FCFSfile);
	    fclose(FCFSfile);
	    for (i = 0; i < numUser; ++i)
	    {
	        write(cmd[i][1], PRINTACCEPTSIG, strlen(PRINTACCEPTSIG));
	        memset(buff, 0, sizeof(buff));
	        int n = read(rsp[i][0], buff, 255);
	        buff[n] = 0;
	        if (n <= 0)
	        {
	            printf("pipe error\n");
	            exit(1);
	        }
	    }
	    
	    if(type == 0){
	        rejectCollect(rejectTable, numAppointments, rejectCount, app, 'F');
	        performance('F');	    	
		}

	    else if(type == 1){
	        rejectCollect(rejectTable, numAppointments, rejectCount, app, 'P');
	        performance('P');	    	
		}
		else if (type ==2){
	        rejectCollect(rejectTable, numAppointments, rejectCount, app, 'N');
	        performance('N');
		}
		else if (type ==3){
	        rejectCollect(rejectTable, numAppointments, rejectCount, app, 'A');
	        performance('A');
		}
	    
	}else {
		for (i = 0; i < numUser; ++i)
	    {
	        write(cmd[i][1], INITSIG, strlen(INITSIG));
	    }
		for (i = 0; i < numUser; ++i)
	    {
	        memset(buff, 0, sizeof(buff));
	        int n = read(rsp[i][0], buff, 255);
	        buff[n] = 0;
	        if (n <= 0)
	        {
	            printf("pipe error\n");
	            exit(1);
	        }
	    }
	}

    
}

void reschedule(char app[][255],char keywords[]){
	int k,i,j;
	int data[6];
	char buffRsp[255];
	for(k=0;k<numAppointments;k++)
	{
		if (rejectTable[k]!=0)
		{
			memset(data, 0, sizeof(data));
			setData(app[k], strlen(app[k]), data);
			for(i=1;i<=31;i++)
			{
				int count = 0;
				int found = 0;
				for(j=18;j<=23;j++)
				{
					if(count == data[4])
					{
						found = 1;
						break;
					}
					if (((ac_people[i][j]&data[5])==0)&&(weekend[i]==0))
						count++;
					else 
						count = 0;
				}
				if (found == 1)
					break;
			}
			if (i<=31)
			{
				rejectTable[k]=0;
				int space = 0;
				int t;
				for(t=0;t<strlen(app[k]);t++)
				{
					if (app[k][t]==' ')
						space ++;
					if (space ==3) 
						break;
				}

				app[k][t-1]='0'+i%10;
				app[k][t-2]='0'+i/10;
				app[k][t+1]='0'+(j-data[4])/10;
				app[k][t+2]='0'+(j-data[4])%10;
				int date = i;
		        int startTime = j-data[4];
		        int duration = data[4];
        		for (j = startTime; j < startTime + duration; ++j){
        			ac_people[date][j] += data[5];
				}
            		
			}
		}
	}
	if (strcmp(keywords,"NEW0")!=0){
		for(i =0;i<3;i++){
			if (strcmp(keywords,fileTypes[i])==0){
				communication(app, i, keywords,numAppointments);
				break;
			}
		}
		
	}
	
}

int main(int argc, char *argv[])
{
    if (argc < 6 || argc > 13)
    {
        printf("The number of users is out of range (not between 3 and 10)\n");
        return 0;
    }
    // initialize some data
    init();
    // get start-time, end-time and user-information from the arguments
    globalStartTime = atoi(argv[1]);
    globalEndTime = atoi(argv[2]);
    int i, j;
    for (i = 3; i < argc; ++i)
    {
    	if (argv[i][0]<='z'&&argv[i][0]>='a') argv[i][0]-=32;
    	for(j=1;j<strlen(argv[i]);j++){
    		if (argv[i][j]<='Z'&&argv[i][j]>='A') argv[i][j]+=32;
		}
        strcpy(userName[numUser], argv[i]);
        numUser++;
    }
    printf("~~WELCOME TO APO~~\n");
    // create pipe
    // pipe cmd[i] is used to transmit command from parent to children (parent is write-only and children are read-only)
    // pipe rsp[1] is used to show the responce of each child (parent is read-only and children are write-only)
    for (i = 0; i < numUser; ++i)
    {
        if (pipe(cmd[i]) < 0 || pipe(rsp[i]) < 0)
        {
            printf("User %d Pipe Failed\n", i + 1);
            exit(1);
        }
    }
    int fPID, userID;
    // create fork for each user(child)
    for (userID = 0; userID < numUser; ++userID)
    {
        fPID = fork();
        // only parent can fork to create child process
        // if it's a child process just break and do its job following this for-loop
        if (fPID < 0 || fPID == 0)
            break;
    }

    if (fPID < 0)
    {
        printf("User %d Fork Failed\n", i + 1);
        exit(1);
    }
    else if (fPID == 0)
    {
        close(cmd[userID][1]); // read-only for command
        close(rsp[userID][0]); // write-only for response

        FILE *outfile = NULL;
        int flag = 0;
        int numAccept = 0, numUserApp = 0;
        int data[6], allAppointments[1000][6], timeTable[40][25], acceptAppointments[1000];
        numFile = 1;
        memset(fileName, 0, sizeof(fileName));
        memset(allAppointments, 0, sizeof(allAppointments));
        memset(timeTable, -1, sizeof(timeTable));
        memset(acceptAppointments, 0, sizeof(acceptAppointments));
        
        memset(person_ac_num,0,10*sizeof(int));
        memset(timeu,0,10*sizeof(double));
        all = 0; 
        
        while (1)
        {
            memset(data, 0, sizeof(data));
            memset(buff, 0, sizeof(buff));
            int n = read(cmd[userID][0], buff, 255);
            if (n <= 0)
            {
                printf("pipe error\n");
                exit(1);
            }
            buff[n] = 0;
            if (strcmp(buff, KILLSIG) == 0)
            {
            	
                close(cmd[userID][0]);
                close(rsp[userID][1]);
                exit(0);
            }
            for (i = 0; i <=4; ++i)
            {
                if (strcmp(buff, fileTypes[i]) == 0)
                {
                    flag = 1;
                    strcpy(fileName, fileNames[i]);
                    fileName[4] = '0' + numFile / 10;
                    fileName[5] = '0' + (numFile % 10);
                    write(rsp[userID][1], DONESIG, strlen(DONESIG));
                    if (i!=3&&i!=4) numFile+=1;
                    if (i==3) all++;
                    if (all==3) {
                    	all=0;
                    	numFile++;
					}
                    break;
                }
            }
            if (flag == 1)
            {
                flag = 0;
                continue;
            }if (strcmp(buff,INITSIG)==0){
                flag = 0;
                numAccept = 0;
                numUserApp = 0;
                memset(fileName, 0, sizeof(fileName));
                memset(allAppointments, 0, sizeof(allAppointments));
                memset(timeTable, -1, sizeof(timeTable));
                memset(acceptAppointments, 0, sizeof(acceptAppointments));
                
		        memset(person_ac_num,0,10*sizeof(int));
		        memset(timeu,0,10*sizeof(double));
		        write(rsp[userID][1], DONESIG, strlen(DONESIG));
                
            	continue;
			}
			
            if (strcmp(buff, PRINTACCEPTSIG) == 0)
            {
                outfile = fopen(fileName, "a");
                if (outfile == NULL)
                {
                    printf("Open file failed\n");
                    exit(1);
                }
                fprintf(outfile, ". %s, you have %d appointments.\n", userName[userID], numAccept);
                fprintf(outfile, "Date          Start    End      Type               People\n");
                fprintf(outfile, "====================================================================================\n");
                fflush(outfile);
				int previous = -1;
				for(i=1;i<=31;i++){
					for(j=18;j<=22;j++){
						if (timeTable[i][j]!=-1){
							if (timeTable[i][j]!=previous){
								previous = timeTable[i][j];
								int type = allAppointments[timeTable[i][j]][0];
								int day = allAppointments[timeTable[i][j]][2];
								int year = globalStartTime / 10000;
								int month = (globalStartTime % 10000) / 100;
								int startTime = allAppointments[timeTable[i][j]][3];
								int endTime = startTime + allAppointments[timeTable[i][j]][4];
								int people = allAppointments[timeTable[i][j]][5];
								fprintf(outfile, "%d-%d%d-%d%d    %d:00    %d:00    %-15s    ", year, month / 10, month % 10, day / 10, day % 10, startTime, endTime, commands[type]);
								fflush(outfile);
								int t;
			                    for (t = 0; t < numUser; ++t)
			                    {
			                        if (t != userID && (people & (1 << t)))
			                        {
			                            fprintf(outfile, "%s ", userName[t]);
			                            fflush(outfile);
			                        }
			                    }
			                    fprintf(outfile, "\n");
			                    fflush(outfile);
							}

						}
					}
				}
                fprintf(outfile, "                            - End of %s's Schedule -\n", userName[userID]);
                fflush(outfile);
                fclose(outfile);

                flag = 0;
                numAccept = 0;
                numUserApp = 0;
                memset(fileName, 0, sizeof(fileName));
                memset(allAppointments, 0, sizeof(allAppointments));
                memset(timeTable, -1, sizeof(timeTable));
                memset(acceptAppointments, 0, sizeof(acceptAppointments));
		        memset(person_ac_num,0,10*sizeof(int));
		        memset(timeu,0,10*sizeof(double));

                write(rsp[userID][1], DONESIG, strlen(DONESIG));
                continue;
            }
            setData(buff, n, data);
            for (i = 0; i < 6; ++i)
                allAppointments[numUserApp][i] = data[i];
            numUserApp++;
            int date = data[2];
            int startTime = data[3];
            int duration = data[4];
            int rejectFlag = 0;
            for (i = startTime; i < startTime + duration; ++i)
            {
                if (weekend[date]==1||timeTable[date][i] !=-1 )
                {
                    rejectFlag = 1;
                    break;
                }
            }

            if (rejectFlag == 1)
            {
                write(rsp[userID][1], REJECTSIG, strlen(REJECTSIG));
            }
            else
            {
                write(rsp[userID][1], ACCEPTSIG, strlen(ACCEPTSIG));
            }

            memset(buff, 0, sizeof(buff));
            n = read(cmd[userID][0], buff, 255);
            if (n <= 0)
            {
                printf("pipe error\n");
                exit(1);
            }
            if (strcmp(buff, REJECTSIG) == 0)
            {
                write(rsp[userID][1], DONESIG, strlen(DONESIG));
                continue;
            }
            // record
            for (i = startTime; i < startTime + duration; ++i){
            	timeTable[date][i] = numUserApp-1;
            	person_ac_num[userID]++;
			}
                
            acceptAppointments[numAccept] = numUserApp - 1;
            numAccept++;
            write(rsp[userID][1], DONESIG, strlen(DONESIG));
        }
        
    }
    else
    {
        requestFile = fopen("./All_Requests.dat", "w+");
        if (requestFile == NULL)
        {
            printf("Open File Failed\n");
            return 0;
        }
        for (i = 0; i < numUser; ++i)
        {
            close(cmd[i][0]); // write-only for command
            close(rsp[i][1]); // read-only for response
        }
        while (1)
        {
            printf("Please enter appointment:\n");
            memset(buff, 0, sizeof(buff));
            int n = read(STDIN_FILENO, buff, 255);
            if (n <= 0)
            {
                printf("invalid input\n");
                continue;
            }
            // clear the newline character
            if (buff[n-1]=='\n'){
            	
			}
            buff[--n] = 0;
            int ret = checkCommand(buff, n);
            if (ret == 1) // end Program
            {
                for (i = 0; i < numUser; ++i)
                {
                    write(cmd[i][1], KILLSIG, strlen(KILLSIG));
                }
                for(i=0;i<numUser;i++){
        	        wait(NULL);
        	        close(cmd[i][1]);
        	        close(rsp[i][0]);
        	        
		        }
                printf("-> Bye!\n");
                break;
            }
            else if (ret > 3 && ret <8)
            {
                // record schedule
                printf("-> [Recorded]\n");
                fprintf(requestFile, "%s\n", buff);
                fflush(requestFile);
                strcpy(appointments[numAppointments], buff);
                numAppointments++;
            }
            else if (ret == 8)
			{
            	char* path = getFilename(buff);
            	char line[255];
            	FILE *file = fopen(path, "r");
				if (!file)
			    {
			        perror(path);
			        return EXIT_FAILURE;
			    }
            	int j,k = 0;
            	memset(line, 0, sizeof(line));
                while(fgets(line, 255, file)) 
				{
					if (line[strlen(line) - 1] =='\n'){
						line[strlen(line) - 1] = 0;
					}
	                fprintf(requestFile, "%s\n", line);
	                fflush(requestFile);
	                strcpy(appointments[numAppointments], line);
	                numAppointments++;
	                printf("-> [Recorded]\n");
	                memset(line, 0, sizeof(line));
				}
				fclose(file);
			}
            else if (ret == 2)
            {
                // printSchd
                if (buff[10] == 'F') // FCFS
                {
                	char copyapp[1000][255];
                	int i,j;
                	for(i=0;i<numAppointments;i++){
                		strcpy(copyapp[i],appointments[i]);
					}
					
                    communication(copyapp, 0, "FCFS",numAppointments);
                    reschedule(copyapp,"FCFS");
                    
                }
                else if (buff[10] == 'P') // Priority
                {
					prioritySort();
                    memset(rejectTable, 0, sizeof(rejectTable));
					communication(Pappointments, 1, "Priority",numAppointments);
					reschedule(Pappointments,"Priority");
                }
                else if (buff[10] == 'N') //new algorithm
                {
					//5
					newsort();
					communication(Lappointments, 4, "NEW0",numFIVE);
					reschedule(Lappointments,"NEW0");
					//4
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR);
					reschedule(Lappointments,"NEW0");
					//3
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR+numTHREE);
					reschedule(Lappointments,"NEW0");					
					//2
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR+numTHREE+numTWO);
					reschedule(Lappointments,"NEW0");
					//1
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR+numTHREE+numTWO+numONE);
					reschedule(Lappointments,"NEW");
                }
                    
                else if (buff[10] == 'A') // All
                {
                	// fcfs
                	numFile++;
                	all=1;
                    communication(appointments, 3, "ALL",numAppointments);
                    all=2;
					prioritySort();
                    communication(Pappointments, 3, "ALL",numAppointments);
                    all=3;
                    newsort();
					communication(Lappointments, 4, "NEW0",numFIVE);
					reschedule(Lappointments,"NEW0");
					//4
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR);
					reschedule(Lappointments,"NEW0");
					//3
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR+numTHREE);
					reschedule(Lappointments,"NEW0");					
					//2
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR+numTHREE+numTWO);
					reschedule(Lappointments,"NEW0");
					//1
					communication(Lappointments, 4, "NEW0",numFIVE+numFOUR+numTHREE+numTWO+numONE);
					reschedule(Lappointments,"NEW0");
					communication(Lappointments, 3, "ALL",numFIVE+numFOUR+numTHREE+numTWO+numONE);
					all=0;
                }
            }
            else
            {
                printf("invalid input\n");
            }
        }
        fclose(requestFile);
    }
    return 0;
}
