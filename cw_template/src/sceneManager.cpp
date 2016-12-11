#define GLM_ENABLE_EXPERIMENTAL
#include "sceneManager.h"

#include <vector>
using namespace glm;
using namespace std;

effect effG;
// macro for gl check for error and println
void CheckGL() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("An OGL error has occured: %u\n", err);
	}
}

void SceneManager::Init()
{

	double xpos = 0; // create initial vars for mouse position
	double ypos = 0;


	// Set input mode - hide the cursor
	GLFWwindow* window = renderer::get_window();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Capture initial mouse position	
	glfwGetCursorPos(window, &xpos, &ypos);

	// initialise gui
	initialiseGUI(window);

	// initialise the scene, set up shaders and stuff
	effG = effect();
	effG.add_shader("shaders/phys_grid.vert", GL_VERTEX_SHADER);
	effG.add_shader("shaders/phys_grid.frag", GL_FRAGMENT_SHADER);
	effG.build();


	// create a line of atoms

	// calculate width and hieght of grid
	width = sizeof(atomlist) / sizeof(atomlist[0]);
	height = sizeof(atomlist[0]) / sizeof(Atom);


	// generate initial positions for grid
	for (int n = 0; n < width; ++n)
	{
		for (int m = 0; m < height; ++m)
		{
			atomlist[n][m].normal = dvec3(0.0, 1.0, 0.0);
			atomlist[n][m].position = dvec3(n, 0.0, m);
			atomlist[n][m].prev_pos = atomlist[n][m].position;
			atomlist[n][m].constraint = false;
		}
	}
	
	atomlist[0][0].constraint = true;
	/*atomlist[5][0].constraint = true;
	atomlist[0][5].constraint = true;
	atomlist[5][5].constraint = true;*/


	phong = effect();
	phong.add_shader("shaders/phys_phong_new.vert", GL_VERTEX_SHADER);
	phong.add_shader("shaders/phys_phong_new.frag", GL_FRAGMENT_SHADER);
	phong.build();

	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width() / static_cast<float>(renderer::get_screen_height()));
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

	light.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light.set_direction(vec3(0.0f, 1.0f, 0.0f));
	mat = material(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), 25.0f);


	// create indicies
	generate_indices();

	Init_Mesh();
	//m_vao;

	// set clear as grey
	renderer::setClearColour(0.3, 0.3, 0.3);


	init_springs();

}

void SceneManager::init_springs()
{
	// create a link between all atoms with springs
	for (int n = 0; n < height; ++n)
	{
		for (int m = 0; m < width; ++m)
		{
			if (n+1 < height)
				springs.push_back(SpringPhys(atomlist[n][m], atomlist[n+1][m]));

			if (m+1 < height)
				springs.push_back(SpringPhys(atomlist[n][m], atomlist[n][m+1]));
		}
	}

}

void SceneManager::generate_indices()
{
	// for each row 
	for (int c = 0; c < width -1; ++c)
	{
		// for each column in the list draw squares
		for (int r = 0; r < height - 1; ++r)
		{
			// calculate the indices for a square
			// clockwise order of vertex
			indices.push_back((c * width) + r);
			indices.push_back((c * width) + r + 1.0);
			indices.push_back(((c + 1.0)* width) + r + 1.0);
			indices.push_back(((c + 1.0)* width) + r + 1.0);
			indices.push_back(((c + 1.0)* width) + r);
			indices.push_back((c * width) + r);
		}
	}
}

void SceneManager::Init_Mesh()
{
	// Generate a buffer for the indices
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//allocate buffer

	if (m_vao == 0)
	{
		//generate and check for errors if vao is not gen
		glGenVertexArrays(1, &m_vao);
		assert(!CHECK_GL_ERROR && "Couldn't create VAO.");
	}



	// bind vao
	glBindVertexArray(m_vao);

	// generate buffers on GPU
	glGenBuffers(1, &atom_buffer);

	// bind vbo (this for update)
	glBindBuffer(GL_ARRAY_BUFFER, atom_buffer);

	auto a = sizeof(atomlist);


	// intialise vector buffer 
	std::vector<vec3> vf;
	for (auto &aarray : atomlist)
	{
		for (auto ab : aarray)
		{
			vf.push_back(ab.position);
			vf.push_back(ab.normal);
		}
	}

	// bind data to buffer
	// this for update
	glBufferData(GL_ARRAY_BUFFER, vf.size() * sizeof(vec3), &vf[0], GL_DYNAMIC_DRAW);

	// set incoming value expect.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, (void*)sizeof(vec3));
	glEnableVertexAttribArray(0); // pos location
	glEnableVertexAttribArray(1); // normal loc

	assert(!CHECK_GL_ERROR && "Error creating Buffer with OpenGL");

	glBindVertexArray(0);

	// bind effect
	glUseProgram(phong.get_program());
	assert(!CHECK_GL_ERROR && "ERROR PROGRAM");

	// Tell OpenGL what the output looks like
	const GLchar* attrib_names[2] =
	{
		"vertex_position",
		"transformed_normal"
	};

	// Relink program
	glLinkProgram(phong.get_program());
	// check if buffer was created
	assert(!CHECK_GL_ERROR && "Error creating Buffer with OpenGL");
}

