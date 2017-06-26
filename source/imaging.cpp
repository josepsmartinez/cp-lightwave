#include "imaging.h"

void stretchContrast(unit **u, int N, int NFRAMES, unit *zero)
{
    int i, frame;
    unit min, max, scale = 2000.0;

    min = 9999;
    max = -9999;
    for (frame = 0; frame < NFRAMES; frame++)
    {
        for (i = 0; i < N; i++)
        {

            if (u[frame][i] < min)
                min = u[frame][i];
            if (u[frame][i] > max)
                max = u[frame][i];

        }
    }
    if (max > min)
        scale = scale / (max - min);
    for (frame = 0; frame < NFRAMES; frame++)
        for (i = 0; i < N; i++)
                u[frame][i] = scale * (u[frame][i] - min);

    *zero = - scale * min;
}

// u : [time, space] -> sequential saves of frame : [magnitude, space] (colors represent data type)
void saveFrames (unit **u, int N, int NFRAMES, int y_points, int zero, int bw)
{
    FILE *ppmfile;
    char filename[32];
    int i, j, k, z, frame, idx;
    int i_data;
    byte lut[256][3]; // Colour lookup table
    byte *rgb; // Framebuffer

    if (bw)
        // Greyscale
        for (i=0; i<256; i++)
            lut[i][0] = lut[i][1] = lut[i][2] = (byte)i;
    else
    {
        // Colour
        for (i = 0; i < 256; i++)
        {
            lut[i][0] = (byte)i;
            lut[i][1] = (byte)(127 + 2 * (i < 64 ? i : (i < 192 ? 128 - i : i - 255)));
            lut[i][2] = (byte)(255 - i);
        }
    }

    /* colors for each data type
     * */

    rgb = (byte*) malloc (3 * N * y_points * sizeof (byte*));
    zero = zero; // finds index 0

    for (frame = 0; frame < NFRAMES; frame++)
    {
        k = 0;

        for (i = 0; i < y_points; i++)
        {

            // SPECIAL CODE HERE

            //if(i_data!=214) printf("Value for row %d:%d on frame %d\n", j, i_data, frame);
            for (j = 0; j < N; j++)
            { // for each pixel on the column, find its appropriate color based on data info
                i_data = u[frame][j];
                if (i == zero) { // colors X-axis
                    idx = 255; // black

                } else if (i >= i_data-1 && i <= i_data+1) { // colors E-field
                    idx = 0; // some other color
                    //printf("FOUND VALUE ROW! %d %d\n", j, i);
                }
                else {
                    idx = 100;
                }

                // need to store, pass and color material/environment fields


                //k = j;
                rgb[k++] = (byte)lut[idx][0];
                rgb[k++] = (byte)lut[idx][1];
                rgb[k++] = (byte)lut[idx][2];
                /*
                idx = (int)u[frame][i][j];
                rgb[k++] = (byte)lut[idx][0];  // Red
                rgb[k++] = (byte)lut[idx][1];  // Green
                rgb[k++] = (byte)lut[idx][2];  // Blue
                */
            }
        }

        // Show colour map (comment this out if you don't like it).
        /*
        if (N >= 255)
        {
            int i0 = N / 2 - 128;
            for (i = 0; i < 256; i++)
            {
                k = 3 * ((i + i0) * N + 10);
                for (j = 0; j < 16; j++)
                {
                    rgb[k++] = lut[i][0];
                    rgb[k++] = lut[i][1];
                    rgb[k++] = lut[i][2];
                }
            }
        }
        */
        sprintf (filename, "../lightwave/frames/%04d.ppm", frame);
        ppmfile = fopen (filename, "wb");
        fprintf (ppmfile, "P6\n");
        fprintf (ppmfile, "%d %d\n", N, y_points);
        fprintf (ppmfile, "255\n");
        fwrite (rgb, sizeof (byte), 3 * N * y_points, ppmfile);
        fclose (ppmfile);
    }
    free (rgb);

}





