/********************************************************** 
 * f3d_compass_helpers.h
 * 
 * Author: Nick Palumbo, Tim
 *   Date Created: March 4, 2016
 *   Last Modified by: Nick Palumbo
 *   Date Last Modified: March 10, 2016
 *   Assignment: lab8
 *   Part of: lab8
 */

void lcd_drawCircle(void);
int calculateDirection(float heading, int currentDirection);
void lcd_drawMarker(int direction);
void led_drawMarker(int currentDirection);
void lcd_drawMarble(float pitch, float roll);
void lcd_clearDirection(void);
