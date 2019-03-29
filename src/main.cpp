#include "scene/scene.h"
#include "mypng/myypng.h"
#include "ray_tracer.h"
int main()
{
	ray_tracer rt;
	rt.init_scene(string("../../scene/Scene01/room1.obj"), string("22"));
	rt.trace_scene();

	myypng png(100, 200);
	Color black(1, 1, 1);
	Color red(1.1, 0, 0);
	Color green(0, 1.2, 0);
	Color blue(0, 0, 3);
	Color colors[] = { black, red, green, blue, Color(2, 2, 2) };
	for (int x = 0; x < 100; x++)
	{
		//Color c = colors[x % 5];
		for (int y = 0; y < 200; y++)
		{
			png.setImagePixel(x, y, y < 100 ? red : green);
		}
	}
	png.writeImage("test.png");
}