#include <stdio.h>  
#include <stdlib.h> 
#include <math.h>

int findMinDistance(bool *isVisited, int *request, int numRequest, int fromValue)
{
    int i, min_index, min = 0x7FFFFFFF;
    for (i = 0; i < numRequest; i++)
    {
        if (!isVisited[i] && abs(fromValue - request[i]) < min)
        {
            min_index = i;
            min = abs(fromValue - request[i]);
        }
    }
    isVisited[min_index] = true;
    return request[min_index];
}

void FCFS(int *request, int numRequest, int beginningTrack)
{
    int i, prePosition, totalDistance = 0;
    printf("FCFS\n");
    prePosition = beginningTrack;
    printf("%d", prePosition);
    for (i = 0; i < numRequest; i++)
    {
        // printf(" -> %d", request[i]);
        totalDistance += abs(prePosition - request[i]);
        prePosition = request[i];
    }
    printf("\n%d\n", totalDistance);
}

void SSTF(int *request, int numRequest, int beginningTrack)
{
    int i, prePosition, curPosition, totalDistance = 0;
    bool *isVisited = new bool[numRequest];
    for (i = 0; i < numRequest; i++)
    {
        isVisited[i] = false;
    }
    printf("SSTF\n");
    prePosition = beginningTrack;
    printf("%d", prePosition);
    for (i = 0; i < numRequest; i++)
    {
        curPosition = findMinDistance(isVisited, request, numRequest, prePosition);
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }
    printf("\n%d\n", totalDistance);
}

int SCAN(int *request, int numRequest, int beginningTrack)
{
    int i, j, temp, prePosition, curPosition, totalDistance = 0, left_nearest_index = 0;
    printf("SCAN\n");
    prePosition = beginningTrack;
    printf("%d", prePosition);
    // sort the request
    for (i = 0; i < numRequest; i++)
    {
        // printf("request is %d left is %d\n", request[i], left_nearest_index);
        for (j = i; j < numRequest; j++)
        {
            if (i == 0 && request[j] <= beginningTrack)
            {
                left_nearest_index++;
            }
            if (request[i] > request[j])
            {
                temp = request[i];
                request[i] = request[j];
                request[j] = temp;
            }
        }
    }
    for (i = left_nearest_index - 1 ; i >= 0; i--)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }

    printf(" -> 0");
    totalDistance += abs(prePosition - 0);
    prePosition = 0;

    for (i = left_nearest_index ; i < numRequest; i++)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }
    printf("\n%d\n", totalDistance);
    return left_nearest_index;
}

//access the disk location in CSCAN
void CSCAN(int *request, int numRequest, int beginningTrack, int numTracks, int left_nearest_index)
{  
    int i, j, temp, prePosition, curPosition, totalDistance = 0;
    printf("C-SCAN\n");
    prePosition = beginningTrack;
    printf("%d", prePosition);

    for (i = left_nearest_index - 1 ; i >= 0; i--)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }

    /* Keep in mind that the huge jump doesn't count as a head movement. */
    printf(" -> 0");
    totalDistance += abs(prePosition - 0);
    // prePosition = 0;

    printf(" -> %d", numTracks - 1);
    // totalDistance += abs(prePosition - (numTracks - 1));
    prePosition = numTracks - 1;

    for (i = numRequest - 1; i >= left_nearest_index ; i--)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }
    printf("\n%d\n", totalDistance);
}

//access the disk location in LOOK
void LOOK(int *request, int numRequest, int beginningTrack, int left_nearest_index)
{
    int i, j, temp, prePosition, curPosition, totalDistance = 0;
    printf("LOOK\n");
    prePosition = beginningTrack;
    printf("%d", prePosition);

    for (i = left_nearest_index - 1 ; i >= 0; i--)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }

    for (i = left_nearest_index ; i < numRequest; i++)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }
    printf("\n%d\n", totalDistance);
}

//access the disk location in CLOOK
void CLOOK(int *request, int numRequest, int beginningTrack, int left_nearest_index)
{
    int i, j, temp, prePosition, curPosition, totalDistance = 0;
    printf("C-LOOK\n");
    prePosition = beginningTrack;
    printf("%d", prePosition);

    for (i = left_nearest_index - 1 ; i >= 0; i--)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }

    /* Keep in mind that the huge jump doesn't count as a head movement. */

    printf(" -> %d", request[numRequest - 1]);
    // totalDistance += abs(prePosition - (numTracks - 1));
    prePosition = request[numRequest - 1];

    for (i = numRequest - 2; i >= left_nearest_index ; i--)
    {
        curPosition = request[i];
        // printf(" -> %d", curPosition);
        totalDistance += abs(prePosition - curPosition);
        prePosition = curPosition;
    }
    printf("\n%d\n", totalDistance);
}

int main()
{
    int *request, beginningTrack, numRequest, numTracks, i, ans;
    int left_nearest_index;
    scanf("%d", &beginningTrack);
    scanf("%d", &numTracks);
    scanf("%d", &numRequest);
    request = new int[numRequest];

    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    //access the disk location using FCFS algorithm
    FCFS(request, numRequest, beginningTrack);

    //access the disk location using SSTF algorithm
    SSTF(request, numRequest, beginningTrack);

    //access the disk location using SCAN algorithm
    left_nearest_index = SCAN(request, numRequest, beginningTrack);

    // access the disk location using CSCAN algorithm
    CSCAN(request, numRequest, beginningTrack, numTracks, left_nearest_index);

    // access the disk location using LOOK algorithm
    LOOK(request, numRequest, beginningTrack, left_nearest_index);

    // access the disk location using CLOOK algorithm
    CLOOK(request, numRequest, beginningTrack, left_nearest_index);
    return 0;
}
