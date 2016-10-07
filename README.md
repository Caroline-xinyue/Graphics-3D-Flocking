# Graphics-3D-Flocking
Group Partners: Tu Luan, Xinyue Zhang

Please use make to compile the two executables: ./hw3 and ./hw3_debug with debuggin information using stepping debugging.

The program meets all the requirements and is inspired by the implementation suggestions from the assignment, local behavior psuedocode and the OpenGL official manual of the two GL functoins.

This assignment we model the behavior of 3D flock of boids through through the global motion toward the goal and the local behaviors of Alignment, Cohesion an dSeparation based on the rules by Craig Reynolds and algorithm pseudocode found online. We mainly design our algorithm based on this page: www.kfish.org/boids/pseudocode.html. User can use the Plus and Minus key to adjust the number of boids.

The goal is by default moving automatically and leading the general direction of the boids. But user can also use key 'm' or 'M' to switch to the self control mode and control with the arrow keys for x and z movements and 'k'/'K' and 'l'/'L' keys for the up and down movements along our y axes(the y axes in our program is the Z axes in reality). Also user can adjust the speed of the cube through 'b'/'B' and 'c'/'C'.

During the beginning discussion, we think that given the nature of the problem, we can use either the Linked List or a dynamically allocated array. We chose the latter because we think even though we don't know exactly the number of boids, given the size of the canvas, the number of boids generated should be generally constant, which could be achieved through a dynamically allocated array on the heap. Also, instead of loading a vector library, we implement the Vector structs and all useful functions in our ll.c and ll.h files.

We use the default OpenGL coordinates with Z pointing outward to us from the screen, so the sky and ground(modeled as a 50*50 checkerboard pattern suggested in the assignment) in our program actually locates on the Y axes. We draw the boids as two triangles of radius 200(consider modeling the whole boids body using a sphere) in 3D which starts randomly in a roughly 500*500*500 box near the point (2400, 0, 1000). We draw the goal as a colored cube of size 250 locatedinitially at (2000, 1000, 0).

We implement the three views which based on the relative position of the flock and the goal. User can switch between them through keyboard shortcuts.
The default view is taken from an observer located at the fixed position DEFAULT_EYE - FLOOR_HEIGHT 18000 + 5000 = 23000 above the ground, centered over the origin and look toward the centroid of the boids or the cube when there are no boids anymore. Similarly, we implement the trailing view and the side view based on the implementation suggestions provided except that we are not so sure about what does it mean by 'whose y field of view is 30 degrees' and 'whose x field of view is 40 degrees'. We additionally make sure that when there are no more boids, our camera will follow the cube goal where the location/eye of the camera located at the last most recent position(the origin by default at the beginning).

Keyboard Controls:
Up: Move the goal cube down or to the positive z axis
Down: Move the goal cube up or to the negative z axis
Left: Move the goal left or to the negative x axis
Right: Move the goal right or to the positive x axis
'k'/'K': Move the goal to the negative y axis toward the ground
'l'/'L': Move the goal to the positive y axis toward the sky
'b'/'B': Accelerate the cube velocity
'c'/'C': Deccelerate the cube velocity
// All the above keys are only valid when the autoMode is false, which means user are manually controlling the goal.

Plus key: Add a new boid to the flock
Minus key: Delete the last boid in the flock
'v'/'V': Use the DEFAULT view
't'/'T': Use the TRAILING view
's'/'S': Use the SIDE view
'p'/'P': Toggle to pause and resume the animation
'd'/'D': Pause the animation when it's not paused and perform single step debugging through console outputs. Note that I add compilation flags to the printf statements, so I use ./hw3_debug executable to compile for debugging.
'w'/'W': Toggle between the Banking motion and the Flapping motion, since the boids are flying towrad the goal, they are turning constantly but it's not realistic for a bird to flap while bank so we implement this in two modes.
'm'/'M': Toggle to control whether the goal will move automatically or be manually controlled.
'q'/'Q': Close the window and clear all the memory allocated for the program.

Extra Credit Features Implemented:
1. Flapping: our boids all flap at the same time
2. Banking: the angle at which the boid banks doesn't depend on the sharpness of the turn
3. Shadow: a vertical projection of the boid on the ground
