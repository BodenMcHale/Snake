/*
	License
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MIT License

	Copyright (c) 2022, Boden McHale

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.


	Gameplay
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	The player controls a snake, which roams around in the console, picking up food, trying to avoid hitting its own tail or the edges of the playing area.
	Each time the snake eats a piece of food, its tail grows longer, making the game increasingly difficult. 
	The user controls the direction of the snake's head (up, down, left, or right), and the snake's body follows.

	Author
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Twitter: @Boden_McHale https://twitter.com/Boden_McHale
	Blog: https://lostrabbitdigital.com/blog/

	Last Updated: June 13th 2022
*/

#include <iostream>
#include <list>
#include <chrono>
#include <thread>
#include <Windows.h>
using namespace std;

int screenWidth = 120;
int screenHeight = 30;

struct SnakeSegment
{
	int x;
	int y;
};

int main()
{
	// Create screen buffer
	wchar_t *screen = new wchar_t[screenWidth * screenHeight];
	for (int i = 0; i < screenWidth * screenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	while (1)
	{
		// Reset
		list<SnakeSegment> snake = { {60, 15} };
		int foodX = 30;
		int foodY = 15;
		int score = 0;
		int snakeDirection = 3;
		bool dead = false;
		bool keyLeft = false, keyRight = false, keyLeftOld = false, keyRightOld = false;


		while (!dead)
		{
			// Frame Timing
			auto t1 = chrono::system_clock::now();
			while ((chrono::system_clock::now() - t1) < ((snakeDirection % 2 == 1) ? 60ms : 100ms)) // Vertical Speed : Horizontal speed
			{
				// Get Input
				keyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
				keyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

				if (keyRight && !keyRightOld)
				{
					snakeDirection++;
					if (snakeDirection == 4)
						snakeDirection = 0;
				}

				if (keyLeft && !keyLeftOld)
				{
					snakeDirection--;
					if (snakeDirection == -1)
						snakeDirection = 3;
				}

				// Reset keys
				keyLeftOld = keyLeft;
				keyRightOld = keyRight;
			}

			/* GAME LOGIC */

			// Snake controls
			switch (snakeDirection)
			{
			case 0: // Up
				snake.push_front({ snake.front().x, snake.front().y - 1 });
				break;
			case 1: // Right
				snake.push_front({ snake.front().x + 1, snake.front().y });
				break;
			case 2: // Down
				snake.push_front({ snake.front().x, snake.front().y + 1 });
				break;
			case 3: // Left
				snake.push_front({ snake.front().x - 1, snake.front().y });
				break;
			}

			// Collision with food
			if (snake.front().x == foodX && snake.front().y == foodY)
			{
				// Add to the score
				score++;

				// Respawn food
				while (screen[foodY * screenWidth + foodX] != L' ')
				{
					foodX = rand() % screenWidth;
					foodY = (rand() & (screenHeight)-3) + 3;
				}

				// Add another segment to the snake
				for (int i = 0; i < 1; i++)
					snake.push_back({ snake.back().x, snake.back().y });
			}

			// Collision with world
			if (snake.front().x < 0 || snake.front().x >= screenWidth)
				dead = true;
			if (snake.front().y < 3 || snake.front().y >= screenHeight)
				dead = true;


			// Collision with snake
			for (list<SnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
					dead = true;

			// Remove endpoint of snakes tail
			snake.pop_back();

			/* DISPLAY */

			// Clear Screen
			for (int i = 0; i < screenWidth * screenHeight; i++)
				screen[i] = L' ';

			// Draw stats and border
			for (int i = 0; i < screenWidth; i++)
			{
				screen[i] = L'=';
				screen[2 * screenWidth + i] = L'=';
			}
			wsprintf(&screen[screenWidth + 38], L"www.LostRabbitDigital.com - Snake - Score: %d", score);

			// Draw snake body
			for (auto s : snake)
				screen[s.y * screenWidth + s.x] = dead ? L'x' : L'o';

			// Draw snake head 
			screen[snake.front().y * screenWidth + snake.front().x] = dead ? L'X' : L'O';

			// Draw food
			screen[foodY * screenWidth + foodX] = L'*';

			// Dead prompt
			if (dead)
				wsprintf(&screen[15 * screenWidth + 38], L"Press 'SPACE' to play again.", score);

			// Display frame
			WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
		}
		
		// Wait for space
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
	}

	return 0;
}
