#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4

static void	write_num(int n)
{
	char	c;

	c = n + '0';
	write(1, &c, 1);
}

static void	write_space(void)
{
	write(1, " ", 1);
}

static void	write_str(const char *s)
{
	while (*s)
		write(1, s++, 1);
}

/* Count visible from left->right */
static int	visible_count(int *line)
{
	int	i;
	int	max;
	int	count;

	i = 0;
	max = 0;
	count = 0;
	while (i < SIZE)
	{
		if (line[i] > max)
		{
			max = line[i];
			count++;
		}
		i++;
	}
	return (count);
}

/* Right->left */
static int	visible_count_rev(int *line)
{
	int	tmp[SIZE];
	int	i;

	i = 0;
	while (i < SIZE)
	{
		tmp[i] = line[SIZE - 1 - i];
		i++;
	}
	return (visible_count(tmp));
}

/* No dups in row/col if we place val at (row,col) */
static int	is_valid(int grid[SIZE][SIZE], int row, int col, int val)
{
	int	i;

	i = 0;
	while (i < SIZE)
	{
		if (grid[row][i] == val)
			return (0);
		if (grid[i][col] == val)
			return (0);
		i++;
	}
	return (1);
}

/* Backtracking to fill a 4x4 Latin square */
static int	fill_grid(int grid[SIZE][SIZE], int cell)
{
	int	row;
	int	col;
	int	val;
	int	k;

	if (cell == SIZE * SIZE)
		return (1);
	row = cell / SIZE;
	col = cell % SIZE;
	val = 1 + (rand() % SIZE);
	k = 0;
	while (k < SIZE)
	{
		if (is_valid(grid, row, col, val))
		{
			grid[row][col] = val;
			if (fill_grid(grid, cell + 1))
				return (1);
			grid[row][col] = 0;
		}
		val = (val % SIZE) + 1;
		k++;
	}
	return (0);
}

/* Compute the 16 clues in solver order into clues[16] */
static void	build_clues(int grid[SIZE][SIZE], int clues[16])
{
	int	col;
	int	row;
	int	col_vals[SIZE];

	col = 0;
	while (col < SIZE)
	{
		row = 0;
		while (row < SIZE)
		{
			col_vals[row] = grid[row][col];
			row++;
		}
		clues[col] = visible_count(col_vals);
		clues[4 + col] = visible_count_rev(col_vals);
		col++;
	}
	row = 0;
	while (row < SIZE)
	{
		clues[8 + row] = visible_count(grid[row]);
		clues[12 + row] = visible_count_rev(grid[row]);
		row++;
	}
}

/* Print one set: Clues line + Grid block */
static void	print_set(int grid[SIZE][SIZE], int clues[16], int idx)
{
	int	i;
	int	r;
	int	c;

	write_str("Set ");
	write_num(idx);
	write_str(":\nClues: ");
	i = 0;
	while (i < 16)
	{
		write_num(clues[i]);
		if (i != 15)
			write_space();
		i++;
	}
	write(1, "\n", 1);
	write_str("Grid:\n");
	r = 0;
	while (r < SIZE)
	{
		c = 0;
		while (c < SIZE)
		{
			write_num(grid[r][c]);
			if (c < SIZE - 1)
				write_space();
			c++;
		}
		write(1, "\n", 1);
		r++;
	}
	write(1, "\n", 1);
}

int	main(void)
{
	int	set_idx;
	int	grid[SIZE][SIZE];
	int	clues[16];
	int	r;
	int	c;

	srand((unsigned int)time(NULL));
	set_idx = 1;
	while (set_idx <= 5)
	{
		r = 0;
		while (r < SIZE)
		{
			c = 0;
			while (c < SIZE)
			{
				grid[r][c] = 0;
				c++;
			}
			r++;
		}
		/* mix up RNG a bit per set */
		rand();
		fill_grid(grid, 0);
		build_clues(grid, clues);
		print_set(grid, clues, set_idx);
		set_idx++;
	}
	return (0);
}