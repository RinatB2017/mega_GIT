//--------------------------------------------------------------------------------------------------
/**
@file	main.cpp

@author	Randy Gaul
@date	10/10/2014

	Copyright (c) 2014 Randy Gaul http://www.randygaul.net

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
		1. The origin of this software must not be misrepresented; you must not
			 claim that you wrote the original software. If you use this software
			 in a product, an acknowledgment in the product documentation would be
			 appreciated but is not required.
		2. Altered source versions must be plainly marked as such, and must not
			 be misrepresented as being the original software.
		3. This notice may not be removed or altered from any source distribution.
*/
//--------------------------------------------------------------------------------------------------

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include "../freeglut/freeglut.h"
#include "Clock.h"
#include "Demo.h"
#include "../imgui/imgui.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Demo.h"

void UpdateScene( f32 time )
{
	// The time accumulator is used to allow the application to render at
	// a frequency different from the constant frequency the physics sim-
	// ulation is running at (default 60Hz).
	static f32 accumulator = 0;
	accumulator += time;

	accumulator = q3Clamp01( accumulator );
	while ( accumulator >= dt )
	{
		if ( !paused )
		{
			scene.Step( );
			demos[ currentDemo ]->Update( );
		}

		else
		{
			if ( singleStep )
			{
				scene.Step( );
				demos[ currentDemo ]->Update( );
				singleStep = false;
			}
		}

		accumulator -= dt;
	}
}

int main( int argc, char** argv )
{
	// Will create OpenGL context, initialize ImGui, and call UpdateScene with
	// a frequency of dt Hz, and setup the available demos.
	return InitApp( argc, argv );
}
