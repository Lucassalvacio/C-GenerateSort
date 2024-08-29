#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100
#define WIDTH 35
#define HEIGHT 35

void cls(){
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void hideCursor(){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}


int maze[HEIGHT][WIDTH];
int dir[4][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

void printMaze(){
	cls();
	hideCursor();
    for(int i = 0; i < HEIGHT; ++i){
        for(int j = 0; j < WIDTH; ++j){
            printf("%c", maze[i][j] == 0 ? 219 : ' ');
        }
        printf("\n");
    }
}

void carve(int x, int y){
    int d, i, j, x1, y1, count;
    
    while(1){
        count = 0;
        
        for(d = 0; d < 4; ++d){
            i = dir[d][0] * 2;
            j = dir[d][1] * 2;
            x1 = x + i;
            y1 = y + j;
            
            if(x1 > 0 && x1 < WIDTH && y1 > 0 && y1 < HEIGHT && maze[y1][x1] == 0){
                ++count;
            }
        }
        if(!count){
            return;
        }

        while(1){
            d = rand() % 4;
            i = dir[d][0] * 2;
            j = dir[d][1] * 2;
            x1 = x + i;
            y1 = y + j;
            
            if(x1 > 0 && x1 < WIDTH && y1 > 0 && y1 < HEIGHT && maze[y1][x1] == 0){
            	printMaze();
                maze[y][x] = 1;
                maze[y + dir[d][1]][x + dir[d][0]] = 1;
                maze[y1][x1] = 1;
                carve(x1, y1);
                break;
            }
        }
    }
}

void backtrack(int x, int y){
	for(int i = 0; i < HEIGHT; ++i){
        for(int j = 0; j < WIDTH; ++j){
            maze[i][j] = 0;
        }
    }
    carve(x, y);
    maze[1][1] = 2;
    maze[HEIGHT - 2][WIDTH - 2] = 3;
}


void printBar(int arr[], int n){
	cls();
	hideCursor();
	printf("\n\n\n");
	for(int i = 15; i > 0; i--){
		printf("          ");
		for(int j = 0; j < n; j++){
			if(arr[j] >= i){
				printf("%c", 219);
			}else{
				printf(" ");
			}
		}
		printf("\n");
	}
}

//DFS

char wall = 219;
char mazeDFS[HEIGHT][WIDTH];

void printMazeDFS() {
	cls();
	hideCursor();
	printf("\n\n\n    ");
	for(int i = 0; i < WIDTH+2; i++) printf("%c", wall);
	puts("");
    for(int i = 0; i < HEIGHT; i++){
    	printf("    %c", wall);
        for(int j = 0; j < WIDTH; j++){
            printf("%c", mazeDFS[i][j]);
        }
        printf("%c", wall);
        printf("\n");
    }
    printf("    ");
    for(int i = 0; i < WIDTH+2; i++) printf("%c", wall);
}

//Create Maze Block (Default Map, Full of Walls)
void initializeMaze(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            mazeDFS[i][j] = 219;
        }
    }
//    printMazeDFS();
}

bool isValidCell(int x, int y){
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}


bool isWall(int x, int y){
    return mazeDFS[y][x] == wall;
}

bool hasUnvisitedNeighbor(int x, int y){
    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    for(int i = 0; i < 4; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(isValidCell(nx, ny) && isWall(nx, ny)){
            return true;
        }
    }
    return false;
}

void generateMaze(int x, int y){
    mazeDFS[y][x] = ' ';
    int directions[4] = {0, 1, 2, 3};
    for(int i = 0; i < 4; i++){
//    	printf("1"); getchar();
        int randomIndex = rand() % 4;
        int temp = directions[i];
        directions[i] = directions[randomIndex];
        directions[randomIndex] = temp;
    }

    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    for(int i = 0; i < 4; i++){
//    	printf("2");
        int nx = x + dx[directions[i]];
        int ny = y + dy[directions[i]];

        int nnx = nx + dx[directions[i]];
        int nny = ny + dy[directions[i]];

        if(isValidCell(nnx, nny) && isWall(nnx, nny)){
            mazeDFS[ny][nx] = ' ';
//            printMazeDFS();
            generateMaze(nnx, nny);
        }
    }
}

void DFS(int x, int y){
	initializeMaze();
	generateMaze(x, y);
	printMazeDFS();
}


// Heap Sort
int heap[SIZE + 1];
int totalData = 0;

