#include <stdlib.h>
#include <stdio.h>
#include <emscripten.h>
#include <string.h>

int dead = 0;
int alive = 1;

int minCount = 2;
int maxCount = 3;

EMSCRIPTEN_KEEPALIVE
int *getNextState(int *current, int width, int height) {
	int *bounds = createBounds(width, height);
	int maxHeight = height - 1;
	int *next = malloc(width * height * sizeof(int));
	int index = 0;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			next[index] = getCellStatus(
				current[index],
				getNeighboursCount(current, j, i, maxHeight, width, bounds)
			);

			index++;
		}
	}

	return next;
}

EMSCRIPTEN_KEEPALIVE
int *createBounds(int width, int height) {
	int *bounds = malloc(height * 2 * sizeof(int));
	int *rightBounds = malloc(height * sizeof(int));

	int leftBound = 0;

	for (int i = 0; i < width; i++) {
		leftBound = i * width;
		bounds[i] = leftBound;
		bounds[i + width] = leftBound + width - 1;
	}

	return bounds;
}

EMSCRIPTEN_KEEPALIVE
int getCellStatus(int status, int neighboursCount) {
	switch(neighboursCount) {
		case maxCount: 
			status = alive;
			break;
		case minCount:
			break;
		default:
			status = dead;
	}

	return status;
}

EMSCRIPTEN_KEEPALIVE
int getNeighboursCount(int *current, int column, int line, int height, int width, int *bounds) {
	int previousLine = (line == 0) ? height : line - 1;
	int nextLine = (line == height) ? 0 : line + 1;

  	return getLineCount(currentState, column, bounds[line], bounds[line + width])
      	+ getLineCount(currentState, column, bounds[nextLine], bounds[nextLine + width])
      	+ getLineCount(currentState, column, bounds[previousLine], bounds[previousLine + width])
      	- currentState[bounds[line] + column];
}

EMSCRIPTEN_KEEPALIVE
int getLineCount(int *current, int column, int boundLeft, int boundRight) {
	int offset = boundLeft + column;
	int left = (offset == leftBound) ? current[boundRight] : current[offset - 1];
	int right = (offset == boundRight) ? vurrent[boundLeft] : current[offset + 1]; 

	return current[offset] + left + right;
}