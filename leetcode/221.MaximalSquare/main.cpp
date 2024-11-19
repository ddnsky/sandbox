#include <iostream>
#include <string>
#include <vector>

#include <cassert>

using namespace std;
// https://leetcode.com/problems/maximal-square/description/

    int maximalSquare1(const vector<vector<char>>& matrix) {
        int M[300][300];
        const auto H = matrix.size();
        const auto W = matrix[0].size();

        int Mcount = 0;
        for(int y=0; y<H; ++y) {
            Mcount += (M[y][W-1]=(matrix[y][W-1]=='1'));
        }

        for(int y=0; y<H; ++y) {
            for(int x=W-2; x>=0; --x) {
                auto hasValue = matrix[y][x]=='1';
                M[y][x] = hasValue? M[y][x+1]+1 : 0;
                Mcount += hasValue;
            }
        }
        if (Mcount<=3)
            return Mcount!=0;
        if (Mcount==W*H) 
            return std::min(W,H)*std::min(W,H);

        int Wmax = 1;
        for(int y=0; y<H; ++y) {
            int height = H-y;
            for(int x=0; x<W; ++x) {
                auto width = std::min(M[y][x],height);
                if (width<=Wmax)
                    continue;
                for(int yy = 1; yy<width; ++yy) {
                    auto w = M[y+yy][x];
                    if (w<width) {
                        if (w<=yy) {
                            width = yy;
                            break;
                        }
                        width = w;
                    }
                }
                Wmax = std::max(Wmax, width);
            }
        }
        return Wmax*Wmax;       
    }


    int maximalSquare(const vector<vector<char>>& matrix) {
        int buf[300*2];
        const auto H = matrix.size();
        const auto W = matrix[0].size();
        int* prvRow = buf/*+(sizeof(buf)/sizeof(buf[0])-2*W)*/;
        int* curRow = prvRow+W;

        std::fill(prvRow, prvRow+W, 0);

        int Wmax = 0;
        for(int y=0; y<H; ++y) {
            Wmax = std::max(Wmax, curRow[0] = (matrix[y][0] == '1'));
            for(int x=1; x<W; ++x) {
                curRow[x] = 0;
                if (matrix[y][x]=='1') {
                    Wmax = std::max(Wmax, curRow[x] = 1 + std::min(curRow[x-1], std::min(prvRow[x-1], prvRow[x])));
                }
            }
            std::swap(prvRow, curRow);
        }

        return Wmax*Wmax;       
    }


int main()
{
    assert( 4 == maximalSquare({{'1','0','1','0','0'},{'1','0','1','1','1'},{'1','1','1','1','1'},{'1','0','0','1','0'}}));
    assert( 9 == maximalSquare({{'1','1','1','1'},{'1','1','1','1'},{'1','1','1','1'}}));
    assert( 1 == maximalSquare({{'0','1'},{'1','0'}}));
    assert( 0 == maximalSquare({{'0'}}));
    assert( 1 == maximalSquare({{'1'}}));
    assert( 4 == maximalSquare({{'1','1','1','1','1'},{'1','1','1','1','1'},{'0','0','0','0','0'},{'1','1','1','1','1'},{'1','1','1','1','1'}}));
    
    
	return 0;
}