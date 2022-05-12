#version 330 core

out vec4 FragColor; // output the colors
in vec3 color; // input from vertex shader
in vec3 Normal;
in vec3 current_position;

// Gets the color of the light from the main function
uniform vec4 light_color;
// Gets the position of the light from the main function
uniform vec3 light_pos;
// Gets the position of the camera from the main function
uniform vec3 cam_pos;

void main()
{
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
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 8);
	float specular = spec_amount * specular_light;

   FragColor = light_color * (diffuse + ambient + specular);
}