# Asteroids

Asteroids clone using SDL2 with an ECS architecture

## Notes / Considerations

### ECS
* Components are being created on the heap as shared_ptr, this does not create a "packed array of components" that takes advantage of CPU caching
* Systems query combos of ComponentTypes A&B by intersecting all entities that have A with all entities that have B (set intersection is taking almost 20% of the CPU time)
* In pure ECS all data lives in Components, so events should be stored in Components as well. This leads to lots of pooling instead of a simple callback from an Observer pattern, for example
	* Maybe create Events as "Components" and append them to an EventQueue entity (how can multiple events of the same time be triggered in a single frame?)
	* Alternatively append lambda/callbacks to the components, so the component will have both data + callbacks

### Physics
* Collision checking is very naive, checking all colliders against all other unchecked colliders -> O(n2)
	* Implementing quadtrees might help, although all objects are dynamic so objects will jump from one sector to another A LOT!
