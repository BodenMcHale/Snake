# Snake
The player controls a snake, which roams around in the console, picking up food, trying to avoid hitting its own tail or the edges of the playing area. Each time the snake eats a piece of food, its tail grows longer, making the game increasingly difficult. The user controls the direction of the snake's head (up, down, left, or right), and the snake's body follows.

![image](https://user-images.githubusercontent.com/11488952/173201530-a754161c-e1b8-4c92-8cb5-36b24cb22c89.png)
(Example of gameplay)

![image](https://user-images.githubusercontent.com/11488952/173201698-94f2b09a-537e-48f1-97fc-1a6f2493f606.png)
(Example of game over screen)


To remove the www.LostRabbitDigital.com watermark from the game, edit the string on line 133.
```
  Ln 133    wsprintf(&screen[screenWidth + 38], L"www.LostRabbitDigital.com - Snake - Score: %d", score);
```
