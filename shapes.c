#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_SHAPE_TYPES 7
#define SHAPE_DIMENSIONS 4

/**
 * @brief Tetris shapes recorded in binary, 
 * using 4x4 grid.
 */
const int shapes[NUM_SHAPE_TYPES] = {
    0b1000100010001000, // I
    0b1100110000000000, // O
    0b1000110001000000, // S
    0b1100011000000000, // Z
    0b1000100011000000, // L
    0b1000111000000000, // J
    0b1110010000000000, // T
};

/**
 * @brief Given a shape's row binary int, get the row string.
 * 
 * @param value 
 * @return char* The row string of the shape's binary int.
 */
char *get_shape_row_str(int value)
{
    char *out_str = malloc(2 * SHAPE_DIMENSIONS + 1);
    for (int i = 0; i < SHAPE_DIMENSIONS; i++)
    {
        if ((value >> (SHAPE_DIMENSIONS - 1 - i)) & 1)
        {
            out_str[2 * i] = '[';
            out_str[2 * i + 1] = ']';
        }
        else
        {
            out_str[2 * i] = '`';
            out_str[2 * i + 1] = ' ';
        }
    }
    out_str[2 * SHAPE_DIMENSIONS] = '\0'; // Null terminate the string
    return out_str;
}

/**
 * @brief Get the shape's binary int, get the shape's string array.
 * 
 * @param shape The binary int of the shape.
 * @return char** The string array of the shape.
 */
char **get_shape_str(int shape)
{
    char **shape_string = malloc(pow(SHAPE_DIMENSIONS + 1, 2));
    for (int i = 0; i < SHAPE_DIMENSIONS; i++)
    {
        int mask = 0b1111000000000000 >> (i * SHAPE_DIMENSIONS);
        int row = (shape & mask) >> ((SHAPE_DIMENSIONS - i - 1) * SHAPE_DIMENSIONS);
        char *shape_binary = get_shape_row_str(row);
        shape_string[i] = shape_binary;
    }
    return shape_string;
}

/**
 * @brief Print a shape's string array in matrix format.
 * 
 * @param shape_matrix String array of the shape.
 */
void print_shape(char **shape_matrix)
{
    for (int i = 0; i < SHAPE_DIMENSIONS; i++)
    {
        for (int j = 0; j < SHAPE_DIMENSIONS * 2; j++)
        {
            printf("%c", shape_matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Convert a coordinate pair to a bit position.
 * 
 * @param y y-value of the coordinate.
 * @param x x-value of the coordinate.
 * @return int The bit position.
 */
int coord_to_bitpos(int y, int x)
{
    return (y * SHAPE_DIMENSIONS) + x;
}

/**
 * @brief Convert a bit position to a coordinate pair.
 * 
 * @param bitpos 
 * @return int* 
 */
int *bitpos_to_coord(int bitpos)
{
    int *y_x = malloc(2 * sizeof(int));
    y_x[0] = bitpos / SHAPE_DIMENSIONS;
    y_x[1] = bitpos % SHAPE_DIMENSIONS;
    return y_x;
}

/**
 * @brief Rotate a shape.
 * 
 * @param shape The binary int of the shape. 
 * @param rot Num. of 90-degree roatations.
 * @return int The binary int of the rotated shape.
 */
int rotate(int shape, int rot)
{
    rot = rot % 4;

    if (rot == 0)
    {
        return shape;
    }

    int rotated_matrix = 0b0000000000000000;

    for (int i = 0; i < SHAPE_DIMENSIONS; i++)
    {
        for (int j = 0; j < SHAPE_DIMENSIONS; j++)
        {
            int new_bitpos = coord_to_bitpos(i, j);

            int old_x, old_y;

            switch (rot)
            {
            case 1: // 90 degrees
                old_x = SHAPE_DIMENSIONS - 1 - i;
                old_y = j;
                break;
            case 2: // 180 degrees
                old_x = SHAPE_DIMENSIONS - 1 - j;
                old_y = SHAPE_DIMENSIONS - 1 - i;
                break;
            case 3: // 270 degrees
                old_x = i;
                old_y = SHAPE_DIMENSIONS - 1 - j;
                break;
            default:
                fprintf(stderr, "Invalid rotation\n");
                return -1;
            }

            int old_bitpos = coord_to_bitpos(old_y, old_x);

            if (shape & (1 << (old_bitpos)))
            {
                rotated_matrix |= (1 << new_bitpos);
            }
        }
    }

    return rotated_matrix;
}

int main()
{

    for (int i = 0; i < NUM_SHAPE_TYPES; i++)
    {
        int S_shape = shapes[i];

        for (int j = 0; j < 4; j++)
        {
            printf("%d\n", j);
            print_shape(get_shape_str(rotate(S_shape, j)));
        }

        printf("\n==============\n");
    }
}