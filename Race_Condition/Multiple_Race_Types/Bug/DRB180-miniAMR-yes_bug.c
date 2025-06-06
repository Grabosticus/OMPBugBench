#include <stdio.h>
#include <stdlib.h>

typedef long long size_t_num;

int total_blocks;
int refinement_level;
int total_variables;
int size_x, size_y, size_z;
int error_threshold;

double tolerance;

typedef struct {
   size_t_num id;
   int depth;
   int refinement;
   int assigned_proc;
   size_t_num parent_id;
   double ****values;
} grid_block;

grid_block *grid;

void compute_stencil(int variable_idx, int stencil_size)
{
   int x, y, z, block_idx;
   double before, middle, after, temp[size_x+2][size_y+2][size_z+2];
   grid_block *current_block;

   int thread_id;

#pragma omp parallel default(shared) private(x, y, z, current_block)
  {
      for (block_idx = 0; block_idx < total_blocks; ++block_idx) {
         current_block = &grid[block_idx];
         for (x = 1; x <= size_x; x++)
            for (y = 1; y <= size_y; y++)
               for (z = 1; z <= size_z; z++)
                  temp[x][y][z] = (current_block->values[variable_idx][x-1][y  ][z  ] +
                                   current_block->values[variable_idx][x  ][y-1][z  ] +
                                   current_block->values[variable_idx][x  ][y  ][z-1] +
                                   current_block->values[variable_idx][x  ][y  ][z  ] +
                                   current_block->values[variable_idx][x  ][y  ][z+1] +
                                   current_block->values[variable_idx][x  ][y+1][z  ] +
                                   current_block->values[variable_idx][x+1][y  ][z  ])/7.0;
         for (x = 1; x <= size_x; x++)
            for (y = 1; y <= size_y; y++)
               for (z = 1; z <= size_z; z++)
                  current_block->values[variable_idx][x][y][z] = temp[x][y][z];
      }
  }
}


void allocate_memory(void)
{
   int x, y, z, var_idx, block_idx;

   grid = (grid_block *) malloc(total_blocks * sizeof(grid_block));

   for (block_idx = 0; block_idx < total_blocks; block_idx++) {
      grid[block_idx].id = -1;
      grid[block_idx].values = (double ****) malloc(total_variables * sizeof(double ***));
      for (var_idx = 0; var_idx < total_variables; var_idx++) {
         grid[block_idx].values[var_idx] = (double ***)
                              malloc((size_x + 2) * sizeof(double **));
         for (x = 0; x < size_x + 2; x++) {
            grid[block_idx].values[var_idx][x] = (double **)
                                   malloc((size_y + 2) * sizeof(double *));
            for (y = 0; y < size_y + 2; y++)
               grid[block_idx].values[var_idx][x][y] = (double *)
                                     malloc((size_z + 2) * sizeof(double));
         }
      }
   }
}

void free_memory(void)
{
   int x, y, var_idx, block_idx;

   for (block_idx = 0; block_idx < total_blocks; block_idx++) {
      for (var_idx = 0; var_idx < total_variables; var_idx++) {
         for (x = 0; x < size_x + 2; x++) {
            for (y = 0; y < size_y + 2; y++)
               free(grid[block_idx].values[var_idx][x][y]);
            free(grid[block_idx].values[var_idx][x]);
         }
         free(grid[block_idx].values[var_idx]);
      }
      free(grid[block_idx].values);
   }
   free(grid);
}

void initialize_grid(void)
{
   int block_idx, var_idx, x, y, z, l, m, n, region_size, direction;
   int x_start, x_end, y_start, y_end, z_start, z_end;
   int x_block, y_block, z_block;
   size_t_num block_number;
   grid_block *current_block;

   for (n = block_idx = z_start = z = 0; z < 1; z++)
      for (z_end = 0; z_end < 1; z_start++, z_end++)
         for (y_start = y = 0; y < 1; y++)
            for (y_end = 0; y_end < 1; y_start++, y_end++)
               for (x_start = x = 0; x < 1; x++)
                  for (x_end = 0; x_end < 1; x_start++, x_end++, block_idx++) {
                     current_block = &grid[n];
                     current_block->depth = 0;
                     current_block->id = block_idx;
                     for (var_idx = 0; var_idx < total_variables; var_idx++)
                        for (x_block = 1; x_block <= size_x; x_block++)
                           for (y_block = 1; y_block <= size_y; y_block++)
                              for (z_block = 1; z_block <= size_z; z_block++)
                                 current_block->values[var_idx][x_block][y_block][z_block] =
                                    ((double) rand()) / ((double) RAND_MAX);
                    n++;
                  }
}

void run_simulation(void)
{
  int starting_point, block_number, var_idx;

  initialize_grid();

  for (var_idx = 0; var_idx < total_variables; var_idx++) {
     compute_stencil(var_idx, 7);
  }
}

int main(int argc, char* argv[])
{   
  total_blocks = 500;
  refinement_level = 5;
  total_variables = 40;
  size_x = 10;
  size_y = 10;
  size_z = 10;

  allocate_memory();
 
  run_simulation();

  free_memory();
  return 0;
}