int getParent(int idx){
	return idx/2;
}

int getLeftChild(int idx){
	return idx*2;
}

int getRightChild(int idx){
	return idx*2+1;
}

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void upHeap(int idx){
	if(idx <= 1){
		return;
	}
	
	int parent = getParent(idx);
	if(heap[idx] < heap[parent]){
		swap(&heap[idx], &heap[parent]);
		upHeap(parent);
	}
} 

void downHeap(int idx){
	int child1 = getLeftChild(idx);
	int child2 = getRightChild(idx);
	
	if(child1 > totalData){
		return;
	}
	
	int smallest = idx;
	
	if(heap[smallest] > heap[child1]){
		smallest = child1;
	}
	
	if(heap[smallest] > heap[child2]){
		smallest = child2;
	}
	
	if(child2 <= totalData){
		if(heap[child2] < heap[smallest]){
			smallest = child2;
		}
	}
	
	if(smallest != idx){
		swap(&heap[idx], &heap[smallest]);
		downHeap(smallest);
	}else{
		return;
	}
}

void insert(int value){
	if(totalData == SIZE){
		return;
	}
	
	totalData++;
	
	heap[totalData] = value;
	upHeap(totalData);
}

int popRoot(){
	if(totalData == 0) return NULL;
	int returnData = heap[1];
	heap[1] = heap[totalData];
	totalData--;
	downHeap(1);
	return returnData;
}

void heapSort(int arr[]){
	for(int i = 0; i < SIZE; i++){
		insert(arr[i]);
	}
	for(int i = 0; i < SIZE; i++){
		arr[i] = popRoot();
		printBar(arr, SIZE);
	}
}


//Quick Sort
int partition(int arr[], int l, int h, int n){
	int pivot = arr[h];

	int i = (l - 1);

	for(int j = l; j < h; j++){
		if(arr[j] <= pivot){
			printBar(arr, n);
			i++;
			swap(&arr[i], &arr[j]);
    	}
	}
	swap(&arr[i + 1], &arr[h]);
	return (i + 1);
}

void quickSort(int arr[], int l, int h, int n){
	if(l < h){
    	int pi = partition(arr, l, h, n);
    	quickSort(arr, l, pi - 1, n);
    	quickSort(arr, pi + 1, h, n);
	}
}


//Merge Sort
void merge(int arr[], int p, int q, int r, int n){
	int n1 = q - p + 1;
	int n2 = r - q;
	int L[n1], M[n2];
	int i, j, k;
	i = 0;
	j = 0;
	k = p;
		
	for(int i = 0; i < n1; i++){
		L[i] = arr[p + i]; 	
	}
	for(int j = 0; j < n2; j++){
		M[j] = arr[q + 1 + j];
	}
	while(i < n1 && j < n2){
	    if(L[i] <= M[j]){
	    	arr[k] = L[i];
	    	i++;
	    }else{
			arr[k] = M[j];
			j++;
    	}
    	k++;
    	printBar(arr, n);
  	}
	while(i < n1){
	    arr[k] = L[i];
	    i++;
	    k++;
	}
	while(j < n2){
	    arr[k] = M[j];
	    j++;
	    k++;
	}
}

void mergeSort(int arr[], int l, int r, int n){
	if(l < r){
	    int m = l + (r - l) / 2;
	
	    mergeSort(arr, l, m, n);
	    mergeSort(arr, m + 1, r, n);
	
	    merge(arr, l, m, r, n);
	}
}


//Testing Bars using Bubble
void bubbleSort(int arr[], int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n-1-i; j++){
			if(arr[j] > arr[j+1]){
				int temp = arr[j+1];
				arr[j+1] = arr[j];
				arr[j] = temp;
				printBar(arr, n);
			}
		}
	}
}


//Solve Maze with DFS
bool isDestination(int x, int y, int destX, int destY){
    return x == destX && y == destY;
}

bool isValidCellSolve(int x, int y){
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT &&  mazeDFS[y][x] != wall;
}

bool solveMazeDFS(int x, int y, int destX, int destY, bool visited[HEIGHT][WIDTH]){
    if(isDestination(x, y, destX, destY)){
        return true;
    }

    visited[y][x] = true;
    mazeDFS[y][x] = '*';
	printMazeDFS();
    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    for(int i = 0; i < 4; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];
//		printf("%d %d %d %d", ny, nx, !visited[ny][nx], isValidCellSolve(nx, ny));getchar();
        if(isValidCellSolve(nx, ny) && !visited[ny][nx]){
//        	printf("1");getchar();
            // Recursively solve the maze from the neighboring cell
            if(solveMazeDFS(nx, ny, destX, destY, visited)){
//            	printf("2");getchar();
                return true;
            }
        }
    }
    return false;
}

