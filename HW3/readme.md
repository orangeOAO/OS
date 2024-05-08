# 作業系統 #HW3 Programming Projects
## Team Member
- 資工三 110590011 劉承軒 -
- 資工三 110590018 劉承翰 - Chap.7、9
- 資工三 110590056 林星主 - Chap.8、整理文件

## Chap.7 The Dining Philosophers Problem
### Description
- Pthread mutex locks and condition variables
### Code
```c=
#include <iostream>
#include <pthread.h>
#include <unistd.h>

const int NUM_PHILOSOPHERS = 5;

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];
pthread_cond_t conditions[NUM_PHILOSOPHERS];
pthread_mutex_t print_count;

enum State { THINKING, HUNGRY, EATING };

State states[NUM_PHILOSOPHERS];

void test(int i) 
{
    int left = i;
    int right = (i + 1) % NUM_PHILOSOPHERS;
    if (states[i] == HUNGRY &&
        states[left] != EATING &&
        states[right] != EATING) 
    {
        states[i] = EATING;
        pthread_cond_signal(&conditions[i]);
    }
}

void pickup(int i) {
    pthread_mutex_lock(&chopsticks[i]);
    states[i] = HUNGRY;
    test(i);
    while (states[i] != EATING) {
        pthread_cond_wait(&conditions[i], &chopsticks[i]);
    }
    pthread_mutex_unlock(&chopsticks[i]);
}

void putdown(int i) {
    pthread_mutex_lock(&chopsticks[i]);
    states[i] = THINKING;
    test((i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
    test((i + 1) % NUM_PHILOSOPHERS);
    pthread_mutex_unlock(&chopsticks[i]);
}

void* philosopher(void* num) {
    int i = *(int*) num;

    while (true) {
        pthread_mutex_lock(&print_count);
        std::cout << "Philosopher " << i << " is thinking.\n";
        pthread_mutex_unlock(&print_count);
        sleep(1);
        pickup(i);
        pthread_mutex_lock(&print_count);
        std::cout << "Philosopher " << i << " is eating.\n";
        pthread_mutex_unlock(&print_count);
        sleep(1);
        putdown(i);
    }
    return nullptr;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int thread_ids[NUM_PHILOSOPHERS];

    pthread_mutex_init(&print_count,nullptr);

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&chopsticks[i], nullptr);
        pthread_cond_init(&conditions[i], nullptr);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], nullptr, philosopher, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&chopsticks[i]);
        pthread_cond_destroy(&conditions[i]);
    }

    return 0;
}
```
![圖片](https://github.com/orangeOAO/OS/blob/main/HW3/Chap7.png?raw=true)


## Chap.8 Banker’s Algorithm
### Description
- Write a multithreaded program that implements the banker’s algorithm discussed in Section 8.6.3. This assignment combines three topics: (1) multithreading (2) preventing race conditions (3) deadlock avoidance.
- Create n customer threads that request and release resources from the bank. The customers will continually loop, requesting and then releasing random numbers of resources. The banker will grant a request if it satisfies the safety algorithm.
- Since multiple threads will concurrently access shared data, access must be controlled through mutex locks to prevent race conditions.
- You should invoke your program by passing the number of resources of each type on the command line.
### Input
- Formatting:
    - Resources:3
    - Processes:5
    - Example:
        ```
            3 3 2 // Available
            0 1 0 // Allocation
            2 0 0
            3 0 2
            2 1 1
            0 0 2
            7 5 3 // Max
            3 2 2
            9 0 2
            2 2 2
            4 3 3
        ```
- Test case:
    - safe.txt
        ```
            3 3 2
            0 1 0
            2 0 0
            3 0 2
            2 1 1
            0 0 2
            7 5 3
            3 2 2
            9 0 2
            2 2 2
            4 3 3
        ```
    - Unsafe.txt
        ```
            1 1 1
            0 1 0
            2 0 0
            3 0 2
            2 1 1
            0 0 2
            7 5 3
            3 2 2
            9 0 2
            2 2 2
            4 3 3
        ```
### Code
```c=
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// global variables delaration;
#define M 3 // no of resources
#define N 5 // no of processoses
int i = 0;
int j = 0;
pthread_mutex_t mutex;
int init[M];
int avail[M];
int allocmatrix[N][M];
int MaxMatrix[N][M];
int NeedMatrix[N][M];

void *procs(void *procsID)
{
    int pID = *(int *)procsID;
    // generate random requests
    sleep(1);
    int request[M];
    pthread_mutex_lock(&mutex);

    // NeedMatrix vector ko initialize karna
    for (i = 0; i < M; i++)
    {
        if (NeedMatrix[pID][i] != 0)
        {
            request[i] = rand() % NeedMatrix[pID][i];
        }
        else
        {
            request[i] = 0;
        }
    }

    printReqOrRelVector(request);

    getRes(pID, request);

    pthread_mutex_unlock(&mutex);

    // random release vector gernerate karne ke liye
    sleep(1);
    int releaseVector[M];
    pthread_mutex_lock(&mutex);

    // release vector ko initialize karna
    for (i = 0; i < M; i++)
    {
        if (allocmatrix[pID][i] != 0)
        {
            releaseVector[i] = rand() % allocmatrix[pID][i];
        }
        else
        {
            releaseVector[i] = 0;
        }
    }

    printReqOrRelVector(releaseVector);
    relRes(pID, releaseVector);
    pthread_mutex_unlock(&mutex);
}

int getRes(int pID, int request[])
{
    if (casegreaterthanneed(pID, request) == -1)
    {
        printf("Number of requested Resources is more than Needed.\n");
        return -1;
    }
    printf("Resources are being allocated...\n");

    if (enoughtoalloccase(request) == -1)
    {
        printf("Not enough Resources.\n");
        return -1;
    }

    for (i = 0; i < M; ++i)
    {
        NeedMatrix[pID][i] -= request[i];
        allocmatrix[pID][i] += request[i];
        avail[i] -= request[i];
    }
    printf("Checking is the state is safe...\n");

    if (safemodecase() == 0)
    {
        printf("\nx========================x\n|Safe Mode. Resources Allocated|\nx=========================x\n");
        exit(1);
        return 0;
    }
    else
    {
        printf("\nx=====================x\n|State is not safe.          |\nx=====================x\n");
        exit(1);
        return -1;
    }
}

int relRes(int pID, int releaseVector[])
{

    if (caseengoughtorel(pID, releaseVector) == -1)
    {
        printf("Not enought Resources.\n");
        return -1;
    }
    for (i = 0; i < M; i++)
    {
        allocmatrix[pID][i] -= releaseVector[i];
        NeedMatrix[pID][i] += releaseVector[i];
        avail[i] += releaseVector[i];
    }
    printf("Released.\nMetrix Available:\n");
    showavail();
    printf("Metrix Allocated:\n");
    Showalloc();
    printf("Metrix Need:\n");
    ShowNeed();
    return 0;
}

int caseengoughtorel(int pID, int releaseVector[])
{
    for (i = 0; i < M; ++i)
    {
        if (releaseVector[i] <= allocmatrix[pID][i])
        {
            continue;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

int casegreaterthanneed(int pID, int request[])
{

    for (i = 0; i < M; ++i)
    {
        if (request[i] <= NeedMatrix[pID][i])
        {
            continue;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

int enoughtoalloccase(int request[])
{

    for (i = 0; i < M; ++i)
    {
        if (request[i] <= avail[i])
        {
            continue;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

void ShowNeed()
{
    for (i = 0; i < N; ++i)
    {
        printf("{ ");
        for (j = 0; j < M; ++j)
        {
            printf("%d, ", NeedMatrix[i][j]);
        }
        printf("}\n");
    }
    return;
}

void Showalloc()
{
    for (i = 0; i < N; ++i)
    {
        printf("{ ");
        for (j = 0; j < M; ++j)
        {
            printf("%d, ", allocmatrix[i][j]);
        }
        printf("}\n");
    }
    return;
}

void showavail()
{
    for (i = 0; i < M; ++i)
    {
        printf("%d, ", avail[i]);
    }
    printf("\n");
    return;
}

void printReqOrRelVector(int vec[])
{
    for (i = 0; i < M; ++i)
    {
        printf("%d, ", vec[i]);
    }
    printf("\n");
    return;
}

int safemodecase()
{
    int finish[N] = {0};
    int work[M];

    for (i = 0; i < M; i++)
    {
        work[i] = avail[i];
    }

    int k;
    for (i = 0; i < N; i++)
    {
        if (finish[i] == 0)
        {
            for (j = 0; j < M; j++)
            {
                if (NeedMatrix[i][j] <= work[j])
                {

                    if (j == M - 1)
                    {
                        finish[i] = 1;
                        for (k = 0; k < M; ++k)
                        {
                            work[k] += allocmatrix[i][k];
                        }

                        i = -1;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            continue;
        }
    }

    for (i = 0; i < N; i++)
    {
        if (finish[i] == 0)
        {

            return -1;
        }
        else
        {
            continue;
        }
    }

    return 0;
}

int main()
{
    printf("Enter available VECTOR\n");

    for (i = 0; i < M; i++)
    {
        scanf("%d", &init[i]);
        avail[i] = init[i];
    }
    printf("Enter Allocation Matrix\n");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {

            scanf("%d", &allocmatrix[i][j]);
        }
    }
    printf("Enter MAX Matrix\n");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            scanf("%d", &MaxMatrix[i][j]);
        }
    }

    // NeedMatrix matrix ko initialize karna
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < M; ++j)
        {
            NeedMatrix[i][j] = MaxMatrix[i][j] - allocmatrix[i][j];
        }
    }

    printf("Available Vector is:\n");
    showavail();

    printf("Allocation Matrix is:\n");
    Showalloc();

    printf("Need Matrix is:\n");
    ShowNeed();

    pthread_mutex_init(&mutex, NULL);
    pthread_attr_t attrDefault;
    pthread_attr_init(&attrDefault);
    pthread_t *tid = malloc(sizeof(pthread_t) * N);

    int *pid = malloc(sizeof(int) * N); // process ID

    // initialize pid and create pthreads
    for (i = 0; i < N; i++)
    {
        *(pid + i) = i;
        pthread_create((tid + i), &attrDefault, procs, (pid + i));
    }

    // join threads
    for (i = 0; i < N; i++)
    {
        pthread_join(*(tid + i), NULL);
    }
    return 0;
}
```
![圖片](https://github.com/orangeOAO/OS/blob/main/HW3/Chap8.png?raw=true)

## Chap.9 Contiguous Memory Allocation
### Description
- In Section 9.2, we presented different algorithms for contiguous memory allocation.
- This project will involve managing a contiguous region of memory of size MAX where addresses may range from 0 ... MAX− 1. 
- Your program must respond to four different requests:
    1. Request for a contiguous block of memory
    2. Release of a contiguous block of memory
    3. Compact unused holes of memory into one single block
    4. Report the regions of free and allocated memory
- Your program will be passed the initial amount of memory at startup

### Code
```c=
#include <iostream>
#include <vector>
#include <algorithm>

struct Block {
    int start;
    int size;
    bool free;
};

class MemoryManager {
private:
    std::vector<Block> blocks;
    int totalSize;

public:
    MemoryManager(int size) : totalSize(size) {
        blocks.push_back({0, size, true});  // Start with one large free block
    }

    int allocate(int size) {
        for (size_t i = 0; i < blocks.size(); ++i) {
            if (blocks[i].free && blocks[i].size >= size) {
                int originalSize = blocks[i].size;
                int originalStart = blocks[i].start;
                blocks[i].size = size;
                blocks[i].free = false;

                if (originalSize > size) {
                    blocks.insert(blocks.end(), {originalStart + size, originalSize - size, true});
                }

                return blocks[i].start;
            }
        }

        return -1;  // Not enough memory
    }

    void free(int start) {
        for (auto &block : blocks) {
            if (block.start == start && !block.free) {
                block.free = true;
                merge();
                return;
            }
        }
    }

    void merge() {
    if (blocks.empty()) return;  // If no blocks, return early

    // First, sort blocks by starting address to ensure they are in order
    std::sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b) {
        return a.start < b.start;
    });

    // Create a new vector to store merged blocks
    std::vector<Block> mergedBlocks;
    mergedBlocks.push_back(blocks[0]);  // Start with the first block

    // Iterate over the blocks and merge adjacent free blocks
    for (int i = 1; i < blocks.size(); ++i) {
        Block& lastBlock = mergedBlocks.back();
        const Block& currentBlock = blocks[i];
        
        // If the last block in the merged list is free and adjacent to the current block, merge them
        if (lastBlock.free && currentBlock.free && (lastBlock.start + lastBlock.size == currentBlock.start)) {
            lastBlock.size += currentBlock.size;  // Extend the size of the last block
        } else {
            mergedBlocks.push_back(currentBlock);  // Otherwise, just add the current block
        }
    }

    // Replace old block list with the new merged list
    blocks = std::move(mergedBlocks);
}


    void compact() {
        int freeSpace = 0;
        blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
            [&freeSpace](const Block& b) {
                if (b.free) freeSpace += b.size;
                return b.free;
            }), blocks.end());
        if (freeSpace > 0) {
            blocks.push_back({totalSize - freeSpace, freeSpace, true});
            std::sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b) {
                return a.start < b.start;
            });
        }
    }

    void report() {
        for (const auto& block : blocks) {
            std::cout << "Block start at: " << block.start << ", size: " << block.size
                      << ", " << (block.free ? "Free" : "Allocated") << std::endl;
        }
    }
};

int main() {
    MemoryManager manager(1024);  // Example with 1024 units of memory

    int loc1 = manager.allocate(100);
    int loc2 = manager.allocate(200);
    int loc3 = manager.allocate(500);

    manager.report();
    manager.free(loc1);
    manager.compact();
    manager.report();

    return 0;
}
```
![圖片](https://github.com/orangeOAO/OS/blob/main/HW3/Chap9.png?raw=true)
