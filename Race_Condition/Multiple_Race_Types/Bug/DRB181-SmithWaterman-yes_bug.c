#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define PATH -1
#define NONE 0
#define UP 1
#define LEFT 2
#define DIAGONAL 3

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(a,b) ((a) > (b) ? a : b)

long long int rows;
long long int cols;

int scoreMatch = 5;
int scoreMismatch = -3;
int scoreGap = -4;

char *seqA, *seqB;

void generateSequences() {
    srand(time(NULL));

    long long int idx;
    for (idx = 0; idx < rows; idx++) {
        int randomVal = rand() % 4;
        if (randomVal == 0)
            seqA[idx] = 'A';
        else if (randomVal == 2)
            seqA[idx] = 'C';
        else if (randomVal == 3)
            seqA[idx] = 'G';
        else
            seqA[idx] = 'T';
    }

    for (idx = 0; idx < cols; idx++) {
        int randomVal = rand() % 4;
        if (randomVal == 0)
            seqB[idx] = 'A';
        else if (randomVal == 2)
            seqB[idx] = 'C';
        else if (randomVal == 3)
            seqB[idx] = 'G';
        else
            seqB[idx] = 'T';
    }
}

long long int elementsInDiagonal(long long int diagIdx) {
    if (diagIdx < rows && diagIdx < cols) {
        return diagIdx;
    }
    else if (diagIdx < max(rows, cols)) {
        long long int minVal = min(rows, cols);
        return minVal - 1;
    }
    else {
        long long int minVal = min(rows, cols);
        return 2 * minVal - diagIdx + labs(rows - cols) - 2;
    }
}

int calculateScore(long long int rowIdx, long long int colIdx) {
    if (seqA[colIdx - 1] == seqB[rowIdx - 1])
        return scoreMatch;
    else
        return scoreMismatch;
}

void computeSimilarity(long long int rowIdx, long long int colIdx, int* scoreMatrix, int* pathMatrix, long long int* maxPosition) {

    int scoreUp, scoreLeft, scoreDiag;

    long long int linearIdx = rows * rowIdx + colIdx;

    scoreUp = scoreMatrix[linearIdx - rows] + scoreGap;
    scoreLeft = scoreMatrix[linearIdx - 1] + scoreGap;
    scoreDiag = scoreMatrix[linearIdx - rows - 1] + calculateScore(rowIdx, colIdx);

    int bestScore = NONE;
    int bestDirection = NONE;

    if (scoreDiag > bestScore) {
        bestScore = scoreDiag;
        bestDirection = DIAGONAL;
    }

    if (scoreUp > bestScore) {
        bestScore = scoreUp;
        bestDirection = UP;
    }

    if (scoreLeft > bestScore) {
        bestScore = scoreLeft;
        bestDirection = LEFT;
    }

    scoreMatrix[linearIdx] = bestScore;
    pathMatrix[linearIdx] = bestDirection;

    if (bestScore > scoreMatrix[*maxPosition]) {
    #pragma omp critical
        *maxPosition = linearIdx;
    }
}

void calculateFirstDiagonalElement(long long int *diagIdx, long long int *startRow, long long int *startCol) {
    if (*diagIdx < cols) {
        *startRow = *diagIdx;
        *startCol = 1;
    } else {
        *startRow = cols - 1;
        *startCol = *diagIdx - cols + 2;
    }
}

int main(int argc, char* argv[]) {
    rows = 2048;
    cols = 2048;

#ifdef DEBUG
    printf("\nMatrix[%lld][%lld]\n", cols, rows);
#endif

    seqA = malloc(rows * sizeof(char));
    seqB = malloc(cols * sizeof(char));

    rows++;
    cols++;

    int *scoreMatrix;
    scoreMatrix = calloc(rows * cols, sizeof(int));

    int *pathMatrix;
    pathMatrix = calloc(rows * cols, sizeof(int));

    generateSequences();

    long long int maxPosition = 0;
    long long int diagIdx, elemIdx;

    double startTime = omp_get_wtime();

    long long int startRow, startCol, calcRow, calcCol;

    long long int totalDiagonals = rows + cols - 3;
    long long int elementsInDiag;

    #pragma omp parallel default(none) shared(scoreMatrix, pathMatrix, maxPosition, totalDiagonals) private(elementsInDiag, diagIdx, startRow, startCol, calcRow, calcCol, elemIdx)
    {
        for (diagIdx = 1; diagIdx <= totalDiagonals; ++diagIdx)
        {
            elementsInDiag = elementsInDiagonal(diagIdx);
            calculateFirstDiagonalElement(&diagIdx, &startRow, &startCol);
            #pragma omp for
            for (elemIdx = 1; elemIdx <= elementsInDiag; ++elemIdx)
            {
                calcRow = startRow - elemIdx + 1;
                calcCol = startCol + elemIdx - 1;
                computeSimilarity(calcRow, calcCol, scoreMatrix, pathMatrix, &maxPosition);
            }
        }
    }
}