//Solve Maze with BFS - Error
bool solveMazeBFS(int startX, int startY, int destX, int destY){
    Cell queue[WIDTH * HEIGHT];
    int front = 0;
    int rear = 0;
    bool visited[HEIGHT][WIDTH];
    memset(visited, false, sizeof(visited));
    queue[rear++] = (Cell){ startX, startY };
    visited[startY][startX] = true;
    mazeDFS[startY][startX] = '*';
    
    while(front != rear){
        Cell current = queue[front++];
        int x = current.x;
        int y = current.y;

        if(isDestination(x, y, destX, destY)){
            return true;
        }
        int dx[4] = {1, 0, -1, 0};
        int dy[4] = {0, 1, 0, -1};

        for(int i = 0; i < 4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (isValidCell(nx, ny) && !visited[ny][nx]) {
                queue[rear++] = (Cell){nx, ny};
                visited[ny][nx] = true;
                mazeDFS[ny][nx] = '*';
                
            }
        }
    }

    return false;
}



void barGeneration(){
	system("cls");
	int sortingMethod;
	srand(time(NULL));
	int arr[SIZE] = {0};
	int n = sizeof(arr)/4;
	
	bool gameRunning = true;
		system("Color 9");
	for(int i = 0; i < n; i++){
		int a = (rand() % 15) + 1;
		arr[i] = a;
		cls();
		hideCursor();
		printf("\n\n");
		printf("Genertaing Random Bars\n");
		for(int k = 15; k > 0; k--){
			printf("          ");
			for(int j = 0; j < i; j++){
				if(arr[j] >= k){
					printf("%c", 219);
				}else{
					printf(" ");
				}
			}
			printf("\n");
		}
	}
			system("Color 7");
	system("cls");
	printBar(arr, n);
	printf("Select Sorting Method\n1. Merge Sort\n2. Quick Sort\n3. Heap Sort\n>> ");
	scanf("%d", &sortingMethod); getchar();
	if(sortingMethod == 1){
		system("cls");
		mergeSort(arr, 0, n-1, n);
		printf("\n\n          Press Enter to Continue...");getchar();
	}else if(sortingMethod == 2){
		system("cls");
		quickSort(arr, 0, n-1, n);
		printf("\n\n          Press Enter to Continue...");getchar();
	}else if(sortingMethod == 3){
		system("cls");
		heapSort(arr);
		printf("\n\n          Press Enter to Continue...");getchar();
	}
}

void mazeSolve(){
	system("cls");
	printMazeDFS();
	bool visited[HEIGHT][WIDTH];
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			visited[i][j] = false;
		}
	}
	int input;
	printf("Select Solving Method\n1. DFS\n2. BFS\n>> ");
	scanf("%d", &input); getchar();
	switch(input){
		case 1:{
			if(solveMazeDFS(0, 0, WIDTH-1, HEIGHT-1, visited)){
				printf("Press Enter to Continue...");getchar();
			}
		}
	}
}

void mazeGeneration(){
	system("cls");
	int input, x, y;
	printf("Pick Method\n1. Backtracking\n2. DFS Maze Generation\n>> ");
	scanf("%d", &input);
	if(input == 1){
		printf("Select (x, y) starting cordinates: ");
		scanf("%d %d", &x, &y); getchar();
		backtrack(x, y); 
		printf("\n   Press Enter to Continue..."); getchar();
		mazeSolveNorm();
	}else if(input == 2){
		printf("Select (x, y) starting cordinates: ");
		scanf("%d %d", &x, &y); getchar();
		DFS(x, y);
		printf("\n   Press Enter to Continue..."); getchar();
		mazeSolve();
	}
}


int main(){
	srand(time(NULL));
	int menuInput;
	
	bool programRunning = true;

	do{
		system("cls");
		printf("KCVisualizer\n");
		printf("Select Activity:\n1. Sorting\n2. Maze Generator\n>> ");
		scanf("%d", &menuInput);
		if(menuInput == 1){
			barGeneration();
		}else{
			mazeGeneration();
		}
	}while(programRunning);
}
