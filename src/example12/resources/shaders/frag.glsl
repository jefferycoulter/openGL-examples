#version 330 core

out vec4 FragColor; // output the colors

in vec3 current_position;
in vec3 Normal;
in vec3 color; // input from vertex shader
in vec2 coords;

// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;

// Gets the color of the light from the main function
uniform vec4 light_color;
// Gets the position of the light from the main function
uniform vec3 light_pos;
// Gets the position of the camera from the main function
uniform vec3 cam_pos;

vec4 point_light()
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 light_vec = light_pos - current_position;

	// intensity of light with respect to distance
	float dist = length(light_vec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 light_direction = normalize(light_vec);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - current_position);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = spec_amount * specular_light;

	return (texture(diffuse0, coords) * (diffuse * inten + ambient) + texture(specular0, coords).r * specular * inten) * light_color;
}

vec4 direct_light()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - current_position);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = spec_amount * specular_light;

	return (texture(diffuse0, coords) * (diffuse + ambient) + texture(specular0, coords).r * specular) * light_color;
}

vec4 spot_light()
{
	// controls how big the area that is lit up is
	float outer_cone = 0.90f;
	float inner_cone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 light_direction = normalize(light_pos - current_position);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - current_position);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = spec_amount * specular_light;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
	float inten = clamp((angle - outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f);

	return (texture(diffuse0, coords) * (diffuse * inten + ambient) + texture(specular0, coords).r * specular * inten) * light_color;
}

void main()
{
	// outputs final color
	FragColor = spot_light();
}