#include "headers/touch.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

TSPoint Touch::read(void)
{
    TSPoint tp = ts.getPoint();

    // restore shared pins
    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);
    // TFT control pins
    digitalWrite(YP, HIGH);
    digitalWrite(XM, HIGH);

    // calibration
    tp.x = map(tp.x, TS_MINX, TS_MAXX, 320, 0);
    tp.y = map(tp.y, TS_MINY, TS_MAXY, 480, 0);

    // swap x and y because of rotation
    int temp = tp.x;
    tp.x = tp.y;
    tp.y = temp;

    return tp;
}

void Touch::run_handles(const Handle handles[], uint8_t count, const Touch::Args& args)
{
    TSPoint tp = Touch::read();

    // if the touch is pressed
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
    {
        // call all handlers
        for (size_t i = 0; i < count; i++)
        {
            Handle h = handles[i];
            if (tp.x > h.x_s &&
                tp.x < h.x_s + h.w &&
                tp.y > h.y_s &&
                tp.y < h.y_s + h.h &&
                h.condition)
            {
                h.callback(args);
            }
        }
    }
}
