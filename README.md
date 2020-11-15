# Asteroids

Asteroids clone using SDL2 with an ECS architecture

## TODO:
* Create enemy ship
	* sprite
	* AI movement
	* AI attack
	* enemy sound effect "danger music"
* Weapon progression system
* Secondary weapon ammo
* Create Heat seaking missile
* Replace all 2d sprites with OpenGL rendering
	* Ship
	* Asteroids
	* Enemy ship
	* Bullet
	* Missile/Mine
	* Particles?
* Menus / Game Over Screen + Highscore
* Improve ECS performance
	* pre-computed entity lists by ComponentType queries
	* components in sequential vector/array instead of shared_ptr's
* Move callbacks to inside systems, to remove logic from entity creation
* Move inputs to it's own component

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
