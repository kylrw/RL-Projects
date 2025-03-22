# There are 20 variants of the `count_islands` function,
# and each of the variants has a bug.
# Can you find all bugs by writing testcases?

# Edit the main function to run your own testcases.
# Two examples are provided.
def main():
  grid = [
  ' ##   ',
  '  ##  ',
  '      ',
  ' # ## ',
  ' # ###',
  '     #']
  run_test(grid)

  grid = [
    '   ',
    ' # ',
    '   '
  ]
  run_test(grid)

  # Type error
  grid = (
    '   ',
    ' # ',
    '   '
    )
  run_test(grid)

  # lower bound error
  grid = [
    ''
    ]
  run_test(grid)

  grid = [
  '######',
  '######',
  '######',
  '######',
  '######',
  '######',
  '######',
  '######',
  '######',
  '######']
  run_test(grid)

  grid = [
  ' ##   ',
  '   #  ',
  '      ',
  ' # ## ',
  ' # ###',
  '     #']
  run_test(grid)

  grid = [
  ' ##   ',
  '   #  ',
  '      ',
  '## ###',
  '## ###',
  '      ']
  run_test(grid)

  grid = [
  ' ##   ',
  '   #  ',
  '      ',
  '## ###',
  '## ###',
  ' #    ']
  run_test(grid)

  grid = [
  '',
  ' #    ',
  '   #  ',
  '    # ',
  '     #']
  run_test(grid)

  grid = [
  '#     ',
  '      ',
  '      ',
  '      ',
  '      ',
  '     #']
  run_test(grid)

  grid = [
  '#     ',
  '      ',
  '  llll',
  '      ',
  '      ',
  '     #']
  run_test(grid)
 
  grid = [
  '#                                       ']
  run_test(grid)

  grid = ['#']
  run_test(grid)

  grid = ["#" * 100 for _ in range(100)]
  # Expected: 1 island.
  run_test(grid)

# Challenge your own assumptions.
# Maybe this function doesn't do exactly what you think ;)
def count_islands(grid):
  # Verify grid is valid
  if type(grid) != list or len(set(map(len, grid))) != 1:
    return None
  height = len(grid)
  width = len(grid[0])
  visited = set()
  island_count = 0
  for base_row in range(height):
    for base_col in range(width):
      if (grid[base_row][base_col] == '#' and
          (base_row, base_col) not in visited):
        # Found a new island
        island_count += 1
        visited.add((base_row, base_col))
        stack = [(base_row, base_col)]
        while stack:
          # Perform a BFS to find all cells in the island
          row, col = stack.pop()
          # Hint: the next 4 lines are important.
          for diff_row in (-1, 0, 1):
            for diff_col in (-1, 0, 1):
              neighbor_row = (row + diff_row) % height
              neighbor_col = (col + diff_col) % width
              if ((neighbor_row, neighbor_col) not in visited and
                  grid[neighbor_row][neighbor_col] == '#'):
                # Add valid neighbor to the visit stack
                stack.append((neighbor_row, neighbor_col))
                # Mark neighbor as visited
                visited.add((neighbor_row, neighbor_col))
  return island_count

# "encrypted" source code, don't look inside!
import base64, zlib
FUNCTIONS = '''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'''
run_test=lambda x:None # This is just to make error-checking nicer
exec(zlib.decompress(base64.b64decode(FUNCTIONS)))
main()