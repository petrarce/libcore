#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <eigen3/Eigen/Core>
#include <cstdint>
namespace solutions
{
class GameOfLife
{

public:
	using GameGrid = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
	static void Iterate(GameGrid& grid);

private:
};
} // namespace solutions

#endif // GAMEOFLIFE_H
