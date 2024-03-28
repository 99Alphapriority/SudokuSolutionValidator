#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/*structure for passing data to threads*/
typedef struct
{
	int tid;
	int row;
	int column;
}parameters;

/*global array to store results of each thread*/
int res[27] = {0};

int board[9][9] = {
	{6,2,4,5,3,9,1,8,7},
	{5,1,9,7,2,8,6,3,4},
	{8,3,7,6,1,4,2,9,5},
	{1,4,3,8,6,5,7,2,9},
	{9,5,8,2,4,7,3,6,1},
	{7,6,2,3,9,1,4,5,8},
	{3,7,1,9,5,6,8,4,2},
	{4,9,6,1,8,2,5,7,3},
	{2,8,5,4,7,3,9,1,6}
};

void *checkRow(void* param)
{
	parameters *data = (parameters*) param;
	int row[10][10] = {0};

	for(int i = 1; i < 10; i++)
	{
		for(int j = 1; j < 10; j++)
		{
			row[i][j]++;
			if(row[i][j] == 2)
			{	
				res[data->tid] = 1;
				pthread_exit(0);
			}
		}
	}	

	res[data->tid] = 0;
	pthread_exit(0);
}

void *checkColumn(void* param)
{
	parameters *data = (parameters*) param;
	int col[10][10] = {0};
	
	for(int i = 1; i < 10; i++)
	{
		for(int j = 1; j < 10; j++)
		{
			col[j][i]++;
			if(col[j][i] == 2)
			{
				res[data->tid] = 1;
				pthread_exit(0);
			}
		}
	}

	res[data->tid] = 0;
	pthread_exit(0);
}

void *checkSubgrid(void* param)
{
	parameters *data = (parameters*) param;
	int row = 0, col = 0;
	int grid[10];

	switch (data->row)
	{
		case 1:
			row = 3;
			break;
	
		case 4:
			row = 6;
			break;
	
		case 7:
			row = 9;
			break;
	}

	switch (data->column)
	{
		case 1:
			col = 3;
			break;
	
		case 4:
			col = 6;
			break;

		case 7:
			col = 9;
			break;
	}

	for(int i = data->row - 1; i < row; i++)
	{
		for(int j = data->column - 1; j < col; j++)
		{
			grid[board[i][j]]++;
			if(grid[board[i][j]] == 2)
			{
				res[data->tid] = 1;
				pthread_exit(0);
			}
		}
	}
	
	res[data->tid] = 0;
	pthread_exit(0);
}

int main()
{
	pthread_t workers[11];
	parameters data[11];
	int r = 1;
	int c = 1;

	for(int i = 0; i < 11; i++)
	{
		data[i].tid = i;
	}

	for(int i = 0; i < 2; i++)
	{
		data[i].row = 0;
		data[i].column = 0;
	}
	
	for(int i = 2; i < 11; i++)
	{
		data[i].row = r;
		data[i].column = c;
		c += 3;
		
		if(i == 4 || i == 6)
		{
			r += 3;
			c = 1;
		}
	}

	/*create threads*/
	pthread_create(&workers[0], NULL, &checkRow, (void*) &data[0]);
	pthread_create(&workers[1], NULL, &checkRow, (void*) &data[1]);
	
	for(int i = 2; i < 11; i++)
		pthread_create(&workers[i], NULL, &checkSubgrid, (void*) &data[i]);

	for(int i = 0; i < 11; i++)
		pthread_join(workers[i], NULL);

	for(int i = 0; i < 11; i++)
	{
		if(res[i] == 1)
		{
			fprintf(stdout,"Soduku solution is invalid\n");
			return 0;
		}
	}
	
	fprintf(stdout,"Soduku solution is valid\n");
	return 0;
}
