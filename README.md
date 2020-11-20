# Asteroids

Asteroids clone using SDL2 with an ECS architecture

[Download the game here](https://github.com/andreferreirav2/asteroids-sdl2/raw/master/asteroids_x64_build.zip)

## Play

* `2` to play in 2D
* `3` to play in 3D

* `LEFT` and `RIGHT` to rotate the ship sideways
* `UP` to accelerate
* `SPACE` to fire main weapon (gun)
* `DOWN` to fire secondary weapon (mine)

or using a game controller

* `LEFT-STICK` to move ship
* shooting not implemented yet

## Build

The following steps were heavily inspired on [Lazy Foo's tutorials](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

1. Download the following libs online and place them on the same directory, for example, `<your_path>\SDL\libs`:
	```
	SDL2-2.0.12/
	SDL2_image-2.0.5/
	SDL2_mixer-2.0.4/
	SDL2_ttf-2.0.15/
	glew-2.1.0/
	glfw-3.3.2.bin.WIN64/
	```
2. Add the header files: `Project > VC++ Directories > Include Directories` 
	```
	<your_path>\SDL\libs\SDL2_ttf-2.0.15\include;<your_path>\SDL\libs\SDL2_image-2.0.5\include;<your_path>\SDL\libs\SDL2_mixer-2.0.4\include;<your_path>\SDL\libs\SDL2-2.0.12\include;<your_path>\SDL\libs\glew-2.1.0\include;<your_path>\SDL\libs\glfw-3.3.2.bin.WIN64\include;$(IncludePath)
	```
3. Add the lib files: `Project > VC++ Directories > Library Directories` 
	```
	<your_path>\SDL\libs\SDL2_ttf-2.0.15\include;<your_path>\SDL\libs\SDL2_image-2.0.5\include;<your_path>\SDL\libs\SDL2_mixer-2.0.4\include;<your_path>\SDL\libs\SDL2-2.0.12\include;<your_path>\SDL\libs\glew-2.1.0\include;<your_path>\SDL\libs\glfw-3.3.2.bin.WIN64\include;$(IncludePath)
	```
4. Add the libs to the project's linker dependencies: `Project > Linker > Input > Additional Dependencies` 
	```
	SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_mixer.lib;SDL2_ttf.lib;glew32.lib;opengl32.lib;glu32.lib;%(AdditionalDependencies)
	```
5. Add the path to all the libs to your PATH variable: `Windows Key > Edit the environment variables > Enviroment Variables > Path`  
	```
	<your_path>\SDL\libs\SDL2-2.0.12\lib\x64
	<your_path>\SDL\libs\SDL2_image-2.0.5\lib\x64
	<your_path>\SDL\libs\SDL2_mixer-2.0.4\lib\x64
	<your_path>\SDL\libs\SDL2_ttf-2.0.15\lib\x64
	<your_path>\SDL\libs\glew-2.1.0\lib\Release\x64
	```
6. Restart Visual Studio
7. Build!

## Notes / Considerations

### ECS
* Components are being created on the heap as shared_ptr, this does not create a "packed array of components" that takes advantage of CPU caching
* Systems query combos of ComponentTypes A&B by intersecting all entities that have A with all entities that have B (set intersection is taking almost 20% of the CPU time)
	* ? Create vectors of entities of recurring queries (transform+rigidbody). Update queries on add/remove entity
* In pure ECS all data lives in Components, so events should be stored in Components as well. This leads to lots of pooling instead of a simple callback from an Observer pattern, for example
	* Maybe create Events as "Components" and append them to an EventQueue entity (how can multiple events of the same time be triggered in a single frame?)
	* Alternatively append lambda/callbacks to the components, so the component will have both data + callbacks

### Physics
* Collision checking is very naive, checking all colliders against all other unchecked colliders -> O(n2)
	* Implementing quadtrees might help, although all objects are dynamic so objects will jump from one sector to another A LOT!


## TODO:
* enemy sound effect "danger music"
* Create onCollide system + onDestroy system
	* Don't call manager.deleteEntity directly, instead put it on a queue and delete them all at the end of the frame
	* Will allow all collision callbacks to run before anything is deleted
* Weapon progression system
	* Level up?
* Replace all 2d sprites with OpenGL rendering
	* Particles?
* Improve ECS performance
	* pre-computed entity lists by ComponentType queries
	* components in sequential vector/array instead of shared_ptr's
* Move callbacks to inside systems, to remove logic from entity creation
* Move inputs to it's own component
* Make world dimensions not tied to screen 
* Refactor main menu code

## Resources

* https://lazyfoo.net/tutorials/SDL/index.php
* https://lazyfoo.net/tutorials/OpenGL/index.php
* https://en.wikibooks.org/wiki/OpenGL_Programming
* http://www.opengl-tutorial.org/
* https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
* https://blog.winter.dev/2020/designing-a-physics-engine/
* https://www.youtube.com/c/TheChernoProject/videos
* https://github.com/JoeyDeVries/LearnOpenGL
* https://www.freetype.org/
* http://ogldev.atspace.co.uk/index.html
* ECS Architecture
	* http://paulgestwicki.blogspot.com/2012/03/components-and-systems-of-morgans-raid.html?m=1
	* http://entity-systems.wikidot.com/
	* https://cbpowell.wordpress.com/2012/10/30/entity-component-game-programming-using-jruby-and-libgdx-part-1/
	* https://austinmorlan.com/posts/entity_component_system/
	* http://t-machine.org/index.php/2007/09/03/entity-systems-are-the-future-of-mmog-development-part-1/
	* https://github.com/SanderMertens/flecs
	* https://github.com/IainWinter/IwEngine/tree/master/IwEngine/include/iw/entity