void SceneManager::renderParticles()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	renderGUI();

	// Bind the effect
	glUseProgram(phong.get_program());
	

	// for debug
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Set the MVP matrix
	auto M = mat4(1.0f);
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MV = P* V * M;
	glUniformMatrix4fv(
		phong.get_uniform_location("MVP"),
		1,
		GL_FALSE,
		value_ptr(MV));

	auto ac = phong.get_uniform_location("MVP");
	auto ab = phong.get_uniform_location("VP");
	CheckGL();
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Bind the back particle buffer for rendering
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, atom_buffer);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	// Disable vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SceneManager::render_floor()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	static geometry geom = geometry_builder::create_plane();
	renderer::bind(effG);
	auto M = glm::scale(mat4(1.0f), vec3(10.0f, 1.0, 10.0f));
	mat3 N(1.0f);
	mat.set_diffuse(vec4(0.4, 0.4, 0.4, 1.0));
	renderer::bind(mat, "mat");
	renderer::bind(light, "light");
	glUniformMatrix4fv(effG.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(PV * M));
	glUniformMatrix4fv(effG.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
	glUniformMatrix3fv(effG.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
	renderer::render(geom);
}

void SceneManager::Update(double delta_time)
{
	updateGUI();

	PV = cam.get_projection() * cam.get_view();
	cam.update(static_cast<float>(delta_time));

	// positions of the atoms will have changed
	// room for optimisation here
	// update new position buffer
	glBindBuffer(GL_ARRAY_BUFFER, atom_buffer);

	// intialise vector buffer 
	std::vector<vec3> vf;
	for (auto &aarray : atomlist)
	{
		for (auto ab : aarray)
		{
			vf.push_back(ab.position);
			vf.push_back(ab.normal);
		}
	}

	// bind NEW data to buffer
	glBufferData(GL_ARRAY_BUFFER, vf.size() * sizeof(vec3), &vf[0], GL_DYNAMIC_DRAW);

	update_camera(delta_time);
}

void SceneManager::update_camera(float delta_time)
{
	GLFWwindow* window = renderer::get_window();

	// The ratio of pixels to rotation - remember the fov
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

	double new_x = 0;
	double new_y = 0;

	glfwGetCursorPos(window, &new_x, &new_y);	// Get the current cursor position

	if (firstMouse)							 // if first mouse take cursor positons from initalised vars
	{
		current_x = initialX;
		current_y = initialY;
		firstMouse = false;
	}

	double delta_x = 0;
	double delta_y = 0;

	delta_x = new_x - current_x;		 // Calculate delta of cursor positions from last frame
	delta_y = new_y - current_y;


	delta_x *= ratio_width;								 // Multiply deltas by ratios - gets actual change in orientation
	delta_y *= -ratio_height;

	cam.rotate((float)delta_x, (float)delta_y);     // Rotate cameras by delta :: delta_y - x-axis rotation :: delta_x - y-axis rotation

	float multiplier = 100.0f;

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
		cam.move(vec3(0.0f, 0.0f, 1.0f)*delta_time*multiplier);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
		cam.move(vec3(-1.0f, 0.0f, 0.0f)*delta_time*multiplier);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
		cam.move(vec3(1.0f, 0.0f, 0.0f)*delta_time*multiplier);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
		cam.move(vec3(0.0f, 0.0f, -1.0f)*delta_time*multiplier);


	glfwGetCursorPos(window, &new_x, &new_y);  // update cursor pos
	current_x = new_x;
	current_y = new_y;
}

dvec3 SceneManager::calculate_acceleration(const Atom &a)
{
	// get force from spring.

	// add impulse 

	return a.force;
}

void SceneManager::update_grid(const double time, const double delta_time)
{
	// update physics
	// collision detection
	// collision resolution
	// force/movement calculations

	// add impulse here
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_1))
		atomlist[3][3].force += dvec3(0.0, 50.0, 0.0);

	// update spring
	for (auto &spring : springs)
	{
		spring.CalculateForce();
	}

	// calculate positons for each atom.
	for (auto &atomArray : atomlist)
	{
		for (auto &atom : atomArray)
		{
			// change atom.pos here.

			if (atom.constraint)
				continue;

			// use verlet integration

			// calculate acceleration from forces
			dvec3 acc = calculate_acceleration(atom);

			// calculate vel from current and previous pos
			dvec3 velocity = atom.position - atom.prev_pos;

			// set previous to current pos
			atom.prev_pos = atom.position;

			// get new position
			//atom.position += velocity + (acc * delta_time * delta_time);


			// use simplectic 
			////newPos = oldPos + dt * newVelocity
			////newVelocity = oldVelocity + dt * acc

			//dvec3 newVelocity = velocity + (delta_time * acc);
			//atom.position += delta_time * newVelocity;

			atom.position = (2.0 * atom.position) - atom.prev_pos + (pow(delta_time, 2.0) * acc);

		
			// reset force
			atom.force = dvec3(0);
		}
	}


}

void SceneManager::SetCameraPos(const glm::vec3 &p0) {
	cam.set_position(p0);
	PV = cam.get_projection() * cam.get_view();
}