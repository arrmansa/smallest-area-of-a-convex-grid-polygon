#include <array>
#include <chrono>
#include <iostream>

struct point {
	int x;
	int y;
};

template<std::size_t N>
bool create_polygon_jit(std::array<point, N>& points, int n, int target_n, int gridsize, int curr_area, int& min_area, std::array<point, N>& soln) 
{
	if (n >= 3)
	{
		int x1 = points[n - 3].x - points[n - 2].x, y1 = points[n - 3].y - points[n - 2].y;
		int x2 = points[n - 1].x - points[n - 2].x, y2 = points[n - 1].y - points[n - 2].y;
		if (x1 * y2 - x2 * y1 <= 0) // Angle > 180
			return false;
		if (x1 * x2 + y1 * y2 > 0)  // Angle >= 90
			return false;
		if (points[1].x == 0 and points[n - 1].x == 0)
			return true;

		int add_area = points[n - 1].x * points[n - 2].y - points[n - 1].y * points[n - 2].x;
		if (add_area <= 0)
			return true;

		curr_area += add_area;
		if (curr_area >= min_area)
			return true;

		if (n == target_n)
		{
			min_area = curr_area;
			soln = points;
			return true;
		}

		int min_i = std::max(0, points[n - 1].x - 3);
		int max_i = std::min(gridsize, points[n - 1].x + 4);

		int min_j = std::max(- gridsize / 2, points[n - 1].y - 3);
		int max_j = std::min(gridsize / 2 + 1, points[n - 1].y + 4);
		
		if (x2 > 0)
		{
			if (y2 > 0)
			{
				if (x2 > y2)
				{
					for (int j = min_j; j < max_j; j++){
						for (int i = max_i - 1; i > min_i - 1; i--){
							points[n].x = i;
							points[n].y = j;
							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].x == max_i - 1)
							break;
					}
				}
				else
				{
					for (int j = max_j - 1; j > min_j - 1; j--)
					{
						for (int i = max_i - 1; i > min_i - 1; i--)
						{
							points[n].x = i;
							points[n].y = j;

							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].x == max_i - 1)
							break;
					}
				}
			}
			else
			{
				if (x2 > -y2)
				{
					for (int i = max_i - 1; i > min_i - 1; i--)
					{
						for (int j = min_j; j < max_j; j++)
						{
							points[n].x = i;
							points[n].y = j;

							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].y == min_j)
							break;
					}
				}
				else
				{
					for (int i = min_i; i < max_i; i++)
					{
						for (int j = min_j; j < max_j; j++)
						{
							points[n].x = i;
							points[n].y = j;
							;
							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].y == min_j)
							break;
					}
				}
			}
		}
		else
		{
			if (y2 > 0)
			{
				if (-x2 > y2)
				{
					for (int i = min_i; i < max_i; i++)
					{
						for (int j = max_j - 1; j > min_j - 1; j--)
						{
							points[n].x = i;
							points[n].y = j;

							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].y == max_j - 1)
							break;
					}
				}
				else
				{
					for (int i = max_i - 1; i > min_i - 1; i--)
					{
						for (int j = max_j - 1; j > min_j - 1; j--)
						{
							points[n].x = i;
							points[n].y = j;

							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].y == max_j - 1)
							break;
					}
				}
			}
			else
			{
				if (-x2 > -y2)
				{
					for (int j = max_j - 1; j > min_j - 1; j--)
					{
						for (int i = min_i; i < max_i; i++)
						{
							points[n].x = i;
							points[n].y = j;

							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].x == min_i)
							break;
					}
				}
				else
				{
					for (int j = min_j; j < max_j; j++)
					{
						for (int i = min_i; i < max_i; i++)
						{
							points[n].x = i;
							points[n].y = j;

							if (!create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln))
								break;
						}
						if (points[n].x == min_i)
							break;
					}
				}

			}
		}
	}
	else
	{
		int min_i = std::max(0, points[n - 1].x - 3);
		int max_i = std::min(gridsize, points[n - 1].x + 4);

		int min_j = std::max(-gridsize / 2, points[n - 1].y - 3);
		int max_j = std::min(gridsize / 2, points[n - 1].y + 4);

		for (int i = min_i; i < max_i; i++)
		{
			for (int j = min_j; j < max_j; j++)
			{
				points[n].x = i;
				points[n].y = j;
				create_polygon_jit(points, n + 1, target_n, gridsize, curr_area, min_area, soln);
			}
		}
	}
	points[n].x = 0;
	points[n].y = 0;
	return true;
}

int main() {
	using namespace std::chrono;
	auto t1 = steady_clock::now();

	const int n = 16;
	std::array<point, n> points{};

	int gridSize = 11;
	int minArea = gridSize * gridSize;
	
	std::array<point, n> result;
	
	for (point& p : result){
		p.x = 0;
		p.y = 0;
	}
	
	create_polygon_jit(points, 1, n, gridSize, 0, minArea, result);

	std::cout << "Result: ";
	for (point& p : result)
		std::cout << "("<< p.x << ", " << p.y << ") ";
	std::cout << "\nArea: " << minArea << "\n";

	auto t2 = steady_clock::now();
	auto time_span = duration_cast<duration<double>>(t2 - t1);
	std::cout << "Time:" << time_span.count() << " seconds.\n";
}