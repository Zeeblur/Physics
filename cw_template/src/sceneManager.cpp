#define GLM_ENABLE_EXPERIMENTAL
#include "sceneManager.h"

#include <vector>
using namespace glm;
using namespace std;

effect effG;

// storage of indicies data
vector<unsigned int> indices;

void CheckGL() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("An OGL error has occured: %u\n", err);
	}
}

void SceneManager::Init()
{

	// initialise the scene, set up shaders and stuff
	effG = effect();
	effG.add_shader("shaders/phys_grid.vert", GL_VERTEX_SHADER);
	effG.add_shader("shaders/phys_grid.frag", GL_FRAGMENT_SHADER);
	effG.build();
	// create a line of atoms
	//for (unsigned int i = 0; i < 8; ++i)
	//{
	//	atomlist[i].normal = dvec3(0.0, 1.0, 0.0);
	//	atomlist[i].position = dvec3(0.0 + i, 1.0, 0.0);
	//}

	// calculate width and hieght of grid
	width = sizeof(atomlist) / sizeof(atomlist[0]);
	height = sizeof(atomlist[0]) / sizeof(Atom);


	// generate initial positions for grid
	for(int n = 0; n < width; ++n)
	{
		for (int m = 0; m < height; ++m)
		{
			atomlist[n][m].normal = dvec3(0.0, 1.0, 0.0);
			atomlist[n][m].position = dvec3(m, n, 0.0);
		}
	}

	phong = effect();
	phong.add_shader("shaders/phys_basic.vert", GL_VERTEX_SHADER);
	phong.add_shader("shaders/phys_basic.frag", GL_FRAGMENT_SHADER);
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


}


void SceneManager::generate_indices()
{
	// for each row 
	for (int c = 0; c < width; ++c)
	{
		// for each column in the list draw squares
		for (int r = 0; r < height -1; ++r)
		{
			// calculate the indices for a square
			// clockwise order of vertex
			indices.push_back((c * width) + r);
			indices.push_back((c * width) + r + 1.0);
			indices.push_back(((c + 1.0 )* width) + r + 1.0);
			indices.push_back(((c + 1.0 )* width) + r + 1.0);
			indices.push_back(((c + 1.0 )* width) + r);
			indices.push_back((c * width) + r);
		}
	}
}

GLuint elementbuffer;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3)*2, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3)*2, (void*)sizeof(vec3));
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
	// Bind the effect
	glUseProgram(phong.get_program());

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

	// draw
	//glDrawArrays(GL_TRIANGLES, 0, 6);


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

void SceneManager::rendershit()
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
	PV = cam.get_projection() * cam.get_view();
	cam.update(static_cast<float>(delta_time));
}

void SceneManager::SetCameraPos(const glm::vec3 &p0) {
	cam.set_position(p0);
	PV = cam.get_projection() * cam.get_view();
/*
#include <glm/glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace glm;
using namespace graphics_framework;

namespace sm
{
	// store camera and effect instances
	free_camera _cam;
	effect effB;
	effect effG;
	effect effP;
	mat4 PV;
	directional_light light;
	material mat;

	//scenelist
	vector<unique_ptr<Entity>> SceneList;

	// return camera pointer
	camera* GetCam() { return &_cam; }

	// Initialse camera, lighting and effects
	void Init()
	{
		effB = effect();
		effB.add_shader("shaders/phys_basic.vert", GL_VERTEX_SHADER);
		effB.add_shader("shaders/phys_basic.frag", GL_FRAGMENT_SHADER);
		effB.build();
		effP = effect();
		effP.add_shader("shaders/phys_phong.vert", GL_VERTEX_SHADER);
		effP.add_shader("shaders/phys_phong.frag", GL_FRAGMENT_SHADER);
		effP.build();
		effG = effect();
		effG.add_shader("shaders/phys_grid.vert", GL_VERTEX_SHADER);
		effG.add_shader("shaders/phys_grid.frag", GL_FRAGMENT_SHADER);
		effG.build();
		_cam.set_position(vec3(10.0f, 10.0f, 10.0f));
		_cam.set_target(vec3(0.0f, 0.0f, 0.0f));
		auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
		_cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

		light.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
		light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		light.set_direction(vec3(0.0f, 1.0f, 0.0f));
		mat = material(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), 25.0f);

	}
	
	// update camera
	void Update(double delta_time)
	{
		// update proj/view matrix
		PV = _cam.get_projection() * _cam.get_view();
		_cam.update(static_cast<float>(delta_time));
	}

	unique_ptr<Entity> CreateParticle(const vec3 &pos)
	{
		unique_ptr<Entity> ent(new Entity());
		ent->SetPosition(vec3(0, 5.0 + (double)(rand() % 200) / 20.0, 0));
		//unique_ptr<Component> physComponent(new cPhysics());
		unique_ptr<cShapeRenderer> renderComponent(new cShapeRenderer(cShapeRenderer::SPHERE));
		renderComponent->SetColour(phys::RandomColour());
		//ent->AddComponent(physComponent);
		//ent->AddComponent(unique_ptr<Component>(new cSphereCollider()));
		ent->AddComponent(unique_ptr<Component>(move(renderComponent)));
		return ent;
	}

	*/
}